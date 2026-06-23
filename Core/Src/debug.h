///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <debug.h>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_
#define Debug_message 1

// #pragma message("debug.h")
extern 	int _write(int file, char* p, int len);

#ifndef Debug_message
#define Debug_message 1
#endif

#if Debug_message

    #define HDMITX_DEBUG_PRINTF(x)  printf x
    #define HDCP_DEBUG_PRINTF(x)  printf x
    #define EDID_DEBUG_PRINTF(x)  printf x
    #define HDMITX_DEBUG_INFO(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF(x)
    #define HDCP_DEBUG_PRINTF(x)
    #define EDID_DEBUG_PRINTF(x)
    #define HDMITX_DEBUG_INFO(x)
#endif


#if( Debug_message & (1<<1))
    #define HDMITX_DEBUG_PRINTF1(x) printf x
    #define HDCP_DEBUG_PRINTF1(x) printf x
    #define EDID_DEBUG_PRINTF1(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF1(x)
    #define HDCP_DEBUG_PRINTF1(x)
    #define EDID_DEBUG_PRINTF1(x)
#endif

#if( Debug_message & (1<<2))
    #define HDMITX_DEBUG_PRINTF2(x) printf x
    #define HDCP_DEBUG_PRINTF2(x) printf x
    #define EDID_DEBUG_PRINTF2(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF2(x)
    #define HDCP_DEBUG_PRINTF2(x)
    #define EDID_DEBUG_PRINTF2(x)
#endif

#if( Debug_message & (1<<3))
    #define HDMITX_DEBUG_PRINTF3(x) printf x
    #define HDCP_DEBUG_PRINTF3(x) printf x
    #define EDID_DEBUG_PRINTF3(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF3(x)
    #define HDCP_DEBUG_PRINTF3(x)
    #define EDID_DEBUG_PRINTF3(x)
#endif

#if( Debug_message & (1<<4))
    #define HDMITX_DEBUG_PRINTF4(x) printf x
    #define HDCP_DEBUG_PRINTF4(x) printf x
    #define EDID_DEBUG_PRINTF4(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF4(x)
    #define HDCP_DEBUG_PRINTF4(x)
    #define EDID_DEBUG_PRINTF4(x)
#endif

#if( Debug_message & (1<<5))
    #define HDMITX_DEBUG_PRINTF5(x) printf x
    #define HDCP_DEBUG_PRINTF5(x) printf x
    #define EDID_DEBUG_PRINTF5(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF5(x)
    #define HDCP_DEBUG_PRINTF5(x)
    #define EDID_DEBUG_PRINTF5(x)
#endif

#if( Debug_message & (1<<6))
    #define HDMITX_DEBUG_PRINTF6(x) printf x
    #define HDCP_DEBUG_PRINTF6(x) printf x
    #define EDID_DEBUG_PRINTF6(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF6(x)
    #define HDCP_DEBUG_PRINTF6(x)
    #define EDID_DEBUG_PRINTF6(x)
#endif

#if( Debug_message & (1<<7))
    #define HDMITX_DEBUG_PRINTF7(x) printf x
    #define HDCP_DEBUG_PRINTF7(x) printf x
    #define EDID_DEBUG_PRINTF7(x) printf x
#else
    #define HDMITX_DEBUG_PRINTF7(x)
    #define HDCP_DEBUG_PRINTF7(x)
    #define EDID_DEBUG_PRINTF7(x)
#endif


#endif//  _DEBUG_H_
