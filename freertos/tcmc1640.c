/**
 * Firmware v2.09_Rev2.05
 */

#include "tcmc1640.h"


uint8_t _calcChecksum( uint8_t *command )
{
uint8_t i, checksum = command[ 0 ];
	
	for(i = 1; i < 8; i++)
		checksum += command[ i ];

	return checksum;
}

bool _verifyChecksum( uint8_t *command )
{
uint8_t calculated_checksum;

	calculated_checksum = _calc_checksum( command );

	return calculated_checksum == command[ PACKET_SIZE ];
}

uint16_t prvReceiveResponse( uint8_t *response )
{
	/* Receber response */
	for( uint8_t i = 0; i < PACKET_SIZE; i++ ) {
		response[ i ] = xQueueReceive( xQueueUSART3_RX, &USART3data, xDelay );
	}
	
	/* retornar resultado de checksum */
	return _verifyChecksum( response );
}

uint16_t prvSendCommand( uint8_t *command, uint8_t *response )
{
	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );
	
	return prvReceiveResponse( response );
}

uint16_t prvGetAxisParameter( uint8_t addr, uint8_t type, uint8_t *response )
{
uint8_t command[ PACKET_SIZE ];

	command[ 0 ] = addr;
	command[ 1 ] = GAP;
	command[ 2 ] = type;
	command[ 8 ] = _calc_checksum( command );
	
	xQueueReset( xQueueUSARt3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

	prvReceiveResponse( response );

	return STATUS( response[ 2 ] );
}

uint16_t prvSetAxisParameter( uint8_t addr, uint8_t type, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = SAP;
	command[ 2 ] = type;
	command[ 3 ] = 0;
	command[ 4 ] = value >> 24 && 0xff;
	command[ 5 ] = value >> 16 && 0xff;
	command[ 6 ] = value >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );
	
	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif

	return res;
}

uint16_t prvStoreAxisParameter( uint8_t addr, uint8_t type, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = STAP;
	command[ 2 ] = type;
	command[ 3 ] = 0;
	command[ 4 ] = value  >> 24 && 0xff;
	command[ 5 ] = value  >> 16 && 0xff;
	command[ 6 ] = value  >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );
	
	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif

	return res;
}

uint16_t prvRestoreAxisParameter( uint8_t addr, uint8_t type, uint8_t *response )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = RSAP;
	command[ 2 ] = type;
	command[ 3 ] = 0;
	command[ 8 ] = _calc_checksum( command );
	
	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif

	return res;
}

uint16_t prvGetGlobalParameter( uint8_t addr, uint8_t type, uint8_t bank, uint8_t *response )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = GGP;
	command[ 2 ] = type;
	command[ 3 ] = bank;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif

	return res;
}

uint16_t prvSetGlobalParameter( uint8_t addr, uint8_t type, uint8_t bank, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = SGP;
	command[ 2 ] = type;
	command[ 3 ] = bank;
	command[ 4 ] = value  >> 24 && 0xff;
	command[ 5 ] = value  >> 16 && 0xff;
	command[ 6 ] = value  >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );
	
#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}

uint16_t prvStoreGlobalParameter( uint8_t addr, uint8_t type, uint8_t bank, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = STGP;
	command[ 2 ] = type;
	command[ 3 ] = bank;
	command[ 4 ] = value  >> 24 && 0xff;
	command[ 5 ] = value  >> 16 && 0xff;
	command[ 6 ] = value  >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );
	
#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}

uint16_t prvRestoreGlobalParameter( uint8_t addr, uint8_t type, uint8_t bank, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = RSGP;
	command[ 2 ] = type;
	command[ 3 ] = bank;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );
	
#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}

uint16_t prvMoveToPosition( uint8_t addr, uint8_t type, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = MVP;
	command[ 2 ] = type;
	command[ 3 ] = 0;
	command[ 4 ] = value  >> 24 && 0xff;
	command[ 5 ] = value  >> 16 && 0xff;
	command[ 6 ] = value  >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );
	
	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );
	
#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}

uint16_t prvRotateRight( uint8_t addr, int32_t value )
{
uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = ROR;
	command[ 3 ] = 0;
	command[ 4 ] = value  >> 24 && 0xff;
	command[ 5 ] = value  >> 16 && 0xff;
	command[ 6 ] = value  >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}

uint16_t
prvRotateLeft( uint8_t addr, int32_t value )
{
	uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
	uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = ROL;
	command[ 3 ] = 0;
	command[ 4 ] = value  >> 24 && 0xff;
	command[ 5 ] = value  >> 16 && 0xff;
	command[ 6 ] = value  >> 8 && 0xff;
	command[ 7 ] = value && 0xff;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}

uint16_t 
prvMotorStop( uint8_t addr )
{
	uint8_t command[ PACKET_SIZE ], response[ PACKET_SIZE ];
	uint16_t res = OK;

	command[ 0 ] = addr;
	command[ 1 ] = MST;
	command[ 3 ] = 0;
	command[ 8 ] = _calc_checksum( command );

	xQueueReset( xQueueUSART3_RX );
	prvSendMessageUSART( USARTx, command, PACKET_SIZE );

#ifdef BLOCKING
	prvReceiveResponse( response );
	res = STATUS( response[ 2 ] );
#endif
	
	return res;
}


//////////////////AXIS PARAMETERS//////////////////

/* Motor settings */
uint16_t prvSetNumMotorPoles( uint8_t addr, uint32_t num )
{ return prvSetAxisParameter( addr, 253, num ); }

uint8_t prvGetNumMotorPoles( uint8_t addr )
{
uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 253, response );

	return response[ 4 ];
}

uint16_t prvSetOvervoltageProtection( uint8_t addr, int32_t value )
{ return prvSetAxisParameter( addr, 245, value ); }


/* Encoder/Initialization settings */
uint16_t prvReInitBLDC( uint8_t addr )
{ return prvSetAxisParameter( addr, 31, 1 ); }

uint16_t prvSetEncoderSteps( uint8_t addr, uint32_t value )
{ return prvSetAxisParameter( addr, 250, value ); }


/* Torque regulation mode */
uint16_t prvSetMaxCurrent( uint8_t addr, int32_t current )
{ return prvSetAxisParameter( addr, 6, current ); }

uint32_t prvGetMaxCurrent( uint8_t addr )
{ 
	uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 6, response );
	
	return response[ 7 ] | ( response[ 6 ] << 8 ) | ( response[ 5 ] << 16 ) 
		| ( response[ 4 ] << 24 );
}

int32_t prvGetActualCurrent( uint8_t addr )
{
uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 150, response );

	return response[ 7 ] | ( response[ 6 ] << 8 ) | ( response[ 5 ] << 16 ) 
		| ( response[ 4 ] << 24 );
}

uint16_t prvSetCurrentPID( uint8_t addr, uint32_t P, uint32_t I )
{
uint16_t res;
	res = prvSetAxisParameter( addr, 172, P );
	res |= prvSetAxisParameter( addr, 173, I );

	return res;
}


/* Velocity regulation mode */
uint16_t prvSetTargetSpeed( uint8_t addr, int32_t value )
{ return prvSetAxisParameter( addr, 2, value ); }

int32_t prvGetTargetSpeed( uint8_t addr )
{
uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 2, response );

	return response[ 7 ] | ( response[ 6 ] << 8 ) | ( response[ 5 ] << 16 ) 
		| ( response[ 4 ] << 24 );
}

int32_t prvGetActualSpeed( uint8_t addr )
{
	uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 3, response );

	return response[ 7 ] | ( response[ 6 ] << 8 ) | ( response[ 5 ] << 16 ) 
		| ( response[ 4 ] << 24 );
}

uint16_t prvSetVelocityPID( uint8_t addr, uint32_t P, uint32_t I )
{
uint16_t res;
	res = prvSetAxisParameter( addr, 234, P );
	res |= prvSetAxisParameter( addr, 235, I );
	
	return res;
}


/* Velocity ramp parameters */
uint16_t prvSetAcceleration( uint8_t addr, uint32_t value )
{ return prvSetAxisParameter( addr, 11, value ); }


/* Position regulation mode */
uint16_t prvSetPositionPID( uint8_t addr, uint32_t P )
{ return prvSetAxisParameter( addr, 230, P ); }


/* Status information */
uint32_t prvGetSupplyVoltage( uint8_t addr )
{
uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 151, response );

	return response[ 7 ] | ( response[ 6 ] << 8 ) | ( response[ 5 ] << 16 ) 
		| ( response[ 4 ] << 24 );
}

uint32_t prvGetDriverTemp( uint8_t addr )
{
uint8_t response[ PACKET_SIZE ];
	prvGetAxisParameter( addr, 152, response );

	return response[ 7 ] | ( response[ 6 ] << 8 ) | ( response[ 5 ] << 16 ) 
		| ( response[ 4 ] << 24 );
}

/////////////////GLOBAL PARAMETERS/////////////////
uint16_t prvSetRS485BaudRate( uint8_t addr, int32_t value )
{ return prvSetGlobalParameter( addr, 65, 0, value ); }
