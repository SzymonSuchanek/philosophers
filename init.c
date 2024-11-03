/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:23:56 by ssuchane          #+#    #+#             */
/*   Updated: 2024/11/03 18:26:32 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_mutex(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->total_threads);
	if (!data->forks)
		ft_error("Malloc failed for forks\n");
	data->philo = malloc(sizeof(t_thread) * data->total_threads);
	if (!data->philo)
		ft_error("Malloc failed for philosophers\n");
	init_mutex_or_exit(&data->print_mutex);
	init_mutex_or_exit(&data->start_routine_mutex);
	init_mutex_or_exit(&data->is_dead_mutex);
	init_mutex_or_exit(&data->completed_mutex);
	i = -1;
	while (++i, i < data->total_threads)
	{
		init_mutex_or_exit(&data->forks[i]);
		init_mutex_or_exit(&data->philo[i].last_meal_mutex);
		init_mutex_or_exit(&data->philo[i].cycles_mutex);
		init_mutex_or_exit(&data->philo[i].is_dead_mutex);
	}
}

void	init_philos(t_thread *philo, t_data *data, int id, int total_threads)
{
	philo->id = id + 1;
	philo->data = data;
	philo->last_meal = data->start;
	philo->cycles = -1;
	if (data->total_threads > 0)
	{
		if (data->cycles != -1)
			philo->cycles = data->cycles;
	}
	if (philo->id % 2 == 0)
	{
		philo->fork_left = &data->forks[id];
		philo->fork_right = &data->forks[(id + 1) % total_threads];
	}
	else
	{
		philo->fork_right = &data->forks[id];
		philo->fork_left = &data->forks[(id + 1) % total_threads];
	}
}

void	init_data(t_data *data, char **av)
{
	int	cycles;
	int	i;
	int	start;

	data->start = get_time_in_ms();
	data->total_threads = ft_atoi(av[1]);
	data->tt_die = ft_atoi(av[2]);
	data->tt_eat = ft_atoi(av[3]);
	data->tt_sleep = ft_atoi(av[4]);
	data->is_dead = 0;
	if (av[5])
		data->cycles = ft_atoi(av[5]);
	else
		data->cycles = -1;
	i = -1;
	while (++i, i < data->total_threads)
		init_philos(&data->philo[i], data, i, data->total_threads);
}

int	init_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->total_threads)
		create_thread(&data->philo[i].thread, &routine, &data->philo[i]);
	if (data->total_threads > 1)
		create_thread(&data->monitor_thread, &monitor_routine, data);
	i = -1;
	while (++i < data->total_threads)
		pthread_join(data->philo[i].thread, NULL);
	if (data->total_threads > 1)
		pthread_join(data->monitor_thread, NULL);
	return (0);
}
