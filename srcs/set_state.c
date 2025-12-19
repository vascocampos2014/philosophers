/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:34 by vaires-m          #+#    #+#             */
/*   Updated: 2025/06/16 14:44:35 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	write_status(philo, false, EATING);
	precise_usleep(philo->table->time_to_eat * 1000);
	if (philo->table->must_eat_count != -2)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		philo->times_ate++;
		pthread_mutex_unlock(&philo->meal_mutex);
	}
}

void	think(t_philo *philo)
{
	long	time_since_meal;
	time_t	think_time;

	write_status(philo, false, THINKING);
	time_since_meal = get_time_in_ms() - philo->last_meal;
	if (time_since_meal > philo->table->time_to_die)
	{
		write_status(philo, true, DIED);
		pthread_mutex_lock(&philo->table->death_mutex);
		philo->table->simulation_end = true;
		pthread_mutex_unlock(&philo->table->death_mutex);
		return ;
	}
	think_time = (philo->table->time_to_die - time_since_meal
			- philo->table->time_to_eat) / 2;
	if (think_time < 0)
		think_time = 0;
	if (think_time > 200)
		think_time = 200;
	precise_usleep(think_time);
}

void	philo_sleep(t_philo *philo)
{
	write_status(philo, false, SLEEPING);
	precise_usleep(philo->table->time_to_sleep * 1000);
}