/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 03:39:37 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/12 03:40:59 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

void	ft_openpty(int *fd_ptmx, int *fd_pts, struct winsize *win)
{
	if ((*fd_ptmx = ft_openpt(O_RDWR)) == -1)
		ft_critical_error("Error openpt");
	ft_grantpt(*fd_ptmx);
	ft_unlockpt(*fd_ptmx);
	if ((*fd_pts = open(ft_ptsname(*fd_ptmx), O_RDWR)) == -1)
		ft_critical_error("Error open pts");
	ft_grantpt(*fd_pts);
	ft_unlockpt(*fd_pts);
	ioctl(*fd_pts, TIOCSWINSZ, (char *)win);
}

int		ft_tcsetattr(int fd, int opt, const struct termios *t)
{
	struct termios	localterm;

	if (opt & TCSASOFT)
	{
		localterm = *t;
		localterm.c_cflag |= CIGNORE;
		t = &localterm;
	}
	if ((opt & ~TCSASOFT) == TCSANOW)
		return (ioctl(fd, TIOCSETA, t));
	else if ((opt & ~TCSASOFT) == TCSADRAIN)
		return (ioctl(fd, TIOCSETAW, t));
	else if ((opt & ~TCSASOFT) == TCSAFLUSH)
		return (ioctl(fd, TIOCSETAF, t));
	else
	{
		errno = EINVAL;
		return (-1);
	}
}

void	ft_cfmakeraw(struct termios *t)
{
	t->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL |
			IXON);
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	t->c_cflag &= ~(CSIZE | PARENB);
	t->c_cflag |= CS8;
}

int		ft_tcgetattr(int fd, struct termios *t)
{
	return (ioctl(fd, TIOCGETA, t));
}

int		ft_isatty(int fd)
{
	struct termios	t;

	return (ft_tcgetattr(fd, &t) != -1);
}
