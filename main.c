/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozakkare <ozakkare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:05:59 by ozakkare          #+#    #+#             */
/*   Updated: 2021/11/17 18:17:18 by ozakkare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

void	*handle_philo(void *philo_)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philo_;
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

t_philosopher	*create_nodes(t_data *data)
{
	int				i;
	t_philosopher	*new;
	t_philosopher	*curr;

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
	return (new);
}

void	create_threads(t_data *data, t_philosopher *philo)
{
	unsigned long	time;
	int				i;
	pthread_t		threads;

	i = -1;
	time = get_time_mls();
	while (++i < data->number_of_philosophers)
	{
		philo = philo->next;
		philo->entry_time = time;
		usleep(1);
		pthread_create(&threads, NULL, &handle_philo, philo);
	}
}

t_philosopher	*philosophers(t_data *data)
{
	t_philosopher	*philo;

	if (pthread_mutex_init(&data->mutex, NULL) != 0)
		printf("\n mutex init failed\n");
	philo = create_nodes(data);
	create_threads(data, philo);
	return (philo);
}

int	main(int argc, char **argv)
{
	t_data			data;
	int				philos_checked;
	t_philosopher	*philo;

	philos_checked = 0;
	if (check_data(argc, argv, &data))
		return (0);
	philo = philosophers(&data);
	while (1)
	{
		philo = philo->next;
		if (check_death(philo))
			return (0);
		if (data.meals != -1 && philo->meals == data.meals)
		{
			++philos_checked;
			philo->meals = -1;
			if (philos_checked == data.number_of_philosophers)
				return (0);
		}
		usleep(1000);
	}
	return (0);
}
