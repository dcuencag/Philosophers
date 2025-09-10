/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:37:46 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/10 13:15:27 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data_values(t_data *data, char **av)
{
	data->num_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->times_must_eat = -1;
	if (av[5])
		data->times_must_eat = ft_atoi(av[5]);
	data->simulation_running = 1;
	data->start_time = get_time();
}

int	init_data_resources(t_data *data)
{
	if (data->num_philosophers == 0 || data->times_must_eat == 0)
		return (1);
	if (pthread_mutex_init(&data->write_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->death_mutex, NULL))
	{
		pthread_mutex_destroy(&data->write_mutex);
		return (1);
	}
	data->forks_mutex = malloc(sizeof(pthread_mutex_t)
			* data->num_philosophers);
	if (!data->forks_mutex)
	{
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	destroy_data_resources(t_data *data)
{
	if (data->forks_mutex)
		free(data->forks_mutex);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}
