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

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "philosophers.h"

unsigned long	gettime(void)
{
	struct timeval	crn_time;

	gettimeofday(&crn_time, NULL);
	return (crn_time.tv_sec * 1000) + (crn_time.tv_usec / 1000);
}

void setargs(int ac, char **av, t_arrgs *args)
{
	(*args).n_philos = ft_atoi(av[1]);
	(*args).die_t = ft_atoi(av[2]);
	(*args).eat_t = ft_atoi(av[3]);
	(*args).sleep_t = ft_atoi(av[4]);
	if (ac == 6)
		(*args).times_eat = ft_atoi(av[5]);
	else
		(*args).times_eat = -1;
	(*args).start_t = gettime();
}

int 	check_and_set_input(int ac, char **av, t_arrgs *args)
{
	if (ac != 5 && ac != 6)
	{
		write(2, "Error: incorrect args\n", 27);
		return (1);
	}
	setargs(ac, av, &(*args));
	if (!args->die_t ||!args->eat_t || !args->sleep_t || !args->times_eat)
	{
		write(2, "Error: please enter positive values\n", 36);
		return (1);
	}
	if (args->n_philos < 2)
	{
		write(2, "Error: please enter more than one philosopher\n", 46);
		return (1);
	}
	g_dead = false;
	pthread_mutex_init(&(*args).write_lock, NULL);
	return (0);
}

void	faksleep(unsigned int sleep_t)
{
	unsigned int zzz;

	zzz = 0;
	while (zzz < sleep_t)
	{
		usleep(100);
		zzz += 100;
	}
}

void	*do_de_ting(void *phil)
{
	t_philo			*philo;
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t *right_fork;

	philo = (t_philo*)phil;
	id = philo->philo_nr;
	right_fork = &philo->args->forks[id];
	left_fork = &philo->args->forks[(id + 1) % philo->args->n_philos];
	if (id % 2)
		usleep(100);
	int i = 0;
	while (i != philo->args->times_eat)
	{
		// pickup forks
		if (g_dead)
		{
			return (NULL);
		}
		pthread_mutex_lock(right_fork);
		write_lock(id, "picked up right fork", philo->args);
		if (g_dead)
		{
			pthread_mutex_unlock(right_fork);
			return (NULL);
		}
		pthread_mutex_lock(left_fork);
		write_lock(id, "picked up left fork", philo->args);
		if (g_dead)
		{
			pthread_mutex_unlock(left_fork);
			pthread_mutex_unlock(right_fork);
			return (NULL);
		}
		// eating
		pthread_mutex_lock(&philo->time_check_lock);
		philo->last_eaten_t = gettime();
		pthread_mutex_unlock(&philo->time_check_lock);
		philo->times_eaten++;
		write_lock(id, "is eating", philo->args);
		usleep(philo->args->eat_t * 1000);
		// drop forks
		pthread_mutex_unlock(left_fork);
		write_lock(id, "dropped up left fork", philo->args);
		pthread_mutex_unlock(right_fork);
		write_lock(id, "dropped up right fork", philo->args);
		// sleeping
		if (g_dead)
		{
			return (NULL);
		}
		write_lock(id, "is sleeping", philo->args);
		usleep(philo->args->sleep_t * 1000);
		write_lock(id, "is thinking", philo->args);
		i++;
	}
	return (NULL);
}

int		main(int ac, char **av)
{
	t_arrgs			args;
	pthread_t		*threads;
	t_philo			*philos;
	unsigned int	i;

	if (check_and_set_input(ac, av, &args))
		return (1);
	args.forks = malloc(sizeof(pthread_mutex_t) * args.n_philos);
	threads = malloc(sizeof(pthread_t) * args.n_philos);
	philos = malloc(sizeof(t_philo) * args.n_philos);
	i = 0;
	pthread_mutex_init(&g_dead_lock, NULL);
	while (i < args.n_philos)
	{
		pthread_mutex_init(&args.forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < args.n_philos)
	{
		philos[i].philo_nr = i;
		philos[i].args = &args;
		philos[i].last_eaten_t = args.start_t;
		philos[i].times_eaten = 0;
		pthread_mutex_init(&philos[i].time_check_lock, NULL);
		pthread_create(&threads[i], NULL, do_de_ting, &philos[i]);
		i++;
	}
	while (1)
	{
		bool stop = false;
		i = 0;
		while (i < args.n_philos)
		{
			pthread_mutex_lock(&philos[i].time_check_lock);
			if (gettime() - philos[i].last_eaten_t > args.die_t)
			{
				if (philos[i].times_eaten != args.times_eat)
				{
					write_lock(i, "has died", &args);
					g_dead = true;
				}
				stop = true;
				pthread_mutex_unlock(&philos[i].time_check_lock);
				break ;
			}
			pthread_mutex_unlock(&philos[i].time_check_lock);
			i++;
		}
		if (g_dead || stop)
		{
			break ;
		}
	}
	i = 0;
	while (i < args.n_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	i = 0;
	pthread_mutex_destroy(&args.write_lock);
	pthread_mutex_destroy(&philos[i].time_check_lock);
	pthread_mutex_destroy(&g_dead_lock);
	while (i < args.n_philos)
	{
		pthread_mutex_destroy(&args.forks[i]);
		i++;
	}
	free(args.forks);
	free(threads);
	free(philos);
	return (0);
}
