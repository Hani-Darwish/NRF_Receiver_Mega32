//
//#include "../Utilities/std_types.h"
//#include "json.h"
//#include <string.h>
//#include <stdlib.h>
//#include "../MCAL/EF_UART.h"
//
//U8_t EF_b_BuildJsonFormat ( NodeReportType * NodeReport_ptr, char * JsonFrame )
//{
//	char TempArray  [10] = {0};
//	U8_t TempNum = 0;
////
////	for (iterator = 0; iterator < strlen(JsonFrame) ; iterator ++ )
////	{
////		JsonFrame [iterator] = 0;
////	}
//
//	if (JsonFrame == NULL )
//	{
//		return FALSE;
//	}
//
//	strcat (JsonFrame , "{\"node\": \"" );
//	strcat (JsonFrame , NodeReport_ptr->NodeName );
//	strcat (JsonFrame , "\"," );
//
//	strcat (JsonFrame , "  \"temperature\":" );
//	itoa( (int)NodeReport_ptr->Temperature,TempArray ,10 );
//	strcat (JsonFrame , TempArray );
//
//	strcat (JsonFrame , ", \"pressure\":" );
//	itoa_Convert ( NodeReport_ptr->AtomPressure, (U8_t*)TempArray ,&TempNum );
//	strcat (JsonFrame , TempArray );
//
//	strcat (JsonFrame , ", \"soil\":" );
//	itoa ( (int)NodeReport_ptr->SoilMoisture,TempArray ,10 );
//	strcat (JsonFrame , TempArray );
//
//	strcat (JsonFrame , ", \"lux\":" );
//	if ((int)NodeReport_ptr->Lux < 0)
//	{
//		NodeReport_ptr->Lux = 0;
//	}
//	itoa_Convert ( NodeReport_ptr->Lux, (U8_t*)TempArray ,&TempNum );
//	strcat (JsonFrame , TempArray );
//
//
//	strcat (JsonFrame , ", \"humidity\":" );
//	itoa ( (int)NodeReport_ptr->Humidity,TempArray ,10 );
//	strcat (JsonFrame , TempArray );
//
//    strcat (JsonFrame , ", \"battery\":" );
//	itoa ( (int)NodeReport_ptr->Battery,TempArray ,10 );
//    strcat (JsonFrame , TempArray );
//
//    strcat (JsonFrame , ", \"yellowstatus\":" );
//	itoa ( (int)NodeReport_ptr->YellowStatus,TempArray ,10 );
//    strcat (JsonFrame , TempArray );
//
//    strcat (JsonFrame , ", \"redstatus\":" );
//	itoa ( (int)NodeReport_ptr->RedStatus,TempArray ,10 );
//    strcat (JsonFrame , TempArray );
//
//    strcat (JsonFrame , ", \"greenstatus\":" );
//	itoa( (int)NodeReport_ptr->GreenStatus,TempArray ,10 );
//    strcat (JsonFrame , TempArray );
//    strcat (JsonFrame , "}" );
//    strcat (JsonFrame , 0 );
//
//	return TRUE;
//}
