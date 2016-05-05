#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int fd;
void sig_handler(int signo)
{
	if(signo == SIGINT){
		printf("Signal Rxd\n");
		close(fd);
		exit(1);
	}
}

int main()
{

	int ret = 0;
	int num = 0;
	char buf[5] = {0};

	fd = open("/dev/ttyACM1", O_RDWR); //ABS
	printf("Steer Port = %d\n", fd);
	if(signal(SIGINT, sig_handler) == SIG_ERR) 
	{
		printf("Cannot catch signal\n");
	}

	ret = snprintf(buf, 5, "%d", num);
	printf("Converted String: %s No of Bytes %d\n", buf, ret);

	while(1)
	{
		write(fd, buf, 4);
	}
	return 0;
}
