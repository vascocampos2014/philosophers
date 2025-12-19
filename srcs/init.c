/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:27 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/19 10:57:59 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

t_table	*init_table(int ac, char **av)
{
	t_table	*table;
	int		i;

	(void)ac;
	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	table->num_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->must_eat_count = -2;
	if (av[5])
		table->must_eat_count = ft_atoi(av[5]);
	table->simulation_end = false;
	table->start_time = get_time_in_ms() + 100;
	pthread_mutex_init(&table->death_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	if (!table->forks)
		return (NULL);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	return (table);
}

t_philo	*init_philo(t_table *table)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * table->num_philos);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < table->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].times_ate = 0;
		philos[i].last_meal = 0;
		philos[i].table = table;
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		if (i % 2 == 0)
		{
			philos[i].fork[0] = i;
			philos[i].fork[1] = (i + 1) % table->num_philos;
		}
		else
		{
			philos[i].fork[0] = (i + 1) % table->num_philos;
			philos[i].fork[1] = i;
		}
		i++;
	}
	return (philos);
}
