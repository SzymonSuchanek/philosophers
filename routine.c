/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:19 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/16 19:26:41 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_single_philosopher(t_thread *philo, long start_time)
{
	pthread_mutex_lock(philo->fork_left);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %i has taken a fork\n", get_time_in_ms() - start_time,
		philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	ft_usleep(philo->data->tt_die);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %i has died\n", get_time_in_ms() - start_time, philo->id);
	philo->data->is_dead = 1;
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(philo->fork_left);
}

void	take_forks(t_thread *philo, long start_time)
{
	pthread_mutex_lock(philo->fork_left);
	pthread_mutex_lock(philo->fork_right);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->is_dead)
	{
		printf("%ld %i has taken a fork\n", get_time_in_ms() - start_time,
			philo->id);
		printf("%ld %i has taken a fork\n", get_time_in_ms() - start_time,
			philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	eat(t_thread *philo, long start_time)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->is_dead)
	{
		printf("%ld %i is eating\n", get_time_in_ms() - start_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	philo->last_meal = get_time_in_ms();
	ft_usleep(philo->data->tt_eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}

void	sleep_and_think(t_thread *philo, long start_time)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->is_dead)
		printf("%ld %i is sleeping\n", get_time_in_ms() - start_time,
			philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	ft_usleep(philo->data->tt_sleep);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->is_dead)
		printf("%ld %i is thinking\n", get_time_in_ms() - start_time,
			philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*routine(void *arg)
{
	t_thread	*philo;
	long		start_time;

	philo = (t_thread *)arg;
	start_time = get_time_in_ms();
	philo->last_meal = start_time;
	if (philo->data->total_threads == 1)
	{
		handle_single_philosopher(philo, start_time);
		return (NULL);
	}
	while (philo->cycles != 0 && !philo->data->is_dead)
	{
		take_forks(philo, start_time);
		eat(philo, start_time);
		sleep_and_think(philo, start_time);
		philo->cycles--;
		usleep(10);
	}
	return (NULL);
}
