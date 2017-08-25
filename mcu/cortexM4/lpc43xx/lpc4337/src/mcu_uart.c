/*

 * mcu_uart.c
 *
 *  Created on: 7 jul. 2017
 *      Author: lucho
 */
/*==================[inclusions]=============================================*/
#include "mcu.h"
#include "mcu_uart.h"
#include "chip.h"


/*==================[macros and definitions]=================================*/

typedef struct
{
   portPin_type tx;
   portPin_type rx;
   uint16_t modefunc;
}p_uart_type;

static const p_uart_type p_uart[] =
{
   {{7,1},   {7,2},   FUNC6},       /*TX:{port-pin} RX:{port-pin}  modeFunc*/
};


/*==================[external functions definition]==========================*/


/* Transmit and receive ring buffers */
static RINGBUFF_T txring, rxring;

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 512	/* Send */
#define UART_RRB_SIZE 512	/* Receive */

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];





void USART_2_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_USART2, &rxring, &txring);
}



void mcu_uart2_init(uint32_t b){
	//Setup UART
		/* Setup UART for 115.2K8N1 */
	Chip_SCU_PinMux(p_uart[0].tx.port, p_uart[0].tx.pin, MD_PDN,p_uart[0].modefunc);
	Chip_SCU_PinMux(p_uart[0].rx.port, p_uart[0].rx.pin, MD_PLN|MD_EZI|MD_ZI,p_uart[0].modefunc);

			Chip_UART_Init(LPC_USART2);
			Chip_UART_SetBaud(LPC_USART2, b);
			Chip_UART_ConfigData(LPC_USART2, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
			Chip_UART_TXEnable(LPC_USART2);

			/* Before using the ring buffers, initialize them using the ring
			   buffer init function */
			RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
			RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

			/* Reset and enable FIFOs, FIFO trigger level 3 (14 chars) */
			Chip_UART_SetupFIFOS(LPC_USART2, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |
									UART_FCR_TX_RS | UART_FCR_TRG_LEV3));
			/* Enable receive data and line status interrupt */
			Chip_UART_IntEnable(LPC_USART2, (UART_IER_RBRINT | UART_IER_RLSINT));

				/* preemption = 1, sub-priority = 1 */
		//	NVIC_SetPriority(USART2_IRQn, 1);
		//	NVIC_EnableIRQ(USART2_IRQn);

}

void mcu_uart_write2(uint8_t *buf, int32_t Longitud){
	Chip_UART_SendRB(LPC_USART2,&txring,buf,Longitud);

}

int32_t mcu_uart_read2(uint8_t *buf, int32_t Longitud){
	return Chip_UART_ReadRB(LPC_USART2,&rxring,buf,Longitud);

}


