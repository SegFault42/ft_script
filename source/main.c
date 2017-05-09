#include "../include/script.h"

void	create_shell(int fd_ptmx, int fd_pts, char **tab, char **environ)
{
	if (close(fd_ptmx) == -1)
		ft_critical_error("close ptmx failure");
	ft_dprintf(1, PURPLE"fd_pts = %d\n"END, fd_pts);
	if (ft_login_tty(fd_pts))
		ft_critical_error("login_tty failure");
	if (execve(tab[0], &tab[0], environ) < 0)
		dprintf(2, "Execeve failure : %s\n", strerror(errno));
	exit(EXIT_FAILURE);
}

void	open_new_shell(int argc, char **argv, char **environ, int fd_typescript)
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
	if ((tty_flag = ft_isatty(STDIN_FILENO)))
	{
		if (ft_tcgetattr(STDIN_FILENO, &tt) == -1)
			ft_critical_error("Error ft_tcgetattr");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win) == -1)
			ft_critical_error("Error ioctl");
		if (openpty(&fd_ptmx, &fd_pts, NULL, &tt, &win) == -1)
			ft_critical_error("Error openpty");
	}
	else
	{
		if (openpty(&fd_ptmx, &fd_pts, NULL, NULL, NULL) == -1)
			ft_critical_error("Error openpty");
		dprintf(1, GREEN"Openpty Success\n"END);
	}
	if (tty_flag)
	{
		rtt = tt;
		ft_cfmakeraw(&rtt);
		rtt.c_cflag &= ~ECHO;
		(void)ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &rtt);
		dprintf(1, GREEN"ft_tcsetattr Success\n"END);
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
		if (argc == 1)
			dprintf(1, "Script started, file is typescript\n");
		else
			dprintf(1, "Script started, file is %s\n", argv[1]);
		/*if ((fd_ptmx = ft_openpt(O_RDWR)) == -1)*/
		/*{*/
			/*ft_dprintf(1, RED"======Open PTMX Failure=======\n"END);*/
			/*exit(EXIT_FAILURE);*/
		/*}*/
		/*ft_dprintf(1, GREEN"Open PTMX Success\n"END);*/

		/*if (ft_grantpt(fd_ptmx) == -1)*/
			/*ft_dprintf(1, RED"Grantpt Failure : %s\n"END, strerror(errno));*/
		/*if (ft_unlockpt(fd_ptmx) == -1)*/
			/*ft_dprintf(1, RED"Unlockpt Failure : %s\n"END, strerror(errno));*/
		/*ft_dprintf(1, GREEN"Grantpt and Unlockpt Success\n"END);*/

		/*ft_dprintf(1, CYAN"Slave name = %s\n"END, ft_ptsname(fd_ptmx));*/

		/*if ((fd_pts = open(ft_ptsname(fd_ptmx), O_RDWR)) == -1)*/
		/*{*/
			/*ft_dprintf(1, RED"======Open PTS Failure=======\n"END);*/
			/*exit(EXIT_FAILURE);*/
		/*}*/
		/*ft_dprintf(1, GREEN"Open PTS Success\n"END);*/
		/*[>if ((new_fd = dup2(0, fd_pts)) == -1)<]*/
		/*[>{<]*/
		/*[>ft_dprintf(1, RED"======dup2 Failure=======\n"END);<]*/
		/*[>exit(EXIT_FAILURE);<]*/
		/*[>}<]*/
		/*[>ft_dprintf(1, GREEN"dup2 Success\n"END);<]*/


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
				write(fd_typescript, buffer_out, ret_read);
				ft_dprintf(1, GREEN"Write STDOUT_FILENO > buffer_out Success\n"END);
			}
		kill(father, SIGTERM);
		ft_dprintf(1, PURPLE"KILL Success\n"END);
	}
	if (argc == 1)
		dprintf(1, "Script done, file is typescript\n");
	else
		dprintf(1, "Script done, file is %s\n", argv[1]);
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
	ft_dprintf(1, GREEN"Open typescript success\n"END);

	return (fd);
}

int	main(int argc, char **argv, char	 **environ)
{
	int	fd_typescript;

	fd_typescript = create_typescript();
	open_new_shell(argc, argv, environ, fd_typescript);
}
