///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <mcu.h>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/

#ifndef _MCU_H_
#define _MCU_H_

#include "it68013CX_515XRAM.h"
#include "it68013CX_typedef.h"
#include "stm32f1xx_hal.h" // STM32 HAL 라이브러리 헤더 포함
//#define MEGAWIN82516 TRUE    //IT6802 Demo board for TI442x platform

#ifdef MEGAWIN82516
#include "REG_MPC82G516.H"
#else
#include "it68013CX_reg_c51.h"
#endif

#ifdef Enable_IR
#include "Remote.h"
#endif



#define _IT6802_BOARD_



#ifndef _MCU_8051_
#define _MCU_8051_
#endif

//#define SUPPORT_UART_CMD

// #define        Enable_IT6802_CEC        //for ARC and HEAC Function

#ifdef Enable_IT6802_CEC

    #ifdef Enable_IT6802_CEC
        #include "it680x_cec.h"
    #endif

    #define    DEBUG_IT6802_CEC
    #define IT6802CECGPIOid        0        // for identife IT6802_CEC use Pin 1.0 & 1.1
#endif



/****************************************************************************/

/****************************************************************************/

#define IT6802A0_HDMI_ADDR 0x94    //Hardware Fixed I2C address of IT6802 HDMI
#define IT6802B0_HDMI_ADDR 0x90    //Hardware Fixed I2C address of IT6802 HDMI
#define MxL_ADDR 0xE0    //Software programmable I2C address of IT6802 MxL
#define EDID_ADDR 0xA8    //Software programmable I2C address of IT6802 EDID RAM
#define CEC_ADDR 0xC8    //Software programmable I2C address of IT6802 CEC


#define DP_ADDR 0x90
#define ADC_ADDR 0x90



#define HDMI_DEV  0
#define DP_DEV      0

#define RXDEV           0
#define MAXRXDEV        1
/****************************************************************************/

/****************************************************************************/
#define HDMI_DEV        0
#define SWADR        0x96
#define EDID_HEAD    0xA0

#define DELAY_TIME        1
#define IDLE_TIME        100

#define HIGH            1
#define LOW                0
#define ACTIVE          1
#define DISABLE         0

/****************************************************************************/
/****************************************************************************/

#if 0

#ifdef MEGAWIN82516
sbit Hold_Pin                   = P2^0;    //for it6802 with MEGAWIN82516
sbit  SEL_PORT_1        = P3^0;
#else
sbit Hold_Pin                   = P1^5;    //for it6802 demo board
sbit SEL_PORT_1               = P1^6;    //for it6802 demo board
#define _HPDMOS_
#endif

sbit  EDID_WP1    = P0^4;
sbit  EDID_WP2    = P0^7;
sbit  EDID_WP3    = P1^4;

//sbit  gpHPD0    = P1^7;

sbit  gpHPD0    = P4^3;



sbit VsyncToggle = P0^2;

sbit  LED1        = P2^7;
//sbit  LED1        = P2^0;
sbit  LED2        = P2^6;


sbit  AFE_RESET_PIN        = P3^5;

#else

//yjh2026 회로도 보고 핀 다시 잡음 (F103C8은 GPIOD/E 없음)
#ifdef MEGAWIN82516
  #define Hold_Pin_PORT          GPIOC
  #define Hold_Pin_PIN           GPIO_PIN_0
  #define SEL_PORT_1_PORT        GPIOD
  #define SEL_PORT_1_PIN         GPIO_PIN_0
#else
  #define Hold_Pin_PORT          GPIOB  //yjh2026 TODO - 미연결, 안씀
  #define Hold_Pin_PIN           GPIO_PIN_5

  #define SEL_PORT_1_PORT        GPIOB  //yjh2026 TODO - 미연결, 안씀
  #define SEL_PORT_1_PIN         GPIO_PIN_6

  #define _HPDMOS_
#endif

//yjh2026 TODO - EDID_WP 회로도에 없음, 코드에서도 안씀
#define EDID_WP1_PORT            GPIOB
#define EDID_WP1_PIN             GPIO_PIN_5  //yjh2026 dummy

#define EDID_WP2_PORT            GPIOB
#define EDID_WP2_PIN             GPIO_PIN_5  //yjh2026 dummy

#define EDID_WP3_PORT            GPIOB
#define EDID_WP3_PIN             GPIO_PIN_5  //yjh2026 dummy

//yjh2026 TODO - gpHPD0은 int변수로 처리중, GPIO 안씀 (HPD는 커넥터 직결)
#define gpHPD0_PORT              GPIOB  //yjh2026 GPIOE->GPIOB (GPIOE 없음)
#define gpHPD0_PIN               GPIO_PIN_5  //yjh2026 dummy

//yjh2026 TODO - VsyncToggle 미연결, 안씀 (PA2는 UART2임)
#define VsyncToggle_PORT         GPIOB
#define VsyncToggle_PIN          GPIO_PIN_5  //yjh2026 dummy

//yjh2026 LED1->STATUS_LED(PB14), LED2->REC_LED(PB15) 로 대체
#define LED1_PORT                GPIOB
#define LED1_PIN                 GPIO_PIN_14 //yjh2026 STATUS_LED

#define LED2_PORT                GPIOB
#define LED2_PIN                 GPIO_PIN_15 //yjh2026 REC_LED

//yjh2026 TODO - AFE_RESET 미연결, MEGAWIN 전용 (GPIOD 없음)
#define AFE_RESET_PIN_PORT       GPIOB
#define AFE_RESET_PIN_PIN        GPIO_PIN_5  //yjh2026 dummy


#endif

#define    Remote_Receive_Pin    P3_3

#ifdef _HPDMOS_
    #define HPDON        0
    #define HPDOFF        1
#else
    #define HPDON        1
    #define HPDOFF        0
#endif

#endif
