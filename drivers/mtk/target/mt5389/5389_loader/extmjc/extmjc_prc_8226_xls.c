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
 * $RCSfile: extmjc_prc_8226_xls.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         This file contains all implementations for 8205 translator
 *
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC
#ifdef DRV_SUPPORT_EXTMJC_UART_PROTOCOL

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "extmjc_debug.h"
#include "extmjc_prc_8226_xls.h"
#include "extmjc_prc.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define EXTMSG_SHORT_MIN_SIZE               ((UINT16) 16)
#define EXTMSG_LONG_MIN_SIZE                ((UINT16) 24)
#define EXTMSG_ONE_UNIT_MAX_SIZE            ((UINT16) 4)
#define EXTMSG_TAIL_MAX_SIZE                ((UINT16) 6)
#define EXTMSG_TAIL_MIN_SIZE                ((UINT16) 4)

#define EXTMSG_START_SEQ_SIZE               ((UINT16) 8)

#define EXTMSG_CNTR_FIELD_IDX               ((UINT32) 0)
#define EXTMSG_TYPE_FIELD_IDX               ((UINT32) 1)

#define EXTMSG_LENGTH_FIELD_IDX             ((UINT32) 2)

#define EXTMSG_SHORT_BODY_FIELD_IDX         ((UINT32) 2)
#define EXTMSG_LONG_BODY_FIELD_IDX          ((UINT32) 4)

#define EXTMSG_RELIABLE_MASK                ((UINT8) 0x80)
#define EXTMSG_GET_MSG_ID(ui1_cntr)         ((ui1_cntr) & 0x07)
#define EXTMSG_GET_FREE_MSG_ID(ui1_cntr)    ( ((ui1_cntr) & 0x38) >> 3)

#define EXTMSG_COMPOSE_CTRL_FIELD(ui1_inmsg_id, ui1_inmsg_free_id) \
    ( ((ui1_inmsg_id) & 0x07) + (((ui1_inmsg_free_id) & 0x07) << 3) )


#define PREAMBLE_BYTE                       ((UINT8) 0x50)

#define SOM1_BYTE                           ((UINT8) 0xC8)
#define SOM2_BYTE                           ((UINT8) 0xC9)
#define EOM_BYTE                            ((UINT8) 0xCA)
#define ESC_BYTE                            ((UINT8) 0xCB)

#define RESERVED_FIRST_BYTE                 ((UINT8) 0xC8)
#define RESERVED_LAST_BYTE                  ((UINT8) 0xCB)

#define LONG_MSG_FIRST_MSG_TYPE             ((UINT8) 0xD0)
#define LONG_MSG_LAST_MSG_TYPE              ((UINT8) 0xFE)

#define RESP_MSG_TYPE                       ((UINT8) 0xFF)

#define CALC_CHK_SUM(ui1_chk_sum, ui1_data) ((ui1_chk_sum) + (ui1_data))

/*
#define EXTMSG_APPEND_SINGLE_BYTE(pui1_msg, ui1_data) \
    *(pui1_msg) = PREAMBLE_BYTE; \
    *((pui1_msg) + 1) = ui1_data; \
    pui1_msg += 2
*/  
  
#define EXTMSG_APPEND_SINGLE_BYTE(pui1_msg, ui1_data) \
    *(pui1_msg) = ui1_data; \
    pui1_msg ++

#define EXTMSG_APPEND(pui1_msg, ui1_data, ui1_chk_sum) \
    if ( ((ui1_data) >= RESERVED_FIRST_BYTE) &&  \
         ((ui1_data) <= RESERVED_LAST_BYTE) )   \
    { \
        (ui1_chk_sum) = CALC_CHK_SUM(ui1_chk_sum, ESC_BYTE); \
        EXTMSG_APPEND_SINGLE_BYTE(pui1_msg, ESC_BYTE); \
        ui1_data ^= 0xFF; \
    } \
    (ui1_chk_sum) = CALC_CHK_SUM(ui1_chk_sum, ui1_data); \
    EXTMSG_APPEND_SINGLE_BYTE(pui1_msg, ui1_data)

#define EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE         ((UINT8) 0)
#define EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE          ((UINT8) 1)
#define EXTMJC_PRC_8226_XLS_COMPOSE_ESC_STATE          ((UINT8) 2)
#define EXTMJC_PRC_8226_XLS_COMPOSE_BODY_STATE         ((UINT8) 3)
#define EXTMJC_PRC_8226_XLS_COMPOSE_EOM_STATE          ((UINT8) 4)
#define EXTMJC_PRC_8226_XLS_COMPOSE_CCESC_STATE        ((UINT8) 5)
#define EXTMJC_PRC_8226_XLS_COMPOSE_END_STATE          ((UINT8) 6)

#define EXTMJC_PRC_8226_XLS_SHORT_MSG_SIZE             ((UINT32) 52)
#define EXTMJC_PRC_8226_XLS_BASIC_BUF_SIZE             ((UINT32) 56)
#define EXTMJC_PRC_8226_XLS_LONG_MSG_SIZE              ((UINT32) 65539)

#define LONG_MSG_BODY_MAX_SIZE                  ((UINT32) 65535)
#define SHORT_MSG_BODY_MAX_SIZE                 ((UINT32) 50)

typedef struct _EXTMJC_PRC_8226_XLS_T
{
    UINT8   ui1_state;

    UINT8   ui1_cc;

    BOOL    b_short_msg;
    
    UINT8   ui1_received_cc;

    UINT32  ui4_buf_idx;

    UINT32  ui4_buf_max_size;
    
    UINT8   *pui1_extmsg_buf;
} EXTMJC_PRC_8226_XLS_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/ 
static VOID extmjc_prc_8226_xls_extmsg_reset(VOID);

static BOOL extmjc_prc_8226_xls_compose_extmsg(UINT8  ui1_data);


static INT32 extmjc_prc_8226_xls_extmsg_to_inmsg(
    BOOL*   pb_reliable_inmsg,
    BOOL*   pb_short_resp,
    UINT8*  pui1_inmsg_id,
    UINT8*  pui1_inmsg_free_id,
    UINT8*  pui1_inmsg_type,
    UINT8*  pui1_inmsg_body,
    UINT16  ui2_inmsg_body_max_len,
    UINT16* pui2_inmsg_body_len);

static INT32 extmjc_prc_8226_xls_inmsg_to_extmsg(
    BOOL    b_reliable_inmsg,
    BOOL    b_short_resp,
    UINT8   ui1_inmsg_id,
    UINT8   ui1_inmsg_free_id,
    UINT8   ui1_inmsg_type,
    UINT16  ui2_inmsg_body_len,
    UINT8*  pui1_inmsg_body,
    UINT8*  pui1_extmsg,
    UINT32  ui4_extmsg_max_len,
    UINT32* pui4_extmsg_len);

static INT32 extmjc_prc_8226_xls_valid_inmsg_fct(
    BOOL                        b_reliable_inmsg,
    EXTMJC_PRC_XLS_VALID_MSG_TYPE  t_msg_type,
    UINT8                       ui1_inmsg_type,
    UINT16                      ui2_inmsg_body_len,
    UINT8*                      pui1_inmsg_body);

static void host_to_MSB16(
    UINT16 ui2_host,
    UINT8  aui1_MSBData[]);

static UINT16 MSB16_to_host(UINT8  aui1_MSBData[]);

static VOID extmjc_prc_8226_xls_compose_som_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data);

static VOID extmjc_prc_8226_xls_compose_esc_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data);

static VOID extmjc_prc_8226_xls_compose_body_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data);

static VOID extmjc_prc_8226_xls_compose_eom_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data);

static VOID extmjc_prc_8226_xls_compose_ccesc_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data);


const EXTMJC_PRC_XLS_T t_extmjc_prc_8226_xls_intf =
{
    extmjc_prc_8226_xls_extmsg_reset,
    extmjc_prc_8226_xls_compose_extmsg,
    extmjc_prc_8226_xls_extmsg_to_inmsg,
    extmjc_prc_8226_xls_inmsg_to_extmsg,
    extmjc_prc_8226_xls_valid_inmsg_fct
};

EXTMJC_PRC_8226_XLS_T t_extmjc_prc_8226_xls;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_init 
 *
 * Description: Initialize 8205 translator.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
BOOL extmjc_prc_8226_xls_init(VOID)
{
    t_extmjc_prc_8226_xls.ui1_cc = 0;
    t_extmjc_prc_8226_xls.ui1_received_cc = 0;
    t_extmjc_prc_8226_xls.ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE;
    t_extmjc_prc_8226_xls.b_short_msg = TRUE;

    t_extmjc_prc_8226_xls.ui4_buf_idx = 0;
    
    t_extmjc_prc_8226_xls.pui1_extmsg_buf = x_mem_alloc(EXTMJC_PRC_8226_XLS_SHORT_MSG_SIZE);
    if (NULL == t_extmjc_prc_8226_xls.pui1_extmsg_buf)
    {
        return (FALSE);
    }
    
    t_extmjc_prc_8226_xls.ui4_buf_max_size = EXTMJC_PRC_8226_XLS_SHORT_MSG_SIZE;

    if (EXTMJC_PRCR_OK != extmjc_prc_reg_xls(&t_extmjc_prc_8226_xls_intf))
    {
        LOG(1, "ERR: extmjc_prc_8226_xls_init: Exit FALSE\n");
        return (FALSE);
    }

    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: host_to_MSB16 
 *
 * Description: Translate 2 byte data from host to MSB format.
 *
 * Inputs:  ui2_host        2 byte data in host format.
 *
 * Outputs: aui1_MSBData    2 byte array in MSB format.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static void host_to_MSB16(
    UINT16 ui2_host,
    UINT8  aui1_MSBData[])
{
    aui1_MSBData[0] = (UINT8) (ui2_host >> 8);
    aui1_MSBData[1] = (UINT8) (ui2_host & 0x00FF);
}

/*-----------------------------------------------------------------------------
 * Name: MSB16_to_host 
 *
 * Description: Translate 2 byte data form MSB to host format.
 *
 * Inputs:  aui1_MSBData    2 byte array in MSB format
 *
 * Outputs: -
 *
 * Returns: 2 byte data in host format.
 ----------------------------------------------------------------------------*/
static UINT16 MSB16_to_host(UINT8  aui1_MSBData[])
{
    UINT16 ui2_host;
    ui2_host = (UINT16) aui1_MSBData[0] << 8;
    ui2_host += (UINT16) aui1_MSBData[1];

    return (ui2_host);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_extmsg_reset 
 *
 * Description: Reset the 8205 compose state machine.
 *
 * Inputs:  - 
 *
 * Outputs: - 
 *
 * Returns: - 
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_8226_xls_extmsg_reset(VOID)
{
    t_extmjc_prc_8226_xls.ui1_cc = 0;
    t_extmjc_prc_8226_xls.ui1_received_cc = 0;
    t_extmjc_prc_8226_xls.ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE;
    t_extmjc_prc_8226_xls.b_short_msg = TRUE;

    t_extmjc_prc_8226_xls.ui4_buf_idx = 0;
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_verify_length 
 *
 * Description: Reset the 8205 compose state machine.
 *
 * Inputs:  - Pointer points to a 8205 translator
 *
 * Outputs: - 
 *
 * Returns: - If the input message is larger than the maximum size of message, 
 *            FALSE is returned. Otherwise, TRUE is returned.
 ----------------------------------------------------------------------------*/
static BOOL extmjc_prc_8226_xls_compose_verify_length(
    EXTMJC_PRC_8226_XLS_T* pt_this)
{
    UINT8*          pui1_new_buf;

    /* Verify Size */
    if (TRUE == pt_this->b_short_msg)
    {
        /* Short message, */
        if (EXTMJC_PRC_8226_XLS_SHORT_MSG_SIZE <= pt_this->ui4_buf_idx)
        {
            extmjc_prc_8226_xls_extmsg_reset();
            return (FALSE);
        }
    }
    else
    {
        /* Long message*/
        if (EXTMJC_PRC_8226_XLS_LONG_MSG_SIZE <= pt_this->ui4_buf_idx)
        {
            extmjc_prc_8226_xls_extmsg_reset();
            return (FALSE);
        }
        
        /* resize */
        if (pt_this->ui4_buf_idx == pt_this->ui4_buf_max_size)
        {

            pui1_new_buf = x_mem_alloc(
                pt_this->ui4_buf_max_size + EXTMJC_PRC_8226_XLS_SHORT_MSG_SIZE);

            if (NULL == pui1_new_buf)
            {
                extmjc_prc_8226_xls_extmsg_reset();
                return (FALSE);
            }
            else
            {
                x_memcpy(pui1_new_buf, pt_this->pui1_extmsg_buf, pt_this->ui4_buf_idx);
                x_mem_free(pt_this->pui1_extmsg_buf);
                pt_this->pui1_extmsg_buf = pui1_new_buf;
                pt_this->ui4_buf_max_size = 
                    pt_this->ui4_buf_max_size + EXTMJC_PRC_8226_XLS_SHORT_MSG_SIZE;
            }
            
        }
    }

    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_som_state 
 *
 * Description: Compose function as the state machine is in SOM state.
 *
 * Inputs:  pt_this     Pointer points to 8205 translator
 *          ui1_data    Input character
 *
 * Outputs: - 
 *
 * Returns: - 
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_8226_xls_compose_som_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data)
{
    switch (ui1_data)
    {
        case SOM1_BYTE:
            pt_this->b_short_msg = TRUE;
            break;

        case SOM2_BYTE:
            pt_this->b_short_msg = FALSE;
            break;

        case ESC_BYTE:
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_ESC_STATE;
            pt_this->ui1_cc = ESC_BYTE;
            break;

        case EOM_BYTE:
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE;
            break;

        default: /* other characters */
            pt_this->ui1_cc = ui1_data;
            pt_this->pui1_extmsg_buf[0] = ui1_data;
            pt_this->ui4_buf_idx = 1;

            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_BODY_STATE;
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_esc_state 
 *
 * Description: Compose function as the state machine is in ESC state.
 *
 * Inputs:  pt_this     Pointer points to 8205 translator
 *          ui1_data    Input character
 *
 * Outputs: - 
 *
 * Returns: - 
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_8226_xls_compose_esc_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data)
{
    if ( (ui1_data >= RESERVED_FIRST_BYTE) ||
         (ui1_data <= RESERVED_LAST_BYTE) )
    {
        pt_this->ui1_cc += ui1_data;
        ui1_data ^= 0xFF;
        pt_this->pui1_extmsg_buf[pt_this->ui4_buf_idx] = ui1_data;
        pt_this->ui4_buf_idx ++;
        pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_BODY_STATE;
    }
    else
    {
        pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE;
        pt_this->ui1_cc = 0;
        pt_this->ui4_buf_idx = 0;
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_body_state 
 *
 * Description: Compose function as the state machine is in BODY state.
 *
 * Inputs:  pt_this     Pointer points to 8205 translator
 *          ui1_data    Input character
 *
 * Outputs: - 
 *
 * Returns: - 
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_8226_xls_compose_body_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data)
{
    switch (ui1_data)
    {
        case SOM1_BYTE:
            pt_this->b_short_msg = TRUE;
            pt_this->ui1_cc = 0;
            pt_this->ui4_buf_idx = 0;
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE;
            break;

        case SOM2_BYTE:
            pt_this->b_short_msg = FALSE;
            pt_this->ui1_cc = 0;
            pt_this->ui4_buf_idx = 0;
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE;
            break;
        
        case ESC_BYTE:
            /* To ESC state */
            if (TRUE == extmjc_prc_8226_xls_compose_verify_length(pt_this))
            {
                pt_this->ui1_cc += ESC_BYTE;
                pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_ESC_STATE;
            }
            break;

        case EOM_BYTE:
            /* To EOM state */
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_EOM_STATE;
            break;

        default: 
            /* other characters, stay at the same state */
            if (TRUE == extmjc_prc_8226_xls_compose_verify_length(pt_this))
            {
                pt_this->ui1_cc += ui1_data;
                pt_this->pui1_extmsg_buf[pt_this->ui4_buf_idx] = ui1_data;
                pt_this->ui4_buf_idx ++;
            }
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_eom_state 
 *
 * Description: Compose function as the state machine is in EOM state.
 *
 * Inputs:  pt_this     Pointer points to 8205 translator
 *          ui1_data    Input character
 *
 * Outputs: - 
 *
 * Returns: - 
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_8226_xls_compose_eom_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data)
{
    switch (ui1_data)
    {
        case SOM1_BYTE:
        case SOM2_BYTE:
        case EOM_BYTE:
            /* unexpected character, to idle state */
            pt_this->ui1_cc = 0;
            pt_this->ui4_buf_idx = 0;
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE;
            break;

        case ESC_BYTE:
            /* To ESC CC state */
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_CCESC_STATE;
            break;

        default: 
            /* other characters, to end state */
            pt_this->ui1_received_cc = ui1_data;
            pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_END_STATE;
            break;
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_ccesc_state 
 *
 * Description: Compose function as the state machine is in CCESC state.
 *
 * Inputs:  pt_this     Pointer points to 8205 translator
 *          ui1_data    Input character
 *
 * Outputs: - 
 *
 * Returns: - 
 ----------------------------------------------------------------------------*/
static VOID extmjc_prc_8226_xls_compose_ccesc_state(
    EXTMJC_PRC_8226_XLS_T* pt_this,
    UINT8           ui1_data)
{
    if ( (ui1_data >= RESERVED_FIRST_BYTE) ||
         (ui1_data <= RESERVED_LAST_BYTE) )
    {
        ui1_data ^= 0xFF;
        pt_this->ui1_received_cc = ui1_data;
        pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_END_STATE;
    }
    else
    {
        
        pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE;
        pt_this->ui1_cc = 0;
        pt_this->ui4_buf_idx = 0;
    }
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_compose_extmsg 
 *
 * Description: This callback function is called by ATV Manager to compose 
 *              external messages. 
 *
 * Inputs:  ui1_data    Data of external messages
 *
 * Outputs: - 
 *
 * Returns: FALSE    The state machine is composing a external message.
 *          TRUE     The state machine finds a complete external message.                                    
 ----------------------------------------------------------------------------*/
static BOOL extmjc_prc_8226_xls_compose_extmsg(UINT8 ui1_data)
{
    EXTMJC_PRC_8226_XLS_T* pt_this;
    UINT16          ui2_inmsg_len;
    UINT8           ui1_msg_type;
    
    pt_this = &t_extmjc_prc_8226_xls;
    

    switch ( pt_this->ui1_state )
    {
        case EXTMJC_PRC_8226_XLS_COMPOSE_IDLE_STATE:
            if (SOM1_BYTE == ui1_data)
            {
                pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE;
                pt_this->ui1_cc = 0;
                pt_this->ui4_buf_idx = 0;
                pt_this->b_short_msg = TRUE;
            }

            if (SOM2_BYTE == ui1_data)
            {
                pt_this->ui1_state = EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE;
                pt_this->ui1_cc = 0;
                pt_this->ui4_buf_idx = 0;
                pt_this->b_short_msg = FALSE;
            }
            break;
        
        case EXTMJC_PRC_8226_XLS_COMPOSE_SOM_STATE:
            extmjc_prc_8226_xls_compose_som_state(pt_this, ui1_data);
            break;
        
        case EXTMJC_PRC_8226_XLS_COMPOSE_ESC_STATE:
            extmjc_prc_8226_xls_compose_esc_state(pt_this, ui1_data);
            break;
        
        case EXTMJC_PRC_8226_XLS_COMPOSE_BODY_STATE:
            extmjc_prc_8226_xls_compose_body_state(pt_this, ui1_data);
            break;
        
        case EXTMJC_PRC_8226_XLS_COMPOSE_EOM_STATE:
            extmjc_prc_8226_xls_compose_eom_state(pt_this, ui1_data);
            break;
        
        case EXTMJC_PRC_8226_XLS_COMPOSE_CCESC_STATE:
            extmjc_prc_8226_xls_compose_ccesc_state(pt_this, ui1_data);
            break;
        
        case EXTMJC_PRC_8226_XLS_COMPOSE_END_STATE:
        default:
            LOG(1, "ERR: Translator enter invalid state\n");
            
            break;
    }

    if (EXTMJC_PRC_8226_XLS_COMPOSE_END_STATE == pt_this->ui1_state)
    {
        /* Verify checksum */
        if (pt_this->ui1_cc != pt_this->ui1_received_cc)
        {
            extmjc_prc_8226_xls_extmsg_reset();
            return (FALSE);
        }

        /* long message verify length */
        if (FALSE == pt_this->b_short_msg)
        {
            /* Get length field */
            ui2_inmsg_len = MSB16_to_host( 
                (pt_this->pui1_extmsg_buf + EXTMSG_LENGTH_FIELD_IDX) );

            /* Wrong length field */
            if (ui2_inmsg_len != pt_this->ui4_buf_idx - EXTMSG_LONG_BODY_FIELD_IDX)
            {
                extmjc_prc_8226_xls_extmsg_reset();
                return (FALSE);
            }
        }

        /* Verify long / short message type */
        ui1_msg_type = pt_this->pui1_extmsg_buf[EXTMSG_TYPE_FIELD_IDX];
        if (TRUE == pt_this->b_short_msg)
        {
            if ( (ui1_msg_type >= LONG_MSG_FIRST_MSG_TYPE) &&
                 (ui1_msg_type <= LONG_MSG_LAST_MSG_TYPE) )
            {
                extmjc_prc_8226_xls_extmsg_reset();
                return (FALSE);
            }
        }
        else /* long message */
        {
            if (ui1_msg_type < LONG_MSG_FIRST_MSG_TYPE)
            {
                extmjc_prc_8226_xls_extmsg_reset();
                return (FALSE);
            }
        }

        return (TRUE);
    }
    
    return (FALSE);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_extmsg_to_inmsg 
 *
 * Description: This callback function is called by ATV Manager to translate 
 *              external messages to internal messages.
 *
 * Inputs:  ui2_inmsg_max_len   The size of the data part of internal message 
 *                              buffer.
 *
 * Outputs: 
 *          pb_reliable_inmsg   Contains the reliable/unreliable information 
 *                              of the message.
 *          pui1_inmsg_id       Contains the id field of the message.
 *          pui1_inmsg_freeid   Contains the free id field of the message.
 *          pui1_inmsg_type     Contains the message type field of the message.
 *          pui1_inmsg_body     Pointer points to the body part of internal 
 *                              message buffer.
 *          pui2_inmsg_body_len If return value is EXTMJC_PRC_XLS_OK or EXTMJC_PRC_XLS_UNKNOWN_MSG, 
 *                              this field contains the length of the internal message. 
 *                              If return value is EXTMJC_PRC_XLS_INSUFFICIENT_BUF, this field 
 *                              contains the required memory size of internal message. 
 *                              If return value is EXTMJC_PRC_XLS_XLS_IN_WRONG_STATE, this field is useless.
 *
 * Returns: EXTMJC_PRC_XLS_OK                  Routine was successful
 *          EXTMJC_PRC_XLS_INSUFFICIENT_BUF    The size of intermal message buffer 
 *                                      is insufficient.
 *          EXTMJC_PRC_XLS_UNKNOWN_MSG         Unknown internal message       
 *          EXTMJC_PRC_XLS_XLS_IN_WRONG_STATE  Call the translation function as not in
 *                                      EXTMJC_PRC_XLS_COMPOSE_END_STATE state.
 ----------------------------------------------------------------------------*/
static INT32 extmjc_prc_8226_xls_extmsg_to_inmsg(
    BOOL*   pb_reliable_inmsg,
    BOOL*   pb_short_resp,
    UINT8*  pui1_inmsg_id,
    UINT8*  pui1_inmsg_free_id,
    UINT8*  pui1_inmsg_type,
    UINT8*  pui1_inmsg_body,
    UINT16  ui2_inmsg_body_max_len,
    UINT16* pui2_inmsg_body_len)
{
    UINT16          ui2_inmsg_body_len;
    UINT8           ui1_cntr;
    EXTMJC_PRC_8226_XLS_T* pt_this;
    
    pt_this = &t_extmjc_prc_8226_xls;
    
    /* Verify state */
    if (EXTMJC_PRC_8226_XLS_COMPOSE_END_STATE != pt_this->ui1_state)
    {
        LOG(1, "ERR: In wrong state\n");
        return (EXTMJC_PRC_XLS_XLS_IN_WRONG_STATE);
    }

    /* Get message id, free message id, reliable, b_short_resp */
    ui1_cntr = pt_this->pui1_extmsg_buf[EXTMSG_CNTR_FIELD_IDX];
    if (0 == (ui1_cntr & EXTMSG_RELIABLE_MASK))
    {
        *pb_reliable_inmsg = TRUE;
    }
    else
    {
        *pb_reliable_inmsg = FALSE;
    }

    *pui1_inmsg_id = EXTMSG_GET_MSG_ID(ui1_cntr);
    *pui1_inmsg_free_id = EXTMSG_GET_FREE_MSG_ID(ui1_cntr);
    *pb_short_resp = pt_this->b_short_msg;

    /* Get message type field */
    *pui1_inmsg_type = pt_this->pui1_extmsg_buf[EXTMSG_TYPE_FIELD_IDX];

    if (TRUE == pt_this->b_short_msg)
    {
        ui2_inmsg_body_len = pt_this->ui4_buf_idx - EXTMSG_SHORT_BODY_FIELD_IDX;
        *pui2_inmsg_body_len = ui2_inmsg_body_len;

        if (ui2_inmsg_body_len > ui2_inmsg_body_max_len)
        {
            return (EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
        }

        x_memcpy(pui1_inmsg_body, 
                 pt_this->pui1_extmsg_buf + EXTMSG_SHORT_BODY_FIELD_IDX, 
                 ui2_inmsg_body_len);
    }
    else
    {
        ui2_inmsg_body_len = MSB16_to_host( 
            (pt_this->pui1_extmsg_buf + EXTMSG_LENGTH_FIELD_IDX) );

        *pui2_inmsg_body_len = ui2_inmsg_body_len;
        
        if (ui2_inmsg_body_len > ui2_inmsg_body_max_len)
        {
            return (EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
        }

        x_memcpy(pui1_inmsg_body, 
                 pt_this->pui1_extmsg_buf + EXTMSG_LONG_BODY_FIELD_IDX, 
                 ui2_inmsg_body_len);

        *pui2_inmsg_body_len = ui2_inmsg_body_len;
    }
    
    return (EXTMJC_PRC_XLS_OK);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_inmsg_to_extmsg 
 *
 * Description: This callback function is called by ATV Manager to translate 
 *              internal messages to external messages.
 *
 * Inputs:  b_reliable_inmsg    Whether a reliable message
 *          ui1_inmsg_id        ID field of the message
 *          ui1_inmsg_free_id   Free id field of the message
 *          ui1_inmsg_type      Type of internal message
 *          ui2_inmsg_body_len  Length of the internal message.
 *          pui1_inmsg_body     Pointer points to the body part of internal message.
 *          ui2_extmsg_max_len  The maximum size of external message buffer
 *
 * Outputs: pui1_extmsg         Pointer points to the buffer of external messages.
 *          pui2_extmsg_len     If return value is EXTMJC_PRC_XLS_OK or EXTMJC_PRC_XLS_UNKNOWN_MSG, 
 *                              this field contains the length of the external message. 
 *                              If return value is EXTMJC_PRC_XLS_INSUFFICIENT_BUF, this field 
 *                              contains the required memory size of external message. 
 *                              If return value is EXTMJC_PRC_XLS_WRONG_BODY_LEN, this field is useless.
 *
 * Returns: EXTMJC_PRC_XLS_OK                  Routine was successful
 *          EXTMJC_PRC_XLS_INSUFFICIENT_BUF    The size of intermal message buffer 
 *                                      is insufficient.
 *          EXTMJC_PRC_XLS_UNKNOWN_MSG         Unknown internal message       
 *          EXTMJC_PRC_XLS_WRONG_BODY_LEN      The length of internal message is wrong.
 ----------------------------------------------------------------------------*/
static INT32 extmjc_prc_8226_xls_inmsg_to_extmsg(
    BOOL    b_reliable_inmsg,
    BOOL    b_short_resp,
    UINT8   ui1_inmsg_id,
    UINT8   ui1_inmsg_free_id,
    UINT8   ui1_inmsg_type,
    UINT16  ui2_inmsg_body_len,
    UINT8*  pui1_inmsg_body,
    UINT8*  pui1_extmsg,
    UINT32  ui4_extmsg_max_len,
    UINT32* pui4_extmsg_len)
{
    UINT8 ui1_data = 0;
    UINT8 ui1_chk_sum = 0;
    UINT8 *pui1_extmsg_cursor;
    UINT8 *pui1_extmsg_end;
    UINT8 au8_len[2];
    UINT8 ui1_ctrl;
    BOOL  b_short_msg;

    if (RESP_MSG_TYPE == ui1_inmsg_type)
    {
        b_short_msg = b_short_resp;
    }
    else
    {
    if ( (ui1_inmsg_type >= LONG_MSG_FIRST_MSG_TYPE) &&
         (ui1_inmsg_type <= LONG_MSG_LAST_MSG_TYPE) )
    {
        b_short_msg = FALSE;
    }
        else
        {
            b_short_msg = TRUE;
        }
    }

    /* predict a very long length */
    *pui4_extmsg_len = ui2_inmsg_body_len * 4 + EXTMSG_LONG_MIN_SIZE;

    pui1_extmsg_cursor = pui1_extmsg;
    pui1_extmsg_end = pui1_extmsg + ui4_extmsg_max_len;

    /* Verify ext buf size */
    if (TRUE == b_short_msg)
    {
        if (1 != ui1_inmsg_type)
        {
            if (EXTMSG_SHORT_MIN_SIZE > ui4_extmsg_max_len)
            {
                return(EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
            }
        }
        else
        {
            /* connect message */
            if (EXTMSG_SHORT_MIN_SIZE + EXTMSG_START_SEQ_SIZE > ui4_extmsg_max_len)
            {
                return(EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
            }
        }
    }
    else
    {
        if (EXTMSG_LONG_MIN_SIZE > ui4_extmsg_max_len)
        {
            return(EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
        }
    }

    /* connected message append start sequence */
    if ((1 == ui1_inmsg_type) || (0xFF == ui1_inmsg_type))
    {
#if 1
        pui1_extmsg_cursor[0] = 0x54;
        pui1_extmsg_cursor[1] = 0x8A;
        pui1_extmsg_cursor[2] = 0x51;
        pui1_extmsg_cursor[3] = 0x37;
        pui1_extmsg_cursor[4] = 0x56;
        pui1_extmsg_cursor[5] = 0x92;
        pui1_extmsg_cursor[6] = 0x1C;
        pui1_extmsg_cursor[7] = 0xB2;
#endif
        pui1_extmsg_cursor = pui1_extmsg_cursor + EXTMSG_START_SEQ_SIZE;
    }

    /* append start of message */
    if (TRUE == b_short_msg)
    {
        EXTMSG_APPEND_SINGLE_BYTE(pui1_extmsg_cursor, SOM1_BYTE);
    }
    else
    {
        EXTMSG_APPEND_SINGLE_BYTE(pui1_extmsg_cursor, SOM2_BYTE);
    }

    /* append control byte */
    ui1_ctrl = EXTMSG_COMPOSE_CTRL_FIELD(ui1_inmsg_id, ui1_inmsg_free_id);
    if (FALSE == b_reliable_inmsg)
    {
        ui1_ctrl += EXTMSG_RELIABLE_MASK;
    }
    EXTMSG_APPEND(pui1_extmsg_cursor, ui1_ctrl, ui1_chk_sum);

    /* append message type */
    EXTMSG_APPEND(pui1_extmsg_cursor, ui1_inmsg_type, ui1_chk_sum);
    
    /* long message append length */
    if (FALSE == b_short_msg)
    {
        host_to_MSB16(ui2_inmsg_body_len, au8_len);
        EXTMSG_APPEND(pui1_extmsg_cursor, au8_len[0], ui1_chk_sum);
        EXTMSG_APPEND(pui1_extmsg_cursor, au8_len[1], ui1_chk_sum);
    }

    /* append data field */
    while ( ui2_inmsg_body_len > 0 )
    {
        /* Check remain length */
        if ( (pui1_extmsg_end - pui1_extmsg_cursor) 
             < EXTMSG_ONE_UNIT_MAX_SIZE )
        {
            return(EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
        }

        ui1_data = *(pui1_inmsg_body);

        EXTMSG_APPEND(pui1_extmsg_cursor, ui1_data, ui1_chk_sum);

        pui1_inmsg_body++;
        ui2_inmsg_body_len--;
    }

    /* append end of message */
    if ( ((ui1_chk_sum) >= RESERVED_FIRST_BYTE) && 
         ((ui1_chk_sum) <= RESERVED_LAST_BYTE) )
    {
        if ( (pui1_extmsg_end - pui1_extmsg_cursor) 
             < EXTMSG_TAIL_MAX_SIZE )
        {
            return(EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
        }

        EXTMSG_APPEND_SINGLE_BYTE(pui1_extmsg_cursor, EOM_BYTE);

        EXTMSG_APPEND_SINGLE_BYTE(pui1_extmsg_cursor, ESC_BYTE);

        ui1_chk_sum ^= 0xFF;
    } 
    else
    {
        if ( (pui1_extmsg_end - pui1_extmsg_cursor) 
             < EXTMSG_TAIL_MIN_SIZE )
        {
            return(EXTMJC_PRC_XLS_INSUFFICIENT_BUF);
        }

        EXTMSG_APPEND_SINGLE_BYTE(pui1_extmsg_cursor, EOM_BYTE);
    }

    EXTMSG_APPEND_SINGLE_BYTE(pui1_extmsg_cursor, ui1_chk_sum);

    *pui4_extmsg_len = (UINT32) (pui1_extmsg_cursor - pui1_extmsg);

    return(EXTMJC_PRC_XLS_OK);
}

/*-----------------------------------------------------------------------------
 * Name: extmjc_prc_8226_xls_valid_inmsg_fct 
 *
 * Description: This callback function is called by ATV Manager to validate 
 *              internal messages.
 *
 * Inputs:  b_reliable_inmsg    Whether a reliable message
 *          ui1_inmsg_type      Type of internal message
 *          ui2_inmsg_body_len  Length of the internal message.
 *          pui1_inmsg_body     Pointer points to the body part of internal message.
 *
 *
 * Returns: EXTMJC_PRC_XLS_OK                  Routine was successful
 *          EXTMJC_PRC_XLS_UNKNOWN_MSG         Unknown internal message       
 *          EXTMJC_PRC_XLS_WRONG_BODY_LEN      The length of internal message is wrong.
 ----------------------------------------------------------------------------*/
static INT32 extmjc_prc_8226_xls_valid_inmsg_fct(
    BOOL                    b_reliable_inmsg,
    EXTMJC_PRC_XLS_VALID_MSG_TYPE  t_msg_type,
    UINT8                   ui1_inmsg_type,
    UINT16                  ui2_inmsg_body_len,
    UINT8*                  pui1_inmsg_body)
{
    if (REQ_MSG == t_msg_type)
    {
        if (ui1_inmsg_type < LONG_MSG_FIRST_MSG_TYPE)
        {
            if (SHORT_MSG_BODY_MAX_SIZE < ui2_inmsg_body_len)
            {
                return (EXTMJC_PRC_XLS_WRONG_BODY_LEN);
            }
        }
    }

    if (SHORT_RESP_MSG == t_msg_type)
    {
        if (SHORT_MSG_BODY_MAX_SIZE < ui2_inmsg_body_len)
        {
            return (EXTMJC_PRC_XLS_WRONG_BODY_LEN);
        }
    }

    return (EXTMJC_PRC_XLS_OK);
}

#endif /* DRV_SUPPORT_EXTMJC_UART_PROTOCOL */
#endif /* DRV_SUPPORT_EXTMJC */

