/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:34:07 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/01 15:42:08 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	main(int ac, char **av)
{
	t_cfg cfg;
	(void)av;
	
	if (!parse_args(ac, av, &cfg))
	{
		printf("Error\n");
		return (1);
	}
	return (0);
}
