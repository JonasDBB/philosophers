/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jbennink <jbennink@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/16 08:36:45 by jbennink      #+#    #+#                 */
/*   Updated: 2020/11/16 08:36:45 by jbennink      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

typedef struct	s_arrgs {
	int	n_philos;
	int	die_t;
	int	eat_t;
	int	sleep_t;
	int	times_eat;
}				t_arrgs;

int		ft_atoi(const char *str);

#endif
