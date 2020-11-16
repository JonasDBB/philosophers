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

int		main(int ac, char **av)
{
	t_arrgs	args;

	if (ac != 5 && ac != 6)
	{
		write(2, "Error: incorrect arguments\n", 27);
		return (1);
	}
	setargs(ac, av, &args);
	struct timeval
	gettimeofday()
//	printf("n = %i\ndie_t = %i\neat_t = %i\nsleep_t = %i\ntimes_eat = %i\n", args.n_philos, args.die_t, args.eat_t, args.sleep_t, args.times_eat);


	return (0);
}
