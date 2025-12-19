/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:32 by vaires-m          #+#    #+#             */
/*   Updated: 2025/06/16 14:53:55 by vaires-m         ###   ########.fr       */
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

void *monitor_routine(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_table *table = philos[0].table;
    int i;
    int finished;

    while (1)
    {
        finished = 0;
        i = 0;
        while (i < table->num_philos)
        {
            pthread_mutex_lock(&philos[i].meal_mutex);
            long time_since_meal = get_time_in_ms() - philos[i].last_meal;
            if (table->must_eat_count > 0 && philos[i].times_ate >= table->must_eat_count)
                finished++;
            if (time_since_meal > table->time_to_die && philos[i].last_meal != 0)
            {
                pthread_mutex_lock(&table->death_mutex);
                if (!table->simulation_end)
                {
                    table->simulation_end = true;
                    print_status(&philos[i], "died");
                }
                pthread_mutex_unlock(&table->death_mutex);
                pthread_mutex_unlock(&philos[i].meal_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&philos[i].meal_mutex);
            i++;
        }
        if (table->must_eat_count > 0 && finished == table->num_philos)
        {
            pthread_mutex_lock(&table->death_mutex);
            table->simulation_end = true;
            pthread_mutex_unlock(&table->death_mutex);
            return NULL;
        }
        usleep(1000); // Check every 1ms
    }
    return NULL;
}
