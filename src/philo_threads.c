/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:16:13 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/10 13:16:14 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks_mutex[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks_mutex[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_philo_data(t_data *data, int i)
{
	data->philosophers[i].id = i + 1;
	data->philosophers[i].meals_eaten = 0;
	data->philosophers[i].last_meal_time = data->start_time;
	data->philosophers[i].left_fork = &data->forks_mutex[i];
	data->philosophers[i].right_fork = &data->forks_mutex[(i + 1)
		% data->num_philosophers];
	data->philosophers[i].data = data;
	if (pthread_mutex_init(&data->philosophers[i].meal_mutex, NULL))
	{
		while (--i >= 0)
			pthread_mutex_destroy(&data->philosophers[i].meal_mutex);
		return (1);
	}
	return (0);
}

static int	allocate_memory(t_data *data)
{
	data->philosophers = malloc(sizeof(t_philo) * data->num_philosophers);
	if (!data->philosophers)
		return (1);
	data->philosophers_threads = malloc(sizeof(pthread_t)
			* data->num_philosophers);
	if (!data->philosophers_threads)
	{
		free(data->philosophers);
		return (1);
	}
	return (0);
}

int	setup_philosophers_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (init_philo_data(data, i))
			return (1);
		i++;
	}
	return (0);
}

int	setup_philosophers_and_forks(t_data *data)
{
	if (allocate_memory(data))
		return (1);
	if (init_forks(data))
	{
		free(data->philosophers);
		free(data->philosophers_threads);
		return (1);
	}
	if (setup_philosophers_data(data))
		return (1);
	return (0);
}
