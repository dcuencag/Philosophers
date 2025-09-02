/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:59:36 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 12:59:49 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	sim_should_print(t_sim *sim)
{
	int ended;

	pthread_mutex_lock(&sim->m_end);
	ended = sim->end;
	pthread_mutex_unlock(&sim->m_end);
	return (!ended);
}

void	log_state(t_sim *sim, int id, const char *msg)
{
	long long ts;

	pthread_mutex_lock(&sim->m_print);
	if (!sim_should_print(sim)) {
		pthread_mutex_unlock(&sim->m_print);
		return;
	}
	ts = now_ms(sim);
	printf("%lld %d %s\n", ts, id, msg);
	pthread_mutex_unlock(&sim->m_print);
}

/* Mensaje de muerte: siempre debe salir (aunque end esté marcado). */
void	log_death(t_sim *sim, int id)
{
	long long ts;

	pthread_mutex_lock(&sim->m_print);
	ts = now_ms(sim);
	printf("%lld %d died\n", ts, id);
	pthread_mutex_unlock(&sim->m_print);
}
