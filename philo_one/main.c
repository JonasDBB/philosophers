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
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "philosophers.h"

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
}

void	print_time()
{
	struct timeval	crn_time;
	struct timezone	tmzone;
	int				hour;
	int				min;
	int				sec;

	gettimeofday(&crn_time, &tmzone); // timezone does nothing on linux
	long hms = crn_time.tv_sec % 86400;
	hms += tmzone.tz_dsttime * 3600;
	hms -= tmzone.tz_minuteswest * 60;
	hms = (hms + 86400) % 86400;
	hour = (int)hms / 3600;
	min = (int)(hms % 3600) / 60;
	sec = (int)(hms % 3600) % 60;
	printf("time is %i:%i:%i\n", hour, min, sec);
}

int 	check_and_set_input(int ac, char **av, t_arrgs *args)
{
	if (ac != 5 && ac != 6)
	{
		write(2, "Error: incorrect args\n", 27);
		return (1);
	}
	setargs(ac, av, &(*args));
	if (args->die_t <= 0 || args->eat_t <= 0 || args->sleep_t <= 0)
	{
		write(2, "Error: please enter positive time values\n", 41);
		return (1);
	}
	if (args->n_philos <= 0)
	{
		write(2, "Error: please enter positive number of philosophers\n", 52);
		return (1);
	}
	return (0);
}

void	*eat(void *args)
{
	t_arrgs *argum = (t_arrgs*)args;
	pthread_mutex_lock(&argum->forks[argum->philo_nr]);
	pthread_mutex_lock(&argum->forks[(argum->philo_nr + 1) % argum->n_philos]);
	printf("locked fork %d and %d on philo %d\n", argum->philo_nr, (argum->philo_nr + 1) % argum->n_philos, argum->philo_nr);
	printf("eating for %d\n", argum->eat_t);
	usleep(argum->eat_t);
	printf("done eating\n");
	pthread_mutex_unlock(&argum->forks[argum->philo_nr]);
	pthread_mutex_unlock(&argum->forks[(argum->philo_nr + 1) % argum->n_philos]);
	return (NULL);
}

int		main(int ac, char **av)
{
	t_arrgs	args;

	if (check_and_set_input(ac, av, &args))
		return (1);
	print_time();
	//	printf("n = %i\ndie_t = %i\neat_t = %i\nsleep_t = %i\ntimes_eat = %i\n", args.n_philos, args.die_t, args.eat_t, args.sleep_t, args.times_eat);

	pthread_t		philos[args.n_philos];
	args.forks = malloc(sizeof(pthread_mutex_t) * args.n_philos);
	int i = 0;
	while (i < args.n_philos)
	{
		pthread_mutex_init(&args.forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < args.n_philos)
	{
		args.philo_nr = i;
		pthread_create(&philos[i], NULL, eat, &args);
		i++;
	}
	i = 0;
	while (i < args.n_philos)
	{
		pthread_join(philos[i], NULL);
		i++;
	}
	i = 0;
	while (i < args.n_philos)
	{
		pthread_mutex_destroy(&args.forks[i]);
		i++;
	}
	return (0);
}
