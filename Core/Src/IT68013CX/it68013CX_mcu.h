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

#ifdef MEGAWIN82516
  // P2.0 -> GPIOC, Pin 0 (예시)
  #define Hold_Pin_PORT          GPIOC
  #define Hold_Pin_PIN           GPIO_PIN_0

  // P3.0 -> GPIOD, Pin 0 (예시)
  #define SEL_PORT_1_PORT        GPIOD
  #define SEL_PORT_1_PIN         GPIO_PIN_0
#else
  // P1.5 -> GPIOB, Pin 5 (예시)
  #define Hold_Pin_PORT          GPIOB
  #define Hold_Pin_PIN           GPIO_PIN_5

  // P1.6 -> GPIOB, Pin 6 (예시)
  #define SEL_PORT_1_PORT        GPIOB
  #define SEL_PORT_1_PIN         GPIO_PIN_6

  #define _HPDMOS_
#endif

// P0.4 -> GPIOA, Pin 4 (예시)
#define EDID_WP1_PORT            GPIOA
#define EDID_WP1_PIN             GPIO_PIN_4

// P0.7 -> GPIOA, Pin 7 (예시)
#define EDID_WP2_PORT            GPIOA
#define EDID_WP2_PIN             GPIO_PIN_7

// P1.4 -> GPIOB, Pin 4 (예시)
#define EDID_WP3_PORT            GPIOB
#define EDID_WP3_PIN             GPIO_PIN_4

// P4.3 -> GPIOE, Pin 3 (예시 - STM32F103 대용량 모델에 GPIOE 존재)
#define gpHPD0_PORT              GPIOE
#define gpHPD0_PIN               GPIO_PIN_3

// P0.2 -> GPIOA, Pin 2 (예시)
#define VsyncToggle_PORT         GPIOA
#define VsyncToggle_PIN          GPIO_PIN_2

// P2.7 -> GPIOC, Pin 7 (예시)
#define LED1_PORT                GPIOC
#define LED1_PIN                 GPIO_PIN_7

// P2.6 -> GPIOC, Pin 6 (예시)
#define LED2_PORT                GPIOC
#define LED2_PIN                 GPIO_PIN_6

// P3.5 -> GPIOD, Pin 5 (예시)
#define AFE_RESET_PIN_PORT       GPIOD
#define AFE_RESET_PIN_PIN        GPIO_PIN_5


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
