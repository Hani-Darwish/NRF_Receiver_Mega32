
#include <util/delay.h>

#include "EF_Frame.h"
#include "../HAL/nrf24l01.h"
#include "../MCAL/EF_UART.h"



#define START_PROGRM_DATA          'A'
#define END_PROGRM_DATA            'E'
#define REPORT_ERROR_FRAMLENGTH     13


enum
{
	programing_correct,
	programing_fault,
};


U8_t Send_StartPrograming(void);
U8_t Send_EndPrograming(void);
U8_t Send_DayPosition(U8_t* day_position);
U8_t Send_StartBlock(U8_t block_ID);
U8_t Send_ProgramingBlock(U8_t block_ID);
U8_t get_DaysPerMonth(U8_t month_index);

U8_t Send_ErrorFrame(U8_t *months_error );

U8_t Receive_ErrorFrame(U8_t *bufferin, U8_t *blocks_error, U8_t *blocks_error_number);
U8_t Receive_Programing_Mode(U8_t *bufferin, U8_t *months_error);



