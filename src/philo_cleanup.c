/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:41:31 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/10 13:15:37 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_philosophers(t_data *data)
{
	int	i;

	if (data->philosophers)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			pthread_mutex_destroy(&data->philosophers[i].meal_mutex);
			i++;
		}
		free(data->philosophers);
		data->philosophers = NULL;
	}
}

static void	cleanup_forks(t_data *data)
{
	int	i;

	if (data->forks_mutex)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			pthread_mutex_destroy(&data->forks_mutex[i]);
			i++;
		}
		free(data->forks_mutex);
		data->forks_mutex = NULL;
	}
}

void	cleanup_all(t_data *data)
{
	if (data->philosophers)
		cleanup_philosophers(data);
	if (data->philosophers_threads)
	{
		free(data->philosophers_threads);
		data->philosophers_threads = NULL;
	}
	if (data->forks_mutex)
		cleanup_forks(data);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

/*************para debug, comentar***********/
/* void	print_final_stats(t_data *data)
{
	int	i;

	printf("\n=== FINAL STATS ===\n");
	i = 0;
	while (i < data->num_philosophers)
	{
		printf("Philo %d ate %d times\n",
			data->philosophers[i].id,
			data->philosophers[i].meals_eaten);
		i++;
	}
} */
/*******************************************/