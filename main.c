#include "./philosophers.h"
pthread_mutex_t			mutex;
pthread_mutex_t			mutex_fork;

void    get_data(t_data *data,int argc, char **argv)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals = -1;
	if(argc == 6)
		data->meals = ft_atoi(argv[5]);
}

unsigned long	get_time_mls(void)
{
	struct timeval	time;
	unsigned long	i;

	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (i);
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
	else if(status == 2)
		printf("%lu: %d is sleeping\n", time, philo->index);
	else if(status == 3)
		printf("%lu: %d is thinking\n", time, philo->index);
	pthread_mutex_unlock(&mutex);
}

void handle_forks(int operation, t_philosopher *philo)
{
	if(operation == 1)
	{
		pthread_mutex_lock(&mutex_fork);
		philo->fork= 1;
		philo->next->fork= 1;
		pthread_mutex_unlock(&mutex_fork);
	}
	else
	{
		pthread_mutex_lock(&mutex_fork);
		philo->fork= 0;
		philo->next->fork= 0;
		pthread_mutex_unlock(&mutex_fork);
	}
}

void eat(t_philosopher *philo)
{
	unsigned long time;

	time = get_time_mls() - philo->entry_time;
	handle_forks(1, philo);
	print(1, philo);
	philo->status = 1;
	philo->last_meal = time;
	usleep(philo->data->time_to_eat * 1000);
	handle_forks(0, philo);
}

int check_death(t_philosopher *philo)
{
	unsigned long time;

	time = get_time_mls() - philo->entry_time;
	if(time - philo->last_meal > philo->data->time_to_die)
	{
		pthread_mutex_lock(&mutex);
		printf("index: %d\ntime: %lu\nlastmeal: %lu\ntimetodie: %lu\n",philo->index, time, philo->last_meal, philo->data->time_to_die);
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	return 1;
}

void sleep_philo(t_philosopher *philo)
{
	unsigned long time;
	unsigned long i;
	int death;

	time = get_time_mls() - philo->entry_time;
	philo->status = 2;
	print(2, philo);
	i = 0;
	while (i < philo->data->time_to_sleep * 1000)
	{
		death = check_death(philo);
		if (!death)
		{
			printf("%d-----------DEAATH--------\n", philo->index);
			return ;
		}
		usleep(i);
		i += 900;
	}
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

void check_philo(t_philosopher *philo)
{
	if (philo->status == 1)
		sleep_philo(philo);
	if (philo->status == 2)
		think(philo);
	if (philo->status == 3 || philo->status == 0)
		if(!philo->fork && !philo->next->fork)
			eat(philo);	
}

void    add_node(t_philosopher **curr, t_philosopher **new, int i)
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

void *handle_philo(void *philo_)
{
	t_philosopher *philo = (t_philosopher *)philo_;
	handle_first_meals(philo);
	// while (1)
		check_philo(philo);
}

void create_philosophers(t_data *data)
{
	int i;
	t_philosopher *new;
	t_philosopher *curr;

	pthread_t threads[data->number_of_philosophers];
	i = -1;
	unsigned long entry_time = get_time_mls();
	while (++i < data->number_of_philosophers)
	{
		new = malloc(sizeof(t_philosopher));
		new->entry_time = entry_time;
		new->data = data;
		new->index = i + 1;
		new->fork = 0;
		new->status = 0;
		add_node(&curr, &new, i);
	}
	t_philosopher *tmp;

	i = -1;
	tmp = new;
	while (++i < data->number_of_philosophers)
	{
		tmp = tmp->next;
		// usleep(1);
		if ((i + 1) % 2)
			pthread_create(&threads[i], NULL, &handle_philo, tmp);
	}
	i = -1;
	tmp = new;
	while (++i < data->number_of_philosophers)
	{
		tmp = tmp->next;
		// usleep(1);
		if (!((i + 1) % 2))
			pthread_create(&threads[i], NULL, &handle_philo, tmp);
	}
	i = -1;
	while(1);
	// while (++i < data->number_of_philosophers)
	// 	pthread_join(threads[i], NULL);

}

int main(int argc, char **argv){
	t_data data;

	if (argc != 5 && argc != 6)
		return 1;
	if (pthread_mutex_init(&mutex, NULL) != 0)
		printf("\n mutex init failed\n");
	if (pthread_mutex_init(&mutex_fork, NULL) != 0)
		printf("\n mutex init failed\n");
	get_data(&data, argc, argv);
	create_philosophers(&data);
}