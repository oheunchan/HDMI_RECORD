///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <hdmitx_input.c>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/
#include "hdmitx.h"
#include "hdmitx_drv.h"

#ifdef HDMITX_INPUT_INFO
extern HDMITXDEV hdmiTxDev[HDMITX_MAX_DEV_COUNT] ;


LONG CalcRCLK();
LONG CalcAudFS();
LONG CalcRCLK();
ULONG AbsoluteLong(LONG r);
BYTE IsPCLKStable();
#define InitCEC() HDMITX_SetI2C_Byte(0x0F, 0x08, 0x00)
#define DisableCEC() HDMITX_SetI2C_Byte(0x0F, 0x08, 0x08)

#define BIT2 	0x00000004	//oec20240608

LONG CalcAudFS()
{
    // LONG RCLK ;
    LONG Cnt ;
    LONG FS ;

    // RCLK = CalcRCLK();
    Switch_HDMITX_Bank(0);
    Cnt = (LONG)HDMITX_ReadI2C_Byte(0x60);
	/*20230112*/
	if( HDMITX_ReadI2C_Byte(REG_TX_INPUT_MODE) & 0x20 )
	{
    	FS =  hdmiTxDev[0].RCLK / 4 ;
	}else{
		FS =  hdmiTxDev[0].RCLK / 2 ;
	}
	FS /= Cnt ;
	HDMITX_DEBUG_PRINTF1(("FS = %ld RCLK = %ld, Cnt = %ld\r\n",FS,hdmiTxDev[0].RCLK,Cnt)) ;
    return FS ;
}

LONG CalcPCLK()
{
	BYTE uc, div ;
	int i ;
	long sum , count, PCLK   ;

	Switch_HDMITX_Bank(0);
	uc = HDMITX_ReadI2C_Byte(0x5F) & 0x80 ;

	if( ! uc )
	{
	    return 0 ;
	}
    // InitCEC();
    // // uc = CEC_ReadI2C_Byte(0x09) & 0xFE ;
    // CEC_WriteI2C_Byte(0x09, 1);
    // delay1ms(100);
    // CEC_WriteI2C_Byte(0x09, 0);
    // RCLK = CEC_ReadI2C_Byte(0x47);
    // RCLK <<= 8 ;
    // RCLK |= CEC_ReadI2C_Byte(0x46);
    // RCLK <<= 8 ;
    // RCLK |= CEC_ReadI2C_Byte(0x45);
    // DisableCEC();
    // // RCLK *= 160 ; // RCLK /= 100 ;
    // // RCLK in KHz.

	HDMITX_SetI2C_Byte(0xD7, 0xF0, 0x80);
//	delay1ms(1);
	HAL_Delay(1);	//oec20240608

	HDMITX_SetI2C_Byte(0xD7, 0x80, 0x00);

	count = HDMITX_ReadI2C_Byte(0xD7) & 0xF ;
	count <<= 8 ;
	count |= HDMITX_ReadI2C_Byte(0xD8);
	for( div = 7 ; div > 0 ; div-- )
	{
	    // printf("div = %d\n",(int)div) ;
		if(count < (1<<(11-div)) )
		{
			break ;
		}
	}
	HDMITX_SetI2C_Byte(0xD7, 0x70, div<<4);

    uc = HDMITX_ReadI2C_Byte(0xD7) & 0x7F ;
	for( i = 0 , sum = 0 ; i < 100 ; i ++ )
	{
		HDMITX_WriteI2C_Byte(0xD7, uc|0x80) ;
//		delay1ms(1);
		HAL_Delay(1);	//oec20240608

		HDMITX_WriteI2C_Byte(0xD7, uc) ;

		count = HDMITX_ReadI2C_Byte(0xD7) & 0xF ;
		count <<= 8 ;
		count |= HDMITX_ReadI2C_Byte(0xD8);
		sum += count ;
	}
	sum /= 100 ; count = sum ;

    HDMITX_DEBUG_PRINTF1(("RCLK(in GetPCLK) = %ld\r\n",hdmiTxDev[0].RCLK));
	HDMITX_DEBUG_PRINTF1(("div = %d, count = %d\r\n",(int)div,(int)count) );
	HDMITX_DEBUG_PRINTF1(("count = %ld\r\n",count) );

	PCLK = hdmiTxDev[0].RCLK * 128 / count * 16 ;
	PCLK *= (1<<div);

	// if( HDMITX_ReadI2C_Byte(0x70) & 0x10 )

    if(HDMITX_ReadI2C_Byte(0x70) & (B_TX_2X656CLK) )/*20230606*//*66120 CCIR656*/
    {
        PCLK /= 2 ;
    }

	HDMITX_DEBUG_PRINTF1(("PCLK = %ld\r\n",PCLK) );
	return PCLK ;
}

LONG CalcRCLK()
{
	// BYTE uc ;
	int i ;
	long sum, RCLKCNT  ;
    BYTE RCLK_20M;

	InitCEC();
	sum = 0 ;
	for( i = 0 ; i < 5 ; i++ )
	{
		// uc = CEC_ReadI2C_Byte(0x09) & 0xFE ;
		CEC_WriteI2C_Byte(0x09, 1);
//		delay1ms(100);
		HAL_Delay(100);	//oec20240608

		CEC_WriteI2C_Byte(0x09, 0);
		RCLKCNT = CEC_ReadI2C_Byte(0x47);
		RCLKCNT <<= 8 ;
		RCLKCNT |= CEC_ReadI2C_Byte(0x46);
		RCLKCNT <<= 8 ;
		RCLKCNT |= CEC_ReadI2C_Byte(0x45);
		// HDMITX_DEBUG_PRINTF1(("RCLK = %ld\n",RCLKCNT) );
		sum += RCLKCNT ;
	}
	DisableCEC();

	RCLKCNT = sum * 32 ;

    RCLK_20M = (HDMITX_ReadI2C_Byte(0x5D)&BIT2)>>2 ; /*20230925*/
    if(RCLK_20M)    RCLKCNT = RCLKCNT>>1;

	HDMITX_DEBUG_PRINTF(("RCLK = %ld,%03ld,%03ld\r\n",RCLKCNT/1000000,(RCLKCNT%1000000)/1000,RCLKCNT%1000));
	return RCLKCNT ;
}

USHORT hdmitx_getInputHTotal()
{
    BYTE uc ;
    USHORT hTotal ;
    HDMITX_SetI2C_Byte(0x0F,1,0) ;
    HDMITX_SetI2C_Byte(0xA8,8,8) ;

    uc = HDMITX_ReadI2C_Byte(0xB2) ;
    hTotal = (uc&1)?(1<<12):0 ;
    uc = HDMITX_ReadI2C_Byte(0x91) ;
    hTotal |= ((USHORT)uc)<<4 ;
    uc = HDMITX_ReadI2C_Byte(0x90) ;
    hTotal |= (uc&0xF0) >> 4 ;
    HDMITX_SetI2C_Byte(0xA8,8,0) ;
    return hTotal ;
}

USHORT hdmitx_getInputVTotal()
{
    BYTE uc ;
    USHORT vTotal ;
    HDMITX_SetI2C_Byte(0x0F,1,0) ;
    HDMITX_SetI2C_Byte(0xA8,8,8) ;

    uc = HDMITX_ReadI2C_Byte(0x99) ;
    vTotal = ((USHORT)uc&0xF)<<8 ;
    uc = HDMITX_ReadI2C_Byte(0x98) ;
    vTotal |= uc;
    HDMITX_SetI2C_Byte(0xA8,8,0) ;
    return vTotal ;
}

BOOL hdmitx_isInputInterlace()
{
    BYTE uc ;

    HDMITX_SetI2C_Byte(0x0F,1,0) ;
    HDMITX_SetI2C_Byte(0xA8,8,8) ;

    uc = HDMITX_ReadI2C_Byte(0xA5) ;
    HDMITX_SetI2C_Byte(0xA8,8,0) ;
    return uc&(1<<4)?TRUE:FALSE ;
}

BYTE hdmitx_getAudioCount()
{
    return HDMITX_ReadI2C_Byte(REG_TX_AUD_COUNT) ;
}

ULONG AbsoluteLong(LONG r)
{
	LONG mask = r>>(sizeof(LONG)*8-1);
	r = (r+mask) ^ mask;
	return (ULONG)r;
}

BYTE IsPCLKStable()
{
	static LONG prePCLK = 0;
	LONG diff;
	ULONG diff_rate;
	LONG currPCLK = CalcPCLK();
	if(currPCLK == 0){
		return FALSE;
	}
	diff = currPCLK - prePCLK;
	diff_rate = (ULONG)(AbsoluteLong(diff)/currPCLK)*100;
	HDMITX_DEBUG_PRINTF(("prePCLK = %ld, currPCLK = %ld, diff = %ld, diff_rate = %lu%%\r\n",prePCLK,currPCLK,diff,diff_rate));
	prePCLK = currPCLK;
	if(diff_rate < 10){
		HDMITX_DEBUG_PRINTF(("PCLK Stable!\r\n"));
		return TRUE;
	}
	return FALSE;
}
#endif
