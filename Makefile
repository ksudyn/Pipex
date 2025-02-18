NAME		= pipex

CC			= gcc
CFLAGS		= -Wall -Werror -Wextra -g3 -Iincludes

SRCS		:=	$(addprefix src/, \
				files.c main.c pipex.c process.c utils.c)

OBJS		=	$(SRCS:.c=.o)

LIBFT_DIR	= Libft
LIBFT		= $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all
