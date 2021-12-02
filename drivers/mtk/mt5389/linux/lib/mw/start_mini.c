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
 * $RCSfile: start_mini.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/2 $
 * $SWAuthor: Justin Deng $
 * $MD5HEX: 832f63f52fd8e67aec2460928c6b58f3 $
 *
 * Description:
 *         This file contains all Middleware startup implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "dbg.h"
#include "handle.h"
#include "x_common.h"
#include "os.h"
#include "x_start_mini.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Abort stuff. */
#ifdef ABORT
#undef ABORT
#endif

#define ABORT(_cat, _val) dbg_abort ((DBG_MOD_START | _cat | _val))

#define DBG_ABRT_RTOS_INIT_FAILURE      ((UINT32) 0x00000000)
#define DBG_ABRT_INTERNAL_INIT_FAILURE  ((UINT32) 0x00000001)


extern INT32 _thread_init(VOID);
extern INT32 _timer_init(VOID);
extern INT32 msg_q_init(VOID);
extern INT32 ev_grp_init(VOID);

// Ugly... We really shouldn't do it here......
// OSAI driver_os init should be done by OSAI itself...
extern void HANDLE_Init(void);
extern void OSDRV_Init(void);


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static BOOL b_init = FALSE;  /* Keeps track of the initialization status. */


/*----------------------------------------------------------------------------
 * Function: os_init()
 *
 * Description:
 *      This routine is called as part of system start up. it creates the
 *      default memory partition. it should be called once. it performs
 *      1. check arguments.
 *      2. create mem global struct.
 *      3. create default memory partition.
 *      4. call init routine of each OS module.
 *
 * Inputs:
 *      pv_addr: pointer to a memory block
 *      z_size: size of the memory block
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_INV_ARG: invalid argument
 *      OSR_NO_RESOURCE: no resource (memory).
 *      OSR_EXIST: this routine was called previously.
 *---------------------------------------------------------------------------*/
INT32
os_init(const VOID *pv_addr, SIZE_T z_size)
{
    INT32 i4_i = OSR_OK;
    static BOOL fgInited = FALSE;

    /* this routine shall only be called once */
    /* to create the default memory partition */
    if (!fgInited)
    {
        /* now initialize other modules */
        if (_thread_init() != OSR_OK)
        {
            i4_i = OSR_NO_RESOURCE;
        }
        if (_timer_init() != OSR_OK)
        {
            i4_i = OSR_NO_RESOURCE;
        }
        if (msg_q_init() != OSR_OK)
        {
            i4_i = OSR_NO_RESOURCE;
        }
        if (ev_grp_init() != OSR_OK)
        {
            i4_i = OSR_NO_RESOURCE;
        }
        if (i4_i == OSR_OK)
        {
            fgInited = TRUE;
        }

        return(i4_i);
    }
    else
    {
        /* default memory partition established */
        /* i.e. os_init() called before */
        return(OSR_EXIST);
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_start_mini
 *
 * Description: This API is used as minimal Middleware initialization.
 *              Essentially it will only initialize the handle and OS library.
 *
 * Inputs:  pt_rtos_config   References the generic RTOS configuration structure.
 *          ui2_num_handles  Contains the number of handles to be allocated.
 *          pv_mem_addr      Contains the first memory addres to be used by
 *                           the system.
 *          z_mem_size       Contains the size of the memory referenced by
 *                           "pv_mem_addr".
 *
 * Outputs: -
 *
 * Returns: INITR_OK            Routine successful.
 *          INITR_ALREDAY_INIT  Alreday initialized.
 ----------------------------------------------------------------------------*/
INT32 x_start_mini (GEN_CONFIG_T*  pt_rtos_config,
                    UINT16         ui2_num_handles,
                    VOID*          pv_mem_addr,
                    SIZE_T         z_mem_size)
{
    INT32 i4_return;

    i4_return = INITR_ALREADY_INIT;

    if (! (b_init))
    {
        OSDRV_Init();

        HANDLE_Init();

        if (os_init (pv_mem_addr, z_mem_size) != OSR_OK)
        {
            ABORT (DBG_CAT_INIT, DBG_ABRT_INTERNAL_INIT_FAILURE);
        }

        b_init = TRUE;
        i4_return = INITR_OK;
    }

    return (i4_return);
}

