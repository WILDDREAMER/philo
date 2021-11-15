#include "./philosophers.h"
pthread_mutex_t mutex;

void get_data(t_data *data, int argc, char **argv)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals = -1;
	if (argc == 6)
		data->meals = ft_atoi(argv[5]);
}

void add_node(t_philosopher **curr, t_philosopher **new, int i)
{
	if (!i)
	{
		(*curr) = (*new);
		(*curr)->next = (*new);
		(*curr)->prev = (*new);
	}
	else
	{
		(*new)->next = (*curr)->next;
		(*curr)->next = (*new);
		(*new)->prev = (*curr);
		(*curr) = (*new);
	}
}

unsigned long get_time_mls(void)
{
	struct timeval time;
	unsigned long i;

	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (i);
}

void ft_usleep(unsigned long time)
{
	unsigned long current_time = get_time_mls();
	usleep((time * 1e3) - 10000);
	while (get_time_mls() - current_time < time)
		continue;
}

void print(int status, t_philosopher *philo)
{
	unsigned long time;

	time = get_time_mls() - philo->entry_time;
	pthread_mutex_lock(&mutex);
	if (status == 1)
	{
		printf("%lu: %d has taken a fork\n", time, philo->index);
		printf("%lu: %d has taken a fork\n", time, philo->index);
		printf("%lu: %d is eating\n", time, philo->index);
	}
	else if (status == 2)
		printf("%lu: %d is sleeping\n", time, philo->index);
	else if (status == 3)
		printf("%lu: %d is thinking\n", time, philo->index);
	pthread_mutex_unlock(&mutex);
}

// void handle_forks(int operation, t_philosopher *philo)
// {
// 	if (operation == 1)
// 	{
// 		pthread_mutex_lock(&mutex_fork);
// 		philo->fork = 1;
// 		philo->next->fork = 1;
// 		pthread_mutex_unlock(&mutex_fork);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&mutex_fork);
// 		philo->fork = 0;
// 		philo->next->fork = 0;
// 		pthread_mutex_unlock(&mutex_fork);
// 	}
// }

void eat(t_philosopher *philo)
{
	unsigned long time;

	time = get_time_mls() - philo->entry_time;
	print(1, philo);
	philo->status = 1;
	philo->last_meal = time;
	ft_usleep(philo->data->time_to_eat);
}

int check_death(t_philosopher *philo)
{
	unsigned long time;

	time = get_time_mls() - philo->entry_time;
	if (time - philo->last_meal > philo->data->time_to_die)
	{
		printf("%d-----------DEAATH--------\n", philo->index);
		printf("index: %d\ntime: %lu\nlastmeal: %lu\ntimetodie: %lu\n", philo->index, time, philo->last_meal, philo->data->time_to_die);
		return 1;
	}
	return 0;
}

int sleep_philo(t_philosopher *philo)
{
	int death;
	unsigned long current_time = get_time_mls();

	philo->status = 2;
	print(2, philo);
	while (current_time + philo->data->time_to_sleep > get_time_mls())
	{
		death = check_death(philo);
		if (death)
			return 1;
		usleep(1000);
	}

	return 0;
}

void think(t_philosopher *philo)
{
	philo->status = 3;
	print(3, philo);
}

void handle_first_meals(t_philosopher *philo)
{
	if (philo->status == 0 && philo->index != philo->data->number_of_philosophers && philo->index % 2)
		eat(philo);
}

int check_philo(t_philosopher *philo)
{
	int death;

	philo->time = get_time_mls() - philo->entry_time;
	if (philo->status == 1)
	{
		death = sleep_philo(philo);
		if (death)
			return 1;
	}
	if (philo->status == 2)
		think(philo);
	if (philo->status == 3 || philo->status == 0)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->next->fork);
		eat(philo);
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
	}
	if (check_death(philo))
		return 1;
	return 0;
}

void *handle_philo(void *philo_)
{
	int *death;

	death = malloc(sizeof(int));
	t_philosopher *philo = (t_philosopher *)philo_;
	// handle_first_meals(philo);
	while (1)
	{
		*death = check_philo(philo);
		if (*death)
			return ((void *)death);
	}
}

int create_philosophers(t_data *data)
{
	int i;
	t_philosopher *new;
	t_philosopher *curr;
	int *result;

	pthread_t threads[data->number_of_philosophers];
	i = -1;
	unsigned long entry_time = get_time_mls();
	while (++i < data->number_of_philosophers)
	{
		new = malloc(sizeof(t_philosopher));
		new->entry_time = entry_time;
		new->data = data;
		new->index = i + 1;
		if (pthread_mutex_init(&new->fork, NULL) != 0)
			printf("\n mutex init failed\n");
		new->status = 0;
		new->last_meal = 0;
		add_node(&curr, &new, i);
		pthread_create(&threads[i], NULL, &handle_philo, new);
	}

	i = -1;
	while (++i < data->number_of_philosophers)
	{
		pthread_join(threads[i], (void *)&result);
		printf("-> %d\n", *result);
		if (*result)
			return 1;
	}
}

int main(int argc, char **argv)
{
	t_data data;

	if (argc != 5 && argc != 6)
		return 1;
	if (pthread_mutex_init(&mutex, NULL) != 0)
		printf("\n mutex init failed\n");
	get_data(&data, argc, argv);
	if (create_philosophers(&data))
		return 1;
	return 0;
}