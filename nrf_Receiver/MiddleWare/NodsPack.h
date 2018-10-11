/*
 * NodsPack.h
 *
 *  Created on: Nov 14, 2017
 *      Author: Embeddedfab Co
 */

#ifndef MIDDLEWARE_NODSPACK_H_
#define MIDDLEWARE_NODSPACK_H_

#include "../ServiceLayer/cwpack.h"
#include "../Utilities/std_types.h"
#include "../ServiceLayer/cwpack.h"




U16_t 	encode_messagepack_data(cw_pack_context *pc, const char * Buffer ,NodeReportType *NodeData);
//int32_t decode_messagepack_data(const char * pMsgPack,NodeConfigParmeter *NodeConfig, int32_t length);
int32_t decode_messagepack_data(const char * pMsgPack, NodeReportType *node_report, int32_t length);


void Processing_NodeData_Parametrs(NodeConfigParmeter *NodeConfig) ;


#endif /* MIDDLEWARE_NODSPACK_H_ */
