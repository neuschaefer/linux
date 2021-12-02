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
/*  FILE DESCRIPTION                            tuner_if.h             */
/*                                                                     */
/***********************************************************************/
/** @file tuner_if.h
 *  This file implement tuner control interface.
 */
  
#ifndef _TUNER_IF_H_
#define _TUNER_IF_H_

#include "u_rm.h"
#include "x_tuner.h"

/* Tuner driver API return values */
#define TDMGR_INITIALIZED               ((INT32) 1)
#define DRVAPI_TUNER_OK                 ((INT32) 0)
#define DRVAPI_TUNER_NOT_INIT           ((INT32) -1)
#define DRVAPI_TUNER_ALREDAY_INIT       ((INT32) -2)
#define DRVAPI_TUNER_ERROR              ((INT32) -1024)

#define ATD_NOT_USE     FALSE     //DTV_X_IDTV0801 is FALSE;lei110810;

typedef enum
{
    cTURNKEY_IF = 0,
    cMTAL_IF,
    cSDAL_IF,
    cLINUX__IF,
}MW_INTF_T;


typedef enum
{
    ALPHA_0 = 0,
    ALPHA_1,
    ALPHA_2,
    ALPHA_4,
}ALPHA_INFO_T;

typedef enum
{
    NATIVE_INTERLEAVE = 0,
    INDEPTH_INTERLEAVE
}INTERLEAVE_INFO_T;

/*********** Signal structure *************************************/
typedef struct
{
    INTERLEAVE_INFO_T  eInDepth;            /* inner interleaver mode   */
    ALPHA_INFO_T       eAlpha;              /* hierarchical information */
} HIER_INFO;

typedef struct
{
    INT32       Frequency;                  /* carrier frequency (in Hz)   */
    UINT32      SymbolRate;                 /* Symbol Rate (in Baud)       */
    INT32       QAMSize;                    /* QAM size (16/32/64/128/256) */
    UINT32      e_bandwidth;                /*EU mw: BW_UNKNOWN = 0, BW_6_MHz, BW_7_MHz, BW_8_MHz*/
    UINT32      e_hierarchy_priority;       /*EU mw: HIR_PRIORITY_UNKNOWN = 0, HIR_PRIORITY_NONE, HIR_PRIORITY_HIGH, HIR_PRIORITY_LOW */
    HIER_INFO   sHierInfo;                  /*Hierarchical information*/
    UINT8       u1GI;                       /* Guard Interval:  00->PN420, 01->PN595, 11->PN945 */
    UINT8       u1TimDeInterl;              /* Time Interleaver: 0->M=240, 1->M=720*/
    UINT8       u1FecRate;                  /* Code Rate: 00->0.4, 01->0.6, 10->0.8 */
    UINT8       u1CarrierMode;              /* Carrier Mode: 0->Muticarrier, 1->Singal carrier*/
    UINT8       u1PnNumber;                 /* 0->Variable phase for PN sequence, 1->Constant phase */
    UINT32      ui4_NCO;
    TUNER_DVB_T_T2_MODE_T u1DVBT_T2_indicator;
       UINT8                       ui1_plp_id; /*This 8-bit field uniquely identifies a data PLP*/
} SIGNAL;

typedef struct
{
    BOOL    isNotify;                       /* notify or not          */
    UINT8   val;                            /* notify value: ON | OFF */
} EWS_INFO;

/* enum for set acq action */
typedef enum
{
    TUNE_FREQ_ONLY = 0,
    ACQ_DEMOD_ONLY,
    ACQ_ACTION_DEFAULT
}TUNER_ACQ_ACTION_T;

/* Set special operations */
#define TUNER_SET_SPECIAL_TYPE(_x)                              (RM_SET_TYPE_LAST_ENTRY - (_x))   
#define TUNER_SET_SPECIAL_TYPE_ACQ_ACTION_CLASS                 TUNER_SET_SPECIAL_TYPE(1)
/*for scart type, set to T_ANA pd glue to check monitor process status*/
#define TUNER_SET_TYPE_TUNER_LOCAL_DEFINE_SCART_OUT_DO_ACQ      TUNER_SET_SPECIAL_TYPE(2)
#define TUNER_SET_TYPE_TUNER_LOCAL_DEFINE_RESUME_PI_MONITOR     TUNER_SET_SPECIAL_TYPE(3)

#define TUNER_GET_SPECIAL_TYPE(_x)                              (RM_GET_TYPE_LAST_ENTRY - (_x))   
#define TUNER_GET_SPECIAL_TYPE_EWS_STATUS                       TUNER_GET_SPECIAL_TYPE(1)

#ifndef tuner_break_fct
#define tuner_break_fct
typedef INT32     (*x_break_fct)(void *pArg);
#endif
/*************** UTDIS Function Declaration *********************************/
//-------------------------------------------------------------------------
/** TunerSetMwIntf
 *  The API for setting u1MWIntface type. 
 */
//-------------------------------------------------------------------------
void TunerSetMwIntf(UINT8 u1Value);
//-------------------------------------------------------------------------
/** TunerOpen
 *  Let tuner device driver to initialize itself and get ready for providing tuner services. 
 *  If this function return failure, please call TunerClose to release all resources.
 *  @retval 0           Success 
 *  @retval < 0         Fail 
 */
//-------------------------------------------------------------------------
INT32 TunerOpen(void);
//-------------------------------------------------------------------------
/** TunerClose
 *  Free tuner device driver resource or let tuner device to enter sleep mode.
 *  @retval 0           Success 
 *  @retval < 0         Fail 
 */
//-------------------------------------------------------------------------
INT32 TunerClose(void);

//-------------------------------------------------------------------------
/** TunerAcq
 *  The upper layer application prepares the desired connection information and 
 *  then passes them into tuner device driver via this function. Tuner device driver 
 *  then instructs hardware to get connection according to the passing-down connection parameters.
 *  @param  TunerID         TunerID
 *  @param  e_conn_type     Connect types.
 *  @param  pv_conn_info    Pointer of tuner connection information structure.
 *  @param  z_conn_info_len Length of pv_conn_info.
 *  @param  _BreakFct       Callback function returns 1 if it wants to terminate the connection-process, Otherwise it returns 0.
 *  @param  pvArg           The tuner device driver passes this parameter to _BreakFct when calling _BreakFct.
 *  @retval TRUE            Success
 *  @retval FALSE           Fail
 */
//-------------------------------------------------------------------------
BOOL TunerAcq(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                SIZE_T z_conn_info_len, x_break_fct _BreakFct,
                void* pvArg);

//-------------------------------------------------------------------------
/** TunerDisc
 *  Disconnect tuner connection.  
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 */
//-------------------------------------------------------------------------
void TunerDisc(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);        

//-------------------------------------------------------------------------
/** TunerDetachI2C
 *  Tuner Detach I2C function.
 *  @param  TunerID     TunerID
 *  @param  bDetach     1: Enable deatch I2C  0: Disable deatch I2C
 */
//-------------------------------------------------------------------------
void TunerDetachI2C(UINT16 TunerID, BOOL bDetach);

//-------------------------------------------------------------------------
/** TunerGetSignal
 *  get modulation information.  
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 *  @param  _pSignal    Modulation info. structure to be filled.
 */
//-------------------------------------------------------------------------
void TunerGetSignal(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal);

//-------------------------------------------------------------------------
/** TunerGetSync
 *  Check if the tuner device is in sync.  
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 *  @retval 1           In sync state.
 *  @retval 0           Not in sync state.
 */
//-------------------------------------------------------------------------
INT16 TunerGetSync(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);

//-------------------------------------------------------------------------
/** TunerGetSignalLevel
 *  Get signal level in terms of percentage.  
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 *  @return             signal level
 */
//-------------------------------------------------------------------------
UINT8  TunerGetSignalLevel(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type); // (RegGetAGC2()*100L)/0x3ff

//-------------------------------------------------------------------------
/** TunerGetSignalLeveldBm
 *  Get signal level in multiples of (0.1dBm).
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 *  @return             signal level
 */
//-------------------------------------------------------------------------
INT16 TunerGetSignalLeveldBm(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);

//-------------------------------------------------------------------------
/** TunerGetSignalSNR
 *  Get SNR.
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 *  @return SNR.
 */
//-------------------------------------------------------------------------
UINT16  TunerGetSignalSNR(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);

//-------------------------------------------------------------------------
/** TunerGetSignalPER
 *  Get the packet error rate.
 *  @param  TunerID     TunerID
 *  @param  e_conn_type Connect types.
 *  @return             PER.
 */
//-------------------------------------------------------------------------
UINT16  TunerGetSignalPER(UINT16 TunerID, DRV_CONN_TYPE_T e_conn_type);

//-------------------------------------------------------------------------
/** TunerGetVer
 *  Get tuner device driver¡¦s version information.
 *  @param  TunerID     TunerID
 *  @return A pointer of version.
 */
//-------------------------------------------------------------------------
char *TunerGetVer(UINT16 TunerID);

//-------------------------------------------------------------------------
/** TunerGetAttribute
 *  Get attribute for low level driver.
 *  @param  TunerID     TunerID
 *  @param  e_set_type Connect types.
 *  @retval <0          Error
 *  @retval 0           DRVAPI_TUNER_OK.
 */
//-------------------------------------------------------------------------
INT16 TunerGetAttribute(UINT16 TunerID,DRV_GET_TYPE_T  e_get_type, void *pvAnaAttribute, SIZE_T* pzAnaAttributeLen);

//-------------------------------------------------------------------------
/** TunerGetAttribute2
 *  Get attribute for low level driver.
 *  @param  TunerID     TunerID
 *  @param  e_set_type Connect types.
 *  @retval <0          Error
 *  @retval 0           DRVAPI_TUNER_OK.
 */
//-------------------------------------------------------------------------
INT16 TunerGetAttribute2(UINT16 TunerID,DRV_GET_TYPE_T  e_get_type, void *pvAnaAttribute, SIZE_T* pzAnaAttributeLen,DRV_CONN_TYPE_T e_conn_type);

//-------------------------------------------------------------------------
/** TunerSetAttribute
 *  Set attribute for low level driver.
 *  @param  TunerID     TunerID
 *  @param  e_set_type Connect types.
 *  @retval <0          Error
 *  @retval 0           DRVAPI_TUNER_OK.
 */
//-------------------------------------------------------------------------
INT32 TunerSetAttribute(UINT16 TunerID, DRV_SET_TYPE_T  e_set_type, const VOID *pvAnaAttribute, SIZE_T zAnaAttributeLen);

//-------------------------------------------------------------------------
/** TunerGetTsFmt
 *  Get TS format.
 *  @param  pvTsFmt    TS format. (pvTsFmt MUST point to TS_FMT_T structure)
 */
//-------------------------------------------------------------------------
void TunerGetTsFmt(void  *pvTsFmt);

//-------------------------------------------------------------------------
/** TunerSetTsSP
 *  Set TS SP.
 *  @param  ucTsSP     TS SP. (ucTsSP MUST reference to SIG_SP_T enumeration)
 */
//-------------------------------------------------------------------------
void TunerSetTsSP(UCHAR  ucTsSP);

//-------------------------------------------------------------------------
/** TunerSetTsTristate
 *  Set TS tristate.
 *  @param  ucTristate     TS tristate. (ucTristate MUST reference to BOOL_T enumeration)
 */
//-------------------------------------------------------------------------
void TunerSetTsTristate(UCHAR  ucTristate);

//-------------------------------------------------------------------------
/** TunerSetTsOutput
 *  Set TS output for Ziggo issue.
 *  @param  ucOutput     TS output enable. (ucOutput MUST reference to BOOL_T enumeration)
 */
//-------------------------------------------------------------------------
void TunerSetTsOutput(UCHAR  ucOutput);

//-------------------------------------------------------------------------
/** TunerSetCiCardType
 *  Set CI card type.
 *  @param  pucAttri     pucAttri[0]: CI card type, pucAttri[1]: action
 */
//-------------------------------------------------------------------------
void TunerSetCiCardType(UCHAR *pucAttri);

//-------------------------------------------------------------------------
/** DigTunerBypassI2C
 *  Bypass i2c connected to tuner 
 *  @param  bSwitchOn     when set TRUE,Bypass i2c to tuner connected, FALSE to disconnect i2c to tuner
 *  @retval 
 */
//-------------------------------------------------------------------------
void DigTunerBypassI2C(BOOL bSwitchOn);

#endif /* _TUNER_IF_H_ */
