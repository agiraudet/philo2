/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:25:27 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 12:49:01 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_are_fat(t_rules *ruleset)
{
	int	i;

	if (ruleset->nb_eat_to_end == -1)
		return (0);
	i = 0;
	pthread_mutex_lock(&ruleset->food);
	while (i < ruleset->nb_philo)
	{
		if (ruleset->meals[i++] < ruleset->nb_eat_to_end)
		{
			pthread_mutex_unlock(&ruleset->food);
			return (0);
		}
	}
	pthread_mutex_unlock(&ruleset->food);
	return (1);
}

void	philo_letgo(t_philo *self)
{
	if (self->hold[0] || self->hold[1])
		philo_use_fork(self, PUT_FORK);
}

int	philo_is_alive(t_philo *self)
{
	long int	ms;

	ms = time_getstamp(&self->ruleset->clock);
	if (self->ruleset->tm_to_die < ms - self->last_meal)
	{
		pthread_mutex_lock(&self->death->lock);
		self->death->dead = 1;
		pthread_mutex_unlock(&self->death->lock);
		msg_put(self, ms, "died");
	}
	if (self->death->dead || philo_are_fat(self->ruleset))
	{
		philo_letgo(self);
		return (0);
	}
	return (1);
}

void	philo_wait(t_philo *self, long int tm_to_wait)
{
	long int	ms;
	long int	life;

	if (self->death->dead)
	{
		return ;
	}
	ms = time_getstamp(&self->ruleset->clock);
	if (ms + tm_to_wait > self->last_meal + self->ruleset->tm_to_die)
	{
		life = (self->last_meal + self->ruleset->tm_to_die) - ms;
		life++;
		usleep(life * 1000);
		philo_is_alive(self);
	}
	else
		usleep(tm_to_wait * 1000);
}

void	*philo_run(void *self_ptr)
{
	t_philo	*self;

	if (!self_ptr)
		return (0);
	self = (t_philo *)self_ptr;
	self->last_meal = time_getstamp(&self->ruleset->clock);
	self->last_sleep = time_getstamp(&self->ruleset->clock);
	philo_loop(self);
	free(self);
	return (0);
}
