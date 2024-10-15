/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:00:42 by marvin            #+#    #+#             */
/*   Updated: 2024/10/15 22:11:53 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_error(char *message)
{
	printf("%s", message);
	exit(1);
}

void	validate_input(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = -1;
		while (j++, av[i][j])
			if (av[i][0] == '0' || (!(av[i][j] >= '0' && av[i][j] <= '9')))
				ft_error("Arguments need to be of positive value.\n");
		i++;
	}
}

int	ft_atoi(char *s)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (result);
}

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

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

int has_philosopher_died(t_thread *philo, long current_time, long tt_die)
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

void	ft_usleep(long ms)
{
	long	elapsed_time;
	long	start_time;

	start_time = get_time_in_ms();
	elapsed_time = 0;
	while (elapsed_time < ms)
	{
		usleep(10);
		elapsed_time = get_time_in_ms() - start_time;
	}
}

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

void	init_mutex_or_exit(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
		ft_error("Mutex initialization failed\n");
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
		init_mutex_or_exit(&data->forks[i]);
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
	pthread_t	monitor_thread;
	int			i;

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
	if (pthread_create(&monitor_thread, NULL, &monitor, data) != 0)
	{
		perror("Monitor thread creating failed.");
		exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i, i < data->total_threads)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(monitor_thread, NULL);
}

void	destroy_data(t_data *data)
{
	int	i;

	i = -1;
	while (i++, i < data->total_threads)
	{
		pthread_mutex_destroy(&data->forks[i]);
	}
	free(data->forks);
	free(data->philo);
}

int	main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		ft_error("Malloc failed\n");
	if (!(ac >= 5 && ac <= 6))
		ft_error("Invalid number of arguments.\n");
	else
		validate_input(ac, av);
	data->total_threads = ft_atoi(av[1]);
	init_mutex(data);
	init_data(data, av);
	init_threads(data);
	destroy_data(data);
	return (0);
}
