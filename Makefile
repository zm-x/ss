NAME = minishell

FLAGS = -Wall -Wextra -Werror

SRC = get_next_line.c get_next_line_utils.c minishell.c split.c

OBJ = ${SRC:.c=.o}

all: $(NAME)

bonus: all

$(NAME): $(OBJ)
	@$(CC) $(OBJ) $(FLAGS) -lreadline -o $(NAME)

%.o: %.c
	@$(CC) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

ra: all clean

.PHONY: all bonus clean fclean