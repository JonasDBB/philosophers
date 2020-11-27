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

void	start_philo(t_philo *philo)
{
	pthread_t loop;

	philo->death_sem = sem_open("ded", O_CREAT, 0644, 1);
	if (philo->death_sem == SEM_FAILED)
		exit(1);
	sem_unlink("ded");
	if (pthread_create(&loop, NULL, philo_loop, philo))
		exit(1);
	while (!monitor_philos(philo))
		continue;
	pthread_join(loop, NULL);
	exit(0);
}

void	corona(pid_t *pids, unsigned int i)
{
	while (i)
	{
		i--;
		kill(pids[i], INFECT);
	}
	exit(1);
}

void	start_processes(t_arrgs *args, t_philo *philos, pid_t *pids)
{
	unsigned int	i;
	pid_t			pd;

	i = 0;
	while (i < (*args).n_philos)
	{
		pd = fork();
		if (pd < 0)
			exit(1);
		if (pd == 0)
			start_philo(&philos[i]);
		else
			pids[i] = pd;
		usleep(10);
		i++;
	}
}

void	wait_for_death(t_arrgs *args, pid_t *pids)
{
	int				status;
	unsigned int	i;

	i = 0;
	while (i < (*args).n_philos)
	{
		waitpid(-1, &status, WUNTRACED);
		if (WEXITSTATUS(status) != 0)
			corona(pids, (*args).n_philos);
		i++;
	}
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
	start_processes(&args, philos, pids);
	wait_for_death(&args, pids);
	exit(0);
}
