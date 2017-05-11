/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabougue <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 21:43:22 by rabougue          #+#    #+#             */
/*   Updated: 2017/05/11 21:54:57 by rabougue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/script.h"

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
