/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:21 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/21 20:18:07 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	cleanup_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->death_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	free(table->forks);
	free(table);
}

void	cleanup_philos(t_philo *philos, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	free(philos);
}

bool	has_simulation_stopped(t_table *table)
{
	bool	stopped;

	pthread_mutex_lock(&table->death_mutex);
	stopped = table->simulation_end;
	pthread_mutex_unlock(&table->death_mutex);
	return (stopped);
}

int	one_philo(t_table *table)
{
	printf("0 1 has taken a fork\n");
	usleep(table->time_to_die * 1000);
	printf("%ld 1 died\n", get_time_in_ms() - table->start_time);
	cleanup_table(table);
	return (0);
}
