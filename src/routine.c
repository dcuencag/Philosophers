/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:00:26 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 13:22:59 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
/* 
static void	take_forks(t_philo *p)
{
	// Estrategia: impar coge izquierda->derecha, par derecha->izquierda
	pthread_mutex_t *first;
	pthread_mutex_t *second;

	if (p->cfg->nphilo == 1)
	{
		pthread_mutex_lock(p->left_fork);
		log_state(p->sim, p->id, "has taken a fork");
		// No existe segundo tenedor. Espera hasta morir (monitor cortará).
		// Dormimos un poco para no quemar CPU:
		while (!sim_get_end(p->sim))
			precise_usleep_ms(p->sim, 1);
		pthread_mutex_unlock(p->left_fork);
		return;
	}
	if (p->id % 2 == 1) {
		first = p->left_fork;
		second = p->right_fork;
	} else {
		first = p->right_fork;
		second = p->left_fork;
	}

	pthread_mutex_lock(first);
	log_state(p->sim, p->id, "has taken a fork");
	pthread_mutex_lock(second);
	log_state(p->sim, p->id, "has taken a fork");
}

static void	put_forks(t_philo *p)
{
	// Coincide con el orden de take_forks
	if (p->cfg->nphilo == 1)
		return;
	if (p->id % 2 == 1) {
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
	} else {
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
}

static void	do_eat(t_philo *p)
{
	pthread_mutex_lock(&p->m_meal);
	p->last_meal_ms = now_ms(p->sim);
	p->meals_eaten += 1;
	pthread_mutex_unlock(&p->m_meal);

	log_state(p->sim, p->id, "is eating");
	precise_usleep_ms(p->sim, p->cfg->t_eat);
}

static void	do_sleep(t_philo *p)
{
	log_state(p->sim, p->id, "is sleeping");
	precise_usleep_ms(p->sim, p->cfg->t_sleep);
}

static void	do_think(t_philo *p)
{
	log_state(p->sim, p->id, "is thinking");
	// Pequeña desincronización para reducir contención
	if (p->cfg->nphilo > 1)
		precise_usleep_ms(p->sim, 1);
}
 */
void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 == 0)
		precise_usleep_ms(p->sim, p->cfg->t_eat / 2);
	while (!sim_get_end(p->sim))
	{
		action_think(p);
		if (sim_get_end(p->sim))
			break ;
		action_take_forks(p);
		if (p->cfg->nphilo == 1)
			break ;
		if (sim_get_end(p->sim))
		{
			action_put_forks(p);
			break ;
		}
		action_eat(p);
		action_put_forks(p);
		if (sim_get_end(p->sim))
			break ;
		action_sleep(p);
	}
	return (NULL);
}
