/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 19:44:07 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/11 00:49:45 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRIPT_H
# define SCRIPT_H

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
#include <util.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include "../libft/includes/libft.h"

#include <paths.h>
#include <time.h>

#define PTMX "/dev/ptmx"

typedef enum option option;

enum option
{
	OPT_A,
	OPT_D,
	OPT_F,
	OPT_P,
	OPT_Q,
	OPT_R,
	OPT_END
};

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
	char			buff;
	int				status;
	int				fd_ptmx;
	int				fd_pts;
	int				fd_typescript;
	int				new_fd;
	int				ret_select;
	int				ret_read;
	int				tty_flag;
}					t_script;

typedef struct		s_argp
{
	char			*sign;
	bool			active;
	char			*description;
}					t_argp;

/*
 ** sys.c
 */
int		ft_tcsetattr(int fd, int opt, const struct termios *t);
int		ft_tcgetattr(int fd, struct termios *t);
void	ft_openpty(int *fd_ptmx, int *fd_pts);
void	ft_cfmakeraw(struct termios *t);
int		ft_login_tty(int fd);
int		ft_openpt(int flags);
int		ft_unlockpt(int fd);
char	*ft_ptsname(int fd);
int		ft_grantpt(int fd);
int		ft_isatty(int fd);

void	get_option(char **argv);

#endif
