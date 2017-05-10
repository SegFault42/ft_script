#include "../include/script.h"

void	create_shell(int fd_ptmx, int fd_pts, char **tab, char **environ)
{
	if (close(fd_ptmx) == -1)
		ft_critical_error("close ptmx failure");
	if (ft_login_tty(fd_pts))
		ft_critical_error("login_tty failure");
	if (execve(tab[0], &tab[0], environ) < 0)
		dprintf(2, RED"Execeve failure : %s\n"END, strerror(errno));
	exit(EXIT_FAILURE);
}

void	print_time(int fd)
{
	time_t	second;

	second = time(NULL);
	ft_dprintf(fd, "%s", ctime(&second));
}

void	open_new_shell(int argc, char **argv, char **environ, int fd_typescript)
{
	pid_t	father;
	int		status = 0;
	char	*tab[] = {"/bin/zsh", NULL};
	char	buff = 0;
	int		fd_ptmx = 0;
	int		fd_pts = 0;
	int		new_fd = 0;
	struct timeval	*tvp;
	int		ret_select = 0;
	int		ret_read = 0;
	fd_set	rfd;
	char	buffer_in[BUFSIZ] = {0};
	char	buffer_out[BUFSIZ] = {0};
	struct termios	rtt;
	struct termios	stt;
	struct termios	tt;
	struct winsize	win;
	int		tty_flag = 0;

	tvp = NULL;
	memset(&rtt, 0, sizeof(struct termios));
	memset(&stt, 0, sizeof(struct termios));
	memset(&tt, 0, sizeof(struct termios));
	memset(&win, 0, sizeof(struct winsize));
	memset(&rfd, 0, sizeof(fd_set));
	ft_dprintf(1, "Script started, output file is typescript\n");
	ft_dprintf(fd_typescript, "Script started on ");
	print_time(fd_typescript);
	if ((tty_flag = ft_isatty(STDIN_FILENO)))
	{
		if (ft_tcgetattr(STDIN_FILENO, &tt) == -1)
			ft_critical_error("Error ft_tcgetattr");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win) == -1)
			ft_critical_error("Error ioctl");
		ft_openpty(&fd_ptmx, &fd_pts);
	}
	else
		ft_openpty(&fd_ptmx, &fd_pts);
	if (tty_flag)
	{
		rtt = tt;
		ft_cfmakeraw(&rtt);
		rtt.c_cflag &= ~ECHO;
		ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &rtt);
	}
	if ((father = fork()) < 0)
	{
		dprintf(2, "Fork failure : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (father == 0)
	{
		create_shell(fd_ptmx, fd_pts, tab, environ);
		close(fd_pts);
	}
		FD_ZERO(&rfd);
		while (0xDEADBEEF)
		{
			memset(&buffer_in, 0, BUFSIZ);
			memset(&buffer_out, 0, BUFSIZ);
			/*ft_dprintf(1, GREEN"Memset buffers Success\n"END);*/
			FD_SET(fd_ptmx, &rfd);
			FD_SET(STDIN_FILENO, &rfd);
			ret_select = select(fd_ptmx + 1, &rfd, 0, 0, tvp);
			if (ret_select < 0 && errno != EINTR)
			{
				ft_dprintf(1, ORANGE"EINTR error : %s\n"END, strerror(errno));
				break ;
			}
			/*ft_dprintf(1, GREEN"Select Success\n"END);*/
			if (ret_select > 0 && FD_ISSET(STDIN_FILENO, &rfd))
			{
				ret_read = read(STDIN_FILENO, buffer_in, BUFSIZ);
				if (ret_read < 0)
				{
					ft_dprintf(1, ORANGE"read STDIN_FILENO error\n"END);
					break ;
				}
				if (ret_read == 0)
				{
					(void)write(fd_ptmx, buffer_in, 0);
					break ;
				}
				if (ret_read > 0)
				{
					write(fd_ptmx, buffer_in, ret_read);
				}
				/*ft_dprintf(1, GREEN"Write fd_ptmx > buffer_in Success\n"END);*/
			}
			if (ret_select > 0 && FD_ISSET(fd_ptmx, &rfd))
			{
				ret_read = read(fd_ptmx, buffer_out, sizeof(buffer_out));
				if (ret_read < 0)
				{
					ft_dprintf(1, ORANGE"buffer_out error\n"END);
					break ;
				}
				if (ret_read == 0)
					break ;
				write(STDOUT_FILENO, buffer_out, ret_read);
				write(fd_typescript, buffer_out, ret_read);
				/*ft_dprintf(1, GREEN"Write STDOUT_FILENO > buffer_out Success\n"END);*/
			}
		}
	kill(father, SIGTERM);
	if (tty_flag)
		ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &tt);
	ft_dprintf(1, "\nScript done, output file is typescript\n");
	ft_dprintf(fd_typescript, "\nScript done on ");
	print_time(fd_typescript);
}

static int	create_typescript()
{
	int	fd;

	if (access("./typescript", F_OK) == 0)
		fd = open("./typescript", O_APPEND | O_RDWR);
	else
		fd = open("./typescript", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		ft_dprintf(2, "Open failure %s\n", strerror(errno));
	return (fd);
}

int	main(int argc, char **argv, char	 **environ)
{
	int	fd_typescript;

	fd_typescript = create_typescript();
	open_new_shell(argc, argv, environ, fd_typescript);
}
