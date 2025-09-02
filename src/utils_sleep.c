/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:19:12 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 13:53:35 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void precise_usleep_ms(t_sim *sim, int ms)
{
    long long start = walltime_ms();
    while (!sim_get_end(sim))
    {
        if (walltime_ms() - start >= (long long)ms)
            break;
        // Reduce CPU usage with longer sleep
        usleep(500); // Increased from 200
    }
}