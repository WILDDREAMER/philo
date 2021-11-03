NAME = philo

SRC = helpers.c main.c

all : $(NAME)

$(NAME):$(SRC)
	gcc $(SRC) -pthread -o philo

clean:
	rm -rf philo.dSYM
	rm -rf *.o

fclean: clean
	rm -f philo
	rm -rf philo.dSYM
re: fclean all