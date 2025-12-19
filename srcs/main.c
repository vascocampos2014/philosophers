/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:29 by vaires-m          #+#    #+#             */
/*   Updated: 2025/06/16 14:47:32 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	invalid_args(int ac)
{
	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo number_of_philosophers ");
		printf("time_to_die time_to_eat time_to_sleep [must_eat_count]\n");
		return (1);
	}
	return (0);
}

static void	start_simulation(t_table *table, t_philo *philos)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_create(&philos[i].thread, NULL, philosopher, &philos[i]);
		if (i % 2 == 0)
			usleep(1000);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, philos);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

int	main(int ac, char **av)
{
	t_table	*table;
	t_philo	*philos;

	if (invalid_args(ac))
		return (1);
	table = init_table(ac, av);
	if (!table)
		return (1);
	philos = init_philo(table);
	if (!philos)
		return (1);
	if (table->num_philos == 1)
	{
		one_philo(table);
		cleanup_philos(philos, table->num_philos);
		return (0);
	}
	start_simulation(table, philos);
	cleanup_philos(philos, table->num_philos);
	cleanup_table(table);
	return (0);
}