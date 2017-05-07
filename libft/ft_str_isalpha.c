/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_isalpha.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/19 17:54:57 by rabougue          #+#    #+#             */
/*   Updated: 2017/04/19 17:55:01 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

bool	ft_str_isalpha(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] < 'A' && str[i] > 'Z') || (str[i] < 'a' && str[i] > 'z'))
			return (false);
		++i;
	}
	return (true);
}
