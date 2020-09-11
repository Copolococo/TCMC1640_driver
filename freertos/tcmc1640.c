#include "tcmc1640.h"


uint8_t 
_calcChecksum(uint8_t *command)
{
	uint8_t i, checksum=command[0];
	for(i=1;i<8;i++)
		checksum+=command[i];

	return checksum;
}

bool 
_verifyChecksum(uint8_t *command)
{
	uint8_t calculated_checksum;
	calculated_checksum = _calc_checksum(command);

	return calculated_checksum == command[9];
}

uint16_t
_processStatus(uint8_t status)
{
	uint16_t res = ERROR;
	switch(status)
	{
		case 100:
			res = OK;
			break;
		case 101:
			res = OK;
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

uint16_t 
prvSendCommand(uint8_t *command uint8_t *response)
{
	OS_send_command(command);
	OS_read_response(response);
	if (!_verifyChecksum(response))	{
		OS_print("Response has a wrong checksum\n");
		return ERROR;
	}
	
	return STATUS(response[2]);
}

uint16_t 
getAxisParameter(uint8_t addr, uint8_t type, uint8_t *response)
{
	uint8_t command[9];
	command[0] = addr;
	command[1] = GAP;
	command[2] = type;
	command[8] = _calc_checksum(command);
	
	prvSendMessageUART(USARTx, command, 9);

	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	return STATUS(response[2]);
}

uint16_t 
moveToPosition(uint8_t addr, uint8_t type, int value)
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = MVP;
	command[2] = type;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);
	
	prvSendMessageUART(USARTx, command, 9);
	
	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	return STATUS(response[2]);
}

uint16_t 
setAxisParameter(uint8_t addr, uint8_t type, int value)
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = SAP;
	command[2] = type;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);
	
	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUART(USARTx, command, 9);

	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	return STATUS(response[2]);
}

uint16_t 
rotateRight(uint8_t addr, int value);
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = ROR;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	return STATUS(response[2]);
}

int 
rotateLeft(uint8_t addr, int value);
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = ROL;
	command[3] = 0;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	return STATUS(response[2]);
}

uint16_t 
motorStop(uint8_t addr)
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = MST;
	command[3] = 0;
	command[8] = _calc_checksum(command);

	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	return STATUS(response[2]);
}

uint16_t
setMaxCurrent(uint8_t addr, UINT8_T current)
{ return setAxisParameter(addr, 6, current); }

int
getMaxCurrent(uint8_t addr)
{ 
	uint8_t response[9];
	getAxisParameter(addr, 6, response);
	
	return response[7] | (response[6]<<8) | (response[5]<<16) 
		| (response[4]<<24);
}


