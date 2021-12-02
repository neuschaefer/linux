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
 * Description:
 *      Middleware driver - Bufer Agent
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_cli.h"

#include "x_rm_dev_types.h"
#include "x_dbg.h"
#include "x_cli.h"
#include "drv_comp_id.h"
#include "bagt_mw.h"
#include "block_mw.h"
#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#endif
#include "feeder_if.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif
#if (defined(CC_MT5395) || defined(CC_MT5365)|| defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389))
#include "dmx_if.h"
#endif
#include "x_debug.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define BAGT_FEEDER
#define Bagt_Q_SIZE 16

#ifdef LINUX_TURNKEY_SOLUTION
#define BAGT_REORDER_ACK
#endif
typedef enum _BUF_AGT_FLOW_CTRL_STATE
{
    BAGT_FLWCTL_NONE,
    BAGT_FLWCTL_PRE_LOAD_PENDING,
    BAGT_FLWCTL_PRE_LOAD_STARTED,
    BAGT_FLWCTL_PRE_LOAD_ABORTED,
    BAGT_FLWCTL_PRE_LOAD_DONE,
    BAGT_FLWCTL_PRE_LOAD_DISCARDED
} BUF_AGT_FLOW_CTRL_STATE;

typedef struct _BUF_AGENT_INT_DATA_T
{
    buf_agt_mw_nfy_fct          pf_drv_nfy;
    buf_agt_mw_get_position_fct pf_get_pos;
    x_buf_agt_nfy_fct           pf_mw_nfy;
    x_buf_agt_data_req_ack_fct  pf_mw_ack_fct;
    VOID*                       pv_drv_tag;
    VOID*                       pv_mw_tag;
    BUF_AGT_SRC_ALIGN_INFO_T    t_align_info;
    BOOL                        fgFlowCtrl;
    BUF_AGT_FLOW_CTRL_T         t_flow_ctrl;
    BUF_AGT_FLOW_CTRL_STATE     eFlowCtrlState;                        
    FeederSourceType            eDataType;
    BOOL                        bNotUseFeeder;
    BOOL                        fgPushMode;
    UINT32                      u4BufAddr;
    UINT32                      pid;
    UINT32                      ui4_bagt_qid;
    BOOL                        fgQuickConsume;
    UINT32 u4AckCount;
    UINT32 u4ConsumeCount;
    UINT32 u4ReqCount;
#if (defined(CC_MT5395) || defined(CC_MT5365)|| defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389))
    DMX_VUDU_KEY_T              rDmxVuduKey[2];
#endif
}   BUF_AGENT_INT_DATA_T;

BUF_AGT_CTRL_IBC_AES_ENCRYPT_INFO_T rBagt_IBC_AES_INFO;

#ifdef BAGT_URI_MODE_1
typedef struct _BUF_AGT_CONSUME_T
{
    UINT32 u4EndWPtr;
    BUF_AGT_DATA_BLK_T rBagtDataBlk;
} BUF_AGT_CONSUME_T;
#endif

/*----------------------------------------------------------------------------
 * Static variables
 *---------------------------------------------------------------------------*/
#ifdef LINUX_TURNKEY_SOLUTION
buf_agt_reg_act_fct g_pf_buf_agt_req_act_fct   = NULL;
#endif

static BOOL                     gb_bagt_inited = FALSE;
static BAGT_COMP_ID_T                   ui2_bagt_id[FEEDER_SOURCE_INVALID];
static UINT64                   ui8_bagt_offset[BAGT_COMP_NS] = {0};
static BUF_AGENT_INT_DATA_T     gat_bagt_data[BAGT_COMP_NS+1];
static HANDLE_T _hBagtMsgQ[BAGT_COMP_NS] = {0};
static HANDLE_T _hBagtOldDataBlkQ[BAGT_COMP_NS] = {0};

#ifdef BAGT_URI_MODE_1
static BUF_AGT_CONSUME_T _aarConsumeList[BAGT_COMP_NS][Bagt_Q_SIZE + 1];  // +1: plus a dummy entry
static UINT32 _au4ConsumeListRp[BAGT_COMP_NS];
static UINT32 _au4ConsumeListWp[BAGT_COMP_NS];
static UINT32 _u4LastReqEndWPtr;
static UINT32 _u4LastAckEndWPtr;
#else
static BUF_AGT_DATA_BLK_T   t_old_data_blk[BAGT_COMP_NS];
static BOOL                 b_old_data_blk[BAGT_COMP_NS];
#endif


static BOOL fgUseLastData[BAGT_COMP_NS] = {0};
static FEEDER_REQ_DATA_T rLastDataBagt[BAGT_COMP_NS];  // megaa 20110824
static UINT32 u4IdCnt[BAGT_COMP_NS] = {0,0};

#ifdef LINUX_TURNKEY_SOLUTION
extern UCHAR adapt_fbm_is_fbm_usr_vir(UINT32 addr);
extern UCHAR adapt_fbm_is_fbm_knl_vir(UINT32 addr);
extern UINT32 adapt_fbm_knl_to_usr(UINT32 knl_addr);
extern UINT32 adapt_fbm_usr_to_knl(UINT32 usr_addr);
extern INT32 rm_copy_to_user(VOID *pv_user,VOID* pv_kernel,UINT32 size);
extern VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
extern INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
#endif

#ifdef CLI_SUPPORT
static UINT16                   ui2_bagt_dbg_level;
#if 0
static HANDLE_T                 h_bagt = NULL_HANDLE;
static BUF_AGT_DATA_BLK_T       t_data_blk;
static BUF_AGT_FCT_TAB_T        t_fct_tab = {0};

static INT32 _buf_agt_cli_mw_get_align(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _buf_agt_cli_mw_get_fct(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _buf_agt_cli_mw_get_pos(
    INT32          i4_argc, 
    const CHAR**   pps_argv);

static INT32 _buf_agt_cli_mw_set_align(
    INT32           i4_argc, 
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_mw_set_nfy(
    INT32           i4_argc, 
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_mw_set_flush(
    INT32           i4_argc, 
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_mw_ack(
    INT32           i4_argc, 
    const CHAR**    pps_argv);
#endif /* 0 */

static INT32 _buf_agt_cli_drv_data_consumed(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_flush(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_data_req(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_get_src_info(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_disconn(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_drv_conn(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static UINT16 _buf_agt_get_dbg_level(VOID);

static INT32 _buf_agt_cli_get_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv);

static INT32 _buf_agt_cli_set_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv);

#undef DBG_LEVEL_MODULE
#define DBG_LEVEL_MODULE                _buf_agt_get_dbg_level()

/* IO Manager command table */
static CLI_EXEC_T at_bagt_cmd_tbl[] =
{
#if 0
    {"mwgetalign",      "mga",  _buf_agt_cli_mw_get_align,      NULL,   "get DMA source alignment from MW",             CLI_SUPERVISOR},
    {"mwgetfct",        "mgf",  _buf_agt_cli_mw_get_fct,        NULL,   "get ack function from MW",                     CLI_SUPERVISOR},
    {"mwgetposition",   "mgp",  _buf_agt_cli_mw_get_pos,        NULL,   "get playing position from MW",                 CLI_SUPERVISOR},
    {"mwsetalign",      "msa",  _buf_agt_cli_mw_set_align,      NULL,   "set DMA source alignment from MW",             CLI_SUPERVISOR},
    {"mwsetnfy",        "msn",  _buf_agt_cli_mw_set_nfy,        NULL,   "set notify func from MW",                      CLI_SUPERVISOR},
    {"mwsetflush",      "msf",  _buf_agt_cli_mw_set_flush,      NULL,   "Send flush done from MW",                      CLI_SUPERVISOR},    
    {"mwack",           "ma",   _buf_agt_cli_mw_ack,            NULL,   "Send ack from MW",                             CLI_SUPERVISOR},
#endif /* 0 */
    {"drvconnect",      "dc",   _buf_agt_cli_drv_conn,          NULL,   "Connect Fake driver to Buffer Agent",          CLI_SUPERVISOR},
    {"drvdisconnect",   "dd",   _buf_agt_cli_drv_disconn,       NULL,   "Disconnect Fake driver",                       CLI_SUPERVISOR},
    {"drvgetsource",    "dgs",  _buf_agt_cli_drv_get_src_info,  NULL,   "Get DMA source information from Fake driver",  CLI_SUPERVISOR},
    {"drvdatareq",      "ddr",  _buf_agt_cli_drv_data_req,      NULL,   "Send data request from Fake driver",           CLI_SUPERVISOR},
    {"drvflush",        "df",   _buf_agt_cli_drv_flush,         NULL,   "Send flush from Fake driver",                  CLI_SUPERVISOR},
    {"drvdataconsumed", "ddc",  _buf_agt_cli_drv_data_consumed, NULL,   "Send data consumed info from Fake driver",     CLI_SUPERVISOR},
 	{CLI_GET_DBG_LVL_STR, NULL, _buf_agt_cli_get_dbg_level,     NULL,   CLI_GET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
	{CLI_SET_DBG_LVL_STR, NULL, _buf_agt_cli_set_dbg_level,     NULL,   CLI_SET_DBG_LVL_HELP_STR,           CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

/* IO Manager root command table */
static CLI_EXEC_T at_bagt_root_cmd_tbl[] =
{
	{"bagt",     NULL,        NULL,       at_bagt_cmd_tbl,      "Buffer Agent commands",   CLI_SUPERVISOR},
	END_OF_CLI_CMD_TBL
};
#endif
#ifdef BAGT_REORDER_ACK
#define BAGT_MW_ACKLIST_MAX 0x20
typedef struct 
{
  BUF_AGT_DATA_BLK_T rAckBlk;
  BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond;
  BOOL fgUsed;
  void *pNextBlk;
}MW_ACK_BLK_T;

typedef struct
{
  UINT32 u4AckNum;
  UINT32 u4WaitAckId;
  BOOL   fgWaitFirst;
  MW_ACK_BLK_T rAckBlks;
}MW_ACK_LIST_T;

static MW_ACK_LIST_T _rMwAckList[BAGT_COMP_NS];
#endif

#ifdef LINUX_TURNKEY_SOLUTION
static BAGT_NFY_PARAM bagt_param;
static BAGT_NFY_PARAM bagt_mmap_param;
static x_buf_agt_nfy_fct bagt_nfy_tbl[BAGT_COMP_NS+1];

static VOID set_bagt_nfy(BAGT_COMP_ID_T ui4_id)
{
    bagt_param.pf_nfy = bagt_nfy_tbl[ui4_id];
}

static VOID set_mmap_bagt_nfy(BAGT_COMP_ID_T ui4_id)
{
    bagt_mmap_param.pf_nfy = bagt_nfy_tbl[ui4_id];
}

static VOID bagt_nfy_fct(VOID*                  pv_nfy_tag,
                         BUF_AGT_NFY_COND_T     e_nfy_cond,
                         UINT32                 ui4_data_1,
                         UINT32                 ui4_data_2)
{
    bagt_param.pv_nfy_tag = pv_nfy_tag;
    bagt_param.e_nfy_cond = e_nfy_cond;
    bagt_param.ui4_data_1 = ui4_data_1;
    bagt_param.ui4_data_2 = ui4_data_2;

    _CB_PutEvent_Adpt(CB_DRV_BAGT_NFY, sizeof(BAGT_NFY_PARAM),(void *)&bagt_param);
}

static INT32  bagt_mmap_nfy_fct(VOID*                   pv_nfy_tag,
                                BUF_AGT_NFY_COND_T      e_nfy_cond,
                                UINT32                  pid,
                                UINT32                  ui4_data_1,
                                UINT32                  ui4_data_2)
/*MHF Linux ui4_data_1 containts
a bufer pointer that should be converted with mmap*/
{
    BUF_AGT_DATA_BLK_T *prDataBlock = NULL;
    UINT8* pui1_temp = NULL;

    bagt_mmap_param.pv_nfy_tag = pv_nfy_tag;
    bagt_mmap_param.e_nfy_cond = e_nfy_cond;

#if 0 /* For fix overlap data */
    lock_nfy_param_buf(PARAM_BAGT_BUF_NFY);
    prDataBlock = (BUF_AGT_DATA_BLK_T *)get_nfy_param_buf(PARAM_BAGT_BUF_NFY, sizeof(BUF_AGT_DATA_BLK_T));
#endif

    if(adapt_fbm_is_fbm_knl_vir((UINT32)((BUF_AGT_DATA_BLK_T *)(ui4_data_1))->pui1_buff))
    {
        /* Backup original kernel address. */
        pui1_temp = ((BUF_AGT_DATA_BLK_T *)(ui4_data_1))->pui1_buff;

        ((BUF_AGT_DATA_BLK_T *)(ui4_data_1))->pui1_buff
            = (UINT8 *)adapt_fbm_knl_to_usr((UINT32)((BUF_AGT_DATA_BLK_T *)(ui4_data_1))->pui1_buff);
    }

#if 0 /* For fix overlap data */
    rm_copy_to_user(prDataBlock, ui4_data_1, sizeof(BUF_AGT_DATA_BLK_T));

    unlock_nfy_param_buf(PARAM_BAGT_BUF_NFY);
#else
    bagt_mmap_param.t_data_1 = *((BUF_AGT_DATA_BLK_T *)(ui4_data_1));
    //x_memcpy(&bagt_mmap_param.t_data_1, ui4_data_1, sizeof(BUF_AGT_DATA_BLK_T));
#endif

    if(pui1_temp != NULL)
    {
        /* restore original kernel address. */
        ((BUF_AGT_DATA_BLK_T *)(ui4_data_1))->pui1_buff = pui1_temp;
    }

    bagt_mmap_param.ui4_data_1 = (UINT32)prDataBlock;
    bagt_mmap_param.ui4_data_2 = ui4_data_2;

    return _CB_PutEvent_Adpt(CB_DRV_BAGT_MMAP_NFY, sizeof(BAGT_NFY_PARAM), (void *)&bagt_mmap_param);
}
#endif

#ifdef BAGT_REORDER_ACK
static BOOL _buf_agt_acklist_add(MW_ACK_LIST_T *prList,BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond,BUF_AGT_DATA_BLK_T *prBlk)
{
   MW_ACK_BLK_T *pAckBlk=&prList->rAckBlks;

   while(pAckBlk)
   {
     if(pAckBlk->fgUsed==FALSE)
     {
        break;
     }
	 else
	 {
	    pAckBlk=(MW_ACK_BLK_T *)pAckBlk->pNextBlk;
		
	 }
   }

   if(pAckBlk) // find a unused block
   {
      pAckBlk->e_ack_cond=e_ack_cond;
	  x_memcpy(&pAckBlk->rAckBlk,prBlk,sizeof(BUF_AGT_DATA_BLK_T));
	  pAckBlk->fgUsed=TRUE;
   }
   else
   {  
      MW_ACK_BLK_T *pTailBlk=&prList->rAckBlks;
      pAckBlk=(MW_ACK_BLK_T *)x_mem_alloc(sizeof(MW_ACK_BLK_T));
	  if(!pAckBlk)
	  {
        return FALSE;
	  }
	  
	  while(pTailBlk)
	  {
        if(pTailBlk->pNextBlk)
        {
           pTailBlk=(MW_ACK_BLK_T *)pTailBlk->pNextBlk;
        }
		else
		{
           break;
		}
	  }
      
	  x_memcpy(&pAckBlk->rAckBlk,prBlk,sizeof(BUF_AGT_DATA_BLK_T));
	  pAckBlk->e_ack_cond=e_ack_cond;
	  pAckBlk->fgUsed=TRUE;
	  pAckBlk->pNextBlk=NULL;
	  pTailBlk->pNextBlk= (void *)pAckBlk;
   }

   return TRUE;
}

static BOOL _buf_agt_acklist_get(MW_ACK_LIST_T *prList,UINT32 u4Ackid,
	BUF_AGT_DATA_BLK_T **prBlk,BUF_AGT_DATA_REQ_ACK_COND_T *e_ack_cond)
{
	MW_ACK_BLK_T *pAckBlk=&prList->rAckBlks;
    
    while(pAckBlk)
    {
       if(pAckBlk->fgUsed && (pAckBlk->rAckBlk.ui4_req_id==u4Ackid))
       {
          *prBlk=&pAckBlk->rAckBlk;
		  *e_ack_cond=pAckBlk->e_ack_cond;
		  pAckBlk->fgUsed=FALSE;
		  return TRUE;
       }
	   pAckBlk=(MW_ACK_BLK_T *)pAckBlk->pNextBlk;
    }
    return FALSE;
}

static void _buf_agt_acklist_free(MW_ACK_LIST_T *prList)
{
	MW_ACK_BLK_T *pAckBlk=(MW_ACK_BLK_T *)prList->rAckBlks.pNextBlk;
    MW_ACK_BLK_T *pAckNext;
	while(pAckBlk)
	{
       pAckNext=(MW_ACK_BLK_T *)pAckBlk->pNextBlk;
	   x_mem_free((void *)pAckBlk);
	   pAckBlk=pAckNext;
	}
	prList->rAckBlks.pNextBlk=NULL;
	prList->rAckBlks.fgUsed=FALSE;
	return;
}

static void _buf_agt_acklist_clean(MW_ACK_LIST_T *prList)
{
	MW_ACK_BLK_T *pAckBlk=&prList->rAckBlks;
	while(pAckBlk)
	{  
	   pAckBlk->fgUsed=FALSE;
       pAckBlk=(MW_ACK_BLK_T *)pAckBlk->pNextBlk;
	}
	return;
}

#endif


/*----------------------------------------------------------------------------
 * Static functions
 *---------------------------------------------------------------------------*/
#ifdef CLI_SUPPORT
#if 0
/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_handle_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static VOID _buf_agt_cli_mw_handle_nfy_fct(
    HANDLE_T   h_obj,
    UINT8      ui1_port,
    RM_COND_T  e_nfy_cond,
    VOID*      pv_tag,
    UINT32     ui4_data)
{
    switch (e_nfy_cond)
    {
        case RM_COND_IGNORE:
            break;
            
        case RM_COND_CLOSED:
        case RM_COND_CLOSING:
        {
            /* Set the RTC handle to NULL */
            h_bagt = NULL_HANDLE;
        }
        break;

        default:
            break;
    }
    return ;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_handle
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_handle(VOID)
{
    INT32              i4_ret;
    OBJ_TO_COMP_REL_T  e_obj_to_comp_rel = OBJ_COMP_REL_IGNORE;
    DRV_TYPE_T         e_type            = DRVT_BUF_AGENT;
    UINT16             ui2_id            = ANY_COMP_ID;
    const CHAR*        ps_name           = NULL;
    CTRL_TYPE_T        e_ctrl            = CTRL_TYPE_SHARED;
    VOID*              pv_comp_sel_data  = NULL;
    rm_comp_sel_fct    pf_comp_sel       = NULL;
    VOID*              pv_tag            = NULL;
    rm_nfy_fct         pf_nfy            = _buf_agt_cli_mw_handle_nfy_fct;
    RM_COND_T          e_rm_cond;
        
    if (h_bagt == NULL_HANDLE)
    {
        i4_ret = rm_open_comp(NULL_HANDLE,
                              e_obj_to_comp_rel,
                              e_type,
                              ui2_id,
                              ps_name,
                              e_ctrl,
                              pv_comp_sel_data,
                              pf_comp_sel,
                              pv_tag,
                              pf_nfy,
                              &h_bagt,
                              &e_rm_cond);
        if (i4_ret == RMR_OK)
        {
            i4_rc = BAGTR_OK;
        }
        else
        {
            i4_rc = BAGTR_FAILED;
        }
    }
    else
    {
        /* handle is already initialized. */
        i4_ret = BAGTR_OK;
    }
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_align
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_align(
    INT32          i4_argc, 
    const CHAR**   pps_argv)
{
    BUF_AGT_SRC_ALIGN_INFO_T    t_info;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    z_len = sizeof(BUF_AGT_SRC_ALIGN_INFO_T);
    i4_ret = rm_get(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_GET_TYPE_SRC_ALIGN_INFO,
                    (VOID*)&t_info,
                    &z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("get the align info: align = %d, min_size = %d\n",
                   t_info.ui4_align,
                   t_info.ui4_min_blk_size);
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't get align info\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_fct(
    INT32          i4_argc, 
    const CHAR**   pps_argv)
{
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    z_len = sizeof(BUF_AGT_FCT_TAB_T);
    i4_ret = rm_get(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_GET_TYPE_ACK_FCT_TAB,
                    (VOID*)&t_fct_tab,
                    &z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("get ack fct tab ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't get ack fct tab\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_get_pos
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_get_pos(
    INT32          i4_argc, 
    const CHAR**   pps_argv)
{
    BUF_AGT_POSITION_T          t_pos;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    z_len = sizeof(BUF_AGT_POSITION_T);
    i4_ret = rm_get(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_GET_TYPE_POSITION,
                    (VOID*)&t_pos,
                    &z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("get pos req_id %d, offset %d\n",
                   t_pos.ui4_current_req_id,
                   t_pos.ui4_current_byte_offset);
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't get position\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_set_align
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_set_align(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    BUF_AGT_SRC_ALIGN_INFO_T    t_info;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    if (i4_argc < 4)
    {
        x_dbg_stmt("msa [align] [max] [min]\n");
        return BAGTR_OK;
    }

    t_info.ui4_align = x_strtoll(pps_argv[1], NULL, 10);
    t_info.ui4_min_blk_size = x_strtoll(pps_argv[2], NULL, 10);
    z_len = sizeof(BUF_AGT_SRC_ALIGN_INFO_T);

    i4_ret = rm_set(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_SRC_ALIGN_INFO,
                    (VOID*)&t_info,
                    z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("set the align info: align = %d, min_size = %d\n",
                   t_info.ui4_align,
                   t_info.ui4_min_blk_size);
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't set align info\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_cli_mw_nfy_fct(
    VOID*                  pv_nfy_tag,
    BUF_AGT_NFY_COND_T     e_nfy_cond,
    UINT32                 ui4_data_1,
    UINT32                 ui4_data_2)
{
    switch (e_nfy_cond)
    {
        case BUF_AGT_NFY_COND_DATA_REQ:
            t_data_blk = *(BUF_AGT_DATA_BLK_T*)ui4_data_1;
            x_dbg_stmt("_buf_agt_cli_mw_nfy_fct BUF_AGT_NFY_COND_DATA_REQ req id %d, buf addr %x, size %d\n",
                       t_data_blk.ui4_req_id,
                       t_data_blk.pui1_buff,
                       t_data_blk.z_size);
            break;
        case BUF_AGT_NFY_COND_FLUSH_REQ:
            x_dbg_stmt("_buf_agt_cli_mw_nfy_fct BUF_AGT_NFY_COND_FLUSH_REQ\n");
            break;
        case BUF_AGT_NFY_COND_DATA_CONSUMED:
            x_dbg_stmt("_buf_agt_cli_mw_nfy_fct BUF_AGT_NFY_COND_DATA_CONSUMED req_id %d\n",
                       ui4_data_1);
            break;
        default:
            x_dbg_stmt("unknown _buf_agt_cli_mw_nfy_fct condition\n");
            break;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_data_consumed
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_set_nfy(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    BUF_AGT_NFY_INFO_T          t_info;
    SIZE_T                      z_len;
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    t_info.pf_nfy = _buf_agt_cli_mw_nfy_fct;
    t_info.pv_tag = NULL;
    z_len = sizeof(BUF_AGT_NFY_INFO_T);

    i4_ret = rm_set(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_NFY_FCT,
                    (VOID*)&t_info,
                    z_len,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("set nfy fct ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't set nfy fct\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_set_flush
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_set_flush(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    RM_COND_T                   e_rm_cond;
    INT32                       i4_ret;

    if (BAGTR_OK != _buf_agt_cli_mw_get_handle())
    {
        return BAGTR_FAILED;
    }

    i4_ret = rm_set(h_bagt,
                    FALSE,
                    ANY_PORT_NUM,
                    BUF_AGT_SET_TYPE_FLUSH_DONE,
                    (VOID*)NULL,
                    0,
                    &e_rm_cond);
    if (i4_ret == RMR_OK)
    {
        x_dbg_stmt("set flush done ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("Can't flush done\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_mw_ack
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_mw_ack(
    INT32           i4_argc, 
    const CHAR**    pps_argv)
{
    BOOL                            b_req;
    BUF_AGT_DATA_REQ_ACK_COND_T     e_ack_cond = BUF_AGT_DATA_REQ_ACK_COND_DONE;

    if (0 == x_strcmp(pps_argv[1], "c"))
    {
        e_ack_cond = BUF_AGT_DATA_REQ_ACK_COND_CANCELED; 
    }
    else if (0 == x_strcmp(pps_argv[1], "e"))
    {
        e_ack_cond = BUF_AGT_DATA_REQ_ACK_COND_ERROR; 
    }

    b_req = t_fct_tab.pf_data_req_ack(0,
                                      e_ack_cond,
                                      &t_data_blk);

    if (b_req)
    {
        x_dbg_stmt("pf_data_req_ack ok\n");
        return BAGTR_OK;
    }
    else
    {
        x_dbg_stmt("pf_data_req_ack failed\n");
        return BAGTR_FAILED;
    }
}
#endif /* 0 */

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_data_consumed
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_data_consumed(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32               i4_ret;
    BUF_AGT_DATA_BLK_T  t_data_blk;
    BAGT_COMP_ID_T              ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = BAGT_COMP_1;
    }
    else
    {
        ui2_id = (BAGT_COMP_ID_T)x_strtoll(pps_argv[1], NULL, 10);
    }

    t_data_blk.pui1_buff    = (UINT8*)0x008800;
    t_data_blk.z_size       = 1280;
    t_data_blk.ui4_req_id   = 888;

    i4_ret = buf_agt_mw_data_consumed(ui2_id,
                                      &t_data_blk);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_consumed failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_consumed ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_flush
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_flush(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32               i4_ret;
    BAGT_COMP_ID_T          ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = BAGT_COMP_1;
    }
    else
    {
        ui2_id = (BAGT_COMP_ID_T)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_flush(ui2_id);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_flush failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_flush ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_data_req
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_data_req(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32               i4_ret;
    BUF_AGT_DATA_BLK_T  t_data_blk;
    BAGT_COMP_ID_T              ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = BAGT_COMP_1;
    }
    else
    {
        ui2_id = (BAGT_COMP_ID_T)x_strtoll(pps_argv[1], NULL, 10);
    }

    t_data_blk.pui1_buff    = (UINT8*)0x007700;
    t_data_blk.z_size       = 10240;
    t_data_blk.ui4_req_id   = 365;

    i4_ret = buf_agt_mw_data_req(ui2_id,
                                 &t_data_blk);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_req failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_data_req ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_get_src_info
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_get_src_info(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    BUF_AGT_SRC_ALIGN_INFO_T    t_data_blk;
    INT32                       i4_ret;
    BAGT_COMP_ID_T                      ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = BAGT_COMP_1;
    }
    else
    {
        ui2_id = (BAGT_COMP_ID_T)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_get_source_info(ui2_id,
                                        &t_data_blk);
    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_get_source_info failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_get_source_info align %d, min size %d\n",
               t_data_blk.ui4_align,
               t_data_blk.ui4_min_blk_size);
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_disconn
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_disconn(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;
    BAGT_COMP_ID_T          ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = BAGT_COMP_1;
    }
    else
    {
        ui2_id = (BAGT_COMP_ID_T)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_disconnect(ui2_id);

    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_disconnect failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_disconnect ok\n");
    }


    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_cli_nfy_fct(
    VOID*                     pv_tag,
    BUF_AGT_MW_NFY_COND_T     e_nfy_cond,
    UINT32                    ui4_data_1,
    UINT32                    ui4_data_2)
{
    BUF_AGT_DATA_BLK_T*       pt_data_blk;

    switch (e_nfy_cond)
    {
        case BUF_AGT_MW_NFY_COND_DATA_REQ_DONE:
            x_dbg_stmt("[BAGT] BUF_AGT_MW_NFY_COND_DATA_REQ_DONE notify\n");
            break;

        case BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR:
            x_dbg_stmt("[BAGT] BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR notify\n");
            break;

        case BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED:
            x_dbg_stmt("[BAGT] BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED notify\n");
            break;

        case BUF_AGT_MW_NFY_COND_FLUSH_DONE:
            x_dbg_stmt("[BAGT] Flush done notify\n");
            return TRUE;

        default:
            x_dbg_stmt("[BAGT] Unknown notify cond %d\n",
                   e_nfy_cond);
            return TRUE;
    }

    pt_data_blk = (BUF_AGT_DATA_BLK_T*)ui4_data_1;
    
    x_dbg_stmt("[BAGT] data blk req_id %d, buf addr %x, buf size %d\n",
           pt_data_blk->ui4_req_id,
           pt_data_blk->pui1_buff,
           pt_data_blk->z_size);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_get_position_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_cli_get_position_fct(
    VOID*       pv_tag,
    UINT32*     pui4_current_req_id,
    UINT32*     pui4_current_byte_offset)
{
    *pui4_current_req_id = 100;
    *pui4_current_byte_offset = 123;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_drv_conn
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_drv_conn(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;
    BAGT_COMP_ID_T          ui2_id;

    if (i4_argc < 2)
    {
        ui2_id = BAGT_COMP_1;
    }
    else
    {
        ui2_id = (BAGT_COMP_ID_T)x_strtoll(pps_argv[1], NULL, 10);
    }

    i4_ret = buf_agt_mw_connect(ui2_id,
                                _buf_agt_cli_nfy_fct,
                                _buf_agt_cli_get_position_fct,
                                NULL);

    if (i4_ret != BAGTR_OK)
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_connect failed %d\n", i4_ret);
    }
    else
    {
        x_dbg_stmt("[BAGT] buf_agt_mw_connect ok\n");
    }

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
static UINT16 _buf_agt_get_dbg_level(VOID)
{
    if (gb_bagt_inited)
    {
        return ui2_bagt_dbg_level;
    }
    else
    {
        return 0;
    }
}

/*-----------------------------------------------------------------------------
 * Name: iom_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_set_dbg_level(
    UINT16          ui2_level)
{
    if (gb_bagt_inited)
    {
        ui2_bagt_dbg_level = ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_get_dbg_level
 *
 * Description: This API gets the current setting of IO Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_get_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;

    /* */

    i4_ret = x_cli_show_dbg_level(_buf_agt_get_dbg_level());

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_set_dbg_level
 *
 * Description: This API sets the debug level of IO Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 *          CLIR_INV_CMD_USAGE      Invalid command usage.
 *          CLIR_CMD_EXEC_ERROR     CLI command execution failed.
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_cli_set_dbg_level(
    INT32           i4_argc,
    const CHAR**    pps_argv)
{
    INT32           i4_ret;
    UINT16          ui2_dbg_level;

    /* bagt.sdl [level] */

    /* Check arguments */
	if ((i4_argc != 2) ||
	    (pps_argv == NULL))
	{
		x_dbg_stmt("bagt.sdl [level]\n\r");
		x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
		return CLIR_INV_CMD_USAGE;
	}

    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);

    if (i4_ret == CLIR_OK)
    {
        if (_buf_agt_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_ret = CLIR_CMD_EXEC_ERROR;
        }
        else
        {
            i4_ret = x_cli_show_dbg_level(_buf_agt_get_dbg_level());
        }
    }
    else
    {
		x_dbg_stmt("gagt.sdl [level]\n\r");
		x_dbg_stmt("  [level]: n=none/e=error, a=api, i=info\n\r");
		return CLIR_INV_CMD_USAGE;
    }

    return i4_ret;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_handle_encrypt
 *
 * Description: Handle encrypt information in IBC
 *
 * Inputs:  ui2_buf_agt_id  BAGT ID
 *          prParam         IBC parameter
 *
 * Outputs: - 
 *
 * Returns: None
 *
 ----------------------------------------------------------------------------*/
static VOID _buf_agt_handle_encrypt(BAGT_COMP_ID_T ui2_buf_agt_id, BUF_AGT_CTRL_IBC_ENCRYPT_INFO_T *prParam)
{
#if (defined(CC_MT5395) || defined(CC_MT5365)|| defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389))
    //DMX_VUDU_KEY_T *prDmxVuduKey = &gat_bagt_data[ui2_buf_agt_id].rDmxVuduKey;
    DMX_VUDU_KEY_T *prDmxVuduKey = NULL;
    BUF_AGT_CTRL_IBC_AES_ENCRYPT_INFO_T *prAesInfo;

    if (!prParam)
        return;

    prAesInfo = &prParam->u_encrypt_info.t_aes;

	if((prAesInfo->ui1_mask & BUF_AGT_AES_ENCRYPT_EVEN)!=0)
	{
		//even
		prDmxVuduKey = &gat_bagt_data[ui2_buf_agt_id].rDmxVuduKey[0];
	}
	else
	{
		//odd
		prDmxVuduKey = &gat_bagt_data[ui2_buf_agt_id].rDmxVuduKey[1];
	}

    x_memset((VOID *)prDmxVuduKey, 0, sizeof(DMX_VUDU_KEY_T));


    switch (prParam->e_encrypt_mode)
    {
    case BUF_AGT_CTRL_IBC_ENCRYPT_MODE_AES_ECB:
        prDmxVuduKey->eMode = DMX_DESC_MODE_AES_ECB;
        break;
    case BUF_AGT_CTRL_IBC_ENCRYPT_MODE_AES_CBC:
        prDmxVuduKey->eMode = DMX_DESC_MODE_AES_CBC;
        break;
	default:
		break;
    }

    x_memcpy((VOID *)rBagt_IBC_AES_INFO.aui1_uniqueKey, (VOID *)prAesInfo->aui1_uniqueKey, sizeof(rBagt_IBC_AES_INFO.aui1_uniqueKey));

    if((prAesInfo->ui1_mask & BUF_AGT_AES_ENCRYPT_EVEN)!=0)
    {
        x_memcpy((VOID *)rBagt_IBC_AES_INFO.aui1_even_key, (VOID *)prAesInfo->aui1_even_key, sizeof(rBagt_IBC_AES_INFO.aui1_even_key));
    }
    else
    {
        x_memcpy((VOID *)rBagt_IBC_AES_INFO.aui1_odd_key, (VOID *)prAesInfo->aui1_odd_key, sizeof(rBagt_IBC_AES_INFO.aui1_odd_key));
    }

    x_memcpy((VOID *)rBagt_IBC_AES_INFO.aui1_IV, (VOID *)prAesInfo->aui1_IV, sizeof(rBagt_IBC_AES_INFO.aui1_IV));

    prDmxVuduKey->u1Index = 0;
    prDmxVuduKey->fgEven = (BOOL)(prAesInfo->ui1_mask & BUF_AGT_AES_ENCRYPT_EVEN);
    //prDmxVuduKey->pu1UniqueKey = prAesInfo->aui1_uniqueKey;
    prDmxVuduKey->pu1UniqueKey = (UINT8 *)rBagt_IBC_AES_INFO.aui1_uniqueKey;
    prDmxVuduKey->fgWrappedKey = prAesInfo->b_warp_key;
    prDmxVuduKey->pu1Key = prDmxVuduKey->fgEven ? (UINT8 *)rBagt_IBC_AES_INFO.aui1_even_key : (UINT8 *)rBagt_IBC_AES_INFO.aui1_odd_key;
    prDmxVuduKey->u1KeyLen = prAesInfo->ui1_key_len;
    prDmxVuduKey->fgWarppedIV = prAesInfo->b_warp_IV;
    //prDmxVuduKey->pu1IV = prAesInfo->aui1_IV;
    prDmxVuduKey->pu1IV = (UINT8 *)rBagt_IBC_AES_INFO.aui1_IV;
    prDmxVuduKey->u1IvLen = 16;
    //prDmxVuduKey->eRtbMode = DMX_DESC_RTB_MODE_SCTE52;
    prDmxVuduKey->eRtbMode = (DMX_DESC_RTB_MODE_T)prParam->e_rtb_mode;
#else
    UNUSED(ui2_buf_agt_id);
    UNUSED(prParam);
#endif
}


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_req_ack_fct
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_req_ack_fct(
    UINT16                      ui2_buf_agt_id_tmp,
    BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond,
    BUF_AGT_DATA_BLK_T*         pt_data_blk)
{
    FEEDER_REQ_DATA_T       rData;
#if 0  // megaa 20110824: use each instance's own instead!!!
    static FEEDER_REQ_DATA_T rLastData;
#endif
    FEEDER_REQ_DATA_T      rMsgQ;
    BUF_AGT_MW_NFY_COND_T       e_mw_ack_cond;
    SIZE_T                  zMsgSize;
    INT32                   i4Ret;
    UINT16                  u2MsgQIdx;
    BOOL                        b_ret;
    FEEDER_NOTIFY_COND eCondition = FEEDER_NOTIFY_DATA_REQ_DONE;
    BAGT_COMP_ID_T	ui2_buf_agt_id =(BAGT_COMP_ID_T)ui2_buf_agt_id_tmp;

    if(ui2_buf_agt_id>=BAGT_COMP_NS)
    {
       return FALSE;
    }
	
    if ((NULL == gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy) && 
    	  (gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder))
    {
        /* TODO: Add debug information */
        return FALSE;
    }

    x_memset((void *)&rData,0,sizeof(FEEDER_REQ_DATA_T));

    /* convert the notify condition */
    switch (e_ack_cond)
    {
        case BUF_AGT_DATA_REQ_ACK_COND_DONE:
            e_mw_ack_cond = BUF_AGT_MW_NFY_COND_DATA_REQ_DONE;
            eCondition = FEEDER_NOTIFY_DATA_REQ_DONE;
            break;
        case BUF_AGT_DATA_REQ_ACK_COND_ERROR:
            //ASSERT(0);
            e_mw_ack_cond = BUF_AGT_MW_NFY_COND_DATA_REQ_ERROR;
            eCondition = FEEDER_NOTIFY_ERROR;
            break;
        case BUF_AGT_DATA_REQ_ACK_COND_CANCELED:
            //ASSERT(0);
            e_mw_ack_cond = BUF_AGT_MW_NFY_COND_DATA_REQ_CANCELED;
            //eCondition = FEEDER_NOTIFY_DATA_REQ_CANCELED;
            break;
        default:
            /* TODO: Add debug information */
            //ASSERT(0);
            return FALSE;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    if(pt_data_blk->e_data_fmt != DATA_FMT_IBC && adapt_fbm_is_fbm_usr_vir((UINT32)pt_data_blk->pui1_buff))
    {
        pt_data_blk->pui1_buff = (UINT8 *)adapt_fbm_usr_to_knl((UINT32)pt_data_blk->pui1_buff);
    }
#endif
    if (gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder)    
    {
    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                    e_mw_ack_cond,
                                    (UINT32)pt_data_blk,
                                    0);
    }    
    else
    {
        if (gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState == BAGT_FLWCTL_PRE_LOAD_STARTED)
        {
            gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_PRE_LOAD_DONE;
            FeederSetPreLoadDone(gat_bagt_data[ui2_buf_agt_id].eDataType, pt_data_blk->z_size);
            buf_agt_mw_data_consumed(ui2_buf_agt_id, pt_data_blk);  // Todo: check if this is necessary and whether it needs to comply with BAGT_URI_MODE_1
            return TRUE;
        }
        
        zMsgSize= sizeof(FEEDER_REQ_DATA_T);
        i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rMsgQ, &zMsgSize,
				(void*)&_hBagtMsgQ[ui2_buf_agt_id], 1, X_MSGQ_OPTION_WAIT);
	
	    ASSERT(zMsgSize == sizeof(rMsgQ));

	    if (i4Ret != OSR_OK)
	    {
            LOG(0, "_hBagtMsgQ Rcv : Fail ! ret=0x%08x\n",i4Ret);
		    return FALSE;
	    }
                
        rData.eFeederIBC = FEEDER_IBC_NONE;
        rData.eDataType = gat_bagt_data[ui2_buf_agt_id].eDataType;
        rData.u4ReadSize = pt_data_blk->z_size;
        if (gat_bagt_data[ui2_buf_agt_id].fgPushMode)
        {
            rData.u4WriteAddr = gat_bagt_data[ui2_buf_agt_id].u4BufAddr + (UINT32)pt_data_blk->pui1_buff;
        }
        else
        {
            rData.u4WriteAddr = (UINT32)pt_data_blk->pui1_buff;
        }
	    rData.u4AlignOffset = rMsgQ.u4AlignOffset;
        rData.u8FilePos = pt_data_blk->ui8_offset + pt_data_blk->z_size;
        rData.fgEof = (pt_data_blk->ui4_flags & BUF_AGT_GET_DATA_EOF) ? TRUE : FALSE;
        
        // IBC handling
        if (pt_data_blk->e_data_fmt == DATA_FMT_IBC)
        {
            switch (pt_data_blk->t_extra.t_ctrl_ibc.e_cmd_type)
            {
            case BUF_AGT_CTRL_IBC_TYPE_EOS:
                rData.fgEof = TRUE;
                break;
            
            case BUF_AGT_CTRL_IBC_TYPE_ENCRYPT:
                _buf_agt_handle_encrypt(ui2_buf_agt_id, &pt_data_blk->t_extra.t_ctrl_ibc.t_param.t_set_encrypt_info);
#if (defined(CC_MT5395) || defined(CC_MT5365)|| defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389))
                rData.eFeederIBC = FEEDER_IBC_DMX_VUDU_KEY;
		if((pt_data_blk->t_extra.t_ctrl_ibc.t_param.t_set_encrypt_info.u_encrypt_info.t_aes.ui1_mask& BUF_AGT_AES_ENCRYPT_EVEN)!=0)
		{
			//even
			rData.u4WriteAddr = (UINT32)&gat_bagt_data[ui2_buf_agt_id].rDmxVuduKey[0];
		}
		else
		{
			//odd
			rData.u4WriteAddr = (UINT32)&gat_bagt_data[ui2_buf_agt_id].rDmxVuduKey[1];
		}
                rData.u4ReadSize = sizeof(DMX_VUDU_KEY_T);
#else
                ASSERT(0);
#endif
                break;
                
            default:
                break;
            }
        }

        LOG(7, "_hBagtMsgQ <= id=%ld size=%ld align= 0x%x FileOff=0x%08x\n",pt_data_blk->ui4_req_id, rMsgQ.u4ReadSize, rMsgQ.u4AlignOffset, (UINT32)(rData.u8FilePos&0xffffffff));
        //LOG(0, "receive id = %d\n", pt_data_blk->ui4_req_id);
        
        if (rMsgQ.fgPartial)
        {
            LOG(7, "first part in the tail of ring buf, id = %d\n", rData.u4Id);
            fgUseLastData[ui2_buf_agt_id] = TRUE;
            x_memcpy((VOID *)&rLastDataBagt[ui2_buf_agt_id], (VOID *)&rData, sizeof(FEEDER_REQ_DATA_T));  // megaa 20110824
            b_ret = TRUE;
        }
        else
        {        
            if (fgUseLastData[ui2_buf_agt_id])
            {
                UINT32 u4CurSize = rData.u4ReadSize;
                BOOL fgEof=rData.fgEof;
                fgUseLastData[ui2_buf_agt_id] = FALSE;
                if (rData.eDataType != rLastDataBagt[ui2_buf_agt_id].eDataType)  // megaa 20110824
                {
                    LOG(0, "\n\n\n[BAGT] $$$$$$$$$$$$$$$$$$$$$ too bad !!!!!!!!!!!!!!!!!!\n\n\n");
                }
                x_memcpy((VOID *)&rData, (VOID *)&rLastDataBagt[ui2_buf_agt_id], sizeof(FEEDER_REQ_DATA_T));  // megaa 20110824
                rData.u4ReadSize = rLastDataBagt[ui2_buf_agt_id].u4ReadSize + u4CurSize;  // megaa 20110824
                rData.fgEof=fgEof;
            }
            rData.u4Id = rMsgQ.u4Id;
            
            if (gat_bagt_data[ui2_buf_agt_id].fgPushMode)
            {
#if 1
                i4Ret = x_msg_q_send(_hBagtOldDataBlkQ[ui2_buf_agt_id], pt_data_blk, sizeof(BUF_AGT_DATA_BLK_T), 255);
                ASSERT(i4Ret == OSR_OK);
                gat_bagt_data[ui2_buf_agt_id].u4AckCount++; 
#else
                x_memcpy(&t_old_data_blk[ui2_buf_agt_id], pt_data_blk, sizeof(t_old_data_blk[ui2_buf_agt_id]));
                b_old_data_blk[ui2_buf_agt_id] = TRUE;
#endif
            }

            b_ret = FeederNotifyHandle(gat_bagt_data[ui2_buf_agt_id].eDataType, eCondition, (UINT32)&rData);
        }
#ifdef BAGT_URI_MODE_1
        _u4LastAckEndWPtr = rData.u4WriteAddr + rData.u4ReadSize;
        _aarConsumeList[ui2_buf_agt_id][_au4ConsumeListWp[ui2_buf_agt_id]].u4EndWPtr = _u4LastAckEndWPtr;
        x_memcpy(&_aarConsumeList[ui2_buf_agt_id][_au4ConsumeListWp[ui2_buf_agt_id]].rBagtDataBlk, pt_data_blk, sizeof(BUF_AGT_DATA_BLK_T));
        _au4ConsumeListWp[ui2_buf_agt_id]++;
        _au4ConsumeListWp[ui2_buf_agt_id] %= (Bagt_Q_SIZE + 1);
        ASSERT(_au4ConsumeListWp[ui2_buf_agt_id] != _au4ConsumeListRp[ui2_buf_agt_id]);
#else
#if 1  // megaa 20100421
        if (gat_bagt_data[ui2_buf_agt_id].fgQuickConsume)  // megaa 20100513
        {
            buf_agt_mw_data_consumed(ui2_buf_agt_id, pt_data_blk);
        }
        else if (!gat_bagt_data[ui2_buf_agt_id].fgPushMode)
        {
            if (b_old_data_blk[ui2_buf_agt_id])
            {
                buf_agt_mw_data_consumed(ui2_buf_agt_id, &t_old_data_blk[ui2_buf_agt_id]);
                b_old_data_blk[ui2_buf_agt_id] = FALSE;
            }
            x_memcpy(&t_old_data_blk[ui2_buf_agt_id], pt_data_blk, sizeof(t_old_data_blk[ui2_buf_agt_id]));
            b_old_data_blk[ui2_buf_agt_id] = TRUE;
        }
#endif
#endif
    }
    if (FALSE == b_ret)
    {
        return FALSE;
    }

    return b_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_src_align_info
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_src_align_info(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T*   pt_align_info)
{
    *pt_align_info = gat_bagt_data[ui2_buf_agt_id].t_align_info;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_ack_fct_tab
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_ack_fct_tab(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_FCT_TAB_T*          pt_ack_fct)
{
    pt_ack_fct->pf_data_req_ack = gat_bagt_data[ui2_buf_agt_id].pf_mw_ack_fct;
    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_position
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_position(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_POSITION_T*         pt_position)
{
    BOOL                        b_ret;

    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_get_pos)
    {
        return BAGTR_NOT_REGISTERED;
    }

    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_get_pos(
                                    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                    &pt_position->ui4_current_req_id,
                                    &pt_position->ui4_current_byte_offset);
    if (FALSE == b_ret)
    {
        return BAGTR_FAILED;
    }

    return BAGTR_OK;
}

#ifdef BAGT_URI_MODE_2
/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_buffer_fullness
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_buffer_fullness(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_BUFFER_FULLNESS_T*  pt_fullness)
{
    INT32                        b_ret;

    if (gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder)
    {
        return BAGTR_FAILED;
    }
    
    b_ret = FeederGetBufferFullness(gat_bagt_data[ui2_buf_agt_id].eDataType,
                                    &pt_fullness->ui4_length,
                                    &pt_fullness->ui4_percentage);
    if (FEEDER_E_OK != b_ret)
    {
        return BAGTR_FAILED;
    }

    return BAGTR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get_shared_memory
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get_shared_memory(
    BAGT_COMP_ID_T                          ui2_buf_agt_id,
    BUF_AGT_BUFFER_SHARED_MEMORY_T* pt_shared_memory)
{
    INT32             b_ret;
    FEEDER_BUF_INFO_T rFeederBufInfo;
    UINT32            u4Size;
#ifdef LINUX_TURNKEY_SOLUTION
    UINT32            u4Addr;
#endif

    if (gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder)
    {
        return BAGTR_FAILED;
    }
    
    b_ret = FeederGetBufferInfo(gat_bagt_data[ui2_buf_agt_id].eDataType,
                                    &rFeederBufInfo);
    if (FEEDER_E_OK != b_ret)
    {
        return BAGTR_FAILED;
    }
    
    gat_bagt_data[ui2_buf_agt_id].u4BufAddr = rFeederBufInfo.u4StartAddr;
    u4Size = rFeederBufInfo.u4Size;
#ifdef LINUX_TURNKEY_SOLUTION
    u4Addr = PHYSICAL(rFeederBufInfo.u4StartAddr);
    
    pt_shared_memory->pv_buffer     = (VOID *)adapt_fbm_knl_to_usr(rFeederBufInfo.u4StartAddr);
    pt_shared_memory->pv_private    = (VOID *)u4Addr;

#else
    pt_shared_memory->pv_buffer     = (VOID *)rFeederBufInfo.u4StartAddr;
    pt_shared_memory->pv_private    = (VOID *)rFeederBufInfo.u4StartAddr;
#endif
    pt_shared_memory->z_buffer_size = u4Size;

    return BAGTR_OK;
}


#ifdef FEEDER_MULTI_INSTANCE
static UINT32 _buf_agt_QueryFeeder(UINT8 u1AttachedSrcId, 
    FeederSourceType eSrcType, 
    FEEDER_BUF_INFO_T *pQryBufInfo, 
    FEEDER_REQ_DATA_T* pt_ReqData)
#else
static UINT32 _buf_agt_QueryFeeder(FeederSourceType eSrcType, FEEDER_BUF_INFO_T *pQryBufInfo, 
	                                      FEEDER_REQ_DATA_T* pt_ReqData)
	                                      
#endif
{
    BUF_AGT_DATA_BLK_T rDataBlock;
#ifdef LINUX_TURNKEY_SOLUTION
    //BUF_AGT_DATA_BLK_T *prDataBlock;
#endif
    BAGT_COMP_ID_T u2_id;
    INT32 i4Ret;
    BOOL b_ret;

    u2_id = ui2_bagt_id[eSrcType];
    rDataBlock.pui1_buff = (UINT8*)pt_ReqData->u4WriteAddr;
   
    rDataBlock.ui8_offset = pt_ReqData->u8FilePos;            
    rDataBlock.z_size = (UINT32)pt_ReqData->u4ReadSize;
    rDataBlock.ui4_flags = BUF_AGT_SET_DATA_OFFSET;

    if (u2_id >= BAGT_COMP_NS)
    {
        return 0;
    }
    rDataBlock.ui4_req_id = u4IdCnt[u2_id];
    u4IdCnt[u2_id]++;

#ifndef LINUX_TURNKEY_SOLUTION
    b_ret = gat_bagt_data[u2_id].pf_mw_nfy(
                                    gat_bagt_data[u2_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    (UINT32)&rDataBlock,
                                    (UINT32)NULL);
#else
    set_mmap_bagt_nfy(u2_id);
    b_ret = bagt_mmap_nfy_fct( gat_bagt_data[u2_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    gat_bagt_data[u2_id].pid,
                                    (UINT32)&rDataBlock,
                                    (UINT32)NULL);
#endif

	
    if (!b_ret)
    {
	    LOG(5,"Bagt requestdata return fail\n");
		return 0;
    }
	
    gat_bagt_data[u2_id].u4ReqCount++;
	
    i4Ret = x_msg_q_send(_hBagtMsgQ[u2_id], pt_ReqData, sizeof(FEEDER_REQ_DATA_T), 255);
    
    LOG(7, "_hBagtMsgQ => id=%ld size=%ld align= 0x%x offset=0x%08x write=0x%08x\n", 
                        rDataBlock.ui4_req_id, pt_ReqData->u4ReadSize, pt_ReqData->u4AlignOffset, 
                        (UINT32)(rDataBlock.ui8_offset&0xffffffff), (pt_ReqData->u4WriteAddr));

    //LOG(0, "send id = %d\n", rDataBlock.ui4_req_id);
    
	while (i4Ret == OSR_TOO_MANY)
	{
		x_thread_delay(10);
		i4Ret = x_msg_q_send(_hBagtMsgQ[u2_id], pt_ReqData, sizeof(FEEDER_REQ_DATA_T), 255);
        
        LOG(7, "_hBagtMsgQ => id=%ld size=%ld align= 0x%x offset=0x%08x\n", 
                            rDataBlock.ui4_req_id, pt_ReqData->u4ReadSize, pt_ReqData->u4AlignOffset, 
                            (UINT32)(rDataBlock.ui8_offset&0xffffffff));
	}
    
    ui8_bagt_offset[u2_id] = 0;

    if (b_ret)
    {
#ifdef BAGT_URI_MODE_1
        _u4LastReqEndWPtr = pt_ReqData->u4WriteAddr + pt_ReqData->u4ReadSize;
#endif
        return pt_ReqData->u4ReadSize;
    }
    else
    {
        return 0;
    }    
}

// megaa 20100414
static VOID _buf_agt_PreLoad(BAGT_COMP_ID_T ui2_id)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    BUF_AGT_DATA_BLK_T rDataBlock;
#ifdef LINUX_TURNKEY_SOLUTION
    //BUF_AGT_DATA_BLK_T *prDataBlock;
#endif
    BOOL b_ret;

    u4Addr = u4Size = 0;
    FeederInitPreLoad(gat_bagt_data[ui2_id].eDataType, &u4Addr, &u4Size);
    
    rDataBlock.pui1_buff = (UINT8*)u4Addr;
    rDataBlock.ui8_offset = 0;            
    rDataBlock.z_size = u4Size;
    rDataBlock.ui4_flags = BUF_AGT_SET_DATA_OFFSET;
    rDataBlock.ui4_req_id = 0x9999;

#ifndef LINUX_TURNKEY_SOLUTION
    b_ret = gat_bagt_data[ui2_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    (UINT32)&rDataBlock,
                                    (UINT32)NULL);
#else
    set_mmap_bagt_nfy(ui2_id);
    b_ret = bagt_mmap_nfy_fct( gat_bagt_data[ui2_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    gat_bagt_data[ui2_id].pid,
                                    (UINT32)&rDataBlock,
                                    (UINT32)NULL);
#endif

    if (b_ret)
    {
        // success
    }
    else
    {
        // failed (Todo: error handling?)
    }
}

// megaa 20100414
static UINT32 _buf_agt_Push_Consume(FeederSourceType eSrcType, FEEDER_BUF_INFO_T *pQryBufInfo, UINT32 u4NewRPtr)
{
    BAGT_COMP_ID_T ui2_id = ui2_bagt_id[eSrcType];
#if 1
    INT32 i4Ret;
    SIZE_T zMsgSize;
    UINT16 u2MsgQIdx;
    BUF_AGT_DATA_BLK_T rMsgQ;
    
    if (ui2_id >= BAGT_COMP_NS)
    {
        ASSERT(0);
        return 0;
    }
    
    zMsgSize= sizeof(BUF_AGT_DATA_BLK_T);
    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rMsgQ, &zMsgSize,
				(void*)&_hBagtOldDataBlkQ[ui2_id], 1, X_MSGQ_OPTION_WAIT);
    if (i4Ret != OSR_OK)
    {
        printf("i4Ret = %d, &_hBagtOldDataBlkQ = 0x%08X\n", i4Ret, (UINT32)&_hBagtOldDataBlkQ);
        ASSERT(0);
    }
    if (zMsgSize != sizeof(BUF_AGT_DATA_BLK_T))
    {
        printf("zMsgSize = %d\n", zMsgSize);
        ASSERT(0);
    }
    buf_agt_mw_data_consumed(ui2_id, &rMsgQ);
#else
    if (b_old_data_blk[ui2_id])
    {
        buf_agt_mw_data_consumed(ui2_id, &t_old_data_blk[ui2_id]);
        b_old_data_blk[ui2_id] = FALSE;
    }
#endif
    
    return 0;
}

#ifdef BAGT_URI_MODE_1
static UINT32 _buf_agt_Consume(FeederSourceType eSrcType, FEEDER_BUF_INFO_T *pQryBufInfo, UINT32 u4NewRPtr)
{
    UINT32 u4OldRPtr = pQryBufInfo->u4ReadAddr;
    UINT32 u4EndWPtr;
    BAGT_COMP_ID_T ui2_id = ui2_bagt_id[eSrcType];
    BOOL fgDoConsume = FALSE;
    UINT32 i;
    
    //ASSERT(eSrcType == FEEDER_PROGRAM_SOURCE);
    
    if (_u4LastReqEndWPtr == u4NewRPtr)  // tmp solution, so tricky!!!
    {
        while (_u4LastReqEndWPtr != _u4LastAckEndWPtr)
        {
            x_thread_delay(10);  // ToDo: use mutex to protect racing condition of accessing _u4LastAckEndWPtr
        }
    }
    
    while (1)
    {
        fgDoConsume = FALSE;
        for (i = _au4ConsumeListRp[ui2_id]; i != _au4ConsumeListWp[ui2_id]; )
        {
            u4EndWPtr = _aarConsumeList[ui2_id][i].u4EndWPtr;
            if ((u4OldRPtr <  u4EndWPtr && u4NewRPtr >= u4EndWPtr) ||
        	    (u4OldRPtr <  u4EndWPtr && u4NewRPtr <  u4EndWPtr && u4NewRPtr < u4OldRPtr) ||
        	    (u4OldRPtr >= u4EndWPtr && u4NewRPtr >= u4EndWPtr && u4NewRPtr < u4OldRPtr))
            {
                fgDoConsume = TRUE;
                break;
            }
            i++;
            i %= (Bagt_Q_SIZE + 1);
        }
    
        if (fgDoConsume)
        {
            ASSERT(i == _au4ConsumeListRp[ui2_id]);
            buf_agt_mw_data_consumed(ui2_id, &_aarConsumeList[ui2_id][i].rBagtDataBlk);
            _au4ConsumeListRp[ui2_id]++;
            _au4ConsumeListRp[ui2_id] %= (Bagt_Q_SIZE + 1);
        }
        else
        {
            break;
        }
    }
    
    return 0;
}

static VOID _buf_agt_AllConsume(FeederSourceType eSrcType)
{
    BAGT_COMP_ID_T ui2_id = ui2_bagt_id[eSrcType];
    UINT32 i;
    
    ASSERT(eSrcType == FEEDER_PROGRAM_SOURCE);

    while (_u4LastReqEndWPtr != _u4LastAckEndWPtr)
    {
        x_thread_delay(10);  // ToDo: use mutex to protect racing condition of accessing _u4LastAckEndWPtr
    }

    for (i = _au4ConsumeListRp[ui2_id]; i != _au4ConsumeListWp[ui2_id]; )
    {
        buf_agt_mw_data_consumed(ui2_id, &_aarConsumeList[ui2_id][i].rBagtDataBlk);
        i++;
        i %= (Bagt_Q_SIZE + 1);
    }
}
#endif

#ifdef BAGT_URI_MODE_2
// megaa 20100414
static VOID _buf_agt_Overflow(FeederSourceType eSrcType)
{
    BAGT_COMP_ID_T ui2_id = ui2_bagt_id[eSrcType];
    
    ASSERT(eSrcType == FEEDER_PROGRAM_SOURCE);
    buf_agt_mw_data_overflow(ui2_id);
}
// megaa 20100414
static VOID _buf_agt_Underflow(FeederSourceType eSrcType)
{
    BAGT_COMP_ID_T ui2_id = ui2_bagt_id[eSrcType];
    
    ASSERT(eSrcType == FEEDER_PROGRAM_SOURCE);
    buf_agt_mw_data_underflow(ui2_id);
}
#endif

#ifdef FEEDER_MULTI_INSTANCE
static INT64 _buf_agt_Seek(UINT8 u1AttachedSrcId, FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo)
#else
static INT64 _buf_agt_Seek(FeederSourceType eSrcType, MM_FILE_OPS_T *prFileInfo)
#endif
{
    BAGT_COMP_ID_T u2_id;
    u2_id = ui2_bagt_id[eSrcType];
    
    if (u2_id >= BAGT_COMP_NS)
    {
        return (INT64)-1;
    }
    
    ui8_bagt_offset[u2_id] = prFileInfo->u8Offset;
    /* ui4_bagt_qid = prFileInfo->u4QueryID; */

    return (INT64)ui8_bagt_offset[u2_id];
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_feeder_buffer_layout
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_feeder_buffer_layout(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_SOURCE_TYPE_T         type)
{

    if(ui2_buf_agt_id>BAGT_COMP_NS)
    {
        return BAGTR_INV_ARG;
    }

    if(type==BUF_AGT_SOURCE_TYPE_DEFAULT)
    {
        FeederSetSkypeSourceFlag(FALSE);
    }
    else
    {	//skype bufffer layout, no double buffer
        FeederSetSkypeSourceFlag(TRUE);
    }

    return BAGTR_OK;
}
/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_nfy_fct
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_nfy_fct(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_NFY_INFO_T*         pt_nfy_info)
{
    pt_nfy_info->pv_previous_tag    = gat_bagt_data[ui2_buf_agt_id].pv_mw_tag;
    pt_nfy_info->pf_previous_nfy    = gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy;

    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag         = pt_nfy_info->pv_tag;
#ifndef LINUX_TURNKEY_SOLUTION
    gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy         = pt_nfy_info->pf_nfy;
#else
    bagt_nfy_tbl[ui2_buf_agt_id] = pt_nfy_info->pf_nfy;
    gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy = (x_buf_agt_nfy_fct)bagt_nfy_fct;
#endif

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_src_align_info
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_src_align_info(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T*   pt_align_info)
{
    UINT32                      ui4_align;

    /* error checking 
        1. ui4_align should be > 0.
        2. ui4_align should be powered of 2. */
    if (pt_align_info->ui4_align == 0)
    {
        return BAGTR_FAILED;
    }

    ui4_align = pt_align_info->ui4_align;
    
    while (ui4_align)
    {
        if (ui4_align & 0x1)
        {
            /* get non-zero bit, doing check */
            ui4_align >>= 1;
            if (ui4_align)
            {
                return BAGTR_FAILED;
            }
        }
        else
        {
            ui4_align >>= 1;
        }
    }

    /* Send the notify to audio driver if someone changes the alignment info */
    if (NULL != gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy)
    {
        BOOL b_ret;

        b_ret = gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy(
                                        gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                        BUF_AGT_MW_NFY_COND_ALIGN_INFO,
                                        (UINT32)pt_align_info,
                                        0);
        if (FALSE == b_ret)
        {
            return BAGTR_FAILED;
        }
    }

    gat_bagt_data[ui2_buf_agt_id].t_align_info = *pt_align_info;

   
#ifdef FEEDER_MULTI_INSTANCE
    FeederSetBagtInfo(gat_bagt_data[ui2_buf_agt_id].eDataType, pt_align_info->ui4_align, pt_align_info->ui4_min_blk_size);
#else
    FeederSetBagtInfo(pt_align_info->ui4_align, pt_align_info->ui4_min_blk_size);
#endif

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_flush_done
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_flush_done(
    BAGT_COMP_ID_T                      ui2_buf_agt_id)
{
    BOOL                        b_ret;

    if (gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy)
    {
        b_ret = gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag,
                                    BUF_AGT_MW_NFY_COND_FLUSH_DONE,
                                    0,
                                    0);
        if (FALSE == b_ret)
        {
        }
    }
    else
    {
    }

    return BAGTR_OK;
}


#ifdef BAGT_URI_MODE_2
/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_flow_ctrl
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_flow_ctrl(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BUF_AGT_FLOW_CTRL_T*        pt_flow_ctrl)
{
    // do basic checking, tmp solution
    if (pt_flow_ctrl->ui4_buffer_len < 10 * 1024 || pt_flow_ctrl->ui4_buffer_len > FEEDER_DEFAULT_BUF_SIZE)
    {
        return BAGTR_INV_ARG;
    }
    
    // do basic checking, tmp solution
    if (pt_flow_ctrl->ui4_keep_buffer_threshold >= pt_flow_ctrl->ui4_buffer_len / 2)
    {
        return BAGTR_INV_ARG;
    }
    
    // do basic checking, tmp solution
    if (pt_flow_ctrl->ui4_re_buferf_threshold == 0 || pt_flow_ctrl->ui4_re_buferf_threshold > pt_flow_ctrl->ui4_buffer_len)
    {
        return BAGTR_INV_ARG;
    }

    if (FEEDER_E_OK != FeederSetUriMode(gat_bagt_data[ui2_buf_agt_id].eDataType, pt_flow_ctrl->ui4_buffer_len, pt_flow_ctrl->ui4_keep_buffer_threshold, pt_flow_ctrl->ui4_re_buferf_threshold))
    {
        return BAGTR_FAILED;
    }
    
    gat_bagt_data[ui2_buf_agt_id].t_flow_ctrl    = *pt_flow_ctrl;
    gat_bagt_data[ui2_buf_agt_id].fgFlowCtrl     = TRUE;
    gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_NONE;
    gat_bagt_data[ui2_buf_agt_id].fgPushMode     = FALSE;
    gat_bagt_data[ui2_buf_agt_id].u4BufAddr      = 0;
    
    return BAGTR_OK;
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set_push_mode
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set_push_mode(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    BOOL                        fgPushMode)
{
    gat_bagt_data[ui2_buf_agt_id].fgPushMode = fgPushMode;
    
    return BAGTR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_drv_init
 *
 * Description: <function description>
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
static BOOL _buf_agt_drv_init(VOID)
{
    INT32       i4_idx;

    if (gb_bagt_inited)
    {
        return TRUE;
    }

    for (i4_idx = 0; i4_idx < BAGT_COMP_NS; i4_idx++)
    {
        gat_bagt_data[i4_idx].pf_drv_nfy                     = NULL;
        gat_bagt_data[i4_idx].pf_get_pos                     = NULL;
        gat_bagt_data[i4_idx].pf_mw_nfy                      = NULL;
        gat_bagt_data[i4_idx].pv_drv_tag                     = NULL;
        gat_bagt_data[i4_idx].pv_mw_tag                      = NULL;
        u4IdCnt[i4_idx]=0;
#ifndef LINUX_TURNKEY_SOLUTION
        gat_bagt_data[i4_idx].pf_mw_ack_fct          = _buf_agt_req_ack_fct;
#else
        gat_bagt_data[i4_idx].pf_mw_ack_fct          = (x_buf_agt_data_req_ack_fct)g_pf_buf_agt_req_act_fct;
#endif
#ifdef BAGT_REORDER_ACK
        x_memset((void *)&_rMwAckList[i4_idx],0,sizeof(MW_ACK_LIST_T));
        _rMwAckList[i4_idx].fgWaitFirst=TRUE;
#endif
        /* the default alignment value is 1 */
        gat_bagt_data[i4_idx].t_align_info.ui4_align         = BLOCK_DEF_ALIGNMENT;
        gat_bagt_data[i4_idx].t_align_info.ui4_min_blk_size  = 512;
        gat_bagt_data[i4_idx].bNotUseFeeder          = FALSE;
        gat_bagt_data[i4_idx].fgFlowCtrl             = FALSE;
        gat_bagt_data[i4_idx].eFlowCtrlState         = BAGT_FLWCTL_NONE;
        gat_bagt_data[i4_idx].fgPushMode             = FALSE;
        gat_bagt_data[i4_idx].u4BufAddr              = 0;
        gat_bagt_data[i4_idx].pid                    = 0;
        gat_bagt_data[i4_idx].fgQuickConsume         = FALSE;
        gat_bagt_data[i4_idx].u4AckCount = 0;
        gat_bagt_data[i4_idx].u4ConsumeCount = 0;
        gat_bagt_data[i4_idx].u4ReqCount = 0;
#ifdef FEEDER_MULTI_INSTANCE
        FeederSetBagtInfo(gat_bagt_data[i4_idx].eDataType, BLOCK_DEF_ALIGNMENT, gat_bagt_data[i4_idx].t_align_info.ui4_min_blk_size);
#endif
    }
#ifndef FEEDER_MULTI_INSTANCE
    FeederSetBagtInfo(BLOCK_DEF_ALIGNMENT, gat_bagt_data[0].t_align_info.ui4_min_blk_size);
#endif

#ifdef CLI_SUPPORT
    ui2_bagt_dbg_level = DBG_LEVEL_NONE;
#endif /* CLI_SUPPORT */

    gb_bagt_inited = TRUE;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_get
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_get_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_get(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID*           pv_data,
    SIZE_T*         pz_size)
{
    INT32           i4_ret = RMR_OK;

    /* Check if initialized */
    if (!gb_bagt_inited)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    /* Check component ID */
    if ((pt_comp_id == NULL) ||
        (pt_comp_id->e_type != DRVT_BUF_AGENT) ||
        (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1)))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    /* handle get type */
    switch (e_get_type)
    {
        case BUF_AGT_GET_TYPE_SRC_ALIGN_INFO:
            /* Get source DMA alignment information */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_SRC_ALIGN_INFO_T)))
            {
                *pz_size = sizeof(BUF_AGT_SRC_ALIGN_INFO_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_src_align_info((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                                (BUF_AGT_SRC_ALIGN_INFO_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;

        case BUF_AGT_GET_TYPE_ACK_FCT_TAB:
            /* Get Acknowledgement function table */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_FCT_TAB_T)))
            {
                *pz_size = sizeof(BUF_AGT_FCT_TAB_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_ack_fct_tab((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                             (BUF_AGT_FCT_TAB_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;

        case BUF_AGT_GET_TYPE_POSITION:
            /* Get Position of media data which is playing on */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_POSITION_T)))
            {
                *pz_size = sizeof(BUF_AGT_POSITION_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_position((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                          (BUF_AGT_POSITION_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;

#ifdef BAGT_URI_MODE_2
        case BUF_AGT_GET_TYPE_BUFFER_FULLNESS:
            /* Get buffer fullness for URI mode */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_BUFFER_FULLNESS_T)))
            {
                *pz_size = sizeof(BUF_AGT_BUFFER_FULLNESS_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_buffer_fullness((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                          (BUF_AGT_BUFFER_FULLNESS_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;
#endif

        case BUF_AGT_GET_TYPE_SHARED_MEMORY:
            /* Get shared memory for push mode */
            if (NULL == pz_size)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if ((NULL == pv_data) ||
                (*pz_size < sizeof(BUF_AGT_BUFFER_SHARED_MEMORY_T)))
            {
                *pz_size = sizeof(BUF_AGT_BUFFER_SHARED_MEMORY_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            i4_ret = _buf_agt_get_shared_memory((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                          (BUF_AGT_BUFFER_SHARED_MEMORY_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_GET_FAILED;
            }
            break;
#ifdef ENABLE_MULTIMEDIA               
#ifdef CC_53XX_SWDMX_V2            
        case BUF_AGT_GET_TYPE_TIME_TO_SIZE:
            if(NULL == pv_data)
            {
                i4_ret = RMR_DRV_INV_GET_INFO;
            }
            else
            {
                SWDMX_GetInfo(0, eSWDMX_GET_TIME_TO_BYTES_INFO,&(((BUF_AGT_BUFFER_TIME_TO_SIZE_T*)(pv_data))->ui4_ms),&(((BUF_AGT_BUFFER_TIME_TO_SIZE_T*)(pv_data))->ui4_size),0);
            }
            break;
#endif            
#endif
        default:
            i4_ret = RMR_DRV_INV_GET_INFO;
            break;
    }

    return i4_ret;
}
 
/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_set_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
static INT32 _buf_agt_set(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_data,
    SIZE_T          z_size)
{
    INT32           i4_ret = RMR_OK;
    BUF_AGT_ACK_NFY_T  *prAck;

    /* Check if initialized */
    if (!gb_bagt_inited)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    /* Check component ID */
    if ((pt_comp_id == NULL) || 
        (pt_comp_id->e_type != DRVT_BUF_AGENT) ||
        (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1)))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    /* handle set type here */
    switch (e_set_type)
    {

        case BUF_AGT_SET_TYPE_SOURCE_TYPE:
            /* Set Notify function table */
            if (((BUF_AGT_SOURCE_TYPE_T)((UINT32)pv_data) > BUF_AGT_SOURCE_TYPE_SKYPE)||
                (z_size < sizeof(BUF_AGT_SOURCE_TYPE_T)))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4_ret = _buf_agt_set_feeder_buffer_layout(pt_comp_id->ui2_id,
                                         (BUF_AGT_SOURCE_TYPE_T)((UINT32)pv_data));
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;
            
        case BUF_AGT_SET_TYPE_NFY_FCT:
            /* Set Notify function table */
            if ((NULL == pv_data) ||
                (z_size < sizeof(BUF_AGT_NFY_INFO_T)))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4_ret = _buf_agt_set_nfy_fct((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                         (BUF_AGT_NFY_INFO_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;

        case BUF_AGT_SET_TYPE_SRC_ALIGN_INFO:
            /* Set source DMA alignment information */
            if ((NULL == pv_data) ||
                (z_size < sizeof(BUF_AGT_SRC_ALIGN_INFO_T)))
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4_ret = _buf_agt_set_src_align_info((BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                                (BUF_AGT_SRC_ALIGN_INFO_T*)pv_data);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;

        case BUF_AGT_SET_TYPE_FLUSH_DONE:
            /* Notify decoder that the flush request is finished */
            i4_ret = _buf_agt_set_flush_done((BAGT_COMP_ID_T)pt_comp_id->ui2_id);
            if (BAGTR_OK != i4_ret)
            {
                i4_ret = RMR_DRV_SET_FAILED;
            }
            break;

        case BUF_AGT_SET_TYPE_ACK:
        {
#ifndef BAGT_REORDER_ACK
            prAck = (BUF_AGT_ACK_NFY_T *)pv_data;
            if (!_buf_agt_req_ack_fct((UINT16)pt_comp_id->ui2_id, prAck->e_cond, &prAck->t_block))
            {
               i4_ret = RMR_DRV_SET_FAILED;
            }
#else
            MW_ACK_LIST_T *pMwAckList=&_rMwAckList[(BAGT_COMP_ID_T)pt_comp_id->ui2_id];
            prAck = (BUF_AGT_ACK_NFY_T *)pv_data;
			
            if(pMwAckList->u4AckNum>BAGT_MW_ACKLIST_MAX)
            {
                _buf_agt_acklist_clean(pMwAckList);
                pMwAckList->u4AckNum=0;
                pMwAckList->u4WaitAckId=prAck->t_block.ui4_req_id;
            }
			if(!_hBagtMsgQ[(BAGT_COMP_ID_T)pt_comp_id->ui2_id])
			{
               return RMR_DRV_SET_FAILED;
			}
			if(pMwAckList->fgWaitFirst && (prAck->t_block.ui4_req_id<pMwAckList->u4WaitAckId))
			{
               return RMR_DRV_SET_FAILED;
			}

			if(pMwAckList->fgWaitFirst && (prAck->t_block.ui4_req_id==pMwAckList->u4WaitAckId))
			{
               pMwAckList->fgWaitFirst=FALSE;
			}
			
            if(prAck->t_block.ui4_req_id!=pMwAckList->u4WaitAckId)
            {
                if(!_buf_agt_acklist_add(pMwAckList,prAck->e_cond,&prAck->t_block))
                {
                    i4_ret = RMR_DRV_SET_FAILED;
                }
                else
                {
                    pMwAckList->u4AckNum++;
                }
            }
            else
            {
                if (!_buf_agt_req_ack_fct((UINT16)pt_comp_id->ui2_id, prAck->e_cond, &prAck->t_block))
                {
                    i4_ret = RMR_DRV_SET_FAILED;
                    break;
                }
                if(pMwAckList->u4AckNum)
                {
                    UINT32 u4Index=0,u4ReqId=0,u4AckNum=0;
                    BUF_AGT_DATA_REQ_ACK_COND_T blk_cond=BUF_AGT_DATA_REQ_ACK_COND_UNKNOWN;
                    BUF_AGT_DATA_BLK_T *pBlk=NULL;
                    u4ReqId=prAck->t_block.ui4_req_id+1;
                    u4AckNum=pMwAckList->u4AckNum;
				    pMwAckList->u4WaitAckId=u4ReqId;

                    for(u4Index=0;u4Index<u4AckNum;u4Index++)
                    {
                        if(!_buf_agt_acklist_get(pMwAckList,u4ReqId,&pBlk,&blk_cond))
                        {
                            break;
                        }
                        pMwAckList->u4AckNum--;
                        u4ReqId++;
                        pMwAckList->u4WaitAckId=u4ReqId;
                        if (!_buf_agt_req_ack_fct((UINT16)pt_comp_id->ui2_id, blk_cond,pBlk))
                        {
                            i4_ret = RMR_DRV_SET_FAILED;
                            break;
                        }
                    } 
                }
                else
                {
                    pMwAckList->u4WaitAckId=prAck->t_block.ui4_req_id+1;
                }
            }
#endif
            break;
        }

        case BUF_AGT_SET_TYPE_PRELOAD:
            gat_bagt_data[(BAGT_COMP_ID_T)pt_comp_id->ui2_id].eFlowCtrlState = BAGT_FLWCTL_PRE_LOAD_PENDING;
            i4_ret = RMR_OK;
            break;

        case BUF_AGT_SET_OPERATION_MODE:
            {
                BUF_MODE_CTRL_T t_mode_ctrl;
                if ((NULL == pv_data) ||
                    (z_size < sizeof(BUF_MODE_CTRL_T)))
                {
                    return RMR_DRV_INV_SET_INFO;
                }
                t_mode_ctrl = *(BUF_MODE_CTRL_T*)pv_data;
                gat_bagt_data[(BAGT_COMP_ID_T)pt_comp_id->ui2_id].pid = t_mode_ctrl.ui4_pid;
                gat_bagt_data[(BAGT_COMP_ID_T)pt_comp_id->ui2_id].fgQuickConsume = t_mode_ctrl.b_quick_consume;
#ifdef ENABLE_MULTIMEDIA   
#ifdef CC_53XX_SWDMX_V2
                //set to swdmx pre buffer threshold
                if(t_mode_ctrl.t_feeder_buf_info.e_mode == BUF_AGT_FEEDER_BUF_MODE_TIME)
                {
                    SWDMX_SetInfo(0,eSWDMX_SET_PLAYBACK_THRESHOLD,eSWDMX_BUF_MODE_TIME,t_mode_ctrl.t_feeder_buf_info.u.ui4_time_ms,0);
                }
                else if(t_mode_ctrl.t_feeder_buf_info.e_mode == BUF_AGT_FEEDER_BUF_MODE_BYTE)
                {
                    SWDMX_SetInfo(0,eSWDMX_SET_PLAYBACK_THRESHOLD,eSWDMX_BUF_MODE_BYTE,0,0);
                }
#endif
#endif
                switch (t_mode_ctrl.e_op_mode)
                {
                    case BUF_AGT_OPERATION_MODE_PULL:
                        i4_ret = _buf_agt_set_push_mode(
                                    (BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                    FALSE);
                        if (BAGTR_OK != i4_ret)
                        {
                            i4_ret = RMR_DRV_SET_FAILED;
                        }
                        else
                        {
                            i4_ret = RMR_OK;
                        }
                        break;
                        
                    case BUF_AGT_OPERATION_MODE_CIRCULAR:
                        i4_ret = _buf_agt_set_push_mode(
                                    (BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                    FALSE);
                        if (BAGTR_OK != i4_ret)
                        {
                            i4_ret = RMR_DRV_SET_FAILED;
                            break;
                        }
#ifdef BAGT_URI_MODE_2
                        i4_ret = _buf_agt_set_flow_ctrl(
                                    (BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                    &t_mode_ctrl.u.t_flow_ctrl);
#else
                        i4_ret = BAGTR_FAILED;
#endif
                        if (BAGTR_OK != i4_ret)
                        {
                            i4_ret = RMR_DRV_SET_FAILED;
                        }
                        else
                        {
                            i4_ret = RMR_OK;
                        }
                        break;
                        
                    case BUF_AGT_OPERATION_MODE_PUSH_SHARE_MEM:
                        i4_ret = _buf_agt_set_push_mode(
                                    (BAGT_COMP_ID_T)pt_comp_id->ui2_id,
                                    TRUE);
                        if (BAGTR_OK != i4_ret)
                        {
                            i4_ret = RMR_DRV_SET_FAILED;
                        }
                        else
                        {
                            i4_ret = RMR_OK;
                        }
                        break;
                        
                    default:
                        i4_ret = RMR_DRV_SET_FAILED;
                        break;
                }
            }
            break;

        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
            break;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_cli_init
 *
 * Description: This API initializes Buffer Agent CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: BAGTR_OK                    Routine successful.
 *          BAGTR_CLI_ERROR             CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 _buf_agt_cli_init(VOID)
{
#ifdef CLI_SUPPORT
    INT32       i4_ret;

    /* Attach Buffer Agent CLI command table to CLI */
    i4_ret = x_cli_attach_cmd_tbl(at_bagt_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);

    if (i4_ret != CLIR_OK)
    {
        return BAGTR_CLI_ERROR;
    }
    return BAGTR_OK;
#else
    return BAGTR_OK;
#endif
}

//=====================================================================
// Interface functions

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_connect
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pf_nfy          References.
 *          pf_get_pos      References.
 *          pv_tag          References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_connect(
    BAGT_COMP_ID_T                      ui2_buf_agt_id,
    buf_agt_mw_nfy_fct          pf_nfy,
    buf_agt_mw_get_position_fct pf_get_pos, 
    VOID*                       pv_tag)
{
    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* pt_nfy and pf_get_pos can't be NULL.
       pv_tag could be NULL */
    if ((NULL == pf_nfy) || (NULL == pf_get_pos))
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if Buffer Agent has been connected */
    if ((gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy) ||
        (gat_bagt_data[ui2_buf_agt_id].pf_get_pos))
    {
        return BAGTR_ALREADY_CONN;
    }

    /* set the value */
    gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy = pf_nfy;
    gat_bagt_data[ui2_buf_agt_id].pf_get_pos = pf_get_pos;
    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag = pv_tag;

    gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder = TRUE;
    gat_bagt_data[ui2_buf_agt_id].fgFlowCtrl     = FALSE;
    gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_NONE;
    gat_bagt_data[ui2_buf_agt_id].eDataType = FEEDER_AUDIO_SOURCE;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_disconnect
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_disconnect(
    BAGT_COMP_ID_T  ui2_buf_agt_id)
{
    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }
    //printk("disconn BAGT #%d: ack cnt = %d, consume cnt = %d, req cnt = %d\n", ui2_buf_agt_id, gat_bagt_data[ui2_buf_agt_id].u4AckCount, gat_bagt_data[ui2_buf_agt_id].u4ConsumeCount, gat_bagt_data[ui2_buf_agt_id].u4ReqCount);

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* set the value */
    gat_bagt_data[ui2_buf_agt_id].pf_drv_nfy = NULL;
    gat_bagt_data[ui2_buf_agt_id].pf_get_pos = NULL;
    gat_bagt_data[ui2_buf_agt_id].pv_drv_tag = NULL;

    gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder = FALSE;
    gat_bagt_data[ui2_buf_agt_id].fgFlowCtrl     = FALSE;
    gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_NONE;

    return BAGTR_OK;
}

INT32 buf_agt_dmx_connect(BAGT_COMP_ID_T ui2_buf_agt_id)
{
    FEEDER_QUERY_DATA rSetQuery;
    
    if (ui2_buf_agt_id >= BAGT_COMP_NS)
    {
        ASSERT(0);
        return RMR_DRV_ERROR;
    }
    
    gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder = FALSE;
    //gat_bagt_data[ui2_buf_agt_id].fgFlowCtrl     = FALSE;
    //gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_NONE;
    if (ui2_buf_agt_id == BAGT_COMP_1)
    {
    gat_bagt_data[ui2_buf_agt_id].eDataType = FEEDER_PROGRAM_SOURCE;
        ui2_bagt_id[FEEDER_PROGRAM_SOURCE] = BAGT_COMP_1;
    }
    else if (ui2_buf_agt_id == BAGT_COMP_2)
    {
        gat_bagt_data[ui2_buf_agt_id].eDataType = FEEDER_AUDIO_SOURCE;
        ui2_bagt_id[FEEDER_AUDIO_SOURCE] = BAGT_COMP_2;
    }
    #ifdef CC_SKYPE_FINE_INSTANCE
    else if (ui2_buf_agt_id == BAGT_COMP_3)
    {
        gat_bagt_data[ui2_buf_agt_id].eDataType = FEEDER_PROGRAM_SOURCE_2;
        ui2_bagt_id[FEEDER_PROGRAM_SOURCE_2] = BAGT_COMP_3;
    }
    else if (ui2_buf_agt_id == BAGT_COMP_4)
    {
        gat_bagt_data[ui2_buf_agt_id].eDataType = FEEDER_AUDIO_SOURCE_2;
        ui2_bagt_id[FEEDER_PROGRAM_SOURCE_2] = BAGT_COMP_4;
    }
    else if (ui2_buf_agt_id == BAGT_COMP_5)
    {
        gat_bagt_data[ui2_buf_agt_id].eDataType = FEEDER_AUDIO_SOURCE_3;
        ui2_bagt_id[FEEDER_PROGRAM_SOURCE_2] = BAGT_COMP_5;
    }    
    #endif
    gat_bagt_data[ui2_buf_agt_id].ui4_bagt_qid = 0;
    fgUseLastData[ui2_buf_agt_id]=FALSE;

    if ((ui2_buf_agt_id == BAGT_COMP_1) && (_hBagtMsgQ[BAGT_COMP_1] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtMsgQ[BAGT_COMP_1], "Bagt_CMDQ1", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
    }
    else if ((ui2_buf_agt_id == BAGT_COMP_2) && (_hBagtMsgQ[BAGT_COMP_2] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtMsgQ[BAGT_COMP_2], "Bagt_CMDQ2", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
    }
    #ifdef CC_SKYPE_FINE_INSTANCE
    else if ((ui2_buf_agt_id == BAGT_COMP_3) && (_hBagtMsgQ[BAGT_COMP_3] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtMsgQ[BAGT_COMP_3], "Bagt_CMDQ3", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
    }
    else if ((ui2_buf_agt_id == BAGT_COMP_4) && (_hBagtMsgQ[BAGT_COMP_4] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtMsgQ[BAGT_COMP_4], "Bagt_CMDQ4", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
    }
    else if ((ui2_buf_agt_id == BAGT_COMP_5) && (_hBagtMsgQ[BAGT_COMP_5] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtMsgQ[BAGT_COMP_5], "Bagt_CMDQ5", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
    }
    #endif
    
    if ((ui2_buf_agt_id == BAGT_COMP_1) && (_hBagtOldDataBlkQ[BAGT_COMP_1] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtOldDataBlkQ[BAGT_COMP_1], "Bagt_BLKQ1", sizeof(BUF_AGT_DATA_BLK_T), Bagt_Q_SIZE) == OSR_OK);
    }
    else if ((ui2_buf_agt_id == BAGT_COMP_2) && (_hBagtOldDataBlkQ[BAGT_COMP_2] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtOldDataBlkQ[BAGT_COMP_2], "Bagt_BLKQ2", sizeof(BUF_AGT_DATA_BLK_T), Bagt_Q_SIZE) == OSR_OK);
    }
    #ifdef CC_SKYPE_FINE_INSTANCE
    else if ((ui2_buf_agt_id == BAGT_COMP_3) && (_hBagtOldDataBlkQ[BAGT_COMP_3] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtOldDataBlkQ[BAGT_COMP_3], "Bagt_BLKQ3", sizeof(BUF_AGT_DATA_BLK_T), Bagt_Q_SIZE) == OSR_OK);
    }
    else if ((ui2_buf_agt_id == BAGT_COMP_4) && (_hBagtOldDataBlkQ[BAGT_COMP_4] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtOldDataBlkQ[BAGT_COMP_4], "Bagt_BLKQ4", sizeof(BUF_AGT_DATA_BLK_T), Bagt_Q_SIZE) == OSR_OK);
    }
    else if ((ui2_buf_agt_id == BAGT_COMP_5) && (_hBagtOldDataBlkQ[BAGT_COMP_5] == (HANDLE_T)NULL))
    {
        VERIFY(x_msg_q_create(&_hBagtOldDataBlkQ[BAGT_COMP_5], "Bagt_BLKQ5", sizeof(BUF_AGT_DATA_BLK_T), Bagt_Q_SIZE) == OSR_OK);
    }    
	#endif
	
#ifdef BAGT_URI_MODE_1
    _au4ConsumeListRp[ui2_buf_agt_id] = _au4ConsumeListWp[ui2_buf_agt_id] = 0;
    _u4LastReqEndWPtr = _u4LastAckEndWPtr = 0;
#else
    b_old_data_blk[ui2_buf_agt_id] = FALSE;
#endif
    
    FeederOpen(gat_bagt_data[ui2_buf_agt_id].eDataType);

#ifdef FEEDER_MULTI_INSTANCE
    FeederSetInfo(gat_bagt_data[ui2_buf_agt_id].eDataType, FEEDER_MM_USE, TRUE);
#else
    FeederSetInfo(FEEDER_MM_USE, TRUE);
#endif    
    if (!gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder)
    {
        rSetQuery.eDataType = (FeederSourceType)ui2_buf_agt_id;
        rSetQuery.pfnSource = _buf_agt_QueryFeeder;
        rSetQuery.pfnSeekPos= _buf_agt_Seek;
#ifdef BAGT_URI_MODE_1
        rSetQuery.pfnConsume = _buf_agt_Consume;
#else
        if (gat_bagt_data[ui2_buf_agt_id].fgPushMode)
        {
            rSetQuery.pfnConsume = _buf_agt_Push_Consume;
        }
        else
        {
            rSetQuery.pfnConsume = NULL;
        }
#endif
#ifdef BAGT_URI_MODE_2
        rSetQuery.pfnOverflow = _buf_agt_Overflow;
        rSetQuery.pfnUnderflow = _buf_agt_Underflow;
#else
        rSetQuery.pfnOverflow = NULL;
        rSetQuery.pfnUnderflow = NULL;
#endif
        rSetQuery.u4QuerySize = 0;
        FeederRegisterPullDataFunc(gat_bagt_data[ui2_buf_agt_id].eDataType, &rSetQuery);
    }    

#ifdef FEEDER_MULTI_INSTANCE
	FeederSetBagtInfo(gat_bagt_data[ui2_buf_agt_id].eDataType, gat_bagt_data[ui2_buf_agt_id].t_align_info.ui4_align, gat_bagt_data[ui2_buf_agt_id].t_align_info.ui4_min_blk_size);
    FeederSetFlush(gat_bagt_data[ui2_buf_agt_id].eDataType);
#else
	FeederSetBagtInfo(gat_bagt_data[ui2_buf_agt_id].t_align_info.ui4_align, gat_bagt_data[ui2_buf_agt_id].t_align_info.ui4_min_blk_size);
    FeederSetFlush();
#endif    
    
#ifdef LINUX_TURNKEY_SOLUTION
    set_bagt_nfy(ui2_buf_agt_id);
#endif

#ifdef BAGT_REORDER_ACK
    _buf_agt_acklist_free(&_rMwAckList[ui2_buf_agt_id]);
    _rMwAckList[ui2_buf_agt_id].u4AckNum=0;
    _rMwAckList[ui2_buf_agt_id].u4WaitAckId=u4IdCnt[ui2_buf_agt_id];
	_rMwAckList[ui2_buf_agt_id].fgWaitFirst=TRUE;
#endif
    gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(gat_bagt_data[ui2_buf_agt_id].pv_mw_tag, 
                            BUF_AGT_NFY_COND_FLUSH_REQ, (UINT32)NULL, (UINT32)NULL);
    
    
    FeederSetAppQueryMode(gat_bagt_data[ui2_buf_agt_id].eDataType, FEEDER_PULL_MODE);
    FeederStart(gat_bagt_data[ui2_buf_agt_id].eDataType);
    FeederSetRing(gat_bagt_data[ui2_buf_agt_id].eDataType, TRUE);

    if (gat_bagt_data[ui2_buf_agt_id].fgFlowCtrl)
    {
        BUF_AGT_FLOW_CTRL_T *pt_flow_ctrl = &(gat_bagt_data[ui2_buf_agt_id].t_flow_ctrl);
        
        if (FEEDER_E_OK != FeederSetUriMode(gat_bagt_data[ui2_buf_agt_id].eDataType, pt_flow_ctrl->ui4_buffer_len, pt_flow_ctrl->ui4_keep_buffer_threshold, pt_flow_ctrl->ui4_re_buferf_threshold))  // megaa 20100414
        {
            return RMR_DRV_ERROR;
        }

        if (gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState == BAGT_FLWCTL_PRE_LOAD_PENDING)
        {
            _buf_agt_PreLoad(ui2_buf_agt_id);
            gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_PRE_LOAD_STARTED;
#if 0  // test code
            while (gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState != BAGT_FLWCTL_PRE_LOAD_DONE)
            {
                x_thread_delay(10);
            }
#endif
        }
    }

    return RMR_OK;
}

#if defined(CC_53XX_SWDMX_V2)
INT32 buf_agt_set_feedersrc(BAGT_COMP_ID_T ui2_buf_agt_id, UINT32 u4SrcType, UINT8 u1SwdmxId)
{
    FeederSourceType eSourceType = (FeederSourceType)u4SrcType;
    FEEDER_QUERY_DATA rSetQuery;

    if(eSourceType>=FEEDER_SOURCE_INVALID /*|| 
        (!gat_bagt_data[ui2_buf_agt_id].bConnected)*/)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    ui2_bagt_id[eSourceType] = ui2_buf_agt_id;
    gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder = FALSE;
    gat_bagt_data[ui2_buf_agt_id].eDataType = eSourceType;
    gat_bagt_data[ui2_buf_agt_id].ui4_bagt_qid = 0;
	   
    if(_hBagtMsgQ[ui2_buf_agt_id]== (HANDLE_T)NULL)
    {
        if(ui2_buf_agt_id==BAGT_COMP_1)
        {
            VERIFY(x_msg_q_create(&_hBagtMsgQ[ui2_buf_agt_id], "Bagt_CMDQ1", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
        }
        else if (ui2_buf_agt_id == BAGT_COMP_2)
        {
            VERIFY(x_msg_q_create(&_hBagtMsgQ[ui2_buf_agt_id], "Bagt_CMDQ2", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
        }
        #ifdef CC_SKYPE_FINE_INSTANCE
        else if (ui2_buf_agt_id == BAGT_COMP_3)
        {
            VERIFY(x_msg_q_create(&_hBagtMsgQ[ui2_buf_agt_id], "Bagt_CMDQ3", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
        }
        else if (ui2_buf_agt_id == BAGT_COMP_4)
        {
            VERIFY(x_msg_q_create(&_hBagtMsgQ[ui2_buf_agt_id], "Bagt_CMDQ4", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
        }
        else if (ui2_buf_agt_id == BAGT_COMP_5)
        {
            VERIFY(x_msg_q_create(&_hBagtMsgQ[ui2_buf_agt_id], "Bagt_CMDQ5", sizeof(FEEDER_REQ_DATA_T), Bagt_Q_SIZE) == OSR_OK);
        }        
        #endif
    }
	   
    b_old_data_blk[ui2_buf_agt_id] = FALSE;
    FeederOpen(gat_bagt_data[ui2_buf_agt_id].eDataType);
    FeederSetInfo(eSourceType,FEEDER_MM_USE, TRUE);

    rSetQuery.eDataType  = eSourceType;
    rSetQuery.pfnSource  = _buf_agt_QueryFeeder;
    rSetQuery.pfnSeekPos = _buf_agt_Seek;
#ifdef BAGT_URI_MODE_1
    rSetQuery.pfnConsume = _buf_agt_Consume;
#else
    if (gat_bagt_data[ui2_buf_agt_id].fgPushMode)
    {
        rSetQuery.pfnConsume = _buf_agt_Push_Consume;
    }
    else
    {
        rSetQuery.pfnConsume = NULL;
    }
#endif
#ifdef BAGT_URI_MODE_2
    rSetQuery.pfnOverflow = _buf_agt_Overflow;
    rSetQuery.pfnUnderflow = _buf_agt_Underflow;
#else
    rSetQuery.pfnOverflow = NULL;
    rSetQuery.pfnUnderflow = NULL;
#endif
    rSetQuery.u4QuerySize= 0;
    rSetQuery.u1PlaymgrAttachedId = u1SwdmxId;

    FeederRegisterPullDataFunc(gat_bagt_data[ui2_buf_agt_id].eDataType, &rSetQuery);

    FeederSetFlush(gat_bagt_data[ui2_buf_agt_id].eDataType);
    if(gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(gat_bagt_data[ui2_buf_agt_id].pv_mw_tag, 
        					   BUF_AGT_NFY_COND_FLUSH_REQ, (UINT32)NULL, (UINT32)NULL);
    }
    FeederSetAppQueryMode(gat_bagt_data[ui2_buf_agt_id].eDataType, FEEDER_PULL_MODE);
    FeederStart(gat_bagt_data[ui2_buf_agt_id].eDataType);
    FeederSetRing(gat_bagt_data[ui2_buf_agt_id].eDataType, TRUE);
#ifdef BAGT_REORDER_ACK
    _buf_agt_acklist_free(&_rMwAckList[ui2_buf_agt_id]);
    _rMwAckList[ui2_buf_agt_id].u4AckNum=0;
	_rMwAckList[ui2_buf_agt_id].u4WaitAckId=u4IdCnt[ui2_buf_agt_id];
	_rMwAckList[ui2_buf_agt_id].fgWaitFirst=TRUE;
#endif

    return RMR_OK;
}
#endif

INT32 buf_agt_dmx_disconnect(BAGT_COMP_ID_T ui2_buf_agt_id)
{

   if(ui2_buf_agt_id>=BAGT_COMP_NS)
   { 
      return RMR_OK;
   }
   
#ifdef BAGT_URI_MODE_1
    _buf_agt_AllConsume(gat_bagt_data[ui2_buf_agt_id].eDataType);
    _au4ConsumeListRp[ui2_buf_agt_id] = _au4ConsumeListWp[ui2_buf_agt_id] = 0;
    _u4LastReqEndWPtr = _u4LastAckEndWPtr = 0;
#else
    if (b_old_data_blk[ui2_buf_agt_id])
    {
        buf_agt_mw_data_consumed(ui2_buf_agt_id, &t_old_data_blk[ui2_buf_agt_id]);
        b_old_data_blk[ui2_buf_agt_id] = FALSE;
    }
#endif
    gat_bagt_data[ui2_buf_agt_id].bNotUseFeeder = FALSE;
    gat_bagt_data[ui2_buf_agt_id].fgFlowCtrl     = FALSE;
    gat_bagt_data[ui2_buf_agt_id].eFlowCtrlState = BAGT_FLWCTL_NONE;
#ifdef FEEDER_MULTI_INSTANCE
    FeederSetFlush(gat_bagt_data[ui2_buf_agt_id].eDataType);
#else
    FeederSetFlush();
#endif    
    FeederStop(gat_bagt_data[ui2_buf_agt_id].eDataType);
    FeederClose(gat_bagt_data[ui2_buf_agt_id].eDataType);

#ifdef LINUX_TURNKEY_SOLUTION
    set_bagt_nfy(ui2_buf_agt_id);
#endif

#ifdef BAGT_REORDER_ACK
    _buf_agt_acklist_free(&_rMwAckList[ui2_buf_agt_id]);
    _rMwAckList[ui2_buf_agt_id].u4AckNum=0;
    _rMwAckList[ui2_buf_agt_id].u4WaitAckId=u4IdCnt[ui2_buf_agt_id];
	_rMwAckList[ui2_buf_agt_id].fgWaitFirst=TRUE;
#endif
    gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(gat_bagt_data[ui2_buf_agt_id].pv_mw_tag, 
                            BUF_AGT_NFY_COND_FLUSH_REQ, (UINT32)NULL, (UINT32)NULL);
    
    if (_hBagtMsgQ[ui2_buf_agt_id] != (HANDLE_T)NULL)
    {
        VERIFY(x_msg_q_delete(_hBagtMsgQ[ui2_buf_agt_id]) == OSR_OK);
    }
    _hBagtMsgQ[ui2_buf_agt_id]  = (HANDLE_T)NULL;

    if (_hBagtOldDataBlkQ[ui2_buf_agt_id] != (HANDLE_T)NULL)
    {
        VERIFY(x_msg_q_delete(_hBagtOldDataBlkQ[ui2_buf_agt_id]) == OSR_OK);
    }
    _hBagtOldDataBlkQ[ui2_buf_agt_id] = (HANDLE_T)NULL;

    fgUseLastData[ui2_buf_agt_id]=FALSE;

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_get_source_info
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pt_data_blk     References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_get_source_info( 
    BAGT_COMP_ID_T                    ui2_buf_agt_id,
    BUF_AGT_SRC_ALIGN_INFO_T* pt_data_blk)
{
    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check parameter */
    if (NULL == pt_data_blk)
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    *pt_data_blk = gat_bagt_data[ui2_buf_agt_id].t_align_info;

    return BAGTR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_data_req
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pt_data_blk     References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_data_req(
    BAGT_COMP_ID_T                  ui2_buf_agt_id,
    BUF_AGT_DATA_BLK_T*     pt_data_blk)
{
    BOOL                    b_ret;
#ifdef LINUX_TURNKEY_SOLUTION
    //BUF_AGT_DATA_BLK_T *prDataBlock;
#endif

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check parameter */
    if (NULL == pt_data_blk)
    {
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

#ifndef LINUX_TURNKEY_SOLUTION
    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    (UINT32)pt_data_blk,
                                    (UINT32)NULL);
#else
    set_mmap_bagt_nfy(ui2_buf_agt_id);
    b_ret = bagt_mmap_nfy_fct(gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_REQ,
                                    gat_bagt_data[ui2_buf_agt_id].pid,
                                    (UINT32)pt_data_blk,
                                    (UINT32)NULL);
#endif
    gat_bagt_data[ui2_buf_agt_id].u4ReqCount++; 
    
    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        //ASSERT(0);
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_flush
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_flush(
    BAGT_COMP_ID_T                    ui2_buf_agt_id)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    set_bagt_nfy(ui2_buf_agt_id);
#endif
    b_ret = gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_FLUSH_REQ,
                                    (UINT32)NULL,
                                    (UINT32)NULL);

    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_data_consumed
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *          pt_data_blk     References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_data_consumed(
    BAGT_COMP_ID_T                  ui2_buf_agt_id,
    BUF_AGT_DATA_BLK_T*     pt_data_blk)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        //ASSERT(0);
        return BAGTR_INV_ID;
    }

    /* check parameter */
    if (NULL == pt_data_blk)
    {
        //ASSERT(0);
        return BAGTR_INV_ARG;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        //ASSERT(0);
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        ASSERT(0);
        return BAGTR_NOT_REGISTERED;
    }
#ifdef LINUX_TURNKEY_SOLUTION
    set_bagt_nfy(ui2_buf_agt_id);
#endif

    b_ret =  gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_CONSUMED,
                                    (UINT32)pt_data_blk->ui4_req_id,
                                    (UINT32)NULL);

    if (b_ret)
    {
        gat_bagt_data[ui2_buf_agt_id].u4ConsumeCount++;
        return BAGTR_OK;
    }
    else
    {
        //ASSERT(0);
        LOG(5,"Bagt consumedata return fail\n");
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_data_overflow
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_data_overflow(
    BAGT_COMP_ID_T                  ui2_buf_agt_id)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    set_bagt_nfy(ui2_buf_agt_id);
#endif

    b_ret =  gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_OVERFLOW,
                                    0,
                                    (UINT32)NULL);

    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        return BAGTR_FAILED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: buf_agt_mw_data_underflow
 *
 * Description: <function description>
 *
 * Inputs:  ui2_buf_agt_id  References.
 *
 * Outputs: - 
 *
 * Returns: i4_ret              Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_mw_data_underflow(
    BAGT_COMP_ID_T                  ui2_buf_agt_id)
{
    BOOL                    b_ret;

    /* check parameters */
    if (ui2_buf_agt_id > (BAGT_COMP_NS - 1))
    {
        return BAGTR_INV_ID;
    }

    /* check if Buffer Agent is inited successfully */
    if (!gb_bagt_inited)
    {
        return BAGTR_NOT_INIT;
    }

    /* check if middleware notify function is registered */
    if (NULL == gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy)
    {
        return BAGTR_NOT_REGISTERED;
    }

#ifdef LINUX_TURNKEY_SOLUTION
    set_bagt_nfy(ui2_buf_agt_id);
#endif

    b_ret =  gat_bagt_data[ui2_buf_agt_id].pf_mw_nfy(
                                    gat_bagt_data[ui2_buf_agt_id].pv_mw_tag,
                                    BUF_AGT_NFY_COND_DATA_UNDERFLOW,
                                    0,
                                    (UINT32)NULL);

    if (b_ret)
    {
        return BAGTR_OK;
    }
    else
    {
        return BAGTR_FAILED;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _buf_agt_connect
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_set_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 _buf_agt_connect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_CONN_TYPE_T  e_conn_type,
    const VOID*      pv_conn_info,
    SIZE_T           z_conn_info_len,
    VOID*            pv_tag,
    x_rm_nfy_fct     pf_nfy)
{
    /* Check component type and id */

    if (NULL == pt_comp_id)
    {
        return RMR_DRV_INV_CONN_INFO;
    }    

    if (DRVT_BUF_AGENT != pt_comp_id->e_type)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1))
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_disconn
 *
 * Description: <function description>
 *
 * Inputs:  pt_comp_id      References.
 *          e_set_type      References.
 *          pv_data         References.
 *          pz_size         References.
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 _buf_agt_disconn(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_DISC_TYPE_T  e_disc_type,
    const VOID*      pv_disc_info,
    SIZE_T           z_disc_info_len)
{
    //printk("Disconn BAGT #%d: ack cnt = %d, consume cnt = %d, req cnt = %d\n", pt_comp_id->ui2_id, gat_bagt_data[pt_comp_id->ui2_id].u4AckCount, gat_bagt_data[pt_comp_id->ui2_id].u4ConsumeCount, gat_bagt_data[pt_comp_id->ui2_id].u4ReqCount);
	
    /* Check component type and id */
    if (NULL == pt_comp_id)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (DRVT_BUF_AGENT != pt_comp_id->e_type)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->ui2_id > (BAGT_COMP_NS - 1))
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->b_sel_out_port != FALSE)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _buf_agt_set
 *
 * Description: Initialize middleware buffer agent driver
 *
 * Inputs:  -
 *
 * Outputs: - 
 *
 * Returns: RMR_OK  Success.
 *
 ----------------------------------------------------------------------------*/
INT32 buf_agt_init(void)
{
    INT32 i4_ret;

    /* Component ID */
    DRV_COMP_REG_T t_comp_reg;

    /* Function table */
    DRV_COMP_FCT_TBL_T t_comp_fct;

    /* Setup component ID */
    t_comp_reg.e_type = DRVT_BUF_AGENT;
    t_comp_reg.e_id_type = ID_TYPE_RANGE;
    t_comp_reg.u.t_range.ui2_first_id = BAGT_COMP_1;
    t_comp_reg.u.t_range.ui2_delta_id = 1;
#if 1  //def FEEDER_MULTI_INSTANCE  megaa 20100421
    t_comp_reg.u.t_range.ui2_num_of_ids = BAGT_COMP_NS;
#else
    t_comp_reg.u.t_range.ui2_num_of_ids = BAGT_COMP_NS - 1;
#endif    
    t_comp_reg.u.t_range.pv_first_tag = NULL;
    t_comp_reg.u.t_range.pv_delta_tag = NULL;
    t_comp_reg.u.t_range.ui1_port = 1;

    /* Setup component function table */
    t_comp_fct.pf_rm_connect = _buf_agt_connect;
    t_comp_fct.pf_rm_disconnect = _buf_agt_disconn;
    t_comp_fct.pf_rm_get = _buf_agt_get;
    t_comp_fct.pf_rm_set = _buf_agt_set;

    /* init cli */
    i4_ret = _buf_agt_cli_init();
    if (BAGTR_OK != i4_ret)
    {
        return RMR_DRV_ERROR;
    }

    /* init buffer agent here */    
    if (!_buf_agt_drv_init())   /* Init failed. */
    {
        return RMR_DRV_ERROR;
    }

    /* Register component to Resource Manager database */
    i4_ret = x_rm_reg_comp(&t_comp_reg, 1, 1, "", 0, &t_comp_fct, NULL, 0);

    return i4_ret;
}

#ifdef LINUX_TURNKEY_SOLUTION
BOOL buf_agt_req_ack_fct(
    UINT16                      ui2_buf_agt_id,
    BUF_AGT_DATA_REQ_ACK_COND_T e_ack_cond,
    BUF_AGT_DATA_BLK_T*         pt_data_blk)
{
    return  _buf_agt_req_ack_fct(ui2_buf_agt_id, e_ack_cond,pt_data_blk);
}
#endif

void buf_agt_query(UINT32 u4ID)
{
    LOG(0, "BAGT #%d: ack cnt = %d, consume cnt = %d, req cnt = %d\n", u4ID, gat_bagt_data[u4ID].u4AckCount, gat_bagt_data[u4ID].u4ConsumeCount, gat_bagt_data[u4ID].u4ReqCount);
}



