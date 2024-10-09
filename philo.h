/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:25:00 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/09 18:32:06 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_data
{
	int				total_threads;
	pthread_t		*threads;
	struct s_thread	**philo;
}					t_data;

typedef struct s_thread
{
	int				id;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				cycles;
	fork_left;
	fort_right;
}					t_thread;

#endif