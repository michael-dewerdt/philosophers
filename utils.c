/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mea <mea@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 13:15:18 by mea               #+#    #+#             */
/*   Updated: 2022/06/14 11:41:03 by mea              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_atoi(const char *str)
{
	int	neg;
	int	i;
	int	num;

	i = 0;
	neg = 1;
	num = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}
	return (num * neg);
}

void	print_action(int time, t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->is_writing);
	if (philo->table->death == 0)
		printf("%d ms: Philosopher %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->is_writing);
}

int		meal_checker(t_table *table)
{
	int	i;

	if (table->nb_of_meal_min < 0)
		return (0);
	i = -1;
	while (++i < table->nb_of_philo)
	{
		if (table->philo[i].nb_of_meal < table->nb_of_meal_min)
			return (0);
	}
	return (1);
}

void	death_checker(t_table *table)
{
	size_t	i;
	
	i = -1;
	while (++i < table->nb_of_philo)
	{
		if (table->philo[i].eating == 1)
			continue ;
		pthread_mutex_lock(&table->is_dying);
		if ((actual_time() - table->philo[i].last_meal_time) >= table->time_to_die)
		{
			printf("Last meal time : %d\n", table->philo[i].last_meal_time);
			print_action(actual_time(), table->philo + i, "is dead\n");
			table->death = 1;
			pthread_mutex_unlock(&table->is_dying);
			return ;
		}
		pthread_mutex_unlock(&table->is_dying);
	}
	return;
}
