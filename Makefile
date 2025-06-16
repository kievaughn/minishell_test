# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/04 13:42:42 by dimendon          #+#    #+#              #
#    Updated: 2025/06/16 16:11:11 by dimendon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.MAKEFLAGS: --no-print-directory

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I./includes -I./src/libft
LDFLAGS = -lreadline -lncurses

TARGET      = minishell
OBJ_PATH    = obj/
LIBFT_DIR   = src/libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

SRCS =	src/builtins/custom_cd.c \
		src/builtins/custom_echo.c \
		src/builtins/custom_exit.c \
		src/builtins/custom_pwd.c \
		src/builtins/custom_export.c \
		src/cleanup.c \
		src/controller.c \
		src/handlers.c \
		src/helpers.c \
		src/main.c \
		src/utils.c

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

.PHONY: all clean fclean re

