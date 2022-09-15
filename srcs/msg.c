/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiraude <agiraude@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 11:03:47 by agiraude          #+#    #+#             */
/*   Updated: 2022/09/14 18:53:21 by agiraude         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;

	if (!(*needle))
		return ((char *)haystack);
	while (*haystack)
	{
		if (*haystack == *needle)
		{
			i = 0;
			while (needle[i] && haystack[i] == needle[i])
				i++;
			if (needle[i] == 0)
				return ((char *)haystack);
		}
		haystack++;
	}
	return (0);
}

void	msg_put(t_philo *self, long int ms, char *msg)
{
	static int	keep_going = 1;

	pthread_mutex_lock(&self->ruleset->talk);
	if (!keep_going)
	{
		pthread_mutex_unlock(&self->ruleset->talk);
		return ;
	}
	printf("%ld %d %s\n", ms, self->id, msg);
	if (ft_strstr(msg, "died"))
		keep_going = 0;
	pthread_mutex_unlock(&self->ruleset->talk);
}
