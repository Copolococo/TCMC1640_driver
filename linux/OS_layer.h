//C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include "tcmc1640.h"

// Common data types
#define UINT8_T (unsigned char)	// 8 bit unsigned integer 
#define UINT64_T (unsigned long long) // 64 bit unsigned integer
#define INT64_T (long long) // 64 signed integer


int OS_send_command(UINT8_T *command);

int OS_read_response(UINT8_T *response_buffer);

int OS_open_device( void );

void OS_close_device( void );
