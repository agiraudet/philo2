/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:25:27 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 15:27:34 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_use_fork(t_philo *self, int use)
{
	if (use == PICK_FORK)
	{
		while (philo_is_alive(self))
		{
			if (forkmaster_ask(self))
				break ;
		}
		if (philo_is_alive(self))
		{
			msg_put(self, time_getstamp(self->ruleset),
					"has taken a fork");
			self->hold[0] = 1;
			msg_put(self, time_getstamp(self->ruleset),
					"has taken a fork");
			self->hold[1] = 1;
		}
	}
	else if (use == PUT_FORK)
	{
		forkmaster_tell(self);
		self->hold[0] = 0;
		self->hold[1] = 0;
	}
}

void	philo_sleep(t_philo *self)
{
	long int	ms;

	ms = time_getstamp(self->ruleset);
	self->last_sleep = ms;
	if (!philo_is_alive(self))
		return ;
	msg_put(self, ms, "is sleeping");
	philo_wait(self, self->ruleset->tm_to_sleep);
}

void	philo_think(t_philo *self)
{
	long int	ms;
	int			dead;

	pthread_mutex_lock(&self->death->lock);
	dead = self->death->dead;
	pthread_mutex_unlock(&self->death->lock);
	if (dead)
		return ;
	ms = time_getstamp(self->ruleset);
	if (!philo_is_alive(self))
		return ;
	msg_put(self, ms, "is thinking");
}

void	philo_eat(t_philo *self)
{
	long int	ms;

	if (self->death->dead || self->hold[0] == 0 || self->hold[1] == 0)
		return ;
	ms = time_getstamp(self->ruleset);
	self->last_meal = ms;
	pthread_mutex_lock(&self->ruleset->food);
	self->ruleset->meals[self->id - 1] += 1;
	pthread_mutex_unlock(&self->ruleset->food);
	msg_put(self, ms, "is eating");
	philo_wait(self, self->ruleset->tm_to_eat);
}

int	philo_loop(t_philo *self)
{
	if (self->id % 2 == 0)
		philo_wait(self, self->ruleset->tm_to_eat);
	while (philo_is_alive(self))
	{
		philo_use_fork(self, PICK_FORK);
		philo_eat(self);
		forkmaster_tell(self);
		philo_sleep(self);
		philo_think(self);
	}
	return (0);
}
