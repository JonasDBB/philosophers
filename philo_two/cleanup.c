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

void	destroy_sems(t_arrgs *args, t_philo *philos)
{
	sem_close(args->forks);
	sem_close(args->death_sem);
	sem_close(args->write_sem);
	free(philos);
}

void	join_threads(t_philo *philos, unsigned int i)
{
	while (i)
	{
		i--;
		pthread_join(philos[i].thread, NULL);
	}
	destroy_sems(philos[0].args, philos);
}
