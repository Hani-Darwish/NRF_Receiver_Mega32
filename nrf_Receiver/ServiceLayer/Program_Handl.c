
#include "Program_Handl.h"

/* the buiding frame array  */
extern U8_t g_FramBuild[MAX_FRAM_SIZE];

static U8_t u8MonthDays[13]   = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


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

	return Start_ProgramStatus;
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



U8_t Send_ErrorFrame(U8_t *months_error )
{

	U8_t index = 0;
	gstr_buildFram buildFram_str;
	U8_t Send_ErrorFram_Status = nrf_transmit_failed;

	buildFram_str.source_ID      = SOURCE_ID;
	buildFram_str.destination_ID = DESTINATION_ID;
	buildFram_str.massage_type   = CONFIG_MASSAGE;
	buildFram_str.frame_type     = SNGLE_FRAM;
	buildFram_str.Payload_length = MAX_PAYLOAD_LENGTH;
	buildFram_str.subcomand      = programing_error_cmnd;

	for(index = 0; index < REPORT_ERROR_FRAMLENGTH; index++)
	{
		buildFram_str.payload[index] = months_error[index];
	}
	/* build frame for start programming */
	EF_nrf_Build_Fram(& buildFram_str);
	/* send data over nrf module */
	Send_ErrorFram_Status = EF_nrf24l01_writeData(g_FramBuild, MAX_FRAM_SIZE);
	return Send_ErrorFram_Status;

}


U8_t Receive_ErrorFrame(U8_t *bufferin, U8_t *blocks_error, U8_t *blocks_error_number)
{

	U8_t error_status = 0;
	str_received_data recv_data;

	*blocks_error_number = 0;

	/* parse the reciving data */
	EF_parseFrame(bufferin, &recv_data);



	if(recv_data.subcomand == programing_error_cmnd)
	{
		/* first element in the payload will be the number of blocks error   */

		if(recv_data.payload[0] == 0)
		{
			EF_void_UART_SendString("NO Error detected \n");
			error_status = programing_correct;

		}
		else
		{

			error_status = programing_fault;
			*blocks_error_number = recv_data.payload[0];

			EF_void_UART_SendString("ERROR Reporting ");
			EF_void_UART_Send_Integer(*blocks_error_number);
			EF_void_UART_SendString(" months ");


			for(U8_t i=1; i <= recv_data.payload[0]; i++)
			{
				blocks_error[i-1] = recv_data.payload[i];

				EF_void_UART_Send_Integer(recv_data.payload[i]);
				EF_void_UART_SendString(" ,");

			}
			EF_void_UART_SendString("\n");

		}

	}

	return error_status;
}



U8_t Receive_Programing_Mode(U8_t *bufferin, U8_t *months_error)
{

	U8_t endProgram_flag = 0;

	static U8_t DayPrMonth = 0;
	static U8_t months_error_index = 0;
	str_received_data recv_data;

	/*Parse data to get the sub command  */
	EF_parseFrame(bufferin, &recv_data);

	switch(recv_data.subcomand)
	{

	case start_programing_cmnd:
		/* init the index */
		months_error_index = 0;

		EF_void_UART_SendString("Received start programming command\n");
		break;
	case start_block_cmnd:
		EF_void_UART_SendString("received start block for month ");
		EF_void_UART_Send_Integer(recv_data.payload[0]);
		EF_void_UART_SendString("\n");
		DayPrMonth = 0;
		break;
	case programing_block_cmnd:
		DayPrMonth++;
		//TODO save payload into the EEPROM
		break;
	case end_block_cmnd:

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
			/*add block number */
			months_error[months_error_index+1] =  recv_data.payload[0];
			months_error_index++;
			/* */
			months_error[0] = months_error_index;

		}
		break;
	case end_programing_cmnd:

		endProgram_flag = TRUE;

		EF_void_UART_SendString("received end programming mode\n");
		if(months_error_index == 0)
		{
			EF_void_UART_SendString("No error in programming\n");
		}
		else
		{
			EF_void_UART_SendString("ERROR in Block ");
			for(U8_t iter = 1; iter <= months_error_index; iter++)
			{
				EF_void_UART_Send_Integer(months_error[iter]);
				EF_void_UART_SendString("  ");
			}
			EF_void_UART_SendString("\n");
		}
		break;

	default:
		break;

	}
	return endProgram_flag;

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

