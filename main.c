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
	unsigned long current_time;

	current_time = get_time_mls();
	usleep((time * 1e3) - 5000);
	while (get_time_mls() - current_time < time)
		continue;
}

void print(int status, t_philosopher *philo)
{
	pthread_mutex_lock(&mutex);
	if (status == 1)
	{
		printf("%lu: %d has taken a fork\n", get_time_mls() - philo->entry_time, philo->index);
		printf("%lu: %d has taken a fork\n", get_time_mls() - philo->entry_time, philo->index);
		printf("%lu: %d is eating\n", get_time_mls() - philo->entry_time, philo->index);
	}
	else if (status == 2)
		printf("%lu: %d is sleeping\n", get_time_mls() - philo->entry_time, philo->index);
	else if (status == 3)
		printf("%lu: %d is thinking\n", get_time_mls() - philo->entry_time, philo->index);
	pthread_mutex_unlock(&mutex);
}

void eat(t_philosopher *philo)
{
	philo->status = 1;
	if (philo->meals == philo->data->meals)
		philo->meals = -1;
	else
		philo->meals++;
	print(1, philo);
	philo->last_meal = get_time_mls() - philo->entry_time;
	ft_usleep(philo->data->time_to_eat);
}

int check_death(t_philosopher *philo)
{
	unsigned long time;
	int status;

	status = philo->status;
	time = get_time_mls() - philo->entry_time;
	if (time - philo->last_meal > philo->data->time_to_die)
		if (status != 1)
		{
			printf("\e[1;31m%lu: philosopher %d died  last meal: %lu\e[0;37m\n", time, philo->index, philo->last_meal);
			return 1;
		}
	return 0;
}

void sleep_philo(t_philosopher *philo)
{
	int death;
	unsigned long current_time = get_time_mls();

	philo->status = 2;
	print(2, philo);
	ft_usleep(philo->data->time_to_sleep);
}

void think(t_philosopher *philo)
{
	philo->status = 3;
	print(3, philo);
}

void *handle_philo(void *philo_)
{
	t_philosopher *philo = (t_philosopher *)philo_;
	while (1)
	{
		if (philo->status == 1)
			sleep_philo(philo);
		if (philo->status == 2)
			think(philo);
		if (philo->status == 3)
		{
			pthread_mutex_lock(&philo->fork);
			pthread_mutex_lock(&philo->next->fork);
			eat(philo);
			pthread_mutex_unlock(&philo->fork);
			pthread_mutex_unlock(&philo->next->fork);
		}
	}
}

void create_philosophers(t_data *data)
{
	int i;
	t_philosopher *new;
	t_philosopher *curr;
	int *result;

	pthread_t threads[data->number_of_philosophers];
	i = -1;
	while (++i < data->number_of_philosophers)
	{
		new = malloc(sizeof(t_philosopher));
		new->data = data;
		new->index = i + 1;
		if (pthread_mutex_init(&new->fork, NULL) != 0)
			printf("\n mutex init failed\n");
		new->status = 3;
		new->last_meal = 0;
		new->meals = 0;
		add_node(&curr, &new, i);
	}
	i = -1;
	unsigned long time = get_time_mls();
	while (++i < data->number_of_philosophers)
	{
		new = new->next;
		new->entry_time = time;
		usleep(1);
		pthread_create(&threads[i], NULL, &handle_philo, new);
	}
	i = -1;
	int philos_checked = 0;
	while (1)
	{
		new = new->next;
		if (check_death(new))
			exit(0);
		if (data->meals != -1 && new->meals == data->meals)
		{
			++philos_checked;
			new->meals = -1;
		}
		if (philos_checked == data->number_of_philosophers)
			exit(0);
		usleep(1000);
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
	create_philosophers(&data);
	return 0;
}