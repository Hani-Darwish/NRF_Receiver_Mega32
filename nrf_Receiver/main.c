
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "HAL/nrf24l01.h"
#include "HAL/nrf24l01registers.h"
#include "ServiceLayer/EF_Frame.h"
#include "Utilities/EF_SpechialTIMER.h"
#include "MiddleWare/NodsPack.h"
#include "MCAL/EF_SPI.h"
#include "MCAL/EF_UART.h"

#include "ServiceLayer/Program_Handl.h"

/* define debug print enable or disable on uart */
#define DEBUGENABLED 0
#define CHECK_DEBUG  0

/*RF channels */
//#define RF_CHNL_1          76
//#define RF_CHNL_2          92


/* Config UART for debugging */
UART_cfg_str uart_cfg = {9600, 8, ONE_STOP_BIT, NO_PARITY, FALSE, FALSE, 1,1 };

/* set addresses */
uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;
uint8_t addrtx1[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP1;
uint8_t addrtx2[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP2;
uint8_t addrtx3[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP3;
uint8_t addrtx4[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP4;
uint8_t addrtx5[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP5;


extern U8_t g_rfChanl;



int main(void)
{

	uint8_t i = 0;
	uint8_t data_recvd_pipe = 0;
//	char rf_chanel = NRF24L01_CH;
	U8_t months_error[13] = {0};
	U8_t end_progStutas = 0;

	wrt_nrf_DataStatus ret_write = 0;

	/* nrf24l01 variables */
	uint8_t bufferin[NRF24L01_PAYLOAD];
	/*init special timer */
	EF_void_TimerInit();
	EF_void_TimerCreate(CHNL_SCN_ID,CHNL_SCN_TIMEOUT);
	EF_SpiInit(1);
	EF_void_UART_Init(&uart_cfg);
	EF_nrf24l01_init(g_rfChanl);
	EF_void_UART_SendString("Init as a receiver \n");

	/* load the buffer to send over rf */
	for(i=0; i<sizeof(bufferin); i++)
		bufferin[i] = 0;

	for(;;)
	{
		/*--------------------------------------------------------------------------------------------------------
		 * 									  Receive Mode
		 * --------------------------------------------------------------------------------------------------------*/
		// timer APIS
		EF_void_TimerStart(CHNL_SCN_ID);
		do
		{
			memset(bufferin,0,sizeof(bufferin));

			if(EF_nrf24l01_readready(&data_recvd_pipe))   //if data is ready
			{
				/* read buffer */
				EF_nrf24l01_readData(bufferin);
				end_progStutas = Receive_Programing_Mode(bufferin, months_error);

				if(end_progStutas == TRUE)
				{
					EF_void_UART_SendString("send error frame \n");
					/* change mode to TX to send the error frame */
					/* set tx address for pipe 2 */
					nrf24l01_settxaddr(addrtx2);
					nrf24l01_setTXMode();
					Send_ErrorFrame(months_error);
					nrf24l01_setRXMode();
				}


			}/* end of read data */

		}while(1);

//		}while(!(EF_BOOLEAN_TimerCheck(CHNL_SCN_ID)));
		EF_void_TimerStop(CHNL_SCN_ID);
		EF_void_TimerReset(CHNL_SCN_ID);

	}
	return 0;
}
