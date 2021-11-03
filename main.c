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

unsigned long	get_time_mls(void)
{
	struct timeval	time;
	unsigned long	i;

	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (i);
}

void *handle_philo(void *vars)
{
    pthread_mutex_lock(&mutex);
    unsigned long time;
    time = get_time_mls() - ((t_struct *)vars)->entry_time;
    // printf("%d: %lu \n",((t_struct *)vars)->current_philo->fork->holder, time);
    // printf("%d\n", ((t_struct *)vars)->current_philo->prev->index);
    pthread_mutex_unlock(&mutex);
}

void create_philosophers(t_data *data)
{
    int i;
    t_struct        vars;
    t_philosophers *head;
    t_philosophers *new;
    t_philosophers *curr;
    t_fork         fork;

    pthread_t threads[data->number_of_philosophers];
    i = -1;
    vars.entry_time = get_time_mls();
    while (++i < data->number_of_philosophers)
    {
        new = malloc(sizeof(t_philosophers));
        vars.data = data;
        new->index = i + 1;
        fork.holder = 0;
        new->fork = &fork;
        new->status = 0;
        if (!i)
        {
            head = new;
            head->next = new;
            head->prev = new;
            curr = new;
        }
        else
        {
            new->next = head;
            curr->next = new;
            new->prev = curr;
            head->prev = new;
            curr = new;
        }
        vars.current_philo = new;
        pthread_create(&threads[i], NULL, &handle_philo, &vars);
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