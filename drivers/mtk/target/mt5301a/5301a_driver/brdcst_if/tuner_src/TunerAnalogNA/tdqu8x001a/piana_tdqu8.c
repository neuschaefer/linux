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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: piana_tdqu8.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "PI_ATuner.h"
	#include "PI_Def.h"
	#include "pi_anana_if.h"

#include "piana_tdqu8.h"
//#include "piana_needrefine.h"
#include "pi_anana_tvsys_info.h"//fgDrvTunerCheckTVDLock declaration
#include "pd_anana_glue.h"
#include "pi_anana_if.h"//For Driver Auto Test log macro define
//#include "tvsys_info.h"
//#include "pd_anana_i2c.h"
#include "c_model.h"  // for SGP error code
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Log the debug message
#define TUNER_SCAN_DEBUG        0
static UINT32 PreLockFreq;
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TUNER_PART_FM                  0x07
#define IF_PART_PONR                   0x01
#define IF_PART_AFC                    0x1E
#define IF_PART_FMIFL                  0x20
#define IF_PART_VIFL                   0x40
#define IF_PART_AFCWIN                 0x80

#define fcFAST_CHANGE_CHAN      1
#define fcIGNORE_TVD_LOCK       0
#define fcANA_PIPD_ARCH         0

//#define cANA_TUNER_IF_FREQ      45750
//#define cDIG_TUNER_IF_FREQ      44000
//#define cANA_TUNER_LO_STEP      62500

#if fcFAST_CHANGE_CHAN
#define cAUTO_SCAN_TYPE         eTUNER_UNI_AUTO_SCAN
#else
#define cAUTO_SCAN_TYPE         eTUNER_AUTO_FINE_TUNE
#endif

/**
*  fine tune step:      IF_PART_AFCWIN & IF_PART_VIFL are both ON
*  check step:          IF_PART_AFCWIN is ON, but IF_PART_VIFL is OFF
*  snow step:           IF_PART_AFCWIN is OFF
*/
const UINT8 SCAN_STEP[4][3] =
{
  /** { fine tune step, check step, snow step}  */
  { SCAN_SMALL_STEP,    SCAN_MIDDLE_STEP, SCAN_MIDDLE_STEP},    // Auto Scan Steps
  { SCAN_TINY_STEP, SCAN_TINY_STEP, SCAN_SMALL_STEP},   // Auto Fine Tune Steps
  { SCAN_SMALL_STEP,    SCAN_SMALL_STEP, SCAN_MIDDLE_STEP}, // Semi Scan Steps
  { SCAN_SMALL_STEP,    SCAN_SMALL_STEP, SCAN_MIDDLE_STEP}  // Uni-type Auto Scan Steps
};

/***********************************************************************/
/*  Description : Set the sound system of given sub system             */
/*  Parameters  : Sub system index                                     */
/***********************************************************************/
const strucIFPART TDQU8_TunerIFSupport[TUNERARRAYDEPTH] =
{
// PAL/BG
    { 0x16, 0x70, 0x49 },
// PAL/DK
    { 0x16, 0x70, 0x4B },
// PAL/I
    { 0x16, 0x70, 0x4A },
// SECAM/L
    { 0x06, 0x50, 0x4B },
// SECAM/L1
    { 0x86, 0x50, 0x53 },
// NTSC/M
    { 0xD2, 0x30, 0x44 },
};

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
EXTERN BOOL fgEnableATP;//Flag to check whether show  Driver ATP Auto Test log 
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions 
//-----------------------------------------------------------------------------
//todo: pi_tuner.h
//INT16 Tuner_SetFreq(void *psTunerCtx, UINT32 Freq, UINT8 u1Mode, UINT16 u2Step);
//BOOL Tuner_GetStatus(void *psTunerCtx);
//todo: PD_ATUNER.h
EXTERN  VOID    DigTunerBypassI2C(BOOL bSwitchOn);
//VOID *GetDigiTunerCtx(VOID);
//UINT8 u1I2cWriteTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

/*
 *  TDQU8_TunerSetIfSystem
 *  Set Tuner patameters
 *  @param  u1SubSysIdx  set sub-system index.
 *  @param  *TunerS  tuner's parameters
 *  @param  *TunerDataCnt  The number of tuner parameters.
 *  @retval   TRUE : Set Successful.
 */
static BOOL TDQU8_TunerSetIfSystem(UINT8 u1SubSysIdx, UINT8 *TunerS, UINT8 *TunerDataCnt)
{
const strucIFPART *pTmpIf;

    if (u1SubSysIdx >= SYS_SUB_TOTAL)
        u1SubSysIdx = SYS_SUB_NTSC_M;

        mcSHOW_DBG_MSG(("TDQU8_TunerSetIfSystem\n"));
    *TunerDataCnt = 4;
    pTmpIf = &(TDQU8_TunerIFSupport[u1SubSysIdx]);
    TunerS[0] = 0x00;                       // First Register addr. (SAD)
    TunerS[1] = pTmpIf->bBdata;
    TunerS[2] = pTmpIf->bCdata;
    TunerS[3] = pTmpIf->bEdata;

// for PHILIPS_1236MK5
    //TunerS[1] |= 0x01;

// for PHILIPS_1216MEMK5
//    TunerS[1] |= 0x40;
//    if (pTmpIf->bCdata > 0x50)              // B/G, D/K, I
//    if (u1SubSysIdx <= SYS_SUB_PAL_I)       // B/G, D/K, I
//        TunerS[2] |= 0x02;

    return TRUE;
}

/*
 *  TDQU8_Tuner_SetSystem
 *  Set Sub system Index
  * @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  u1SubSysIdx  	set sub-system index.
 *  @retval   TRUE : Set Successful.
 */
static BOOL TDQU8_Tuner_SetSystem(ATV_PI_CTX_T *psAtvPiCtx, UINT8 u1SubSysIdx)
{
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;
UINT8   TunerS[4], TunerDataCnt;
//void *psI2cCtx;

    if (!TDQU8_TunerSetIfSystem(u1SubSysIdx, TunerS, &TunerDataCnt))
        return (0);
    
    //mcSHOW_DBG_MSG2(("fcNEW_ANA_PIPD_ARCH = 2\n"));
    mcSHOW_DBG_MSG2(("IF_%02X: (%d) %02X-%02X-%02X-%02X\n",
                   pTDQU8_AnaTuner->u1IF_I2cAddr, TunerDataCnt, TunerS[0], TunerS[1],
                   TunerS[2], TunerS[3]));

    /* send this data */
   // if (u1I2cWriteTuner(pTDQU8_AnaTuner->pDigiTunerCtx, pTDQU8_AnaTuner->u1IF_I2cAddr, TunerS, TunerDataCnt))
   if(ICtrlBus_I2cTunerWrite(270,pTDQU8_AnaTuner->u1IF_I2cAddr, TunerS, TunerDataCnt))
    {
    	#ifdef ERR_I2C_ANA_DEMOD
        PDWNC_WriteErrorCode(ERR_I2C_ANA_DEMOD);    // // Write the ANA_DEMOD error code into EEPROM
        mcSHOW_HW_MSG(("####Err Code: ERR_I2C_ANA_DEMOD\n"));              
    	#endif
        mcSHOW_HW_MSG(("TunerSet IFSetsystem failed!\n"));
        return 0;
    }
    return 1;
}

static BOOL fgIFPartReadComm(UINT8 bAddr, UINT8 bSubAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize)
{
UINT8 i;
//UINT16 u2ByteCnt;

//JWEI 2006/09/19
//advised from James Wei and modify @0924 BS Chen
#if 0 //jackson, PD_ATuner.h already removed
    mcTUNER_DETACH_I2C(TRUE);
#endif
//DigTunerBypassI2C(TRUE);
    for (i = 0; i < TUNER_I2C_RETRY; i++)
    {
    //    u2ByteCnt = SIF_ReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize);
        //u2ByteCnt = SIF_NIMReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize);
       // u2ByteCnt = SIF_TunerReadNoSubAddr(SIF_NPTV_CLOCK, bAddr, pbData, u1TunerCtrlSize);
        //mcSHOW_DBG_MSG(("RetVal=%d\n",u2ByteCnt));
        //if (u2ByteCnt == u1TunerCtrlSize)
        if(!ICtrlBus_I2cTunerRead(270,bAddr,pbData,u1TunerCtrlSize))
        {   
#if 0 //jackson, PD_ATuner.h already removed
            mcTUNER_DETACH_I2C(FALSE);
#endif
           // DigTunerBypassI2C(FALSE);
            mcSHOW_DBG_MSG3(("fgIFPartRead: %02X=%02X\n", bAddr, *pbData));
           return TRUE;
        }
    }

#if 0 //jackson, PD_ATuner.h already removed
    mcTUNER_DETACH_I2C(FALSE);
#endif
    // DigTunerBypassI2C(FALSE);
    mcSHOW_HW_MSG(("IF Part Read 0x%02X fail!\n", bAddr));
    return FALSE;
}

/************************************************************************
     Function : BOOL fgIFPartRead( UINT8 bAddr, UINT8 *prData )
  Description : Get single byte of IF Part Programming (Read Mode)
    Parameter :    bAddr  : Slave Address
                *prData   : Output Data Content Pointer
       Return : TRUE   : Successful
                FALSE  : Failure
I2C Timming Chart:
 S   Slave Addr  R/W  A  Data  A/N  P
|__/\__________/\___/\_/\____/\___/\_|...
Description:
1. S means I2C Start condition.
2. Slave Address means 1000011x where x is the value of R/W
3. R/W Read mode if One.
4. A means Acknowledge, generated by slave.
5. Data means D data (Transimitted byte after read conditon-status register)
   MSB                                      LSB
   Bit7    Bit6  Bit5   Bit4  Bit3  Bit2  Bit1  Bit0
R  AFCWIN  VIFL  FMIFL  AFC4  AFC3  AFC2  AFC1  PONR
6. A/N means Acknowledge not, generated by the master.
7. P means STOP condition, Generated by the master.
************************************************************************/
/*
 *  fgIFPartRead
 *  Read Tuner Part parameters
  * @param  bAddr		The address we want to access
 *  @param  *pbData  	Buffer for read parameters
 *  @retval   value from reading DEMOD parameters.
 */
static BOOL fgIFPartRead(UINT8 bAddr, UINT8 *pbData)
{
    return fgIFPartReadComm(bAddr, 0, pbData, 1);
}

/*******************************************************************************************
 Description: BOOL fgGetCurIFStatus(UINT8 bAddr, UINT8 *pbData, UINT8 bMask )
     Entery:  bAddr : IF portion of Tuner Slave Address.
              pbData: To Save the return value of this function.
              bMask : To get these bits to expect, 0 Not expect, 1, expect.
                      If you have not master Tuner H/W structure.
                      Please use these macro definition below for this parameters.
      IF_PART_PONR:   1:  After Power On reset or after supply breakdown,
                      0:  After a successful reading of the status register.
      IF_PART_AFC:    These four bits is used to monitor AFC Status.
      IF_PART_FMIFL:  0: FM IF level low, 1: FM IF level High
      IF_PART_VIFL:   0: Video IF Level Low, 1: Video IF Level High.
      IF_PART_AFCWIN: 0: Frequency of VIF outside AFC Window, 1: Frequency of VIF inside AFC Window.
      Return: return TRUE if access succesive
********************************************************************************************/
/*
 *  fgGetCurIFStatus
 *  Read Tuner Part parameters
  * @param  bAddr		The address we want to access
 *  @param  *pbData  	Buffer for read parameters
 *  @retval   value from reading DEMOD parameters.
 */
static BOOL fgGetCurIFStatus(UINT8 bAddr, UINT8 *pbData, UINT8 bMask)
{
UINT8 bTimeout =  5;
BOOL fgReturn = FALSE;
UINT8 bCntThreshold = 2;

    do
    {
        *pbData = 0;
    // After a successful reading of the status register
        if ((TRUE == fgIFPartRead(bAddr, pbData))
         && ((*pbData & IF_PART_PONR) != IF_PART_PONR))
        {
            if (((*pbData & IF_PART_VIFL) == IF_PART_VIFL)
             || ((*pbData & IF_PART_FMIFL) == IF_PART_FMIFL))
            {                               // VIFL lock
                fgReturn = TRUE;
                break;
            }
            else                            // VIFL not lock
            {
                bCntThreshold--;
            }
        }
        else    // The PONR is not ready or IFPartRead fail
        {
            bTimeout--;
        }
    }
    while ((bTimeout != 0) && (bCntThreshold != 0));

    *pbData &= bMask;

    if (!bCntThreshold)   // Check 2 times to make sure VIFL is really not locked.
    {
        fgReturn = TRUE;
    }

    return fgReturn;
}

/*
 *  bDrvMonitorAFCStatus
 *  Read Tuner Part parameters
 * @param  bAFCBits: ID of TV system that driver is able to be supported.
 *  @retval   return AGC Time Constant (ATC) paramenter if succesive.
 */
static UINT8 bDrvMonitorAFCStatus(UINT8 bAFCBits)
{
  return ((bAFCBits & 0x10) ? (7-((bAFCBits&0x0F)>>1)) : (bAFCBits&0x0F)>>1);
}

/*
 *  bDrvTunerGetScanStep
 *  Get Scan Step (Frquency Offset)
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  bScanType AUTO_SCAN,  AUTO_FINE_TUNE,  SEMI_SCAN, UNI_AUTO_SCAN
 *  @param  bDirection SCAN_DIRECTION_DEC, SCAN_DIRECTION_INC
 *  @retval   Next scan step. Defined in SCAN_STEP[][] according to differnt scan type.
 */
UINT8 bDrvTunerGetScanStep(ATV_PI_CTX_T *psAtvPiCtx, UINT8 bScanType, UINT8 bDirection)
{
UINT8 bIFStatus, bStep;
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;

    fgGetCurIFStatus(pTDQU8_AnaTuner->u1IF_I2cAddr, &bIFStatus,
                     IF_PART_AFCWIN | IF_PART_AFC | IF_PART_VIFL);

    pTDQU8_AnaTuner->u1VIFLock = 0;
    if (((bIFStatus & IF_PART_AFCWIN) == IF_PART_AFCWIN))
    {
        if (bDrvMonitorAFCStatus(bIFStatus) <= 2)
        {
        // Current Freq is very close to the nominal VIF, don't tune the freq anymore.
            pTDQU8_AnaTuner->u1VIFLock = 2;
            return 0;
        }
        else if (bDrvMonitorAFCStatus(bIFStatus) < 0x07)
        {
        // Current Freq is near the nominal VIF; judge bStep from AFC status
            pTDQU8_AnaTuner->u1VIFLock = 1;
            bStep = SCAN_TUNE_STEP(bIFStatus);
            if ((bIFStatus & IF_PART_AFC) & 0x10)
            {
                return (0x80 | bStep);
            }
            else
            {
                return (bStep);
            }
        }
        else
        {
            if ((bIFStatus & IF_PART_VIFL) == IF_PART_VIFL)
            {
                bStep =  SCAN_STEP[bScanType][0];   // fine tune step
            }
            else    // Sometimes the AFCWIN will be set even no VIFL inside AFCWIN
            {
                bStep =  SCAN_STEP[bScanType][1];   // check step
            }
        }
    }
    else            // IF_PART_AFCWIN not active
    {
        bStep = SCAN_STEP[bScanType][2];            // snow step
    }

    if (bDirection == SCAN_DIRECTION_DEC)
        bStep = 0x80 | bStep;

    return bStep;
}

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
/*
 *  TDQU84_AnaTunerGetVer
 *  Get Tuner Version.
 *  @param  void.
 *  @retval   TDQU84_ANA_TUNER_VER.
 */
//-----------------------------------------------------------------------------
UINT8 *TDQU8_AnaTunerGetVer(void){
    return ("analog TDQU8");
}

//----------------------------------------------------------------------------- 
/*
 *  TDQU8_AnaTunerInit
 *  Tuner Driver Initial FUnction
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  IFDEMOD_I2c_Addr	a I2C address of DEMOD.
 *  @param  RF_I2c_Addr  a I2C address of RF Tuner.
 *  @retval   TRUE : Success.
 */
//-----------------------------------------------------------------------------
BOOL TDQU8_AnaTunerInit(ATV_PI_CTX_T *psAtvPiCtx, UINT8 IFDEMOD_I2c_Addr, UINT8 RF_I2c_Addr)
{
//ATV_PI_CTX_T *pTDQU8_AnaTuner = NULL;
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;

//    pTDQU8_AnaTuner = (ATV_PI_CTX_T *) mcALLOC_MEM(sizeof(ATV_PI_CTX_T));
    if (pTDQU8_AnaTuner == NULL)
        return FALSE;
//    pTDQU8_AnaTuner->u1DEMOD_I2c_Addr = IFDEMOD_I2c_Addr;
//    pTDQU8_AnaTuner->u1RF_I2cAddr = RF_I2c_Addr;
//#if (fcNEW_ANA_PIPD_ARCH == 2)
    //pTDQU8_AnaTuner->pDigiTunerCtx = GetDigiTunerCtx();
//#endif
    pTDQU8_AnaTuner->u1IF_I2cAddr = TUNER_ONE_ADDR_IF;
    pTDQU8_AnaTuner->u1RF_I2cAddr = TUNER_ONE_ADDR_TUNER;
    //pTDQU8_AnaTuner->u2IF_Freq = TDQU8_ANA_PIC_IF;
   // pTDQU8_AnaTuner->u2LO_Step = TDQU8_ANA_FREQ_STEP;
//    pTDQU8_AnaTuner->u4Frequency = TDQU8_ANA_INIT_FREQ;
   // pTDQU8_AnaTuner->u1DEMOD_state = TDQU8_ANA_UNLOCK;
    pTDQU8_AnaTuner->u1AnaMode = MOD_ANA_CABLE;
    pTDQU8_AnaTuner->u1VIFLock = 0;
//    pTDQU8_AnaTuner->aSW_Ver_HW_Ver = TDQU8_ANA_TUNER_VER;

//    *ppsAtvPiCtx = pTDQU8_AnaTuner;
    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  TDQU8_AnaTunerClose
 *  Tuner driver close FUnction
 *  @param  psAtvPiCtx  a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   void
 */
//-----------------------------------------------------------------------------
void TDQU8_AnaTunerClose(ATV_PI_CTX_T *psAtvPiCtx)
{
//    mcFREE_MEM(psAtvPiCtx);
}

//----------------------------------------------------------------------------- 
/*
 *  fgDrvTunerSetSystem
 *  Set the sound system of given sub system into tuner HW
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  u1SubSysIdx  	set a sub system type.
 *  @retval   TRUE : Set Successful.
 */
//-----------------------------------------------------------------------------
//#if (fcTUNER_TYPE == cANA_PHILIPS_TDQU8) || (fcTUNER_TYPE == cANA_PHILIPS_TDQU8E) || (fcTUNER_TYPE == cANA_TUNER_ALL)
BOOL TDQU8_TunerSetSystem(ATV_PI_CTX_T *psAtvPiCtx, UINT8 u1SubSysIdx)
{
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;
//mcLINT_UNUSED(u1SubSysIdx);

    mcSHOW_DBG_MSG(("IFSetsystem TDQU8_TunerSetSystem\n"));
    pTDQU8_AnaTuner->u1SubSysIdx = u1SubSysIdx;
    return TDQU8_Tuner_SetSystem(psAtvPiCtx, u1SubSysIdx);
}
//#endif

//----------------------------------------------------------------------------- 
/*
 *  TDQU8_TunerGetSystem
 *  Get the sound system of given sub system into tuner HW
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @retval   Sub system index.
 */
//-----------------------------------------------------------------------------
UINT8 TDQU8_TunerGetSystem(ATV_PI_CTX_T *psAtvPiCtx)
{
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;

    return pTDQU8_AnaTuner->u1SubSysIdx;
}

//----------------------------------------------------------------------------- 
/*
 *  TDQU8_AnaTunerSetFreq
 *  Set the frequency into tuner part.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz	aFrequency value in KHz.
 *  @param  u1AnaMode	Cable/Air Mode.
 *  @retval   TRUE : I2C programming successful.
 *  @retval   FALSE : FALSE => I2C programming fail 
 */
//-----------------------------------------------------------------------------
BOOL TDQU8_AnaTunerSetFreq(ATV_PI_CTX_T *psAtvPiCtx, PARAM_SETFREQ_T *param)
{
BOOL fgRetSts = TRUE;
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;
UINT8 ii = 0;
INT16   i2Err;

    //if ((wFreqInKHz < 55000 )
    // || (wFreqInKHz > 863000 ))
    //    return FALSE;

    pTDQU8_AnaTuner->u1AnaMode = param->Modulation;
    pTDQU8_AnaTuner->u4Freq = param->Freq;

i2Err = ITuner_SetFreq(ITunerGetCtx(),param);
    if (i2Err < 0)//-1
      {                                   // I2C bus error
          mcSHOW_HW_MSG(("Tuner bus error!\n"));
          return FALSE;
      }
   else if (i2Err > 0)//1
      {                                   // frequency out of range
            mcSHOW_HW_MSG(("Frequency out of range!\n"));
            return FALSE;
      }
         

   while (1)
      {
            //mcDELAY_MS(10); 
            ii ++;
            ITuner_OP(ITunerGetCtx(),itGetStatus,0,&fgRetSts);
            if (fgRetSts)
              {
                  mcSHOW_DBG_MSG3(("Tuner PLL locked in %d ms\n", ii*10));
                    break;
                }

            if ((ii * 10) >= 100)
               {                               // Timeout 200ms
                    mcSHOW_HW_MSG(("### Tuner ERROR ### PLL can't lock in 100ms\n"));
                //    break;
                    return FALSE;
               }

	     mcDELAY_MS(10); 
         
      }
           mcSHOW_DBG_MSG3(("PLL set sucessed\n")); 
           return fgRetSts;

}

//----------------------------------------------------------------------------- 
/*
 *  TDQU84_bDrvTunerCheckVIFLock
 *  Check if programmed frequency has strong VIF signal nearby
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  wFreqInKHz  	The frequenccy in KHz need to check the VIF status
 *  @retval   	 2  => Current frequency is very close to the nominal VIF.
           		 1  => Current frequency is is near the nominal VIF.
           		 0  => Current frequency is not close to the nominal VIF yet.
 */
//-----------------------------------------------------------------------------
UINT8 TDQU8_bDrvTunerCheckVIFLock(ATV_PI_CTX_T *psAtvPiCtx,PARAM_SETFREQ_T *param)
{
UINT8 bIFStatus, bAFCStatus;
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;

    fgGetCurIFStatus(pTDQU8_AnaTuner->u1IF_I2cAddr, &bIFStatus,
                     IF_PART_AFCWIN | IF_PART_AFC | IF_PART_VIFL);

    pTDQU8_AnaTuner->u1VIFLock = 0;
    if ((bIFStatus & IF_PART_AFCWIN) == IF_PART_AFCWIN)
    {
        bAFCStatus = bDrvMonitorAFCStatus(bIFStatus);
        mcSHOW_DBG_MSG(("AFC status: 0x%X (IF Status = 0x%X)\n", bAFCStatus, bIFStatus));

        if (bAFCStatus <= 2)
        {
            //Printf("AFC status : return 2\n"); //BS;060731
        // Current Freq is very close to the nominal VIF, don't tune the freq anymore.
            pTDQU8_AnaTuner->u1VIFLock = 2;
            return 2;
        }
        else if (bAFCStatus < 0x07)
        {
        // Current Freq is near the nominal VIF.
            pTDQU8_AnaTuner->u1VIFLock = 1;
            return 1;
        }
    }

// IF_PART_AFCWIN not active or (bAFCStatus == 7)
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  TDQU84_AnaTunerSearchNearbyFreq
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  bScanType		AUTO_FINE_TUNE, UNI_AUTO_SCAN
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function
 *  @retval   TRUE  =>  center freq. found
            	    FALSE =>  center freq. not found (no signal on this channel)
 */
//-----------------------------------------------------------------------------
/* cANA_TUNER_SEARCH */
UINT8 TDQU8_AnaTunerSearchNearbyFreq(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,
                                       x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;
UINT8 u1AnaMode = 0;
UINT32 wFRTmp=0;
UINT8 bStep, bScanState;
UINT8   ii;
UINT8   ucVIFLock;
BOOL    fgTVDLock = FALSE;
UINT8 bATVStatus = 0;
PARAM_SETFREQ_T param;
UINT8 bScanType = eTUNER_UNI_AUTO_SCAN;


    if (pTDQU8_AnaTuner == NULL)
        return (bATVStatus);

    if (wFreqInKHz != NULL)
    {
        wFRTmp = *wFreqInKHz;
	 PreLockFreq = *wFreqInKHz;
    }
    else
    {
          mcSHOW_DBG_MSG(("bATVStatus=%d\n", bATVStatus));
        return (bATVStatus);
    }

    pTDQU8_AnaTuner->isBreak = _BreakFct;
    u1AnaMode = pTDQU8_AnaTuner->u1AnaMode;
    param.Freq=wFRTmp;
    param.Modulation=u1AnaMode;
    param.fgAutoSearch=1;
    
    // Initial check to see if curent freq need full range AFT or not
//JWEI 2006/10/26
param.Freq=wFRTmp;
#if fcFAST_CHANGE_CHAN
     
    for (ii = 0; ii < (200 / (cCHECK_VIF_DELAY * 10)); ii ++)
    {
        mcDELAY_MS(10 * cCHECK_VIF_DELAY);
        ucVIFLock = TDQU8_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
        //if (ucVIFLock >= 1)
        if (ucVIFLock >= 2)
            break;
    }
#else
    ucVIFLock = TDQU8_bDrvTunerCheckVIFLock(psAtvPiCtx,&param);
#endif
#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG(("[ATuner] SearchNearbyTime.CenterFreq=%3u ms\n",
             (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
#endif
    //if (ucVIFLock >= 1)
    if (ucVIFLock >= 2)
    {                                       // Note: don't check TVD if VIF not lock
          mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
        fgTVDLock = fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME);
    #if fcTIMING_MEASURE
         mcSHOW_DBG_MSG(("VIFLock=%d, TVDLock=%d\n", ucVIFLock, fgTVDLock));
    #endif
    }
   mcSHOW_DBG_MSG(("channel center freq = %dKHz\n",*wFreqInKHz));
#if !fcIGNORE_TVD_LOCK
    //if ((bScanType == cAUTO_SCAN_TYPE) && (ucVIFLock >= 1) && (TRUE == fgTVDLock))
    if ((bScanType == cAUTO_SCAN_TYPE) && (ucVIFLock >= 2) && (TRUE == fgTVDLock))
#else
     //if ((bScanType == cAUTO_SCAN_TYPE) && (ucVIFLock >= 1))
    if ((bScanType == cAUTO_SCAN_TYPE) && (ucVIFLock >= 2))
#endif
    {
    #if TUNER_SCAN_DEBUG
        LogS("1. Very close!");
    #endif
    #if !fcFAST_CHANGE_CHAN
        bScanState = TUNER_VIF_LOCK_STATE;
    #else
        mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);
       // mcSHOW_DBG_MSG(("bATVStatus=%d ms\n", bATVStatus));
       //Driver ATP Auto Test log
       mcMSG_IN_SEARCHNEARBY_AUTO_TEST(6250)
       
        mcSHOW_DBG_MSG(("bATVStatus=%d (3 means found channel)\n", bATVStatus));
        return bATVStatus;
    #endif
    }
    else
    {
            bATVStatus =0;
        wFRTmp -= TV_FINE_TUNE_THRESHOLD_KHZ / 2;
        bScanState = TUNER_PLL_LOCK_STATE;
    }

    while (1)
    {
        if (pTDQU8_AnaTuner->isBreak && pTDQU8_AnaTuner->isBreak(pvArg))
        {
            mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
            return bATVStatus;
            //return (FALSE);
        }
        mcDELAY_MS(1);

    #if fcTIMING_MEASURE
        mcSHOW_DBG_MSG(("[ATuner] SearchNearbyTime.AroundCenterFreq=%3u ms\n",
                        (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
    #endif
        switch (bScanState)
        {
            case TUNER_PLL_LOCK_STATE:
                 mcSHOW_DBG_MSG(("PLL check state now\n"));
                // Check if current freq is outside TV_FINE_TUNE_THRESHOLD window
                if (wFRTmp > (*wFreqInKHz + TV_FINE_TUNE_THRESHOLD_KHZ/2))
                {
                #if TUNER_SCAN_DEBUG
                    LogS("4. Hit upper bound !");
                #endif
                    return (FALSE);
                }

                // Update new freq into tuner part
                //wDRTmp = wDrvTunerCheckScanDR(wDRTmp);
               // mcSHOW_DBG_MSG(("wFRTmp = %d, searchnearby\n",wFRTmp));
                 mcSHOW_DBG_MSG(("Current freq =%dKHz\n", wFRTmp));
                 param.Freq=wFRTmp;
                TDQU8_AnaTunerSetFreq(psAtvPiCtx,&param);

                //Ray  if (fgCheckTunerFreqLock(psAtvPiCtx))
                if (1)
                {
                    bScanState = TUNER_VIF_LOCK_STATE;
                }
                else
                {
                // Assume Tuner Device have not been inspected, AutoFine Behavior should be cont.
                    wFRTmp += SCAN_TINY_STEP_KHZ;
                }
            #if fcTIMING_MEASURE
                mcSHOW_DBG_MSG(("[ATuner] SearchNearbyTime.PLLstate=%3u ms / %d\n",
                         (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                         (INT16) (wFRTmp - *wFreqInKHz)));
            #endif
                break;

            case TUNER_VIF_LOCK_STATE:
                  mcSHOW_DBG_MSG(("VIF state now\n"));
                bStep = bDrvTunerGetScanStep(psAtvPiCtx, bScanType, SCAN_DIRECTION_INC);

            #if fcTIMING_MEASURE
                mcSHOW_DBG_MSG(("[ATuner] SearchNearby.Step=%02X\n", bStep));
            #endif

                if (bStep != 0)
                {
                 if ((bStep & 0x80) == 0x80)
                 {
                        bStep = bStep & 0x7F;
                      switch(bStep)
                      {
                        case 1:
                            wFRTmp -= 50;
                            break;
                        case 2:
                            wFRTmp -= 100;
                            break;
                        case 3:
                            wFRTmp -= 150;
                            break;
                        case 4:
                            wFRTmp -= 250;
                            break;
                        case 8:
                            wFRTmp -= 500;
                            break;
                        default:
                          break;
                      }
                }
                else
                {
                        bStep = bStep & 0x7F;
                      switch(bStep)
                      {
                        case 1:
                            wFRTmp += 50;
                            break;
                        case 2:
                            wFRTmp += 100;
                            break;
                        case 3:
                            wFRTmp += 150;
                            break;
                        case 4:
                            wFRTmp += 250;
                            break;
                        case 8:
                            wFRTmp += 500;
                            break;
                        default:
                          break;
                     }
                }
              //  mcSHOW_DBG_MSG(("wFRTmp=%d \n", wFRTmp));
                //    if ((bStep & 0x80) == 0x80)
                //        wFRTmp -= (bStep & 0x7F);
                //    else
                //        wFRTmp += bStep;

                #if TUNER_SCAN_DEBUG
                    LogSW("2. Try again:",wFRTmp);
                #endif

                    bScanState = TUNER_PLL_LOCK_STATE;
                }
                else
                {
                      mcSET_BIT(bATVStatus, cpANA_LOCK_STS_VIF);
                #if TUNER_SCAN_DEBUG
                    LogSW("3. Got VIF:",wFRTmp);
                #endif

                   *wFreqInKHz = wFRTmp;
                #if fcTIMING_MEASURE
                    mcSHOW_DBG_MSG(("[ATuner] SearchNearbyTime.VIFstate=%3u ms\n",
                                    (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
                #endif
                    //return (TRUE);//BS;060801;for MW auto search
                    if (fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME) == TRUE)
                    {
                    //Driver ATP Auto test log
                    mcMSG_IN_SEARCHNEARBY_AUTO_TEST(6250)
                    
                          mcSET_BIT(bATVStatus, cpANA_LOCK_STS_TVD);
                #if fcTIMING_MEASURE
                       mcSHOW_DBG_MSG(("[ATuner] SearchNearbyTime.VIF and TVD lock=%3u ms\n", (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
		 #endif		
                //#if fcCHECK_GHOST_CHANNEL
                 //       if (bDrvTunerSIFAlarm(psAtvPiCtx, wFreqOri, *wFRTmp))
                //#endif
                                        mcSHOW_DBG_MSG(("bATVStatus=%d(3 means found channel)\n", bATVStatus));
		              PreLockFreq = wFRTmp;
                            return bATVStatus;
              //  #if fcCHECK_GHOST_CHANNEL
                //        else
                 //           return FALSE;
                //#endif
                    }
                    else
                    {
                #if fcTIMING_MEASURE
                        mcSHOW_DBG_MSG(("[ATuner] SearchNearbyTime.VIF lock and TVD unlock=%3u ms\n", (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
                #endif
                #if !fcIGNORE_TVD_LOCK
                                mcSHOW_DBG_MSG(("bATVStatus=%d(1 means VIF lock and TVD unlock)\n", bATVStatus));
                        return bATVStatus;
                #else
                        mcSHOW_DBG_MSG(("bATVStatus=%d(1 means VIF lock and TVD unlock)\n", bATVStatus));
                        return bATVStatus;
                #endif
                    }
                }
                break;
        }
    }
//    return FALSE;
}

//----------------------------------------------------------------------------- 
/*
 *  TDQU8_MonitorAnaTuner
 *  Find the freq. value that near center frequency of the channel.
 *  @param  psAtvPiCtx  	a type pointer use to point to struct ATV_PI_CTX_T.
 *  @param  bScanType		AUTO_FINE_TUNE, UNI_AUTO_SCAN
 *  @param  *wFreqInKHz  	THe point of frequenccy in KHz need to check the VIF status
 *  @param  _BreakFct  	 	The break function
 *  @param  *pvArg  		The argument of break function
 *  @retval   TRUE  =>  center freq. found
            	    FALSE =>  center freq. not found (no signal on this channel)
 */
//-----------------------------------------------------------------------------
UINT8 TDQU8_MonitorAnaTuner(ATV_PI_CTX_T *psAtvPiCtx, UINT32 *wFreqInKHz,
                              x_break_fct _BreakFct, void *pvArg)
{
ATV_PI_CTX_T *pTDQU8_AnaTuner = psAtvPiCtx;
UINT32 wFreqInKHz1 = PreLockFreq;
UINT8 u1AnaMode = pTDQU8_AnaTuner->u1AnaMode;
UINT8 bATVStatus = mcBIT(cpANA_LOCK_STS_VIF) | mcBIT(cpANA_LOCK_STS_TVD);

#if 0
UINT8 bStep;
UINT8 ucVIFLock;
UINT8 bScanType = eTUNER_UNI_AUTO_SCAN;
#endif 

PARAM_SETFREQ_T param;
static INT32 wFreqInKHz2 = 0;
static INT32 ThreGap = 0;
UINT32 wFreqBoundary = TV_FINE_TUNE_THRESHOLD_KHZ/2;
UINT32 Freq=pTDQU8_AnaTuner->u4Freq;

pTDQU8_AnaTuner->isBreak = _BreakFct;
    mcSHOW_DBG_MSG2(("Monitor frequency = %d\n", *wFreqInKHz));
    mcSHOW_DBG_MSG2(("Last monitor end freq = %dKHz\n",pTDQU8_AnaTuner->u4Freq));
     
    param.Modulation= u1AnaMode;
    param.fgAutoSearch=0;
 
 //Driver Auto Test log 
 mcMSG_IN_MONITOR_AUTO_TEST(6250)

{
	  UINT8  	TVD_TMP,bIFStatus,bAFCStatus;
        
		TVD_TMP = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10);
        DigTunerBypassI2C(TRUE);
		fgGetCurIFStatus(pTDQU8_AnaTuner->u1IF_I2cAddr, &bIFStatus,IF_PART_AFCWIN | IF_PART_AFC | IF_PART_VIFL);
        DigTunerBypassI2C(FALSE);
		bAFCStatus = bDrvMonitorAFCStatus(bIFStatus);
		mcSHOW_DBG_MSG3(("AFC status: 0x%X, IF Status = 0x%X, Vpress = 0x%x\n", bAFCStatus, bIFStatus, TVD_TMP));
		
	  if (TVD_TMP == 0x10)
	  {
	  	mcSHOW_DBG_MSG3(("Vpress = 1;\n"));

             if(((wFreqInKHz1 + wFreqInKHz2) <= (*wFreqInKHz + wFreqBoundary)) && 
           			 ((wFreqInKHz1 + wFreqInKHz2) >= (*wFreqInKHz - wFreqBoundary)))
             	{
	  	       if(bAFCStatus >= (2 + ThreGap))
	  	        { 
           
	  	           if(bIFStatus & 0x10)
	  	            {	
	  		           wFreqInKHz2 -= 50;
                       param.Freq=wFreqInKHz1 + wFreqInKHz2;
                        mcSHOW_DBG_MSG3(("-50KHz (%d)\n",wFreqInKHz1 + wFreqInKHz2));
	  	            }
	  	         else
	  	           {
	  	              wFreqInKHz2 += 50;
                      param.Freq=wFreqInKHz1 + wFreqInKHz2;
	  	              mcSHOW_DBG_MSG3(("+50KHz (%d)\n",wFreqInKHz1 + wFreqInKHz2));
	  	           }
                   DigTunerBypassI2C(TRUE);
	  	               TDQU8_AnaTunerSetFreq(psAtvPiCtx,&param);
                       DigTunerBypassI2C(FALSE);
	  		     ThreGap = 0;
                      }
	  	
	  	    else
	  	     {
			 mcSHOW_DBG_MSG3(("Lock\n"));
	  		if(wFreqInKHz2 != 0) PreLockFreq = wFreqInKHz1 + wFreqInKHz2;
	  		wFreqInKHz2 = 0;
	  		ThreGap = 3;
	  	     }
             	}
	   else if((wFreqInKHz1 + wFreqInKHz2) > (*wFreqInKHz + wFreqBoundary))
	    {
	  	  PreLockFreq = *wFreqInKHz + wFreqBoundary - 50;
	  	  wFreqInKHz2 = 0;
	    	  ThreGap = 0;
	    }
	    else
	    {
	    	PreLockFreq = *wFreqInKHz - wFreqBoundary + 50;
	  		wFreqInKHz2 = 0;
	    	ThreGap = 0;
	    }	
	  }
	  else
	  {
	  	mcSHOW_DBG_MSG3(("Vpress = 0;\n"));
	  	if (pTDQU8_AnaTuner->isBreak)
                {
                  if (pTDQU8_AnaTuner->isBreak(pvArg))
                   {
                      mcSHOW_DBG_MSG3(("[ATuner] SearchNearby.BreakFct\n"));
                      return (FALSE);
                   }
                }       
            // mcSHOW_DBG_MSG(("Out of range, bStep = %d\n", bStep));
            param.Freq=wFreqInKHz1;
             DigTunerBypassI2C(TRUE);
             TDQU8_AnaTunerSetFreq(psAtvPiCtx,&param);
             bATVStatus = TDQU8_AnaTunerSearchNearbyFreq(psAtvPiCtx, &wFreqInKHz1,
                                                      pTDQU8_AnaTuner->isBreak , pvArg);
			 
	      mcSHOW_DBG_MSG3(("bATVStatus = %d\n",bATVStatus));
             if ( bATVStatus !=3 ){
              param.Freq=*wFreqInKHz;
	          TDQU8_AnaTunerSetFreq(psAtvPiCtx,&param);
            }
		  DigTunerBypassI2C(FALSE);
             wFreqInKHz2 = 0;
             ThreGap = 0;
	  }	  
}
    mcSHOW_DBG_MSG3(("bATVStatus = %d\n",bATVStatus));
    return bATVStatus;
}

