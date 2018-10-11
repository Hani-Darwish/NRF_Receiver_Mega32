
#include <util/delay.h>

#include "EF_Frame.h"
#include "../HAL/nrf24l01.h"
#include "../MCAL/EF_UART.h"



#define START_PROGRM_DATA        'A'
#define END_PROGRM_DATA          'E'

U8_t Send_StartPrograming(void);
U8_t Send_EndPrograming(void);
U8_t Send_DayPosition(U8_t* day_position);

U8_t Send_StartBlock(U8_t block_ID);
U8_t Send_ProgramingBlock(U8_t block_ID);

U8_t get_DaysPerMonth(U8_t month_index);




