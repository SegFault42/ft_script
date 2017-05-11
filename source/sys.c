/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 19:45:43 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/11 21:57:55 by rabougue         ###   ########.fr       */
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
