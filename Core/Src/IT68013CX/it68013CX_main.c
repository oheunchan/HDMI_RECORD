///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Main.c>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/
#include "it68013CX_config.h"

#include "it68013CX_mcu.h"
#include "it68013CX_IO.h"
#include "it68013CX_Utility.h"
#include "stdio.h"
#include "it68013CX_version.h"
#include <stdio.h>
#include <string.h>


#ifdef Enable_IR
#include "Remote.h"
#endif

#include "it68013CX_it680x.h"
#include "it68013CX_MxLrx.h"
#include "it68013CX_it680x_regs.h"


#ifdef SUPPORT_I2C_SLAVE
    #include "I2c_Slave.h"
    #include "I2c_Slave_api.h"
#endif // #ifdef SUPPORT_I2C_SLAVE

#ifdef SUPPORT_UART_CMD
unsigned char m_UartCmd = 0;
#endif


void InitMessage();
void HoldSystem(void);
////////////////////////////////////////////////////////
// InitMessage()
//
// init Urt control and print current Version string
//
//
////////////////////////////////////////////////////////

void InitMessage()
{
#ifdef Debug_message
    init_printf();
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("           %s\n",VERSION_STRING);
#if defined(_IT6802_)
    printf("           IT6802\n");
#elif defined(_IT6803_)
    printf("           IT6803\n");
#else
    printf("           IT6801\n");
#endif
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
#endif
}


void HoldSystem(void)
{
    //Hold_Pin=1;
    //while(!Hold_Pin)
    //{
    #if Debug_message
            printf("Hold\\\r");
            printf("Hold-\r");
            printf("Hold/\r");
            printf("Hold|\r");
    #endif
    //}
}




////////////////////////////////////////////////////////
// main()
//
//
//
////////////////////////////////////////////////////////
// extern void Reset_ITE(void);

void it68013CX_main( void )
{
    static BYTE it68013CX_initialized = 0; //yjh2026
    static USHORT loopinterval = 0; //yjh2026
    //yjh2026 - init once, removed while(1) for non-blocking
    if(!it68013CX_initialized)
    {
#ifdef MEGAWIN82516
        P3M0=0x20;
        P3M1=0x20;
        AFE_RESET_PIN=1;        //for TI442x Demo board only
#else
        // P2_3 = 0;    // for DAC enable !!!
#endif

        #ifdef Enable_IR
            initial_INT1();
            initialTimer0();
        #endif

        initialTimer1();
        InitMessage();

        it6802HPDCtrl(1,0);    // HDMI port , set HPD = 0

        delay1ms(1000);    //for power sequence
        IT6802_fsm_init();

        it6802HPDCtrl(1,1);    // HDMI port , set HPD = 1

        //yjh2026 디버깅용 - 5V 체크 무시하고 Port 0 HPD 강제 올림
        chgbank(1);
        hdmirxset(REG_RX_1B0, 0x03, 0x03);
        chgbank(0);
        printf("[DBG] Port 0 HPD forced HIGH\r\n");

        it68013CX_initialized = 1;
        return;
    }

    //yjh2026 - loop body (called repeatedly from main.c)
    #ifdef Enable_IR
        IRHandler();
    #endif

    #if defined(_IT6802_) || defined(_IT6803_)
        #ifdef SUPPORT_UART_CMD
                UartCommand();
        #endif
    #endif

    loopinterval += getloopTicCount();

    ///////////////////////////
    // sys main flow
    ///////////////////////////
    if( loopinterval >=MS_LOOP) //execute once each 50m second
    {
        loopinterval =0;
        IT6802_fsm();
    }
}


#ifdef SUPPORT_UART_CMD
///////////////////////////////////////////////////////////////////////////////
// I2C for original function call
///////////////////////////////////////////////////////////////////////////////
void UartCommand(void)
{
    char STR[1];
    static int address=0x98;
    if(RI)
    {
        while(1)
        {
 //           printf("Operation[W/R/D]:");
            printf("====================================\n");
            printf("Press 1 for Select MxL Port\n");
            printf("Press 2 for Select HDMI Port\n");
            printf("Press I for show infomation\n");
            printf("Press J for use jump to select port\n");
            printf("====================================\n");
            scanf("%01s",STR);printf("\n");
            if(0==strcmp(STR,"D")|| 0==strcmp(STR,"d"))
            {
            Dump_IT680xReg();
            break;
            }
            else if(0==strcmp(STR,"0"))
            {
            IT6802_fsm_init();
            m_UartCmd =1;
            break;
            }
            else if(0==strcmp(STR,"1"))
            {
            it6802PortSelect(F_PORT_SEL_0);
            m_UartCmd =1;
            break;
            }
            else if(0==strcmp(STR,"2"))
            {
            it6802PortSelect(F_PORT_SEL_1);
            m_UartCmd =2;
            break;
            }
            else if(0==strcmp(STR,"j") || 0==strcmp(STR,"J"))
            {
            m_UartCmd =0;
            break;
            }
            else if(0==strcmp(STR,"i") || 0==strcmp(STR,"I"))
            {
                     printf("================ [ Information  ] ============\n");
                get_vid_info();
                show_vid_info();
                break;
            }
            else if(0==strcmp(STR,"W")|| 0==strcmp(STR,"w"))
            {
                int offset,ewdata;
                printf("[Address][Offset][Data]:");
                scanf("%02x %02x %02x",&address,&offset,&ewdata);printf("\n");
                i2c_write_byte((BYTE)address, (BYTE)offset, 1, ((BYTE *)&ewdata)+1, 0);
                i2c_read_byte((BYTE)address,(BYTE)offset, 1, ((BYTE *)&ewdata)+1, 0);
                printf("%02x, %02x, %02x\n",address,offset,ewdata);
            }
            else if( 0==strcmp(STR,"R")|| 0==strcmp(STR,"r"))
            {
                int offset,count,j;
                BYTE DataBf[16];
                printf("[Address][Offset][count]:");
                scanf("%02x %02x %02d",&address,&offset,&count);printf("\n");
                printf("Address = %02x, Offset = %02x,\n",address,offset);
                if(count>26)count=26;
                while(RI==0)
                {
                    printf("[%3X]  ",(int)offset);
                    if(i2c_read_byte(address, offset, count, DataBf, 0))
                    {
                        for( j = 0; j < count; j++ )
                        {
                            if(0==(j%4) && j)
                            {
                                printf(" -");
                            }
                            printf(" %02X",(int)DataBf[j]);
                        }
                        printf("\r");
                    }
                    else
                    {
                        printf("\r*****I2C Read Fail******\n");
                        break;
                    }
                }
                printf("\n");
            }
            else
            {
                printf("Press not 'Z/z'  key continue\n");
                if(0==strcmp(STR,"Z")|| 0==strcmp(STR,"z"))
                {
                    printf("====Continue Normal Operation====\n");
                    break;
                }
                else
                {
                    printf("Press 'Z/z' key to break setup mode\n");
                    continue;
                }

            }
        }
    }
}
#endif

