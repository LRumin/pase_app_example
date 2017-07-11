/*
 * mcu_port.h
 *
 *  Created on: 9 jul. 2017
 *      Author: lucho
 */
/*==================[inclusions]=============================================*/
#include "stdbool.h"
#include "stdint.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

typedef struct
{
   uint8_t port;
   uint8_t pin;
}portPin_type;

extern const portPin_type portPin[];


