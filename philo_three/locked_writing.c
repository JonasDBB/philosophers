/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   locked_writing.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/22 19:47:35 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/22 19:47:35 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static void		ft_putnbr(unsigned long n)
{
	char	c;

	if (n >= 10)
		ft_putnbr(n / 10);
	c = (char)(n % 10 + '0');
	write(1, &c, 1);
}

void			wr_lock(unsigned int philo_id, const char *str, t_arrgs *args)
{
	unsigned long	time;

	time = gettime() - args->start_t;
	sem_wait(args->write_sem);
	ft_putnbr(time);
	write(1, " : [", 4);
	ft_putnbr(philo_id);
	write(1, "] ", 2);
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
	sem_post(args->write_sem);
}
