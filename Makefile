NAME = philo

SRC = helpers.c main.c

all : $(NAME)

$(NAME):$(SRC)
	gcc $(SRC) -pthread -fsanitize=address -o philo

clean:
	rm -rf philo.dSYM
	rm -rf *.o

fclean: clean
	rm -f philo
	rm -rf philo.dSYM
re: fclean all