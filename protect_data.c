/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protect_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 21:21:04 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/23 19:56:52 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_cycles(t_thread *philo)
{
	int	cycles;

	pthread_mutex_lock(&philo->cycles_mutex);
	cycles = philo->cycles;
	pthread_mutex_unlock(&philo->cycles_mutex);
	return (cycles);
}

void	update_cycles(t_thread *philo)
{
	pthread_mutex_lock(&philo->cycles_mutex);
	philo->cycles--;
	pthread_mutex_unlock(&philo->cycles_mutex);
}

int	is_philo_dead(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->is_dead_mutex);
	dead = data->is_dead;
	pthread_mutex_unlock(&data->is_dead_mutex);
	return (dead);
}

void	set_philo_dead(t_data *data)
{
	pthread_mutex_lock(&data->philo->is_dead_mutex);
	data->philo->is_dead = 1;
	pthread_mutex_unlock(&data->philo->is_dead_mutex);
	pthread_mutex_lock(&data->is_dead_mutex);
	data->is_dead = 1;
	pthread_mutex_unlock(&data->is_dead_mutex);
}
