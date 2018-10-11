/*
 * NodsPack.c
 *
 *  Created on: Nov 14, 2017
 *      Author: Embeddedfab Co
 */

#include "NodsPack.h"
#include "../MCAL/EF_UART.h"
#include "util/delay.h"
#include <string.h>

#define NODE_DEBUG

#define Node_1 "abeta"
#define Node_2 "bbeta"
#define Node_3 "cbeta"
#define Node_4 "dbeta"
#define Node_5 "ebeta"
#define Node_6 "fbeta"
#define Node_7 "gbeta"
#define Node_8 "hbeta"
#define Node_9 "ibeta"
#define Node_10 "jbeta"
NodeReportType Node1 ;

 U16_t encode_messagepack_data(cw_pack_context *pc, const char * Buffer ,NodeReportType *NodeData)
{
	U16_t length = 0;
	cw_pack_context_init(pc,Buffer, 60, 0);
	cw_pack_map_size (pc, 2);
	cw_pack_str (pc, "nd", 2);
	cw_pack_str (pc, Node_2 ,5);
	cw_pack_str (pc, "Rep", 3);
	cw_pack_array_size(pc,9);
	cw_pack_unsigned(pc,(uint8_t) NodeData->Temperature);      //array component
	cw_pack_unsigned(pc,(uint16_t)NodeData->AtomPressure);     //array component
	cw_pack_unsigned(pc,(uint8_t) NodeData->SoilMoisture);     //array component
	cw_pack_unsigned(pc,(uint16_t)NodeData->Lux);              //array component
	cw_pack_unsigned(pc,(uint8_t) NodeData->Humidity);         //array component
	cw_pack_unsigned(pc,(uint8_t) NodeData->Battery);          //array component
//	cw_pack_unsigned(pc,(uint8_t) NodeData->YellowStatus);     //array component
//	cw_pack_unsigned(pc,(uint8_t) NodeData->RedStatus);        //array component
//	cw_pack_unsigned(pc,(uint8_t) NodeData->GreenStatus);      //array component
	length = pc->current - pc->start;
	return length;

}







 /*******************************************************************************
  ****Decode message received from node contains sensors reading and node name***
  *******************************************************************************/
// static int32_t decode_messagepack_data(const char * pMsgPack, int32_t length, uint8_t * node_name, uint8_t * node_name_len)
 int32_t decode_messagepack_data(const char * pMsgPack, NodeReportType *node_report, int32_t length)
 {
     char tmp[20];
     uint64_t value;
     int i, f;

     cw_unpack_context uc;
     cw_unpack_context_init(&uc, (void *)pMsgPack, length, 0);

//     UARTprintf("MessagePack: number of bytes %d\n", length);

     /* map */
     cw_unpack_next(&uc);
     if (uc.item.type != CWP_ITEM_MAP)
    	 EF_void_UART_SendString("- ERROR (MAP TYPE)\n");
     /* node */
     cw_unpack_next(&uc);
     if (uc.item.type != CWP_ITEM_STR || uc.item.as.str.length != 2){
    	 EF_void_UART_SendString("- ERROR (STR SIZE)\n");

     }else{
         memset(tmp, 0 , 10);
         memcpy(tmp, uc.item.as.str.start, uc.item.as.str.length);
//         UARTprintf("- %s: ", tmp);
     }
     /* node value */
     cw_unpack_next(&uc);
     if (uc.item.type != CWP_ITEM_STR){
    	 EF_void_UART_SendString("- ERROR (STR TYPE)\n");

     }else{
         memset(tmp, 0 , 10);
         memcpy(node_report->NodeName, uc.item.as.str.start, uc.item.as.str.length);
         memcpy(tmp, uc.item.as.str.start, uc.item.as.str.length);

//         memcpy(node_name, uc.item.as.str.start, uc.item.as.str.length);
//         *node_name_len = uc.item.as.str.length;

//         UARTprintf("%s\n", tmp);
     }
     /* nodeReport or any other object*/
     cw_unpack_next(&uc);
     if (uc.item.type != CWP_ITEM_STR){
    	 EF_void_UART_SendString("- ERROR (STR TYPE)\n");

     }else{

         if ((strncmp("R", uc.item.as.str.start, 1) == 0)){
//        	 EF_void_UART_SendString("- STR TRUE n");
             /* nodeReport value */
             cw_unpack_next(&uc);
             if (uc.item.type != CWP_ITEM_ARRAY){
//                 UARTprintf("- ERROR (ARRAY TYPE)\n");
             }else{
                 f = uc.item.as.array.size;
                 for(i = 0; i < f; i++){
                     cw_unpack_next(&uc);
                     if (uc.item.type == CWP_ITEM_POSITIVE_INTEGER){
                         value = uc.item.as.u64;
                         sprintf(tmp, "%llu", value);
//                         UARTprintf("- %s:\n", tmp);
                         switch(i){
                         case 0:
                        	 node_report->Temperature = uc.item.as.u64;
                        	 EF_void_UART_SendString("\n Temperature = ");
                        	 EF_void_UART_Send_Integer(node_report->Temperature);
                             break;
                         case 1:
                             node_report->AtomPressure = uc.item.as.u64;
                        	 EF_void_UART_SendString("\n AtomPressure = ");
                        	 EF_void_UART_Send_Integer(node_report->AtomPressure);
                             break;
                         case 2:
                             node_report->SoilMoisture = uc.item.as.u64;
                        	 EF_void_UART_SendString("\n SoilMoisture = ");
                        	 EF_void_UART_Send_Integer(node_report->SoilMoisture);
                             break;
                         case 3:
                             node_report->Lux = uc.item.as.u64;
                        	 EF_void_UART_SendString("\n Lux = ");
                        	 EF_void_UART_Send_Integer(node_report->Lux);
                             break;
                         case 4:
                             node_report->Humidity = uc.item.as.u64;
                        	 EF_void_UART_SendString("\n Humidity = ");
                        	 EF_void_UART_Send_Integer(node_report->Humidity);
                             break;
                         case 5:
                        	 node_report->Battery = uc.item.as.u64;
                        	 EF_void_UART_SendString("\n Battery = ");
                        	 EF_void_UART_Send_Integer(node_report->Battery);
                             break;
                         case 6:
                             node_report->YellowStatus = uc.item.as.u64;
                             break;
                         case 7:
                             node_report->RedStatus = uc.item.as.u64;
                             break;
                         case 8:
                             node_report->GreenStatus = uc.item.as.u64;
                             break;
                         default:
                             break;
                         }
                     }
                 } /*END looping through array elements*/
             }
         }
     }
     return 0;
 }












//
//  int32_t decode_messagepack_data(const char * pMsgPack, NodeReportType *node_report, int32_t length)
// {
// 	uint8_t  index, ArraySize;
//
// 	cw_unpack_context uc;
// 	cw_unpack_context_init(&uc, (void *)pMsgPack, length, 0);
// 	/* map */
// 	cw_unpack_next(&uc);
// 	if (uc.item.type != CWP_ITEM_MAP)
// 	{
//#ifdef NODE_DEBUG
// 		EF_void_UART_SendString("- ERROR (MAP TYPE)\n");
//#endif
// 	}
// 	else
// 	{
// 		/* config */
// 		cw_unpack_next(&uc);
// 		if((uc.item.type != CWP_ITEM_STR))
// 		{
//#ifdef NODE_DEBUG
// 			EF_void_UART_SendString("- ERROR (STR TYPE)\n");
//#endif
// 		}
// 		else
// 		{
// 			if (uc.item.type != CWP_ITEM_STR || uc.item.as.str.length != 2)
// 			{
//#ifdef NODE_DEBUG
// 				EF_void_UART_SendString("- ERROR (STR SIZE)\n");
//#endif
// 			}
// 			else
// 			{
//
// 				if (!strncmp("nd", uc.item.as.str.start, 2))
// 				{
// 					cw_unpack_next(&uc);
// 					if (uc.item.type != CWP_ITEM_STR)
// 					{
//#ifdef NODE_DEBUG
// 						EF_void_UART_SendString("- ERROR (STRING TYPE)\n");
//#endif
// 					}
// 					else
// 					{
// 						cw_unpack_next(&uc);
//
//// 						EF_void_UART_SendString(" next string : ");
//// 						EF_void_UART_SendString(uc.item.as.str.start);
//
// 						if (strncmp("Rep", uc.item.as.str.start, 3) == 1)
// 						{
// 							EF_void_UART_SendString("STUCK HERE");
// 						}
//
// 						else
// 						{
// 							cw_unpack_next(&uc);
//
// 	 						ArraySize = uc.item.as.array.size;
//
// 	 						EF_void_UART_SendString("array size = ");
// 	 						EF_void_UART_Send_Integer(uc.item.as.array.size);
// 	 						EF_void_UART_SendString(" \n ");
//
//
// 	 						for(index = 0; index < ArraySize; index++)
// 	 						{
// 	 							cw_unpack_next(&uc);
// 	 							if (uc.item.type == CWP_ITEM_POSITIVE_INTEGER)
// 	 							{
// 	 								switch(index)
// 	 								{
// 	 								case 0:
// 	 									node_report->Temperature = uc.item.as.u64;
// 	 									EF_void_UART_SendString("\n Temperture = ");
// 	 									EF_void_UART_Send_Integer(node_report->Temperature);
// 	 									break;
// 	 								case 1:
//
// 	 									node_report->AtomPressure = uc.item.as.u64;
// 	 									EF_void_UART_SendString("\n Atmospher = ");
// 	 									EF_void_UART_Send_Integer(node_report->AtomPressure);
// 	 									break;
// 	 								case 2:
// 	 									node_report->SoilMoisture = uc.item.as.u64;
// 	 									EF_void_UART_SendString("\n Soil = ");
// 	 									EF_void_UART_Send_Integer(node_report->SoilMoisture);
// 	 									break;
// 	 								case 3:
//
// 	 									node_report->Lux = uc.item.as.u64;
// 	 									EF_void_UART_SendString("\n LUX = ");
// 	 									EF_void_UART_Send_Integer(node_report->Lux);
// 	 									break;
// 	 								case 4:
// 	 									node_report->Humidity = uc.item.as.u64;
// 	 									EF_void_UART_SendString("\n humidity = ");
// 	 									EF_void_UART_Send_Integer(node_report->Humidity);
// 	 									break;
// 	 								case 5:
// 	 									node_report->Battery = uc.item.as.u64;
// 	 									EF_void_UART_SendString("\n Battery = ");
// 	 									EF_void_UART_Send_Integer(node_report->Battery);
// 	 									_delay_ms(1);
// 	 									break;
// 	 								default:
// 	 									break;
// 	 								}
// 	 							}
// 	 						} /*END looping through array elements*/
//
// 						}
// 					}
//
// 				}
// 				else {
//// 						EF_void_UART_SendString("- config error data : \n");
// 				     }
// 			}
//
// 		}
//
// 	}
// 	return 0;
// }
