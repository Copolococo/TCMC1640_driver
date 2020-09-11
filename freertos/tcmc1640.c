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
prvReceiveResponse(uint8_t *response)
{
	uint16_t status = OK;

	// Receber response
	for(uint8_t i=0; i<9; i++) {
		response[i] = xQueueReceive(xQueueUSART3_RX, &USART3data, xDelay);
	}

	if (!_verifyChecksum(response))	{
		OS_print("Response has a wrong checksum\n");
		status = ERROR;
	}

	return status;
}

uint16_t 
prvSendCommand(uint8_t *command uint8_t *response)
{
	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSARt(USARTx, command, 9);
	
	return prvReceiveResponse(response);
}

uint16_t 
prvGetAxisParameter(uint8_t addr, uint8_t type, uint8_t *response)
{
	uint8_t command[9];
	command[0] = addr;
	command[1] = GAP;
	command[2] = type;
	command[8] = _calc_checksum(command);
	
	xQueueReset(xQueueUSARt3_RX);
	prvSendMessageUSART(USARTx, command, 9);

	prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t 
prvSetAxisParameter(uint8_t addr, uint8_t type, int value)
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
	prvSendMessageUSART(USARTx, command, 9);

	//prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t
prvGetGlobalParameter(uint8_t addr, uint8_t type, uint8_t bank, uint8_t *response)
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = GGP;
	command[2] = type;
	command[3] = bank;
	command[8] = _calc_checksum(command);

	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSART(USARTx, command, 9);

	prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t
prvSetGlobalParameter(uint8_t addr, uint8_t type, uint8_t bank, int value)
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = SGP;
	command[2] = type;
	command[3] = bank;
	command[4] = value>>24 && 0xff;
	command[5] = value>>16 && 0xff;
	command[6] = value>>8 && 0xff;
	command[7] = value && 0xff;
	command[8] = _calc_checksum(command);

	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSART(USARTx, command, 9);
	
	//prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t 
prvMoveToPosition(uint8_t addr, uint8_t type, int value)
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
	
	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSART(USARTx, command, 9);
	
	//prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t 
prvRotateRight(uint8_t addr, int value);
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

	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSART(USARTx, command, 9);

	//prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t
prvRotateLeft(uint8_t addr, int value);
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

	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSART(USARTx, command, 9);

	//prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t 
prvMotorStop(uint8_t addr)
{
	uint8_t command[9], response[9];
	command[0] = addr;
	command[1] = MST;
	command[3] = 0;
	command[8] = _calc_checksum(command);

	xQueueReset(xQueueUSART3_RX);
	prvSendMessageUSART(USARTx, command, 9);

	//prvReceiveResponse(response);

	return STATUS(response[2]);
}

uint16_t
prvSetMaxCurrent(uint8_t addr, int current)
{ return prvSetAxisParameter(addr, 6, current); }

uint16_t
prvGetMaxCurrent(uint8_t addr)
{ 
	uint8_t response[9];
	prvGetAxisParameter(addr, 6, response);
	
	return response[7] | (response[6]<<8) | (response[5]<<16) 
		| (response[4]<<24);
}

uint16_t
prvSetRS485BaudRate(uint8_t addr, int value)
{ return prvSetGlobalParameter(addr, 65, 0, value); }
