#include "../include/script.h"

t_argp	g_argp[] =
{
	{"a", 0, "Append the output to file or typescript, retaining the prior contents."},
	{"d", 0, "a"},
	{"F", 0, "a"},
	{"p", 0, "a"},
	{"q", 0, "a"},
	{"r", 0, "a"},
	{0, 0, 0}
};

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

void	reset_terminal(t_script *var_script)
{
	kill(var_script->father, SIGTERM);
	if (var_script->tty_flag)
		ft_tcsetattr(STDIN_FILENO, TCSAFLUSH, &var_script->tt);
}

bool	select_stdin(t_script *var_script)
{
	if (var_script->ret_select > 0 && FD_ISSET(STDIN_FILENO, &var_script->rfd))
	{
		var_script->ret_read = read(STDIN_FILENO, var_script->buffer_in,
				BUFSIZ);
		if (var_script->ret_read < 0)
			return (true);
		if (var_script->ret_read == 0)
			return (true);
		if (var_script->ret_read > 0)
			write(var_script->fd_ptmx, var_script->buffer_in,
					var_script->ret_read);
	}
	return (false);
}

bool	select_ptmx(t_script *var_script, int fd_typescript)
{
	if (var_script->ret_select > 0 &&
			FD_ISSET(var_script->fd_ptmx, &var_script->rfd))
	{
		var_script->ret_read = read(var_script->fd_ptmx, var_script->buffer_out,
				sizeof(var_script->buffer_out));
		if (var_script->ret_read < 0)
			return (true);
		if (var_script->ret_read == 0)
			return (true);
		write(STDOUT_FILENO, var_script->buffer_out, var_script->ret_read);
		write(fd_typescript, var_script->buffer_out, var_script->ret_read);
	}
	return (false);
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
		var_script.ret_select = select(var_script.fd_pts, &var_script.rfd, 0, 0,
				var_script.tvp);
		if (var_script.ret_select < 0 && errno != EINTR)
			break ;
		if (select_stdin(&var_script) == true)
			break ;
		if (select_ptmx(&var_script, fd_typescript) == true)
			break ;
	}
	reset_terminal(&var_script);
}

int		create_typescript(char *name)
{
	int	fd;

	if (access(name, F_OK) == 0)
		fd = open(name, O_TRUNC | O_RDWR);
	else
		fd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR |
				S_IRGRP | S_IROTH);
	if (fd == -1)
		critical_error("Open failure\n");
	return (fd);
}

int		get_name_file(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] != '-')
			return (i);
		++i;
	}
	return (0);
}

void	write_start_or_end(char *name, int fd_typescript, int nb)
{
	if (nb == 0)
	{
		write(1, "Script started, output file is ", 31);
		write(1, name, ft_strlen(name));
		write(1, "\n", 1);
		write(fd_typescript, "Script started on ", 18);
	}
	else
	{
		write(1, "\nScript done, output file is ", 30);
		write(1, name, ft_strlen(name));
		write(1, "\n", 1);
		write(fd_typescript, "\nScript done on ", 16);
	}
	print_time(fd_typescript);
}

int		main(int argc, char **argv, char **environ)
{
	int	fd_typescript;
	int	i;
	static char	name[256] = {0};

	memset(&name, 0, 256);
	get_option(argv);
	i = get_name_file(argv);
		ft_strcat(name, i != 0 ? argv[i] : "typescript");
	fd_typescript = create_typescript(name);
	write_start_or_end(name, fd_typescript, 0);
	script(argc, argv, environ, fd_typescript);
	write_start_or_end(name, fd_typescript, 1);

}
