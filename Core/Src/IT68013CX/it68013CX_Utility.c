///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Utility.c>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/
#include "it68013CX_config.h"
#include "it68013CX_Mcu.h"
#include "it68013CX_IO.h"
#include "it68013CX_Utility.h"
#include "stdio.h"


idata USHORT ucTickCount=0;
idata USHORT loopTicCount = 0;
idata USHORT prevTickCount;
idata USHORT MsdelayCnt =0;

void init_printf(void)
{
    SCON = 0x52;
    T2CON = 0x34;
    TR2 = FALSE;
#ifdef MEGAWIN82516
    TL2 = 0xFD;    //BAUD_SETTING;
    TH2 = 0xFF;    //BAUD_SETTING>>8;
    RCAP2L = 0xFD;    //BAUD_SETTING;
    RCAP2H = 0xFF;    //BAUD_SETTING>>8;
#else
    TL2 = BAUD_SETTING;
    TH2 = BAUD_SETTING>>8;
    RCAP2L = BAUD_SETTING;
    RCAP2H = BAUD_SETTING>>8;
#endif
    TR2 = TRUE;
    TI = 1;
    RI = 0;
    //EA=FALSE;
}

void initialTimer0(void)
{
    TR0=0; // temporarily stop timer 0

    //TMOD &= 0x0F;    // Timer 1, Mode 0, 13 bit
    //TMOD |= 0x10;    // Timer 1, Mode 1, 16 bit

    TMOD &= 0xF0;    // Timer 0, Mode 0, 13 bit
    TMOD |= 0x01;    // Timer 0, Mode 1, 16 bit

    TH0=Tick1ms / 256;
    TL0=Tick1ms % 256;

    TR0    = 1;    // restart the timer
    ET0    = 1;        // Enable Timer Interrupt 0

}

void initialTimer1(void)
{
    TR1=0; // temporarily stop timer 1

    //TMOD &= 0x0F;     //Timer 1, Mode 0, 13 bit
    //TMOD |= 0x11;     //Timer 1, Mode 1, 16 bit

    TMOD = 0x11;     //Timer 1, Mode 1, 16 bit

    TH1= Tick1ms / 256;
    TL1= Tick1ms % 256;

    ucTickCount=0;

    TR1    = 1;    // restart the timer
    ET1    = 1;        // Enable Timer Interrupt 0
    EA=1;
}

void system_tick(void) interrupt 3 {

    TR1=0; // temporarily stop timer 0

    TH1=Tick1ms / 256;
    TL1=Tick1ms % 256;

    ucTickCount++;

    TR1=1; // restart the timer
}


USHORT getloopTicCount()
{
    idata USHORT loopms;

    if(loopTicCount>ucTickCount)
    {
        loopms =  (0xffff-(loopTicCount-ucTickCount));
    }
    else
    {
        loopms =  (ucTickCount-loopTicCount);
    }
    loopTicCount = ucTickCount;
//    MxLRX_DEBUG_PRINTF(" loop ms  = %u\n",loopms));
    return  loopms;

}



USHORT CalTimer(USHORT SetupCnt)
{
    if(SetupCnt>ucTickCount)
    {
        return (0xffff-(SetupCnt-ucTickCount));
    }
    else
    {
        return (ucTickCount-SetupCnt);
    }
}

int TimeOutCheck(USHORT timer, USHORT x)
{
    if(CalTimer(timer)>=x)
    {
        return TRUE;
    }
    return FALSE;
}


BOOL IsTimeOut(USHORT x)
{
    if(CalTimer(prevTickCount) >= x )
    {
        prevTickCount = ucTickCount;
        return TRUE;
    }
    return FALSE;
}

USHORT GetCurrentVirtualTime()
{
    return ucTickCount;
}


void delay1ms(USHORT ms)
{
    USHORT ucStartTickCount,diff;



    ucStartTickCount=ucTickCount;
        do
        {
            if(ucTickCount<ucStartTickCount)
                {
                    diff=0xffff-(ucStartTickCount-ucTickCount);
                }
            else
                {
                    diff=ucTickCount-ucStartTickCount;
                }
        }while(diff<ms);
}

