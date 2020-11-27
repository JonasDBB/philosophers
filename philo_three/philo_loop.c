/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_loop.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jonasbb <jonasbb@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/27 17:34:16 by jonasbb       #+#    #+#                 */
/*   Updated: 2020/11/27 17:34:16 by jonasbb       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_eat(t_philo *philo, unsigned int id)
{
	sem_wait(philo->args->forks);
	wr_lock(id, "picked up first fork", philo->args);
	sem_wait(philo->args->forks);
	wr_lock(id, "picked up second fork", philo->args);
	sem_wait(philo->death_sem);
	philo->last_eaten_t = gettime();
	philo->times_eaten++;
	sem_post(philo->death_sem);
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
	sem_wait(philo->death_sem);
	if (philo->args->dead)
	{
		sem_post(philo->death_sem);
		return (1);
	}
	sem_post(philo->death_sem);
	return (0);
}

static void	check_death(t_philo *philo)
{
	if (philo->times_eaten != philo->args->times_eat)
	{
		wr_lock(philo->philo_nr, "has died", philo->args);
		sem_wait(philo->args->write_sem);
		philo->args->dead = true;
		exit(1);
	}
}

int			monitor_philos(t_philo *philo)
{
	bool	stop;

	stop = false;
	sem_wait(philo->death_sem);
	if (gettime() - philo->last_eaten_t > philo->args->die_t)
	{
		check_death(philo);
		stop = true;
	}
	sem_post(philo->death_sem);
	usleep(500);
	if (stop)
		return (1);
	return (0);
}

void		*philo_loop(void *phil_ptr)
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
