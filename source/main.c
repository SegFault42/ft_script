#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../libft/includes/libft.h"

#define TTYS "/dev/ptmx"
#define BUF_SIZE 1024

void	open_new_shell(int argc, char **argv, char **environ)
{
	pid_t	father;
	int		status;
	char	*tab[] = {"/bin/zsh", NULL};

	if ((father = fork()) < 0)
		dprintf(2, "Fork failure : %s\n", strerror(errno));
	else if (father > 0)
	{
		if (argc == 1)
			dprintf(1, "Script started, file is typescript\n");
		else
			dprintf(1, "Script started, file is %s\n", argv[1]);
		waitpid(father, &status, 0);
		kill(father, SIGTERM);
	}
	else
	{
		if (execve(tab[0], &tab[0], environ) < 0)
			dprintf(2, "Execeve failure : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (argc == 1)
		dprintf(1, "Script done, file is typescript\n");
	else
		dprintf(1, "Script done, file is %s\n", argv[1]);
}

int	main(int argc, char **argv, char **environ)
{
	int	fd;

	if (access("./typescript", F_OK) == 0)
	{
		fd = open("./typescript", O_APPEND);
	}
	else
		fd = open("./typescript", O_CREAT, S_IRWXU);
	if (fd == -1)
		ft_dprintf(2, "Open failure %s\n", strerror(errno));
	open_new_shell(argc, argv, environ);
	close(fd);
}
