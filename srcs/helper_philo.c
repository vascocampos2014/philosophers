/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:24 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/21 20:55:05 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->forks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->forks[philo->fork[1]]);
	write_status(philo, false, GOT_FORK_2);
	return (0);
}

int	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->table->forks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->forks[philo->fork[0]]);
	write_status(philo, false, RELEASED_FORK);
	return (0);
}

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_usleep(long microseconds)
{
	long	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) * 1000 < microseconds)
		usleep(100);
}
