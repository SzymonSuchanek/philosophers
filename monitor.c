/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:56 by ssuchane          #+#    #+#             */
/*   Updated: 2024/11/03 18:31:24 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_monitor(t_thread *philo, long tt_die)
{
	int	should_monitor;

	pthread_mutex_lock(&philo->cycles_mutex);
	should_monitor = !philo->completed;
	pthread_mutex_unlock(&philo->cycles_mutex);
	return (should_monitor);
}

int	all_philosophers_done(t_data *data)
{
	int	all_done;

	pthread_mutex_lock(&data->completed_mutex);
	all_done = (data->completed_threads == data->total_threads);
	pthread_mutex_unlock(&data->completed_mutex);
	return (all_done);
}

void	mark_philosopher_completed(t_thread *philo)
{
	pthread_mutex_lock(&philo->cycles_mutex);
	if (!philo->completed)
	{
		philo->completed = 1;
		pthread_mutex_lock(&philo->data->completed_mutex);
		philo->data->completed_threads++;
		pthread_mutex_unlock(&philo->data->completed_mutex);
	}
	pthread_mutex_unlock(&philo->cycles_mutex);
}

void	check_philosopher(t_thread *philo)
{
	long	current_time;

	current_time = get_time_in_ms();
	if (get_cycles(philo) == 0)
		mark_philosopher_completed(philo);
	else
		check_if_starved(philo, current_time);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	ft_usleep(50);
	i = 0;
	while (1)
	{
		if (should_monitor(&data->philo[i], data->tt_die))
			check_philosopher(&data->philo[i]);
		if (is_philo_dead(data) || all_philosophers_done(data))
			return (NULL);
		i = (i + 1) % data->total_threads;
		ft_usleep(3);
	}
	return (NULL);
}
