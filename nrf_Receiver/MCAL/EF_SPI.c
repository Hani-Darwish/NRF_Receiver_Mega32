/***************************************************************
 *  Source File: EF_SPI.c
 *
 *  Description: SPI driver for ATMEGA32
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     25/08/2014
 *  -------------------------------------------------------
 *  Author :     EmbeddedFab.
 *
 *  Copyright (C) <2014>  <EmbeddedFab>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>

 **************************************************************/
#include "EF_SPI.h"
#include <util/delay.h>
#include <avr/io.h>
#include "EF_SPI_cfg.h"
#include "../Utilities/EF_SpechialTIMER.h"
#include "../Utilities/EF_SpechialTIMER_cfg.h"

/* globle type to return the status of spi */
enu_spiEror g_enu_spiEror;


/*********************************************************************
 * Function    : void EF_SpiInit(U8_t DeviceType);
 *
 * DESCRIPTION : This function used to Initialize SPI Module.
 *
 * PARAMETERS  : U8_t DeviceType : 1 ----> Master
 *                                 0 ----> Slave
 * Return Value: void
 **********************************************************************/

void EF_SpiInit(U8_t DeviceType)
{

	/* Set the device as Master */
	if(MASTER_TYPE == DeviceType)
	{
		/* Enable SPI -- MSB first-- Focs/128 */
		/* make (MOSI) PB5 & (SCK)PB7 & (SS)PB4 : output */
		SPI_DDR |= (1<<MOSI_BIT) | (1<<SS_BIT) | (1<<SCK_BIT);

		SPCR = (  (1<<SPE)   |                /* SPI Enable  */
				(0<<DORD)  |                /* Data Order (0:MSB first / 1:LSB first) */
				(0<<SPR1)  | (1<<SPR0)|     /* SPI Clock Rate */
				(0<<CPOL)  |                /* Clock Polarity (0:SCK low / 1:SCK hi when idle */
				(0<<CPHA)  |                /*  Clock Phase (0:leading / 1:trailing edge sampling)*/
				(1 << MSTR));               /* Mater  */
	}
	else
	{
		SPI_DDR |= (1<<MISO_BIT);		   /* Slave , make (MISO) PB6 : output */
	}

	/* using Special Timer to able some SPI Function to be unstuck */
	EF_void_TimerCreate(SPI_TIMER_ID, SPI_TIMEOUT);
}

/*********************************************************************
 * Function    : U8_t EF_SpiTransfer(U8_t data);
 *
 * DESCRIPTION : This function used to Transfer Data through SPI bus
 *
 * PARAMETERS  : U8_t data  Data user need to transmit.
 *
 *
 * Return Value: Data User received
 **********************************************************************/
BOOLEAN EF_SpiTransfer(U8_t data, U8_t *data_return)
{
	BOOLEAN Status = TRUE;
	EF_void_TimerStart(SPI_TIMER_ID);  		/* using Special Timer */
	SPDR = data; 							/* Put data into buffer, send the data */

	/* Wait for empty transmit buffer  or time out */
	while (!( SPSR & (1<<SPIF)) && (!EF_BOOLEAN_TimerCheck(SPI_TIMER_ID)));
	*data_return = SPDR;   /* Return data register */

	if(EF_BOOLEAN_TimerCheck(SPI_TIMER_ID))
	{
		Status = 0;
	}
	else
	{

		Status = 1;
	}
	EF_void_TimerStop(SPI_TIMER_ID);
	EF_void_TimerReset(SPI_TIMER_ID);
	return Status;
}


/*************************************************************************************
 * Function    : EF_Multi_SpiTransfer(U8_t* Data_in,U8_t* Data_out ,U8_t Data_lenght);
 *
 * DESCRIPTION : This function used to Transfer Data through SPI bus.
 *
 * PARAMETERS  : U8_t* Data_out      pointer to transfered data.
 * 				U8_t Data_lenght    length of the data.
 *
 * Return Value: U8_t* Data_in       pointer to received data.
 ****************************************************************************************/

//void EF_Multi_SpiTransfer(U8_t* Data_in, U8_t* Data_out, U8_t Data_lenght)
//{
//	U8_t index;  /*iterator*/
//	for(index = 0 ; index < Data_lenght; index++)
//	{
////		Data_in[index] = EF_SpiTransfer(Data_out[index]);
//
//		 EF_SpiTransfer(Data_out[index], Data_in[index]);
//
//	}
//}

/********************************************************************************
 * Function    : void EF_Multi_SpiTransmit(U8_t* Data_out, U8_t Data_lenght);
 *
 * DESCRIPTION : This function used to Transmit Data through SPI bus.
 *
 * PARAMETERS  : U8_t* Data_out      pointer to transfered data.
 * 				U8_t Data_lenght    length of the data.
 *
 * Return Value: void.
 *******************************************************************************/
//void EF_Multi_SpiTransmit(U8_t* Data_out, U8_t Data_lenght)
//{
//	U8_t index;  /*iterator*/
//	for(index = 0 ; index < Data_lenght ; index++)
//	{
//		EF_SpiTransfer(Data_out[index],Data_out[index]);
//
//
//	}
//}

