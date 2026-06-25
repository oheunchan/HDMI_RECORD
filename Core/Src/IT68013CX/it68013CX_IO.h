///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.h>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/

#ifndef _IO_h_
#define _IO_h_
#include "it68013CX_typedef.h"

#define i2c_write_byte it68013CX_i2c_write_byte //yjh2026
#define i2c_read_byte it68013CX_i2c_read_byte   //yjh2026

#ifdef SUPPORT_UART_CMD
void UartCommand();
#endif


//BYTE HDMI_IIC_Read( BYTE RegAddr);
//BOOL HDMI_IIC_Write( BYTE RegAddr,BYTE DataIn);
//BYTE HDMI_IIC_SET( BYTE offset, BYTE mask, BYTE datain );


BOOL i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );
BOOL i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );

#ifdef Enable_IT6802_CEC
#include "debug.h"
BYTE IT6802_CEC_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS IT6802_CEC_WriteI2C_Byte(BYTE offset,BYTE buffer );
#endif


//#define hdmirxrd(x) HDMI_IIC_Read(x)
//#define hdmirxwr(x,y) HDMI_IIC_Write(x,y)



void SetEDIDWp(BOOL ENA);


void HotPlug(BYTE Enable);

void init_printf(void);

void SetintActive(BOOL bactive);


#endif
