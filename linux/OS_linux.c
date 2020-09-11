#include "OS_layer.h"


int fd = 0;

int 
OS_open_device( void )
{
	// default baud 9600
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY); 
	
	int res = 0;

	struct termios tty;
	
	if(fd < 0) {
		printf("Error %i from open: %s\n", errno, strerror(errno));
		res = 1;
	}
	
	/* control modes flags */
	tty.c_cflag &= ~PARENB;		// no parity bit
	tty.c_cflag &= ~CSTOPB;		// no stop field
	tty.c_cflag |= CS8;		// 8 bits per byte
	tty.c_cflag &= ~CRTSCTS;	// disable hardware flow control
	tty.c_cflag |= CREAD | CLOCAL;	// enable READ and disable control lines

	/* local flags */
	tty.c_lflag &= ~ICANON;		// disable canon mode
	tty.c_lflag &= ~ECHO;		// disable echo
	tty.c_lflag &= ~ECHOE;		// disable echo erasure
	tty.c_lflag &= ~ECHONL;		// disable NL echo
	tty.c_lflag &= ~ISIG;		// disable signal chars

	/* input flags */
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);				// disable software flow control
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);// disable special byte handling
	
	/* output flags */
	tty.c_oflag &= ~OPOST;		// prevent special character handling
	tty.c_oflag &= ~ONLCR;		// prevent conversion of LF to CRLF

	tty.c_cc[VTIME] = 10;		// wait for up to 1s
	tty.c_cc[VMIN] = 0;

	/* set baudrate */
	cfsetispeed(&tty, B9600);	// input baudrate
	cfsetospeed(&tty, B9600);	// output baudrate
	
	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error %i from tcetattr: %s\n", errno, strerror(errno));
		res |= 2;
	}
	
	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		printf("Error, serial port already locked by another process\n");
		res |= 4;
	}

	return res;
}

int 
OS_send_command(UINT8_T *command)
{ return write(fd, command, 9); }

int
OS_read_response(UINT8_T *response_buffer)
{ return read(fd, response_buffer, 9); }

void
OS_close_device( void )
{ close(fd); }
