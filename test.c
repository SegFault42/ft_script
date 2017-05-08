#include <allheaders.h>

extern int errno;

void fatalErrrExit(int error, char *message)
{
	perror(message);
	perror(strerror(error));
	exit(1);
}

void fatalExit(char *message)
{
	perror(message);
	exit(1);
}

int main(int argc, char *argv[])
{
	char buffer[1024];
	int bufSize = 1024;
	int mfd = -1, sfd = -1;
	int red, wrote;
	pid_t pid;

	if(argc < 2)
		fatalExit("Invalid command line");

	if(strcmp(argv[1], "child") == 0)
	{
		if(argc != 3)
			fatalExit("Invalid command line");

		printf("Child role: openning parent endpoint at %s\n", argv[2]);

		if((sfd = open(argv[2], O_RDWR)) == -1)
			fatalErrorExit(errno, "Cannot open slave stream");

		pid = 0;
	}

	else if(strcmp(argv[1], "parent") == 0)
	{
		if((mfd = getpt()) == -1)
			fatalErrorExit(errno, "Cannot open stream");

		if(grantpt(mfd) == -1) // ioctl(fildes, TIOCGRANTPT, 0);
			fatalErrorExit(errno, "Cannot grant stream");

		if(unlockpt(mfd) == -1) // ioctl(fd, TIOCSPTLCK, 0);
			fatalErrorExit(errno, "Cannot unlock stream");

		printf("Parent role: child endpoint at %s\n", ptsname(mfd));
		pid = 1;
	}

	else if(strcmp(argv[1], "both") == 0)
	{
		if((mfd = getpt()) == -1)
			fatalErrorExit(errno, "Cannot open stream");

		if(grantpt(mfd) == -1)
			fatalErrorExit(errno, "Cannot grant stream");

		if(unlockpt(mfd) == -1)
			fatalErrorExit(errno, "Cannot unlock stream");

		if((sfd = open(ptsname(mfd), O_RDWR)) == -1)
			fatalErrorExit(errno, "Cannot open slave stream");

		pid = fork();
	}

	else fatalExit("Unknown mode of operation");


	switch(pid)
	{
		case -1:
			fatalErrorExit(errno, "Cannot fork");
			break;

		case 0: // Child
			close(mfd);

			while(true)
			{
				if((wrote = write(sfd, "Hello Dad!\n", 11)) < 0)
					fatalErrorExit(errno, "Cannot write to parent");

				if((red = read(sfd, buffer, bufSize)) < 0)
					fatalErrorExit(errno, "Cannot receive from parent");

				else if(red == 0)
					fatalErrorExit(errno, "Parent terminated");

				buffer[red] = '\0';
				printf("P -> C: %s", buffer);

				sleep(random() % 10);
			}

			break;

		default: // Parent
			close(sfd);

			while(true)
			{
				if((wrote = write(mfd, "Hello Child!\n", 13)) < 0)
					fatalErrorExit(errno, "Cannot write to child");

				if((red = read(mfd, buffer, bufSize)) < 0)
					fatalErrorExit(errno, "Cannot receive from child");

				else if(red == 0)
					fatalErrorExit(errno, "Child terminated");

				buffer[red] = '\0';
				printf("P <- C: %s", buffer);

				sleep(random() % 10);
			}

			break;
	}

	return 0;
}
