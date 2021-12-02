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
/*----------------------------------------------------------------------------*/
/*! @file c_txt.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all txt library API and constant for 
 *         other module using.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_TXT
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _X_TXT_H_
#define _X_TXT_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_txt.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* This structure enumerates all event types for the callback function (*x_img_nfy_fct). */
#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------*/

/*! @brief  c_txt_create 
 *              Create txt instance handle.
 *  @param [in] &ph_txt 
 *  @param [out] Txt instance handle
 *  
 *  @Return Return the error code.
 *  @retval TXT_OK      Success. The API execution is successful. 
 *  @retval TXT_ERR     FAIL
 */
/*------------------------------------------------------------------*/


extern INT32 x_txt_open (const CHAR*         buffer,
                            const UINT32        ui4_buffer_size,
                            UINT8               ui1_open_mode,  
                            c_txt_nfy_fct       pf_nfy,         
                            const VOID*         pv_tag,         
                            HANDLE_T*           ph_txt,
                            TXT_SRC_TYPE_T      e_type);


/*! @brief  c_txt_free 
 *              Create txt instance handle.
 *  @param [in] ph_txt 
 *  @param [out] Free txt instance.
 *  
 *  @Return Return the error code.
 *  @retval TXT_OK      Success. The API execution is successful. 
 *  @retval TXT_ERR     FAIL
 */
/*------------------------------------------------------------------*/
extern INT32 x_txt_close(HANDLE_T h_txt);


extern INT32 x_txt_set_attr(HANDLE_T h_txt,
                              UINT32 ui4_cmd_num,
                              TXT_SET_CMD_T* pt_cmd);

/*! @brief  c_txt_get_code
 *              get txt code   
 *  @param [in] h_txt   Current text instance handle. 
 *  @param [out] null
 *  @return Return txt code.
 *  @retval TXT_CODE_UTF8                    
 *  @retval TXT_CODE_UTF8BE    
 *  @retval TXT_CODE_UNICODE 
 *  @retval TXT_CODE_UNICODEBE 
 *  @retval TXT_CODE_UTF32 
 *  @retval TXT_CODE_UTF32BE 
 *  @retval UNKNOWN        Not known the txt code.
 */
/*------------------------------------------------------------------*/

extern INT32 x_txt_get_code_type(void*                 pv_buffer, 
                               UINT32                ui4_buffer_length, 
                               TXT_SRC_TYPE_T        e_src_type, 
                               TXT_ECODE_TYPE_T*      pe_code_type);


extern INT32 x_txt_get_code(HANDLE_T h_txt, 
                            TXT_ECODE_TYPE_T* pe_code_type);


/*! @brief  c_txt_scan
 *              Scan txt file,get txt total page and save each page info.         
 *  @param [in] h_txt   Current text instance handle. 
 *  @param [in] ui4_scan_cnt       scan page count.if the value is 0xffffffff,scan all file.
 *  @param [out] scaned page number(call back).
 *
 *  @Return void.
 */
/*------------------------------------------------------------------*/
extern INT32 x_txt_scan(HANDLE_T h_txt, UINT32 ui4_scan_cnt,
                  c_txt_nfy_fct pf_nfy,const VOID* pv_tag);


/*! @brief  c_txt_cancel_scan 
 *  @param [in] h_txt   Current text instance handle. 
 *  @param [out] null
 *
 *  @Return void
 */
/*------------------------------------------------------------------*/
extern INT32 x_txt_cancel_scan(HANDLE_T h_txt);

/*! @brief  c_txt_get_page_data
 *            (1)Get special page data form page list,and load data to row list.
 *            (2)Async method
 *  @param [in] h_txt   Current text instance handle. 
 *  @param [in] ui4_goto_num    Go to page number.
 *  @param [in] pf_nfy  callback func
 *  @param [out] (1)Page data (2)Call back to AP
 *
 *  @Return void.
 */
/*------------------------------------------------------------------*/
extern INT32 x_txt_get_page_data( HANDLE_T h_txt,UINT32 ui4_goto_num,
                          c_txt_nfy_fct pf_nfy,const VOID* pv_tag );


/*! @brief  c_txt_scroll_up_row
 *            Get next row data(compare current displaying page). 
 *  @param [in] h_txt   Current text instance handle. 
 *  @param [out] Next row data
 *
 *  @Return Return the error code.
 *  @retval TXT_OK      Success. The API execution is successful. 
 *  @retval TXT_ERR     FAIL
 */
/*------------------------------------------------------------------*/
extern INT32 x_txt_scroll_up_row(HANDLE_T h_txt, UINT16 ui2_scroll_num);

/*! @brief  c_txt_scroll_down_row
 *            Get previous row data(compare current displaying page). 
 *  @param [in] h_txt   Current text instance handle. 
 *  @param [out] Previous row data
 *
 *  @Return Return the error code.
 *  @retval TXT_OK      Success. The API execution is successful. 
 *  @retval TXT_ERR     FAIL
 */
/*------------------------------------------------------------------*/
extern INT32 x_txt_scroll_down_row(HANDLE_T h_txt, UINT16 ui2_scroll_num);


extern INT32 x_txt_is_support_ecode(TXT_ECODE_TYPE_T e_code_type);

extern INT32 x_txt_get_file_size(HANDLE_T h_txt, UINT32* pui4_file_size);

extern INT32 x_txt_get_page_rows(HANDLE_T h_txt, UINT16* pui2_rows);

extern INT32 x_txt_get_last_page_rows(HANDLE_T h_txt, UINT16* pui2_rows);

extern INT32 x_txt_pause(HANDLE_T h_txt);

extern INT32 x_txt_resume(HANDLE_T h_txt);

extern INT32 x_txt_style_change(HANDLE_T h_txt, 
                              UINT32* pui4_current_display_page, 
                              UINT32*  pui4_total_page);
extern INT32 x_txt_get_start_line(HANDLE_T h_txt, UINT32* pui4_page_start_line_idx);
    
#ifdef __cplusplus
}
#endif

#endif /* _C_TXT_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_TXT*/
/*----------------------------------------------------------------------------*/
