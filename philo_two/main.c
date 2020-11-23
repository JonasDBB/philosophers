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
		sem_post((*args).death_sem);
		wr_lock(i, "has died", args);
		sem_wait((*args).death_sem);
		(*args).dead = true;
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
		sem_wait(args->death_sem);
		if (gettime() - philos[i].last_eaten_t > (*args).die_t)
		{
			check_death(args, philos, i);
			sem_post(args->death_sem);
			stop = true;
			break ;
		}
		sem_post(args->death_sem);
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
	if (create_sems(&args, philos))
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
