/*

 * mcu_pwm.c
 *
 *  Created on: 7 jul. 2017
 *      Author: lucho
 */
/*==================[inclusions]=============================================*/
#include "mcu.h"
#include "stdint.h"
#include "chip.h"
#include "os.h"

/*==================[macros and definitions]=================================*/

/*==================[external functions definition]==========================*/

extern void mcu_pwm_init(void){
	Chip_SCTPWM_Init(LPC_SCT);
}

extern void mcu_pwm_config(mcu_pwm_channel Channel , uint32_t Frec){

	 Chip_SCTPWM_SetRate(LPC_SCT, Frec);

	 Chip_SCU_PinMux(portPin[Channel].port, portPin[Channel].pin,0,FUNC1);

	 Chip_SCTPWM_SetOutPin(LPC_SCT, 1, 2);

    Chip_SCTPWM_Start(LPC_SCT);
}

extern void mcu_pwm_setDutyCilce(uint32_t duty){
	Chip_SCTPWM_SetDutyCycle(LPC_SCT, 1, Chip_SCTPWM_PercentageToTicks(LPC_SCT, duty));
}


