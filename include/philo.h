/* ************************************************************************** */
/*						                                                    */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:34:25 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/02 12:54:18 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_cfg
{
	int	nphilo;
	int	t_die;
	int	t_eat;
	int	t_sleep;
	int	meals_target;
}	t_cfg;

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long long		last_meal_ms;
	int				meals_eaten;
	pthread_mutex_t	m_meal;
	t_cfg			*cfg;
	t_sim			*sim;
}	t_philo;

struct s_sim
{
	long long		t0_ms;
	volatile int	end;
	pthread_mutex_t	m_end;
	pthread_mutex_t	m_print;
	pthread_mutex_t	*forks;
	t_philo			*ph;
	int				n;
};

int			main(int ac, char **av);
int			parse_args(int ac, char **av, t_cfg *cfg);
int			sim_init(t_sim *sim, t_cfg *cfg);
void		sim_destroy(t_sim *sim);
void		*philo_routine(void *arg);
void		*monitor_routine(void *arg);
long long	walltime_ms(void);
long long	now_ms(t_sim *sim);
void		precise_usleep_ms(t_sim *sim, int ms);
int			sim_get_end(t_sim *sim);
void		sim_set_end(t_sim *sim, int v);
void		log_state(t_sim *sim, int id, const char *msg);
void		log_death(t_sim *sim, int id);
void		action_think(t_philo *p);
void		action_take_forks(t_philo *p);
void		action_eat(t_philo *p);
void		action_put_forks(t_philo *p);
void		action_sleep(t_philo *p);

#endif