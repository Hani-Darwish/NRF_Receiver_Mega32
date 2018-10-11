/*
 * Buffer.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Ibrahim
 */
#ifndef SERVICELAYER_BUFFER_H_
#define SERVICELAYER_BUFFER_H_

#define  UART_COM


typedef struct circular_buffer
{
    void *buffer;     // data buffer
    void *buffer_end; // end of data buffer
    U8_t capacity;  // maximum number of items in the buffer
    int count;     // number of items in the buffer
    U8_t sz;        // size of each item in the buffer
    U8_t *head;       // pointer to head
    U8_t *tail;       // pointer to tail
} circular_buffer;


/****************************************************************************
 * Function    : BufferInit
 *
 * DESCRIPTION : init the UART buffer
 *
 * PARAMETERS  :
 *
 * Return Value: None.
 ******************************************************************************/
void vBuffer_Init();

/****************************************************************************
* Function    : Buffer_readByte
*
* DESCRIPTION : init the UART buffer
*
* PARAMETERS  : item: pointer to received byte
*
* Return Value: return -1 if Buffer is Empty
******************************************************************************/
int Buffer_readByte( U8_t *item);

/****************************************************************************
 * Function    : getAvailableDataCountOnUART
 *
 * DESCRIPTION : check if there is a data in buffer or not
 *
 * PARAMETERS  : uart_cfg :
 *
 * Return Value: return the number of data in the buffer
 * ******************************************************************************/
int16_t Buffer_AvailableDataCount();

/****************************************************************************
* Function    : u8Buffer_Add
*
* DESCRIPTION : init the UART buffer
*
* PARAMETERS  : item:  written byte
*
* Return Value: return true if buffer is full
******************************************************************************/
U8_t u8Buffer_Add(U8_t item);

/****************************************************************************
* Function    : b_IsBufferFull
*
* DESCRIPTION :check if buffer is full or not
*
* PARAMETERS  : None
*
* Return Value: true if full
******************************************************************************/
BOOLEAN b_IsBufferFull ();

/****************************************************************************
* Function    : b_IsBufferEmpty
*
* DESCRIPTION :check if buffer is empty or not
*
* PARAMETERS  : None
*
* Return Value: true if empty
******************************************************************************/
BOOLEAN b_IsBufferEmpty ();
#endif /* SERVICELAYER_BUFFER_H_ */

