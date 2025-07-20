NAME        = philo

SRC_DIR     = src
OBJ_DIR     = obj

SRCS        = main.c

SRC_PATHS   = $(addprefix $(SRC_DIR)/, $(SRCS))
OBJS        = $(SRCS:%.c=$(OBJ_DIR)/%.o)

CC          = cc
CFLAGS      = -Wall -Werror -Wextra
INCLUDES    = -I.
RM          = rm -f

all:
	@echo "\033[1;35m==============================\033[0m"
	@echo "\033[1;31m→ Checking build for philo...\033[0m"
	@echo "\033[1;35m==============================\033[0m"
	@if [ ! -f $(NAME) ]; then \
		echo "\033[1;35m====================\033[0m"; \
		echo "\033[1;34m→ Building philo...\033[0m"; \
		echo "\033[1;35m====================\033[0m"; \
	fi
	@$(MAKE) $(NAME)
	@echo "\033[1;35m=====================================\033[0m"
	@echo "\033[1;32m✓ philo build completed successfully\033[0m"
	@echo "\033[1;35m=====================================\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "\033[1;36m→ Compiling $<\033[0m"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "\033[1;35m===========================\033[0m"
	@echo "\033[1;33m→ Cleaning object files...\033[0m"
	@echo "\033[1;35m===========================\033[0m"
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@echo "\033[1;35m==========================\033[0m"
	@echo "\033[1;33m→ Cleaning executables...\033[0m"
	@echo "\033[1;35m==========================\033[0m"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re