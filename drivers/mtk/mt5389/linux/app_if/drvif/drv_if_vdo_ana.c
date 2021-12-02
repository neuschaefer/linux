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
 * $RCSfile: drv_if_vdo_ana.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdo_ana.c
 *  Driver interface: analog VDO part implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_printf.h"
#include "vdp_drvif.h"
#include "nptv_if.h"
#include "video_timing.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
#include "vdo_ana.h"
#include "vdo_common.h"

#include "drv_if_config.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DRV_VDO_VBITTX_FIFO_SIZE 32
#define DRV_VDO_VBITTX_BUF_NUM_PACKET     512
#define DRV_VDO_VBITTX_BUF_NUM_NODE    (DRV_VDO_VBITTX_BUF_NUM_PACKET / 17)
#define DRV_VDO_VBITTX_BUF_NODE_MAGIC          ((UINT32) 0xF50AF50A)

#define DLIST_INIT(q)                       \
    do                                      \
    {                                       \
        (q)->pt_head = (q)->pt_tail = NULL; \
    } while(0)

#define DLIST_T(type)           \
    struct {                    \
        struct type *pt_head;   \
        struct type *pt_tail;   \
    }
    
#define DLIST_ENTRY_T(type)     \
    struct {                    \
        struct type *pt_prev;   \
        struct type *pt_next;   \
    }
    
#define DLIST_HEAD(q) ((q)->pt_head)

#define DLIST_TAIL(q) ((q)->pt_tail)

#define DLIST_IS_EMPTY(q) ((q)->pt_head == NULL)

#define DLIST_NEXT(ent, field)  ((ent)->field.pt_next)

#define DLIST_PREV(ent, field)  ((ent)->field.pt_prev)

#define DLIST_INSERT_HEAD(ent, q, field)    \
    do                                      \
    {                                       \
        (ent)->field.pt_prev = NULL;                        \
        if (((ent)->field.pt_next = (q)->pt_head) == NULL)  \
        {                                                   \
            (q)->pt_tail = (ent);                           \
        }                                                   \
        else                                                \
        {                                                   \
            ((q)->pt_head)->field.pt_prev = (ent);          \
        }                                                   \
        (q)->pt_head = (ent);                               \
    } while(0)
    
#define DLIST_INSERT_TAIL(ent, q, field)                    \
    do                                                      \
    {                                                       \
        (ent)->field.pt_next = NULL;                        \
        if (((ent)->field.pt_prev = (q)->pt_tail) == NULL)  \
        {                                                   \
            (q)->pt_head = (ent);                           \
        }                                                   \
        else                                                \
        {                                                   \
            ((q)->pt_tail)->field.pt_next = (ent);          \
        }                                                   \
        (q)->pt_tail = (ent);                               \
    } while(0)

#define DLIST_REMOVE(ent, q, field)                         \
    do                                                      \
    {                                                       \
        if ((q)->pt_tail == (ent))                          \
        {                                                   \
            (q)->pt_tail = (ent)->field.pt_prev;            \
        }                                                   \
        else                                                \
        {                                                   \
            ((ent)->field.pt_next)->field.pt_prev = (ent)->field.pt_prev;   \
        }                                                   \
        if ((q)->pt_head == (ent))                          \
        {                                                   \
            (q)->pt_head = (ent)->field.pt_next;            \
        }                                                   \
        else                                                \
        {                                                   \
            ((ent)->field.pt_prev)->field.pt_next = (ent)->field.pt_next;   \
        }                                                   \
        (ent)->field.pt_next = NULL;                        \
        (ent)->field.pt_prev = NULL;                        \
    } while(0)

#define GET_NODE_SIZE( _u2NumPkt )              \
       ((((UINT32) sizeof(DRV_VDO_VBITTX_BUF_NODE_T)) +      \
        (((UINT32) sizeof(DRV_VDO_VBITTX_PACKET_T)) * ((UINT32) ((_u2NumPkt) - 1))) + 3 ) & (~((UINT32) 0x03)))

        
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
	
typedef struct 
{
    UINT16 u2Size;
    void* pvBuf;
} DRV_VDO_VBITTX_FIFO_T;

typedef DLIST_T(_DRV_VDO_VBITTX_BUF_NODE_T)          DLIST_TB_ND;

typedef DLIST_ENTRY_T(_DRV_VDO_VBITTX_BUF_NODE_T)    DLIST_ENTRY_TB_ND;


typedef struct _DRV_VDO_VBITTX_BUF_T
{
    /* constants */
    BOOL                bInited;
    BYTE*               pbBuf;
    UINT32              u4SzBuf;
    
    /* variables */
    UINT32              u4Wtr;
    DLIST_TB_ND         tLstNodes;
    
} DRV_VDO_VBITTX_BUF_T;

typedef struct _DRV_VDO_VBITTX_BUF_NODE_T
{
    UINT32                  u4Magic;
    UINT16                  u2NumPkt;
    DLIST_ENTRY_TB_ND       t_link;
    
    DRV_VDO_VBITTX_PACKET_T        atPkts[1];
    
    /* more packet data starts here */
    
} DRV_VDO_VBITTX_BUF_NODE_T;


#ifndef USE_5371
typedef void (*NPTV_PFN_SIG_NFY_FCT)(
    void*               pvNfyTag,
    UINT32     		u4SigStatus,
    UINT32              u4SigSrc
);

typedef void (*NPTV_PFN_SCART_NFY)(
    UINT8 u1Source, 
    UINT8 u1Status
);

typedef void (*NPTV_PFN_VGA_AUTO_NFY)(
    UINT16 ui2_id, 
    BOOL fgSuccess
);

typedef void (*NPTV_PFN_VGA_AUTOCOLOR_NFY)(
    UINT16 ui2_id, 
    BOOL fgSuccess
);

typedef void (*NPTV_PFN_TTX_NFY)(
    UINT16 u2NumPktAvail, 
    UINT32 _dVbiReadPtr    
);

typedef void (*NPTV_PFN_CC_NFY)(
    UINT8 u1Path, 
    UINT8 d1, 
    UINT8 d2, 
    UINT8 field
);

typedef void (*NPTV_PFN_WSS_NFY)(
    UINT8 u1Path,
    UINT16 u2Data
);

typedef void (*NPTV_PFN_VPS_NFY)(
    UINT16 u2Data
);
#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static UINT32                   _u4TtxFifoRp = 0;
//static UINT32                   _u4TtxFifoWp = 0;
//static DRV_VDO_VBITTX_DATA_T  _arTtxFifo[DRV_VDO_VBITTX_FIFO_SIZE + 1];

static UINT16                   _u2VpsCniLastRcv;
static UINT16                   _u2VpsCniCurr;

static DRV_VDO_SCART_STATUS_T   _rScartStatus;
//static BOOL                     _fgTtxOverflow = FALSE;

static DRV_VDO_VBITTX_BUF_T _rTtxBuf;
static BOOL                        _fgTtxBufErrPaused;
//static UINT32 _u4TtxBufSerialNumber = 0;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


extern void HalFlushInvalidateDCache(void);

extern UINT16 wDrvVideoInputValidDataWidth(UINT8 bPath);

extern UINT16 wDrvVideoInputHeight(UINT8 bPath);

extern UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath);   

extern UINT8 fgApiVideoIsVgaMode(UINT8 bPath, UINT8 bChkFlg);

extern void vDrvAvByp(void); 

extern void vDrvSVdoByp(void);
 
extern void vDrvBypPD(void); 

extern UINT8 fgApiVideoVgaSetClock(UINT8 bPath, UINT16 wValue); 

extern UINT8 fgApiVideoVgaSetPhase(UINT8 bPath, UINT8 bValue);

extern UINT8 fgApiVideoVgaAuto(void);

extern UINT8 fgApiVideoYPbPrAuto(void);

extern INT32 NPTV_SetSigChgNfyFct(const NPTV_PFN_SIG_NFY_FCT pfnFct);

extern INT32 NPTV_SetScartNfy(const NPTV_PFN_SCART_NFY pfnFct);
	
extern INT32 NPTV_SetVgaAutoNfy(const NPTV_PFN_VGA_AUTO_NFY pfnFct);

extern INT32 NPTV_SetVgaAutocolorNfy(const NPTV_PFN_VGA_AUTOCOLOR_NFY pfnFct);

extern void vScartUIChangeInputMode(UINT8 bScartMode);

#ifdef CC_VBI_SUPPORT

extern void vDrvVbiStartNotifyOnOff(BYTE bPath, BYTE bOnOff);

extern void vDrvTTXStartNofifyOnOff(BYTE  bOnOff);

extern void vDrvVPSCNIStartNotifyOnOff(BYTE  bOnOff);

extern void vDrvEUWSSStartNOtifyOnOff(BYTE bPath, BYTE  bOnOff);

extern INT32 NPTV_VBI_SetTtxNfy(const NPTV_PFN_TTX_NFY pfnFct);

extern INT32 NPTV_VBI_SetCcNfy(const NPTV_PFN_CC_NFY pfnFct);

extern INT32 NPTV_VBI_SetVpsNfy(const NPTV_PFN_VPS_NFY pfnFct);

extern INT32 NPTV_VBI_SetWssNfy(const NPTV_PFN_WSS_NFY pfnFct);
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _VbiTtxBufSysInit(void)
{
    DRV_VDO_VBITTX_BUF_T* ptCtx = &_rTtxBuf;
    x_memset( ptCtx, 0, sizeof( DRV_VDO_VBITTX_BUF_T ) );
    
    ptCtx->bInited = FALSE;
    ptCtx->pbBuf   = NULL;
    ptCtx->u4SzBuf = 0;
    ptCtx->u4Wtr   = 0;
    DLIST_INIT( & ptCtx->tLstNodes );


} 

static DRV_ERR_CODE_T _VbiTtxBufInit(void)
{
    /* context: must be called from a thread context and no multithreads enter concurrently */
    DRV_VDO_VBITTX_BUF_T* ptCtx = &_rTtxBuf;
    if ( ptCtx->bInited )
    {
        return DRV_E_OK;
    }
    
    ptCtx->u4SzBuf =  ( ( (UINT32) ( sizeof(DRV_VDO_VBITTX_PACKET_T ) * DRV_VDO_VBITTX_BUF_NUM_PACKET + 
                                     sizeof( DRV_VDO_VBITTX_BUF_NODE_T ) * DRV_VDO_VBITTX_BUF_NUM_NODE + 3 ) )
                            & ( ~((UINT32) 0x03) ) );
    
    ptCtx->pbBuf = (BYTE *) x_mem_alloc( ptCtx->u4SzBuf );
    
    if ( ptCtx->pbBuf == NULL )
    {
        return DRV_E_OUT_OF_MEMORY;
    }
    
    ptCtx->u4Wtr   = 0;
    DLIST_INIT( & ptCtx->tLstNodes );
    ptCtx->bInited = TRUE;
    
    return DRV_E_OK;
}

static BOOL _VbiTtxBufSrchNode(
    const DLIST_TB_ND*          ptLstNodes,
    DRV_VDO_VBITTX_BUF_NODE_T* prBuf)
{
    DRV_VDO_VBITTX_BUF_NODE_T               *ptNode;
    
    ptNode  = DLIST_HEAD( ptLstNodes );
    while ( ptNode )
    {
        if ( ptNode->u4Magic  == DRV_VDO_VBITTX_BUF_NODE_MAGIC &&
        	ptNode == prBuf)
        {
            return TRUE;
        }
             
        ptNode = DLIST_NEXT( ptNode, t_link );
    } 
   
    return FALSE;
}    

static void _VbiTtxBufFreeNodes(
    DLIST_TB_ND*                ptLstNodes,
    const DRV_VDO_VBITTX_BUF_NODE_T*     prBuf)
{
    //DRV_VDO_VBITTX_PACKET_T            *patPktsNode;
    DRV_VDO_VBITTX_BUF_NODE_T                *ptNode;
    
    do 
    {
        ptNode      = DLIST_HEAD( ptLstNodes );
        //patPktsNode = ptNode->atPkts;
        DLIST_REMOVE( ptNode, ptLstNodes, t_link );
        
    } while ( (ptNode != prBuf) && (! DLIST_IS_EMPTY(ptLstNodes)) );
    
}

static DRV_ERR_CODE_T _VbiTtxBufFree(UINT32 u4Tag)
{
    DRV_VDO_VBITTX_BUF_T*            ptCtx = &_rTtxBuf; 
    CRIT_STATE_T    tCS;
    DRV_ERR_CODE_T           rRetCode;
    DRV_VDO_VBITTX_BUF_NODE_T* prBuf = (DRV_VDO_VBITTX_BUF_NODE_T*)((u4Tag >> 8) + (UINT32)(ptCtx->pbBuf));
    
    rRetCode = DRV_E_OK;
    
    tCS = x_crit_start();
    
    if ( DLIST_IS_EMPTY ( & ptCtx->tLstNodes ) )
    {
        rRetCode = DRV_E_BUFFER_UNDERFLOW;
        goto _ERR_TvdTtxBufFree_1;
    }
    
    /* search for the corresponding node */
    if ( _VbiTtxBufSrchNode( & ptCtx->tLstNodes, prBuf ) )
    {
        _VbiTtxBufFreeNodes( & ptCtx->tLstNodes, prBuf );
    }
    else
    {
        rRetCode = DRV_E_FAIL;
        goto _ERR_TvdTtxBufFree_1;
    }
    
#ifdef _DBG_CHECK_CONSISTENCY_
    __TtxBufDebugChkConsistency( ptCtx );
#endif
    
_ERR_TvdTtxBufFree_1:

    x_crit_end( tCS );
        
        
    return rRetCode;
}

static void _VbiTtxBufFlush(void)
{
    DRV_VDO_VBITTX_BUF_T*            ptCtx;
    CRIT_STATE_T    tCS;

    
    tCS = x_crit_start();
    ptCtx = &_rTtxBuf;
    DLIST_INIT( & ptCtx->tLstNodes );
    ptCtx->u4Wtr = 0;
    
    x_crit_end( tCS );
}    

static INT32 _VbiTtxBufAlloc( 
    UINT16                      u2NumPkt,
    DRV_VDO_VBITTX_BUF_NODE_T** pprBuf,
    DRV_VDO_VBITTX_PACKET_T**          ppatPkts)
{
    DRV_VDO_VBITTX_BUF_T*            ptCtx;
    UINT32          u4SzData;
    DRV_VDO_VBITTX_BUF_NODE_T    *ptNodeNew = NULL;
    CRIT_STATE_T    tCS;
    INT32           i4Ret;
    
    *ppatPkts = NULL;
    *pprBuf = NULL;
    
    if ( u2NumPkt == 0 )
    {
        return DRV_E_INV_ARG;
    }
    
    ptCtx = &_rTtxBuf;
    
    u4SzData = GET_NODE_SIZE( u2NumPkt );

    i4Ret = DRV_E_OK;
    tCS = x_crit_start();
    
    if ( DLIST_IS_EMPTY( & ptCtx->tLstNodes ) )
    {
        if ( u4SzData <= ptCtx->u4SzBuf )
        {
            ptNodeNew    = (DRV_VDO_VBITTX_BUF_NODE_T *) (ptCtx->pbBuf);            
            
            ptCtx->u4Wtr = u4SzData;
        }
        else
        {
            i4Ret = DRV_E_BUFFER_OVERFLOW;
            goto _ERR_TvdTtxBufAlloc_1;
        }
    }
    else
    {
        UINT32    u4Rdr;
        
        u4Rdr = ((UINT32) DLIST_HEAD( & ptCtx->tLstNodes )) - ((UINT32) ptCtx->pbBuf);
        
        if (ptCtx->u4Wtr > u4Rdr)
        {
            if ( ptCtx->u4Wtr + u4SzData <= ptCtx->u4SzBuf )
            {
                ptNodeNew     = (DRV_VDO_VBITTX_BUF_NODE_T *) (ptCtx->pbBuf + ptCtx->u4Wtr);
                
                ptCtx->u4Wtr += u4SzData;
            }
            else
            {
                if ( u4SzData <= u4Rdr )
                {
                    ptNodeNew    = (DRV_VDO_VBITTX_BUF_NODE_T *) (ptCtx->pbBuf);              
                    
                    ptCtx->u4Wtr = u4SzData;
                }
                else
                {
                    i4Ret = DRV_E_BUFFER_OVERFLOW;
                    goto _ERR_TvdTtxBufAlloc_1;
                }
            }
        }
        else
        {
            if (ptCtx->u4Wtr + u4SzData <= u4Rdr)
            {
                ptNodeNew     = (DRV_VDO_VBITTX_BUF_NODE_T *) (ptCtx->pbBuf + ptCtx->u4Wtr);
                
                ptCtx->u4Wtr += u4SzData;
            }
            else
            {
                i4Ret = DRV_E_BUFFER_OVERFLOW;
                goto _ERR_TvdTtxBufAlloc_1;
            }
        }
    }
    
    DLIST_INSERT_TAIL( ptNodeNew, & ptCtx->tLstNodes, t_link );


_ERR_TvdTtxBufAlloc_1:

    x_crit_end( tCS );
    
    if ( i4Ret == DRV_E_OK )
    {
        /* init ptNodeNew */
        ptNodeNew->u4Magic  = DRV_VDO_VBITTX_BUF_NODE_MAGIC;
        ptNodeNew->u2NumPkt = u2NumPkt;
        /*
        #ifdef _DBG_CHECK_CONSISTENCY_
            tCS = x_crit_start();
            __TtxBufDebugChkConsistency( ptCtx );
            x_crit_end( tCS );
        #endif
        */
        *ppatPkts           = ptNodeNew->atPkts;
        *pprBuf = ptNodeNew;        
    }
    
    return i4Ret;
}

static void _VbiTtxNfy(UINT16 u2NumPktAvail, 
    UINT32 _dVbiReadPtr)
{
    BYTE *pbRdr = (BYTE *) _dVbiReadPtr ;
    DRV_VDO_PARAM_T* prParam;    
    PFN_DRV_CALLBACK prCallback;   
    //UINT32 i;
    DRV_VDO_VBITTX_PACKET_T    *pat_pkts;
    INT32               i4Ret;
    CRIT_STATE_T        tCS;
    BOOL                fgBufErrPaused;
    DRV_VDO_VBITTX_BUF_NODE_T    *prBuf;
    
    if (u2NumPktAvail == 0)
    {
        return;
    }

    VERIFY(_DRV_VDO_GetParameter(0, &prParam) == DRV_E_OK);
    prCallback = prParam->rCallbackList.pfnVbiTtxData;
    
    tCS = x_crit_start();
    fgBufErrPaused = _fgTtxBufErrPaused;
    x_crit_end(tCS);
    
    if ( fgBufErrPaused || u2NumPktAvail == 0 )
    {
        return;
    }

    i4Ret = _VbiTtxBufAlloc( 
        u2NumPktAvail,
        & prBuf,
        & pat_pkts );
    
    if ( i4Ret == DRV_E_OK )
    {
        if ( prCallback != NULL) 
        {

            /* copy data */
            UINT16              u2i;
            //DRV_VDO_VBITTX_DATA_T   t_ttx_data;
            DRV_VDO_VBITTX_PACKET_T    *pt_pkt;
            
            pt_pkt = pat_pkts;
            HalFlushInvalidateDCache(); /* To make sure that Cache and DRAM are consistent */
            for ( u2i = 0; u2i < u2NumPktAvail; u2i ++ )
            {
                /* both pat_pkts[u2i].aui4_data and pbRdr should be 4-bytes alignment to make copy faster */
                x_memcpy( (VOID *) pt_pkt->au1Buf, (VOID *) pbRdr, 42);
                pt_pkt ++ ;
                pbRdr  += 48 ;
            }
            
            /* prepare notify data */
            //t_ttx_data.u2NumPacket= u2NumPktAvail;
            //t_ttx_data.prPacket = pat_pkts;


            /* notify data arrival */
            prCallback(DRV_VDO_VBITTX_DATA_ARRIVAL | ( ((UINT32)prBuf - (UINT32)_rTtxBuf.pbBuf) << 8));
            /*
            _TVTTXNfyInfoMain.pf_ttx_nfy(
                _TVTTXNfyInfoMain.pv_tag,
                TV_DEC_TTX_COND_DATA_ARRIVAL,
                (VOID *) & t_ttx_data );
            */
        }
    }
    else
    {
        if ( i4Ret == DRV_E_BUFFER_OVERFLOW )
        {
            /* notify data overflow */

            if(prCallback != NULL)
            {
                prCallback(DRV_VDO_VBITTX_OVERFLOW);            
    
                tCS = x_crit_start();
                _fgTtxBufErrPaused = TRUE;
                x_crit_end(tCS);
            }
        }
        else 
        {
            /* notify buffer internal error */
            
            if(prCallback != NULL)
            {
                prCallback(DRV_VDO_VBITTX_ERROR);            
    
                tCS = x_crit_start();
                _fgTtxBufErrPaused = TRUE;
                x_crit_end(tCS);
            }
        }
    }
             
}    


static void _VbiCcNfy(UINT8 u1Path, 
    UINT8 u1Data1, 
    UINT8 u1Data2, 
    UINT8 u1EvenField)
{

    DRV_VDO_PARAM_T* prParam;
    PFN_DRV_CALLBACK pfnCallback;
    
    VERIFY(_DRV_VDO_GetParameter(u1Path, &prParam) == DRV_E_OK);    
    pfnCallback = prParam->rCallbackList.pfnVbiCcData;

    if(pfnCallback != NULL)
    {
        pfnCallback(u1Path | (u1Data1 << 8) | (u1Data2 << 16) | (u1EvenField << 24));
    }
}

static void _VbiVpsNfy(UINT16 u2Data)
{
    /* error handling policy : */
    /* got valid cni code when receiving the same value 2 times */

    DRV_VDO_PARAM_T* prParam;
    PFN_DRV_CALLBACK pfnCallback;
    
    VERIFY(_DRV_VDO_GetParameter(0, &prParam) == DRV_E_OK);    
    pfnCallback = prParam->rCallbackList.pfnVbiVpsData;
    
    if ( u2Data == _u2VpsCniLastRcv )
    {
        /* u2Data is valid */
        if ( u2Data != _u2VpsCniCurr )
        {
        
            if (pfnCallback != NULL)
            {
            
                pfnCallback((UINT32)u2Data);
            }
        
            _u2VpsCniCurr = u2Data;
        }
    }
    
    _u2VpsCniLastRcv = u2Data;

}

static void _VbiWssNfy(UINT8 u1Path, UINT16 u2Data)
{
    UINT32 u4Tag;
    DRV_VDO_PARAM_T* prParam;
    PFN_DRV_CALLBACK pfnCallback;
    
    VERIFY(_DRV_VDO_GetParameter(u1Path, &prParam) == DRV_E_OK);    
    pfnCallback = prParam->rCallbackList.pfnVbiWssData;
    
    u4Tag = u1Path | (u2Data << 8);
    
    if(pfnCallback != NULL)
    {
        pfnCallback(u4Tag);
    }
}

static void _ScartNfy(UINT8 u1Source, UINT8 u1Status)
{
    UINT32 u4Tag;
    DRV_VDO_PARAM_T* prParam;
    PFN_DRV_CALLBACK pfnCallback;
    
    VERIFY(_DRV_VDO_GetParameter(0, &prParam) == DRV_E_OK);    
    
    pfnCallback = prParam->rCallbackList.pfnScartStatus;    
    if(pfnCallback != NULL /*&& prParam->rInputSrc == u1Source*/)
    {
        if(u1Status == SV_WS_4W_3H)
        {
            _rScartStatus.rFs = DRV_VDO_SCART_FS_ASPECT_4_3;
        }
        else if(u1Status == SV_WS_16W_9H)
        {
            _rScartStatus.rFs = DRV_VDO_SCART_FS_ASPECT_16_9;        
        }
        else
        {
            _rScartStatus.rFs = DRV_VDO_SCART_FS_NO_SIGNAL;        
        }
        pfnCallback((UINT32)_rScartStatus.rFs << 8 | u1Source);
    }
}

static void _VgaAutoNfy(UINT16 u2Idx, BOOL fgSuccess)
{
    DRV_VDO_PARAM_T* prParam;
    PFN_DRV_CALLBACK pfnCallback;

    VERIFY(_DRV_VDO_GetParameter(0, &prParam) == DRV_E_OK);  
    pfnCallback = prParam->rCallbackList.pfnVgaAuto;   
    
    if(pfnCallback != NULL)
    {
        pfnCallback(u2Idx);
    }
}

static void _VgaAutocolorNfy(UINT16 u2Idx, BOOL fgSuccess)
{
    DRV_VDO_PARAM_T* prParam;
    PFN_DRV_CALLBACK pfnCallback;

    VERIFY(_DRV_VDO_GetParameter(0, &prParam) == DRV_E_OK);  
    pfnCallback = prParam->rCallbackList.pfnVgaAutocolor;   
    
    if(pfnCallback != NULL)
    {
        pfnCallback(u2Idx);
    }
}
static void _ResChgNfy(void*               pvNfyTag,
    UINT32     		u4SigStatus,
    UINT32              u4SigSrc)
{
    UINT8 u1VideoIndex = (UINT8)((UINT32)pvNfyTag & 0xFF);
    UINT32 u4Tag;
    PFN_DRV_CALLBACK pfnCallback;
    //BOOL fgIsVgaTiming;
    DRV_VDO_PARAM_T *prParam;
    DRV_VDO_INTERNAL_T *prInternal;
    
    VERIFY(_DRV_VDO_GetParameter(u1VideoIndex, &prParam) == DRV_E_OK);  
    VERIFY(_DRV_VDO_GetInternal(u1VideoIndex, &prInternal) == DRV_E_OK);

    prInternal->rInfo.u2Width       = wApiVideoGetInputWidth(u1VideoIndex);    
    prInternal->rInfo.u2Height      = wDrvVideoInputHeight(u1VideoIndex);
    prInternal->rInfo.fgProgressive = !(BOOL)bDrvVideoIsSrcInterlace(u1VideoIndex);
    prInternal->rInfo.fgVgaTiming   = fgApiVideoIsVgaMode(u1VideoIndex, 0);
   
    Printf("[VDO] Signal change notify\n");
    
    if(u4SigStatus == SV_VDO_STABLE)
    {
        prInternal->rInfo.rSigStatus = DRV_VDO_SIGNAL_DETECTED;
    }
    else
    {
        prInternal->rInfo.rSigStatus = DRV_VDO_SIGNAL_NOT_DETECTED;        
    }    
     
    pfnCallback = prParam->rCallbackList.pfnResChange;
    
    if(pfnCallback != NULL)
    {
        u4Tag = (UINT32)prInternal->rInfo.rSigStatus;
        pfnCallback(u4Tag);
    }
}

static DRV_ERR_CODE_T _EnableVBI(UINT8 u1VideoIndex)
{
#ifdef CC_VBI_SUPPORT
    DRV_VDO_PARAM_T* prParam;
    
    VERIFY(_DRV_VDO_GetParameter(u1VideoIndex, &prParam) == DRV_E_OK);    
    if(u1VideoIndex == 0 && prParam->rCallbackList.pfnVbiTtxData != NULL)
    {
        VERIFY(_VbiTtxBufInit() == DRV_E_OK);

        _VbiTtxBufFlush();

         _fgTtxBufErrPaused          = FALSE;
         
        vDrvTTXStartNofifyOnOff(1);
    }
    if(prParam->rCallbackList.pfnVbiCcData != NULL)
    {
        vDrvVbiStartNotifyOnOff(u1VideoIndex, 1);
    }
    if(u1VideoIndex == 0 &&  prParam->rCallbackList.pfnVbiVpsData != NULL)
    {
        _u2VpsCniCurr = 0;
        _u2VpsCniLastRcv = 0;        
        vDrvVPSCNIStartNotifyOnOff(1);    
    }
    if(prParam->rCallbackList.pfnVbiWssData != NULL)
    {
        vDrvEUWSSStartNOtifyOnOff(u1VideoIndex, 1);
    }
#endif           
    return DRV_E_OK;
}

static DRV_ERR_CODE_T _DisableVBI(UINT8 u1VideoIndex)
{
#ifdef CC_VBI_SUPPORT
    if(u1VideoIndex == 0)
    {
        vDrvTTXStartNofifyOnOff(0);
        vDrvVbiStartNotifyOnOff(0, 0);
        vDrvVPSCNIStartNotifyOnOff(0);    
        vDrvEUWSSStartNOtifyOnOff(0, 0);
    }
    else
    {
        vDrvVbiStartNotifyOnOff(1, 0);    
        vDrvEUWSSStartNOtifyOnOff(1, 0);            
    }
#endif       
    return DRV_E_OK;
}

static void _InitVBI(void)
{
#ifdef CC_VBI_SUPPORT
   _VbiTtxBufSysInit();

    _DisableVBI(0);
    _DisableVBI(1);    
    
    NPTV_VBI_SetTtxNfy(_VbiTtxNfy);
    NPTV_VBI_SetCcNfy(_VbiCcNfy);
    NPTV_VBI_SetVpsNfy(_VbiVpsNfy);        
    NPTV_VBI_SetWssNfy(_VbiWssNfy);   
#endif    
}

static void _InitScart(void)
{
    _rScartStatus.rBypass   = DRV_VDO_SCART_BYPASS_FALSE;
    _rScartStatus.rOutput   = DRV_VDO_SCART_OUTPUT_OFF;    
    _rScartStatus.rFs     = DRV_VDO_SCART_FS_NO_SIGNAL;
    _rScartStatus.rFb    = DRV_VDO_SCART_FS_UNKNOWN;   
    NPTV_SetScartNfy(_ScartNfy);    
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DRV_VDO_SetScart
 *  Set SCART operation mode
 * 
 *  @param  u4Mask                  Mask to specify the items to be set
 *  @param  prVbiTtxData		    Pointer to DRV_VDO_SCART_PARAM_T
 *
 *  @retval DRV_E_OK	            Succeed
 *  @retval DRV_E_FAIL	            Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_SetScart(UINT32 u4Mask, DRV_VDO_SCART_PARAM_T* prParam)
{   
#ifdef CC_SCART_SUPPORT
    DRV_ERR_CODE_T rRetCode = DRV_E_OK;
    if(u4Mask & DRV_VDO_SCARTMSK_BYPASS)
    {
        if (prParam->rBypass == DRV_VDO_SCART_BYPASS_TRUE)
        {                   
            bApiVideoSetVideoSrc(0, SV_VS_TUNER1);                    
        }                   
        else if(prParam->rBypass == DRV_VDO_SCART_BYPASS_FALSE)
        {               
            bApiVideoSetVideoSrc(0, SV_VS_SCART1);
        }            
        else
        {
            rRetCode = DRV_E_FAIL;
        }
        _rScartStatus.rBypass = prParam->rBypass;        
    }
    if(u4Mask & DRV_VDO_SCARTMSK_OUTPUT)
    {
        if (prParam->rOutput == DRV_VDO_SCART_OUTPUT_COMPOSITE)
        {                   
            vDrvAvByp();
        }               
        else if(prParam->rOutput == DRV_VDO_SCART_OUTPUT_S_VIDEO)
        {
            vDrvSVdoByp();
        }
        else if(prParam->rOutput == DRV_VDO_SCART_OUTPUT_OFF)
        {
            vDrvBypPD();
        }
        else
        {
            rRetCode = DRV_E_FAIL;
        }
        _rScartStatus.rOutput= prParam->rOutput;                
    }
    if(u4Mask & DRV_VDO_SCARTMSK_MODE)
    {
        vScartUIChangeInputMode(prParam->rMode);
    }
    return rRetCode;
#endif        
}

DRV_ERR_CODE_T _DRV_VDO_SetVga(UINT32 u4Mask, DRV_VDO_VGA_PARAM_T* prParam)
{
    DRV_ERR_CODE_T rRetCode = DRV_E_OK;
    if(u4Mask & DRV_VDO_VGAMSK_CLOCK)
    {
        fgApiVideoVgaSetClock(0, prParam->u1Clock);        
    }
    if(u4Mask & DRV_VDO_VGAMSK_PHASE)
    {
        fgApiVideoVgaSetPhase(0, prParam->u1Phase);    
    }    
    return rRetCode;
}

DRV_ERR_CODE_T _DRV_VDO_Auto(UINT8 u1VideoIndex)
{
    DRV_VDO_PARAM_T* prParam;
    
    VERIFY(_DRV_VDO_GetParameter(u1VideoIndex, &prParam) == DRV_E_OK);
    
    if(prParam->rInputSrc == DRV_IS_VGA_1)
    {
        fgApiVideoVgaAuto();       
    }
    else if(prParam->rInputSrc == DRV_IS_YPBPR_1 || 
        prParam->rInputSrc == DRV_IS_YPBPR_2 ||
        prParam->rInputSrc == DRV_IS_YPBPR_3 ||
        prParam->rInputSrc == DRV_IS_YPBPR_4)
    {
        fgApiVideoYPbPrAuto();
    }
    else
    {
        return DRV_E_FAIL;
    }
    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_VDO_Autocolor(UINT8 u1VideoIndex)
{
    DRV_VDO_PARAM_T* prParam;
    
    VERIFY(_DRV_VDO_GetParameter(u1VideoIndex, &prParam) == DRV_E_OK);
    
    if(prParam->rInputSrc == DRV_IS_VGA_1 ||
        prParam->rInputSrc == DRV_IS_YPBPR_1 || 
        prParam->rInputSrc == DRV_IS_YPBPR_2 ||
        prParam->rInputSrc == DRV_IS_YPBPR_3 ||
        prParam->rInputSrc == DRV_IS_YPBPR_4)
    {
        fgApiVideoAutoColor();
        return DRV_E_OK;        
    }
    else
    {
        return DRV_E_FAIL;
    }
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetVbiTtx
 *  Get VBI TTX data using the notify tag
 * 
 *  @param  u1VideoIndex            Video plane index
 *  @param  prVbiTtxData		    Pointer to DRV_VDO_VBITTX_DATA_T
 *  @param  u4Tag                   The tag used to retrieve data
 *
 *  @retval DRV_E_OK	            Succeed
 *  @retval DRV_E_FAIL	            Fail
 *  @retval DRV_E_BUFFER_UNDERFLOW  Partial data 
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_GetVbiTtx(UINT8 u1VideoIndex, 
	DRV_VDO_VBITTX_DATA_T* prVbiTtxData, INT32 i4Tag)
{
    DRV_VDO_VBITTX_COND_T rCond;   
    DRV_VDO_VBITTX_BUF_NODE_T* prBuf;
    
    UINT32 u4Tag = (UINT32)i4Tag;

    prBuf = (DRV_VDO_VBITTX_BUF_NODE_T*)((u4Tag >> 8) + (UINT32)_rTtxBuf.pbBuf);

    
    rCond = (DRV_VDO_VBITTX_COND_T)(u4Tag & 0xff);
    
    if(rCond == DRV_VDO_VBITTX_DATA_ARRIVAL)
    {            
        if(prBuf->u4Magic != DRV_VDO_VBITTX_BUF_NODE_MAGIC)
        {
            return DRV_E_FAIL;
        }
        prVbiTtxData->u2NumPacket = prBuf->u2NumPkt;      
        prVbiTtxData->prPacket = prBuf->atPkts;           
        return DRV_E_OK;         
    }
    else if(rCond == DRV_VDO_VBITTX_OVERFLOW)
    {
        return DRV_E_BUFFER_OVERFLOW;         
    }
    else
    {
        return DRV_E_INV_ARG;
    }
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_UnlockVbiTtx
 *  Unlock VBI TTX data using the notify tag
 * 
 *  @param  u1VideoIndex        Video plane index
 *  @param  prVbiTtxData		Pointer to DRV_VDO_VBITTX_DATA_T
 *  @param  u4Tag               The tag used to retrieve data
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_UnlockVbiTtx(UINT8 u1VideoIndex, 
	DRV_VDO_VBITTX_DATA_T* prVbiTtxData, INT32 i4Tag)
{
    DRV_ERR_CODE_T rRetCode;
    DRV_VDO_VBITTX_BUF_NODE_T* prBuf;
    UINT32 u4Tag = (UINT32)i4Tag;
    
    prBuf = (DRV_VDO_VBITTX_BUF_NODE_T*)((u4Tag >> 8) + (UINT32)_rTtxBuf.pbBuf);
    VERIFY( prBuf->atPkts == prVbiTtxData->prPacket);
    
    rRetCode = _VbiTtxBufFree(u4Tag);
    
    if ( rRetCode != DRV_E_OK )
    {
        /* Internal Error in TTX Buffer */
        CRIT_STATE_T        tCS;
        
        //TVD_DBG_MSG(( "\r\n[TVD%d] TTX BUF FREE ERROR (%d) \r\n",  pt_comp_id->ui2_id, i4Ret ));
        
        /* Pause buffer input on error */
        tCS = x_crit_start();
        _fgTtxBufErrPaused = TRUE;
        x_crit_end(tCS);
    
        rRetCode = DRV_E_FAIL;
    }
    return rRetCode;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetVbiCc
 *  Get VBI CC data using the notify tag
 * 
 *  @param  u1VideoIndex        Video plane index
 *  @param  prVbiCcData		    Pointer to DRV_VDO_VBICC_DATA_T
 *  @param  u4Tag               The tag used to retrieve data
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_GetVbiCc(UINT8 u1VideoIndex, 
	DRV_VDO_VBICC_DATA_T* prVbiCcData, INT32 i4Tag)
{
    UINT32 u4Tag = (UINT32)i4Tag;
    if((u4Tag & 0xff) == u1VideoIndex)
    {
        prVbiCcData->u1Byte1        = (u4Tag >> 8) & 0xff;
        prVbiCcData->u1Byte2        = (u4Tag >> 16) & 0xff;    
        prVbiCcData->fgEvenField    = (u4Tag >> 24) & 0xff;        
        return DRV_E_OK;        
    }
    return DRV_E_FAIL;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetVbiWss
 *  Get VBI WSS data using the notify tag
 * 
 *  @param  u1VideoIndex        Video plane index
 *  @param  prVbiWssData        Pointer to DRV_VDO_VBIWSS_DATA_T
 *  @param  u4Tag               The tag used to retrieve data
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_GetVbiWss(UINT8 u1VideoIndex, 
	DRV_VDO_VBIWSS_DATA_T* prVbiWssData, INT32 i4Tag)
{
    UINT32 u4Tag = (UINT32)i4Tag;
    if((u4Tag & 0xff) == u1VideoIndex)
    {
        prVbiWssData->u2Data = (u4Tag >> 8) & 0xffff;
        return DRV_E_OK;        
    }
    return DRV_E_FAIL;   
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetVbiVps
 *  Get VBI VPS data using the notify tag
 * 
 *  @param  u1VideoIndex        Video plane index
 *  @param  prVbiVpsData		    Pointer to DRV_VDO_VBIVPS_DATA_T
 *  @param  u4Tag               The tag used to retrieve data
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_GetVbiVps(UINT8 u1VideoIndex, 
	DRV_VDO_VBIVPS_DATA_T* prVbiVpsData, INT32 i4Tag)
{
    UINT32 u4Tag = (UINT32)i4Tag;
    prVbiVpsData->u2Data = u4Tag & 0xffff;
    return DRV_E_OK;            
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetScartPin8Nfy
 *  Get SCART pin 8 status via notify tag
 * 
 *  @param  u1VideoIndex        Video plane index
 *  @param  prScartPin8		    Pointer to DRV_VDO_SCART_PIN8_T
 *  @param  u4Tag               The tag used to retrieve data
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_GetScartFs(UINT8 u1VideoIndex, 
	DRV_VDO_SCART_FS_T* prScartPin8, INT32 i4Tag)
{
    UINT32 u4Tag = (UINT32)i4Tag;
    if(u1VideoIndex != (u4Tag & 0xff))
    {
        return DRV_E_FAIL;
    }
    *prScartPin8 = (DRV_VDO_SCART_FS_T)((u4Tag >> 8) & 0xff);
    return DRV_E_OK;            
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_GetScartStatus
 *  Get SCART status
 * 
 *  @param  u1VideoIndex        Video plane index
 *  @param  prScartStatus	    Pointer to DRV_VDO_SCART_STATUS_T
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_GetScartStatus(UINT8 u1VideoIndex, 
	DRV_VDO_SCART_STATUS_T* prScartStatus)
{
    prScartStatus->rBypass  = _rScartStatus.rBypass;
    prScartStatus->rOutput  = _rScartStatus.rOutput;    
    prScartStatus->rFs    = _rScartStatus.rFs;
    prScartStatus->rFb   = _rScartStatus.rFb;    
    return DRV_E_OK;            
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_ANA_Init
 *  Initialize analog video API
 *
 *  @param  void
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_ANA_Init(void)
{
    _InitVBI();
    _InitScart();
    NPTV_SetSigChgNfyFct(_ResChgNfy);
    NPTV_SetVgaAutoNfy(_VgaAutoNfy);
    NPTV_SetVgaAutocolorNfy(_VgaAutocolorNfy);    
    return DRV_E_OK;            
}
//-----------------------------------------------------------------------------
/** _DRV_VDO_ANA_Start
 *  Start playing and displaying analog video source
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_ANA_Start(UINT8 u1VideoIndex)
{
    DRV_VDO_PARAM_T* prParam;

    fgApiVideoColorSystem(u1VideoIndex, (UINT8)SV_CS_AUTO);    
    VERIFY(_DRV_VDO_GetParameter(u1VideoIndex, &prParam) == DRV_E_OK);
    VERIFY(_EnableVBI(u1VideoIndex) == DRV_E_OK);

#ifndef __MODEL_slt__
    _VDP_vDrvDIFWOnOff(0, 1);    
#endif
    
    VERIFY(_DRV_VDO_DriverUnmute(u1VideoIndex) == DRV_E_OK);
    
    return DRV_E_OK;
}

//-----------------------------------------------------------------------------
/** _DRV_VDO_ANA_Stop
 *  Stop playing analog video source
 *
 *  @param  u1VideoIndex          Video plane index
 *
 *  @retval DRV_E_OK	        Succeed
 *  @retval DRV_E_FAIL	        Fail
 */
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_VDO_ANA_Stop(UINT8 u1VideoIndex)
{
    VERIFY(_DRV_VDO_DriverMute(u1VideoIndex) == DRV_E_OK);
    VERIFY(_DisableVBI(u1VideoIndex) == DRV_E_OK);    
    return DRV_E_OK;
}


