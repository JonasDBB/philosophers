/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/16 08:17:11 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/16 08:17:11 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	check_death(t_arrgs *args, t_philo *philos, unsigned int i)
{
	if (philos[i].times_eaten != (*args).times_eat)
	{
		wr_lock(i, "has died", args);
		pthread_mutex_lock(&(*args).death_lock);
		(*args).dead = true;
		pthread_mutex_unlock(&(*args).death_lock);
	}
}

int		monitor_philos(t_arrgs *args, t_philo *philos)
{
	unsigned int	i;
	bool			stop;

	i = 0;
	stop = false;
	while (i < (*args).n_philos)
	{
		pthread_mutex_lock(&philos[i].time_check_lock);
		if (gettime() - philos[i].last_eaten_t > (*args).die_t)
		{
			check_death(args, philos, i);
			pthread_mutex_unlock(&philos[i].time_check_lock);
			stop = true;
			break ;
		}
		pthread_mutex_unlock(&philos[i].time_check_lock);
		i++;
	}
	usleep(500);
	if (stop)
		return (1);
	return (0);
}

int		main(int ac, char **av)
{
	t_arrgs			args;
	t_philo			*philos;

	if (check_and_set_input(ac, av, &args))
		return (1);
	if (setup_malloc(&args, &philos))
		return (1);
	if (create_forks(&args, philos))
		return (1);
	if (init_philos(&args, philos))
		return (1);
	if (start_threads(&args, philos))
		return (1);
	while (!monitor_philos(&args, philos))
		continue ;
	join_threads(philos, args.n_philos);
	return (0);
}
