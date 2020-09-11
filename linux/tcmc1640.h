/**
 * Firmware v2.09_Rev2.05
 */

#include "OS_layer.h"

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


/* Helper functions */
UINT8_T _calcChecksum(UINT8_T *command);

bool _verifyChecksum(UINT8_T *command);

/* Error handling */

int _processStatus(UINT8_T status);

/* Instructions */
int sendCommand(UINT8_T *command, UINT8_T *response);

int getAxisParameter(UINT8_T addr, UINT8_T type, UINT8_T *response);

int moveToPosition(UINT8_T addr, UINT8_T type, UINT8_T value);

int setAxisParameter(UINT8_T addr, UINT8_T type, UINT64_T value);

int rotateRight(UINT8_T addr, INT64_T value); 

int rotateLeft(UINT8_T addr, INT64_T value); 

int motorStop(UINT8_T addr);

/* Torque regulation mode */
int setMaxCurrent(UINT8_T addr, UINT8_T current);

int getMaxCurrent(UINT8_T addr);


