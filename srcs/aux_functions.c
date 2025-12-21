/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:14 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/21 20:19:22 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	is_number(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(char *str)
{
	int	nbr;
	int	i;
	int	signal;

	nbr = 0;
	i = 0;
	signal = 1;
	if (str[i] == '-')
	{
		signal = -1;
		i++;
	}
	while (is_number(str[i]))
	{
		nbr *= 10;
		nbr += (str[i] - '0');
		i++;
	}
	return (signal * nbr);
}

void	print_status(t_philo *philo, const char *status)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	printf("%ld %d %s\n",
		get_time_in_ms() - philo->table->start_time,
		philo->id,
		status);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

static const char	*get_message(t_status status)
{
	if (status == DIED)
		return ("died");
	if (status == EATING)
		return ("is eating");
	if (status == SLEEPING)
		return ("is sleeping");
	if (status == THINKING)
		return ("is thinking");
	if (status == GOT_FORK_1 || status == GOT_FORK_2)
		return ("has taken a fork");
	if (status == RELEASED_FORK)
		return ("has released both forks");
	return ("");
}

void	write_status(t_philo *philo, bool died, t_status status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->print_mutex);
	if (!has_simulation_stopped(philo->table))
	{
		timestamp = get_time_in_ms() - philo->table->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, get_message(status));
		if (died)
			philo->table->simulation_end = true;
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}
