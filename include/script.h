/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 19:44:07 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/12 00:55:51 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRIPT_H
# define SCRIPT_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/cdefs.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <termios.h>
# include <util.h>
# include <sys/ioctl.h>
# include <sys/tty.h>
# include "../libft/includes/libft.h"
# include <paths.h>
# include <time.h>

# define PTMX "/dev/ptmx"

typedef enum		e_option
{
	OPT_A,
	OPT_D,
	OPT_F,
	OPT_P,
	OPT_Q,
	OPT_R,
	OPT_END
}					t_option;

typedef struct		s_script
{
	struct termios	rtt;
	struct termios	stt;
	struct termios	tt;
	struct winsize	win;
	struct timeval	*tvp;
	pid_t			father;
	fd_set			rfd;
	char			buffer_in[BUFSIZ];
	char			buffer_out[BUFSIZ];
	char			name[256];
	char			cmd[255];
	char			buff;
	int				status;
	int				fd_ptmx;
	int				fd_pts;
	int				fd_typescript;
	int				new_fd;
	int				ret_select;
	int				ret_read;
	int				tty_flag;
	char			pad[4];
}					t_script;

typedef struct		s_argp
{
	char			*sign;
	bool			active;
	char			pad[7];
}					t_argp;

/*
** sys.c
*/
int					ft_tcsetattr(int fd, int opt, const struct termios *t);
int					ft_tcgetattr(int fd, struct termios *t);
void				ft_openpty(int *fd_ptmx, int *fd_pts, struct winsize *win);
void				ft_cfmakeraw(struct termios *t);
int					ft_login_tty(int fd);
int					ft_openpt(int flags);
int					ft_unlockpt(int fd);
char				*ft_ptsname(int fd);
int					ft_grantpt(int fd);
int					ft_isatty(int fd);
/*
** sys.c
*/
void				get_option(char **argv);
/*
** tools.c
*/
void				print_time(int fd);
void				critical_error(char *error);
int					get_name_file(char **argv);
void				get_cmd(char **argv, char *cmd);
/*
** terminal.c
*/
void				reset_terminal(t_script *var_script);
void				re_init_sub_shell(t_script *var_script);
void				create_shell(t_script *var_script, char **tab,
								char **environ);
void				make_terminal_raw(t_script *var_script);
void				init_tty(t_script *var_script);
/*
** select.c
*/
bool				select_stdin(t_script *var_script);
bool				select_ptmx(t_script *var_script, int fd_typescript);

#endif
