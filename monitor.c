/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:56 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/16 19:27:25 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_death_message(t_data *data, long current_time, int philo_id)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!data->is_dead)
	{
		data->is_dead = 1;
		printf("%ld %i died\n", current_time, philo_id);
	}
	pthread_mutex_unlock(&data->print_mutex);
}

int	has_philosopher_died(t_thread *philo, long current_time, long tt_die)
{
	return (current_time - philo->last_meal > tt_die);
}

void	*monitor(void *arg)
{
	t_data	*data;
	long	current_time;
	int		i;

	data = (t_data *)arg;
	while (!data->is_dead)
	{
		i = 0;
		while (i < data->total_threads)
		{
			current_time = get_time_in_ms();
			if (has_philosopher_died(&data->philo[i], current_time,
					data->tt_die))
			{
				print_death_message(data, current_time
					- data->philo[i].last_meal, data->philo[i].id);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
