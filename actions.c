/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozakkare <ozakkare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:00:19 by ozakkare          #+#    #+#             */
/*   Updated: 2021/11/16 18:59:31 by ozakkare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

void	eat(t_philosopher *philo)
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

int	check_death(t_philosopher *philo)
{
	unsigned long	time;
	int				status;

	status = philo->status;
	time = get_time_mls() - philo->entry_time;
	if (time - philo->last_meal > philo->data->time_to_die)
	{
		if (status != 1)
		{
			printf("\e[0;31m%lu: philosopher %d died\e[0;37m\n", get_time_mls()
				- philo->entry_time, philo->index);
			return (1);
		}
	}
	return (0);
}

void	sleep_philo(t_philosopher *philo)
{
	philo->status = 2;
	print(2, philo);
	ft_usleep(philo->data->time_to_sleep);
}

void	think(t_philosopher *philo)
{
	philo->status = 3;
	print(3, philo);
}
