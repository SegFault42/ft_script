/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 21:11:00 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/11 21:58:14 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

void	get_cmd(char **argv, char *cmd)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		ft_strcat(cmd, argv[i]);
		ft_strcat(cmd, " ");
		++i;
	}
}

int		get_name_file(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] != '-')
		{
			if (ft_strlen(argv[i]) > 255)
				return (-1);
			return (i);
		}
		++i;
	}
	return (0);
}

void	critical_error(char *error)
{
	write(2, error, ft_strlen(error));
	exit(EXIT_FAILURE);
}

void	print_time(int fd)
{
	time_t	second;
	char	*s_time;

	second = time(NULL);
	s_time = ctime(&second);
	write(fd, s_time, ft_strlen(s_time));
}
