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
 * $RCSfile: mw_drv_post_init.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "x_start_common.h"
#include "x_os.h"
#include "x_rm.h"


#ifndef LINUX_TURNKEY_SOLUTION
#include "x_rm_dev_types.h"
#include "x_sys_name.h"
#else
#include "u_rm_dev_types.h"
#undef _U_SYS_NAME_H_
#include "u_sys_name.h"
#endif

#include "x_printf.h"
#include "x_timer.h"
#include "x_assert.h"
#include "drv_comp_id.h"
#include "drvcust_if.h"
#include "mw_drv_post_init.h"

#include "TDIS.h"
#include "x_util.h"

#define MW_PRINTF(x...)                                 \
    do {                                                \
        if (DRVCUST_InitGet(eFlagBootVerbose)) {        \
            Printf(x);                                  \
        }                                               \
    } while (0)

#define TRACE(X) \
    do \
    { \
        MW_PRINTF(X); \
    } while (0)

//#define TRACE(X) do {} while (0)

    #define RETURN_INITR_FAIL() \
    do \
    { \
        MW_PRINTF("operation failed\n"); \
        return INITR_FAIL; \
    } while(0)

    #define TRY(X) \
        do \
        { \
            INT32 xret; \
            xret = X; \
            if (xret) \
            { \
                i4Ret = xret; \
                goto _loc_catch; \
            } \
        } while (0)

    #define CATCH(X) \
        _loc_catch:

    //#define TRY(X) X
    //#define CATCH() {}

typedef struct _DRV_MODULE_REG_T
{
    DRV_COMP_REG_T* prComp;
    UINT16 u2Num;
    UINT16 u2Max;
} DRV_MODULE_REG_T;

typedef struct _POST_INIT_INFO_T
{
    void* pvSpace;
    DRV_MODULE_REG_T rTuner;                /* frontend */
    DRV_MODULE_REG_T rAnaTuner;             /* analog tuner */
    DRV_MODULE_REG_T rDmxPcr;               /* demux pcr */
    DRV_MODULE_REG_T rDmxPes;               /* demux pes */
    DRV_MODULE_REG_T rDmxPesMem;            /* demux pes memory */
    DRV_MODULE_REG_T rDmxCntrEs;            /* demux container es*/
    DRV_MODULE_REG_T rDmxCntrMem;           /* demux container mem */
    DRV_MODULE_REG_T rDmxCntrCtrl;          /* demux container control */
    DRV_MODULE_REG_T rDmxSec;               /* demux section */
    DRV_MODULE_REG_T rDmxFlt;               /* demux filter */
    DRV_MODULE_REG_T rDmxDesc;              /* demux descrambler */
    DRV_MODULE_REG_T rTvd;                  /* tv decoder */
    DRV_MODULE_REG_T rMpv;                  /* mpeg video decoder */
    DRV_MODULE_REG_T rAud;                  /* audio decoder */
    DRV_MODULE_REG_T rVdp;                  /* video plane */
    DRV_MODULE_REG_T rOsd;                  /* on screen display */
    DRV_MODULE_REG_T rPmx;                  /* plane mixer */
    DRV_MODULE_REG_T rJpg;                  /* jpeg hardware decoder */
    DRV_MODULE_REG_T rScart;                /* scart */
    DRV_MODULE_REG_T rBagt;                 /* buffer agent */
#ifdef TIME_SHIFT_SUPPORT
    DRV_MODULE_REG_T rRecDmx;               /* record demux */
    DRV_MODULE_REG_T rRecDmxPcr;            /* record demux pcr */
    DRV_MODULE_REG_T rPvrCtrl;              /* pvr control */
    DRV_MODULE_REG_T rPvrStrm;              /* pvr stream */
    DRV_MODULE_REG_T rPvrStrmPcr;           /* pvr stream pcr */ 
#endif  // TIME_SHIFT_SUPPORT
    DRV_MODULE_REG_T rStcCtrl;              /* STC Control */
#ifdef CC_SUPPORT_TVE    
    DRV_MODULE_REG_T rTve;      	        /* tv encoder */
    DRV_MODULE_REG_T rVdout;      	        /* video out */
#endif
    DRV_MODULE_REG_T rGroupMain;            /* main group */
    DRV_MODULE_REG_T rGroupSub;             /* sub group */
    DRV_MODULE_REG_T rGroupAux;             /* aux group */
    DRV_MODULE_REG_T rGroupAuxSund;         /*aux sound group*/
    DRV_MODULE_REG_T rGroupTrdSund;         /*aux sound group*/
#ifdef CC_SUPPORT_TVE    
    DRV_MODULE_REG_T rGroupTvByPass; 		/* tvbypass group */
    DRV_MODULE_REG_T rGroupMonitorByPass;	/* monitorbypass group */
#endif
} POST_INIT_INFO_T;

static INT32 _InitModule(DRV_MODULE_REG_T* prComp, DRV_TYPE_T eDrvType, UINT16 u2StartId, UINT16 u2Num);
static INT32 _RegCompConn(DRV_COMP_REG_T* prOutComp, UINT16 u2OutNum, DRV_COMP_REG_T* prInComp, UINT16 u2InNum, CONN_DIR_TYPE_T eDir);
static INT32 _RegModuleConn(DRV_MODULE_REG_T* prOutModule, DRV_MODULE_REG_T* prInModule, CONN_DIR_TYPE_T eDir);
static INT32 _AddComp2Module(DRV_MODULE_REG_T* prModule, DRV_COMP_REG_T* prComp);
static INT32 _AddModule2Module(DRV_MODULE_REG_T* prDst, DRV_MODULE_REG_T* prSrc);
static INT32 _InitGroupMain(POST_INIT_INFO_T* info);
static INT32 _InitGroupSub(POST_INIT_INFO_T* info);
static INT32 _InitGroupAux(POST_INIT_INFO_T* info);
static INT32 _InitGroupAuxSund(POST_INIT_INFO_T* info);

/* major functions */
POST_INIT_INFO_T *_CreateCompInfo(void);
void _DeleteCompInfo(POST_INIT_INFO_T* info);
static DRV_COMP_REG_T* _InitCompTuner(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompPcr(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompPes(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompPesMem(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
#ifdef ENABLE_MULTIMEDIA
static DRV_COMP_REG_T* _InitCompCntrEs(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompCntrMem(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompCntrCtrl(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
#endif
static DRV_COMP_REG_T* _InitCompSec(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompDesc(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);

static DRV_COMP_REG_T* _InitCompFlt(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompMpv(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompAud(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompVdp(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompPmx(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompOsd(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompJpg(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompScart_0(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompBagt(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
#ifdef CC_SUPPORT_TVE
static DRV_COMP_REG_T* _InitCompTve(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
static DRV_COMP_REG_T* _InitCompVdout(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart);
#endif /*CC_SUPPORT_TVE*/

static INT32 _InitCompInfo(POST_INIT_INFO_T* info);
static INT32 _RegConnLists(POST_INIT_INFO_T* info);
static INT32 _RegCompExclLists(POST_INIT_INFO_T* info);
static INT32 _RegConnExclLists(POST_INIT_INFO_T* info);
static INT32 _RegGroups(POST_INIT_INFO_T* info);

/* implementation */
static INT32 _InitModule(DRV_MODULE_REG_T* prModule, DRV_TYPE_T eDrvType, UINT16 u2StartId, UINT16 u2Num)
{
    DRV_COMP_REG_T *prComp;
    UINT16 u2Idx;

    if (prModule == NULL)
    {
        RETURN_INITR_FAIL();
    }

    prComp = prModule->prComp;
    for (u2Idx = 0; u2Idx < u2Num; u2Idx++)
    {
        prComp[u2Idx].e_type            = eDrvType;
        prComp[u2Idx].e_id_type         = ID_TYPE_IND;
        prComp[u2Idx].u.t_ind.pv_tag    = NULL;
        prComp[u2Idx].u.t_ind.ui1_port  = ALL_PORTS;
        prComp[u2Idx].u.t_ind.ui2_id    = u2StartId + u2Idx;
    }
    prModule->u2Max = u2Num;
    prModule->u2Num = u2Num;
    return INITR_OK;
}

static INT32 _RegCompConn(DRV_COMP_REG_T* prOutComp, UINT16 u2OutNum, DRV_COMP_REG_T* prInComp, UINT16 u2InNum, CONN_DIR_TYPE_T eDir)
{
    INT32 i4Ret;
    UINT16 u2Loop;

    if (prOutComp   != NULL &&
        prInComp    != NULL &&
        u2OutNum    >  0    &&
        u2InNum     >  0)
    {
        for (u2Loop = 0; u2Loop < u2InNum; u2Loop++)
        {
            i4Ret = x_rm_reg_conn_list(&prInComp[u2Loop], prOutComp, u2OutNum, eDir, FALSE);
            if (i4Ret)
            {
                return i4Ret;
            }
        }
        return INITR_OK;
    }
    RETURN_INITR_FAIL();
}

static INT32 _RegModuleConn(DRV_MODULE_REG_T* prOutModule, DRV_MODULE_REG_T* prInModule, CONN_DIR_TYPE_T eDir)
{
    if (prOutModule != NULL &&
        prInModule  != NULL)
    {
        return _RegCompConn(prOutModule->prComp, prOutModule->u2Num,
                            prInModule->prComp,  prInModule->u2Num,
                            eDir);
    }
    RETURN_INITR_FAIL();
}

static INT32 _AddComp2Module(DRV_MODULE_REG_T* prModule, DRV_COMP_REG_T* prComp)
{
    if (prModule            != NULL &&
        prModule->prComp    != NULL &&
        prComp              != NULL)
    {
        if (prModule->u2Num < prModule->u2Max)
        {
            x_memcpy(&prModule->prComp[prModule->u2Num], prComp, sizeof(DRV_COMP_REG_T));
            prModule->u2Num++;
            return INITR_OK;
        }
    }
    RETURN_INITR_FAIL();
}

static INT32 _AddModule2Module(DRV_MODULE_REG_T* prDst, DRV_MODULE_REG_T* prSrc)
{
    if (prDst           != NULL &&
        prSrc           != NULL &&
        prDst->prComp   != NULL &&
        prSrc->prComp   != NULL)
    {
        UINT16 u2Loop;
        INT32 i4Ret;
        for (u2Loop = 0; u2Loop < prSrc->u2Num; u2Loop++)
        {
            //MW_PRINTF("\tadd loop(%d) comp[%d:%d]\n", u2Loop, prSrc->prComp[u2Loop].e_type, prSrc->prComp[u2Loop].u.t_ind.ui2_id);
            i4Ret = _AddComp2Module(prDst, &prSrc->prComp[u2Loop]);
            if (i4Ret)
            {
                TRACE("Error when add component to module\n");
                return i4Ret;
            }
        }
        return INITR_OK;
    }
    RETURN_INITR_FAIL();
}

static INT32 _InitGroupMain(POST_INIT_INFO_T* info)
{
    /* main group contains [all tuner] [pcr0] [all mpv] [aud0] [vdp0] [pmx0] [tvd0] */
    INT32 i4Ret;
    DRV_MODULE_REG_T* prMain;

    if (info->rTuner.u2Num    == 0 ||
        info->rDmxPcr.u2Num   == 0 ||
        info->rMpv.u2Num      == 0 ||
        info->rAud.u2Num      == 0 ||
        info->rVdp.u2Num      == 0 ||
        info->rPmx.u2Num      == 0 ||
        info->rTvd.u2Num      == 0 )
    {
        TRACE("Main group init failed when init\n");
        RETURN_INITR_FAIL();
    }

    /* main group */
    prMain = &info->rGroupMain;
    /* all tuner */
    i4Ret = _AddModule2Module(prMain, &info->rTuner);
    if (i4Ret)
    {
        TRACE("Main group init failed when add tuner\n");
        return i4Ret;
    }
    /* pcr0 */
    i4Ret = _AddComp2Module  (prMain, &info->rDmxPcr.prComp[0]);
    if (i4Ret)
    {
        TRACE("Main group init failed when add pcr\n");
        return i4Ret;
    }
    /* mpv 4 */
    //i4Ret = _AddModule2Module(prMain, &info->rMpv);
    i4Ret = _AddComp2Module(prMain, &info->rMpv.prComp[4]);
    if (i4Ret)
    {
        TRACE("Main group init failed when add mpv\n");
        return i4Ret;
    }
    /* aud0 */
    i4Ret = _AddComp2Module  (prMain, &info->rAud.prComp[0]);
    if (i4Ret)
    {
        TRACE("Main group init failed when add aud\n");
        return i4Ret;
    }
    /* vdp0 */
    i4Ret = _AddComp2Module  (prMain, &info->rVdp.prComp[0]);
    if (i4Ret)
    {
        TRACE("Main group init failed when add vdp\n");
        return i4Ret;
    }
    /* pmx0 */
    i4Ret = _AddComp2Module  (prMain, &info->rPmx.prComp[0]);
    if (i4Ret)
    {
        TRACE("Main group init failed when add pmx\n");
        return i4Ret;
    }
    /* tvd0 */
    i4Ret = _AddComp2Module (prMain, &info->rTvd.prComp[0]);
    if (i4Ret)
    {
        TRACE("Main group init failed when add tvd\n");
        return i4Ret;
    }

    return INITR_OK;
}

static INT32 _InitGroupSub(POST_INIT_INFO_T* info)
{
    /* sub group contains [all tuner] [pcr1] [all mpv] [aud0] [vdp1] [pmx0] [tvd1] */
    INT32 i4Ret;
    DRV_MODULE_REG_T* prSub;

    if (info->rVdp.u2Num <= 1 ||
        info->rDmxPcr.u2Num <= 1)
    {
        info->rGroupSub.u2Num = 0;
        return INITR_OK;
    }

    /* sub group */
    prSub = &info->rGroupSub;
    /* all tuner */
    i4Ret = _AddModule2Module(prSub, &info->rTuner);
    if (i4Ret)
    {
        return i4Ret;
    }
    /* pcr1 */
    i4Ret = _AddComp2Module  (prSub, &info->rDmxPcr.prComp[1]);
    if (i4Ret)
    {
        return i4Ret;
    }
#if defined(CC_3D_MM_DS_SUPPORT) || defined(CC_FBM_TWO_FBP)    /* all mpv */
    i4Ret = _AddComp2Module (prSub, &info->rMpv.prComp[2]);
#else
    i4Ret = _AddModule2Module(prSub, &info->rMpv);
#endif
    if (i4Ret)
    {
        return i4Ret;
    }
    /* aud0 */
    i4Ret = _AddComp2Module  (prSub, &info->rAud.prComp[0]);
    if (i4Ret)
    {
        return i4Ret;
    }
    /* vdp1 */
    i4Ret = _AddComp2Module  (prSub, &info->rVdp.prComp[1]);
    if (i4Ret)
    {
        return i4Ret;
    }
    /* pmx0 */
    i4Ret = _AddComp2Module  (prSub, &info->rPmx.prComp[0]);
    if (i4Ret)
    {
        return i4Ret;
    }
    /* tvd1 */
    i4Ret = _AddComp2Module (prSub, &info->rTvd.prComp[1]);
    if (i4Ret)
    {
        return i4Ret;
    }

    return INITR_OK;
}

static INT32 _InitGroupAux(POST_INIT_INFO_T* info)
{
    return INITR_OK;
}

static INT32 _InitGroupAuxSund(POST_INIT_INFO_T* info)
{    
    /* AuxSund group contains  [pcr1] [aud1]/[aud2] */    
    INT32 i4Ret;    DRV_MODULE_REG_T* prAuxSund;    /* AuxSund group */    
    
    prAuxSund = &info->rGroupAuxSund;         
    
    if (info->rAud.u2Num <= 1 ||        info->rDmxPcr.u2Num <= 1)    
    { 
        info->rGroupAuxSund.u2Num = 0;         
        return INITR_OK;    
    }
    
    /* pcr1 */    
    i4Ret = _AddComp2Module(prAuxSund, &info->rDmxPcr.prComp[1]);    
    if (i4Ret)    
    { 
       return i4Ret;
    } 
    /*AuxSund always map to aud[2]*/
    i4Ret = _AddComp2Module(prAuxSund, &info->rAud.prComp[2]);
#if 0  
  #ifdef CC_MT5391_AUD_3_DECODER
    /* aud2 */
    i4Ret = _AddComp2Module(prAuxSund, &info->rAud.prComp[2]);
  #else
    /* aud1 */
    i4Ret = _AddComp2Module(prAuxSund, &info->rAud.prComp[1]);
  #endif /* CC_MT5391_AUD_3_DECODER */
#endif  
    if (i4Ret)   
    {    
        return i4Ret;  
    }   
    return INITR_OK;
}

static INT32 _InitGroupTrdSund(POST_INIT_INFO_T* info)
{    
    /* AuxSund group contains  [pcr1] [aud1]/[aud2] */    
    INT32 i4Ret;    DRV_MODULE_REG_T* prTrdSund;    /* AuxSund group */    
    
    prTrdSund = &info->rGroupTrdSund;         
    
    if (info->rAud.u2Num <= 1 ||        info->rDmxPcr.u2Num <= 1)    
    { 
        info->rGroupTrdSund.u2Num = 0;         
        return INITR_OK;    
    }
    
    /* pcr1 */    
    i4Ret = _AddComp2Module(prTrdSund, &info->rDmxPcr.prComp[1]);    
    if (i4Ret)    
    { 
       return i4Ret;
    } 
    i4Ret = _AddComp2Module(prTrdSund, &info->rAud.prComp[0]);
    if (i4Ret)   
    {    
        return i4Ret;  
    }   
    return INITR_OK;
}



#ifdef CC_SUPPORT_TVE
static INT32 _InitGroupTvByPass(POST_INIT_INFO_T* info)
{
	/*TvByPass group contains [All tuner] [pcr0] [vd4] [ad1] [tvd3] [tve0] [scart0] */
    INT32 i4Ret;
    DRV_MODULE_REG_T* prTvByPass;

	if (info->rTuner.u2Num    == 0 ||
        info->rDmxPcr.u2Num   == 0 ||
        info->rMpv.u2Num      == 0 ||
        info->rAud.u2Num      == 0 ||
        info->rTvd.u2Num	  == 0 ||
        info->rTve.u2Num	  == 0 ||
        info->rVdout.u2Num	  == 0 )
    {
        TRACE("TvByPass group init failed when init\n");
        RETURN_INITR_FAIL();
    }

    /* TvByPass group */
    prTvByPass = &info->rGroupTvByPass;
    /* all tuner */
    i4Ret = _AddModule2Module(prTvByPass, &info->rTuner);
    if (i4Ret)
    {
        TRACE("TvByPass group init failed when add tuner\n");
        return i4Ret;
    }
    /* pcr0 */
    i4Ret = _AddComp2Module(prTvByPass, &info->rDmxPcr.prComp[0]);
    if (i4Ret)
    {
        TRACE("TvByPass group init failed when add pcr\n");
        return i4Ret;
    }
    /* mpv 4 */
    i4Ret = _AddComp2Module(prTvByPass, &info->rMpv.prComp[4]);
    if (i4Ret)
    {
        TRACE("TvByPass group init failed when add mpv\n");
        return i4Ret;
    }
    /* aud1 */
    i4Ret = _AddComp2Module(prTvByPass, &info->rAud.prComp[1]);
    if (i4Ret)
    {
        TRACE("TvByPass group init failed when add aud\n");
        return i4Ret;
    }    
    /* tvd3 */
    i4Ret = _AddComp2Module(prTvByPass, &info->rTvd.prComp[3]);
    if (i4Ret)
    {
        TRACE("TvByPass group init failed when add tvd\n");
        return i4Ret;
    }

    /* tve0 */
    i4Ret = _AddComp2Module(prTvByPass, &info->rTve.prComp[0]);
    if (i4Ret)
    {
        TRACE("TvByPass group init failed when add tvd\n");
        return i4Ret;
    }
    
    /* scart0 */
    i4Ret = _AddComp2Module(prTvByPass, &info->rVdout.prComp[0]);
     if (i4Ret)   
     {    
        TRACE("TvByPass group init failed when add tvd\n");
        return i4Ret;
    }

    return INITR_OK;
}

static INT32 _InitGroupMonitorByPass(POST_INIT_INFO_T* info)
{
	/*MonitorByPass group contains [All tuner] [pcr0] [vd4] [ad0] [tvd2] [tve0] [scart1] */
    INT32 i4Ret;
    DRV_MODULE_REG_T* prMonitorByPass;

	if (info->rTuner.u2Num    == 0 ||
        info->rDmxPcr.u2Num   == 0 ||
        info->rMpv.u2Num      == 0 ||
        info->rAud.u2Num      == 0 ||
        info->rTvd.u2Num	  == 0 ||
        info->rTve.u2Num	  == 0 ||
        info->rVdout.u2Num	  == 0 )
    {
        TRACE("MonitorByPass group init failed when init\n");
        RETURN_INITR_FAIL();
    }

    /* TvByPass group */
    prMonitorByPass = &info->rGroupMonitorByPass;
    /* all tuner */
    i4Ret = _AddModule2Module(prMonitorByPass, &info->rTuner);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add tuner\n");
        return i4Ret;
    }
    /* pcr0 */
    i4Ret = _AddComp2Module(prMonitorByPass, &info->rDmxPcr.prComp[0]);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add pcr\n");
        return i4Ret;
    }
    /* mpv 4 */
    i4Ret = _AddComp2Module(prMonitorByPass, &info->rMpv.prComp[4]);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add mpv\n");
        return i4Ret;
    }
    /* aud0 */
    i4Ret = _AddComp2Module(prMonitorByPass, &info->rAud.prComp[0]);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add aud\n");
        return i4Ret;
    }    
    /* tvd2 */
    i4Ret = _AddComp2Module(prMonitorByPass, &info->rTvd.prComp[2]);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add tvd\n");
        return i4Ret;
    }

    /* tve1 */
    i4Ret = _AddComp2Module(prMonitorByPass, &info->rTve.prComp[1]);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add tvd\n");
         return i4Ret;  
     }   
    
    /* scart1 */
    i4Ret = _AddComp2Module(prMonitorByPass, &info->rVdout.prComp[1]);
    if (i4Ret)
    {
        TRACE("MonitorByPass group init failed when add tvd\n");
        return i4Ret;
    }

     return INITR_OK;
}
#endif /* CC_SUPPORT_TVE */

POST_INIT_INFO_T *_CreateCompInfo(void)
{
    /* N_DRVC_ALL, N_GRP_ALL */
    POST_INIT_INFO_T* info;
    /* create post init data instance */
    info = (POST_INIT_INFO_T*) x_mem_alloc(sizeof(POST_INIT_INFO_T));
    if (info == NULL)
    {
        return (POST_INIT_INFO_T*)NULL;
    }
    x_memset(info, 0, sizeof(POST_INIT_INFO_T));
    /* allocate space for all connected driver reg data */
    info->pvSpace = (void*) x_mem_alloc(
        sizeof(DRV_COMP_REG_T) * (N_DRVC_ALL + N_GRP_ALL));
    if (info->pvSpace == NULL)
    {
        x_mem_free(info);
        return (POST_INIT_INFO_T*)NULL;
    }
    x_memset(info->pvSpace, 0,
        sizeof(DRV_COMP_REG_T) * (N_DRVC_ALL + N_GRP_ALL));
    return info;
}

void _DeleteCompInfo(POST_INIT_INFO_T* info)
{
    if (info)
    {
        if (info->pvSpace)
        {
            x_mem_free(info->pvSpace);
        }
        x_mem_free(info);
    }
}

static DRV_COMP_REG_T* _InitCompTuner(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    TUNER_CONFIG_TBL_T      tTunerCfgTbl;
    DRV_COMP_REG_T*         prNext;
    UINT16                  i, k;

    TDIS_RetrieveTunerCfgTbl(&tTunerCfgTbl);

    if (tTunerCfgTbl.TunerNum > MAX_TUNER_NUM)
    {
        ASSERT(tTunerCfgTbl.TunerNum <= MAX_TUNER_NUM);
        tTunerCfgTbl.TunerNum = MAX_TUNER_NUM;
    }

    info->rTuner.prComp = prStart;
    k = 0;
    for (i=0;i<tTunerCfgTbl.TunerNum;i++){
        if (tTunerCfgTbl.tTunerCfg[i].i2TunerCapability & cWITH_DRVT_TUNER_TER_DIG){
            info->rTuner.prComp[k].e_type           = DRVT_TUNER_TER_DIG;
            info->rTuner.prComp[k].e_id_type        = ID_TYPE_IND;
            info->rTuner.prComp[k].u.t_ind.pv_tag   = NULL;
            info->rTuner.prComp[k].u.t_ind.ui1_port = ALL_PORTS;
            info->rTuner.prComp[k].u.t_ind.ui2_id   = tTunerCfgTbl.tTunerCfg[i].TunerID;
            k++;
        }
        if (tTunerCfgTbl.tTunerCfg[i].i2TunerCapability & cWITH_DRVT_TUNER_CAB_DIG){
            info->rTuner.prComp[k].e_type           = DRVT_TUNER_CAB_DIG;
            info->rTuner.prComp[k].e_id_type        = ID_TYPE_IND;
            info->rTuner.prComp[k].u.t_ind.pv_tag   = NULL;
            info->rTuner.prComp[k].u.t_ind.ui1_port = ALL_PORTS;
            info->rTuner.prComp[k].u.t_ind.ui2_id   = tTunerCfgTbl.tTunerCfg[i].TunerID;
            k++;
        }
        if (tTunerCfgTbl.tTunerCfg[i].i2TunerCapability & cWITH_DRVT_TUNER_SAT_DIG){
            info->rTuner.prComp[k].e_type           = DRVT_TUNER_SAT_DIG;
            info->rTuner.prComp[k].e_id_type        = ID_TYPE_IND;
            info->rTuner.prComp[k].u.t_ind.pv_tag   = NULL;
            info->rTuner.prComp[k].u.t_ind.ui1_port = ALL_PORTS;
            info->rTuner.prComp[k].u.t_ind.ui2_id   = tTunerCfgTbl.tTunerCfg[i].TunerID;
            k++;
        }
    }
    info->rTuner.u2Num = k;
    info->rTuner.u2Max = k;

    prNext = &info->rTuner.prComp[info->rTuner.u2Num];
    return prNext;
}

static DRV_COMP_REG_T* _InitCompAnaTuner(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rAnaTuner.prComp = prStart;

    /* analog tuner - terristral */
    prStart->e_type             = DRVT_TUNER_TER_ANA;
    prStart->e_id_type          = ID_TYPE_IND;
    prStart->u.t_ind.pv_tag     = NULL;
    prStart->u.t_ind.ui1_port   = ALL_PORTS;
    prStart->u.t_ind.ui2_id     = 0;

    /* analog tuner - cable */
    prStart++;
    prStart->e_type             = DRVT_TUNER_CAB_ANA;
    prStart->e_id_type          = ID_TYPE_IND;
    prStart->u.t_ind.pv_tag     = NULL;
    prStart->u.t_ind.ui1_port   = ALL_PORTS;
    prStart->u.t_ind.ui2_id     = 0;

    info->rAnaTuner.u2Num = 2;  /* ter + cab */
    info->rAnaTuner.u2Max = 2;  /* ter + cab */

    prNext = &info->rAnaTuner.prComp[info->rAnaTuner.u2Num];
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPcr(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxPcr.prComp = prStart;
    _InitModule(&info->rDmxPcr, DRVT_DEMUX_TS_PCR, DMX_PCR_COMP_ID_START, N_DRVC_PCR);
    prNext = prStart + info->rDmxPcr.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPes(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxPes.prComp = prStart;
    _InitModule(&info->rDmxPes, DRVT_DEMUX_TS_PES_PACKET, DMX_PES_COMP_ID_START, N_DRVC_PES);
    prNext = prStart + info->rDmxPes.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPesMem(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxPesMem.prComp = prStart;
    _InitModule(&info->rDmxPesMem, DRVT_DEMUX_TS_PES_PACKET_MEMORY, DMX_PES_MEM_COMP_ID_START, N_DRVC_PES_MEM);
    prNext = prStart + info->rDmxPesMem.u2Num;
    return prNext;
}

#ifdef ENABLE_MULTIMEDIA
static DRV_COMP_REG_T* _InitCompCntrEs(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxCntrEs.prComp = prStart;
    _InitModule(&info->rDmxCntrEs, DRVT_DEMUX_CONTAINER_ES, DMX_MM_CONTAINER_ES_COMP_ID_START, N_DRVC_CTNR_ES);
    prNext = prStart + info->rDmxCntrEs.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompCntrMem(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxCntrMem.prComp = prStart;
    _InitModule(&info->rDmxCntrMem, DRVT_DEMUX_CONTAINER_ES_MEMORY, DMX_MM_CONTAINER_ES_MEM_COMP_ID_START, N_DRVC_CTNR_ES_MEM);
    prNext = prStart + info->rDmxCntrMem.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompCntrCtrl(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxCntrCtrl.prComp = prStart;
    _InitModule(&info->rDmxCntrCtrl, DRVT_DEMUX_CONTROL, DMX_MM_CONTROL_COMP_ID_START, N_DRVC_CTNR_CTRL);
    prNext = prStart + info->rDmxCntrCtrl.u2Num;
    return prNext;

}
#ifdef TIME_SHIFT_SUPPORT
static DRV_COMP_REG_T* _InitCompRecDmx(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rRecDmx.prComp = prStart;    
    _InitModule(&info->rRecDmx, DRVT_DEMUX_REC, REC_DMX_COMP_ID_START, REC_DMX_COMP_NUM);
    prNext = prStart + info->rRecDmx.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompRecPcrDmx(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rRecDmxPcr.prComp = prStart;    
    _InitModule(&info->rRecDmxPcr, DRVT_DEMUX_REC_PCR, REC_DMX_PCR_COMP_ID_START, REC_DMX_PCR_COMP_NUM);
    prNext = prStart + info->rRecDmxPcr.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPvrCtrl(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rPvrCtrl.prComp = prStart;
    _InitModule(&info->rPvrCtrl, DRVT_PVR_CTRL,
                PVR_CTRL_COMP_ID_START, PVR_CTRL_COMP_NUM);
    prNext = prStart + info->rPvrCtrl.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPvrStrm(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rPvrStrm.prComp = prStart;
    _InitModule(&info->rPvrStrm, DRVT_PVR_STREAM,
                PVR_STREAM_COMP_ID_START, PVR_STREAM_COMP_NUM);
    prNext = prStart + info->rPvrStrm.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPvrStrmPcr(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rPvrStrmPcr.prComp = prStart;
    _InitModule(&info->rPvrStrmPcr, DRVT_PVR_STREAM_PCR,
                PVR_STREAM_PCR_COMP_ID_START, PVR_STREAM_PCR_COMP_NUM);
    prNext = prStart + info->rPvrStrmPcr.u2Num;
    return prNext;
}
#endif  // TIME_SHIFT_SUPPORT
#endif

static DRV_COMP_REG_T* _InitCompSec(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxSec.prComp = prStart;
    _InitModule(&info->rDmxSec, DRVT_DEMUX_TS_SECTION_MEMORY, DMX_SEC_COMP_ID_START, N_DRVC_SEC);
    prNext = prStart + info->rDmxSec.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompDesc(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxDesc.prComp = prStart;
    _InitModule(&info->rDmxDesc, DRVT_DESCRAMBLER, DMX_PES_COMP_ID_START, N_DRVC_PES);
    prNext = prStart + info->rDmxDesc.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompTvd(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rTvd.prComp = prStart;
    _InitModule(&info->rTvd, DRVT_TV_DEC, TVD_COMP_ID_START, N_DRVC_TVD);
    prNext = prStart + info->rTvd.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompFlt(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rDmxFlt.prComp = prStart;
    _InitModule(&info->rDmxFlt, DRVT_DEMUX_SECTION_FILTER, DMX_FLT_COMP_ID_START, N_DRVC_FLT);
    prNext = prStart + info->rDmxFlt.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompMpv(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rMpv.prComp = prStart;
    _InitModule(&info->rMpv, DRVT_VID_DEC, MPV_COMP_1, N_DRVC_MPV);
    prNext = prStart + info->rMpv.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompAud(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rAud.prComp = prStart;
    _InitModule(&info->rAud, DRVT_AUD_DEC, AUD_COMP_1, N_DRVC_AUD);
    prNext = prStart + info->rAud.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompVdp(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rVdp.prComp = prStart;
    _InitModule(&info->rVdp, DRVT_VID_PLANE, VDP_COMP_1, N_DRVC_VDP);
    prNext = prStart + info->rVdp.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompPmx(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rPmx.prComp = prStart;
    _InitModule(&info->rPmx, DRVT_PLA_MXR, PMX_COMP_1, N_DRVC_PMX);
    prNext = prStart + info->rPmx.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompOsd(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rOsd.prComp = prStart;
    _InitModule(&info->rOsd, DRVT_OSD_PLANE, OSD_COMP_1, N_DRVC_OSD);
    prNext = prStart + info->rOsd.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompJpg(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rJpg.prComp = prStart;
    _InitModule(&info->rJpg, DRVT_JPG_DEC, JPG_COMP_1, N_DRVC_JPG);
    prNext = prStart + info->rJpg.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompScart_0(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rScart.prComp = prStart;
    _InitModule(&info->rScart, DRVT_AVC_SCART, SCART_COMP_1, N_DRVC_SCART);
    prNext = prStart + info->rScart.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompBagt(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rBagt.prComp = prStart;
    _InitModule(&info->rBagt, DRVT_BUF_AGENT, BAGT_COMP_1, N_DRVC_BAGT);
    prNext = prStart + info->rBagt.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompStcCtrl(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rStcCtrl.prComp = prStart;
    _InitModule(&info->rStcCtrl, DRVT_STC_CTRL, STC_CTRL_COMP_1, N_DRVC_STC_CTRL);
    prNext = prStart + info->rStcCtrl.u2Num;
    return prNext;
}

#ifdef CC_SUPPORT_TVE
static DRV_COMP_REG_T* _InitCompTve(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rTve.prComp = prStart;
    _InitModule(&info->rTve, DRVT_TV_ENC, TVENC_COMP_1, N_DRVC_TVE);
    prNext = prStart + info->rTve.u2Num;
    return prNext;
}

static DRV_COMP_REG_T* _InitCompVdout(POST_INIT_INFO_T* info, DRV_COMP_REG_T* prStart)
{
    DRV_COMP_REG_T* prNext;
    info->rVdout.prComp = prStart;
    _InitModule(&info->rVdout, DRVT_VIDEO_OUT, VDO_OUT_COMP_1, N_DRVC_VIDEO_OUT);
    prNext = prStart + info->rVdout.u2Num;
    return prNext;
}
#endif /* CC_SUPPORT_TVE */

static INT32 _InitCompInfo(POST_INIT_INFO_T* info)
{
    DRV_COMP_REG_T* prArray;
    if (info            == NULL ||
        info->pvSpace   == NULL)
    {
        /* null info address */
        RETURN_INITR_FAIL();
    }

    prArray = (DRV_COMP_REG_T*)info->pvSpace;
    /* tuner */
    prArray = _InitCompTuner(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* analog tuner */
    prArray = _InitCompAnaTuner(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* demux pcr */
    prArray = _InitCompPcr(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* demux pes */
    prArray = _InitCompPes(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* demux pes memory */
    prArray = _InitCompPesMem(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }

    /* demux container es */
#ifdef ENABLE_MULTIMEDIA
    prArray = _InitCompCntrEs(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
#endif

    /* demux container es memory*/
#ifdef ENABLE_MULTIMEDIA
    prArray = _InitCompCntrMem(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
#endif

    /* demux container control */
    
#ifdef ENABLE_MULTIMEDIA
    prArray = _InitCompCntrCtrl(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
#endif

    /* demux sec */
    prArray = _InitCompSec(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* descrambler sec */
    prArray = _InitCompDesc(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* demux flt */
    prArray = _InitCompFlt(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* tvd */
    prArray = _InitCompTvd(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* mpv */
    prArray = _InitCompMpv(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* aud */
    prArray = _InitCompAud(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* vdp */
    prArray = _InitCompVdp(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* pmx */
    prArray = _InitCompPmx(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* osd */
    prArray = _InitCompOsd(info, prArray);
    if (prArray == NULL)
    {
        RETURN_INITR_FAIL();
    }
    /* jpg */
    prArray = _InitCompJpg(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }
    /* scart */
    prArray = _InitCompScart_0(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }   
    /* buffer agent */
    prArray = _InitCompBagt(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }   

#ifdef TIME_SHIFT_SUPPORT
    /* record demux */
    prArray = _InitCompRecDmx(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }

    prArray = _InitCompRecPcrDmx(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }

    /* pvr control */
    prArray = _InitCompPvrCtrl(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }

    /* pvr stream */
    prArray = _InitCompPvrStrm(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }

    /* pvr stream pcr */
    prArray = _InitCompPvrStrmPcr(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }    
#endif  // TIME_SHIFT_SUPPORT

    /* stc control */
    prArray = _InitCompStcCtrl(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }   

#ifdef CC_SUPPORT_TVE
    /* tv encoder */
    prArray = _InitCompTve(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }
	/* video out */
    prArray = _InitCompVdout(info, prArray);
    if (prArray == NULL)
    {
        return INITR_FAIL;
    }
#endif /*CC_SUPPORT_TVE*/

    info->rGroupMain.prComp = prArray;
    info->rGroupMain.u2Max  = N_GRP_MAIN;
    prArray += N_GRP_MAIN;

    info->rGroupSub.prComp  = prArray;
    info->rGroupSub.u2Max   = N_GRP_SUB;
    prArray += N_GRP_SUB;

    info->rGroupAux.prComp  = prArray;
    info->rGroupAux.u2Max   = N_GRP_AUX;
    prArray += N_GRP_AUX;

    info->rGroupAuxSund.prComp  = prArray;    
    info->rGroupAuxSund.u2Max   = N_GRP_AUX_SUND;    
    prArray += N_GRP_AUX_SUND;   

   info->rGroupTrdSund.prComp  = prArray;    
    info->rGroupTrdSund.u2Max   = N_GRP_TRD_SUND;    
    prArray += N_GRP_TRD_SUND;   
 

#ifdef CC_SUPPORT_TVE    
    info->rGroupTvByPass.prComp = prArray;
    info->rGroupTvByPass.u2Max  = N_GRP_TVBYPASS;
    prArray += N_GRP_TVBYPASS;
    
    info->rGroupMonitorByPass.prComp = prArray;
    info->rGroupMonitorByPass.u2Max  = N_GRP_MONITORBYPASS;
    prArray += N_GRP_MONITORBYPASS;
#endif /*CC_SUPPORT_TVE*/
    
    return INITR_OK;
}

static INT32 _RegConnLists(POST_INIT_INFO_T* info)
{
    INT32 i4Ret = INITR_OK;
    /* analog tuner to tvd & aud */
    TRACE("Register ana tuner <--> tvd\n");
    i4Ret = _RegModuleConn(&info->rAnaTuner, &info->rTvd, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("ana tuner<-->tvd failed\n");
    }

    TRACE("Register ana tuner <--> aud\n");
    i4Ret = _RegModuleConn(&info->rAnaTuner, &info->rAud, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("ana tuner<-->aud failed\n");
    }

    #ifdef NEW_COUNTRY_TYPE
    if (GetTargetCountryMask(COUNTRY_ATV_MASK) == COUNTRY_ATV_EU)
    #else
    #ifdef COUNTRY_FROM_EEPROM
    if (((GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS) == COUNTRY_EU)
    #else
    if (GetTargetCountry() == COUNTRY_EU)
    #endif
    #endif
    {
        /* scart connects to analog tuner and tvd */
        TRACE("Register ana tuner <--> scart\n");
        i4Ret = _RegModuleConn(&info->rAnaTuner, &info->rScart, CONN_DIR_INP_TO_OUT);
        if (i4Ret != INITR_OK)
        {
            TRACE("ana tuner<-->scart failed\n");
        }
            
        TRACE("Register scart <--> aud\n");
        i4Ret = _RegModuleConn(&info->rScart, &info->rAud, CONN_DIR_INP_TO_OUT);
        if (i4Ret != INITR_OK)
        {
            TRACE("ana tuner<-->aud failed\n");
        }

        TRACE("scart <--> tvd\n");
        i4Ret = _RegModuleConn(&info->rScart, &info->rTvd, CONN_DIR_INP_TO_OUT);
        if (i4Ret != INITR_OK)
        {
            TRACE("scart<-->tvd failed\n");
        }
    }

#ifdef CC_SUPPORT_TVE
	/*Video out connects to TVE*/
	TRACE("Register tve0 <--> video out 0\n");
	TRY((x_rm_reg_conn_list(&info->rTve.prComp[0], &info->rVdout.prComp[0], 1, CONN_DIR_OUT_TO_INP, FALSE)));
	                 
	TRACE("Register tve1 <--> video out 1\n");
	TRY((x_rm_reg_conn_list(&info->rTve.prComp[1], &info->rVdout.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
#endif /* CC_SUPPORT_TVE */

    TRACE("Register buffer agent <--> aud\n");
    i4Ret = _RegModuleConn(&info->rBagt, &info->rAud, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("buffer agent<-->aud failed\n");
    }

    TRACE("buffer agent<-->mpv4 failed\n");            
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rBagt.prComp[0], info->rBagt.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
#if defined(CC_3D_MM_DS_SUPPORT) || defined(CC_FBM_TWO_FBP)
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rBagt.prComp[0], info->rBagt.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rBagt.prComp[1], info->rBagt.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rVdp.prComp[1], &info->rBagt.prComp[1], info->rBagt.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rBagt.prComp[2], info->rBagt.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rVdp.prComp[1], &info->rBagt.prComp[2], info->rBagt.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));    
#endif
     /* demux pcr,pes,sec connect to tuner */
    TRACE("Register tuner <--> pcr\n");
    TRY(_RegModuleConn(&info->rTuner, &info->rDmxPcr, CONN_DIR_INP_TO_OUT));
    TRACE("Register tuner <--> pes\n");
    TRY(_RegModuleConn(&info->rTuner, &info->rDmxPes, CONN_DIR_INP_TO_OUT));
    if(info->rDmxPesMem.u2Num != 0)
    {
        TRACE("Register tuner <--> pes_mem\n");
        TRY(_RegModuleConn(&info->rTuner, &info->rDmxPesMem, CONN_DIR_INP_TO_OUT));
    }
    TRACE("Register tuner <--> sec\n");
    TRY(_RegModuleConn(&info->rTuner, &info->rDmxSec, CONN_DIR_INP_TO_OUT));
    
    /* demux connect to buffer agent */
#ifdef ENABLE_MULTIMEDIA
#ifdef TIME_SHIFT_SUPPORT
    TRACE("Register buffer agent <--> pcr\n");
    TRY(_RegModuleConn(&info->rBagt, &info->rDmxPcr, CONN_DIR_INP_TO_OUT));
#endif  // TIME_SHIFT_SUPPORT
    if(info->rDmxPesMem.u2Num != 0)
    {
        TRACE("Register buffer agent <--> pes_mem\n");
        TRY(_RegModuleConn(&info->rBagt, &info->rDmxPesMem, CONN_DIR_INP_TO_OUT));
    }
    TRACE("Register buffer agent <--> sec\n");
    TRY(_RegModuleConn(&info->rBagt, &info->rDmxSec, CONN_DIR_INP_TO_OUT));
#endif
    TRACE("Register buffer agent <--> pes\n");
    TRY(_RegModuleConn(&info->rBagt, &info->rDmxPes, CONN_DIR_INP_TO_OUT));

#ifdef ENABLE_MULTIMEDIA
    TRACE("Register buffer agent <--> cntr_es\n");
    TRY(_RegModuleConn(&info->rBagt, &info->rDmxCntrEs, CONN_DIR_INP_TO_OUT));
    
    TRACE("Register buffer agent <--> cntr_ctrl\n");
    TRY(_RegModuleConn(&info->rBagt, &info->rDmxCntrCtrl, CONN_DIR_INP_TO_OUT));

    TRACE("Register buffer agent <--> cntr_es\n");
    TRY(_RegModuleConn(&info->rBagt, &info->rDmxCntrMem, CONN_DIR_INP_TO_OUT));

    /*aud/mpv connect to cntr_es*/
    TRACE("Register cntr_es <--> aud\n");
    TRY(_RegModuleConn(&info->rDmxCntrEs, &info->rAud, CONN_DIR_INP_TO_OUT));
    TRACE("Register pes <--> mpv\n");
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rDmxCntrEs.prComp[0], info->rDmxCntrEs.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
#if defined(CC_3D_MM_DS_SUPPORT) || defined(CC_FBM_TWO_FBP)
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rDmxCntrEs.prComp[0], info->rDmxCntrEs.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
#endif
#endif

    /* aud/mpv connect to pes */
    TRACE("Register pes <--> aud\n");
    TRY(_RegModuleConn(&info->rDmxPes, &info->rAud, CONN_DIR_INP_TO_OUT));
    TRACE("Register pes <--> mpv\n");
    /*TRY(_RegModuleConn(&info->rDmxPes, &info->rMpv, CONN_DIR_INP_TO_OUT));*/
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rDmxPes.prComp[0], info->rDmxPes.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
#ifdef CC_FBM_TWO_FBP
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rDmxPes.prComp[0], info->rDmxPes.u2Num, CONN_DIR_INP_TO_OUT, FALSE)));
#endif
    /* vdp connect to mpv */
    TRACE("Register mpv <--> vdp\n");
    /* TRY(_RegModuleConn(&info->rMpv, &info->rVdp, CONN_DIR_INP_TO_OUT)); */
    /* sd decoder */
    /* Remove unused sd decoder's connection list,
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[0], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[1], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[3], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    */
    /* hdsd decoder */
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rVdp.prComp[0], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
#if defined(CC_3D_MM_DS_SUPPORT) || defined(CC_FBM_TWO_FBP)
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rVdp.prComp[0], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[2], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));
#endif   
    /* vdp connect to tvd */
    /*
    TRY(_RegModuleConn(&info->rTvd, &info->rVdp, CONN_DIR_INP_TO_OUT));
    */
    TRY((x_rm_reg_conn_list(&info->rTvd.prComp[0], &info->rVdp.prComp[0], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    TRY((x_rm_reg_conn_list(&info->rTvd.prComp[1], &info->rVdp.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));

#ifdef CC_SUPPORT_TVE    
	/* tvd/vid_dec connect to tve */
    TRACE("Register mpv4 <--> tve0\n");
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rTve.prComp[0], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    
    TRACE("Register mpv4 <--> tve1\n");
    TRY((x_rm_reg_conn_list(&info->rMpv.prComp[4], &info->rTve.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));    
    TRACE("Register tvd3 <--> tve0\n");
    TRY((x_rm_reg_conn_list(&info->rTvd.prComp[3], &info->rTve.prComp[0], 1, CONN_DIR_OUT_TO_INP, FALSE)));
    
    TRACE("Register tvd2 <--> tve1\n");
    TRY((x_rm_reg_conn_list(&info->rTvd.prComp[2], &info->rTve.prComp[1], 1, CONN_DIR_OUT_TO_INP, FALSE)));    
#endif /* CC_SUPPORT_TVE */    
    
    /* flt supports sec */
    TRACE("Register sec <--> flt\n");
    TRY(_RegModuleConn(&info->rDmxFlt, &info->rDmxSec, CONN_DIR_TO_SUPPORT));
    /* hard wired descrambler to PID filter */
    /* iolo: disable descrambler for 5381 
    for(i=0; i < N_DRVC_PES; i++)
    {
        i4Ret = x_rm_reg_conn_list(&info->rDmxPes.prComp[i],
                              &info->rDmxDesc.prComp[i],1,
                              CONN_DIR_TO_SUPPORT, TRUE);
        if (i4Ret)
        {
            MW_PRINTF("descrambler to pes fail - %d, continue without return error.\n", i);
        }
        i4Ret = 0;
    }
    */
    /* video out 1 vdp--pmx0 */
    TRACE("Register pmx0 <--> vdp\n");
    TRY(_RegCompConn(&info->rPmx.prComp[0], 1,
                    info->rVdp.prComp, info->rVdp.u2Num,
                    CONN_DIR_OUT_TO_INP));
    TRACE("Register pmx0 <--> osd\n");
    TRY(_RegCompConn(&info->rPmx.prComp[0], 1,
                    info->rOsd.prComp, info->rOsd.u2Num,
                    CONN_DIR_OUT_TO_INP));
    /* video out 2 */
    if (info->rPmx.u2Num > 1)
    {
        if (info->rVdp.u2Num > 1)
        {
            TRACE("Register vdp1 <--> pmx1\n");
            TRY(_RegCompConn(&info->rPmx.prComp[1], 1,
                            &info->rVdp.prComp[1], 1,
                            CONN_DIR_OUT_TO_INP));
        }
        if (info->rOsd.u2Num > 1)
        {
            TRACE("Register osd1 <--> pmx1\n");
            TRY(_RegCompConn(&info->rPmx.prComp[1], 1,
                            &info->rOsd.prComp[1], 1,
                            CONN_DIR_OUT_TO_INP));
        }
    }

#ifdef TIME_SHIFT_SUPPORT
    TRACE("Register tuner <--> rec demux\n");
    i4Ret = _RegModuleConn(&info->rTuner, &info->rRecDmx, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("dig tuner<-->rec dmx failed\n");
    }
    TRACE("Register tuner <--> rec demux pcr\n");
    i4Ret = _RegModuleConn(&info->rTuner, &info->rRecDmxPcr, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("dig tuner<-->rec dmx pcr failed\n");
    }    
    TRACE("Register rec dmx <--> pvr stream\n");
    i4Ret = _RegModuleConn(&info->rRecDmx, &info->rPvrStrm, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("rec dmx<-->pvr stream failed\n");
    } 
    TRACE("Register rec dmx <--> pvr stream pcr\n");
    i4Ret = _RegModuleConn(&info->rRecDmxPcr, &info->rPvrStrmPcr, CONN_DIR_INP_TO_OUT);
    if (i4Ret != INITR_OK)
    {
        TRACE("rec dmx<-->pvr stream pcr failed\n");
    }
#endif  // TIME_SHIFT_SUPPORT

    CATCH()
    {
        return i4Ret;
    }
}

static INT32 _RegCompExclLists(POST_INIT_INFO_T* info)
{
    /*
        driver init should take care of comp excl list
        if these components are within one driver module
    */
    
#if defined(CC_MT5387) && !defined(CC_MT5363)
	UINT32 i4Ret = INITR_OK;
    /* jpg <--> mpg */
    TRY(x_rm_reg_comp_excl_list(info->rJpg.prComp,
        info->rMpv.prComp, info->rMpv.u2Num));
    CATCH()
    {
        UNUSED(i4Ret);
        return INITR_OK;
        //return i4Ret;
    }
#else
    return INITR_OK;
#endif
}

static INT32 _RegConnExclLists(POST_INIT_INFO_T* info)
{
    INT32 i4Ret = INITR_OK;
    UINT16 u2CompIndex;
    /* mpv specific conn excl list */
    UINT16 u2NumMpvHd;
    UINT16 u2NumMpvSd;
    UINT16 u2MpvIndex;
    UINT16 u2MpvHd;
    DRV_COMP_REG_T arExclComps[2];

    /* one pes can connect from one decoder(mpv or aud) */
    for (u2CompIndex = 0; u2CompIndex < info->rDmxPes.u2Num; u2CompIndex++)
    {
        TRY(x_rm_reg_conn_excl_list(
            &info->rDmxPes.prComp[u2CompIndex],
            info->rMpv.prComp,
            info->rMpv.u2Num + info->rAud.u2Num));
    }

    /* each mpv can connect to only one vdp */
#ifdef CC_SUPPORT_TVE
    /* SCART_OUT support, must ingore the exclusion list*/
#else    
    if (info->rVdp.u2Num > 1)
    {
        for (u2CompIndex = 0; u2CompIndex < info->rMpv.u2Num; u2CompIndex++)
        {
            TRY(x_rm_reg_conn_excl_list(
                &info->rMpv.prComp[u2CompIndex],
                info->rVdp.prComp,
                info->rVdp.u2Num));
        }
    }
#endif /* CC_SUPPORT_TVE */

    /* vdp1/osd1 can connect to one of pmx0 or pmx1 at the same time */
    if (info->rPmx.u2Num > 1)
    {
        if (info->rVdp.u2Num > 1)
        {
            TRY(x_rm_reg_conn_excl_list(
                &info->rVdp.prComp[1],
                info->rPmx.prComp,
                info->rPmx.u2Num));
        }
        if (info->rOsd.u2Num > 1)
        {
            TRY(x_rm_reg_conn_excl_list(
                &info->rOsd.prComp[1],
                info->rPmx.prComp,
                info->rPmx.u2Num));
        }
    }

    /* type II of conn excl list : when mpv_hd decoder connected to vdp,
    all other mpv can not connect to this vdp */

    /* setup */
    u2NumMpvHd = N_DRVC_MPVHD;
    u2NumMpvSd = (N_DRVC_MPV - N_DRVC_MPVHD);

    for (u2CompIndex = 0; u2CompIndex < info->rVdp.u2Num; u2CompIndex++)
    {
        for (u2MpvHd = 0; u2MpvHd < u2NumMpvHd; u2MpvHd++)
        {
            arExclComps[0] = info->rMpv.prComp[u2NumMpvSd + u2MpvHd];
            for (u2MpvIndex = 0; u2MpvIndex < N_DRVC_MPV; u2MpvIndex++)
            {
                if (u2MpvIndex < (u2NumMpvSd + u2MpvHd))
                {
                    arExclComps[1] = info->rMpv.prComp[u2MpvIndex];
                    TRY(x_rm_reg_conn_excl_list(
                        &info->rVdp.prComp[u2CompIndex],
                        arExclComps,
                        2));
                }
            }
        }
    }

    CATCH()
    {
        return i4Ret;
    }
}

static INT32 _RegGroups(POST_INIT_INFO_T* info)
{
    INT32 i4Ret = INITR_OK;

    /* main */
    if (info->rGroupMain.u2Num == 0)
    {
        RETURN_INITR_FAIL();
    }
    TRY(x_rm_reg_group_name(SN_PRES_MAIN_DISPLAY,
        info->rGroupMain.prComp, info->rGroupMain.u2Num));
    /* sub */
    if (info->rGroupSub.u2Num)
    {
        TRY(x_rm_reg_group_name(SN_PRES_SUB_DISPLAY,
            info->rGroupSub.prComp, info->rGroupSub.u2Num));
    }
    /* aux */
    if (info->rGroupAux.u2Num)
    {
        TRY(x_rm_reg_group_name(SN_PRES_AUX_DISPLAY,
            info->rGroupAux.prComp, info->rGroupAux.u2Num));
    }

    /*sound aux*/    
    if (info->rGroupAuxSund.u2Num)    
    {     
        TRY(x_rm_reg_group_name(SN_PRES_AUX_SOUND,    
            info->rGroupAuxSund.prComp, info->rGroupAuxSund.u2Num)); 
    }
    
    /*trd sond*/
    if (info->rGroupAuxSund.u2Num)    
    {     
        TRY(x_rm_reg_group_name(SN_PRES_TRD_SOUND,    
            info->rGroupTrdSund.prComp, info->rGroupTrdSund.u2Num)); 
    }
#ifdef CC_SUPPORT_TVE    
    /*TvByPass*/
    if (info->rGroupSub.u2Num)
    {
        TRY(x_rm_reg_group_name(SN_PRES_TV_BYPASS,
            info->rGroupTvByPass.prComp, info->rGroupTvByPass.u2Num));
    }
    
    /*MonitorByPass*/
    if (info->rGroupSub.u2Num)
    {
        TRY(x_rm_reg_group_name(SN_PRES_MONITOR_BYPASS,
            info->rGroupMonitorByPass.prComp, info->rGroupMonitorByPass.u2Num));
    }
#endif /* CC_SUPPORT_TVE */
    
    CATCH()
    {
        return i4Ret;
    }
}

INT32 MW_PostInit(void)
{
    INT32 i4Ret = INITR_OK;
    POST_INIT_INFO_T* info;

    HAL_TIME_T rTimeResult;
    HAL_TIME_T rTimeStart;
    HAL_TIME_T rTimeEnd;
    HAL_GetTime(&rTimeStart);

    TRACE("Create post init comp info\n");
    info = _CreateCompInfo();
    if (info == NULL)
    {
        RETURN_INITR_FAIL();
    }

    TRACE("Init post init comp info\n");
    TRY(_InitCompInfo(info));
    TRACE("Init main group\n");
    TRY(_InitGroupMain(info));
    TRACE("Init sub group\n");
    TRY(_InitGroupSub(info));
    TRACE("Init aux group\n");
    TRY(_InitGroupAux(info));
    TRACE("Init aux sound group\n");    
    TRY(_InitGroupAuxSund(info));
    TRACE("Init trd sound group\n");    
    TRY(_InitGroupTrdSund(info));

#ifdef CC_SUPPORT_TVE     
    TRACE("Init TvByPass group\n");
    TRY(_InitGroupTvByPass(info));
    TRACE("Init MonitorByPass group\n");
    TRY(_InitGroupMonitorByPass(info));  
#endif
        MW_PRINTF("\ttuner(%d) : %d\n"
                "\tpcr(%d) : %d\n"
                "\tpes(%d) : %d\n"
                "\tpes_mem(%d): %d\n"
                "\tsec(%d) : %d\n"
                "\tflt(%d) : %d\n"
                "\ttvd(%d) : %d\n"
                "\tmpv(%d) : %d\n"
                "\taud(%d) : %d\n"
                "\tvdp(%d) : %d\n"
                "\tosd(%d) : %d\n"
                "\tpmx(%d) : %d\n",
                info->rTuner.prComp[0].e_type, info->rTuner.u2Num,
                info->rDmxPcr.prComp[0].e_type, info->rDmxPcr.u2Num,
                info->rDmxPes.prComp[0].e_type, info->rDmxPes.u2Num,
                info->rDmxPesMem.prComp[0].e_type, info->rDmxPesMem.u2Num,
                info->rDmxSec.prComp[0].e_type, info->rDmxSec.u2Num,
                info->rDmxFlt.prComp[0].e_type, info->rDmxFlt.u2Num,
                info->rTvd.prComp[0].e_type, info->rTvd.u2Num,
                info->rMpv.prComp[0].e_type, info->rMpv.u2Num,
                info->rAud.prComp[0].e_type, info->rAud.u2Num,
                info->rVdp.prComp[0].e_type, info->rVdp.u2Num,
                info->rOsd.prComp[0].e_type, info->rOsd.u2Num,
                info->rPmx.prComp[0].e_type, info->rPmx.u2Num);

#ifdef CC_SUPPORT_TVE
    MW_PRINTF("\tvideo-out(%d) : %d\n"
              "\ttve(%d) : %d\n",
              info->rVdout.prComp[0].e_type, info->rVdout.u2Num,
              info->rTve.prComp[0].e_type, info->rTve.u2Num);
#endif

    TRACE("Register connection lists\n");
    TRY(_RegConnLists(info));
    TRACE("Register component exclusive lists\n");
    TRY(_RegCompExclLists(info));
    TRACE("Register connection excluseive lists\n");
    TRY(_RegConnExclLists(info));
    TRACE("Register groups\n");
    TRY(_RegGroups(info));

    TRACE("Delete post init comp info\n");
    _DeleteCompInfo(info);

    CATCH()
    {
        TRACE(i4Ret == INITR_OK ?
            "Post init done successfully\n" :
            "Post init failed\n");

        HAL_GetTime(&rTimeEnd);
        HAL_GetDeltaTime(&rTimeResult, &rTimeStart, &rTimeEnd);
        MW_PRINTF("post init time : %u.%06u sec\n", rTimeResult.u4Seconds, rTimeResult.u4Micros);
        return i4Ret;
    }
}
