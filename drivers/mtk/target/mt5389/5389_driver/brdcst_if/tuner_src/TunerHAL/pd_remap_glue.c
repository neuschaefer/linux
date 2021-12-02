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
 * $RCSfile: pd_glue_remap.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/** @file pd_glue_dvbc.c
 *  dvbc glue layer
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "u_tuner.h"
#include "tuner_if.h"
#include "queue.h"
#include "TDIS.h"
#include "TDMgr.h"
#include "pd_remap_glue.h"
#include "drvapi_tuner.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


/* example 
#define PD_REMAP_TER_DIG  DVBT
#define PD_REMAP_CAB_DIG  DVBC
//#define PD_REMAP_SAT_DIG  DVBS
#define PD_REMAP_TER_ANA AnalogEU
#define PD_REMAP_CAB_ANA AnalogEU
 */
#define PDNAME_CALLXFR(pd,Func) pd##_Tuner##Func 
#define PDNAME_CALL(pd,Func)  PDNAME_CALLXFR(pd,Func)
#define MKSTR(call)  #call

#ifdef PD_REMAP_TER_DIG
extern INT32 PDNAME_CALL(PD_REMAP_TER_DIG,Open)(PTD_CTX ptTDCtx,
    TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx);
extern INT32 PDNAME_CALL(PD_REMAP_TER_DIG,Close)(PTD_SPECIFIC_CTX ptTDSpecificCtx);
#endif
#ifdef PD_REMAP_CAB_DIG
extern INT32 PDNAME_CALL(PD_REMAP_CAB_DIG,Open)(PTD_CTX ptTDCtx,
    TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx);
extern INT32 PDNAME_CALL(PD_REMAP_CAB_DIG,Close)(PTD_SPECIFIC_CTX ptTDSpecificCtx);
#endif
#ifdef PD_REMAP_SAT_DIG
extern INT32 PDNAME_CALL(PD_REMAP_SAT_DIG,Open)(PTD_CTX ptTDCtx,
    TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx);
extern INT32 PDNAME_CALL(PD_REMAP_SAT_DIG,Close)(PTD_SPECIFIC_CTX ptTDSpecificCtx);
#endif
#ifdef PD_REMAP_TER_ANA
extern INT32 PDNAME_CALL(PD_REMAP_TER_ANA,Open)(PTD_CTX ptTDCtx,
    TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx);
extern INT32 PDNAME_CALL(PD_REMAP_TER_ANA,Close)(PTD_SPECIFIC_CTX ptTDSpecificCtx);
#endif
#ifdef PD_REMAP_CAB_ANA
extern INT32 PDNAME_CALL(PD_REMAP_CAB_ANA,Open)(PTD_CTX ptTDCtx,
    TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx);
extern INT32 PDNAME_CALL(PD_REMAP_CAB_ANA,Close)(PTD_SPECIFIC_CTX ptTDSpecificCtx);
#endif
#ifdef PD_REMAP_SAT_ANA
extern INT32 PDNAME_CALL(PD_REMAP_SAT_ANA,Open)(PTD_CTX ptTDCtx,
    TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx);
extern INT32 PDNAME_CALL(PD_REMAP_SAT_ANA,Close)(PTD_SPECIFIC_CTX ptTDSpecificCtx);
#endif


#define PD_REMAP_INSTANCE 1  /* 2 instance require extend LTDIS */
#define SKIP_ARG 255

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static REMAP_PD_CTX_T pPdCtxREMAP[2];

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#if PD_REMAP_INSTANCE == 1 
static INT32   RemapGetTunerID(UINT16 TunerID,DRV_CONN_TYPE_T e_conn_type)
{
#if defined(PD_REMAP_TER_DIG) || defined(PD_REMAP_CAB_DIG) || defined(PD_REMAP_SAT_DIG)
    return 1;
#else
    return 0;
#endif
}

static INT32   RemapGetConnType(UINT16 TunerID,DRV_CONN_TYPE_T e_conn_type)
{
    if (e_conn_type!=SKIP_ARG)
        return e_conn_type;

#if defined(PD_REMAP_TER_DIG) || defined(PD_REMAP_CAB_DIG) || defined(PD_REMAP_SAT_DIG)
    return  pPdCtxREMAP[1].e_conn_type;
#else
    return  pPdCtxREMAP[0].e_conn_type;
#endif
}

/* For protecting MW SetAttribute before TunerConnect. 
  * CR: DTV00095790.  Hui @ 20100917.
  * DVBS-scan need set Air-Attribute before scan.
  * This is only use for DVBS!!!
  */
void SetRemapConnType(DRV_CONN_TYPE_T e_conn_type)
{
    if(e_conn_type != TUNER_CONN_TYPE_SAT_DIG)
        return;

    if (e_conn_type == SKIP_ARG)
        return;

#if defined(PD_REMAP_TER_DIG) || defined(PD_REMAP_CAB_DIG) || defined(PD_REMAP_SAT_DIG)
    pPdCtxREMAP[1].e_conn_type = e_conn_type;
#else
    pPdCtxREMAP[0].e_conn_type = e_conn_type;
#endif
    return;
}

#else
static INT32   RemapGetTunerID(UINT16 TunerID,DRV_CONN_TYPE_T e_conn_type)
{
    if (TunerID != SKIP_ARG)
        return TunerID;

    switch(e_conn_type)
    {
        case  TUNER_CONN_TYPE_TER_ANA:
        case  TUNER_CONN_TYPE_CAB_ANA:
        case  TUNER_CONN_TYPE_SAT_ANA:
            return 0;
    
        default:
        case  TUNER_CONN_TYPE_TER_DIG:
        case  TUNER_CONN_TYPE_CAB_DIG:
        case  TUNER_CONN_TYPE_SAT_DIG:
            return  1;
        
    }
}

static INT32   RemapGetConnType(UINT16 TunerID,DRV_CONN_TYPE_T e_conn_type)
{
    if (e_conn_type!=SKIP_ARG)
        return e_conn_type;

    return pPdCtxREMAP[RemapGetTunerID(TunerID,e_conn_type)].e_conn_type;
}
#endif

static INT32   RemapFctTbl(DRV_CONN_TYPE_T e_conn_type)
{
    switch(e_conn_type)
    {
        case  TUNER_CONN_TYPE_TER_ANA:
        case  TUNER_CONN_TYPE_TER_DIG:
            return 0;
        case  TUNER_CONN_TYPE_CAB_ANA:
        case  TUNER_CONN_TYPE_CAB_DIG:
            return  1;
        case  TUNER_CONN_TYPE_SAT_ANA:
        case  TUNER_CONN_TYPE_SAT_DIG:
            return  2;
        default:
            return -1;
    }
}

//-------------------------------------------------------------------------
/**
 *  This API implement close tuner. It should performs
 *  1. free all allocated resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDSpecificCtx   Pointer to tuner driver context.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------
static INT32 REMAP_TunerClose(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 i4Ret=DRVAPI_TUNER_OK;

    if(RemapGetTunerID(SKIP_ARG,SKIP_ARG)==0)
    {
    #ifdef PD_REMAP_TER_ANA
        //i4Ret = PDNAME_CALL(PD_REMAP_TER_ANA,Close)(ptTDSpecificCtx);
        if(pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Ter.pf_TunerClose)
            i4Ret =pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Ter.pf_TunerClose(ptTDSpecificCtx);

        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_TER_ANA,Close)) " fail\r\n"));
            return i4Ret;
        }
    #endif

    #ifdef PD_REMAP_CAB_ANA
        {
            //i4Ret = PDNAME_CALL(PD_REMAP_CAB_ANA,Close)(ptTDSpecificCtx);
            if(pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Cab.pf_TunerClose)
                i4Ret =pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Cab.pf_TunerClose(ptTDSpecificCtx);
            
            if(i4Ret != DRVAPI_TUNER_OK){
                mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_CAB_ANA,Close))" fail\r\n"));
                return i4Ret;
            }
        }
    #endif

    #ifdef PD_REMAP_SAT_ANA
        {
            //i4Ret = PDNAME_CALL(PD_REMAP_CAB_ANA,Close)(ptTDSpecificCtx);
            if(pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Sat.pf_TunerClose)
                i4Ret =pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Sat.pf_TunerClose(ptTDSpecificCtx);
            
            if(i4Ret != DRVAPI_TUNER_OK){
                mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_SAT_ANA,Close))" fail\r\n"));
                return i4Ret;
            }
        }
    #endif
    }

    if(RemapGetTunerID(SKIP_ARG,SKIP_ARG)==1)
    {
    #ifdef PD_REMAP_TER_DIG
        //i4Ret = PDNAME_CALL(PD_REMAP_TER_DIG,Close)(ptTDSpecificCtx);
        if(pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Ter.pf_TunerClose)
            i4Ret =pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Ter.pf_TunerClose(ptTDSpecificCtx);
        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_TER_DIG,Close))" fail\r\n"));
            return i4Ret;
        }
    #endif

    #ifdef PD_REMAP_CAB_DIG
        {
//          i4Ret = PDNAME_CALL(PD_REMAP_CAB_DIG,Close)(ptTDSpecificCtx);
            if(pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Cab.pf_TunerClose)
                i4Ret =pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Cab.pf_TunerClose(ptTDSpecificCtx);
            if(i4Ret != DRVAPI_TUNER_OK){
                mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_CAB_DIG,Close))" fail\r\n"));
                return i4Ret;
            }
        }
    #endif

    #ifdef PD_REMAP_SAT_DIG
        {
//          i4Ret = PDNAME_CALL(PD_REMAP_CAB_DIG,Close)(ptTDSpecificCtx);
            if(pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Sat.pf_TunerClose)
                i4Ret =pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Sat.pf_TunerClose(ptTDSpecificCtx);
            if(i4Ret != DRVAPI_TUNER_OK){
                mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_SAT_DIG,Close))" fail\r\n"));
                return i4Ret;
            }
        }
    #endif
    }

    return i4Ret;
}
//-------------------------------------------------------------------------
/**
 *  This API do channel acuqusition.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @param   pv_conn_info       middleware tuner info structure, refer u_tuner.h
 *  @param   z_conn_info_len    length of pv_conn_info
 *  @param   _BreakFct          TunerHAL callback API for Vendor_TunerAcq to check whether need to break TunerAcq
 *  @param   *pvArg             pass this parameter to _BreakFct when calling _BreakFct
 *  @retval  1                  TPS LOCK
 *  @retval  0                  TPS UNLOCK
 */
//-------------------------------------------------------------------------
static BOOL REMAP_TunerAcq(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, void* pv_conn_info,
                            SIZE_T z_conn_info_len, x_break_fct _BreakFct, void* pvArg)
{

    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);
    if(pPdCtxREMAP[id].e_conn_type != e_conn_type)
    {
        //call Disc
        switch(RemapFctTbl(pPdCtxREMAP[id].e_conn_type))
        {
            case 0:
                    if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDisc)
                         pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDisc(ptTDSpecificCtx,pPdCtxREMAP[id].e_conn_type);
                    break;
            case 1:
                    if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDisc)
                         pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDisc(ptTDSpecificCtx,pPdCtxREMAP[id].e_conn_type);
                    break;
            case 2:
                    if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDisc)
                         pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDisc(ptTDSpecificCtx,pPdCtxREMAP[id].e_conn_type);
                    break;
            default:
                    mcSHOW_USER_MSG(("unacceptable TunerClose e_conn_type=%d\r\n",e_conn_type));
                    break;
        }
    }

    pPdCtxREMAP[id].e_conn_type = e_conn_type; //set conn_type

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDisc)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerAcq(ptTDSpecificCtx,
                        e_conn_type, pv_conn_info, z_conn_info_len, _BreakFct, pvArg);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDisc)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerAcq(ptTDSpecificCtx,
                        e_conn_type, pv_conn_info, z_conn_info_len, _BreakFct, pvArg);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDisc)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerAcq(ptTDSpecificCtx,
                        e_conn_type, pv_conn_info, z_conn_info_len, _BreakFct, pvArg);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerAcq e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}
//-------------------------------------------------------------------------
/**
 *  This API check lock status
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  1                  LOCK
 *  @retval  0                  UNLOCK
 */
//-------------------------------------------------------------------------
static INT16 REMAP_TunerGetSync(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGerSync)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGerSync(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGerSync)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGerSync(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGerSync)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGerSync(ptTDSpecificCtx,
                        e_conn_type);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetSync e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}

//-------------------------------------------------------------------------
/**
 *  This API do disconnect tuner
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  void
 */
//-------------------------------------------------------------------------
static VOID  REMAP_TunerDisc(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGerSync)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDisc(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGerSync)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDisc(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGerSync)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDisc(ptTDSpecificCtx,
                        e_conn_type);
                break;
        default:
                mcSHOW_USER_CRI_MSG(("unacceptable TunerDisc e_conn_type=%d\r\n",e_conn_type));
                break;
    }
//  return 0;

}

//-------------------------------------------------------------------------
/**
 *  This API get modulation information.
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  Outputs:
 *  @param   *_pSignal          Modulation info. structure to be filled.
 *  @retval  void
 */
//-------------------------------------------------------------------------
static void REMAP_TunerGetSignal(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type, SIGNAL *_pSignal)
{
    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignal)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignal(ptTDSpecificCtx,
                        e_conn_type, _pSignal);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignal)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignal(ptTDSpecificCtx,
                        e_conn_type, _pSignal);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignal)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignal(ptTDSpecificCtx,
                        e_conn_type, _pSignal);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetSignal e_conn_type=%d\r\n",e_conn_type));
                break;
    }
//  return 0;
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in terms of percentage.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0~100              signal level
 */
//-------------------------------------------------------------------------
static UINT8 REMAP_TunerGetSignalLevel(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalLevel)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalLevel(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalLevel)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalLevel(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalLevel)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalLevel(ptTDSpecificCtx,
                        e_conn_type);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetSignalLevel e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;
}

//-------------------------------------------------------------------------
/**
 *  This API get signal level in multiples of (0.1dBm).
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  0 ~ -900           signal level
 */
//-------------------------------------------------------------------------
static INT16 REMAP_TunerGetSignalLeveldBm(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalLeveldBm)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalLeveldBm(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalLeveldBm)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalLeveldBm(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalLeveldBm)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalLeveldBm(ptTDSpecificCtx,
                        e_conn_type);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetSignalLeveldBm e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}

static UINT16 REMAP_TunerGetSignalPER(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{

    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalPER)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalPER(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalPER)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalPER(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalPER)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalPER(ptTDSpecificCtx,
                        e_conn_type);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetSignalPER e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}

static UINT16 REMAP_TunerGetSignalSNR(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CONN_TYPE_T e_conn_type)
{
    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalSNR)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetSignalSNR(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalSNR)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetSignalSNR(ptTDSpecificCtx,
                        e_conn_type);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalSNR)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetSignalSNR(ptTDSpecificCtx,
                        e_conn_type);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetSignalSNR e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;
}

static void REMAP_TunerSetMpgFmt(PTD_SPECIFIC_CTX ptTDSpecificCtx, MPEG_FMT_T  *pt_mpeg_fmt)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetMpgFmt)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetMpgFmt(ptTDSpecificCtx,
                        pt_mpeg_fmt);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetMpgFmt)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetMpgFmt(ptTDSpecificCtx,
                        pt_mpeg_fmt);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetMpgFmt)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetMpgFmt(ptTDSpecificCtx,
                        pt_mpeg_fmt);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerSetMpgFmt e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    //return 0;
}

static char *REMAP_TunerGetVer(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetVer)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetVer(ptTDSpecificCtx);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetVer)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetVer(ptTDSpecificCtx);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetVer)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetVer(ptTDSpecificCtx);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetVer e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}

static INT32 REMAP_TunerNimTest(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerNimTest)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerNimTest(ptTDSpecificCtx);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerNimTest)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerNimTest(ptTDSpecificCtx);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerNimTest)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerNimTest(ptTDSpecificCtx);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerNimTest e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;
}

static void REMAP_TunerSetRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx, UCHAR ucRegSet, UCHAR ucRegAddr, UCHAR ucRegValue)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetRegSetting)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetRegSetting(
                        ptTDSpecificCtx, ucRegSet, ucRegAddr, ucRegValue);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetRegSetting)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetRegSetting(
                        ptTDSpecificCtx, ucRegSet, ucRegAddr, ucRegValue);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetRegSetting)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetRegSetting(
                        ptTDSpecificCtx, ucRegSet, ucRegAddr, ucRegValue);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerSetRegSetting e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    //return 0;

}

static void REMAP_TunerShowRegSetting(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerShowRegSetting)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerShowRegSetting(
                        ptTDSpecificCtx);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerShowRegSetting)
                    pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerShowRegSetting(
                        ptTDSpecificCtx);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerShowRegSetting)
                    pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerShowRegSetting(
                        ptTDSpecificCtx);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerShowRegSetting e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    //return 0;

}

static void REMAP_TunerTestI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerTestI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerTestI2C(
                        ptTDSpecificCtx);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerTestI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerTestI2C(
                        ptTDSpecificCtx);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerTestI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerTestI2C(
                        ptTDSpecificCtx);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerTestI2C e_conn_type=%d\r\n",e_conn_type));
                break;
    }
//  return 0;

}
static void REMAP_TunerCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx, INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerCommand)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerCommand(
                        ptTDSpecificCtx, i4Argc, aszArgv);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerCommand)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerCommand(
                        ptTDSpecificCtx, i4Argc, aszArgv);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerCommand)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerCommand(
                        ptTDSpecificCtx, i4Argc, aszArgv);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerCommand e_conn_type=%d\r\n",e_conn_type));
                break;
    }
//  return 0;
}

static void REMAP_TunerDtdCommand(PTD_SPECIFIC_CTX ptTDSpecificCtx,INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDtdCommand)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDtdCommand(
                        ptTDSpecificCtx, i4Argc, aszArgv);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDtdCommand)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDtdCommand(
                        ptTDSpecificCtx, i4Argc, aszArgv);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDtdCommand)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDtdCommand(
                        ptTDSpecificCtx, i4Argc, aszArgv);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerDtdCommand e_conn_type=%d\r\n",e_conn_type));
                break;
    }
//  return 0;

}

static void REMAP_TunerDetachI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bDetach)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDetachI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerDetachI2C(
                        ptTDSpecificCtx, bDetach);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDetachI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerDetachI2C(
                        ptTDSpecificCtx, bDetach);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDetachI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerDetachI2C(
                        ptTDSpecificCtx, bDetach);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerDetachI2C e_conn_type=%d\r\n",e_conn_type));
                break;
    }
//  return 0;
    
}

static void REMAP_TunerBypassI2C(PTD_SPECIFIC_CTX ptTDSpecificCtx, BOOL bSwitchOn)
{

#if !defined(CC_TER_DEMOD_TUNER_BYPASS) && !defined(CC_CAB_DEMOD_TUNER_BYPASS) && !defined(CC_SAT_DEMOD_TUNER_BYPASS)
    INT32 e_conn_type;
#endif
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

#if defined(CC_TER_DEMOD_TUNER_BYPASS)  /* Fix TER demod to do tunerBypass */

    if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerBypassI2C)
         pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerBypassI2C(
            ptTDSpecificCtx, bSwitchOn);
    
#elif defined(CC_CAB_DEMOD_TUNER_BYPASS)  /* Fix CAB demod to do tunerBypass */
    if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerBypassI2C)
         pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerBypassI2C(
            ptTDSpecificCtx, bSwitchOn);

#elif defined(CC_SAT_DEMOD_TUNER_BYPASS)  /* Fix SAT demod to do tunerBypass */
    if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerBypassI2C)
         pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerBypassI2C(
            ptTDSpecificCtx, bSwitchOn);


#else /* default */
    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerBypassI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerBypassI2C(
                        ptTDSpecificCtx, bSwitchOn);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerBypassI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerBypassI2C(
                        ptTDSpecificCtx, bSwitchOn);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerBypassI2C)
                     pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerBypassI2C(
                        ptTDSpecificCtx, bSwitchOn);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerDetachI2C e_conn_type=%d\r\n",e_conn_type));
                break;
    }
#endif

//  return 0;
    
}

//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_XXXXX
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_XXXXX_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT16 REMAP_TunerGetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetAttribute(
                        ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetAttribute(
                        ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetAttribute(
                        ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetAttribute e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;
}
//-------------------------------------------------------------------------
/**
 *  This API get attribute from tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_get_type         tuner get type, refer x_tuner.h TUNER_GET_TYPE_XXXXX
 *  Outputs:
 *  @param   *pzAttribute       Attribute structure to be filled. Refer x_tuner.h structure definition TUNER_XXXXX_ATTRIBUTE_T
 *  @param   *pzAttributeLen    length to be filled.
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT16 REMAP_TunerGetAttribute2(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_GET_TYPE_T  e_get_type,
                                VOID *pzAttribute, SIZE_T* pzAttributeLen, DRV_CONN_TYPE_T e_conn_type)
{
    INT32 id=RemapGetTunerID(SKIP_ARG,e_conn_type);

    switch(RemapFctTbl(e_conn_type))
    {
        case 0:
            if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetAttribute2)
            {
                return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetAttribute2(
                    ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen, e_conn_type);
            }
            else
            {
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetAttribute(
                        ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen);
            }
            break;
        case 1:
            if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetAttribute2)
            {
                return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetAttribute2(
                    ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen, e_conn_type);
            }
            else
            {
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetAttribute(
                        ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen);
            }
            break;
        case 2:
            if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetAttribute2)
            {
                return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetAttribute2(
                    ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen, e_conn_type);
            }
            else
            {
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetAttribute(
                        ptTDSpecificCtx, e_get_type, pzAttribute, pzAttributeLen);
            }
            break;
        default:
            mcSHOW_USER_MSG(("unacceptable TunerGetAttribute2 e_conn_type=%d\r\n",e_conn_type));
            break;
    }

    return 0;
}
//-------------------------------------------------------------------------
/**
 *  This API set attribute to tuner driver
 *  Inputs:
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @param   e_set_type         tuner set type, check x_tuner.h set operation TUNER_SET_TYPE_XXXX
 *  @param   *pvAttribute       set value.
 *  @param   zAttributeLen      length of *pvAttribute.
 *  @param   e_conn_type        tuner connection type, refer x_tuner.h
 *  @retval  DRVAPI_TUNER_OK    success.
 *  @retval  DRVAPI_TUNER_ERROR fail.
 */
//-------------------------------------------------------------------------
static INT32 REMAP_TunerSetAttribute(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_SET_TYPE_T  e_set_type,
                                        const VOID *pvAttribute, SIZE_T zAttributeLen)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetAttribute(
                        ptTDSpecificCtx, e_set_type, pvAttribute, zAttributeLen);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetAttribute(
                        ptTDSpecificCtx, e_set_type, pvAttribute, zAttributeLen);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetAttribute)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetAttribute(
                        ptTDSpecificCtx, e_set_type, pvAttribute, zAttributeLen);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerSetAttribute e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}
#ifdef fcADD_DIAG_INFO
static INT32 REMAP_TunerSetDiagCmd(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_SET_TYPE_T e_tuner_set_type, VOID* pv_set_info, SIZE_T z_size)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetDiagCmd)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerSetDiagCmd(
                        ptTDSpecificCtx, e_tuner_set_type, pv_set_info, z_size);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetDiagCmd)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerSetDiagCmd(
                        ptTDSpecificCtx, e_tuner_set_type, pv_set_info, z_size);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetDiagCmd)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerSetDiagCmd(
                        ptTDSpecificCtx, e_tuner_set_type, pv_set_info, z_size);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerSetDiagCmd e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;

}
static INT32 REMAP_TunerGetDiagInfo(PTD_SPECIFIC_CTX ptTDSpecificCtx, DRV_CUSTOM_TUNER_GET_TYPE_T e_tuner_get_type, VOID* pv_get_info, SIZE_T* pz_size)
{
    INT32 e_conn_type;
    INT32 id=RemapGetTunerID(SKIP_ARG,SKIP_ARG);

    switch(RemapFctTbl(e_conn_type=RemapGetConnType(SKIP_ARG,SKIP_ARG)))
    {
        case 0:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetDiagInfo)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Ter.pf_TunerGetDiagInfo(
                        ptTDSpecificCtx, e_tuner_get_type, pv_get_info, pz_size);
                break;
        case 1:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetDiagInfo)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Cab.pf_TunerGetDiagInfo(
                        ptTDSpecificCtx, e_tuner_get_type, pv_get_info, pz_size);
                break;
        case 2:
                if(pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetDiagInfo)
                    return pPdCtxREMAP[id].tLTDIS_Fct_Tbl_Sat.pf_TunerGetDiagInfo(
                        ptTDSpecificCtx, e_tuner_get_type, pv_get_info, pz_size);
                break;
        default:
                mcSHOW_USER_MSG(("unacceptable TunerGetDiagCmd e_conn_type=%d\r\n",e_conn_type));
                break;
    }
    return 0;
}
#endif
//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
void PD_REMAP_Register_LTDIS_Fct(PTD_CTX ptTDCtx)
{
    TUNER_DEVICE_CTX_T*     pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;

    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerClose           = REMAP_TunerClose;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerAcq             = REMAP_TunerAcq;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGerSync         = REMAP_TunerGetSync;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignal       = REMAP_TunerGetSignal;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLevel  = REMAP_TunerGetSignalLevel;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalLeveldBm = REMAP_TunerGetSignalLeveldBm;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalPER    = REMAP_TunerGetSignalPER;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetSignalSNR    = REMAP_TunerGetSignalSNR;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetMpgFmt       = REMAP_TunerSetMpgFmt;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetVer          = REMAP_TunerGetVer;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerNimTest         = REMAP_TunerNimTest;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetRegSetting   = REMAP_TunerSetRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerShowRegSetting  = REMAP_TunerShowRegSetting;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerTestI2C         = REMAP_TunerTestI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerCommand         = REMAP_TunerCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDtdCommand      = REMAP_TunerDtdCommand;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDetachI2C       = REMAP_TunerDetachI2C;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerDisc            = REMAP_TunerDisc;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetAttribute    = REMAP_TunerSetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute    = REMAP_TunerGetAttribute;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerBypassI2C       = REMAP_TunerBypassI2C;
#ifdef fcADD_DIAG_INFO
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerSetDiagCmd      = REMAP_TunerSetDiagCmd;
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetDiagInfo     = REMAP_TunerGetDiagInfo;
#endif
    pTunerDeviceCtx->tLTDIS_Fct_Tbl.pf_TunerGetAttribute2   = REMAP_TunerGetAttribute2;
}

//-------------------------------------------------------------------------
/**
 *  This API is driver entry point. It performs
 *  1. allocate and initialize resource.
 *  2. do demod/tuner initialization.
 *  3. do LTDIS API mapping
 *  @param   ptTDCtx            TunerHAL Context use for LTDIS API mapping
 *  @param   ptTunerCfgInfo     Tuner configuration information.
 *  @param   ptTDSpecificCtx    Pointer to tuner driver context.
 *  @retval  DRVAPI_TUNER_OK    Success
 *  @retval  DRVAPI_TUNER_ERROR Fail
 */
//-------------------------------------------------------------------------

INT32 REMAP_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* ptTDSpecificCtx)
{

    INT32 i4Ret=DRVAPI_TUNER_OK;
    INT32 initialed=0;
    //create pdctx used in this file, you also can add any declaration you want in pdctx

    if(ptTunerCfgInfo->TunerID ==0)
    {
    #ifdef PD_REMAP_TER_ANA
	  x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )
        i4Ret = PDNAME_CALL(PD_REMAP_TER_ANA,Open)(ptTDCtx, ptTunerCfgInfo, ptTDSpecificCtx);
        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_TER_ANA,Open))" fail\r\n"));
            return i4Ret;
        }
        x_memcpy(&pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Ter,
            &((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl ,sizeof(LTDIS_FCT_TBL_T));
        initialed=1;
        pPdCtxREMAP[0].e_conn_type = TUNER_CONN_TYPE_TER_ANA; //init conn_type to TER_ANA   
    #endif

    #ifdef PD_REMAP_CAB_ANA
        {
        pPdCtxREMAP[0].Cab_Ter_Sat=0;
		x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )
        i4Ret = PDNAME_CALL(PD_REMAP_CAB_ANA,Open)(ptTDCtx, ptTunerCfgInfo, ptTDSpecificCtx);
        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_CAB_ANA,Open))" fail\r\n"));
            return i4Ret;
        }
        x_memcpy(&pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Cab,
            &((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl ,sizeof(LTDIS_FCT_TBL_T));
        initialed=1;
        pPdCtxREMAP[0].e_conn_type = TUNER_CONN_TYPE_CAB_ANA; //init conn_type to CAB_ANA
        }
    #endif

    #ifdef PD_REMAP_SAT_ANA
        {
	    x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )		
        pPdCtxREMAP[0].Cab_Ter_Sat=2;
        i4Ret = PDNAME_CALL(PD_REMAP_SAT_ANA,Open)(ptTDCtx, ptTunerCfgInfo, ptTDSpecificCtx);
        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_SAT_ANA,Open))" fail\r\n"));
            return i4Ret;
        }
        x_memcpy(&pPdCtxREMAP[0].tLTDIS_Fct_Tbl_Sat,
            &((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl ,sizeof(LTDIS_FCT_TBL_T));
        initialed=1;
        pPdCtxREMAP[0].e_conn_type = TUNER_CONN_TYPE_SAT_ANA; //init conn_type to SAT_ANA
        }
    #endif
        pPdCtxREMAP[0].initialed=initialed;
    }

     if(ptTunerCfgInfo->TunerID ==1)
    {   
    #ifdef PD_REMAP_TER_DIG     
	  x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )	
        i4Ret = PDNAME_CALL(PD_REMAP_TER_DIG,Open)(ptTDCtx, ptTunerCfgInfo, ptTDSpecificCtx);
        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_TER_DIG,Open))" fail\r\n"));
            return i4Ret;
        }
        x_memcpy(&pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Ter,
            &((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl ,sizeof(LTDIS_FCT_TBL_T));
        initialed=1;
        pPdCtxREMAP[1].e_conn_type = TUNER_CONN_TYPE_TER_DIG; //init conn_type to TER_DIG
    #endif

    #ifdef PD_REMAP_CAB_DIG   
        {       
	    x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )		
        i4Ret = PDNAME_CALL(PD_REMAP_CAB_DIG,Open)(ptTDCtx, ptTunerCfgInfo, ptTDSpecificCtx);       
        if(i4Ret != DRVAPI_TUNER_OK){
            mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_CAB_DIG,Open))" fail\r\n"));
            return i4Ret;
        }
        x_memcpy(&pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Cab,
            &((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl ,sizeof(LTDIS_FCT_TBL_T));
        }
        initialed=1;
        pPdCtxREMAP[1].e_conn_type = TUNER_CONN_TYPE_CAB_DIG; //init conn_type to CAB_DIG
    #endif

#ifdef PD_REMAP_SAT_DIG     
            {           
	       x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )
            i4Ret = PDNAME_CALL(PD_REMAP_SAT_DIG,Open)(ptTDCtx, ptTunerCfgInfo, ptTDSpecificCtx);
            if(i4Ret != DRVAPI_TUNER_OK){
                mcSHOW_USER_CRI_MSG((MKSTR(PDNAME_CALL(PD_REMAP_SAT_DIG,Open))" fail\r\n"));
                return i4Ret;
            }
            x_memcpy(&pPdCtxREMAP[1].tLTDIS_Fct_Tbl_Sat,
                &((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl ,sizeof(LTDIS_FCT_TBL_T));
            }
            initialed=1;
            pPdCtxREMAP[1].e_conn_type = TUNER_CONN_TYPE_SAT_DIG; //init conn_type to SAT_DIG
    #endif

        pPdCtxREMAP[1].initialed=initialed;
    }

    if(!initialed)
    {
        mcSHOW_USER_CRI_MSG(("REMAP_TunerOpen fail, please check defines of PD_REMAP_TER_DIG/ANA\n"));
        return DRVAPI_TUNER_ERROR;
    }
    x_memset(&((TUNER_DEVICE_CTX_T*)ptTDCtx)->tLTDIS_Fct_Tbl, 0, sizeof(LTDIS_FCT_TBL_T));//d120104_Haibo:Reset function table to avoid to effect other function table(The size of funtion table is not different )
    //register LTDIS APIs to HAL
    PD_REMAP_Register_LTDIS_Fct(ptTDCtx);

    return i4Ret;
}

