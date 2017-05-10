#include "../include/script.h"

void	critical_error(char *error)
{
	write(2, error, ft_strlen(error));
	exit(EXIT_FAILURE);
}

void	create_shell(int fd_ptmx, int fd_pts, char **tab, char **environ)
{
	if (close(fd_ptmx) == -1)
		critical_error("close ptmx failure\n");
	if (ft_login_tty(fd_pts))
		critical_error("login_tty failure\n");
	if (execve(tab[0], &tab[0], environ) < 0)
		write(2, "Execeve failure\n", 16);
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

void	init_tty(t_script *var_script)
{
	if ((var_script->tty_flag = ft_isatty(STDIN_FILENO)))
	{
		if (ft_tcgetattr(STDIN_FILENO, &var_script->tt) == -1)
			critical_error("Error ft_tcgetattr\n");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &var_script->win) == -1)
			critical_error("Error ioctl\n");
		ft_openpty(&var_script->fd_ptmx, &var_script->fd_pts);
	}
	else
		ft_openpty(&var_script->fd_ptmx, &var_script->fd_pts);
}

void	make_terminal_raw(t_script *var_script)
{
	if (var_script->tty_flag)
	{
		var_script->rtt = var_script->tt;
		ft_cfmakeraw(&var_script->rtt);
		var_script->rtt.c_cflag &= ~ECHO;
		ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &var_script->rtt);
	}
}

void	fork_shell(t_script *var_script, char **tab, char **environ)
{
	if ((var_script->father = fork()) < 0)
	{
		write(2, "Fork failure\n", 13);
		exit(EXIT_FAILURE);
	}
	if (var_script->father == 0)
	{
		create_shell(var_script->fd_ptmx, var_script->fd_pts, tab, environ);
		close(var_script->fd_pts);
	}
}

void	re_init_sub_shell(t_script *var_script)
{
	memset(&var_script->buffer_in, 0, BUFSIZ);
	memset(&var_script->buffer_out, 0, BUFSIZ);
	FD_SET(var_script->fd_ptmx, &var_script->rfd);
	FD_SET(STDIN_FILENO, &var_script->rfd);
}

void	script(int argc, char **argv, char **environ, int fd_typescript)
{
	char	*tab[] = {"/bin/zsh", NULL};
	t_script	var_script;

	ft_memset(&var_script, 0, sizeof(t_script));
	init_tty(&var_script);
	make_terminal_raw(&var_script);
	fork_shell(&var_script, tab , environ);
	FD_ZERO(&var_script.rfd);
	while (0xDEADBEEF)
	{
		re_init_sub_shell(&var_script);
		var_script.ret_select = select(var_script.fd_ptmx + 1, &var_script.rfd, 0, 0, var_script.tvp);
		if (var_script.ret_select < 0 && errno != EINTR)
		{
			write(2, "EINTR error\n", 12);
			break ;
		}
		if (var_script.ret_select > 0 && FD_ISSET(STDIN_FILENO, &var_script.rfd))
		{
			var_script.ret_read = read(STDIN_FILENO, var_script.buffer_in, BUFSIZ);
			if (var_script.ret_read < 0)
			{
				write(2, "read STDIN_FILENO error\n", 24);
				break ;
			}
			if (var_script.ret_read == 0)
			{
				write(var_script.fd_ptmx, var_script.buffer_in, 0);
				break ;
			}
			if (var_script.ret_read > 0)
				write(var_script.fd_ptmx, var_script.buffer_in, var_script.ret_read);
		}
		if (var_script.ret_select > 0 && FD_ISSET(var_script.fd_ptmx, &var_script.rfd))
		{
			var_script.ret_read = read(var_script.fd_ptmx, var_script.buffer_out, sizeof(var_script.buffer_out));
			if (var_script.ret_read < 0)
			{
				write(2, "buffer_out error\n", 17);
				break ;
			}
			if (var_script.ret_read == 0)
				break ;
			write(STDOUT_FILENO, var_script.buffer_out, var_script.ret_read);
			write(fd_typescript, var_script.buffer_out, var_script.ret_read);
		}
	}
	kill(var_script.father, SIGTERM);
	if (var_script.tty_flag)
		ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &var_script.tt);
}

static int	create_typescript()
{
	int	fd;

	if (access("./typescript", F_OK) == 0)
		fd = open("./typescript", O_APPEND | O_RDWR);
	else
		fd = open("./typescript", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		critical_error("Open failure\n");
	return (fd);
}

int	main(int argc, char **argv, char **environ)
{
	int	fd_typescript;

	fd_typescript = create_typescript();

	write(1, "Script started, output file is typescript\n", 42);
	write(fd_typescript, "Script started on ", 18);
	print_time(fd_typescript);

		script(argc, argv, environ, fd_typescript);

	write(1, "\nScript done, output file is typescript\n", 41);
	write(fd_typescript, "\nScript done on ", 16);
	print_time(fd_typescript);
}
