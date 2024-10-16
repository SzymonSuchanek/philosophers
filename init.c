/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:23:56 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/18 20:33:54 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_mutex_or_exit(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
		ft_error("Mutex initialization failed\n");
}

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
	philo->last_meal = get_time_in_ms();
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

void	init_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i, i < data->total_threads)
	{
		if (pthread_create(&data->philo[i].thread, NULL, &routine,
				&data->philo[i]) != 0)
		{
			perror("Thread creating failed.");
			exit(EXIT_FAILURE);
		}
	}
	if (pthread_create(&data->monitor_thread, NULL, &monitor_routine,
			data) != 0)
	{
		perror("Monitor thread creating failed.");
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i, i < data->total_threads)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(data->monitor_thread, NULL);
}
