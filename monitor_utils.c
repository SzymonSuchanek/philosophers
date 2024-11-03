/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:26:56 by ssuchane          #+#    #+#             */
/*   Updated: 2024/11/03 18:06:49 by ssuchane         ###   ########.fr       */
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

void	check_if_starved(t_thread *philo, long current_time)
{
	long	last_meal_time;
	long	elapsed_time;

	pthread_mutex_lock(&philo->last_meal_mutex);
	last_meal_time = philo->last_meal;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	if (current_time - last_meal_time <= philo->data->tt_die)
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_philo_dead(philo->data))
	{
		pthread_mutex_lock(&philo->data->start_routine_mutex);
		elapsed_time = get_time_in_ms() - philo->data->start_routine;
		pthread_mutex_unlock(&philo->data->start_routine_mutex);
		printf("%ld %i died\n", elapsed_time, philo->id);
		set_philo_dead(philo->data);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}
