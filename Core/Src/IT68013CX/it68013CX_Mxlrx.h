///*****************************************
//  Copyright (C) 2009-2025
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <MxLrx.h>
//   @author ITE
//   @date   2025/02/11
//   @fileversion: ITE_HDMIRX_SAMPLE_V2.13
//******************************************/
#ifndef _MxLRX_H_
#define _MxLRX_H_

#include "it68013CX_config.h"
#include "it68013CX_debug.h"
#include "it68013CX_Mcu.h"
#include "it68013CX_IO.h"
#include "it68013CX_Utility.h"
#include "stdio.h"




#ifdef _ENABLE_IT68XX_MxL_FUNCTION_
    #define _SUPPORT_RCP_                TRUE        // for support it6811 and it6802 MxL CsigBus
    #define _SUPPORT_RAP_                TRUE        // for support it6811 and it6802 MxL CsigBus
    //#define _SUPPORT_UCP_                FALSE        // for support it6811 and it6802 MxL CsigBus
#endif

//FIX_ID_021 xxxxx        //To use CP_100ms for CsigBus_100ms and CEC_100m
//FIX_ID_004 xxxxx        //Add calibration ring osc for CsigBus
#define _SelectExtCrystalForCsigBus_    TRUE
//FIX_ID_004 xxxxx
//FIX_ID_021 xxxxx



#define FIX_ID_013
#define FIX_ID_013_



// Debug Mode
#define EnCsigBusDbgMode    FALSE
#define MSCCsigBusDbgCtrl    TRUE
#define  DDCCsigBusDbgCtrl    FALSE
#define  RCLKFreqSel    1    //; //0: RING/2; 1: RING/4; 2: RING/8; 3: RING/16
#define GenPktRecType    0x81
#define PPHDCPOpt    TRUE    //2013-0509 MxL 1080p packet pixel mode HDCP

#ifndef IT6811B0
#define PPHDCPOpt2    TRUE    //2013-0509 MxL 1080p packet pixel mode HDCP
#else
#define PPHDCPOpt2    FALSE    //only for it6811b0
#endif

//FIX_ID_021 xxxxx        //To use CP_100ms for CsigBus_100ms and CEC_100m
//FIX_ID_004 xxxxx //Add 100ms calibration for CsigBus
//#ifdef _SelectExtCrystalForCsigBus_
#define T10usSrcSel   TRUE    //FALSE: 100ms calibration , TRUR: 27MHz Crystal(only IT6802)
//#else
//#define T10usSrcSel   FALSE     //FALSE: 100ms calibration , TRUR: 27MHz Crystal(only IT6802)
//#endif
//FIX_ID_004 xxxxx
//FIX_ID_021 xxxxx

#define EnMSCBurstWr    TRUE
#define MSCBurstWrID    TRUE   // TRUE: from MxL5E/MxL5F
#define MSCBurstWrOpt    FALSE  // TRUE: Not write Adopter ID unsigned char o ScratchPad
#define EnPktFIFOBurst    TRUE
// DDC Option
#define EnDDCSendAbort    TRUE  // Send ABORT after segment write with EOF
//CsigBus Capability
// FIX_ID_046 MxL version changed from 2.0 -> 2.2
#define MxLVersion    0x22
#define PLIM    1
#define POW    1
#define DEV_TYPE_SINK    1 //06-26
#define DEV_TYPE    1
#define ADOPTER_ID_H    0x02
#define ADOPTER_ID_L    0x45
#define DEVICE_ID_H        0x68
#define DEVICE_ID_L        0x02
#define AckHigh    0xB
#define AckLow    1
// CsigBus INput Option
#define EnCsigBusDeGlitch    TRUE
//---------------------//
//----- WatchDog -----//
//--------------------//
#define DeltaNum    1
#define RegBurstWrTOSel    2 // 2    //0: 320ms, 1: 340ms, 2: 360ms (ATC)
#define Reg100msTOAdj    2 // 2    //00: 100ms, 01: 99ms, 10: 101ms (ATC)
#define EnMSCHwRty    FALSE
#define EnHWPathEn    FALSE

// FIX_ID_050
// #ifndef _SUPPORT_UCP_
// FIX_ID_057
#ifdef _SUPPORT_UCP_
#define MSCRxUCP2Nack    TRUE
#else
#define MSCRxUCP2Nack    FALSE
#endif
//~FIX_ID_050

/////////////////////////////////////////
//CsigBus command fire wait time
//Maxmun time for determin CsigBus fail
//    CsigBusWAITTIME(ms) x CsigBusWAITNUM
/////////////////////////////////////////
//FIX_ID_024 xxxxx    //Fixed for RCP compliance issue
#define CsigBusWAITTIME    1
#define CsigBusWAITNUM     100
//FIX_ID_024    xxxxx

/*****************************************************************************/
/* Type defs enums  **********************************************************/
/*****************************************************************************/

enum
{
  MHD_RAP_CMD_POLL             = 0x00,
  MHD_RAP_CMD_CHG_ACTIVE_PWR  = 0x10,
  MHD_RAP_CMD_CHG_QUIET       = 0x11,
  MHD_RAP_CMD_END             = 0x12
};

//RAPK sub commands
enum
{
    MHD_MSC_MSG_RAP_NO_ERROR                = 0x00,     // RAP No Error
    MHD_MSC_MSG_RAP_UNRECOGNIZED_ACT_CODE    = 0x01,
    MHD_MSC_MSG_RAP_UNSUPPORTED_ACT_CODE    = 0x02,
    MHD_MSC_MSG_RAP_RESPONDER_BUSY            = 0x03
};

enum
{
    RCP_SELECT          = 0x00,
    RCP_UP              = 0x01,
    RCP_DOWN            = 0x02,
    RCP_LEFT            = 0x03,
    RCP_RIGHT           = 0x04,
    RCP_RIGHT_UP        = 0x05,
    RCP_RIGHT_DOWN      = 0x06,
    RCP_LEFT_UP         = 0x07,
    RCP_LEFT_DOWN       = 0x08,
    RCP_ROOT_MENU       = 0x09,
    RCP_SETUP_MENU      = 0x0A,
    RCP_CONTENTS_MENU   = 0x0B,
    RCP_FAVORITE_MENU   = 0x0C,
    RCP_EXIT            = 0x0D,

    //0x0E - 0x1F are reserved

    RCP_NUM_0           = 0x20,
    RCP_NUM_1           = 0x21,
    RCP_NUM_2           = 0x22,
    RCP_NUM_3           = 0x23,
    RCP_NUM_4           = 0x24,
    RCP_NUM_5           = 0x25,
    RCP_NUM_6           = 0x26,
    RCP_NUM_7           = 0x27,
    RCP_NUM_8           = 0x28,
    RCP_NUM_9           = 0x29,

    RCP_DOT             = 0x2A,
    RCP_ENTER           = 0x2B,
    RCP_CLEAR           = 0x2C,

    //0x2D - 0x2F are reserved

    RCP_CH_UP           = 0x30,
    RCP_CH_DOWN         = 0x31,
    RCP_PRE_CH          = 0x32,
    RCP_SOUND_SELECT    = 0x33,
    RCP_INPUT_SELECT    = 0x34,
    RCP_SHOW_INFO       = 0x35,
    RCP_HELP            = 0x36,
    RCP_PAGE_UP         = 0x37,
    RCP_PAGE_DOWN       = 0x38,

    //0x39 - 0x40 are reserved

    RCP_VOL_UP            = 0x41,
    RCP_VOL_DOWN        = 0x42,
    RCP_MUTE            = 0x43,
    RCP_PLAY            = 0x44,
    RCP_STOP            = 0x45,
    RCP_PAUSE           = 0x46,
    RCP_RECORD          = 0x47,
    RCP_REWIND          = 0x48,
    RCP_FAST_FWD        = 0x49,
    RCP_EJECT           = 0x4A,
    RCP_FWD             = 0x4B,
    RCP_BKWD            = 0x4C,

    //0x4D - 0x4F are reserved

    RCP_ANGLE            = 0x50,
    RCP_SUBPICTURE       = 0x51,

    //0x52 - 0x5F are reserved

    RCP_PLAY_FUNC       = 0x60,
    RCP_PAUSE_PLAY_FUNC = 0x61,
    RCP_RECORD_FUNC     = 0x62,
    RCP_PAUSE_REC_FUNC  = 0x63,
    RCP_STOP_FUNC       = 0x64,
    RCP_MUTE_FUNC       = 0x65,
    RCP_UN_MUTE_FUNC    = 0x66,
    RCP_TUNE_FUNC       = 0x67,
    RCP_MEDIA_FUNC      = 0x68,

    //0x69 - 0x70 are reserved

    RCP_F1              = 0x71,
    RCP_F2              = 0x72,
    RCP_F3              = 0x73,
    RCP_F4              = 0x74,
    RCP_F5              = 0x75,

    //0x76 - 0x7D are reserved

    RCP_VS              = 0x7E,
    RCP_RSVD            = 0x7F

};


typedef enum  {
    RCP_Received = 0,
    RCP_Error,
    RCP_ACK,
    RCP_Transfer,
    RCP_Empty,
    RCP_Unknown
}RCPState_Type;

typedef enum  {
    RCP_Result_OK = 0,
    RCP_Result_FAIL,
    RCP_Result_ABORT,
    RCP_Result_Transfer,
    RCP_Result_Finish,
    RCP_Result_Unknown
}RCPResult_Type;



#define RCVABORT        2
#define RCVNACK         3
#define ARBLOSE         4

#define FWTXFAIL        5
#define FWRXPKT         6



#define MxLInt00B       0x20
#define DCAP_CHG        0x01
#define DSCR_CHG        0x02
#define REQ_WRT         0x04
#define GRT_WRT         0x08

#define MxLInt01B       0x21
#define EDID_CHG        0x01

#define MxLSts00B       0x30
#define DCAP_RDY        0x01

#define MxLSts01B       0x31
#define NORM_MODE       0x03
#define PACK_MODE       0x02
#define PATH_EN         0x08
#define MUTED           0x10

#define MSG_MSGE    0x02
#define MSG_RCP        0x10
#define MSG_RCPK    0x11
#define MSG_RCPE    0x12
#define MSG_RAP        0x20
#define MSG_RAPK    0x21
#define MSG_UCP        0x30
#define MSG_UCPK    0x31
#define MSG_UCPE    0x32


#define MxL  1
#define HDMI 0

#define BUS10B 1
#define BUS20B 0


#define MAXRCPINDEX 5
// FIX_ID_043
#define MAX_TOGGLE_FAIL_COUNT 16
//~FIX_ID_043


#if 1
/*****************************************************************************/
/* Type defs struct **********************************************************/
/*****************************************************************************/
struct IT6802_REG_INI
{
    unsigned char ucAddr;
    unsigned char andmask;
    unsigned char ucValue;
};


struct IT6802_VIDEO_CONFIGURE_REG
{
    unsigned char ucReg51;
    unsigned char ucReg65;
};

typedef struct _3D_SourceConfiguration
{
    unsigned char          Format;              /**< Type of 3D source format expected or found.                                                        */
    unsigned char    LR_Reference;        /**< Source of the 3D L/R reference.                                                                    */
    unsigned char FrameDominance;      /**< Left or Right Eye is first in L/R image pair.                                                      */
    unsigned char     LR_Encoding;         /**< Type of 3D L/R encoding expected or detected.                                                      */
    unsigned char    TB_Reference;        /**< Top/Bottom reference for vertically sub-sampled sources.                                           */
    unsigned char    OE_Reference;        /**< Odd/Even reference for horizontally sub-sampled sources.                                           */
    unsigned char NumActiveBlankLines;                 /**< Number of lines separating vertically packed L/R data to be removed (cropped)
                                                  *  before being displayed. Does not include any embedded encoding.                                    */
    unsigned char NumberOfEncodedLines;                /**< Number of encoded lines in one L/R eye frame of the display data
                                                  *  to be blanked out with "Blanking Color". (assumed same number in second eye frame)                 */
    unsigned int  LeftEncodedLineLocation;             /**< Active line number of 1st encoded line in one Left eye frame of the display data (-1=unknown).     */
    unsigned int  RightEncodedLineLocation;            /**< Active line number of 1st encoded line in one Right eye frame of the display data (-1=unknown).
                                                  *  If format is Horizontally Packed, set RightEncodedLineLocation=LeftEncodedLineLocation             */
    unsigned char BlankingColor;                 /**< Color to use when blanking (or masking off) any embedded L/R encoding.                             */
} SRC_3D_SOURCE_CONFIG;

typedef struct _de3dframe
{
    unsigned char VIC;
    unsigned char HB0;
    unsigned char HB1;
    unsigned char HB2;
    unsigned char PB0;
    unsigned char PB1;
    unsigned char PB2;
    unsigned char PB3;
    unsigned char PB4;
    unsigned char PB5;
    unsigned char PB6;
    unsigned char PB7;
} DE3DFRAME;

typedef struct _set_de3d_frame
{
    unsigned char Vic;
    unsigned int V_total;        // Vtotal -1
    unsigned int V_act_start;    // VTotal -1 + Vactive_start -1
    unsigned int V_act_end;      // Vactive end -1
    unsigned int V_sync_end;     // LSB(Vtotal -1 + sync With)
    unsigned int V_2D_active_total;     // V_2D_active_total

} SET_DE3D_FRAME;

typedef enum
{
    VSYNC_SEPARATED_HALF,       /**< VSync separated (field sequential) format.                         */
    VSYNC_SEPARATED_FULL,       /**< VSync separated (frame sequential progressive) format.             */
    VERT_PACKED_HALF,           /**< Over Under (vertically packed) half resolution format.             */
    VERT_PACKED_FULL,           /**< Over Under (vertically packed) full resolution format.             */
    HORIZ_PACKED_HALF,          /**< Side by Side (horizontally packed) half resolution format.         */
    HORIZ_PACKED_FULL,          /**< Side by Side (horizontally packed) full resolution format.         */
    UNDEFINED_FORMAT            /**< Undefined format.                                                  */
} SRC_3D_FORMAT;

typedef enum _pixel_mode
{
    SINGLE_PIXEL,
    DUAL_PIXEL  ,
    MODE_UNKNOWN
}PIXEL_MODE;


//FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
#define MaxEQIndex 3
//FIX_ID_010 xxxxx

//FIX_ID_001 xxxxx Add Auto EQ with Manual EQ
#ifdef _SUPPORT_EQ_ADJUST_
struct it6802_eq_data
{
unsigned char ucEQState;
unsigned char ucAuthR0;                                //20130327 for R0 fail issue
unsigned char ucECCvalue;                                //20130328 for acc ecc error
unsigned char ucECCfailCount;                            //20130328 for acc ecc error
unsigned char ucPkt_Err;
unsigned char ucPortID;
unsigned char f_manualEQadjust;
//FIX_ID_010 xxxxx    //Add JudgeBestEQ to avoid wrong EQ setting
unsigned char ErrorCount[MaxEQIndex];
//FIX_ID_010 xxxxx
};
#endif
//FIX_ID_001 xxxxx



//FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    #define B_MSC_Waiting        0x10
    #define B_DevCapChange        0x08
    #define B_3DSupporpt        0x04 // bit2 B_3DSupporpt
    #define B_ReadDevCap        0x02 // bit1 B_ReadDevCap
    #define B_DiscoveryDone        0x01 // bit0 B_DiscoveryDone
//FIX_ID_005 xxxxx


//FIX_ID_013    xxxxx    //For MSC 3D request issue
#define MSC_3D_VIC        (0x0010)
#define MSC_3D_DTD        (0x0011)

typedef enum _PARSE3D_STA{
    PARSE3D_START,
    PARSE3D_LEN,
    PARSE3D_STRUCT_H,
    PARSE3D_STRUCT_L,
    PARSE3D_MASK_H,
    PARSE3D_MASK_L,
    PARSE3D_VIC,
    PARSE3D_DONE
}PARSE3D_STA;

typedef enum _MxL3D_STATE{
    MxL3D_REQ_START,
    MxL3D_REQ_WRT,
    MxL3D_GNT_WRT,
    MxL3D_WRT_BURST,
    MxL3D_REQ_DONE
}MxL3D_STATE;

struct PARSE3D_STR
{
    unsigned char    uc3DEdidStart;
    unsigned char    uc3DBlock;
    unsigned char    uc3DInfor[32];
    unsigned char    ucVicStart;
    unsigned char    ucVicCnt;
    unsigned char    uc3DTempCnt;
    unsigned char    ucDtdCnt;
    unsigned char    bVSDBspport3D;
};
//FIX_ID_013    xxxxx


#endif


/*****************************************************************************/
/* Error codes ***************************************************************/
/*****************************************************************************/
/* Error code specifying an I2C driver error */
#define IT680x_I2C_DRIVER_ERROR                     -4
#define IT680x_I2C_DRIVER_TERMINAL_ERROR            -5

/* Error code specifying RTA errors */
#define IT680x_RTA_SEM_IN_USE                       -6
#define IT680x_RTA_ERROR                            -7

/* Error code specifying other errors */
#define IT680x_DATA_INVALID                         -8






/*****************************************************************************/
/* Public function prototypes ************************************************/
/*****************************************************************************/


/* IT680x IO Functions   ***********************************************************/
/* IT680x Configuration and Initialization ***********************************/
void IT6802_fsm_init(void);
/* HDMI RX functions   *********************************************************/
void it6802PortSelect(unsigned char ucPortSel);
void it6802HPDCtrl(unsigned char ucport,unsigned char ucEnable);
/* HDMI Audio function    *********************************************************/
/* HDMI Video function    *********************************************************/
/* HDMI Interrupt function    *********************************************************/



/* MxLRX functions    *********************************************************/

/* RCP functions    *******************************************************/
#ifdef _SUPPORT_RCP_
    void RCPinitQ(struct it6802_dev_data *it6802);
    int RCPKeyPop(struct it6802_dev_data *it6802);
    void SwitchRCPResult(struct it6802_dev_data *it6802,RCPResult_Type RCPResult);
    void SwitchRCPStatus(struct it6802_dev_data *it6802,RCPState_Type RCPState);
    void RCPManager(struct it6802_dev_data *it6802);

    //FIX_ID_005 xxxxx    //Add CsigBus Event Handler
    void IT6802CsigBusEventManager(struct it6802_dev_data *it6802);
    //FIX_ID_005 xxxxx


    //FIX_ID_014 xxxxx
    void IT6802HDMIEventManager(struct it6802_dev_data *it6802);
    //FIX_ID_014 xxxxx

    //void RCPKeyPush(unsigned char ucKey);

    //FIX_ID_018    xxxxx    //modify 1K pull-down to 1.033K ohm HDMI Reg1C0[3:2]=2
    void IT6802_WakeupProcess(void);
    //FIX_ID_018 xxxxx



    //FIX_ID_024 xxxxx        //Fixed for RCP compliance issue
    void WaitRCPresponse(struct it6802_dev_data *it6802);
    //FIX_ID_024 xxxxx
#endif


/* MxLRX functions    *********************************************************/
//FIX_ID_036    xxxxx //Enable MxL Function for IT68XX
#ifdef _ENABLE_IT68XX_MxL_FUNCTION_

    #ifdef _SUPPORT_RCP_
        void parse_rcpkey( unsigned char rcpkey );
        void MxL_parse_RCPkey(struct it6802_dev_data *it6802);
    #endif

    #ifdef _SUPPORT_RAP_
        void MxL_parse_RAPkey(struct it6802_dev_data *it6802);
    #endif

    #ifdef _SUPPORT_RAP_
        void ucp_report_event( unsigned char key);
        void MxL_parse_UCPkey(struct it6802_dev_data *it6802);
    #endif

    void MxL_read_mscmsg( struct it6802_dev_data *it6802 );
    int mscWait( void );
    int mscFire( int offset, int wdata );
    int CsigBus_send_mscmsg( struct it6802_dev_data *it6802 );

    /*  MxL interrupt    *******************************************************/
    void parse_devcap(unsigned char *devcap );
    int read_devcap_hw( struct it6802_dev_data *it6802 );
    int set_MxLint( unsigned char offset, unsigned char field );
    int set_MxLsts( unsigned char offset, unsigned char status );
    void write_burst(struct it6802_dev_data *it6802, int offset, int byteno );
    void set_wrburst_data(struct it6802_dev_data *it6802, int offset, int byteno );

    /* Driver State Machine Process **********************************************/
    void IT6802MxLInterruptHandler(struct it6802_dev_data *it6802);
#endif
//FIX_ID_036    xxxxx
/* MxL interrupt    *******************************************************/
/* MxL 3D functions    *******************************************************/

/* MxL 3D functions    *******************************************************/


#ifdef FIX_ID_013_
//FIX_ID_013    xxxxx    //For MSC 3D request issue
    #ifdef  _SUPPORT_EDID_RAM_
        struct PARSE3D_STR* get_EDID_VSDB_3Ddata(void);
        void EDID_ParseVSDB_3Dblock(struct PARSE3D_STR *pstParse3D);
    #endif
    void Msc_3DProcess(MxL3D_STATE *e3DReqState);
    MxL3D_STATE MSC_3DInforSend(unsigned char b3dDtd);
    unsigned char Msc_WriteBurstDataFill(unsigned char ucOffset, unsigned char ucByteNo, unsigned char *pucData);
    //FIX_ID_013    xxxxx
#else
    void v3d_unsupport1st(struct it6802_dev_data *it6802);
    void v3d_unsupport2nd(struct it6802_dev_data *it6802);
#endif //FIX_ID_013


/* EDID RAM  functions    *******************************************************/
/* RCP functions    *******************************************************/
#ifdef _SUPPORT_RCP_
    void RCPKeyPush(unsigned char ucKey);
#endif


#endif
