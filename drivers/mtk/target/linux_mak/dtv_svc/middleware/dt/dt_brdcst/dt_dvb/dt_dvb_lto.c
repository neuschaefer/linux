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
 * $RCSfile: dt_dvb_lto.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the APIs to receive and decode the ATSC's
 *         System Time Table (STT) objects.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "dt/_dt.h"
#include "dt/dt_dbg.h"
#include "dt/dt_brdcst/dt_dvb/dt_dvb_lto.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define DESCR_TAG_DVB_LOC_TIME_OFFS  ((UINT8) 0x58)

/* Buffer allignment to store the local time offset entries. */
#define LTO_BUFF_ALIGN  32

/* A single local time offset entry is 13 Bytes in size. */
#define LTO_ENTRY_SIZE  13

#define GET_LTO_ENTRY_REF(_lto_buff, _idx)  ((((INT32)(_idx * LTO_ENTRY_SIZE)) < ((INT32)_lto_buff->ui2_data_len)) ? &(_lto_buff->pui1_buff [_idx * LTO_ENTRY_SIZE]) : NULL)

/* Flags. */
#define LTO_FLAG_DEAD_LTO_CTRL          ((UINT8) 0x01)
#define LTO_FLAG_NUM_LTO_ENTRIES_VALID  ((UINT8) 0x02)

/* Buffer control structure. */
typedef struct _LTO_BUFF_CTRL_T
{
    UINT8*  pui1_buff;            /* References the buffer holding the local time offset entries. */

    UINT16  ui2_buff_len;         /* Total buffer length. */
    UINT16  ui2_data_len;         /* Valid data length. */

    UINT16  ui2_ref_cnt;          /* Reference counter. */

    UINT16  ui2_num_lto_entries;  /* Number of unique LTO entries. */

    UINT8  ui1_flags;             /* Some flags. */
}   LTO_BUFF_CTRL_T;


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static LTO_BUFF_CTRL_T*  pt_sys_lto_buff    = NULL;
static LTO_BUFF_CTRL_T*  pt_client_lto_buff = NULL;

static ISO_3166_COUNT_T  t_sys_count_code    = {'\0'};
static ISO_3166_COUNT_T  t_client_count_code = {'\0'};

static UINT8  ui1_sys_region_id    = 0;
static UINT8  ui1_client_region_id = 0;

static BOOL b_sys_count_code_set = FALSE;

static UINT32   ui4_config_flag = (UINT32)(DT_DVB_MATCH_CNTRY_AND_REG_CODE |
                                         DT_DVB_MATCH_CNTRY_CODE |
                                         DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE );

static BOOL     b_config_lto_chg_time = FALSE;
static BOOL     b_config_lto_raw_tz_when_dst_ctrl_off = FALSE;




/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static LTO_BUFF_CTRL_T* dt_alloc_buff_ctrl (UINT16  ui2_buff_len);
static VOID dt_free_buff_ctrl (LTO_BUFF_CTRL_T*  pt_lto_buff);
static VOID dt_extend_buff (LTO_BUFF_CTRL_T*  pt_lto_buff,
                            UINT16            ui2_buff_len);

static UINT16 dt_get_lto_len (const UINT8*  pui1_descr_list,
                              UINT16        ui2_descr_list_len);
static VOID dt_write_buff (const UINT8*      pui1_descr_list,
                           UINT16            ui2_descr_list_len,
                           LTO_BUFF_CTRL_T*  pt_lto_buff);
static BOOL dt_dup_buff (LTO_BUFF_CTRL_T*   pt_src_lto_buff,
                         LTO_BUFF_CTRL_T**  ppt_dest_lto_buff);
static UINT8* dt_find_lto_entry_ref (LTO_BUFF_CTRL_T*  pt_lto_buff,
                                     UINT16            ui2_idx);
static BOOL dt_get_tz_offset (const UINT8*  pui1_descr_list,
                              UINT16        ui2_descr_list_len,
                              DVB_TOT_T*    pt_tot);


/*-----------------------------------------------------------------------------
 * Name: dt_alloc_buff_ctrl
 *
 * Description: This API allocates an LTO buffer control object plus a buffer
 *              if argument "ui2_buff_len" is greater than '0'.
 *
 * Inputs:  ui2_buff_len  Contains the required buffer length.
 *
 * Outputs: -
 *
 * Returns: A reference to the allocated LTO buffer control object or NULL.
 ----------------------------------------------------------------------------*/
static LTO_BUFF_CTRL_T* dt_alloc_buff_ctrl (UINT16  ui2_buff_len)
{
    LTO_BUFF_CTRL_T* pt_lto_buff;


    if ((pt_lto_buff = (LTO_BUFF_CTRL_T*) x_mem_alloc (sizeof (LTO_BUFF_CTRL_T))) != NULL)
    {
        x_memset (((VOID*) pt_lto_buff), 0, sizeof (LTO_BUFF_CTRL_T));

        if (ui2_buff_len > 0)
        {
            if ((pt_lto_buff->pui1_buff = (UINT8*) x_mem_alloc ((SIZE_T) ui2_buff_len)) != NULL)
            {
                pt_lto_buff->ui2_buff_len = ui2_buff_len;
            }
            else
            {
                /* Oops, out of resources. Clean-up. */
                x_mem_free ((VOID*) pt_lto_buff);

                pt_lto_buff = NULL;
            }
        }
    }

    return (pt_lto_buff);
}


/*-----------------------------------------------------------------------------
 * Name: dt_free_buff_ctrl
 *
 * Description: This API frees an LTO buffer control object plus the actual
 *              buffer if one is available.
 *
 * Inputs:  pt_lto_buff  References the LTO buffer control object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID dt_free_buff_ctrl (LTO_BUFF_CTRL_T*  pt_lto_buff)
{
    if (pt_lto_buff != NULL)
    {
        if (pt_lto_buff->pui1_buff != NULL)
        {
            x_mem_free ((VOID*) pt_lto_buff->pui1_buff);
        }

        x_mem_free ((VOID*) pt_lto_buff);
    }
}


/*-----------------------------------------------------------------------------
 * Name: dt_extend_buff
 *
 * Description: This API extends the buffer in a LTO buffer control object.
 *
 * Inputs:  pt_lto_buff   References the LTO buffer control object.
 *          ui2_buff_len  Specifies the minimum required buffer length.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID dt_extend_buff (LTO_BUFF_CTRL_T*  pt_lto_buff,
                            UINT16            ui2_buff_len)
{
    UINT8* pui1_buff;


    if (ui2_buff_len > 0)
    {
        /* Adjust the minimum buffer length. */
        ui2_buff_len = (UINT16)(((ui2_buff_len + (LTO_BUFF_ALIGN - 1)) / LTO_BUFF_ALIGN) * LTO_BUFF_ALIGN);

        if ((pui1_buff = (UINT8*) x_mem_alloc ((SIZE_T) ui2_buff_len)) != NULL)
        {
            /* Ok, we successfully allocated a new buffer.   */
            /* Now free the existing one (if one is actually */
            /* available) and set the new one.               */
            if (pt_lto_buff->pui1_buff != NULL)
            {
                x_mem_free ((VOID*) (pt_lto_buff->pui1_buff));
            }

            pt_lto_buff->pui1_buff    = pui1_buff;
            pt_lto_buff->ui2_buff_len = ui2_buff_len;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: dt_get_lto_len
 *
 * Description: This API parses a list of descriptors and calculates the space
 *              required to store the 'local_time_offset' descriptors.
 *
 * Inputs:  pui1_descr_list     References the list of descriptors.
 *          ui2_descr_list_len  Contains the length of descriptors.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static UINT16 dt_get_lto_len (const UINT8*  pui1_descr_list,
                              UINT16        ui2_descr_list_len)
{
    UINT16 ui2_descr_len;
    UINT16 ui2_lto_len;


    ui2_lto_len = 0;

    /* The minimum length of a 'local_time_offset' descriptor */
    /* is equal to (LTO_ENTRY_SIZE + 2).                      */
    while (ui2_descr_list_len >= (LTO_ENTRY_SIZE + 2))
    {
        ui2_descr_len = (UINT16)( ((UINT16) (pui1_descr_list [1])) + 2 );

        if (pui1_descr_list [0] == DESCR_TAG_DVB_LOC_TIME_OFFS)
        {
            ui2_lto_len += (ui2_descr_len - 2);
        }

        /* And update the references and indexes. */
        pui1_descr_list += ui2_descr_len;

        if (ui2_descr_len <= ui2_descr_list_len)
        {
            ui2_descr_list_len -= ui2_descr_len;
        }
        else
        {
            ui2_descr_list_len = 0;
        }
    }

    return (ui2_lto_len);
}


/*-----------------------------------------------------------------------------
 * Name: dt_write_buff
 *
 * Description: This API parses a list of descriptors and copies valid
 *              'local_time_offset' descriptor entries to the specified buffer.
 *              Valid means that an entry must be LTO_ENTRY_SIZE in size.
 *
 * Inputs:  pui1_descr_list     References the list of descriptors.
 *          ui2_descr_list_len  Contains the length of descriptors.
 *          pt_lto_buff         References the LTO buffer control object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID dt_write_buff (const UINT8*      pui1_descr_list,
                           UINT16            ui2_descr_list_len,
                           LTO_BUFF_CTRL_T*  pt_lto_buff)
{
    UINT16 ui2_descr_len;
    UINT16 ui2_lto_len;


    pt_lto_buff->ui2_data_len = 0;

    /* The minimum length of a 'local_time_offset' descriptor */
    /* is equal to (LTO_ENTRY_SIZE + 2).                      */
    while (ui2_descr_list_len >= (LTO_ENTRY_SIZE + 2))
    {
        ui2_descr_len = (UINT16)( ((UINT16) (pui1_descr_list [1])) + 2 );

        if (pui1_descr_list [0] == DESCR_TAG_DVB_LOC_TIME_OFFS)
        {
            /* Adjust the descriptor length if required. */
            if (ui2_descr_len > ui2_descr_list_len)
            {
                ui2_descr_len = ui2_descr_list_len;
            }

            if ((ui2_lto_len = (UINT16)(((ui2_descr_len - 2) / LTO_ENTRY_SIZE) * LTO_ENTRY_SIZE) ) > 0)
            {
                /* And copy the 'local_time_offset' entries. */
                x_memcpy (((VOID*) &(pt_lto_buff->pui1_buff [pt_lto_buff->ui2_data_len])),
                          ((VOID*) &(pui1_descr_list [2])),
                          ((SIZE_T) ui2_lto_len));

                pt_lto_buff->ui2_data_len += ui2_lto_len;
            }
        }

        /* And update the references and indexes. */
        pui1_descr_list += ui2_descr_len;

        if (ui2_descr_len <= ui2_descr_list_len)
        {
            ui2_descr_list_len -= ui2_descr_len;
        }
        else
        {
            ui2_descr_list_len = 0;
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: dt_dup_buff
 *
 * Description: This API duplicates a LTO buffer control object.
 *
 * Inputs:  pt_src_lto_buff    References the source LTO buffer control object.
 *          ppt_dest_lto_buff  Refernces the initial destination LTO buffer
 *                             control object.
 *
 * Outputs: ppt_dest_lto_buffer  References the duplicated LTO buffer control
 *                               object.
 *
 * Returns: TRUE if clients shall be notified of an update else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL dt_dup_buff (LTO_BUFF_CTRL_T*   pt_src_lto_buff,
                         LTO_BUFF_CTRL_T**  ppt_dest_lto_buff)
{
    BOOL             b_nfy;
    CRIT_STATE_T     t_state;
    LTO_BUFF_CTRL_T* pt_new_lto_buff;
    LTO_BUFF_CTRL_T* pt_old_lto_buff;


    b_nfy = FALSE;

    /* Allocate a new LTO buffer control object. */
    if ((pt_new_lto_buff = dt_alloc_buff_ctrl (pt_src_lto_buff->ui2_buff_len)) != NULL)
    {
        x_memcpy (((VOID*) pt_new_lto_buff->pui1_buff),
                  ((VOID*) pt_src_lto_buff->pui1_buff),
                  ((SIZE_T) pt_src_lto_buff->ui2_data_len));

        pt_new_lto_buff->ui2_data_len = pt_src_lto_buff->ui2_data_len;

        /* And now switch the old and new LTO buffer.  */
        /* This must be done inside a critical section */
        /* since the old LTO buffer may still be       */
        /* accessed by some other client.              */
        t_state = x_crit_start ();

        if ((pt_old_lto_buff = (*ppt_dest_lto_buff)) != NULL)
        {
            (*ppt_dest_lto_buff) = pt_new_lto_buff;

            if (pt_old_lto_buff->ui2_ref_cnt == 0)
            {
                /* Ok, no-one accesses this old LTO control */
                /* block. Simply free it.                   */
                x_crit_end (t_state);

                dt_free_buff_ctrl (pt_old_lto_buff);
            }
            else
            {
                /* Aha, this old buffer control block is */
                /* still accessed by some other client.  */
                /* Simply mark it as dead and the client */
                /* will clean it up.                     */
                pt_old_lto_buff->ui1_flags |= LTO_FLAG_DEAD_LTO_CTRL;

                x_crit_end (t_state);
            }

            /* Signal that clients shall be notified. */
            b_nfy = TRUE;
        }
        else
        {
            (*ppt_dest_lto_buff) = pt_new_lto_buff;

            x_crit_end (t_state);
            b_nfy = TRUE;
        }
    }

    return (b_nfy);
}


/*-----------------------------------------------------------------------------
 * Name: dt_find_lto_entry_ref
 *
 * Description: This API searches through the LTO buffer for the entry as
 *              specified by argument "ui2_idx". This function will also ensure
 *              that duplicate entries will be skipped.
 *
 * Inputs:  pt_lto_buff  References the LTO buffer control object.
 *          ui2_idx      Contains the LTO index as requested by the client.
 *
 * Outputs: -
 *
 * Returns: A reference to the found LTO entry or NULL.
 ----------------------------------------------------------------------------*/
static UINT8* dt_find_lto_entry_ref (LTO_BUFF_CTRL_T*  pt_lto_buff,
                                     UINT16            ui2_idx)
{
    BOOL   b_done;
    UINT8* pui1_curr_entry;
    UINT8* pui1_match_entry;
    UINT16 ui2_match_idx;


    b_done          = FALSE;
    pui1_curr_entry = GET_LTO_ENTRY_REF (pt_lto_buff, ui2_idx);

    while ((! (b_done))               &&
           (pui1_curr_entry != NULL))
    {
        ui2_match_idx = 0;
        pui1_match_entry = NULL;

        /* Check if there is a duplicate entry with the same */
        /* country code and region id.                       */
        while ((ui2_match_idx < ui2_idx)                                                             &&
               ((pui1_match_entry = GET_LTO_ENTRY_REF (pt_lto_buff, ui2_match_idx)) != NULL)   &&
               ((pui1_match_entry  [0]         !=  pui1_curr_entry [0])                      ||
                (pui1_match_entry  [1]         !=  pui1_curr_entry [1])                      ||
                (pui1_match_entry  [2]         !=  pui1_curr_entry [2])                      ||
                ((pui1_match_entry [3] & 0xfc) != (pui1_curr_entry [3] & 0xfc))))
        {
            ui2_match_idx++;
        }

        if ((pui1_match_entry == NULL)  ||
            (ui2_match_idx >= ui2_idx))
        {
            /* No duplicate entries detected. Terminate first loop. */
            b_done = TRUE;
        }
        else
        {
            /* Duplicate entries detected. Skip this one. */
            ui2_idx++;

            pui1_curr_entry = GET_LTO_ENTRY_REF (pt_lto_buff, ui2_idx);
        }
    }

    return (pui1_curr_entry);
}

/*-----------------------------------------------------------------------------
 * Name: dt_find_lto_entry_by_cntry_region
 *
 * Description: This API searches through the LTO buffer for the entry as
 *              specified by argument cntry and region id. If wild card is
 *              use, then it can matches any region id.
 *
 * Inputs:  pt_lto_buff  References the LTO buffer control object.
 *          ui2_idx      Contains the LTO index as requested by the client.
 *
 * Outputs: -
 *
 * Returns: A reference to the found LTO entry or NULL.
 ----------------------------------------------------------------------------*/
static UINT8* dt_find_lto_entry_by_cntry_region
(
    const UINT8*  pui1_descr_list,
    UINT16        ui2_descr_list_len,
    BOOL          b_wild_card_cntry,
    BOOL          b_wild_card_region
)
{
    UINT8*       pui1_lto_entry = NULL;
    UINT16       ui2_idx;
    UINT16       ui2_descr_len;
    BOOL         b_out_valid;
    
    b_out_valid = FALSE;
    
    /* And now, search for a matcing country code and region id. */
    while ((! (b_out_valid))                             &&
           (ui2_descr_list_len >= (LTO_ENTRY_SIZE + 2)))
    {
        ui2_descr_len = (UINT16)( ((UINT16) (pui1_descr_list [1])) + 2) ;

        if ((pui1_descr_list [0] == DESCR_TAG_DVB_LOC_TIME_OFFS) &&
            (ui2_descr_len >= (LTO_ENTRY_SIZE + 2)))
        {
            /* And now process this 'local_time_offset' */
            /* descriptor. However, only do that if a   */
            /* valid LTO entry is available.            */
            ui2_idx = 2;

            if ( (b_wild_card_cntry == FALSE)  &&  (b_wild_card_region == FALSE) )
            {
                
                while (((ui2_descr_len - ui2_idx) >= LTO_ENTRY_SIZE)                       &&
                       ((pui1_descr_list  [ui2_idx    ]         != ((UINT8)t_sys_count_code [0])) ||
                        (pui1_descr_list  [ui2_idx + 1]         != ((UINT8)t_sys_count_code [1])) ||
                        (pui1_descr_list  [ui2_idx + 2]         != ((UINT8)t_sys_count_code [2])) ||
                        ((pui1_descr_list [ui2_idx + 3] & 0xfc) != ui1_sys_region_id))
                    )
                {
                    /*
                    x_dbg_stmt("check exact match for cntry code [%s] region code [%d]\n",
                               t_sys_count_code, ui1_sys_region_id);
                    */
                    DBG_INFO(("check exact match for cntry code [%s] region code [%d]\n",t_sys_count_code, ui1_sys_region_id));
                    ui2_idx += LTO_ENTRY_SIZE;
                }
            }

            if ( (b_wild_card_cntry == FALSE)  &&  (b_wild_card_region == TRUE) )
            {
                /* x_dbg_stmt("check match for cntry code [%s] and any region code.\n", 
                               t_sys_count_code);
                */
                DBG_INFO(("check match for cntry code [%s] and any region code.\n",t_sys_count_code));
                
                while (((ui2_descr_len - ui2_idx) >= LTO_ENTRY_SIZE)                       &&
                       ((pui1_descr_list  [ui2_idx    ]         != ((UINT8)t_sys_count_code [0])) ||
                        (pui1_descr_list  [ui2_idx + 1]         != ((UINT8)t_sys_count_code [1])) ||
                        (pui1_descr_list  [ui2_idx + 2]         != ((UINT8)t_sys_count_code [2])))
                    )
                {
                    ui2_idx += LTO_ENTRY_SIZE;
                }
            }

            /* Match found. Process it and finish. */
            if ((ui2_descr_len - ui2_idx) >= LTO_ENTRY_SIZE)
            {
#ifdef DEBUG                
                ISO_3166_COUNT_T  t_count_code;
                UINT8             ui1_region_id;
#endif                
    
                b_out_valid = TRUE;

                /* save the offset location of LTO entry */
                pui1_lto_entry = (UINT8*) pui1_descr_list + ui2_idx ;

#ifdef DEBUG               
                t_count_code[0]=pui1_descr_list[ui2_idx];
                t_count_code[1]=pui1_descr_list[ui2_idx+1];
                t_count_code[2]=pui1_descr_list[ui2_idx+2];
                t_count_code[3]='\0';
                ui1_region_id=(pui1_descr_list[ui2_idx+3]&0xfc);

                DBG_INFO(("DVB: Use cntry code [%s] region code [%d] setting for local time offset.\n",t_count_code, ui1_region_id));
                
#endif
                
            }
        }

        /* And update the references and indexes. */
        pui1_descr_list += ui2_descr_len;

        if (ui2_descr_len <= ui2_descr_list_len)
        {
            ui2_descr_list_len -= ui2_descr_len;
        }
        else
        {
            ui2_descr_list_len = 0;
        }
    }
    return pui1_lto_entry;
}

/*-----------------------------------------------------------------------------
 * Name: dt_get_tz_offset
 *
 * Description: This API parses a list of descriptors and searches for the
 *              DVB 'local_time_offset' descriptor which matches the systems
 *              country coe and region id. If a match was found, this API will
 *              return TRUE else FALSE. All locking operations are performed
 *              using critical sections.
 *
 * Inputs:  pui1_descr_list     References the list of descriptors.
 *          ui2_descr_list_len  Contains the length of descriptors.
 *
 * Outputs: pt_tot  pointer to structure containing TOT descriptor information:
 *                  current local time offset, next local time offset,
 *                  time of transition.
 *
 * Returns: TRUE if the output values are valied else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL dt_get_tz_offset (const UINT8*  pui1_descr_list,
                              UINT16        ui2_descr_list_len,
                              DVB_TOT_T*    pt_tot)
{
    UINT8*       pui1_lto_entry = NULL;
    CHAR*        pc_src;
    CHAR*        pc_dest;
    DTG_T        t_dtg;
    CRIT_STATE_T t_state;
    TIME_T       t_loc_offset;
    TIME_T       t_next_loc_offset;
    BOOL         b_out_valid;
    UINT32       ui4_flag;

/* x_dbg_stmt("--------- Descriptor List Length:   [%d] ---------\n", ui2_descr_list_len); */

    b_out_valid = FALSE;
    
    /* If the client contry code and region id have been */
    /* updated, copy them to the system data area.       */
    t_state = x_crit_start ();

    if (b_sys_count_code_set)
    {
        /* Copy the country code and region id values. */
        pc_src  = &(t_client_count_code [0]);
        pc_dest = &(t_sys_count_code    [0]);

        while (((*(pc_dest++)) = (*(pc_src++))) != '\0');

        ui1_sys_region_id = ui1_client_region_id;

        b_sys_count_code_set = FALSE;
    }

    ui4_flag = ui4_config_flag;
    
    x_crit_end (t_state);

    /* And now, search for a matching country code and region id. */
    
    pui1_lto_entry = dt_find_lto_entry_by_cntry_region (
        pui1_descr_list, ui2_descr_list_len, FALSE, FALSE);

    if ( (pui1_lto_entry == NULL) &&
         (ui4_flag & DT_DVB_MATCH_CNTRY_CODE)
        )
    {
        /* try again, match any region id. */
        pui1_lto_entry = dt_find_lto_entry_by_cntry_region (
            pui1_descr_list, ui2_descr_list_len, FALSE, TRUE);

        if ( (pui1_lto_entry == NULL) &&
             (ui4_flag & DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE)
            )
        {
            /* try again, match any entry. */
            pui1_lto_entry = dt_find_lto_entry_by_cntry_region (
                pui1_descr_list, ui2_descr_list_len, TRUE, TRUE);
        }
    }
    
    if ( pui1_lto_entry != NULL )
    {
        b_out_valid = TRUE;
        
        t_loc_offset = ((((TIME_T) BCD_2_DECIMAL  (pui1_lto_entry [4])) * 3600) +
                        (((TIME_T) BCD_2_DECIMAL (pui1_lto_entry [5])) *   60));

        x_dt_mjd_bcd_to_dtg(&(pui1_lto_entry[6]), &t_dtg);
                
        t_next_loc_offset = ((((TIME_T) BCD_2_DECIMAL (pui1_lto_entry [11])) * 3600) +
                             (((TIME_T) BCD_2_DECIMAL (pui1_lto_entry [12])) *   60));

        if ((pui1_lto_entry [3] & 0x01) != 0)
        {
            t_loc_offset  = -t_loc_offset;
            t_next_loc_offset = -t_next_loc_offset;
        }

        /* And determine if daylight savings is active or not. */
        pt_tot->t_tm_of_change = x_dt_dtg_to_sec(&t_dtg);
#if 0        
        x_dbg_stmt("tm of change: %lld current offset %lld next offset %lld\n",
                   pt_tot->t_tm_of_change, t_loc_offset, t_next_loc_offset);
#endif
        DBG_INFO(("TOT infor:: tm of change: %lld(%llu) current offset %lld next offset %lld\n", 
            pt_tot->t_tm_of_change,pt_tot->t_this_utc, t_loc_offset, t_next_loc_offset));
        
        if ( pt_tot->t_tm_of_change > pt_tot->t_this_utc )
        {
            DBG_INFO(("DST transition is sometime in the future.\r\n"));
            /* DST transition is sometime in the future. */
            pt_tot->t_loc_tm_offset = t_loc_offset;
            pt_tot->t_next_tm_offset = t_next_loc_offset;
            pt_tot->b_dst = ( pt_tot->t_loc_tm_offset > pt_tot->t_next_tm_offset );
        }
        else
        {
            /*Sony*/
            if (b_config_lto_chg_time == TRUE)
            {
                DBG_INFO(("TOT infor:: tm of change: %lld  t_this_utc %lld ingore this LTO\n", pt_tot->t_tm_of_change, pt_tot->t_this_utc));
                b_out_valid = FALSE;
            }
            else
            {
                DBG_INFO(("DST transition has already occured.\r\n"));
                /* DST transition has already occured. */
                pt_tot->t_loc_tm_offset = t_next_loc_offset;
                pt_tot->t_next_tm_offset = t_loc_offset;
                pt_tot->b_dst = ( pt_tot->t_loc_tm_offset > pt_tot->t_next_tm_offset );
            }
        }

        /* deduce the TZ offset value */
        if ( pt_tot->b_dst == TRUE )
        {
            pt_tot->t_local_tz_offset = pt_tot->t_next_tm_offset;

            /* For Nordig certification, there is a test case where the TOT local time
               offset between current and future is greater than 1 hr.  Because in
               the current design for DVB, we don't record the current and future
               local time offset, instead we treat the local time offset as consist of
               offset due to timezone + offest due to Day-Light-Saving.  To emulate the
               DVB local time offset when its delta between current and future local time
               offset is greater than one hour, we compute the time offset due to
               Day-light-Saving.  
            */
            pt_tot->t_offset_for_dst = pt_tot->t_loc_tm_offset - pt_tot->t_next_tm_offset;
        }
        else
        {
            pt_tot->t_local_tz_offset = pt_tot->t_loc_tm_offset;

            pt_tot->t_offset_for_dst = pt_tot->t_next_tm_offset - pt_tot->t_loc_tm_offset ;
        }

        /* for Sony, when dst_ctrl is off, tz return raw data */
        if (b_config_lto_raw_tz_when_dst_ctrl_off && (x_dt_get_dst_ctrl() == FALSE))
        {
            pt_tot->t_local_tz_offset = pt_tot->t_loc_tm_offset;
        }
#if 0             
        x_dbg_stmt("[TOT] this [%lld] next [%lld] dst [%d] trans tm [%lld] curr_tm [%ldd] tz [%lld]\n",
                   pt_tot->t_loc_tm_offset,
                   pt_tot->t_next_tm_offset,
                   pt_tot->b_dst,
                   pt_tot->t_tm_of_change,
                   pt_tot->t_this_utc,
                   pt_tot->t_local_tz_offset);
#endif

        DBG_INFO(("[TOT] this [%lld] next [%lld] dst [%d] trans tm [%lld] curr_tm [%lld] tz [%lld]\n",
            pt_tot->t_loc_tm_offset,pt_tot->t_next_tm_offset,
            pt_tot->b_dst,pt_tot->t_tm_of_change,pt_tot->t_this_utc,
            pt_tot->t_local_tz_offset));
         
    }
        
    return (b_out_valid);
}

/*-----------------------------------------------------------------------------
 * Name: dt_dvb_lto_set_sys_count_code
 *
 * Description: This API sets the system country code plus region id. In case
 *              the country code string is greater than (ISO_3166_COUNT_LEN-1),
 *              only the first (ISO_3166_COUNT_LEN-1) characters are copied.
 *
 * Inputs:  t_count_code   Contains the country code.
 *          ui2_region_id  Contains the region id.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID dt_dvb_lto_set_sys_count_code (ISO_3166_COUNT_T  t_count_code,
                                    UINT16            ui2_region_id)
{
    CHAR*        pc_src;
    CHAR*        pc_dest;
    UINT16       ui2_idx;
    CRIT_STATE_T t_state;


    /* Copy the country code and region id values. */
    pc_src  = &(t_count_code        [0]);
    pc_dest = &(t_client_count_code [0]);
    ui2_idx = ISO_3166_COUNT_LEN - 1;

    t_state = x_crit_start ();

    while ((ui2_idx > 0)                               &&
           (((*(pc_dest++)) = (*(pc_src++))) != '\0'))
    {
        ui2_idx--;
    }

    (*pc_dest) = '\0';

    ui1_client_region_id = (UINT8) ((ui2_region_id << 2) & 0x00fc);

    b_sys_count_code_set = TRUE;

    x_crit_end (t_state);
}


/*-----------------------------------------------------------------------------
 * Name: dt_dvb_lto_get_sys_count_code
 *
 * Description: This API returns the system country code plus region id.
 *
 * Inputs:  -
 *
 * Outputs: pt_count_code   Contains the country code.
 *          pui2_region_id  Contains the region id.
 *
 * Returns: DTR_OK if successful else a negative error code.
 ----------------------------------------------------------------------------*/
INT32 dt_dvb_lto_get_sys_count_code (ISO_3166_COUNT_T*  pt_count_code,
                                     UINT16*            pui2_region_id)
{
    CHAR*        pc_src;
    CHAR*        pc_dest;
    INT32        i4_return;
    CRIT_STATE_T t_state;


    i4_return = DTR_INV_ARG;

    if ((pt_count_code  != NULL)  &&
        (pui2_region_id != NULL))
    {
        i4_return = DTR_OK;

        /* Copy the country code and region id values. */
        pc_src  = &(t_client_count_code [0]);
        pc_dest = &((*pt_count_code)    [0]);

        t_state = x_crit_start ();

        while (((*(pc_dest++)) = (*(pc_src++))) != '\0');

        (*pui2_region_id) = (UINT16) ((ui1_client_region_id >> 2) & 0x3f);

        x_crit_end (t_state);
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dt_dvb_lto_prc_lto_descrs
 *
 * Description: This API parses a list of descriptors and processes the DVB
 *              'local_time_offset' descriptors. All locking operations are
 *              performed using critical sections.
 *
 * Inputs:  pui1_descr_list     References the list of descriptors.
 *          ui2_descr_list_len  Contains the length of descriptors.
 *
 * Outputs: pt_tot  pointer to structure containing tot descriptor information.
 *
 * Returns: TRUE if the output values are valied else FALSE.
 ----------------------------------------------------------------------------*/
BOOL dt_dvb_lto_prc_lto_descrs (const UINT8*  pui1_descr_list,
                                UINT16        ui2_descr_list_len,
                                DVB_TOT_T*    pt_tot)
{
    BOOL             b_nfy;
    BOOL             b_out_valid;
    UINT16           ui2_lto_len;
    CRIT_STATE_T     t_state;
    LTO_BUFF_CTRL_T* pt_lto_buff;

    b_out_valid = FALSE;

    if ((pui1_descr_list != NULL) &&
        (ui2_descr_list_len > 0))
    {
        /* If no LTO buffer is allocated, do that first. */
        /* This will be only done once.                  */
        if (pt_sys_lto_buff == NULL)
        {
            pt_sys_lto_buff = dt_alloc_buff_ctrl (0);
        }

        if (pt_sys_lto_buff != NULL)
        {
            b_nfy = FALSE;

            if ((ui2_lto_len = dt_get_lto_len (pui1_descr_list, ui2_descr_list_len)) > 0)
            {
                /* Extend the actual buffer if required. */
                if (ui2_lto_len > pt_sys_lto_buff->ui2_buff_len)
                {
                    dt_extend_buff (pt_sys_lto_buff, ui2_lto_len);
                }

                if (ui2_lto_len <= pt_sys_lto_buff->ui2_buff_len)
                {
                    /* And now, write the 'local_time_ofset' entries */
                    /* to the allocated / extended buffer.           */
                    dt_write_buff (pui1_descr_list, ui2_descr_list_len, pt_sys_lto_buff);

                    /* Duplicate the buffer if the client one does not */
                    /* exist or if it contains different data.         */
                    if ((pt_client_lto_buff == NULL)                                        ||
                        (pt_client_lto_buff->ui2_data_len != pt_sys_lto_buff->ui2_data_len) ||
                        (x_memcmp (((VOID*) pt_client_lto_buff->pui1_buff),
                                   ((VOID*) pt_sys_lto_buff->pui1_buff),
                                   ((SIZE_T) pt_sys_lto_buff->ui2_data_len)) != 0))
                    {
                        b_nfy = dt_dup_buff (pt_sys_lto_buff, &pt_client_lto_buff);
                    }
                }

                /* And eventually, extract the time zone */
                /* from the descriptor list.             */
                b_out_valid = dt_get_tz_offset (pui1_descr_list,
                                                ui2_descr_list_len,
                                                pt_tot);
            }
            else
            {
                /* Only reset the actual data length in the buffer. */
                /* Note that the client's data length must be reset */
                /* in a critical section.                           */
                if (pt_sys_lto_buff->ui2_data_len > 0)
                {
                    pt_sys_lto_buff->ui2_data_len = 0;

                    t_state = x_crit_start ();

                    if ((pt_lto_buff = pt_client_lto_buff) != NULL)
                    {
                        pt_client_lto_buff = NULL;

                        if (pt_lto_buff->ui2_ref_cnt == 0)
                        {
                            /* Ok, no-one accesses the client LTO control */
                            /* block. Simply free it.                     */
                            x_crit_end (t_state);

                            dt_free_buff_ctrl (pt_lto_buff);
                        }
                        else
                        {
                            /* Aha, the client buffer control block is */
                            /* still accessed by some other client.    */
                            /* Simply mark it as dead and the client   */
                            /* will clean it up.                       */
                            pt_lto_buff->ui1_flags |= LTO_FLAG_DEAD_LTO_CTRL;

                            x_crit_end (t_state);
                        }

                        /* Signal that clients shall be notified. */
                        b_nfy = TRUE;
                    }
                    else
                    {
                        x_crit_end (t_state);
                    }
                }
            }

            if (b_nfy)
            {
                /* And notify clients of changes in the LTO entries. */
                dt_nfy_all_clients (DT_LTO_UPDATED, 0);
            }
        }
    }

    return (b_out_valid);
}


/*-----------------------------------------------------------------------------
 * Name: dt_dvb_lto_get_lto
 *
 * Description: This API returns an LTO entry from the client buffer. All
 *              locking operations are performed using critical sections.
 *
 * Inputs:  ui2_idx  Contains the index.
 *
 * Outputs: pt_count_code   Contains the country code.
 *          pui2_region_id  Contains the region id.
 *          pt_tz_offset    Contains the time offset.
 *
 * Returns: DTR_OK if successful else a negative error code.
 ----------------------------------------------------------------------------*/
INT32 dt_dvb_lto_get_lto (UINT16             ui2_idx,
                          ISO_3166_COUNT_T*  pt_count_code,
                          UINT16*            pui2_region_id,
                          TIME_T*            pt_tz_offset)
{
    BOOL             b_free;
    INT32            i4_return;
    UINT8*           pui1_lto_entry;
    CRIT_STATE_T     t_state;
    LTO_BUFF_CTRL_T* pt_lto_buff;


    i4_return = DTR_INV_ARG;

    /* Validate the arguments. */
    if ((pt_count_code  != NULL)  &&
        (pui2_region_id != NULL))
    {
        b_free    = FALSE; 
        
        i4_return = DTR_INV_IDX;

        /* And access the cient LTO buffer. */
        t_state = x_crit_start ();

        if ((pt_lto_buff = pt_client_lto_buff) != NULL)
        {
            pt_lto_buff->ui2_ref_cnt++;

            x_crit_end (t_state);

            if ((pui1_lto_entry = dt_find_lto_entry_ref (pt_lto_buff, ui2_idx)) != NULL)
            {
                i4_return = DTR_OK;

                /* Ok, an entry was found. Argument "pui1_lto_entry" */
                /* references the beginning of a LTO entry.          */
                (*pt_count_code) [0] = (CHAR)pui1_lto_entry [0];
                (*pt_count_code) [1] = (CHAR)pui1_lto_entry [1];
                (*pt_count_code) [2] = (CHAR)pui1_lto_entry [2];
                (*pt_count_code) [3] = '\0';

                (*pui2_region_id) = (((UINT16) pui1_lto_entry [3]) >> 2) & 0x003f;

                /* Return the time zone offset if the client specified */
                /* a return reference in argument "pt_tz_offset".      */
                if (pt_tz_offset != NULL)
                {
                    (*pt_tz_offset) = ((((TIME_T) BCD_2_DECIMAL (pui1_lto_entry [4])) * 3600) +
                                       (((TIME_T) BCD_2_DECIMAL (pui1_lto_entry [5])) *   60));

                    if ((pui1_lto_entry [3] & 0x01) != 0)
                    {
                        (*pt_tz_offset) = -(*pt_tz_offset);
                    }
                }
            }
            /* Ok, done with this operation. Decrement the */
            /* reference count and free the LTO buffer     */
            /* control block if so required.               */
            t_state = x_crit_start ();

            if (pt_lto_buff->ui2_ref_cnt > 0)
            {
                pt_lto_buff->ui2_ref_cnt--;
            }

            b_free = ((pt_lto_buff->ui2_ref_cnt == 0)                            &&
                      ((pt_lto_buff->ui1_flags & LTO_FLAG_DEAD_LTO_CTRL) != 0));
        }

        x_crit_end (t_state);

        if ((pt_lto_buff != NULL) &&
            (b_free))
        {
            /* Free the LTO buffer control block. However, do NOT   */
            /* set "pt_client_lto_buff" to NULL since that variable */
            /* is maintained when a TOT table is being processed.   */
            dt_free_buff_ctrl (pt_lto_buff);
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: dt_dvb_lto_get_num_lto_entries
 *
 * Description: This API returns the number of unique LTO entries in the client
 *              buffer. All locking operations are performed using critical
 *              sections.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The number of unique LTO entries.
 ----------------------------------------------------------------------------*/
UINT16 dt_dvb_lto_get_num_lto_entries (VOID)
{
    BOOL             b_free;
    UINT16           ui2_num_lto_entries;
    CRIT_STATE_T     t_state;
    LTO_BUFF_CTRL_T* pt_lto_buff;

    b_free = FALSE;
    ui2_num_lto_entries = 0;

    /* Access the cient LTO buffer. */
    t_state = x_crit_start ();

    if ((pt_lto_buff = pt_client_lto_buff) != NULL)
    {
        
        if ((pt_lto_buff->ui1_flags & LTO_FLAG_NUM_LTO_ENTRIES_VALID) == 0)
        {
            /* Hm, we still have to determine the number */
            /* of unique LTO entries.                    */
            pt_lto_buff->ui2_ref_cnt++;

            x_crit_end (t_state);


            while (dt_find_lto_entry_ref (pt_lto_buff, ui2_num_lto_entries) != NULL)
            {
                ui2_num_lto_entries++;
            }

            /* Save this number in the control block and */
            /* decrement the reference counter.          */
            t_state = x_crit_start ();

            pt_lto_buff->ui2_num_lto_entries  = ui2_num_lto_entries;
            pt_lto_buff->ui1_flags           |= LTO_FLAG_NUM_LTO_ENTRIES_VALID;

            if (pt_lto_buff->ui2_ref_cnt > 0)
            {
                pt_lto_buff->ui2_ref_cnt--;
            }
        }
        else
        {
            ui2_num_lto_entries = pt_lto_buff->ui2_num_lto_entries;
        }

        b_free = ((pt_lto_buff->ui2_ref_cnt == 0)                            &&
                  ((pt_lto_buff->ui1_flags & LTO_FLAG_DEAD_LTO_CTRL) != 0));
    }

    x_crit_end (t_state);

    if ((pt_lto_buff != NULL) &&
        (b_free))
    {
        /* Free the LTO buffer control block. However, do NOT   */
        /* set "pt_client_lto_buff" to NULL since that variable */
        /* is maintained when a TOT table is being processed.   */
        dt_free_buff_ctrl (pt_lto_buff);
    }

    return (ui2_num_lto_entries);
}

/*-----------------------------------------------------------------------------
 * Name: dt_dvb_set_config
 *
 * Description: This API returns the number of unique LTO entries in the client
 *              buffer. All locking operations are performed using critical
 *              sections.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The number of unique LTO entries.
 ----------------------------------------------------------------------------*/
INT32 dt_dvb_set_config (UINT32  ui4_flag)
{
    CRIT_STATE_T     t_state;
    UINT32           ui4_tmp_val = 0;

    if ( ui4_flag & DT_DVB_MATCH_CNTRY_AND_REG_CODE )
    {
        ui4_tmp_val = (UINT32)DT_DVB_MATCH_CNTRY_AND_REG_CODE;
    }

    if ( ui4_flag & DT_DVB_MATCH_CNTRY_CODE   )
    {
        ui4_tmp_val = (UINT32)(DT_DVB_MATCH_CNTRY_CODE | DT_DVB_MATCH_CNTRY_AND_REG_CODE);
    }

    if ( ui4_flag & DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE )
    {
        ui4_tmp_val = (UINT32) (DT_DVB_MATCH_CNTRY_AND_REG_CODE |
                                DT_DVB_MATCH_CNTRY_CODE    |
                                DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE );
    }

    if ( ui4_tmp_val != 0 )
    {
        t_state = x_crit_start ();

        ui4_config_flag = ui4_tmp_val;
        
        x_crit_end (t_state);
    }

    if ( ui4_flag & DT_DVB_LTO_CHECK_CHG_TIME )
    {
        t_state = x_crit_start ();
        b_config_lto_chg_time = TRUE;
        x_crit_end (t_state);
    }

    if ( ui4_flag & DT_DVB_LTO_RAW_TZ_WHEN_DST_CTRL_OFF )
    {
        t_state = x_crit_start ();
        b_config_lto_raw_tz_when_dst_ctrl_off = TRUE;
        x_crit_end (t_state);
    }
    
    return DTR_OK;
}

