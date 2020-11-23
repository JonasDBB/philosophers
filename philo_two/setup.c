/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   setup.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/22 19:31:21 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/22 19:31:21 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	setargs(int ac, char **av, t_arrgs *args)
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

int			check_and_set_input(int ac, char **av, t_arrgs *args)
{
	if (ac != 5 && ac != 6)
	{
		write(2, "Error: incorrect args\n", 27);
		return (1);
	}
	setargs(ac, av, &(*args));
	if (!args->die_t || !args->eat_t || !args->sleep_t || !args->times_eat)
	{
		write(2, "Error: please enter positive values\n", 36);
		return (1);
	}
	if (args->n_philos < 2)
	{
		write(2, "Error: please enter more than one philosopher\n", 46);
		return (1);
	}
	(*args).dead = false;
	return (0);
}

int			setup_malloc(t_arrgs *args, t_philo **philos)
{
	(*philos) = malloc(sizeof(t_philo) * (*args).n_philos);
	if (!(*philos))
		return (1);
	return (0);
}

int			create_sems(t_arrgs *args, t_philo *philos)
{
	args->forks = sem_open("forks", O_CREAT | O_EXCL, 0644, args->n_philos);
	if (args->forks == SEM_FAILED)
	{
		free(philos);
		return (1);
	}
	sem_unlink("forks");
	args->death_sem = sem_open("death", O_CREAT | O_EXCL, 0644, 1);
	if (args->death_sem == SEM_FAILED)
	{
		free(philos);
		return (1);
	}
	sem_unlink("death");
	args->write_sem = sem_open("write", O_CREAT | O_EXCL, 0644, 1);
	if (args->write_sem == SEM_FAILED)
	{
		free(philos);
		return (1);
	}
	sem_unlink("write");
	return (0);
}

int			init_philos(t_arrgs *args, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < (*args).n_philos)
	{
		philos[i].philo_nr = i;
		philos[i].args = args;
		philos[i].last_eaten_t = (*args).start_t;
		philos[i].times_eaten = 0;
		philos[i].t_check_sem = sem_open("t_check", O_CREAT, 0644, 1);
		if (philos[i].t_check_sem == SEM_FAILED)
		{
			destroy_time_sems(philos, i);
			return (1);
		}
		sem_unlink("time_check");
		i++;
	}
	return (0);
}
