/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 19:44:07 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/09 20:06:46 by rabougue         ###   ########.fr       */
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

#define PTMX "/dev/ptmx"

/*
** sys.c
*/

int		ft_tcsetattr(int fd, int opt, const struct termios *t);
int		ft_tcgetattr(int fd, struct termios *t);
void	ft_cfmakeraw(struct termios *t);
int		ft_login_tty(int fd);
int		ft_openpt(int flags);
int		ft_unlockpt(int fd);
char	*ft_ptsname(int fd);
int		ft_grantpt(int fd);
int		ft_isatty(int fd);

#endif
