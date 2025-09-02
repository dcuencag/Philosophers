/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:17:47 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 13:18:15 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	take_forks_internal(t_philo *p)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (p->cfg->nphilo == 1)
	{
		pthread_mutex_lock(p->left_fork);
		log_state(p->sim, p->id, "has taken a fork");
		while (!sim_get_end(p->sim))
			precise_usleep_ms(p->sim, 1);
		pthread_mutex_unlock(p->left_fork);
		return ;
	}
	if (p->id % 2 == 1)
	{
		first = p->left_fork;
		second = p->right_fork;
	}
	else
	{
		first = p->right_fork;
		second = p->left_fork;
	}
	pthread_mutex_lock(first);
	log_state(p->sim, p->id, "has taken a fork");
	pthread_mutex_lock(second);
	log_state(p->sim, p->id, "has taken a fork");
}

static void	put_forks_internal(t_philo *p)
{
	if (p->cfg->nphilo == 1)
		return ;
	if (p->id % 2 == 1)
	{
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
	}
	else
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
}

/* ---- API usada desde routine.c ---- */

void	action_think(t_philo *p)
{
	log_state(p->sim, p->id, "is thinking");
	if (p->cfg->nphilo > 1)
		precise_usleep_ms(p->sim, 1);
}

void	action_take_forks(t_philo *p)
{
	take_forks_internal(p);
}

void	action_eat(t_philo *p)
{
	pthread_mutex_lock(&p->m_meal);
	p->last_meal_ms = now_ms(p->sim);
	p->meals_eaten += 1;
	pthread_mutex_unlock(&p->m_meal);
	log_state(p->sim, p->id, "is eating");
	precise_usleep_ms(p->sim, p->cfg->t_eat);
}

void	action_put_forks(t_philo *p)
{
	put_forks_internal(p);
}

void	action_sleep(t_philo *p)
{
	log_state(p->sim, p->id, "is sleeping");
	precise_usleep_ms(p->sim, p->cfg->t_sleep);
}