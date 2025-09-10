/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dancuenc <dancuenc@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 13:18:36 by dancuenc          #+#    #+#             */
/*   Updated: 2025/09/10 13:18:37 by dancuenc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;
	int				simulation_running;
	long			start_time;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	death_mutex;
	pthread_t		*philosophers_threads;
	pthread_t		monitor_thread;
	t_philo			*philosophers;
}	t_data;

int		is_digit(char c);
int		ft_atoi(const char *str);

// Data initialization functions
int		init_data(t_data *data, char **av);
int		setup_philosophers_and_forks(t_data *data);
int		setup_philosophers_data(t_data *data);
int		start_simulation(t_data *data);
void	cleanup_all(t_data *data);
int		check_args(int ac, char **av);

// Utility functions
long	get_time(void);
void	ft_usleep(long time);
void	print_status(t_philo *philo, char *status);

// Philosopher functions
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
int		check_death(t_data *data);
int		all_philosophers_ate(t_data *data);

// Action functions
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);
void	init_data_values(t_data *data, char **av);
int		init_data_resources(t_data *data);
void	destroy_data_resources(t_data *data);

/****************para debug, comentar*********************/
/* void	print_final_stats(t_data *data); */
/*********************************************************/

#endif