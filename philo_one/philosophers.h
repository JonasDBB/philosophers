/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/16 08:36:45 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/16 08:36:45 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdbool.h>
# include <pthread.h>
# include <stddef.h>
# include <unistd.h>

bool			g_dead;
pthread_mutex_t	g_dead_lock;

typedef struct	s_arrgs {
	unsigned int	n_philos;
	unsigned int	die_t;
	unsigned int	eat_t;
	unsigned int	sleep_t;
	int				times_eat;
	unsigned long	start_t;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
}				t_arrgs;

typedef struct	s_philo {
	pthread_t		thread;
	int				times_eaten;
	unsigned int	philo_nr;
	unsigned long	last_eaten_t;
	pthread_mutex_t	time_check_lock;
	t_arrgs			*args;
}				t_philo;

void			write_lock(unsigned int philo_id, const char *str, t_arrgs *args);
int				ft_atoi(const char *str);


unsigned long	gettime(void);

#endif
