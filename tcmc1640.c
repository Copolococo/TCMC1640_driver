#include "tcmc1640.h"

UINT8_T _calc_checksum(UINT8_T *command)
{
	UINT8_T i, checksum=command[0];
	for(i=1;i<8;i++)
		checksum+=command[i];

	return checksum;
}

bool _verify_checksum(UINT8_T *command)
{
	UINT8_T calculated_checksum;
	calculated_checksum = _calc_checksum(command);

	return calculated_checksum == command[9];
}

int TCMC1640_send_command(UINT8_T *command UINT8_T *response)
{
	OS_send_command(command);
	OS_read_response(response);
	
	// return error code 100 is success
	return response[3];
}

int TCMC1640_get_axis_parameter(UINT8_T addr, UINT8_T type)
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = GAP;
	command[2] = type;
	command[8] = _calc_checksum(command);

	OS_send_command(command);
	OS_read_response(response);

	return response[3];
}

int TCMC1640_move_to_position(UINT8_T addr, UINT8_T type, UINT64_T value)
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = MVP;
	command[2] = type;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	OS_send_command(command);
	OS_read_response(response);

	return response[3];
}

int TCMC1640_set_axis_parameter(UINT8_T addr, UINT8_T type, UINT64_T value)
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = SAP;
	command[2] = type;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	OS_send_command(command);
	OS_read_response(response);

	return response[3];
}

int TCMC1640_rotate_right(UINT8_T addr, INT64_T value);
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = ROR;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	OS_send_command(command);
	OS_read_response(response);

	return response[3];
}

int TCMC1640_rotate_left(UINT8_T addr, INT64_T value);
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = ROL;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	OS_send_command(command);
	OS_read_response(response);

	return response[3];
}

int TCMC1640_motor_stop(UINT8_T addr)
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = MST;
	command[3] = 0;
	command[8] = _calc_checksum(command);

	OS_send_command(command);
	OS_read_response(response);

	return response[3];
}
