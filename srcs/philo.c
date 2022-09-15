/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:25:27 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 17:25:04 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	philo_thinking_time(t_philo *self, long int ms, int *will_die)
{
	long int	tm_fork_l;
	long int	tm_fork_r;
	long int	tm_of_death;

	pthread_mutex_lock(&self->ruleset->ff[self->fork_l]);
	tm_fork_l = self->ruleset->free_fork[self->fork_l];
	pthread_mutex_unlock(&self->ruleset->ff[self->fork_l]);
	pthread_mutex_lock(&self->ruleset->ff[self->fork_r]);
	tm_fork_r = self->ruleset->free_fork[self->fork_r];
	pthread_mutex_unlock(&self->ruleset->ff[self->fork_r]);
	tm_of_death = self->last_meal + self->ruleset->tm_to_die;
	if (tm_of_death < tm_fork_l || tm_of_death < tm_fork_r)
	{
		*will_die = 1;
		return (tm_of_death - ms);
	}
	*will_die = 0;
	if (tm_fork_l < tm_fork_r)
		return (tm_fork_l - ms);
	return (tm_fork_r - ms);
}

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

int	philo_is_alive(t_philo *self)
{
	long int	ms;
	int			dead;

	dead = 0;
	ms = time_getstamp(self->ruleset);
	if (self->ruleset->tm_to_die < ms - self->last_meal)
	{
		pthread_mutex_lock(&self->death->lock);
		self->death->dead = 1;
		pthread_mutex_unlock(&self->death->lock);
		msg_put(self, ms, "died");
		return (0);
	}
//	pthread_mutex_lock(&self->death->lock);
	dead = self->death->dead;
//	pthread_mutex_unlock(&self->death->lock);
	if (dead || philo_are_fat(self->ruleset))
	{
		forkmaster_tell(self);
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
	ms = time_getstamp(self->ruleset);
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
	self->last_meal = time_getstamp(self->ruleset);
	self->last_sleep = time_getstamp(self->ruleset);
	philo_loop(self);
	free(self);
	return (0);
}
