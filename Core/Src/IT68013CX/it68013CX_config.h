///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <config.h>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define _MCU_8051_

#ifndef _MCU_8051_
#include "platform.h"
#endif

#ifdef _IT66021_
    #if defined(_IT6802_)
        #undef _IT6802_
    #endif

    #if defined(_IT6803_)
        #undef _IT6803_
    #endif

    #if !defined(_IT6801_)
        #define _IT6801_
    #endif
#endif

#ifdef _IT6602_
    #if defined(_IT6801_)
        #undef _IT6801_
    #endif

    #if !defined(_IT6802_)
        #define _IT6802_
    #endif

    #if defined(_IT6803_)
        #undef _IT6803_
    #endif
#endif

#if (!defined(_IT6801_))&&(!defined(_IT6802_))&&(!defined(_IT6803_))
    #define _IT6802_
#endif

#ifdef _IT6803_
    #pragma message("defined _IT6803_")
    //#define ENABLE_IT6803    // only for IT6803 Usage
#endif


#ifdef _IT6802_
#pragma message("defined _IT6802_")
#endif

#ifdef _IT6801_
#pragma message("defined _IT6801_")
#endif

//#define SUPPORT_I2C_SLAVE
#ifdef SUPPORT_I2C_SLAVE
#pragma message ("SUPPORT_I2C_SLAVE defined")
#endif

#define _EN_DUAL_PIXEL_CTRL_
#define _EN_BLOCK_PWRDN_
#define SUPPORT_OUTPUTRGB
#define SUPPORT_INPUTYUV

//#define _ENABLE_IT68XX_MxL_FUNCTION_
#define _SUPPORT_EDID_RAM_    TRUE

#define _SUPPORT_AUTO_EQ_    TRUE
#define _SUPPORT_EQ_ADJUST_    TRUE

//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
//#define EnableCalFs
//FIX_ID_023 xxxxx

//FIX_ID_028 xxxxx //For Debug Audio error with S2
//#define _FIX_ID_028_ // fast audio switch instead of checking audio stable
//FIX_ID_028 xxxxx

//FIX_ID_026 xxxxx        //Support RB SWAP for TTL output
//#define _SUPPORT_RBSWAP_
//FIX_ID_026 xxxxx

#define OUTPUT_COLOR_DEPTH OUT8B
// #define OUTPUT_COLOR_DEPTH OUT10B
// #define OUTPUT_COLOR_DEPTH OUT12B


//#define _FORCE_DISABLE_HDCP_
#endif // _CONFIG_H_
