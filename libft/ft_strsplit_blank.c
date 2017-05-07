/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 13:39:24 by rabougue          #+#    #+#             */
/*   Updated: 2017/04/19 17:38:57 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"

static size_t	count_nb_tab(char const *s)
{
	size_t	i;
	size_t	nb_tab;

	i = 0;
	nb_tab = 0;
	while (s[i])
	{
		while ((s[i] == ' ' || s[i] == '\t') && s[i] != '\0')
			++i;
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\0')
		{
			++nb_tab;
			while (s[i] != ' ' && s[i] != '\t' && s[i] != '\0')
				++i;
		}
	}
	return (nb_tab);
}

static void		fill_tab(char **tab, char const *str)
{
	size_t	len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		len = 0;
		while (str[i] == '\t' || str[i] == ' ')
			++i;
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\0')
		{
			while (str[i + len] != ' ' && str[i + len] != '\t'
			&& str[i + len] != '\0')
				++len;
			tab[j] = ft_strsub(str, i, len);
			++j;
		}
		i += len;
	}
	tab[j] = NULL;
}

char			**ft_strsplit_blank(char const *s)
{
	char	**str;
	size_t	nb_tab;

	if (s == NULL)
		return (NULL);
	nb_tab = count_nb_tab(s);
	str = (char **)ft_memalloc(sizeof(char *) * nb_tab + 1);
	if (str != NULL)
	{
		fill_tab(str, s);
	}
	return (str);
}
