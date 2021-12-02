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

#ifndef _DRVAPI_TUNER_H_
#define _DRVAPI_TUNER_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "drv_tuner.h"
#include "x_tuner.h"
#include "x_drvapi_tuner.h"
#include "queue.h"

/*********** MW API related constants *******************************/


/*********** OS related constants ***********************************/
#define cMAX_ACQ_TIME           3000        // in ms
#define cMAX_RESET_TIME         500         // in ms
//#define cUNCLOCK_POLLING_TIME   500         // in ms
#define cUNCLOCK_POLLING_TIME   100         // in ms
//#define cLOCK_POLLING_TIME      cUNCLOCK_POLLING_TIME
//#define cLOCK_POLLING_TIME      (cUNCLOCK_POLLING_TIME / 10 + 1)
#define cLOCK_POLLING_TIME      10          // min resolution = 10 ms
//#define cUNLOCK_CNT_THRD        (500 / cUNCLOCK_POLLING_TIME)
#define cUNLOCK_CNT_THRD        (1000 / cLOCK_POLLING_TIME)
#define cUNLOCK_TIME_THRD       1000        // in ms
#define cTHREAD_STACK_SIZE      2048
//#define cTHREAD_PRIORITY        90
#define cTHREAD_PRIORITY        101
//#define cTHREAD_PRIORITY        10          // highest = 0, lowest = 127
#define _TUNER_NOTIFY_TAG_      ((VOID *) 0x35670766)

/***********************************************************************/
//#define fgTUNER_SYNC_MASK_INB   0
//#define fgTUNER_SYNC_MASK_OOB   1

//#define fgTUNER_NSEL_INB        6           // 0: select,     1: not select
//#define fgTUNER_SEL_OOB         7           // 0: not select, 1: select
//#define TUNER_DTV_NOTIFY_ENABLE   1
#define TUNER_DTV_NOTIFY_ENABLE   0  //for MTFE


/***********************************************************************/
//#define DRVAPI_TUNER_ID         DRVAPI_TUNER_MTK_MT5111_ID1
#define cszDEMOD_VENDOR_MODEL   "MTK_MT5111"
#if 1
#define cTUNER_DEFAULT_FREQ     533000
#define cTUNER_DEFAULT_MOD      MOD_VSB_8
//#define cTUNER_DEFAULT_FREQ     605
//#define cTUNER_DEFAULT_MOD      MOD_QAM_64
#define cTUNER_DEFAULT_TYPE     DRVT_TUNER_TER_DIG
#else
#define cTUNER_DEFAULT_FREQ     513000
#define cTUNER_DEFAULT_MOD      MOD_QAM_256
#define cTUNER_DEFAULT_TYPE     DRVT_TUNER_CAB_DIG
#endif
#define cTUNER_DEFAULT_SYMRATE  772

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define DRVAPI_TUNER_CABLE_FREQ_HIGH    ((UINT32) 865000000)
#define DRVAPI_TUNER_CABLE_FREQ_LOW     ((UINT32)  53000000)
#define DRVAPI_TUNER_CABLE_BAUD_HIGH    ((UINT32)   8000000)
#define DRVAPI_TUNER_CABLE_BAUD_LOW     ((UINT32)    450000)

#define DRVAPI_TUNER_TER_FREQ_HIGH      ((UINT32) 865000000)
#define DRVAPI_TUNER_TER_FREQ_LOW       ((UINT32)  53000000)

#define DRVAPI_TUNER_OOB_TX_FREQ_HIGH   ((UINT32)  40000000)
#define DRVAPI_TUNER_OOB_TX_FREQ_LOW    ((UINT32)   8000000)

#define DRVAPI_TUNER_OOB_RX_FREQ_HIGH   ((UINT32) 130000000)
#define DRVAPI_TUNER_OOB_RX_FREQ_LOW    ((UINT32)  70000000)
#define DRVAPI_TUNER_OOB_RX_BAUD_HIGH   ((UINT32)   2048000)
#define DRVAPI_TUNER_OOB_RX_BAUD_LOW    ((UINT32)    772000)
#define DRVAPI_TUNER_OOB_RX_DATARATE_HIGH   ((UINT32)   3088000)
#define DRVAPI_TUNER_OOB_RX_DATARATE_LOW    ((UINT32)   1544000)

enum
{
    cTUNER_TYPE_CAB = 0,
    cTUNER_TYPE_TER,
    cTUNER_TYPE_OOB_TX,
    cTUNER_TYPE_OOB_RX,
    cTUNER_TYPE_STATE,
    cTUNER_TYPE_NUM
};

typedef union
{
    TUNER_CAB_DIG_TUNE_INFO_T           t_cab_tuner_info;
    TUNER_TER_DIG_TUNE_INFO_T           t_ter_tuner_info;
    TUNER_CAB_ANA_TUNE_INFO_T           t_cana_tuner_info;
    TUNER_TER_ANA_TUNE_INFO_T           t_tana_tuner_info;
    TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T    t_orx_tuner_info;
    TUNER_CAB_DIG_OOB_TX_TUNE_INFO_T    t_otx_tuner_info;
    TUNER_SAT_DIG_TUNE_INFO_T           t_sat_tuner_info;
} TUNER_ALL_INFO_T;

typedef struct
{
    UINT32              ui4_freq;
    UINT32              ui4_sym_rate;
    TUNER_MODULATION_T  e_mod;
//    VOID                *pvTunerInfo;
} TUNER_INTER_INFO_T;

typedef struct
{
    //++Important: Link should be put in the first place of this data structure.
    GENERIC_LIST_ENTRY              Link;
    //--
    VOID*                           ptParentTunerCtx;
    DRV_COMP_ID_T                   t_tuner_comp_id;
    DRV_COMP_REG_T                  t_tuner_comp_reg;
    DRV_CONN_TYPE_T                 e_conn_type;
    char                            comp_name[32];
    char                            thread_name[32];
    BOOL                            fgTunerThreadRunning;
    MUTEX_T                         t_tuner_state_lock;
    SEMA_T                          t_tuner_disconnected;
    SEMA_T                          t_tuner_disconnecting;
    SEMA_T                          t_tuner_connecting;
    SEMA_T                          t_tuner_sync;
    THREAD_T                        t_tuner_thread;

    #if 0//remove for Daniel test
    SEMA_T                          t_sync_tunerprocess_acq_and_mwtask;
    #endif
    
    TUNER_STATE_T                   e_tuner_state;
    BOOL                            b_disable_nfy;
    x_rm_nfy_fct                    pf_tuner_nfy;
    VOID*                           pv_tuner_nfy_tag;
    TUNER_CAB_DIG_TUNE_INFO_T       t_tuner_info;
    TUNER_ALL_INFO_T                t_conn_info;
    SIZE_T                          z_conn_info_len;
    BOOL                            OobSpecInv;

} STATE_INFO_T, *PSTATE_INFO_T;

typedef struct  _TUNER_CTX_T
{
    //++Important: Link should be put in the first place of this data structure.
    GENERIC_LIST_ENTRY          Link;
    //--
    UINT16                      TunerID;
    TUNER_CONFIG_T              tTunerCfg;
    SEMA_T                      RefSema;
    long                        RefCnt;
    
    GENERIC_LIST_ENTRY          StateInfoList;
} TUNER_CTX_T, *PTUNER_CTX_T;

typedef struct _PHY_TUNER_INFO_T
{
    UCHAR           ucPhyTunerIndex;
    BOOL            fgIsOob;
} PHY_TUNER_INFO_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

INT32 Tuner_Init(void);

#if TUNER_DTV_NOTIFY_ENABLE
INT32 Tuner_Notify(DRV_COND_T e_nfy_cond, UINT32 u4Msg);
#endif

S32 TunerBreak(void * pArg);
INT32 Tuner_Kill(void);

extern UINT32 os_get_sys_tick(void);

extern INT32 TunerSetAttribute(UINT16 TunerID,
                           DRV_SET_TYPE_T  e_set_type,
                           const VOID *pvAnaAttribute,
                           SIZE_T zAnaAttributeLen);
                           
extern INT16 TunerGetAttribute(UINT16 TunerID,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen);

extern INT16 TunerGetAttribute2(UINT16 TunerID,
                           DRV_GET_TYPE_T  e_get_type,
                           void *pvAnaAttribute,
                           SIZE_T* pzAnaAttributeLen,
                           DRV_CONN_TYPE_T e_conn_type);

extern BOOL TunerNotifyEnable(UINT16 TunerID);
#endif /* _DRVAPI_TUNER_H_ */
