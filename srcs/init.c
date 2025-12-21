/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:44:27 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/21 20:12:17 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	init_mutexes(t_table *table)
{
	int	i;

	pthread_mutex_init(&table->death_mutex, NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	if (!table->forks)
		return (1);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	return (0);
}

t_table	*init_table(int ac, char **av)
{
	t_table	*table;

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
	if (init_mutexes(table))
	{
		free(table);
		return (NULL);
	}
	return (table);
}

static void	assign_forks(t_philo *philo, int i, int num_philos)
{
	if (i % 2 == 0)
	{
		philo->fork[0] = i;
		philo->fork[1] = (i + 1) % num_philos;
	}
	else
	{
		philo->fork[0] = (i + 1) % num_philos;
		philo->fork[1] = i;
	}
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
		assign_forks(&philos[i], i, table->num_philos);
		i++;
	}
	return (philos);
}
