///*****************************************
//  Copyright (C) 2009-2023
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <OEM1_Main.c>
//   @author Jin.Lin@ite.com.tw
//   @date   2023/09/27
//   @fileversion: IT66122 Sample_0.92
//******************************************/
#include <stdio.h>

//#include "IO_IT6350.h"
//#include "version.h"
#if 0
#include "66122_src/66122_6350.h"
#include "66122_src/hdmitx_sys.h"
#include "66122_src/hdmitx_drv.h"
#else
#include "66122_6350.h"
#include "hdmitx_sys.h"
#include "hdmitx_drv.h"
#include "Global_hid.h"
#endif
//****************************************************************************
unsigned char g_current_system_state = 0;
unsigned char g_previous_system_state = 0;
//****************************************************************************



#define	VERSION_EDID_SERIALID	0x092
#define	VERID0	'0'
#define	VERID1	'9'
#define	VERID2	'2'
#define	VER_MAJOR	0x00
#define	VER_MINOR	0x92
#define	BUILD_DATE_STR	"2023/09/27 11:40"
#define	RANDOM_VALUE_1	0xEA
#define	RANDOM_VALUE_2	0x33
#define	RANDOM_VALUE_3	0xF2
#define	RANDOM_VALUE_4	0x9F


#define SWITCH_ON 0
#define SWITCH_OFF 1
#define DIPSW1 1

#define TRP4 1
#define TRP3 1
#define TRP2 1
#define TRP1 1
#define Button2 1

void change_config_by_switch(void);
extern 	void LED_ON_OFF(u8 onoff);	//oec20250705

// int led_count = 0;
// int led_state = 0;

static int g_curr_byte_mode=0;
static int g_prev_byte_mode=0;
static int g_curr_dessc=0;
static int g_prev_dessc=0;
static int g_curr_hdcp=1;
static int g_prev_hdcp=1;
static int g_hold_state=0;

#ifndef _FIXED_CONFIG_
static char g_enable_byte_mode_sel=1;
#endif
static char g_enable_hdcp_sel=1;
static char g_enable_dessc_sel=1;
static char g_uart_hold=0;

int g_main_state = STAT_INIT;


void change_config_by_switch(void){/*20230606*//*Fix TTL Bus & in/out Color Mode*/
    BYTE in_color = 0;
    BYTE out_color = 0;
    BYTE in_color_mode = 0;
    BYTE video_signal_type = 0;
    printf("\r\n\r\n\r\n--------------- config of 66122 ---------------\r\n");
    //DIPSW
    #ifdef IT66120
    if(1){/*20230530*//*20230606*/
    #else
    if(DIPSW1 == SWITCH_ON){
    #endif
        printf("\tTTL Bus : DDR Mode\r\n");
        setHDMITX_VideoSignalType(( T_MODE_INDDR|T_MODE_PCLKDIV2));
    }else{
        printf("\tTTL Bus : SDR Mode\r\n");
        setHDMITX_VideoSignalType(0);
    }
    /*20230606*/
    #ifdef IT66120
    printf("\tOverWrite TTL Bus : T_MODE_INDDR|T_MODE_PCLKDIV2|T_MODE_CCIR656\r\n");
    setHDMITX_VideoSignalType(( T_MODE_INDDR|T_MODE_PCLKDIV2|T_MODE_CCIR656));
    #endif

    //TRP12
    #ifdef IT66120
    in_color = 0x01;
    #else
    in_color = (TRP1>>3) + (TRP2>>3);
    #endif
    switch(in_color){
        case 0x00:
        case 0x03:
            printf("\tForce Input Color Space setting to RGB444\r\n");
            SetInputColorMode(F_MODE_RGB444);
            break;
        case 0x01:
            printf("\tForce Input Color Space setting to YUV422 \r\n");
            SetInputColorMode(F_MODE_YUV422);
            break;
        case 0x02:
            printf("\tForce Input Color Space setting to YUV444 \r\n");
            SetInputColorMode(F_MODE_YUV444);
            break;
        default:
            break;
    }
    //TRP34
    #ifdef IT66120
    out_color = 0x01;
    #else
    out_color = (TRP3>>5) + (TRP4>>4);
    #endif
    switch(out_color){
        case 0x00:
        case 0x03:
            printf("\tForce Output Color Space setting to RGB444 \r\n");
            SetOutputColorMode(F_MODE_RGB444);
            break;
        case 0x01:
            printf("\tForce Output Color Space setting to YUV422 \r\n");
            SetOutputColorMode(F_MODE_YUV422);
            break;
        case 0x02:
            printf("\tForce Output Color Space setting to YUV444 \r\n");
            SetOutputColorMode(F_MODE_YUV444);
            break;
        default:
            break;
    }
    /*20230220*/
    in_color_mode = GetInputColorMode();
    video_signal_type = GetInputSignalType();
    hdmitx_SetInputMode(in_color_mode, video_signal_type);
    printf("--------------- =============== ---------------\r\n");
}

void led_show_status(void)
{

}
extern unsigned int ite_int_check; //oec20251221

// BYTE step_cnt;
extern void Reset_ITE(void);

void OEM_1_MainLoop(void)
{
	BYTE count=0;
	//BYTE loop_cnt=0;
	// static BYTE boot_option=0;
	// static WORD loop_cnt=0;
	// static WORD dbg_cnt=0;
	//	BYTE test=0xff;

	switch(g_main_state){
	
		case STAT_IRQ:

			// if(!boot_option)
			// {
			// 	HDMITX_ChangeDisplayOption(HDMI_1080p60,HDMI_YUV422) ;
			// 	printf("\r\nHDMITX_ChangeDisplayOption(HDMI_1080p60,HDMI_YUV422)\r\n");
			// 	boot_option=1;
			// }

			// if(loop_cnt<500)	loop_cnt++;
			// else{	HDMITX_DevLoopProc();	loop_cnt=0;}	
			HDMITX_DevLoopProc();   //oec202506705
            // printf("################reset_flag %d",reset_flag);
            //if(ite_int_check)   //oec20251221

			#if 0
			if(dbg_cnt<300)		dbg_cnt++;
			else {
							printf("[0xBF]: %x\r\n",HDMITX_ReadI2C_Byte(0xBF));
//							printf("[0xa1]: %x\r\n",HDMITX_ReadI2C_Byte(0xa1));

			//				printf("[0x71]: %x\r\n",HDMITX_ReadI2C_Byte(0x71)); 
			//				printf("[0x11]: %x\r\n",HDMITX_ReadI2C_Byte(0x11)); 
			//				printf("[0x12]: %x\r\n",HDMITX_ReadI2C_Byte(0x12)); 
			//				printf("[0x14]: %x\r\n",HDMITX_ReadI2C_Byte(0x14)); 
			//				printf("[0x15]: %x\r\n",HDMITX_ReadI2C_Byte(0x15)); 
			dbg_cnt=0;
			}
			#endif


			count ++ ;
			if( count > 50 )
			{
				DumpHDMITXReg() ;
				count = 0 ;
			}
			
			break;
		
		case STAT_UART_INIT:

			Reset_ITE();
			g_main_state = STAT_IRQ;
			//HDMITX_WriteI2C_Byte(0xBf,0x22);  //CBCR SWAP
			break;
		
		case STAT_INIT:
            printf("**FW Version V1.0**\r\n");
			printf("IT66122_STAT_INIT\r\n");
		    // Reset_ITE(); //oec20251221
			InitHDMITX_Variable();
			InitHDMITX();
			LED_ON_OFF(0);
        	HDMITX_ChangeDisplayOption(HDMI_1080p60,HDMI_YUV422) ;  //oec202506705
		 	printf("\r\nHDMITX_ChangeDisplayOption(HDMI_1080p60,HDMI_YUV422)\r\n"); //oec202506705
		//              change_config_by_switch();/*20221219*/
		//               HDMITX_ChangeDisplayOption(HDMI_1080p60,GetOutputColorMode()) ;
		//				HDMITX_ChangeDisplayOption(HDMI_1080p30,GetOutputColorMode()) ;

		//                HDMITX_ChangeDisplayOption(HDMI_4k30,GetOutputColorMode()) ;
		//              ResetVideoPixelCock();/*20230201*/
		#ifdef CONFIG_SPDINFOFRAME_SMAPLE
			ConfigSPDInfoFrm();/*20230210*/
		#endif                
			g_main_state = STAT_UART_INIT;
		//          }
			break;
						
		default:		break;
	}
}




#include <stdio.h>
// #include <string.h>
//#include "../../IT6350/code/include.h"
//#include "../../IT6350/code/api/api_adc.h"
//#include "../IO_IT6350.h"
//#include "global.h"
//#include "global_hid.h"
#define _IT6350_USBFW_
#include "global_hid.h"

// typedef unsigned char UCHAR;


#define FW_VER_MAJOR 1
#define FW_VER_MINOR 5

extern UCHAR i2c_block_dma_read(UCHAR ucCH, UCHAR ucSlaveID, UCHAR* pInputBuf, UCHAR ucWCount, UCHAR* pOutputBuf, UCHAR ucRCount);
extern UCHAR i2c_block_dma_write(UCHAR ucCH,UCHAR ucSlaveID, UCHAR ucAddr, UCHAR* pInputBuf, UCHAR ucWCount);
//extern UINT8 i2c_block_dma_write2(UINT8 ucCH,UINT8 ucSlaveID, UINT8* pInputBuf, UINT8 ucWCount);

extern unsigned char g_i2cPreScale;

static unsigned char g_io_led = 1;
static int g_result;
static struct kIRP prev_irp;
char g_led_pwm_off = 0;

//char g_opt_i2c_mode = OPTION_I2C_MODE_HARDWARE;
char g_opt_i2c_mode = OPTION_I2C_MODE_SOFTWARE;

void get_feature_report_adc_handler( struct kIRP *irp );
void set_feature_report_adc_handler( struct kIRP *irp );
void get_feature_report_lcd_handler( struct kIRP *irp );
void set_feature_report_lcd_handler( struct kIRP *irp );
void get_feature_report_set_option_handler( struct kIRP *irp );
void get_feature_report_get_option_handler( struct kIRP *return_irp );
void lcd_show_voltage(unsigned short value);
void lcd_data(unsigned char value);

#define DEBUG_HID 0
#define USBIIC_VER 1

// #if (USBIIC_VER==2)
//     static unsigned char g_i2c_dev_no = 1;  // for v2
// #else
//     static unsigned char g_i2c_dev_no = 1;  // for 6265 evb
// #endif


#if DEBUG_HID
    #define _debug_HID(x) printf x
    #define print_buf(x) _print_buf x
#else
    #define _debug_HID(x)
    #define print_buf(x)
#endif

#if 0
    #define LED3_ON(x) {GPDRA &= ~0x01;}
    #define LED3_OFF(x) {GPDRA |= 0x01;}
    #define LED4_ON(x) {GPDRA &= ~0x02;}
    #define LED4_OFF(x) {GPDRA |= 0x02;}
#else
    #if (USBIIC_VER==2)
        #define LED3_ON(x) {GPCRA0 = ALT;}
        #define LED3_OFF(x) {GPCRA0 = INPUT;}
        //#define LED4_ON(x) {}
        //#define LED4_OFF(x) {}


        #define LED1_ON(x) {GPCRA1 = ALT;}
        #define LED1_OFF(x) {GPCRA1 = INPUT;}
        #define LED4_ON(x) {GPCRA4 = ALT;}
        #define LED4_OFF(x) {GPCRA4 = INPUT;}
        #define LED5_ON(x) {GPCRA5 = ALT;}
        #define LED5_OFF(x) {GPCRA5 = INPUT;}

        void usb_set_led_state(char state)
        {
            static char prev_state = 99;

            if (state==7) {
                prev_state = 7;
            }
            else if (prev_state > state) {
                prev_state = state;
            }

            state = prev_state;

            if (state & 0x01) {
                LED1_ON();
            }else{
                LED1_OFF();
            }
            if ( state & 0x02 ) {
                LED4_ON();
            }else{
                LED4_OFF();
            }
            if ( state & 0x04 ) {
                LED5_ON();
            }else{
                LED5_OFF();
            }
        }

    #else
        #define LED3_ON(x) {}
        #define LED3_OFF(x) {}
        #define LED4_ON(x) {}
        #define LED4_OFF(x) {}

        #define usb_set_led_state(x)

    #endif
#endif



//#if DEBUG_HID
void _print_buf( UCHAR *Buffer, int Length, char *PreFix, char *SufFix, char Cut)
{
    int i;
    printf(PreFix);
    for (i=0 ; i<Length ; i++)
    {
        printf("%02x ",(int)Buffer[i]);
        if ( (i % Cut) == (Cut-1) ) {
            printf("\r\n");
        }
    }
    printf(SufFix);
}
//#endif

