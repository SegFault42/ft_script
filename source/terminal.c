/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 21:23:03 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/12 01:10:26 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

extern t_argp	g_argp[];
char			*g_tab[] = {"llllllllllllll", NULL};

void	init_tty(t_script *var_script)
{
	if ((var_script->tty_flag = ft_isatty(STDIN_FILENO)))
	{
		if (ft_tcgetattr(STDIN_FILENO, &var_script->tt) == -1)
			critical_error("Error ft_tcgetattr\n");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, &var_script->win) == -1)
			critical_error("Error ioctl\n");
		ft_openpty(&var_script->fd_ptmx, &var_script->fd_pts, &var_script->win);
	}
	else
		ft_openpty(&var_script->fd_ptmx, &var_script->fd_pts, &var_script->win);
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

void	create_shell(t_script *var_script, char **tab, char **environ)
{
	/*static char	*cmd[] = {"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii", 0};*/

	if (close(var_script->fd_ptmx) == -1)
		critical_error("close ptmx failure\n");
	if (ft_login_tty(var_script->fd_pts))
		critical_error("login_tty failure\n");
	if (var_script->cmd[0])
	{
		/*ft_memset(g_tab[1], 0, 255);*/
		/*printf("%zu\n", sizeof(g_tab[0]));*/
		printf("%s\n", g_tab[0]);
		printf("%s\n", g_tab[1]);
		if (execve(g_tab[0], &g_tab[0], environ) < 0)
			write(2, "Execeve failure()\n", 16);
	}
	else
	{
		if (execve(tab[0], &tab[0], environ) < 0)
			write(2, "Execeve failure\n", 16);
	}
	exit(EXIT_FAILURE);
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
