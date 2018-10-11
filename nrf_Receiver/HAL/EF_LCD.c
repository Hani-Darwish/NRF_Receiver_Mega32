/***************************************************************
 *  Source File: EF_LCD.c
 *
 *  Description: This simple driver for Character LCD 2x16 4 bit mode.
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     25/08/2014
 *  -------------------------------------------------------
 *  Author :
 *
 *  Copyright (c) EmbeddedFab, 2014
 *
 *  This unpublished material is proprietary to EmbeddedFab.
 *  All rights reserved. The methods and techniques described
 *  herein are considered trade secrets and/or confidential.
 *  Reproduction or distribution, in whole or in part, is
 *  forbidden except by express written permission of
 *  EmbeddedFab.
 **************************************************************/
/**************************************************************
 * Notes: This driver can use with 4 bit mode only.
 *
 **************************************************************/

#include <string.h>
#include "EF_LCD.h"
#include <avr/io.h>
#include <stdlib.h>



/*********************************************************************
 * Function    : void initLCD(void);
 *
 * DESCRIPTION : This function used to Initialize LCD Ports and send suitable default
 *               CMD before start display.
 *
 * PARAMETERS  : Void.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_init()
{
	/* Init Control PIN as Output */
	LCD_CNTRL_DDR |= (1 << LCD_RS_PIN | 1 << LCD_RW_PIN | 1 << LCD_ENABLE_PIN);
	LCD_CNTRL_PORT = 0x00;

	/* Init Data PIN as Output 4,5,6,7 */
	LCD_DATA_DDR  |= 0xF0;
	LCD_DATA_PORT |= 0x00;

	_delay_ms(10);
	EF_void_LCD_send_command(0x28);            /* Command ..0x28---> 4-bit mode - 2 line - 5x7 font */
	EF_void_LCD_send_command(0x0C);            /* Command ..0x0C---> Display no cursor - no blink */
	EF_void_LCD_send_command(0x06);            /* Command ..0x06---> Automatic Increment - No Display shift */
	EF_void_LCD_send_command(0x80);            /* Command ..0x80---> Address DDRAM with 0 offset 80h */
	_delay_ms(20);
}


/*********************************************************************
 * Function    : void EF_EF_void_LCD_send_command(U8_t cmnd);
 *
 * DESCRIPTION : This function used to send CMD to LCD like move or control
 *               display mode.
 *
 * PARAMETERS  : U8_t cmnd:  Command User want to send.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_send_command(U8_t cmnd)
{
	/*RS and RW will be LOW */
	LCD_CNTRL_PORT &= ~(1<<LCD_RW_PIN);
	LCD_CNTRL_PORT &= ~(1<<LCD_RS_PIN);

	/* output high nibble first */
	LCD_DATA3_PORT &= ~_BV(LCD_DATA3_PIN);
	LCD_DATA2_PORT &= ~_BV(LCD_DATA2_PIN);
	LCD_DATA1_PORT &= ~_BV(LCD_DATA1_PIN);
	LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);
	if(cmnd & 0x80) LCD_DATA3_PORT |= _BV(LCD_DATA3_PIN);
	if(cmnd & 0x40) LCD_DATA2_PORT |= _BV(LCD_DATA2_PIN);
	if(cmnd & 0x20) LCD_DATA1_PORT |= _BV(LCD_DATA1_PIN);
	if(cmnd & 0x10) LCD_DATA0_PORT |= _BV(LCD_DATA0_PIN);

	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);

	/* output low nibble */
	LCD_DATA3_PORT &= ~_BV(LCD_DATA3_PIN);
	LCD_DATA2_PORT &= ~_BV(LCD_DATA2_PIN);
	LCD_DATA1_PORT &= ~_BV(LCD_DATA1_PIN);
	LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);
	if(cmnd & 0x08) LCD_DATA3_PORT |= _BV(LCD_DATA3_PIN);
	if(cmnd & 0x04) LCD_DATA2_PORT |= _BV(LCD_DATA2_PIN);
	if(cmnd & 0x02) LCD_DATA1_PORT |= _BV(LCD_DATA1_PIN);
	if(cmnd & 0x01) LCD_DATA0_PORT |= _BV(LCD_DATA0_PIN);
	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
	_delay_us(300);
}

/*********************************************************************
 * Function    : void EF_void_LCD_send_data(U8_t data);
 *
 * DESCRIPTION : This function used to send data to LCD .
 *
 * PARAMETERS  : U8_t Data:  Data User want to send.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_send_data(char data)
{


	LCD_CNTRL_PORT &= ~(1<<LCD_RW_PIN);
	LCD_CNTRL_PORT |= (1<<LCD_RS_PIN);

	/* output high nibble first */
	LCD_DATA3_PORT &= ~_BV(LCD_DATA3_PIN);
	LCD_DATA2_PORT &= ~_BV(LCD_DATA2_PIN);
	LCD_DATA1_PORT &= ~_BV(LCD_DATA1_PIN);
	LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);
	if(data & 0x80) LCD_DATA3_PORT |= _BV(LCD_DATA3_PIN);
	if(data & 0x40) LCD_DATA2_PORT |= _BV(LCD_DATA2_PIN);
	if(data & 0x20) LCD_DATA1_PORT |= _BV(LCD_DATA1_PIN);
	if(data & 0x10) LCD_DATA0_PORT |= _BV(LCD_DATA0_PIN);

	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);

	/* output low nibble */
	LCD_DATA3_PORT &= ~_BV(LCD_DATA3_PIN);
	LCD_DATA2_PORT &= ~_BV(LCD_DATA2_PIN);
	LCD_DATA1_PORT &= ~_BV(LCD_DATA1_PIN);
	LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);
	if(data & 0x08) LCD_DATA3_PORT |= _BV(LCD_DATA3_PIN);
	if(data & 0x04) LCD_DATA2_PORT |= _BV(LCD_DATA2_PIN);
	if(data & 0x02) LCD_DATA1_PORT |= _BV(LCD_DATA1_PIN);
	if(data & 0x01) LCD_DATA0_PORT |= _BV(LCD_DATA0_PIN);

	LCD_CNTRL_PORT |= (1<<LCD_ENABLE_PIN);
	_delay_us(2);
	LCD_CNTRL_PORT &= ~(1<<LCD_ENABLE_PIN);
	_delay_us(300);
}

/***********************************************************************
 * Function    : void EF_void_LCD_goto(U8_t y, U8_t x);
 *
 * DESCRIPTION : This function moves the cursor the line y column x
 *               on the LCD module.
 *
 * PARAMETERS  : U8_t y, U8_t x.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_goto(U8_t y, U8_t x)
{
	U8_t firstAddress[] = {0x80,0xC0,0x94,0xD4};

	EF_void_LCD_send_command(firstAddress[y-1] + x-1);
	_delay_ms(10);
}

void EF_void_LCD_print_NUM(S16_t s16Numberstring,U8_t u8ColNumber)
{
	U8_t i = 0;
	char s8DisplayStr[10] = { '*','*' ,'*','*' ,'*','*','*' ,'*','*' ,'*'};

	itoa(s16Numberstring, s8DisplayStr, 10);
	for(i = 0; i < 9; i++)
	{
		EF_void_LCD_goto(u8ColNumber, (i + 1));
		EF_void_LCD_send_data(s8DisplayStr[i]);
	}
}

/***********************************************************************
 * Function    : void EF_void_LCD_print(U8_t *string);
 *
 * DESCRIPTION : This function used to display String user
 *               want to diplay .
 *
 * PARAMETERS  : U8_t *string:  String User want to send.
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_print(char *string)
{
	U8_t i = 0;

	while(string[i]!=0)
	{
		EF_void_LCD_send_data(string[i]);
		i++;
	}
}


void EF_void_LCD_print_ByLength(unsigned char *string, U8_t length)
{
	U8_t i = 0;

	while( i < length)
	{
		EF_void_LCD_send_data(string[i]);
		i++;
	}
}


void EF_void_LCD_Clear_Screen(void)
{
	EF_void_LCD_send_command(1<<0);
	_delay_us(2);
	_delay_ms(10);
}


void EF_void_uploadCustomChar(U8_t location, unsigned char *character)
{
	U8_t i;

	if(location<8)
	{
		EF_void_LCD_send_command(0x40+(location*8));
		for(i=0;i<8;i++)
			EF_void_LCD_send_data(character[i]);
	}
}
