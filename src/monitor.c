/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:02:22 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 13:53:56 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int all_reached_target(t_sim *sim)
{
    int target = sim->ph[0].cfg->meals_target;
    if (target < 0)
        return (0);
    
    int count = 0;
    for (int i = 0; i < sim->n; i++)
    {
        pthread_mutex_lock(&sim->ph[i].m_meal);
        int eaten = sim->ph[i].meals_eaten;
        pthread_mutex_unlock(&sim->ph[i].m_meal);
        if (eaten >= target)
            count++;
    }
    return (count == sim->n);
}

void	*monitor_routine(void *arg)
{
	t_sim		*sim;
	int			i;
	long long	now;
	long long	since_last;
	int			t_die;

	sim = (t_sim *)arg;
	while (!sim_get_end(sim))
	{
		i = 0;
		while (i < sim->n && !sim_get_end(sim))
		{
			pthread_mutex_lock(&sim->ph[i].m_meal);
			now = now_ms(sim);
			since_last = now - sim->ph[i].last_meal_ms;
			t_die = sim->ph[i].cfg->t_die;
			pthread_mutex_unlock(&sim->ph[i].m_meal);
			if (since_last >= (long long)t_die)
			{
				log_death(sim, sim->ph[i].id);
				sim_set_end(sim, 1);
				return (NULL);
			}
			i++;
		}
		if (all_reached_target(sim))
		{
			sim_set_end(sim, 1);
			return (NULL);
		}
		usleep(200);
	}
	return (NULL);
}
