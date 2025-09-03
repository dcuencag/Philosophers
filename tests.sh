#!/usr/bin/env bash
# tests.sh — batería visual de tests para philo (parte obligatoria), con spinner y cabeceras
set -euo pipefail
IFS=$'\n\t'

# ==== Configuración general ====
BIN=./philo
MAKE=make
FMT_REGEX='^[0-9]+ [1-9][0-9]* (has taken a fork|is eating|is sleeping|is thinking|died)$'
GREEN="\033[1;32m"; RED="\033[1;31m"; YELLOW="\033[1;33m"; BLUE="\033[1;34m"; CYAN="\033[1;36m"; DIM="\033[2m"; RESET="\033[0m"
CHECK="${GREEN}✅${RESET}"; CROSS="${RED}❌${RESET}"

# ==== UI helpers ====
hr() { printf "${DIM}%s${RESET}\n" "────────────────────────────────────────────────────────"; }
title() {
  hr
  printf "${CYAN}▶ %s${RESET}\n" "$1"
  hr
}
sub() { printf "${BLUE}==>${RESET} %s\n" "$1"; }
note() { printf "${DIM}%s${RESET}\n" "$1"; }
pause_short() { sleep 0.15; }  # micro-pausas estéticas

# Spinner (mientras se ejecuta un comando en segundo plano o una espera)
SPINNER_PID=""
spinner_start() {
  # imprime en la línea actual un spinner discreto
  local frames='|/-\'
  (
    i=0
    trap 'exit 0' TERM INT
    while :; do
      i=$(( (i+1) % 4 ))
      printf "\r${DIM}… trabajando ${frames:i:1}${RESET} "
      sleep 0.08
    done
  ) &
  SPINNER_PID=$!
}
spinner_stop() {
  if [ -n "${SPINNER_PID:-}" ] && kill -0 "$SPINNER_PID" 2>/dev/null; then
    kill "$SPINNER_PID" 2>/dev/null || true
    wait "$SPINNER_PID" 2>/dev/null || true
  fi
  SPINNER_PID=""
  printf "\r\033[K"  # limpia la línea del spinner
}

# ==== Utilidades ====

# timeout portable: usa gtimeout si existe; si no, un wrapper Python
timeout_run() {
  local secs="$1"; shift
  if command -v gtimeout >/dev/null 2>&1; then
    gtimeout "${secs}" "$@"
  else
    python - "$secs" "$@" <<'PY'
import os, sys, time, subprocess, signal
secs = float(sys.argv[1])
cmd = sys.argv[2:]
p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
start = time.time()
out, err = "", ""
try:
    while True:
        ret = p.poll()
        if ret is not None:
            o, e = p.communicate()
            out += o; err += e
            sys.stdout.write(out); sys.stderr.write(err)
            sys.exit(ret)
        if time.time() - start >= secs:
            try: p.terminate()
            except: pass
            try:
                time.sleep(0.1)
                p.kill()
            except: pass
            sys.exit(124)  # emula timeout(1)
        time.sleep(0.01)
except KeyboardInterrupt:
    try: p.kill()
    except: pass
    sys.exit(130)
PY
  fi
}

# Builder
build() {
  title "Build"
  sub "Compilando (make re)…"
  pause_short
  spinner_start
  "$MAKE" -s re
  spinner_stop
  if [[ ! -x "$BIN" ]]; then
    echo -e "${CROSS} No existe binario $BIN"
    exit 1
  fi
  echo -e "${CHECK} ${GREEN}philo compilado correctamente${RESET}"
  pause_short
}

# Validadores
check_format() {
  local file="$1"
  [[ ! -s "$file" ]] && return 0
  if grep -vE "$FMT_REGEX" "$file" >/dev/null 2>&1; then
    echo "Formato inválido en líneas:"
    grep -n -vE "$FMT_REGEX" "$file" || true
    return 1
  fi
  return 0
}

count_died() {
  local file="$1"
  grep -c ' died$' "$file" 2>/dev/null || true
}

# ===== Infra de casos =====
TMPDIR="$(mktemp -d)"
trap 'spinner_stop; rm -rf "$TMPDIR"' EXIT

declare -a SUMMARY_NAME=()
declare -a SUMMARY_PASS=()
declare -a SUMMARY_NOTE=()
record() {
  SUMMARY_NAME+=("$1"); SUMMARY_PASS+=("$2"); SUMMARY_NOTE+=("$3")
}

run_case_logs() {
  # args: name, cmd, expect_die(true|false|optional), expect_finish(true|false), timeout_secs
  local name="$1"; local cmd="$2"; local expect_die="$3"; local expect_finish="$4"; local to="$5"
  local out="$TMPDIR/${name// /_}.out"
  local rc start end dur note pass
  echo ""
  sub "Caso: ${YELLOW}${name}${RESET}"
  note "CMD: $cmd"
  pause_short
  start="$(date +%s)"
  set +e
  spinner_start
  timeout_run "$to" bash -lc "$cmd" >"$out" 2>&1
  rc=$?
  spinner_stop
  set -e
  end="$(date +%s)"; dur=$(( end - start ))

  pass=true; note=""
  if [[ "$expect_finish" == "true" ]]; then
    [[ $rc -eq 0 ]] || { pass=false; note+="rc=$rc (esperado 0). "; }
  else
    [[ $rc -eq 124 ]] || { pass=false; note+="no timeout (rc=$rc). "; }
  fi

  if ! check_format "$out"; then pass=false; note+="formato inválido. "; fi

  local died; died="$(count_died "$out")"
  if [[ "$expect_die" == "true" ]]; then
    [[ "$died" -eq 1 ]] || { pass=false; note+="se esperaba 1 died, hay $died. "; }
  elif [[ "$expect_die" == "false" ]]; then
    [[ "$died" -eq 0 ]] || { pass=false; note+="no se esperaba died, hay $died. "; }
  fi

  echo -e "Duración: ${dur}s | rc=$rc | died=$died"
  if $pass; then
    echo -e "${CHECK} ${GREEN}OK${RESET}"
  else
    echo -e "${CROSS} ${RED}FALLO${RESET}  ${DIM}$note${RESET}"
    echo -e "${DIM}Primeras líneas de salida:${RESET}"
    sed -n '1,30p' "$out" | sed 's/^/| /'
  fi
  record "$name" "$pass" "rc=$rc, died=$died, ${note}dur=${dur}s"
  pause_short
}

run_case_parse_error() {
  # args: name, cmd
  local name="$1"; local cmd="$2"
  local out="$TMPDIR/${name// /_}.out"
  local rc start end dur pass=true note=""
  echo ""
  sub "Caso: ${YELLOW}${name}${RESET}"
  note "CMD: $cmd"
  pause_short
  start="$(date +%s)"
  set +e
  spinner_start
  bash -lc "$cmd" >"$out" 2>&1
  rc=$?
  spinner_stop
  set -e
  end="$(date +%s)"; dur=$(( end - start ))

  if [[ $rc -eq 0 && -z "$(grep -i 'error' "$out" || true)" ]]; then
    pass=false; note="se esperaba error (rc!=0 o 'Error' en salida). "
  fi
  if grep -E "$FMT_REGEX" "$out" >/dev/null 2>&1; then
    pass=false; note+="aparecen logs de simulación. "
  fi

  echo -e "Duración: ${dur}s | rc=$rc"
  if $pass; then
    echo -e "${CHECK} ${GREEN}OK${RESET}"
  else
    echo -e "${CROSS} ${RED}FALLO${RESET}  ${DIM}$note${RESET}"
    echo -e "${DIM}Salida:${RESET}"
    sed -n '1,30p' "$out" | sed 's/^/| /'
  fi
  record "$name" "$pass" "rc=$rc, ${note}dur=${dur}s"
  pause_short
}

# ===== Suite =====
main() {
  title "Philosophers — Test Suite"
  pause_short
  build

  title "Parsing"
  run_case_parse_error "no_args"                "$BIN"
  run_case_parse_error "few_args"               "$BIN 1 800 200"
  run_case_parse_error "too_many_args"          "$BIN 1 800 200 200 7 9"
  run_case_parse_error "nonnumeric"             "$BIN a 800 200 200"
  run_case_parse_error "zero_philos"            "$BIN 0 800 200 200"
  run_case_parse_error "zero_die"               "$BIN 5 0 200 200"

  title "Comportamiento"
  run_case_logs "one_philo_dies"       "$BIN 1 800 200 200"          true  true   2
  run_case_logs "stable_no_goal"       "$BIN 5 800 200 200"          false false  2
  run_case_logs "goal_1_finishes"      "$BIN 5 800 200 200 1"        false true   5
  run_case_logs "goal_3_finishes"      "$BIN 5 800 200 200 3"        false true   12
  run_case_logs "tight_timings_die"    "$BIN 4 310 200 100"          true  true   2
  run_case_logs "edge_sum_equals_die"  "$BIN 2 600 200 400"          true  true   3
  run_case_logs "pair_stable"          "$BIN 2 600 200 200"          false false  2

  title "Resumen"
  printf "%-28s | %-6s | %s\n" "Caso" "Estado" "Notas"
  printf -- "----------------------------------------------\n"
  local i=0
  while [[ $i -lt ${#SUMMARY_NAME[@]} ]]; do
    local status="${SUMMARY_PASS[$i]}"
    local icon="$CHECK"; [[ "$status" == "true" ]] || icon="$CROSS"
    printf "%-28s | %-6b | %s\n" "${SUMMARY_NAME[$i]}" "$icon" "${SUMMARY_NOTE[$i]}"
    i=$(( i + 1 ))
  done

  local any_fail=0
  i=0
  while [[ $i -lt ${#SUMMARY_PASS[@]} ]]; do
    [[ "${SUMMARY_PASS[$i]}" == "true" ]] || any_fail=1
    i=$(( i + 1 ))
  done

  if [[ $any_fail -eq 0 ]]; then
    echo -e "\n${CHECK} ${GREEN}TODOS LOS TESTS PASARON${RESET}"
    exit 0
  else
    echo -e "\n${CROSS} ${RED}ALGUNOS TESTS FALLARON${RESET}"
    exit 1
  fi
}

main "$@"
