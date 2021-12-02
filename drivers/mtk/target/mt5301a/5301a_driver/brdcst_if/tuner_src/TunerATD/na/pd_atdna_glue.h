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
 * $RCSfile: pd_anana_glue.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pd_atdna_glue.h
 *  Header file of analog NA pd gule layer.
 */

#ifndef PD_ATDNA_GLUE_H
#define PD_ATDNA_GLUE_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//#include "PI_ATuner_dbg.h"
#include "PI_Def_dbg.h"
//#include "PI_ATuner.h"
	#include "PI_Def.h"
	#include "pi_atdna_if.h" //EC_debug_d9403: Rename from pi_anana_if.h to pi_atdna_if.h
//#include "PD_ATuner_dbg.h"

#include "x_rm_dev_types.h"
#include "TDIS.h"
#include "queue.h"
#include "TDMgr.h"
#include "util.h"
//#include "tvsys_info.h"
#include "drvapi_tuner.h"

#ifndef CC_MT5381
#include "source_table.h"
#else
#include "source_select.h"
#endif

#include "pd_tuner_type.h"
#include "pd_atd_glue.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define fcTIMING_MEASURE        1
#define cANA_TUNER_IF_FREQ      45750000
#define fcADD_I2C_EXTRA_DELAY   1
#define TUNER_I2C_RETRY 1

//Pluto : Don't Remove 
#define fcENABLE_MONITOR        2      
//--------------
//#define cpANA_LOCK_STS_VIF      0 //move to pi_atdna_if.h
//#define cpANA_LOCK_STS_TVD      1
#if defined(CC_MT5391) 
 #if (fcTUNER_TYPE==cSHARP_VA1G5BF) 
  #define AFT_AD_NUM                  6             // MT539x, and cSHARP_VA1G5BF use the Servo AD6 in Philips SPACE
 #else
  #define AFT_AD_NUM                  0             // MT539x, For the Servo AD0
 #endif
#else // MT5360
 #if (fcTUNER_TYPE==cLG_TDVRH751F)
  #define AFT_AD_NUM                  2             // MT536x and cLG_TDVRH751F (temp use, need to be customized), For the Servo AD2
 #else
  #define AFT_AD_NUM                  5             // MT536x, For the Servo AD5
 #endif
#endif

#define cCHECK_VIF_DELAY        2           // in ms
// Changed from 105k to 100k.
//#define SIF_NPTV_CLOCK          0x100
#define SIF_NPTV_CLOCK          270
//#define SIF_NPTV_CLOCK          0x2A0
//#define SIF_NPTV_CLOCK          0x400
#define cTUNER_I2C_DELAY_US     100         // in us
//#define cTUNER_I2C_DELAY_US     50          // in us
//#define cTUNER_I2C_DELAY_US     u2Delay     // in us
#define cANA_TUNER_INIT_FREQ    55000       // Khz
//#define cMAX_READ_NUM           8
//-------------


EXTERN UINT8 _bTunerType;
EXTERN BOOL _fgAutoSearch;//Yulia 2005.10.24

EXTERN  UINT8   fgEnI2CDetach;
#if fcTIMING_MEASURE
EXTERN  UINT32  u2TickStart;
#endif


//EXTERN  BOOL  fgUpdCurFreq;


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/***********************************************************************/
/*              Base Register Addr.                                    */
/***********************************************************************/
#define cRISC_ATD_BASE          (IO_VIRT + 0x2A000)  //jackson need move this to correct location

//jackson need refine this. refer sv_const.h
#define SV_ON              1
#define SV_OFF             0

#define SV_TRUE            1
#define SV_FALSE           0

#define cIF_DATA_SIZE           8
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#if 0
//jackson need refine this. refer sv_const.h
/* Video Path (VP) */
enum {
	SV_VP_MAIN,
	SV_VP_PIP,
	SV_VP_NA,
	SV_VP_MAX
};
//jackson need refine this. refer sv_const.h
/* signal type */
enum
{
	SV_ST_TV,
	SV_ST_AV,
	SV_ST_SV,
	SV_ST_YP,
	SV_ST_VGA,
	SV_ST_DVI,
	SV_ST_MPEG, // add for 5371
	SV_ST_MAX
};
//jackson need refine this. refer sv_const.h
/* Video Signal Status -- Add by Cindy */
enum VIDE_SIGNAL_STATUS_LIST
{
	SV_VDO_NOSIGNAL		= 0,
	SV_VDO_NOSUPPORT,
	SV_VDO_UNKNOWN,		// Still doing mode detect
	SV_VDO_STABLE
};
#endif

//jackson need refine this. move to ana pi ?
enum
{
    eTUNER_AUTO_SCAN,
    eTUNER_AUTO_FINE_TUNE,
    eTUNER_SEMI_SCAN,
    eTUNER_UNI_AUTO_SCAN
};
      

enum
{
    cCUST_REQ_ANA_0 = 0,                    /* Enable Monitor Thread */
    cCUST_REQ_ANA_SIZE
};

//#include "anana_pd_glue_if.h"
#if 0
typedef struct _ATV_PD_CTX_T
{
// ATD PI context
    ATV_PI_CTX_T        sAtvPiCtx;
// Hardware Configure
    INT32       Frequency;                  /* Carrier frequency (in Hz) */
    UINT8       u1VIFLock;
    UINT8       u1TunerAcqType;
    SEMA_T      hHalLock;
    UINT32      u4DrvCustTunerCtrl;
    UINT32      u4AcqFrequency;
    BOOL        fgBreakMonitor;
} ATV_PD_CTX_T, *PATV_PD_CTX_T;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//move from PI_ATuner.h
#define cATUNER_CTX(psCtx)                          (&((psCtx)->sAtvPiCtx))
#define cATUNER_PI_CTX(psCtx)                       cATUNER_CTX(psCtx)
#define cATUNER_PD_CTX(psCtx)                       (psCtx)

#define mcTUNER_DETACH_I2C(bDetach)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//Jackson: todo: refine this, check whether this api have been used.
// need move to export API ?
//INT32 TunerNullBreak(VOID* pArg); 

UINT32 AtdNA_GetNoiseLevel(void);
UINT16 AtdNA_GetIFAGC(void);
BOOL AtdNA_SetLNA(BOOL fgLNA);

//BOOL fgTunerPartRead(UINT8 bAddr, UINT8 *pbData);
//BOOL fgTunerPartWrite(UINT8 bAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize);
//BOOL fgIFPartReadComm(UINT8 bAddr, UINT8 bSubAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize);
//BOOL fgIFPartWriteComm(UINT8 bAddr, UINT8 bSubAddr, UINT8 *pbData, UINT8 u1TunerCtrlSize);

#endif /* PD_ATDNA_GLUE_H */

