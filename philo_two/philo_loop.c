/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_loop.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/22 19:34:23 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/22 19:34:23 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_eat(t_philo *philo, unsigned int id)
{
	sem_wait(philo->args->forks);
	wr_lock(id, "picked up first fork", philo->args);
	sem_wait(philo->args->forks);
	wr_lock(id, "picked up second fork", philo->args);
	sem_wait(philo->args->death_sem);
	philo->last_eaten_t = gettime();
	sem_post(philo->args->death_sem);
	philo->times_eaten++;
	wr_lock(id, "is eating", philo->args);
	faksleep(philo->args->eat_t);
	sem_post(philo->args->forks);
	wr_lock(id, "dropped first fork", philo->args);
	sem_post(philo->args->forks);
	wr_lock(id, "dropped second fork", philo->args);
}

static int	philo_sleep_and_death_check(t_philo *philo, unsigned int id)
{
	wr_lock(id, "is sleeping", philo->args);
	faksleep(philo->args->sleep_t);
	sem_wait(philo->args->death_sem);
	if (philo->args->dead)
	{
		sem_post(philo->args->death_sem);
		return (1);
	}
	sem_post(philo->args->death_sem);
	return (0);
}

static void	*philo_loop(void *phil_ptr)
{
	t_philo			*philo;
	unsigned int	id;
	int				i;

	philo = (t_philo*)phil_ptr;
	id = philo->philo_nr;
	if (id % 2)
		usleep(100);
	i = 0;
	while (i != philo->args->times_eat)
	{
		if (i)
			wr_lock(id, "is thinking", philo->args);
		philo_eat(philo, id);
		if (philo_sleep_and_death_check(philo, id))
			return (NULL);
		i++;
	}
	return (NULL);
}

int			start_threads(t_arrgs *args, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < (*args).n_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_loop, &philos[i]))
		{
			join_threads(philos, i);
			return (1);
		}
		usleep(10);
		i++;
	}
	return (0);
}
