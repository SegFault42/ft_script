/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 15:14:04 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/10 05:29:18 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

void	ft_critical_error(char *error)
{
	ft_dprintf(2, RED"%s : %s \n"END, error, strerror(errno));
	exit(EXIT_FAILURE);
}
