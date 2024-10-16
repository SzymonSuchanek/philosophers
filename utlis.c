/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utlis.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:29:31 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/16 19:29:44 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
