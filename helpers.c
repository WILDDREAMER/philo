/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozakkare <ozakkare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:04:06 by ozakkare          #+#    #+#             */
/*   Updated: 2021/11/17 18:01:49 by ozakkare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

void	get_data(t_data *data, int argc, char **argv)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals = -1;
	if (argc == 6)
		data->meals = ft_atoi(argv[5]);
}

void	add_node(t_philosopher **curr, t_philosopher **new, int i)
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

unsigned long	get_time_mls(void)
{
	struct timeval	time;
	unsigned long	i;

	gettimeofday(&time, NULL);
	i = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (i);
}

void	ft_usleep(unsigned long time)
{
	unsigned long	current_time;

	current_time = get_time_mls();
	usleep((time * 1e3) - 5000);
	while (get_time_mls() - current_time < time)
		continue ;
}

void	print(int status, t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	if (status == 1)
	{
		printf("%lu: %d has taken a fork\n", get_time_mls()
			- philo->entry_time, philo->index);
		printf("%lu: %d has taken a fork\n", get_time_mls()
			- philo->entry_time, philo->index);
		printf("%lu: %d is eating\n", get_time_mls()
			- philo->entry_time, philo->index);
	}
	else if (status == 2)
		printf("%lu: %d is sleeping\n", get_time_mls()
			- philo->entry_time, philo->index);
	else if (status == 3)
		printf("%lu: %d is thinking\n", get_time_mls()
			- philo->entry_time, philo->index);
	else if (status == 4)
		printf("\e[0;31m%lu: philosopher %d died\e[0;37m\n", get_time_mls()
			- philo->entry_time, philo->index);
	if (status != 4)
		pthread_mutex_unlock(&philo->data->mutex);
}
