///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.c>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/
#include "it68013CX_config.h"
#include "it68013CX_MCU.h"
#include "it68013CX_IO.h" //yjh2026
//#include "nop.h"
#include <stdio.h>

#ifdef _Myson_CS8957_
BYTE xdata  PADMOD0            _at_  0x0f50;
BYTE xdata  PORT4OE            _at_  0x0f57;
BYTE xdata  PORT40             _at_  0x0f58;
BYTE xdata  PORT41             _at_  0x0f59;
BYTE xdata  PORT42             _at_  0x0f5A;
BYTE xdata  PORT43             _at_  0x0f5B;
BYTE xdata  PORT44             _at_  0x0f5C;
void InitMysonIO()
{
     PADMOD0 = 0;
     PORT4OE = 0x00;
     PORT40 = 0;
     PORT42 = 0;
     PORT43 = 0;

}
#endif


//BYTE idata I2CDEV=0;

///////////////////////////////////////////////////////////////////////////////
//IO Pin Config For I2C control
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#if 0
#ifdef _IT6802_BOARD_
sbit  DEV0_SCL_PORT = P1^0;
sbit  DEV0_SDA_PORT = P1^1;


sbit  DEV1_SCL_PORT = P1^2;
sbit  DEV1_SDA_PORT = P1^3;

sbit  DEV2_SCL_PORT = P1^5;
sbit  DEV2_SDA_PORT = P1^6;

sbit  DEV3_SCL_PORT = P1^0;
sbit  DEV3_SDA_PORT = P1^1;

sbit  DEV4_SCL_PORT = P1^2;
sbit  DEV4_SDA_PORT = P1^3;
#else
sbit  DEV0_SCL_PORT = P0^0;
sbit  DEV0_SDA_PORT = P0^1;


sbit  DEV1_SCL_PORT = P0^2;
sbit  DEV1_SDA_PORT = P0^3;

sbit  DEV2_SCL_PORT = P0^5;
sbit  DEV2_SDA_PORT = P0^6;

sbit  DEV3_SCL_PORT = P0^0;
sbit  DEV3_SDA_PORT = P0^1;

sbit  DEV4_SCL_PORT = P0^2;
sbit  DEV4_SDA_PORT = P0^3;

#endif
#endif

extern int gpHPD0; //oec2026

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////

void HotPlug(BYTE bEnable)
{
     if(bEnable)
         gpHPD0 = HPDON;
     else
         gpHPD0 = HPDOFF;

}

void SetintActive(BOOL bactive)
{
    bactive =0;  //fake code

}
#if 0

void set_8051_scl( BOOL bit_value,BYTE device )
{
//     stall5us;
     switch(device)
     {
         case 0:
             DEV0_SCL_PORT=bit_value;
            break;
         case 1:
             DEV1_SCL_PORT=bit_value;
            break;
        case 2:
             DEV2_SCL_PORT=bit_value;
            break;
         case 3:
             DEV3_SCL_PORT=bit_value;
            break;
         case 4:
             DEV4_SCL_PORT=bit_value;
            break;
        default:
             DEV0_SCL_PORT=bit_value;
            break;
     }
//     stall5us;
}

void set_8051_sda( BOOL bit_value,BYTE device )
{
//     stall5us;
     switch( device )
     {
         case 0:
             DEV0_SDA_PORT=bit_value;
            break;
         case 1:
             DEV1_SDA_PORT=bit_value;
            break;
         case 2:
             DEV2_SDA_PORT=bit_value;
            break;
         case 3:
             DEV3_SDA_PORT=bit_value;
            break;
         case 4:
             DEV4_SDA_PORT=bit_value;
            break;

        default:
             DEV0_SDA_PORT=bit_value;
             break;
     }
//     stall5us;
}

BOOL get_8051_sda( BYTE device )
{
     switch( device )
     {
         case 0:
             DEV0_SDA_PORT=1;
             return DEV0_SDA_PORT;
            break;
         case 1:
             DEV1_SDA_PORT=1;
             return DEV1_SDA_PORT;
            break;
         case 2:
             DEV2_SDA_PORT=1;
             return DEV2_SDA_PORT;
            break;

         case 3:
             DEV3_SDA_PORT=1;
             return DEV3_SDA_PORT;
            break;

         case 4:
             DEV4_SDA_PORT=1;
             return DEV4_SDA_PORT;
            break;

        default:
             DEV0_SDA_PORT=1;
             return DEV0_SDA_PORT;
             break;
     }
}

void i2c_8051_start( BYTE device )
{
    set_8051_sda( HIGH ,device);
    set_8051_scl( HIGH ,device);
    set_8051_sda( LOW  ,device);
    set_8051_scl( LOW  ,device);
}

void i2c_8051_write( BYTE byte_data,BYTE device )
{
 BYTE data bit_cnt,tmp;
 BOOL data bit_value;

     for(bit_cnt=0; bit_cnt<8; bit_cnt++) {
         tmp=(byte_data << bit_cnt) & 0x80;
         bit_value=tmp && 0x80;

         set_8051_sda( bit_value,device);
         set_8051_scl( HIGH     ,device);
         set_8051_scl( LOW      ,device);
     }
}

BOOL i2c_8051_wait_ack( BYTE device )
{
 BOOL data ack_bit_value;

    set_8051_sda( HIGH,device );
    set_8051_scl( HIGH,device );
    ack_bit_value=get_8051_sda( device );
    set_8051_scl( LOW ,device);

    return ack_bit_value;
}

BYTE i2c_8051_read( BYTE device )
{
 BYTE data bit_cnt,byte_data;
 BOOL data bit_value;

     byte_data=0;
     for(bit_cnt=0; bit_cnt<8; bit_cnt++) {
         set_8051_scl( HIGH,device );

         bit_value=get_8051_sda( device );

         byte_data=(byte_data << 1) | bit_value;

         set_8051_scl( LOW,device );
     }

     return byte_data;
}

void i2c_8051_send_ack( BOOL bit_value,BYTE device )
{
     set_8051_sda( bit_value,device );
     set_8051_scl( HIGH ,device);
     set_8051_scl( LOW ,device);
     set_8051_sda( HIGH,device );
}

void i2c_8051_end( BYTE device )
{
     set_8051_sda( LOW,device );
     set_8051_scl( HIGH,device );
     set_8051_sda( HIGH,device );
}

BOOL i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{

     BYTE data i;



     i2c_8051_start(device);

     i2c_8051_write(address&0xFE,device);
     if( i2c_8051_wait_ack(device)==1 )    {
         i2c_8051_end(device);
     return 0;
      }

     i2c_8051_write(offset,device);
     if( i2c_8051_wait_ack(device)==1 )    {
         i2c_8051_end(device);
     return 0;
     }

     for(i=0; i<byteno-1; i++) {
          i2c_8051_write(*p_data,device);
          if( i2c_8051_wait_ack(device)==1 ) {
              i2c_8051_end(device);
         return 0;
         }
         p_data++;
     }

     i2c_8051_write(*p_data,device);
     if( i2c_8051_wait_ack(device)==1 )    {
          i2c_8051_end(device);
     return 0;
     }
     else {
          i2c_8051_end(device);
     return 1;
     }
}

BOOL i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{
 BYTE data i;

     i2c_8051_start(device);

     i2c_8051_write(address&0xFE,device);
     if( i2c_8051_wait_ack(device)==1 ) {
         i2c_8051_end(device);
         return 0;
     }

     i2c_8051_write(offset,device);
     if( i2c_8051_wait_ack(device)==1 ) {
         i2c_8051_end(device);
         return 0;
     }

     i2c_8051_start(device);

     i2c_8051_write(address|0x01,device);
     if( i2c_8051_wait_ack(device)==1 ) {
         i2c_8051_end(device);
         return 0;
     }

     for(i=0; i<byteno-1; i++) {
         *p_data=i2c_8051_read(device);
         i2c_8051_send_ack(LOW,device);

         p_data++;
     }

     *p_data=i2c_8051_read(device);
     i2c_8051_send_ack(HIGH,device);
     i2c_8051_end(device);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////////////
//IIC control Functions
//
///////////////////////////////////////////////////////////////////////////////////////

#ifdef Enable_IT6802_CEC
BYTE IT6802_CEC_ReadI2C_Byte(BYTE RegAddr)
{
     BYTE  p_data;
     BOOL    FLAG;

    FLAG=i2c_read_byte(CEC_ADDR,RegAddr,1,&p_data,IT6802CECGPIOid);

      if(FLAG==0)
    {
             CEC_DEBUG_PRINTF(("IT6802_CEC I2C ERROR !!!"));
            CEC_DEBUG_PRINTF(("=====  Read Reg0x%X=\n",RegAddr));

    }

     return p_data;
}


SYS_STATUS IT6802_CEC_WriteI2C_Byte(BYTE offset,BYTE buffer )
{
     BOOL  flag;

     flag=i2c_write_byte(CEC_ADDR,offset,1,&buffer,IT6802CECGPIOid);

     return !flag;
}

#endif

#else //yjh2026

extern I2C_HandleTypeDef hi2c1;

BOOL i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{
	HAL_StatusTypeDef ret;
	(void)device;
	ret = HAL_I2C_Mem_Write(&hi2c1, address, offset, I2C_MEMADD_SIZE_8BIT, p_data, byteno, 100);
	if(ret != HAL_OK)
	{
		printf("i2c_wr err 0x%02X 0x%02X\r\n", address, offset);
		return FALSE;
	}
	return TRUE;
}

BOOL i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{
	HAL_StatusTypeDef ret;
	(void)device;
	ret = HAL_I2C_Mem_Read(&hi2c1, address, offset, I2C_MEMADD_SIZE_8BIT, p_data, byteno, 100);
	if(ret != HAL_OK)
	{
		printf("i2c_rd err 0x%02X 0x%02X\r\n", address, offset);
		return FALSE;
	}
	return TRUE;
}

#endif