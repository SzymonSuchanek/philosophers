/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:25:00 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/23 21:24:05 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				total_threads;
	pthread_mutex_t	*forks;
	struct s_thread	*philo;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	long			start_routine;
	int				start;
	pthread_mutex_t	start_routine_mutex;
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	is_dead_mutex;
	int				is_dead;
	int				cycles;
}					t_data;

typedef struct s_thread
{
	pthread_t		thread;
	int				id;
	int				cycles;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	long			last_meal;
	struct s_data	*data;
	int				is_dead;
	pthread_mutex_t	is_dead_mutex;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	cycles_mutex;
}					t_thread;

// init.c
void				init_mutex_or_exit(pthread_mutex_t *mutex);
void				init_philos(t_thread *philo, t_data *data, int id,
						int total_threads);
void				init_mutex(t_data *data);
void				init_data(t_data *data, char **av);
int					init_threads(t_data *data);

// monitor.c
void				print_death_message(t_data *data, long current_time,
						int philo_id);
int					has_philosopher_died(t_thread *philo, long current_time,
						long tt_die);
void				monitor(t_thread *philo, long current_time, long tt_die);
void				*monitor_routine(void *arg);

// philo.c
int					create_thread(pthread_t *thread,
						void *(*start_routine)(void *), void *arg);
void				init_mutex_or_exit(pthread_mutex_t *mutex);

// protect_data.c
int					get_cycles(t_thread *philo);
void				update_cycles(t_thread *philo);
int					is_philo_dead(t_data *data);
void				set_philo_dead(t_data *data);

// routine.c
void				handle_single_philosopher(t_thread *philo, long start_time);
void				take_forks(t_thread *philo, long start_time);
void				eat(t_thread *philo, long start_time);
void				sleep_and_think(t_thread *philo, long start_time);
void				*routine(void *arg);

// utils.c
long				get_time_in_ms(void);
void				ft_usleep(long ms);
long				get_last_meal(t_thread *philo);
void				update_last_meal(t_thread *philo, long time);

// validate_input.c
int					ft_error(char *message);
int					validate_input(int ac, char **av);
int					ft_atoi(char *s);

#endif