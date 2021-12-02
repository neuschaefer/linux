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
 * $RCSfile: ntsc_vbi_psr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains specific implementation.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "strm_mngr/video_hdlr/x_sm_video_hdlr.h"
#include "strm_mngr/video_hdlr/ntsc_vbi_psr/ntsc_vbi_psr.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define NTSC_VBI_CLSS_CURRENT_CONT               ((UINT8)0x02)
#define NTSC_VBI_CLSS_FUTURE_CONT                ((UINT8)0x04)
#define NTSC_VBI_CLSS_CHANNEL_CONT               ((UINT8)0x06)
#define NTSC_VBI_CLSS_MISC_CONT                  ((UINT8)0x08)
#define NTSC_VBI_CLSS_PUB_SVC_CONT               ((UINT8)0x0A)
#define NTSC_VBI_CLSS_RSVD_CONT                  ((UINT8)0x0C)
#define NTSC_VBI_CLSS_PRVT_DATA_CONT             ((UINT8)0x0E)

#define NTSC_VBI_PARITY_ERR                      ((UINT8)0xFF)

#define VBI_CLS_IDX( cls )   ((UINT8)((cls - 1)>>1))

#define NTSC_IS_XDS( cmd ) (                                                \
                             ( cmd->u.t_cmd_bytes.uc_b1 <= 0x0F &&          \
                               cmd->u.t_cmd_bytes.uc_b1 > 0x00              \
                             )? TRUE: FALSE                                 \
                           )



#define NTSC_IS_CMD( cmd ) (                                                \
                             ( ( cmd->u.t_cmd_bytes.uc_b1 >= 0x10 &&        \
                                 cmd->u.t_cmd_bytes.uc_b1 <= 0x1F ) &&      \
                               ( cmd->u.t_cmd_bytes.uc_b2 >= 0x20 &&        \
                                 cmd->u.t_cmd_bytes.uc_b2 <= 0x7F )         \
                             )? TRUE: FALSE                                 \
                           )

#define NTSC_IS_XDS_INFO_CHAR( c )                                          \
                           (                                                \
                             ( c >= 0x20 && c <= 0x7F )? TRUE: FALSE        \
                           )

#define XDS_CMD_IS_CONT( _char )   ( ( _char%2 == 0 )? TRUE:FALSE )

#define GET_BIT( _char, _bit)  ( (_char & MAKE_BIT_MASK_8(_bit)) >> _bit )

#define MASK_PARITY( _char )   ( _char & (UCHAR)0x7F )

#define MAX_NUM_VBI_PSR        ((SIZE_T) 2)
#define MAX_CLSS_NUM           ((NTSC_VBI_CLSS_ALL-1)>>1)
#define MAX_VBI_BUFF_NUM       ((SIZE_T) 8)

typedef struct _NTSC_VBI_PSR_T
{
    BOOL        b_in_use;
    HANDLE_T    h_vs;
    UINT8       ui1_active_class;
    BOOL        b_is_caption_pkt;
    UINT8       aui1_active_type[MAX_CLSS_NUM];
    UINT8       aui1_cmplt_type[MAX_CLSS_NUM];
    UINT8       aaui1_vbi_info[MAX_CLSS_NUM][MAX_NTSC_VBI_INFO_SIZE];
    UINT8       aaui1_cmplt_vbi_info[MAX_CLSS_NUM][MAX_NTSC_VBI_INFO_SIZE];
    SIZE_T      az_vbi_info_sz[MAX_CLSS_NUM];
    SIZE_T      az_cmplt_vbi_info_sz[MAX_CLSS_NUM];
    UINT8       aui1_curr_rating[MAX_CLSS_NUM];
    UINT32      ui4_curr_rating_tick;
    NTSC_VBI_DATA_T  at_vbi[MAX_VBI_BUFF_NUM];
    UINT32      aui4_vbi_refcnt[MAX_VBI_BUFF_NUM];
    UINT8       ui1_vbi_data_cnt;
    UINT32      ui4_vbi_data_over_flow_cnt;
} NTSC_VBI_PSR_T;

typedef struct _SM_CC_NTSC_RAW_CMD_T
{
    union
    {
        struct
        {
#ifdef _CPU_LITTLE_ENDIAN_
            UCHAR    uc_b2;
            UCHAR    uc_b1;
#elif defined( _CPU_BIG_ENDIAN_ )
            UCHAR    uc_b1;
            UCHAR    uc_b2;
#endif  /*_CPU_LITTLE_ENDIAN_ */
        } t_cmd_bytes;
        UINT16 ui2_cmd;
    } u;
} SM_CC_NTSC_RAW_CMD_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL                b_ntsc_vbi_psr_init = FALSE;
static NTSC_VBI_PSR_T      at_vbi_psr[MAX_NUM_VBI_PSR];
static BOOL                gb_ntsc_vbi_psr_dbg_dump = FALSE;
static UINT32              gui4_ref_cnt = 0;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _check_odd_parity
 *
 * Description: <function description>
 *
 * Inputs:  uc_in References.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static BOOL _check_odd_parity( UCHAR uc_in )
{
    UCHAR uc_out = 0;

    uc_out += GET_BIT( uc_in, 0 );
    uc_out += GET_BIT( uc_in, 1 );
    uc_out += GET_BIT( uc_in, 2 );
    uc_out += GET_BIT( uc_in, 3 );
    uc_out += GET_BIT( uc_in, 4 );
    uc_out += GET_BIT( uc_in, 5 );
    uc_out += GET_BIT( uc_in, 6 );
    uc_out += GET_BIT( uc_in, 7 );

    return (BOOL)(uc_out & MAKE_BIT_MASK_8(0));
}

VOID _ntsc_vbi_psr_dump_bin( const UINT8* pui1_buff, SIZE_T z_size )
{
    UINT32 ui4_i = 0;

    for ( ui4_i = 0; ui4_i < z_size; ++ui4_i )
    {
        x_dbg_stmt( "%.2X ", pui1_buff[ui4_i] );
    }
}

static VOID _ntsc_vbi_psr_set_refcnt(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id,
    UINT8               ui1_idx,
    UINT32              ui4_ref_cnt )
{
    if ( ui1_idx >= (UINT8)MAX_VBI_BUFF_NUM || t_vbi_psr_id >= (UINT32)MAX_NUM_VBI_PSR )
    {
        return;
    }

    at_vbi_psr[t_vbi_psr_id].aui4_vbi_refcnt[ui1_idx] = ui4_ref_cnt;
    if ( ui4_ref_cnt == 0 )
    {
        x_memset( at_vbi_psr[t_vbi_psr_id].aui4_vbi_refcnt, 0,
            sizeof(UINT32)*MAX_VBI_BUFF_NUM );
    }
}

static UINT8 _ntsc_vbi_psr_get_free_vid_data_idx(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id,
    UINT8               ui1_class,
    UINT8               ui1_type )
{
    UINT8         ui1_idx = 0, ui1_j = 0;
    UINT32        ui4_min_cnt_idx = (UINT32)0xFFFFFFFF;
    CRIT_STATE_T  t_cs;

    t_cs = x_crit_start();

    for ( ui1_idx = 0; ui1_idx < (UINT8)MAX_VBI_BUFF_NUM; ui1_idx++ )
    {
        if ( at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_idx].ui1_class == ui1_class &&
             at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_idx].ui1_type == ui1_type )
        {
            _ntsc_vbi_psr_set_refcnt( t_vbi_psr_id, ui1_idx, ++gui4_ref_cnt );
            x_crit_end( t_cs );

            return ui1_idx;
        }
    }

    for ( ui1_idx = 0; ui1_idx < (UINT8)MAX_VBI_BUFF_NUM; ui1_idx++ )
    {
        if ( at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_idx].ui1_class == 0 )
        {
            _ntsc_vbi_psr_set_refcnt( t_vbi_psr_id, ui1_idx, ++gui4_ref_cnt );
            x_crit_end( t_cs );

            return ui1_idx;
        }
    }

    ui1_idx = (UINT8)MAX_VBI_BUFF_NUM;
    for ( ui1_j = 0; ui1_j < (UINT8)MAX_VBI_BUFF_NUM; ui1_j++ )
    {
        if ( ui4_min_cnt_idx > at_vbi_psr[t_vbi_psr_id].aui4_vbi_refcnt[ui1_j] )
        {
            ui4_min_cnt_idx = at_vbi_psr[t_vbi_psr_id].aui4_vbi_refcnt[ui1_j];
            ui1_idx = ui1_j;
        }
    }
    if ( ui1_idx != (UINT8)MAX_VBI_BUFF_NUM )
    {
        _ntsc_vbi_psr_set_refcnt( t_vbi_psr_id, ui1_idx, ++gui4_ref_cnt );
        x_crit_end( t_cs );

        return ui1_idx;
    }

    x_crit_end( t_cs );

    return 0xFF;
}

static UINT8 _ntsc_vbi_psr_get_vid_data_by_class_type(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id,
    UINT8               ui1_class,
    UINT8               ui1_type )
{
    UINT8         ui1_idx = 0;
    CRIT_STATE_T  t_cs;

    t_cs = x_crit_start();

    for ( ui1_idx = 0; ui1_idx < (UINT8)MAX_VBI_BUFF_NUM; ui1_idx++ )
    {
        if ( at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_idx].ui1_class == ui1_class &&
             at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_idx].ui1_type == ui1_type )
        {
            x_crit_end( t_cs );

            return ui1_idx;
        }
    }

    x_crit_end( t_cs );

    return 0xFF;
}

static VOID _ntsc_vbi_psr_term_pkt(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id,
    UINT8               ui1_checksum )
{
    UINT8            ui1_cls_idx;
    UINT8            ui1_type;
    SIZE_T           z_size;
    UINT8*           pui1_data = NULL;
    SIZE_T           z_i;
    UINT8            ui1_sum = 0;
    NTSC_VBI_DATA_T* pt_vbi = NULL;
    NTSC_VBI_DATA_T  t_vbi;
    BOOL             b_is_parity_err = FALSE;
    CRIT_STATE_T     t_cs;
    UINT8            ui1_vbi_data_idx = 0xFF;

    ui1_cls_idx = VBI_CLS_IDX(at_vbi_psr[t_vbi_psr_id].ui1_active_class );

    if (ui1_cls_idx >= (UINT8)MAX_CLSS_NUM)
    {
        return;
    }

    z_size    = at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx];
    ui1_type  = at_vbi_psr[t_vbi_psr_id].aui1_active_type[ui1_cls_idx];
    pui1_data = at_vbi_psr[t_vbi_psr_id].aaui1_vbi_info[ui1_cls_idx];

    if ( z_size > MAX_NTSC_VBI_INFO_SIZE ||
         z_size == 0 )
    {
        return;
    }

    b_is_parity_err = ( (ui1_checksum==NTSC_VBI_PARITY_ERR)? TRUE: FALSE );

    /* Check Sum */
    ui1_sum += ((ui1_cls_idx<<1) + 1);
    ui1_sum += ui1_type;

    for( z_i = 0; z_i < z_size; z_i++ )
    {
        ui1_sum += pui1_data[z_i];
        if ( pui1_data[z_i] == NTSC_VBI_PARITY_ERR )
        {
            b_is_parity_err = TRUE;
        }
    }

    ui1_sum += NTSC_VBI_CLSS_ALL;
    ui1_sum += ui1_checksum;

    /* Check-Sum only use 7-bits, EIA/CEA-608-B 8.6.3 */
    if ( (ui1_sum&0x7F) != 0 || b_is_parity_err )
    {
        UINT32   ui4_tick_now = x_os_get_sys_tick();
        INT32    i4_tick_delta = 0;

        i4_tick_delta = (INT32)(ui4_tick_now - at_vbi_psr[t_vbi_psr_id].ui4_curr_rating_tick);
        if ( i4_tick_delta < 0 )
        {
            /* take care of round */
            i4_tick_delta = (INT32)(at_vbi_psr[t_vbi_psr_id].ui4_curr_rating_tick +
                            (0xFFFFFFFF - ui4_tick_now));
        }

        /* If this packet if for CONTENT ADVISORY, and there is a old rating
           record recorded in 5 seconds(1000 ticks), send upper layer the old
           record instead. */
        if ( ((ui1_cls_idx<<1) + 1) == NTSC_VBI_CLSS_CURRENT &&
             ui1_type == NTSC_VBI_CLSS_1_TYPE_CONT_ADV &&
             at_vbi_psr[t_vbi_psr_id].ui4_curr_rating_tick != 0 &&
             i4_tick_delta <= (6200 / 5) )
        {
            /* rating is only 2 bytes */
            z_size = 2;
            x_memcpy( pui1_data, at_vbi_psr[t_vbi_psr_id].aui1_curr_rating,
                      z_size );
        }
        else
        {
            if ( gb_ntsc_vbi_psr_dbg_dump )
            {
                x_dbg_stmt( "\r\n{VBI} Discard packet with error checksum, tick=%u\r\n",
                    x_os_get_sys_tick() );
                x_dbg_stmt( "\t[%.2X %.2X ", ((ui1_cls_idx<<1) + 1), ui1_type );
                _ntsc_vbi_psr_dump_bin( pui1_data, z_size );
                x_dbg_stmt( "%.2X %.2X]\r\n", NTSC_VBI_CLSS_ALL, ui1_checksum );
            }
            return;
        }
    }
    else
    {
        if ( ((ui1_cls_idx<<1) + 1) == NTSC_VBI_CLSS_CURRENT && 
             ui1_type == NTSC_VBI_CLSS_1_TYPE_CONT_ADV )
        {
            if (z_size > (SIZE_T)MAX_CLSS_NUM)
            {   /*for kwlock warning. */
                z_size = (SIZE_T)MAX_CLSS_NUM;
            }
            x_memcpy( at_vbi_psr[t_vbi_psr_id].aui1_curr_rating, pui1_data, z_size );
            at_vbi_psr[t_vbi_psr_id].ui4_curr_rating_tick = x_os_get_sys_tick();
        }
    }

    if ( gb_ntsc_vbi_psr_dbg_dump )
    {
        x_dbg_stmt( "\r\n{VBI} Notify packet, tick=%u\r\n", x_os_get_sys_tick() );
        x_dbg_stmt( "\t[%.2X %.2X ", ((ui1_cls_idx<<1) + 1), ui1_type );
        _ntsc_vbi_psr_dump_bin( pui1_data, z_size );
        x_dbg_stmt( "%.2X %.2X]\r\n\r\n", NTSC_VBI_CLSS_ALL, ui1_checksum );
    }

    t_vbi.ui1_class = (UINT8)((ui1_cls_idx << 1) + 1);
    t_vbi.ui1_type = ui1_type;
    t_vbi.z_info_size = z_size;
    x_memcpy( t_vbi.aui1_info, pui1_data, z_size );

    t_cs = x_crit_start();

    ui1_vbi_data_idx = _ntsc_vbi_psr_get_free_vid_data_idx(
        t_vbi_psr_id, t_vbi.ui1_class, t_vbi.ui1_type );
    if ( ui1_vbi_data_idx < (UINT8)MAX_VBI_BUFF_NUM )
    {
        pt_vbi = &at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_vbi_data_idx];
        *pt_vbi = t_vbi;
        at_vbi_psr[t_vbi_psr_id].ui1_vbi_data_cnt++;
    }
    else
    {
        at_vbi_psr[t_vbi_psr_id].ui4_vbi_data_over_flow_cnt++;
    }

    x_crit_end( t_cs );

    x_sm_video_hdlr_nfy_vbi_data(
        at_vbi_psr[t_vbi_psr_id].h_vs,
        t_vbi_psr_id,
        (VOID*)&t_vbi,
        sizeof(NTSC_VBI_DATA_T) );
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

BOOL ntsc_vbi_psr_open(
    HANDLE_T            h_vs,
    VSH_VBI_PSR_ID_T*   pt_vbi_psr_id )
{
    UINT32              ui4_i;
    CRIT_STATE_T        t_cs;
    NTSC_VBI_PSR_T*     pt_vbi_psr = NULL;

    if ( !pt_vbi_psr_id )
    {
        return FALSE;
    }

    t_cs = x_crit_start();

    for( ui4_i = 0; ui4_i < MAX_NUM_VBI_PSR; ui4_i++ )
    {
        if ( !at_vbi_psr[ui4_i].b_in_use )
        {
            pt_vbi_psr = &at_vbi_psr[ui4_i];
            pt_vbi_psr->b_in_use = TRUE;
            break;
        }
    }

    x_crit_end( t_cs );

    if ( !pt_vbi_psr )
    {
        return FALSE;
    }

    *pt_vbi_psr_id = ui4_i;

    at_vbi_psr[ui4_i].h_vs = h_vs;
    at_vbi_psr[ui4_i].ui1_active_class = NTSC_VBI_CLSS_ALL;
    at_vbi_psr[ui4_i].b_is_caption_pkt = TRUE;

    x_memset(
        at_vbi_psr[ui4_i].aui1_active_type, 0,
        MAX_CLSS_NUM*sizeof(UINT8) );
    x_memset(
        at_vbi_psr[ui4_i].aui1_cmplt_type, 0,
        MAX_CLSS_NUM*sizeof(UINT8) );
    x_memset(
        at_vbi_psr[ui4_i].aaui1_vbi_info, 0,
        MAX_CLSS_NUM*MAX_NTSC_VBI_INFO_SIZE*sizeof(UINT8) );
    x_memset(
        at_vbi_psr[ui4_i].aaui1_cmplt_vbi_info, 0,
        MAX_CLSS_NUM*MAX_NTSC_VBI_INFO_SIZE*sizeof(UINT8) );
    x_memset(
        at_vbi_psr[ui4_i].az_vbi_info_sz, 0,
        MAX_CLSS_NUM*sizeof(SIZE_T) );
    x_memset(
        at_vbi_psr[ui4_i].az_cmplt_vbi_info_sz, 0,
        MAX_CLSS_NUM*sizeof(SIZE_T) );

    x_memset( at_vbi_psr[ui4_i].aui1_curr_rating, 0,
        MAX_CLSS_NUM*sizeof(UINT8) );
    at_vbi_psr[ui4_i].ui4_curr_rating_tick = 0;

    x_memset( at_vbi_psr[ui4_i].at_vbi, 0,
        sizeof(NTSC_VBI_DATA_T)*MAX_VBI_BUFF_NUM );
    at_vbi_psr[ui4_i].ui1_vbi_data_cnt = 0;
    at_vbi_psr[ui4_i].ui4_vbi_data_over_flow_cnt = 0;

    x_memset( at_vbi_psr[ui4_i].aui4_vbi_refcnt, 0,
        sizeof(UINT32)*MAX_VBI_BUFF_NUM );

    return TRUE;
}

BOOL ntsc_vbi_parse(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id,
    UINT8*              pui1_vbi_data,
    SIZE_T              z_vbi_data,
    VOID*               pv_tag )
{
    SM_CC_NTSC_RAW_CMD_T    t_raw_cmd;
    SIZE_T                  z_curr;
    UINT8*                  pui1_iter;

    if ( t_vbi_psr_id >= MAX_NUM_VBI_PSR )
    {
        return FALSE;
    }
    if ( at_vbi_psr[t_vbi_psr_id].b_in_use == FALSE ||
         !pui1_vbi_data )
    {
        return FALSE;
    }
    if ( pv_tag )
    {
        *((BOOL*)pv_tag) = FALSE;
    }

    pui1_iter = pui1_vbi_data;
    for( z_curr = 0; z_curr < z_vbi_data; z_curr += 3 )
    {
        BOOL  b_even_field = *( pui1_iter + z_curr);
        UCHAR uc_org_b1 = 0;
        UCHAR uc_org_b2 = 0;


        uc_org_b1 = t_raw_cmd.u.t_cmd_bytes.uc_b1 = *( pui1_iter + z_curr + 1);
        uc_org_b2 = t_raw_cmd.u.t_cmd_bytes.uc_b2 = *( pui1_iter + z_curr + 2 );

        if ( !_check_odd_parity( t_raw_cmd.u.t_cmd_bytes.uc_b1 ) )
        {
            t_raw_cmd.u.t_cmd_bytes.uc_b1 = NTSC_VBI_PARITY_ERR;
        }
        if ( !_check_odd_parity( t_raw_cmd.u.t_cmd_bytes.uc_b2 ) )
        {
            t_raw_cmd.u.t_cmd_bytes.uc_b2 = NTSC_VBI_PARITY_ERR;
        }

        t_raw_cmd.u.t_cmd_bytes.uc_b1 = MASK_PARITY( t_raw_cmd.u.t_cmd_bytes.uc_b1 );
        t_raw_cmd.u.t_cmd_bytes.uc_b2 = MASK_PARITY( t_raw_cmd.u.t_cmd_bytes.uc_b2 );

        if ( !b_even_field )
        {
            /* If this packet is CC command, set CC indicator to TRUE */
            if ( NTSC_IS_CMD( (&t_raw_cmd) ) && pv_tag )
            {
                *((BOOL*)pv_tag) = TRUE;
            }
            continue;
        }

        if ( gb_ntsc_vbi_psr_dbg_dump )
        {
            if ( t_raw_cmd.u.t_cmd_bytes.uc_b1 == NTSC_VBI_PARITY_ERR ||
                 t_raw_cmd.u.t_cmd_bytes.uc_b2 == NTSC_VBI_PARITY_ERR )
            {
                x_dbg_stmt( "[%.2X(%.2X) %.2X(%.2X)]",
                    t_raw_cmd.u.t_cmd_bytes.uc_b1, uc_org_b1,
                    t_raw_cmd.u.t_cmd_bytes.uc_b2, uc_org_b2 );
            }
            else
            {
                x_dbg_stmt( "[%.2X %.2X]",
                    t_raw_cmd.u.t_cmd_bytes.uc_b1,
                    t_raw_cmd.u.t_cmd_bytes.uc_b2 );
            }
        }

        if ( NTSC_IS_CMD( (&t_raw_cmd) ) )
        {
            at_vbi_psr[t_vbi_psr_id].b_is_caption_pkt = TRUE;

            /* If this packet is CC command, set CC indicator to TRUE */
            if ( pv_tag )
            {
                *((BOOL*)pv_tag) = TRUE;
            }
            continue;
        }
        else if ( NTSC_IS_XDS( (&t_raw_cmd) ) )
        {
            UINT8  ui1_cls_idx = 0;

            if ( gb_ntsc_vbi_psr_dbg_dump )
            {
                x_dbg_stmt( "<<- " );
            }

            switch( t_raw_cmd.u.t_cmd_bytes.uc_b1 )
            {
            case NTSC_VBI_CLSS_CURRENT:
            case NTSC_VBI_CLSS_CURRENT_CONT:

            case NTSC_VBI_CLSS_FUTURE:
            case NTSC_VBI_CLSS_FUTURE_CONT:

            case NTSC_VBI_CLSS_CHANNEL:
            case NTSC_VBI_CLSS_CHANNEL_CONT:

            case NTSC_VBI_CLSS_MISC:
            case NTSC_VBI_CLSS_MISC_CONT:

            case NTSC_VBI_CLSS_PUB_SVC:
            case NTSC_VBI_CLSS_PUB_SVC_CONT:

            case NTSC_VBI_CLSS_RSVD:
            case NTSC_VBI_CLSS_RSVD_CONT:

            case NTSC_VBI_CLSS_PRVT_DATA:
            case NTSC_VBI_CLSS_PRVT_DATA_CONT:
                break;

            case NTSC_VBI_CLSS_ALL:
                _ntsc_vbi_psr_term_pkt(t_vbi_psr_id, t_raw_cmd.u.t_cmd_bytes.uc_b2 );

                ui1_cls_idx = VBI_CLS_IDX(at_vbi_psr[t_vbi_psr_id].ui1_active_class );
                
                if (ui1_cls_idx >= (UINT8)MAX_CLSS_NUM)
                {
                    continue;
                }

                at_vbi_psr[t_vbi_psr_id].aui1_active_type[ui1_cls_idx] = 0;
                at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx] = 0;
                at_vbi_psr[t_vbi_psr_id].ui1_active_class = NTSC_VBI_CLSS_ALL;

            default:
                at_vbi_psr[t_vbi_psr_id].b_is_caption_pkt = TRUE;
                continue;
            }

            at_vbi_psr[t_vbi_psr_id].ui1_active_class =
                t_raw_cmd.u.t_cmd_bytes.uc_b1;

            ui1_cls_idx = VBI_CLS_IDX(at_vbi_psr[t_vbi_psr_id].ui1_active_class );
            if (ui1_cls_idx >= (UINT8)MAX_CLSS_NUM)
            {
                continue;
            }

            if ( t_raw_cmd.u.t_cmd_bytes.uc_b2 !=
                 at_vbi_psr[t_vbi_psr_id].aui1_active_type[ui1_cls_idx] )
            {
                at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx] = 0;
                at_vbi_psr[t_vbi_psr_id].aui1_active_type[ui1_cls_idx] =
                    t_raw_cmd.u.t_cmd_bytes.uc_b2;
            }

            at_vbi_psr[t_vbi_psr_id].b_is_caption_pkt = FALSE;
        }
        else
        {
            UINT8  ui1_atv_cls = at_vbi_psr[t_vbi_psr_id].ui1_active_class;
            UINT8  ui1_cls_idx = VBI_CLS_IDX( ui1_atv_cls );

            if (ui1_cls_idx >= (UINT8)MAX_CLSS_NUM)
            {
                continue;
            }

            if ( at_vbi_psr[t_vbi_psr_id].b_is_caption_pkt )
            {
                continue;
            }
            if ( ui1_atv_cls >= NTSC_VBI_CLSS_ALL )
            {
                continue;
            }
            if ( !NTSC_IS_XDS_INFO_CHAR( t_raw_cmd.u.t_cmd_bytes.uc_b1 ) )
            {
                continue;
            }

            if ( XDS_CMD_IS_CONT(ui1_atv_cls) )
            {
                SIZE_T z_size = at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx];

                if (z_size + 1 >= MAX_NTSC_VBI_INFO_SIZE )
                {
                    continue;
                }

                at_vbi_psr[t_vbi_psr_id].aaui1_vbi_info[ui1_cls_idx][z_size++] =
                    t_raw_cmd.u.t_cmd_bytes.uc_b1;
                at_vbi_psr[t_vbi_psr_id].aaui1_vbi_info[ui1_cls_idx][z_size++] =
                    t_raw_cmd.u.t_cmd_bytes.uc_b2;

                at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx] = z_size;
            }
            else /* IS START of a class */
            {
                SIZE_T z_size =
                    at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx] = 0;

                at_vbi_psr[t_vbi_psr_id].aaui1_vbi_info[ui1_cls_idx][z_size++] =
                    t_raw_cmd.u.t_cmd_bytes.uc_b1;
                at_vbi_psr[t_vbi_psr_id].aaui1_vbi_info[ui1_cls_idx][z_size++] =
                    t_raw_cmd.u.t_cmd_bytes.uc_b2;

                at_vbi_psr[t_vbi_psr_id].az_vbi_info_sz[ui1_cls_idx] = z_size;

                /* Increace active class by 1, to simulate that following
                   data are continue data. */
                at_vbi_psr[t_vbi_psr_id].ui1_active_class++;
            }
        }
    }

    return TRUE;
}

BOOL ntsc_vbi_psr_get(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id,
    VOID*               pv_vbi_data,
    SIZE_T*             pz_vbi_data_size )
{
    BOOL                b_success = FALSE;
    CRIT_STATE_T        t_cs;
    UINT8               ui1_vbi_data_idx = 0xFF;
    NTSC_VBI_DATA_T*    pt_app_vbi = (NTSC_VBI_DATA_T*)pv_vbi_data;
    NTSC_VBI_DATA_T*    pt_vbi = NULL;

    if ( t_vbi_psr_id >= MAX_NUM_VBI_PSR ||
         !pz_vbi_data_size || *pz_vbi_data_size < sizeof(NTSC_VBI_DATA_T) ||
         !pt_app_vbi )
    {
        return FALSE;
    }
    if ( at_vbi_psr[t_vbi_psr_id].b_in_use == FALSE )
    {
        return FALSE;
    }

    t_cs = x_crit_start();

    ui1_vbi_data_idx = _ntsc_vbi_psr_get_vid_data_by_class_type(
        t_vbi_psr_id,
        pt_app_vbi->ui1_class,
        pt_app_vbi->ui1_type );

    if ( ui1_vbi_data_idx < (UINT8)MAX_VBI_BUFF_NUM )
    {
        pt_vbi = &at_vbi_psr[t_vbi_psr_id].at_vbi[ui1_vbi_data_idx];

        if ( pt_app_vbi->z_info_size >= pt_vbi->z_info_size )
        {
            pt_app_vbi->z_info_size = pt_vbi->z_info_size;
            x_memcpy( pt_app_vbi->aui1_info, pt_vbi->aui1_info, pt_vbi->z_info_size );
        }
        if ( gb_ntsc_vbi_psr_dbg_dump )
        {
            x_dbg_stmt( "{VBI} ntsc_vbi_psr_get, VBI cls=%d, type=%d \r\n",
                pt_app_vbi->ui1_class, pt_app_vbi->ui1_type );
        }

        pt_vbi->ui1_class = 0;
        pt_vbi->ui1_type = 0;

        at_vbi_psr[t_vbi_psr_id].ui1_vbi_data_cnt--;
        b_success = TRUE;
    }

    x_crit_end( t_cs );

    *pz_vbi_data_size = sizeof(NTSC_VBI_DATA_T);

    return b_success;
}

BOOL ntsc_vbi_psr_close(
    VSH_VBI_PSR_ID_T    t_vbi_psr_id )
{
    CRIT_STATE_T        t_cs;

    if ( t_vbi_psr_id >= MAX_NUM_VBI_PSR )
    {
        return FALSE;
    }
    if ( at_vbi_psr[t_vbi_psr_id].b_in_use == FALSE )
    {
        return FALSE;
    }

    t_cs = x_crit_start();

    at_vbi_psr[t_vbi_psr_id].b_in_use = FALSE;

    x_crit_end( t_cs );

    return TRUE;
}

BOOL x_ntsc_vbi_init( VOID )
{
    UINT32 ui4_i;
    INT32  i4_ret;

    if ( b_ntsc_vbi_psr_init )
    {
        return TRUE;
    }

    for( ui4_i = 0; ui4_i < MAX_NUM_VBI_PSR; ui4_i++ )
    {
        at_vbi_psr[ui4_i].b_in_use = FALSE;
        at_vbi_psr[ui4_i].h_vs = NULL_HANDLE;
        at_vbi_psr[ui4_i].ui1_active_class = NTSC_VBI_CLSS_ALL;
        at_vbi_psr[ui4_i].b_is_caption_pkt = TRUE;

        x_memset(
            at_vbi_psr[ui4_i].aui1_active_type, 0,
            MAX_CLSS_NUM*sizeof(UINT8) );
        x_memset(
            at_vbi_psr[ui4_i].aui1_cmplt_type, 0,
            MAX_CLSS_NUM*sizeof(UINT8) );
        x_memset(
            at_vbi_psr[ui4_i].aaui1_vbi_info, 0,
            MAX_CLSS_NUM*MAX_NTSC_VBI_INFO_SIZE*sizeof(UINT8) );
        x_memset(
            at_vbi_psr[ui4_i].aaui1_cmplt_vbi_info, 0,
            MAX_CLSS_NUM*MAX_NTSC_VBI_INFO_SIZE*sizeof(UINT8) );
        x_memset(
            at_vbi_psr[ui4_i].az_vbi_info_sz, 0,
            MAX_CLSS_NUM*sizeof(SIZE_T) );
        x_memset(
            at_vbi_psr[ui4_i].az_cmplt_vbi_info_sz, 0,
            MAX_CLSS_NUM*sizeof(SIZE_T) );

        x_memset( at_vbi_psr[ui4_i].aui1_curr_rating, 0,
            MAX_CLSS_NUM*sizeof(UINT8) );
        at_vbi_psr[ui4_i].ui4_curr_rating_tick = 0;
    }

    i4_ret = x_sm_video_hdlr_reg_vbi_psr(
        SM_VSH_VBI_TYPE_NTSC,
        ntsc_vbi_psr_open,
        ntsc_vbi_parse,
        ntsc_vbi_psr_get,
        ntsc_vbi_psr_close );
    if ( i4_ret != SMR_OK )
    {
        return FALSE;
    }

    return TRUE;
}

VOID x_ntsc_vbi_dbg_ctrl( BOOL b_on )
{
    gb_ntsc_vbi_psr_dbg_dump = b_on;
}
