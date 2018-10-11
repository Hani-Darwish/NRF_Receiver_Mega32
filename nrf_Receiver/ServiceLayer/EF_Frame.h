/************************************************************
 * this mdule used to build the frame
 * to be sent over rf module
 *
 ************************************************************/
#include "../Utilities/std_types.h"


#ifndef EF_FRAME_H_
#define EF_FRAME_H_

#define MAX_FRAM_SIZE            32
#define MAX_PAYLOAD_LENGTH       20
#define CRC_DATA_LENGTH          28
#define CRC_LENGTH               2  /* two bytes CRC*/


/* frame index  */
#define START_FRAM_INDEX         0
#define SOURCE_ID_INDEX          1
#define DESTINATION_ID_INDEX     2
#define PACKET_ID_LOW_INDEX      3
#define PACKET_ID_HIGH_INDEX     4
#define MASSAGE_TYPE_INDEX       5
#define GROUP_MASGE_INDEX        6

#define SUB_COMND_INDEX          7

#define PALOAD_LENGTH_INDEX      8     /* the length of the actual data (payload) */
#define PAYLOAD_INDEX            9
#define CRC_INDEX                29
#define END_FRAM_INDEX           31
/*frame content  */
#define START_FRAM         		 0xEF
#define SOURCE_ID           	 0xAA
#define DESTINATION_ID      	 0xC2
#define END_FRAM           		 0xFE
/* massage type */
#define CONFIG_MASSAGE        	 0
#define SENSOR_READ_MASSAGE    	 1
#define ACTION_MASSAGE         	 2
#define ERROR_MASSAGE          	 3
#define EMERGNY_MASSAGE        	 4
/* group and configure */
#define NODE_TYPE              	 0
#define GATE_WAY_TYPE		   	 1
#define REPEATER_TYPE		   	 2
/* frames type */
#define SNGLE_FRAM               3
#define BULK_DATA_FRAM           4
#define CHUNK_DATA_FRAM          5
#define END_BULK_DATA_FRAM       6
/* grouping byte map */
#define SOURCE_LOW_BIT           0
#define SOURCE_HIGH_BIT          1
#define FRAME_TYPE_LOW_BIT       2
#define FRAME_TYPE_HIGH_BIT      3
/*   */



typedef enum
{
	config_massage,
	sensor_read_massage,
	action_massage,
	error_massage,
	emergency_massage,
	reserved,
}enu_massage_type;

typedef enum
{
	node_type,
	gatway_type,
	repeater,
}enu_source_type;


typedef enum
{
	start_programing_cmnd,
	end_programing_cmnd,
	start_block_cmnd,
	programing_block_cmnd,
	end_block_cmnd
}enu_subcomand;


///* massage type */
//typedef union
//{
//	struct
//	{
//		U8_t config_massage  		:1;
//		U8_t sensor_read_massage    :1;
//		U8_t action_massage  		:1;
//		U8_t error_massage   		:1;
//		U8_t reserved        		:4;
//	}massage_type_str;
//	U8_t massage_type;
//}uni_massage_type;
//


/* enum for parser return */
typedef enum
{
	massage_received_ok,
	massage_crc_error,
	massage_start_error,
	massage_stop_error,
	massage_wrong_frame
}enu_parser_return;

/* received data structure  */
typedef struct
{
	U8_t start_frame;
	U8_t souce_id;
	U8_t destination_id;
	U16_t packet_id;
	enu_massage_type massage_type;
	enu_source_type source_type;
	enu_subcomand subcomand;

	U8_t frame_type;
	U8_t payload_length;
	U8_t payload[20];
	U16_t crc_check;
	U8_t end_frame;
}str_received_data;


/* build frame stucture  */
typedef struct
{
	U8_t source_ID;
	U8_t destination_ID;
	U8_t frame_type;
	enu_massage_type massage_type;
	enu_subcomand subcomand;
	U8_t Payload_length;
	U8_t payload[20];
}gstr_buildFram;


/**********************************************************************************************************
 * Function    : void EF_nrf_Build_Fram(U8_t source_ID, U8_t destination_ID,
										U8_t payload_length, U8_t * payload);
 *
 * DESCRIPTION : Build the EF frame to be sent over nRF24l01
 *
 * PARAMETERS  : U8_t source_ID,      unique id for transmitter.
 *  			 U8_t destination_ID, unique id for receiver.
 *  			 U8_t payload_length, the data length.
 *  			 U8_t * payload,      pointer to useful data.
 *
 * Return Value: None.
 *
 *******************************************************************************************************/
void EF_nrf_Build_Fram(gstr_buildFram* buildFram_str);


/**********************************************************************************************************
 * Function    : void CalculateCrc(U8_t data_length, U8_t *data, U8_t *crc);
 *
 * DESCRIPTION : calculate the CRC
 *
 * PARAMETERS  : U8_t data_length,  the length of data to calculate the CRC.
 * 				 U8_t *data,		pointer to the data.
 * 				 U8_t *crc   		pointer to the return crc result.
 *
 * Return Value: None.
 *
 *******************************************************************************************************/
void EF_CalculateCrc(U8_t data_length, U8_t *data, U8_t *crc);


/**********************************************************************************************************
 * Function    : U8_t EF_set_massageType(enu_massage_type massage_type);
 *
 * DESCRIPTION : this function used to set the massage type to the frame.
 *
 * PARAMETERS  : enu_massage_type massage_type : enum to massage type could be
 * 				 config,reading,action,error or emergency;
 *
 *
 * Return Value: U8_t the value that will set to the frame.
 *
 *******************************************************************************************************/
U8_t EF_set_massageType(U8_t massage_type);

/**********************************************************************************************************
 * Function    : U8_t EF_set_groupConfig(U8_t source_type, U8_t frame_type);
 *
 * DESCRIPTION : this function used to set soucrec type (Node,gateway or repeater).
 * 				and frame type(single,bulk,chunk or end frame )
 *
 * PARAMETERS  : U8_t source_type
 * 				 U8_t frame_type
 *
 * Return Value: U8_t the value that will set to the frame.
 *******************************************************************************************************/
U8_t EF_set_groupConfig(U8_t source_type, U8_t frame_type);


/* get massage type */
U8_t EF_get_MassageType(U8_t received_massage);

U8_t EF_get_SourceType(U8_t received_group);

U8_t EF_parseFrame(U8_t *const received_data, str_received_data *gstr_received_data);






#endif /* TEST_H_ */

