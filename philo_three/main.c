/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jonasbb <jonasbb@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/26 15:50:42 by jonasbb       #+#    #+#                 */
/*   Updated: 2020/11/26 15:50:42 by jonasbb       ########   odam.nl         */
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

int			setup_malloc(t_arrgs *args, t_philo **philos, pid_t **pids)
{
	(*philos) = malloc(sizeof(t_philo) * (*args).n_philos);
	if (!(*philos))
		return (1);
	(*pids) = malloc(sizeof(pid_t) * (*args).n_philos);
	if (!(*pids))
	{
		free(*philos);
		return (1);
	}
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
	args->write_sem = sem_open("write", O_CREAT | O_EXCL, 0644, 1);
	if (args->write_sem == SEM_FAILED)
	{
		free(philos);
		return (1);
	}
	sem_unlink("write");
	return (0);
}

void		init_philos(t_arrgs *args, t_philo *philos)
{
	unsigned int	i;

	i = 0;
	while (i < (*args).n_philos)
	{
		philos[i].philo_nr = i;
		philos[i].args = args;
		philos[i].last_eaten_t = (*args).start_t;
		philos[i].times_eaten = 0;
		i++;
	}
}

static void	philo_eat(t_philo *philo, unsigned int id)
{
	sem_wait(philo->args->forks);
	wr_lock(id, "picked up first fork", philo->args);
	sem_wait(philo->args->forks);
	wr_lock(id, "picked up second fork", philo->args);
	sem_wait(philo->death_sem);
	philo->last_eaten_t = gettime();
	sem_post(philo->death_sem);
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
	sem_wait(philo->death_sem);
	if (philo->args->dead)
	{
		sem_post(philo->death_sem);
		return (1);
	}
	sem_post(philo->death_sem);
	return (0);
}

void	check_death(t_philo *philo)
{
	if (philo->times_eaten != philo->args->times_eat)
	{
		sem_post(philo->death_sem);
		wr_lock(philo->philo_nr, "has died", philo->args);
		sem_wait(philo->death_sem);
		philo->args->dead = true;
	}
}

int		monitor_philos(t_philo *philo)
{
	bool	stop;

	stop = false;
	sem_wait(philo->death_sem);
	if (gettime() - philo->last_eaten_t > philo->args->die_t)
	{
		check_death(philo);
//		sem_post(philo->death_sem);
		stop = true;
//		break ;
	}
	sem_post(philo->death_sem);
	usleep(500);
	if (stop)
		return (1);
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

#define infect SIGKILL
void		corona(pid_t *pids, unsigned int i)
{
	while (i)
	{
		kill(pids[i], infect);
		i--;
	}
	exit(1);
}

void		start_philo(t_philo *philo)
{
	pthread_t loop;

	philo->death_sem = sem_open("ded", O_CREAT, 0644, 1);
	if (philo->death_sem == SEM_FAILED)
		exit (1);
	sem_unlink("ded");
	if (pthread_create(&loop, NULL, philo_loop, philo))
		exit (1);

}

int		main(int ac, char **av)
{
	t_arrgs			args;
	t_philo			*philos;
	pid_t			*pids;

	if (check_and_set_input(ac, av, &args))
		return (1);
	if (setup_malloc(&args, &philos, &pids))
		return (1);
	if (create_sems(&args, philos))
		return (1);
	init_philos(&args, philos);
	unsigned int i = 0;
	while (i < args.n_philos)
	{
		pid_t pd;
		pd = fork();
		if (pd < 0)
			exit(1);
		if (pd == 0)
			start_philo(&philos[i]);//philoloop
		else
			pids[i] = pd;
		usleep(10);
		i++;
	}
	int status;
	i = 0;
	while (i < args.n_philos)
	{
		waitpid(-1, &status, WUNTRACED);
		if (WEXITSTATUS(status) != 0)
		{
			sem_wait(args.write_sem);
			corona(pids, args.n_philos);
		}
		i++;
	}
	// done eating
	exit (0);
}
