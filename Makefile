# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/04 13:42:42 by dimendon          #+#    #+#              #
#    Updated: 2025/08/25 10:01:35 by dimendon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


.MAKEFLAGS: --no-print-directory

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I./includes -I./src/libft

VALGRIND = valgrind --leak-check=full --track-origins=yes --trace-children-skip='*/bin/*,*/sbin/*,/usr/bin/*' \
--trace-children=yes --track-fds=yes --suppressions=readline.supp

LDFLAGS = -lreadline -lncurses

TARGET      = minishell
OBJ_PATH    = obj/
LIBFT_DIR   = src/libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

SRCS =	        src/builtins/custom_export/export_check.c \
				src/builtins/custom_export/export_env.c \
				src/builtins/custom_export/export_print.c \
				src/builtins/custom_export/export_utils.c \
				src/builtins/custom_cd.c \
		        src/builtins/custom_echo.c \
				src/builtins/custom_env.c \
                src/builtins/custom_exit.c \
                src/builtins/custom_pwd.c \
                src/builtins/custom_unset.c \
				src/env/env_lookup.c \
				src/env/env_utils.c \
				src/misc/utils.c \
				src/parsing/expander_utils.c \
				src/parsing/expander.c \
				src/parsing/redir_split.c \
				src/parsing/split_pipes.c \
				src/parsing/tokenize.c \
				src/parsing/word_split.c \
				src/piping/pipeline.c \
				src/piping/redirections.c \
				src/piping/pipeline_utils.c \
				src/piping/redirections_utils.c \
				src/cleanup.c \
		        src/controller.c \
				src/controller_helper.c \
                src/handlers.c \
                src/main.c

OBJS = $(SRCS:src/%.c=$(OBJ_PATH)%.o)
HEADERS = includes/minishell.h

GREEN  = \033[0;32m
YELLOW = \033[1;33m
CYAN   = \033[0;36m
RESET  = \033[0m

all: $(LIBFT_LIB) $(TARGET)

$(OBJ_PATH)%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "$(CYAN)Compiling minishell source: $<$(RESET)\n"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@printf "$(GREEN)Linking minishell executable...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(LDFLAGS) -o $(TARGET)
	@printf "$(GREEN)minishell built successfully!$(RESET)\n"

$(LIBFT_LIB):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) build

# Run with Valgrind
run: $(TARGET)
	@echo "$(YELLOW)Running with Valgrind...$(RESET)"
	$(VALGRIND) ./$(TARGET)

clean:
	@printf "$(YELLOW)Cleaning object files (minishell)...$(RESET)\n"
	@rm -f $(OBJS)
	@rm -rf $(OBJ_PATH)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@printf "$(YELLOW)Removing executable (minishell)...$(RESET)\n"
	@rm -f $(TARGET)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re run

