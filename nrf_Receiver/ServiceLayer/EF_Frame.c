#include "string.h"
#include "../ServiceLayer/EF_Frame.h"
#include "../MCAL/EF_UART.h"


str_received_data gstr_received_data;
/* global frame buffer */
U8_t g_FramBuild[MAX_FRAM_SIZE];
/* counter for build frame occurrence */
U16_t g_Packet_ID;

/* build the frame */
//void EF_nrf_Build_Fram(U8_t source_ID, U8_t destination_ID, U8_t payload_length, U8_t * payload)
void EF_nrf_Build_Fram(gstr_buildFram* buildFram_str)
{

	U8_t  crc_cheksum[2]  = {0};
	U8_t  crc_data_length = CRC_DATA_LENGTH;
	U8_t  iterator = 0;

	memset(g_FramBuild,0,MAX_FRAM_SIZE);

	/* increment packet id */
	++g_Packet_ID;
	/* add start byte  */
	g_FramBuild[START_FRAM_INDEX]      = START_FRAM;
	g_FramBuild[SOURCE_ID_INDEX]       = buildFram_str->source_ID;
	g_FramBuild[DESTINATION_ID_INDEX]  = buildFram_str->destination_ID;
	/* add packet id (sequence number)*/
	g_FramBuild[PACKET_ID_LOW_INDEX]   = (U8_t)(g_Packet_ID);
	g_FramBuild[PACKET_ID_HIGH_INDEX]  = (U8_t)(g_Packet_ID >> 8);
	/* add massage type to the frame */
	g_FramBuild[MASSAGE_TYPE_INDEX]    = EF_set_massageType(buildFram_str->massage_type);
	/* add grouping byte to the frame */
	g_FramBuild[GROUP_MASGE_INDEX]     = EF_set_groupConfig(GATE_WAY_TYPE , buildFram_str->frame_type);
	/* add sub comand to the frame */
	g_FramBuild[SUB_COMND_INDEX]       = buildFram_str->subcomand;
	/* add the length of data */
	g_FramBuild[PALOAD_LENGTH_INDEX]   = buildFram_str->Payload_length;
	/* add the payload data to the frame */
	for (iterator = 0; iterator < buildFram_str->Payload_length; iterator++)
	{
		g_FramBuild[PAYLOAD_INDEX + iterator]  = *(buildFram_str->payload + iterator);
	}

	/* calculate CRC from [1 to the end of frame] */
	EF_CalculateCrc(crc_data_length, g_FramBuild, crc_cheksum);
	/* add the 2 byte crc to the frame */
	for(iterator = 0; iterator<2; iterator++)
	{
		g_FramBuild[CRC_INDEX + iterator] = crc_cheksum[iterator];
	}
	/* add the end of frame */
	g_FramBuild[END_FRAM_INDEX] = END_FRAM;
	/* handle overflow of packed id */
	if (g_Packet_ID == 65535)
	{
		g_Packet_ID = 0;
	}

	//	EF_void_UART_SendString("CRC = ");
	//	EF_void_UART_SendArray(crc_cheksum,2);
	//	EF_void_UART_SendString("  \n  ");

//	EF_void_UART_SendString("frame ");
//	EF_void_UART_SendArray(g_FramBuild,32);
//	EF_void_UART_SendString(" \n ");
}


/**************************************************************
 	 	 	 	 	 CRC calculate
 ************************************************************/
void EF_CalculateCrc(U8_t data_length, U8_t *data, U8_t *crc)
{
	U8_t counter;
	U8_t crcLow = 0, crcHigh = 0, crcCarry;
	U8_t polyLow = 0x05, polyHigh = 0x80;
	U8_t shiftRegister;
	U8_t dataBit, crcBit;
	for (counter = 0; counter < data_length; counter++)
	{
		for (shiftRegister = 0x80; shiftRegister > 0x00; shiftRegister >>= 1)
		{
			dataBit = (data[counter] & shiftRegister) ? 1 : 0;
			crcBit = crcHigh >> 7;
			// Shift CRC to the left by 1.
			crcCarry = crcLow >> 7;
			crcLow  <<= 1;
			crcHigh <<= 1;
			crcHigh |= crcCarry;
			if ((dataBit ^ crcBit) != 0)
			{
				crcLow ^= polyLow;
				crcHigh ^= polyHigh;
			}
		}
	}
	crc[0] = crcHigh;
	crc[1] = crcLow;
}

/* set massage type  */
U8_t EF_set_massageType(U8_t massage_type)
{
	U8_t ret_massage_type = 0;
	switch(massage_type)
	{
	case config_massage:
		ret_massage_type = CONFIG_MASSAGE;
		break;
	case sensor_read_massage:
		ret_massage_type = SENSOR_READ_MASSAGE;
		break;
	case action_massage:
		ret_massage_type = ACTION_MASSAGE;
		break;
	case error_massage:
	ret_massage_type = ERROR_MASSAGE;
		break;
	case emergency_massage:
		ret_massage_type = EMERGNY_MASSAGE;
		break;
	default:
		ret_massage_type = SENSOR_READ_MASSAGE;
		break;
	}
return ret_massage_type;
}


/* set grouping and configuration */
U8_t EF_set_groupConfig(U8_t source_type, U8_t frame_type)
{
	U8_t ret_group_value = 0;

	/* handle source type */
	if (source_type == NODE_TYPE)
	{
		ret_group_value  |= (0 << SOURCE_LOW_BIT ) | (0 << SOURCE_HIGH_BIT);
	}
	else if(source_type == GATE_WAY_TYPE)
	{
		ret_group_value  |= (1 << SOURCE_LOW_BIT) | ( 0 << SOURCE_HIGH_BIT );
	}
	else if(source_type == REPEATER_TYPE)
	{
		ret_group_value  |= (1 << SOURCE_LOW_BIT) | (1 << SOURCE_HIGH_BIT);
	}

	/* handle frame type */
	if(frame_type == SNGLE_FRAM)
	{
		ret_group_value |= (0 << FRAME_TYPE_LOW_BIT) | (0 << FRAME_TYPE_HIGH_BIT);
	}
	else if(frame_type == BULK_DATA_FRAM)
	{
		ret_group_value |= (1 << FRAME_TYPE_LOW_BIT) | (0 << FRAME_TYPE_HIGH_BIT);
	}
	else if(frame_type == CHUNK_DATA_FRAM)
	{
		ret_group_value |= (0 << FRAME_TYPE_LOW_BIT) | (1 << FRAME_TYPE_HIGH_BIT);
	}
	else if(frame_type == END_BULK_DATA_FRAM)
	{
		ret_group_value |= (1 << FRAME_TYPE_LOW_BIT) | (1 << FRAME_TYPE_HIGH_BIT);
	}

	return ret_group_value;
}

/* parse the receiving frame */
enu_parser_return EF_parseFrame(U8_t *const received_data, str_received_data *gstr_received_data)
{
	U8_t  iterator = 0;
	U8_t  crc_calculated[CRC_LENGTH]  = {0};
	U8_t  received_crc[CRC_LENGTH] = {0};
	U8_t  crc_data_length = CRC_DATA_LENGTH;
	U16_t packet_id = 0;
	enu_parser_return parser_return;


	/*check start and the end of the frame */
	if(received_data[START_FRAM_INDEX] != START_FRAM )
	{
		parser_return = massage_start_error;

	}
	else
	{

		if(received_data[END_FRAM_INDEX] != END_FRAM)
		{
			parser_return = massage_stop_error;
		}
		else
		{
			/* the received CRC */
			received_crc[0] = received_data[CRC_INDEX];
			received_crc[1] = received_data[CRC_INDEX + 1];
			/* calculate CRC from [1 to the end of frame] */
			EF_CalculateCrc(crc_data_length, received_data, crc_calculated);
			/* compare the received CRC with value calculated after received */
			if((memcmp(received_crc, crc_calculated, CRC_LENGTH)))
			{
				parser_return = massage_crc_error;
			}
			else
			{

				/* handle 2 byte packet id */
				packet_id = ((received_data[PACKET_ID_HIGH_INDEX]<<8) + (received_data[PACKET_ID_LOW_INDEX] ));
				/* handle payload */

//				EF_void_UART_SendString("\npayload=");
				for (iterator = 0; iterator < MAX_PAYLOAD_LENGTH ; iterator++)
				{
					(gstr_received_data->payload[iterator])  = received_data[PAYLOAD_INDEX + iterator];
//					EF_void_UART_PutChar(gstr_received_data->payload[iterator]);

				}
				gstr_received_data->subcomand      = received_data[SUB_COMND_INDEX];
				gstr_received_data->start_frame    = received_data[START_FRAM_INDEX];
				gstr_received_data->souce_id       = received_data[SOURCE_ID_INDEX];
				gstr_received_data->destination_id = received_data[DESTINATION_ID_INDEX];
				gstr_received_data->packet_id      = packet_id;


				gstr_received_data->massage_type   = EF_get_MassageType(received_data[MASSAGE_TYPE_INDEX]);
				gstr_received_data->source_type    = EF_get_SourceType(received_data[GROUP_MASGE_INDEX]);


			//	gstr_received_data->frame_type     =
				gstr_received_data->payload_length = received_data[PALOAD_LENGTH_INDEX];
			//	gstr_received_data->payload        =
				gstr_received_data->crc_check      = received_data[CRC_INDEX];
				gstr_received_data->end_frame      = received_data[END_FRAM_INDEX];


			}


		}

	}




//	EF_void_UART_SendString(" start = ");
//	EF_void_UART_PutChar(gstr_received_data->start_frame);
//	EF_void_UART_SendString(" sourec = ");
//	EF_void_UART_PutChar(gstr_received_data->souce_id );
//	EF_void_UART_SendString(" destination = ");
//	EF_void_UART_PutChar(gstr_received_data->destination_id);
//	EF_void_UART_SendString(" length = ");
//	EF_void_UART_PutChar(gstr_received_data->payload_length);
//	EF_void_UART_SendString(" end = ");
//	EF_void_UART_PutChar(gstr_received_data->end_frame);

//	EF_void_UART_SendString("packet id = ");
//	EF_void_UART_Send_Integer(gstr_received_data->packet_id);
//	EF_void_UART_SendString("        ");

//	EF_void_UART_SendString("massaage =  ");
//	EF_void_UART_Send_Integer(gstr_received_data->massage_type);
//	EF_void_UART_SendString("Packet");
//	EF_void_UART_Send_Integer(gstr_received_data->packet_id);
//	EF_void_UART_SendString(" Data  ");
//	EF_void_UART_Send_IntgerArray(gstr_received_data->payload,20);
//	EF_void_UART_SendString(" \n ");

//	EF_void_UART_SendString(" calculated = ");
//	EF_void_UART_SendArray(crc_calculated,2);
//	EF_void_UART_SendString(" \n ");


return parser_return;
}


/* get massage type */
U8_t EF_get_MassageType(U8_t received_massage)
{
	enu_massage_type massage_type_ret = reserved;

	switch(received_massage)
	{
	case CONFIG_MASSAGE:
		massage_type_ret = config_massage;
		break;
	case SENSOR_READ_MASSAGE:
		massage_type_ret = sensor_read_massage;
		break;
	case ACTION_MASSAGE:
		massage_type_ret = action_massage;
		break;
	case ERROR_MASSAGE:
		massage_type_ret = error_massage;
		break;
	case EMERGNY_MASSAGE:
		massage_type_ret = emergency_massage;
		break;
	default:
		break;
	}

return massage_type_ret;
}

/* get source type */

enu_source_type EF_get_SourceType(U8_t received_group)
{

	/* TODO test this function  */
	enu_source_type source_type_ret = 0;

	if(  (received_group & (1<<SOURCE_HIGH_BIT))     )
	{
		EF_void_UART_SendString(" source_repeater ");
//		source_type_ret = repeater;
	}

	if(((received_group & (1 << SOURCE_LOW_BIT)) == 0) && (received_group & (1 << SOURCE_HIGH_BIT)) == 0)
	{
		source_type_ret = node_type;
//		EF_void_UART_SendString("source node ");
	}
	if(((received_group & (1 << SOURCE_LOW_BIT)) == 1) && (received_group & (1 << SOURCE_HIGH_BIT)) == 0)
	{
		source_type_ret = gatway_type;
//		EF_void_UART_SendString("source gateway ");
	}
//	if(((received_group & (1 << SOURCE_LOW_BIT)) == 1) && ((received_group & (1 << SOURCE_HIGH_BIT)) == 1))

	return source_type_ret;
}

