/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozakkare <ozakkare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:03:24 by ozakkare          #+#    #+#             */
/*   Updated: 2021/11/17 18:17:48 by ozakkare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

static int	whitespace(char c)
{
	if (c == '\n' || c == '\v' || c == '\f' || c == '\t' || c == '\r')
		return (1);
	if (c == ' ')
		return (1);
	return (0);
}

int	check_str(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{	
		if (!(str[i] >= '0' && str[i] <= '9') && str[i] != '+')
			return (0);
		i++;
	}
	return (1);
}

long long	ft_atoi(const char *str)
{
	int				i;
	int				negativity;
	unsigned long	result;

	result = 0;
	negativity = 1;
	i = 0;
	if (!check_str(str))
		return (0);
	while (whitespace(str[i]))
		i++;
	if (str[i] == '-')
	{
		return (0);
		negativity = -1;
	}
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= 48 && str[i] <= 57)
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (result * negativity);
}

int	check_data(int argc, char **argv, t_data *data)
{
	if (argc != 5 && argc != 6)
	{
		printf("\e[0;31mUNVALID ARGUMENTS\e[0;37m\n");
		return (1);
	}
	get_data(data, argc, argv);
	if (data->number_of_philosophers <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (data->meals <= 0 && argc == 6))
	{
		printf("\e[0;31mUNVALID ARGUMENTS\e[0;37m\n");
		return (1);
	}
	return (0);
}
