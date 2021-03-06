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
# include <stdlib.h>
# include <sys/time.h>

typedef struct	s_arrgs {
	bool			dead;
	unsigned int	n_philos;
	unsigned int	die_t;
	unsigned int	eat_t;
	unsigned int	sleep_t;
	int				times_eat;
	unsigned long	start_t;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_lock;
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

int				start_threads(t_arrgs *args, t_philo *philos);

/*
** setup.c
*/
int				check_and_set_input(int ac, char **av, t_arrgs *args);
int				setup_malloc(t_arrgs *args, t_philo **philos);
int				create_forks(t_arrgs *args, t_philo *philos);
int				init_philos(t_arrgs *args, t_philo *philos);

/*
** cleanup.c
*/
void			destroy_singles(t_arrgs *args);
void			destroy_forks(t_arrgs *args, unsigned int i, t_philo *philos);
void			destroy_time_mutex(t_philo *philos, unsigned int i);
void			join_threads(t_philo *philos, unsigned int i);

/*
** locked_writing.c
*/
void			wr_lock(unsigned int philo_id, const char *str, t_arrgs *args);

/*
** ft_helper_functions.c
*/
unsigned long	gettime(void);
void			faksleep(unsigned int sleep_t);
int				ft_atoi(const char *str);

#endif
