#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define TTYS "/dev/ttys000"
#define BUF_SIZE 1024

int	main()
{
	int		fd = -1;
	char	buf[BUF_SIZE] = {0};

	fd = open(TTYS, O_RDONLY | O_NOCTTY);
	if (fd == -1)
	{
		dprintf(2, "Open failure !");
		return (EXIT_FAILURE);
	}
	while (read(fd, buf, sizeof(buf)) != 0)
	{
		dprintf(1, "%s\n", buf);
		memset(buf, 0, sizeof(buf));
	}
	close(fd);
}
