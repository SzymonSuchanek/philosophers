/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:56 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/17 21:58:31 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_death_message(t_data *data, long current_time, int philo_id)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!is_philo_dead(data))
	{
		set_philo_dead(data);
		printf("%ld %i died\n", current_time, philo_id);
	}
	pthread_mutex_unlock(&data->print_mutex);
}

int	has_philosopher_died(t_thread *philo, long current_time, long tt_die)
{
	long	last_meal;

	last_meal = get_last_meal(philo);
	return (current_time - last_meal > tt_die);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	long	current_time;
	long	tt_die;
	int		i;

	data = (t_data *)arg;
	current_time = get_time_in_ms();
	tt_die = data->tt_die;
	i = 0;
	while (i < data->total_threads)
	{
		monitor(&data->philo[i], current_time, tt_die);
		i++;
	}
	return (NULL);
}

void	monitor(t_thread *philo, long current_time, long tt_die)
{
	if (has_philosopher_died(philo, current_time, tt_die))
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		if (!is_philo_dead(philo->data))
		{
			printf("%ld %i has died\n", get_time_in_ms() - current_time,
				philo->id);
			set_philo_dead(philo->data);
		}
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
}
