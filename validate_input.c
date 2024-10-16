/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:29:00 by ssuchane          #+#    #+#             */
/*   Updated: 2024/10/16 19:29:09 by ssuchane         ###   ########.fr       */
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
