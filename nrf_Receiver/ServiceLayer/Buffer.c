/*
 * Buffer.c
 *
 *  Created on: ??�/??�/????
 *      Author: Ibrahim
 */
#include <avr/io.h>
#include "../Utilities/EF_SpechialTIMER.h"
#include "../Utilities/std_types.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Buffer.h"
#include "../MCAL/EF_UART.h"
#define BUFFER_LENGTH    50
U8_t BufferArray[BUFFER_LENGTH] = {0};

circular_buffer CircularBuffer ;
BOOLEAN isBufferFull =0;
BOOLEAN isBufferEmpty =0;

/****************************************************************************
* Function    : BufferInit
*
* DESCRIPTION : init the UART buffer
*
* PARAMETERS  : None.
*
* Return Value: None.
******************************************************************************/
void vBuffer_Init( )
{
    CircularBuffer.buffer     = BufferArray;
    CircularBuffer.buffer_end = (char *)CircularBuffer.buffer + BUFFER_LENGTH;
    CircularBuffer.capacity   = BUFFER_LENGTH;
    CircularBuffer.count = 0;
    CircularBuffer.sz    = 1;
    CircularBuffer.head  = CircularBuffer.buffer;
    CircularBuffer.tail  = CircularBuffer.buffer;
}


/****************************************************************************
* Function    : u8Buffer_Add
*
* DESCRIPTION : init the UART buffer
*
* PARAMETERS  : item:  written byte
*
* Return Value: return true if buffer is full
******************************************************************************/
U8_t u8Buffer_Add(U8_t item)
{
	/* if array is full , set flag */
    if(CircularBuffer.count == CircularBuffer.capacity)
    {
    	isBufferFull = TRUE;
    }
    else
    {
    	isBufferFull = FALSE;
    }
    /* add the new element to (head) then increment (head) pointer */
    *(CircularBuffer.head) =  item;
    CircularBuffer.head = CircularBuffer.head + CircularBuffer.sz;
    //data will be overwrite
    if(CircularBuffer.head == CircularBuffer.buffer_end)
    {
        CircularBuffer.head = CircularBuffer.buffer;
    }
    CircularBuffer.count++;
    return isBufferFull;
}

/****************************************************************************
* Function    : Buffer_readByte
*
* DESCRIPTION : init the UART buffer
*
* PARAMETERS  : item: pointer to received byte
*
* Return Value: return -1 if Buffer is Empty
******************************************************************************/
int Buffer_readByte( U8_t *item)
{

    if(CircularBuffer.count == 0)
    {
    	isBufferEmpty = TRUE;
        return -1;
    }
    else
    {
    	isBufferEmpty = FALSE;
    }
    /* read the tail element and then increment tail pointer */
    *item =  *CircularBuffer.tail;
    CircularBuffer.tail++;
    if(CircularBuffer.tail == CircularBuffer.buffer_end)
    {
        CircularBuffer.tail = CircularBuffer.buffer;
    }
    CircularBuffer.count--;
    return TRUE;
}

/****************************************************************************
* Function    : Buffer_AvailableDataCount
*
* DESCRIPTION :return the number of unread data in buffer
*
* PARAMETERS  : None
*
* Return Value: number of data in buffer
******************************************************************************/
int16_t Buffer_AvailableDataCount()
{
//	EF_void_UART_PutChar(CircularBuffer.count);
//	EF_void_UART_SendArray((U8_t*) "\r\n", 2);
	return CircularBuffer.count;
}

/****************************************************************************
* Function    : b_IsBufferEmpty
*
* DESCRIPTION :check if buffer is empty or not
*
* PARAMETERS  : None
*
* Return Value: true if empty
******************************************************************************/
BOOLEAN b_IsBufferEmpty ()
{
	return isBufferEmpty;
}

/****************************************************************************
* Function    : b_IsBufferFull
*
* DESCRIPTION :check if buffer is full or not
*
* PARAMETERS  : None
*
* Return Value: true if full
******************************************************************************/
BOOLEAN b_IsBufferFull ()
{
	return isBufferFull;
}

ISR(USART_RX_vect)
{
   uint8_t data;
   data = UDR;
   if(CircularBuffer.count != CircularBuffer.capacity)
   {
	   *(CircularBuffer.head) = data;
	   CircularBuffer.head++;
	   //data will be overwrite
	   if(CircularBuffer.head == CircularBuffer.buffer_end)
	   {
	       CircularBuffer.head = CircularBuffer.buffer;
	   }
	   CircularBuffer.count++;
   }
}


// SPI Transmission/reception complete ISR
ISR(SPI_STC_vect)
{
	uint8_t data;

	data = SPDR;
	if(CircularBuffer.count != CircularBuffer.capacity)
	{
		*(CircularBuffer.head) = data;
		CircularBuffer.head++;
		//data will be overwrite
		if(CircularBuffer.head == CircularBuffer.buffer_end)
		{
			CircularBuffer.head = CircularBuffer.buffer;
		}
		CircularBuffer.count++;
	}
}
