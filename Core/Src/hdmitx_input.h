///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <hdmitx_input.h>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/
#ifndef _HDMITX_DEBUG_H_
#define _HDMITX_DEBUG_H_

#ifdef HDMITX_INPUT_INFO
LONG CalcPCLK();
LONG CalcAudFS();
LONG CalcRCLK();
BYTE hdmitx_getAudioCount() ;

USHORT hdmitx_getInputHTotal();
USHORT hdmitx_getInputVTotal();
BOOL hdmitx_isInputInterlace();
BYTE IsPCLKStable();//20221213
#endif

#endif
