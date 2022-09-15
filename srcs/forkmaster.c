/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkmaster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 15:33:35 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 18:45:15 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_take(t_philo *self, int dir)
{
	if (dir == LEFT)
	{
		pthread_mutex_lock(&self->ruleset->forks[self->fork_l]);
		self->hold[0] = 1;
		pthread_mutex_lock(&self->ruleset->ff[self->fork_l]);
		self->ruleset->free_fork[self->fork_l] = time_getstamp(self->ruleset)
			+ self->ruleset->tm_to_eat;
		pthread_mutex_unlock(&self->ruleset->ff[self->fork_l]);
	}
	else
	{
		pthread_mutex_lock(&self->ruleset->forks[self->fork_r]);
		self->hold[1] = 1;
		pthread_mutex_lock(&self->ruleset->ff[self->fork_r]);
		self->ruleset->free_fork[self->fork_r] = time_getstamp(self->ruleset)
			+ self->ruleset->tm_to_eat;
		pthread_mutex_unlock(&self->ruleset->ff[self->fork_r]);
	}
}

int	forkmaster_ask(t_philo *self)
{
	if (self->ruleset->nb_philo == 1)
	{
		msg_put(self, time_getstamp(self->ruleset), "has taken a fork");
		philo_wait(self, self->ruleset->tm_to_die);
		pthread_mutex_lock(&self->death->lock);
		self->death->dead = 1;
		pthread_mutex_unlock(&self->death->lock);
		msg_put(self, time_getstamp(self->ruleset), "died");
		return (0);
	}
	if (self->id % 2 == 1)
		fork_take(self, LEFT);
	else
		fork_take(self, RIGHT);
	if (self->id % 2 == 1)
		fork_take(self, RIGHT);
	else
		fork_take(self, LEFT);
	return (1);
}

void	forkmaster_tell(t_philo *self)
{
	if (self->hold[0])
	{
		pthread_mutex_unlock(&self->ruleset->forks[self->fork_l]);
		self->hold[0] = 0;
	}
	if (self->hold[1])
	{
		pthread_mutex_unlock(&self->ruleset->forks[self->fork_r]);
		self->hold[1] = 0;
	}
}

void	forkmaster_del(pthread_mutex_t *forks, int n)
{
	int	i;

	i = 0;
	while (i != n)
		pthread_mutex_destroy(&forks[i++]);
	free(forks);
}

pthread_mutex_t	*forkmaster_create(int n)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	if (!forks)
		return (0);
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			i--;
			while (i)
				pthread_mutex_destroy(&forks[i--]);
			return (0);
		}
		i++;
	}
	return (forks);
}
