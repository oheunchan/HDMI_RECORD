///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it680X.c>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/



//FIX_ID_001        //Dr. Liu suggestion to enable Auto EQ with Manual EQ to avoid some special HDMI cable issue.
//FIX_ID_002        //Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
//FIX_ID_003        //Add IT6802 Video Output Configure setting
//FIX_ID_004        //Add 100ms calibration for CsigBus
//FIX_ID_005        //Add CsigBus Event Handler
//FIX_ID_006        //Add P2_0 for switch Exteranl 24c04 EEPROM and Internal IT6802 EDID RAM
//FIX_ID_007        //for debug IT6681 HDCP issue7
//FIX_ID_008        //Add SW reset when HDMI / MxL device un-plug  !!!
//FIX_ID_009        //Verify interrupt event with reg51[0] select port
//FIX_ID_010        //Add JudgeBestEQ to avoid wrong EQ setting
//FIX_ID_011        //Use FW send PATH_EN{Sink}=1
//FIX_ID_012        //For SamSung Galaxy Note wake up fail iss ue
//FIX_ID_013        //For MSC 3D request issue
//FIX_ID_014        //For HDCP Auth Start with EQ Adjust issue
//FIX_ID_015        //Add RCP timeout mechanism for peer device no RCPK or RCPE response
//FIX_ID_016        //Support Dual Pixel Mode for IT6803 Only
//FIX_ID_017        //Disable IPLockChk
//FIX_ID_018        //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
//FIX_ID_019        //modify ENHYS control for MxL mode
//FIX_ID_020        //Turn off DEQ for HDMI port 1 with 20m DVI Cable
//FIX_ID_021        //To use CP_100ms calibration for CsigBus and CEC
//FIX_ID_022        //Fixed for CEC capacitor issue
//FIX_ID_023        //Fixed for Audio Channel Status Error with invalid HDMI source
//FIX_ID_024        //Fixed for RCP compliance issue
//FIX_ID_025        //Adjust H/W Mute time
//FIX_ID_026        //Support RB SWAP for TTL output
//FIX_ID_027        //Support RGB limited / Full range convert
//FIX_ID_028        //For Debug Audio error with S2
//FIX_ID_029        //fixed Ulta-2000 HDCP fail issue at Receiver mode
//FIX_ID_030        //fixed video lost at 640x480 timing
//FIX_ID_031        //change CsigBus pull down 1K after 75ms power 5V stable
//FIX_ID_032        //Support HDCP Repeater function for HDMI Tx device
//FIX_ID_033        //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//FIX_ID_034        //Add MxL HPD Control by it6802HPDCtrl( )
//FIX_ID_035        //For MTK6592 HDMI to SII MxL TX compliance issue
//FIX_ID_036        //Enable MxL Function for IT68XX
//FIX_ID_037        //Allion MxL compliance issue !!!
//FIX_ID_038        //Add Reg8B[1] reset for IT6803 Dual Pixel mode
//FIX_ID_039        //fix image flick when enable RGB limited / Full range convert
//FIX_ID_040        //fix MSC 3D request issue for Huawei Mobile and Action Micro Dongle
//FIX_ID_041        //Add EDID reset
//FIX_ID_042        //Disable HDCP 1.1 feature to avoid compilance issue from ilegal HDCP 1.1 source device
//FIX_ID_043        // Limit Auto EQ Polarity Toggle Count by Special Pattern.
//FIX_ID_044        // Add YUV420 support
// omit FIX_ID_045        // 4.3.5.1 change MxL 2A[2:1] = 00 -> 11
//FIX_ID_046        // MxL version changed from 2.0 -> 2.2
//FIX_ID_047        // FIX 3D report string.
//FIX_ID_048        // 4.3.5.1 fail because of HPD not tri-state
//FIX_ID_049        // 4.3.5.1 change MxL 2A[2:1] = 11 -> 00
//FIX_ID_050        // 6.3.11.23 CBM: DUT Receives (0x68) MSC_MSG - Sub-command Illegal
                    // if not process UCP error, need to reply the UCP command ACK.
//FIX_ID_051        // unify CEC timer with MxL timer
//FIX_ID_052        // DIFF THE DCLK Delay under CSC power on and power down.
//FIX_ID_053        // Change AIO Strength for some weak connectivity. default value -> 0xAA
//FIX_ID_054        // seperate EDID into a dedicated file.
//FIX_ID_055        // IT680x_Init_HDMIRepeater add option for enable/disable HDCP repeater
//FIX_ID_056        // add IT680x_Enable_HDCPRepeater() for enable/disable HDCP repeater dynamically.
//FIX_ID_057        // 6.3.11.23 CBM: DUT Receives (0x68) MSC_MSG - Sub-command Illegal
                    // if not process UCP error, need to reply the UCP command ACK.
                    // if not defined _SUPPORT_UCP_,
                    // #define MSCRxUCP2Nack    FALSE
//FIX_ID_058        // MxL_parse_RCPkey() should ignore the "release" status.
//FIX_ID_059        // Fix bug for it6802->prevAVIDB[1] to sync the register value(Reg215), or the interrupt "Reg09h&0x01" may not be triggered correctly

// 2014-0527 MxL compliance issue !!!
// REG_RX_1C0 : 0x80 [7] PWSB_LV =0 , [3:2] 1K resistance
// REG_RX_1BB : 0x0A [4] CsigBus Schimitter trigger =0
//
//
// MxL_RX_01 : 0xD8 [6:4] OSC division = 5 , [3:2]
// MxL_RX_29 : 0x83 [7] Enable Crystall
// MxL_RX_2A : 0x41 [2:1] 5vStableTime2CDsenseGoHigh = 11 150ms
// MxL_RX_2B : 0x1A [2] output HPD when wakeup fail trigger, [0] wkeup fail tigger by FW
//
//
// REG_RX_1C0 = 0x04    //2014-0527 +10% for W1070
// MxL_RX_2B = 0x1A
//
/*****************************************************************************/
/* Header Files Included *****************************************************/
/*****************************************************************************/

#include "it68013CX_it680x.h"
#include "it68013CX_it680X_regs.h"

#ifndef OUTPUT_COLOR_DEPTH
#define OUTPUT_COLOR_DEPTH OUT12B
#endif // OUTPUT_COLOR_DEPTH


/*****************************************************************************/
/* Private and Local Variables    ********************************************/
/*****************************************************************************/
// volatile USHORT ucTickCount;

struct it6802_dev_data it6802DEV;
unsigned long m_ROSCCLK;

unsigned char  V3D_EntryCnt = 0;
unsigned char  wrburstoff, wrburstnum;
unsigned char  TxWrBstSeq = 0;

unsigned char  wakeupcnt = 0;
#define MinEQValue    0x03
unsigned char CLKStbChkCnt = 0;//ite_180509


int gpHPD0 = 0; //oec2026

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_EQ_ADJUST_
    #define MaxEQIndex 3
    unsigned char IT6802EQTable[]={0xFF,0x9F,0x83};

    //for EQ state machine handler
    //#define    MAXSYNCOFF        5
    #define    MAXECCWAIT        (10)
    #define    EQSTATE_WAIT        (20)
    #define    EQSTATE_START        (EQSTATE_WAIT+MAXECCWAIT)
    #define    EQSTATE_LOW        (EQSTATE_WAIT+EQSTATE_START+(MAXECCWAIT*1))
    #define    EQSTATE_MIDDLE    (EQSTATE_WAIT+EQSTATE_START+(MAXECCWAIT*2))
    #define    EQSTATE_HIGH        (EQSTATE_WAIT+EQSTATE_START+(MAXECCWAIT*3))
    #define    EQSTATE_END        (255-100)
    #define    MINECCFAILCOUNT    (MAXECCWAIT/2)
#endif

#ifdef _SUPPORT_AUTO_EQ_
    unsigned char ucPortAMPOverWrite[2];
    unsigned char ucPortAMPValid[2];
    unsigned char ucChannelB[2];    // ch0
    unsigned char ucChannelG[2];    // ch1
    unsigned char ucChannelR[2];    // ch2
    unsigned char ucEQMode[2];
#endif
//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
unsigned char HdmiI2cAddr=IT6802B0_HDMI_ADDR;
//FIX_ID_002 xxxxx

//for debug video format only
int CurTMDSCLK;
VTiming CurVTiming;
AVI_InfoFrame aviinfoframe;
//int GCP_CD       = CD8BIT; //24 bits per pixel
int InColorMode  = RGB444; //RGB444, YCbCr422, YCbCr444
int OutColorMode = RGB444; //RGB444, YCbCr422, YCbCr444
int OutCD        = OUT8B;
static int VIC; //yjh2026
//for debug only


#ifdef _FIX_ID_028_
    //FIX_ID_028 xxxxx //For Debug Audio error with S2
    static unsigned char m_bAudioWaiting=0;
    //FIX_ID_028 xxxxx
#else
    #ifdef EnableCalFs
        //FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
        static unsigned int m_u16TMDSCLK=0;
        static unsigned char m_ForceFsValue=0;
        //static unsigned long m_ROSCCLK;
        static unsigned char m_AudioChannelStatusErrorCount=0;
        #define MAX_AUDIO_CHANNEL_STATUS_ERROR 4
        //FIX_ID_023 xxxxx
    #endif
#endif

// FIX_ID_051 CEC time unit calibration as MxL initial calubration
#ifdef Enable_IT6802_CEC
    extern u8_t timeunit;
#endif
//~FIX_ID_051
/****************************************************************************/
/*                            EDID Argument                                    */
/****************************************************************************/
unsigned char  VSDB_Addr;// for EDID RAM function
unsigned char  txphyadr[2], txphyA, txphyB, txphyC, txphyD, txphylevel;    // for CEC function
unsigned char  rxphyadr[2][2];// for EDID RAM function
unsigned char  rxphyA, rxphyB, rxphyC, rxphyD, rxcurport;    // for CEC function

#ifdef FIX_ID_013
//FIX_ID_013    xxxxx    //For MSC 3D request issue
unsigned char uc3DDtd[]={0x00};
struct PARSE3D_STR    st3DParse;
MxL3D_STATE e3DReqState = MxL3D_REQ_DONE;
unsigned char SVD_LIST[16];
    //unsigned char STRUCTURE_3D[16]={1,0,0,0,0,0,2,0,4,0,0,0,0,0,0,0};
//FIX_ID_013    xxxxx
#endif //FIX_ID_013


/*****************************************************************************/
/* Init, Power, and IO Structures ********************************************/
/*****************************************************************************/


////////////////////////////////////////////////////////////////////
// It6802 chip inital table
////////////////////////////////////////////////////////////////////
_CODE struct IT6802_REG_INI  IT6802_HDMI_INIT_TABLE[] = {
//port 0
    {REG_RX_00F,    0x03,    0x00},    //change bank 0
    {REG_RX_010,    0xFF,    0x08},    //[3]1: Register reset
    {REG_RX_00F,    0x03,    0x00},    //change bank 0
    {REG_RX_034,    0xFF,    MxL_ADDR+0x01},    //I2C Slave Addresss for MxL block

    {REG_RX_010,    0xFF,    0x17},    //[4]Auto Video Reset [2]Int Reset [1]Audio Reset [0]Video Reset

    {REG_RX_011,    0xFF,    0x1F},    //Port 0: [4]EQ Reset [3]CLKD5 Reset [2]CDR Reset [1]HDCP Reset [0]All logic Reset
    {REG_RX_018,    0xFF,    0x1F},    //Port 1: [4]EQ Reset [3]CLKD5 Reset [2]CDR Reset [1]HDCP Reset [0]All logic Reset

    {REG_RX_012,    0xFF,    0xF8},    //Port 0: [7:3] MxL Logic reset

    {REG_RX_010,    0xFF,    0x10},    //[4]Auto Video Reset [2]Int Reset [1]Audio Reset [0]Video Reset

    {REG_RX_011,    0xFF,    0xA0},    //Port 0: [7] Enable Auto Reset when Clock is not stable [5]Enable Auto Reset
    {REG_RX_018,    0xFF,    0xA0},    //Port 1: [7] Enable Auto Reset when Clock is not stable [5]Enable Auto Reset

#ifdef _FORCE_DISABLE_HDCP_
    {REG_RX_00F,    0x03,   0x01},

    {REG_RX_1A8,0xFF,0x00},
    {REG_RX_1A9,0xFF,0x00},

    {REG_RX_00F,    0x03,   0x00},

    {REG_RX_02D,0x04,0x04},
    {REG_RX_045,0x04,0x04},
    {REG_RX_090,0xFF,0x44},
    {REG_RX_011,0x02,0x02},
    {REG_RX_011,0x02,0x00},
    {REG_RX_018,0x02,0x02},
    {REG_RX_018,0x02,0x00},
    {REG_RX_02D,0x04,0x00},
    {REG_RX_045,0x04,0x00},
    {REG_RX_090,0xFF,0x00},

#endif

    {REG_RX_012,    0xFF,    0x00},    //Port 0: [7:3] MxL Logic reset

    {REG_RX_00F,    0x03,    0x01},    //change bank 1    //2013-0430 Andrew suggestion
// FIX_ID_048
//    {REG_RX_1B0,    0x03,    0x01},    // MxL Port Set HPD = 0 at Power On initial state
    {REG_RX_1B0,    0x03,    0x00},    // MxL Port Set HPD tri-state at Power On initial state
//~FIX_ID_048

    {REG_RX_00F,    0x03,    0x00},    //change bank 0    //2013-0430 Andrew suggestion
    {REG_RX_017,    0xC0,    0x80},    //Port 0: [7:6] = 10 invert Port 0 input HCLK , CLKD5I    //2013-0430 Andrew suggestion
    {REG_RX_01E,    0xC0,    0x00},    //Port 1: [7:6] = 00 invert Port 1 input TMDS , CLKD5I    //2013-0430 Andrew suggestion

    #ifdef Enable_IT6802_CEC
        {REG_RX_00E,    0xFF,    0xFF},    //for enable CEC Clock
        {REG_RX_086,    0xFF,    (CEC_ADDR|0x01)},    //CEC chip Slave Adr
    #endif


    {REG_RX_016,    0x08,    0x08},    //Port 0: [3]1: Enable CLKD5 auto power down
    {REG_RX_01D,    0x08,    0x08},    //Port 1: [3]1: Enable CLKD5 auto power down


    {REG_RX_02B,    0xFF,    0x07},    //FixTek3D
    //FIX_ID_042 xxxxx //Disable HDCP 1.1 feature to avoid compilance issue from ilegal HDCP 1.1 source device
        {REG_RX_031,    0xFF,    0x09},    //[7:4]Enable repeater function [3:0] SCL hold time count & Update Ri sel
        {REG_RX_049,    0xFF,    0x09},    //[7:4]Enable repeater function [3:0] SCL hold time count & Update Ri sel
    //FIX_ID_042 xxxxx
//FIX_ID_017 xxxxx Disable IPLockChk
//FIX_ID_001 xxxxx UseIPLock = 0 for avoid clk change
    {REG_RX_035,    0x1E,    (0x10+(DeltaNum<<2))},    //[3:2] RCLKDeltaSel , [1] UseIPLock = 0
    {REG_RX_04B,    0x1E,    (0x10+(DeltaNum<<2))},    //[3:2] RCLKDeltaSel , [1] UseIPLock = 0
//FIX_ID_001 xxxxx
//FIX_ID_017 xxxxx
    {REG_RX_054,    0xFF,    (1<<4)+RCLKFreqSel},    //[1:0]RCLK frequency select
    {REG_RX_06A,    0xFF,    GenPktRecType},            //Decide which kind of packet to be fully recorded on General PKT register
    {REG_RX_074,    0xFF,    0xA0},    //[7]Enable i2s and SPDIFoutput [5]Disable false DE output
    {REG_RX_050,    0x1F,    0x10|DCLK_DELAY_CSC_ON},    //[4]1: Invert output DCLK and DCLK DELAY 2 Step

//    {REG_RX_065,    0x0C,    0x00},    //[3:2]0=8bits Output color depth
//    {REG_RX_065,    0x0C,    0x04},    //[3:2]1=10bits Output color depth

    {REG_RX_065,    0x0C,    (OUTPUT_COLOR_DEPTH<<2)},    //[3:2]2=12bits Output color depth

//2025/02/11 jjtseng
//    {REG_RX_07A,    0x80,    0x80},    //[7]1: enable audio B Frame Swap Interupt
      /*20230105 for Roger & Jaming tuning Audio*/
    {REG_RX_07A,    0x80,    0x00},    //[7]1: enable audio B Frame Swap Interupt
//~2025/02/11 jjtseng

//    {REG_RX_02D,    0x03,    0x03},    //[1:0] 11: Enable HDMI/DVI mode over-write

    {REG_RX_085,    0x02,    0x02},    //[1]1: gating avmute in video detect module

//    {REG_RX_051,    0x80,    0x80},    //[7]1: power down color space conversion logic

#ifdef  _SUPPORT_EDID_RAM_
#define DISABLE_PORT0_EDIDRAM 0
#define DISABLE_PORT1_EDIDRAM 0
    {REG_RX_0C0,    0x43,    0x40|(DISABLE_PORT1_EDIDRAM<<1)|(DISABLE_PORT0_EDIDRAM<<1)},    //[0]1:Reg_P0DisableShadow
    {REG_RX_087,    0xFF,    (EDID_ADDR|0x01)},    //[7:1] EDID RAM Slave Adr ,[0]1: Enable access EDID block
#else
    {REG_RX_0C0,    0x03,    0x03},    //[0]1:Reg_P0DisableShadow
    {REG_RX_087,    0xFF,    (0x00)},    //[7:1] EDID RAM Slave Adr ,[0]1: Enable access EDID block
#endif

    {REG_RX_071,    0x08,    0x00},    //Reg71[3] RegEnPPColMode must clear to 0 for andrew suggestion 2013-0502
//FIX_ID_030 xxxxx fixed video lost at 640x480 timing
    {REG_RX_037,    0xFF,    0xA6},    //Reg37 Reg_P0_WCLKValidNum must set to 0xA6 for andrew suggestion 2014-0403
    {REG_RX_04D,    0xFF,    0xA6},    //Reg4D Reg_P1_WCLKValidNum must set to 0xA6 for andrew suggestion 2014-0403
//FIX_ID_030 xxxxx
    {REG_RX_067,    0x80,    0x00},    //Reg67[7] disable HW CSCSel

    // {REG_RX_07A,B_CTS_RES,B_CTS_RES},
    {REG_RX_07A,0x80+0x30,B_AUD_INC+B_CTS_RES},

//FIX_ID_037 xxxxx //Allion MxL compliance issue debug !!!
//FIX_ID_018 xxxxx    modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
//2014-0526 MxL compliance issue Debug disable ->    {REG_RX_1C0,    0x8C,    0x08},    //[7] PWSB_LV = 0    //2013-0430 Andrew suggestion
// Reg1C0[3:2] = 00 -> 1.08Kohm    0 %
// Reg1C0[3:2] = 01 -> 1.18Kohm    +10 %
// Reg1C0[3:2] = 10 -> 0.98Kohm    -10%
// Reg1C0[3:2] = 11 -> 0.88Kohm    -20%
//FIX_ID_018 xxxxx
#if defined(_IT6802_) || defined(_IT6803_)
    {REG_RX_077, 0x80, 0x00},
    {REG_RX_00F, 0x03, 0x01},    //change bank 1
    {REG_RX_1C0, 0x8C, 0x08},
    {REG_RX_00F, 0x03, 0x00},    //change bank 0
#else
    {REG_RX_077, 0x80, 0x80},     // IT6801 Audio i2s sck and mclk is common pin
    {REG_RX_00F, 0x03, 0x01},    //change bank 1
    {REG_RX_1C0, 0x8C, 0x88},
    {REG_RX_00F, 0x03, 0x00},    //change bank 0
#endif
//FIX_ID_037 xxxxx

    #ifdef _HBR_I2S_
        {REG_RX_07E,B_HBRSel,0x00},
    #else
        {REG_RX_07E,B_HBRSel,B_HBRSel},
    #endif

    {REG_RX_052,(B_DisVAutoMute),(B_DisVAutoMute)},                //Reg52[5] = 1 for disable Auto video MUTE
    {REG_RX_053,(B_VDGatting|B_VIOSel|B_TriVDIO|B_TriSYNC),(B_VIOSel|B_TriVDIO|B_TriSYNC)},                //Reg53[7][5] = 01    // for disable B_VDIO_GATTING

    {REG_RX_058,0xFF,0x33},            // Reg58 for 4Kx2K Video output Driving Strength

    //FIX_ID_053        // Change AIO Strength for some weak connectivity. default value -> 0xAA
    {REG_RX_059,0xFF,0xAA},            // Reg59 for Audio output Driving Strength


    ////////////////////////////////////////////////////////////////
    // EQ Part.
    ////////////////////////////////////////////////////////////////
//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
//!!!  For Manual Adjust EQ only  !!!
#ifdef _SUPPORT_MANUAL_ADJUST_EQ_
    {REG_RX_03E,0x20,0x20},    // Enable OvWrRsCs
    {REG_RX_026,0x20,0x20},    // Enable OvWrRsCs
#endif
//FIX_ID_001 xxxxx
#ifdef _ONLY_SUPPORT_MANUAL_EQ_ADJUST_
    {REG_RX_026,    0xFF,    0x20},    //Reg26=0x00 disable Auto Trigger
    {REG_RX_03E,    0xFF,    0x20},    //Reg3E=0x00 disable Auto Trigger
#endif

//RS initial valie
// 2013/06/06 added by jau-chih.tseng@ite.com.tw
// Dr. Liu said, reg25/reg3D should set as 0x1F for auto EQ start option.
    {REG_RX_025, 0xFF, DEFAULT_EQVALUE},
    {REG_RX_03D, 0xFF, DEFAULT_EQVALUE},
//~jau-chih.tseng@ite.com.tw
    {REG_RX_027, 0xFF, DEFAULT_EQVALUE},    // B ch
    {REG_RX_028, 0xFF, DEFAULT_EQVALUE},    // G
    {REG_RX_029, 0xFF, DEFAULT_EQVALUE},    // R
    {REG_RX_03F, 0xFF, DEFAULT_EQVALUE},
    {REG_RX_040, 0xFF, DEFAULT_EQVALUE},
    {REG_RX_041, 0xFF, DEFAULT_EQVALUE},

    {REG_RX_00F,    0x03,    0x01},    //change bank 1    //2013-0515 Andrew suggestion    for Auto EQ
    {REG_RX_1BC,    0xFF,    0x06},    //Reg1BC=0x06        //2013-0515 Andrew suggestion    for Auto EQ
//FIX_ID_020 xxxxx        //Turn off DEQ for HDMI port 1 with 20m DVI Cable
    {REG_RX_1CC,    0xFF,    0x00},    //Reg1CC=0x00        for TURN OFF DEQ
    {REG_RX_1C6,      0x07,      0x03},    // [2:0]Reg_P1_ENHYS = 03 for default enable filter to gating output
//FIX_ID_020 xxxxx

    {REG_RX_1B5,    0x03,    0x03},    //Reg1B5[1:0]='11'    for fix Korea K706 MxL pattern Generator    //2013-0515 Andrew suggestion
//FIX_ID_019    xxxxx modify ENHYS control for MxL mode
    {REG_RX_1B8,      0x80,      0x00},    // [7] Reg_HWENHYS = 0
    {REG_RX_1B6,      0x07,      0x03},    // [2:0]Reg_P0_ENHYS = 03 for default enable filter to gating output
//FIX_ID_019    xxxxx


//FIX_ID_029    xxxxx fixed Ulta-2000 HDCP fail issue at Receiver mode
    {REG_RX_110,      0xFF,      0x00},
    {REG_RX_111,      0xFF,      0x00},
    {REG_RX_112,      0xFF,      0x00},
    {REG_RX_113,      0xFF,      0x00}, // for receiver, BKSV should be zero on inital
    {REG_RX_128,      0xFF,      0x00},    // Clear KSV LIST
    {REG_RX_129,      0xFF,      0x00},    // Clear KSV LIST
    {REG_RX_12A,      0xFF,      0x00},    // Clear KSV LIST
    {REG_RX_12B,      0xFF,      0x00},    // Clear KSV LIST
    {REG_RX_12C,      0xFF,      0x00},    // Clear KSV LIST
//FIX_ID_029    xxxxx

    {REG_RX_00F,    0x03,    0x00},    //change bank 0    //2013-0515 Andrew suggestion    for Auto EQ

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
//    for Auto EQ
#ifdef _SUPPORT_AUTO_EQ_
//0704 disable ->    {REG_RX_022,    0xFF,    0x38},    //Reg22=0x38        //2013-0515 Andrew suggestion    for Auto EQ
//0704 disable ->    {REG_RX_03A,    0xFF,    0x38},    //Reg3A=0x38        //2013-0515 Andrew suggestion    for Auto EQ
    {REG_RX_022,    0xFF,    0x00},    // 07-16 Reg22=0x30    power down auto EQ
    {REG_RX_03A,    0xFF,    0x00},    // 07-16 Reg3A=0x30    power down auto EQ

#ifdef ENABLE_AUTO_TRIGGER
    {REG_RX_026,    0xFF,    0x80},    //Reg26=0x80    enable Auto Trigger
    {REG_RX_03E,    0xFF,    0x80},    //Reg3E=0x80    enable Auto Trigger
#else
    {REG_RX_026,    0xFF,    0x00},    //Reg26=0x00 disable Auto Trigger
    {REG_RX_03E,    0xFF,    0x00},    //Reg3E=0x00 disable Auto Trigger
#endif

#else
    {REG_RX_022,    0xFF,    0x00},    // 07-16 Reg22=0x30    power down auto EQ
    {REG_RX_03A,    0xFF,    0x00},    // 07-16 Reg3A=0x30    power down auto EQ

    {REG_RX_026,    0xFF,    0x00},    // 07-16 Reg26=0x00 disable Auto Trigger
    {REG_RX_03E,    0xFF,    0x00},    // 07-16 Reg3E=0x00 disable Auto Trigger

#endif
//FIX_ID_001 xxxxx


    {REG_RX_063,0xFF,0x3F},        //for enable interrupt output Pin
    {REG_RX_073, 0x08, 0x00},        // for HDCPIntKey = false

    {REG_RX_060, 0x40, 0x00},        // disable interrupt mask for NoGenPkt_Rcv

//FIX_ID_017 xxxxx Disable IPLockChk
    {REG_RX_02A, 0x01, 0x00},        // disable PORT 0 EnIPLockChk
    {REG_RX_042, 0x01, 0x00},        // disable PORT 1 EnIPLockChk
//FIX_ID_017 xxxxx


#if defined(_IT6803_)
//FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
    {REG_RX_08C, 0x09, 0x09},        // Reg8C[0] = 1    // SPOutMode: //  for enable IO Mapping for Signal Pixel mode
//FIX_ID_016 xxxxx
#endif

//FIX_ID_025 xxxxx Audio lock method select for HDMI Repeater / splitter application
    {REG_RX_077, 0x0C, 0x08},        // Reg77[3:2] = 01    Audio lock method select
//FIX_ID_025 xxxxx
    {0xFF, 0xFF, 0xFF},
};

#define RegPCLKBInv (0)
#define RegDCLKInv  (0)
#define RegPCLKBDly (0)
_CODE struct IT6802_REG_INI  IT6602_HDMI_INIT_OVERRIDE_TABLE[] = {/*20230302*/
    {REG_RX_050,    0xFF,    0x90},    //[4]1: Invert output DCLK and DCLK DELAY 2 Step/*20221221*/

    {REG_RX_00F,    0x03,    0x01},
    {REG_RX_1B4,    0xFF,    0x81},
    {REG_RX_00F,    0x03,    0x00},

    /*20230525*//*for C0 */
    {REG_RX_017, 0x80, 0x00},
    {REG_RX_01E, 0xC0, 0x40}, // 20240318, invert Port 1 input TMDS , CLKD5I
    //{REG_RX_050, 0x20, 0x00},
    //{REG_RX_050, 0x0C, 0x00},
    //{REG_RX_050, 0x10, 0x00},
    {REG_RX_050, 0x3C, (RegPCLKBInv<<5)|(RegDCLKInv<<4)|(RegPCLKBDly<<2)},
    {REG_RX_056, 0x07, 0x06},/*20231212*/
    // {REG_RX_051, 0x80, 0x00},
    {0xFF, 0xFF, 0xFF},
};


//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
////////////////////////////////////////////////////////////////////
//It6802 MxL inital table
//
//
//
////////////////////////////////////////////////////////////////////
_CODE struct IT6802_REG_INI _CODE IT6802_MxL_INIT_TABLE[] = {

    {MxL_RX_0A,    0xFF,    0x00},    //INT MASK 1: disable corresponding interrupt
    {MxL_RX_08,    0xFF,    0x05},    //INT MASK 1: disable corresponding interrupt , disable TX packet done , Rx Packet Done
    {MxL_RX_09,    0xFF,    0x40},    //INT MASK 1: disable corresponding interrupt, disable DDC Rpd done

    {MxL_RX_52,    0xFF,    0x00},    //0: handle by HW
    {MxL_RX_53,    0xFF,    0x80},    //[7]=1 disable FW mode

    {MxL_RX_32,    0xFF,    0x0C},                    //CsigBus arbitration Low time
    {MxL_RX_81,    0xFF,    MxLVersion},            //DCAP00: MxL_VER_MAJOR and MINOR
    {MxL_RX_82,    0xFF,    (PLIM<<5)+(POW<<4)+DEV_TYPE_SINK},    //DCAP02: DEVICE TYPE
    {MxL_RX_83,    0xFF,    ADOPTER_ID_H},        //DCAP03: ADOPTER_ID
    {MxL_RX_84,    0xFF,    ADOPTER_ID_L},
    {MxL_RX_8B,    0xFF,    DEVICE_ID_H},            //DCAP0B: DEVICE_ID
    {MxL_RX_8C,    0xFF,    DEVICE_ID_L},

    {MxL_RX_28,    0x0F,    0x00},    //Port 0: [0] = 0 RS value of CH1 if req_setEQ=1        //2013-0430 Andrew suggestion

    {MxL_RX_0F,    0x20,    0x20},    //[5]Guard band swap when Pack Mode
//FIX_ID_004 xxxxx //Add 100ms calibration for CsigBus
    {MxL_RX_29,    0x83,    (T10usSrcSel<<7)+(0x03)},    //[7] 1: frome 27M crystall ( _SelectExtCrystalForCsigBus_ )
//FIX_ID_004 xxxxx
    {MxL_RX_39,    0x80,    0x80},    //[7] 1: enable I2C deglitch
    {MxL_RX_00,    0x8F,    (EnCsigBusDbgMode<<7)+(MSCCsigBusDbgCtrl<<2)+DDCCsigBusDbgCtrl},    //Enable CsigBus Debug Mode
//FIX_ID_004 xxxxx //Add 100ms calibration for CsigBus
    {MxL_RX_01,    0xFC,    (EnCsigBusDeGlitch<<7)+(Reg100msTOAdj<<2)+0x50},    //CsigBus configure , [6:4] OSCCLK Divide count select = 5 for bit time =1us ( _SelectExtCrystalForCsigBus_ )
//FIX_ID_004 xxxxx
    {MxL_RX_0C,    0x89,    (PPHDCPOpt<<7)+EnHWPathEn+(PPHDCPOpt2<<3)},   //PATH_EN configure
    {MxL_RX_36,    0xFC,    (AckHigh<<4)+(AckLow<<2)},  //ACK configure
    {MxL_RX_38,    0x20,    EnDDCSendAbort<<5},  //DDC bus configure
    {MxL_RX_5C,    0xFC,    (EnPktFIFOBurst<<7)+(MSCBurstWrID<<6)+(MSCBurstWrOpt<<5)+(EnMSCBurstWr<<4)+(EnMSCHwRty<<3)+(MSCRxUCP2Nack<<2)},  //MSC configure
    {MxL_RX_66,    0x03,    RegBurstWrTOSel},  //Brst WrTOSel

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//FIX_ID_031 xxxxx //change CsigBus pull down 1K after 75ms power 5V  stable .
//FIX_ID_049        // 4.3.5.1 change MxL 2A[2:1] = 11 -> 00
// MxL_reg_2A[2:1] is not the cause of 4.3.5.1 fail,
// HPD push low out is the root cause. resume it by FIX_ID_048 implemented.
    {MxL_RX_2A, 0x07, 0x01},        // MxL2A[2:1]=00 for Set Reg5VStableTSel = 75ms,    [0]=1 enable HW rstddfsm
//FIX_ID_031 xxxxx
//FIX_ID_033 xxxxx

    {MxL_RX_0F, 0x10, 0x00},


    {0xFF, 0xFF, 0xFF},

};


#ifdef _SUPPORT_RAP_
//                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
_CODE unsigned char  SuppRAPCode[32] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
                        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};// 1
#endif

#ifdef _SUPPORT_RCP_
_CODE unsigned char  SuppRCPCode[128]= { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 0
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, // 2
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, // 3
                        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, // 4
                        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, // 6
                        0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};// 7
#endif

#endif
//FIX_ID_036    xxxxx

static _CODE unsigned char bCSCMtx_RGB2YUV_ITU601_16_235[]= //yjh2026
{
    0x00,        0x80,        0x00,
    0xB2,0x04,    0x65,0x02,    0xE9,0x00,
    0x93,0x3C,    0x18,0x04,    0x55,0x3F,
    0x49,0x3D,    0x9F,0x3E,    0x18,0x04
};

static _CODE unsigned char bCSCMtx_RGB2YUV_ITU601_0_255[]= //yjh2026
{
    0x10,        0x80,        0x00,
    0x09,0x04,    0x0E,0x02,    0xC9,0x00,
    0x0F,0x3D,    0x84,0x03,    0x6D,0x3F,
    0xAB,0x3D,    0xD1,0x3E,    0x84,0x03
};

static _CODE unsigned char bCSCMtx_RGB2YUV_ITU709_16_235[]= //yjh2026
{
    0x00,        0x80,        0x00,
    0xB8,0x05,    0xB4,0x01,    0x94,0x00,
    0x4A,0x3C,    0x17,0x04,    0x9F,0x3F,
    0xD9,0x3C,    0x10,0x3F,    0x17,0x04
};

static _CODE unsigned char bCSCMtx_RGB2YUV_ITU709_0_255[]= //yjh2026
{
    0x10,        0x80,        0x00,
    0xE4,0x04,    0x77,0x01,    0x7F,0x00,
    0xD0,0x3C,    0x83,0x03,    0xAD,0x3F,
    0x4B,0x3D,    0x32,0x3F,    0x84,0x03
};


static _CODE unsigned char bCSCMtx_YUV2RGB_ITU601_16_235[] = //yjh2026
{
    0x00,        0x00,        0x00,
    0x00,0x08,    0x6B,0x3A,    0x50,0x3D,
    0x00,0x08,    0xF5,0x0A,    0x02,0x00,
    0x00,0x08,    0xFD,0x3F,    0xDA,0x0D
};

static _CODE unsigned char bCSCMtx_YUV2RGB_ITU601_0_255[] = //yjh2026
{
    0x04,        0x00,        0xA7,
    0x4F,0x09,    0x81,0x39,    0xDD,0x3C,
    0x4F,0x09,    0xC4,0x0C,    0x01,0x00,
    0x4F,0x09,    0xFD,0x3F,    0x1F,0x10
};

static _CODE unsigned char bCSCMtx_YUV2RGB_ITU709_16_235[] = //yjh2026
{
    0x00,        0x00,        0x00,
    0x00,0x08,    0x55,0x3C,    0x88,0x3E,
    0x00,0x08,    0x51,0x0C,    0x00,0x00,
    0x00,0x08,    0x00,0x00,    0x84,0x0E
};

static _CODE unsigned char bCSCMtx_YUV2RGB_ITU709_0_255[] = //yjh2026
{
    0x04,        0x00,        0xA7,
    0x4F,0x09,    0xBA,0x3B,    0x4B,0x3E,
    0x4F,0x09,    0x57,0x0E,    0x02,0x00,
    0x4F,0x09,    0xFE,0x3F,    0xE8,0x10
};

//FIX_ID_027 xxxxx Support Full/Limited Range convert
//full 2 limit
static _CODE unsigned char bCSCMtx_RGB_0_255_RGB_16_235[] = //yjh2026
{
    0x10,        0x10,        0x00,
    0xe0,0x06,    0x00,0x00,    0x00,0x00,
    0x00,0x00,    0xe0,0x06,    0x00,0x00,
    0x00,0x00,    0x00,0x00,    0xe0,0x06,


};

//limit 2 full
static _CODE unsigned char bCSCMtx_RGB_16_235_RGB_0_255[] = //yjh2026
{
    0xED,        0xED,        0x00,
    0x50,0x09,    0x00,0x00,    0x00,0x00,
    0x00,0x00,    0x50,0x09,    0x00,0x00,
    0x00,0x00,    0x00,0x00,    0x50,0x09,
};
//FIX_ID_027 xxxxx

#ifdef  _SUPPORT_EDID_RAM_
//FIX_ID_054        // seperate EDID into a dedicated file.
extern unsigned char _CODE Default_Edid_Block[256] ;
#endif

// disable ->static struct IT6802_VIDEO_CONFIGURE_REG _CODE IT6802_OUTPUT_VIDEO_MODE[] =
// disable ->{
// disable ->
// disable ->// { Reg51 , Reg65}
// disable ->//
// disable ->// Reg51 [6] Half PCLK DDR , [5] Half Bus DDR , [2] CCIR656 mode
// disable ->// Reg65 [7] BTA1004Fmt , [6] SyncEmb , [5:4] output color 0x00 RGB, 0x10 YUV422, 0x20 YUV444
// disable ->//
// disable ->// 1. RGB/YUV422/YUV444
// disable ->// 2. SDR/HalfPCLKDDR/HalfBusDDR
// disable ->// 3. EmbSync/SepSync
// disable ->//
// disable ->{0x00,0x00},  //RGB444_SDR=0,
// disable ->{0x00,0x10},  //YUV422_SDR
// disable ->{0x00,0x20},  //YUV444_SDR
// disable ->
// disable ->{0x00,0x50},  //YUV422_SDR_Emb_Sync
// disable ->{0x04,0x50},  //CCIR656_SDR_Emb_Sync
// disable ->{0x04,0x10},  //CCIR656_SDR_Sep_Sync
// disable ->
// disable ->{0x40,0x00},  //RGB444_HalfPCLKDDR
// disable ->{0x40,0x20},  //YUV444_HalfPCLKDDR
// disable ->{0x40,0x50},  //YUV422_HalfPCLKDDR_Emb_Sync
// disable ->{0x40,0x10},  //YUV422_HalfPCLKDDR_Sep_Sync
// disable ->
// disable ->{0x44,0x10},  //CCIR656_HalfPCLKDDR_Sep_Sync
// disable ->
// disable ->{0x20,0x00},  //RGB444_HalfBusDDR
// disable ->{0x20,0x20},  //YUV444_HalfBusDDR
// disable ->
// disable ->{0x00,0xD0},  //BTA1004_SDR
// disable ->{0x40,0xD0},  //BTA1004_DDR
// disable ->};


_CODE char *VStateStr[] = {
    "VSTATE_Off",
    "VSTATE_TerminationOff",
    "VSTATE_TerminationOn",
    "VSTATE_5VOff",
    "VSTATE_SyncWait",
    "VSTATE_SWReset",
    "VSTATE_SyncChecking",
    "VSTATE_HDCPSet",
    "VSTATE_HDCP_Reset",
    "VSTATE_ModeDetecting",
    "VSTATE_VideoOn",
    "VSTATE_ColorDetectReset",
    "VSTATE_HDMI_OFF",
    "VSTATE_Reserved",
    };

_CODE char  *AStateStr[] = {
    "ASTATE_AudioOff",
    "ASTATE_RequestAudio",
    "ASTATE_ResetAudio",
    "ASTATE_WaitForReady",
    "ASTATE_AudioOn",
    "ASTATE_Reserved"
};


_CODE char  *VModeStateStr[] = {
    "0 eRGB444_SDR",
    "1 eYUV444_SDR",
    "2 eRGB444_DDR",
    "3 eYUV444_DDR",
    "4 eYUV422_Emb_Sync_SDR",
    "5 eYUV422_Emb_Sync_DDR",
    "6 eYUV422_Sep_Sync_SDR",
    "7 eYUV422_Sep_Sync_DDR",
    "8 eCCIR656_Emb_Sync_SDR",
    "9 eCCIR656_Emb_Sync_DDR",
    "10 eCCIR656_Sep_Sync_SDR",
    "11 eCCIR656_Sep_Sync_DDR",
    "12 eRGB444_Half_Bus",
    "13 eYUV444_Half_Bus",
    "14 eBTA1004_SDR",
    "15 eBTA1004_DDR",
};


//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
// #define _Debug_MSC_Abort_Only
#ifdef _Debug_MSC_Abort_Only
unsigned char    m_MxLabortID = 0x00;

// default: do nothing
// 1 test retry HPD with MSC abort command
// 2 test retry DCap Ready with MSC abort command
// 3 test retry Read DCap register with MSC abort command
#define MxL_ABORT_ID    (0x00)
#endif
//FIX_ID_037 xxxxx

/*****************************************************************************/
/*  Function Definitions    **************************************************/
/*****************************************************************************/
//#define VideoTimeOutCheck(x) TimeOutCheck(it6802->m_VideoCountingTimer, (x))
//#define AudioTimeOutCheck(x) TimeOutCheck(it6802->m_AudioCountingTimer, (x))
//#define AssignSWResetVirtualTime() { it6802->m_SWResetTimeOut     = GetCurrentVirtualTime(); }
//#define AssignVideoVirtualTime()   { it6802->m_VideoCountingTimer = GetCurrentVirtualTime(); }
//#define AssignAudioVirtualTime()   { it6802->m_AudioCountingTimer = GetCurrentVirtualTime();}
//#define AssignVideoTimerTimeout(TimeOut) {it6802->m_VideoCountingTimer = (TimeOut);}
//#define AssignAudioTimerTimeout(TimeOut) {it6802->m_AudioCountingTimer = (TimeOut);}



/*****************************************************************************/
/*  Function Prototypes    **************************************************/
/*****************************************************************************/

/* IT680x IO Functions   ***********************************************************/

//
//#ifdef MEGAWIN82516
//static void it6802AutoPortSelect(void);
//#endif

/*****************************************************************************/
/*  IO Functions   ***********************************************************/
/*****************************************************************************/
SYS_STATUS EDID_RAM_Write(unsigned char offset,unsigned char byteno,unsigned char *p_data )
{
    unsigned char flag;
    flag = i2c_write_byte(EDID_ADDR, offset, byteno, p_data, HDMI_DEV);
    return !flag;
}

unsigned char EDID_RAM_Read(unsigned char offset)
{
    unsigned char p_data;
    i2c_read_byte(EDID_ADDR, offset,1, &p_data, HDMI_DEV);
    return p_data;
}

//===============================================================================//
unsigned char IT6802VersionRead( unsigned char RegAddr)
{
    unsigned char    FLAG;
    unsigned char mDataIn;
//FIX_ID_002 xxxxx check IT6802 chip version
    FLAG= i2c_read_byte(HdmiI2cAddr, RegAddr, 1, &mDataIn, HDMI_DEV);
//FIX_ID_002 xxxxx
    if(FLAG==0)
    {
    MxLRX_DEBUG_PRINTF(("IT6802 I2C Read ERROR !!!"));
    MxLRX_DEBUG_PRINTF(("=====  Read Reg0x%X=\n",(int) RegAddr));
    }
    return mDataIn;
}


unsigned char hdmirxrd( unsigned char RegAddr)
{
    unsigned char    FLAG;
    unsigned char mDataIn;
//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    FLAG= i2c_read_byte(HdmiI2cAddr, RegAddr, 1, &mDataIn, HDMI_DEV);
//FIX_ID_002 xxxxx
    if(FLAG==0)
    {
    MxLRX_DEBUG_PRINTF(("HDMI Read ERROR !!!"));
    MxLRX_DEBUG_PRINTF(("=====  Read Reg0x%X=\n",(int) RegAddr));
    }
    return mDataIn;
}

unsigned char hdmirxwr( unsigned char RegAddr,unsigned char DataIn)
{

    unsigned char flag;
    // MxLRX_DEBUG_PRINTF(("HDMI_IIC_Write(%#BX,%#BX)\n", RegAddr, DataIn));

//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    flag= i2c_write_byte(HdmiI2cAddr, RegAddr, 1, &DataIn, HDMI_DEV);
//FIX_ID_002 xxxxx
    if(flag==0)
    {
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    MxLRX_DEBUG_PRINTF(("HDMI I2C ERROR !!!"));
    MxLRX_DEBUG_PRINTF(("=====  Write Reg0x%X=%X =====\n",(int)RegAddr,(int)DataIn));
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    }
    return !flag;
}

unsigned char  hdmirxset( unsigned char  offset, unsigned char  mask, unsigned char  ucdata )
{
    unsigned char  temp;
    temp = hdmirxrd(offset);
    temp = (temp&((~mask)&0xFF))+(mask&ucdata);
    return hdmirxwr(offset, temp);
}

void hdmirxbwr( unsigned char offset, unsigned char byteno, unsigned char *rddata )
{
    unsigned char flag;
    if( byteno>0 )
//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    flag= i2c_write_byte(HdmiI2cAddr, offset, byteno, rddata, HDMI_DEV);
//FIX_ID_002 xxxxx
    if(flag==0)
    {
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    MxLRX_DEBUG_PRINTF(("IT6802 I2C ERROR !!!"));
    MxLRX_DEBUG_PRINTF(("=====  Write Reg0x%X=%X =====\n",(int)offset,(int)rddata));
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    }
}

// for access MxL register for calculate clock count.
//===============================================================================//
unsigned char MxLrxrd( unsigned char offset )
{
    unsigned char    mDataIn;
    unsigned char    FLAG;
    FLAG=i2c_read_byte(MxL_ADDR, offset, 1, &mDataIn, HDMI_DEV);
    if(FLAG==0)
    {
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    MxLRX_DEBUG_PRINTF(("MxL I2C ERROR !!!"));
    MxLRX_DEBUG_PRINTF(("=====  read Reg0x%X=%X =====\n",(int)offset,(int)mDataIn));
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    }
    return mDataIn;
}

unsigned char MxLrxwr( unsigned char offset, unsigned char ucdata )
{
    unsigned char  flag;
    flag= i2c_write_byte(MxL_ADDR, offset, 1, &ucdata, HDMI_DEV);
    if(flag==0)
    {
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    MxLRX_DEBUG_PRINTF(("MxL I2C ERROR !!!"));
    MxLRX_DEBUG_PRINTF(("=====  Write Reg0x%X=%X =====\n",(int)offset,(int)ucdata));
    MxLRX_DEBUG_PRINTF(("====================================\n"));
    }
    return !flag;
}

unsigned char MxLrxset( unsigned char offset, unsigned char mask, unsigned char ucdata )
{
    unsigned char temp;
    temp = MxLrxrd(offset);
    temp = (temp&((~mask)&0xFF))+(mask&ucdata);
    return MxLrxwr(offset, temp);
}

void MxLrxbrd( unsigned char offset, unsigned char byteno, unsigned char *rddata )
{
    if( byteno>0 )
    i2c_read_byte(MxL_ADDR, offset, byteno, rddata, HDMI_DEV);
}

void MxLrxbwr( unsigned char offset, unsigned char byteno, unsigned char *rddata )
{
    if( byteno>0 )
    i2c_write_byte(MxL_ADDR, offset, byteno, rddata, HDMI_DEV);
}

/*****************************************************************************/
/* IT680x Configuration and Initialization ***********************************/
/*****************************************************************************/
#ifdef _IT680x_
struct it6802_dev_data* get_it6802_dev_data(void)
{
    return &it6802DEV;
}


void hdimrx_write_init(struct IT6802_REG_INI _CODE *tdata)
{
    int cnt = 0;
    while(tdata[cnt].ucAddr != 0xFF)
    {
        //printf(" Cnt = %d, addr = %02X,andmask = %02X,ucValue=%02X\n", cnt,(int)tdata[cnt].ucAddr,(int)tdata[cnt].andmask,(int)tdata[cnt].ucValue);
        hdmirxset(tdata[cnt].ucAddr,tdata[cnt].andmask,tdata[cnt].ucValue);
        cnt++;

    }
}

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
void MxLrx_write_init(struct IT6802_REG_INI _CODE *tdata)
{
    int cnt = 0;
    while(tdata[cnt].ucAddr != 0xFF)
    {
        //printf(" Cnt = %d, addr = %02X,andmask = %02X,ucValue=%02X\n", cnt,(int)tdata[cnt].ucAddr,(int)tdata[cnt].andmask,(int)tdata[cnt].ucValue);
        MxLrxset(tdata[cnt].ucAddr,tdata[cnt].andmask,tdata[cnt].ucValue);
        cnt++;
    }
}
#endif
//FIX_ID_036    xxxxx

//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure setting
void IT6802_VideoOutputConfigure_Init(struct it6802_dev_data *it6802,Video_Output_Configure eVidOutConfig)
{
    it6802->m_VidOutConfigMode=eVidOutConfig;
    printf("[MindTec] eVidOutConfig: %d\r\n",eVidOutConfig);
    switch(eVidOutConfig)
    {
        case eRGB444_SDR:
            it6802->m_bOutputVideoMode = F_MODE_RGB444;
//FIX_ID_027 xxxxx Support Full/Limited Range convert
            it6802->m_bOutputVideoMode = F_MODE_RGB444|F_MODE_0_255;      // IT680X output RGB Full Range
            // it6802->m_bOutputVideoMode = F_MODE_RGB444|F_MODE_16_235;    // IT680X output RGB limited Range
//FIX_ID_027 xxxxx
            it6802->m_VidOutDataTrgger=eSDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eYUV444_SDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV444;
            it6802->m_VidOutDataTrgger=eSDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eRGB444_DDR:
            it6802->m_bOutputVideoMode=F_MODE_RGB444;
            it6802->m_VidOutDataTrgger=eHalfPCLKDDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eYUV444_DDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV444;
            it6802->m_VidOutDataTrgger=eHalfPCLKDDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eYUV422_Emb_Sync_SDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eSDR;
            it6802->m_VidOutSyncMode=eEmbSync;
            break;

        case eYUV422_Emb_Sync_DDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eHalfPCLKDDR;
            it6802->m_VidOutSyncMode=eEmbSync;
            break;

        case eYUV422_Sep_Sync_SDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eSDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eYUV422_Sep_Sync_DDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eHalfPCLKDDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eCCIR656_Emb_Sync_SDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eSDR;
            it6802->m_VidOutSyncMode=eCCIR656EmbSync;
            break;

        case eCCIR656_Emb_Sync_DDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eHalfPCLKDDR;
            it6802->m_VidOutSyncMode=eCCIR656EmbSync;
            break;

        case eCCIR656_Sep_Sync_SDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eSDR;
            it6802->m_VidOutSyncMode=eCCIR656SepSync;
            break;

        case eCCIR656_Sep_Sync_DDR:
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eHalfPCLKDDR;
            it6802->m_VidOutSyncMode=eCCIR656SepSync;
            break;

        case eRGB444_Half_Bus:
            it6802->m_bOutputVideoMode=F_MODE_RGB444;
            it6802->m_VidOutDataTrgger=eHalfBusDDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eYUV444_Half_Bus:
            it6802->m_bOutputVideoMode=F_MODE_YUV444;
            it6802->m_VidOutDataTrgger=eHalfBusDDR;
            it6802->m_VidOutSyncMode=eSepSync;
            break;

        case eBTA1004_SDR:    //BTA1004_SDR_Emb_Sync
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eSDR_BTA1004;
            it6802->m_VidOutSyncMode=eEmbSync;
            break;

        case eBTA1004_DDR:  //BTA1004_DDR_Emb_Sync
            it6802->m_bOutputVideoMode=F_MODE_YUV422;
            it6802->m_VidOutDataTrgger=eDDR_BTA1004;        // eHalfPCLKDDR
            it6802->m_VidOutSyncMode=eEmbSync;
            break;

    }

    // only for debug use !!!
    //IT6802_VideoOutputModeSet(it6802);

}
//FIX_ID_003 xxxxx

////////////////////////////////////////////////////////////////////
//int hdmirx_Var_init( void )
//
//
//
////////////////////////////////////////////////////////////////////
void hdmirx_Var_init(struct it6802_dev_data *it6802)
{

    // it6802->m_ucCurrentHDMIPort=0xFF;
    //it6802->m_ucDVISCDToffCNT=0;
    it6802->m_ucSCDTOffCount=0;
    it6802->m_ucEccCount_P0=0;
    it6802->m_ucEccCount_P1=0;
    it6802->m_ucDeskew_P0=0;
    it6802->m_ucDeskew_P1=0;

    it6802->m_SWResetTimeOut=0;
    it6802->m_VideoCountingTimer=0;
    it6802->m_AudioCountingTimer=0;

    it6802->m_bVideoOnCountFlag=FALSE;

    it6802->m_MuteAutoOff=FALSE;
    it6802->m_bUpHDMIMode=FALSE;
    it6802->m_bUpHDCPMode=FALSE;
    it6802->m_NewAVIInfoFrameF=FALSE;
    it6802->m_NewAUDInfoFrameF=FALSE;
    it6802->m_HDCPRepeater=FALSE;

//06-27 disable -->     it6802->m_bOutputVideoMode=HDMIRX_OUTPUT_VID_MODE;

//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure Table
    IT6802_VideoOutputConfigure_Init(it6802,HDMIRX_OUTPUT_VID_MODE);
//FIX_ID_003 xxxxx


    it6802->m_bRxAVmute=FALSE;

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    #ifdef _SUPPORT_EQ_ADJUST_
    it6802->EQPort[F_PORT_SEL_0].ucEQState=0xFF;
    it6802->EQPort[F_PORT_SEL_0].ucAuthR0=0;
    it6802->EQPort[F_PORT_SEL_0].ucECCvalue=0;
    it6802->EQPort[F_PORT_SEL_0].ucECCfailCount=0;
    it6802->EQPort[F_PORT_SEL_0].ucPkt_Err=0;    //Pkt_Err
    it6802->EQPort[F_PORT_SEL_0].ucPortID=F_PORT_SEL_0;

    it6802->EQPort[F_PORT_SEL_1].ucEQState=0xFF;
    it6802->EQPort[F_PORT_SEL_1].ucAuthR0=0;
    it6802->EQPort[F_PORT_SEL_1].ucECCvalue=0;
    it6802->EQPort[F_PORT_SEL_1].ucECCfailCount=0;
    it6802->EQPort[F_PORT_SEL_1].ucPkt_Err=0;
    it6802->EQPort[F_PORT_SEL_1].ucPortID=F_PORT_SEL_1;

    it6802->EQPort[F_PORT_SEL_0].f_manualEQadjust=FALSE;
    it6802->EQPort[F_PORT_SEL_1].f_manualEQadjust=FALSE;
    // FIX_ID_043
    it6802->ToggleCount[F_PORT_SEL_0]=0;
    it6802->ToggleCount[F_PORT_SEL_1]=0;
    //~FIX_ID_043

    #endif

    #ifdef _SUPPORT_AUTO_EQ_

    ucPortAMPOverWrite[1]=0;    //2013-0801
    ucPortAMPValid[1]=0;
    ucChannelB[1]=0;
    ucChannelG[1]=0;
    ucChannelR[1]=0;

    ucPortAMPOverWrite[0]=0;    //2013-0801
    ucPortAMPValid[0]=0;
    ucChannelB[0]=0;
    ucChannelG[0]=0;
    ucChannelR[0]=0;
    #endif
//FIX_ID_001 xxxxx

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    it6802->CsigBusIntEvent=0;
    it6802->CsigBusSeqNo=0;
    it6802->CsigBusWaitNo=0x00;
    it6802->CsigBusDcapReadNo=0x00; //20150608_FIX_CHROMA
//FIX_ID_005 xxxxx

//ITE_150302  Chroma A222908 issue
    it6802->CsigBus_wait_clk_mode  = FALSE;
    it6802->CsigBus_clockmode_timeout = CsigBus_CLK_MODE_TIMEOUT;
//

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    it6802->HDMIIntEvent=0;
    it6802->HDMIWaitNo[0]=0;
    it6802->HDMIWaitNo[1]=0;
//FIX_ID_005 xxxxx


    #ifdef _IT6607_GeNPacket_Usage_
    it6802->m_PollingPacket=0;
    it6802->m_PacketState=0;
    it6802->m_ACPState=0;
    it6802->m_GamutPacketRequest=FALSE;
    it6802->m_GeneralRecPackType=0x00;
    #endif
    it6802->m_ucCurrentHDMIPort = 0xff;

//FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
    it6802->m_DiscoveryDone = 0;
//FIX_ID_034 xxxxx

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
//xxxxx 2014-0529 //Manual Content On/Off
   it6802->m_RAP_ContentOff = 0;
   it6802->m_HDCP_ContentOff = 0;
//xxxxx 2014-0529
//FIX_ID_037 xxxxx
    it6802->m_VState=VSTATE_Unknown;
    it6802->m_AState=ASTATE_Unknown;
    it6802->m_RxHDCPState=RxHDCP_PwrOff;
    #ifndef _SUPPORT_HDMI_REPEATER_
    it6802->m_bHDCPrepeater=FALSE ;
    #endif

    it6802->prevAVIDB[0]=0xFF;
    it6802->prevAVIDB[1]=0xFF;

}



////////////////////////////////////////////////////////////////////
//void hdmitx_rst( void )
//
//\
//
////////////////////////////////////////////////////////////////////
void IT6802_Rst( struct it6802_dev_data *it6802 )
{
    hdmirx_Var_init(it6802);
    hdimrx_write_init(IT6802_HDMI_INIT_TABLE);
    if(it6802->device_version == 0xC0){
        hdimrx_write_init(IT6602_HDMI_INIT_OVERRIDE_TABLE);
    }

    //FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
    #if defined(_IT6803_)
    IT6803OutputPixelModeSet(TRUE);    // true for Signal Pixel mode
    #endif
    //FIX_ID_016 xxxxx


//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
    RCPinitQ(it6802);
    MxLrx_write_init(IT6802_MxL_INIT_TABLE);
#endif
//FIX_ID_036    xxxxx

    Cal_oclk();

#ifdef _SUPPORT_HDMI_REPEATER_
    IT6802SwitchVideoState(it6802, VSTATE_Off) ;
#else
    IT6802SwitchVideoState(it6802, VSTATE_SWReset) ;
#endif
    IT6802SwitchAudioState(it6802, ASTATE_AudioOff) ;

}

//=========================================================================//
void IT6802_Identify_Chip( struct it6802_dev_data *it6802 )/*20230302*/
{

//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
#if 1
//FIX_ID_002 xxxxx

    unsigned char i,j;
    unsigned char VenID;
    unsigned char Result=0;
    unsigned char acIT6802A0Version[4]={0x54,0x49,0x02,0x68};
    unsigned char I2cAddr[2]={IT6802B0_HDMI_ADDR,IT6802A0_HDMI_ADDR};    // it6802A0 i2c addr = 0x94 , but it6802B0 i2c addr = 0x90 !!!

    do
    {
        for(j=0;j<2;j++)
        {
            HdmiI2cAddr    =I2cAddr[j];

            for(i=0;i<4;i++)
            {
                VenID = hdmirxrd(i);
                MxLRX_DEBUG_PRINTF(("VenID[%X]= %X !!!\n",(int)i,(int)VenID));
                if(VenID==acIT6802A0Version[i])
                {
                    Result=1;
                }
                else
                {
                    Result=0;
                    MxLRX_DEBUG_PRINTF(("I2C Addr %X Error: Can not find IT6802 Chip !!!\n",(int)HdmiI2cAddr));
                    break;
                }
            }
            if(Result==1)
            {
                MxLRX_DEBUG_PRINTF(("OK , Find IT6802 Chip I2C Addr %X !!!\n",(int)HdmiI2cAddr));
                it6802->device_version = hdmirxrd(0x04);/*20230525*//*check revision ID*/
                break;
            }
        }
    }
    while(Result==0);
#endif
}

//FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
#if defined(_IT6802_) || defined(_IT6803_)
void IT6803OutputPixelModeSet(unsigned char bSignalPixelMode)
{
    chgbank(0);
    if(bSignalPixelMode)

    {
        HDMIRX_VIDEO_PRINTF(("IT6803OutputPixelModeSet( ) 111111111111 Signal Pixel Mode\n"));
        // 1 .    Signal Pixel Mode
        // Reg0D[3] = 0    // EnPHFClk: 0 for disable PHFCLK for signal pixel mode
        // Reg8C[0] = 1    // SPOutMode: //  for enable IO Mapping for Signal Pixel mode
        // Reg8C[3] = 1    // VDIO3en://  for enable QA IO
        // Reg8B[0] = 0    // EnDualPixel: 0 for disable Dual Pixel mode
        // Reg8B[7] = 1    // PWDDownDualData: //  for gating dual pix data output
        hdmirxset(REG_RX_00D, 0x08, 0x00);
        hdmirxset(REG_RX_08C, 0x09, 0x09);
        hdmirxset(REG_RX_08B, 0x81, 0x80);

    }
    else
    {
//FIX_ID_038 xxxxx //Add Reg8B[1] reset for IT6803 Dual Pixel mode
        hdmirxset(REG_RX_08B, 0x02, 0x02);    //Add Reg8B[1] reset for IT6803 Dual Pixel mode
        HDMIRX_VIDEO_PRINTF(("IT6803OutputPixelModeSet( ) 00000000000000 Dual Pixel Mode\n"));
        // 2.    Dual Pixel Mode
        // Reg0D[3] = 1    // EnPHFClk: //  for enable PHFCLK for Dual pixel mode
        // Reg8C[0] = 0    // SPOutMode: 0 for Disable IO Mapping for Dual Pixel mode
        // Reg8C[3] = 1    // VDIO3en: //  for enable QA IO
        // Reg8B[0] = 1    // EnDualPixel: //  for enable Dual Pixel mode
        // Reg8B[7] = 0    // PWDDownDualData: 0 for PWD UP DUAL DATA
        hdmirxset(REG_RX_00D, 0x08, 0x08);
        hdmirxset(REG_RX_08C, 0x09, 0x08);
        hdmirxset(REG_RX_08B, 0x81, 0x01);
        hdmirxset(REG_RX_08B, 0x02, 0x00);    //Add Reg8B[1] reset for IT6803 Dual Pixel mode
//FIX_ID_038 xxxxx
    }
}

#if defined(_IT6803_)

void IT6803JudgeOutputMode(void)
{
    unsigned char rddata;
    int PCLK;    //, sump;


    rddata = hdmirxrd(0x9A);
    PCLK=(RCLKVALUE*255/rddata);

    HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) PCLK = %d\n",(int) PCLK));

    if(PCLK>160)
    {
        #if defined(_IT6803_)
        IT6803OutputPixelModeSet(FALSE);    // FALSE output Dual pixel mode
        #endif
    }
    else
    {
        #if defined(_IT6803_)
        IT6803OutputPixelModeSet(TRUE);        // TRUE output signal pixel mode
        #endif
    }
}
#endif // defined(_IT6803_)


#endif
//FIX_ID_016 xxxxx


//=========================================================================//
void IT6802_fsm_init(void)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    IT680X_DEBUG_PRINTF(("IT6802_fsm_init( )\n"));

//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    IT6802_Identify_Chip(it6802data);
//FIX_ID_002 xxxxx

    IT6802_Rst(it6802data);

#ifdef    _SUPPORT_HDMI_REPEATER_    //20090916
    IT680x_Init_HDMIRepeater(it6802data, TRUE);
#endif //_SUPPORT_HDCP_REPEATER_


#ifdef Enable_IT6802_CEC
    IT6802_CECInit();
    DumpIT6802_CECReg();
#endif


//FIX_ID_006 xxxxx    //Add P2_0 for switch Exteranl 24c04 EEPROM and Internal IT6802 EDID RAM
#ifndef MEGAWIN82516
    // P2_0=1;
    // if(P2_0==0)
    int test=0; //yjh2026 1이면 EDID RAM 꺼짐, 0이어야 내부 EDID 로드됨
    if(test)
    {
        #ifdef FIX_ID_013_
        //printf("!!!Use External EEPROM 24c04 EDID !!!\n");
        st3DParse.bVSDBspport3D = 0;
        st3DParse.ucVicCnt = 0;
        st3DParse.ucDtdCnt = 0;
        #endif

        // for Disable EDID RAM function !!!
        hdmirxset(REG_RX_0C0, 0x03, 0x03);
//        hdmirxset(REG_RX_087, 0xFF, 0x00);
    }
    else
#endif
    {
        #ifdef  _SUPPORT_EDID_RAM_
            //printf("!!!Use it6802 EDID RAM !!!");

            EDIDRAMInitial(&Default_Edid_Block[0]);

            #ifdef FIX_ID_013_
            //FIX_ID_013    xxxxx    //For MSC 3D request issue
                EDID_ParseVSDB_3Dblock(&st3DParse);
            //FIX_ID_013    xxxxx
            #endif //FIX_ID_013

            //FIX_ID_041 xxxxx Add EDID reset
            // fo IT6803 EDID fail issue
            hdmirxset(REG_RX_0C0, 0x20, 0x20);    //xxxxx 2014-0731 [5] 1 for  reset edid
            delay1ms(1);
            hdmirxset(REG_RX_0C0, 0x20, 0x00);
            // fo IT6803 EDID fail issue
            //FIX_ID_041 xxxxx
        #else

            st3DParse.bVSDBspport3D = 0;
            st3DParse.ucVicCnt = 0;
            st3DParse.ucDtdCnt = 0;

            // for Disable EDID RAM function !!!
            hdmirxset(REG_RX_0C0, 0x03, 0x03);
//            hdmirxset(REG_RX_087, 0xFF, 0x00);

        #endif
    }

    it6802PortSelect(0);    // select port 0
}

#endif


/*****************************************************************************/
/* HDMIRX functions    *******************************************************/
/*****************************************************************************/
#ifdef _IT680x_

void chgbank( int bank )
{
    hdmirxset(0x0F, 0x03, bank&3);
}


unsigned char CheckSCDT(struct it6802_dev_data *it6802)
{
    unsigned char ucPortSel;
    unsigned char sys_state_P0;

    ucPortSel = hdmirxrd(REG_RX_051) & B_PORT_SEL;
    sys_state_P0=hdmirxrd(REG_RX_P0_SYS_STATUS);

    if(ucPortSel == it6802->m_ucCurrentHDMIPort) {

        if(sys_state_P0 & B_P0_SCDT)
        {
            //SCDT on
            //it6802->m_ucSCDTOffCount=0;
            return TRUE;
        }
        else
        {
            //SCDT off
            return FALSE;
        }

    }
    return FALSE;
}


void WaitingForSCDT(struct it6802_dev_data *it6802)
{
    unsigned char sys_state_P0;
    unsigned char sys_state_P1;
    unsigned char ucPortSel;
//    unsigned char ucTMDSClk;

    sys_state_P0=hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_SCDT|B_P0_PWR5V_DET|B_P0_RXCK_VALID);
    sys_state_P1=hdmirxrd(REG_RX_P1_SYS_STATUS) & (B_P1_SCDT|B_P1_PWR5V_DET|B_P1_RXCK_VALID);
    ucPortSel = hdmirxrd(REG_RX_051) & B_PORT_SEL;

    //ite_180509
    if(ucPortSel == 0)
    {
        if((hdmirxrd(0x0A)&0x40) == 0x00)//non MxL for P0
        {
            IPLLOvw_chk(ucPortSel);
        }
    }
    else
    {
        IPLLOvw_chk(ucPortSel);
    }
    //ite_180509
    if(sys_state_P0 & B_P0_SCDT)
    {
        IT6802SwitchVideoState(it6802,VSTATE_SyncChecking);    //2013-0520
        return;
    }
    else
    {
        #ifdef _SUPPORT_EQ_ADJUST_
        if(it6802->EQPort[ucPortSel].f_manualEQadjust==TRUE)        // ignore SCDT off when manual EQ adjust !!!
        {
            return;
        }
        #endif

        if(ucPortSel == F_PORT_SEL_0)
        {

            if((sys_state_P0 & (B_P0_PWR5V_DET|B_P0_RXCK_VALID)) == (B_P0_PWR5V_DET|B_P0_RXCK_VALID))
            {
                it6802->m_ucSCDTOffCount++;
                    EQ_PORT0_PRINTF((" SCDT off count = %X\n",(int)it6802->m_ucSCDTOffCount));
                    EQ_PORT0_PRINTF((" sys_state_P0 = %X\n",(int)hdmirxrd(REG_RX_P0_SYS_STATUS)));

            }
        }
        else
        {
            if((sys_state_P1 & (B_P1_PWR5V_DET|B_P1_RXCK_VALID)) == (B_P1_PWR5V_DET|B_P1_RXCK_VALID))
            {
                it6802->m_ucSCDTOffCount++;
                    EQ_PORT1_PRINTF((" SCDT off count = %X\n",(int)it6802->m_ucSCDTOffCount));
                    EQ_PORT1_PRINTF((" sys_state_P1 = %X\n",(int)hdmirxrd(REG_RX_P1_SYS_STATUS)));

            }
        }

        if((it6802->m_ucSCDTOffCount)>SCDT_OFF_TIMEOUT)
        {
            it6802->m_ucSCDTOffCount=0;


//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//FIX_ID_035    xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
                    EQ_PORT0_PRINTF((" WaitingForSCDT( ) CDR reset !!!\n"));
                    hdmirx_ECCTimingOut(ucPortSel);

                     #ifdef _SUPPORT_AUTO_EQ_
                     //xxxxx
                     DisableOverWriteRS(ucPortSel);
                     TMDSCheck(ucPortSel);
                     //xxxxx
                     #endif

//FIX_ID_035    xxxxx
//FIX_ID_033 xxxxx

        }

    }
}

unsigned char CLKCheck(unsigned char ucPortSel)
{
    unsigned char sys_state;
    if(ucPortSel == F_PORT_SEL_1)
    {
    sys_state = hdmirxrd(REG_RX_P1_SYS_STATUS) & (B_P1_RXCK_VALID);
    }
    else
    {
    sys_state = hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_RXCK_VALID);
    }
    if(sys_state == B_P0_RXCK_VALID)
        return TRUE;
    else
        return FALSE;
}


//---------------------------------------------------------------------------------------------------
//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_AUTO_EQ_
void DisableOverWriteRS (unsigned char ucPortSel)
{

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
    unsigned char uc;
#endif
//FIX_ID_036    xxxxx

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    if(ucPortSel == F_PORT_SEL_1)
    {
#ifdef _SUPPORT_AUTO_EQ_
        ucPortAMPOverWrite[F_PORT_SEL_1]=0;    //2013-0801
        ucPortAMPValid[F_PORT_SEL_1]=0;
//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
//xxxxx 2014-0508 disable ->        ucEqRetryCnt[F_PORT_SEL_1]=0;
//FIX_ID_035 xxxxx
        ucEQMode[F_PORT_SEL_1] = 0; // 0 for Auto Mode
        hdmirxset(REG_RX_03A, 0xFF, 0x00);    // 07-16 Reg3A=0x30    power down auto EQ
        hdmirxset(REG_RX_03E,0x20,0x00);        //Manually set RS Value
        MxLRX_DEBUG_PRINTF((" ############# DisableOverWriteRS( ) port 1 ###############\n"));
#endif
        #ifdef _SUPPORT_EQ_ADJUST_
        it6802data->EQPort[F_PORT_SEL_1].f_manualEQadjust=FALSE;
        it6802data->EQPort[F_PORT_SEL_1].ucEQState=0xFF;
        #endif
        it6802data->m_ucDeskew_P1=0;
        it6802data->m_ucEccCount_P1=0;


        //FIX_ID_014 xxxxx
            it6802data->HDMIIntEvent &=0x0F;;
            it6802data->HDMIWaitNo[F_PORT_SEL_1]=0;
        //FIX_ID_014 xxxxx

    }
    else
    {
#ifdef _SUPPORT_AUTO_EQ_
        ucPortAMPOverWrite[F_PORT_SEL_0]=0;    //2013-0801
        ucPortAMPValid[F_PORT_SEL_0]=0;
//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
//xxxxx 2014-0508 disable ->        ucEqRetryCnt[F_PORT_SEL_0]=0;
//FIX_ID_035 xxxxx
        ucEQMode[F_PORT_SEL_0] = 0; // 0 for Auto Mode
        hdmirxset(REG_RX_022, 0xFF, 0x00);    // 07-16 Reg22=0x30    power down auto EQ
        hdmirxset(REG_RX_026,0x20,0x00);        //Manually set RS Value
        MxLRX_DEBUG_PRINTF((" ############# DisableOverWriteRS( ) port 0 ###############\n"));
#endif

        #ifdef _SUPPORT_EQ_ADJUST_
        it6802data->EQPort[F_PORT_SEL_0].f_manualEQadjust=FALSE;
        it6802data->EQPort[F_PORT_SEL_0].ucEQState=0xFF;
        #endif
        it6802data->m_ucDeskew_P0=0;
        it6802data->m_ucEccCount_P0=0;

        //FIX_ID_014 xxxxx
            it6802data->HDMIIntEvent &=0xF0;;
            it6802data->HDMIWaitNo[F_PORT_SEL_0]=0;
        //FIX_ID_014 xxxxx

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_

        wakeupcnt = 0;    //07-23

        //FIX_ID_005 xxxxx    //Add CsigBus Event Handler
            it6802data->CsigBusIntEvent=0;
            it6802data->CsigBusSeqNo=0;
            it6802data->CsigBusWaitNo=0x00;
            it6802data->CsigBusDcapReadNo=0x00;//20150608_FIX_CHROMA
        //FIX_ID_005 xxxxx

        //FIX_ID_019    xxxxx modify ENHYS control for MxL mode
        chgbank(1);
        hdmirxset(REG_RX_1B8,0x80,0x00);    // [7] Reg_HWENHYS = 0
        hdmirxset(REG_RX_1B6,0x07,0x03);    // [2:0]Reg_P0_ENHYS = 03  [2:0]Reg_P0_ENHYS = 03 for default enable filter to gating output
        chgbank(0);
        uc = MxLrxrd(0x05);
        MxLrxwr(0x05,uc);
        //FIX_ID_019    xxxxx

        #if 1
        //FIX_ID_024 xxxxx    //Fixed for RCP compliance issue
            it6802data->m_bRCPTimeOut = FALSE;
            it6802data->m_bRCPError = FALSE;
        //FIX_ID_024 xxxxx
        #endif

        //FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
        it6802data->m_DiscoveryDone = 0;
        //FIX_ID_034 xxxxx

        //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
        MxLrxset(MxL_RX_2B, 0x04,0x00);    // MxL2B[2] 0 for disable HW wake up fail machanism
        #ifdef _Debug_MSC_Abort_Only
        m_MxLabortID = 0;    // test MSC Abort command only !!!
        #endif
        chgbank(1);
        hdmirxset(REG_RX_1C0, 0x8C, 0x08);
        MxLRX_DEBUG_PRINTF(("Reset 1k pull down to -10 percent\n"));
        chgbank(0);
        //xxxxx 2014-0529 //Manual Content On/Off
        it6802data->m_RAP_ContentOff = 0;
        it6802data->m_HDCP_ContentOff = 0;
        //xxxxx 2014-0529
        //FIX_ID_037 xxxxx
#endif
//FIX_ID_036    xxxxx

    }

}

void AmpValidCheck (unsigned char ucPortSel)
{

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

#ifdef _SUPPORT_AUTO_EQ_
    unsigned char uc;

    if(ucPortSel == F_PORT_SEL_1)
    {
        chgbank(1);
        uc = hdmirxrd(REG_RX_1D8);
        EQ_PORT1_PRINTF((" ############# AmpValidCheck( ) port 1 ###############\n"));
        EQ_PORT1_PRINTF((" ############# Reg1D8 = %X ###############\n",(int)uc));
        EQ_PORT1_PRINTF((" ############# Reg1DC = %X ###############\n",(int)hdmirxrd(REG_RX_1DC)));


        if((uc&0x03)==0x03)
        {
            ucChannelB[1] = hdmirxrd(REG_RX_1DD);
            ucPortAMPValid[1]|=0x03;
            EQ_PORT1_PRINTF((" ############# B AMP VALID port 1 Reg1DD = 0x%X  ###############\n",(int)ucChannelB[1]));
        }

        if((uc&0x0C)==0x0C)
        {
            ucChannelG[1]= hdmirxrd(REG_RX_1DE);
            ucPortAMPValid[1]|=0x0C;
            EQ_PORT1_PRINTF((" ############# G AMP VALID port 1 Reg1DD = 0x%X  ###############\n",(int)ucChannelG[1]));
        }

        if((uc&0x30)==0x30)
        {
            ucChannelR[1]= hdmirxrd(REG_RX_1DF);
            ucPortAMPValid[1]|=0x30;
            EQ_PORT1_PRINTF((" ############# R AMP VALID port 1 Reg1DD = 0x%X  ###############\n",(int)ucChannelR[1]));
        }
        chgbank(0);

//FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
        if((ucPortAMPValid[1]&0x3F)==0x3F)
        {
            OverWriteAmpValue2EQ(F_PORT_SEL_1);

            //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            #ifdef _SUPPORT_EQ_ADJUST_
            HDMIStartEQDetect(&(it6802data->EQPort[F_PORT_SEL_1]));
            #endif
            //FIX_ID_001 xxxxx
        }
//FIX_ID_010 xxxxx

    }
    else
    {
        chgbank(1);
        uc = hdmirxrd(REG_RX_1D0);
        ucPortAMPValid[0] = 0;/*20221228*/
        EQ_PORT0_PRINTF((" ############# AmpValidCheck( ) port 0 ###############\n"));
        EQ_PORT0_PRINTF((" ############# REG_RX_1D0 = %X ###############\n",(int) uc));
        EQ_PORT0_PRINTF((" ############# Reg1D4 = %X ###############\n",(int) hdmirxrd(REG_RX_1D4)));
        EQ_PORT0_PRINTF((" ############# ucPortAMPValid[0] = %X ###############\n",(int) ucPortAMPValid[0]));/*20221228*/
        if((uc&0x03)==0x03)
        {
            ucChannelB[0] = hdmirxrd(REG_RX_1D5);
            ucPortAMPValid[0]|=0x03;
            EQ_PORT0_PRINTF((" ############# B AMP VALID port 0 Reg1D5 = 0x%X  ###############\n",(int) ucChannelB[0]));
        }

        if((uc&0x0C)==0x0C)
        {
            ucChannelG[0]= hdmirxrd(REG_RX_1D6);
            ucPortAMPValid[0]|=0x0C;
            EQ_PORT0_PRINTF((" ############# G AMP VALID port 0 Reg1D6 = 0x%X  ###############\n",(int) ucChannelG[0]));
        }

        if((uc&0x30)==0x30)
        {
            ucChannelR[0]= hdmirxrd(REG_RX_1D7);
            ucPortAMPValid[0]|=0x30;
            EQ_PORT0_PRINTF((" ############# R AMP VALID port 0 Reg1D7 = 0x%X  ###############\n",(int) ucChannelR[0]));
        }
        chgbank(0);

    //07-08
    if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
    {
        if((ucPortAMPValid[0]&0x03)==0x03)
        {

        //FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
            OverWriteAmpValue2EQ(F_PORT_SEL_0);
        //FIX_ID_010 xxxxx

            //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            #ifdef _SUPPORT_EQ_ADJUST_
            HDMIStartEQDetect(&(it6802data->EQPort[F_PORT_SEL_0]));
            #endif
            //FIX_ID_001 xxxxx


        }

    }
    else
    {
        if((ucPortAMPValid[0]&0x3F)==0x3F)
        {
            EQ_PORT0_PRINTF((" ############# ucPortAMPValid[0]&0x3F)==0x3F  ############# \n"));/*20221228*/
        //FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
            OverWriteAmpValue2EQ(F_PORT_SEL_0);
        //FIX_ID_010 xxxxx

            //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            #ifdef _SUPPORT_EQ_ADJUST_
            HDMIStartEQDetect(&(it6802data->EQPort[F_PORT_SEL_0]));
            #endif
            //FIX_ID_001 xxxxx

        }
    }

    }
#endif
}

void TogglePolarity (unsigned char ucPortSel)
{
#ifdef _SUPPORT_AUTO_EQ_


    //xxxxx only for IT6802A0 Version
    unsigned char ucPortSelCurrent;
    ucPortSelCurrent = hdmirxrd(REG_RX_051)&B_PORT_SEL;

    #ifdef _ONLY_SUPPORT_MANUAL_EQ_ADJUST_
        return;
    #endif

    //FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
    //FIX_ID_002 xxxxx
    {

        if(ucPortSelCurrent !=ucPortSel)
            return;
    }
    //xxxxx


    if(ucPortSel == F_PORT_SEL_1)
    {
        EQ_PORT1_PRINTF((" ############# TogglePolarity Port 1###############\n"));
        chgbank(1);

        hdmirxset(REG_RX_1C5, 0x10, 0x00);

        //FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
        if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
        {
            //xxxxx only for IT6802A0 Version
            if( (hdmirxrd(REG_RX_1B9)&0x80)>>7 )
            {
            hdmirxset(REG_RX_1B9, 0x80, 0x00);    // Change Polarity
            }
            else
            {
            hdmirxset(REG_RX_1B9, 0x80, 0x80);    // Change Polarity
            }
            //xxxxx
        }
        else
        {
            if( (hdmirxrd(REG_RX_1C9)&0x80)>>7 )
            {
            hdmirxset(REG_RX_1C9, 0x80, 0x00);    // Change Polarity
            }
            else
            {
            hdmirxset(REG_RX_1C9, 0x80, 0x80);    // Change Polarity
            }
        }
        //FIX_ID_002 xxxxx

        hdmirxset(REG_RX_1C5, 0x10, 0x10);

        chgbank(0);


        EQ_PORT1_PRINTF((" ############# TogglePolarity Trigger Port 1 EQ ###############\n"));
        //hdmirxset(0x3E, 0x80, 0x00);
        //#ifdef    ENABLE_AUTO_TRIGGER
        //hdmirxset(0x3E, 0x80, 0x80);
        //#endif
        hdmirxset(REG_RX_03A, 0xFF, 0x38);
        hdmirxset(REG_RX_03A, 0x04, 0x04);
        hdmirxset(REG_RX_03A, 0x04, 0x00);

    }
    else
    {

        EQ_PORT0_PRINTF((" ############# TogglePolarity Port 0###############\n"));
        chgbank(1);
        hdmirxset(REG_RX_1B5, 0x10, 0x00);

        //xxxxx only for IT6802A0 Version
        if( (hdmirxrd(REG_RX_1B9)&0x80)>>7 )
        {
        hdmirxset(REG_RX_1B9, 0x80, 0x00);    // Change Polarity
        }
        else
        {
        hdmirxset(REG_RX_1B9, 0x80, 0x80);    // Change Polarity
        }
        //xxxxx

        hdmirxset(REG_RX_1B5, 0x10, 0x10);
        chgbank(0);


        EQ_PORT0_PRINTF((" ############# TogglePolarity Trigger Port 0 EQ ###############\n"));
        //hdmirxset(0x26, 0x80, 0x00);
        //#ifdef    ENABLE_AUTO_TRIGGER
        //hdmirxset(0x26, 0x80, 0x80);
        //#endif
//        hdmirxset(REG_RX_022, 0x04, 0x00);
        hdmirxset(REG_RX_022, 0xFF, 0x38);    //07-04
        hdmirxset(REG_RX_022, 0x04, 0x04);
        hdmirxset(REG_RX_022, 0x04, 0x00);
    }
#endif
}

void TMDSCheck(unsigned char ucPortSel)
{
#ifdef _SUPPORT_AUTO_EQ_
    unsigned int ucTMDSClk;
    unsigned char rddata;
    unsigned char ucClk;

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    EQ_PORT0_PRINTF(("TMDSCheck() !!!\n"));


    if(ucPortSel == F_PORT_SEL_1)
    {
        ucClk = hdmirxrd(REG_RX_092);
        rddata = hdmirxrd(0x90);

        if(ucClk != 0)
        {

        if( rddata&0x04 )
            ucTMDSClk=2*RCLKVALUE*256/ucClk;
        else if( rddata&0x08 )
            ucTMDSClk=4*RCLKVALUE*256/ucClk;
        else
            ucTMDSClk=RCLKVALUE*256/ucClk;


        EQ_PORT1_PRINTF((" Port 1 TMDS CLK  = %d\n",(int) ucTMDSClk));
        }


        //FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
        if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
        {
            if(ucTMDSClk<TMDSCLKVALUE_480P || ucTMDSClk > TMDSCLKVALUE_1080P )
                hdmirxwr(REG_RX_038,0x00);    // Dr. Liu suggestion to 0x00
            else
                hdmirxwr(REG_RX_038,0x3F);    // Dr. Liu suggestion to 0x3F
        }
        //FIX_ID_002 xxxxx


        EQ_PORT1_PRINTF((" HDMI Reg038  = %X\n",(int) hdmirxrd(REG_RX_038)));

        chgbank(1);
        EQ_PORT1_PRINTF((" HDMI Reg1C1  = %X ,Reg1C2  = %X\n",hdmirxrd(REG_RX_1C1),hdmirxrd(REG_RX_1C2)));
        chgbank(0);

        if(ucPortAMPOverWrite[1]==0)    // 2013-0801
        {
            //FIX_ID_001 xxxxx check State of AutoEQ
            chgbank(1);
            rddata=hdmirxrd(REG_RX_1DC);
            chgbank(0);
            if(rddata==0)
            //FIX_ID_001 xxxxx
            {
                EQ_PORT1_PRINTF((" ############# Trigger Port 1 EQ ###############\n"));
                hdmirxset(REG_RX_03A, 0xFF, 0x38);    //07-04
                hdmirxset(REG_RX_03A, 0x04, 0x04);
                hdmirxset(REG_RX_03A, 0x04, 0x00);
                // FIX_ID_043
                // Tirgger Auto EQ, ToggleCount = 0;
                it6802data->ToggleCount[F_PORT_SEL_1] = 0;
                //~FIX_ID_043
            }
        }
        else
        {
                EQ_PORT1_PRINTF((" ############# B_PORT1_TimingChgEvent###############\n"));
                it6802data->HDMIIntEvent |= (B_PORT1_Waiting);
                it6802data->HDMIIntEvent |= (B_PORT1_TimingChgEvent);
                it6802data->HDMIWaitNo[1]=MAX_TMDS_WAITNO;
        }


    }
    else
    {
        EQ_PORT0_PRINTF((" HDMI Reg90  = %X ,Reg91  = %X\n",(int) hdmirxrd(0x90),(int) hdmirxrd(0x91)));
        ucClk = hdmirxrd(REG_RX_091);
        rddata = hdmirxrd(0x90);

        if(ucClk != 0)
        {
        if( rddata&0x01 )
            ucTMDSClk=2*RCLKVALUE*256/ucClk;
        else if( rddata&0x02 )
            ucTMDSClk=4*RCLKVALUE*256/ucClk;
        else
            ucTMDSClk=RCLKVALUE*256/ucClk;

        EQ_PORT0_PRINTF((" Port 0 TMDS CLK  = %X\n",(int) ucTMDSClk));
    }

//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
    {

        //FIX_ID_007 xxxxx    //for debug IT6681  HDCP issue
            if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
            {
                chgbank(1);
                hdmirxset(REG_RX_1B1,0x20,0x20);//Reg1b1[5] = 1 for enable over-write
                hdmirxset(REG_RX_1B2,0x07,0x01);    // default 0x04 , change to 0x01
                EQ_PORT0_PRINTF((" HDMI Reg1B1  = %X ,Reg1B2  = %X\n",(int) hdmirxrd(REG_RX_1B1),(int) hdmirxrd(REG_RX_1B2)));
                chgbank(0);
            }
            else
            {
                chgbank(1);
                hdmirxset(REG_RX_1B1,0x20,0x00);//Reg1b1[5] = 0 for disable over-write
                hdmirxset(REG_RX_1B2,0x07,0x04);    // default 0x04
                EQ_PORT0_PRINTF((" HDMI Reg1B1  = %X ,Reg1B2  = %X\n",(int) hdmirxrd(REG_RX_1B1),(int) hdmirxrd(REG_RX_1B2)));
                chgbank(0);
            }
        //FIX_ID_007 xxxxx
    }
//FIX_ID_002 xxxxx




//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
    if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
    {
        if(ucTMDSClk<TMDSCLKVALUE_480P || ucTMDSClk > TMDSCLKVALUE_1080P )
            hdmirxwr(REG_RX_020,0x00);    // Dr. Liu suggestion to 0x00
        else
            hdmirxwr(REG_RX_020,0x3F);    // Dr. Liu suggestion to 0x3F
    }
//FIX_ID_002 xxxxx

        EQ_PORT0_PRINTF((" HDMI Reg020  = %X\n",(int) hdmirxrd(REG_RX_020)));


        //FIX_ID_019    xxxxx modify ENHYS control for MxL mode
        if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
        {
            chgbank(1);
            hdmirxset(REG_RX_1B8,0x80,0x00);    // [7] Reg_HWENHYS = 0
            hdmirxset(REG_RX_1B6,0x07,0x00);    // [2:0]Reg_P0_ENHYS = 00 for MxL mode only  [2:0]Reg_P0_ENHYS = 00 for disable ENHYS
            chgbank(0);
        }
        //FIX_ID_019    xxxxx


        if(ucPortAMPOverWrite[0]==0)    // 2013-0801
        {
            //FIX_ID_001 xxxxx check State of AutoEQ
            chgbank(1);
            rddata=hdmirxrd(REG_RX_1D4);
            chgbank(0);

//FIX_ID_032 xxxxx    //Support HDCP Repeater function for HDMI Tx device
#ifdef _ONLY_SUPPORT_MANUAL_EQ_ADJUST_
        #ifndef _SUPPORT_HDCP_REPEATER_
            //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            #ifdef _SUPPORT_EQ_ADJUST_
            HDMIStartEQDetect(&(it6802data->EQPort[F_PORT_SEL_0]));
            #endif
            //FIX_ID_001 xxxxx
        #endif
#else
            if(rddata==0)
            //FIX_ID_001 xxxxx
            {
                EQ_PORT0_PRINTF((" ############# Trigger Port 0 EQ ###############\n"));
                hdmirxset(REG_RX_022, 0xFF, 0x38);    //07-04
                hdmirxset(REG_RX_022, 0x04, 0x04);
                hdmirxset(REG_RX_022, 0x04, 0x00);
                // FIX_ID_043
                // Tirgger Auto EQ, ToggleCount = 0;
                it6802data->ToggleCount[F_PORT_SEL_0] = 0;
                //~FIX_ID_043

            }
#endif
//FIX_ID_032 xxxxx


//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
            // if Authentication start interrupt with CKon interrupt then do TMDSCheck() at first.
            it6802data->HDMIIntEvent &= ~(B_PORT0_TMDSEvent|B_PORT0_Waiting|B_PORT0_TimingChgEvent);
//FIX_ID_033 xxxxx
        }
        else
        {
                EQ_PORT0_PRINTF((" ############# B_PORT0_TimingChgEvent###############\n"));
                it6802data->HDMIIntEvent |= (B_PORT0_Waiting);
                it6802data->HDMIIntEvent |= (B_PORT0_TimingChgEvent);
                it6802data->HDMIWaitNo[0]=MAX_TMDS_WAITNO;
        }


    }

#endif
}




void OverWriteAmpValue2EQ (unsigned char ucPortSel)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    EQ_DEBUG_PRINTF((" 111111111111111111 OverWriteAmpValue2EQ 111111111111111111111111111111\n"));
    EQ_DEBUG_PRINTF((" 111111111111111111 OverWriteAmpValue2EQ 111111111111111111111111111111\n"));
    EQ_DEBUG_PRINTF((" 111111111111111111 OverWriteAmpValue2EQ 111111111111111111111111111111\n"));
    EQ_DEBUG_PRINTF((" 111111111111111111 OverWriteAmpValue2EQ 111111111111111111111111111111\n"));
    EQ_DEBUG_PRINTF((" 111111111111111111 OverWriteAmpValue2EQ 111111111111111111111111111111\n"));
    EQ_DEBUG_PRINTF((" 111111111111111111 OverWriteAmpValue2EQ 111111111111111111111111111111\n"));


    if(ucPortSel == F_PORT_SEL_1)
    {
        if((ucPortAMPValid[1]&0x3F)==0x3F)
        {
            ucPortAMPOverWrite[F_PORT_SEL_1]=1;    //2013-0801
            ucEQMode[F_PORT_SEL_1] = 0; // 0 for Auto Mode
            EQ_PORT1_PRINTF(("#### REG_RX_03E = 0x%X ####\n",(int) hdmirxrd(REG_RX_03E)));
            hdmirxset(REG_RX_03E,0x20,0x20);    //Manually set RS Value
            EQ_PORT1_PRINTF(("#### REG_RX_03E = 0x%X ####\n",(int) hdmirxrd(REG_RX_03E)));

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
            if(ucChannelB[F_PORT_SEL_1]  < MinEQValue )
            {
            hdmirxwr(REG_RX_03F,MinEQValue);
            }
            else
            {
            hdmirxwr(REG_RX_03F,(ucChannelB[F_PORT_SEL_1] & 0x7F));
            }

            if(ucChannelG[F_PORT_SEL_1]  < MinEQValue )
            {
            hdmirxwr(REG_RX_040,MinEQValue);
            }
            else
            {
            hdmirxwr(REG_RX_040,(ucChannelG[F_PORT_SEL_1] & 0x7F));
            }

            if(ucChannelR[F_PORT_SEL_1]  < MinEQValue )
            {
            hdmirxwr(REG_RX_041,MinEQValue);
            }
            else
            {
            hdmirxwr(REG_RX_041,(ucChannelR[F_PORT_SEL_1] & 0x7F));
            }

            //xxxxx 2014-0421 disable ->            hdmirxset(REG_RX_03F,0xFF,(ucChannelB[F_PORT_SEL_1] & 0x7F));
            //xxxxx 2014-0421 disable ->            hdmirxset(REG_RX_040,0xFF,(ucChannelG[F_PORT_SEL_1] & 0x7F));
            //xxxxx 2014-0421 disable ->            hdmirxset(REG_RX_041,0xFF,(ucChannelR[F_PORT_SEL_1] & 0x7F));

            //if Auto EQ done  interrupt then clear HDMI Event !!!
            it6802data->HDMIIntEvent &= ~(B_PORT1_TMDSEvent|B_PORT1_Waiting|B_PORT1_TimingChgEvent);


            EQ_PORT1_PRINTF((" ############# Over-Write port 1 EQ###############\n"));
            EQ_PORT1_PRINTF((" ############# B port 1 Reg03F = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_03F)));
            EQ_PORT1_PRINTF((" ############# G port 1 Reg040 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_040)));
            EQ_PORT1_PRINTF((" ############# R port 1 Reg041 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_041)));

            //    HDMICheckErrorCount(&(it6802data->EQPort[F_PORT_SEL_1]));    //07-04 for port 1

            //hdmirxset(REG_RX_03A, 0xFF, 0x00);    //07-08 [3] power down    ?????
            hdmirxwr(REG_RX_03A, 0x00);    // power down auto EQ
            hdmirxwr(0xD0, 0xC0);
//FIX_ID_033 xxxxx

        }
    }
    else
    {

    //07-08
    if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
    {
        if((ucPortAMPValid[F_PORT_SEL_0]&0x03)==0x03)
        {
            ucPortAMPOverWrite[F_PORT_SEL_0]=1;    //2013-0801
            ucEQMode[F_PORT_SEL_0] = 0; // 0 for Auto Mode
            EQ_PORT0_PRINTF(("#### REG_RX_026 = 0x%X ####\n",(int) hdmirxrd(REG_RX_026)));

            hdmirxset(REG_RX_026,0x20,0x20);    //Manually set RS Value
            EQ_PORT0_PRINTF(("#### REG_RX_026 = 0x%X ####\n",(int) hdmirxrd(REG_RX_026)));

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
            if((ucChannelB[F_PORT_SEL_0] ) < MinEQValue)
            {
            hdmirxwr(REG_RX_027,(MinEQValue));
            hdmirxwr(REG_RX_028,(MinEQValue));    //07-08 using B channal to over-write G and R channel
            hdmirxwr(REG_RX_029,(MinEQValue));
            }
            else
            {
            hdmirxwr(REG_RX_027,(ucChannelB[F_PORT_SEL_0] & 0x7F));
            hdmirxwr(REG_RX_028,(ucChannelB[F_PORT_SEL_0] & 0x7F));    //07-08 using B channal to over-write G and R channel
            hdmirxwr(REG_RX_029,(ucChannelB[F_PORT_SEL_0] & 0x7F));
            }

            EQ_PORT0_PRINTF((" ############# Over-Write port 0 MxL EQ###############\n"));
            EQ_PORT0_PRINTF((" ############# B port 0 REG_RX_027 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_027)));
            EQ_PORT0_PRINTF((" ############# G port 0 REG_RX_028 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_028)));
            EQ_PORT0_PRINTF((" ############# R port 0 REG_RX_029 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_029)));

            //    HDMICheckErrorCount(&(it6802data->EQPort[F_PORT_SEL_0]));    //07-04 for port 1

            hdmirxwr(REG_RX_022, 0x00);    // power down auto EQ
        hdmirxwr(0xD0, 0x30);
//FIX_ID_033 xxxxx
        }

    }
    else
    {
        if((ucPortAMPValid[F_PORT_SEL_0]&0x3F)==0x3F)
        {
            ucPortAMPOverWrite[F_PORT_SEL_0]=1;    //2013-0801
            ucEQMode[F_PORT_SEL_0] = 0; // 0 for Auto Mode
            EQ_PORT0_PRINTF(("#### REG_RX_026 = 0x%X ####\n",(int) hdmirxrd(REG_RX_026)));
            hdmirxset(REG_RX_026,0x20,0x20);    //Manually set RS Value
            EQ_PORT0_PRINTF(("#### REG_RX_026 = 0x%X ####\n",(int) hdmirxrd(REG_RX_026)));

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
            if(ucChannelB[F_PORT_SEL_0]  < MinEQValue )
            {
            hdmirxwr(REG_RX_027,MinEQValue);
            }
            else
            {
            hdmirxwr(REG_RX_027,(ucChannelB[F_PORT_SEL_0] & 0x7F));
            }

            if(ucChannelG[F_PORT_SEL_0]  < MinEQValue )
            {
            hdmirxwr(REG_RX_028,MinEQValue);
            }
            else
            {
            hdmirxwr(REG_RX_028,(ucChannelG[F_PORT_SEL_0] & 0x7F));
            }

            if(ucChannelR[F_PORT_SEL_0]  < MinEQValue )
            {
            hdmirxwr(REG_RX_029,MinEQValue);
            }
            else
            {
            hdmirxwr(REG_RX_029,(ucChannelR[F_PORT_SEL_0] & 0x7F));
            }

//xxxxx 2014-0421 disable ->            hdmirxset(REG_RX_027,0xFF,(ucChannelB[F_PORT_SEL_0] & 0x7F));
//xxxxx 2014-0421 disable ->            hdmirxset(REG_RX_028,0xFF,(ucChannelG[F_PORT_SEL_0] & 0x7F));
//xxxxx 2014-0421 disable ->            hdmirxset(REG_RX_029,0xFF,(ucChannelR[F_PORT_SEL_0] & 0x7F));

            //if Auto EQ done  interrupt then clear HDMI Event !!!
            it6802data->HDMIIntEvent &= ~(B_PORT0_TMDSEvent|B_PORT0_Waiting|B_PORT0_TimingChgEvent);

            EQ_PORT0_PRINTF((" ############# Over-Write port 0 EQ###############\n"));
            EQ_PORT0_PRINTF((" ############# B port 0 REG_RX_027 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_027)));
            EQ_PORT0_PRINTF((" ############# G port 0 REG_RX_028 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_028)));
            EQ_PORT0_PRINTF((" ############# R port 0 REG_RX_029 = 0x%X  ###############\n",(int) hdmirxrd(REG_RX_029)));

            //    HDMICheckErrorCount(&(it6802data->EQPort[F_PORT_SEL_0]));    //07-04 for port 1

            //hdmirxset(REG_RX_022, 0xFF, 0x00);    //07-08 [3] power down
            hdmirxwr(REG_RX_022, 0x00);    // power down auto EQ
        hdmirxwr(0xD0, 0x30);
//FIX_ID_033 xxxxx
        }
    }

    }
}
//-------------------------------------------------------------------------------------------------------
#endif



//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_EQ_ADJUST_

void HDMIStartEQDetect(struct it6802_eq_data *ucEQPort)
/*
 * This is the HDMIRX Start EQ Detect
 * @param it6802_eq_data
 * @return void
 */
{
    unsigned char ucPortSel;


//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
if( ucEQPort->ucPortID == F_PORT_SEL_0)
{
    if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
    {
        // for MxL mode , there are no need to adjust EQ for long cable.
        return;
    }
}
//FIX_ID_035 xxxxx

    if(ucEQPort->ucEQState==0xFF)
    {
        ucPortSel = hdmirxrd(REG_RX_051) & B_PORT_SEL;

        if(ucPortSel == ucEQPort->ucPortID)
        {
        HDMISwitchEQstate(ucEQPort,0);    // for SCDT off state
        }
        else
        {
        HDMISwitchEQstate(ucEQPort,EQSTATE_WAIT+1);    //for SCDT on state
        }

        ucEQPort->f_manualEQadjust=TRUE;
        HDMIAdjustEQ(ucEQPort);

//FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
        ucEQPort->ErrorCount[0] = MAXECCWAIT;
        ucEQPort->ErrorCount[1] = MAXECCWAIT;
        ucEQPort->ErrorCount[2] = MAXECCWAIT;
//FIX_ID_010 xxxxx
    }
}

void HDMISetEQValue(struct it6802_eq_data *ucEQPort,unsigned char ucIndex)
/*
 * This is the HDMIRX Set Manual EQ value
 * @param it6802_eq_data
 * @return void
 */
{
    if(ucIndex<MaxEQIndex)
    {
        if(ucEQPort->ucPortID==F_PORT_SEL_0)
        {
#ifdef _SUPPORT_AUTO_EQ_
            ucEQMode[F_PORT_SEL_0] = 1; // 1 for Manual Mode
#endif
            hdmirxset(REG_RX_026, 0x20, 0x20);    //07-04 add for adjust EQ
            hdmirxwr(REG_RX_027,IT6802EQTable[ucIndex]);
            EQ_PORT0_PRINTF(("Port=%X ,ucIndex = %X ,HDMISetEQValue Reg027 = %X\n",(int) ucEQPort->ucPortID,(int) ucIndex,(int) hdmirxrd(REG_RX_027)));
        }
        else
        {
#ifdef _SUPPORT_AUTO_EQ_
            ucEQMode[F_PORT_SEL_1] = 1; // 1 for Manual Mode
#endif
            hdmirxset(REG_RX_03E, 0x20, 0x20);    //07-04 add for adjust EQ
            hdmirxwr(REG_RX_03F,IT6802EQTable[ucIndex]);
            EQ_PORT1_PRINTF(("Port=%X ,ucIndex = %X ,HDMISetEQValue Reg03F = %X\n",(int) ucEQPort->ucPortID,(int) ucIndex,(int) hdmirxrd(REG_RX_03F)));
        }

    }

}


void HDMISwitchEQstate(struct it6802_eq_data *ucEQPort,unsigned char state)
/*
 * This is the HDMIRX Switch EQ State
 * @param it6802_eq_data
 * @return void
 */
{

    ucEQPort->ucEQState=state;

    EQ_DEBUG_PRINTF(("!!! Port=%X ,HDMISwitchEQstate %X\n",(int) ucEQPort->ucPortID,(int) ucEQPort->ucEQState));

    switch(ucEQPort->ucEQState)
    {
        case EQSTATE_START:
            HDMISetEQValue(ucEQPort,0);
            break;
        case EQSTATE_LOW:
            HDMISetEQValue(ucEQPort,1);
            break;
        case EQSTATE_MIDDLE:
            HDMISetEQValue(ucEQPort,2);
            break;
        case EQSTATE_HIGH:
            HDMISetEQValue(ucEQPort,3);
            break;

        default:
            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            //xxxxx 2014-0529 //HDCP Content On/Off
            IT6802_HDCP_ContentOff(ucEQPort->ucPortID, 0);
            //xxxxx 2014-0529
            //FIX_ID_037 xxxxx

            HDMISetEQValue(ucEQPort,0xff);    //dont care
            break;

    }

    // !!! re-start the error count !!!

    ucEQPort->ucPkt_Err=0;
    ucEQPort->ucECCvalue=0;
    ucEQPort->ucECCfailCount=0;

}

void HDMICheckSCDTon(struct it6802_eq_data *ucEQPort)
/*
 * This is the HDMIRX SCDT on
 * @param it6802_eq_data
 * @return void
 */
{
    unsigned char ucResult = 0;
    unsigned char Receive_Err;
    unsigned char ucStatus;
    unsigned char ucCurrentPort;
    unsigned char ucHDCP;

    ucCurrentPort = hdmirxrd(REG_RX_051) & B_PORT_SEL;

    if(ucEQPort->ucPortID != ucCurrentPort)
        return;


    if(ucEQPort->ucPortID==F_PORT_SEL_1)
    {
        ucStatus = hdmirxrd(REG_RX_P1_SYS_STATUS);
        // !!! check ECC error register  !!!
        Receive_Err = hdmirxrd(REG_RX_0B7);
        hdmirxwr(REG_RX_0B7,Receive_Err);

        ucHDCP = hdmirxrd(REG_RX_095);
    }
    else
    {
        ucStatus = hdmirxrd(REG_RX_P0_SYS_STATUS);
        // !!! check ECC error register  !!!
        Receive_Err = hdmirxrd(REG_RX_0B2);
        hdmirxwr(REG_RX_0B2,Receive_Err);

        ucHDCP = hdmirxrd(REG_RX_093);
    }


    if((ucStatus & (B_P0_SCDT|B_P0_PWR5V_DET|B_P0_RXCK_VALID)) == (B_P0_PWR5V_DET|B_P0_RXCK_VALID))
    {
            ucEQPort->ucECCfailCount++;
    }

    EQ_DEBUG_PRINTF(("Port=%d, CheckSCDTon=%d, Receive_Err=%X, ucECCfailCount=%X, SCDT=%X, HDCP=%X\n",
                (int) ucEQPort->ucPortID,(int) ucEQPort->ucEQState,(int) Receive_Err,(int)ucEQPort->ucECCfailCount,(int) ucStatus,(int) ucHDCP));

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
    if((Receive_Err & 0xC0) != 0x00)
    {
            ucEQPort->ucECCvalue++;

            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            //xxxxx 2014-0529 //Manual Content On/Off
            IT6802_HDCP_ContentOff(ucEQPort->ucPortID, 1);
            //xxxxx 2014-0529
            //FIX_ID_037 xxxxx

            //xxxxx 2014-0421
            //if((Receive_Err & 0xC0) == 0xC0)
            if(ucEQPort->ucECCvalue > ((MINECCFAILCOUNT/2)))
            //xxxxx 2014-0421
            {
                ucEQPort->ucECCvalue = 0;
                EQ_DEBUG_PRINTF(("HDMICheckSCDTon() for ECC / Deskew issue !!!"));

                if(ucEQPort->ucPortID==F_PORT_SEL_1)
                {
                    if(hdmirxrd(REG_RX_038) == 0x00)
                        hdmirxwr(REG_RX_038,0x3F);    // Dr. Liu suggestion to 0x00
                    //else
                    //    hdmirxwr(REG_RX_038,0x00);    // Dr. Liu suggestion to 0x3F

                    EQ_PORT1_PRINTF(("Port 1 Reg38=%X !!!\n",(int) hdmirxrd(REG_RX_038)));
                }
                else
                {
                    if(hdmirxrd(REG_RX_020) == 0x00)
                    {
                        hdmirxwr(REG_RX_020,0x3F);    // Dr. Liu suggestion to 0x00
                    //else
                    //    hdmirxwr(REG_RX_020,0x00);    // Dr. Liu suggestion to 0x3F

                    EQ_PORT0_PRINTF(("Port 0 Reg20=%X !!!\n",(int) hdmirxrd(REG_RX_020)));
                    }
                }
            }
    }
//FIX_ID_033 xxxxx

    if(ucEQPort->ucEQState == EQSTATE_WAIT-1)
    {

        //FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
            EQ_DEBUG_PRINTF(("Port=%d, CheckSCDTon=%d, Receive_Err=%X, ucECCfailCount=%X, SCDT=%X, HDCP=%X\n",
                    (int) ucEQPort->ucPortID,(int) ucEQPort->ucEQState,(int) Receive_Err,(int)ucEQPort->ucECCfailCount,(int) ucStatus,(int) ucHDCP));

            if((Receive_Err & 0xC0) == 0xC0)
            {
                EQ_DEBUG_PRINTF(("HDMICheckSCDTon() CDR reset for Port %d ECC_TIMEOUT !!!\n",ucCurrentPort));
                hdmirx_ECCTimingOut(ucCurrentPort);

                HDMISwitchEQstate(ucEQPort,EQSTATE_END);
                return;
            }
        //FIX_ID_033 xxxxx

#ifdef _SUPPORT_AUTO_EQ_
        if((ucEQPort->ucECCfailCount)==0)
        {


            if(ucEQPort->ucPortID==F_PORT_SEL_1)
            {
                if(ucEQMode[F_PORT_SEL_1] == 0)    // verfiy Auto EQ Value wehn auto EQ finish
                {

                    if(    ((ucChannelB[F_PORT_SEL_1] & 0x7F)<0x0F) ||
                        ((ucChannelG[F_PORT_SEL_1] & 0x7F)<0x0F) ||
                        ((ucChannelR[F_PORT_SEL_1] & 0x7F)<0x0F) )

                    {
                        ucResult    = 1;    // 1 for EQ start
                    }

                }
            }
            else
            {
                if(ucEQMode[F_PORT_SEL_0] == 0)    // verfiy Auto EQ Value when auto EQ finish
                {
                    if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
                    {
                        if((ucChannelB[F_PORT_SEL_0] & 0x7F)<0x0F)
                        {
                            ucResult    = 1;    // 1 for EQ start
                        }
                    }
                    else
                    {
                        if(    ((ucChannelB[F_PORT_SEL_0] & 0x7F)<0x0F) ||
                            ((ucChannelG[F_PORT_SEL_0] & 0x7F)<0x0F) ||
                            ((ucChannelR[F_PORT_SEL_0] & 0x7F)<0x0F) )

                        {
                            ucResult    = 1;    // 1 for EQ start
                        }

                    }
                }
            }

            if( ucResult == 0)    // no need to do manual EQ adjust when SCDT always On !!!
            {
                HDMISwitchEQstate(ucEQPort,EQSTATE_END);
                return;
            }

        }
#endif

        HDMISwitchEQstate(ucEQPort,EQSTATE_WAIT);
    }
}

void HDMIPollingErrorCount(struct it6802_eq_data *ucEQPort)
/*
 * This is the HDMIPollingErrorCount
 * @param
 * @return void
 */
{
    unsigned char Receive_Err;
    unsigned char Video_Err;
    unsigned char Code_Err;
    unsigned char Pkt_Err;
    unsigned char CrtErr;
    unsigned char ucHDCP;
    unsigned char ucStatus;

    unsigned char ucCurrentPort;
    ucCurrentPort = hdmirxrd(REG_RX_051)&B_PORT_SEL;



    if(ucEQPort->ucPortID==F_PORT_SEL_1)
    {
        ucStatus = hdmirxrd(REG_RX_P1_SYS_STATUS);
        // !!! check ECC error register  !!!
        Receive_Err = hdmirxrd(REG_RX_0B7);
        Video_Err = hdmirxrd(REG_RX_0B8)&0xE0;
        Code_Err = hdmirxrd(REG_RX_0B9);
        Pkt_Err = hdmirxrd(REG_RX_0BA);
        CrtErr = hdmirxrd(REG_RX_0BB);

        hdmirxwr(REG_RX_0B7,Receive_Err);
        hdmirxwr(REG_RX_0B8,Video_Err);
        hdmirxwr(REG_RX_0B9,Code_Err);
        hdmirxwr(REG_RX_0BA,Pkt_Err);
        hdmirxwr(REG_RX_0BB,CrtErr);

        ucHDCP = hdmirxrd(REG_RX_095);
    }
    else
    {
        ucStatus = hdmirxrd(REG_RX_P0_SYS_STATUS);
        // !!! check ECC error register  !!!
        Receive_Err = hdmirxrd(REG_RX_0B2);
        Video_Err = hdmirxrd(REG_RX_0B3)&0xE0;
        Code_Err = hdmirxrd(REG_RX_0B4);
        Pkt_Err = hdmirxrd(REG_RX_0B5);
        CrtErr = hdmirxrd(REG_RX_0B6);

        hdmirxwr(REG_RX_0B2,Receive_Err);
        hdmirxwr(REG_RX_0B3,Video_Err);
        hdmirxwr(REG_RX_0B4,Code_Err);
        hdmirxwr(REG_RX_0B5,Pkt_Err);
        hdmirxwr(REG_RX_0B6,CrtErr);


        ucHDCP = hdmirxrd(REG_RX_093);
    }

    //xxxxx 2013-0903
    if(ucCurrentPort == ucEQPort->ucPortID)
    {
        if((ucStatus & B_P0_SCDT) == 0x00)
        {
            Receive_Err = 0xFF;

        //xxxxx 2013-0812  ++++
        ucEQPort->ucECCfailCount |= 0x80;
        //xxxxx 2013-0812

        }
    }
    //xxxxx 2013-0903

    EQ_DEBUG_PRINTF(("Port=%d ,EQState2No=%d, Receive_Err=%X, HDCP=%X\n",
                    (int) ucEQPort->ucPortID,(int) ucEQPort->ucEQState,(int) Receive_Err,(int) ucHDCP));

#if 1
//FIX_ID_007 xxxxx    //07-18 xxxxx for ATC 8-7 Jitter Tolerance
    if(Pkt_Err==0xFF ||Code_Err==0xFF)
    {
        ucEQPort->ucPkt_Err++;    // judge whether CDR reset
    }
    else
    {
        ucEQPort->ucPkt_Err=0;
    }

    if(ucEQPort->ucPkt_Err > (MINECCFAILCOUNT-2))
    {

        if( ucEQPort->ucEQState > EQSTATE_START)
        {

//FIX_ID_020 xxxxx        //Turn off DEQ for HDMI port 1 with 20m DVI Cable
            EQ_DEBUG_PRINTF(("1111111111111111111111111111111111111111111111111111111111111111111111111\n"));

            if(ucEQPort->ucPortID==F_PORT_SEL_1)
            {
                Code_Err = hdmirxrd(REG_RX_0B9);
                hdmirxwr(REG_RX_0B9,Code_Err);

                if(Code_Err == 0xFF)
                {
                    if(hdmirxrd(REG_RX_038) == 0x00)
                        hdmirxwr(REG_RX_038,0x3F);    // Dr. Liu suggestion to 0x00
                    else
                        hdmirxwr(REG_RX_038,0x00);    // Dr. Liu suggestion to 0x3F
                    EQ_DEBUG_PRINTF(("Port 1 Reg38=%X !!!\n",(int) hdmirxrd(REG_RX_038)));
                }
            }
            else
            {
                Code_Err = hdmirxrd(REG_RX_0B4);
                hdmirxwr(REG_RX_0B4,Code_Err);

                if(Code_Err == 0xFF)
                {
                    if(hdmirxrd(REG_RX_020) == 0x00)
                        hdmirxwr(REG_RX_020,0x3F);    // Dr. Liu suggestion to 0x00
                    else
                        hdmirxwr(REG_RX_020,0x00);    // Dr. Liu suggestion to 0x3F

                    EQ_DEBUG_PRINTF(("Port 0 Reg20=%X !!!\n",(int) hdmirxrd(REG_RX_020)));
                }
            }
            EQ_DEBUG_PRINTF(("1111111111111111111111111111111111111111111111111111111111111111111111111\n"));
//FIX_ID_020 xxxxx

            if(ucEQPort->ucPortID==F_PORT_SEL_0)
            {

                hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST),(B_P0_DCLKRST|B_P0_CDRRST/*|B_P0_SWRST*/));
                hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST),0x00);
                EQ_PORT0_PRINTF((" HDMIPollingErrorCount( ) Port 0 CDR reset !!!!!!!!!!!!!!!!!!\n"));
            }
            else
            {
                hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST),(B_P1_DCLKRST|B_P1_CDRRST/*|B_P1_SWRST*/));
                hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST),0x00);
                EQ_PORT0_PRINTF((" HDMIPollingErrorCount( ) Port 1 CDR reset !!!!!!!!!!!!!!!!!!\n"));
            }
        }
        ucEQPort->ucPkt_Err=0;

//xxxxx 2013-0812  ++++
        ucEQPort->ucECCfailCount |= 0x40;
        ucEQPort->ucECCfailCount &= 0xF0;
//xxxxx 2013-0812

    }
    //07-18 xxxxx
//FIX_ID_007 xxxxx
#endif


//    if(Receive_Err>32 )

//xxxxx 2013-0812  ++++
    if(Receive_Err != 0 )
//xxxxx 2013-0812
    {
        EQ_DEBUG_PRINTF(("Video_Err = %X\n",(int) Video_Err));
        EQ_DEBUG_PRINTF(("Code_Err = %X\n",(int) Code_Err));
        EQ_DEBUG_PRINTF(("Pkt_Err = %X\n",(int) Pkt_Err));
        EQ_DEBUG_PRINTF(("CrtErr = %X\n",(int) CrtErr));

        ucEQPort->ucECCvalue++;
        ucEQPort->ucECCfailCount++;
    }
    else
    {
        ucEQPort->ucECCfailCount=0;
    }

//    EQ_DEBUG_PRINTF(("ucEQPort->ucECCvalue = %X 666666666666666666666666\n",ucEQPort->ucECCvalue));
//xxxxx 2013-0812  ++++
#if 1
    if((ucEQPort->ucECCfailCount & 0x7F) < (0x40) )    // before CDR reset , dont care pkt_error and code_error
    {

        if(Pkt_Err==0xFF ||Code_Err==0xFF)
            return;
    }
#endif
//xxxxx 2013-0812

//    if((ucEQPort->ucECCfailCount & 0x7F) > (0x40 + MINECCFAILCOUNT-2))

//xxxxx 2013-0812  ++++
    if((ucEQPort->ucECCfailCount & 0x0F) > (MINECCFAILCOUNT-2))
    {

        ucEQPort->ucECCvalue=MAXECCWAIT;

        ucCurrentPort = hdmirxrd(REG_RX_051)&B_PORT_SEL;

        if(ucEQPort->ucPortID==F_PORT_SEL_1)
        {
            ucStatus = hdmirxrd(REG_RX_P1_SYS_STATUS);
        }
        else
        {
            ucStatus = hdmirxrd(REG_RX_P0_SYS_STATUS);
        }

        if(ucCurrentPort == ucEQPort->ucPortID)
        {
            if(((ucStatus & B_P0_SCDT) == 0x00) || ((ucEQPort->ucECCfailCount & 0x80) != 0x00))
            {
                ucEQPort->ucECCvalue=MAXECCWAIT | 0x80;    // 0x80 for Identify SCDT off with Ecc error
            }
        }

        StoreEccCount(ucEQPort);    // abnormal judge ucECCvalue mode


        if(ucEQPort->ucEQState<EQSTATE_START)
            HDMISwitchEQstate(ucEQPort,EQSTATE_START);
        else if(ucEQPort->ucEQState<EQSTATE_LOW)
            HDMISwitchEQstate(ucEQPort,EQSTATE_LOW);
        else if(ucEQPort->ucEQState<EQSTATE_MIDDLE)
            HDMISwitchEQstate(ucEQPort,EQSTATE_MIDDLE);
        else if(ucEQPort->ucEQState<=EQSTATE_HIGH)
            HDMISwitchEQstate(ucEQPort,EQSTATE_HIGH);
    }
//xxxxx 2013-0812

}

void HDMIJudgeECCvalue(struct it6802_eq_data *ucEQPort)
/*
 * This is the HDMIJudgeECCvalue
 * @param it6802_eq_data
 * @return void
 */
{

    //unsigned char uc;

    EQ_DEBUG_PRINTF(("!!! HDMI Judge ECCvalue( ) %X!!!\n",(int) ucEQPort->ucECCvalue));

    StoreEccCount(ucEQPort);    // normal judge ucECCvalue mode

    if((ucEQPort->ucECCvalue) > (MAXECCWAIT/2))
    {
        //uc = CheckErrorCode(ucEQPort);

        //if(CheckErrorCode()==FALSE)
        //if(uc == FALSE)
        {

            if(ucEQPort->ucEQState==EQSTATE_START)
                HDMISwitchEQstate(ucEQPort,EQSTATE_START);
            else if(ucEQPort->ucEQState==EQSTATE_LOW)
                HDMISwitchEQstate(ucEQPort,EQSTATE_LOW);
            else if(ucEQPort->ucEQState==EQSTATE_MIDDLE)
                HDMISwitchEQstate(ucEQPort,EQSTATE_MIDDLE);
            else if(ucEQPort->ucEQState==EQSTATE_HIGH)
                HDMISwitchEQstate(ucEQPort,EQSTATE_HIGH);
        }
    }
    else
    {
        HDMISwitchEQstate(ucEQPort,EQSTATE_END);    // quit EQadjust( )
    }


    ucEQPort->ucPkt_Err=0;
    ucEQPort->ucECCvalue=0;
    ucEQPort->ucECCfailCount=0;

}


void HDMIAdjustEQ(struct it6802_eq_data *ucEQPort)
/*
 * This is the HDMIAdjustEQ
 * @param it6802_eq_data
 * @return void
 */
{
    unsigned char ucCurrentPort;
    ucCurrentPort = hdmirxrd(REG_RX_051)&B_PORT_SEL;

    switch(ucEQPort->ucEQState)
    {
         case EQSTATE_WAIT:
            break;
         case EQSTATE_START:
        case EQSTATE_LOW:
        case EQSTATE_MIDDLE:
            HDMIJudgeECCvalue(ucEQPort);
            break;

        case EQSTATE_HIGH:
            HDMIJudgeECCvalue(ucEQPort);
             ucEQPort->ucEQState=EQSTATE_END;
            break;

        case EQSTATE_HIGH+1:
        case EQSTATE_END+1:

            //xxxxx 2013-0904 for pc debug only
            ucEQPort->f_manualEQadjust = FALSE;
            ucEQPort->ucEQState    = 0xFF;
            //xxxxx 2013-0904 for pc debug only



            if(ucEQPort->ucPortID==ucCurrentPort)
            IT6802VideoCountClr();

             break;

        case 0xff:
        default:
            break;
    }

    if(ucEQPort->ucEQState != 0xFF)
    {

        if(ucEQPort->ucEQState < EQSTATE_WAIT)            //20120410
            HDMICheckSCDTon(ucEQPort);
        else if(ucEQPort->ucEQState < EQSTATE_HIGH)
            HDMIPollingErrorCount(ucEQPort);

        ucEQPort->ucEQState++;
    }
    else
    {
        ucEQPort->f_manualEQadjust = FALSE;
    }
}


//FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
void StoreEccCount(struct it6802_eq_data *ucEQPort)
{

    EQ_DEBUG_PRINTF(("StoreEccCount() ucEQPort->ucECCvalue = %02X\n",(int) ucEQPort->ucECCvalue));

    if(ucEQPort->ucEQState <= EQSTATE_LOW)
        ucEQPort->ErrorCount[0] = ucEQPort->ucECCvalue;
    else if(ucEQPort->ucEQState <= EQSTATE_MIDDLE)
        ucEQPort->ErrorCount[1] = ucEQPort->ucECCvalue;
    else if(ucEQPort->ucEQState <= EQSTATE_HIGH)
    {
        ucEQPort->ErrorCount[2] = ucEQPort->ucECCvalue;
        JudgeBestEQ(ucEQPort);
    }

}



void JudgeBestEQ(struct it6802_eq_data *ucEQPort)
{
    unsigned char i,j,Result;

        j=0;
        Result=ucEQPort->ErrorCount[0];

        for(i=1;i<MaxEQIndex;i++)
        {
//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
// use Min value to be best EQ !            if(Result>ucEQPort->ErrorCount[i])
// use Max value to be best EQ !            if(Result>=ucEQPort->ErrorCount[i])
            if(Result>=ucEQPort->ErrorCount[i])
//FIX_ID_033 xxxxx
            {
                Result=ucEQPort->ErrorCount[i];
                j=i;
            }
        }

        EQ_DEBUG_PRINTF((" Best IT6802EQTable ErrorCount[%X]=%X !!! IT6802EQTable Value=%X !!!\n",(int) j,(int) Result,(int) IT6802EQTable[j]));

        //if(j==0 && Result==0)
//2014-0102 bug xxxxx !!!!!
        if(ucEQPort->ucPortID==F_PORT_SEL_0)
        {
#ifdef _SUPPORT_AUTO_EQ_
                if((hdmirxrd(REG_RX_027) & 0x80) == 0)/*20231214*/
                {
                    //printf(" Use Auto EQ Value\n",j,Result,IT6802EQTable[j]);

        //            AmpValidCheck(ucEQPort->ucPortID);
                    OverWriteAmpValue2EQ(ucEQPort->ucPortID);
                }
                else
#endif
                {
                    hdmirxset(REG_RX_026, 0x20, 0x20);    //07-04 add for adjust EQ
                    hdmirxwr(REG_RX_027,IT6802EQTable[j]);
                    EQ_PORT0_PRINTF(("Port=%X ,ucIndex = %X ,JudgeBestEQ Reg027 = %X\n",(int) ucEQPort->ucPortID,(int) j,(int) hdmirxrd(REG_RX_027)));
                }

        }
//#ifdef ENABLE_INTPUT_PORT1
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        else
        {
#ifdef _SUPPORT_AUTO_EQ_
                if((hdmirxrd(REG_RX_03F) & 0x80) == 0)/*20231214*/
                {
                    //printf(" Use Auto EQ Value\n",j,Result,IT6802EQTable[j]);

        //            AmpValidCheck(ucEQPort->ucPortID);
                    OverWriteAmpValue2EQ(ucEQPort->ucPortID);
                }
                else
#endif
                {

                    hdmirxset(REG_RX_03E, 0x20, 0x20);    //07-04 add for adjust EQ
                    hdmirxwr(REG_RX_03F,IT6802EQTable[j]);
                    EQ_PORT1_PRINTF(("Port=%X ,ucIndex = %X ,JudgeBestEQ Reg03F = %X\n",(int) ucEQPort->ucPortID,(int) j,(int) hdmirxrd(REG_RX_03F)));

                }
        }
//-------------------------------------------------------------------------------------------------------
//#endif
//2014-0102 bug xxxxx



}

//---------------------------------------------------------------------------------------------------
void IT6802VideoCountClr(void)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();
    it6802data->m_VideoCountingTimer = 1;
}
#endif
//FIX_ID_001 xxxxx
//---------------------------------------------------------------------------------------------------



unsigned char CheckAVMute(void)
{

    unsigned char ucAVMute;
    unsigned char ucPortSel;

    ucAVMute=hdmirxrd(REG_RX_0A8) & (B_P0_AVMUTE|B_P1_AVMUTE);
    ucPortSel = hdmirxrd(REG_RX_051)&B_PORT_SEL;

    if(((ucAVMute & B_P0_AVMUTE)&& (ucPortSel == F_PORT_SEL_0 ))||
    ((ucAVMute & B_P1_AVMUTE)&& (ucPortSel == F_PORT_SEL_1 )))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}




//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
unsigned char CheckPlg5VPwr(unsigned char ucPortSel)
{
    unsigned char sys_state_P0;
    unsigned char sys_state_P1;

    if(ucPortSel==0)
    {
        sys_state_P0 = hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_PWR5V_DET;
        if((sys_state_P0 & B_P0_PWR5V_DET))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        sys_state_P1 = hdmirxrd(REG_RX_P1_SYS_STATUS) & B_P1_PWR5V_DET;
        if((sys_state_P1 & B_P1_PWR5V_DET))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }

    }
}
//FIX_ID_037 xxxxx

// ---------------------------------------------------------------------------
unsigned char IsHDMIMode(void)
{

    unsigned char sys_state_P0;
    unsigned char sys_state_P1;
    unsigned char ucPortSel;

    sys_state_P0=hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_HDMI_MODE;
    sys_state_P1=hdmirxrd(REG_RX_P1_SYS_STATUS) & B_P1_HDMI_MODE;
    ucPortSel = hdmirxrd(REG_RX_051) & B_PORT_SEL;

    if(((sys_state_P0 & B_P0_HDMI_MODE)&& (ucPortSel == F_PORT_SEL_0 ))||
    ((sys_state_P1 & B_P1_HDMI_MODE)&& (ucPortSel == F_PORT_SEL_1 )))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

// ---------------------------------------------------------------------------
unsigned char IsVideoOn(void)
{

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    if(it6802data->m_VState == VSTATE_VideoOn )

    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
// ---------------------------------------------------------------------------
void GetAVIInfoFrame(struct it6802_dev_data *it6802)
{
    chgbank(2);
    it6802->ColorMode = ((hdmirxrd(REG_RX_AVI_DB1)&0x60)>>5);
    it6802->Colorimetry = ((hdmirxrd(REG_RX_AVI_DB2)&0xC0)>>6);
    it6802->ExtendedColorimetry = ((hdmirxrd(REG_RX_AVI_DB3)&0x70)>>4);
    it6802->RGBQuantizationRange = ((hdmirxrd(REG_RX_AVI_DB3)&0x0C)>>2);
    it6802->VIC = ((hdmirxrd(REG_RX_AVI_DB4)&0x7F));
    it6802->YCCQuantizationRange = ((hdmirxrd(REG_RX_AVI_DB5)&0xC0)>>6);
    chgbank(0);

//FIX_ID_027 xxxxx Support RGB limited / Full range convert
    if(it6802->RGBQuantizationRange == 0 )
    {
    if( it6802->VIC >=2 )
    {
    // CE Mode
    it6802->RGBQuantizationRange = 1; // limited range
    }
    else
    {
    // IT mode
    it6802->RGBQuantizationRange = 2; // Full range
    }
    }
//FIX_ID_027     xxxxx

    VIDEOTIMNG_DEBUG_PRINTF(("AVI ColorMode = %X\n",(int) it6802->ColorMode));
    VIDEOTIMNG_DEBUG_PRINTF(("AVI Colorimetry = %X\n",(int) it6802->Colorimetry));
    VIDEOTIMNG_DEBUG_PRINTF(("AVI ExtendedColorimetry = %X\n",(int) it6802->ExtendedColorimetry));
    VIDEOTIMNG_DEBUG_PRINTF(("AVI RGBQuantizationRange = %X\n",(int) it6802->RGBQuantizationRange));
    VIDEOTIMNG_DEBUG_PRINTF(("AVI VIC = %X\n",(int) it6802->VIC));
    VIDEOTIMNG_DEBUG_PRINTF(("AVI YCCQuantizationRange = %X\n",(int) it6802->YCCQuantizationRange));
}


// ---------------------------------------------------------------------------
void SetVideoInputFormatWithInfoFrame(struct it6802_dev_data *it6802)
{
    unsigned char uc;
    unsigned char bAVIColorModeIndicated=FALSE;
//    unsigned char bOldInputVideoMode=it6802->m_bInputVideoMode;

    chgbank(2);
    uc=hdmirxrd(REG_RX_215);    //REG_RX_AVI_DB1
    chgbank(0);
    //FIX_ID_059
    it6802->prevAVIDB[1] = uc;
    //FIX_ID_059
    it6802->m_bInputVideoMode &=~F_MODE_CLRMOD_MASK;


    switch((uc>>O_AVI_COLOR_MODE)&M_AVI_COLOR_MASK)
    {
        case B_AVI_COLOR_YUV444:
            HDMIRX_VIDEO_PRINTF(("input YUV444 mode "));
            it6802->m_bInputVideoMode |=F_MODE_YUV444;
            break;
        case B_AVI_COLOR_YUV422:
            HDMIRX_VIDEO_PRINTF(("input YUV422 mode "));
            it6802->m_bInputVideoMode |=F_MODE_YUV422;
            break;
        case B_AVI_COLOR_RGB24:
            HDMIRX_VIDEO_PRINTF(("input RGB24 mode "));
            it6802->m_bInputVideoMode |=F_MODE_RGB24;
            break;
//FIX_ID_044  add YCbCr420 supporting
        case B_AVI_COLOR_YUV420:
            HDMIRX_VIDEO_PRINTF(("input YUV420 mode "));
            it6802->m_bInputVideoMode |=F_MODE_YUV420;
            break;
//~FIX_ID_044
        default:
            return;
    }

    #if 1    //InfoFrame BUG
    chgbank(2);
//    uc=hdmirxrd(REG_RX_218);
//    if(uc>0) it6802->m_bInputVideoMode |= F_MODE_16_235;
//    else it6802->m_bInputVideoMode &= ~ F_MODE_16_235;
    uc=(hdmirxrd(REG_RX_217)&0x0F)>>2;
    if(uc == 0x01) it6802->m_bInputVideoMode |= F_MODE_16_235;
    else it6802->m_bInputVideoMode &= ~ F_MODE_16_235;
    chgbank(0);
    #endif

    HDMIRX_DEBUG_PRINT(("SetVideoInputFormatWithInfoFrame - RegAE=%X it6802->m_bInputVideoMode=%X\n",(int)uc,(int) it6802->m_bInputVideoMode));
    uc=hdmirxrd(REG_RX_IN_CSC_CTRL);
    uc &=~B_IN_FORCE_COLOR_MODE;
    hdmirxwr(REG_RX_IN_CSC_CTRL,uc);
}

// ---------------------------------------------------------------------------
void SetColorimetryByInfoFrame(struct it6802_dev_data *it6802)
{
    unsigned char i;


//    if(it6802->m_NewAVIInfoFrameF)
    {
    chgbank(2);
    i=hdmirxrd(REG_RX_216);    //REG_RX_AVI_DB2
    chgbank(0);
        i &=M_AVI_CLRMET_MASK<<O_AVI_CLRMET;

        if(i==(B_AVI_CLRMET_ITU601<<O_AVI_CLRMET))
        {
            it6802->m_bInputVideoMode &=~F_MODE_ITU709;
        }
        else if(i==(B_AVI_CLRMET_ITU709<<O_AVI_CLRMET))
        {
            it6802->m_bInputVideoMode |=F_MODE_ITU709;

        }
    }

}

// #if 0
//
//
//
// void DumpCSCReg(void)
// {
//     ushort    i,j;
//     BYTE ucData;
//
//     printf("\n       ");
//     for(j = 0; j < 16; j++)
//     {
//         printf(" %02X",(int) j);
//         if((j == 3)||(j==7)||(j==11))
//         {
//                 printf(" :");
//         }
//     }
//    printf("\n");
//
//     chgbank(1);
//
//     for(i = 0x70; i < 0x90; i+=16)
//     {
//         printf("[%03X]  ",i);
//         for(j = 0; j < 16; j++)
//         {
//             ucData = hdmirxrd((BYTE)((i+j)&0xFF));
//             printf(" %02X",(int) ucData);
//             if((j == 3)||(j==7)||(j==11))
//             {
//                 printf(" :");
//             }
//         }
//         printf("\n");
//
//     }
//
//    printf("\n        =====================================================\n");
//
//     chgbank(0);
//     for(i = 0x60; i < 0x70; i+=16)
//     {
//         printf("[%03X]  ",i);
//         for(j = 0; j < 16; j++)
//         {
//             ucData = hdmirxrd((BYTE)((i+j)&0xFF));
//             printf(" %02X",(int) ucData);
//             if((j == 3)||(j==7)||(j==11))
//             {
//                 printf(" :");
//             }
//         }
//         printf("\n");
//
//     }
//
// }
//
// void IT6802_UpdateCSC(unsigned char *cAVIInfoFrame,unsigned char *cHDMI_DVI_mode,unsigned char *cHDMI_Input_CSC,unsigned char *cRGBQuantData)
// {
//
//     unsigned char ucInputYUV;
//     unsigned char ucITU709;
//     unsigned char ucInputFullRange;
//     //unsigned char ucOutputYUV;
//
//     struct it6802_dev_data *it6802data = get_it6802_dev_data();
//
//     GetAVIInfoFrame(it6802data);
//
//     *cHDMI_Input_CSC = it6802data->ColorMode;
//
//     if((it6802data->ColorMode == 1) || (it6802data->ColorMode == 2))
//     {
//         ucInputYUV=1;
//
//         if((it6802data->YCCQuantizationRange == 1))
//         {
//             ucInputFullRange = 1;
//         }
//         else
//         {
//             ucInputFullRange = 0;
//         }
//
//         if((it6802data->Colorimetry == 2))
//         {
//             ucITU709 = 1;
//             *cHDMI_Input_CSC = 2;    //Receiver_YUV_709;
//         }
//         else
//         {
//             ucITU709 = 0;
//             *cHDMI_Input_CSC = 1;    //Receiver_YUV_601;
//         }
//
//     }
//     else
//     {
//         ucInputYUV=0;
//         *cHDMI_Input_CSC = 0;    //Receiver_RGB;
//
//         if((it6802data->RGBQuantizationRange == 2))
//             ucInputFullRange = 1;
//         else
//             ucInputFullRange = 0;
//     }
//
//     if(ucInputFullRange == 1)
//     {
//         *cRGBQuantData = 2;
//     }
//     else
//     {
//         *cRGBQuantData = 1;
//     }
//
//     if(IsHDMIMode()==TRUE)
//     {
//         *cAVIInfoFrame = TRUE;
//         *cHDMI_DVI_mode  = TRUE;
//     }
//     else
//     {
//         *cAVIInfoFrame = FALSE;
//         *cHDMI_DVI_mode  = FALSE;
//     }
//
// }
//
//
// void SetCSCbyOSD(unsigned char ucInputYUV, unsigned char ucITU709, unsigned char ucFullRange, unsigned char ucOutputYUV)
// {
//     //struct it6802_dev_data *it6802data = get_it6802_dev_data();
//
//     if(ucOutputYUV ==0)
//     {
//         //Convert input RGB / YUV To output RGB444
//         // 0. RGB / YUV
//         // 1. ITU601 / ITU709
//         // 2. Data Range ( Limit / Full )
//         pCSCLookup = CSCOutputRGB[ucInputYUV][ucITU709][ucFullRange];
//         pCSCLookupConfig = &IT6802_OUTPUT_RGB_CONFIG_REG[ucInputYUV][ucFullRange];
//     }
//     else
//     {
//         //Convert input RGB / YUV To output YUV444
//         // 0. RGB / YUV
//         // 1. ITU601 / ITU709
//         // 2. Data Range ( Limit / Full )
//         pCSCLookup = CSCOutputYUV[ucInputYUV][ucITU709][ucFullRange];
//         pCSCLookupConfig = &IT6802_OUTPUT_YUV_CONFIG_REG[ucInputYUV][ucFullRange];
//     }
//
//
//     chgbank(1);
//     hdmirxbwr(REG_RX_170,21,pCSCLookup);
//     chgbank(0);
//     hdmirxset(REG_RX_065,0x33, pCSCLookupConfig->ucReg65);
//     hdmirxwr(REG_RX_067,pCSCLookupConfig->ucReg67);
//     hdmirxwr(REG_RX_068,pCSCLookupConfig->ucReg68);
//
//     if(ucOutputYUV == 1)
//     {
//         if(ucInputYUV == 0)
//         {
//             hdmirxset(REG_RX_065,0x30, 0x10);    // Input RGB Outptu YUV422 with CSC
//         }
//         else
//         {
//             hdmirxset(REG_RX_065,0x33, 0x10);    // Input YUV Outptu YUV422 with bypass CSC
//         }
//
//     }
//
//     DumpCSCReg();
// }
// //xxxxx
// #endif
// ---------------------------------------------------------------------------
void SetColorSpaceConvert(struct it6802_dev_data *it6802)
{
    unsigned char csc;
    //    unsigned char uc;
    unsigned char cscpwd;
    unsigned char filter = 0; // filter is for Video CTRL DN_FREE_GO, EN_DITHER, and ENUDFILT

#ifdef DISABLE_HDMI_CSC
    HDMIRX_VIDEO_PRINTF(("IT680x - HDMI Color Space Convert is disabled\n"));

    csc = B_CSC_BYPASS;
    it6802->m_bOutputVideoMode = it6802->m_bInputVideoMode;
    cscpwd = B_PWD_CSC;
#else
    HDMIRX_VIDEO_PRINTF(("\n!!! SetColorSpaceConvert( ) !!!\n"));
    cscpwd = 0;

//FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
#ifdef _AVOID_REDUNDANCE_CSC_
    if((it6802->m_Backup_OutputVideoMode == it6802->m_bOutputVideoMode) &&  (it6802->m_Backup_InputVideoMode == it6802->m_bInputVideoMode))
    {
        HDMIRX_VIDEO_PRINTF(("I/P and O/P color without change , No need to setup CSC convert again\n"));
        return;
    }
#endif
//FIX_ID_039 xxxxx

    chgbank(1);    /*20231214*//*Reset RGB & YCbCr Offset*/
    hdmirxwr(REG_RX_170,0x10);
    hdmirxwr(REG_RX_171,0x80);
    hdmirxwr(REG_RX_172,0x00);
    chgbank(0);

    //HDMIRX_VIDEO_PRINTF(("Input mode is YUV444 "));
    switch(it6802->m_bOutputVideoMode&F_MODE_CLRMOD_MASK)
    {
    #if defined(SUPPORT_OUTPUTYUV444)
    case F_MODE_YUV444:
         HDMIRX_VIDEO_PRINTF(("Output mode is YUV444\n"));
        switch(it6802->m_bInputVideoMode&F_MODE_CLRMOD_MASK)
        {
        //FIX_ID_044        // Add YUV420 support
        case F_MODE_YUV420:
            HDMIRX_VIDEO_PRINTF(("Input mode is YUV420\n"));
            csc = B_CSC_BYPASS;
            cscpwd = B_PWD_CSC;

            break;
        case F_MODE_YUV444:
             HDMIRX_VIDEO_PRINTF(("Input mode is YUV444\n"));
            csc = B_CSC_BYPASS;
            cscpwd = B_PWD_CSC;

            break;
        case F_MODE_YUV422:
            HDMIRX_VIDEO_PRINTF(("Input mode is YUV422\n"));
            csc = B_CSC_BYPASS;
            if(it6802->m_bOutputVideoMode & F_MODE_EN_UDFILT)// RGB24 to YUV422 need up/dn filter.
            {
                filter |= B_RX_EN_UDFILTER;
            }

            if(it6802->m_bOutputVideoMode & F_MODE_EN_DITHER)// RGB24 to YUV422 need up/dn filter.
            {
                filter |= B_RX_EN_UDFILTER | B_RX_DNFREE_GO;
            }

            break;
        case F_MODE_RGB24:
             HDMIRX_VIDEO_PRINTF(("Input mode is RGB444\n"));
            csc = B_CSC_RGB2YUV;
            break;
        }
        break;
    #endif

    #if defined(SUPPORT_OUTPUTYUV422)

    case F_MODE_YUV422:
        switch(it6802->m_bInputVideoMode&F_MODE_CLRMOD_MASK)
        {
        case F_MODE_YUV444:
             HDMIRX_VIDEO_PRINTF(("Input mode is YUV444\n"));
            if(it6802->m_bOutputVideoMode & F_MODE_EN_UDFILT)
            {
                filter |= B_RX_EN_UDFILTER;
            }
            csc = B_CSC_BYPASS;
            break;
        case F_MODE_YUV422:
             HDMIRX_VIDEO_PRINTF(("Input mode is YUV422\n"));
            csc = B_CSC_BYPASS;

            // if output is YUV422 and 16 bit or 565, then the dither is possible when
            // the input is YUV422 with 24bit input, however, the dither should be selected
            // by customer, thus the requirement should set in ROM, no need to check
            // the register value .
            if(it6802->m_bOutputVideoMode & F_MODE_EN_DITHER)// RGB24 to YUV422 need up/dn filter.
            {
                filter |= B_RX_EN_UDFILTER | B_RX_DNFREE_GO;
            }
            break;
        case F_MODE_RGB24:
             HDMIRX_VIDEO_PRINTF(("Input mode is RGB444\n"));
            if(it6802->m_bOutputVideoMode & F_MODE_EN_UDFILT)// RGB24 to YUV422 need up/dn filter.
            {
                filter |= B_RX_EN_UDFILTER;
            }
            csc = B_CSC_RGB2YUV;
            break;
        }
        break;
    #endif

    #if defined(SUPPORT_OUTPUTRGB)
    case F_MODE_RGB24:
         HDMIRX_VIDEO_PRINTF(("Output mode is RGB24\n"));
        switch(it6802->m_bInputVideoMode&F_MODE_CLRMOD_MASK)
        {
        case F_MODE_YUV444:
             HDMIRX_VIDEO_PRINTF(("Input mode is YUV444\n"));
            csc = B_CSC_YUV2RGB;
            break;
        case F_MODE_YUV422:
             HDMIRX_VIDEO_PRINTF(("Input mode is YUV422\n"));
            csc = B_CSC_YUV2RGB;
            if(it6802->m_bOutputVideoMode & F_MODE_EN_UDFILT)// RGB24 to YUV422 need up/dn filter.
            {
                filter |= B_RX_EN_UDFILTER;
            }
            if(it6802->m_bOutputVideoMode & F_MODE_EN_DITHER)// RGB24 to YUV422 need up/dn filter.
            {
                filter |= B_RX_EN_UDFILTER | B_RX_DNFREE_GO;
            }
            break;
        case F_MODE_RGB24:
             HDMIRX_VIDEO_PRINTF(("Input mode is RGB444\n"));
            csc = B_CSC_BYPASS;
            cscpwd = B_PWD_CSC;

            break;
        }
        break;
    #endif
    }


    #if defined(SUPPORT_OUTPUTYUV)
    // set the CSC associated registers
    if(csc == B_CSC_RGB2YUV)
    {
        // HDMIRX_VIDEO_PRINTF(("CSC = RGB2YUV "));
    //FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
        //default to turn off CSC offset
        hdmirxset(REG_RX_067,0x78,0x00);
        hdmirxwr(REG_RX_068,0x00);
        HDMIRX_VIDEO_PRINTF((" Clear Reg67 and Reg68 ...\n"));
    //FIX_ID_039 xxxxx
        if(it6802->m_bInputVideoMode & F_MODE_ITU709)
        {
            HDMIRX_VIDEO_PRINTF(("ITU709 "));

            if(it6802->m_bInputVideoMode & F_MODE_16_235)
            {
                HDMIRX_VIDEO_PRINTF((" 16-235\n"));
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_RGB2YUV_ITU709_16_235),&bCSCMtx_RGB2YUV_ITU709_16_235[0]);
            }
            else
            {
                HDMIRX_VIDEO_PRINTF((" 0-255\n"));
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_RGB2YUV_ITU709_0_255),&bCSCMtx_RGB2YUV_ITU709_0_255[0]);
            }
        }
        else
        {
            HDMIRX_VIDEO_PRINTF(("ITU601 "));
            if(it6802->m_bInputVideoMode & F_MODE_16_235)
            {
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_RGB2YUV_ITU601_16_235),&bCSCMtx_RGB2YUV_ITU601_16_235[0]);
                HDMIRX_VIDEO_PRINTF((" 16-235\n"));
            }
            else
            {
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_RGB2YUV_ITU601_0_255),&bCSCMtx_RGB2YUV_ITU601_0_255[0]);
                HDMIRX_VIDEO_PRINTF((" 0-255\n"));
            }
        }
    }
    #endif
    #if defined(SUPPORT_OUTPUTRGB)
    if(csc == B_CSC_YUV2RGB)
    {
        HDMIRX_VIDEO_PRINTF(("CSC = YUV2RGB "));
    //FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
        //default to turn off CSC offset
        hdmirxset(REG_RX_067,0x78,0x00);
        hdmirxwr(REG_RX_068,0x00);
        HDMIRX_VIDEO_PRINTF((" Clear Reg67 and Reg68 ...\n"));
    //FIX_ID_039 xxxxx
        if(it6802->m_bInputVideoMode & F_MODE_ITU709)
        {
            HDMIRX_VIDEO_PRINTF(("ITU709 "));
            if(it6802->m_bOutputVideoMode & F_MODE_16_235)
            {
                HDMIRX_VIDEO_PRINTF(("16-235\n"));
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_YUV2RGB_ITU709_16_235),&bCSCMtx_YUV2RGB_ITU709_16_235[0]);
            }
            else
            {
                HDMIRX_VIDEO_PRINTF(("0-255\n"));
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_YUV2RGB_ITU709_0_255),&bCSCMtx_YUV2RGB_ITU709_0_255[0]);
            }
        }
        else
        {
            HDMIRX_VIDEO_PRINTF(("ITU601 "));
            if(it6802->m_bOutputVideoMode & F_MODE_16_235)
            {
                HDMIRX_VIDEO_PRINTF(("16-235\n"));
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_YUV2RGB_ITU601_16_235),&bCSCMtx_YUV2RGB_ITU601_16_235[0]);
            }
            else
            {
                HDMIRX_VIDEO_PRINTF(("0-255\n"));
        chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_YUV2RGB_ITU601_0_255),&bCSCMtx_YUV2RGB_ITU601_0_255[0]);

            }
        }

    }

//FIX_ID_027 xxxxx Support Full/Limited Range convert
    if(csc == B_CSC_BYPASS)
    {

        if((it6802->m_bInputVideoMode&F_MODE_CLRMOD_MASK)==F_MODE_RGB24)
        {
            if(it6802->RGBQuantizationRange == 1)    // Limited range from HDMI source
            {
                 if((it6802->m_bOutputVideoMode & F_MODE_16_235)!=F_MODE_16_235)    // Full range to back-end device
                {
                    // RedText;
                    VIDEOTIMNG_DEBUG_PRINTF((" bCSCMtx_RGB_16_235_RGB_0_255\n"));
                    // printf("pccmd w 65 02 90;\n");
                    // printf("pccmd w 67 78 90;\n");
                    // printf("pccmd w 68 ED 90;\n");
                    // WhileText;
                    csc = B_CSC_RGB2YUV;
                    chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                    hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_RGB_16_235_RGB_0_255),&bCSCMtx_RGB_16_235_RGB_0_255[0]);
                    chgbank(0);
                    //hdmirxset(REG_RX_065,0x03,0x02);    // B_CSC_RGB2YUV
                    hdmirxset(REG_RX_067,0x78,0x78);
                    hdmirxwr(REG_RX_068,0xED);
                    cscpwd = 0;

                 }
            }
            else if(it6802->RGBQuantizationRange == 2) //Full range from HDMI source
            {
                 if((it6802->m_bOutputVideoMode & F_MODE_16_235)==F_MODE_16_235)    // Limited range to back-end device
                {
                    // RedText;
                    VIDEOTIMNG_DEBUG_PRINTF((" bCSCMtx_RGB_0_255_RGB_16_235\n"));
                    // printf("pccmd w 65 02 90;\n");
                    // printf("pccmd w 67 40 90;\n");
                    // printf("pccmd w 68 10 90;\n");
                    // WhileText;
                    csc = B_CSC_RGB2YUV;
                    chgbank(1);    //for CSC setting Reg170 ~ Reg184 !!!!
                    hdmirxbwr(REG_RX_170,sizeof(bCSCMtx_RGB_0_255_RGB_16_235),&bCSCMtx_RGB_0_255_RGB_16_235[0]);
                    chgbank(0);
                    //hdmirxset(REG_RX_065,0x03,0x02);    // B_CSC_RGB2YUV
                    hdmirxset(REG_RX_067,0x78,0x40);
                    hdmirxwr(REG_RX_068,0x10);
                    cscpwd = 0;
                 }
            }
        }
    }
//FIX_ID_027 xxxxx

    #endif // SUPPORT_OUTPUTRGB

#endif    //end of DISABLE_HDMI_CSC

    chgbank(0);
        hdmirxset(REG_RX_OUT_CSC_CTRL,(M_CSC_SEL_MASK),csc);

    // set output Up/Down Filter, Dither control
    hdmirxset(REG_RX_VIDEO_CTRL1,(B_RX_DNFREE_GO|B_RX_EN_DITHER|B_RX_EN_UDFILTER),filter);

//FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
    if(csc == B_CSC_BYPASS)
    {
        //default to turn off CSC offset
        hdmirxset(REG_RX_067,0x78,0x00);
        hdmirxwr(REG_RX_068,0x00);
    }

    hdmirxset(REG_RX_051, B_PWD_CSC, cscpwd);/*20230525*/
    if(it6802->device_version != 0xC0){
        if(cscpwd&B_PWD_CSC)
        {
            hdmirxset(REG_RX_050, 0x03, DCLK_DELAY_CSC_OFF);
        }
        else
        {
            hdmirxset(REG_RX_050, 0x03, DCLK_DELAY_CSC_ON);
        }
    }

#ifdef _AVOID_REDUNDANCE_CSC_
    it6802->m_Backup_OutputVideoMode = it6802->m_bOutputVideoMode;
    it6802->m_Backup_InputVideoMode = it6802->m_bInputVideoMode;
#endif
//FIX_ID_039 xxxxx
}

// ---------------------------------------------------------------------------
void SetNewInfoVideoOutput(struct it6802_dev_data *it6802)
{

    HDMIRX_VIDEO_PRINTF(("SetNewInfoVideoOutput()\n"));

    SetVideoInputFormatWithInfoFrame(it6802);
    SetColorimetryByInfoFrame(it6802);
    SetColorSpaceConvert(it6802);

    SetVideoOutputColorFormat(it6802);    //2013-0502

//    get_vid_info();
//    show_vid_info();

}

// ---------------------------------------------------------------------------
void SetVideoInputFormatWithoutInfoFrame(struct it6802_dev_data *it6802,unsigned char bInMode)
{
    unsigned char i;

    i=hdmirxrd(REG_RX_IN_CSC_CTRL);
    i |=B_IN_FORCE_COLOR_MODE;

    i &=(~M_INPUT_COLOR_MASK);
    it6802->m_bInputVideoMode &=~F_MODE_CLRMOD_MASK;
    #if 1    //InfoFrame BUG
    it6802->m_bInputVideoMode &= ~ F_MODE_16_235;
    #endif
    switch(bInMode)
    {
    case F_MODE_YUV444:
    i |=B_INPUT_YUV444;
    it6802->m_bInputVideoMode |=F_MODE_YUV444;
    break;
    case F_MODE_YUV422:
    i |=B_INPUT_YUV422;
        it6802->m_bInputVideoMode |=F_MODE_YUV422;
        break;
    case F_MODE_RGB24:
    i |=B_INPUT_RGB24;
        it6802->m_bInputVideoMode |=F_MODE_RGB24;
        break;
    default:
        return;
    }
    hdmirxwr(REG_RX_IN_CSC_CTRL,i);

    //FIX_ID_059
    // reset the prevAVIDB while DVI mode.
    it6802->prevAVIDB[0] = 0xFF;
    it6802->prevAVIDB[1] = 0xFF;
    //FIX_ID_059
}
// ---------------------------------------------------------------------------
void SetColorimetryByMode(struct it6802_dev_data *it6802)
{
    unsigned char  RxClkXCNT;
    RxClkXCNT=hdmirxrd(REG_RX_PIXCLK_SPEED);

   HDMIRX_VIDEO_PRINTF((" SetColorimetryByMode() REG_RX_PIXCLK_SPEED=%X\n", (int) RxClkXCNT));

    it6802->m_bInputVideoMode &=~F_MODE_ITU709;

     if(RxClkXCNT <0x34)
    {

        it6802->m_bInputVideoMode |=F_MODE_ITU709;
    }
    else
    {

        it6802->m_bInputVideoMode &=~F_MODE_ITU709;
    }

}
// ---------------------------------------------------------------------------
void SetDVIVideoOutput(struct it6802_dev_data *it6802)
{
    HDMIRX_VIDEO_PRINTF(("SetDVIVideoOutput()\n"));

    SetVideoInputFormatWithoutInfoFrame(it6802,F_MODE_RGB24);
    SetColorimetryByMode(it6802);
    SetColorSpaceConvert(it6802);

    SetVideoOutputColorFormat(it6802);    //2013-0502
}


#define _ENABLE_YCC_BLANK_VALUE_ 1  //oec20260628

void IT6802_VideoOutputModeSet(struct it6802_dev_data *it6802)
{
    unsigned char ucReg51;
    unsigned char ucReg65;
    unsigned char CBL, YBL ;

    HDMIRX_VIDEO_PRINTF(("IT6802_VideoOutputModeSet()\n"));

    HDMIRX_VIDEO_PRINTF((VModeStateStr[(unsigned char)it6802->m_VidOutConfigMode]));
    HDMIRX_VIDEO_PRINTF(("+++ \n"));


    ucReg51 = hdmirxrd(REG_RX_051)&0x9B;    // Reg51 [6] Half PCLK DDR , [5] Half Bus DDR , [2] CCIR656 mode
    ucReg65 = hdmirxrd(REG_RX_065)&0x0F;    // Reg65 [7] BTA1004Fmt , [6] SyncEmb , [5:4] output color 0x00 RGB, 0x10 YUV422, 0x20 YUV444

    CBL=0 ;
    YBL=0 ;
    switch((it6802->m_bOutputVideoMode&F_MODE_CLRMOD_MASK))
    {
        case F_MODE_RGB444:
            ucReg65|=(B_OUTPUT_RGB24);        // 0x00 B_OUTPUT_RGB24
            #ifdef _SUPPORT_RBSWAP_
            hdmirxset(REG_RX_064, 0x18, 0x08);
            #endif
            break;
        case F_MODE_YUV422:
            ucReg65|=(B_OUTPUT_YUV422);        // 0x10 B_OUTPUT_YUV422
            #ifdef _SUPPORT_RBSWAP_
            hdmirxset(REG_RX_064, 0x18, 0x10);
            #endif
            #ifdef _ENABLE_YCC_BLANK_VALUE_
            CBL=0x80 ;
            YBL=0x10 ;
            #endif
            break;
        case F_MODE_YUV444:
            ucReg65|=(B_OUTPUT_YUV444);        // 0x20 B_OUTPUT_YUV444
            #ifdef _SUPPORT_RBSWAP_
            hdmirxset(REG_RX_064, 0x18, 0x10);
            #endif
            #ifdef _ENABLE_YCC_BLANK_VALUE_
            CBL=0x80 ;
            YBL=0x10 ;
            #endif
            break;
    }
    //SPECIAL CASE      ITE_150409
    if(( it6802->m_VidOutDataTrgger == eHalfPCLKDDR) &&
        (it6802->m_VidOutSyncMode !=eCCIR656EmbSync) &&
        (it6802->m_VidOutSyncMode !=eCCIR656SepSync))
    {
        IT6802_HalfPCLKFinetune(it6802);
    }
    if(it6802->device_version == 0xC0){
    // 20240311 DCLK Invert
        if(( it6802->m_VidOutDataTrgger == eHalfPCLKDDR) &&
            ((it6802->m_VidOutSyncMode ==eCCIR656EmbSync) ||
            (it6802->m_VidOutSyncMode ==eCCIR656SepSync)))
        {
            IT6802_DCLKInvertConfigure(1);
        }else{
            IT6802_DCLKInvertConfigure(0);
        }
    }

    if((it6802->m_VidOutSyncMode == eCCIR656EmbSync) ||
       (it6802->m_VidOutSyncMode == eCCIR656SepSync))
    {
        hdmirxset(REG_RX_064, 0x80, 0x80);
        hdmirxset(REG_RX_064, 0x80, 0x00);
    }
    switch(it6802->m_VidOutDataTrgger)
    {
        case eSDR:
            break;

        case eHalfPCLKDDR:
            ucReg51|=(B_HALF_PCLKC);            // 0x40 half PCLK
            break;

        case eHalfBusDDR:
            ucReg51|=(B_OUT_DDR);                // 0x20 half bus
            break;

        case eSDR_BTA1004:
            ucReg65|=(B_BTA1004Fmt|B_SyncEmb);    // 0x80 BTA1004 + 0x40 SyncEmb
            break;

        case eDDR_BTA1004:
            ucReg51|=(B_HALF_PCLKC);            // 0x40 half PCLK
            ucReg65|=(B_BTA1004Fmt|B_SyncEmb);    // 0x80 BTA1004 + 0x40 SyncEmb
            break;

    }

    switch(it6802->m_VidOutSyncMode)
    {
        case eSepSync:
            break;

        case eEmbSync:
            ucReg65|=(B_SyncEmb);    // 0x40 SyncEmb
            break;

        case eCCIR656SepSync:
            ucReg51|=(B_CCIR656);    // 0x04 CCIR656
            break;

        case eCCIR656EmbSync:
            ucReg51|=(B_CCIR656);    // 0x04 CCIR656
            ucReg65|=(B_SyncEmb);    // 0x40 SyncEmb
            break;
    }


    HDMIRX_VIDEO_PRINTF(("Reg51 = %X ",(int) ucReg51));
    HDMIRX_VIDEO_PRINTF(("Reg65 = %X\n",(int) ucReg65));

    hdmirxwr(REG_RX_051,ucReg51);
    hdmirxwr(REG_RX_065,ucReg65);

    chgbank(1) ;
    hdmirxwr(REG_RX_18D, CBL);
    hdmirxwr(REG_RX_18E, YBL);
    chgbank(0) ;

}
//FIX_ID_003 xxxxx
void IT6802_HalfPCLKFinetune(struct it6802_dev_data *it6802) //ITE_150409
{
    unsigned char rddata,OutInMode;
    int PCLK;
    unsigned char C0rev ;

    if (it6802->device_version == 0xC0) C0rev = 1 ;
    else C0rev = 0 ;

    rddata = hdmirxrd(0x9A);
    PCLK=(int)(RCLKVALUE*255/rddata);
    OutInMode = ((it6802->m_bOutputVideoMode&F_MODE_CLRMOD_MASK) << 4)|(it6802->m_bInputVideoMode&F_MODE_CLRMOD_MASK);
    if(PCLK<250)          // Clive
    {
        HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) PCLK Low =================== %d\n",(int) PCLK));
        HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) OutInMode = 0x%x\n",(int) OutInMode));
        HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) REG_RX_050 = 0x%x\n",(int) hdmirxrd(0x50)));
        //RGB24     0
        //YUV422   1
        //YUV444   2
        //YUV420   3
        switch(OutInMode)
        {
            case 0x22://Out + In =>  YUV444 + YUV444
            case 0x23://Out + In =>  YUV444 + YUV420
            case 0x20://Out + In =>  YUV444 + RGB24
            case 0x13://Out + In =>  YUV422 + YUV420
            case 0x11://Out + In =>  YUV422 + YUV422
            case 0x03://Out + In =>  RGB444 + YUV420
            case 0x02://Out + In =>  RGB444 + YUV444
            case 0x00://Out + In =>  RGB444 + RGB24
                      if(!C0rev) {
                          hdmirxset(REG_RX_050, 0x0C, 0x00);
                      }
                      hdmirxset(REG_RX_064, 0x80, 0x80);
                      hdmirxset(REG_RX_064, 0x80, 0x00);
                      break;
            case 0x21://Out + In =>  YUV444 + YUV422
            case 0x12://Out + In =>  YUV422 + YUV444
            case 0x10://Out + In =>  YUV422 + RGB24
            case 0x01://Out + In =>  RGB444 + YUV422
                      if(!C0rev) {
                          hdmirxset(REG_RX_050, 0x1C, 0x00);
                      }
                      hdmirxset(REG_RX_064, 0x80, 0x80);
                      hdmirxset(REG_RX_064, 0x80, 0x00);
                      break;
        }
        if(C0rev) {
            hdmirxset(REG_RX_056, 0x07, 0x06);
        }

        }
    else       // PCLK > 250  run high speed mode.
    {
        HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) PCLK High =================== %d\n",(int) PCLK));
        HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) OutInMode = 0x%x\n",(int) OutInMode));
        HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) REG_RX_050 = 0x%x\n",(int) hdmirxrd(0x50)));
        switch(OutInMode)
        {
            case 0x22://Out + In =>  YUV444 + YUV444
            case 0x23://Out + In =>  YUV444 + YUV420
            case 0x20://Out + In =>  YUV444 + RGB24
            case 0x13://Out + In =>  YUV422 + YUV420
            case 0x11://Out + In =>  YUV422 + YUV422
            case 0x03://Out + In =>  RGB444 + YUV420
            case 0x02://Out + In =>  RGB444 + YUV444
            case 0x00://Out + In =>  RGB444 + RGB24
                      if(!C0rev) {
                          hdmirxset(REG_RX_050, 0x0C, 0x08);
                      }
                      hdmirxset(REG_RX_064, 0x80, 0x80);
                      hdmirxset(REG_RX_064, 0x80, 0x00);
                      break;
            case 0x21://Out + In =>  YUV444 + YUV422
            case 0x12://Out + In =>  YUV422 + YUV444
            case 0x10://Out + In =>  YUV422 + RGB24
            case 0x01://Out + In =>  RGB444 + YUV422
                      if(!C0rev) {
                         hdmirxset(REG_RX_050, 0x1C, 0x08);
                      }
                        hdmirxset(REG_RX_064, 0x80, 0x80);
                        hdmirxset(REG_RX_064, 0x80, 0x00);
                       break;
        }
        if(C0rev) {
            hdmirxset(REG_RX_056, 0x07, 0x04);
        }
    }
    HDMIRX_VIDEO_PRINTF(("IT6803JudgeOutputMode( ) REG_RX_050 = 0x%x\n",(int) hdmirxrd(0x50)));
}

void IT6802_DCLKInvertConfigure(unsigned char IsHalfBusDDR)
{// 20240311
    unsigned char rddata;
    unsigned long PCLK;

    if(!IsHalfBusDDR){
        hdmirxset(REG_RX_050,0x10,0x00);
    }else{
        rddata = hdmirxrd(0x9A);
        PCLK=(unsigned long)(RCLKVALUE*255/rddata);// 20240312
        HDMIRX_VIDEO_PRINTF(("IT6802_DCLKInvertConfigure( ) PCLK = %ld\n",(unsigned long) PCLK));
        // if((PCLK<16)||((PCLK>70)&&(PCLK<90)))
        // {
        //     hdmirxset(REG_RX_050,0x10,0x10);
        // }else{
        //     hdmirxset(REG_RX_050,0x10,0x00);
        // }
        if(PCLK>20){//20240708
            hdmirxset(REG_RX_050,0x10,0x00);
        }else{
            hdmirxset(REG_RX_050,0x10,0x10);
        }
        chgbank(1);
        if(PCLK>100){
            hdmirxset(REG_RX_1B8,0x20,0x00);
        }else{
            hdmirxset(REG_RX_1B8,0x20,0x20);
        }
        chgbank(0);
    }
}



void IT6802VideoOutputConfigure(struct it6802_dev_data *it6802)
{

    // Configure Output color space convert

    it6802->m_bUpHDMIMode =IsHDMIMode();
    if(it6802->m_bUpHDMIMode==FALSE)
    {
        SetDVIVideoOutput(it6802);
        //yjh2026 DVI는 AVI 없어 RGB로 잡힘 -> 카메라가 DVI YUV422라 여기서 YUV422 고정
        SetVideoInputFormatWithoutInfoFrame(it6802, F_MODE_YUV422);
        SetColorSpaceConvert(it6802);
    }
    else
    {
//FIX_ID_027 xxxxx        //Support RGB limited / Full range convert
        GetAVIInfoFrame(it6802);
        SetNewInfoVideoOutput(it6802);
//FIX_ID_027 xxxxx
    }

    it6802->m_NewAVIInfoFrameF=FALSE;

    // Configure Output Color Depth

    it6802->GCP_CD = ((hdmirxrd(0x99)&0xF0)>>4);
    switch( it6802->GCP_CD ) {
    case 5 :
    VIDEOTIMNG_DEBUG_PRINTF(( "\n Output ColorDepth = 30 bits per pixel\n"));
    hdmirxset(0x65, 0x0C, 0x04);
    break;
    case 6 :
    VIDEOTIMNG_DEBUG_PRINTF(( "\n Output ColorDepth = 36 bits per pixel\n"));
    hdmirxset(0x65, 0x0C, 0x08);
    break;
    default :
    VIDEOTIMNG_DEBUG_PRINTF(( "\n Output ColorDepth = 24 bits per pixel\n"));
    hdmirxset(0x65, 0x0C, 0x00);
    break;
    }

    // Configure TTL Video Output mode
    IT6802_VideoOutputModeSet(it6802);

    //yjh2026 컬러검출 확인용 (HDMImode 0=DVI 1=HDMI / AVIcolor 0=RGB 1=422 2=444)
    printf("[RX-FMT] HDMImode=%d AVIcolor=%d InVidMode=0x%02X OutVidMode=0x%02X\r\n",
           (int)it6802->m_bUpHDMIMode, (int)it6802->ColorMode,
           (int)it6802->m_bInputVideoMode, (int)it6802->m_bOutputVideoMode);
}

// ---------------------------------------------------------------------------
void SetVideoOutputColorFormat(struct it6802_dev_data *it6802)
{
    switch(it6802->m_bOutputVideoMode&F_MODE_CLRMOD_MASK) {
        case F_MODE_RGB24 :
            hdmirxset(REG_RX_OUT_CSC_CTRL,(M_OUTPUT_COLOR_MASK),B_OUTPUT_RGB24);
            #ifdef _SUPPORT_RBSWAP_
                hdmirxset(REG_RX_064, 0x18, 0x08);
            #endif

            break;
        case F_MODE_YUV422 :
            hdmirxset(REG_RX_OUT_CSC_CTRL,(M_OUTPUT_COLOR_MASK),B_OUTPUT_YUV422);
            #ifdef _SUPPORT_RBSWAP_
                hdmirxset(REG_RX_064, 0x18, 0x10);
            #endif
            break;
        case F_MODE_YUV444 :
            hdmirxset(REG_RX_OUT_CSC_CTRL,(M_OUTPUT_COLOR_MASK),B_OUTPUT_YUV444);
            #ifdef _SUPPORT_RBSWAP_
                hdmirxset(REG_RX_064, 0x18, 0x08);
            #endif
            break;
    }
}

void it6802PortSelect(unsigned char ucPortSel)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();

#if defined(_IT6802_) || defined(_IT6803_)
    if(ucPortSel == F_PORT_SEL_0 )
        hdmirxset(REG_RX_051, B_PORT_SEL, F_PORT_SEL_0); //select port 0
    else
        hdmirxset(REG_RX_051, B_PORT_SEL, F_PORT_SEL_1); //select port 1
#else
    hdmirxset(REG_RX_051, B_PORT_SEL, F_PORT_SEL_0); //select port 0
    it6802data->m_ucCurrentHDMIPort = F_PORT_SEL_0;
#endif

    if(it6802data->m_ucCurrentHDMIPort != ucPortSel)
    {
        IT6802SwitchVideoState(it6802data,VSTATE_SyncWait);
        it6802data->m_ucCurrentHDMIPort = ucPortSel;
        it6802data->prevAVIDB[0]=0xFF;
        it6802data->prevAVIDB[1]=0xFF;
        VIDEOTIMNG_DEBUG_PRINTF(("it6802PortSelect = %X\n",(int) ucPortSel));
    }

}

void it6802HPDCtrl(unsigned char ucport,unsigned char ucEnable)
{

    struct it6802_dev_data *it6802data = get_it6802_dev_data();
    HDMIRX_VIDEO_PRINTF(("it6802HPDCtrl(%bd,%bd);\n",ucport,ucEnable)) ;
    if(ucport == 0)
    {
        if(ucEnable == 0)
        {

            // Disable HDMI DDC Bus to access IT680X EDID RAM


//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
//FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
            if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
            {
                //IT6802_VBusEnableByOSD(FALSE);
                if(it6802data->m_DiscoveryDone != 0)
                {
                    if( mscFire(MxL_MSC_CtrlPacket0, B_CLR_HPD) == SUCCESS )
                    {
                        HDMIRX_VIDEO_PRINTF(("Port 0 HPD MxL 00000\n"));
                    }

                    //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
                    return;
                    //FIX_ID_037 xxxxx
                }
                HDMIRX_VIDEO_PRINTF(("[MxL] Port 0 HPD Tri-State\n"));
                chgbank(1);
                hdmirxset(REG_RX_1B0, 0x03, 0x00); //clear port 0 HPD=1 for EDID update
                chgbank(0);

            } // B_P0_MxL_MODE
//FIX_ID_034 xxxxx
//FIX_ID_036    xxxxx
        // FIX_ID_048
            else // HDMI mode.
 #endif
            {
                if((hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_PWR5V_DET))
                {
                    HDMIRX_VIDEO_PRINTF(("Port 0 HPD HDMI 00000\n"));
                    chgbank(1);
                    hdmirxset(REG_RX_1B0, 0x03, 0x01); //clear port 0 HPD=1 for EDID update
                    chgbank(0);
                }
                else
                {
                    HDMIRX_VIDEO_PRINTF(("Port 0 HPD Tri-State\n"));
                    chgbank(1);
                    hdmirxset(REG_RX_1B0, 0x03, 0x00); //clear port 0 HPD=1 for EDID update
                    chgbank(0);
                }
            } // not B_P0_MxL_MODE
        //~FIX_ID_048

        }
        else
        {


            if((hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_PWR5V_DET))
            {


                // Enable HDMI DDC bus to access IT680X EDID RAM

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
//FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
                if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
                {

                    if(it6802data->m_DiscoveryDone != 0)
                    {
                        if( mscFire(MxL_MSC_CtrlPacket0, B_SET_HPD) == SUCCESS )
                        {
                            HDMIRX_VIDEO_PRINTF(("Port 0 HPD MxL 11111\n"));

                        }

                        return;
                    }
                    HDMIRX_VIDEO_PRINTF(("[MxL] Port 0 HPD Tri-State\n"));
                    chgbank(1);
                    hdmirxset(REG_RX_1B0, 0x03, 0x00); //clear port 0 HPD=1 for EDID update
                    chgbank(0);
                } // B_P0_MxL_MODE
                else
//FIX_ID_034 xxxxx
 #endif
//FIX_ID_036    xxxxx
            // FIX_ID_048 // only HDMI mode reply HPD pull high
                {
                    HDMIRX_VIDEO_PRINTF(("Port 0 HPD HDMI 11111\n"));
                    chgbank(1);
                    hdmirxset(REG_RX_1B0, 0x03, 0x03); //set port 0 HPD=1
                    chgbank(0);
                } // not B_P0_MxL_MODE
            //~FIX_ID_048
            } // ((hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_PWR5V_DET))
            else
            {
                HDMIRX_VIDEO_PRINTF(("Port 0 HPD but HPD Tristate by 5V.\n"));
                chgbank(1);
                hdmirxset(REG_RX_1B0, 0x03, 0x00); //set port 0 Tri-State
                chgbank(0);
            } // not ((hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_PWR5V_DET))
        } // if(ucEnable == 0)
    } // if(ucport == 0)
#if defined(_IT6802_) || defined(_IT6803_)
    else
    {
         if(ucEnable)
         {
            // Enable HDMI DDC bus to access IT680X EDID RAM

            HDMIRX_VIDEO_PRINTF(("Port 1 HPD 11111\n"));
             gpHPD0 = HPDON;
         }
         else
         {
            // Disable HDMI DDC Bus to access IT680X EDID RAM

            HDMIRX_VIDEO_PRINTF(("Port 1 HPD 00000\n"));
             gpHPD0 = HPDOFF;
         }
    }
#endif
}


void hdmirx_ECCTimingOut(unsigned char ucport)
{
    IT6802_DEBUG_INT_PRINTF(("CDR reset for hdmirx_ECCTimingOut()\n"));

    if(ucport == F_PORT_SEL_0)
    {

        it6802HPDCtrl(0,0);    // MxL port , set HPD = 0

        hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST|B_P0_HDCPRST|B_P0_SWRST),(B_P0_DCLKRST|B_P0_CDRRST|B_P0_HDCPRST|B_P0_SWRST));
        delay1ms(300);
        hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST|B_P0_HDCPRST|B_P0_SWRST),0x00);

        //set port 0 HPD=1
        it6802HPDCtrl(0,1);    // MxL port , set HPD = 1

    }
    else
    {
        //set port 1 HPD=0
        it6802HPDCtrl(1,0);    // HDMI port , set HPD = 0


        hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST|B_P1_HDCPRST|B_P1_SWRST),(B_P1_DCLKRST|B_P1_CDRRST|B_P1_HDCPRST|B_P1_SWRST));
        delay1ms(300);
        hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST|B_P1_HDCPRST|B_P1_SWRST),0x00);

        //set port 1 HPD=1
        it6802HPDCtrl(1,1);    // HDMI port , set HPD = 1
    }
}

#endif


#ifdef _IT680x_
// ***************************************************************************
// Audio function
// ***************************************************************************
void aud_fiforst( void )
{
    unsigned char uc;

    hdmirxset(REG_RX_010, 0x02, 0x02);
    hdmirxset(REG_RX_010, 0x02, 0x00);
    uc = hdmirxrd(REG_RX_07B);
    hdmirxwr(REG_RX_07B, uc);
    hdmirxwr(REG_RX_07B, uc);
    hdmirxwr(REG_RX_07B, uc);
    hdmirxwr(REG_RX_07B, uc);
}


#ifdef _FIX_ID_028_
// ---------------------------------------------------------------------------
//FIX_ID_028 xxxxx //For Debug Audio error with S2
void IT6802AudioOutputEnable(unsigned char bEnable)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    m_bAudioWaiting=FALSE;


    hdmirxset(REG_RX_HWMuteCtrl,(B_HWMuteClr),(B_HWMuteClr));
    hdmirxset(REG_RX_HWMuteCtrl,(B_HWAudMuteClrMode),(B_HWAudMuteClrMode));
    hdmirxset(REG_RX_HWMuteCtrl,(B_HWMuteClr),(0));
    hdmirxset(REG_RX_HWMuteCtrl,(B_HWAudMuteClrMode),(0));
    aud_fiforst();

    if(bEnable==TRUE)
    {
        hdmirxset(REG_RX_052,(B_TriI2SIO|B_TriSPDIF),0x00);
            IT6802_SwitchAudioState(it6802, ASTATE_AudioOn) ;


        HDMIRX_AUDIO_PRINTF((" === IT6802AudioOutputEnable 11111111111 ====\n"));
    }
    else
    {
        hdmirxset(REG_RX_052,(B_TriI2SIO|B_TriSPDIF),(B_TriI2SIO|B_TriSPDIF));
        IT6802_SwitchAudioState(it6802, ASTATE_AudioOff) ;
        HDMIRX_AUDIO_PRINTF((" === IT6802AudioOutputEnable 00000000000 ====\n"));
    }
}
//FIX_ID_028 xxxxx

#else

// ---------------------------------------------------------------------------
void IT6802AudioOutputEnable(unsigned char bEnable)
{
    if(bEnable==TRUE)
    {
        // 2024/11/18 added by jj
        aud_fiforst();
        //~jjtseng
        hdmirxset(REG_RX_052,(B_TriI2SIO|B_TriSPDIF),0x00);
    #ifdef AUDIO_OFF_PULL_LOW
        hdmirxset(REG_RX_RST_CTRL, B_AUDRST, 0);
    #endif
    }
    else
    {
#ifdef EnableCalFs
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
        m_u16TMDSCLK = 0;
        m_AudioChannelStatusErrorCount = 0;
        hdmirxset(REG_RX_074,0x40,0x00);    // reg74[6]=0 disable Force FS mode
//FIX_ID_023 xxxxx
#endif

    #ifndef AUDIO_OFF_PULL_LOW
        hdmirxset(REG_RX_052,(B_TriI2SIO|B_TriSPDIF),(B_TriI2SIO|B_TriSPDIF));
    #else
        hdmirxset(REG_RX_052,(B_TriI2SIO|B_TriSPDIF),(0));
        hdmirxset(REG_RX_RST_CTRL, B_AUDRST, B_AUDRST);
    #endif
    }
}
#endif
// ---------------------------------------------------------------------------

void hdmirx_ResetAudio(void)
{
    unsigned char uc;
    hdmirxset(REG_RX_RST_CTRL,B_AUDRST,B_AUDRST);
    hdmirxset(REG_RX_RST_CTRL,B_AUDRST,0x00);
    uc = hdmirxrd(REG_RX_07B);
    hdmirxwr(REG_RX_07B, uc);
    hdmirxwr(REG_RX_07B, uc);
    hdmirxwr(REG_RX_07B, uc);
    hdmirxwr(REG_RX_07B, uc);
}

// ---------------------------------------------------------------------------
void hdmirx_SetHWMuteClrMode(void)
{
    hdmirxset(REG_RX_HWMuteCtrl,(B_HWAudMuteClrMode),(B_HWAudMuteClrMode));
}
// ---------------------------------------------------------------------------
void hdmirx_SetHWMuteClr(void)
{
    hdmirxset(REG_RX_HWMuteCtrl,(B_HWMuteClr),(B_HWMuteClr));
}
// ---------------------------------------------------------------------------
void hdmirx_ClearHWMuteClr(void)
{
    hdmirxset(REG_RX_HWMuteCtrl,(B_HWMuteClr),0);
}



// ---------------------------------------------------------------------------
void getHDMIRXInputAudio(AUDIO_CAPS *pAudioCaps)
{

    unsigned char uc;

    uc = hdmirxrd(REG_RX_0AE);    // REG_RX_AUD_CHSTAT3
    pAudioCaps->SampleFreq = uc & M_FS;

    uc = hdmirxrd(REG_RX_0AA);    //REG_RX_AUDIO_CH_STAT
    pAudioCaps->AudioFlag = uc & 0xF0;
    pAudioCaps->AudSrcEnable=uc&M_AUDIO_CH;
    pAudioCaps->AudSrcEnable|=hdmirxrd(REG_RX_0AA)&M_AUDIO_CH;

    if( (uc & (B_HBRAUDIO|B_DSDAUDIO)) == 0)
    {
        uc = hdmirxrd(REG_RX_0AB);    //REG_RX_AUD_CHSTAT0

        if( (uc & B_NLPCM ) == 0 )
        {
            pAudioCaps->AudioFlag |= B_CAP_LPCM;
        }
    }

#ifdef EnableCalFs
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
    if( hdmirxrd(REG_RX_074) & 0x40)
    {
        AudioFsCal();
    }
//FIX_ID_023 xxxxx
#endif
}


#ifdef EnableCalFs
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
// ---------------------------------------------------------------------------
void DumpNCTSReg(void)
{
#if 0
    ushort    i,j;
    BYTE ucData;

    HDMIRX_AUDIO_PRINTF(("\n       "));
    for(j = 0; j < 16; j++)
    {
        HDMIRX_AUDIO_PRINTF((" %02X",(int) j));
        if((j == 3)||(j==7)||(j==11))
        {
                HDMIRX_AUDIO_PRINTF((" :"));
        }
    }
   HDMIRX_AUDIO_PRINTF(("\n"));

    chgbank(2);

    for(i = 0xB0; i < 0xD0; i+=16)
    {
        HDMIRX_AUDIO_PRINTF(("[%03X]  ",i));
        for(j = 0; j < 16; j++)
        {
            ucData = hdmirxrd((BYTE)((i+j)&0xFF));
            HDMIRX_AUDIO_PRINTF((" %02X",(int) ucData));
            if((j == 3)||(j==7)||(j==11))
            {
                HDMIRX_AUDIO_PRINTF((" :"));
            }
        }
        HDMIRX_AUDIO_PRINTF(("\n"));

    }

   HDMIRX_AUDIO_PRINTF(("\n        =====================================================\n"));

    chgbank(0);
#endif
}

// ---------------------------------------------------------------------------
unsigned int PCLKGet(void)
{
    unsigned char rddata;
    unsigned int PCLK;    //, sump;


    rddata = hdmirxrd(0x9A);
    // 2024/8/15 modified by jjtseng
    // m_ROSCLK = 12xxx~14xxx, * 255 will over the 16bit range!!
    // PCLK = (m_ROSCCLK * 255/rddata)/1000;

    PCLK = m_ROSCCLK / 100 ;
    PCLK *= 255 ;
    PCLK /= rddata ;
    PCLK /= 10;

    //~jjtseng 2024/8/15
    //HDMIRX_AUDIO_PRINTF(("PCLKGet( ) PCLK = %ld\n",(int) PCLK));

    return PCLK;

}

// ---------------------------------------------------------------------------
void TMDSGet(void)
{
    unsigned char ucCurrentPort;
    unsigned int ucTMDSClk=0;
    unsigned char rddata;
    unsigned char ucClk;


    ucCurrentPort = hdmirxrd(REG_RX_051) & B_PORT_SEL;


    if(ucCurrentPort == F_PORT_SEL_1)
    {
        ucClk = hdmirxrd(REG_RX_092);
        rddata = hdmirxrd(0x90);

        if(ucClk != 0)
        {

            // 2024/08/15 modified by jjtseng
            // if( rddata&0x04 )
            //     ucTMDSClk=2*RCLKVALUE*256/ucClk;
            // else if( rddata&0x08 )
            //     ucTMDSClk=4*RCLKVALUE*256/ucClk;
            // else
            //     ucTMDSClk=RCLKVALUE*256/ucClk;

            ucTMDSClk=m_ROSCCLK;
            ucTMDSCLK /= 100 ;
            ucTMDSCLK *=256;
            ucTMDSCLK /= ucClk ;
            ucTMDSCLK /= 10 ;

            if( rddata&0x04 )
                ucTMDSClk *= 2 ;
            else if( rddata&0x08 )
                ucTMDSClk= *= 4 ;


            //~jjtseng 2024/08/15


            HDMIRX_AUDIO_PRINTF((" TMDSGet() Port 1 TMDS org  = %d\n",(int) m_u16TMDSCLK));
        }
    }
    else
    {

        if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))    // judge MxL mode
        {
            ucTMDSClk = PCLKGet();
            //HDMIRX_AUDIO_PRINTF(("MxL use Pclk to calculate FS %d\n",(int) ucTMDSClk));
        }
        else        //else HDMI mode
        {
        ucClk = hdmirxrd(REG_RX_091);
        rddata = hdmirxrd(0x90);

        if(ucClk != 0)
        {
            // 2024/08/15 modified by jjtseng
            // if( rddata&0x01 )
            //     ucTMDSClk=2*RCLKVALUE*256/ucClk;
            // else if( rddata&0x02 )
            //     ucTMDSClk=4*RCLKVALUE*256/ucClk;
            // else
            //     ucTMDSClk=RCLKVALUE*256/ucClk;

            ucTMDSClk=m_ROSCCLK;
            ucTMDSCLK /= 100 ;
            ucTMDSCLK *=256;
            ucTMDSCLK /= ucClk ;
            ucTMDSCLK /= 10 ;

            if( rddata&0x04 )
                ucTMDSClk *= 2 ;
            else if( rddata&0x08 )
                ucTMDSClk= *= 4 ;

            //~jjtseng 2024/08/15

                //HDMIRX_AUDIO_PRINTF(("HDMI use TMDS to calculate FS %d\n",(int) ucTMDSClk));
            }
        }

        HDMIRX_AUDIO_PRINTF((" TMDSGet() Port 0 TMDS org  = %d\n",(int) m_u16TMDSCLK));
    }

    if(m_u16TMDSCLK==0)
    {
    m_u16TMDSCLK = ucTMDSClk;
    }
    else
    {
    m_u16TMDSCLK = (( ucTMDSClk+m_u16TMDSCLK ));
    m_u16TMDSCLK /=2;
    }
}


// ---------------------------------------------------------------------------
char m_prevForceFsValue=0xF;
void AudioFsCal(void)
{

    unsigned long u32_N;
    unsigned long u32_CTS;
    unsigned char u8_FS;
    unsigned char uc;
    unsigned long sum=0;
    unsigned char resetAudio=0;

    hdmirxset(REG_RX_077,0x01,0x01);
    chgbank(2);
    u32_N = (unsigned long)(hdmirxrd(REG_RX_2C0) & 0x0F);
    u32_N += (unsigned long)hdmirxrd(REG_RX_2BF)<<4;
    u32_N += (unsigned long)hdmirxrd(REG_RX_2BE)<<12;


    u32_CTS = (unsigned long)((hdmirxrd(REG_RX_2C0) & 0xF0)>>4);
    u32_CTS += (unsigned long)hdmirxrd(REG_RX_2C2)<<4;
    u32_CTS += (unsigned long)hdmirxrd(REG_RX_2C1)<<12;
    chgbank(0);


    DumpNCTSReg();
    HDMIRX_AUDIO_PRINTF((" u32_N %ld\n",(unsigned long) u32_N));
    HDMIRX_AUDIO_PRINTF((" u32_CTS %ld\n",(unsigned long) u32_CTS));

    if((u32_N == 0) || (u32_CTS == 0))
        return;

    TMDSGet();

//    u8_FS=(unsigned char ) (((m_u16TMDSCLK*1000)*u32_N)/(128*u32_CTS));
    sum = (u32_N*1000*m_u16TMDSCLK);
    u8_FS=(unsigned char ) (sum/(128*u32_CTS));

#if 0
    u8_FS = m_u16TMDSCLK;
    HDMIRX_AUDIO_PRINTF(("m_u16TMDSCLK %d\n",(int) u8_FS));
    u8_FS = (((u32_N*m_u16TMDSCLK*78)/u32_CTS)/10);
#endif
    HDMIRX_AUDIO_PRINTF(("u8_FS %d\n",(int) u8_FS));
    m_prevForceFsValue = m_ForceFsValue;

    //Judge FS by FS calulate
    if(u8_FS>25 && u8_FS<=38)
    {
        // FS=32k , Calu Value = 29k~36k
        m_ForceFsValue = (B_32K);
    }
    else if(u8_FS>38 && u8_FS<=46)
    {
        // FS=44k , Calu Value = 41k~46k
        m_ForceFsValue = (B_44P1K);
    }
    else if(u8_FS>46 && u8_FS<=58)
    {
        // FS=48k , Calu Value = 47k~51k
        m_ForceFsValue = (B_48K);
    }
    else if(u8_FS>78 && u8_FS<=92)
    {
        // FS=88k , Calu Value = 85k~91k
        m_ForceFsValue = (B_88P2K);
    }
    else if(u8_FS>92 && u8_FS<=106)
    {
        // FS=96k , Calu Value = 93k~99k
        m_ForceFsValue = (B_96K);
    }
    else if(u8_FS>166 && u8_FS<=182)
    {
        // FS=176k , Calu Value = 173k~179k
        m_ForceFsValue = (B_176P4K);
    }
    else if(u8_FS>182 && u8_FS<=202)
    {
        // FS=192k , Calu Value = 188k~195k
        m_ForceFsValue = (B_192K);
    }


    uc = hdmirxrd(REG_RX_0AE);    // REG_RX_AUD_CHSTAT3
    HDMIRX_AUDIO_PRINTF(("REG_RX_0AE %x ,",(int) ( uc & M_FS) ));
    HDMIRX_AUDIO_PRINTF(("m_ForceFsValue %x\n",(int) ( m_ForceFsValue) ));
    if(( uc & M_FS) == ( m_ForceFsValue ))
    {
        m_AudioChannelStatusErrorCount=0;
        // no need to enable Force FS mode
        HDMIRX_AUDIO_PRINTF(("CHS_FS %x , !!!No need !!! to enable Force FS mode\n",(int) ( uc & M_FS) ));
        if( hdmirxrd(REG_RX_074) & 0x40)
        {
            resetAudio = 1;
        }
        hdmirxset(REG_RX_074,0x40,0x00);    // reg74[6]=0 disable Force FS mode
        if( resetAudio == 1 )
        {
            hdmirx_ResetAudio();
        }
        return;
    }

    HDMIRX_AUDIO_PRINTF(("m_AudioChannelStatusErrorCount=%bd\n",m_AudioChannelStatusErrorCount));

    if(m_AudioChannelStatusErrorCount>MAX_AUDIO_CHANNEL_STATUS_ERROR)
    {
        m_AudioChannelStatusErrorCount=0;
        m_ForceFsValue &= 0xF;
        m_ForceFsValue |= hdmirxrd(REG_RX_07B)  & 0xF0;
    // a. if find Audio Error in a period timers,assue the FS message is wrong,then try to force FS setting.
    // b. set Reg0x74[6]=1=> select Force FS mode.
    hdmirxset(REG_RX_074,0x40,0x40);    // reg74[6]=1
    // f. set Reg0x7e[3:0]=0 (at leasst three times)=> force FS value
    // g. if Audio still Error,then repeat b~f setps.(on f setp,set another FS value
    // 0:44,1K,2: 48K,3:32K,8:88.2K,A:96K,C:176.4K,E:192K)
        hdmirxwr(REG_RX_07B,m_ForceFsValue);
        // hdmirxwr(REG_RX_07B,m_ForceFsValue);
        // hdmirxwr(REG_RX_07B,m_ForceFsValue);
        // hdmirxwr(REG_RX_07B,m_ForceFsValue);
        hdmirx_ResetAudio();
        HDMIRX_AUDIO_PRINTF(("ForceAudio\n",(int) ( uc & M_FS) ));

        HDMIRX_AUDIO_PRINTF(("CHS_FS %x , !!! Enable Force FS mode !!!\n",(int) ( uc & M_FS) ));
    }

}
//FIX_ID_023 xxxxx
#endif


// ---------------------------------------------------------------------------

void IT6802SwitchAudioState(struct it6802_dev_data *it6802,Audio_State_Type state)
{
//    unsigned char uc;

    if( it6802->m_AState == state )
    {
        return;
    }

    HDMIRX_AUDIO_PRINTF((AStateStr[(unsigned char)state]));
    HDMIRX_AUDIO_PRINTF(("+++\n"));

    it6802->m_AState=state;
    //AssignAudioVirtualTime();

#ifdef _FIX_ID_028_
    // if under FIX_ID_028, do nothing on switch state.
    #ifdef _SUPPORT_HDMI_REPEATER_
    if( it6802->m_AState != ASTATE_AudioOn)
    {
        IT680X_DownStream_AudioReady(OFF) ;
    }
    else
    {
        IT680X_DownStream_AudioReady(it6802, ON) ;
    }
    #endif
#else
    #ifdef _SUPPORT_HDMI_REPEATER_
    if( it6802->m_AState != ASTATE_AudioOn)
    {
        IT680X_DownStream_AudioReady(it6802, OFF) ;

    }
    #endif
    switch(it6802->m_AState)
    {
    case ASTATE_AudioOff:
        hdmirxset(REG_RX_RST_CTRL, B_AUDRST, B_AUDRST);
        IT6802AudioOutputEnable(OFF);

        break;
    case ASTATE_RequestAudio:
        IT6802AudioOutputEnable(OFF);

        break;

    case ASTATE_WaitForReady:
        hdmirx_SetHWMuteClr();
        hdmirx_ClearHWMuteClr();
        it6802->m_AudioCountingTimer = AUDIO_READY_TIMEOUT;
        break;

    case ASTATE_AudioOn:
#ifdef EnableCalFs
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
        AudioFsCal();
//FIX_ID_023 xxxxx
#endif
        IT6802AudioOutputEnable(ON);

        HDMIRX_AUDIO_PRINTF(("Cat6023 Audio--> Audio flag=%02X,Ch No=%02X,Fs=%02X ...\n",
                                (int)it6802->m_RxAudioCaps.AudioFlag,
                                (int)it6802->m_RxAudioCaps.AudSrcEnable,
                                (int)it6802->m_RxAudioCaps.SampleFreq));
        #ifdef _SUPPORT_HDMI_REPEATER_
        IT680X_DownStream_AudioReady(it6802, ON) ;
        #endif

        break;
    }
#endif

}


#ifdef _FIX_ID_028_
//FIX_ID_028 xxxxx //For Debug Audio error with S2
//remove --> static void IT6802AudioHandler(struct it6802_dev_data *it6802)
//remove --> {
//remove --> }
//FIX_ID_028 xxxxx
#else
void IT6802AudioHandler(struct it6802_dev_data *it6802)
{
//    unsigned char uc;

   if(it6802->m_AudioCountingTimer > MS_LOOP)
   {
   it6802->m_AudioCountingTimer -= MS_LOOP;
   }
   else
   {
   it6802->m_AudioCountingTimer = 0;
   }


   if(it6802->m_RxHDCPState==RxHDCP_ModeCheck)
       return;

    switch(it6802->m_AState)
    {
    case ASTATE_RequestAudio:

        getHDMIRXInputAudio(&(it6802->m_RxAudioCaps));

        if(it6802->m_RxAudioCaps.AudioFlag & B_CAP_AUDIO_ON)
        {

        hdmirxset(REG_RX_MCLK_CTRL,M_MCLKSel,B_256FS);

            if(it6802->m_RxAudioCaps.AudioFlag& B_CAP_HBR_AUDIO)
            {
                HDMIRX_AUDIO_PRINTF(("+++++++++++ B_CAP_HBR_AUDIO +++++++++++++++++\n"));

                hdmirxset(REG_RX_077, 0x0C, 0x00);/*20230221*/
                hdmirxset(REG_RX_07A, 0x30, 0x30);/*20230221*/
                //2024/11/29 modified by ITE
                hdmirxset(REG_RX_07E, 0x30, 0x00);/*20241129*/// to keep reg7E[6] value in initial table.
                //~ITE 2024/11/29

                hdmirxset(REG_RX_MCLK_CTRL,M_MCLKSel,B_128FS);    // MCLK = 128fs only for HBR audio

                hdmirx_SetHWMuteClrMode();
                hdmirx_ResetAudio();
            }
            else if(it6802->m_RxAudioCaps.AudioFlag& B_CAP_DSD_AUDIO )
            {
                // 2025/02/11 resume reg77[3:2] setting by HBR
                // {REG_RX_077, 0x0C, 0x08},        // Reg77[3:2] = 01    Audio lock method select
                hdmirxset(REG_RX_077, 0x0C, 0x08);
                //~2025/02/11 jjtseng

                // 2025/02/11 resume reg7A[5:4] setting
                // hdmirxset(REG_RX_07A, 0x80, 0x00);/*20230221*/
                // {REG_RX_07A,B_CTS_RES,B_CTS_RES},
                hdmirxset(REG_RX_07A, 0x80+0x30, 0x00+B_CTS_RES);/*20230221*/
                //~2025/02/11


                hdmirxwr(REG_RX_07E, 0x70);/*20230221*/

                hdmirx_SetHWMuteClrMode();
                hdmirx_ResetAudio();
            }
            else
            {

                // 2025/02/11 resume reg77[3:2] setting by HBR
                // {REG_RX_077, 0x0C, 0x08},        // Reg77[3:2] = 01    Audio lock method select
                hdmirxset(REG_RX_077, 0x0C, 0x08);
                //~2025/02/11 jjtseng

                // 2025/02/11 resume reg7A[5:4] setting
                // {REG_RX_07A,B_CTS_RES,B_CTS_RES},
                hdmirxset(REG_RX_07A, 0x30, B_CTS_RES);
                //~2025/02/11

                //2024/11/29 modified by ITE
                hdmirxset(REG_RX_07E, 0x30, 0x00);/*20241129*/ // to keep reg7E[6] value in initial table.
                //~ITE 2024/11/29
                hdmirxset(REG_RX_HWMuteCtrl,B_HWMuteClr,0x00);
                hdmirx_SetHWMuteClrMode();
                hdmirx_ResetAudio();

            }

            IT6802SwitchAudioState(it6802,ASTATE_WaitForReady);

        }
        break;

    case ASTATE_WaitForReady:

    //if(AudioTimeOutCheck(AUDIO_READY_TIMEOUT))
#ifdef EnableCalFs
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
    TMDSGet();
//FIX_ID_023 xxxxx
#endif
    if(it6802->m_AudioCountingTimer==0)
        {
            IT6802SwitchAudioState(it6802,ASTATE_AudioOn);
        }
        break;

    case ASTATE_AudioOn:
    //if(AudioTimeOutCheck(AUDIO_MONITOR_TIMEOUT)==TRUE)
    if(it6802->m_AudioCountingTimer==0)
    {
        AUDIO_CAPS CurAudioCaps;
        //it6802->m_AudioCountingTimer = GetCurrentVirtualTime();
        //AssignAudioTimerTimeout(AUDIO_MONITOR_TIMEOUT);
        it6802->m_AudioCountingTimer = AUDIO_MONITOR_TIMEOUT;

        getHDMIRXInputAudio(&CurAudioCaps);

            if(it6802->m_RxAudioCaps.AudioFlag != CurAudioCaps.AudioFlag
               || it6802->m_RxAudioCaps.AudSrcEnable != CurAudioCaps.AudSrcEnable
               || it6802->m_RxAudioCaps.SampleFreq != CurAudioCaps.SampleFreq)
            {
            //it6802->m_ucHDMIAudioErrorCount=0;
            IT6802SwitchAudioState(it6802,ASTATE_RequestAudio);
            }
    }
        break;
    }
}

#endif

#endif
#ifdef _IT680x_
// ***************************************************************************
// Video function
// ***************************************************************************


void IT6802_AFE_Rst( void )
{

//FIX_ID_008 xxxxx    //Add SW reset when HDMI / MxL device un-plug  !!!

    unsigned char Reg51h;

    struct it6802_dev_data *it6802data = get_it6802_dev_data();        //2013-0814

    chgbank(0);
    Reg51h = hdmirxrd(0x51);
    if( Reg51h&0x01 )
    {
        MxLRX_DEBUG_PRINTF(("=== port 1 IT6802_AFE_Rst() ===\n"));
        hdmirxset(REG_RX_018, 0x01, 0x01);
        delay1ms(1);
        hdmirxset(REG_RX_018, 0x01, 0x00);
        #ifdef _SUPPORT_AUTO_EQ_
        DisableOverWriteRS(1);    //2013-1129
        #endif
    }
    else
    {
        MxLRX_DEBUG_PRINTF(("=== port 0 IT6802_AFE_Rst() ===\n"));
        hdmirxset(REG_RX_011, 0x01, 0x01);
        delay1ms(1);
        hdmirxset(REG_RX_011, 0x01, 0x00);
        #ifdef _SUPPORT_AUTO_EQ_
        DisableOverWriteRS(0);    //2013-1129 for MxL unplug detected
        #endif


        // move to DisableOverWriteRS( )
        ////FIX_ID_019    xxxxx modify ENHYS control for MxL mode
        //chgbank(1);
        //hdmirxset(REG_RX_1B8,0x80,0x00);    // [7] Reg_HWENHYS = 0
        //hdmirxset(REG_RX_1B6,0x07,0x03);    // [2:0]Reg_P0_ENHYS = 03
        //chgbank(0);
        ////FIX_ID_019    xxxxx

    }

    it6802data->m_ucSCDTOffCount=0;    //2013-0814

//FIX_ID_008 xxxxx
}



//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
//xxxxx 2014-0529 //HDCP Content On/Off
void IT6802_ManualVideoTristate(unsigned char bOff)
{
    if(bOff)
    {
    hdmirxset(REG_RX_053,(B_VDGatting|B_VIOSel),(B_VDGatting|B_VIOSel));    //Reg53[7][5] = 11    // for enable B_VDIO_GATTING and VIO_SEL
    hdmirxset(REG_RX_052,(B_DisVAutoMute),(B_DisVAutoMute));                //Reg52[5] = 1 for disable Auto video MUTE
    hdmirxset(REG_RX_053,(B_TriVDIO),(0x00));                                //Reg53[2:0] = 000;         // 0 for enable video io data output
    HDMIRX_VIDEO_PRINTF(("+++++++++++ Manual Video / Audio off  +++++++++++++++++\n"));
    }
    else
    {
    hdmirxset(REG_RX_053,(B_TriSYNC),(0x00));                                //Reg53[0] = 0;                 // for enable video sync
    hdmirxset(REG_RX_053,(B_TriVDIO),(0x00));                                //Reg53[3:1] = 000;         // 0 for enable video io data output
    hdmirxset(REG_RX_053,(B_TriVDIO),(B_TriVDIO));                            //Reg53[2:0] = 111;         // 1 for enable tri-state of video io data
    hdmirxset(REG_RX_053,(B_TriVDIO),(0x00));                                //Reg53[2:0] = 000;         // 0 for enable video io data output
    hdmirxset(REG_RX_053,(B_VDGatting|B_VIOSel),(B_VDGatting|B_VIOSel));    //Reg53[7][5] = 11    // for enable B_VDIO_GATTING and VIO_SEL
    hdmirxset(REG_RX_053,(B_VDGatting|B_VIOSel),(B_VIOSel));                //Reg53[7][5] = 01    // for disable B_VDIO_GATTING
    HDMIRX_VIDEO_PRINTF(("+++++++++++ Manual Video on  +++++++++++++++++\n"));
    }
}
void IT6802_HDCP_ContentOff(unsigned char ucPort , unsigned char bOff)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();        //2013-0814

    if(IT6802_IsSelectedPort(ucPort)==FALSE)
    return;

    if(bOff != 0)
    {
        //******** Content Off ********//
        IT6802_ManualVideoTristate(1);
        it6802data->m_HDCP_ContentOff = 1;
        HDMIRX_VIDEO_PRINTF(("+++++++++++ HDCP Content Off   +++++++++++++++++\n"));
    }
    else
    {
        if(it6802data->m_VState == VSTATE_VideoOn)
        {
            if(it6802data->m_HDCP_ContentOff == 1 )
            {
                //******** Content On ********//
                IT6802_ManualVideoTristate(0);
                HDMIRX_VIDEO_PRINTF(("+++++++++++ HDCP Content On   +++++++++++++++++\n"));
            }
        }

        it6802data->m_HDCP_ContentOff = 0;
    }

}


#ifdef _SUPPORT_RAP_
void IT6802_RAPContentOff(unsigned char bOff)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();        //2013-0814

    if(IT6802_IsSelectedPort(0)==FALSE)
    return;

    if(bOff != 0)
    {
        //******** RAP Content Off ********//
        IT6802_ManualVideoTristate(1);
        it6802data->m_RAP_ContentOff = 1;
        HDMIRX_VIDEO_PRINTF(("+++++++++++ RAP Content Off   +++++++++++++++++\n"));

        //xxxxx 2014-0603 for RAP Content off
        IT6802AudioOutputEnable(0);
        //xxxxx 2014-0603
    }
    else
    {
        if(it6802data->m_VState == VSTATE_VideoOn)
        {
            if(it6802data->m_RAP_ContentOff == 1 )
        {
            //******** RAP Content On ********//
            IT6802_ManualVideoTristate(0);
            HDMIRX_VIDEO_PRINTF(("+++++++++++ RAP Content On   +++++++++++++++++\n"));

            #ifndef _FIX_ID_028_
            //FIX_ID_028 xxxxx //For Debug Audio error with S2
            //xxxxx 2014-0603 for RAP Content On
            IT6802SwitchAudioState(it6802data,ASTATE_RequestAudio);
            //xxxxx 2014-0603
            //FIX_ID_028 xxxxx //For Debug Audio error with S2
            #endif

        }
        }
        it6802data->m_RAP_ContentOff = 0;
    }
}
#endif // _SUPPORT_RAP_
//xxxxx 2014-0529
//FIX_ID_037 xxxxx

void IT6802_SetVideoMute(struct it6802_dev_data *it6802,unsigned char bMute)
{

    if(bMute)
    {
        //******** AV Mute -> ON ********//
        hdmirxset(REG_RX_053,(B_VDGatting|B_VIOSel),(B_VDGatting|B_VIOSel));    //Reg53[7][5] = 11    // for enable B_VDIO_GATTING and VIO_SEL
        hdmirxset(REG_RX_052,(B_DisVAutoMute),(B_DisVAutoMute));                //Reg52[5] = 1 for disable Auto video MUTE
        hdmirxset(REG_RX_053,(B_TriVDIO),(0x00));                                //Reg53[2:0] = 000;         // 0 for enable video io data output

        HDMIRX_VIDEO_PRINTF(("+++++++++++ IT6802_SetVideoMute -> On +++++++++++++++++\n"));
    }
    else
    {
        if(it6802->m_VState == VSTATE_VideoOn)
        {
            //******** AV Mute -> OFF ********//
            hdmirxset(REG_RX_053,(B_TriSYNC),(0x00));                                //Reg53[0] = 0;                 // for enable video sync
            hdmirxset(REG_RX_053,(B_TriVDIO),(0x00));                                //Reg53[3:1] = 000;         // 0 for enable video io data output

            if(CheckAVMute()==TRUE)
            {
                hdmirxset(REG_RX_052,(B_DisVAutoMute),(B_DisVAutoMute));                //Reg52[5] = 1 for disable Auto video MUTE
            }
            else
            {

                // enable output
                hdmirxset(REG_RX_064, 0x80, 0x80); // video fifo reset
                hdmirxset(REG_RX_064, 0x80, 0x00); // video fifo reset
                delay1ms(10);

                hdmirxset(REG_RX_053,(B_TriVDIO),(B_TriVDIO));                            //Reg53[2:0] = 111;         // 1 for enable tri-state of video io data
                hdmirxset(REG_RX_053,(B_TriVDIO),(0x00));                                //Reg53[2:0] = 000;         // 0 for enable video io data output

                hdmirxset(REG_RX_053,(B_VDGatting|B_VIOSel),(B_VDGatting|B_VIOSel));    //Reg53[7][5] = 11    // for enable B_VDIO_GATTING and VIO_SEL
                hdmirxset(REG_RX_053,(B_VDGatting|B_VIOSel),(B_VIOSel));                //Reg53[7][5] = 01    // for disable B_VDIO_GATTING

                HDMIRX_VIDEO_PRINTF(("+++++++++++  IT6802_SetVideoMute -> Off +++++++++++++++++\n"));

                //        get_vid_info();
                //        show_vid_info();

            }

        }

    }

}




//static void IT6802VideoOutputCDSet(void)
//{
//
//    unsigned char GCP_CD;
//    GCP_CD = ((hdmirxrd(0x99)&0xF0)>>4);
//
//    switch( GCP_CD ) {
//    case 5 :
//    printf("\n Output ColorDepth = 30 bits per pixel\n");
//    hdmirxset(0x65, 0x0C, 0x04);
//    break;
//    case 6 :
//    printf( "\n Output ColorDepth = 36 bits per pixel\n");
//    hdmirxset(0x65, 0x0C, 0x08);
//    break;
//    default :
//    printf( "\n Output ColorDepth = 24 bits per pixel\n");
//    hdmirxset(0x65, 0x0C, 0x00);
//    break;
//    }
//}


void IT6802VideoOutputEnable(unsigned char bEnableOutput)
{
//    struct it6802_dev_data *it6802data = get_it6802_dev_data();
    if(bEnableOutput)
    {
        // enable output


        // enable output
        hdmirxset(REG_RX_064, 0x80, 0x80); // video fifo reset
        hdmirxset(REG_RX_064, 0x80, 0x00); // video fifo reset
        delay1ms(10);

        hdmirxset(REG_RX_053,(B_TriSYNC|B_TriVDIO),(0x00));
        HDMIRX_VIDEO_PRINTF(("---------------- IT6802VideoOutputEnable -> On ----------------\n"));
//        IT6802VideoOutputCDSet();

        //FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
        #if defined(_IT6803_)
            IT6803JudgeOutputMode();
        #endif
        //FIX_ID_016 xxxxx

    }
    else
    {
        // disable output
        hdmirxset(REG_RX_053,(B_TriSYNC|B_TriVDIO),(B_TriSYNC|B_TriVDIO));
        HDMIRX_VIDEO_PRINTF(("---------------- IT6802VideoOutputEnable -> Off ----------------\n"));

        //FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
        #if defined(_IT6803_)
            hdmirxset(REG_RX_08C, 0x08, 0x00);        // Reg8C[3] = 0    // VDIO3en: //  for disable QA IO
        #endif
        //FIX_ID_016 xxxxx


    }
}


void IT6802SwitchVideoState(struct it6802_dev_data *it6802,Video_State_Type  eNewVState)
{
    // printf("[MindTec] IT6802SwitchVideoState: %d -> %d\n", it6802->m_VState, eNewVState);
    if(it6802->m_VState==eNewVState)
        return;

    printf("[MindTec] IT6802SwitchVideoState: %d -> %d\n", it6802->m_VState, eNewVState);

    HDMIRX_VIDEO_PRINTF((VStateStr[(unsigned char)eNewVState]));
    HDMIRX_VIDEO_PRINTF(("+++\n"));


    it6802->m_VState=eNewVState;
//    it6802->m_VideoCountingTimer = GetCurrentVirtualTime(); // get current time tick, and the next tick judge in the polling handler.

    #ifdef _SUPPORT_HDMI_REPEATER_
    if( it6802->m_VState != VSTATE_VideoOn )
    {
        IT680X_DownStream_VideoReady(it6802, OFF) ;
    }
    #endif

    switch(it6802->m_VState)
    {
    case VSTATE_Off:
        it6802HPDCtrl(0, 0) ;
        it6802HPDCtrl(1, 0) ; // if downstream switch off the state, it should be hold and HPD goes to off.

        break ;

    case VSTATE_SWReset:
        {
            IT6802VideoOutputEnable(FALSE);
        //FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
        #ifdef _AVOID_REDUNDANCE_CSC_
            it6802->m_Backup_OutputVideoMode = 0xFF;
            it6802->m_Backup_InputVideoMode = 0xFF;
        #endif
        //FIX_ID_039 xxxxx
            IT6802_AFE_Rst();
        #ifdef Enable_IT6802_CEC
        //xxxxx FIX_ID_022        //Fixed for CEC capacitor issue
            IT6802_ResetCEC();
        //xxxxx
        #endif
            if(CheckPlg5VPwr(0)==FALSE)
            {
                it6802HPDCtrl(0,0);    // clear ucport's HPD = 0
            }
            else
            {
                it6802HPDCtrl(0,1);    // set ucport's HPD = 1
            }

            if(CheckPlg5VPwr(1)==FALSE)
            {
                it6802HPDCtrl(1,0);    // clear ucport's HPD = 0
            }
            else
            {
                it6802HPDCtrl(1,1);    // set ucport's HPD = 1
            }
        // Enable HDMI DDC bus to access IT680X EDID RAM


        }
        break;

    case VSTATE_SyncWait: {
            // 1. SCDT off interrupt
            // 2. VideoMode_Chg interrupt
            IT6802VideoOutputEnable(FALSE);
//FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
#ifdef _AVOID_REDUNDANCE_CSC_
            it6802->m_Backup_OutputVideoMode = 0xFF;
            it6802->m_Backup_InputVideoMode = 0xFF;
#endif
//FIX_ID_039 xxxxx
            it6802->m_NewAVIInfoFrameF=FALSE;
            it6802->m_ucDeskew_P0=0;
            it6802->m_ucDeskew_P1=0;
            //it6802->m_ucSCDTOffCount=0;

            #ifdef Enable_Vendor_Specific_packet
            it6802->f_de3dframe_hdmi = FALSE;
            hdmirxwr(REG_RX_06A, 0x82);
            #endif

        }
        break;

    case VSTATE_SyncChecking: {
            // 1. SCDT on interrupt
            //AssignVideoVirtualTime(VSATE_CONFIRM_SCDT_COUNT);
            //AssignVideoTimerTimeout(VSATE_CONFIRM_SCDT_COUNT);

            it6802->m_VideoCountingTimer = VSATE_CONFIRM_SCDT_COUNT;

            #ifdef Enable_Vendor_Specific_packet
            hdmirxwr(REG_RX_06A, 0x82);
            #endif

        }
        break;

    case VSTATE_VideoOn: {
            IT6802VideoOutputConfigure(it6802);
            IT6802VideoOutputEnable(TRUE);
            IT6802SwitchAudioState(it6802,ASTATE_RequestAudio);

            get_vid_info();
            show_vid_info();

            hdmirxwr(0x84, 0x8F);    //2011/06/17 xxxxx, for enable Rx Chip count

            #ifdef Enable_Vendor_Specific_packet
            hdmirxwr(REG_RX_06A, 0x81);
            #endif


            //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            //    #ifdef _SUPPORT_EQ_ADJUST_
            //    HDMIStartEQDetect(&(it6802->EQPort[it6802->m_ucCurrentHDMIPort]));
            //    #endif
            //FIX_ID_001 xxxxx

            //xxxxx 2013-0812 @@@@@
                it6802->m_ucSCDTOffCount=0;
            //xxxxx 2013-0812
            #ifdef _SUPPORT_HDMI_REPEATER_
            IT680X_DownStream_VideoReady(it6802, ON) ;
            #endif

        }
        break;
}


}

// ---------------------------------------------------------------------------
void IT6802VideoHandler(struct it6802_dev_data *it6802)
{
//    unsigned char uc;

    if(it6802->m_VideoCountingTimer > MS_LOOP)
    {
        it6802->m_VideoCountingTimer -= MS_LOOP;
    }
    else
    {
        it6802->m_VideoCountingTimer = 0;
    }



    switch(it6802->m_VState)
    {

    case VSTATE_SyncWait: {
            //Waiting for SCDT on interrupt !!!
            //if(VideoCountingTimer==0)

            WaitingForSCDT(it6802);

#if 0
            if(TimeOutCheck(eVideoCountingTimer)==TRUE) {
                MxLRX_DEBUG_PRINTF(("------------SyncWait time out -----------\n"));
                SWReset_HDMIRX();
                return;
            }
#endif
        }
        break;

    case VSTATE_SyncChecking:{
                //if(VideoTimeOutCheck(VSATE_CONFIRM_SCDT_COUNT))
                if(it6802->m_VideoCountingTimer == 0)
                {
                    if(CheckAVMute()==0){
                        IT6802SwitchVideoState(it6802,VSTATE_VideoOn);
                    }
                }
        }
        break;

    case VSTATE_VideoOn:

        if(it6802->m_NewAVIInfoFrameF==TRUE)
        {
            if(it6802->m_RxHDCPState != RxHDCP_ModeCheck)
            {
                IT6802VideoOutputConfigure(it6802);
                it6802->m_NewAVIInfoFrameF=FALSE;
            }

        }

        if(hdmirxrd(REG_RX_053)&B_VDGatting)
        {
            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            //xxxxx 2014-0529 //Manual Content On/Off
            //if(IT6802_IsSelectedPort(0)
            {
                if((it6802->m_RAP_ContentOff == 0) && (it6802->m_HDCP_ContentOff == 0))
                {
                    if(CheckAVMute()==FALSE)
                    {
                    IT6802_SetVideoMute(it6802,OFF);
                    }
                }
            }
            //xxxxx 2014-0529
            //FIX_ID_037 xxxxx
        }

#ifdef _FIX_ID_028_
//FIX_ID_028 xxxxx //For Debug Audio error with S2
        if( hdmirxrd(REG_RX_0AA) & 0x80)
        {
            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            if(it6802->m_RAP_ContentOff == 0)    //xxxxx 2014-0529 //Manual Content On/Off
            {
                if(it6802->m_AState  != ASTATE_AudioOn)
                {
                    it6802->m_AudioCountingTimer = AUDIO_READY_TIMEOUT;
                    it6802->m_AState = ASTATE_AudioOn;
                    m_bAudioWaiting = TRUE;
                }
                else
                {

                   if(it6802->m_AudioCountingTimer > MS_LOOP)
                   {
                   it6802->m_AudioCountingTimer -= MS_LOOP;
                   }
                   else
                   {
                       it6802->m_AudioCountingTimer = 0;
                       if(m_bAudioWaiting == TRUE)
                    {
                    IT6802AudioOutputEnable(TRUE);
                    }
                   }

                }
            }        //xxxxx 2014-0529
            //FIX_ID_037 xxxxx
        }
        else
        {
            if(it6802->m_AState  == ASTATE_AudioOn)
            IT6802AudioOutputEnable(FALSE);
        }
//FIX_ID_028 xxxxx
#endif


        break;
    }


}

#endif

#ifdef _IT680x_
// ***************************************************************************
// Interrupt function
// ***************************************************************************
void hdmirx_INT_5V_Pwr_Chg(struct it6802_dev_data *it6802,unsigned char ucport)
{

    unsigned char ucPortSel;
    ucPortSel = hdmirxrd(REG_RX_051)&B_PORT_SEL;

    // ite180509
    chgbank(1);
    hdmirxset(0xB1, 0x60, 0x00);
    hdmirxset(0xC1, 0x60, 0x00);
    chgbank(0);
    CLKStbChkCnt=0;
//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
    if(ucPortSel == ucport)
    {
        if(CheckPlg5VPwr(ucport)==TRUE)
        {
            IT6802_DEBUG_INT_PRINTF(("#### Power 5V ON ####\n"));
            IT6802SwitchVideoState(it6802,VSTATE_SyncWait);
            it6802HPDCtrl(ucport,1);    // set ucport's HPD = 1
        }
        else
        {
            IT6802_DEBUG_INT_PRINTF(("#### Power 5V OFF ####\n"));
            it6802HPDCtrl(ucport,0);    // clear ucport's HPD = 0
            IT6802SwitchVideoState(it6802,VSTATE_SWReset);
        }
    }
    else
    {
        if(CheckPlg5VPwr(ucport)==FALSE)
        {
            #ifdef _SUPPORT_AUTO_EQ_
            DisableOverWriteRS(ucport);
            #endif
            it6802HPDCtrl(ucport,0);    // clear ucport's HPD = 0
        }
        else
        {
            it6802HPDCtrl(ucport,1);    // set ucport's HPD = 1
        }
    }
//FIX_ID_037 xxxxx
}
// ---------------------------------------------------------------------------
void hdmirx_INT_P0_ECC(struct it6802_dev_data *it6802)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();


    if((it6802->m_ucEccCount_P0++) > ECC_TIMEOUT)
    {

        #ifdef _SUPPORT_EQ_ADJUST_
        if(it6802->EQPort[F_PORT_SEL_0].f_manualEQadjust==TRUE)    // ignore ECC interrupt when manual EQ adjust !!!
        return;
        #endif

        it6802->m_ucEccCount_P0=0;

        IT6802_DEBUG_INT_PRINTF(("CDR reset for Port0 ECC_TIMEOUT !!!\n"));

        hdmirx_ECCTimingOut(F_PORT_SEL_0);
        // disable ->if((hdmirxrd(0x0A)&0x40))
        // disable ->//if( MxL_Mode )
        // disable ->{
        // disable ->MxLrxset(MxL_RX_28,0x40,0x40);
        // disable ->//it6802HPDCtrl(0,1);
        // disable ->delay1ms(100);
        // disable ->//it6802HPDCtrl(0,0);
        // disable ->MxLrxset(MxL_RX_28,0x40,0x00);
        // disable ->
        // disable ->}
        // disable ->else
        // disable ->{
        // disable ->it6802HPDCtrl(0,0);    // MxL port , set HPD = 0
        // disable ->}
        // disable ->
        // disable ->hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST|B_P0_SWRST),(B_P0_DCLKRST|B_P0_CDRRST|B_P0_SWRST));
        // disable ->delay1ms(300);
        // disable ->hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST|B_P0_SWRST),0x00);
        // disable ->
        // disable ->
        // disable ->
        // disable ->//set port 0 HPD=1
        // disable ->it6802HPDCtrl(0,1);    // MxL port , set HPD = 1
        //IT6802SwitchVideoState(it6802,VSTATE_SyncWait);

    }
}

// ---------------------------------------------------------------------------
void hdmirx_INT_P1_ECC(struct it6802_dev_data *it6802)
{


    if((it6802->m_ucEccCount_P1++) > ECC_TIMEOUT)
    {
        #ifdef _SUPPORT_EQ_ADJUST_
        if(it6802->EQPort[F_PORT_SEL_1].f_manualEQadjust==TRUE)    // ignore ECC interrupt when manual EQ adjust !!!
        return;
        #endif

        it6802->m_ucEccCount_P1=0;

        IT6802_DEBUG_INT_PRINTF(("CDR reset for Port1 ECC_TIMEOUT !!!\n"));

        hdmirx_ECCTimingOut(F_PORT_SEL_1);
        // disable ->//set port 1 HPD=0
        // disable ->//HotPlug(0);
        // disable ->//xxxxx 2013-0801
        // disable ->it6802HPDCtrl(1,0);    // HDMI port , set HPD = 0
        // disable ->//xxxxx
        // disable ->
        // disable ->hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST|B_P1_SWRST),(B_P1_DCLKRST|B_P1_CDRRST|B_P1_SWRST));
        // disable ->delay1ms(200);
        // disable ->hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST|B_P1_SWRST),0x00);
        // disable ->
        // disable ->//set port 1 HPD=1
        // disable ->//HotPlug(1);
        // disable ->//xxxxx 2013-0801
        // disable ->it6802HPDCtrl(1,1);    // HDMI port , set HPD = 1
        // disable ->//xxxxx


    }
}

// ---------------------------------------------------------------------------
void hdmirx_INT_P0_Deskew(struct it6802_dev_data *it6802)
{
    if((it6802->m_ucDeskew_P0++) > DESKEW_TIMEOUT)
    {
        #ifdef _SUPPORT_EQ_ADJUST_
        if(it6802->EQPort[F_PORT_SEL_0].f_manualEQadjust==TRUE)    // ignore ECC interrupt when manual EQ adjust !!!
        return;
        #endif
        it6802->m_ucDeskew_P0=0;

        IT6802_DEBUG_INT_PRINTF(("CDR reset for Port0 DESKEW_TIMEOUT !!!\n"));
//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
// 07-16 disable ->    #ifdef _SUPPORT_EQ_ADJUST_
// 07-16 disable ->    if(it6802->f_manualEQadjust[0]==TRUE)
// 07-16 disable ->        HDMISwitchEQstate(&it6802->EQPort[F_PORT_SEL_0],EQSTATE_WAIT+1);    //for SCDT on state
// 07-16 disable ->    #endif
//FIX_ID_001 xxxxx
//        hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST),(B_P0_DCLKRST|B_P0_CDRRST));
//        hdmirxset(REG_RX_011,(B_P0_DCLKRST|B_P0_CDRRST),0x00);

        if(hdmirxrd(REG_RX_020) == 0x00)
            hdmirxwr(REG_RX_020,0x3F);    // Dr. Liu suggestion to 0x00
        else
            hdmirxwr(REG_RX_020,0x00);    // Dr. Liu suggestion to 0x3F

    }
}

// ---------------------------------------------------------------------------
void hdmirx_INT_P1_Deskew(struct it6802_dev_data *it6802)
{
    if((it6802->m_ucDeskew_P1++) > DESKEW_TIMEOUT)
    {
        #ifdef _SUPPORT_EQ_ADJUST_
        if(it6802->EQPort[F_PORT_SEL_1].f_manualEQadjust==TRUE)    // ignore ECC interrupt when manual EQ adjust !!!
        return;
        #endif

        it6802->m_ucDeskew_P1=0;

        IT6802_DEBUG_INT_PRINTF(("CDR reset for Port1 DESKEW_TIMEOUT !!!\n"));
//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
// 07-16 disable ->    #ifdef _SUPPORT_EQ_ADJUST_
// 07-16 disable ->    if(it6802->f_manualEQadjust[1]==TRUE)
// 07-16 disable ->        HDMISwitchEQstate(&it6802->EQPort[F_PORT_SEL_1],EQSTATE_WAIT+1);    //for SCDT on state
// 07-16 disable ->    #endif
//FIX_ID_001 xxxxx
//        hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST),(B_P1_DCLKRST|B_P1_CDRRST));
//        hdmirxset(REG_RX_018,(B_P1_DCLKRST|B_P1_CDRRST),0x00);



        //it6802->EQPort[F_PORT_SEL_1].f_manualEQadjust=TRUE;
        //HDMISwitchEQstate(&it6802->EQPort[F_PORT_SEL_1],EQSTATE_END);    //for SCDT on state
        //HDMICheckECC(&it6802->EQPort[F_PORT_SEL_1]);

        if(hdmirxrd(REG_RX_038) == 0x00)
            hdmirxwr(REG_RX_038,0x3F);    // Dr. Liu suggestion to 0x00
        else
            hdmirxwr(REG_RX_038,0x00);    // Dr. Liu suggestion to 0x3F
    }
}


// ---------------------------------------------------------------------------
//FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
void hdmirx_INT_HDMIMode_Chg(struct it6802_dev_data *it6802,unsigned char ucport)
{
    unsigned char ucPortSel;
    ucPortSel = hdmirxrd(REG_RX_051)&B_PORT_SEL;

    if(ucPortSel != ucport)
        return;
//FIX_ID_009 xxxxx

    if(IsHDMIMode()){
            if(it6802->m_VState==VSTATE_VideoOn)
            {
                IT6802SwitchAudioState(it6802,ASTATE_RequestAudio);
        }
        it6802->m_bUpHDMIMode = TRUE;
        IT6802_DEBUG_INT_PRINTF(("#### HDMI/DVI Mode : HDMI ####\n"));
    }
    else
    {
        IT6802SwitchAudioState(it6802,ASTATE_AudioOff);
        it6802->m_NewAVIInfoFrameF=FALSE;
            if(it6802->m_VState==VSTATE_VideoOn)
            {
                SetDVIVideoOutput(it6802);
            }
        it6802->m_bUpHDMIMode = FALSE;
        IT6802_DEBUG_INT_PRINTF(("#### HDMI/DVI Mode : DVI ####\n"));
    }
}

// ---------------------------------------------------------------------------
void hdmirx_INT_SCDT_Chg(struct it6802_dev_data *it6802)
{
    if(CheckSCDT(it6802) == TRUE){
        IT6802_DEBUG_INT_PRINTF(("#### SCDT ON ####\n"));
        IT6802SwitchVideoState(it6802,VSTATE_SyncChecking);
    }
    else{
        IT6802_DEBUG_INT_PRINTF(("#### SCDT OFF ####\n"));
        IT6802SwitchVideoState(it6802,VSTATE_SyncWait);
        IT6802SwitchAudioState(it6802,ASTATE_AudioOff);

//        TMDSCheck(it6802->m_ucCurrentHDMIPort);
//        TogglePolarity (it6802->m_ucCurrentHDMIPort);


    }
}


//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_AUTO_EQ_
void hdmirx_INT_EQ_FAIL(struct it6802_dev_data *it6802,unsigned char ucPortSel)
{
    if(ucPortSel>F_PORT_SEL_1)
        return;

#ifdef _SUPPORT_EQ_ADJUST_
    if(it6802->EQPort[ucPortSel].f_manualEQadjust==FALSE)    // ignore EQ fail interrupt when manual EQ adjust !!!
#endif
    {
        if(CheckPlg5VPwr(ucPortSel))
        {

            //07-08
            if( ucPortSel    == 0)
            {
                if((it6802->HDMIIntEvent & (B_PORT0_TMDSEvent)))
                {
                    IT6802_DEBUG_INT_PRINTF(("#### hdmirx_INT_EQ_FAIL not yet !!! ####\n"));

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
                    if((it6802->HDMIIntEvent & (B_PORT0_Waiting))==0)
                    {
                        hdmirxwr(REG_RX_022, 0x00);    // power down auto EQ
                        it6802->HDMIIntEvent |= (B_PORT0_Waiting);
                        it6802->HDMIIntEvent |= (B_PORT0_TMDSEvent);
                        it6802->HDMIWaitNo[0]=MAX_TMDS_WAITNO;
                    }
                    else if((it6802->HDMIIntEvent & (B_PORT0_TMDSEvent)))
                    {
                        it6802->HDMIIntEvent |= (B_PORT0_Waiting);
                        it6802->HDMIWaitNo[0] += MAX_HDCP_WAITNO;
                    }

//FIX_ID_033 xxxxx

                    return;
                }

                if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
                {
                    if((ucPortAMPValid[0]&0x03)!=0x03)
                        AmpValidCheck(ucPortSel);

                }
                else
                {
                    if((ucPortAMPValid[ucPortSel]&0x3F)!=0x3F)
                        AmpValidCheck(ucPortSel);

                }
            }
            else
            {
                if((it6802->HDMIIntEvent & (B_PORT1_TMDSEvent)))
                {
                    IT6802_DEBUG_INT_PRINTF(("#### hdmirx_INT_EQ_FAIL not yet !!! ####\n"));

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
                    if((it6802->HDMIIntEvent & (B_PORT1_Waiting))==0)
                    {
                        hdmirxwr(REG_RX_03A, 0x00);    // power down auto EQ
                        it6802->HDMIIntEvent |= (B_PORT1_Waiting);
                        it6802->HDMIIntEvent |= (B_PORT1_TMDSEvent);
                        it6802->HDMIWaitNo[1]=MAX_TMDS_WAITNO;
                    }
                    else if((it6802->HDMIIntEvent & (B_PORT1_TMDSEvent)))
                    {
                        it6802->HDMIIntEvent |= (B_PORT1_Waiting);
                        it6802->HDMIWaitNo[1] += MAX_HDCP_WAITNO;
                    }
//FIX_ID_033 xxxxx

                    return;
                }

                if((ucPortAMPValid[ucPortSel]&0x3F)!=0x3F)
                    AmpValidCheck(ucPortSel);
            }

//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
//xxxxx 2014-0508 disable ->            ucEqRetryCnt[ucPortSel]++;
//xxxxx 2014-0508 disable ->            if( ucEqRetryCnt[ucPortSel]>=EQRETRYFAILCNT)
//FIX_ID_035 xxxxx
            {

                //07-08
                if( ucPortSel    == 0)
                {
                // FIX_ID_043
                // If Toggle Count too much, stop AUTO EQ and Force manual EQ;
                    if(it6802->ToggleCount[ucPortSel]>=MAX_TOGGLE_FAIL_COUNT)
                    {
                        IT6802_DEBUG_INT_PRINTF(("#### hdmirx_INT_EQ0_FAIL has toggled, and manual EQ!!! it6802->ToggleCount[%bd] = %bd ####\n",ucPortSel,it6802->ToggleCount[ucPortSel]));
                        hdmirxwr(REG_RX_022, 0x00);
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_0]));
                    }
                    else
                    {
                        if(hdmirxrd(REG_RX_P0_SYS_STATUS) & (B_P0_MxL_MODE))
                        {
                            if((ucPortAMPValid[0]&0x03)!=0x03)
                            {
                                TogglePolarity (ucPortSel);
                                it6802->ToggleCount[ucPortSel]++;
                            }

                        }
                        else
                        {
                            if((ucPortAMPValid[ucPortSel]&0x3F)!=0x3F)
                            {
                                TogglePolarity (ucPortSel);
                                it6802->ToggleCount[ucPortSel]++;
                            }

                        }
                        IT6802_DEBUG_INT_PRINTF(("it6802->ToggleCount[%bd] = %bd\n",ucPortSel,it6802->ToggleCount[ucPortSel]));

                    }
                }
                else
                {
                    if(it6802->ToggleCount[ucPortSel]>=MAX_TOGGLE_FAIL_COUNT)
                    {

                        IT6802_DEBUG_INT_PRINTF(("#### hdmirx_INT_EQ1_FAIL has toggled, and manual EQ!!! it6802->ToggleCount[%bd] = %bd ####\n",ucPortSel,it6802->ToggleCount[ucPortSel]));
                        hdmirxwr(REG_RX_03A, 0x00);
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_1]));
                    }
                    else
                    {
                        if((ucPortAMPValid[ucPortSel]&0x3F)!=0x3F)
                        {
                            TogglePolarity (ucPortSel);
                            it6802->ToggleCount[ucPortSel]++;
                        }
                    }
                    IT6802_DEBUG_INT_PRINTF(("it6802->ToggleCount[%bd] = %bd\n",ucPortSel,it6802->ToggleCount[ucPortSel]));
                }
                // FIX_ID_043
//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
//xxxxx 2014-0508 disable ->                ucEqRetryCnt[ucPortSel]=0;
//FIX_ID_035 xxxxx
            }
        }
    }

}
#endif
//FIX_ID_001 xxxxx
#endif


#ifdef _IT680x_
// disable -> /*****************************************************************************/
// disable -> /* HDCP functions    *********************************************************/
// disable -> /*****************************************************************************/
// disable ->
// disable -> static void hdcpsts( void )
// disable -> {
// disable ->
// disable ->    int AKSV1, AKSV2, AKSV3, AKSV4, AKSV5;
// disable ->    int BKSV1, BKSV2, BKSV3, BKSV4, BKSV5;
// disable ->    int BMi1,BMi2, BMi3, BMi4, BMi5, BMi6, BMi7, BMi8;
// disable ->    int Mi1, Mi2, Mi3, Mi4, Mi5, Mi6, Mi7, Mi8;
// disable ->
// disable ->     chgbank(2);
// disable ->
// disable ->    BKSV1 = hdmirxrd(0x9F);
// disable ->    BKSV2 = hdmirxrd(0xA0);
// disable ->    BKSV3 = hdmirxrd(0xA1);
// disable ->    BKSV4 = hdmirxrd(0xA2);
// disable ->    BKSV5 = hdmirxrd(0xA3);
// disable ->
// disable ->    AKSV1 = hdmirxrd(0x9A);
// disable ->    AKSV2 = hdmirxrd(0x9B);
// disable ->    AKSV3 = hdmirxrd(0x9C);
// disable ->    AKSV4 = hdmirxrd(0x9D);
// disable ->    AKSV5 = hdmirxrd(0x9E);
// disable ->
// disable ->    BMi1 = hdmirxrd(0xA4);
// disable ->    BMi2 = hdmirxrd(0xA5);
// disable ->    BMi3 = hdmirxrd(0xA6);
// disable ->    BMi4 = hdmirxrd(0xA7);
// disable ->    BMi5 = hdmirxrd(0xA8);
// disable ->    BMi6 = hdmirxrd(0xA9);
// disable ->    BMi7 = hdmirxrd(0xAA);
// disable ->    BMi8 = hdmirxrd(0xAB);
// disable ->
// disable ->    Mi1 = (~BMi1)&0xFF;
// disable ->    Mi2 = (~BMi2)&0xFF;
// disable ->    Mi3 = (~BMi3)&0xFF;
// disable ->    Mi4 = (~BMi4)&0xFF;
// disable ->    Mi5 = (~BMi5)&0xFF;
// disable ->    Mi6 = (~BMi6)&0xFF;
// disable ->    Mi7 = (~BMi7)&0xFF;
// disable ->    Mi8 = (~BMi8)&0xFF;
// disable ->
// disable ->    MxLRX_DEBUG_PRINTF(("AKSV = 0x%X%X%X%X%X%X%X%X%X%X\n", (int) AKSV5>>4,(int)  AKSV5&0x0F,
// disable ->                                              (int)  AKSV4>>4, (int) AKSV4&0x0F,
// disable ->                                               (int) AKSV3>>4, (int) AKSV3&0x0F,
// disable ->                                               (int) AKSV2>>4, (int) AKSV2&0x0F,
// disable ->                                               (int) AKSV1>>4, (int) AKSV1&0x0F));
// disable ->
// disable ->    MxLRX_DEBUG_PRINTF(("BKSV = 0x%X%X%X%X%X%X%X%X%X%X\n", (int) BKSV5>>4,(int)  BKSV5&0x0F,
// disable ->                                               (int) BKSV4>>4, (int) BKSV4&0x0F,
// disable ->                                               (int) BKSV3>>4, (int) BKSV3&0x0F,
// disable ->                                               (int) BKSV2>>4, (int) BKSV2&0x0F,
// disable ->                                               (int) BKSV1>>4, (int) BKSV1&0x0F));
// disable ->
// disable ->    MxLRX_DEBUG_PRINTF(("M0   = 0x%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X%X\n", (int) Mi8>>4,(int)  Mi8&0x0F,
// disable ->                                                          (int)  Mi7>>4,(int)  Mi7&0x0F,
// disable ->                                                         (int)   Mi6>>4, (int) Mi6&0x0F,
// disable ->                                                          (int)  Mi5>>4, (int) Mi5&0x0F,
// disable ->                                                           (int) Mi4>>4, (int) Mi4&0x0F,
// disable ->                                                           (int) Mi3>>4, (int) Mi3&0x0F,
// disable ->                                                           (int) Mi2>>4, (int) Mi2&0x0F,
// disable ->                                                           (int) Mi1>>4, (int) Mi1&0x0F));
// disable ->    chgbank(0);
// disable ->
// disable -> }
#endif



//#ifdef _IT680x_
//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
/*****************************************************************************/
/* MxLRX functions    *********************************************************/
/*****************************************************************************/
#ifdef _SUPPORT_RCP_
////////////////////////////////////////////////////////////////////
//void parse_rcpkey( unsigned char rcpkey )
//
//
//
////////////////////////////////////////////////////////////////////
void parse_rcpkey( unsigned char rcpkey )
{
     unsigned char suppkey;

     suppkey = TRUE;

     MxLRX_DEBUG_PRINTF(("KeyCode=%X ==>",(int)  rcpkey&0x7F));

     switch( rcpkey&0x7F ) {
     case 0x00: MxLRX_DEBUG_PRINTF(("Select")); break;
     case 0x01: MxLRX_DEBUG_PRINTF(("Up")); break;
     case 0x02: MxLRX_DEBUG_PRINTF(("Down")); break;
     case 0x03: MxLRX_DEBUG_PRINTF(("Left")); break;
     case 0x04: MxLRX_DEBUG_PRINTF(("Right")); break;
     case 0x05: MxLRX_DEBUG_PRINTF(("Right-Up")); break;
     case 0x06: MxLRX_DEBUG_PRINTF(("Right-Down")); break;
     case 0x07: MxLRX_DEBUG_PRINTF(("Left-Up")); break;
     case 0x08: MxLRX_DEBUG_PRINTF(("Left-Down")); break;
     case 0x09: MxLRX_DEBUG_PRINTF(("Root Menu")); break;
     case 0x0A: MxLRX_DEBUG_PRINTF(("Setup Menu")); break;
     case 0x0B: MxLRX_DEBUG_PRINTF(("Contents Menu")); break;
     case 0x0C: MxLRX_DEBUG_PRINTF(("Favorite Menu")); break;
     case 0x0D: MxLRX_DEBUG_PRINTF(("Exit")); break;

     case 0x20: MxLRX_DEBUG_PRINTF(("Numeric 0")); break;
     case 0x21: MxLRX_DEBUG_PRINTF(("Numeric 1")); break;
     case 0x22: MxLRX_DEBUG_PRINTF(("Numeric 2")); break;
     case 0x23: MxLRX_DEBUG_PRINTF(("Numeric 3")); break;
     case 0x24: MxLRX_DEBUG_PRINTF(("Numeric 4")); break;
     case 0x25: MxLRX_DEBUG_PRINTF(("Numeric 5")); break;
     case 0x26: MxLRX_DEBUG_PRINTF(("Numeric 6")); break;
     case 0x27: MxLRX_DEBUG_PRINTF(("Numeric 7")); break;
     case 0x28: MxLRX_DEBUG_PRINTF(("Numeric 8")); break;
     case 0x29: MxLRX_DEBUG_PRINTF(("Numeric 9")); break;
     case 0x2A: MxLRX_DEBUG_PRINTF(("Dot")); break;
     case 0x2B: MxLRX_DEBUG_PRINTF(("Enter")); break;
     case 0x2C: MxLRX_DEBUG_PRINTF(("Clear")); break;

     case 0x30: MxLRX_DEBUG_PRINTF(("Channel Up")); break;
     case 0x31: MxLRX_DEBUG_PRINTF(("Channel Down")); break;
     case 0x32: MxLRX_DEBUG_PRINTF(("Previous Channel")); break;
     case 0x33: MxLRX_DEBUG_PRINTF(("Sound Select")); break;
     case 0x34: MxLRX_DEBUG_PRINTF(("Input Select")); break;
     case 0x35: MxLRX_DEBUG_PRINTF(("Show Information")); break;
     case 0x36: MxLRX_DEBUG_PRINTF(("Help")); break;
     case 0x37: MxLRX_DEBUG_PRINTF(("Page Up")); break;
     case 0x38: MxLRX_DEBUG_PRINTF(("Page Down")); break;

     case 0x41: MxLRX_DEBUG_PRINTF(("Volume Up")); break;
     case 0x42: MxLRX_DEBUG_PRINTF(("Volume Down")); break;
     case 0x43: MxLRX_DEBUG_PRINTF(("Mute")); break;
     case 0x44: MxLRX_DEBUG_PRINTF(("Play")); break;
     case 0x45: MxLRX_DEBUG_PRINTF(("Stop")); break;
     case 0x46: MxLRX_DEBUG_PRINTF(("Pause")); break;
     case 0x47: MxLRX_DEBUG_PRINTF(("Record")); break;
     case 0x48: MxLRX_DEBUG_PRINTF(("Rewind")); break;
     case 0x49: MxLRX_DEBUG_PRINTF(("Fast Forward")); break;
     case 0x4A: MxLRX_DEBUG_PRINTF(("Eject")); break;
     case 0x4B: MxLRX_DEBUG_PRINTF(("Forward")); break;
     case 0x4C: MxLRX_DEBUG_PRINTF(("Backward")); break;

     case 0x50: MxLRX_DEBUG_PRINTF(("Angle")); break;
     case 0x51: MxLRX_DEBUG_PRINTF(("Subpicture")); break;

     case 0x60: MxLRX_DEBUG_PRINTF(("Play_Function")); break;
     case 0x61: MxLRX_DEBUG_PRINTF(("Pause_Play_Function")); break;
     case 0x62: MxLRX_DEBUG_PRINTF(("Record_Function")); break;
     case 0x63: MxLRX_DEBUG_PRINTF(("Pause_Record_Function")); break;
     case 0x64: MxLRX_DEBUG_PRINTF(("Stop")); break;
     case 0x65: MxLRX_DEBUG_PRINTF(("Mute")); break;
     case 0x66: MxLRX_DEBUG_PRINTF(("Restore_Volume_Function")); break;
     case 0x67: MxLRX_DEBUG_PRINTF(("Tune_Function")); break;
     case 0x68: MxLRX_DEBUG_PRINTF(("Select_Media_Function")); break;

     case 0x71: MxLRX_DEBUG_PRINTF(("F1 (Blue)")); break;
     case 0x72: MxLRX_DEBUG_PRINTF(("F2 (Red)")); break;
     case 0x73: MxLRX_DEBUG_PRINTF(("F3 (Green)")); break;
     case 0x74: MxLRX_DEBUG_PRINTF(("F4 (Yellow)")); break;
     case 0x75: MxLRX_DEBUG_PRINTF(("F5")); break;
     case 0x7E: MxLRX_DEBUG_PRINTF(("Vendor_Specific")); break;

     default  : MxLRX_DEBUG_PRINTF(("ERROR: Reserved RCP sub-command code !!!\n")); suppkey = FALSE;
     }

     if( suppkey ) {
         if( rcpkey&0x80 )
             MxLRX_DEBUG_PRINTF((" Key Release\n"));
         else
             MxLRX_DEBUG_PRINTF((" Key Press\n"));
     }
}

////////////////////////////////////////////////////////////////////
//static void MxL_parse_RCPkey(struct it6802_dev_data *it6802)
//
//
//
////////////////////////////////////////////////////////////////////
void MxL_parse_RCPkey(struct it6802_dev_data *it6802)
{
    parse_rcpkey( it6802->rxmsgdata[1]);

    // FIX_ID_058
    if( (SuppRCPCode[it6802->rxmsgdata[1]])&0x7F ){

        it6802->txmsgdata[0] = MSG_RCPK;
        it6802->txmsgdata[1] = it6802->rxmsgdata[1];
        MxL_MSC_DEBUG_PRINTF(("Send a RCPK with action code = 0x%02X\n", (int) it6802->txmsgdata[1]));

        //not yet !!!rcp_report_event(it6802->rxmsgdata[1]);

    }
    else {
        it6802->txmsgdata[0] = MSG_RCPE;
        it6802->txmsgdata[1] = 0x01;

        MxL_MSC_DEBUG_PRINTF(("Send a RCPE with status code = 0x%02X\n",(int)  it6802->txmsgdata[1]));
    }

    CsigBus_send_mscmsg(it6802);
    SwitchRCPResult(it6802,RCP_Result_Finish);

}
#endif


#ifdef _SUPPORT_RAP_
void MxL_parse_RAPkey(struct it6802_dev_data *it6802)
{
    //parse_rapkey( it6802->rxmsgdata[1]);

    it6802->txmsgdata[0] = MSG_RAPK;

    if( SuppRAPCode[it6802->rxmsgdata[1]]) {
        it6802->txmsgdata[1] = 0x00;
    }
    else{
        it6802->txmsgdata[1] = 0x02;
    }

    switch( it6802->rxmsgdata[1] ) {
        case 0x00:
            MxL_MSC_DEBUG_PRINTF(("Poll\n"));
            break;
        case 0x10:
            MxL_MSC_DEBUG_PRINTF(("Change to CONTENT_ON state\n"));
            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            //xxxxx 2014-0529 //Manual Content On/Off
            IT6802_RAPContentOff(0);    //(1);    //xxxxx 2014-0520 For Turn on Video output
            //xxxxx 2014-0529
            //FIX_ID_037 xxxxx
            break;
        case 0x11:
            MxL_MSC_DEBUG_PRINTF(("Change to CONTENT_OFF state\n"));
            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            //xxxxx 2014-0529 //Manual Content On/Off
            IT6802_RAPContentOff(1);    //xxxxx 2014-0520 For Turn off Video output
            //xxxxx 2014-0529
            //FIX_ID_037 xxxxx
            break;
        default  :

        it6802->txmsgdata[1] = 0x01;
        MxL_MSC_DEBUG_PRINTF(("ERROR: Unknown RAP action code 0x%02X !!!\n",(int)  it6802->rxmsgdata[1]));
        MxL_MSC_DEBUG_PRINTF(("Send a RAPK with status code = 0x%02X\n", (int) it6802->txmsgdata[1]));
     }

         if( it6802->rxmsgdata[1]==0x00 || it6802->rxmsgdata[1]==0x10 || it6802->rxmsgdata[1]==0x11 ) {
             it6802->txmsgdata[0] = MSG_RAPK;
//             if( RAPBusyCnt!=0 )
//                 txmsgdata[1] = 0x03;
//         else
        {
                 if( SuppRAPCode[it6802->rxmsgdata[1]] ) {
                     it6802->txmsgdata[1] = 0x00;
                     //RAPBusyCnt = RAPBUSYNUM;
                 }
                 else
                     it6802->txmsgdata[1] = 0x02;
             }
//             printf("Send a RAPK with status code = 0x%02X (RxSeqNum=%X)\n", txmsgdata[1], RxRapMsgSeq++);
//             send_mscmsg();
         }
         else {
             it6802->txmsgdata[0] = MSG_RAPK;
             it6802->txmsgdata[1] = 0x01;
//             printf("Send a RAPK with status code = 0x%02X (RxSeqNum=%X)\n", txmsgdata[1], RxRapMsgSeq++);
//             send_mscmsg();
//             TxMSGIdle = TRUE;
         }

    CsigBus_send_mscmsg(it6802);
}

#endif

#ifdef _SUPPORT_UCP_
void ucp_report_event( unsigned char key)
{
   // struct it6802_dev_data *it6802data = get_it6802_dev_data();

    //MxL_MSC_DEBUG_PRINTF(("ucp_report_event key: %X\n", (int) key));
    //input_report_key(it6802data->ucp_input, (unsigned int)key+1, 1);
    //input_report_key(it6802data->ucp_input, (unsigned int)key+1, 0);
    //input_sync(it6802data->ucp_input);

}
void MxL_parse_UCPkey(struct it6802_dev_data *it6802)
{

    //parse_ucpkey( it6802->rxmsgdata[1] );

    if( (it6802->rxmsgdata[1]&0x80)==0x00 ) {
        it6802->txmsgdata[0] = MSG_UCPK;
        it6802->txmsgdata[1] = it6802->rxmsgdata[1];
        ucp_report_event(it6802->rxmsgdata[1]);
    }
    else {
        it6802->txmsgdata[0] = MSG_UCPE;
        it6802->txmsgdata[1] = 0x01;
    }

    CsigBus_send_mscmsg(it6802);
}
#endif


////////////////////////////////////////////////////////////////////
//static void MxL_read_mscmsg( struct it6802_dev_data *it6802 )
//
//
//
////////////////////////////////////////////////////////////////////
void MxL_read_mscmsg( struct it6802_dev_data *it6802 )
{
//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
    unsigned int CsigBuswaitcnt=0;
    unsigned char MxL04;
//FIX_ID_024    xxxxx

    //MxLtxbrd(0x60, 2, &it6802->rxmsgdata[0]);
    it6802->rxmsgdata[0] = MxLrxrd(0x60);
    it6802->rxmsgdata[1] = MxLrxrd(0x61);

    switch( it6802->rxmsgdata[0] ) {
        case MSG_MSGE :
            MxL_MSC_DEBUG_PRINTF(("RX MSGE => "));
            switch( it6802->rxmsgdata[1] ) {
                case 0x00:
                    MxL_MSC_DEBUG_PRINTF(("No Error\n"));
                    break;
                case 0x01:
                    MxL_MSC_DEBUG_PRINTF(("ERROR: Invalid sub-command code !!!\n"));
                    break;
                default  :
                    MxL_MSC_DEBUG_PRINTF(("ERROR: Unknown MSC_MSG status code 0x%02X !!!\n", (int) it6802->rxmsgdata[1]));
            }
             break;
#ifdef _SUPPORT_RCP_
        case MSG_RCP  :
            MxL_parse_RCPkey(it6802);
            break;
        case MSG_RCPK :
             MxL_MSC_DEBUG_PRINTF(("RX RCPK  => "));
            parse_rcpkey( it6802->rxmsgdata[1]);
//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
            it6802->m_bRCPTimeOut = FALSE;
//FIX_ID_024    xxxxx
            SwitchRCPResult(it6802,RCP_Result_Finish);
             break;
        case MSG_RCPE :
             switch( it6802->rxmsgdata[1] ){
                case 0x00: MxL_MSC_DEBUG_PRINTF(("No Error\n")); break;
                case 0x01: MxL_MSC_DEBUG_PRINTF(("ERROR: Ineffective RCP Error Code !!!\n")); break;
                case 0x02: MxL_MSC_DEBUG_PRINTF(("Responder Busy ...\n")); break;
                default  : MxL_MSC_DEBUG_PRINTF(("ERROR: Unknown RCP status code !!!\n"));
             }
//FIX_ID_024 xxxxx    //Fixed for RCP compliance issue
            {

                    it6802->m_bRCPError = TRUE;
                    it6802->m_bRCPTimeOut = TRUE;
                    do
                    {
                        CsigBuswaitcnt++;
                        //MxL_INT_PRINTF(("IT6802-MSG_RCPE then waiting RCPK ...\n"));

                        MxL04 = MxLrxrd(MxL_RX_04);
                        if( MxL04&0x10 )
                        {
                            MxLrxwr(MxL_RX_04,0x10);

                            it6802->rxmsgdata[0] = MxLrxrd(0x60);
                            it6802->rxmsgdata[1] = MxLrxrd(0x61);

                            if(it6802->rxmsgdata[0] == MSG_RCPK)
                            {
                                parse_rcpkey( it6802->rxmsgdata[1]);
                                it6802->m_bRCPTimeOut=FALSE;
                            }

                            break;
                        }

                        //MxL_MSC_DEBUG_PRINTF((" MxLrxrd(0x04)= 0x%02X !!!\n",(int)   MxL04));
                        delay1ms(1);
                    }
                    while(CsigBuswaitcnt<CsigBusWAITNUM);

                    MxL_MSC_DEBUG_PRINTF((" MxLrxrd(0x04)= 0x%02X !!!\n",(int)   MxLrxrd(0x04)));
                    MxL_MSC_DEBUG_PRINTF((" MxLrxrd(0x05)= 0x%02X !!!\n",(int)   MxLrxrd(0x05)));
                    MxL_MSC_DEBUG_PRINTF((" MxLrxrd(0x06)= 0x%02X !!!\n",(int)   MxLrxrd(0x06)));
                    MxL_MSC_DEBUG_PRINTF((" it6802->rxmsgdata[0]= 0x%02X !!!\n",(int)   it6802->rxmsgdata[0]));
                    MxL_MSC_DEBUG_PRINTF((" it6802->rxmsgdata[1]= 0x%02X !!!\n",(int)   it6802->rxmsgdata[1]));
            }
//FIX_ID_024    xxxxx
            SwitchRCPResult(it6802,RCP_Result_Finish);

             break;
#endif
#ifdef _SUPPORT_RAP_
        case MSG_RAP  :
            MxL_parse_RAPkey(it6802);
            break;
        case MSG_RAPK :
             MxL_MSC_DEBUG_PRINTF(("RX RAPK  => "));
             switch( it6802->rxmsgdata[1] ) {
                case 0x00: MxL_MSC_DEBUG_PRINTF(("No Error\n")); break;
                case 0x01: MxL_MSC_DEBUG_PRINTF(("ERROR: Unrecognized Action Code !!!\n")); break;
                case 0x02: MxL_MSC_DEBUG_PRINTF(("ERROR: Unsupported Action Code !!!\n")); break;
                case 0x03: MxL_MSC_DEBUG_PRINTF(("Responder Busy ...\n")); break;
                default  : MxL_MSC_DEBUG_PRINTF(("ERROR: Unknown RAP status code 0x%02X !!!\n",(int)  it6802->rxmsgdata[1]));
            }
            break;
#endif
#ifdef _SUPPORT_UCP_
        case MSG_UCP  :
            MxL_parse_UCPkey(it6802);
            break;
        case MSG_UCPK :
             break;
        case MSG_UCPE :
             switch( it6802->rxmsgdata[1] ){
                case 0x00: MxL_MSC_DEBUG_PRINTF(("No Error\n")); break;
                case 0x01: MxL_MSC_DEBUG_PRINTF(("ERROR: Ineffective UCP Key Code !!!\n")); break;
                default  : MxL_MSC_DEBUG_PRINTF(("ERROR: Unknown UCP status code !!!\n"));
             }
             break;
#endif
        default :
            MxL_MSC_DEBUG_PRINTF(("ERROR: Unknown MSC_MSG sub-command code 0x%02X !!!\n",(int)  it6802->rxmsgdata[0]));
            it6802->txmsgdata[0] = MSG_MSGE;
            it6802->txmsgdata[1] = 0x01;
            CsigBus_send_mscmsg(it6802);
            SwitchRCPResult(it6802,RCP_Result_Finish);
    }
}

////////////////////////////////////////////////////////////////////
//void mscWait( void )
//
//
//
////////////////////////////////////////////////////////////////////
int mscWait( void )
{

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler

    int CsigBuswaitcnt;
    //unsigned char MxL04, MxL05, rddata[2];
    unsigned char MxL05, rddata[2];

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    CsigBuswaitcnt = 0;

    do
    {
        CsigBuswaitcnt++;
        delay1ms(CsigBusWAITTIME);
        MxL05 = MxLrxrd(MxL_RX_05);
        //printf("MSC Wait %X !!!\n",MxL05);
        if(MxL05 & 0x03) break;
    }
    while(CsigBuswaitcnt<CsigBusWAITNUM);
//    while( (MxLrxrd(0x1C)&0x06)!=0  && CsigBuswaitcnt<CsigBusWAITNUM);
//    while( (MxLrxrd(0x1C)&0x02)==0x02 && CsigBuswaitcnt<CsigBusWAITNUM   );

    if( MxL05&0x01 )
    {
    MxLrxwr(MxL_RX_05, 0x01);
    //printf("msc Wait Req Done ...Ok %X !!!\n",MxL05);
    }

     MxL05 = MxLrxrd(0x05);
     if( (CsigBuswaitcnt==CsigBusWAITNUM) || MxL05&0x02 )
     {
         if( CsigBuswaitcnt==CsigBusWAITNUM )
        MxLRX_DEBUG_PRINTF(("ERROR: MSC Wait TimeOut !!!\n"));


         if( MxL05&0x02 ) {
             MxLRX_DEBUG_PRINTF(("MSC Req Fail Interrupt ...Fail\n"));
             MxLrxbrd(0x18, 2, &rddata[0]);
             MxLrxwr(MxL_RX_05, 0x02);

             if( rddata[0]&0x01 ) {
                 MxLRX_DEBUG_PRINTF(("Incomplete Packet !!!\n"));
                 MxLrxwr(0x18, 0x01);
             }
             if( rddata[0]&0x02 ) {
                 MxLRX_DEBUG_PRINTF(("MSC 100ms TimeOut !!!\n"));
                 MxLrxwr(0x18, 0x02);
             }
             if( rddata[0]&0x04 ) {
                 MxLRX_DEBUG_PRINTF(("Protocol Error !!!\n"));
                 MxLrxwr(0x18, 0x04);
             }
             if( rddata[0]&0x08 ) {
                 MxLRX_DEBUG_PRINTF(("Retry > 32 times !!!\n"));
                 MxLrxwr(0x18, 0x08);
             }
             if( rddata[0]&0x10 ) {
                 MxLRX_DEBUG_PRINTF(("Receive ABORT Packet !!!\n"));
                 MxLrxwr(0x18, 0x10);
//                 return RCVABORT;
             }
             if( rddata[0]&0x20 ) {
                // MSGNackCnt++;
                // MSCFailCnt--;
                 MxLRX_DEBUG_PRINTF(("MSC_MSG Requester Receive NACK Packet !!!\n"));
                 MxLrxwr(0x18, 0x20);
//                 return RCVNACK;
             }
             if( rddata[0]&0x40 ) {
                 MxLRX_DEBUG_PRINTF(("Disable HW Retry and MSC Requester Arbitration Lose at 1st Packet !!!\n"));
                 MxLrxwr(0x18, 0x40);

//                if( enarblosetrg )
//                    printf("Trgger !!!");

//                 return ARBLOSE;
             }
            if( rddata[0]&0x80 ) {
                 MxLRX_DEBUG_PRINTF(("Disable HW Retry and MSC Requester Arbitration Lose before 1st Packet !!!\n"));

                 MxLrxwr(0x18, 0x80);
//                 return ARBLOSE;
             }

             if( rddata[1]&0x01 ) {
                 MxLRX_DEBUG_PRINTF(("TX FW Fail in the middle of the command sequence !!!\n"));
                 MxLrxwr(0x19, 0x01);
//                 return FWTXFAIL;
             }
             if( rddata[1]&0x02 ) {
                 MxLRX_DEBUG_PRINTF(("TX Fail because FW mode RxPktFIFO not empty !!!\n"));
                 MxLrxwr(0x19, 0x02);
//                 return FWRXPKT;
             }
         }
         else
             MxLRX_DEBUG_PRINTF(("Unknown Issue !!!\n"));

     MxLRX_DEBUG_PRINTF(("\n\n"));


    MxLRX_DEBUG_PRINTF(("!!! mscWait Fail !!!\n"));

#if 0
    printf("MxL04 %X\n",MxLrxrd(0x04));
    printf("MxL05 %X\n",MxLrxrd(0x05));
    printf("MxL06 %X\n\n",MxLrxrd(0x06));

    printf("MxL15 %X\n",MxLrxrd(0x15));
    printf("MxL16 %X\n",MxLrxrd(0x16));
    printf("MxL17 %X\n",MxLrxrd(0x17));
    printf("MxL18 %X\n",MxLrxrd(0x18));
    printf("MxL19 %X\n",MxLrxrd(0x19));
    printf("MxL1A %X\n",MxLrxrd(0x1A));
    printf("MxL1B %X\n",MxLrxrd(0x1B));
    printf("MxL1C %X\n\n",MxLrxrd(0x1C));
#endif

     return FAIL;
     }


//FIX_ID_005 xxxxx

    return SUCCESS;

}




//FIX_ID_037 xxxxx //Allion MxL compliance issue debug !!!
////////////////////////////////////////////////////////////////////
//void mscCheckResult( void )
//
//
//
////////////////////////////////////////////////////////////////////
int mscCheckResult(void)
{
//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
// disable ->    int  fwmodeflag = FALSE;
// disable ->    int  wrburstflag = FALSE;
    int mscreqsts=FAIL;
    int result=SUCCESS;
    int ucMxL1C=0;

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    //TickCountPrint();

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
    if(it6802data->CsigBusWaitNo !=0)
        return FAIL;
//FIX_ID_037 xxxxx

// disable ->    if( offset==0x51)
// disable ->    {
// disable ->        if(wdata==0x80 )
// disable ->            fwmodeflag  = TRUE;
// disable ->        if(wdata==0x01 )
// disable ->            wrburstflag = TRUE;
// disable ->    }


    ucMxL1C = MxLrxrd(MxL_RX_1C);

    //for debug only !!!
//RedText;
    //IT6802_MxL_DEBUG_PRINTF(("IT6802-MSC check() ucMxL1C = %X\n",(int) ucMxL1C));
    IT6802_MxL_DEBUG_PRINTF(("IT6802-mscCheckResult ()\n"));
//WhiteText;
    //for debug only !!!

    if((ucMxL1C & 0x07)==0)
    {
        //Allion MxL compliance issue debug !!! disable -> MxLrxwr((unsigned char)offset, (unsigned char)wdata);
        mscreqsts = mscWait();
        //IT6802_MxL_DEBUG_PRINTF(("IT6802-mscCheckResult () %X\n",(int) mscreqsts));

        if(mscreqsts != SUCCESS)
        {
            MxLRX_DEBUG_PRINTF(("mscreqsts = FAIL\n"));
//2014-0526 disable MxL compliance issue ->
            result = FAIL;
        }

    }
    else
    {
        MxLRX_DEBUG_PRINTF(("ucMxL1C %X\n",(int) ucMxL1C));
//2014-0526 disable MxL compliance issue ->
        result = FAIL;
    }

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
#ifdef _Debug_MSC_Abort_Only
//GreenText;
    if(m_MxLabortID == MxL_ABORT_ID)
    {
        it6802data->CsigBusIntEvent |= (B_MSC_Waiting);
        it6802data->CsigBusWaitNo=MAX_BUSY_WAITNO;    //100ms
        result = FAIL;
        MxLRX_DEBUG_PRINTF(("m_MxLabortID == MxL_ABORT_ID\n"));
        m_MxLabortID++;
    }
    else
    {
        MxLRX_DEBUG_PRINTF(("m_MxLabortID =%d\n",(int) m_MxLabortID));
    }
//WhiteText;
#endif
//FIX_ID_037 xxxxx

    if(result==FAIL)
    {
        it6802data->CsigBusIntEvent |= (B_MSC_Waiting);
        it6802data->CsigBusWaitNo=MAX_BUSY_WAITNO;    //100ms
    }

    return (result==SUCCESS)?SUCCESS:FAIL;

//FIX_ID_005 xxxxx
}
//FIX_ID_037 xxxxx
//FIX_ID_037 xxxxx //Allion MxL compliance issue debug !!!

////////////////////////////////////////////////////////////////////
//void mscFire( void )
//
//
//
////////////////////////////////////////////////////////////////////
int mscFire( int offset, int wdata )
{
//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
// disable ->    int  fwmodeflag = FALSE;
// disable ->    int  wrburstflag = FALSE;
    int mscreqsts=FAIL;
    int result=SUCCESS;
    int ucMxL1C=0;

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    // TickCountPrint();

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
    if(it6802data->CsigBusWaitNo !=0)
        return FAIL;
//FIX_ID_037 xxxxx

// disable ->    if( offset==0x51)
// disable ->    {
// disable ->        if(wdata==0x80 )
// disable ->            fwmodeflag  = TRUE;
// disable ->        if(wdata==0x01 )
// disable ->            wrburstflag = TRUE;
// disable ->    }


    ucMxL1C = MxLrxrd(MxL_RX_1C);

    //for debug only !!!
//RedText;
    //IT6802_MxL_DEBUG_PRINTF(("IT6802-MSC FIRE() ucMxL1C = %X\n",(int) ucMxL1C));
    IT6802_MxL_DEBUG_PRINTF(("IT6802-MSC FIRE() offset = %X wdata = %X\n",(int) offset , (int) wdata));
//WhiteText;
    //for debug only !!!

    if((ucMxL1C & 0x07)==0)
    {
        MxLrxwr((unsigned char)offset, (unsigned char)wdata);
        mscreqsts = mscWait();
        //IT6802_MxL_DEBUG_PRINTF(("IT6802-MSC FIRE() %X\n",(int) mscreqsts));

        if(mscreqsts != SUCCESS)
        {
            MxLRX_DEBUG_PRINTF(("mscreqsts = FAIL\n"));
//2014-0526 disable MxL compliance issue ->
            result = FAIL;
        }

    }
    else
    {
        MxLRX_DEBUG_PRINTF(("ucMxL1C %X\n",(int) ucMxL1C));
//2014-0526 disable MxL compliance issue ->
        result = FAIL;
    }

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
#ifdef _Debug_MSC_Abort_Only
// GreenText;
    if(m_MxLabortID == MxL_ABORT_ID)
    {
        it6802data->CsigBusIntEvent |= (B_MSC_Waiting);
        it6802data->CsigBusWaitNo=MAX_BUSY_WAITNO;    //100ms
        result = FAIL;
        MxLRX_DEBUG_PRINTF(("m_MxLabortID == MxL_ABORT_ID\n"));
        m_MxLabortID++;
    }
    else
    {
        MxLRX_DEBUG_PRINTF(("m_MxLabortID =%d\n",(int) m_MxLabortID));
    }
//  WhiteText;
#endif
//FIX_ID_037 xxxxx

    if(result==FAIL)
    {
        it6802data->CsigBusIntEvent |= (B_MSC_Waiting);
        it6802data->CsigBusWaitNo=MAX_BUSY_WAITNO;    //100ms
    }

    return (result==SUCCESS)?SUCCESS:FAIL;

//FIX_ID_005 xxxxx
}
//FIX_ID_037 xxxxx


int CsigBus_send_mscmsg( struct it6802_dev_data *it6802 )
{
    int uc;

    MxLrxwr(0x54, it6802->txmsgdata[0]);
    MxLrxwr(0x55, it6802->txmsgdata[1]);
    uc = mscFire(MxL_MSC_CtrlPacket1, B_MSC_MSG);

    return (uc==SUCCESS)?SUCCESS:FAIL;
}

#if 1
//== MxL interrupt ===
//
//
//
//
void parse_devcap(unsigned char *devcap )
{
    MxLRX_DEBUG_PRINTF(("\nParsing Device Capability Register ...\n\n"));
    MxLRX_DEBUG_PRINTF(("DEV_STATE=%02X\n", (int) devcap[0]));

    MxLRX_DEBUG_PRINTF(("MxL_VER_MAJOR/MINOR=%02X\n", (int) devcap[1]));

    MxLRX_DEBUG_PRINTF(("DEV_TYPE="));
    switch(devcap[2]&0x0F){
        case 0: MxLRX_DEBUG_PRINTF(("ERROR: DEV_TYPE at least one bit must be set !!!\n")); break;
        case 1: MxLRX_DEBUG_PRINTF(("DEV_TYPE = Sink, ")); break;
        case 2: MxLRX_DEBUG_PRINTF(("DEV_TYPE = Source, ")); break;
        case 3: MxLRX_DEBUG_PRINTF(("DEV_TYPE = Dongle, ")); break;
        default: MxLRX_DEBUG_PRINTF(("ERROR: Reserved for future use !!! ")); break;
    }
    MxLRX_DEBUG_PRINTF(("POW = %02X\n", (int) (devcap[2]&0x10)>>4));
    MxLRX_DEBUG_PRINTF(("PLIM = %02X\n", (int) (devcap[2]&0x60)>>5));

    MxLRX_DEBUG_PRINTF(("ADOPTER_ID_H=0x%02X, ADOPTER_ID_L=0x%02X\n",(int)  devcap[3], (int) devcap[4]));

    MxLRX_DEBUG_PRINTF(("VID_LINK_MODE:\n"));
    MxLRX_DEBUG_PRINTF(("    SUPP_RGB444   = %02X\n", (int) (devcap[5]&0x01)>>0));
    MxLRX_DEBUG_PRINTF(("    SUPP_YCBCR444 = %02X\n", (int) (devcap[5]&0x02)>>1));
    MxLRX_DEBUG_PRINTF(("    SUPP_YCBCR422 = %02X\n", (int) (devcap[5]&0x04)>>2));
    MxLRX_DEBUG_PRINTF(("    SUPP_PPIXEL   = %02X\n", (int) (devcap[5]&0x08)>>3));
    MxLRX_DEBUG_PRINTF(("    SUPP_ISLANDS  = %02X\n", (int) (devcap[5]&0x10)>>4));
    MxLRX_DEBUG_PRINTF(("    SUPP_VGA      = %02X\n",(int)  (devcap[5]&0x20)>>5));

    MxLRX_DEBUG_PRINTF(("AUD_LINK_MODE:\n"));
    MxLRX_DEBUG_PRINTF(("    SUPP_2CH  = %02X\n", (int) (devcap[6]&0x01)>>0));
    MxLRX_DEBUG_PRINTF(("    SUPP_8CH  = %02X\n", (int) (devcap[6]&0x02)>>1));

    if( devcap[7]&0x80 ) {
        MxLRX_DEBUG_PRINTF(("VIDEO_TYPE:\n"));
        MxLRX_DEBUG_PRINTF(("    VT_GRAPHICS = %02X\n",(int)  (devcap[7]&0x01)>>0));
        MxLRX_DEBUG_PRINTF(("    VT_PHOTO    = %02X\n", (int) (devcap[7]&0x02)>>1));
        MxLRX_DEBUG_PRINTF(("    VT_CINEMA   = %02X\n", (int) (devcap[7]&0x04)>>2));
        MxLRX_DEBUG_PRINTF(("    VT_GAME     = %02X\n", (int) (devcap[7]&0x08)>>3));
    }
    else
        MxLRX_DEBUG_PRINTF(("Not Support VIDEO_TYPE !!!\n"));

    MxLRX_DEBUG_PRINTF(("LOG_DEV_MAP:\n"));
    MxLRX_DEBUG_PRINTF(("    LD_DISPLAY  = %02X\n",(int)  (devcap[8]&0x01)>>0));
    MxLRX_DEBUG_PRINTF(("    LD_VIDEO    = %02X\n",(int)  (devcap[8]&0x02)>>1));
    MxLRX_DEBUG_PRINTF(("    LD_AUDIO    = %02X\n", (int) (devcap[8]&0x04)>>2));
    MxLRX_DEBUG_PRINTF(("    LD_MEDIA    = %02X\n",(int)  (devcap[8]&0x08)>>3));
    MxLRX_DEBUG_PRINTF(("    LD_TUNER    = %02X\n", (int) (devcap[8]&0x10)>>4));
    MxLRX_DEBUG_PRINTF(("    LD_RECORD   = %02X\n",(int)  (devcap[8]&0x20)>>5));
    MxLRX_DEBUG_PRINTF(("    LD_SPEAKER  = %02X\n",(int)  (devcap[8]&0x40)>>6));
    MxLRX_DEBUG_PRINTF(("    LD_GUI      = %02X\n", (int) (devcap[8]&0x80)>>7));

    MxLRX_DEBUG_PRINTF(("BANDWIDTH = %XMHz\n", (int) devcap[9]*5));

    MxLRX_DEBUG_PRINTF(("FEATURE_FLAG:\n"));
    MxLRX_DEBUG_PRINTF(("         RCP_SUPPORT = %02X\n", (int) (devcap[10]&0x01)>>0));
    MxLRX_DEBUG_PRINTF(("         RAP_SUPPORT = %02X\n", (int) (devcap[10]&0x02)>>1));
    MxLRX_DEBUG_PRINTF(("         SP_SUPPORT  = %02X\n", (int) (devcap[10]&0x04)>>2));
    MxLRX_DEBUG_PRINTF(("    UCP_SEND_SUPPORT = %02X\n", (int) (devcap[10]&0x08)>>3));
    MxLRX_DEBUG_PRINTF(("    UCP_RECV_SUPPORT = %02X\n", (int) (devcap[10]&0x10)>>4));

    MxLRX_DEBUG_PRINTF(("DEVICE_ID_H=0x%02X, DEVICE_ID_L=0x%02X\n", (int) devcap[11], (int) devcap[12]));

    if( devcap[10]&0x04 )
        MxLRX_DEBUG_PRINTF(("SCRATCHPAD_SIZE = %02X Bytes\n", (int) devcap[13]));

    MxLRX_DEBUG_PRINTF(("INT_SIZE  = %02X Bytes\n", (int) (devcap[14]&0x0F)+1));
    MxLRX_DEBUG_PRINTF(("STAT_SIZE = %02X Bytes\n",(int)  ((devcap[14]&0xF0)>>4)+1));

}

////////////////////////////////////////////////////////////////////
//void read_devcap_hw( void )
//
//
//
////////////////////////////////////////////////////////////////////
int read_devcap_hw( struct it6802_dev_data *it6802 )
{
    unsigned char offset;

    IT6802_MxL_DEBUG_PRINTF(("IT6802-\nRead Device Capability Start ...\n"));

    for(offset=0; offset<0x10; offset++) {

        MxLrxwr(0x54, offset);

        if( mscFire(MxL_MSC_CtrlPacket0, B_READ_DEVCAP) == SUCCESS )
            it6802->MxL_devcap[offset] = MxLrxrd(0x56);
        else
            return -1;


        IT6802_MxL_DEBUG_PRINTF(("IT6802-DevCap[%X]=%X\n", (int)offset, (int)it6802->MxL_devcap[offset]));
    }

    IT6802_MxL_DEBUG_PRINTF(("IT6802-Read Device Capability End...\n"));

    parse_devcap(&it6802->MxL_devcap[0]);

    return 0;

}


int set_MxLint( unsigned char offset, unsigned char field )
{
    int uc;
    MxLrxwr(0x54, offset);
    MxLrxwr(0x55, field);
    uc = mscFire(MxL_MSC_CtrlPacket0, B_WRITE_STAT_SET_INT);

    return (uc==SUCCESS)?SUCCESS:FAIL;

}

int set_MxLsts( unsigned char offset, unsigned char status )
{
    int uc;
    MxLrxwr(0x54, offset);
    MxLrxwr(0x55, status);
    uc = mscFire(MxL_MSC_CtrlPacket0, B_WRITE_STAT_SET_INT);

    if(uc==SUCCESS)
        MxLRX_DEBUG_PRINTF(("uc==SUCCESS\n"));
    else
        MxLRX_DEBUG_PRINTF(("uc = FAIL\n"));


    return (uc==SUCCESS)?SUCCESS:FAIL;
}

// disable -> static void write_burst(struct it6802_dev_data *it6802, int offset, int byteno )
// disable -> {
// disable ->     set_wrburst_data(it6802, offset, byteno);
// disable -> //    MxLRX_DEBUG_PRINTF(("Fire WRITE_BURST (WrData=0x%X) ...\n", it6802->txscrpad[3]));
// disable -> //  enarblosetrg = TRUE;
// disable ->     mscFire(MxL_MSC_CtrlPacket1, B_WRITE_BURST);        // fire WRITE_BURST
// disable -> //  enarblosetrg = FALSE;
// disable ->     MxLrxset(0x00, 0x80, EnCsigBusDbgMode<<7);  // Restore setting
// disable -> }
// disable ->
// disable -> static void set_wrburst_data(struct it6802_dev_data *it6802, int offset, int byteno )
// disable -> {
// disable ->     int i;
// disable ->
// disable ->     if( byteno<=2 || (offset+byteno)>16 ) {
// disable ->         MxLRX_DEBUG_PRINTF(("ERROR: Set Burst Write Data Fail !!!\n"));
// disable ->         return;
// disable ->     }
// disable ->
// disable -> //    MxLrxbwr(0x5E, 2, &it6802->txscrpad[0]);
// disable ->     MxLrxwr(0x54, 0x40+offset);
// disable ->
// disable ->     if( EnMSCBurstWr ) {
// disable ->         MxLrxset(0x00, 0x80, 0x00); // Disable CsigBus Debug Mode when using Burst Write
// disable ->
// disable ->
// disable ->         if( MSCBurstWrID )
// disable ->             i = 2;
// disable ->         else
// disable ->             i = 0;
// disable ->
// disable ->        for(; i<byteno; i++)
// disable ->             MxLrxwr(0x59, it6802->txscrpad[i]);
// disable ->
// disable ->     }
// disable ->     else
// disable ->         MxLrxwr(0x55, it6802->txscrpad[2]);
// disable -> }


#endif

//#if 0
//static void v3d_burst1st(struct it6802_dev_data *it6802)
//{
//     it6802->txscrpad[0]  = 0x00;
//     it6802->txscrpad[1]  = 0x11;
//     it6802->txscrpad[2]  = 0xC1;
//     it6802->txscrpad[3]  = 0x05;
//     it6802->txscrpad[4]  = 0x01;
//     it6802->txscrpad[5]  = 0x05;
//     it6802->txscrpad[6]  = 0x00;
//     it6802->txscrpad[7]  = 0x07;
//     it6802->txscrpad[8]  = 0x00;
//     it6802->txscrpad[9]  = 0x07;
//     it6802->txscrpad[10] = 0x00;
//     it6802->txscrpad[11] = 0x07;
//     it6802->txscrpad[12] = 0x00;
//     it6802->txscrpad[13] = 0x07;
//     it6802->txscrpad[14] = 0x00;
//     it6802->txscrpad[15] = 0x07;
//
//     MxLrxwr(0x5E, it6802->txscrpad[0]);
//     MxLrxwr(0x5F, it6802->txscrpad[1]);
//     wrburstoff = 0;
//     wrburstnum = 16;
//     write_burst(it6802,wrburstoff, wrburstnum);
//     MxLRX_DEBUG_PRINTF(("Set DSCR_CHG to 1 (TxSeqNum=%X) ...\n", TxWrBstSeq++));
//     set_MxLint(MxLInt00B, DSCR_CHG);
//    V3D_EntryCnt++;
//     set_MxLint(MxLInt00B, REQ_WRT);
//}
//
//static void v3d_burst2nd(struct it6802_dev_data *it6802)
//{
//     it6802->txscrpad[0]  = 0x00;
//     it6802->txscrpad[1]  = 0x10;
//     it6802->txscrpad[2]  = 0xB8;
//     it6802->txscrpad[3]  = 0x0F;
//     it6802->txscrpad[4]  = 0x01;
//     it6802->txscrpad[5]  = 0x05;
//     it6802->txscrpad[6]  = 0x00;
//     it6802->txscrpad[7]  = 0x07;
//     it6802->txscrpad[8]  = 0x00;
//     it6802->txscrpad[9]  = 0x07;
//     it6802->txscrpad[10] = 0x00;
//     it6802->txscrpad[11] = 0x07;
//     it6802->txscrpad[12] = 0x00;
//     it6802->txscrpad[13] = 0x07;
//     it6802->txscrpad[14] = 0x00;
//     it6802->txscrpad[15] = 0x07;
//
//     MxLrxwr(0x5E, it6802->txscrpad[0]);
//     MxLrxwr(0x5F, it6802->txscrpad[1]);
//     wrburstoff = 0;
//     wrburstnum = 16;
//     write_burst(it6802,wrburstoff, wrburstnum);
//     MxLRX_DEBUG_PRINTF(("Set DSCR_CHG to 1 (TxSeqNum=%X) ...\n", TxWrBstSeq++));
//     set_MxLint(MxLInt00B, DSCR_CHG);
//    V3D_EntryCnt++;
//     set_MxLint(MxLInt00B, REQ_WRT);
//}
//
//static void v3d_burst3rd(struct it6802_dev_data *it6802)
//{
//     it6802->txscrpad[0]  = 0x00;
//     it6802->txscrpad[1]  = 0x10;
//     it6802->txscrpad[2]  = 0xB7;
//     it6802->txscrpad[3]  = 0x0F;
//     it6802->txscrpad[4]  = 0x02;
//     it6802->txscrpad[5]  = 0x05;
//     it6802->txscrpad[6]  = 0x00;
//     it6802->txscrpad[7]  = 0x07;
//     it6802->txscrpad[8]  = 0x00;
//     it6802->txscrpad[9]  = 0x07;
//     it6802->txscrpad[10] = 0x00;
//     it6802->txscrpad[11] = 0x07;
//     it6802->txscrpad[12] = 0x00;
//     it6802->txscrpad[13] = 0x07;
//     it6802->txscrpad[14] = 0x00;
//     it6802->txscrpad[15] = 0x07;
//
//     MxLrxwr(0x5E, it6802->txscrpad[0]);
//     MxLrxwr(0x5F, it6802->txscrpad[1]);
//     wrburstoff = 0;
//     wrburstnum = 16;
//     write_burst(it6802,wrburstoff, wrburstnum);
//     MxLRX_DEBUG_PRINTF(("Set DSCR_CHG to 1 (TxSeqNum=%X) ...\n", TxWrBstSeq++));
//     set_MxLint(MxLInt00B, DSCR_CHG);
//    V3D_EntryCnt++;
//     set_MxLint(MxLInt00B, REQ_WRT);
//}
//
//static void v3d_burst4th(struct it6802_dev_data *it6802)
//{
//     it6802->txscrpad[0]  = 0x00;
//     it6802->txscrpad[1]  = 0x10;
//     it6802->txscrpad[2]  = 0xB6;
//     it6802->txscrpad[3]  = 0x0F;
//     it6802->txscrpad[4]  = 0x03;
//     it6802->txscrpad[5]  = 0x05;
//     it6802->txscrpad[6]  = 0x00;
//     it6802->txscrpad[7]  = 0x07;
//     it6802->txscrpad[8]  = 0x00;
//     it6802->txscrpad[9]  = 0x07;
//     it6802->txscrpad[10] = 0x00;
//     it6802->txscrpad[11] = 0x07;
//     it6802->txscrpad[12] = 0x00;
//     it6802->txscrpad[13] = 0x07;
//     it6802->txscrpad[14] = 0x00;
//     it6802->txscrpad[15] = 0x07;
//
//     MxLrxwr(0x5E, it6802->txscrpad[0]);
//     MxLrxwr(0x5F, it6802->txscrpad[1]);
//     wrburstoff = 0;
//     wrburstnum = 16;
//     write_burst(it6802,wrburstoff, wrburstnum);
//     MxLRX_DEBUG_PRINTF(("Set DSCR_CHG to 1 (TxSeqNum=%X) ...\n", TxWrBstSeq++));
//     set_MxLint(MxLInt00B, DSCR_CHG);
//    V3D_EntryCnt++;
//}
//
//
//
//static void v3d_unsupport1st(struct it6802_dev_data *it6802)
//{
////FIX_ID_013    xxxxx    //For Acer MxL Dongle MSC 3D request issue
//     it6802->txscrpad[0]  = 0x00;
//     it6802->txscrpad[1]  = 0x10;
//     it6802->txscrpad[2]  = 0xF0;
//     it6802->txscrpad[3]  = 0x00;
////FIX_ID_013    xxxxx
//
//
//     MxLrxwr(0x5E, it6802->txscrpad[0]);
//     MxLrxwr(0x5F, it6802->txscrpad[1]);
//     wrburstoff = 0;
//     wrburstnum = 4;
//     write_burst(it6802,wrburstoff, wrburstnum);
//
//     MxLRX_DEBUG_PRINTF(("Set DSCR_CHG to 1 (TxSeqNum=%X) ...\n", TxWrBstSeq++));
//     set_MxLint(MxLInt00B, DSCR_CHG);
//    V3D_EntryCnt++;
//     set_MxLint(MxLInt00B, REQ_WRT);
//}
//
//static void v3d_unsupport2nd(struct it6802_dev_data *it6802)
//{
////FIX_ID_013    xxxxx    //For Acer MxL Dongle MSC 3D request issue
//     it6802->txscrpad[0]  = 0x00;
//     it6802->txscrpad[1]  = 0x11;
//     it6802->txscrpad[2]  = 0xEF;
//     it6802->txscrpad[3]  = 0x00;
////FIX_ID_013    xxxxx
//
//     MxLrxwr(0x5E, it6802->txscrpad[0]);
//     MxLrxwr(0x5F, it6802->txscrpad[1]);
//     wrburstoff = 0;
//     wrburstnum = 4;
//     write_burst(it6802,wrburstoff, wrburstnum);
//     MxLRX_DEBUG_PRINTF(("Set DSCR_CHG to 1 (TxSeqNum=%X) ...\n", TxWrBstSeq++));
//     set_MxLint(MxLInt00B, DSCR_CHG);
//    V3D_EntryCnt++;
//}
//
//#endif
/* MxL 3D functions    *******************************************************/

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
unsigned char Msc_WriteBurstDataFill(unsigned char ucOffset, unsigned char ucByteNo, unsigned char *pucData)
{
    unsigned char    ucTemp;

    if(ucByteNo<=2 || (ucOffset+ucByteNo)>16){
        MxL3D_DEBUG_PRINTF(("ERROR: Set Burst Write Data Fail\n"));
        return FALSE;
    }else{
        MxLrxwr(0x5E, pucData[0]);
        MxLrxwr(0x5F, pucData[1]);
        MxLrxwr(0x54, 0x40+ucOffset);
        if(MSCBurstWrID)
            ucTemp = 2;
        else
            ucTemp = 0;

        //disable ->    MxLRX_DEBUG_PRINTF(("Msc_WriteBurstDataFill ="));
        for(; ucTemp < ucByteNo;)
        {
            //disable ->    MxLRX_DEBUG_PRINTF((" 0x%x\n ", (int)pucData[ucTemp]));
            MxLrxwr(0x59, pucData[ucTemp++]);
        }
        //disable ->    MxLRX_DEBUG_PRINTF(("\n"));
        return TRUE;
    }
}



MxL3D_STATE MSC_3DInforSend(unsigned char b3dDtd)
{
    unsigned char    ucTemp, uc3DInforLen;
    unsigned char    ucWBData[16];
    unsigned char    uc3DTempCnt;
    MxL3D_STATE eRet3dState;

    uc3DTempCnt = st3DParse.uc3DTempCnt & 0x7F;

    if(b3dDtd){
        ucWBData[0] = MSC_3D_DTD >> 8;
        ucWBData[1] = MSC_3D_DTD & 0xff;
        ucWBData[3] = st3DParse.ucDtdCnt;
    }else{
        ucWBData[0] = MSC_3D_VIC >> 8;
        ucWBData[1] = MSC_3D_VIC & 0xff;
        ucWBData[3] = st3DParse.ucVicCnt;
    }
    ucWBData[2] = 0;
    ucWBData[4] = (uc3DTempCnt / 5) + 1;

    ucWBData[5] = ucWBData[3] - uc3DTempCnt;
    if(ucWBData[5] > 5){
        ucWBData[5] = 5;
        eRet3dState = MxL3D_REQ_WRT;
        st3DParse.uc3DTempCnt += 5;
        MxL3D_DEBUG_PRINTF(("*** MSC_3DInforSend MxL3D_REQ_WRT ***\n"));
    }else{
        if(b3dDtd){
            st3DParse.uc3DTempCnt = 0;
            eRet3dState = MxL3D_REQ_WRT;
            MxL3D_DEBUG_PRINTF(("*** MSC_3DInforSend DTD Done ***\n"));
        }else{
            st3DParse.uc3DTempCnt = 0x80;
            eRet3dState = MxL3D_REQ_DONE;
            MxL3D_DEBUG_PRINTF(("*** MSC_3DInforSend VIC Done ***\n"));
        }
    }
    uc3DInforLen = 6 + (ucWBData[5] * 2);
    for(ucTemp = 6; ucTemp < uc3DInforLen; ){
        ucWBData[ucTemp++] = 0x00;
        if(b3dDtd){
            ucWBData[ucTemp++] = uc3DDtd[uc3DTempCnt++];
        }else{
            ucWBData[ucTemp++] = st3DParse.uc3DInfor[uc3DTempCnt++];
        }
    }
    do{
        if(--ucTemp != 2){
            ucWBData[2] -= ucWBData[ucTemp];
        }
    }while(ucTemp);


    MxLRX_DEBUG_PRINTF(("txscrpad --> "));
    for(ucTemp=0;ucTemp < uc3DInforLen;ucTemp++)
    {
    MxLRX_DEBUG_PRINTF(("%x ",(int)ucWBData[ucTemp]));
    }
    MxLRX_DEBUG_PRINTF(("\n"));

    if(TRUE == Msc_WriteBurstDataFill(0x00, uc3DInforLen, ucWBData))
    {
        mscFire(MxL_MSC_CtrlPacket1, B_WRITE_BURST);        // fire WRITE_BURST
        set_MxLint(MxLInt00B, DSCR_CHG);
    }
    // disable -> MxLRX_DEBUG_PRINTF(("*** MSC_3DInforSend eRet3dState = 0x%x ***\n", (int)eRet3dState));
    return eRet3dState;
}


void Msc_3DProcess(MxL3D_STATE *e3DReqState)
{
    unsigned char    ucTemp;
    unsigned char    uc;

    MxLRX_DEBUG_PRINTF(("***Msc_3DProcess***\n"));
    switch(*e3DReqState){
        case    MxL3D_REQ_DONE:
                //MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MxL3D_REQ_DONE\n"));
            break;
        case    MxL3D_REQ_START:
                MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MxL3D_REQ_START\n"));
                for(ucTemp=0;ucTemp < 16;ucTemp++)
                {
                    uc =st3DParse.uc3DInfor[ucTemp];
                    MxLRX_DEBUG_PRINTF(("%x ",(int)uc));
                }
                MxLRX_DEBUG_PRINTF(("\n***Msc_3DProcess*** MxL3D_REQ_START\n"));

                ucTemp = sizeof(uc3DDtd);
                MxLRX_DEBUG_PRINTF(("\n uc3DDtd = %X\n",(int) uc3DDtd[0]));
                if((ucTemp == 1) && (uc3DDtd[0]==0))
                ucTemp = 0;
                st3DParse.ucDtdCnt = ucTemp;
                st3DParse.uc3DTempCnt = 0x80;


                set_MxLint(MxLInt00B, REQ_WRT);
                *e3DReqState = MxL3D_REQ_WRT;


            break;
        case    MxL3D_REQ_WRT:
                    MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MxL3D_REQ_WRT\n"));
                    *e3DReqState = MxL3D_GNT_WRT;
            break;
        case    MxL3D_GNT_WRT:
                MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MxL3D_GNT_WRT\n"));
                if(st3DParse.uc3DTempCnt & 0x80){
                    *e3DReqState = MSC_3DInforSend(TRUE);
                    //MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MSC_3DInforSend(TRUE)\n"));
                }else{
                    *e3DReqState = MSC_3DInforSend(FALSE);
                    //MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MSC_3DInforSend(FALSE)\n"));
                }
                    if(*e3DReqState == MxL3D_REQ_DONE){
                        st3DParse.uc3DTempCnt = 0x80;
                    MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MxL3D_REQ_DONE\n"));
                }else{

                    set_MxLint(MxLInt00B, REQ_WRT);
                    *e3DReqState = MxL3D_REQ_WRT;
                    MxLRX_DEBUG_PRINTF(("***Msc_3DProcess*** MxL3D_REQ_WRT\n"));
                }
            break;
    default:
        break;

    }

}
//FIX_ID_013    xxxxx
#else
void write_burst(struct it6802_dev_data *it6802, int offset, int byteno )
{
    set_wrburst_data(it6802,offset, byteno);
//    MxLRX_DEBUG_PRINTF(("Fire WRITE_BURST (WrData=0x%X) ...\n", it6802data->txscrpad[3]));
//  enarblosetrg = TRUE;
    mscFire(MxL_MSC_CtrlPacket1, B_WRITE_BURST);        // fire WRITE_BURST
//  enarblosetrg = FALSE;
    MxLrxset(0x00, 0x80, EnCsigBusDbgMode<<7);  // Restore setting
}

void set_wrburst_data(struct it6802_dev_data *it6802, int offset, int byteno )
{
    int i;
// disable ->
    if( byteno<=2 || (offset+byteno)>16 )
   {
        MxLRX_DEBUG_PRINTF(("ERROR: Set Burst Write Data Fail !!!\n"));
        return;
    }
// disable ->
//    MxLrxbwr(0x5E, 2, &it6802data->txscrpad[0]);
    MxLrxwr(0x54, 0x40+offset);
// disable ->
    if( EnMSCBurstWr )
  {
        MxLrxset(0x00, 0x80, 0x00); // Disable CsigBus Debug Mode when using Burst Write
// disable ->
// disable ->
        if( MSCBurstWrID )
            i = 2;
        else
            i = 0;
// disable ->
        for(; i<byteno; i++)
            MxLrxwr(0x59, it6802->txscrpad[i]);
// disable ->
    }
    else
        MxLrxwr(0x55, it6802->txscrpad[2]);
}


void v3d_unsupport1st(struct it6802_dev_data *it6802)
{
//FIX_ID_013    xxxxx    //For MSC 3D request issue
     it6802->txscrpad[0]  = 0x00;
     it6802->txscrpad[1]  = 0x10;
     it6802->txscrpad[2]  = 0xEF;
     it6802->txscrpad[3]  = 0x00;
     it6802->txscrpad[4]  = 0x01;
     it6802->txscrpad[5]  = 0x00;
//FIX_ID_013    xxxxx

     MxLrxwr(0x5E, it6802->txscrpad[0]);
     MxLrxwr(0x5F, it6802->txscrpad[1]);
     wrburstoff = 0;
     wrburstnum = 6;
     write_burst(it6802,wrburstoff, wrburstnum);

    // disable ->      #ifdef DEBUG_MODE
    // disable ->      dbmsg_ftrace(DBM_DPATH,"Set DSCR_CHG to 1 (TxSeqNum=%d) ...\n", TxWrBstSeq++);
    // disable ->      #endif
     set_MxLint(MxLInt00B, DSCR_CHG);
     V3D_EntryCnt++;
     set_MxLint(MxLInt00B, REQ_WRT);
}

void v3d_unsupport2nd(struct it6802_dev_data *it6802)
{
//FIX_ID_013    xxxxx    //For MSC 3D request issue
     it6802->txscrpad[0]  = 0x00;
     it6802->txscrpad[1]  = 0x11;
     it6802->txscrpad[2]  = 0xEE;
     it6802->txscrpad[3]  = 0x00;
     it6802->txscrpad[4]  = 0x01;
     it6802->txscrpad[5]  = 0x00;
//FIX_ID_013    xxxxx

     MxLrxwr(0x5E, it6802->txscrpad[0]);
     MxLrxwr(0x5F, it6802->txscrpad[1]);
     wrburstoff = 0;
     wrburstnum = 6;
     write_burst(it6802,wrburstoff, wrburstnum);
    // disable ->      #ifdef DEBUG_MODE
    // disable ->      dbmsg_ftrace(DBM_DPATH,"Set DSCR_CHG to 1 (TxSeqNum=%d) ...\n", TxWrBstSeq++);
    // disable ->      #endif
     set_MxLint(MxLInt00B, DSCR_CHG);
     V3D_EntryCnt++;
}

#endif //FIX_ID_013    xxxxx


#if 1
//FIX_ID_021 xxxxx        //To use CP_100ms for CsigBus_100ms and CEC_100ms
////////////////////////////////////////////////////////////////////
//static unsigned char OSCvalueCompare(unsigned long *calibrationValue)
//
//
//
////////////////////////////////////////////////////////////////////
unsigned char OSCvalueCompare(unsigned long *calibrationValue)
{
    unsigned long diff_cp;
    unsigned long cp_cal_mean = CP_MEAN_VALUE;

    if(*calibrationValue != 0)
    {
    *calibrationValue /= 100;
    }

    if( *calibrationValue >= cp_cal_mean )
    {
    diff_cp = (*calibrationValue-cp_cal_mean);
    CsigBus_CAL_PRINTF(("diff_cp=%ld\n", diff_cp));
    }
    else
    {
    diff_cp = (cp_cal_mean-*calibrationValue);
    CsigBus_CAL_PRINTF(("diff_cp=%ld\n", diff_cp));
    }

    if( diff_cp>CP_MAX_DEVIATION )
    {
    CsigBus_CAL_PRINTF(("The Calibration Value Error\n"));
    return FALSE;
    }

    CsigBus_CAL_PRINTF(("calibrationValue = %ld MHz\n", *calibrationValue));

    return TRUE;
}


////////////////////////////////////////////////////////////////////
//static unsigned long CP_OCLK( void )
//
//
//
////////////////////////////////////////////////////////////////////
unsigned long CP_OCLK( void )
{
    unsigned char  SIP_00B;
    unsigned char SIP_01B;
    unsigned char SIP_02B;
    unsigned char CP_100ms_00B;
    unsigned char CP_100ms_01B;
    unsigned char CP_100ms_02B;
    unsigned long rddata;

    hdmirxwr(0x0F, 0x00);
    hdmirxwr(0x73, 0x05);
    hdmirxwr(0x0F, 0x01);
    hdmirxwr(REG_RX_157, 0x12);
    hdmirxwr(REG_RX_157, 0x02);
    hdmirxwr(REG_RX_156, 0x01);

    // set Read Address and enable
    hdmirxwr(REG_RX_150, 0x00);
    hdmirxwr(REG_RX_151, 0x00);
    hdmirxwr(REG_RX_153, 0x04); // reg153
    SIP_00B=hdmirxrd(REG_RX_161);
    SIP_01B=hdmirxrd(REG_RX_161);
    SIP_02B=hdmirxrd(REG_RX_161);

    CsigBus_CAL_PRINTF(("SIP_00B=%X\n",(int)SIP_00B));
    CsigBus_CAL_PRINTF(("SIP_01B=%X\n",(int)SIP_01B));
    CsigBus_CAL_PRINTF(("SIP_02B=%X\n",(int)SIP_02B));

    if( SIP_00B==0x02 && SIP_01B==0x02 && SIP_02B==0x02 )
    {
        hdmirxwr(REG_RX_150, 0x01);
        hdmirxwr(REG_RX_151, 0x60);
        hdmirxwr(REG_RX_153, 0x04);
        CP_100ms_00B=hdmirxrd(REG_RX_161);
        CP_100ms_01B=hdmirxrd(REG_RX_161);
        CP_100ms_02B=hdmirxrd(REG_RX_161);

        // disable -> if(CP_100ms_02B > 0x60 || CP_100ms_02B < 0x40)
        // disable -> {
        // disable -> CsigBus_CAL_PRINTF(("SIPROM CP 100MS Error\n"));
        // disable -> rddata = 0;
        // disable -> }
        // disable -> else
        {
            rddata =(unsigned long)(CP_100ms_02B);
            rddata <<= 8 ;
            rddata |=(unsigned long)(CP_100ms_01B) ;
            rddata <<= 8 ;
            rddata |=(unsigned long) (CP_100ms_00B);
            CsigBus_CAL_PRINTF(("SIPROM CP 100MS %x\n",rddata));
        }
    }
    else if( SIP_00B==0xFF && SIP_01B==0x00 && SIP_02B==0xFF )
    {
        hdmirxwr(REG_RX_150, 0x03);
        hdmirxwr(REG_RX_151, 0x60);
        hdmirxwr(REG_RX_153, 0x04);
        CP_100ms_00B=hdmirxrd(REG_RX_161);
        CP_100ms_01B=hdmirxrd(REG_RX_161);
        CP_100ms_02B=hdmirxrd(REG_RX_161);
        // disable -> if(CP_100ms_02B > 0x60 || CP_100ms_02B < 0x40)
        // disable -> {
        // disable -> CsigBus_CAL_PRINTF(("SIPROM CP 100MS Error\n"));
        // disable -> rddata = 0;
        // disable -> }
        // disable -> else
        {
            rddata =(unsigned long)(CP_100ms_02B);
            rddata <<= 8 ;
            rddata |=(unsigned long)(CP_100ms_01B) ;
            rddata <<= 8 ;
            rddata |=(unsigned long) (CP_100ms_00B);
            CsigBus_CAL_PRINTF(("SIPROM CP 100MS %x\n",rddata));
        }
    }
    else
    {
        CsigBus_CAL_PRINTF(("SIPROM CP 100MS Error\n"));
        rddata = 0;
    }

    hdmirxwr(0x0F, 0x00);
    hdmirxwr(0x73, 0x00);

    CsigBus_CAL_PRINTF(("CP_100ms_00B=%X\n",(int)CP_100ms_00B));
    CsigBus_CAL_PRINTF(("CP_100ms_01B=%X\n",(int)CP_100ms_01B));
    CsigBus_CAL_PRINTF(("CP_100ms_02B=%X\n",(int)CP_100ms_02B));

    return rddata;
}

//FIX_ID_004 xxxxx //Add 100ms calibration for CsigBus
//#ifndef _SelectExtCrystalForCsigBus_
////////////////////////////////////////////////////////////////////
//void cal_oclk( void )
//
//
//
////////////////////////////////////////////////////////////////////
void Cal_oclk( void )
{

    #if 0 // disable CsigBus Calibration of MCU
        unsigned char i;
        unsigned long rddata;
    #endif
    unsigned char oscdiv;
    unsigned long OSCCLK;
    unsigned long sum=0;

    //float m _ROSCCLK;
    unsigned int t10usint, t10usflt;

    #if 0 // disable CsigBus Calibration of MCU
        // 100ms OSC calibartion by MCU
        for(i=0; i<1; i++)
        {
    //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
            MxLrxset(MxL_RX_01,0x01, 0x01);
            delay1ms(99);
            MxLrxset(MxL_RX_01,0x01, 0x00);
    //FIX_ID_037 xxxxx

            rddata = (unsigned long)MxLrxrd(MxL_RX_12);
            rddata += (unsigned long)MxLrxrd(MxL_RX_13)<<8;
            rddata += (unsigned long)MxLrxrd(MxL_RX_14)<<16;

            sum += rddata;
        }
        CsigBus_CAL_PRINTF(("loop=%d, cnt=%ld  sum =%ld\n",(int) i, rddata,sum));

        sum /= i;


        OSCCLK = (unsigned long) sum;

        CsigBus_CAL_PRINTF(("MCU calculate 100ms = %ld MHz\n", OSCCLK));
        if(OSCvalueCompare(&OSCCLK)==FALSE)
    #endif

    {
        OSCCLK = CP_OCLK();
        CsigBus_CAL_PRINTF(("CP_OSCCLK = %ld MHz\n", OSCCLK));
        if(OSCvalueCompare(&OSCCLK)==FALSE)
        {
            OSCCLK = CP_MEAN_VALUE;
            //OSCCLK = (unsigned long) sum;
            //OSCCLK /= 100;
            CsigBus_CAL_PRINTF(("Use CP_MEAN_VALUE = %ld MHz\n", OSCCLK));
        }
        else
        {
            CsigBus_CAL_PRINTF(("Use CP Calibration Value\n"));
        }
    }
    #if 0 // disable CsigBus Calibration of MCU
        else
        {
            CsigBus_CAL_PRINTF(("Use MCU calculate OSC Value = %ld MHz\n", OSCCLK));
        }
    #endif


    CsigBus_CAL_PRINTF(("OSCCLK=%ld MHz\n", OSCCLK/1000));
    oscdiv = (unsigned char) (OSCCLK/1000/10);

    if( ((OSCCLK/1000/oscdiv)-10)>(10-(OSCCLK/1000/(oscdiv+1))) )
    {
        oscdiv++;
    }
//GreenText;
    CsigBus_CAL_PRINTF(("!!! oscdiv = %d !!!\n", (int) oscdiv));
//WhiteText;
    MxLrxset(MxL_RX_01, 0x70, oscdiv<<4);


    CsigBus_CAL_PRINTF(("OCLK=%ld MHz\n", OSCCLK/1000/oscdiv));

    // FIX_ID_051 calibration CEC timeout
    #ifdef Enable_IT6802_CEC
        timeunit = (u8_t)(OSCCLK/640);
        CsigBus_CAL_PRINTF(("CEC timeunit (T100us) = %X\n",(int)timeunit));
        // u32_CEC = OSCCLK*100;
        // u32_CEC = u32_CEC>>8;
        // u32_CEC /= 0xFB;
        // CsigBus_CAL_PRINTF(("u32_CEC = %X\n",(int) u32_CEC));
    #endif
    //~FIX_ID_051



    MxLrxset(MxL_RX_29,0x83,0x83);    //[7] 1: use 27M crystall ( _SelectExtCrystalForCsigBus_ )

    if( RCLKFreqSel==0 )
    m_ROSCCLK = OSCCLK/2;   // 20 MHz
    else if( RCLKFreqSel==1 )
    m_ROSCCLK = OSCCLK/4;   // 10 MHz
    else if( RCLKFreqSel==2 )
    m_ROSCCLK = OSCCLK/8;
    else
    m_ROSCCLK = OSCCLK/16;

    //RCLK*=1.1;    // add for CTS (RCLK*=1.1)

    t10usint = m_ROSCCLK/100;

    // don't use floating calculation on firmware.
    // t10usflt = (m_ROSCCLK/100 - t10usint)*128;
    t10usflt = m_ROSCCLK%100;
    t10usflt *= 128;
    t10usflt /= 100;

    CsigBus_CAL_PRINTF(("RCLK=%ld MHz\n", m_ROSCCLK/1000));
    CsigBus_CAL_PRINTF(("T10usInt=0x%X, T10usFlt=0x%X\n",(int) t10usint,(int) t10usflt));
    MxLrxwr(0x02, t10usint&0xFF);
    MxLrxwr(0x03, ((t10usint&0x100)>>1)+t10usflt);
    CsigBus_CAL_PRINTF(("MxL reg02=%X, reg03=%X\n",(int)MxLrxrd(0x02),(int) MxLrxrd(0x03)));
    //     RCLK/=1.1;    // add for CTS (RCLK*=1.1)
    CsigBus_CAL_PRINTF(("\n"));

}
//FIX_ID_021 xxxxx
//#endif
#endif
#else//20240919
unsigned char OSCvalueCompare(unsigned long *calibrationValue)
{
    unsigned long diff_cp;
    unsigned long cp_cal_mean = CP_MEAN_VALUE;

    if(*calibrationValue != 0)
    {
    *calibrationValue /= 100;
    }

    if( *calibrationValue >= cp_cal_mean )
    {
    diff_cp = (*calibrationValue-cp_cal_mean);
    CsigBus_CAL_PRINTF(("diff_cp=%ld\n", diff_cp));
    }
    else
    {
    diff_cp = (cp_cal_mean-*calibrationValue);
    CsigBus_CAL_PRINTF(("diff_cp=%ld\n", diff_cp));
    }

    if( diff_cp>CP_MAX_DEVIATION )
    {
    CsigBus_CAL_PRINTF(("The Calibration Value Error\n"));
    return FALSE;
    }

    CsigBus_CAL_PRINTF(("calibrationValue = %ld MHz\n", *calibrationValue));

    return TRUE;
}


////////////////////////////////////////////////////////////////////
//static unsigned long CP_OCLK( void )
//
//
//
////////////////////////////////////////////////////////////////////
unsigned long CP_OCLK( void )
{
    unsigned char  SIP_00B;
    unsigned char SIP_01B;
    unsigned char SIP_02B;
    unsigned char CP_100ms_00B;
    unsigned char CP_100ms_01B;
    unsigned char CP_100ms_02B;
    unsigned long rddata;

    hdmirxwr(0x0F, 0x00);
    hdmirxwr(0x73, 0x05);
    hdmirxwr(0x0F, 0x01);
    hdmirxwr(REG_RX_157, 0x12);
    hdmirxwr(REG_RX_157, 0x02);
    hdmirxwr(REG_RX_156, 0x01);

    // set Read Address and enable
    hdmirxwr(REG_RX_150, 0x00);
    hdmirxwr(REG_RX_151, 0x00);
    hdmirxwr(REG_RX_153, 0x04); // reg153
    SIP_00B=hdmirxrd(REG_RX_161);
    SIP_01B=hdmirxrd(REG_RX_161);
    SIP_02B=hdmirxrd(REG_RX_161);

    CsigBus_CAL_PRINTF(("SIP_00B=%X\n",(int)SIP_00B));
    CsigBus_CAL_PRINTF(("SIP_01B=%X\n",(int)SIP_01B));
    CsigBus_CAL_PRINTF(("SIP_02B=%X\n",(int)SIP_02B));

    if( SIP_00B==0x02 && SIP_01B==0x02 && SIP_02B==0x02 )
    {
        hdmirxwr(REG_RX_150, 0x01);
        hdmirxwr(REG_RX_151, 0x60);
        hdmirxwr(REG_RX_153, 0x04);
        CP_100ms_00B=hdmirxrd(REG_RX_161);
        CP_100ms_01B=hdmirxrd(REG_RX_161);
        CP_100ms_02B=hdmirxrd(REG_RX_161);

        // disable -> if(CP_100ms_02B > 0x60 || CP_100ms_02B < 0x40)
        // disable -> {
        // disable -> CsigBus_CAL_PRINTF(("SIPROM CP 100MS Error\n"));
        // disable -> rddata = 0;
        // disable -> }
        // disable -> else
        {
            rddata =(unsigned long)(CP_100ms_02B);
            rddata <<= 8 ;
            rddata |=(unsigned long)(CP_100ms_01B) ;
            rddata <<= 8 ;
            rddata |=(unsigned long) (CP_100ms_00B);
            CsigBus_CAL_PRINTF(("SIPROM CP 100MS %x\n",rddata));
        }
    }
    else if( SIP_00B==0xFF && SIP_01B==0x00 && SIP_02B==0xFF )
    {
        hdmirxwr(REG_RX_150, 0x03);
        hdmirxwr(REG_RX_151, 0x60);
        hdmirxwr(REG_RX_153, 0x04);
        CP_100ms_00B=hdmirxrd(REG_RX_161);
        CP_100ms_01B=hdmirxrd(REG_RX_161);
        CP_100ms_02B=hdmirxrd(REG_RX_161);
        // disable -> if(CP_100ms_02B > 0x60 || CP_100ms_02B < 0x40)
        // disable -> {
        // disable -> CsigBus_CAL_PRINTF(("SIPROM CP 100MS Error\n"));
        // disable -> rddata = 0;
        // disable -> }
        // disable -> else
        {
            rddata =(unsigned long)(CP_100ms_02B);
            rddata <<= 8 ;
            rddata |=(unsigned long)(CP_100ms_01B) ;
            rddata <<= 8 ;
            rddata |=(unsigned long) (CP_100ms_00B);
            CsigBus_CAL_PRINTF(("SIPROM CP 100MS %x\n",rddata));
        }
    }
    else
    {
        CsigBus_CAL_PRINTF(("SIPROM CP 100MS Error\n"));
        rddata = 0;
    }

    hdmirxwr(0x0F, 0x00);
    hdmirxwr(0x73, 0x00);

    CsigBus_CAL_PRINTF(("CP_100ms_00B=%X\n",(int)CP_100ms_00B));
    CsigBus_CAL_PRINTF(("CP_100ms_01B=%X\n",(int)CP_100ms_01B));
    CsigBus_CAL_PRINTF(("CP_100ms_02B=%X\n",(int)CP_100ms_02B));

    return rddata;
}
void Cal_oclk( void )
{
    unsigned char oscdiv;
    unsigned long OSCCLK;

    {
        OSCCLK = CP_OCLK();
        IT6802_DEBUG_INT_PRINTF(("CP_OSCCLK = %ld MHz\n", OSCCLK));
        if(OSCvalueCompare(&OSCCLK)==FALSE)
        {
            OSCCLK = CP_MEAN_VALUE;
            //OSCCLK = (unsigned long) sum;
            //OSCCLK /= 100;
            IT6802_DEBUG_INT_PRINTF(("Use CP_MEAN_VALUE = %ld MHz\n", OSCCLK));
        }
        else
        {
            IT6802_DEBUG_INT_PRINTF(("Use CP Calibration Value\n"));
        }
    }

    IT6802_DEBUG_INT_PRINTF(("OSCCLK=%ld MHz\n", OSCCLK/1000));
    oscdiv = (unsigned char) (OSCCLK/1000/10);

    if( ((OSCCLK/1000/oscdiv)-10)>(10-(OSCCLK/1000/(oscdiv+1))) )
    {
        oscdiv++;
    }

    IT6802_DEBUG_INT_PRINTF(("OCLK=%ld MHz\n", OSCCLK/1000/oscdiv));

    // FIX_ID_051 calibration CEC timeout
    #ifdef Enable_IT6602_CEC
        timeunit = (u8_t)(OSCCLK/640);
        IT6802_DEBUG_INT_PRINTF(("CEC timeunit (T100us) = %X\n",(int)timeunit));
    #endif


    if( RCLKFreqSel==0 )
    m_ROSCCLK = OSCCLK/2;   // 20 MHz
    else if( RCLKFreqSel==1 )
    m_ROSCCLK = OSCCLK/4;   // 10 MHz
    else if( RCLKFreqSel==2 )
    m_ROSCCLK = OSCCLK/8;
    else
    m_ROSCCLK = OSCCLK/16;
    IT6802_DEBUG_INT_PRINTF(("RCLK=%ld MHz\n", m_ROSCCLK/1000));
}

#endif


#ifdef _SUPPORT_EDID_RAM_
/*****************************************************************************/
/* EDID RAM  functions    *******************************************************/
/*****************************************************************************/

//static unsigned char UpdateEDIDRAM(_CODE unsigned char *pEDID,unsigned char BlockNUM)
unsigned char UpdateEDIDRAM(unsigned char *pEDID,unsigned char BlockNUM)
{
    unsigned char  i,offset,sum =0;

    if ( BlockNUM == 0x02 )
        offset = 0x00+128*0x01;
    else
        offset = 0x00+128*BlockNUM;

    EDID_DEBUG_PRINTF(("block No =%02X offset = %02X\n",(int) BlockNUM,(int) offset));


    for(i=0;i<0x7F;i++)
    {
        EDID_RAM_Write(offset,1 ,(pEDID+offset));

        EDID_DEBUG_PRINTF(("%02X ",(int) *(pEDID+offset)));
        sum += *(pEDID+offset);
        offset ++;
//        pEDID++;

            if((i % 16) == 15)
            {
            EDID_DEBUG_PRINTF(("\n"));
            }


    }
    sum = 0x00- sum;
//    EDID_RAM_Write(offset,1,&sum);
    return    sum;
}

void EnableEDIDupdata(void)
{
    EDID_DEBUG_PRINTF(("EnableEDIDupdata()\n"));

//    HotPlug(0);    //clear port 1 HPD=0 for Enable EDID update

//    chgbank(1);
//    hdmirxset(REG_RX_1B0, 0x03, 0x01); //clear port 0 HPD=1 for EDID update
//    chgbank(0);

    it6802HPDCtrl(0,0);    // HDMI/MxL port 0, set HPD = 0
    it6802HPDCtrl(1,0);    // HDMI port 1, set HPD = 0
}

void DisableEDIDupdata(void)
{
    EDID_DEBUG_PRINTF(("DisableEDIDupdata()\n"));

//xxxxx 2013-0801 disable HPD Control
#if 0
    HotPlug(1);    //set port 1 HPD=1

    chgbank(1);
    hdmirxset(REG_RX_1B0, 0x03, 0x03); //set port 0 HPD=1
    chgbank(0);
#endif
//xxxxx
}


//static void EDIDRAMInitial(_CODE unsigned char *pIT6802EDID)
void EDIDRAMInitial(unsigned char *pIT6802EDID)
{

    unsigned char Block0_CheckSum;
    unsigned char Block1_CheckSum;
    unsigned char u8_VSDB_Addr;
    unsigned char BlockNo;

    u8_VSDB_Addr=0;

    EnableEDIDupdata();

    for(BlockNo=0;BlockNo<2;BlockNo++){

    EDID_DEBUG_PRINTF(("IT6802 EDIDRAMInitial = %02X\n", (int) BlockNo));

        if(BlockNo==0)
        {
            Block0_CheckSum =  UpdateEDIDRAM(pIT6802EDID,0);
            hdmirxwr(REG_RX_0C4,Block0_CheckSum);        //Port 0 Bank 0 CheckSum
            hdmirxwr(REG_RX_0C8,Block0_CheckSum);        //Port 1 Bank 0 CheckSum

            EDID_DEBUG_PRINTF((" Block0_CheckSum = %02X\n", (int) Block0_CheckSum));
        }
        else
        {
            Block1_CheckSum =  UpdateEDIDRAM(pIT6802EDID,1);
            EDID_DEBUG_PRINTF((" Block1_CheckSum = %02X\n", (int) Block1_CheckSum));
            u8_VSDB_Addr=Find_Phyaddress_Location(pIT6802EDID,1);

            EDID_DEBUG_PRINTF(("u8_VSDB_Addr = %02X\n", (int) u8_VSDB_Addr));
            PhyAdrSet();

            if(u8_VSDB_Addr!=0)
            {

                UpdateEDIDReg(u8_VSDB_Addr, pIT6802EDID[u8_VSDB_Addr],pIT6802EDID[u8_VSDB_Addr+1], Block1_CheckSum);
                EDID_DEBUG_PRINTF(("EDID Parsing OK\n"));
            }
        }
    }

    DisableEDIDupdata();
}


//static unsigned char Find_Phyaddress_Location(_CODE unsigned char *pEDID,unsigned char Block_Number)
unsigned char Find_Phyaddress_Location(unsigned char *pEDID,unsigned char Block_Number)
{
    unsigned char AddStart;
    unsigned char tag, count;
    unsigned char offset,End;
    unsigned char u8_VSDB_Addr;

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
    unsigned char u8_3DPresent_Addr;
    unsigned char ucTemp;
    struct PARSE3D_STR *pstParse3D = get_EDID_VSDB_3Ddata();
//FIX_ID_013    xxxxx
#endif //FIX_ID_013

    if ( Block_Number == 0x02 )
        AddStart = 0x00+128*0x01;
    else
        AddStart = 0x00+128*Block_Number;

    if((*(pEDID+AddStart))!=0x2 || (*(pEDID+AddStart+1))!=0x3)
        return 0;
    End = (*(pEDID+AddStart+2));
    u8_VSDB_Addr=0;

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
    // initial value then check with SVD and VSDB block to find the SVD of 3D support timing
    pstParse3D->bVSDBspport3D = 0x00;
    pstParse3D->ucVicCnt=0;
//FIX_ID_013    xxxxx
#endif //FIX_ID_013

    for(offset=(AddStart+0x04);offset<(AddStart+End); )
    {


        tag=(*(pEDID+offset))>>5;
        count=(*(pEDID+offset)) & 0x1f;

        //#ifdef printf_EDID
        EDID_DEBUG_PRINTF(("offset = %X , Tag = %X , count =%X\n", (int) offset,(int)  tag, (int) count));
        //#endif

        offset++;
    if(tag==0x03)    // HDMI VSDB Block of EDID
        {
            //#ifdef printf_EDID
                EDID_DEBUG_PRINTF(("HDMI VSDB Block address = %X\n",(int)  offset));
            //#endif

            if(    (*(pEDID+offset  ))==0x03 &&
                (*(pEDID+offset+1))==0x0C &&
                (*(pEDID+offset+2))==0x0    )
            {
                u8_VSDB_Addr=offset+3;
                txphyadr[0]=(*(pEDID+offset+3));
                txphyadr[1]=(*(pEDID+offset+4));
                //#ifdef printf_EDID
                EDID_DEBUG_PRINTF(("txphyadr[0] = %X\n",(int)  txphyadr[0]));
                EDID_DEBUG_PRINTF(("txphyadr[1] = %X\n",(int)  txphyadr[1]));
                //#endif

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue

                if(count < 7)        // no 3D support !!!
                    return u8_VSDB_Addr;

                u8_3DPresent_Addr = offset+7;

                ucTemp = *(pEDID+offset+7);

                if(ucTemp & 0x80)                // Video and Audio Latency present
                    u8_3DPresent_Addr += 2;

                if(ucTemp & 0x40)            // Interlaced Video and Audio Latency present
                    u8_3DPresent_Addr += 2;

                if(ucTemp & 0x20)                // HDMI additional video format present
                {
                     u8_3DPresent_Addr++;
                }
                pstParse3D->uc3DEdidStart = u8_3DPresent_Addr;

                pstParse3D->uc3DBlock = Block_Number;
                // FIX_ID_047, only 3D support VSDB enable bVSDBsupport3D
                if( (*(pEDID+u8_3DPresent_Addr))&0x80)
                {
                    pstParse3D->bVSDBspport3D = 0x01;        // for identify the HDMI VSDB 3D support
                }
                // FIX_ID_047, only 3D support VSDB enable bVSDBsupport3D
//FIX_ID_013    xxxxx
#endif //FIX_ID_013
                return u8_VSDB_Addr;
            }
        }

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue

            if(tag==0x02)    // Short Video Descriptor of EDID
            {
                //#ifdef printf_EDID
                EDID_DEBUG_PRINTF(("Short Video Descriptor Address = %X, VIC count = %X\n",(int)  offset,(int) count));
                //#endif

            // get the SVD size
            pstParse3D->ucVicCnt = count;

            for(ucTemp=0;ucTemp<count;ucTemp++)
            {
                u8_3DPresent_Addr = (*(pEDID+offset+ucTemp) ) & 0x7F;
                SVD_LIST[ucTemp] = u8_3DPresent_Addr;
                MxLRX_DEBUG_PRINTF(("SVD[%X] = %X\n", ucTemp,u8_3DPresent_Addr));
            }
         }
//FIX_ID_013    xxxxx
#endif //FIX_ID_013

        offset=offset+count;
    }
    return 0;
}



void UpdateEDIDReg(unsigned char u8_VSDB_Addr, unsigned char CEC_AB, unsigned char CEC_CD, unsigned char Block1_CheckSum)
{

    unsigned char  A_Addr_AB, A_Addr_CD, A_Block1_CheckSum;
    unsigned char  B_Addr_AB, B_Addr_CD, B_Block1_CheckSum;

    A_Addr_AB=rxphyadr[0][0];
    A_Addr_CD=rxphyadr[0][1];

    B_Addr_AB=rxphyadr[1][0];
    B_Addr_CD=rxphyadr[1][1];


    A_Block1_CheckSum=(Block1_CheckSum+CEC_AB+CEC_CD -A_Addr_AB-A_Addr_CD)%0x100;
    B_Block1_CheckSum=(Block1_CheckSum+CEC_AB+CEC_CD -B_Addr_AB-B_Addr_CD)%0x100;


    hdmirxwr(REG_RX_0C1,u8_VSDB_Addr);            //VSDB Start Address
    hdmirxwr(REG_RX_0C2,A_Addr_AB);                    //Port 0 AB
    hdmirxwr(REG_RX_0C3,A_Addr_CD);                //Port 0 CD
    hdmirxwr(REG_RX_0C5,A_Block1_CheckSum);        //Port 0 Bank 1 CheckSum

    hdmirxwr(REG_RX_0C6,B_Addr_AB);                    //Port 1 AB
    hdmirxwr(REG_RX_0C7,B_Addr_CD);                //Port 1 CD
    hdmirxwr(REG_RX_0C9,B_Block1_CheckSum);        //Port 1 Bank 1 CheckSum



}
void PhyAdrSet(void)
{

#if 0
//only for HDMI switch
    txphyA = (txphyadr[0]&0xF0)>>4;
    txphyB = (txphyadr[0]&0x0F);
    txphyC = (txphyadr[1]&0xF0)>>4;
    txphyD = (txphyadr[1]&0x0F);

    if( txphyA==0 && txphyB==0 && txphyC==0 && txphyD==0 )
        txphylevel = 0;
    else if( txphyB==0 && txphyC==0 && txphyD==0 )
        txphylevel = 1;
    else if( txphyC==0 && txphyD==0 )
        txphylevel = 2;
    else if( txphyD==0 )
        txphylevel = 3;
    else
        txphylevel = 4;

    rxcurport = 0;
    switch( txphylevel )
    {
        case 0:
            rxphyadr[0][0] = 0x10;
            rxphyadr[0][1] = 0x00;
            rxphyadr[1][0] = 0x20;
            rxphyadr[1][1] = 0x00;
            break;
        case 1:
            rxphyadr[0][0] = (txphyA<<4)+0x01;
            rxphyadr[0][1] = 0x00;
            rxphyadr[1][0] = (txphyA<<4)+0x02;
            rxphyadr[1][1] = 0x00;
            break;
        case 2:
            rxphyadr[0][0] = txphyadr[0];
            rxphyadr[0][1] = 0x10;
            rxphyadr[1][0] = txphyadr[0];
            rxphyadr[1][1] = 0x20;
            break;
        case 3:
            rxphyadr[0][0] = txphyadr[0];
            rxphyadr[0][1] = (txphyC<<4)+0x01;
            rxphyadr[1][0] = txphyadr[0];
            rxphyadr[1][1] = (txphyC<<4)+0x02;
            break;
        default:
            rxphyadr[0][0] = 0xFF;
            rxphyadr[0][1] = 0xFF;
            rxphyadr[1][0] = 0xFF;
            rxphyadr[1][1] = 0xFF;
            break;
    }
    EDID_DEBUG_PRINTF(("\nDnStrm PhyAdr = %X, %X, %X, %X\n", (int) txphyA,(int)  txphyB, (int) txphyC, (int) txphyD));

    rxphyA = (rxphyadr[0][0]&0xF0)>>4;
    rxphyB = (rxphyadr[0][0]&0x0F);
    rxphyC = (rxphyadr[0][1]&0xF0)>>4;
    rxphyD = (rxphyadr[0][1]&0x0F);
    EDID_DEBUG_PRINTF((" PortA PhyAdr = %X, %X, %X, %X\n",(int)  rxphyA, (int) rxphyB, (int) rxphyC,(int)  rxphyD));

    rxphyA = (rxphyadr[1][0]&0xF0)>>4;
    rxphyB = (rxphyadr[1][0]&0x0F);
    rxphyC = (rxphyadr[1][1]&0xF0)>>4;
    rxphyD = (rxphyadr[1][1]&0x0F);
    EDID_DEBUG_PRINTF((" PortB PhyAdr = %X, %X, %X, %X\n", (int) rxphyA,(int)  rxphyB,(int)  rxphyC,(int)  rxphyD));
#endif

//#ifdef Enable_IT6802_CEC
//    if(Myself_LogicAdr==DEVICE_ID_TV)
//#endif
    {
        rxphyadr[0][0] = 0x10;
        rxphyadr[0][1] = 0x00;
        rxphyadr[1][0] = 0x20;
        rxphyadr[1][1] = 0x00;

    }



}

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
#ifdef  _SUPPORT_EDID_RAM_
struct PARSE3D_STR* get_EDID_VSDB_3Ddata(void)
{
    return &st3DParse;
}

void EDID_ParseVSDB_3Dblock(struct PARSE3D_STR *pstParse3D)
{

    unsigned char    ucTemp;
    unsigned char    uc3DMulti;
    unsigned char    uc3DEdidEnd = 0xFF;
    unsigned char    ucRdPtr = pstParse3D->uc3DEdidStart;

    PARSE3D_STA    e3DEdidState = PARSE3D_START;


    //check with HDMI VSDB block of EDID
    if(pstParse3D->bVSDBspport3D == 0x00)
    {
        pstParse3D->ucVicCnt=0;
        return;
    }


    // Re-initial bVSDBspport3D =0 then check with 3D_Structure and 3D_MASK at HDMI VSDB block of EDID
    pstParse3D->bVSDBspport3D = 0x00;


    MxL3D_DEBUG_PRINTF(("***   EDID_ParseVSDB_3Dblock   ***\n"));
    MxL3D_DEBUG_PRINTF(("MxL 3D [2]LR [1]TB [0]FS\n"));

    if(ucRdPtr ==0)
        return;

        for(;ucRdPtr <= uc3DEdidEnd;){

            // disable ->    MxLRX_DEBUG_PRINTF(("Default_Edid_Block[%x]=0x%x\n",(int) ucRdPtr,(int) Default_Edid_Block[ucRdPtr]));

            // disable ->    for(ucTemp=0; ucTemp<16; ucTemp++)
            // disable ->    {
            // disable ->        MxLRX_DEBUG_PRINTF(("%x ", (int)pstParse3D->uc3DInfor[ucTemp]));
            // disable ->    }

            switch(e3DEdidState){
                    case    PARSE3D_START:
                            uc3DMulti = Default_Edid_Block[ucRdPtr++];

                            if(uc3DMulti & 0x80){
                                uc3DMulti &= 0x60;
                                e3DEdidState = PARSE3D_LEN;
                            }else{
                                return;
                            }
                        break;
                    case    PARSE3D_LEN:
                            uc3DEdidEnd = (Default_Edid_Block[ucRdPtr] >> 5) + (Default_Edid_Block[ucRdPtr] & 0x1F) +ucRdPtr;
                            ucRdPtr += (Default_Edid_Block[ucRdPtr] >> 5) + 1;
                            e3DEdidState = PARSE3D_STRUCT_H;
                        break;
                    case    PARSE3D_STRUCT_H:
                            switch(uc3DMulti){
                                case    0x20:
                                case    0x40:
                                        if(Default_Edid_Block[ucRdPtr++] & 0x01){
                                            uc3DMulti |= 0x04;
                                        }
                                        e3DEdidState = PARSE3D_STRUCT_L;
                                    break;
                                default:
                                        e3DEdidState = PARSE3D_VIC;
                                    break;
                            }
                        break;
                    case    PARSE3D_STRUCT_L:
                            ucTemp = Default_Edid_Block[ucRdPtr++];
                            if(ucTemp & 0x40)
                                uc3DMulti |= 0x02;
                            if(ucTemp & 0x01)
                                uc3DMulti |= 0x01;


                            if((uc3DMulti & 0x60) == 0x20){
                                e3DEdidState = PARSE3D_VIC;
                                uc3DMulti &= 7;

                                for(ucTemp=0; ucTemp<16; ucTemp++){
                                    pstParse3D->uc3DInfor[ucTemp] = uc3DMulti;
                                    MxL3D_DEBUG_PRINTF(("VSD[%d]=0x%x\n", (int)ucTemp,(int) uc3DMulti));
                                }

                            }else{
                                e3DEdidState = PARSE3D_MASK_H;
                                uc3DMulti &= 7;
                            }
                        break;
                    case    PARSE3D_MASK_H:

                            if(Default_Edid_Block[ucRdPtr])
                                pstParse3D->bVSDBspport3D = 0x01;    //for identify 3D_MASK have Short Video Descriptor (SVD) support 3D format

                            for(ucTemp=0; ucTemp<8; ucTemp++){
                                if(Default_Edid_Block[ucRdPtr] & (1<<ucTemp)){
                                    pstParse3D->uc3DInfor[ucTemp+8] = uc3DMulti;
                                    MxL3D_DEBUG_PRINTF(("VSD[%d]=0x%x\n",(int) ucTemp+8,(int) uc3DMulti));
                                }else{
                                    pstParse3D->uc3DInfor[ucTemp+8] = 0;
                                }
                            }
                            ucRdPtr++;
                            e3DEdidState = PARSE3D_MASK_L;
                        break;
                    case    PARSE3D_MASK_L:

                            if(Default_Edid_Block[ucRdPtr])
                                pstParse3D->bVSDBspport3D = 0x01;    //for identify 3D_MASK have SVD support 3D format

                            for(ucTemp=0; ucTemp<8; ucTemp++){
                                if(Default_Edid_Block[ucRdPtr] & (1<<ucTemp)){
                                    pstParse3D->uc3DInfor[ucTemp] = uc3DMulti;
                                    MxL3D_DEBUG_PRINTF(("VSD[%d]=0x%x\n", (int)ucTemp, (int)uc3DMulti));
                                }else{
                                    pstParse3D->uc3DInfor[ucTemp] = 0;
                                }
                            }
                            ucRdPtr++;
                            e3DEdidState = PARSE3D_VIC;
                        break;
                    case    PARSE3D_VIC:
// disable ->                            ucTemp = Default_Edid_Block[ucRdPtr]>>4;
// disable ->                            if(pstParse3D->ucVicCnt > ucTemp){
// disable ->                                pstParse3D->uc3DInfor[ucTemp] |= STRUCTURE_3D[Default_Edid_Block[ucRdPtr] & 0xF];
// disable ->                                MxLRX_DEBUG_PRINTF(("Vic[%d]=0x%x\n", ucTemp, STRUCTURE_3D[Default_Edid_Block[ucRdPtr] & 0xF]));
// disable ->                            }
// disable ->                            if(Default_Edid_Block[ucRdPtr] & 0x8 )
// disable ->                            ucRdPtr+=2;
// disable ->                            else
                                ucRdPtr+=1;
                        break;
                    default:
                        break;
                }
        }
}
#endif
//FIX_ID_013    xxxxx
#endif //FIX_ID_013



#endif


//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_

#ifdef _SUPPORT_RCP_
/*****************************************************************************/
/* RCP functions    *******************************************************/
/*****************************************************************************/

void RCPinitQ(struct it6802_dev_data *it6802)
{
    it6802->RCPhead = 0;
    it6802->RCPtail = 0;
    it6802->RCPResult = RCP_Result_Finish;
}

void RCPKeyPush(unsigned char ucKey)
{
    unsigned char    i;

    struct it6802_dev_data *it6802 = get_it6802_dev_data();

    // IF buffer is full , can't put data
    if( ( it6802->RCPhead % MAXRCPINDEX ) == ( ( it6802->RCPtail+1 ) % MAXRCPINDEX ) )
    {
        RCP_DEBUG_PRINTF(("RCPKeyPush Full !!!\n"));
        SwitchRCPStatus(it6802,RCP_Transfer);
        return;
    }

    // put in buffer
    it6802->RCPtail += 1;
    i=it6802->RCPtail % MAXRCPINDEX;
    it6802->RCPTxArray[i]=ucKey;            // push press
//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
    if((it6802->m_bRCPTimeOut )==FALSE)
    {
    // put in buffer
    it6802->RCPtail += 1;
    i=it6802->RCPtail % MAXRCPINDEX;
    it6802->RCPTxArray[i]=ucKey|0x80;        // push release
    }
//FIX_ID_024    xxxxx

    SwitchRCPStatus(it6802,RCP_Transfer);

}

int RCPKeyPop(struct it6802_dev_data *it6802)
{

    // is Empty
    if( (it6802->RCPhead % MAXRCPINDEX) == (it6802->RCPtail % MAXRCPINDEX) )
    {
        RCP_DEBUG_PRINTF(("RCPKeyPop Empty !!!\n"));
        return FAIL;
    }

    it6802->RCPhead += 1;

    it6802->txmsgdata[0]=MSG_RCP;
    it6802->txmsgdata[1]=it6802->RCPTxArray[ it6802->RCPhead % MAXRCPINDEX ];    //tx new key to peer

    RCP_DEBUG_PRINTF(("RCPKeyPop() key = %X\n",(int) it6802->txmsgdata[1]));

//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
    if(((it6802->m_bRCPTimeOut )==TRUE) || ((it6802->m_bRCPError )==TRUE))
    {
        if(it6802->txmsgdata[1]&0x80)    // since SamSung Note did not check bit7 press/release bit. so skip release key for SamSung Note
        {
            it6802->m_bRCPError = FALSE;
            RCP_DEBUG_PRINTF(("### skip release key for SamSung Note  ###\n"));
            return SUCCESS;
        }
    }
//FIX_ID_024    xxxxx
    CsigBus_send_mscmsg(it6802);
    SwitchRCPResult(it6802,RCP_Result_Transfer);

//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
    WaitRCPresponse(it6802);
//FIX_ID_024    xxxxx
    return SUCCESS;
}


//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
void WaitRCPresponse(struct it6802_dev_data *it6802)
{
    unsigned char CsigBuswaitcnt=0;
    unsigned char MxL04;

    //if(it6802->RCPResult == RCP_Result_Transfer)
    {
        do
        {
            CsigBuswaitcnt++;
            // MxL_INT_PRINTF(("IT6802-SwitchRCPStatus() MSC RX MSC_MSG Interrupt ...\n"));
            delay1ms(CsigBusWAITTIME);
            MxL04 = MxLrxrd(MxL_RX_04);
            if( MxL04&0x10 )
            {
                MxLrxwr(MxL_RX_04,0x10);
                MxL_read_mscmsg(it6802 );
                break;
            }
        }
        while(CsigBuswaitcnt<CsigBusWAITNUM);
    }
}
//FIX_ID_024    xxxxx

void SwitchRCPResult(struct it6802_dev_data *it6802,RCPResult_Type RCPResult)
{
    it6802->RCPResult = RCPResult;
    //FIX_ID_015    xxxxx peer device no response
    it6802->RCPCheckResponse=0;    //xxxxx 2013-0806
    //FIX_ID_015    xxxxx
    switch(RCPResult)
    {
        case RCP_Result_OK:        RCP_DEBUG_PRINTF(("RCP_Result_OK\n"));   break;
        case RCP_Result_FAIL:        RCP_DEBUG_PRINTF(("RCP_Result_FAIL\n"));   break;
        case RCP_Result_ABORT:    RCP_DEBUG_PRINTF(("RCP_Result_ABORT\n"));   break;
        case RCP_Result_Transfer:    RCP_DEBUG_PRINTF(("RCP_Result_Transfer\n"));   break;
        case RCP_Result_Finish:    RCP_DEBUG_PRINTF(("RCP_Result_Finish\n"));   break;
        case RCP_Result_Unknown:    break;

    }
}

void SwitchRCPStatus(struct it6802_dev_data *it6802,RCPState_Type RCPState)
{
    it6802->RCPState = RCPState;
}

void RCPManager(struct it6802_dev_data *it6802)
{

    switch(it6802->RCPState)
    {

        case RCP_Transfer:    //Send New RCP key
            {
                if(it6802->RCPResult == RCP_Result_Finish)
                {
                    if(RCPKeyPop(it6802)==FAIL)
                        SwitchRCPStatus(it6802,RCP_Empty);
                }
                //FIX_ID_015    xxxxx peer device no response
                else
                {
                    //xxxxx 2013-0806 peer device no response
                    if(it6802->RCPCheckResponse++>6)    // 6 x 50ms = 300ms --> MxL spec
                    {
//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
                        it6802->m_bRCPTimeOut = TRUE;
//FIX_ID_024    xxxxx
                        RCP_DEBUG_PRINTF(("\n\n### RCP Timeout, Peer device no RCPE or RCPK response  ###\n"));
                        //FIX_ID_024 xxxxx disable -> CsigBus_send_mscmsg(it6802);
                        SwitchRCPResult(it6802,RCP_Result_Finish);
                    }

                }
                //FIX_ID_015    xxxxx
            }
            break;

        default :
            break;
    }
}

#endif

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
void IT6802CsigBusEventManager(struct it6802_dev_data *it6802)
{
    unsigned char CsigBusStatus;

    //for debug only !!!
    //MxLRX_DEBUG_PRINTF((" it6802->CsigBusIntEvent ...%X...%X\n",it6802->CsigBusIntEvent,it6802->CsigBusSeqNo));
    //for debug only !!!

    //ITE_150302  Chroma A222908 issue
    if(it6802->CsigBusIntEvent == 0)
    {

        if((it6802->m_DiscoveryDone == TRUE)&&(it6802->CsigBus_wait_clk_mode == TRUE))
        {
            if(it6802->CsigBus_clockmode_timeout >= 0)
                it6802->CsigBus_clockmode_timeout --;
            else
                it6802->CsigBus_clockmode_timeout = 0;

                MxLRX_DEBUG_PRINTF(( "it6802 Wait it6802->CsigBus_clockmode_timeout!!!... %d...\r\n",(int)it6802->CsigBus_clockmode_timeout));

            if(it6802->CsigBus_clockmode_timeout == 0)
            {
                it6802->CsigBus_wait_clk_mode    = FALSE;
                it6802->CsigBus_clockmode_timeout = CsigBus_CLK_MODE_TIMEOUT;
                MxLrxset(MxL_RX_0C, 0x02, 0x02);     //MxL0C[1] FW send PATH_EN{Sink}=1
                MxLRX_DEBUG_PRINTF(( "it6802 Wait clcok time out!!!... ...\r\n"));
            }
        }
    }
    //
    if(it6802->CsigBusIntEvent !=0)
    {

        CsigBusStatus = MxLrxrd(MxL_RX_1C);
        if((CsigBusStatus & 0x07)==0)
        {

        //for debug only !!!
        //TickCountPrint();
        //MxLRX_DEBUG_PRINTF((" it6802->CsigBusIntEvent ...%X...%X\n",it6802->CsigBusIntEvent,it6802->CsigBusSeqNo));
        //for debug only !!!

//============================================================================

            if((it6802->CsigBusIntEvent & B_3DSupporpt) == B_3DSupporpt)
            {


                   // MxLRX_DEBUG_PRINTF(("MxL GNT_WRT Interrupt (TxSeqNum=%d) ...\n", TxWrBstSeq));
                // disable --> if( EnMxL3DSupport==TRUE ) {
                // disable -->     if( V3D_EntryCnt==0 )
                // disable -->         v3d_burst1st(it6802);
                // disable -->     else if( V3D_EntryCnt==1 )
                // disable -->              v3d_burst2nd(it6802);
                // disable -->     else if( V3D_EntryCnt==2 )
                // disable -->              v3d_burst3rd(it6802);
                // disable -->     else if( V3D_EntryCnt==3 ) {
                // disable -->              v3d_burst4th(it6802);
                // disable -->              MxLRX_DEBUG_PRINTF((" ### 3D supporpt Write_Burst End ###\n"));
                // disable -->     }
                // disable --> }
                // disable --> else {
                // disable -->     if( V3D_EntryCnt==0 )
                // disable -->         v3d_unsupport1st(it6802);
                // disable -->     else if( V3D_EntryCnt==1 ) {
                // disable -->              v3d_unsupport2nd(it6802);
                // disable -->              MxLRX_DEBUG_PRINTF((" ### 3D Un-Support Write_Burst End ###\n"));
                // disable -->     }
                // disable -->
                // disable --> }
                // disable --> it6802->CsigBusIntEvent &= ~(B_3DSupporpt);    // finish MSC

                //FIX_ID_037 xxxxx //2014-0522 MxL compliance issue !!!
                it6802->CsigBusWaitNo =0;
                //FIX_ID_037 xxxxx

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
                if(e3DReqState != MxL3D_REQ_DONE)
                {
                    Msc_3DProcess(&e3DReqState);
                }
                else
                {
                    it6802->CsigBusIntEvent &= ~(B_3DSupporpt);    // finish MSC
                }
//FIX_ID_013    xxxxx    //For MSC 3D request issue
#else
                if( V3D_EntryCnt==0 )
                    v3d_unsupport1st(it6802);
                    else if( V3D_EntryCnt==1 ) {
                    v3d_unsupport2nd(it6802);
                    it6802->CsigBusIntEvent &= ~(B_3DSupporpt);    // finish MSC
                    MxLRX_DEBUG_PRINTF((" ### 3D Un-Support Write_Burst End ###\n"));
                }
#endif //FIX_ID_013

            }
//============================================================================
            if((it6802->CsigBusIntEvent & B_MSC_Waiting) == B_MSC_Waiting)
            {

                if(it6802->CsigBusWaitNo == 0)
                {
                    it6802->CsigBusIntEvent &= ~(B_MSC_Waiting);
                    MxLRX_DEBUG_PRINTF(("waiting B_MSC_Waiting  OK ...\n"));
                }
                else
                {
                    it6802->CsigBusWaitNo--;
                    MxLRX_DEBUG_PRINTF(("waiting B_MSC_Waiting  %X ...\n",(int) it6802->CsigBusWaitNo));
                }
                return;
            }

//============================================================================
            if((it6802->CsigBusIntEvent & B_DiscoveryDone) == B_DiscoveryDone)
            {
                switch(it6802->CsigBusSeqNo)
                {
                    //20150608_FIX_CHROMA
                    case 0:
                        CsigBusStatus = set_MxLsts(MxLSts00B, DCAP_RDY);
                        if(CsigBusStatus != SUCCESS)
                        {
                            MxLRX_DEBUG_PRINTF(("Set DCAP_RDY Need to retry ...\n"));
                            return;
                        }
                        CsigBusStatus = set_MxLint(MxLInt00B, DCAP_CHG); //send device cap change

                        if(CsigBusStatus != SUCCESS)
                        {
                            MxLRX_DEBUG_PRINTF(("Set DCAP_CHG Need to retry ...\n"));
                            return ;
                        }
                        it6802->CsigBusSeqNo=1;
                        // break;

                        //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
                        #ifdef _Debug_MSC_Abort_Only
                        m_MxLabortID++;
                        #endif
                        //FIX_ID_037 xxxxx
                    case 1:
                    //FIX_ID_011 xxxx    //Use FW send PATH_EN{Sink}=1
                            MxLrxset(MxL_RX_0C, 0x02, 0x02);     //MxL0C[1] FW send PATH_EN{Sink}=1
                            //2013-0905 change PATH_EN method ->CsigBusStatus = set_MxLsts(MxLSts01B, PATH_EN);
                            //2013-0905 change PATH_EN method ->if(CsigBusStatus != SUCCESS)
                            //2013-0905 change PATH_EN method ->{
                            //2013-0905 change PATH_EN method ->MxLRX_DEBUG_PRINTF(("Set PATH_EN Need to retry ...\n"));
                            //2013-0905 change PATH_EN method ->return;
                            //2013-0905 change PATH_EN method ->}

                            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
                            CsigBusStatus = mscCheckResult();
                            if(CsigBusStatus != SUCCESS)
                            {
                                // GreenText;
                                MxLRX_DEBUG_PRINTF(("Set PATH_EN Need to retry ...\n"));
                                // WhiteText;
                                return;
                            }
                            //FIX_ID_037 xxxxx

                            it6802->CsigBusSeqNo=2;
                            //break;

                            //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
                            #ifdef _Debug_MSC_Abort_Only
                            m_MxLabortID++;
                            #endif
                            //FIX_ID_037 xxxxx

                    //FIX_ID_011 xxxx
                    case 2:

                        CsigBusStatus = mscFire(MxL_MSC_CtrlPacket0, B_SET_HPD);
                        if(CsigBusStatus != SUCCESS)
                        {
                            MxLRX_DEBUG_PRINTF(("Set B_SET_HPD Need to retry ...\n"));
                            return;
                        }



                        it6802->CsigBusIntEvent &= ~(B_DiscoveryDone);
                        it6802->CsigBusSeqNo=0x00;
                        MxLRX_DEBUG_PRINTF(("DiscoveryDone Finish OK ...\n"));

                        //FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
                        it6802->m_DiscoveryDone = 1;
                        //FIX_ID_034 xxxxx

                        //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
                        #ifdef _Debug_MSC_Abort_Only
                        m_MxLabortID++;
                        #endif
                        //FIX_ID_037 xxxxx

                            break;
                }
                //20150608_FIX_CHROMA
                return;
            }



//============================================================================
            if((it6802->CsigBusIntEvent & B_DevCapChange) == B_DevCapChange    || (it6802->CsigBusIntEvent & B_ReadDevCap) == B_ReadDevCap)
            {
                if(MxLrxrd(0xB0)&0x01 )
                {
                    MxLrxwr(0x54, it6802->CsigBusDcapReadNo);
                    CsigBusStatus = mscFire(MxL_MSC_CtrlPacket0, B_READ_DEVCAP);
                    if(CsigBusStatus != SUCCESS)
                    {
                        MxLRX_DEBUG_PRINTF(("Set B_READ_DEVCAP Need to retry ...%X\n",(int)it6802->CsigBusDcapReadNo));
                        return;
                    }

                    it6802->MxL_devcap[it6802->CsigBusDcapReadNo] = MxLrxrd(0x56);
                    IT6802_MxL_DEBUG_PRINTF(("IT6802-DevCap[%X]=%X\n", (int)it6802->CsigBusDcapReadNo, (int)it6802->MxL_devcap[it6802->CsigBusDcapReadNo]));
                    it6802->CsigBusDcapReadNo++;

                    if(it6802->CsigBusDcapReadNo < 0x10)
                    {
                        return;
                    }

                    it6802->CsigBusIntEvent &= ~(B_ReadDevCap|B_DevCapChange);    // finish MSC    // finish B_PATH_EN
                    IT6802_MxL_DEBUG_PRINTF(("IT6802-Read Device Capability End...\n"));
                    parse_devcap(&it6802->MxL_devcap[0]);

                }
                else
                {
                    IT6802_DEBUG_INT_PRINTF(("IT6802-DCapRdy Change from '1' to '0'\n"));
                }
                return;

            }

//============================================================================
        }
    }
}

//FIX_ID_005 xxxxx


//FIX_ID_014 xxxxx


//FIX_ID_018    xxxxx    //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
void IT6802_WakeupProcess(void)
{
    if(wakeupcnt==0)
    {
        return;
    }

    wakeupcnt++;
    //MxLRX_DEBUG_PRINTF(("WakeUp Interrupt %d\n",wakeupcnt));

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
     MxLrxset(MxL_RX_2B, 0x04,0x04);    // MxL2B[2] 1 for enable HW wake up fail machanism

    if(( wakeupcnt%CDSENSE_WAKEUP)==(CDSENSE_WAKEUP-1))
    {
        chgbank(1);
        if((hdmirxrd(REG_RX_1C0)&0x0C) != 0x08)
        {
            hdmirxset(REG_RX_1C0, 0x8C, 0x08);
            MxLRX_DEBUG_PRINTF(("1k pull down  -10 percent\n"));
        }
        else
        {
            hdmirxset(REG_RX_1C0, 0x8C, 0x0C);
            MxLRX_DEBUG_PRINTF(("1k pull down  -20 percent\n"));
            //2014-0606 disable -> hdmirxset(REG_RX_1C0, 0x8C, 0x04);
            //2014-0606 disable -> MxLRX_DEBUG_PRINTF(("1k pull down  +10 percent for W1070 only\n"));

        }
        chgbank(0);
    }
//FIX_ID_037 xxxxx

    if( wakeupcnt==IGNORE_WAKEUP)
    {
        //FIX_ID_002 xxxxx    Check IT6802 chip version Identify
        {
            if( MxLrxrd(MxL_RX_28)&0x08 )
            {
                MxLrxset(MxL_RX_28, 0x08, 0x00);
            }
            else
            {
                MxLrxset(MxL_RX_28, 0x08, 0x08);
            }

        }
        //FIX_ID_002 xxxxx
    }
     else if( wakeupcnt==TOGGLE_WAKEUP )    //07-23 for SamSung Galaxy Note
     {
        if((hdmirxrd(REG_RX_019)&0x80) != 0x80)    // Reg19[7] = 1 it is D0 chip
        {
            MxLRX_DEBUG_PRINTF(("WakeUp Interrupt %d\n",(int) wakeupcnt));
            MxLrxset(MxL_RX_28,0x40,0x40);
            //it6802HPDCtrl(0,1);
            delay1ms(200);
            //it6802HPDCtrl(0,0);
            MxLrxset(MxL_RX_28,0x40,0x00);
        }
     }
     else if( wakeupcnt==RENEW_WAKEUP )    //07-23 for SamSung Galaxy Note
     {
        wakeupcnt=0;
     }



}

#endif
//FIX_ID_036    xxxxx

//FIX_ID_014 xxxxx
void IT6802HDMIEventManager(struct it6802_dev_data *it6802)
{
    if(it6802->HDMIIntEvent !=0)
    {
//============================================================================

            if((it6802->HDMIIntEvent & B_PORT0_Waiting) == B_PORT0_Waiting)
            {
                if(it6802->HDMIWaitNo[0] == 0)
                {
                    it6802->HDMIIntEvent &= ~(B_PORT0_Waiting);
                    MxLRX_DEBUG_PRINTF(("B_PORT0_Waiting  OK ...\n"));
                }
                else
                {
                    it6802->HDMIWaitNo[0]--;
                    MxLRX_DEBUG_PRINTF(("B_PORT0_Waiting  %X ...Event=%X ...Reg93=%X\n",
                            (int) it6802->HDMIWaitNo[0],(int) it6802->HDMIIntEvent,(int) hdmirxrd(0x93)));
                }
            }
            else
            {
                 if((it6802->HDMIIntEvent & B_PORT0_TMDSEvent) == B_PORT0_TMDSEvent)
                 {
                    if(CLKCheck(F_PORT_SEL_0))
                    {
                             IT6802_DEBUG_INT_PRINTF(("TMDSEvent &&&&& Port 0 Rx CKOn Detect &&&&&\n"));
                        #ifdef _SUPPORT_AUTO_EQ_
                        TMDSCheck(F_PORT_SEL_0);
                        #else
                        //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
                        #ifdef _SUPPORT_EQ_ADJUST_
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_0]));
                        #endif
                        //FIX_ID_001 xxxxx
                        #endif
                        it6802->HDMIIntEvent &= ~(B_PORT0_TMDSEvent);    // finish MSC
                    }
                }
                else if((it6802->HDMIIntEvent & B_PORT0_TimingChgEvent) == B_PORT0_TimingChgEvent)
                {
                    if(CLKCheck(F_PORT_SEL_0))
                    {
                             IT6802_DEBUG_INT_PRINTF(("TimingChgEvent &&&&& Port 0 Rx CKOn Detect &&&&&\n"));
                        //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
                        #ifdef _SUPPORT_EQ_ADJUST_
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_0]));
                        #endif
                        //FIX_ID_001 xxxxx

                        it6802->HDMIIntEvent &= ~(B_PORT0_TimingChgEvent);    // finish MSC
                    }
                }

            }
//============================================================================

//============================================================================

            if((it6802->HDMIIntEvent & B_PORT1_Waiting) == B_PORT1_Waiting)
            {
                if(it6802->HDMIWaitNo[1] == 0)
                {
                    it6802->HDMIIntEvent &= ~(B_PORT1_Waiting);
                    MxLRX_DEBUG_PRINTF(("B_PORT1_Waiting  OK ...\n"));
                }
                else
                {
                    it6802->HDMIWaitNo[1]--;
                    MxLRX_DEBUG_PRINTF(("B_PORT1_Waiting  %X ...\n",(int) it6802->HDMIWaitNo[1]));
                }
            }
            else
            {
                 if((it6802->HDMIIntEvent & B_PORT1_TMDSEvent) == B_PORT1_TMDSEvent)
                 {
                    if(CLKCheck(F_PORT_SEL_1))
                    {
                             IT6802_DEBUG_INT_PRINTF(("TMDSEvent &&&&& Port 1 Rx CKOn Detect &&&&&\n"));
                        #ifdef _SUPPORT_AUTO_EQ_
                        TMDSCheck(F_PORT_SEL_1);
                        #else
                        //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
                        #ifdef _SUPPORT_EQ_ADJUST_
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_1]));
                        #endif
                        //FIX_ID_001 xxxxx
                        #endif
                        it6802->HDMIIntEvent &= ~(B_PORT1_TMDSEvent);    // finish MSC
                    }
                }
                else if((it6802->HDMIIntEvent & B_PORT1_TimingChgEvent) == B_PORT1_TimingChgEvent)
                {
                    if(CLKCheck(F_PORT_SEL_1))
                    {
                        IT6802_DEBUG_INT_PRINTF(("TimingChgEvent &&&&& Port 1 Rx CKOn Detect &&&&&\n"));
                        #ifdef _SUPPORT_EQ_ADJUST_
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_1]));
                        #endif
                        it6802->HDMIIntEvent &= ~(B_PORT1_TimingChgEvent);    // finish MSC
                    }
                }

            }
//============================================================================
//============================================================================
//============================================================================

    }

}
// disable -> //FIX_ID_014 xxxxx
// disable ->
// disable ->
// disable -> //FIX_ID_018    xxxxx    //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
// disable -> static void IT6802_WakeupProcess(void)
// disable -> {
// disable ->    if(wakeupcnt==0)
// disable ->    {
// disable ->        return;
// disable ->    }
// disable ->
// disable ->    wakeupcnt++;
// disable ->    //MxLRX_DEBUG_PRINTF(("WakeUp Interrupt %d\n",wakeupcnt));
// disable ->
// disable ->     MxLrxset(MxL_RX_2B, 0x04,0x04);
// disable ->    if(( wakeupcnt%CDSENSE_WAKEUP)==(CDSENSE_WAKEUP-1))
// disable ->    {
// disable ->        chgbank(1);
// disable ->        if((hdmirxrd(REG_RX_1C0)&0x0C) == 0x0C)
// disable ->        {
// disable ->            hdmirxset(REG_RX_1C0, 0x8C, 0x08);
// disable ->            MxLRX_DEBUG_PRINTF(("1k pull down  -10 percent\n"));
// disable ->        }
// disable ->        else
// disable ->        {
// disable ->            hdmirxset(REG_RX_1C0, 0x8C, 0x0C);
// disable ->            MxLRX_DEBUG_PRINTF(("1k pull down  -20 percent\n"));
// disable ->        }
// disable ->        chgbank(0);
// disable ->    }
// disable ->
// disable ->
// disable ->    if( wakeupcnt==IGNORE_WAKEUP)
// disable ->    {
// disable ->        //FIX_ID_002 xxxxx    Check IT6802 chip version Identify
// disable ->        {
// disable ->            if( MxLrxrd(MxL_RX_28)&0x08 )
// disable ->            {
// disable ->                MxLrxset(MxL_RX_28, 0x08, 0x00);
// disable ->            }
// disable ->            else
// disable ->            {
// disable ->                MxLrxset(MxL_RX_28, 0x08, 0x08);
// disable ->            }
// disable ->
// disable ->        }
// disable ->        //FIX_ID_002 xxxxx
// disable ->    }
// disable ->     else if( wakeupcnt==TOGGLE_WAKEUP )    //07-23 for SamSung Galaxy Note
// disable ->     {
// disable ->        if((hdmirxrd(REG_RX_019)&0x80) != 0x80)    // Reg19[7] = 1 it is D0 chip
// disable ->        {
// disable ->            MxLRX_DEBUG_PRINTF(("WakeUp Interrupt %d\n",(int) wakeupcnt));
// disable ->            MxLrxset(MxL_RX_28,0x40,0x40);
// disable ->            //it6802HPDCtrl(0,1);
// disable ->            delay1ms(200);
// disable ->            //it6802HPDCtrl(0,0);
// disable ->            MxLrxset(MxL_RX_28,0x40,0x00);
// disable ->        }
// disable ->     }
// disable ->     else if( wakeupcnt==RENEW_WAKEUP )    //07-23 for SamSung Galaxy Note
// disable ->     {
// disable ->        wakeupcnt=0;
// disable ->     }
// disable ->
// disable ->
// disable ->
// disable -> }
//FIX_ID_018    xxxxx


//FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
unsigned char  IT6802_IsSelectedPort(unsigned char ucPortSel)
{
    unsigned char ucCurrentPort;

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    ucCurrentPort = hdmirxrd(REG_RX_051) & B_PORT_SEL;

    if(ucCurrentPort == ucPortSel)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//FIX_ID_009 xxxxx

/*****************************************************************************/
/* Driver State Machine Process **********************************************/
/*****************************************************************************/
#ifdef _IT680x_

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
void IT6802MxLInterruptHandler(struct it6802_dev_data *it6802)
{
    unsigned char MxL04, MxL05, MxL06;
    unsigned char MxLA0, MxLA1, MxLA2, MxLA3;
    unsigned char i,rddata;

    MxL04 = 0x00;
    MxL05 = 0x00;
    MxL06 = 0x00;
    MxLA0 = 0x00;
    MxLA1 = 0x00;
    MxLA2 = 0x00;
    MxLA3 = 0x00;

    MxL04 = MxLrxrd(0x04);
    MxL05 = MxLrxrd(0x05);
    MxL06 = MxLrxrd(0x06);

    MxLrxwr(0x04,MxL04);
    MxLrxwr(0x05,MxL05);
    MxLrxwr(0x06,MxL06);

    MxLA0 = MxLrxrd(0xA0);
    MxLA1 = MxLrxrd(0xA1);
    MxLA2 = MxLrxrd(0xA2);
    MxLA3 = MxLrxrd(0xA3);

    MxLrxwr(0xA0,MxLA0);
    MxLrxwr(0xA1,MxLA1);
    MxLrxwr(0xA2,MxLA2);
    MxLrxwr(0xA3,MxLA3);

//FIX_ID_018    xxxxx    //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
    IT6802_WakeupProcess();
//FIX_ID_018    xxxxx

    if( MxL04&0x01 ) {
        MxL_INT_PRINTF(("IT6802-CsigBus Link Layer TX Packet Done Interrupt ...\n"));
    }

    if( MxL04&0x02 ) {

        MxL_INT_PRINTF(("IT6802-ERROR: CsigBus Link Layer TX Packet Fail Interrupt ...\n"));
        MxL_INT_PRINTF(("IT6802- TX Packet error Status reg15=%X\n", (int)MxLrxrd(0x15)));

        rddata = MxLrxrd(0x15);

        MxLrxwr(0x15, rddata&0xF0);

    }

    if( MxL04&0x04 ) {
        MxL_INT_PRINTF(("IT6802-CsigBus Link Layer RX Packet Done Interrupt ...\n"));
    }

    if( MxL04&0x08 ) {

         MxL_INT_PRINTF(("IT6802-ERROR: CsigBus Link Layer RX Packet Fail Interrupt ...\n"));

         MxL_INT_PRINTF(("IT6802- TX Packet error Status reg15=%X\n", (int)MxLrxrd(0x15)));

         rddata = MxLrxrd(0x15);

         MxLrxwr(0x15, rddata&0x0F);

//FIX_ID_008 xxxxx    //Add SW reset when HDMI / MxL device un-plug  !!!
        rddata=hdmirxrd(REG_RX_P0_SYS_STATUS);
        MxLRX_DEBUG_PRINTF(("IT6802- Port 0 MxL unplug detect = %X !!!\n",(int) rddata));
        //delay1ms(10);
        //if((rddata & (B_P0_SCDT|B_P0_RXCK_VALID))== 0)
        if((rddata & (B_P0_SCDT))== 0)
        {
            //FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
            if(IT6802_IsSelectedPort(0) == TRUE)
            //FIX_ID_009 xxxxx
            {
                IT6802SwitchVideoState(it6802,VSTATE_SWReset);
                //FIX_ID_040 xxxxx fix MSC 3D request issue for Huawei Mobile
                #ifdef _SUPPORT_AUTO_EQ_
                DisableOverWriteRS(0);    //xxxxx 20140718 for MxL unplug detected
                #endif    /* _SUPPORT_AUTO_EQ_ */
                //FIX_ID_040 xxxxx
            }
            else
            {
                hdmirxset(REG_RX_011, 0x01, 0x01);
                delay1ms(1);
                hdmirxset(REG_RX_011, 0x01, 0x00);
#ifdef _SUPPORT_AUTO_EQ_
                DisableOverWriteRS(0);    //2013-1129 for MxL unplug detected        ss
#endif
            }
        }
//FIX_ID_008 xxxxx




    }

    if( MxL04&0x10 ) {
        MxL_INT_PRINTF(("IT6802-MSC RX MSC_MSG Interrupt ...\n"));
            MxL_read_mscmsg(it6802 );

//FIX_ID_005 xxxxx    //for waiting RAP content on
//2013-1003 not yet ->        it6802->CsigBusIntEvent |= (B_MSC_Waiting);
//2013-1003 not yet ->        it6802->CsigBusWaitNo=MAX_PATHEN_WAITNO;
//FIX_ID_005 xxxxx

    }

    if( MxL04&0x20 ) {
         MxL_INT_PRINTF(("IT6802-MSC RX WRITE_STAT Interrupt ...\n"));
    }

    if( MxL04&0x40 ) {

        MxL_INT_PRINTF(("IT6802-MSC RX WRITE_BURST Interrupt  ...\n"));
    }


    if( MxL05&0x01 ) {

        MxL_INT_PRINTF(("IT6802-MSC Req Done Interrupt ...\n"));
    }


    if( MxL05&0x02 )
     {

        MxL_INT_PRINTF(("IT6802-MSC Req Fail Interrupt (Unexpected) ...\n"));
        MxL_INT_PRINTF(("IT6802-MSC Req Fail reg18= %X\n",(int)MxLrxrd(0x18)));

        rddata = MxLrxrd(0x18);
        MxLrxwr(0x18, rddata);

        rddata = MxLrxrd(0x19);

        if( rddata&0x01 )
            MxL_INT_PRINTF(("IT6802-ERROR: TX FW Fail in the middle of the command sequence !!!\n"));
        if( rddata&0x02 )
            MxL_INT_PRINTF(("IT6802-ERROR: TX Fail because FW mode RxPktFIFO not empty !!!\n"));

        MxLrxwr(0x19, rddata);


    }

    if( MxL05&0x04 ) {
         MxLrxwr(0x05, 0x04);

         MxL_INT_PRINTF(("IT6802-MSC Rpd Done Interrupt ...\n"));
    }

    if( MxL05&0x08 ) {
        MxL_INT_PRINTF(("IT6802-MSC Rpd Fail Interrupt ...\n" ));
        MxL_INT_PRINTF(("IT6802-MSC Rpd Fail status reg1A=%X reg1B=%X\n", (int)MxLrxrd(0x1A),(int)MxLrxrd(0x1B)));

         rddata = MxLrxrd(0x1A);

        MxLrxwr(0x1A, rddata);

         rddata = MxLrxrd(0x1B);
         if( rddata&0x01 )
             MxL_INT_PRINTF(("IT6802-ERROR: Retry > 32 times !!!\n"));
         if( rddata&0x02 ) {
             MxL_INT_PRINTF(("IT6802-ERROR: Receive ABORT Packet !!!\n"));
             //get_msc_errcode();
         }

         MxLrxwr(0x1B, rddata);


    }

    if( MxL05&0x10 ) {

         MxLrxwr(0x05, 0x10);
         //ITE_150302  Chroma A222908 issue
         it6802->CsigBus_wait_clk_mode = FALSE;
             it6802->CsigBus_clockmode_timeout = CsigBus_CLK_MODE_TIMEOUT;
         //

         if( (MxLrxrd(0xB1)&0x07) == 0x02 ) {
            IT6802_DEBUG_INT_PRINTF(("MxL Clock Mode : PackPixel Mode ...\n"));
         }
         else if( (MxLrxrd(0xB1)&0x07) == 0x03 ){
            IT6802_DEBUG_INT_PRINTF(("MxL Clock Mode : 24 bits Mode ...\n"));
         }else
         {
             //ITE_150302  Chroma A222908 issue
            IT6802_DEBUG_INT_PRINTF(("MxL Clock Mode : unknown ...\n"));
            if(    (MxLrxrd(0xB1)&0x08) == 0x08)//path_en == true
            {
                it6802->CsigBus_wait_clk_mode = TRUE;
                it6802->CsigBus_clockmode_timeout = CsigBus_CLK_MODE_TIMEOUT;
            }
            //
         }

    }

    if( MxL05&0x20 ) {

        MxL_INT_PRINTF(("IT6802-DDC Req Fail Interrupt (Hardware) ...\n"));

    }

    if( MxL05&0x40 ) {

        MxL_INT_PRINTF(("IT6802-DDC Rpd Done Interrupt ...\n"));
    }

    if( MxL05&0x80 ) {

        MxL_INT_PRINTF(("IT6802-DDC Rpd Fail Interrupt ...\n"));

         rddata  = MxLrxrd(0x16);
         if( rddata&0x01 )
             MxL_INT_PRINTF(("RxState=IDLE, receive non-SOF packet !!!\n"));
         if( rddata&0x02 )
             MxL_INT_PRINTF(("RxState/=IDLE, receive unexpected packet !!!\n"));
         if( rddata&0x04 )
             MxL_INT_PRINTF(("100ms timeout !!!\n"));
         if( rddata&0x08 )
             MxL_INT_PRINTF(("100ms timeout caused by link layer error !!!\n"));
         if( rddata&0x10 )
             MxL_INT_PRINTF(("Receive unexpected STOP !!!\n"));
         if( rddata&0x20 )
             MxL_INT_PRINTF(("Transmit packet failed !!!\n"));
         if( rddata&0x40 )
             MxL_INT_PRINTF((" DDC bus hang !!!\n"));
         if( rddata&0x80 )
             MxL_INT_PRINTF(("TxState/=IDLE, receive new packet !!!\n"));

         MxLrxwr(0x16, rddata);

         rddata = MxLrxrd(0x17);
         if( rddata&0x01 )
             MxL_INT_PRINTF(("Receive TxDDCArbLose !!!\n"));

         MxLrxwr(0x17, rddata);

    }

     if( MxL06&0x01 ) {
         MxLrxwr(0x06, 0x01);
        MxLRX_DEBUG_PRINTF(("CsigBus disvovery wakeup interrupt %d  [MxL06&0x01]\n",(int)wakeupcnt));
        MxLRX_DEBUG_PRINTF(("MxL_RX_2A = %X\n",(int)MxLrxrd(MxL_RX_2A)));
        wakeupcnt = 0;

        //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
        //FIX_ID_018 xxxxx    modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
        chgbank(1);
        // FIX_ID_037 , disable -> hdmirxset(REG_RX_1C0, 0x8C, 0x08);
        // FIX_ID_037 , disable -> MxLRX_DEBUG_PRINTF(("1K pull-down -10 percent\n"));
        hdmirxset(REG_RX_1C0, 0x8C, 0x08);
        MxLRX_DEBUG_PRINTF(("1K pull-down -10 percent\n"));
        //2014-0606 disable -> hdmirxset(REG_RX_1C0, 0x8C, 0x00);    //xxxxx 2014-0604 set to default value when discovery Done !!!
        //2014-0606 disable -> MxLRX_DEBUG_PRINTF(("set 1K pull-down to default value\n"));
        chgbank(0);
         //FIX_ID_018 xxxxx
        //FIX_ID_037 xxxxx

        //FIX_ID_002 xxxxx    Check IT6802 chip version Identify
        //if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
        {
            MxLrxset(MxL_RX_28, 0x08, 0x00);//MxL28[3] RegCsigBusFloatAdj [Discovery related pulse width select]
        }
        //FIX_ID_002 xxxxx

        //FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
             MxLrxset(MxL_RX_2B, 0x04,0x00);    // MxL2B[2] 0 for disable HW wake up fail machanism
        //FIX_ID_037 xxxxx

     }

     if( MxL06&0x02 ) {
         MxLrxwr(0x06, 0x02);


        //FIX_ID_012    xxxxx    //For SamSung Galaxy Note wake up fail issue
        //FIX_ID_018    xxxxx    //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
        if(wakeupcnt==0)
        {
            wakeupcnt=1;
        }
        //FIX_ID_018    xxxxx
        //xxxxx FIX_ID_012

    }

    if( MxL06&0x04 )
    {

        MxLRX_DEBUG_PRINTF(("it6802-CsigBus Discovery Done Interrupt ...\n"));
        MxLRX_DEBUG_PRINTF(("CsigBus Discovery Done Interrupt ...\n"));
        wakeupcnt = 0;
        TxWrBstSeq = 0;

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    it6802->CsigBusIntEvent=0;
    it6802->CsigBusSeqNo=0;

    it6802->CsigBusWaitNo=0x00;
    it6802->CsigBusIntEvent |= B_DiscoveryDone;


//FIX_ID_012 xxxxx        //For SamSung Galaxy Note wake up fail issue
    it6802->CsigBusIntEvent |= (B_MSC_Waiting);
    it6802->CsigBusWaitNo=MAX_DISCOVERY_WAITNO;    //100ms
//FIX_ID_012 xxxxx

//FIX_ID_024    xxxxx    //Fixed for RCP compliance issue
    it6802->m_bRCPTimeOut = FALSE;
    it6802->m_bRCPError = FALSE;
//FIX_ID_024    xxxxx

//FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
    it6802->m_DiscoveryDone = 0;
//FIX_ID_034 xxxxx

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
#ifdef _Debug_MSC_Abort_Only
    m_MxLabortID=1;
#endif
//FIX_ID_037 xxxxx
#if 0
// disable ->        if( EnHWPathEn==FALSE )
// disable ->        {
// disable ->
// disable ->            // DEVCAP initialization here
// disable ->            MxLRX_DEBUG_PRINTF(("Set DCAP_RDY to 1 ...\n"));
// disable ->            set_MxLsts(MxLSts00B, DCAP_RDY);
// disable ->
// disable ->            MxLRX_DEBUG_PRINTF(("Set HPD to 1 ...\n"));
// disable ->//            MxLrxwr(MxL_MSC_CtrlPacket0, B_SET_HPD);    // SET_HPD
// disable ->             mscFire(MxL_MSC_CtrlPacket0, B_SET_HPD);
// disable ->
// disable ->
// disable ->            MxLRX_DEBUG_PRINTF(("Set PATH_EN to 1 ...\n"));
// disable ->            set_MxLsts(MxLSts01B, PATH_EN);
// disable ->        }
#endif
//FIX_ID_005 xxxxx

    }

    //if( MxL06&0x08 ) {
    ////        IT6802_DEBUG_INT_PRINTF(("it6802-CsigBus Discovery Fail Interrupt ... ==> %dth Fail\n",(int) it6802->DisvFailCnt));
    //}

    if( MxL06&0x10 ) {

        MxLRX_DEBUG_PRINTF(("IT6802-CsigBus PATH_EN Change Interrupt ...\n"));

        //FIX_ID_005 xxxxx    //Add CsigBus Event Handler
        it6802->CsigBusIntEvent |= B_MSC_Waiting;
        it6802->CsigBusWaitNo = MAX_PATHEN_WAITNO;
        //FIX_ID_005 xxxxx

        //ITE_150302  Chroma A222908 issue
        it6802->CsigBus_clockmode_timeout = CsigBus_CLK_MODE_TIMEOUT;
            it6802->CsigBus_wait_clk_mode = FALSE;
        rddata = MxLrxrd(0xB1);
        if(    (rddata&0x08) == 0x08)
        {
             if(((rddata&0x07) != 0x02 )&&((rddata&0x07) != 0x03 ))
            {
                MxLRX_DEBUG_PRINTF(( "IT6802-MxL Clock Mode : unknow ... ...\r\n"));
                it6802->CsigBus_wait_clk_mode = TRUE;
            }
        }
        //
//07-01        if( EnHWPathEn )
//        MxLrxwr(MxL_MSC_CtrlPacket0, B_SET_HPD);    // SET_HPD
//07-01        mscFire(MxL_MSC_CtrlPacket0, B_SET_HPD);

#if 0    // disable trigger EQ
        hdmirxset(0x26, 0x80, 0x00);
        delay1ms(1);
        hdmirxset(0x26, 0x80, 0x80);
        hdmirxset(0x22, 0x04, 0x04);
#endif


//        hdmirxset(0x22, 0x04, 0x04); //trigger AutoEQ

    }

    if( MxL06&0x20 )
    {

        MxL_INT_PRINTF(("it6802-CsigBus MUTE Change Interrupt ...\n"));
        MxL_INT_PRINTF(("it6802-Current CsigBus MUTE Status = %X\n", (int)(MxLrxrd(0xB1)&0x10)>>4));
    }

    if( MxL06&0x40 )
    {

        MxL_INT_PRINTF(("IT6802-CsigBus DCapRdy Change Interrupt ...\n"));

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler

    it6802->CsigBusIntEvent |= B_ReadDevCap;
    it6802->CsigBusDcapReadNo = 0;//20150608_FIX_CHROMA
//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
    //if(it6802->m_DiscoveryDone != 0)//20150608_FIX_CHROMA
    //{
    //    it6802->CsigBusSeqNo = 0x00;
    //}
//FIX_ID_037 xxxxx

#if 0
// disable ->        if( MxLrxrd(0xB0)&0x01 )
// disable ->        {
// disable ->
// disable ->            read_devcap_hw(it6802);  // READ_DEVCAP hardware mode
// disable ->
// disable ->            IT6802_DEBUG_INT_PRINTF(("IT6802-Set DCAP_RDY to 1 ...\n"));
// disable ->
// disable ->            set_MxLsts(MxLSts00B, DCAP_RDY);
// disable ->
// disable ->        }
// disable ->        else
// disable ->        {
// disable ->            IT6802_DEBUG_INT_PRINTF(("IT6802-DCapRdy Change from '1' to '0'\n"));
// disable ->        }
#endif
//FIX_ID_005 xxxxx
    }

    if( MxL06&0x80 )
    {

        MxL_INT_PRINTF(("IT6802-VBUS Status Change Interrupt ...\n"));
        MxL_INT_PRINTF(("IT6802-Current VBUS Status = %X\n",(int) (MxLrxrd(0x10)&0x08)>>3));

    }


    if( MxLA0&0x01 ) {
         MxL_INT_PRINTF(("IT6802-MxL Device Capability Change Interrupt ...\n"));

//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    it6802->CsigBusIntEvent |= B_DevCapChange;
    it6802->CsigBusDcapReadNo =0;//20150608_FIX_CHROMA
//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
    //if(it6802->m_DiscoveryDone != 0)//20150608_FIX_CHROMA
    //{
    //    it6802->CsigBusSeqNo = 0x00;
    //}
//FIX_ID_037 xxxxx

#if 0
// disable ->            if( MxLrxrd(0xB0)&0x01 ){
// disable ->                read_devcap_hw(it6802);      // READ_DEVCAP HardWare mode
// disable ->            }
// disable ->            else
// disable ->            {
// disable ->                IT6802_DEBUG_INT_PRINTF(("IT6802-MxL Device Capability is still not Ready !!!\n"));
// disable ->            }
#endif
//FIX_ID_005 xxxxx
    }

    if( MxLA0&0x02 ) {
        MxL_INT_PRINTF(("IT6802-MxL DSCR_CHG Interrupt ......\n"));

         MxLrxbrd(0xC0, 16, &(it6802->rxscrpad[0]));

         rddata = MxLrxrd(0x64);

         for(i=0; i<rddata; i++)
             MxL_INT_PRINTF(("RX Scratch Pad [%02d] = 0x%X\n",(int)  i, (int) it6802->rxscrpad[i]));

         if( rddata>16 )
             MxL_INT_PRINTF(("ERROR: Receive Scratch Pad Data > 16 bytes !!!\n"));

         if( it6802->rxscrpad[0]!=MxLrxrd(0x83) || it6802->rxscrpad[1]!=MxLrxrd(0x84) ) {
             MxL_INT_PRINTF(("Adopter ID = 0x%X %X\n", (int) MxLrxrd(0x83), (int) MxLrxrd(0x84)));
             MxL_INT_PRINTF(("ERROR: Adopter ID Mismatch !!!\n"));
         }


    }

    if( MxLA0&0x04 ) {

        MxL_INT_PRINTF(("IT6802-MxL REQ_WRT Interrupt  ...\n"));

        MxL_INT_PRINTF(("IT6802-Set GRT_WRT to 1  ...\n"));

        set_MxLint(MxLInt00B, GRT_WRT);

         rddata = MxLrxrd(0x64);

         for(i=0; i<rddata; i++)
             MxL_INT_PRINTF(("RX Scratch Pad [%02d] = 0x%X\n",(int)  i,(int)  MxLrxrd(0xc0+i)));


    }

    if( MxLA0&0x08 )
    {

        MxLRX_DEBUG_PRINTF(("IT6802-[**]MxL GNT_WRT Interrupt  ...\n"));

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
        //FIX_ID_005 xxxxx    //Add CsigBus Event Handler
        // disable -->    it6802->CsigBusIntEvent |= B_3DSupporpt;
        if(e3DReqState == MxL3D_REQ_WRT)
        {
            e3DReqState = MxL3D_GNT_WRT;
        }
        //FIX_ID_005 xxxxx
//FIX_ID_013    xxxxx
#else
        //FIX_ID_037 xxxxx //2014-0522 MxL compliance issue !!!
        it6802->CsigBusWaitNo =0;
        //FIX_ID_037 xxxxx

                //FIX_ID_040 xxxxx fix MSC 3D request issue for Huawei Mobile
//xxxxx 20140718 disable for Huawei Mobile issue ->
//xxxxx 20140718 disable for Huawei Mobile issue ->        it6802->CsigBusIntEvent |= B_3DSupporpt;
//xxxxx 20140718 disable for Huawei Mobile issue ->
                //FIX_ID_040 xxxxx
                if( V3D_EntryCnt==0 )
                    v3d_unsupport1st(it6802);
                    else if( V3D_EntryCnt==1 ) {
                    v3d_unsupport2nd(it6802);
                    it6802->CsigBusIntEvent &= ~(B_3DSupporpt);    // finish MSC
                    MxLRX_DEBUG_PRINTF((" ### 3D Un-Support Write_Burst End ###\n"));
                }

#endif //FIX_ID_013    xxxxx
    }



     if( MxLA0&0x10 ) {
         MxLrxwr(0xA0, 0x10);

         MxLRX_DEBUG_PRINTF(("### 3D Request Interrupt ...###\n"));

#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
    // disable -->         if( EnMSCWrBurst3D==TRUE ) {
    // disable -->            V3D_EntryCnt = 0;
    // disable -->            set_MxLint(MxLInt00B, REQ_WRT);
    // disable -->         }

    if(e3DReqState == MxL3D_REQ_DONE)
    {
        it6802->CsigBusIntEvent |= B_3DSupporpt;
        e3DReqState = MxL3D_REQ_START;
    }
//FIX_ID_013    xxxxx
#else


    //FIX_ID_037 xxxxx //2014-0522 MxL compliance issue !!!
    it6802->CsigBusWaitNo =0;
    //FIX_ID_037 xxxxx

    V3D_EntryCnt = 0;
    set_MxLint(MxLInt00B, REQ_WRT);
#endif //FIX_ID_013

     }

    if( MxLA1&0x02 ) {
        IT6802_DEBUG_INT_PRINTF(("IT6802-MxL EDID Change Interrupt ...\n"));
    }

}

#endif
//FIX_ID_036    xxxxx

void IT6802HDMIInterruptHandler(struct it6802_dev_data *it6802)
{
    unsigned char Reg05h;
    unsigned char Reg06h;
    unsigned char Reg07h;
    unsigned char Reg08h;
    unsigned char Reg09h;
    unsigned char Reg0Ah;
//    unsigned char Reg0Bh;
    unsigned char RegD0h;


    if( it6802->m_VState == VSTATE_Off )
    {
        return ; // if VSTATE is off, do not proecess any event.
    }
    chgbank(0);

    Reg05h = hdmirxrd(REG_RX_005);
    Reg06h = hdmirxrd(REG_RX_006);
    Reg07h = hdmirxrd(REG_RX_007);
    Reg08h = hdmirxrd(REG_RX_008);
    Reg09h = hdmirxrd(REG_RX_009);

    Reg0Ah = hdmirxrd(REG_RX_P0_SYS_STATUS);
//    Reg0Bh = hdmirxrd(REG_RX_P1_SYS_STATUS);
    RegD0h = hdmirxrd(REG_RX_0D0);

    hdmirxwr(REG_RX_005, Reg05h);
    hdmirxwr(REG_RX_006, Reg06h);
    hdmirxwr(REG_RX_007, Reg07h);
    hdmirxwr(REG_RX_008, Reg08h);
    hdmirxwr(REG_RX_009, Reg09h);
//2013-0606 disable ==>
    hdmirxwr(REG_RX_0D0, RegD0h&0x0F);


//    IT6802_DEBUG_INT_PRINTF(("111111111111111111111111 STATUS 111111111111111111111= %X\n",hdmirxrd(REG_RX_P0_SYS_STATUS)));
     if( Reg05h!=0x00 )
    {

        IT6802_DEBUG_INT_PRINTF(("Reg05 = %X\n",(int) Reg05h));

         if( Reg05h&0x80 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Port 0 HDCP Off Detected ###\n"));
            it6802->m_ucEccCount_P0=0;
         }

         if( Reg05h&0x40 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Port 0 ECC Error %X ####\n",(int) (it6802->m_ucEccCount_P0)));
//            HDMICheckErrorCount(&(it6802->EQPort[F_PORT_SEL_0]));    //07-04 for port 0
            hdmirx_INT_P0_ECC(it6802);
         }

         if( Reg05h&0x20 ) {

             IT6802_DEBUG_INT_PRINTF(("#### Port 0 HDMI/DVI Mode change ####\n"));
//FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
            if(CLKCheck(0))
            hdmirx_INT_HDMIMode_Chg(it6802,0);
//FIX_ID_009 xxxxx

         }

         if( Reg05h&0x08 )
         {
            IT6802_DEBUG_INT_PRINTF3(("#### Port 0 HDCP Authentication Start ####\n"));
            it6802->m_ucEccCount_P0=0;
            #ifdef _SUPPORT_HDMI_REPEATER_
                hdmirxset(REG_RX_P0_BCAPS,B_KSV_READY,0 );    // Clear KSV FIFO Ready.
                if(it6802->m_ucCurrentHDMIPort == 0 )
                {
                    IT680X_DownStream_AuthStart(it6802) ;
                }
            #endif


            #ifdef _SUPPORT_AUTO_EQ_
            //FIX_ID_014 xxxxx
                if( ucPortAMPOverWrite[F_PORT_SEL_0] == 0)
                {
                    if((it6802->HDMIIntEvent & (B_PORT0_Waiting))==0)
                    {
                        hdmirxwr(REG_RX_022, 0x00);    // power down auto EQ

                        it6802->HDMIIntEvent |= (B_PORT0_Waiting);
                        it6802->HDMIIntEvent |= (B_PORT0_TMDSEvent);
                        it6802->HDMIWaitNo[0]=MAX_TMDS_WAITNO;
                    }
                    else if((it6802->HDMIIntEvent & (B_PORT0_TMDSEvent)))
                    {
                        it6802->HDMIIntEvent |= (B_PORT0_Waiting);
                        it6802->HDMIWaitNo[0] += MAX_HDCP_WAITNO;
                    }
                }
                else
                {
                    if((it6802->HDMIIntEvent & (B_PORT0_TMDSEvent)))
                    {
                        it6802->HDMIIntEvent |= (B_PORT0_Waiting);
                        it6802->HDMIWaitNo[0] += MAX_HDCP_WAITNO;
                    }
                }
            //FIX_ID_014 xxxxx
            #endif

//FIX_ID_005 xxxxx    //for waiting RAP content on
            if( (Reg0Ah&0x40))
            {
                it6802->CsigBusIntEvent |= (B_MSC_Waiting);
                it6802->CsigBusWaitNo=MAX_CsigBus_WAITNO;
            }
//FIX_ID_005 xxxxx


         }

         if( Reg05h&0x10 ) {
             IT6802_DEBUG_INT_PRINTF3(("#### Port 0 HDCP Authentication Done ####\n"));

            if( (Reg0Ah&0x40))
            {
                it6802->CsigBusIntEvent |= (B_MSC_Waiting);
                it6802->CsigBusWaitNo=MAX_CsigBus_WAITNO;
            }

            #ifdef _SUPPORT_HDMI_REPEATER_
            //FIX_ID_032 xxxxx    //Support HDCP Repeater function for HDMI Tx device
            #ifdef _SUPPORT_HDMI_REPEATER_
                hdmirxset(REG_RX_P0_BCAPS,B_KSV_READY,0 );    // Clear KSV FIFO Ready.
                if(it6802->m_ucCurrentHDMIPort == 0 )
                {
                    IT680X_DownStream_AuthDone(it6802) ;
                }
            #endif

                #ifdef _SUPPORT_AUTO_EQ_
                //FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
                    if(ucPortAMPOverWrite[0]==0)    // 2013-0801
                    {
                        it6802->HDMIIntEvent &= ~(B_PORT0_Waiting);
                        it6802->HDMIWaitNo[0]= 0;
                        it6802->HDMIIntEvent |= B_PORT0_TMDSEvent;
                        //return;
                    }
                //FIX_ID_033 xxxxx
                #endif

            #else

                #ifdef _SUPPORT_AUTO_EQ_
                //FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
                //FIX_ID_032 xxxxx    //Support HDCP Repeater function for HDMI Tx device
                //xxxxx 2014-0421
                //FIX_ID_014 xxxxx
                    if(ucPortAMPOverWrite[0]==0)    // 2013-0801
                    {
                        it6802->HDMIIntEvent &= ~(B_PORT0_Waiting);
                        it6802->HDMIWaitNo[0]= 0;
                        it6802->HDMIIntEvent |= B_PORT0_TMDSEvent;
                        //return;
                    }
                //FIX_ID_014 xxxxx
                //xxxxx 2014-0421
                //FIX_ID_032 xxxxx
                //FIX_ID_033 xxxxx
                #endif

            //FIX_ID_032 xxxxx
            #endif
         }

         if( Reg05h&0x04 )
         {
             IT6802_DEBUG_INT_PRINTF(("#### Port 0 Input Clock Change Detect ####\n"));
             if((hdmirxrd(0x0C)&0x40) == 0x00)//P0 clock not stable  ite180509
             {
                chgbank(1);
                hdmirxset(0xB1, 0x60, 0x00);
                chgbank(0);
                CLKStbChkCnt=0;
             }
         }

         if( Reg05h&0x02 )
         {

            it6802->m_ucEccCount_P0=0;
            it6802->m_ucDeskew_P0=0;
            //it6802->m_ucDeskew_P1=0;
            //it6802->m_ucEccCount_P1=0;

            IT6802_DEBUG_INT_PRINTF(("#### Port 0 Rx CKOn Detect ####\n"));
            hdmirxset(0x13,0x07,0x07);
            delay1ms(5);
            hdmirxset(0x13,0x07,0x00);

//#ifdef _SUPPORT_HDCP_REPEATER_
#if 1
//FIX_ID_032 xxxxx    //Support HDCP Repeater function for HDMI Tx device
            if(CLKCheck(F_PORT_SEL_0))
            {
                #ifdef _SUPPORT_AUTO_EQ_
                    TMDSCheck(F_PORT_SEL_0);
                #else
                    //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
                    #ifdef _SUPPORT_EQ_ADJUST_
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_0]));
                    #endif
                    //FIX_ID_001 xxxxx
                #endif
            }
#else

            // NO --> Authentication Start    &&    Input Clock Change Detect    &&     B_PORT1_TMDSEvent
            if(( Reg05h&0x08 )==0 && ( Reg05h&0x04 )==0  &&  (it6802->HDMIIntEvent & (B_PORT0_TMDSEvent))==0)
            {
                    if(CLKCheck(F_PORT_SEL_0))
                    {
                        #ifdef _SUPPORT_AUTO_EQ_
                        TMDSCheck(F_PORT_SEL_0);
                        #endif
                    }
            }
            else
            {
                if(( Reg05h&0x10 ) == 0)
                {
                    if((it6802->HDMIIntEvent & (B_PORT0_Waiting))==0)
                    {
                        hdmirxwr(REG_RX_022, 0x00);    // power down auto EQ

                        //FIX_ID_019 xxxxx modify ENHYS control for MxL mode
                        if(hdmirxrd(REG_RX_P0_SYS_STATUS) & B_P0_RXCK_VALID)
                        {
                            it6802->HDMIIntEvent |= (B_PORT0_Waiting);
                            it6802->HDMIIntEvent |= (B_PORT0_TMDSEvent);
                            it6802->HDMIWaitNo[0]=MAX_TMDS_WAITNO;
                        }
                        //FIX_ID_019 xxxxx
                    }
                }
                else
                {
                    if(CLKCheck(F_PORT_SEL_0))
                    {
                        #ifdef _SUPPORT_AUTO_EQ_
                        TMDSCheck(F_PORT_SEL_0);
                        #endif
                    }
                }
            }
//FIX_ID_032 xxxxx
#endif
         }

         if( Reg05h&0x01 ) {
            IT6802_DEBUG_INT_PRINTF(("#### Port 0 Power 5V change ####\n"));
            hdmirx_INT_5V_Pwr_Chg(it6802,0);


            // disable ->// disable ->FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            // disable ->if(CheckPlg5VPwr(F_PORT_SEL_0)==FALSE)
            // disable ->{
            // disable ->
            // disable ->    #ifdef _SUPPORT_EQ_ADJUST_
            // disable ->    DisableOverWriteRS(F_PORT_SEL_0);
            // disable ->    #endif
            // disable ->}
            // disable ->// disable ->FIX_ID_001 xxxxx

         }
     }

     if( Reg06h!=0x00 )
     {
         if( Reg06h&0x80 ) {
            IT6802_DEBUG_INT_PRINTF(("#### Port 1 HDCP Off Detected ###\n"));
            it6802->m_ucEccCount_P1=0;

         }

         if( Reg06h&0x40 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Port 1 ECC Error ####\n"));
            hdmirx_INT_P1_ECC(it6802);
         }

         if( Reg06h&0x20 )
         {
             IT6802_DEBUG_INT_PRINTF(("#### Port 1 HDMI/DVI Mode change ####\n"));
//FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
            if(CLKCheck(1))
            hdmirx_INT_HDMIMode_Chg(it6802,1);
//FIX_ID_009 xxxxx
         }

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//xxxxx 2014-0421 disabe->         if( Reg06h&0x10 )
//xxxxx 2014-0421 disabe->         {
//xxxxx 2014-0421 disabe->             IT6802_DEBUG_INT_PRINTF(("#### Port 1 HDCP Authentication Done ####\n"));
//xxxxx 2014-0421 disabe-> //FIX_ID_014 xxxxx
//xxxxx 2014-0421 disabe->            if((it6802->HDMIIntEvent & (B_PORT1_Waiting)))
//xxxxx 2014-0421 disabe->            {
//xxxxx 2014-0421 disabe->                it6802->HDMIWaitNo[1] = 0;
//xxxxx 2014-0421 disabe->            }
//xxxxx 2014-0421 disabe-> //FIX_ID_014 xxxxx
//xxxxx 2014-0421 disabe->
//xxxxx 2014-0421 disabe->         }
//FIX_ID_033 xxxxx



         if( Reg06h&0x08 )
         {
             IT6802_DEBUG_INT_PRINTF(("#### Port 1 HDCP Authentication Start ####\n"));
            it6802->m_ucEccCount_P1=0;

#ifdef _SUPPORT_AUTO_EQ_
//FIX_ID_014 xxxxx
            if( ucPortAMPOverWrite[F_PORT_SEL_1] == 0)
            {
                if((it6802->HDMIIntEvent & (B_PORT1_Waiting))==0)
                {
                    IT6802_DEBUG_INT_PRINTF((" power down auto EQ of PORT 1\n"));
                    hdmirxwr(REG_RX_03A, 0x00);    // power down auto EQ

                    it6802->HDMIIntEvent |= (B_PORT1_Waiting);
                    it6802->HDMIIntEvent |= (B_PORT1_TMDSEvent);
                    it6802->HDMIWaitNo[1]=MAX_TMDS_WAITNO;
                }
                else if((it6802->HDMIIntEvent & (B_PORT1_TMDSEvent)))
                {
                    it6802->HDMIIntEvent |= (B_PORT1_Waiting);
                    it6802->HDMIWaitNo[1] += MAX_HDCP_WAITNO;
                }
            }
            else
            {
                if((it6802->HDMIIntEvent & (B_PORT1_TMDSEvent)))
                {
                    it6802->HDMIIntEvent |= (B_PORT1_Waiting);
                    it6802->HDMIWaitNo[1] += MAX_HDCP_WAITNO;
                }
            }
//FIX_ID_014 xxxxx
#endif
            #ifdef _SUPPORT_HDMI_REPEATER_
                hdmirxset(REG_RX_P1_BCAPS,B_KSV_READY,0 );    // Clear KSV FIFO Ready.
                if(it6802->m_ucCurrentHDMIPort == 1 )
                {
                    IT680X_DownStream_AuthStart(it6802) ;
                }
            #endif

         }

         if( Reg06h&0x10 )
         {
             IT6802_DEBUG_INT_PRINTF(("#### Port 1 HDCP Authentication Done ####\n"));
//FIX_ID_014 xxxxx
            if((it6802->HDMIIntEvent & (B_PORT1_Waiting)))
            {
                it6802->HDMIWaitNo[1] = 0;
            }
//FIX_ID_014 xxxxx


#ifdef _SUPPORT_AUTO_EQ_
//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
// disable ->    if( ucPortAMPValid[F_PORT_SEL_1] == 0)
            if(ucPortAMPOverWrite[1]==0)    // 2013-0801
            {
                it6802->HDMIIntEvent &= ~(B_PORT1_Waiting);
                it6802->HDMIWaitNo[1]= 0;
                it6802->HDMIIntEvent |= B_PORT1_TMDSEvent;
            }
//FIX_ID_033 xxxxx
#endif

            #ifdef _SUPPORT_HDMI_REPEATER_
                hdmirxset(REG_RX_P1_BCAPS,B_KSV_READY,0 );    // Clear KSV FIFO Ready.
                if(it6802->m_ucCurrentHDMIPort == 1 )
                {
                    IT680X_DownStream_AuthDone(it6802) ;
                }
            #endif

         }



         if( Reg06h&0x04 )
         {
             IT6802_DEBUG_INT_PRINTF(("#### Port 1 Input Clock Change Detect ####\n"));
              if((hdmirxrd(0x0C)&0x80) == 0x00)//P1 clock not stable  ite180509
             {
                chgbank(1);
                hdmirxset(0xC1, 0x60, 0x00);
                chgbank(0);
                CLKStbChkCnt=0;
             }
         }

         if( Reg06h&0x02 )
         {
            IT6802_DEBUG_INT_PRINTF(("#### Port 1 Rx CKOn Detect ####\n"));
            hdmirxset(0x1A,0x07,0x07);
            delay1ms(5);
            hdmirxset(0x1A,0x07,0x00);
            //it6802->m_ucEccCount_P0=0;
            //it6802->m_ucDeskew_P0=0;
            it6802->m_ucDeskew_P1=0;
            it6802->m_ucEccCount_P1=0;

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//FIX_ID_032 xxxxx    //Support HDCP Repeater function for HDMI Tx device
#if 0

            if(CLKCheck(F_PORT_SEL_1))
            {
            TMDSCheck(F_PORT_SEL_1);
            }
#else
            // NO --> Authentication Start    &&    Input Clock Change Detect    &&     B_PORT1_TMDSEvent
            if(( Reg06h&0x08 )==0 && ( Reg06h&0x04 )==0  &&  (it6802->HDMIIntEvent & (B_PORT1_TMDSEvent))==0)
            {
                    if(CLKCheck(F_PORT_SEL_1))
                    {
                        #ifdef _SUPPORT_AUTO_EQ_
                        TMDSCheck(F_PORT_SEL_1);
                        #endif
                    }
            }
            else
            {
                if(( Reg06h&0x10 ) == 0)
                {
                    if((it6802->HDMIIntEvent & (B_PORT1_Waiting))==0)
                    {
                        hdmirxwr(REG_RX_03A, 0x00);    // power down auto EQ
                        it6802->HDMIIntEvent |= (B_PORT1_Waiting);
                        it6802->HDMIIntEvent |= (B_PORT1_TMDSEvent);
                        it6802->HDMIWaitNo[1]=MAX_TMDS_WAITNO;
                    }
                }
                else
                {
                    if(CLKCheck(F_PORT_SEL_1))
                    {
                        #ifdef _SUPPORT_AUTO_EQ_
                        TMDSCheck(F_PORT_SEL_1);
                        #else
                        //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
                        #ifdef _SUPPORT_EQ_ADJUST_
                        HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_1]));
                        #endif
                        //FIX_ID_001 xxxxx
                        #endif
                    }
                }
            }
#endif
//FIX_ID_032 xxxxx
//FIX_ID_033 xxxxx


        }



         if( Reg06h&0x01 )
         {
            IT6802_DEBUG_INT_PRINTF(("#### Port 1 Power 5V change ####\n"));
            hdmirx_INT_5V_Pwr_Chg(it6802,1);
            // disable ->//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
            // disable ->if(CheckPlg5VPwr(F_PORT_SEL_1)==FALSE)
            // disable ->{
            // disable ->    #ifdef _SUPPORT_EQ_ADJUST_
            // disable ->    DisableOverWriteRS(F_PORT_SEL_1);
            // disable ->    #endif
            // disable ->}
            // disable ->//FIX_ID_001 xxxxx
         }

         }

     if( Reg07h!=0x00)
     {
         // 2024/11/19 modified by jjtseng
        if( Reg07h&0x40 )
        {
            IT6802_DEBUG_INT_PRINTF(("#### Audio Auto Mute ####\n"));
            // 2024/11/19 modified by jjtseng
            hdmirx_SetHWMuteClr();
            hdmirx_ClearHWMuteClr();
            //~jjtseng
        }

        if( Reg07h&0x80 )
        {
            IT6802_DEBUG_INT_PRINTF(("#### Audio FIFO Error ####\n"));
            /*20230105 for Roger & Jaming tuning Audio*/
            hdmirxset(REG_RX_07A, 0x80, 0x00);
            /*20230105*/

            // 2024/11/19 modified by jjtseng
            hdmirx_SetHWMuteClr();
            hdmirx_ClearHWMuteClr();
            //~jjtseng

            aud_fiforst();
            #ifdef EnableCalFs
            //FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
                m_AudioChannelStatusErrorCount++;
                AudioFsCal();
            //FIX_ID_023 xxxxx
            #endif
        }

         // 2024/11/19 modified by jjtseng
        // if( Reg07h&0x40 ) {
        //     IT6802_DEBUG_INT_PRINTF(("#### Audio Auto Mute ####\n"));
        // }
         //~jjtseng 2024/11/19

         if( Reg07h&0x20 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Packet Left Mute ####\n"));
             IT6802_SetVideoMute(it6802,OFF);
             #ifdef _SUPPORT_HDMI_REPEATER_
                 IT680X_DownStream_AVMuteSet(it6802,FALSE) ;
             #endif
         }

         if( Reg07h&0x10 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Set Mute Packet Received ####\n"));
             #ifdef _SUPPORT_HDMI_REPEATER_
                 IT680X_DownStream_AVMuteSet(it6802,TRUE) ;
             #endif
             IT6802_SetVideoMute(it6802,ON);
         }

         if( Reg07h&0x08 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Timer Counter Tntterrupt ####\n"));
            //if(it6802->m_VState == VSTATE_VideoOn)
            //    hdmirxset(0x84,0x80,0x80);    //2011/06/17 xxxxx, for enable Rx Chip count

         }

         if( Reg07h&0x04 ) {
             IT6802_DEBUG_INT_PRINTF(("#### Video Mode Changed ####\n"));
         }

         if( Reg07h&0x02 ) {
            hdmirx_INT_SCDT_Chg(it6802);
         }

         if( Reg07h&0x01 ) {
             if( (Reg0Ah&0x40)>>6 )
             {
                 IT6802_DEBUG_INT_PRINTF(("#### Port 0 Bus Mode : MxL ####\n"));

                //FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
                if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
                {
                    chgbank(1);
                    hdmirxset(REG_RX_1B6,0x07,0x00);
                    //FIX_ID_007 xxxxx    //for debug IT6681  HDCP issue
                    hdmirxset(REG_RX_1B1,0x20,0x20);//Reg1b1[5] = 1 for enable over-write
                    hdmirxset(REG_RX_1B2,0x07,0x01);    // default 0x04 , change to 0x01
                    IT6802_DEBUG_INT_PRINTF((" Port 0 Bus Mode Reg1B1  = %X ,Reg1B2  = %X\n",(int) hdmirxrd(REG_RX_1B1),(int) hdmirxrd(REG_RX_1B2)));
                    //FIX_ID_007 xxxxx
                    chgbank(0);
                }
                //FIX_ID_002 xxxxx

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
                    //xxxxx 2014-0522 disable for Allion MxL debug !!! -->    it6802HPDCtrl(0,1);    // MxL port , set HPD = 1
//FIX_ID_037 xxxxx

             }
             else
             {
                IT6802_DEBUG_INT_PRINTF(("#### Port 0 Bus Mode : HDMI ####\n"));
                //FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
                    if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
                    {
                        chgbank(1);
                        hdmirxset(REG_RX_1B6,0x07,0x03);
                        ////FIX_ID_007 xxxxx    //for debug IT6681  HDCP issue
                        hdmirxset(REG_RX_1B1,0x20,0x00);//Reg1b1[5] = 0 for disable over-write
                        hdmirxset(REG_RX_1B2,0x07,0x04);    // default 0x04 , change to 0x01
                        EQ_PORT0_PRINTF((" Port 0 Bus Mode Reg1B1  = %X ,Reg1B2  = %X\n",(int) hdmirxrd(REG_RX_1B1),(int) hdmirxrd(REG_RX_1B2)));
                        ////FIX_ID_007 xxxxx
                        chgbank(0);
                    }
                //FIX_ID_002 xxxxx

             }

         }
     }

     if( Reg08h!=0x00)
     {
         if( Reg08h&0x80 ) {
            //             MxLRX_DEBUG_PRINTF(("#### No General Packet 2 Received ####\n"));
         }

         if( Reg08h&0x40 ) {
            //             MxLRX_DEBUG_PRINTF(("#### No General Packet Received ####\n"));
         }

         if( Reg08h&0x20 ) {
             IT6802_DEBUG_INT_PRINTF(("#### No Audio InfoFrame Received ####\n"));
         }

         if( Reg08h&0x10) {
             IT6802_DEBUG_INT_PRINTF(("#### No AVI InfoFrame Received ####\n"));
         }

         if( Reg08h&0x08 ) {
             IT6802_DEBUG_INT_PRINTF(("#### CD Detect ####\n"));

         }

         if( Reg08h&0x04 ) {
            //             MxLRX_DEBUG_PRINTF(("#### Gen Pkt Detect ####\n"));
             IT6802_DEBUG_INT_PRINTF(("#### 3D InfoFrame Detect ####\n"));

                #ifdef Enable_Vendor_Specific_packet
                    if(it6802->f_de3dframe_hdmi == FALSE)
                    {
                    it6802->f_de3dframe_hdmi = IT6802_DE3DFrame(TRUE);
                    }
                #endif

         }

         if( Reg08h&0x02 ) {
             IT6802_DEBUG_INT_PRINTF(("#### ISRC2 Detect ####\n"));
         }

         if( Reg08h&0x01 ) {
             IT6802_DEBUG_INT_PRINTF(("#### ISRC1 Detect ####\n"));
         }
     }

    if( Reg09h!=0x00 )
    {
        if( Reg09h&0x80 )
        {
            IT6802_DEBUG_INT_PRINTF(("#### H2V Buffer Skew Fail ####\n"));
        }

        if( Reg09h&0x40 )
        {

            //FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
            if(HdmiI2cAddr==IT6802A0_HDMI_ADDR)
            {
                hdmirxwr(0x09, 0x20); //bug ~ need to update by Andrew
            }
            else
            {
                hdmirxwr(0x09, 0x40);
            }
            //FIX_ID_002 xxxxx
            IT6802_DEBUG_INT_PRINTF(("#### Port 1 Deskew Error ####\n"));
            hdmirx_INT_P1_Deskew(it6802);
        }

        if( Reg09h&0x20 ) {
            hdmirxwr(0x09, 0x20);
            IT6802_DEBUG_INT_PRINTF(("#### Port 0 Deskew Error ####\n"));
            hdmirx_INT_P0_Deskew(it6802);
        }

        if( Reg09h&0x10 ) {
            IT6802_DEBUG_INT_PRINTF(("#### New Audio Packet Received ####\n"));
        }

        if( Reg09h&0x08 ) {
            IT6802_DEBUG_INT_PRINTF(("#### New ACP Packet Received ####\n"));
        }

        if( Reg09h&0x04 ) {
            IT6802_DEBUG_INT_PRINTF(("#### New SPD Packet Received ####\n"));
        }

        if( Reg09h&0x02) {
            IT6802_DEBUG_INT_PRINTF(("#### New MPEG InfoFrame Received ####\n"));
        }

        if( Reg09h&0x01) {
            unsigned char AVIDB[2] ;

            IT6802_DEBUG_INT_PRINTF(("#### New AVI InfoFrame Received ####\n"));
            //IT6802VideoOutputConfigure();
            chgbank(2) ;
            AVIDB[0] = hdmirxrd(REG_RX_AVI_DB0) ;
            AVIDB[1] = hdmirxrd(REG_RX_AVI_DB1) ;
            chgbank(0) ;
            if( (AVIDB[0] != it6802->prevAVIDB[0])||
            (AVIDB[1] != it6802->prevAVIDB[1]))
            {
                it6802->m_NewAVIInfoFrameF=TRUE;
            }
            it6802->prevAVIDB[0] = AVIDB[0] ;
            it6802->prevAVIDB[1] = AVIDB[1] ;
        }

    }


    if( RegD0h!=0x00 )
    {
// disable        if( RegD0h&0x08)
// disable        {
// disable            EQ_DEBUG_PRINTF(("#### Port 1 Rx Clock change detect Interrupt ####\n"));
// disable        }
// disable
// disable        if( RegD0h&0x04)
// disable        {
// disable            EQ_DEBUG_PRINTF(("#### Port 0 Rx Clock change detect Interrupt ####\n"));
// disable        }
     if( RegD0h&0x10 )
     {

        hdmirxwr(0xD0, 0x30);
        RegD0h&=0x30;
//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
        //xxxxx 2014-0508 disable ->        ucEqRetryCnt[0]=0;
//FIX_ID_035 xxxxx
         EQ_PORT0_PRINTF(("#### Port 0 EQ done interrupt ####\n"));

        #ifdef _SUPPORT_AUTO_EQ_
        //2013-0923 disable ->    ucPortAMPOverWrite[0]=1;    //2013-0801
        AmpValidCheck(0);    //2013-0801
        #endif

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    #ifdef _SUPPORT_EQ_ADJUST_
    HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_0]));
    #endif
//FIX_ID_001 xxxxx

 }

     if( RegD0h&0x40 )
     {

    hdmirxwr(0xD0, 0xC0);
    RegD0h&=0xC0;
//FIX_ID_035 xxxxx //For MTK6592 HDMI to SII MxL TX compliance issue
    //xxxxx 2014-0508 disable ->    ucEqRetryCnt[1]=0;
//FIX_ID_035 xxxxx
     EQ_PORT1_PRINTF(("#### Port 1 EQ done interrupt ####\n"));


    #ifdef _SUPPORT_AUTO_EQ_
    //2013-0923 disable ->    ucPortAMPOverWrite[1]=1;    //2013-0801
    AmpValidCheck(1);    //2013-0801
    #endif

    // disable -> //FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    // disable ->    #ifdef _SUPPORT_EQ_ADJUST_
    // disable ->    HDMIStartEQDetect(&(it6802->EQPort[F_PORT_SEL_1]));
    // disable ->    #endif
    // disable -> //FIX_ID_001 xxxxx
 }

    if( RegD0h&0x20)
    {

    hdmirxwr(0xD0, 0x20);
    EQ_PORT0_PRINTF(("#### Port 0 EQ Fail Interrupt ####\n"));
//    HDMICheckErrorCount(&(it6802->EQPort[F_PORT_SEL_0]));    //07-04 for port 0
//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    #ifdef _SUPPORT_AUTO_EQ_
    hdmirx_INT_EQ_FAIL(it6802,F_PORT_SEL_0);
    #endif
//FIX_ID_001 xxxxx
}

    if( RegD0h&0x80)
        {

    hdmirxwr(0xD0, 0x80);
    EQ_PORT1_PRINTF(("#### Port 1 EQ Fail Interrupt ####\n"));
//    HDMICheckErrorCount(&(it6802->EQPort[F_PORT_SEL_1]));    //07-04 for port 0
//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    #ifdef _SUPPORT_AUTO_EQ_
    hdmirx_INT_EQ_FAIL(it6802,F_PORT_SEL_1);
    #endif
//FIX_ID_001 xxxxx
}



    }

}

void IT6802_fsm(void)
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();

#if defined(_IT6802_) || defined(_IT6803_)
    #ifdef SUPPORT_UART_CMD
    if(m_UartCmd == 0)
    #endif
    {

        #ifndef Enable_IR
        it6802AutoPortSelect(it6802data);
        #endif
    }
#endif

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
    IT6802MxLInterruptHandler(it6802data);
#endif
//FIX_ID_036    xxxxx

    IT6802HDMIInterruptHandler(it6802data);
    IT6802VideoHandler(it6802data);

    //yjh2026 RX 상태 확인용
    {
        static int rx_dbg_cnt = 0;
        if(++rx_dbg_cnt >= 20)
        {
            unsigned char p0_sys = hdmirxrd(REG_RX_P0_SYS_STATUS);
            unsigned char portsel = hdmirxrd(REG_RX_051) & B_PORT_SEL;
            printf("[RX] VState=%d port=%d P0_SYS=0x%02X (5V=%d RXCK=%d SCDT=%d)\r\n", it6802data->m_VState, portsel, p0_sys, (p0_sys&0x01), (p0_sys>>3)&1, (p0_sys>>7)&1);
            //yjh2026 HDCP 상태 확인용 (enc/auth)
            {
                unsigned char p0_hdcp = hdmirxrd(REG_RX_P0_HDCP_STATUS); // 0x93
                printf("[RX-HDCP] Reg0x93=0x%02X (bit4_enc=%d bit0_auth=%d) RxHDCPState=%d\r\n",
                       p0_hdcp, (p0_hdcp>>4)&1, p0_hdcp&1, (int)it6802data->m_RxHDCPState);
            }
            rx_dbg_cnt = 0;
        }
    }
#ifndef _FIX_ID_028_
//FIX_ID_028 xxxxx //For Debug Audio error with S2
    IT6802AudioHandler(it6802data);
//FIX_ID_028 xxxxx
#endif

#ifdef _SUPPORT_HDMI_REPEATER_
    IT680X_DownStream_Polling(it6802data) ;
#endif

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
    RCPManager(it6802data);
#endif
//FIX_ID_036    xxxxx

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
        #ifdef _SUPPORT_EQ_ADJUST_
        if(it6802data->EQPort[F_PORT_SEL_0].f_manualEQadjust==TRUE)
            HDMIAdjustEQ(&(it6802data->EQPort[F_PORT_SEL_0]));    // for port 0

        if(it6802data->EQPort[F_PORT_SEL_1].f_manualEQadjust==TRUE)
            HDMIAdjustEQ(&(it6802data->EQPort[F_PORT_SEL_1]));    // for port 1
        #endif
//FIX_ID_001 xxxxx

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    IT6802CsigBusEventManager(it6802data);
//FIX_ID_005 xxxxx
#endif
//FIX_ID_036    xxxxx

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//xxxxx 2014-0421 disable -> #ifndef _SUPPORT_HDCP_REPEATER_
//FIX_ID_032 xxxxx    //Support HDCP Repeater function for HDMI Tx device
//FIX_ID_014 xxxxx    //Add HDMI Event Handler
    IT6802HDMIEventManager(it6802data);
//FIX_ID_014 xxxxx
//FIX_ID_032 xxxxx
//xxxxx 2014-0421 disable -> #endif
//FIX_ID_033 xxxxx


    #ifdef Enable_IT6802_CEC
    CECManager();                // detect CEC for IT6802_CEC
    #endif
}

#if defined(_IT6802_) || defined(_IT6803_)
void it6802AutoPortSelect(struct it6802_dev_data *it6802)
{

#if 0
    if(SEL_PORT_1==1)
    {
        if(it6802->m_ucCurrentHDMIPort != 0)
        {
            it6802PortSelect(0);
        }
    }
    else
        {
            if(it6802->m_ucCurrentHDMIPort == 0)
            {
                it6802PortSelect(1);
            }

        }
#endif

}
#endif

void get_vid_info( void )
{
#if 1
    int HSyncPol, VSyncPol, InterLaced;
    int HTotal, HActive, HFP, HSYNCW;
    int VTotal, VActive, VFP, VSYNCW;
//    int rddata;
//    int i;
//    unsigned long PCLK, sump;
    unsigned int ucTMDSClk;    //, sumt;
    unsigned char ucPortSel;
    unsigned char rddata;
    unsigned char ucClk;

    // 2024/8/15 modified by jau-chih.tseng
    // int PCLK;    //, sump;
    unsigned int PCLK ;
    unsigned int RCLKx10 ;
    //~jjtseng 2024/8/15



    // 2024/8/15 modified by jau-chih.tseng

    rddata = hdmirxrd(0x9A);

    RCLKx10 = m_ROSCCLK / 100 ;
    PCLK = RCLKx10 * 255;
    PCLK /= rddata ;
    PCLK /= 10;

    ucPortSel = hdmirxrd(REG_RX_051) & B_PORT_SEL;
    rddata = hdmirxrd(0x90);

    // 2024/8/29 modified by jjtseng

    ucTMDSClk = PCLK;
    if( IsHDMIMode() )
    {
        switch(hdmirxrd(0x99)&0xF0)
        {
        case 0x50:
            ucTMDSClk += PCLK /4 ; // 1.25 PCLK ;
            break ;
        case 0x60:
            ucTMDSClk += PCLK /2 ; // 1.5 PCLK ;
            break ;
        case 0x70:
            ucTMDSClk += PCLK ; // 2 PCLK ;
            break ;
        }
    }


    //if(ucPortSel == F_PORT_SEL_1)
    //{
    //
    //        VIDEOTIMNG_DEBUG_PRINTF(("Reg51[0] = 1 Active Port HDMI\n"));
    //        ucClk = hdmirxrd(REG_RX_092);
    //        if(ucClk != 0 )
    //        {
    //
    //            ucTMDSClk=RCLKx10*256/ucClk;
    //            if( rddata&0x04 )
    //                ucTMDSClk *=2 ;
    //            else if( rddata&0x08 )
    //                ucTMDSClk *=4 ;
    //
    //
    //            ucTMDSClk /= 10 ;
    //            VIDEOTIMNG_DEBUG_PRINTF((" Port 1 TMDS CLK  = %d\n",(int)ucTMDSClk));
    //        }
    //        else ucTMDSClk = 0 ;
    //
    //        //EQ_DEBUG_PRINTF((" HDMI Reg92  = %X\n",(int) hdmirxrd(0x92)));
    //        //EQ_DEBUG_PRINTF((" HDMI Reg38  = %X\n",(int) hdmirxrd(0x38)));
    //}
    //else
    //{
    //        VIDEOTIMNG_DEBUG_PRINTF(("Reg51[0] = 0 Active Port MxL\n"));
    //        ucClk = hdmirxrd(REG_RX_091);
    //        if(ucClk != 0 )
    //        {
    //
    //            ucTMDSClk=RCLKx10*256/ucClk;
    //            if( rddata&0x01 )
    //                ucTMDSClk *=2 ;
    //            else if( rddata&0x02 )
    //                ucTMDSClk *=4 ;
    //
    //            ucTMDSClk /= 10 ;
    //            VIDEOTIMNG_DEBUG_PRINTF(("Port 0 TMDS CLK  = %d\n",(int)ucTMDSClk));
    //        }
    //        else ucTMDSClk = 0 ;
    //        //EQ_DEBUG_PRINTF((" HDMI Reg91  = %X\n",(int) hdmirxrd(0x91)));
    //        //EQ_DEBUG_PRINTF((" HDMI Reg20  = %X\n",(int) hdmirxrd(0x20)));
    //
    //}
    ////~jjtseng 2024/8/15
    //~jjtseng 2024/8/29

    InterLaced = (hdmirxrd(0x99)&0x02)>>1;

    HTotal   = ((hdmirxrd(0x9D)&0x3F)<<8) + hdmirxrd(0x9C);
    HActive  = ((hdmirxrd(0x9F)&0x3F)<<8) + hdmirxrd(0x9E);
    HFP      = ((hdmirxrd(0xA1)&0xF0)<<4) + hdmirxrd(0xA2);
    HSYNCW   = ((hdmirxrd(0xA1)&0x01)<<8) + hdmirxrd(0xA0);
    HSyncPol = hdmirxrd(0xA8)&0x04>>2;

    VTotal   = ((hdmirxrd(0xA4)&0x0F)<<8) + hdmirxrd(0xA3);
    VActive  = ((hdmirxrd(0xA4)&0xF0)<<4) + hdmirxrd(0xA5);
    VFP      = hdmirxrd(0xA7)&0x3F;
    VSYNCW   = hdmirxrd(0xA6)&0x1F;
    VSyncPol = (hdmirxrd(0xA8)&0x08)>>3;

//    CurVTiming.TMDSCLK     = (int)TMDSCLK;
    CurTMDSCLK             = (int)ucTMDSClk;
    CurVTiming.PCLK        = (int)PCLK;
    CurVTiming.HActive     = HActive;
    CurVTiming.HTotal      = HTotal;
    CurVTiming.HFrontPorch = HFP;
    CurVTiming.HSyncWidth  = HSYNCW;
    CurVTiming.HBackPorch  = HTotal - HActive - HFP - HSYNCW;
    CurVTiming.VActive     = VActive;
    CurVTiming.VTotal      = VTotal;
    CurVTiming.VFrontPorch = VFP;
    CurVTiming.VSyncWidth  = VSYNCW;
    CurVTiming.VBackPorch  = VTotal - VActive - VFP - VSYNCW;
    CurVTiming.ScanMode    = (InterLaced)&0x01;
    CurVTiming.VPolarity   = (VSyncPol)&0x01;
    CurVTiming.HPolarity   = (HSyncPol)&0x01;

    printf("[HDMI RX] Input resolution: %d x %d (HTotal=%d, VTotal=%d, PCLK=%d)\n",
           HActive, VActive, HTotal, VTotal, PCLK);
#endif
}

void IPLLOvw_chk(int port)
{
    BYTE reg0,reg1,Reg4Eh,Regsts,IPLL_HS2,CLKStable,tmp;
     // emily add 20180508 add for SDI-HDMI source start

    //float CalTMDSCLK=0;
    reg0 = hdmirxrd(0x90);
    if(!port)
    {
        reg0 &= 0x03;
        reg1 = hdmirxrd(0x91);
        Regsts = hdmirxrd(0x0A);
    }
    else//port1
    {
        reg0 = (reg0&0x0C)>>2;
        reg1 = hdmirxrd(0x92);
        Regsts = hdmirxrd(0x0B);
    }
    if(reg0 == 0)
    {
        tmp = reg1;
    }
    else
    {
        if(reg0 == 0x01)
        {
            tmp = reg1>>1;//div2
        }
        else// 2,3
        {
            tmp = reg1>>2;//div4
        }
    }
    Reg4Eh = hdmirxrd(0x4E);
    IPLL_HS2= (Reg4Eh& 0xC0)>>(6+port);
    CLKStable= (hdmirxrd(0x0C)&0xC0)>>(6+port);
    if(CLKStable)
    {
        printf("IPLL check %x \n",(int)CLKStbChkCnt);
        if(CLKStbChkCnt==0x10)
        {
            // printf(" Reg0Ah=%02X, Reg4Eh=%02X, Reg90h=%02X, Reg91h=%02X\n", Reg0Ah, Reg4Eh, Reg90h, Reg91h);
            //if(reg0&0x02) CalTMDSCLK= 4*RCLK*256/(reg1+1.0);
            //else if(reg0&0x01) CalTMDSCLK= 2*RCLK*256/(reg1+1.0);
            //else CalTMDSCLK= RCLK*256/(reg1+1.0);
            //printf("TMDSCLK = %3.3fMHz\n", (float)(CalTMDSCLK)/1000);
            //if(CalTMDSCLK > 150000 && P0_IPLL_HS2==0)
            if((tmp <= 27) && (!IPLL_HS2))// high mode
            { //OverWrite
                if((Regsts&0x80) == 0)
                {
                    chgbank(1);
                    //printf("TMDSCLK = %3.3fMHz  :: ", (float)(CalTMDSCLK)/1000);
                    printf(" Regsts=%02X, Reg4Eh=%02X, reg0=%02X, reg1=%02X\n", (int)Regsts, (int)Reg4Eh, (int)reg0, (int)reg1);

                    printf("OverWrite IPLL/OPLL \n");
                    if(!port)
                    {
                        hdmirxset(0xB2, 0x05, 0x04);
                        hdmirxset(0xB1, 0xE0, 0xE0);
                    }
                    else
                    {
                        hdmirxset(0xC2, 0x05, 0x04);
                        hdmirxset(0xB1, 0xC0, 0xC0);
                        hdmirxset(0xC1, 0x20, 0x20);
                    }
                    chgbank(0);
                }
            }
            CLKStbChkCnt=0;
        }
        else
        {
            CLKStbChkCnt++;
        }
    }
    else
    { // not OverWrite
        chgbank(1);
        if(!port)
        {
            hdmirxset(0xB1, 0x60, 0x00);
        }
        else
        {
            hdmirxset(0xB1, 0x40, 0x00);
            hdmirxset(0xC1, 0x20, 0x00);
        }
        chgbank(0);
    }

}
void show_vid_info(void)
{
#if 1
    int InBPC, InBPP;
    int MxL_Mode;
    int MxL_CLK_Mode;
    int GCP_CD       = CD8BIT; //24 bits per pixel

    unsigned long FrameRate;

    GCP_CD = ((hdmirxrd(0x99)&0xF0)>>4);

    switch( GCP_CD ) {
    case 5 :
    VIDEOTIMNG_DEBUG_PRINTF(("I/P ColorDepth = 30 bits per pixel\n"));
    InBPC=10;
    hdmirxset(0x65, 0x0C, 0x04);
    OutCD = OUT10B;
    break;
    case 6 :
    VIDEOTIMNG_DEBUG_PRINTF(("I/P ColorDepth = 36 bits per pixel\n"));
    InBPC=12;
    hdmirxset(0x65, 0x0C, 0x08);
    OutCD = OUT12B;
    break;
    default :
    VIDEOTIMNG_DEBUG_PRINTF(("I/P ColorDepth = 24 bits per pixel\n"));
    InBPC=8;
    hdmirxset(0x65, 0x0C, 0x00);
    OutCD = OUT8B;
    break;
    }

    switch( OutCD ) {
    case 1 :
    VIDEOTIMNG_DEBUG_PRINTF(("O/P ColorDepth = 30 bits per pixel\n"));
    break;
    case 2 :
    VIDEOTIMNG_DEBUG_PRINTF(("O/P ColorDepth = 36 bits per pixel\n"));
    break;
    default :
    VIDEOTIMNG_DEBUG_PRINTF(("O/P ColorDepth = 24 bits per pixel\n"));
    break;
    }

    chgbank(2);
    InColorMode = (hdmirxrd(0x15)&0x60)>>5;
    chgbank(0);

    if( InColorMode==1 ) { //YCbCr422
    InBPP = InBPC*2;
    }
    else
    {
    InBPP = InBPC*3;
    }

    switch( InColorMode ) {
    case 0 :
    VIDEOTIMNG_DEBUG_PRINTF(("Input Color Mode = RGB444\n"));
    //         hdmirxset(0xAE, 0x01, 0x01);
    //         defaultrgb();
    break;
    case 1 :
    VIDEOTIMNG_DEBUG_PRINTF(("Input Color Mode = YCbCr422\n"));
    //         hdmirxset(0xAE, 0x01, 0x00);
    //         yuv422torgb();
    break;
    case 2 :
    VIDEOTIMNG_DEBUG_PRINTF(("Input Color Mode = YCbCr444\n"));
    //         hdmirxset(0xAE, 0x01, 0x00);
    //         yuv444torgb();
    break;
    default :
    VIDEOTIMNG_DEBUG_PRINTF(("Input Color Mode = Reserved !!!\n"));
    break;
    }


    OutColorMode = (hdmirxrd(0x65)&0x30)>>4;
    switch( OutColorMode ) {
    case 0 :
    VIDEOTIMNG_DEBUG_PRINTF(("Output Color Mode = RGB444\n"));
    //         hdmirxset(0x65, 0x30, 0x00);
    break;
    case 1 :
    VIDEOTIMNG_DEBUG_PRINTF(("Output Color Mode = YCbCr422\n"));
    //         hdmirxset(0x65, 0x30, 0x10);
    break;
    case 2 :
    VIDEOTIMNG_DEBUG_PRINTF(("Output Color Mode = YCbCr444\n"));
    //         hdmirxset(0x65, 0x30, 0x20);
    break;
    default :
    VIDEOTIMNG_DEBUG_PRINTF(("Output Color Mode = Reserved !!!\n"));
    break;
    }


    //    VIDEOTIMNG_DEBUG_PRINTF(("Video Input Timing: %s\n", s_VMTable[VIC].format));
    //    VIDEOTIMNG_DEBUG_PRINTF(("TMDSCLK = %3.3fMHz\n", (unsigned long)(CurTMDSCLK)/1000));
    //    VIDEOTIMNG_DEBUG_PRINTF(("PCLK = %3.3fMHz\n", (unsigned long)(CurVTiming.PCLK)/1000));

    VIDEOTIMNG_DEBUG_PRINTF(("HFrontPorch = %d\n", CurVTiming.HFrontPorch));
    VIDEOTIMNG_DEBUG_PRINTF(("HSyncWidth = %d\n", CurVTiming.HSyncWidth));
    VIDEOTIMNG_DEBUG_PRINTF(("HBackPorch = %d\n", CurVTiming.HBackPorch));
    VIDEOTIMNG_DEBUG_PRINTF(("VFrontPorch = %d\n", CurVTiming.VFrontPorch));
    VIDEOTIMNG_DEBUG_PRINTF(("VSyncWidth = %d\n", CurVTiming.VSyncWidth));
    VIDEOTIMNG_DEBUG_PRINTF(("VBackPorch = %d\n", CurVTiming.VBackPorch));

    FrameRate = (unsigned long)(CurVTiming.PCLK)*1000*1000;
    FrameRate /= CurVTiming.HTotal;
    FrameRate /= CurVTiming.VTotal;
    VIDEOTIMNG_DEBUG_PRINTF(("FrameRate = %ld Hz\n", FrameRate));

    if( CurVTiming.ScanMode==0 ) {
    VIDEOTIMNG_DEBUG_PRINTF(("ScanMode = Progressive\n"));
    }
    else {
    VIDEOTIMNG_DEBUG_PRINTF(("ScanMode = InterLaced\n"));
    }

    if( CurVTiming.VPolarity==1 ) {
    VIDEOTIMNG_DEBUG_PRINTF(("VSyncPol = Positive\n"));
    }
    else {
    VIDEOTIMNG_DEBUG_PRINTF(("VSyncPol = Negative\n"));
    }

    if( CurVTiming.HPolarity==1 ) {
    VIDEOTIMNG_DEBUG_PRINTF(("HSyncPol = Positive\n"));
    }
    else {
    VIDEOTIMNG_DEBUG_PRINTF(("HSyncPol = Negative\n"));
    }

    if(((hdmirxrd(0x51)&0x01)))
    {
        VIDEOTIMNG_DEBUG_PRINTF(("Port= 1 ,Reg18=%X ,",(int)hdmirxrd(REG_RX_018)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg38=%X, ",(int)hdmirxrd(REG_RX_038)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg3E=%X, ",(int)hdmirxrd(REG_RX_03E)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg3F=%X, ",(int)hdmirxrd(REG_RX_03F)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg40=%X\n",(int)hdmirxrd(REG_RX_040)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg41=%X\n",(int)hdmirxrd(REG_RX_041)));
        chgbank(1);
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_B_CS=%X  ",(int)(hdmirxrd(REG_RX_1DD)&0x80)>>7));
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_G_CS=%X  ",(int)(hdmirxrd(REG_RX_1DE)&0x80)>>7));
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_R_CS=%X\n",(int)(hdmirxrd(REG_RX_1DF)&0x80)>>7));
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_B_RS=%X  ",(int)(hdmirxrd(REG_RX_1DD)&0x7F)));
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_G_RS=%X  ",(int)(hdmirxrd(REG_RX_1DE)&0x7F)));
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_R_RS=%X\n",(int)(hdmirxrd(REG_RX_1DF)&0x7F)));
        VIDEOTIMNG_DEBUG_PRINTF((" Reg1C1  = %X , Reg1C2  = %X\n",(int)hdmirxrd(REG_RX_1C1),(int)hdmirxrd(REG_RX_1C2)));
        chgbank(0);
    }
    else
    {

        VIDEOTIMNG_DEBUG_PRINTF(("Port= 0 ,Reg11=%X ,",(int)hdmirxrd(REG_RX_011)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg20=%X, ",(int)hdmirxrd(REG_RX_020)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg26=%X, ",(int)hdmirxrd(REG_RX_026)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg27=%X, ",(int)hdmirxrd(REG_RX_027)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg28=%X, ",(int)hdmirxrd(REG_RX_028)));
        VIDEOTIMNG_DEBUG_PRINTF(("Reg29=%X\n",(int)hdmirxrd(REG_RX_029)));
        chgbank(1);
        VIDEOTIMNG_DEBUG_PRINTF(("Rec_B_CS=%X  ",(int)(hdmirxrd(REG_RX_1D5)&0x80)>>7));
         VIDEOTIMNG_DEBUG_PRINTF(("Rec_G_CS=%X  ",(int)(hdmirxrd(REG_RX_1D6)&0x80)>>7));
         VIDEOTIMNG_DEBUG_PRINTF(("Rec_R_CS=%X\n",(int)(hdmirxrd(REG_RX_1D7)&0x80)>>7));

         VIDEOTIMNG_DEBUG_PRINTF(("Rec_B_RS=%X  ",(int)(hdmirxrd(REG_RX_1D5)&0x7F)));
         VIDEOTIMNG_DEBUG_PRINTF(("Rec_G_RS=%X  ",(int)(hdmirxrd(REG_RX_1D6)&0x7F)));
         VIDEOTIMNG_DEBUG_PRINTF(("Rec_R_RS=%X\n",(int)(hdmirxrd(REG_RX_1D7)&0x7F)));
        VIDEOTIMNG_DEBUG_PRINTF(("REG_RX_1B1 = %X ,  REG_RX_1B2 = %X\n",(int)hdmirxrd(REG_RX_1B1),(int)hdmirxrd(REG_RX_1B2)));


        chgbank(0);
    }

    VIDEOTIMNG_DEBUG_PRINTF(("TMDSCLK = %d MHz\n", (int)(CurTMDSCLK)));
    VIDEOTIMNG_DEBUG_PRINTF(("PCLK = %d MHz\n", (int)(CurVTiming.PCLK)));
    VIDEOTIMNG_DEBUG_PRINTF(("HActive = %d\n", CurVTiming.HActive));
    VIDEOTIMNG_DEBUG_PRINTF(("VActive = %d\n", CurVTiming.VActive));
    VIDEOTIMNG_DEBUG_PRINTF(("HTotal = %d\n", CurVTiming.HTotal));
    VIDEOTIMNG_DEBUG_PRINTF(("VTotal = %d\n", CurVTiming.VTotal));

//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
    MxL_Mode     = ((hdmirxrd(0x0A)&0x40)>>6);
    MxL_CLK_Mode = ((MxLrxrd(0xB1)&0x07));
#else
    MxL_Mode = 0;
#endif
//FIX_ID_036    xxxxx
    if( MxL_Mode )
    {
        if( MxL_CLK_Mode==0x02 )
        VIDEOTIMNG_DEBUG_PRINTF(("BUS MODE : MxL PackPixel Mode\n"));
        else
        VIDEOTIMNG_DEBUG_PRINTF(("BUS MODE : MxL 24 bits Mode\n"));
    }
    if(IsHDMIMode())
    {
        VIDEOTIMNG_DEBUG_PRINTF(("HDMI/DVI Mode : HDMI\n"));
    }
    else
    {
        VIDEOTIMNG_DEBUG_PRINTF(("HDMI/DVI Mode : DVI\n"));
    }
#endif
}

#endif


/*****************************************************************************/
/* Power Control Functions  **************************************************/
/*****************************************************************************/


/*********************************************************************************/
/* End of IT680x.c ***************************************************************/
/*********************************************************************************/


#ifdef Enable_Vendor_Specific_packet

#define HDMI_3DFORMAT_PRESENT           0x40
#define HDMI_3DFORMAT_OFF               0x00
#define FRAME_PACKING                   0x00
#define TOP_AND_BOTTOM                  0x60
#define SIDE_BY_SIDE                    0x80


SET_DE3D_FRAME t_3d_syncgen[] =
{
    //640x480      //524   //559   //514   //526
    {0x01      ,0x020C  ,0x022F  ,0x0202  ,0x020E,    480}, // 60Hz
    //480p      //524   //560   //515   //530
    {0x02      ,0x020C  ,0x0230  ,0x0203  ,0x0212,    480}, // 60Hz
    {0x03      ,0x020C  ,0x0230  ,0x0203  ,0x0212,    480}, // 60Hz
    //576p      //624   //668   //619   //629
    {0x11      ,0x0270  ,0x029C  ,0x026B  ,0x0275,    576}, // 50Hz
    {0x12      ,0x0270  ,0x029C  ,0x026B  ,0x0275,    576}, // 50Hz
    //720p      //749   //774   //744   //754
    {0x3c      ,0x02ED  ,0x0306  ,0x02E8  ,0x02F2,    720}, // 24Hz
    {0x3d      ,0x02ED  ,0x0306  ,0x02E8  ,0x02F2,    720}, // 25Hz
    {0x3e      ,0x02ED  ,0x0306  ,0x02E8  ,0x02F2,    720}, // 30Hz
    {0x13      ,0x02ED  ,0x0306  ,0x02E8  ,0x02F2,    720}, // 50Hz
    {0x04      ,0x02ED  ,0x0306  ,0x02E8  ,0x02F2,    720}, // 60Hz

//disable -> 1080i     //1124   //1165   //1120   //1129
//disable ->     {0x05      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    540}, // 50Hz
//disable ->     {0x14      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    540}, // 60Hz
//disable -> 1080i     //1124   //1165   //1120   //1129
//disable ->     {0x20      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    540}, // 24Hz
//disable ->     {0x22      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    540}, // 30Hz
//disable ->     {0x1f      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    540}, // 50Hz
//disable ->     {0x10      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    540}, // 60Hz

    //1080p    //1124   //1165   //1120   //1129
    {0x20      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    1080}, // 24Hz
    {0x21      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    1080}, // 25Hz
    {0x22      ,0x0464  ,0x048D  ,0x0460  ,0x0469,    1080}, // 30Hz

    //default
    {0xFF      ,0x0000  ,0x0000  ,0x0000  ,0x0000,0x0000}
};
//Reg_PGVTotal    749        // 0x2ED
//Reg_PGVActst    774        // 0x306
//Reg_PGVActEd    744        // 0x2E8
//Reg_PGVsyncEd    754        // 0x2F2
#define Reg_PGVTotal_19D    0x9D    //[11:4]    0x19D[7:0]
#define Reg_PGVTotal_19C    0x9C    //[3:0]        0x19C[7:4]
#define Reg_PGVActSt_192    0x92    //[7:0]        0x192[7:0]
#define Reg_PGVActSt_193    0x93    //[11:8]    0x193[3:0]
#define Reg_PGVActEd_193    0x93    //[3:0]        0x193[7:4]
#define Reg_PGVActEd_194    0x94    //[11:4]    0x194[7:0]
#define Reg_PGVSyncEd_19F    0x9F    //[3:0]        0x19F[7:4]
#define Reg_PGVSyncSt_19F    0x9F    //[11:8]    0x19F[3:0]
#define Reg_PGVSyncSt_19E    0x9E    //[7:0]        0x19E[7:0]

#define Reg_PG3DRSt_18F        0x8F    //[7:0]        0x190[11:8] 0x18F[7:0]
#define Reg_PG3DRStEd_190    0x90    //[7:0]        0x191[3:0] 0x18F[11:8]
#define Reg_PG3DREd_191    0x91    //[11:4]        0x191[11:4] 0x190[3:0]

#define REG_RX_066_4_DE3DFrame    0x66    //[4] 1: 3D frame-packet mode to sequence mode
#define REG_RX_085_5_En3DROut        0x85    //[5] 1: Enable 3DR output
//
//pccmd w 0f 01 94
//pccmd w 8f 86 94
//pccmd w 90 41 94
//pccmd w 91 47 94
//pccmd w 92 06 94
//pccmd w 93 83 94
//pccmd w 94 2E 94
//pccmd w 9c d0 94
//pccmd w 9d 2e 94
//pccmd w 9f 22 94
//pccmd w 0f 00 94
//pccmd w 66 58 94

void Dump3DReg(void)
{
    ushort    i,j;
    BYTE ucData;

    VSDB_DEBUG_PRINTF(("\n       "));
    for(j = 0; j < 16; j++)
    {
        VSDB_DEBUG_PRINTF((" %02X",(int) j));
        if((j == 3)||(j==7)||(j==11))
        {
                VSDB_DEBUG_PRINTF((" :"));
        }
    }
   VSDB_DEBUG_PRINTF(("\n"));

    chgbank(1);

    for(i = 0x80; i < 0xa0; i+=16)
    {
        VSDB_DEBUG_PRINTF(("[%03X]  ",i));
        for(j = 0; j < 16; j++)
        {
            ucData = hdmirxrd((BYTE)((i+j)&0xFF));
            VSDB_DEBUG_PRINTF((" %02X",(int) ucData));
            if((j == 3)||(j==7)||(j==11))
            {
                VSDB_DEBUG_PRINTF((" :"));
            }
        }
        VSDB_DEBUG_PRINTF(("\n"));

    }

   VSDB_DEBUG_PRINTF(("\n        =====================================================\n"));

    chgbank(0);
}

unsigned char IT6802_DE3DFrame(unsigned char ena_de3d)
/*
 * This function configures the HDMI DE3DFrame
 * @param uunsigned char ena_de3d
 * @return      TRUE
 *              FALSE
 */
{
    unsigned char i, uc,uc1,uc2,uc3;
    unsigned int v_total;
    unsigned int v_act_start;
    unsigned int v_act_end;
    unsigned int v_sync_end;
    unsigned int v_act_bspace;
    unsigned int v_2d_Vtotal;
    unsigned int HActive;
    unsigned int LR_3D_Start;
    unsigned int LR_3D_End;

    #ifdef DEBUG_MODE
    //dbmsg_trace(DBM_DPATH,"IT680x - HDMI_DE3DFrame\n");
    #endif

    struct it6802_dev_data *it6802data = get_it6802_dev_data();

    if(ena_de3d  == TRUE)
    {

         chgbank(2);
            uc=hdmirxrd(REG_RX_224);
        uc1=hdmirxrd(REG_RX_228);
        uc2=hdmirxrd(REG_RX_229);
        uc3=hdmirxrd(REG_RX_22A);
         chgbank(0);
        if((uc == 0x81)&&(uc1==0x03)&&(uc2==0x0C)&&(uc3==0x00))//ITE_150420  check  IEEE OUI
            // 3D InfoFrame Packet Type is valid
        {
             chgbank(2);
            it6802data->s_Current3DFr.VIC=hdmirxrd(REG_RX_218);    //AVI INFO PB4
            it6802data->s_Current3DFr.HB0=hdmirxrd(REG_RX_224);    // General Packet Header Byte 0
            it6802data->s_Current3DFr.HB1=hdmirxrd(REG_RX_225);
            it6802data->s_Current3DFr.HB2=hdmirxrd(REG_RX_226);
                it6802data->s_Current3DFr.PB0=hdmirxrd(REG_RX_227);    // General Packet Data Byte 0
            it6802data->s_Current3DFr.PB1=hdmirxrd(REG_RX_228);
            it6802data->s_Current3DFr.PB2=hdmirxrd(REG_RX_229);
            it6802data->s_Current3DFr.PB3=hdmirxrd(REG_RX_22A);
            it6802data->s_Current3DFr.PB4=hdmirxrd(REG_RX_22B);
            it6802data->s_Current3DFr.PB5=hdmirxrd(REG_RX_22C);
            it6802data->s_Current3DFr.PB6=hdmirxrd(REG_RX_22D);
            it6802data->s_Current3DFr.PB7=hdmirxrd(REG_RX_22E);
             chgbank(0);

            //#ifdef DEBUG_MODE_3D
            VSDB_DEBUG_PRINTF(("\nIT653x - HDMI_DumpDE3DFrameInfo:\n"));
            VSDB_DEBUG_PRINTF(("        HDMI VIC = 0x%X\n",it6802data->s_Current3DFr.VIC));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe HB0 = 0x%X\n",(int) it6802data->s_Current3DFr.HB0));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe HB1 = 0x%X\n",(int) it6802data->s_Current3DFr.HB1));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe HB2 = 0x%X\n",(int) it6802data->s_Current3DFr.HB2));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB0 = 0x%X\n",(int) it6802data->s_Current3DFr.PB0));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB1 = 0x%X\n",(int) it6802data->s_Current3DFr.PB1));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB2 = 0x%X\n",(int) it6802data->s_Current3DFr.PB2));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB3 = 0x%X\n",(int) it6802data->s_Current3DFr.PB3));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB4 = 0x%X\n",(int) it6802data->s_Current3DFr.PB4));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB5 = 0x%X\n",(int) it6802data->s_Current3DFr.PB5));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB6 = 0x%X\n",(int) it6802data->s_Current3DFr.PB6));
            VSDB_DEBUG_PRINTF(("        Record HDMI vender specific inforframe PB7 = 0x%X\n",(int) it6802data->s_Current3DFr.PB7));
            //#endif



            /******************************  3D integration  *************************************/

            it6802data->de3dframe_config.LR_Reference             =  2; // Source of the 3D L/R reference.
            it6802data->de3dframe_config.FrameDominance           =  0; // Left or Right Eye is first in L/R image pair.
            it6802data->de3dframe_config.LR_Encoding              =  1; // Type of 3D L/R encoding
            it6802data->de3dframe_config.TB_Reference             =  2; // Top/Bottom reference for vertically sub-sampled sources
            it6802data->de3dframe_config.OE_Reference             =  2; // Odd/Even reference for horizontally sub-sampled sources

            it6802data->de3dframe_config.NumActiveBlankLines      =  0; // Number of lines separating vertically packed L/R data to be removed (cropped)before being displayed
            it6802data->de3dframe_config.NumberOfEncodedLines     =  0; // Number of encoded lines in one L/R eye frame of the display data to be blanked out with "Blanking Color".
            it6802data->de3dframe_config.LeftEncodedLineLocation  = -1; // Active line number of 1st encoded line in one Left eye frame of the display data (-1=unknown).
            it6802data->de3dframe_config.RightEncodedLineLocation = -1; // Active line number of 1st encoded line in one Right eye frame of the display data (-1=unknown).
            it6802data->de3dframe_config.BlankingColor            =  7; // Color to use when blanking (or masking off) any embedded L/R encoding

            if(((it6802data->s_Current3DFr.PB4&0xE0) == HDMI_3DFORMAT_PRESENT) && ((it6802data->s_Current3DFr.PB5&0xF0) == FRAME_PACKING))
            {
                i =0;

                while(t_3d_syncgen[i].Vic != 0xFF)
                {
                    if(t_3d_syncgen[i].Vic == it6802data->s_Current3DFr.VIC)
                    {
                        break;
                    }
                    i++;
                }
        v_total     = t_3d_syncgen[i].V_total;
        v_act_start = t_3d_syncgen[i].V_act_start;
        v_act_end   = t_3d_syncgen[i].V_act_end;
        v_sync_end  = t_3d_syncgen[i].V_sync_end;
        v_2d_Vtotal = t_3d_syncgen[i].V_2D_active_total;
        chgbank(1);
        hdmirxset(Reg_PGVTotal_19D, 0xFF, (unsigned char) ((v_total & 0xFF0)>>4));             //pccmd w 9d 2e
        hdmirxset(Reg_PGVTotal_19C, 0xF0, (unsigned char) ((v_total & 0x00F)<<4));            //pccmd w 9c d0
        hdmirxset(Reg_PGVActSt_192, 0xFF, (unsigned char) ((v_act_start & 0x0FF)));            //pccmd w 92 06
        hdmirxset(Reg_PGVActSt_193, 0x0F, (unsigned char) ((v_act_start & 0xF00)>>8));        //pccmd w 93 83
        hdmirxset(Reg_PGVActEd_193, 0xF0, (unsigned char) ((v_act_end & 0x00F)<<4));        //pccmd w 93 83
        hdmirxset(Reg_PGVActEd_194, 0xFF, (unsigned char) ((v_act_end & 0xFF0)>>4));        //pccmd w 94 2E
        hdmirxset(Reg_PGVSyncEd_19F, 0xF0, (unsigned char) ((v_sync_end & 0x00F)<<4));    //pccmd w 9f 22


#if 1
        LR_3D_Start = (v_act_start - (v_2d_Vtotal/2));
        LR_3D_End =(v_act_start + (v_2d_Vtotal/2));
#else
        LR_3D_Start = ((v_total/2));
        LR_3D_End =(LR_3D_Start*3);
#endif

        hdmirxset(Reg_PG3DRSt_18F, 0xFF, (unsigned char) ((LR_3D_Start & 0x0FF)));
        hdmirxset(Reg_PG3DRStEd_190, 0x0F, (unsigned char) ((LR_3D_Start & 0xF00)>>8));
        hdmirxset(Reg_PG3DRStEd_190, 0xF0, (unsigned char) ((LR_3D_End & 0x00F)<<4));
        hdmirxset(Reg_PG3DREd_191, 0xFF, (unsigned char) ((LR_3D_End & 0xFF0)>>4));

        VSDB_DEBUG_PRINTF(("\nv_total = %X or %d\n",(int)  (v_total), (int)  (v_total)));
        VSDB_DEBUG_PRINTF(("Reg_PGVTotal_19D = %X\n",(int)  (hdmirxrd(Reg_PGVTotal_19D))));
        VSDB_DEBUG_PRINTF(("Reg_PGVTotal_19C = %X\n",(int)  (hdmirxrd(Reg_PGVTotal_19C))));
        VSDB_DEBUG_PRINTF(("\nv_act_start = %X or %d\n",(int)  (v_act_start),(int)  (v_act_start)));
        VSDB_DEBUG_PRINTF(("Reg_PGVActSt_192 = %X\n",(int)  (hdmirxrd(Reg_PGVActSt_192))));
        VSDB_DEBUG_PRINTF(("Reg_PGVActSt_193 = %X\n",(int)  (hdmirxrd(Reg_PGVActSt_193))));
        VSDB_DEBUG_PRINTF(("\nv_act_end = %X or %d\n",(int)  (v_act_end),(int)  (v_act_end)));
        VSDB_DEBUG_PRINTF(("Reg_PGVActEd_193 = %X\n",(int)  (hdmirxrd(Reg_PGVActEd_193))));
        VSDB_DEBUG_PRINTF(("Reg_PGVActEd_194 = %X\n",(int)  (hdmirxrd(Reg_PGVActEd_194))));
        VSDB_DEBUG_PRINTF(("\nv_sync_end = %X or %d\n",(int)  (v_sync_end),(int)  (v_sync_end)));
        VSDB_DEBUG_PRINTF(("Reg_PGVSyncEd_19F = %X\n",(int)  (hdmirxrd(Reg_PGVSyncEd_19F))));

        VSDB_DEBUG_PRINTF(("LR_3D_Start = %X or %d\n",(int)  (LR_3D_Start),(int)  (LR_3D_Start)));
        VSDB_DEBUG_PRINTF(("Reg_PG3DRSt_18F = %X\n",(int)  (hdmirxrd(Reg_PG3DRSt_18F))));
        VSDB_DEBUG_PRINTF(("Reg_PG3DRStEd_190 = %X\n",(int)  (hdmirxrd(Reg_PG3DRStEd_190))));
        VSDB_DEBUG_PRINTF(("Reg_PG3DREd_191 = %X\n",(int)  (hdmirxrd(Reg_PG3DREd_191))));
        VSDB_DEBUG_PRINTF(("LR_3D_End = %X or %d\n",(int)  (LR_3D_End),(int)  (LR_3D_End)));

        VSDB_DEBUG_PRINTF(("\n\nv_total = %X or %d\n",(int)  (v_total), (int)  (v_total)));
        VSDB_DEBUG_PRINTF(("v_act_start = %X or %d\n",(int)  (v_act_start),(int)  (v_act_start)));
        VSDB_DEBUG_PRINTF(("v_act_end = %X or %d\n",(int)  (v_act_end),(int)  (v_act_end)));
        VSDB_DEBUG_PRINTF(("v_sync_end = %X or %d\n",(int)  (v_sync_end),(int)  (v_sync_end)));
        VSDB_DEBUG_PRINTF(("LR_3D_Start = %X or %d\n",(int)  (LR_3D_Start),(int)  (LR_3D_Start)));
        VSDB_DEBUG_PRINTF(("LR_3D_End = %X or %d\n",(int)  (LR_3D_End),(int)  (LR_3D_End)));

        chgbank(0);
        hdmirxset(REG_RX_066_4_DE3DFrame, 0x10, 0x10);        // Reg66[4] = 1 for enable 3D FP2FS
        hdmirxset(REG_RX_085_5_En3DROut, 0x20, 0x20);            // Reg85[5] = 1 for enable 3DR output


        Dump3DReg();


        // enable output
        HActive  = ((hdmirxrd(0x9F)&0x3F)<<8) + hdmirxrd(0x9E);
        //ChangePicoResolution(HActive,v_2d_Vtotal);
                v_act_bspace = v_act_start - v_act_end;
            }

            if(((it6802data->s_Current3DFr.PB4&0xE0) == HDMI_3DFORMAT_PRESENT) && (!it6802data->DE3DFormat_HDMIFlag))
            {
                it6802data->DE3DFormat_HDMIFlag = TRUE;
            }

            if(((it6802data->s_Current3DFr.PB4&0xE0) == HDMI_3DFORMAT_PRESENT) && (it6802data->DE3DFormat_HDMIFlag))
            {
                if(((it6802data->s_Current3DFr.PB5&0xF0) == FRAME_PACKING) && (!it6802data->FramePacking_Flag))
                {
                    it6802data->FramePacking_Flag   = TRUE;
                    it6802data->TopAndBottom_Flag   = FALSE;
                    it6802data->SideBySide_Flag     = FALSE;
                    it6802data->oldVIC              = 0;
                }

                if(((it6802data->s_Current3DFr.PB5&0xF0) == FRAME_PACKING) && (it6802data->FramePacking_Flag))
                {
                    it6802data->newVIC = it6802data->s_Current3DFr.VIC;
                    if(it6802data->newVIC != it6802data->oldVIC)
                    {
                        if((it6802data->s_Current3DFr.VIC == 0x3c) || (it6802data->s_Current3DFr.VIC ==0x3e) || (it6802data->s_Current3DFr.VIC == 0x13) ||
                           (it6802data->s_Current3DFr.VIC == 0x04) ||(it6802data->s_Current3DFr.VIC ==0x20) || (it6802data->s_Current3DFr.VIC == 0x22))
                           //(it6802data->s_Current3DFr.VIC == 0x05) ||(it6802data->s_Current3DFr.VIC == 0x14) // 1080i@50&60Hz not supported for frame packing
                        {
                            it6802data->de3dframe_config.NumActiveBlankLines  =  (unsigned char)v_act_bspace;
                            it6802data->de3dframe_config.Format = VERT_PACKED_FULL; // Type of 3D source format is FRAME_PACKING(VERT_PACKED_FULL)

                            #ifdef DEBUG_MODE_3D
                            dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is FRAME_PACKING\n");
                   #else
                VSDB_DEBUG_PRINTF(("IT680x - HDMI_3DFORMAT is FRAME_PACKING\n"));

                            #endif
                        }
                        else
                        {
                            it6802data->de3dframe_config.Format    =  6; // Type of 3D source format is UNDEFINED_FORMAT

                            #ifdef DEBUG_MODE_3D
                            dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is UNDEFINED_FORMAT\n");
                            #endif
                        }
                            #ifdef DEBUG_MODE_3D
                        dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is FRAME_PACKING call detect3D_Port_3D_On( )\n");
                #endif
                        //detect3D_Port_3D_On(&it6802data->de3dframe_config);  //ralph
                        //HDMI_DumpDE3DFrameInfo(&it6802data->s_Current3DFr);
                        it6802data->oldVIC = it6802data->newVIC;
                    }
                }

                if(((it6802data->s_Current3DFr.PB5&0xF0) == TOP_AND_BOTTOM) && (!it6802data->TopAndBottom_Flag))
                {
                    if((it6802data->s_Current3DFr.VIC == 0x3c) || (it6802data->s_Current3DFr.VIC ==0x3e) || (it6802data->s_Current3DFr.VIC == 0x13) || (it6802data->s_Current3DFr.VIC == 0x04) || (it6802data->s_Current3DFr.VIC == 0x05) ||
                       (it6802data->s_Current3DFr.VIC == 0x14) || (it6802data->s_Current3DFr.VIC ==0x20) || (it6802data->s_Current3DFr.VIC == 0x22) || (it6802data->s_Current3DFr.VIC == 0x1f) || (it6802data->s_Current3DFr.VIC == 0x10))
                    {
                        it6802data->de3dframe_config.Format   =  VERT_PACKED_HALF; // Type of 3D source format is TOP_AND_BOTTOM(VERT_PACKED_HALF)

                        #ifdef DEBUG_MODE_3D
                        dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is TOP_AND_BOTTOM\n");
               #else
                VSDB_DEBUG_PRINTF(("IT680x - HDMI_3DFORMAT is TOP_AND_BOTTOM\n"));
                        #endif
                    }
                    else
                    {
                        it6802data->de3dframe_config.Format   =  6; // Type of 3D source format is UNDEFINED_FORMAT

                        #ifdef DEBUG_MODE_3D
                        dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is UNDEFINED_FORMAT\n");
                        #endif
                    }

                    //detect3D_Port_3D_On(&it6802data->de3dframe_config);  //ralph
                    //HDMI_DumpDE3DFrameInfo(&it6802data->s_Current3DFr);

                    it6802data->FramePacking_Flag   = FALSE;
                    it6802data->TopAndBottom_Flag   = TRUE;
                    it6802data->SideBySide_Flag     = FALSE;
                }

                if(((it6802data->s_Current3DFr.PB5&0xF0) == SIDE_BY_SIDE) && (!it6802data->SideBySide_Flag))
                {
                    if((it6802data->s_Current3DFr.VIC == 0x3c) || (it6802data->s_Current3DFr.VIC ==0x3e) || (it6802data->s_Current3DFr.VIC == 0x13) || (it6802data->s_Current3DFr.VIC == 0x04) || (it6802data->s_Current3DFr.VIC == 0x05) ||
                       (it6802data->s_Current3DFr.VIC == 0x14) || (it6802data->s_Current3DFr.VIC ==0x20) || (it6802data->s_Current3DFr.VIC == 0x22) || (it6802data->s_Current3DFr.VIC == 0x1f) || (it6802data->s_Current3DFr.VIC == 0x10))
                    {
                        it6802data->de3dframe_config.Format   =  HORIZ_PACKED_HALF; // Type of 3D source format is SIDE_BY_SIDE(HORIZ_PACKED_HALF)

                        #ifdef DEBUG_MODE_3D
                        dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is SIDE_BY_SIDE\n");
               #else
                VSDB_DEBUG_PRINTF(("IT680x - HDMI_3DFORMAT is SIDE_BY_SIDE\n"));
                        #endif
                    }
                    else
                    {
                        it6802data->de3dframe_config.Format   =  6; // Type of 3D source format is UNDEFINED_FORMAT

                        #ifdef DEBUG_MODE_3D
                        dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is UNDEFINED_FORMAT\n");
                        #endif
                    }

                    //detect3D_Port_3D_On(&it6802data->de3dframe_config);  //ralph
                    //HDMI_DumpDE3DFrameInfo(&it6802data->s_Current3DFr);

                    it6802data->FramePacking_Flag   = FALSE;
                    it6802data->TopAndBottom_Flag   = FALSE;
                    it6802data->SideBySide_Flag     = TRUE;
                }

                #ifdef DEBUG_MODE_3D
                dbmsg_trace(DBM_3D,"\nIT680x - HDMI_3D_SourceConfiguration:\n");
                dbmsg_ftrace(DBM_3D,"        Format                   = %X\n",(int) it6802data->de3dframe_config.Format);
                dbmsg_ftrace(DBM_3D,"        LR_Reference             = %X\n",(int) it6802data->de3dframe_config.LR_Reference);
                dbmsg_ftrace(DBM_3D,"        FrameDominance           = %X\n",(int) it6802data->de3dframe_config.FrameDominance);
                dbmsg_ftrace(DBM_3D,"        LR_Encoding              = %X\n",(int) it6802data->de3dframe_config.LR_Encoding);
                dbmsg_ftrace(DBM_3D,"        TB_Reference             = %X\n",(int) it6802data->de3dframe_config.TB_Reference);
                dbmsg_ftrace(DBM_3D,"        OE_Reference             = %X\n",(int) it6802data->de3dframe_config.OE_Reference);
                dbmsg_ftrace(DBM_3D,"        NumActiveBlankLines      = %X\n",(int) it6802data->de3dframe_config.NumActiveBlankLines);
                dbmsg_ftrace(DBM_3D,"        NumberOfEncodedLines     = %X\n",(int) it6802data->de3dframe_config.NumberOfEncodedLines);
                dbmsg_ftrace(DBM_3D,"        LeftEncodedLineLocation  = %X\n",(int) it6802data->de3dframe_config.LeftEncodedLineLocation);
                dbmsg_ftrace(DBM_3D,"        RightEncodedLineLocation = %X\n",(int) it6802data->de3dframe_config.RightEncodedLineLocation);
                dbmsg_ftrace(DBM_3D,"        BlankingColor            = %X\n",(int) it6802data->de3dframe_config.BlankingColor );

        #else
            VSDB_DEBUG_PRINTF(("\nIT680x - HDMI_3D_SourceConfiguration:\n"));
            VSDB_DEBUG_PRINTF(("        Format                   = %X\n",(int) it6802data->de3dframe_config.Format));
            VSDB_DEBUG_PRINTF(("        LR_Reference             = %X\n",(int) it6802data->de3dframe_config.LR_Reference));
            VSDB_DEBUG_PRINTF(("        FrameDominance           = %X\n",(int) it6802data->de3dframe_config.FrameDominance));
            VSDB_DEBUG_PRINTF(("        LR_Encoding              = %X\n",(int) it6802data->de3dframe_config.LR_Encoding));
            VSDB_DEBUG_PRINTF(("        TB_Reference             = %X\n",(int) it6802data->de3dframe_config.TB_Reference));
            VSDB_DEBUG_PRINTF(("        OE_Reference             = %X\n",(int) it6802data->de3dframe_config.OE_Reference));
            VSDB_DEBUG_PRINTF(("        NumActiveBlankLines      = %X\n",(int) it6802data->de3dframe_config.NumActiveBlankLines));
            VSDB_DEBUG_PRINTF(("        NumberOfEncodedLines     = %X\n",(int) it6802data->de3dframe_config.NumberOfEncodedLines));
            VSDB_DEBUG_PRINTF(("        LeftEncodedLineLocation  = %X\n",(int) it6802data->de3dframe_config.LeftEncodedLineLocation));
            VSDB_DEBUG_PRINTF(("        RightEncodedLineLocation = %X\n",(int) it6802data->de3dframe_config.RightEncodedLineLocation));
            VSDB_DEBUG_PRINTF(("        BlankingColor            = %X\n",(int) it6802data->de3dframe_config.BlankingColor ));
                #endif

                return TRUE;
            }
        }

        if(it6802data->DE3DFormat_HDMIFlag)// 3D InfoFrame Packet Type is not valid
        {
            #ifdef DEBUG_MODE_3D
            dbmsg_trace(DBM_3D,"IT680x - HDMI_3DFORMAT is OFF\n");
            #endif

//ralph
//detect3D_Port_3D_Off();
//mbSend( detect3DMbxID, D3DMSG_STATE_PORT_2D, -1, 0, FALSE, 0);
//dbmsg_ftrace( DBM_3D, "detect3D_Port_3D_Off: Current state=%s\n", detect3DStateStringTable[detect3DState]);

            it6802data->DE3DFormat_HDMIFlag = FALSE;
            it6802data->FramePacking_Flag   = FALSE;
            it6802data->TopAndBottom_Flag   = FALSE;
            it6802data->SideBySide_Flag     = FALSE;
        }

        /******************************  3D integration  *************************************/
    }
    else
    {

    //it6802data->f_de3dframe_hdmi = FALSE;
    hdmirxwr(REG_RX_06A, 0x82);
    hdmirxset(REG_RX_066_4_DE3DFrame, 0x10, 0x00);        // Reg66[4] = 0 for disable 3D FP2FS
    hdmirxset(REG_RX_085_5_En3DROut, 0x20, 0x00);            // Reg85[5] = 0 for disable 3DR output

    }
    return FALSE;
}
#endif

//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure Table
void IT6802ChangeTTLVideoOutputMode(void)
{
    //for test video output format  only !!!
    unsigned char i;
    struct it6802_dev_data *it6802data = get_it6802_dev_data();


    if(it6802data->m_VidOutConfigMode<eVOMreserve)
    it6802data->m_VidOutConfigMode++;
    else
    it6802data->m_VidOutConfigMode=0;

    i=it6802data->m_VidOutConfigMode;


    IT6802_VideoOutputConfigure_Init(it6802data,i);
    IT6802_VideoOutputModeSet(it6802data);
//FIX_ID_003 xxxxx
}



// disable -> //FIX_ID_005 xxxxx    //Add CsigBus Event Handler
// disable -> void IT6802MSCCmdTest(unsigned char ucCmd)
// disable -> {
// disable ->
// disable ->    struct it6802_dev_data *it6802data = get_it6802_dev_data();
// disable ->
// disable ->    switch(ucCmd)
// disable ->    {
// disable ->
// disable -> //    case B_Set_DCAP_RDY:            //    0x80 // bit5 B_3DSupporpt
// disable -> //    case B_PATH_EN:                //    0x40 // bit5 B_3DSupporpt
// disable -> //    case B_HPD:                    //    0x20 // bit4 B_3DSupporpt
// disable ->    case B_DevCapChange:        //    0x10 // bit3 B_3DSupporpt
// disable ->    case B_3DSupporpt:            //    0x04 // bit2 B_3DSupporpt
// disable ->    case B_ReadDevCap:            //    0x02 // bit1 B_ReadDevCap
// disable ->    case B_DiscoveryDone:        //    0x01 // bit0 B_DiscoveryDone
// disable ->            it6802data ->CsigBusIntEvent |= ucCmd;
// disable ->            break;
// disable ->    }
// disable -> //FIX_ID_005 xxxxx
// disable -> }

void IT680x_StandbyMode(unsigned char ucstate)//ITE_151021
{
    struct it6802_dev_data *it6802data = get_it6802_dev_data();
        //ON => Standby  OFF => Resume
        delay1ms(500);
        if(ucstate)
        {
            hdmirxwr(REG_RX_010, 0xC0);
            hdmirxwr(REG_RX_013, 0xF8);
            hdmirxwr(REG_RX_014, 0xFF);
            hdmirxwr(REG_RX_015, 0xFF);
            hdmirxwr(REG_RX_01A, 0xF8);
            hdmirxwr(REG_RX_01B, 0xFF);
            hdmirxwr(REG_RX_01C, 0xFF);
            hdmirxwr(REG_RX_052, 0x3F);
            hdmirxwr(REG_RX_053, 0x3F);
            hdmirxwr(REG_RX_00D, 0x00);
            hdmirxwr(REG_RX_00E, 0x80);
            IT6802_DEBUG_INT_PRINTF(("StandbyMode On \n"));
        }else{
            IT6802_Rst(&it6802DEV);
            #ifdef    _SUPPORT_HDMI_REPEATER_    //20090916
                IT680x_Init_HDMIRepeater(it6802data, TRUE);
            #endif //_SUPPORT_HDCP_REPEATER_
            #ifdef Enable_IT6802_CEC
                IT6802_CECInit();
            #endif
        it6802PortSelect(0);
        }
}






//=============================================================================
void  Dump_IT680xReg(void)//max7088
{

#if 1
    ushort    i,j;
    //BYTE reg;
    //BYTE bank;
    BYTE ucData;

//    printf(" IT680x\n");
//    printf("\n 11111:");
//    printf("\n        ===================================================\r");
    printf("\n       ");
    for(j = 0; j < 16; j++)
    {
        printf(" %02X",(int) j);
        if((j == 3)||(j==7)||(j==11))
        {
                printf(" :");
        }
    }
    printf("\n        =====================================================\n");

chgbank(0);

    for(i = 0; i < 0x100; i+=16)
    {
        printf("[%03X]  ",i);
        for(j = 0; j < 16; j++)
        {
            ucData = hdmirxrd((BYTE)((i+j)&0xFF));
            printf(" %02X",(int) ucData);
            if((j == 3)||(j==7)||(j==11))
            {
                printf(" :");
            }
        }
        printf("\n");
        if((i % 0x40) == 0x30)
        {
    printf("\n        =====================================================\n");
        }
    }

chgbank(1);
    for(i = 0x0; i < 0x100; i+=16)
    {
        printf("[%03X]  ",i+0x100);
        for(j = 0; j < 16; j++)
        {
            ucData = hdmirxrd((BYTE)((i+j)&0xFF));
            printf(" %02X",(int) ucData);
            if((j == 3)||(j==7)||(j==11))
            {
                printf(" :");
            }
        }
        printf("\n");
        if((i % 0x40) == 0x30)
        {
            printf("\n        =====================================================\n");
        }
    }

chgbank(2);
    for(i = 0x0; i < 0x100; i+=16)
    {
        printf("[%03X]  ",i+0x200);
        for(j = 0; j < 16; j++)
        {
            ucData = hdmirxrd((BYTE)((i+j)&0xFF));
            printf(" %02X",(int) ucData);
            if((j == 3)||(j==7)||(j==11))
            {
                printf(" :");
            }
        }
        printf("\n");
        if((i % 0x40) == 0x30)
        {
            printf("\n        =====================================================\n");
        }
    }
chgbank(0);
#endif
}
// disable ->
// disable -> void debugxxx(void)
// disable -> {
// disable -> #if 0
// disable ->
// disable -> #else
// disable ->
// disable ->    unsigned char i;
// disable ->    unsigned char Receive_Err;
// disable ->    unsigned char Video_Err;
// disable ->    unsigned char Code_Err;
// disable ->    unsigned char Pkt_Err;
// disable ->    unsigned char CrtErr;
// disable ->    unsigned char ucStatus;
// disable ->
// disable ->    struct it6802_dev_data *it6802data = get_it6802_dev_data();
// disable ->
// disable ->
// disable ->    IT6802_AFE_Rst();
// disable ->
// disable ->    if(it6802data->m_ucCurrentHDMIPort == F_PORT_SEL_1)
// disable ->    {
// disable ->        hdmirxwr(REG_RX_0B7,Receive_Err);
// disable ->        hdmirxwr(REG_RX_0B8,Video_Err);
// disable ->        hdmirxwr(REG_RX_0B9,Code_Err);
// disable ->        hdmirxwr(REG_RX_0BA,Pkt_Err);
// disable ->        hdmirxwr(REG_RX_0BB,CrtErr);
// disable ->    }
// disable ->    else
// disable ->    {
// disable ->        hdmirxwr(REG_RX_0B2,Receive_Err);
// disable ->        hdmirxwr(REG_RX_0B3,Video_Err);
// disable ->        hdmirxwr(REG_RX_0B4,Code_Err);
// disable ->        hdmirxwr(REG_RX_0B5,Pkt_Err);
// disable ->        hdmirxwr(REG_RX_0B6,CrtErr);
// disable ->
// disable ->    }
// disable ->
// disable ->    EQ_DEBUG_PRINTF(("        [Status] "));
// disable ->    EQ_DEBUG_PRINTF(("[Receive_Err] "));
// disable ->    EQ_DEBUG_PRINTF(("[Video_Err] "));
// disable ->    EQ_DEBUG_PRINTF(("[Code_Err] "));
// disable ->    EQ_DEBUG_PRINTF(("[Pkt_Err] "));
// disable ->    EQ_DEBUG_PRINTF(("[CrtErr]\n"));
// disable ->
// disable ->
// disable ->    for(i=0;i<30;i++)
// disable ->    {
// disable ->
// disable ->        if(it6802data->m_ucCurrentHDMIPort == F_PORT_SEL_1)
// disable ->        {
// disable ->            ucStatus = hdmirxrd(REG_RX_P1_SYS_STATUS);
// disable ->            // !!! check ECC error register  !!!
// disable ->            Receive_Err = hdmirxrd(REG_RX_0B7);
// disable ->            Video_Err = hdmirxrd(REG_RX_0B8)&0xE0;
// disable ->            Code_Err = hdmirxrd(REG_RX_0B9);
// disable ->            Pkt_Err = hdmirxrd(REG_RX_0BA);
// disable ->            CrtErr = hdmirxrd(REG_RX_0BB);
// disable ->
// disable ->            hdmirxwr(REG_RX_0B7,Receive_Err);
// disable ->            hdmirxwr(REG_RX_0B8,Video_Err);
// disable ->            hdmirxwr(REG_RX_0B9,Code_Err);
// disable ->            hdmirxwr(REG_RX_0BA,Pkt_Err);
// disable ->            hdmirxwr(REG_RX_0BB,CrtErr);
// disable ->
// disable ->        }
// disable ->        else
// disable ->        {
// disable ->            ucStatus = hdmirxrd(REG_RX_P0_SYS_STATUS);
// disable ->            // !!! check ECC error register  !!!
// disable ->            Receive_Err = hdmirxrd(REG_RX_0B2);
// disable ->            Video_Err = hdmirxrd(REG_RX_0B3)&0xE0;
// disable ->            Code_Err = hdmirxrd(REG_RX_0B4);
// disable ->            Pkt_Err = hdmirxrd(REG_RX_0B5);
// disable ->            CrtErr = hdmirxrd(REG_RX_0B6);
// disable ->
// disable ->            hdmirxwr(REG_RX_0B2,Receive_Err);
// disable ->            hdmirxwr(REG_RX_0B3,Video_Err);
// disable ->            hdmirxwr(REG_RX_0B4,Code_Err);
// disable ->            hdmirxwr(REG_RX_0B5,Pkt_Err);
// disable ->            hdmirxwr(REG_RX_0B6,CrtErr);
// disable ->
// disable ->        }
// disable ->
// disable ->
// disable ->        EQ_DEBUG_PRINTF((" [%02X] ",(int) ucStatus));        EQ_DEBUG_PRINTF(("      "));
// disable ->        EQ_DEBUG_PRINTF((" [%02X] ",(int) Receive_Err));    EQ_DEBUG_PRINTF(("      "));
// disable ->        EQ_DEBUG_PRINTF((" [%02X] ",(int) Video_Err));        EQ_DEBUG_PRINTF(("      "));
// disable ->        EQ_DEBUG_PRINTF((" [%02X] ",(int) Code_Err));        EQ_DEBUG_PRINTF(("      "));
// disable ->        EQ_DEBUG_PRINTF((" [%02X] ",(int) Pkt_Err));        EQ_DEBUG_PRINTF(("  "));
// disable ->        EQ_DEBUG_PRINTF((" [%02X]\n",(int) CrtErr));
// disable ->
// disable ->
// disable ->    }
// disable ->    delay1ms(500);
// disable -> #endif
// disable -> }
// disable ->
// disable ->
// disable -> void debugEQ(unsigned char ucCmd)
// disable -> {
// disable ->
// disable ->    unsigned char i;
// disable ->    unsigned char j=0;
// disable ->    unsigned char EQTable[8]={0xFF,0x9F,0x8F,0x0F,0x07,0x03,0x01,0x00};
// disable ->    struct it6802_dev_data *it6802data = get_it6802_dev_data();
// disable ->
// disable ->    switch(ucCmd)
// disable ->    {
// disable ->        case 0:
// disable ->            {
// disable -> //                DisableOverWriteRS(0);
// disable -> //                DisableOverWriteRS(1);
// disable ->
// disable ->                #ifdef _SUPPORT_EQ_ADJUST_
// disable ->
// disable ->                if(it6802data->m_ucCurrentHDMIPort == F_PORT_SEL_0)
// disable ->                    HDMIStartEQDetect(&(it6802data->EQPort[F_PORT_SEL_0]));
// disable ->                else
// disable ->                    HDMIStartEQDetect(&(it6802data->EQPort[F_PORT_SEL_1]));
// disable ->                #endif
// disable ->
// disable ->                break;
// disable ->            }
// disable ->
// disable ->        case 1:
// disable ->            {
// disable ->                    #ifdef _SUPPORT_EQ_ADJUST_
// disable ->                    if(it6802data->EQPort[F_PORT_SEL_0].f_manualEQadjust==TRUE)
// disable ->                    {
// disable ->                        //TickCountPrint();
// disable ->                        HDMIAdjustEQ(&(it6802data->EQPort[F_PORT_SEL_0]));    // for port 0
// disable ->                    }
// disable ->
// disable ->                    if(it6802data->EQPort[F_PORT_SEL_1].f_manualEQadjust==TRUE)
// disable ->                        HDMIAdjustEQ(&(it6802data->EQPort[F_PORT_SEL_1]));    // for port 1
// disable ->                    #endif
// disable ->
// disable ->                break;
// disable ->            }
// disable ->
// disable ->        case 2:
// disable ->            {
// disable ->                for(i=0;i<8;i++)
// disable ->                {
// disable ->
// disable ->                    if(it6802data->m_ucCurrentHDMIPort == F_PORT_SEL_0)
// disable ->                    {
// disable ->                        hdmirxset(REG_RX_026, 0x20, 0x20);    //07-04 add for adjust EQ
// disable ->                        hdmirxwr(REG_RX_027,EQTable[i]);
// disable ->                        EQ_DEBUG_PRINTF(("Port=%X ,EQValue Reg027 = %X\n",(int) it6802data->m_ucCurrentHDMIPort,(int) hdmirxrd(REG_RX_027)));
// disable ->                    }
// disable ->                    else
// disable ->                    {
// disable ->                        hdmirxset(REG_RX_03E, 0x20, 0x20);    //07-04 add for adjust EQ
// disable ->                        hdmirxwr(REG_RX_03F,EQTable[i]);
// disable ->                        EQ_DEBUG_PRINTF(("Port=%X ,EQValue Reg03F = %X\n",(int) it6802data->m_ucCurrentHDMIPort,(int) hdmirxrd(REG_RX_03F)));
// disable ->                    }
// disable ->
// disable ->                    debugxxx();
// disable ->
// disable ->                    //if (kbhit())
// disable ->                    break;
// disable ->                }
// disable ->
// disable ->                get_vid_info();
// disable ->                show_vid_info();
// disable ->
// disable ->                if(it6802data->m_ucCurrentHDMIPort == F_PORT_SEL_0)
// disable ->                {
// disable -> #ifdef _SUPPORT_AUTO_EQ_
// disable -> //xxxxx
// disable ->                    DisableOverWriteRS(0);
// disable ->                    TMDSCheck(0);
// disable -> //xxxxx
// disable -> #endif
// disable ->                }
// disable ->                else
// disable ->                {
// disable -> #ifdef _SUPPORT_AUTO_EQ_
// disable -> //xxxxx
// disable ->                    DisableOverWriteRS(1);
// disable ->                    TMDSCheck(1);
// disable -> //xxxxx
// disable -> #endif
// disable ->                }
// disable ->                break;
// disable ->            }
// disable ->
// disable ->
// disable ->    }
// disable -> }


