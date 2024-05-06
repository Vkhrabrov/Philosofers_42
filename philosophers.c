/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:27:22 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/18 21:36:27 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	start_philosophers(t_philo *data, t_program *set, char **argv)
{
	size_t	start_time;
	int		i;

	i = 0;
	start_time = get_current_time();
	while (i < data->num_of_philos)
	{
		set->philos[i].id = i + 1;
		set->philos[i].last_meal = start_time;
		set->philos[i].start_time = start_time;
		set->philos[i].num_of_philos = ft_atoi(argv[1]);
		set->philos[i].time_to_die = ft_atoi(argv[2]);
		set->philos[i].time_to_eat = ft_atoi(argv[3]);
		set->philos[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			set->philos[i].num_times_to_eat = ft_atoi(argv[5]);
		else
			set->philos[i].num_times_to_eat = 0;
		pthread_create(&set->philos[i].thread, NULL,
			philosopher_lifecycle, &set->philos[i]);
		i++;
	}
}

void	initialize_mutexes(t_philo *data, t_program *set)
{
	int	i;

	i = 0;
	set->forks = malloc(data->num_of_philos * sizeof(pthread_mutex_t));
	set->philos = malloc(data->num_of_philos * sizeof(t_philo));
	set->dead_flag = 0;
	while (i < data->num_of_philos)
	{
		pthread_mutex_init(&set->forks[i], NULL);
		set->philos[i].id = i;
		set->philos[i].l_fork = &set->forks[i];
		set->philos[i].r_fork = &set->forks[(i + 1) % data->num_of_philos];
		set->philos[i].dead = &set->dead_flag;
		set->philos[i].write_lock = &set->write_lock;
		set->philos[i].meal_lock = &set->meal_lock;
		set->philos[i].dead_lock = &set->dead_lock;
		pthread_mutex_init(set->philos[i].meal_lock, NULL);
		pthread_mutex_init(set->philos[i].dead_lock, NULL);
		i++;
	}
}

void	initialize_received_data(t_philo *data, char **argv)
{
	data->num_of_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->num_times_to_eat = ft_atoi(argv[5]);
	data->all_ate = 0;
}

int	input_checks(int argc, char **argv)
{
	int	i;

	i = 0;
	while (argv[i + 1] != NULL)
		i++;
	if (argc < 5 || argc > 6)
	{
		printf("Not enough numbers in your input,");
		printf(" think a bit more, then act without any doubt!\n");
		return (1);
	}
	while (i > 0)
	{
		if (atoi(argv[i]) < 1)
		{
			printf("Something's up with your input,");
			printf(" think a bit more, then act without any doubt!\n");
			return (1);
		}
		i--;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	mutex;
	t_program		set;
	t_philo			data;
	pthread_t		monitor_thread;

	if (input_checks(argc, argv) == 0)
	{
		initialize_received_data(&data, argv);
		initialize_mutexes(&data, &set);
		pthread_mutex_init(&mutex, NULL);
		start_philosophers(&data, &set, argv);
		pthread_create(&monitor_thread, NULL, monitor_philosophers, &set);
		join_philosophers(&data, &set);
		pthread_join(monitor_thread, NULL);
		cleanup(&set);
		pthread_mutex_destroy(&mutex);
	}
	return (0);
}
