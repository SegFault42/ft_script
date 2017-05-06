#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define TTYS "/dev/ptmx"
#define BUF_SIZE 1024

int	main(int argc, char **argv, char **environ)
{
	int		fd = -1;
	char	buf[BUF_SIZE] = {0};
	char	*tty_name;
	pid_t	father;
	char	*shell[] = {"/bin/zsh", NULL};
	char	str[BUF_SIZE] = {0};

	if ((father = fork()) < 0)
		dprintf(2, "fork failure\n");
	else if (father == 0)
	{
		if (execve(shell[0], shell, environ) < 0)
		{
			dprintf(2, "execve failure : %s\n", strerror(errno));
			exit(-1);
		}
	}
	tty_name = ttyname(STDIN_FILENO);
	printf("%s\n", tty_name);
	fd = open(tty_name, O_RDONLY | O_NOCTTY);
	if (fd == -1)
	{
		dprintf(2, "Open tty failure !");
		return (EXIT_FAILURE);
	}
	while (read(fd, str, BUF_SIZE) != 0)
	{
		printf("%s\n", str);
	}
	close(fd);
	
}
