/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/16 08:16:58 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/16 08:16:58 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_putnbr(unsigned long n)
{
	char	c;

	if (n >= 10)
		ft_putnbr(n / 10);
	c = (char)(n % 10 + '0');
	write(1, &c, 1);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	write_lock(unsigned int philo_id, const char *str, t_arrgs *args)
{
	unsigned long	time;

//	pthread_mutex_lock(&g_dead_lock);
	if (g_dead)
		return ;
//	pthread_mutex_unlock(&g_dead_lock);
	time = gettime() - args->start_t;
	pthread_mutex_lock(&args->write_lock);
	ft_putnbr(time);
	write(1, " : [", 4);
	ft_putnbr(philo_id);
	write(1, "] ", 2);
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
	pthread_mutex_unlock(&args->write_lock);
}

int		ft_atoi(const char *str)
{
	long	result;
	int		sign;
	long	max;

	max = 9223372036854775807;
	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\r'
		   || *str == '\n' || *str == '\v' || *str == '\f')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (result > (max / 10))
			return (sign > 0) ? (-1) : (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	return ((int)(result * sign));
}
