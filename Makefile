# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: andyandr <andyandr@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/26 01:21:10 by riramana          #+#    #+#              #
#    Updated: 2026/01/10 15:20:17 by riramana         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC		= cc
CFLAGS		= -Wall -Wextra -Werror
LDFLAGS		= -lreadline

SRC_DIR		= srcs
INC_DIR		= includes
LIBFT_DIR	= libft

SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/utils1.c \
			  $(SRC_DIR)/utils2.c \
			  $(SRC_DIR)/utils3.c \
			  $(SRC_DIR)/utils4.c \
			  $(SRC_DIR)/utils5.c \
			  $(SRC_DIR)/utils6.c \
			  $(SRC_DIR)/utils7.c \
			  $(SRC_DIR)/path_utils.c \
			  $(SRC_DIR)/executor.c \
			  $(SRC_DIR)/executor_utils.c \
			  $(SRC_DIR)/chdir_utils.c \
			  $(SRC_DIR)/file_utils.c \
			  $(SRC_DIR)/echo.c \
			  $(SRC_DIR)/echo_utils.c \
			  $(SRC_DIR)/echo_split.c \
			  $(SRC_DIR)/echo_count.c \
			  $(SRC_DIR)/pipeline.c \
			  $(SRC_DIR)/pipeline_utils.c \
			  $(SRC_DIR)/pipeline_cleanup.c \
			  $(SRC_DIR)/pipeline_exec.c \
			  $(SRC_DIR)/pipeline_init.c \
			  $(SRC_DIR)/pipeline_prep.c \
			  $(SRC_DIR)/pipeline_setup.c \
			  $(SRC_DIR)/env_utils.c \
			  $(SRC_DIR)/env_utils2.c \
			  $(SRC_DIR)/export.c \
			  $(SRC_DIR)/unset.c \
			  $(SRC_DIR)/builtins.c \
			  $(SRC_DIR)/builtins_utils.c \
			  $(SRC_DIR)/expand.c \
			  $(SRC_DIR)/expand_utils.c \
			  $(SRC_DIR)/export_utils.c \
			  $(SRC_DIR)/redirection.c \
			  $(SRC_DIR)/redirection_utils.c \
			  $(SRC_DIR)/redirection_extras.c \
			  $(SRC_DIR)/executor_process.c \
			  $(SRC_DIR)/builtin_redirect.c \
			  $(SRC_DIR)/echo_redirect.c \
			  $(SRC_DIR)/heredoc.c \
			  $(SRC_DIR)/input_redirection_utils.c \
			  $(SRC_DIR)/input_redirection.c \
			  $(SRC_DIR)/utils8.c \
			  $(SRC_DIR)/utils9.c \
			  $(SRC_DIR)/utils10.c \
			  $(SRC_DIR)/utils11.c \
			  $(SRC_DIR)/neofetch.c

OBJS		= $(SRCS:.c=.o)

INCLUDES	= -I$(INC_DIR) -I$(LIBFT_DIR)
LIBFT		= $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
