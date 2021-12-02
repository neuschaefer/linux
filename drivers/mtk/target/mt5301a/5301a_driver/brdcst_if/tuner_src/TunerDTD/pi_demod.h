/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/***********************************************************************/
/*                                                                     */
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: pi_demod.h,v $                                       */
/*                                                                     */
/***********************************************************************/

#ifndef _PI_DEMOD_H_INCLUDED_
#define _PI_DEMOD_H_INCLUDED_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "dtd_pi_config.h"
#include "pd_tuner_type.h"
#include "c_model.h"
#include "tuner_interface_if.h"

/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/
#define fcSHOW_HOST_CMD         (0 && fcINTERNAL_TEST)

#define fcADD_AD_SAMPLING       1
#define fcADD_AUTO_QAM          1

#ifdef CC_SDAL
#define fcHOSTCMD_BUSY_WAIT     2
//#define fcHOSTCMD_BUSY_WAIT     0   
#else
#define fcHOSTCMD_BUSY_WAIT     0           // 0: Sleep
                                            // 1: Busy-waiting fixed time
                                            // 2: Busy-waiting and check time
//#define fcHOSTCMD_BUSY_WAIT     2
#endif
#define fcSHORT_LINUX_INIT      0


/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "dtd_if.h"
/***********************************************************************/
/*              fcDEMOD_TYPE                                           */
/***********************************************************************/
#define fcONLY_FOR_MT5112       0           // 0: Only for MT5112
#define fcONLY_FOR_MT5381       1           // 1: Only for MT5381
#define fcBOTH_FOR_MT5381       2           // 2: Both for MT5112 & MT5381

#ifndef fcDEMOD_TYPE
#define fcDEMOD_TYPE            fcONLY_FOR_MT5112
//#define fcDEMOD_TYPE            fcBOTH_FOR_MT5381
#endif

/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/
#define fcCHK_TUNER_PLL         0
#define fcTRY_SET_FREQ_AGAIN    1
#define fcIGNORE_VPRA_STAT      1

/***********************************************************************/
/*              Conditional Compiling Dependent Included Files         */
/***********************************************************************/
#include "Com_defs.h"
//come from com_defs.h and related name is MT5112_XXX
/* Mask Rev. */
#define DTD_CHIP_REV_ES      0x00
#define DTD_CHIP_REV_MP      0x01
#define DTD_CHIP_REV_EE      0x02
#define DTD_CHIP_REV_EE_MP   0x03
#define DTD_CHIP_REV_5381    0x10
#define DTD_CHIP_REV_5382P   0x11
#define DTD_CHIP_REV_5381MP  0x12
#define DTD_CHIP_REV_5391    0x18

#define eVAR_FGDTD_QSYMBOLRATE 0x39

typedef enum
{
    DTD_TUNER_TYPE_OOB_BEGIN = 0,
    DTD_TUNER_ISG_ISG10065 = DTD_TUNER_TYPE_OOB_BEGIN,  // add new OOB tuner type after this line
    DTD_TUNER_SANYO_LA7784,
    DTD_TUNER_SAMSUNG_DTQS300IV241O,
    DTD_TUNER_TYPE_OOB_END              // add new OOB tuner type above this line
} DTD_TUNER_OOB_T;

enum
{
    DTD_MOD_TYPE_BEGIN = 0,
    DTD_MOD_ATSC_8VSB = DTD_MOD_TYPE_BEGIN,
//    DTD_MOD_RESERVED,
    DTD_MOD_J83B_64QAM,
    DTD_MOD_J83B_256QAM,
    DTD_MOD_TYPE_END
};
/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define MTK_DTD_I2C_ADDRESS0 ((UINT16) 0x92)
#define MTK_DTD_I2C_ADDRESS1 ((UINT16) 0x90)

#define cDEMOD_SETTING_TIME     15          // ms
#define cDEMOD_CHECK_TIME       10          // ms
#define cMAX_READ_NUM           cCMD_REG_NUM

/***********************************************************************/
#define cSIGN_CHAR_MAX          0x80
#define cUNSIGN_CHAR_MAX        256

#define cHIGH_RES_SNR_SCALE     512
#define cPER_PRE_SCALE          10
#define cPER_SCALE              10000L
#define cPER_MAX                ((UINT16)(0.5 * cPER_SCALE * cPER_PRE_SCALE))
//#define cTCM_TOV                (5e-4 * cPER_SCALE * cPER_PRE_SCALE)
//#define cTCM_TOV                (1e-3 * cPER_SCALE * cPER_PRE_SCALE)
#define cUINT32_MAX             0xFFFFFFFF

/***********************************************************************/
#define cMAX_TRY_CNT            2
//#define cWAIT_TIME_INTERVAL     200         // ms
//#define cWAIT_TIME_INTERVAL     10      // ms
#define cWAIT_TIME_INTERVAL     (10 * cREGRESSION_TIME) // ms

#if fcHOSTCMD_BUSY_WAIT
#define cMAX_WAIT_LOOP          256
#else
#define cMAX_WAIT_LOOP          6
#endif

#if fcHOSTCMD_BUSY_WAIT == 1
#define cHOSTCMD_DELAY_US       500         // in us
#define mcTUNER_HOSTCMD_WAIT()  fgTunerBusyWait(cHOSTCMD_DELAY_US, FALSE)
#elif fcHOSTCMD_BUSY_WAIT == 2
#define cHOSTCMD_DELAY_US       2048        // in us
#define mcTUNER_HOSTCMD_WAIT()                          \
{                                                       \
    if (psDemodCtx->fgUserGetInfo)                      \
    {                                                   \
        if (fgTunerBusyWait(cHOSTCMD_DELAY_US, FALSE))  \
            break;                                      \
    }                                                   \
    else                                                \
        mcDELAY_MS(cSHORT_SLEEP_TIME);                  \
}
#else
//#define mcTUNER_HOSTCMD_WAIT()      mcDELAY_MS(cWAIT_TIME_INTERVAL)
#define mcTUNER_HOSTCMD_WAIT()      mcDELAY_MS(cSHORT_SLEEP_TIME)
#endif

//+++++++++++++++++++++++++++++++++++++
#define cMOSAIC_CNT_THRD                       ((INT8) 3)
#define cMOSAIC_VSB_ERR_THRD                   ((UINT8) 16)
#define cMOSAIC_Q64_ERR_THRD                   ((UINT8) 20)
#define cMOSAIC_Q64_LOSS_THRD                  ((UINT8) 40) 
#define cMOSAIC_Q256_ERR_THRD                  ((UINT8) 30)
#define cMOSAIC_Q256_LOSS_THRD                 ((UINT8) 50)

#define cUNMOSAIC_CNT_THRD                     ((INT8) -3)
#define cUNMOSAIC_VSB_ERR_THRD                 ((UINT8) 8) 
#define cUNMOSAIC_VSB_LOSS_THRD                ((UINT8) 25) 
#define cUNMOSAIC_Q64_ERR_THRD                 ((UINT8) 6) 
#define cUNMOSAIC_Q64_LOSS_THRD                ((UINT8) 40) 
#define cUNMOSAIC_Q256_ERR_THRD                ((UINT8) 10) 
#define cUNMOSAIC_Q256_LOSS_THRD               ((UINT8) 50)

#define cVSB_NORMAL_PKT_CNT                    ((UINT32) 9212)
#define cQ64_NORMAL_PKT_CNT                    ((UINT32) 13818)
#define cQ256_NORMAL_PKT_CNT                   ((UINT32) 18424)
//----------------------------------------------------------

/* For Set AD sampling rate */
enum
{
    eAD_SAMPLING_50MHZ,
    eAD_SAMPLING_60MHZ
};

enum
{
    eFIX,
    eNOT_FIX
};

/***********************************************************************/
enum
{
    cUP_LOAD_OK = 0,
    cUP_LOAD_ERR_I2C,
    cUP_LOAD_ERR_HW_RDY,
    cUP_LOAD_ERR_CHKSUM_RDY,
    cUP_LOAD_ERR_CHKSUM_DL_OK,  // ucode download process checksum
    cUP_LOAD_ERR_CHKSUM_MEM_OK,  // memory checksum
    cUP_LOAD_ERR_CNT
};

#define cMSG_UP_ERR_I2C         "I2c error!"
#define cMSG_UP_ERR_HW_RDY      "HW not ready!"
#define cMSG_UP_ERR_CHKSUM_RDY  "Chksum not ready!"
#define cMSG_UP_ERR_CHKSUM_OK   "Chksum not Ok!"
#define cMSG_UP_ERR_CHKSUM_DL_OK   "Chksum DL not Ok!"
#define cMSG_UP_ERR_CHKSUM_DEM_OK   "Chksum Mem not Ok!"

/***********************************************************************/
/* For return value of DTD_SetFreq */
#define cRET_SETFREQ_OK         (0)
#define cRET_SETFREQ_I2C_ERROR  (-1)
#define cRET_SETFREQ_RANGE      (-2)
#define cRET_SETFREQ_PLL_LOCK   (1)

/***********************************************************************/
/*              Macro For HostCmd                                      */
/***********************************************************************/
#define mcHOST_CMD(pDemd, Cmd, Rcnt, Wcnt)      fgHostCmd((DEMOD_CTX_T *) pDemd, Cmd, Rcnt, Wcnt)

/***********************************************************************/
#if fcDEMOD_GEN == cMT5112EE
#define mcSET_QUEUE2(pu2QuePtr, ePosOfst)   pu2QuePtr[cCMD_REG_NUM - 2 - (ePosOfst) + 1]
#define mcSET_CMD2(pu2QuePtr)               pu2QuePtr[cCMD_ID_POS + 1]
#else // SOC
#define mcSET_QUEUE2(pu2QuePtr, ePosOfst)   pu2QuePtr[cCMD_REG_NUM - 2 - (ePosOfst)]
#define mcSET_CMD2(pu2QuePtr)               pu2QuePtr[cCMD_ID_POS]
#endif

/***********************************************************************/
enum
{
    cAGC_RF_CTRL = 0,
    cAGC_IF_CTRL,
    cAGC_IF_VOL,
    cAGC_IF_PWR_DBM,
    cAGC_SIG_STRENGTH,
    cVCR_CFO,
    cAGC_INFO_SIZE
};

enum
{    
    eAGC_NOT_FIX,    
    eAGC_FIX
};



/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
VOID DTD_UpdateInfo(DEMOD_CTX_T *psDemodCtx);
VOID DTD_Acquire(DEMOD_CTX_T *psDemodCtx, BOOL fgStart);
BOOL DTD_GetTunerStatus(DEMOD_CTX_T *psDemodCtx);

// End of orig PI_Demod.h
/***********************************************************************/

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define REG_SIZE                250
#define cMAX_CHAN_TABLE_NUM     135

#define ISYS_MD_MASK            0x03

/***********************************************************************/
#define cCFO_STR_SIZE           15

/***********************************************************************/
/* For customer request feature setting */
enum
{
    cCUST_REQ_0 = 0,                        /* Get TunerType by reading EEPROM */
    cCUST_REQ_1,                            /* Get TunerType by tuner I2C Address */
    cCUST_REQ_2,                            /* Get TunerType by reading EEPROM with redundancy protection */
    cCUST_REQ_3,                            /* Get TunerType by reading EEPROM without checksum */
    cCUST_REQ_SIZE
};

/*********** Register Addr. *************************************/
#define DTD_REG_uP_Ctrl      (UINT16) 0x00
#define MT5112_REG_uP_IRA_H     (UINT16) 0x01
#define MT5112_REG_uP_IRA_L     (UINT16) 0x02
#define MT5112_REG_uP_IRData    (UINT16) 0x03
#define DTD_REG_uP_Ctrl_E    (UINT16) 0x04
#define DTD_REG_uP_GPIO_O    (UINT16) 0x05
#define DTD_REG_uP_GPIO_I    (UINT16) 0x06
#define DTD_REG_uP_CMDR_CFG  (UINT16) 0x07
#define DTD_REG_uP_GP_REG00  (UINT16) 0x08
#define DTD_REG_uP_GP_REG01  (UINT16) 0x09
#define DTD_REG_uP_GP_REG02  (UINT16) 0x0A
#define DTD_REG_uP_GP_REG03  (UINT16) 0x0B
#define DTD_REG_uP_GP_REG04  (UINT16) 0x0C
#define DTD_REG_uP_GP_REG05  (UINT16) 0x0D
#define DTD_REG_uP_GP_REG06  (UINT16) 0x0E
#define DTD_REG_uP_GP_REG07  (UINT16) 0x0F
#define DTD_REG_uP_CMDR00    (UINT16) 0x10
#define DTD_REG_uP_CMDR01    (UINT16) 0x11
#define DTD_REG_uP_CMDR02    (UINT16) 0x12
#define DTD_REG_uP_CMDR03    (UINT16) 0x13
#define DTD_REG_uP_CMDR04    (UINT16) 0x14
#define DTD_REG_uP_CMDR05    (UINT16) 0x15
#define DTD_REG_uP_CMDR06    (UINT16) 0x16
#define DTD_REG_uP_CMDR07    (UINT16) 0x17
#define DTD_REG_uP_RSPR00    (UINT16) 0x18
#define DTD_REG_uP_RSPR01    (UINT16) 0x19
#define DTD_REG_uP_RSPR02    (UINT16) 0x1A
#define DTD_REG_uP_RSPR03    (UINT16) 0x1B
#define DTD_REG_uP_RSPR04    (UINT16) 0x1C
#define DTD_REG_uP_RSPR05    (UINT16) 0x1D
#define DTD_REG_uP_RSPR06    (UINT16) 0x1E
#define DTD_REG_uP_RSPR07    (UINT16) 0x1F

#define MT5381_REG_uP_IRA_H  (UINT16) 0x024
#define MT5381_REG_uP_IRA_L  (UINT16) 0x025
#define MT5381_REG_uP_IRData (UINT16) 0x028

#define DTD_REG_uP_RESERVED  (UINT16) 0x29

#define DTD_REG_RISC_CFG     (UINT16) 0x4A4
#define DTD_REG_RISC_CMD00   (UINT16) 0x4C0
#define DTD_REG_RISC_RSP00   (UINT16) 0x4C8
#define DTD_REG_RISC_RSP07   (UINT16) 0x4CF

#define DTD_REG_I2C_Cfg      (UINT16) 0x59

#define DTD_REG_Chip_Fs_Cfg  (UINT16) 0xD5
#define DTD_REG_Chip_MPMD    (UINT16) 0xD8
#define DTD_REG_Chip_ID      (UINT16) 0xD9
#define DTD_REG_Chip_Test_Mode   (UINT16) 0xDA
#define DTD_REG_Chip_Misc_Sel    (UINT16) 0xDD
#define DTD_REG_Chip_Mem_Rst   (UINT16) 0xDE
#define DTD_REG_Chip_Mon_Ctrl   (UINT16) 0xE0   // For MT5392b: For montor clk power on/off
#define MT5112EE_REG_Chip_Mem_Rst (UINT16) 0xE1   // For MT5112EE

#define DTD_REG_PAGE_NUM     0xFF
#define cpCHIP_PAD_AGC_OD       0

#define DTD_REG_VCR_Pilot    0x14D
#define DTD_REG_QCR_TrkK     0x19B
#define DTD_REG_DEI_Cfg      0x4A8

/***********************************************************************/
/*              for MT5381 Register Addr.                              */
/***********************************************************************/
/***********************************************************************/
/*              Base Register Addr.                                    */
/***********************************************************************/
//#define cRISC_ATD_BASE          (IO_VIRT + 0x2A000)
#define cRISC_DTD_BASE          (IO_VIRT + 0x2B000)

/*---------------------------------------------------------------------*/

#define cCKGEN_REG_PMUX0        0x400
#define cCKGEN_REG_PMUX1        0x404
#define cCKGEN_REG_PMUX2        0x408
#define cCKGEN_REG_PMUX3        0x40C
#define cCKGEN_REG_PMUX4        0x410

// Specific
#if (fcDEMOD_GEN == cMT5391_DTD)
#include "dtd_reg_5391.h"
#elif (fcDEMOD_GEN == cMT5387_DTD)
#include "dtd_reg_5387.h"
#endif


/***********************************************************************/
enum
{
    CONN_TYPE_TER = 0,
    CONN_TYPE_CAB,
    CONN_TYPE_OOB_RX
};


/***********************************************************************/
//#define mcSET_REG0(RegIdx)      MT5112_SetReg(psDemodCtx, RegIdx, &ucValue, 1)
#define mcSET_REG(RegIdx, RegValue) \
{                                   \
    ucValue = RegValue;             \
    mcSET_REG0(RegIdx);             \
}

/***********************************************************************/
/*              Global variables                                       */
/***********************************************************************/
/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
EXTERN BOOL fgEnableI2C_Flag;

#if fcSHOW_HOST_CMD
EXTERN UCHAR    fgShowHostCmd;
#endif

/* For EQBist test */
#define EQBIST_MAX_SET_REG_NUM  25
#define EQBIST_MAX_CHK_REG_NUM  4

typedef struct _EQBIST_REG_SCRIPT_T {
	UINT8    u1SetRegNum;
	UINT16   u2SetRegAddr[EQBIST_MAX_SET_REG_NUM];
	UINT8    u1SetRegValue[EQBIST_MAX_SET_REG_NUM];
	UINT8    u1ChkRegNum;
	UINT16   u2ChkRegAddr[EQBIST_MAX_CHK_REG_NUM];
	UINT8    u1ChkRegValue[EQBIST_MAX_CHK_REG_NUM];
} EQBIST_REG_SCRIPT_T;

/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
BOOL fgHostCmd(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt);

UINT8 u1GetRegOrVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt, UINT8 u1CmdId);
UINT8 u1SetRegOrVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt, UINT8 u1CmdId);
UINT8 u1GetRegs(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt);
UINT8 ucGetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr);
UINT8 u1GetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt);
UINT8 ucGetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr);
UINT8 u1SetRegs(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt);
VOID vSetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 ucValue);
UINT8 u1SetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt);
VOID vSetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 ucValue);
VOID vSetFwVarBit(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 u1BitPos);
VOID vClrFwVarBit(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 u1BitPos);
VOID DTD_Get_UPIF_Reg(DEMOD_CTX_T *psDemodCtx);

UINT8 DTD_MapTunerType(DEMOD_CTX_T *psDemodCtx, UINT8 u1TunerType);

/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
CHAR *DTD_GetVer(DEMOD_CTX_T *psDemodCtx);
UINT8 DTD_GetSignalLevel(DEMOD_CTX_T *psDemodCtx);
INT16 DTD_GetSignalLeveldBm(DEMOD_CTX_T *psDemodCtx);
UINT16 DTD_GetSignalPER(DEMOD_CTX_T *psDemodCtx);
UINT16 DTD_GetSignalSNR(DEMOD_CTX_T *psDemodCtx);
//VOID DTD_SetTsFmt(DEMOD_CTX_T *psDemodCtx, MPEG_FMT_T *pt_mpeg_fmt);
VOID DTD_SetTsFmt(DEMOD_CTX_T *psDemodCtx, UINT8 u1MpegFmt);
VOID DTD_SetTsMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel);


INT32 DTD_GetMod(DEMOD_CTX_T *psDemodCtx, UINT8 *peModType);
VOID DTD_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn);
VOID DTD_Sleep(DEMOD_CTX_T *psDemodCtx);
VOID DTD_Wakeup(DEMOD_CTX_T *psDemodCtx);
VOID DTD_PERInit(DEMOD_CTX_T *psDemodCtx);
VOID DTD_AccumPER(DEMOD_CTX_T *psDemodCtx);
VOID DTD_SetCtmrReq(DEMOD_CTX_T *psDemodCtx, UINT32 u4CtmrReq);
UINT32 DTD_GetCtmrReq(DEMOD_CTX_T *psDemodCtx);
VOID DTD_ManualAGC(DEMOD_CTX_T *psDemodCtx, BOOL fgRFEn, BOOL fgIFEn, UINT16 *pu2GainVol);
UINT16 DTD_GetFWAcqTime(DEMOD_CTX_T *psDemodCtx);
UINT16 u2ConvFreq2Chan(DEMOD_CTX_T *psDemodCtx);
#ifdef fcSMART_ANT_DRV
VOID vWriteSmartAnt(DEMOD_CTX_T *psDemodCtx, UINT8 u1Direct, UINT8 u1Gain, BOOL fgAntPol);
BOOL fgDetectSmartAnt(DEMOD_CTX_T *psDemodCtx);
#endif
BOOL DTD_Get_AGCGain_CFO(DEMOD_CTX_T *psDemodCtx, INT32 *pi4Buffer, BOOL *fgIsCFO);
BOOL DTD_GetTFO(DEMOD_CTX_T *psDemodCtx, INT32 *pi4Buffer);
VOID vDTDGetStatus(DEMOD_CTX_T *psDemodCtx);
VOID vDTDGetInfo(DEMOD_CTX_T *psDemodCtx);
VOID szGetCFOStr(INT32 i4Value, BOOL fgIsCFO, CHAR *szCFO);
VOID DTD_GetQAMInterleave(DEMOD_CTX_T *psDemodCtx, UINT8 *u1InterleavingI, UINT8 *u1InterleavingJ);
VOID DTD_SetSpcPolarity(DEMOD_CTX_T *psDemodCtx, BOOL fgDebug, UINT8 u1SpecInv);
VOID DTD_GetTcmBER(DEMOD_CTX_T *psDemodCtx, UINT32 *pu4TotalPkt, UINT32 *pu4ErrSym);
VOID DTD_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx, UINT16 *pu2IfAgcVol);
VOID DTD_SetTSDirectOut(DEMOD_CTX_T *psDemodCtx, BOOL fgOut);
VOID DTD_Get_RevPkt(DEMOD_CTX_T *psDemodCtx, UINT32  *pu4TotalPkt, UINT32  *pu4ErrPkt );

INT8 DTD_SetFreq(DEMOD_CTX_T *psDemodCtx, PARAM_SETFREQ_T * param);
//BOOL Analog_SetFreq(DEMOD_CTX_T *psDemodCtx, UINT32 u4Freq, UINT8 u1Mode, UINT16 u2Step);
BOOL DTD_Tune(DEMOD_CTX_T *psDemodCtx, PARAM_SETFREQ_T * param);
VOID DTD_SetEQBist_RegScript(DEMOD_CTX_T *psDemodCtx, UINT8 u1Opt);
BOOL DTD_CheckEQBistResult(DEMOD_CTX_T *psDemodCtx, UINT8 u1Opt);
VOID DTD_Get_GSCfg(DEMOD_CTX_T *psDemodCtx);
VOID DTD_Get_BrazilCfg(DEMOD_CTX_T *psDemodCtx);
VOID DTD_ControlGS(DEMOD_CTX_T *psDemodCtx, UINT8 fgGSOperation);
VOID DTD_HwEqGSIndex(DEMOD_CTX_T *psDemodCtx);
VOID DTD_FwEqGSIndex(DEMOD_CTX_T *psDemodCtx);
BOOL DTD_GetTsOutFmt(DEMOD_CTX_T *psDemodCtx, UINT8 * u1Ret);
VOID DTD_SetTsOutFmt(DEMOD_CTX_T *psDemodCtx, UINT8 u1Value);
#endif  // _PI_DEMOD_H_INCLUDED_


