# Makefile for Minishell project
# Author: your_login

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -ltermcap

# Source files
SRCS = main.c \
       globals.c \
       libft_utils.c \
       libft_memory.c \
       libft_string.c \
       builtins.c \
       env_utils.c \
       parser.c

OBJS = $(SRCS:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re