/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_helper_functions.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/16 08:16:58 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/16 08:16:58 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	gettime(void)
{
	struct timeval	crn_time;

	gettimeofday(&crn_time, NULL);
	return (crn_time.tv_sec * 1000) + (crn_time.tv_usec / 1000);
}

void			faksleep(unsigned int sleep_t)
{
	unsigned long start;

	start = gettime();
	while ((gettime() - start) < sleep_t)
	{
		usleep(100);
	}
}

int				ft_atoi(const char *str)
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
