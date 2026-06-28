///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it680X.h>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/
#ifndef _IT680X_H_
#define _IT680X_H_


#ifndef _IT680x_
#define _IT680x_
#endif

// #define _MCU_ 1

#ifdef _MCU_
    #define _CODE code
    #include "it68013CX_MxLrx.h"
    #include "it68013CX_it680X_regs.h"
    #include "it68013CX_typedef.h"
    #include "it68013CX_Utility.h"
#else
    #define _CODE const
    #include "it68013CX_MxLrx.h"
    // #include "it68013CX_common.h"
    // #include "i2c.h"
    // #include "int.h"
    // #include "digrxs.h"
    // #include "rta_tsk.h"
    // #include "rta_sem.h"
    // #include "rta_pub.h"
    // #include "ddp442x_rtos_include.h"
    // #include "sysmon.h"
    // #include "mailbox.h"
    // #include "tmr.h"
    // #include "dbmessage.h"
    // #include "gpiofunction.h"
    // #include "datapath.h"
    // #include "src_3d.h"
    // #include "detect3D.h"
    // #include "defines.h"
    // #include "digcontrol.h"
#endif


//FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
#if defined(_IT6803_)
#pragma message ("defined ENABLE_IT6803")
#endif
//FIX_ID_016 xxxxx

/*****************************************************************************/
/* Local Defines    **********************************************************/
/*****************************************************************************/
#define HDMIRX_MAX_KSV 8

//#define DISABLE_HDMI_CSC
#define Enable_Vendor_Specific_packet
//#define EN_DUAL_PIXEL_MODE    //2013-0520


//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure setting
// 0 eRGB444_SDR=0,
// 1    eYUV444_SDR,
// 2    eRGB444_DDR,
// 3    eYUV444_DDR,
// 4    eYUV422_Emb_Sync_SDR,
// 5    eYUV422_Emb_Sync_DDR,
// 6    eYUV422_Sep_Sync_SDR,
// 7    eYUV422_Sep_Sync_DDR,
// 8    eCCIR656_Emb_Sync_SDR,
// 9    eCCIR656_Emb_Sync_DDR,
// 10 eCCIR656_Sep_Sync_SDR,
// 11 eCCIR656_Sep_Sync_DDR,
// 12 eRGB444_Half_Bus,
// 13 eYUV444_Half_Bus,
// 14 eBTA1004_SDR,
// 15 eBTA1004_DDR
//06-27 disable --> #define HDMIRX_OUTPUT_VID_MODE (F_MODE_EN_UDFILT | F_MODE_RGB444)
// #define HDMIRX_OUTPUT_VID_MODE eRGB444_DDR
#define HDMIRX_OUTPUT_VID_MODE eYUV422_Emb_Sync_SDR

//FIX_ID_003 xxxxx


//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//xxxxx 2014-0421 modify 50 to 100
#define MS_LOOP                 100
//xxxxx 2014-0421
//FIX_ID_033 xxxxx

#define MS_TimeOut(x) (x+1)

//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
#define VSATE_CONFIRM_SCDT_COUNT        MS_TimeOut(100)
//FIX_ID_033 xxxxx

#ifdef _FIX_ID_028_
//xxxxx 2014-0417
//FIX_ID_028 xxxxx //For Debug Audio error with S2
#define AUDIO_READY_TIMEOUT                     MS_TimeOut(0)    // change 100ms to 0 for speed up audio on
//FIX_ID_028 xxxxx
//xxxxx 2014-0417
#else
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
#define AUDIO_READY_TIMEOUT                     MS_TimeOut(200)
//FIX_ID_023 xxxxx
#endif
#define AUDIO_MONITOR_TIMEOUT              MS_TimeOut(150)

#define SCDT_OFF_TIMEOUT                      MS_TimeOut(20)        //100 x MS_LOOP = 5000 ms = 5 sec
#define ECC_TIMEOUT                              MS_TimeOut(20)
#define DESKEW_TIMEOUT                        MS_TimeOut(20)




#define  HDCPIntKey   FALSE   //TRUE: Internal HDCP Key, FALSE: SIPROM

#define  VCLK_INV    0
#define  VCLK_DLY    0
#define  EnMultiSeg     TRUE
#define  EnIntEDID      TRUE

//Discovery
#define  CsigBusFloatAdj    FALSE
#define EQFAILCNT 2



//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#define EQRETRYFAILCNT 1    // for EQ interrupt
#define RCLKVALUE 12            // for show TMDS and Pixel Clk

#if 0   //oec20260628
#define TMDSCLKVALUE 160    // for TMDS > 160 then set RS to 00, otherwise set to 3F
#define TMDSCLKVALUE_1080P 160    // for TMDS > 160 then set RS to 00, otherwise set to 3F
#else
#define TMDSCLKVALUE 250    // for TMDS > 160 then set RS to 00, otherwise set to 3F
#define TMDSCLKVALUE_1080P 250    // for TMDS > 160 then set RS to 00, otherwise set
#endif

#define TMDSCLKVALUE_480P 35
#define TMDSCLKVALUE_MxL_ER1 90
#define JUDGE_ER1_VALUE 90

//FIX_ID_001 xxxxx


//FIX_ID_021 xxxxx        //To use CP_100ms for CsigBus_100ms and CEC_100m
//FIX_ID_004 xxxxx        //Add 100ms calibration for CsigBus
//#ifndef _SelectExtCrystalForCsigBus_
#define _RCLK_FREQ_20M  FALSE
//#endif
//FIX_ID_004 xxxxx
//FIX_ID_021 xxxxx


//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
//FIX_ID_033 xxxxx  //Fine-tune EQ Adjust function for HDCP receiver and repeater mode
//FIX_ID_005 xxxx    //Wait for video on then read MxL device capability
#define MAX_CsigBus_WAITNO        (300/MS_LOOP)        // 250ms
#define MAX_PATHEN_WAITNO    (700/MS_LOOP)        // 700ms
#define MAX_BUSY_WAITNO        (2500/MS_LOOP)        // 150ms
#define MAX_DISCOVERY_WAITNO    (100/MS_LOOP)        // 100ms
//FIX_ID_005 xxxx
//FIX_ID_033 xxxxx
//FIX_ID_037 xxxxx

//FIX_ID_014 xxxx
#define MAX_TMDS_WAITNO        (350/MS_LOOP)        // 400ms
#define MAX_HDCP_WAITNO        (100/MS_LOOP)        // 150ms
//FIX_ID_014 xxxx
//FIX_ID_018    xxxxx    //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
#define RENEW_WAKEUP        (12000/MS_LOOP)
#define IGNORE_WAKEUP        (1000/MS_LOOP)
#define TOGGLE_WAKEUP        (4000/MS_LOOP)
#define CDSENSE_WAKEUP        (500/MS_LOOP)
//FIX_ID_018    xxxxx

//ITE_150302  Chroma A222908 issue
#define CsigBus_CLK_MODE_TIMEOUT (3500/MS_LOOP)
//

#define DEFAULT_EQVALUE 0x1F

//FIX_ID_052 DCLK DELAY DIFF FOR CSC ON AND OFF
#define DCLK_DELAY_CSC_ON  0x02
#define DCLK_DELAY_CSC_OFF 0x00
//~FIX_ID_052



#define F_MODE_RGB24  0
#define F_MODE_RGB444  0
#define F_MODE_YUV422 1
#define F_MODE_YUV444 2
//FIX_ID_044  add YCbCr420 supporting
#define F_MODE_YUV420 3 // CEA-861F defined YUV420 3
//~FIX_ID_044
#define F_MODE_CLRMOD_MASK 3
#define F_MODE_ITU709  (1<<4)
#define F_MODE_ITU601  0
#define F_MODE_0_255   0
#define F_MODE_16_235  (1<<5)
#define F_MODE_EN_UDFILT (1<<6)
#define F_MODE_EN_DITHER (1<<7)


#define FAIL            -1
#define ABORT           -2

#define HIGH            1
#define LOW                0

#define CD8BIT            4
#define CD10BIT            5
#define CD12BIT            6
#define CD16BIT            7

#define OUT8B           0
#define OUT10B          1
#define OUT12B          2

#define RGB444            0
#define YCbCr422        1
#define YCbCr444        2

#define NORM            0
#define FAST            1
#define SLOW            2

#define AUD32K            0x3
#define AUD48K            0x2
#define AUD96K            0xA
#define AUD192K            0xE
#define AUD44K            0x0
#define AUD88K            0x8
#define AUD176K            0xC

#define I2S                0
#define SPDIF            1


#define MxL  1
#define HDMI 0



#define SUPPORT_INPUTRGB
#define SUPPORT_INPUTYUV444
#define SUPPORT_INPUTYUV422

#if defined(SUPPORT_INPUTYUV444)|| defined(SUPPORT_INPUTYUV422)
#define SUPPORT_INPUTYUV
#endif


#define SUPPORT_OUTPUTYUV
#define SUPPORT_OUTPUTYUV444
#define SUPPORT_OUTPUTYUV422
//FIX_ID_044  add YCbCr420 supporting
#define SUPPORT_OUTPUTYUV420
//~FIX_ID_044

#if (defined(SUPPORT_OUTPUTYUV444))||(defined(SUPPORT_OUTPUTYUV422))
#define SUPPORT_OUTPUTYUV
#endif

#define F_PORT_SEL_0      0
#define F_PORT_SEL_1      1


//FIX_ID_014 xxxxx
    #define B_PORT1_TimingChgEvent    0x40
    #define B_PORT1_TMDSEvent    0x20
    #define B_PORT1_Waiting        0x10
    #define B_PORT0_TimingChgEvent    0x04
    #define B_PORT0_TMDSEvent    0x02
    #define B_PORT0_Waiting        0x01
//FIX_ID_014 xxxxx

//////////////////////////////////////////////////////////////////////////////////////////////
// ENUM
//////////////////////////////////////////////////////////////////////////////////////////////


//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure setting
typedef enum _Video_Output_Configure
{
    eRGB444_SDR=0,
    eYUV444_SDR,
    eRGB444_DDR,
    eYUV444_DDR,
    eYUV422_Emb_Sync_SDR,
    eYUV422_Emb_Sync_DDR,
    eYUV422_Sep_Sync_SDR,
    eYUV422_Sep_Sync_DDR,
    eCCIR656_Emb_Sync_SDR,
    eCCIR656_Emb_Sync_DDR,
    eCCIR656_Sep_Sync_SDR,
    eCCIR656_Sep_Sync_DDR,
    eRGB444_Half_Bus,
    eYUV444_Half_Bus,
    eBTA1004_SDR,
    eBTA1004_DDR,
    eVOMreserve
}Video_Output_Configure;

typedef enum _Video_OutputDataTrigger_Mode
{
    eSDR=0,
    eHalfPCLKDDR,
    eHalfBusDDR,
    eSDR_BTA1004,
    eDDR_BTA1004
}Video_DataTrigger_Mode;

typedef enum _Video_OutputSync_Mode
{
    eSepSync=0,
    eEmbSync,
    eCCIR656SepSync,
    eCCIR656EmbSync
}Video_OutputSync_Mode;

//FIX_ID_003 xxxxx

typedef enum _Video_State_Type {
    VSTATE_Off=0,
    VSTATE_TerminationOff,
    VSTATE_TerminationOn,
    VSTATE_5VOff,
    VSTATE_SyncWait,
    VSTATE_SWReset,
    VSTATE_SyncChecking,
    VSTATE_HDCPSet,
    VSTATE_HDCP_Reset,
    VSTATE_ModeDetecting,
    VSTATE_VideoOn,
    VSTATE_ColorDetectReset,
    VSTATE_HDMI_OFF,
    VSTATE_Unknown,
    VSTATE_Reserved
} Video_State_Type;


typedef enum _Audio_State_Type {
    ASTATE_AudioOff = 0,
    ASTATE_RequestAudio ,
    ASTATE_ResetAudio,
    ASTATE_WaitForReady,
    ASTATE_AudioOn ,
    ASTATE_Unknown,
    ASTATE_Reserved
} Audio_State_Type;


typedef enum _RxHDCP_State_Type {
    RxHDCP_PwrOff=0,
    RxHDCP_ModeCheck,
    RxHDCP_Receiver,
    RxHDCP_Repeater,
    RxHDCP_SetKSVFifoList,
    RxHDCP_GenVR,
    RxHDCP_WriteVR,
    RxHDCP_Auth_WaitRi,
    RxHDCP_Authenticated,
    RxHDCP_Reserved
} RxHDCP_State_Type;



struct AVI_info
{
    unsigned char ColorMode;
    unsigned char Colorimetry;
    unsigned char ExtendedColorimetry;
    unsigned char RGBQuantizationRange;
    unsigned char YCCQuantizationRange;
    unsigned char VIC;
    //unsigned char PixelRepetition;
};


struct it6802_dev_data
{
    Video_State_Type m_VState;
    Audio_State_Type m_AState;
    RxHDCP_State_Type m_RxHDCPState;
    AUDIO_CAPS m_RxAudioCaps;
    unsigned short m_SWResetTimeOut;
    unsigned short m_VideoCountingTimer;
    unsigned short m_AudioCountingTimer;
    unsigned char m_ucCurrentHDMIPort;
    unsigned char m_bOutputVideoMode;
    unsigned char m_bInputVideoMode;

//FIX_ID_039 xxxxx fix image flick when enable RGB limited / Full range convert
#ifdef _AVOID_REDUNDANCE_CSC_
    unsigned char m_Backup_OutputVideoMode;
    unsigned char m_Backup_InputVideoMode;
#endif
//FIX_ID_039 xxxxx

    unsigned char m_ucSCDTOffCount;
    unsigned char m_ucEccCount_P0;
    unsigned char m_ucEccCount_P1;
    unsigned char m_ucDeskew_P0;
    unsigned char m_ucDeskew_P1;

    SRC_3D_SOURCE_CONFIG de3dframe_config;
    DE3DFRAME s_Current3DFr;

    unsigned char oldVIC;
    unsigned char newVIC;
    unsigned char  f_de3dframe_hdmi;

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
    #ifdef _SUPPORT_EQ_ADJUST_
    struct it6802_eq_data EQPort[2];
    // FIX_ID_043
    unsigned char ToggleCount[2];
    //~FIX_ID_043
    #endif
//FIX_ID_001 xxxxx

    //FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure setting
    Video_Output_Configure m_VidOutConfigMode;
    Video_DataTrigger_Mode m_VidOutDataTrgger;
    Video_OutputSync_Mode m_VidOutSyncMode;
    //FIX_ID_003 xxxxx
//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    unsigned char CsigBusIntEvent;
    unsigned char CsigBusSeqNo;
    unsigned char CsigBusWaitNo;
//FIX_ID_005 xxxxx

    unsigned char CsigBusDcapReadNo;//20150608_FIX_CHROMA

//FIX_ID_014 xxxxx    //Add CsigBus Event Handler
    unsigned char HDMIIntEvent;
    unsigned char HDMIWaitNo[2];
//FIX_ID_014 xxxxx

//FIX_ID_021 xxxxx        //To use CP_100ms for CsigBus_100ms and CEC_100m
// #ifndef _SelectExtCrystalForCsigBus_
//    unsigned long RCLK;
//    unsigned long PCLK;
//
//    int  t10usint;
//    int  t10usflt;
// #endif
//FIX_ID_021 xxxxx

    //AVI_info m_avi;
    unsigned char ColorMode;
    unsigned char Colorimetry;
    unsigned char ExtendedColorimetry;
    unsigned char RGBQuantizationRange;
    unsigned char YCCQuantizationRange;
    unsigned char VIC;

//FIX_ID_034 xxxxx //Add MxL HPD Control by it6802HPDCtrl( )
    unsigned char m_DiscoveryDone;
//FIX_ID_034 xxxxx

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
    //xxxxx 2014-0529 //Manual Content On/Off
    unsigned char m_RAP_ContentOff;
    unsigned char m_HDCP_ContentOff;
    //xxxxx
//FIX_ID_037 xxxxx

    PIXEL_MODE pixelMode;    //Output TTL Pixel mode
    unsigned char GCP_CD;    //Output Color Depth
    unsigned char  DE3DFormat_HDMIFlag:1;
    unsigned char  FramePacking_Flag:1;
    unsigned char  TopAndBottom_Flag:1;
    unsigned char  SideBySide_Flag:1;

    #ifdef _IT6607_GeNPacket_Usage_
    BYTE m_PollingPacket;
    BYTE m_PacketState;
    BYTE m_ACPState;
    BYTE m_GeneralRecPackType;
    BYTE m_GamutPacketRequest:1;
    #endif

//#if(_SUPPORT_HDCP_)
//    //HDCP
//    unsigned char HDCPEnable;
//    HDCPSts_Type Hdcp_state;
//    unsigned int HDCPFireCnt;
//#endif
    // 2015/07/01 added by jjtseng
    // for solving the new AVI interrupt issued by Audio Force MUTE.
    unsigned char prevAVIDB[2] ;

    //CsigBus MSC
    unsigned char MxL_devcap[16];
    unsigned char txmsgdata[2];
    unsigned char rxmsgdata[2];
    unsigned char txscrpad[16];
    unsigned char rxscrpad[16];
    unsigned char RCPTxArray[MAXRCPINDEX];
    unsigned char    RCPhead;
    unsigned char    RCPtail;
    RCPState_Type RCPState;
    RCPResult_Type RCPResult;
//FIX_ID_015    xxxxx peer device no response
    unsigned char RCPCheckResponse;
//FIX_ID_015    xxxxx

//FIX_ID_024    xxxxx Fixed for RCP compliance issue
    unsigned char m_bRCPTimeOut:1;
    unsigned char m_bRCPError:1;
//FIX_ID_024    xxxxx
    unsigned char m_bRxAVmute:1;
    unsigned char m_bVideoOnCountFlag:1;
    unsigned char m_MuteAutoOff:1;
    unsigned char m_bUpHDMIMode:1;
    unsigned char m_bUpHDCPMode:1;
    unsigned char m_NewAVIInfoFrameF:1;
    unsigned char m_NewAUDInfoFrameF:1;
    unsigned char m_HDCPRepeater:1;
    unsigned char m_MuteByPKG:1;
    // pIT680XCallbackList_t    pCallbackFunctionsList;
    unsigned char m_bHDCPrepeater:1;

    //ITE_150302  Chroma A222908 issue
    unsigned char CsigBus_wait_clk_mode;
    unsigned char CsigBus_clockmode_timeout;
    //

    //IT6602 CX/*20230302*/
    unsigned char device_version;//0xC0: IT6602CX, 0xB1: IT6802BX
    //
#ifdef _SUPPORT_HDMI_REPEATER_
    unsigned char m_sinkHPD ;
#endif // _SUPPORT_HDMI_REPEATER_

};
//////////////////////////////////////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////////////////////////////////////


void Cal_oclk( void );
#define CP_MEAN_VALUE        (48696)
#define CP_MAX_DEVIATION    (CP_MEAN_VALUE*50/100)
unsigned char OSCvalueCompare(unsigned long *calibrationValue);
unsigned long CP_OCLK( void );
void IPLLOvw_chk(int port);//ite_180509

unsigned char hdmirxrd( unsigned char RegAddr);
unsigned char hdmirxwr( unsigned char RegAddr,unsigned char DataIn);
unsigned char  hdmirxset( unsigned char  offset, unsigned char  mask, unsigned char  ucdata );
void hdmirxbwr( unsigned char offset, unsigned char byteno, unsigned char *rddata );

unsigned char MxLrxrd( unsigned char offset );
unsigned char MxLrxwr( unsigned char offset, unsigned char ucdata );
unsigned char MxLrxset( unsigned char offset, unsigned char mask, unsigned char ucdata );
void MxLrxbrd( unsigned char offset, unsigned char byteno, unsigned char *rddata );
void MxLrxbwr( unsigned char offset, unsigned char byteno, unsigned char *rddata );
void MxLrx_write_init(struct IT6802_REG_INI _CODE *tdata);

SYS_STATUS EDID_RAM_Write(unsigned char offset,unsigned char byteno,unsigned char *p_data );
unsigned char EDID_RAM_Read(unsigned char offset);
/* IT680x Configuration and Initialization ***********************************/
struct it6802_dev_data* get_it6802_dev_data(void);
void hdimrx_write_init(struct IT6802_REG_INI _CODE *tdata);

//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure setting
void IT6802_VideoOutputConfigure_Init(struct it6802_dev_data *it6802,Video_Output_Configure eVidOutConfig);
//FIX_ID_003 xxxxx

void hdmirx_Var_init(struct it6802_dev_data *it6802);
void IT6802_Rst( struct it6802_dev_data *it6802 );
//FIX_ID_002 xxxxx    Check IT6802 chip version Identify for TogglePolarity and Port 1 Deskew
void IT6802_Identify_Chip( struct it6802_dev_data *it6802 );/*20230302*/


//FIX_ID_016 xxxxx Support Dual Pixel Mode for IT6803 Only
#if defined(_IT6803_)
void IT6803OutputPixelModeSet(unsigned char bSignalPixelMode);
void IT6803JudgeOutputMode(void);
#endif
//FIX_ID_016 xxxxx


/* HDMI RX functions   *********************************************************/
void chgbank(int bank);
unsigned char CheckSCDT(struct it6802_dev_data *it6802);
void WaitingForSCDT(struct it6802_dev_data *it6802);
unsigned char CLKCheck(unsigned char ucPortSel);

//FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
unsigned char  IT6802_IsSelectedPort(unsigned char ucPortSel);
//FIX_ID_009 xxxxx

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_EQ_ADJUST_
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void HDMIStartEQDetect(struct it6802_eq_data *ucEQPort);
void HDMISetEQValue(struct it6802_eq_data *ucEQPort,unsigned char ucIndex);
void HDMISwitchEQstate(struct it6802_eq_data *ucEQPort,unsigned char state);
void HDMICheckSCDTon(struct it6802_eq_data *ucEQPort);
void HDMIPollingErrorCount(struct it6802_eq_data *ucEQPort);
void HDMIJudgeECCvalue(struct it6802_eq_data *ucEQPort);
void HDMIAdjustEQ(struct it6802_eq_data *ucEQPort);
//FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
void JudgeBestEQ(struct it6802_eq_data *ucEQPort);
void StoreEccCount(struct it6802_eq_data *ucEQPort);
//FIX_ID_010 xxxxx
void IT6802VideoCountClr(void);
//-------------------------------------------------------------------------------------------------------
#endif

#ifdef _SUPPORT_AUTO_EQ_
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DisableOverWriteRS (unsigned char ucPortSel);
void AmpValidCheck (unsigned char ucPortSel);
void TogglePolarity (unsigned char ucPortSel);
void TMDSCheck(unsigned char ucPortSel);
void OverWriteAmpValue2EQ (unsigned char ucPortSel);
//-------------------------------------------------------------------------------------------------------
#endif
//FIX_ID_001 xxxxx
unsigned char CheckAVMute(void);
//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
unsigned char CheckPlg5VPwr(unsigned char ucPortSel);
//FIX_ID_037 xxxxx
unsigned char IsHDMIMode(void);
void GetAVIInfoFrame(struct it6802_dev_data *it6802);
void SetVideoInputFormatWithInfoFrame(struct it6802_dev_data *it6802);
void SetColorimetryByInfoFrame(struct it6802_dev_data *it6802);
//static void SetCSCBYPASS(struct it6802_dev_data *it6802);
void SetColorSpaceConvert(struct it6802_dev_data *it6802);
void SetNewInfoVideoOutput(struct it6802_dev_data *it6802);
void SetVideoInputFormatWithoutInfoFrame(struct it6802_dev_data *it6802,unsigned char bInMode);
void SetColorimetryByMode(struct it6802_dev_data *it6802);
void SetDVIVideoOutput(struct it6802_dev_data *it6802);

//FIX_ID_003 xxxxx    //Add IT6802 Video Output Configure setting
void IT6802_VideoOutputModeSet(struct it6802_dev_data *it6802);
//FIX_ID_003 xxxxx

void IT6802VideoOutputConfigure(struct it6802_dev_data *it6802);
void SetVideoOutputColorFormat(struct it6802_dev_data *it6802);
//void it6802PortSelect(unsigned char ucPortSel);
void IT6802_HalfPCLKFinetune(struct it6802_dev_data *it6802); //ITE_150409
void IT6802_DCLKInvertConfigure(unsigned char IsHalfBusDDR);// 20240311
void IT680x_StandbyMode(unsigned char ucstate);//ITE_151021

void hdmirx_ECCTimingOut(unsigned char ucport);

/* HDMI Audio function    *********************************************************/
void aud_fiforst( void );
void IT6802AudioOutputEnable(unsigned char bEnable);
void hdmirx_ResetAudio(void);
void hdmirx_SetHWMuteClrMode(void);
void hdmirx_SetHWMuteClr(void);
void hdmirx_ClearHWMuteClr(void);
void getHDMIRXInputAudio(AUDIO_CAPS *pAudioCaps);
void IT6802SwitchAudioState(struct it6802_dev_data *it6802,Audio_State_Type state);
#ifdef _FIX_ID_028_
//FIX_ID_028 xxxxx //For Debug Audio error with S2
// remove --> static void IT6802AudioHandler(struct it6802_dev_data *it6802);
//FIX_ID_028 xxxxx
#else
void IT6802AudioHandler(struct it6802_dev_data *it6802);
#endif

#ifdef EnableCalFs
//FIX_ID_023 xxxxx        //Fixed for Audio Channel Status Error with invalid HDMI source
void AudioFsCal(void);
unsigned int PCLKGet(void);
void TMDSGet(void);
void DumpNCTSReg(void);
//FIX_ID_023 xxxxx
#endif

/* HDMI Video function    *********************************************************/
void IT6802_AFE_Rst( void );

//FIX_ID_037 xxxxx //Allion MxL compliance issue !!!
//xxxxx 2014-0529 //Content On/Off
void IT6802_HDCP_ContentOff(unsigned char ucPort , unsigned char bOff);
void IT6802_RAPContentOff(unsigned char bOff);
//xxxxx 2014-0529
//FIX_ID_037 xxxxx

void IT6802_SetVideoMute(struct it6802_dev_data *it6802,unsigned char bMute);
//static void IT6802VideoOutputCDSet(void);
void IT6802VideoOutputEnable(unsigned char bEnableOutput);
void IT6802VideoCountClr(void);
void IT6802SwitchVideoState(struct it6802_dev_data *it6802,Video_State_Type  eNewVState);
void IT6802VideoHandler(struct it6802_dev_data *it6802);
void IT680x_StandbyMode(unsigned char ucstate);//ITE_150408


/* HDMI Interrupt function    *********************************************************/
void hdmirx_INT_5V_Pwr_Chg(struct it6802_dev_data *it6802,unsigned char ucport);
void hdmirx_INT_P0_ECC(struct it6802_dev_data *it6802);
void hdmirx_INT_P1_ECC(struct it6802_dev_data *it6802);
void hdmirx_INT_P0_Deskew(struct it6802_dev_data *it6802);
void hdmirx_INT_P1_Deskew(struct it6802_dev_data *it6802);
//FIX_ID_009 xxxxx    //verify interrupt event with reg51[0] select port
void hdmirx_INT_HDMIMode_Chg(struct it6802_dev_data *it6802,unsigned char ucport);
//FIX_ID_009 xxxxx
void hdmirx_INT_SCDT_Chg(struct it6802_dev_data *it6802);

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_AUTO_EQ_
void hdmirx_INT_EQ_FAIL(struct it6802_dev_data *it6802,unsigned char ucPortSel);
#endif
//FIX_ID_001 xxxxx


/* MxL HDCP functions    *********************************************************/
// disable -> static void hdcpsts( void );





/* EDID RAM  functions    *******************************************************/
#ifdef _SUPPORT_EDID_RAM_
//static unsigned char UpdateEDIDRAM(_CODE unsigned char *pEDID,unsigned char BlockNUM);
unsigned char UpdateEDIDRAM(unsigned char *pEDID,unsigned char BlockNUM);
void EnableEDIDupdate(void);
void DisableEDIDupdate(void);
//static void EDIDRAMInitial(_CODE unsigned char *pIT6802EDID);
void EDIDRAMInitial(unsigned char *pIT6802EDID);
//static unsigned char Find_Phyaddress_Location(_CODE unsigned char *pEDID,unsigned char Block_Number);
unsigned char Find_Phyaddress_Location(unsigned char *pEDID,unsigned char Block_Number);
void UpdateEDIDReg(unsigned char u8_VSDB_Addr, unsigned char CEC_AB, unsigned char CEC_CD, unsigned char Block1_CheckSum);
void PhyAdrSet(void);
#endif


/* Driver State Machine Process **********************************************/
void IT6802_fsm(void);

void IT6802ChangeTTLVideoOutputMode(void);
void IT6802MSCCmdTest(unsigned char ucCmd);
void  Dump_IT680xReg(void);
void debugEQ(unsigned char ucCmd);
void debugxxx(void);

void show_vid_info( void );
void get_vid_info( void );


#ifdef SUPPORT_UART_CMD
extern unsigned char m_UartCmd;
#endif



/* Driver State Machine Process **********************************************/
void IT6802HDMIInterruptHandler(struct it6802_dev_data *it6802);
//void IT6802_fsm(void);


#ifndef Enable_IR
void it6802AutoPortSelect(struct it6802_dev_data *it6802);
#endif

//void show_vid_info( void );
//void get_vid_info( void );

#ifdef Enable_Vendor_Specific_packet
void Dump3DReg(void);
unsigned char IT6802_DE3DFrame(unsigned char ena_de3d);
#endif

#ifdef    _SUPPORT_HDMI_REPEATER_
    #include "it680x_repeater.h"
#endif

#endif // _IT680X_H_
