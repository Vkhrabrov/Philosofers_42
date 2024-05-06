/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:09:27 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/18 21:32:08 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	ft_atoi(const char *str)
{
	long	i;
	int		j;
	int		sign_count;

	i = 0;
	j = 1;
	sign_count = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			j = j * -1;
		str++;
		sign_count++;
	}
	if (sign_count > 1)
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		i = (i * 10) + (*str - '0');
		str++;
	}
	return (i * j);
}

void	safe_print(size_t action_time, void *v_philo, char *str)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	pthread_mutex_lock(philo->write_lock);
	printf("%lu %d %s\n", action_time, philo->id, str);
	pthread_mutex_unlock(philo->write_lock);
}

void	join_philosophers(t_philo *data, t_program *set)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(set->philos[i].thread, NULL);
		i++;
	}
}
