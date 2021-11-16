/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozakkare <ozakkare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 18:09:22 by ozakkare          #+#    #+#             */
/*   Updated: 2021/11/16 18:34:58 by ozakkare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_data{
	pthread_mutex_t	mutex;
	int				number_of_philosophers;
	unsigned long	time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals;
}				t_data;

typedef struct s_philosophers{
	struct s_philosophers	*next;
	struct s_philosophers	*prev;
	int						index;
	int						status;
	pthread_mutex_t			fork;
	unsigned long			last_meal;
	struct s_data			*data;
	unsigned long			entry_time;
	int						meals;
}				t_philosopher;

long long		ft_atoi(const char *str);
int				push(t_philosopher **head);
void			get_data(t_data *data, int argc, char **argv);
void			add_node(t_philosopher **curr, t_philosopher **new, int i);
unsigned long	get_time_mls(void);
void			ft_usleep(unsigned long time);
void			print(int status, t_philosopher *philo);
void			eat(t_philosopher *philo);
int				check_death(t_philosopher *philo);
void			sleep_philo(t_philosopher *philo);
void			think(t_philosopher *philo);
int				check_data(int argc, char **argv, t_data *data);

#endif