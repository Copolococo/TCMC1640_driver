/**
 * Firmware v2.09_Rev2.05
 */

/* Library includes */
#include "stm32f4xx.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "stdbool.h"

/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"


/* Commands */
#define ROR	1
#define ROL	2
#define MST	3
#define MVP	4
#define	SAP	5
#define	GAP	6
#define	STAP	7
#define	RSAP	8
#define	SGP	9
#define	GGP	10
#define	STGP	11
#define	RSGP	12

#define	SIO	14
#define	GIO	15

#define CALC	19
#define	COMP	20
#define	JC	21
#define	JA	22
#define	CSUB	23
#define RSUB	24

#define WAIT	27
#define	STOP	28

#define	CALCX	33
#define AAP	34
#define AGP	35

/* RS485 Baud Rates */
#define B9600	0
#define B14400	1
#define B19200	2
#define B28800	3
#define B38400	4
#define B57600	5
#define B76800	6
#define B115200 7


#define OK
#define ERROR	0

#define TX_MODE 1
#define RX_MODE 0

#define STATUS(__VALUE__)	(__VALUE__ == 100 || __VALUE__ == 101)


/* Helper functions */
// IF NECESSARY REPLACE BY MACROS TO AVOID BRANCHING
uint8_t _calcChecksum(uint8_t *command);

bool _verifyChecksum(uint8_t *command);

uint16_t _processStatus(uint8_t status);

void prvReceiveResponse(uint8_t *response);

/* Instructions */
uint16_t prvSendCommand(uint8_t *command, uint8_t *response);

uint16_t prvGetAxisParameter(uint8_t addr, uint8_t type, uint8_t *response);

uint16_t prvSetAxisParameter(uint8_t addr, uint8_t type, int value);

uint16_t prvGetGlobalParameter(uint8_t addr, uint8_t type, uint8_t bank, uint8_t *response);

uint16_t prvSetGlobalParameter(uint8_t addr, uint8_t type, uint8_t bank, int value);

uint16_t prvMoveToPosition(uint8_t addr, uint8_t type, int value);

uint16_t prvRotateRight(uint8_t addr, int value); 

uint16_t prvRotateLeft(uint8_t addr, int value); 

uint16_t prvMotorStop(uint8_t addr);


//////////////////AXIS PARAMETERS//////////////////
/* Torque regulation mode */
uint16_t prvSetMaxCurrent(uint8_t addr, int current);

int prvGetMaxCurrent(uint8_t addr);

//uint16_t prvSetCurrentPID(uint8_t addr, int P, int I);

/////////////////GLOBAL PARAMETERS/////////////////
/* Bank 0 */
uint16_t prvSetRS485BaudRate(uint8_t addr, int value);
