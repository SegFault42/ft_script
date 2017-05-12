/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 19:45:43 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/12 03:40:20 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

int		ft_login_tty(int fd)
{
	(void)setsid();
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
	(void)dup2(fd, 0);
	(void)dup2(fd, 1);
	(void)dup2(fd, 2);
	if (fd > 2)
		(void)close(fd);
	return (0);
}

int		ft_unlockpt(int fd)
{
	return (ioctl(fd, TIOCPTYUNLK));
}

int		ft_openpt(int flags)
{
	int	fd;

	fd = open("/dev/ptmx", flags);
	if (fd >= 0)
		return (fd);
	return (-1);
}

int		ft_grantpt(int fd)
{
	return (ioctl(fd, TIOCPTYGRANT));
}

char	*ft_ptsname(int fd)
{
	static char	ptsnamebuf[128];
	int			error;
	char		*retval;
	struct stat	sbuf;

	retval = NULL;
	error = ioctl(fd, TIOCPTYGNAME, ptsnamebuf);
	if (!error)
	{
		if (stat(ptsnamebuf, &sbuf) == 0)
			retval = ptsnamebuf;
	}
	return (retval);
}
