/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:34:07 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/03 17:05:07 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	main(int ac, char **av)
{
	t_cfg		cfg;
	t_sim		sim;
	pthread_t	monitor;
	int			i;

	if (!parse_args(ac, av, &cfg))
		return (printf("Error\n"), 1);
	if (!sim_init(&sim, &cfg))
		return (printf("Error\n"), 1);
	i = 0;
	while (i < cfg.nphilo)
	{
		if (pthread_create(&sim.ph[i].thread, NULL, philo_routine, &sim.ph[i]) != 0)
		{
			printf("Error\n");
			sim_set_end(&sim, 1);
			sim_destroy(&sim);
			return (1);
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, &sim) != 0)
	{
		printf("Error\n");
		sim_set_end(&sim, 1);
		sim_destroy(&sim);
		return (1);
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < cfg.nphilo)
	{
		pthread_join(sim.ph[i].thread, NULL);
		i++;
	}
	sim_destroy(&sim);
	return (0);
}
