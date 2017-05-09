#include <fcntl.h>
#include <unistd.h>
#include <sys/cdefs.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include "../libft/includes/libft.h"

#define PTMX "/dev/ptmx"
#define BUF_SIZE 1024

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

void	open_new_shell(int argc, char **argv, char **environ)
{
	pid_t	father;
	int		status;
	char	*tab[] = {"/bin/zsh", NULL};
	char	buff;
	int		fd_ptmx;
	int		fd_pts;
	int		new_fd;
	struct timeval	*tvp;
	int		ret_select;
	int		ret_read;
	fd_set	rfd;
	char	buffer_in[BUFSIZ];
	char	buffer_out[BUFSIZ];
	struct termios	rtt;
	struct termios	stt;
	struct termios	tt;
	int		tty_flag;
	struct winsize	win;

	tvp = NULL;
	if (tty_flag == ft_isatty(STDIN_FILENO))
	{
		if (ft_tcgetattr(STDIN_FILENO, &tt) == -1)
		{
			ft_dprintf(2, "Error ft_tcgetattr : %s\n", strerror(errno));
			exit(-1);
		}
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win) == -1)
		{
			ft_dprintf(2, "Error ioctl : %s\n", strerror(errno));
			exit(-1);
		}
	}
	if (tty_flag)
	{
		rtt = tt;
		ft_cfmakeraw(&rtt);
		rtt.c_cflag &= ~ECHO;
		(void)ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &rtt);
	}
	/*dprintf(1, GREEN"2) dup STDIN_FILENO Success\n"END);*/
	if ((father = fork()) < 0)
		dprintf(2, "Fork failure : %s\n", strerror(errno));
	else if (father > 0)
	{
		if (argc == 1)
			dprintf(1, "Script started, file is typescript\n");
		else
			dprintf(1, "Script started, file is %s\n", argv[1]);
		if ((fd_ptmx = ft_openpt(O_RDWR)) == -1)
		{
			ft_dprintf(1, RED"======Open PTMX Failure=======\n"END);
			exit(EXIT_FAILURE);
		}
		ft_dprintf(1, GREEN"Open PTMX Success\n"END);

		if (ft_grantpt(fd_ptmx) == -1)
			ft_dprintf(1, RED"Grantpt Failure : %s\n"END, strerror(errno));
		if (ft_unlockpt(fd_ptmx) == -1)
			ft_dprintf(1, RED"Unlockpt Failure : %s\n"END, strerror(errno));
		ft_dprintf(1, GREEN"Grantpt and Unlockpt Success\n"END);

		ft_dprintf(1, CYAN"Slave name = %s\n"END, ft_ptsname(fd_ptmx));

		if ((fd_pts = open(ft_ptsname(fd_ptmx), O_RDWR)) == -1)
		{
			ft_dprintf(1, RED"======Open PTS Failure=======\n"END);
			exit(EXIT_FAILURE);
		}
		ft_dprintf(1, GREEN"Open PTS Success\n"END);
		/*if ((new_fd = dup2(0, fd_pts)) == -1)*/
		/*{*/
		/*ft_dprintf(1, RED"======dup2 Failure=======\n"END);*/
		/*exit(EXIT_FAILURE);*/
		/*}*/
		/*ft_dprintf(1, GREEN"dup2 Success\n"END);*/


		FD_ZERO(&rfd);
		while (0xDEADBEEF)
		{
			memset(&buffer_in, 0, BUFSIZ);
			memset(&buffer_out, 0, BUFSIZ);
			ft_dprintf(1, GREEN"Memset buffers Success\n"END);
			FD_SET(fd_ptmx, &rfd);
			FD_SET(STDIN_FILENO, &rfd);
			ret_select = select(fd_ptmx + 1, &rfd, 0, 0, tvp);
			if (ret_select < 0 && errno != EINTR)
			{
				ft_dprintf(1, ORANGE"EINTR error : %s\n"END, strerror(errno));
				break ;
			}
			ft_dprintf(1, GREEN"Select Success\n"END);
			if (ret_select > 0 && FD_ISSET(STDIN_FILENO, &rfd))
			{
				ret_read = read(STDIN_FILENO, buffer_in, BUFSIZ);
				if (ret_read < 0)
				{
					ft_dprintf(1, ORANGE"read STDIN_FILENO error\n"END);
					break ;
				}
				if (ret_read == 0)
					(void)write(fd_ptmx, buffer_in, 0);
				if (ret_read > 0)
				{
					write(fd_ptmx, buffer_in, ret_read);
				}
				ft_dprintf(1, GREEN"Write fd_ptmx > buffer_in Success\n"END);
			}
			if (ret_select > 0 && FD_ISSET(fd_ptmx, &rfd))
			{
				ret_read = read(fd_ptmx, buffer_out, sizeof(buffer_out));
				if (ret_read < 0)
				{
					ft_dprintf(1, ORANGE"buffer_out error\n"END);
					break ;
				}
				write(STDOUT_FILENO, buffer_out, ret_read);
				ft_dprintf(1, GREEN"Write STDOUT_FILENO > buffer_out Success\n"END);
			}
		}
		kill(father, SIGTERM);
		ft_dprintf(1, PURPLE"KILL Success\n"END);
	}
	else
	{
		ft_dprintf(1, PURPLE"fd_pts = %d\n"END, fd_pts);
		ft_login_tty(fd_pts);
		if (execve(tab[0], &tab[0], environ) < 0)
			dprintf(2, "Execeve failure : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (argc == 1)
		dprintf(1, "Script done, file is typescript\n");
	else
		dprintf(1, "Script done, file is %s\n", argv[1]);
}

int	main(int argc, char **argv, char	 **environ)
{
	int	fd;

	if (access("./typescript", F_OK) == 0)
		fd = open("./typescript", O_APPEND | O_RDWR);
	else
		fd = open("./typescript", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		ft_dprintf(2, "Open failure %s\n", strerror(errno));
	ft_dprintf(1, GREEN"Open typescript success\n"END);

	open_new_shell(argc, argv, environ);
}
