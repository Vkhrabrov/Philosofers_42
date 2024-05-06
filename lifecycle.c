/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 20:23:35 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/18 21:35:27 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_forks_and_check_solo(t_philo *philo)
{
	size_t	action_time;

	pthread_mutex_lock(philo->l_fork);
	action_time = get_current_time() - philo->start_time;
	if (!some_is_death(philo))
		safe_print(action_time, philo, " has taken a fork");
	if (philo->num_of_philos == 1)
	{
		pthread_mutex_unlock(philo->l_fork);
		ft_usleep(philo->time_to_die);
		return (1);
	}
	pthread_mutex_lock(philo->r_fork);
	action_time = get_current_time() - philo->start_time;
	if (!some_is_death(philo))
		safe_print(action_time, philo, " has taken a fork");
	return (0);
}

void	eating(t_philo *philo)
{
	size_t	action_time;

	if (!some_is_death(philo))
	{
		action_time = get_current_time() - philo->start_time;
		safe_print(action_time, philo, " is eating");
		pthread_mutex_lock(philo->meal_lock);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(philo->meal_lock);
		ft_usleep(philo->time_to_eat);
		pthread_mutex_lock(philo->meal_lock);
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->meal_lock);
	}
	else
		return ;
}

int	sleeping_and_thinking(t_philo *philo)
{
	size_t	action_time;

	if (!some_is_death(philo))
	{
		action_time = get_current_time() - philo->start_time;
		safe_print(action_time, philo, " is sleeping");
		ft_usleep(philo->time_to_sleep);
	}
	else
		return (1);
	if (!some_is_death(philo))
	{
		action_time = get_current_time() - philo->start_time;
		safe_print(action_time, philo, " is thinking");
	}
	else
		return (1);
	return (0);
}

int	meal_counter(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	if (philo->num_times_to_eat
		&& philo->meals_eaten == philo->num_times_to_eat)
	{
		pthread_mutex_unlock(philo->meal_lock);
		printf("Philosopher %d has eaten %d times\n",
			philo->id, philo->meals_eaten);
		pthread_mutex_lock(philo->meal_lock);
		philo->all_ate++;
		pthread_mutex_unlock(philo->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return (0);
}

void	*philosopher_lifecycle(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	if (philo->id % 2 == 0)
		ft_usleep(philo->time_to_eat);
	while (!some_is_death(philo))
	{
		if (get_forks_and_check_solo(philo) == 1)
			return (NULL);
		eating(philo);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		if (meal_counter(philo) == 1)
			return (NULL);
		if (sleeping_and_thinking(philo) == 1)
			return (NULL);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return (NULL);
}
