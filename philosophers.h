/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 20:25:46 by vkhrabro          #+#    #+#             */
/*   Updated: 2024/02/18 21:33:44 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	int				all_ate;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}					t_program;

long	ft_atoi(const char *str);
size_t	get_current_time(void);
int		ft_usleep(size_t milliseconds);
void	*philosopher_lifecycle(void *v_philo);
void	safe_print(size_t action_time, void *v_philo, char *str);
void	*monitor_philosophers(void *v_set);
void	start_philosophers(t_philo *data, t_program *set, char **argv);
void	cleanup(t_program *set);
void	join_philosophers(t_philo *data, t_program *set);
int		some_is_death(t_philo *philo);
int		dead_check(void *v_set);
int		some_is_death(t_philo *philo);

#endif
