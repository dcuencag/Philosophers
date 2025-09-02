/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:56:57 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 13:54:38 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long long	walltime_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL);
}

long long	now_ms(t_sim *sim)
{
	return (walltime_ms() - sim->t0_ms);
}

int	sim_get_end(t_sim *sim)
{
	int v;

	pthread_mutex_lock(&sim->m_end);
	v = sim->end;
	pthread_mutex_unlock(&sim->m_end);
	return (v);
}

void	sim_set_end(t_sim *sim, int v)
{
	pthread_mutex_lock(&sim->m_end);
	sim->end = v;
	pthread_mutex_unlock(&sim->m_end);
}
