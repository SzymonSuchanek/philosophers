/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:00:42 by marvin            #+#    #+#             */
/*   Updated: 2024/10/23 21:23:44 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_thread(pthread_t *thread, void *(*start_routine)(void *), void *arg)
{
	if (pthread_create(thread, NULL, start_routine, arg) != 0)
		ft_error("Thread creation failed");
	return (0);
}

void	init_mutex_or_exit(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
		ft_error("Mutex initialization failed\n");
}

void	destroy_data(t_data *data)
{
	int	i;

	i = -1;
	while (i++, i < data->total_threads)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philo[i].last_meal_mutex);
		pthread_mutex_destroy(&data->philo[i].cycles_mutex);
		pthread_mutex_destroy(&data->philo[i].is_dead_mutex);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->is_dead_mutex);
	free(data->forks);
	free(data->philo);
	free(data);
}

int	main(int ac, char **av)
{
	t_data		*data;
	pthread_t	*monitor_thread;

	data = malloc(sizeof(t_data));
	if (!data)
		ft_error("Malloc failed\n");
	if (!(ac >= 5 && ac <= 6))
		ft_error("Invalid number of arguments.\n");
	else if (validate_input(ac, av))
		return (0);
	data->total_threads = ft_atoi(av[1]);
	init_mutex(data);
	init_data(data, av);
	init_threads(data);
	destroy_data(data);
	return (0);
}
