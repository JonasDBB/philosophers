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

static void	philo_eat(t_philo *philo, unsigned int id,
				pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	pthread_mutex_lock(right_fork);
	wr_lock(id, "picked up right fork", philo->args);
	pthread_mutex_lock(left_fork);
	wr_lock(id, "picked up left fork", philo->args);
	pthread_mutex_lock(&philo->time_check_lock);
	philo->last_eaten_t = gettime();
	pthread_mutex_unlock(&philo->time_check_lock);
	philo->times_eaten++;
	wr_lock(id, "is eating", philo->args);
	faksleep(philo->args->eat_t);
	pthread_mutex_unlock(left_fork);
	wr_lock(id, "dropped left fork", philo->args);
	pthread_mutex_unlock(right_fork);
	wr_lock(id, "dropped right fork", philo->args);
}

static int	philo_sleep_and_death_check(t_philo *philo, unsigned int id)
{
	wr_lock(id, "is sleeping", philo->args);
	faksleep(philo->args->sleep_t);
	pthread_mutex_lock(&philo->args->death_lock);
	if (philo->args->dead)
	{
		pthread_mutex_unlock(&philo->args->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->args->death_lock);
	return (0);
}

static void	*philo_loop(void *phil_ptr)
{
	t_philo			*philo;
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t *right_fork;
	int				i;

	philo = (t_philo*)phil_ptr;
	id = philo->philo_nr;
	right_fork = &philo->args->forks[id];
	left_fork = &philo->args->forks[(id + 1) % philo->args->n_philos];
	if (id % 2)
		usleep(100);
	i = 0;
	while (i != philo->args->times_eat)
	{
		if (i)
			wr_lock(id, "is thinking", philo->args);
		philo_eat(philo, id, left_fork, right_fork);
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
		i++;
	}
	return (0);
}
