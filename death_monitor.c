/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 20:28:15 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/18 21:32:07 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	write_about_death(t_program *set, int i)
{
	size_t	action_time;

	action_time = get_current_time() - set->philos[i].start_time;
	pthread_mutex_lock(&set->write_lock);
	printf("%zu %d died\n", action_time, set->philos[i].id);
	pthread_mutex_unlock(&set->write_lock);
}

void	check_philosopher_deaths(t_program *set)
{
	int		i;

	i = 0;
	while (i < set->philos->num_of_philos)
	{
		pthread_mutex_lock(set->philos->meal_lock);
		if (get_current_time()
			- set->philos[i].last_meal > set->philos[i].time_to_die)
		{
			pthread_mutex_unlock(set->philos->meal_lock);
			pthread_mutex_lock(set->philos[i].dead_lock);
			set->dead_flag = 1;
			pthread_mutex_unlock(set->philos[i].dead_lock);
			pthread_mutex_lock(set->philos->meal_lock);
			if (set->philos->all_ate != 1)
			{
				pthread_mutex_unlock(set->philos->meal_lock);
				write_about_death(set, i);
				return ;
			}
		}
		pthread_mutex_unlock(set->philos->meal_lock);
		i++;
	}
}

void	*monitor_philosophers(void *v_set)
{
	t_program	*set;

	set = (t_program *)v_set;
	while (1)
	{
		check_philosopher_deaths(set);
		ft_usleep(10);
		pthread_mutex_lock(set->philos->meal_lock);
		if ((dead_check(set))
			|| set->philos->all_ate == set->philos->num_of_philos)
		{
			pthread_mutex_unlock(set->philos->meal_lock);
			break ;
		}
		else
			pthread_mutex_unlock(set->philos->meal_lock);
	}
	return (NULL);
}

void	cleanup(t_program *set)
{
	int	i;

	i = -1;
	while (++i < set->philos->num_of_philos)
	{
		pthread_mutex_destroy(&set->forks[i]);
		pthread_mutex_destroy(set->philos[i].meal_lock);
		pthread_mutex_destroy(set->philos[i].dead_lock);
	}
	free(set->forks);
	free(set->philos);
}

int	dead_check(void *v_set)
{
	t_program	*set;

	set = (t_program *)v_set;
	pthread_mutex_lock(set->philos->dead_lock);
	if (set->dead_flag)
	{
		pthread_mutex_unlock(set->philos->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(set->philos->dead_lock);
	return (0);
}
