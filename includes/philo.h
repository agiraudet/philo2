/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 15:59:07 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 17:56:02 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define PICK_FORK 1
# define PUT_FORK 0
# define LEFT 1
# define RIGHT 0

typedef struct s_rules
{
	int				nb_philo;
	long int		tm_to_die;
	long int		tm_to_eat;
	long int		tm_to_sleep;
	long int		start_time;
	int				nb_eat_to_end;
	int				*meals;
	pthread_mutex_t	talk;
	pthread_mutex_t	food;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*ff;
	int				*free_fork;
}				t_rules;

typedef struct s_death
{
	pthread_mutex_t	lock;
	int				dead;
}				t_death;

typedef struct s_philo
{
	int			id;
	int			fork_l;
	int			fork_r;
	int			hold[2];
	long int	last_meal;
	long int	last_sleep;
	long int	tm_of_death;
	t_rules		*ruleset;
	t_death		*death;
}				t_philo;

//srcs/forkmaster.c
int				forkmaster_ask(t_philo *self);
void			forkmaster_tell(t_philo *self);
void			forkmaster_del(pthread_mutex_t *forks, int n);
pthread_mutex_t	*forkmaster_create(int n);

//srcs/msg.c
void			msg_put(t_philo *self, long int ms, char *msg);

//srcs/philo.c
void			philo_letgo(t_philo *self);
int				philo_is_alive(t_philo *self);
void			philo_wait(t_philo *self, long int tm_to_wait);
void			*philo_run(void *self_ptr);
long int	philo_thinking_time(t_philo *self, long int ms, int *will_die);

//srcs/philo_do.c
void			philo_use_fork(t_philo *self, int use);
int				philo_loop(t_philo *self);

//srcs/room.c
int				room_play(t_rules *ruleset);

//srcs/utils.c
int				ft_atoi(const char *nptr);
int				ft_strisnb(const char *str);
long int		time_getstamp(t_rules *ruleset);

#endif
