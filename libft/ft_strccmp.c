/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strccmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/28 18:54:55 by rabougue          #+#    #+#             */
/*   Updated: 2016/11/19 22:17:12 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

int	ft_strccmp(char *s1, char *s2, char c)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] != c)
	{
		if (s1[i] != s2[i])
			return (false);
		++i;
	}
	return (true);
}
