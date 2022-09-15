/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 14:18:30 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 18:04:35 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*room_create_philo(int id, t_rules *ruleset, t_death *death)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (0);
	philo->id = id;
	philo->ruleset = ruleset;
	philo->fork_l = id - 1;
	if (id >= ruleset->nb_philo)
		philo->fork_r = 0;
	else
		philo->fork_r = id;
	philo->death = death;
	philo->hold[0] = 0;
	philo->hold[1] = 0;
	return ((void *)philo);
}

pthread_t	*room_philolst(t_rules *ruleset, t_death *death)
{
	pthread_t	*philo_lst;
	void		*philo_new;
	int			i;

	philo_lst = (pthread_t *)malloc(sizeof(pthread_t) * ruleset->nb_philo);
	if (!philo_lst)
		return (0);
	i = 1;
	while (i <= ruleset->nb_philo)
	{
		philo_new = room_create_philo(i, ruleset, death);
		if (pthread_create(&(philo_lst[i - 1]), NULL, &philo_run, philo_new))
		{
			free(philo_lst);
			return (0);
		}
		i++;
	}
	return (philo_lst);
}

void	room_cleanup(pthread_t *philo_lst, t_death *death, t_rules *ruleset)
{
	int	i;

	forkmaster_del(ruleset->forks, ruleset->nb_philo);
	pthread_mutex_destroy(&ruleset->talk);
	pthread_mutex_destroy(&ruleset->food);
	i = 0;
	while (i < ruleset->nb_philo)
		pthread_mutex_destroy(&ruleset->ff[i++]);
	free(ruleset->ff);
	free(ruleset->free_fork);
	if (death)
		pthread_mutex_destroy(&death->lock);
	free(ruleset->meals);
	free(ruleset);
	if (philo_lst)
		free(philo_lst);
}

int	room_play(t_rules *ruleset)
{
	pthread_t	*philo_lst;
	t_death		death;
	int			i;

	if (pthread_mutex_init(&death.lock, NULL) != 0)
	{
		room_cleanup(0, 0, ruleset);
		return (1);
	}
	death.dead = 0;
	ruleset->start_time = time_getstamp(ruleset);
	philo_lst = room_philolst(ruleset, &death);
	if (!philo_lst)
	{
		room_cleanup(0, &death, ruleset);
		return (1);
	}
	i = 0;
	while (i < ruleset->nb_philo)
		pthread_join(philo_lst[i++], 0);
	room_cleanup(philo_lst, &death, ruleset);
	return (0);
}
