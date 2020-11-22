/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/22 19:27:49 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/22 19:27:49 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_singles(t_arrgs *args)
{
	pthread_mutex_destroy(&(*args).death_lock);
	pthread_mutex_destroy(&(*args).write_lock);
}

void	destroy_forks(t_arrgs *args, unsigned int i, t_philo *philos)
{
	while (i)
	{
		i--;
		pthread_mutex_destroy(&(*args).forks[i]);
	}
	destroy_singles(&(*args));
	free((*args).forks);
	free(philos);
}

void	destroy_time_mutex(t_philo *philos, unsigned int i)
{
	while (i)
	{
		i--;
		pthread_mutex_destroy(&philos[i].time_check_lock);
	}
	destroy_forks(philos[0].args, philos[0].args->n_philos, philos);
}

void	join_threads(t_philo *philos, unsigned int i)
{
	while (i)
	{
		i--;
		pthread_join(philos[i].thread, NULL);
	}
	destroy_time_mutex(philos, philos[0].args->n_philos);
}
