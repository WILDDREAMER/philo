NAME = philo

SRC = helpers.c main.c base.c actions.c

all : $(NAME)
$(NAME):$(SRC)
	gcc $(SRC) -Wall -Wextra -Werror -pthread -o $(NAME)

clean:
	rm -rf philo.dSYM
	rm -rf *.o

fclean: clean
	rm -f philo
	rm -rf philo.dSYM
re: fclean all