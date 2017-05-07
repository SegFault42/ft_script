/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_2d_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/21 19:21:22 by rabougue          #+#    #+#             */
/*   Updated: 2016/09/23 22:19:24 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

void	ft_print_2d_tab_no_nl(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		ft_dprintf(1, "(NULL)\n");
	else
	{
		while (tab[i])
		{
			ft_dprintf(1, "%s", tab[i]);
			++i;
			if (tab[i] != NULL)
				ft_putchar_fd(' ', 1);
		}
		ft_putchar_fd('\n', 1);
	}
}
