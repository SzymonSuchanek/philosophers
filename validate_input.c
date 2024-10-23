/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:29:00 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/23 21:30:44 by ssuchane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *message)
{
	printf("%s", message);
	return (1);
}

int	validate_input(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = -1;
		while (j++, av[i][j])
		{
			if (av[i][0] == '0' || (!(av[i][j] >= '0' && av[i][j] <= '9')))
			{
				ft_error("Arguments need to be of positive value.\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
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
