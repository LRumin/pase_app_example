/* Copyright 2017, Gustavo Muro
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief PASE APP EXAMPLE
 **
 ** ejemplo de aplicación usando CIAA Firmware
 **
 **/

/** \addtogroup
 ** @{ */
/** \addtogroup
 ** @{ */
/** \addtogroup
 ** @{ */

/*==================[inclusions]=============================================*/
#include "os.h"
#include "pase_app_example.h"
#include "bsp.h"
#include "mcu.h"
#include "util.h"
#include <string.h>
/*==================[macros and definitions]=================================*/
#define FIRST_START_DELAY_MS 10
#define PERIOD_MS 100

/*==================[internal data declaration]==============================*/
 uint32_t Duty_Cycle;
 uint8_t Buffer[50];
/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void eventInput1_callBack(mcu_gpio_pinId_enum id, mcu_gpio_eventTypeInput_enum evType)
{
   SetEvent(InputEvTask1, evTask);
}

static void eventInput2_callBack(mcu_gpio_pinId_enum id, mcu_gpio_eventTypeInput_enum evType)
{
   SetEvent(InputEvTask2, evTask);
}

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */


	StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   bsp_init();
   Duty_Cycle = 0;
   Estado_Rampa = RAMPA_PARADA;
   bsp_uart_Init(9600);
   bsp_pwmInit(1000);
   bsp_SetDutyCicle(MCU_PWM_CHANNEL0, 0);
   bsp_SetDutyCicle(MCU_PWM_CHANNEL1, 0);
   bsp_SetDutyCicle(MCU_PWM_CHANNEL2, 0);
   bsp_pwmStart();

   mcu_gpio_setEventInput(MCU_GPIO_PIN_ID_38,
         MCU_GPIO_EVENT_TYPE_INPUT_FALLING_EDGE,
         eventInput1_callBack);

   mcu_gpio_setEventInput(MCU_GPIO_PIN_ID_42,
         MCU_GPIO_EVENT_TYPE_INPUT_RISING_EDGE,
         eventInput2_callBack);

   bsp_uart_write("Trabajo Final \n\r", 16);
   TerminateTask();
}

TASK(InputEvTask1)
{
   while (1)
   {
      WaitEvent(evTask);
      ClearEvent(evTask);
      Binario_to_BCD(bsp_readTimer(), Buffer, 0, 8);
      Buffer[8]='\0';
      switch(Estado_Rampa){
    	  case RAMPA_SUBIDA:
    		  Estado_Rampa = RAMPA_PARADA;
    		  CancelAlarm(ActivateRampaTask);
				break;
    	  case RAMPA_BAJADA:
    		  Estado_Rampa = RAMPA_PARADA;
    		  CancelAlarm(ActivateRampaTask);
    			  break;
    	  case RAMPA_PAUSADA:
    		  Estado_Rampa = RAMPA_PARADA;

				  break;
    	  case RAMPA_PARADA:
    		  SetRelAlarm(ActivateRampaTask, FIRST_START_DELAY_MS, PERIOD_MS);
    		  Estado_Rampa = RAMPA_SUBIDA;
   			  break;
    	  default:
    		  break;
      }
      Color_Led = COLOR_AMARILLO;
      if(Estado_Rampa == RAMPA_PARADA){
    	  bsp_SetDutyCicle(MCU_PWM_CHANNEL0, 0);
    	  bsp_SetDutyCicle(MCU_PWM_CHANNEL1, 0);
    	  bsp_SetDutyCicle(MCU_PWM_CHANNEL2, 0);
    	  strcat(Buffer,": Secuencia Finalizada\n\r");
    	  bsp_uart_write(Buffer, 32);
      }else{
    	  strcat(Buffer,": Inicio Secuencia\n\r");
    	  bsp_uart_write(Buffer, 28);
    	  bsp_uart_write("Encendido Led Amarillo\n\r", 28);
      }

   }
}



TASK(InputEvTask2)
{
   while (1)
   {
      WaitEvent(evTask);
      ClearEvent(evTask);
      Binario_to_BCD(bsp_readTimer(), Buffer, 0, 8);
      Buffer[8]='\0';
      switch(Estado_Rampa){
        	  case RAMPA_SUBIDA:
				  CancelAlarm(ActivateRampaTask);
				  Bk_Estado_Rampa = Estado_Rampa;
				  Estado_Rampa = RAMPA_PAUSADA;
				  strcat(Buffer,": Secuencia Pausada\n\r");
				  bsp_uart_write(Buffer, 29);
				  break;
        	  case RAMPA_BAJADA:
        		  CancelAlarm(ActivateRampaTask);
        		  Bk_Estado_Rampa = Estado_Rampa;
        		  Estado_Rampa = RAMPA_PAUSADA;
        		  strcat(Buffer,": Secuencia Pausada\n\r");
        		  bsp_uart_write(Buffer, 29);
        		  break;
        	  case RAMPA_PAUSADA:
        		  SetRelAlarm(ActivateRampaTask, FIRST_START_DELAY_MS, PERIOD_MS);
        		  Estado_Rampa=Bk_Estado_Rampa;
        		  strcat(Buffer,": Secuencia Reanudada\n\r");
        		  bsp_uart_write(Buffer, 32);
        		  break;
        	  default:
        		  break;
          }


   }
}


TASK(RampaTask){
	switch(Estado_Rampa){
		case RAMPA_SUBIDA:
			Duty_Cycle+=5;
			if(Duty_Cycle == 100){
				Estado_Rampa = RAMPA_BAJADA;
				Binario_to_BCD(bsp_readTimer(), Buffer, 0, 8);
				Buffer[8]='\0';
				strcat(Buffer,": Intensidad Maxima ");
				switch(Color_Led){
					case COLOR_AMARILLO:
						strcat(Buffer,"Led Amarillo\n\r");
						bsp_uart_write(Buffer, 42);
						break;
					case COLOR_ROJO:
						strcat(Buffer,"Led Rojo\n\r");
						bsp_uart_write(Buffer, 38);
						break;
					case COLOR_VERDE:
						strcat(Buffer,"Led Verde\n\r");
						bsp_uart_write(Buffer, 39);
						break;
				}

			}
			break;
		case RAMPA_BAJADA:
			Duty_Cycle-=5;
			if(Duty_Cycle == 0){
				Estado_Rampa = RAMPA_SUBIDA;
			  bsp_SetDutyCicle(MCU_PWM_CHANNEL0, 0);
	    	  bsp_SetDutyCicle(MCU_PWM_CHANNEL1, 0);
	    	  bsp_SetDutyCicle(MCU_PWM_CHANNEL2, 0);
				Duty_Cycle = 5;
				Binario_to_BCD(bsp_readTimer(), Buffer, 0, 8);
				Buffer[8]='\0';
				strcat(Buffer,": Encendido ");
				switch(Color_Led){
					case COLOR_AMARILLO:
						Color_Led = COLOR_ROJO;
						strcat(Buffer,"Led Rojo\n\r");
						bsp_uart_write(Buffer, 30);
						break;
					case COLOR_ROJO:
						Color_Led = COLOR_VERDE;
						strcat(Buffer,"Led Verde\n\r");
						bsp_uart_write(Buffer, 31);
						break;
					case COLOR_VERDE:
						Color_Led = COLOR_AMARILLO;
						strcat(Buffer,"Led Amarillo\n\r");
						bsp_uart_write(Buffer, 34);
						break;
				}
			}
			break;
	}
	switch(Color_Led){
		case COLOR_AMARILLO:
			bsp_SetDutyCicle(MCU_PWM_CHANNEL0, Duty_Cycle);
			break;
		case COLOR_ROJO:
			bsp_SetDutyCicle(MCU_PWM_CHANNEL1, Duty_Cycle);
			break;
		case COLOR_VERDE:
			bsp_SetDutyCicle(MCU_PWM_CHANNEL2, Duty_Cycle);
			break;
		default:
			break;

	}

	TerminateTask();
}

ISR(UART2_IRQHandler){
	bsp_uart_IRQ();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

