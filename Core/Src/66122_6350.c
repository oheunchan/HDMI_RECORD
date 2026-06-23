///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <66122_6350.c>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/
#include "66122_6350.h"
//#include "../../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2c.h"				//oec20240608
#include "../../Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal.h"


unsigned char g_i2c_dev = 1;
extern I2C_HandleTypeDef hi2c1;
extern 	int _write(int file, char* p, int len);

#if 1
#define IT66121_Slave_addr		0x9A

BOOL i2c_write_byte(u8 addr, u8 offset, u8 length, u8 *buffer)

{
	int ret;

    //ret = i2c_block_dma_write(g_i2c_dev, addr, offset, buffer, length );
    ret = HAL_I2C_Mem_Write(&hi2c1, addr, offset,1, buffer, length,HAL_MAX_DELAY);	//oec20240608

    //pr_err("i2c write %02x %02x %d, ret=%d\r\r\n", (int)addr, (int)offset, (int)length, ret);

    if ( 0 > ret )
    {
        pr_err("ERROR: it66122_i2c_write %02x %02x\r\n", addr, offset);
        return false;
    }
    else
    {
        #if _SHOW_IO
        int i;
        pr_info( "i2c_write %02X %02X ", addr, offset, length );
        for( i=0 ; i<length ; i++ )
        {
            pr_info( "%02X ", buffer[i]);
        }
        pr_info( "\r\n");
        #endif
    }
    return true;
}


BOOL i2c_read_byte(u8 addr, u8 offset, u8 length, u8 *buffer)
{
    int ret;

//    ret = i2c_block_dma_read(g_i2c_dev, addr, &offset, 1, buffer, length);
    ret = HAL_I2C_Mem_Read(&hi2c1, addr, offset, 1, buffer, length,HAL_MAX_DELAY);

    //pr_err("i2c read %02x %02x %d, ret=%d\r\r\n", (int)addr, (int)offset, (int)length, ret);
    if ( 0 > ret  )
    {
        pr_err("ERROR: it66122_i2c_read %02x %02x\r\n", addr, offset);
        return false;
    }
    else
    {
        #if _SHOW_IO
        int i;
        pr_info( "i2c_read %02X %02X %d = ", addr, offset, length );

        for( i=0 ; i<length ; i++ )
        {
            pr_info( "%02X ", buffer[i]);
        }
        pr_info( "\r\n");
        #endif
    }
    return true;
}
#endif
/*
 *
 *
 *
 */
SYS_STATUS CEC_WriteI2C_Byte(BYTE RegAddr,BYTE d)
{
    BOOL data flag;

    flag=i2c_write_byte(TX0CECADR,RegAddr,1,&d);
    // printf("pccmd w %x %x %x\r\n",RegAddr,d,TX0CECADR);/*20221228*/
    return !flag;
}

BYTE CEC_ReadI2C_Byte(BYTE RegAddr)
{
    BYTE data p_data;

    i2c_read_byte(TX0CECADR,RegAddr,1,&p_data);

    return p_data;
   
}

SYS_STATUS CEC_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value)
{
    BYTE Temp;
    Temp=CEC_ReadI2C_Byte(Reg);
    Temp&=(~Mask);
    Temp|=Value&Mask;
    return CEC_WriteI2C_Byte(Reg,Temp);
}

/*
 *
 *  TX
 *
 */
BYTE HDMITX_ReadI2C_Byte(BYTE RegAddr)
{
    BYTE data p_data;

    i2c_read_byte(TX0ADR,RegAddr,1,&p_data);

    return p_data;
}

SYS_STATUS HDMITX_WriteI2C_Byte(BYTE RegAddr,BYTE d)
{
    BOOL data flag;

    flag=i2c_write_byte(TX0ADR,RegAddr,1,&d);

    return !flag;
}

SYS_STATUS HDMITX_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N)
{
    BOOL data flag;

    flag=i2c_read_byte(TX0ADR,RegAddr,N,pData);

    return !flag;
}

SYS_STATUS HDMITX_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value)
{
    BYTE Temp;
    if( Mask != 0xFF )
    {
        Temp=HDMITX_ReadI2C_Byte(Reg);
        Temp&=(~Mask);
        Temp|=Value&Mask;
    }
    else
    {
        Temp=Value;
    }
    return HDMITX_WriteI2C_Byte(Reg,Temp);
}

/*
 *
 *  RX
 *
 */
BYTE HDMIRX_ReadI2C_Byte(BYTE RegAddr)
{
    BYTE data p_data;

    i2c_read_byte(RXADR,RegAddr,1,&p_data);

    return p_data;
}

SYS_STATUS HDMIRX_WriteI2C_Byte(BYTE RegAddr,BYTE d)
{
    BOOL data flag;

    flag=i2c_write_byte(RXADR,RegAddr,1,&d);

    return !flag;
}

SYS_STATUS HDMIRX_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N)
{
    BOOL data flag;

    flag=i2c_read_byte(RXADR,RegAddr,N,pData);

    return !flag;
}

SYS_STATUS HDMIRX_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N)
{
    //BOOL data flag;
    //flag=i2c_write_byte(RXADR,RegAddr,N,pData,TX0DEV);
    BOOL flag;
    BYTE I2C_buf[18];
    int     i;
    for (i = 0; i < N; i++)
    {
        I2C_buf[i]=pData[i];
        flag = i2c_write_byte(RXADR, RegAddr++, 1, &I2C_buf[i]);
    }
    //flag = i2c_write_byte(RXADR, RegAddr++, N, I2C_buf, TX0DEV);
    return !flag;
}

SYS_STATUS HDMIRX_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value)
{
    BYTE Temp;
    Temp=HDMIRX_ReadI2C_Byte(Reg);
    Temp&=(~Mask);
    Temp|=Value&Mask;
    return HDMIRX_WriteI2C_Byte(Reg,Temp);
}

SYS_STATUS HDMIRX_ToggleBit(BYTE Reg,BYTE n)
{
    BYTE Temp;
    Temp=HDMIRX_ReadI2C_Byte(Reg);
//    HDMIRX_DEBUG_PRINTF(("INVERVIT  0x%bx[%bx]",Reg,n));
	printf("reg%02X = %02X -> toggle %dth bit ->",(int)Reg,(int)Temp,(int)n) ;
	Temp^=(1<<n) ;
	printf(" %02X\r\n",(int)Temp) ;

//    HDMIRX_DEBUG_PRINTF(("0x%bx\n",Temp));
    return HDMIRX_WriteI2C_Byte(Reg,Temp);
}
