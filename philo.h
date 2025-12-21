/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vaires-m <vaires-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:16:39 by vaires-m          #+#    #+#             */
/*   Updated: 2025/12/21 20:17:42 by vaires-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOS 250

typedef struct s_philo	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5,
	RELEASED_FORK = 6,
}	t_status;

typedef struct s_table
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long			start_time;
	bool			simulation_end;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	pthread_mutex_t	meal_mutex;
	long			last_meal;
	int				times_ate;
	t_table			*table;
	int				fork[2];
}	t_philo;

// Function prototypes
void	cleanup_philos(t_philo *philos, int num_philos);
t_philo	*init_philo(t_table *table);
int		one_philo(t_table *table);
int		take_forks(t_philo *philo);
int		release_forks(t_philo *philo);
bool	has_simulation_stopped(t_table *table);
void	write_status(t_philo *philo, bool reaper, t_status status);
int		ft_atoi(char *str);
long	get_time_in_ms(void);
void	precise_usleep(long microseconds);
void	print_status(t_philo *philo, const char *status);
void	*philosopher(void *data);
void	*monitor_routine(void *arg);
t_table	*init_table(int ac, char **av);
void	cleanup_table(t_table *table);
void	think(t_philo *philo);
void	eat(t_philo *philo);
void	philo_sleep(t_philo *philo);

#endif