/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 19:45:43 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/09 19:48:08 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

int	ft_tcsetattr(int fd, int opt, const struct termios *t)
{
	struct termios localterm;

	if (opt & TCSASOFT) {
		localterm = *t;
		localterm.c_cflag |= CIGNORE;
		t = &localterm;
	}
	switch (opt & ~TCSASOFT) {
		case TCSANOW:
			return (ioctl(fd, TIOCSETA, t));
		case TCSADRAIN:
			return (ioctl(fd, TIOCSETAW, t));
		case TCSAFLUSH:
			return (ioctl(fd, TIOCSETAF, t));
		default:
			errno = EINVAL;
			return (-1);
	}
}

void	ft_cfmakeraw(struct termios *t)
{
	t->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	t->c_oflag &= ~OPOST;
	t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	t->c_cflag &= ~(CSIZE|PARENB);
	t->c_cflag |= CS8;
}

int	ft_tcgetattr(int fd, struct termios *t)
{
	return (ioctl(fd, TIOCGETA, t));
}

int	ft_isatty(int fd)
{
	struct termios	t;

	return(ft_tcgetattr(fd, &t) != -1);
}

int ft_login_tty(int fd)
{
	(void)setsid();
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
	(void) dup2(fd, 0);
	(void) dup2(fd, 1);
	(void) dup2(fd, 2);
	if (fd > 2)
		(void) close(fd);
	return (0);
}

int	ft_unlockpt(int fd)
{
	return ioctl(fd, TIOCPTYUNLK);
}

int	ft_openpt(int flags)
{
	int fd = open("/dev/ptmx", flags);
	if (fd >= 0)
		return (fd);
	return -1;
}

int	ft_grantpt(int fd)
{
	return (ioctl(fd, TIOCPTYGRANT));
}

char	*ft_ptsname(int fd)
{
	static char	ptsnamebuf[ 128];
	int			error;
	char		*retval = NULL;
	struct stat	sbuf;

	error = ioctl(fd, TIOCPTYGNAME, ptsnamebuf);
	if (!error)
	{
		if (stat(ptsnamebuf, &sbuf) == 0)
			retval = ptsnamebuf;
	}
	return (retval);
}
