///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <sha1.h>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/

#ifndef _SHA_1_H_
#define _SHA_1_H_

#ifdef Debug_message
#include <stdio.h>
#endif
// #include <string.h>
#include "debug.h"
#include "config.h"
#include "typedef.h"

#ifndef HDCP_DEBUG_PRINTF
    #define HDCP_DEBUG_PRINTF(x)
#endif //HDCP_DEBUG_PRINTF

#ifndef HDCP_DEBUG_PRINTF1
    #define HDCP_DEBUG_PRINTF1(x)
#endif //HDCP_DEBUG_PRINTF1

#ifndef HDCP_DEBUG_PRINTF2
    #define HDCP_DEBUG_PRINTF2(x)
#endif //HDCP_DEBUG_PRINTF2


#ifndef DISABLE_HDCP
void SHA_Simple(void *p,WORD len,BYTE *output);
void SHATransform(ULONG * h);
#endif

#endif // _SHA_1_H_
