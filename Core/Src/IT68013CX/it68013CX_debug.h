///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <debug.h>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_


#define Debug_message 1

#ifndef Debug_message
#define Debug_message 1
#endif


#if Debug_message
    #include <stdio.h>
    #define MxLRX_DEBUG_PRINTF(x)  // printf  x
    #define EQ_DEBUG_PRINTF(x) printf  x/*20230213*/
    #define EQ_PORT0_PRINTF(x) printf  x/*20230213*/
    #define EQ_PORT1_PRINTF(x) printf  x/*20230213*/
    #define VIDEOTIMNG_DEBUG_PRINTF(x) // printf  x
    #define IT6802_DEBUG_INT_PRINTF(x)   printf  x
    #define IT6802_DEBUG_INT_PRINTF1(x)   //printf  x
    #define IT6802_DEBUG_INT_PRINTF2(x)   //printf  x
    #define IT6802_DEBUG_INT_PRINTF3(x)   printf  x
    #define IT6802_MxL_DEBUG_PRINTF(x) //printf x
    #define MxL_MSC_DEBUG_PRINTF(x) //printf x
    #define HDMIRX_VIDEO_PRINTF(x)   printf  x
    #define HDMIRX_AUDIO_PRINTF(x)  //printf  x
    #define HDMIRX_DEBUG_PRINT(x) //printf x
    #define CEC_DEBUG_PRINTF(x) //printf x
    #define EDID_DEBUG_PRINTF(x) //printf  x
    #define IT680X_DEBUG_PRINTF(x) // printf  x
    #define VSDB_DEBUG_PRINTF(x) //printf  x
    #define RCP_DEBUG_PRINTF(x)  //printf  x
    #define MxL3D_DEBUG_PRINTF(x) //printf  x
    #define CsigBus_CAL_PRINTF(x)  //printf  x
    #define MxL_INT_PRINTF(x)  printf  x
    #define RXHDCP_DEBUG_PRINT(x) printf x

    #define HDCP_DEBUG_PRINTF(x) printf x
    #define HDCP_DEBUG_PRINTF1(x) printf x
    #define HDCP_DEBUG_PRINTF2(x) printf x

#else
    #define MxLRX_DEBUG_PRINTF(x)
    #define EQ_DEBUG_PRINTF(x)
    #define EQ_PORT0_PRINTF(x)
    #define EQ_PORT1_PRINTF(x)
    #define VIDEOTIMNG_DEBUG_PRINTF(x)
    #define IT6802_DEBUG_INT_PRINTF(x)
    #define IT6802_DEBUG_INT_PRINTF1(x)
    #define IT6802_DEBUG_INT_PRINTF2(x)
    #define IT6802_DEBUG_INT_PRINTF3(x)
    #define IT6802_MxL_DEBUG_PRINTF(x)
    #define MxL_MSC_DEBUG_PRINTF(x)
    #define HDMIRX_VIDEO_PRINTF(x)
    #define HDMIRX_AUDIO_PRINTF(x)
    #define HDMIRX_DEBUG_PRINT(x)
    #define CEC_DEBUG_PRINTF(x)
    #define EDID_DEBUG_PRINTF(x)
    #define IT680X_DEBUG_PRINTF(x)
    #define VSDB_DEBUG_PRINTF(x)
    #define RCP_DEBUG_PRINTF(x)
    #define MxL3D_DEBUG_PRINTF(x)
    #define CsigBus_CAL_PRINTF(x)
    #define MxL_INT_PRINTF(x)
    #define RXHDCP_DEBUG_PRINT(x)
#endif




#endif
