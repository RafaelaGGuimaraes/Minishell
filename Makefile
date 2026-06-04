# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgomes-g <rgomes-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/25 19:57:54 by rgomes-g          #+#    #+#              #
#    Updated: 2026/06/04 15:37:24 by rgomes-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell

# --------- DIRECTORIES --------- #

LIBFT_DIR   = libft
INCLUDES    = -Iincludes -I$(LIBFT_DIR)

# --------- SOURCE FILES --------- #

SRCS =  main.c \
        srcs/parsing/lexer.c \
		srcs/parsing/lexer_utils.c \
		srcs/parsing/lexer_list.c \
		srcs/parsing/syntax.c \
        srcs/parsing/expand.c \
        srcs/parsing/parser.c \
		srcs/parsing/parser_utils.c \
        srcs/shared/utils.c \
        srcs/shared/env.c \
		srcs/shared/env_utils.c \
        srcs/shared/signals.c \
        srcs/execution/path.c \
        srcs/execution/redirs.c \
		srcs/builtins/builtins.c \
		srcs/builtins/builtins_env.c \
		srcs/builtins/builtins_utils.c \
 		srcs/execution/executor.c \
		srcs/execution/executor_external.c \
		srcs/execution/executor_utils.c \

OBJS    = $(SRCS:.c=.o)
LIBFT   = $(LIBFT_DIR)/libft.a

# --------- COMPILATION --------- #

CC      = cc
CFLAGS  = -Wall -Wextra -Werror $(INCLUDES)
LDFLAGS = -lreadline
RM      = rm -rf

# --------- COLORS --------- #

GREEN   = \033[0;32m
YELLOW  = \033[0;33m
CYAN    = \033[0;36m
RESET   = \033[0m

# --------- RULES --------- #

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled$(RESET)"

$(LIBFT):
	@echo "$(YELLOW)» building libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	@echo "$(GREEN)✓ libft ready$(RESET)"

%.o: %.c
	@echo "$(CYAN)  compiling $<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

test_path: $(LIBFT) $(filter-out main.o, $(OBJS)) test_path.o
	@$(CC) $(CFLAGS) $^ $(LIBFT) $(LDFLAGS) -o test_path
	@echo "$(GREEN)✓ test_path compiled$(RESET)"
	
run_valgrind:
	@valgrind -q --track-origins=yes --show-leak-kinds=all --track-fds=yes --suppressions=minishell.supp --leak-check=full ./minishell

clean:
	@$(MAKE) clean -C $(LIBFT_DIR) --no-print-directory
	@$(RM) $(OBJS)
	@echo "$(YELLOW)» objects removed$(RESET)"

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR) --no-print-directory
	@$(RM) $(NAME)
	@echo "$(YELLOW)» $(NAME) removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re