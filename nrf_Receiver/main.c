
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



static U8_t u8MonthDays[13]   = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};




/* define debug print enable or disable on uart */
#define DEBUGENABLED 0
#define CHECK_DEBUG  0

/*RF channels */
//#define RF_CHNL_1          76
//#define RF_CHNL_2          92
/* Receiver Scan */


/* Config UART for debugging */
UART_cfg_str uart_cfg = {9600, 8, ONE_STOP_BIT, NO_PARITY, FALSE, FALSE, 1,1 };

/* set addresses */
uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;
uint8_t addrtx1[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP1;
uint8_t addrtx2[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP2;
uint8_t addrtx3[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP3;
uint8_t addrtx4[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP4;
uint8_t addrtx5[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP5;



int main(void)
{

	uint8_t i = 0;
	unsigned int rec_count = 0;
	uint8_t data_recvd_pipe = 0;
	char rf_chanel = NRF24L01_CH;

	U8_t DayPrMonth = 0;

	wrt_nrf_DataStatus ret_write = 0;

	str_received_data recv_data ;

	U8_t months_error[12] = {0};
	static U8_t months_error_index = 0;


	/* nrf24l01 variables */
	uint8_t bufferin[NRF24L01_PAYLOAD];
	/*init special timer */
	EF_void_TimerInit();
	EF_void_TimerCreate(CHNL_SCN_ID,CHNL_SCN_TIMEOUT);
	EF_SpiInit(1);
	EF_void_UART_Init(&uart_cfg);
	EF_nrf24l01_init(rf_chanel);
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
				EF_parseFrame(bufferin, &recv_data);

//				EF_void_UART_SendString("\ndata");
//				EF_void_UART_SendArray(bufferin,32);
//				EF_void_UART_SendString("\n");






				if(recv_data.subcomand == start_programing_cmnd)
				{
					EF_void_UART_SendString("Received start programming command\n");

				}
				else if(recv_data.subcomand == start_block_cmnd)
				{
					EF_void_UART_SendString("received start block for month ");
					EF_void_UART_Send_Integer(recv_data.payload[0]);
					EF_void_UART_SendString("\n");
					DayPrMonth = 0;

				}
				else if(recv_data.subcomand ==  programing_block_cmnd)
				{
					DayPrMonth++;

				}
				else if(recv_data.subcomand == end_programing_cmnd )
				{
					EF_void_UART_SendString("received end programming mode\n");

					if(months_error_index == 0)
					{
						EF_void_UART_SendString("No error in programming\n");
					}
					else
					{
						EF_void_UART_SendString("ERROR in Block ");
						for(U8_t iter = 0; iter < months_error_index; iter++)
						{
							EF_void_UART_Send_Integer(months_error[iter]);
							EF_void_UART_SendString("   ");
						}
						EF_void_UART_SendString("\n");
					}

					/* init the index */
					months_error_index = 0;

				}
				else if(recv_data.subcomand == end_block_cmnd )
				{
					EF_void_UART_SendString("received end block\n");
					EF_void_UART_SendString("received ");
					EF_void_UART_Send_Integer(DayPrMonth);
					EF_void_UART_SendString(" day\n");
					/* check programming error per month */
					U8_t expected_dayspermonth = get_DaysPerMonth(recv_data.payload[0]);
					if(expected_dayspermonth == DayPrMonth)
					{
						EF_void_UART_SendString("received block correct\n");
					}
					else
					{
						EF_void_UART_SendString("received block ERROR\n");
						months_error[months_error_index] =  recv_data.payload[0];
						months_error_index++;

					}

				}

				else
				{

				}









//				rec_count++;
//				EF_void_UART_SendString("receive counter ");
//				EF_void_UART_Send_Integer(rec_count);
//				EF_void_UART_SendString("\n");

			}/* end of read data */


		}while(1);


//		}while(!(EF_BOOLEAN_TimerCheck(CHNL_SCN_ID)));
		EF_void_TimerStop(CHNL_SCN_ID);
		EF_void_TimerReset(CHNL_SCN_ID);

	}
	return 0;
}
