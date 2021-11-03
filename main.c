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

void *handle_philo(void *philo)
{
    printf("%d\n", ((t_philosophers *)philo)->index);
}

void create_philosophers(t_data *data)
{
    int i;
    t_philosophers *head;
    t_philosophers *new;
    t_philosophers *curr;

    pthread_t threads[data->number_of_philosophers];
    i = -1;
    while (++i < data->number_of_philosophers)
    {
        new = malloc(sizeof(t_philosophers));
        new->data = data;
        new->id = threads[i];
        new->index = i + 1;
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
        }
        pthread_create(&threads[i], NULL, &handle_philo, new);
    }
}

int main(int argc, char **argv){
    t_data data;

    if (argc != 5 && argc != 6)
        return 1;
    get_data(&data, argc, argv);
    create_philosophers(&data);
    for(;;);
}