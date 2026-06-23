///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <66122_6350.h>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/
#ifndef _66122_6350_H
#define _66122_6350_H
//#include "../IO_IT6350.h"		//oec20240608
#include "typedef.h"
#include "stdio.h"




extern void HAL_Delay(unsigned long Delay);
//extern HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
//extern HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress,uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);                                    
//extern HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, u16 DevAddress, u16 MemAddress,u16 MemAddSize, u8 *pData, u16 Size, u32 Timeout);
//extern HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, u16 DevAddress, u16 MemAddress,u16 MemAddSize, u8 *pData, u16 Size, u32 Timeout);                                    

                                    
#define USING_I2C 			(1)
#define USING_SMBUS			(0)

#define I2C_SMBusD			(0)
#define I2C_SMBusE			(1)
#define I2C_SMBusF			(2)

#define delay1ms    HAL_Delay	//oec20240608

#define TX0DEV          0
#define TX0ADR    		0x98	//oec20240610	
#define TX0CECADR     	0x9C	
#define RXADR           0x90
#define EDID_ADR        0xA0    // alex 070321

#define pr_err    printf
#define pr_info   printf
#define pr_info2  printf

#define true 1
#define false 0

#define data

#define IDM_HDMI_640x480p60  0
#define IDM_HDMI_480p60      1
#define IDM_HDMI_480p60_16x9 2
#define IDM_HDMI_720p60      3
#define IDM_HDMI_1080i60     4
#define IDM_HDMI_480i60      5
#define IDM_HDMI_480i60_16x9 6
#define IDM_HDMI_1080p60     7
#define IDM_HDMI_576p50      8
#define IDM_HDMI_576p50_16x9 9
#define IDM_HDMI_720p50      10
#define IDM_HDMI_1080i50     11
#define IDM_HDMI_576i50      12
#define IDM_HDMI_576i50_16x9 13
#define IDM_HDMI_1080p50     14
#define IDM_HDMI_1080p24     15
#define IDM_HDMI_1080p25     16
#define IDM_HDMI_1080p30     17
#define IDM_HDMI_4k24        18
#define IDM_HDMI_4k25        19
#define IDM_HDMI_4k30        20
#define IDM_HDMI_4k24_SMPTE  21



enum
{
    STAT_INIT,
    STAT_UART_INIT,
    STAT_IRQ,
};/*20221219*/

BOOL i2c_write_byte(u8 addr, u8 offset, u8 length, u8 *buffer);
BOOL i2c_read_byte(u8 addr, u8 offset, u8 length, u8 *buffer);

BYTE HDMITX_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS HDMITX_WriteI2C_Byte(BYTE RegAddr,BYTE d);
SYS_STATUS HDMITX_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS HDMITX_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS HDMITX_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value);
SYS_STATUS HDMITX_ToggleBit(BYTE Reg,BYTE n);

BYTE CEC_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS CEC_WriteI2C_Byte(BYTE RegAddr,BYTE d);
SYS_STATUS CEC_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS CEC_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS CEC_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value);
SYS_STATUS CEC_ToggleBit(BYTE Reg,BYTE n);

BYTE HDMIRX_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS HDMIRX_WriteI2C_Byte(BYTE RegAddr,BYTE d);
SYS_STATUS HDMIRX_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS HDMIRX_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS HDMIRX_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value);
SYS_STATUS HDMIRX_ToggleBit(BYTE Reg,BYTE n);
#endif
