/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 23:00:42 by marvin            #+#    #+#             */
/*   Updated: 2024/10/14 20:10:47 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// • One or more philosophers sit at a round table.
// There is a large bowl of spaghetti in the middle of the table.

// • The philosophers alternatively eat, think, or sleep.

// • There are also forks on the table. There are as many forks as philosophers.
// • Because serving and eating spaghetti with only one fork is very inconvenient,
//	a
// philosopher takes their right and their left forks to eat, one in each hand.
// • When a philosopher has finished eating,
//	they put their forks back on the table and
// start sleeping. Once awake,
//	they start thinking again. The simulation stops when
// a philosopher dies of starvation.

// • Global variables are forbidden!
// • Your program should take the following arguments:
// ◦ number_of_philosophers: The number of philosophers and forks.
// ◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
// milliseconds since the beginning of their last meal or the beginning of the simulation,
// they die.
// ◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
// During that time, they will need to hold two forks.
// ◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
// ◦ number_of_times_each_philosopher_must_eat (optional argument): If all
// philosophers have eaten at least number_of_times_each_philosopher_must_eat
// times, the simulation stops. If not specified, the simulation stops when a
// philosopher dies.
// • Each philosopher has a number ranging from 1 to number_of_philosophers.
// • Philosopher number 1 sits next to philosopher number number_of_philosophers.
// Any other philosopher number N sits between philosopher number N
//	- 1 and philosopher
// number N + 1.

// About the logs of your program:
// • Any state change of a philosopher must be formatted as follows:
// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died
// Replace timestamp_in_ms with the current timestamp in milliseconds
// and X with the philosopher number.
// • A displayed state message should not be mixed up with another message.
// • A message announcing a philosopher died should be displayed no more than 10 ms
// after the actual death of the philosopher.

// memset, printf, malloc, free, write,
// usleep, gettimeofday, pthread_create,
// pthread_detach, pthread_join, pthread_mutex_init,
// pthread_mutex_destroy, pthread_mutex_lock,
// pthread_mutex_unlock

// • Each philosopher should be a thread.
// • To prevent philosophers from duplicating forks,
//	you should protect the forks state
// with a mutex for each of them.

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

void	ft_usleep(long ms)
{
	long	elapsed_time;

	long start_time = get_time_in_ms(); // Get the current time in milliseconds
	elapsed_time = 0;
	while (elapsed_time < ms)
	{
		usleep(10);                                 
			// Sleep for a short time to avoid busy waiting
		elapsed_time = get_time_in_ms() - start_time; // Update the elapsed time
	}
}

void	*routine(void *arg)
{
	t_thread	*philo;
	long		start_time;
	long		current_time;

	philo = (t_thread *)arg;
	start_time = get_time_in_ms();
	while (1)
	{
		pthread_mutex_lock(philo->fork_left);
		printf("%ld %i has taken the left fork\n", get_time_in_ms()
			- start_time, philo->id);
		pthread_mutex_lock(philo->fork_right);
		printf("%ld %i has taken the right fork\n", get_time_in_ms()
			- start_time, philo->id);
		// Now the philosopher has both forks, so they can eat
		current_time = get_time_in_ms() - start_time;
		printf("%ld %i is eating\n", current_time, philo->id);
		philo->last_meal = current_time; // Record the time of last meal
		ft_usleep(philo->data->tt_eat);        // Simulate eating duration
		// Release forks after eating
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		// Sleeping phase
		current_time = get_time_in_ms() - start_time;
		printf("%ld %i is sleeping\n", current_time, philo->id);
		ft_usleep(philo->data->tt_sleep); // Simulate sleeping duration
		// Thinking phase
		current_time = get_time_in_ms() - start_time;
		printf("%ld %i is thinking\n", current_time, philo->id);
		// Small delay to avoid overwhelming the CPU and provide fairness
		ft_usleep(10); // Small delay before trying to acquire forks again
	}
	return (NULL);
}

void	init_mutex(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->total_threads);
	if (!data->forks)
		ft_error("Malloc failed\n");
	data->philo = malloc(sizeof(t_thread) * data->total_threads);
	if (!data->philo)
		ft_error("Malloc failed\n");
	i = -1;
	while (i++, i < data->total_threads)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			ft_error("Mutex initialization failed\n");
		data->philo[i].id = i + 1;
		data->philo[i].data = data;
	}
}

void	init_data(t_data *data, char **av)
{
	int	i;

	data->total_threads = ft_atoi(av[1]);
	data->tt_die = ft_atoi(av[2]);
	data->tt_eat = ft_atoi(av[3]);
	data->tt_sleep = ft_atoi(av[4]);
	i = -1;
	while (++i, i < data->total_threads)
	{
		if (av[5])
			data->philo[i].cycles = ft_atoi(av[5]);
		else
			data->philo[i].cycles = -1;
		if (data->philo[i].id % 2 == 0)
		{
			data->philo[i].fork_left = &data->forks[i];
			data->philo[i].fork_right = &data->forks[(i + 1)
				% data->total_threads];
		}
		else
		{
			data->philo[i].fork_right = &data->forks[i];
			data->philo[i].fork_left = &data->forks[(i + 1)
				% data->total_threads];
		}
	}
}

void	init_threads(t_data *data)
{
	int	i;

	i = -1;
	while (i++, i < data->total_threads)
		if (pthread_create(&data->philo[i].thread, NULL, &routine,
				&data->philo[i]) != 0)
			// instead of exiting, free the memory that was allocated so far
			// then exit, or create a wrapper for pthread_create
			ft_error("Thread creating failed.\n");
	i = -1;
	while (i++, i < data->total_threads)
		if (pthread_join(data->philo[i].thread, NULL) != 0)
			ft_error("Thread joining failed.\n");
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

// send a message "timestamp_in_ms n has taken a fork"
// unlock the mutexes that were locked
// put n thread to sleep for time_to_sleep milisecond,
//	printing a message  "timestamp_in_ms n is sleeping"
// after time_to_sleep miliseconds put thread on thinking mode printing "timestamp_in_ms n is thinking",
// after that mode is over,
//	n philosopher has chance to take 2 forks again to start eating,
//	and it needs to be before the timer goes to 0
// the timer is time_to_die and it starts when the simulation starts and updates the time to when he stopped eating/started thinking (??)
// otherwise simulation stops,
//	sending a message "timestamp_in_ms n died" not later than 10 miliseconds after the death.
// last argument might indicate number of cycles for each thread,
//	if we reach the number of cycles for specific thread,
//	we stop the thread from executing,