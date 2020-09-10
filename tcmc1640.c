#include "tcmc1640.h"


UINT8_T 
_calcChecksum(UINT8_T *command)
{
	UINT8_T i, checksum=command[0];
	for(i=1;i<8;i++)
		checksum+=command[i];

	return checksum;
}

bool 
_verifyChecksum(UINT8_T *command)
{
	UINT8_T calculated_checksum;
	calculated_checksum = _calc_checksum(command);

	return calculated_checksum == command[9];
}

int
_processStatus(UINT8_T status)
{
	int res = 1;
	switch(status)
	{
		case 100:
			res = 0;
			break;
		case 101:
			res = 0;
			break;
		case 1:
			OS_print("Wrong checksum\n");
			break;
		case 2:
			OS_print("Invalid command\n");
			break;
		case 3:
			OS_print("Wrong type\n");
			break;
		case 4:
			OS_print("Invalid value\n");
			break;
		case 5:
			OS_print("Configuration EEPROM locked\n");
			break;
		case 6:
			OS_print("Command not available\n");
			break;
		default:
			OS_print("Unexpected response\n");
			break;
	}

	return res;
}

int 
sendCommand(UINT8_T *command UINT8_T *response)
{
	OS_send_command(command);
	OS_read_response(response);
	if (!_verifyChecksum(response))	{
		OS_print("Response has a wrong checksum\n");
		return 1;
	}
	// return error code 100 is success
	return _processStatus(response[2]);
}

int 
getAxisParameter(UINT8_T addr, UINT8_T type, UINT8_T *response)
{
	UINT8_T command[9];
	command[0] = addr;
	command[1] = GAP;
	command[2] = type;
	command[8] = _calc_checksum(command);

	return sendCommand(command, response);
}

int 
moveToPosition(UINT8_T addr, UINT8_T type, UINT64_T value)
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

	return sendCommand(command, response);
}

int 
setAxisParameter(UINT8_T addr, UINT8_T type, UINT64_T value)
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

	return sendCommand(command, response);
}

int 
rotateRight(UINT8_T addr, INT64_T value);
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

	return sendCommand(command, response);
}

int 
rotateLeft(UINT8_T addr, INT64_T value);
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

	return sendCommand(command, response);
}

int 
motorStop(UINT8_T addr)
{
	UINT8_T command[9], response[9];
	command[0] = addr;
	command[1] = MST;
	command[3] = 0;
	command[8] = _calc_checksum(command);

	return sendCommand(command, response);
}

int
setMaxCurrent(UINT8_T addr, UINT8_T current)
{ return setAxisParameter(addr, 6, current); }

int
getMaxCurrent(UINT8_T addr)
{ 
	UINT8_T response[9];
	getAxisParameter(addr, 6, response);
	
	return response[7] | (response[6]<<8) | (response[5]<<16) 
		| (response[4]<<24);
}


