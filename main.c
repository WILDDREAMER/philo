#include "./philosophers.h"

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
pthread_mutex_t			mutex;
pthread_mutex_t			mutex_fork;

unsigned long	get_time_mls(void)
{
	struct timeval	time;
	unsigned long	i;

	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (i);
}

void eat(t_struct *st)
{
	t_philosophers *philo;
	unsigned long time;

	time = get_time_mls() - ((t_struct *)st)->entry_time;
	philo = st->current_philo;
	philo->fork->status = 1;
	printf("%lu: %d has taken a fork\n", time, philo->index);
	philo->next->fork->status = 1;
	printf("%lu: %d has taken a fork\n", time, philo->index);
	philo->status = 1;
	philo->last_meal = time;
	printf("%lu: %d is eating\n", time, philo->index);
	usleep(st->data->time_to_eat * 1000);
	philo->fork->status = 0;
	philo->next->fork->status = 0;
}

void handle_first_meals(t_struct *st)
{
	t_philosophers *philo;
	
	philo = st->current_philo;
	if (philo->status == 0 && philo->index != st->data->number_of_philosophers && philo->index % 2)
		eat(st);
}

void sleep_philo(t_struct *st)
{
	t_philosophers *philo;
	unsigned long time;

	time = get_time_mls() - ((t_struct *)st)->entry_time;
	philo = st->current_philo;
	philo->status = 2;
	printf("%lu: %d is sleeping\n", time, philo->index);
	usleep(st->data->time_to_sleep * 1000);
}

void think(t_struct *st)
{
	t_philosophers *philo;
	unsigned long time;

	time = get_time_mls() - ((t_struct *)st)->entry_time;
	philo = st->current_philo;
	philo->status = 3;
	printf("%lu: %d is thinking\n", time, philo->index);
}

void check_philo(t_struct *st)
{
	t_philosophers *philo;

	philo = st->current_philo;
	if (philo->status == 1)
		sleep_philo(st);
	if (philo->status == 2)
		think(st);
	if (philo->status == 3 || philo->status == 0)
		if(!philo->fork->status && !philo->next->fork->status)
			eat(st);
}

void *handle_philo(void *struct_)
{
	t_struct *st = (t_struct *)struct_;
	handle_first_meals((t_struct *)st);
	while (1)
		check_philo(st);
}

void    add_node(t_philosophers **curr, t_philosophers **new, int i)
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

void create_philosophers(t_data *data)
{
	int i;
	t_struct        st;
	t_philosophers *new;
	t_philosophers *curr;
	t_fork         fork;

	pthread_t threads[data->number_of_philosophers];
	i = -1;
	st.entry_time = get_time_mls();
	while (++i < data->number_of_philosophers)
	{
		new = malloc(sizeof(t_philosophers));
		st.data = data;
		new->index = i + 1;
		fork.status = 0;
		new->fork = &fork;
		new->status = 0;
		add_node(&curr, &new, i);
		st.current_philo = new;
		pthread_create(&threads[i], NULL, &handle_philo, &st);
		pthread_join(threads[i], NULL);
	}
}

int main(int argc, char **argv){
	t_data data;

	if (argc != 5 && argc != 6)
		return 1;
	get_data(&data, argc, argv);
	create_philosophers(&data);
}