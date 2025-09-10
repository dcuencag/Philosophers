/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:16:01 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/10 13:16:04 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_data *data)
{
	int		i;
	long	current_time;

	i = 0;
	while (i < data->num_philosophers)
	{
		current_time = get_time();
		pthread_mutex_lock(&data->philosophers[i].meal_mutex);
		if (current_time - data->philosophers[i].last_meal_time
			> data->time_to_die)
		{
			pthread_mutex_unlock(&data->philosophers[i].meal_mutex);
			pthread_mutex_lock(&data->death_mutex);
			data->simulation_running = 0;
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%ld %d died\n", current_time - data->start_time,
				data->philosophers[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->philosophers[i].meal_mutex);
		i++;
	}
	return (0);
}

int	all_philosophers_ate(t_data *data)
{
	int	i;
	int	finished_eating;

	if (data->times_must_eat == -1)
		return (0);
	finished_eating = 0;
	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_lock(&data->philosophers[i].meal_mutex);
		if (data->philosophers[i].meals_eaten >= data->times_must_eat)
			finished_eating++;
		pthread_mutex_unlock(&data->philosophers[i].meal_mutex);
		i++;
	}
	if (finished_eating == data->num_philosophers)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->simulation_running = 0;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_death(data) || all_philosophers_ate(data))
			break ;
		ft_usleep(1);
	}
	return (NULL);
}
