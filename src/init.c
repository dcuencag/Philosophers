/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:56:35 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 13:54:11 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	forks_init(t_sim *sim, int n)
{
	int i;

	sim->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	if (!sim->forks)
		return (0);
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

static int	philos_init(t_sim *sim, t_cfg *cfg)
{
	int i;

	i = 0;
	while (i < cfg->nphilo)
	{
		sim->ph[i].id = i + 1;
		sim->ph[i].cfg = cfg;
		sim->ph[i].sim = sim;
		sim->ph[i].left_fork = &sim->forks[i];
		sim->ph[i].right_fork = &sim->forks[(i + 1) % cfg->nphilo];
		sim->ph[i].meals_eaten = 0;
		sim->ph[i].last_meal_ms = 0;
		if (pthread_mutex_init(&sim->ph[i].m_meal, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	sim_init(t_sim *sim, t_cfg *cfg)
{
	long long	t0;
	int			i;

	sim->n = cfg->nphilo;
	sim->t0_ms = walltime_ms();
	sim->end = 0;
	if (pthread_mutex_init(&sim->m_end, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->m_print, NULL) != 0)
		return (0);
	if (!forks_init(sim, cfg->nphilo))
		return (0);
	sim->ph = (t_philo *)malloc(sizeof(t_philo) * cfg->nphilo);
	if (!sim->ph)
		return (0);
	if (!philos_init(sim, cfg))
		return (0);
	/* init last_meal */
	t0 = now_ms(sim);
	i = 0;
	while (i < cfg->nphilo)
	{
		pthread_mutex_lock(&sim->ph[i].m_meal);
		sim->ph[i].last_meal_ms = t0;
		pthread_mutex_unlock(&sim->ph[i].m_meal);
		i++;
	}
	return (1);
}

void sim_destroy(t_sim *sim)
{
    if (!sim)
        return;
        
    if (sim->ph)
    {
        for (int i = 0; i < sim->n; i++)
        {
            pthread_mutex_destroy(&sim->ph[i].m_meal);
        }
        free(sim->ph);
    }
    
    if (sim->forks)
    {
        for (int i = 0; i < sim->n; i++)
        {
            pthread_mutex_destroy(&sim->forks[i]);
        }
        free(sim->forks);
    }
    
    pthread_mutex_destroy(&sim->m_print);
    pthread_mutex_destroy(&sim->m_end);
}
