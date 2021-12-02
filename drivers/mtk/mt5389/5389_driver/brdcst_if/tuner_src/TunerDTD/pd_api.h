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
/*  $RCSfile: pd_api.h,v $                                         */
/*                                                                     */
/***********************************************************************/

#ifndef _PD_API_H_INCLUDED_
#define _PD_API_H_INCLUDED_

/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "dtd_pd_config.h"
#include "drvapi_tuner.h"
#include "TDIS.h"
#include "tuner_if.h"

#include "ctrl_bus.h"

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
/***********************************************************************/
/*              OS-Specific Functions                                  */
/***********************************************************************/
/**********************************************************************/
/*              I2C Interface                                         */
/**********************************************************************/
#if 0
UINT8 u1I2cRead(void *psI2cCtx, UINT8 u1I2cAddr, UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cWrite(void *psI2cCtx, UINT8 u1I2cAddr, UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cReadOnly(void *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cReadTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cWriteTuner(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
#endif 

/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/
#define fcCHK_CHIP_REV          (1 && fcDEMOD_TYPE)

#define fcADD_HOSTCMD_CHKSUM    0
#define fcADD_CUST_IF           1
#if fcINTERNAL_TEST
#define fcMEASURE_DRIVER_TIME   2
//#define fcMEASURE_DRIVER_TIME   3
                                            // 2: Acq Time
                                            // 3: Init Time
#else
#define fcMEASURE_DRIVER_TIME   0
#endif
#ifdef TUNER_SET_TYPE_SMART_ANTENNA_PARS
//#define fcSMART_ANT_DRV         1
//#define fcSMART_ANT_DRV         0
#else
//#define fcSMART_ANT_DRV         0
#endif

/***********************************************************************/
/*              Conditional Compiling Dependent Included Files         */
/***********************************************************************/
#if fcADD_CUST_IF
#include "drvcust_if.h"
#endif

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
//#define cMAX_I2C_LEN            2048
#define cMAX_I2C_LEN            8
#define cPER_MAX_FLOAT          1.0
#define cMAX_LOAD_CODE_I2C_LEN  255

/*********** Register Addr. *************************************/

#define DTD_REG_PRA_Loop_AD0 0x364
#define DTD_REG_FTS_STATUS   0x410

#define DTD_REG_ANTIF_1      0x490

/***********************************************************************/
enum
{
    DEMOD_INB_ADDR = 0,
    TUNER_INB_ADDR,
    DEMOD_OOB_ADDR,
    ADDR_TABLE_NUM
};

enum
{
    TEMPO_MODE_NORMAL = 0,
    TEMPO_MODE_SLOW
};

/***************** Entry Function Type Define *********************/
typedef INT32 (*BreakFct)(VOID);
//typedef INT16 (*i2MtkTunerSetFreq)(RF_TUNER_CTX_T *psTunerCtx, UINT32 Freq);
typedef INT16 (*i2MtkTunerSetFreq)(VOID *pvTunerInfo, UINT32 Freq);
                                                    /* MHz */

/***************** Entry Function Table *********************/
typedef struct _TUNER_FCT_TBL_T
{
    i2MtkTunerSetFreq       pni2MtkTunerSetFreq;
} TUNER_FCT_TBL_T;

typedef UINT8   DRV_CFG_SET_T[64][2];
typedef struct _DRV_CFG_T
{
    UINT8           ucNum;
    DRV_CFG_SET_T   *pucDrvCfg;
} DRV_CFG_T;

/***********************************************************************/

/* Move PTD_DTD_CTX_T to dtd_pd_glue_if.h */
#include "dtd_pd_glue_if.h"

typedef struct _DEMOD_CFG_T
{
// Configure setting storage
    UINT8           fgSpecInv;              // Spectrum Inverse
    UINT8           fgSampRate;             // Sampling rate selection
} DEMOD_CFG_T;

/***********************************************************************/
#define mcCTMR_DBG_MSG(_x_)     mcSHOW_DBG_MSG2(_x_)
//#define mcCTMR_DBG_MSG(_x_)     mcPRINTF _x_
#define mcCTMR_DBG_MSG2(_x_)    mcSHOW_DBG_MSG2(_x_)
#define mcSHOW_DRVTIME_MSG(_x_) mcSHOW_DBG_MSG1(_x_)


/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
#if fcMEASURE_DRIVER_TIME
EXTERN TIME_TICK_T u4TickStartMeasure, u4PrevStartTime, u4TickCurMeasure;
EXTERN TIME_DIFF_T u4TickDiffCnt0, u4TickDiffCnt1;
#endif
EXTERN UINT32   u4TunerCtrlForce;
EXTERN UINT8    u1TunerDefTypeForce;

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/

/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
INT16 DTD_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx,
                        DRV_CONN_TYPE_T e_conn_type);
VOID vInitDebug(DEMOD_CTX_T *psDemodCtx);
VOID vRestoreCfg(DEMOD_CTX_T *psDemodCtx);
VOID vStartDebug(DEMOD_CTX_T *psDemodCtx);
VOID vDiscDebug(DEMOD_CTX_T *psDemodCtx);
VOID vShowDebug(DEMOD_CTX_T *psDemodCtx);
VOID vStopDebug(DEMOD_CTX_T *psDemodCtx);
VOID vEndDebug(DEMOD_CTX_T *psDemodCtx, BOOL fgBreak);
VOID vCloseDebug(DEMOD_CTX_T *psDemodCtx);

VOID pi_command(TD_DTD_CTX_T* psTDDTDCtx, INT32 i4Argc, const CHAR** aszArgv);
//VOID pi_external_command(TD_DTD_CTX_T* psTDDTDCtx, INT32 i4Argc, const CHAR** aszArgv);
//INT32 pi_nimTest(TD_DTD_CTX_T* psTDDTDCtx);
//VOID pi_setRegSetting(TD_DTD_CTX_T* psTDDTDCtx, UINT8 u1RegSet,
//                            UINT8 u1RegAddr, UINT8 u1RegValue);
//VOID *TunerGetCtx(UINT16 TunerID);
//BOOL Ana_SwitchRfAgc(VOID); //Jackson remove on mt539x, to fix warning zero used for undefined preprocessing identifier
VOID *GetDigiDemodCtx(VOID);

/***********************************************************************/
/*              OS-Specific Functions                                  */
/***********************************************************************/
//UINT8 u1Conv_dB(UINT16 u2InpValue);

/**********************************************************************/
/*              I2C Interface                                         */
/**********************************************************************/
UINT8 u1I2cWriteSpeed(VOID *psI2cCtx, UINT16 u2I2cClkDiv, UINT8 u1I2cAddr,
                      UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cDemRead(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8 u1I2cDemWrite(VOID *psI2cCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
VOID vSetTempoMode(UINT8 ucTempoMode);
UINT16 u1HostReadRsp(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst);
UINT16 u1HostWriteCmd(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT8 u1ParaCnt, UINT8 u1ParaOfst);

UINT8 u1GetNIM_Config(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CfgData, UINT8 u1CfgCnt, UINT8 u1CfgPos);


#endif  // _PD_API_H_INCLUDED_
