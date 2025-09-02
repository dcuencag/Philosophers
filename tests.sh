#!/bin/bash

# Strict mode
set -euo pipefail
IFS=$'\n\t'

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

# Setup
TMPDIR=$(mktemp -d)
LOGFILE="${TMPDIR}/philo_test.log"
PHILO="./philo"
touch "${LOGFILE}"
trap 'rm -rf "${TMPDIR}"' EXIT

# Compile and check executable
compile_philo() {
    echo -e "${BLUE}Compiling philosophers...${NC}"
    if ! make re > /dev/null; then
        echo -e "${RED}Compilation failed${NC}"
        exit 1
    fi

    if [ ! -f "$PHILO" ]; then
        echo -e "${RED}Executable $PHILO not found after compilation${NC}"
        exit 1
    fi

    if [ ! -x "$PHILO" ]; then
        echo -e "${RED}$PHILO is not executable${NC}"
        chmod +x "$PHILO" || exit 1
    fi
}

run_case() {
    local name=$1
    local cmd=$2
    local expect_die=$3
    local expect_finish=$4
    local timeout=$5

    echo -e "${BLUE}Testing: $name${NC}"
    echo "Command: $cmd"
    
    # Clear log
    : > "${LOGFILE}"

    # Run with timeout
    local exit_code=0
    ($cmd & pid=$!; sleep "$timeout"; kill $pid 2>/dev/null || true) > "${LOGFILE}" 2>&1 || exit_code=$?

    echo "Output:"
    cat "${LOGFILE}"

    # Special case for stability test
    if [ "$expect_finish" = "false" ]; then
        local valid_lines=$(grep -cE '^[0-9]+ [1-9][0-9]* (has taken a fork|is eating|is sleeping|is thinking)$' "${LOGFILE}" || echo "0")
        local eating_lines=$(grep -cE '^[0-9]+ [1-9][0-9]* is eating$' "${LOGFILE}" || echo "0")
        
        if [ "$valid_lines" -ge 10 ] && [ "$eating_lines" -ge 3 ]; then
            if ! grep -q "died" "${LOGFILE}"; then
                echo -e "${GREEN}✅ Process stable: $valid_lines actions, $eating_lines meals${NC}"
                return 0
            fi
        fi
        echo -e "${RED}❌ Process not stable: $valid_lines actions, $eating_lines meals${NC}"
        return 1
    fi

    # Regular validation
    if ! grep -Eq '^[0-9]+ [1-9][0-9]* (has taken a fork|is eating|is sleeping|is thinking|died)$' "${LOGFILE}"; then
        echo -e "${RED}❌ Invalid output format${NC}"
        return 1
    fi

    # Death check
    local death_count=$(grep -c "died" "${LOGFILE}" || echo "0")
    if [ "$expect_die" = "true" ] && [ "$death_count" -ne 1 ]; then
        echo -e "${RED}❌ Expected one death, got $death_count${NC}"
        return 1
    elif [ "$expect_die" = "false" ] && [ "$death_count" -ne 0 ]; then
        echo -e "${RED}❌ Expected no deaths, got $death_count${NC}"
        return 1
    fi

    echo -e "${GREEN}✅ Test passed${NC}"
    return 0
}

# First compile
compile_philo

# Run tests
errors=0

run_case "Single philosopher" \
    "$PHILO 1 800 200 200" \
    "true" "true" "2" || ((errors++))

run_case "Five philosophers stable" \
    "$PHILO 5 800 200 200" \
    "false" "false" "2" || ((errors++))

run_case "Five philosophers, one meal" \
    "$PHILO 5 800 200 200 1" \
    "false" "true" "5" || ((errors++))

run_case "Five philosophers, three meals" \
    "$PHILO 5 800 200 200 3" \
    "false" "true" "5" || ((errors++))

run_case "Four philosophers tight timing" \
    "$PHILO 4 310 200 100" \
    "true" "true" "5" || ((errors++))

if [ $errors -eq 0 ]; then
    echo -e "${GREEN}All tests passed successfully!${NC}"
    exit 0
else
    echo -e "${RED}$errors test(s) failed${NC}"
    exit 1
fi