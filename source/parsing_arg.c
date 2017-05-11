/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 23:40:52 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/11 00:49:46 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

extern t_argp g_argp[];

static void	usage(char option)
{
	const char*error = "usage: script [-adkpqr] [-t time] [file [command ...]]";
	const char*error_2 = "[file	 ...]";

	ft_dprintf(STDERR_FILENO, "script: illegal option -- %c\n%s%s\n", option,
			error, error_2);
	exit(EXIT_FAILURE);
}

static void	check_if_option_exist(char *argument)
{
	int	i;
	int	j;
	int	ok;

	i = 1;
	while (argument[i])
	{
		ok = 0;
		j = 0;
		while (g_argp[j].sign)
		{
			if (g_argp[j].sign[0] == argument[i])
			{
				if (ft_strstr(argument, "--") != 0 && ft_strlen(argument) > 2)
					usage(argument[i]);
				ok = 1;
				break ;
			}
			++j;
		}
		if (ok == 0)
			usage(argument[i]);
		++i;
	}
}

void	get_option(char **argv)
{
	int		arg;
	uint8_t	i;

	arg = 1;
	while (argv[arg])
	{
		i = 0;
		if (argv[arg][0] == '-' && g_argp[OPT_END].active == 0)
		{
			check_if_option_exist(argv[arg]);
			while (g_argp[i].sign)
			{
				if (ft_strstr(argv[arg], g_argp[i].sign) != NULL)
					g_argp[i].active = 1;
				++i;
			}
		}
		++arg;
	}
}
