/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 15:55:32 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 18:53:36 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	rule_set_init_ff(t_rules *rs)
{
	int	i;

	rs->free_fork = (int *)malloc(sizeof(int) * rs->nb_philo);
	if (!rs->free_fork)
		return (0);
	i = 0;
	while (i < rs->nb_philo)
		rs->free_fork[i++] = 0;
	rs->ff = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * rs->nb_philo);
	if (!rs->ff)
		return (free_elem(rs->free_fork));
	i = 0;
	while (i < rs->nb_philo)
	{
		if (pthread_mutex_init(&rs->ff[i], NULL) != 0)
		{
			while (--i)
				pthread_mutex_destroy(&rs->ff[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

t_rules	*rule_set_init(t_rules *ruleset)
{
	int	i;

	ruleset->meals = (int *)malloc(sizeof(int) * ruleset->nb_philo);
	if (!ruleset->meals)
		return (0);
	i = 0;
	while (i < ruleset->nb_philo)
		ruleset->meals[i++] = 0;
	if (pthread_mutex_init(&ruleset->talk, NULL) != 0)
		return (free_elem(ruleset->meals));
	if (pthread_mutex_init(&ruleset->food, NULL) != 0)
	{
		pthread_mutex_destroy(&ruleset->talk);
		return (free_elem(ruleset->meals));
	}
	if (!rule_set_init_ff(ruleset))
	{
		pthread_mutex_destroy(&ruleset->talk);
		return (free_elem(ruleset->meals));
	}
	return (ruleset);
}

t_rules	*rules_set(char **argv)
{
	t_rules	*ruleset;

	ruleset = (t_rules *)malloc(sizeof(t_rules));
	if (!ruleset)
		return (0);
	ruleset->nb_philo = ft_atoi(argv[1]);
	ruleset->tm_to_die = ft_atoi(argv[2]);
	ruleset->tm_to_eat = ft_atoi(argv[3]);
	ruleset->tm_to_sleep = ft_atoi(argv[4]);
	ruleset->start_time = 0;
	if (!argv[5])
		ruleset->nb_eat_to_end = -1;
	else
		ruleset->nb_eat_to_end = ft_atoi(argv[5]);
	ruleset->forks = forkmaster_create(ruleset->nb_philo);
	if (!ruleset->forks)
		return (free_elem(ruleset));
	if (!rule_set_init(ruleset))
	{
		forkmaster_del(ruleset->forks, ruleset->nb_philo);
		return (free_elem(ruleset));
	}
	return (ruleset);
}

int	check_input(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("%s: wrong number of arguments\n", argv[0]);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (!ft_strisnb(argv[i]))
		{
			printf("%s: %s: arguments must be numbers\n", argv[0], argv[i]);
			return (0);
		}
		i++;
	}
	if (ft_atoi(argv[1]) <= 0)
	{
		printf("%s: number_of_philosopher must be at leat 1\n", argv[0]);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_rules	*ruleset;

	if (!check_input(argc, argv))
		return (1);
	ruleset = rules_set(argv);
	if (!ruleset)
		return (1);
	room_play(ruleset);
	return (0);
}
