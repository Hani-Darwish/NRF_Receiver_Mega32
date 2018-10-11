/*
 * EF_LCD.h
 *
 *  Created on: ??�/??�/????
 *      Author: Kareem140
 */

#ifndef EF_LCD_H_
#define EF_LCD_H_

#include <avr/io.h>
//#define F_CPU 8000000UL

#include <util/delay.h>

#include "../Utilities/std_types.h"


#define LCD_DATA_PORT	  PORTB
#define LCD_DATA_DDR	  DDRB
#define LCD_DATA_PIN	  PINB

#define LCD_DATA0_PORT    LCD_DATA_PORT     /**< port for 4bit data bit 0 */
#define LCD_DATA1_PORT    LCD_DATA_PORT     /**< port for 4bit data bit 1 */
#define LCD_DATA2_PORT    LCD_DATA_PORT     /**< port for 4bit data bit 2 */
#define LCD_DATA3_PORT    LCD_DATA_PORT     /**< port for 4bit data bit 3 */
#define LCD_DATA0_PIN     4                 /**< pin for 4bit data bit 0  */
#define LCD_DATA1_PIN     5                 /**< pin for 4bit data bit 1  */
#define LCD_DATA2_PIN     6                 /**< pin for 4bit data bit 2  */
#define LCD_DATA3_PIN     7                 /**< pin for 4bit data bit 3  */

#define LCD_CNTRL_PORT	  PORTC
#define LCD_CNTRL_DDR	  DDRC
#define LCD_CNTRL_PIN	  PINC

#define LCD_RS_PIN		  6
#define LCD_RW_PIN		  5
#define LCD_ENABLE_PIN	  7

#define SET_HOUR		  3
#define SET_MINUTE		  4


void EF_void_LCD_print_NUM(S16_t s16Numberstring,U8_t u8ColNumber);

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
void EF_void_LCD_init();


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
void EF_void_LCD_send_command(U8_t cmnd);


/*********************************************************************
* Function    : void EF_void_LCD_send_data(U8_t data);
*
* DESCRIPTION : This function used to send data to LCD .
*
* PARAMETERS  : U8_t Data:  Data User want to send.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_send_data(char data);


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
void EF_void_LCD_goto(U8_t y, U8_t x);

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
void EF_void_LCD_print(char *string);
void EF_void_LCD_print_ByLength(unsigned char *string, U8_t length);
void EF_void_LCD_Clear_Screen(void);

void EF_void_uploadCustomChar(U8_t location, unsigned char *character);
#endif /* EF_LCD_H_ */
