/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:19 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/23 20:04:42 by ssuchane         ###   ########.fr       */
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
	if (is_philo_dead(philo->data))
	{
		pthread_mutex_unlock(philo->fork_left);
		return ;
	}
	ft_usleep(philo->data->tt_die);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_philo_dead(philo->data))
	{
		printf("%ld %i has died\n", get_time_in_ms() - start_time, philo->id);
		set_philo_dead(philo->data);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(philo->fork_left);
}

void	take_forks(t_thread *philo, long start_time)
{
	pthread_mutex_lock(philo->fork_left);
	pthread_mutex_lock(philo->fork_right);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_philo_dead(philo->data))
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
	if (!is_philo_dead(philo->data))
	{
		printf("%ld %i is eating\n", get_time_in_ms() - start_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	update_last_meal(philo, get_time_in_ms());
	ft_usleep(philo->data->tt_eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	update_cycles(philo);
}

void	sleep_and_think(t_thread *philo, long start_time)
{
	if (!is_philo_dead(philo->data))
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld %i is sleeping\n", get_time_in_ms() - start_time,
			philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	ft_usleep(philo->data->tt_sleep);
	if (!is_philo_dead(philo->data))
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld %i is thinking\n", get_time_in_ms() - start_time,
			philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}

void	*routine(void *arg)
{
	t_thread	*philo;
	long		start_time;

	philo = (t_thread *)arg;
	start_time = get_time_in_ms();
	pthread_mutex_lock(&philo->data->start_routine_mutex);
	philo->data->start_routine = start_time;
	pthread_mutex_unlock(&philo->data->start_routine_mutex);
	update_last_meal(philo, start_time);
	if (philo->data->total_threads == 1)
	{
		handle_single_philosopher(philo, start_time);
		return (NULL);
	}
	while (get_cycles(philo) != 0 && !is_philo_dead(philo->data))
	{
		if (philo->id % 2)
			ft_usleep(1);
		take_forks(philo, start_time);
		eat(philo, start_time);
		sleep_and_think(philo, start_time);
	}
	return (NULL);
}
