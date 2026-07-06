NAME        = pipex
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -Iincludes

SRCS        = Pipex/main.c Pipex/pipex.c Pipex/process.c Pipex/utils.c Pipex/files.c
OBJS        = $(SRCS:.c=.o)

SRCS_BONUS  = pipex_bonus/main_bonus.c pipex_bonus/pipex_bonus.c \
              Pipex/process.c Pipex/utils.c Pipex/files.c
OBJS_BONUS  = $(SRCS_BONUS:.c=.o)

LIBFT_DIR   = ./Libft
LIBFT       = $(LIBFT_DIR)/libft.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(LIBFT) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
