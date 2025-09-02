/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:34:25 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/01 15:41:42 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stddef.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_cfg {
	int nphilo;
	int t_die;
	int t_eat;
	int t_sleep;
	int meals_target;
}	t_cfg;

int	main(int ac, char **av);
int parse_args(int ac, char **av, t_cfg *cfg);

#endif