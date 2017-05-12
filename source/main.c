/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 21:43:16 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/12 03:36:03 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

t_argp	g_argp[] =
{
	{"a", 0, {0}},
	{"d", 0, {0}},
	{"F", 0, {0}},
	{"p", 0, {0}},
	{"q", 0, {0}},
	{"r", 0, {0}},
	{0, 0, {0}}
};

void	fork_shell(t_script *var_script, char **tab, char **environ, char **argv)
{
	if ((var_script->father = fork()) < 0)
	{
		write(2, "Fork failure\n", 13);
		exit(EXIT_FAILURE);
	}
	if (var_script->father == 0)
	{
		create_shell(var_script, tab, environ, argv);
		close(var_script->fd_pts);
	}
}

int		create_typescript(char *name)
{
	int	fd;

	if (access(name, F_OK) == 0)
	{
		if (g_argp[OPT_A].active == 1)
			fd = open(name, O_APPEND | O_RDWR);
		else
			fd = open(name, O_TRUNC | O_RDWR);
	}
	else
		fd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR |
				S_IRGRP | S_IROTH);
	if (fd == -1)
		critical_error("Open failure\n");
	return (fd);
}

void	write_start_or_end(char *name, int fd_typescript, int nb)
{
	if (g_argp[OPT_Q].active == 0)
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
}

void	script(char **environ, t_script *var_script, char **argv)
{
	static char	*tab[] = {"/bin/zsh", NULL};

	init_tty(var_script);
	make_terminal_raw(var_script);
	fork_shell(var_script, tab, environ, argv);
	FD_ZERO(&var_script->rfd);
	while (0xDEADBEEF)
	{
		re_init_sub_shell(var_script);
		var_script->ret_select = select(var_script->fd_pts, &var_script->rfd,
				0, 0, var_script->tvp);
		if (var_script->ret_select < 0 && errno != EINTR)
			break ;
		if (select_stdin(var_script) == true)
			break ;
		if (select_ptmx(var_script, var_script->fd_typescript) == true)
			break ;
	}
	reset_terminal(var_script);
}

int		main(int argc, char **argv, char **environ)
{
	t_script	var_script;
	int			i;

	ft_memset(&var_script, 0, sizeof(t_script));
	get_option(argv);
	if ((i = get_name_file(argv)) == -1)
		exit(EXIT_FAILURE);
	if (i > 0)
		get_cmd(&argv[i], var_script.cmd);
	ft_strcat(var_script.name, i != 0 ? argv[i] : "typescript");
	var_script.fd_typescript = create_typescript(var_script.name);
	write_start_or_end(var_script.name, var_script.fd_typescript, 0);
	script(environ, &var_script, &argv[++i]);
	write_start_or_end(var_script.name, var_script.fd_typescript, 1);
	(void)argc;
}
