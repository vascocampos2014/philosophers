/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:32 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/21 20:12:17 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (get_time_in_ms() < philo->table->start_time)
		usleep(100);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	while (!has_simulation_stopped(philo->table))
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}

static int	check_philo_state(t_philo *philo, t_table *table)
{
	long	time_since_meal;
	int		ate_enough;

	ate_enough = 0;
	pthread_mutex_lock(&philo->meal_mutex);
	time_since_meal = get_time_in_ms() - philo->last_meal;
	if (time_since_meal > table->time_to_die && philo->last_meal != 0)
	{
		pthread_mutex_lock(&table->death_mutex);
		if (!table->simulation_end)
		{
			table->simulation_end = true;
			print_status(philo, "died");
		}
		pthread_mutex_unlock(&table->death_mutex);
		pthread_mutex_unlock(&philo->meal_mutex);
		return (-1);
	}
	if (table->must_eat_count > 0 && philo->times_ate >= table->must_eat_count)
		ate_enough = 1;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (ate_enough);
}

static int	check_all_philos(t_philo *philos, t_table *table)
{
	int	i;
	int	finished;
	int	status;

	i = 0;
	finished = 0;
	while (i < table->num_philos)
	{
		status = check_philo_state(&philos[i], table);
		if (status == -1)
			return (1);
		finished += status;
		i++;
	}
	if (table->must_eat_count > 0 && finished == table->num_philos)
	{
		pthread_mutex_lock(&table->death_mutex);
		table->simulation_end = true;
		pthread_mutex_unlock(&table->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_table	*table;

	philos = (t_philo *)arg;
	table = philos[0].table;
	while (1)
	{
		if (check_all_philos(philos, table))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
