/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:25:00 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/16 19:15:23 by ssuchane         ###   ########.fr       */
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
	pthread_t		monitor_thread;
	pthread_mutex_t	print_mutex;
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
}					t_thread;

#endif