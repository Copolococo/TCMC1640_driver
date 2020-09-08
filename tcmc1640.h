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
UINT8_T _calc_checksum(UINT8_T *command);

bool _verify_checksum(UINT8_T *command);


/* Instructions */
int TCMC1640_send_command(UINT8_T *command UINT8_T *response);

int TCMC1640_get_axis_parameter(UINT8_T addr, UINT8_T type);

int TCMC1640_move_to_position(UINT8_T addr, UINT8_T type, UINT8_T value);

int TCMC1640_set_axis_parameter(UINT8_T addr, UINT8_T type, UINT64_T value);

int TCMC1640_rotate_right(UINT8_T addr, INT64_T value); 

int TCMC1640_rotate_left(UINT8_T addr, INT64_T value); 

int TCMC1640_motor_stop(UINT8_T addr);
