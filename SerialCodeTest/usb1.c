#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>

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
	int bytes;
	int rx_num;
	char read_data;

	fd = open("/dev/ttyUSB1", O_RDWR); //ECU COM port
	printf("Fd = %d\n", fd);
	if(signal(SIGINT, sig_handler) == SIG_ERR) {
		printf("Cannot catch signal\n");
	}

	while(1)
	{
		bytes = read(fd, &read_data, 1);
		rx_num = atoi(&read_data);
		printf("%d\n", rx_num);
	}
	return 0;
}
