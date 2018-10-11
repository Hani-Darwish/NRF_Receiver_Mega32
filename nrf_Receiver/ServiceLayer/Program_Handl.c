
#include "Program_Handl.h"

/* the buiding frame array  */
extern U8_t g_FramBuild[MAX_FRAM_SIZE];

static U8_t u8MonthDays[13]   = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

U8_t progring_block[620] = {0};




U8_t Send_StartPrograming(void)
{
	U8_t index = 0;
	gstr_buildFram buildFram_str;
	U8_t Start_ProgramStatus = nrf_transmit_failed;

	buildFram_str.source_ID      = SOURCE_ID;
	buildFram_str.destination_ID = DESTINATION_ID;
	buildFram_str.massage_type   = CONFIG_MASSAGE;
	buildFram_str.frame_type     = SNGLE_FRAM;
	buildFram_str.subcomand      = start_programing_cmnd;

	buildFram_str.Payload_length = MAX_PAYLOAD_LENGTH;

	for(index = 0; index < MAX_PAYLOAD_LENGTH; index++)
	{
		buildFram_str.payload[index] = START_PROGRM_DATA;
	}

	/* build frame for start programming */
	EF_nrf_Build_Fram(& buildFram_str);
	/* send data over nrf module */
	Start_ProgramStatus = EF_nrf24l01_writeData(g_FramBuild, MAX_FRAM_SIZE);

	return (U8_t)Start_ProgramStatus;
}


U8_t Send_EndPrograming(void)
{

	U8_t index = 0;
	gstr_buildFram buildFram_str;
	U8_t END_ProgramStatus = nrf_transmit_failed;

	buildFram_str.source_ID      = SOURCE_ID;
	buildFram_str.destination_ID = DESTINATION_ID;
	buildFram_str.massage_type   = CONFIG_MASSAGE;
	buildFram_str.frame_type     = SNGLE_FRAM;
	buildFram_str.Payload_length = MAX_PAYLOAD_LENGTH;
	buildFram_str.subcomand      = end_programing_cmnd;

	for(index = 0; index < MAX_PAYLOAD_LENGTH; index++)
	{
		buildFram_str.payload[index] = END_PROGRM_DATA;
	}
	/* build frame for start programming */
	EF_nrf_Build_Fram(& buildFram_str);
	/* send data over nrf module */
	END_ProgramStatus = EF_nrf24l01_writeData(g_FramBuild, MAX_FRAM_SIZE);

	return (U8_t)END_ProgramStatus;

}




U8_t Send_StartBlock(U8_t block_ID)
{

	U8_t index = 0;
	gstr_buildFram buildFram_str;
	U8_t Start_BlockStatus = nrf_transmit_failed;

	buildFram_str.source_ID      = SOURCE_ID;
	buildFram_str.destination_ID = DESTINATION_ID;
	buildFram_str.massage_type   = CONFIG_MASSAGE;
	buildFram_str.frame_type     = SNGLE_FRAM;
	buildFram_str.Payload_length = MAX_PAYLOAD_LENGTH;
	buildFram_str.subcomand      = start_block_cmnd;


	/* TODO check the block id */
	buildFram_str.payload[index] = block_ID;

//	for(index = 0; index < MAX_PAYLOAD_LENGTH; index++)
//	{
//		buildFram_str.payload[index] = END_PROGRM_DATA;
//	}
	/* build frame for start programming */
	EF_nrf_Build_Fram(& buildFram_str);
	/* send data over nrf module */
	Start_BlockStatus = EF_nrf24l01_writeData(g_FramBuild, MAX_FRAM_SIZE);
	return (U8_t)Start_BlockStatus;

}


U8_t Send_DayPosition(U8_t* day_position)
{
	U8_t index = 0;
	gstr_buildFram buildFram_str;
	U8_t Send_FramStatus = nrf_transmit_failed;

	buildFram_str.source_ID      = SOURCE_ID;
	buildFram_str.destination_ID = DESTINATION_ID;
	buildFram_str.massage_type   = CONFIG_MASSAGE;
	buildFram_str.frame_type     = SNGLE_FRAM;
	buildFram_str.Payload_length = MAX_PAYLOAD_LENGTH;
	buildFram_str.subcomand      = programing_block_cmnd;

	for(index = 0; index < MAX_PAYLOAD_LENGTH; index++)
	{
		buildFram_str.payload[index] = day_position[index];
	}
	/* build frame for start programming */
	EF_nrf_Build_Fram(& buildFram_str);
	/* send data over nrf module */
	Send_FramStatus = EF_nrf24l01_writeData(g_FramBuild, MAX_FRAM_SIZE);
	return (U8_t)Send_FramStatus;

}



U8_t Send_ProgramingBlock(U8_t block_ID)
{
	U8_t status       = nrf_transmit_failed;

	U8_t month_index  = block_ID;
	U8_t iterator     = 0;
	U8_t position[20] = {0};

	U8_t fail  = 0;
	U8_t succs = 0;

	memset(position,7,sizeof(position));

	/* send start block with id */
	status = Send_StartBlock(block_ID);
	if(status == nrf_transmit_success)
	{
		/*  send data per month  */
		for(iterator = 0; iterator < u8MonthDays[month_index]; iterator++ )
		{
			status = Send_DayPosition(position);
			_delay_ms(150);

			if(status == nrf_transmit_success)
			{
				succs++;
			}
			else
			{
				fail++;
			}


//			EF_void_UART_SendString("day index ");
//			EF_void_UART_Send_Integer(iterator+1);
//			EF_void_UART_SendString("\n");

			/* update the daily position  */
		}


		EF_void_UART_SendString("succes per month = ");
		EF_void_UART_Send_Integer(succs);
		EF_void_UART_SendString("   ");

		EF_void_UART_SendString("fail per month = ");
		EF_void_UART_Send_Integer(fail);
		EF_void_UART_SendString("\n");

	}

	return status;
}


U8_t get_DaysPerMonth(U8_t month_index)
{
	if(month_index > 0)
	{
		return u8MonthDays[month_index];
	}
	else
		return -1;

}

