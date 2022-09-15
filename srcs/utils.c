/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 15:57:46 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/15 12:45:09 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;

	if (!nptr)
		return (0);
	result = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			sign = -1;
	while (*nptr >= '0' && *nptr <= '9')
		result = (*nptr++ - '0') + result * 10;
	return (result * sign);
}

int	ft_strisnb(const char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != ' ' && !(*str >= '0' && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

long int	time_getstamp(pthread_mutex_t *clock)
{
	struct timeval	tm;
	static long int	start_ms = 0;
	long int		ms;

	pthread_mutex_lock(clock);
	gettimeofday(&tm, NULL);
	pthread_mutex_unlock(clock);
	ms = tm.tv_sec * 1000 + tm.tv_usec / 1000;
	if (start_ms == 0)
		start_ms = ms;
	return (ms - start_ms);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (!(*little))
		return (0);
	while (*big && len > 0)
	{
		if (*big == *little)
		{
			i = 0;
			while (little[i] && big[i] == little[i] && i < len)
				i++;
			if (little[i] == '\0')
				return ((char *)big);
		}
		big++;
		len--;
	}
	return (0);
}
