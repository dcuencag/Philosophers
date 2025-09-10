/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:15:02 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/10 13:15:03 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static int	print_arg_error(int type, char *arg)
{
	if (type == 0)
	{
		printf("Error: Incorrect number of arguments.\n");
		printf("Try: ./philo number_of_philosophers time_to_die time_to_eat "
			"time_to_sleep [times_must_eat]\n");
	}
	else if (type == 1)
		printf("Error: Argument '%s' is not a valid number.\n", arg);
	else if (type == 2)
		printf("Error: philo_num must be between 1 and 200.\n");
	else if (type == 3)
		printf("Error: Argument '%s' must be greater than 0.\n", arg);
	return (1);
}

static int	check_numeric_args(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (!is_digit(av[i][j]))
				return (print_arg_error(1, av[i]));
			j++;
		}
		i++;
	}
	return (0);
}

static int	check_range_args(char **av)
{
	int	philo_num;
	int	i;

	philo_num = ft_atoi(av[1]);
	if (philo_num < 1 || philo_num > 200)
		return (print_arg_error(2, NULL));
	i = 2;
	while (i < 5)
	{
		if (ft_atoi(av[i]) <= 0)
			return (print_arg_error(3, av[i]));
		i++;
	}
	return (0);
}

int	check_args(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (print_arg_error(0, NULL));
	if (check_numeric_args(ac, av))
		return (1);
	if (check_range_args(av))
		return (1);
	return (0);
}
