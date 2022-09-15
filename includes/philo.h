/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 15:59:07 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 12:46:30 by agiraude         ###   ########.fr       */
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

typedef struct s_rules
{
	int				mode_val[2];
	int				nb_philo;
	long int		tm_to_die;
	long int		tm_to_eat;
	long int		tm_to_sleep;
	int				nb_eat_to_end;
	int				*meals;
	pthread_mutex_t	talk;
	pthread_mutex_t	food;
	pthread_mutex_t clock;
}				t_rules;

typedef struct s_fork_master
{
	int				nb_fork;
	int				*status;
	int				turn;
	int				turn_mode;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
}				t_fm;

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
	t_rules		*ruleset;
	t_fm		*forkmaster;
	t_death		*death;
}				t_philo;

//forkmaster
int			forkmaster_ask(t_philo *self);
void		forkmaster_tell(t_philo *self);
void		forkmaster_del(t_fm *forkmaster);
t_fm		*forkmaster_create(int n);

//msg
void		msg_put(t_philo *self, long int ms, char *msg);

//philo
int			philo_are_fat(t_rules *ruleset);
void		philo_use_fork(t_philo *self, int use);
void		philo_letgo(t_philo *self);
int			philo_is_alive(t_philo *self);
void		philo_wait(t_philo *self, long int tm_to_wait);
void		*philo_run(void *self_ptr);
int			philo_loop(t_philo *self);

//room
int			room_play(t_rules *ruleset);

//utils
int			ft_atoi(const char *nptr);
int			ft_strisnb(const char *str);
int			ft_strlen(const char *str);
long int	time_getstamp(pthread_mutex_t *clock);
char		*ft_strnstr(const char *big, const char *little, size_t len);

#endif
