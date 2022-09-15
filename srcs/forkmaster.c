/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forkmaster.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 15:33:35 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 12:37:20 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
int	forkmaster_ask(t_philo *self)
{
	t_fm	*fkm;

	fkm = self->forkmaster;
	if (self->fork_l == self->fork_r)
		return (0);
	pthread_mutex_lock(&(fkm->lock));
	if (((self->id % 2 != fkm->turn_mode) || (fkm->status[self->fork_l] == 0
				|| fkm->status[self->fork_r] == 0))
		|| (fkm->turn_mode == 1 && fkm->turn == 0 && self->id != 1))
	{
		pthread_mutex_unlock(&(fkm->lock));
		return (0);
	}
	pthread_mutex_lock(&fkm->forks[self->fork_l]);
	pthread_mutex_lock(&fkm->forks[self->fork_r]);
	fkm->status[self->fork_l] = 0;
	fkm->status[self->fork_r] = 0;
	fkm->turn += 1;
	if (fkm->turn >= self->ruleset->mode_val[fkm->turn_mode])
	{
		fkm->turn = 0;
		fkm->turn_mode ^= 1;
	}
	pthread_mutex_unlock(&(fkm->lock));
	return (1);
}
*/

int forkmaster_ask(t_philo *self)
{
	if (self->ruleset->nb_philo == 1)
	{
		msg_put(self, time_getstamp(), "has taken a fork");
		philo_wait(self,  self->ruleset->tm_to_die);
		pthread_mutex_lock(&self->death->lock);
		self->death->dead = 1;
		pthread_mutex_unlock(&self->death->lock);
		msg_put(self, time_getstamp(), "died");
		return (0);
	}
	if (self->id % 2 == 1)
		pthread_mutex_lock(&self->forkmaster->forks[self->fork_l]);
	else
		pthread_mutex_lock(&self->forkmaster->forks[self->fork_r]);
	if (self->id % 2 == 1)
		pthread_mutex_lock(&self->forkmaster->forks[self->fork_r]);
	else
		pthread_mutex_lock(&self->forkmaster->forks[self->fork_l]);
	return (1);
}


void	forkmaster_tell(t_philo *self)
{
	t_fm	*forkmaster;

	forkmaster = self->forkmaster;
	pthread_mutex_unlock(&forkmaster->forks[self->fork_l]);
	pthread_mutex_unlock(&forkmaster->forks[self->fork_r]);
}

void	forkmaster_del(t_fm *forkmaster)
{
	int	i;

	i = 0;
	while (i != forkmaster->nb_fork)
		pthread_mutex_destroy(&(forkmaster->forks[i++]));
	pthread_mutex_destroy(&forkmaster->lock);
	free(forkmaster->forks);
	free(forkmaster->status);
	free(forkmaster);
}

int	forkmaster_init(t_fm *forkmaster, int n)
{
	int	i;

	forkmaster->nb_fork = 0;
	forkmaster->turn = 0;
	forkmaster->turn_mode = 1;
	if (pthread_mutex_init(&(forkmaster->lock), NULL) != 0)
		return (0);
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&(forkmaster->forks[i]), NULL) != 0)
			return (0);
		forkmaster->nb_fork += 1;
		forkmaster->status[i] = 1;
		i++;
	}
	return (1);
}

t_fm	*forkmaster_create(int n)
{
	t_fm	*forkmaster;

	forkmaster = (t_fm *)malloc(sizeof(t_fm));
	if (!forkmaster)
		return (0);
	forkmaster->status = (int *)malloc(sizeof(int) * n);
	if (!forkmaster->status)
	{
		free(forkmaster);
		return (0);
	}
	forkmaster->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	if (!forkmaster->forks)
	{
		free(forkmaster->status);
		free(forkmaster);
		return (0);
	}
	if (!forkmaster_init(forkmaster, n))
	{
		forkmaster_del(forkmaster);
		return (0);
	}
	return (forkmaster);
}
