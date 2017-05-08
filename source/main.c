#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include "../libft/includes/libft.h"

#define TTYS "/dev/ptmx"
#define BUF_SIZE 1024

void	open_new_shell(int argc, char **argv, char **environ, int fd_file)
{
	int	fd_master;
	int	fd_slave;
	int	fd_stdin;
	char	buff;
	struct termios tty;
	struct termios tty_old;
	char tab[1024];
	int	ret;

	ft_dprintf(1, CYAN"fd_file = %d\n"END, fd_file);
	memset(&tab, 0, 1024);
	memset(&tty, 0, sizeof(tty));
	tty_old = tty;
	fd_stdin = open(STDIN_FILENO, O_RDWR | O_NOCTTY);
	fd_master = open(TTYS, O_RDWR | O_NOCTTY);
	if (fd_master == -1)
	{
		ft_dprintf(2, RED"Error openning ptmx\n"END);
		exit(-1);
	}
	ft_dprintf(2, GREEN"openning ptmx success\n"END);
	fd_slave = open("/dev/ttys001", O_RDWR | O_NOCTTY);
	ft_dprintf(1, CYAN"fd_2 = %d\n"END, fd_slave);
	if (fd_slave == -1)
	{
		ft_dprintf(2, RED"Error openning /dev/ttys001\n"END);
		exit(-1);
	}
		ft_dprintf(2, GREEN"openning /dev/ttys002 success\n"END);
	while ((ret = read(fd_slave, &tab, 1024)) > 0)
	{
		ft_dprintf(2, GREEN"Read ok success\n"END);
		if (ret == -1)
		{
			ft_dprintf(2, RED"Error read fd_slave\n"END);
			exit(-1);
		}
		else if (ret == 0)
			break ;
		if (write(fd_file, &tab, 1024) == -1)
		{
			ft_dprintf(2, RED"Error write fd_file : %s\n"END, strerror(errno));
			exit(-1);
		}
		ft_dprintf(2, GREEN"write ok success\n"END);
		memset(&tab, 0, 1024);
	}
}

/*void	open_new_shell(int argc, char **argv, char **environ, int fd_file)*/
/*{*/
	/*pid_t	father;*/
	/*int		status;*/
	/*char	*tab[] = {"env", "-i", "/bin/zsh", NULL};*/
	/*char	buff;*/
	/*int		fd_tty;*/
	/*[>char	*name;<]*/

	/*[>if ((father = fork()) < 0)<]*/
		/*[>dprintf(2, "Fork failure : %s\n", strerror(errno));<]*/
	/*[>else if (father > 0)<]*/
	/*[>{<]*/
		/*if (argc == 1)*/
			/*dprintf(1, "Script started, file is typescript\n");*/
		/*else*/
			/*dprintf(1, "Script started, file is %s\n", argv[1]);*/
		/*while (1) // <== SHELL LVL 1*/
		/*{*/
			/*[>name = ttyname(STDIN_FILENO);<]*/
			/*[>ft_dprintf(1, PURPLE"tty is  : %s\n"END, name);<]*/
			/*if ((fd_tty = open(TTYS, O_RDWR | O_NOCTTY)) < 0)*/
			/*{*/
				/*ft_dprintf(1, RED"==========ptmx open failure========\n"END);*/
				/*exit(EXIT_FAILURE);*/
			/*}*/
			/*ft_dprintf(1, GREEN"==========ptmx open success========\n"END);*/
			/*int is_tty = isatty(fd_tty);*/
			/*ft_dprintf(1, GREEN"==========is_tty = %d========\n"END, is_tty);*/
			/*int ret = read(fd_tty, &buff, 1);*/
			/*ft_dprintf(2, ORANGE"=====ret (read) = %d=====\n"END, ret);*/
			/*ft_dprintf(2, ORANGE"=====buf = %c=====\n"END, buff);*/
				/*write(fd_file, &buff, 1);*/
			/*close(fd_tty);*/
			/*break ;*/
		/*}*/
		/*[>kill(father, SIGTERM);<]*/
	/*[>}<]*/
	/*[>else // <== SHELL LVL 2<]*/
	/*[>{<]*/
		/*[>if (execve(tab[0], &tab[0], environ) < 0)<]*/
			/*[>dprintf(2, "Execeve failure : %s\n", strerror(errno));<]*/
		/*[>exit(EXIT_FAILURE);<]*/
	/*[>}<]*/
	/*if (argc == 1)*/
		/*dprintf(1, "Script done, file is typescript\n");*/
	/*else*/
		/*dprintf(1, "Script done, file is %s\n", argv[1]);*/
/*}*/

int	main(int argc, char **argv, char **environ)
{
	int	fd;

	if (access("./typescript", F_OK) == 0)
		fd = open("./typescript", O_APPEND | O_RDWR);
	else
		fd = open("./typescript", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		ft_dprintf(2, "Open failure %s\n", strerror(errno));
	ft_dprintf(1, GREEN"==========open file success========\n"END);
	open_new_shell(argc, argv, environ, fd);
	close(fd);
}
