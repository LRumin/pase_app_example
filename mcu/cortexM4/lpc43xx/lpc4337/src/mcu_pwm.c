/*

 * mcu_pwm.c
 *
 *  Created on: 7 jul. 2017
 *      Author: lucho
 */
/*==================[inclusions]=============================================*/
#include "mcu.h"
#include "mcu_pwm.h"
#include "chip.h"


/*==================[macros and definitions]=================================*/

const uint32_t Cout[] =
{
   0,
   0,
   0,
   2,
   5,
   4,
   0,
   7,
   6,
   0,
};

/*==================[external functions definition]==========================*/

extern void mcu_pwm_init(uint32_t Frec){

	Chip_SCTPWM_Init(LPC_SCT);

	Chip_SCTPWM_SetRate(LPC_SCT, Frec);
}



extern void mcu_pwm_config(mcu_pwm_channel Channel,uint32_t pin){

	 Chip_SCU_PinMux(portPin[pin].port, portPin[pin].pin,0,FUNC1);

	 Chip_SCTPWM_SetOutPin(LPC_SCT, Channel, Cout[pin]);

}

extern void mcu_pwm_start(){
	Chip_SCTPWM_Start(LPC_SCT);
}

extern void mcu_pwm_setDutyCicle(uint32_t duty, mcu_pwm_channel Channel){
	Chip_SCTPWM_SetDutyCycle(LPC_SCT, Channel, Chip_SCTPWM_PercentageToTicks(LPC_SCT, duty));
}


