#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <drivers/drv_hrt.h>
#include <string.h>
#include <systemlib/err.h>
//#include <systemlib/systemlib.h> - canceled by publisher - 9.22 - Dennis
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
__EXPORT int rw_uart_main(int argc, char *argv[]);

int set_uart_baudrate(const int fd, unsigned int baud);

int set_uart_baudrate(const int fd, unsigned int baud)
{
int speed;

switch (baud) {
case 9600:   speed = B9600;   break;
case 19200:  speed = B19200;  break;
case 38400:  speed = B38400;  break;
case 57600:  speed = B57600;  break;
case 115200: speed = B115200; break;
default:
	warnx("ERR: baudrate: %d\n", baud);
return -EINVAL;
}

struct termios uart_config;

int termios_state;

/* fill the struct for the new configuration */
tcgetattr(fd, &uart_config);
/* clear ONLCR flag (which appends a CR for every LF) */
uart_config.c_oflag &= ~ONLCR;
/* no parity, one stop bit */
uart_config.c_cflag &= ~(CSTOPB | PARENB);
/* set baud rate */
if ((termios_state = cfsetispeed(&uart_config, speed)) < 0) {
	warnx("ERR: %d (cfsetispeed)\n", termios_state);
	return false;
}

if ((termios_state = cfsetospeed(&uart_config, speed)) < 0) {
	warnx("ERR: %d (cfsetospeed)\n", termios_state);
	return false;
}

if ((termios_state = tcsetattr(fd, TCSANOW, &uart_config)) < 0) {
	warnx("ERR: %d (tcsetattr)\n", termios_state);
	return false;
}

return true;
}

int rw_uart_main(int argc, char *argv[])
{
	char data = '0';
	char buffer[4] = "";

	int uart_read = open("/dev/ttyS2", O_RDWR | O_NOCTTY);//打開串口設備

	if (uart_read < 0) {
		err(1, "failed to open port: %s", "/dev/ttyS2");
		return -1;
	}

	if(false == set_uart_baudrate(uart_read,9600)){
		printf("[YCM]set_uart_baudrate is failed\n");
		return -1;
	}
	printf("[YCM]uart init is successful\n");

	while(true){
		read(uart_read,&data,1);
		if(data == 'R'){
			for(int i = 0;i <4;++i){
				read(uart_read,&data,1);//讀取串口數據
				buffer[i] = data;
				data = '0';
			}
			printf("%s\n",buffer);
		}
	}

	return 0;
}
