#ifndef PHILOLOSOPHERS_H
# define PHILOLOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct 		s_data{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals;
}               	t_data;

typedef struct 		s_fork{
	int				status;
}					t_fork;

typedef struct 		s_philosophers{
	struct s_philosophers	*next;
	struct s_philosophers	*prev;
	int						index;
	int						status;
	struct s_fork			*fork;
	int						last_meal;
}               	t_philosophers;

typedef struct		s_struct{
	pthread_mutex_t			mutex;
	struct s_data			*data;
	unsigned long			entry_time;
	struct s_philosophers	*current_philo;
}					t_struct;

int			ft_isdigit(int c);
size_t		ft_strlen(const char *str);
void		log_error(char *msg);
static int	whitespace(char c);
long long	ft_atoi(const char *str);
int			push(t_philosophers **head);

#endif