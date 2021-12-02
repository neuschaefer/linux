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
 * $RCSfile: x_evctx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/1 $
 * $SWAuthor: Justin Deng $
 * $MD5HEX: 494e45120de18cd8749a98c6245c6c57 $
 *
 * Description:
 *         This contains Event Context library macros, definitons, structures,
 *         and API's, which are exported to middleware modules.
 *---------------------------------------------------------------------------*/

#ifndef _X_EVCTX_H_
#define _X_EVCTX_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_evctx.h"

#ifdef  EVCTX_LEAKAGE_TEST

#include "x_lkg_tst.h"

#else /* EVCTX_LEAKAGE_TEST */

#define x_lt_mem_alloc(z_size)          x_mem_alloc(z_size)
#define x_lt_mem_free(pv_mem_block)     x_mem_free(pv_mem_block)

#endif  /* EVCTX_LEAKAGE_TEST */

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_EVCTX
 *  @{
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief  load current event function

 *  @param  [in] pt_svc_cmd          - evctx command
 *  @param  [in] pv_nfy_tag          - notify tag
 *  @param  [in] pf_nfy              - notify function
 *  @param  [out] ph_event           - pointer of event object handle
 *  @param  [out] pe_cond            - pointer of event condition
 *  
 *  @note   
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/ 
extern INT32 x_evctx_load_current_event (
                    EVCTX_COMMAND_T*    pt_svc_cmd,
                    VOID*               pv_nfy_tag,
                    x_evctx_nfy_fct     pf_nfy,
                    HANDLE_T*           ph_event,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  load next event function

 *  @param  [in] pt_svc_cmd          - evctx command
 *  @param  [in] pv_nfy_tag          - notify tag
 *  @param  [in] pf_nfy              - notify function
 *  @param  [out] ph_event           - pointer of event object handle
 *  @param  [out] pe_cond            - pointer of event condition
 *  
 *  @note   
 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_load_next_event (
                    EVCTX_COMMAND_T*    pt_svc_cmd,
                    VOID*               pv_nfy_tag,
                    x_evctx_nfy_fct     pf_nfy,
                    HANDLE_T*           ph_event,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  load schedule event function

 *  @param  [in] pt_svc_cmd          	- evctx command
 *  @param  [in] pt_offset           		- evctx offset
 *  @param  [in] pv_nfy_tag          	- notify tag
 *  @param  [in] pf_nfy              		- notify function
 *  @param  [out] ph_event           	- pointer of event object handle
 *  @param  [out] pe_cond            	- pointer of event condition
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_load_schedule_event (
                    EVCTX_COMMAND_T*    pt_svc_cmd,
                    EVCTX_OFFSET_T*     pt_offset,
                    VOID*               pv_nfy_tag,
                    x_evctx_nfy_fct     pf_nfy,
                    HANDLE_T*           ph_event,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  delete event function

 *  @param  [in] h_event          - handle of evctx object
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_delete_event (
                    HANDLE_T            h_event);


/*----------------------------------------------------------------------------*/
/*! @brief  get event condition function

 *  @param  [in] h_event                - handle of evctx object
 *  @param  [out] pe_cond               - pointer of event condition
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_get_cond (
                    HANDLE_T            h_event,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  get event info length function

 *  @param  [in] h_event                - handle of evctx object
 *  @param  [in] e_key_type             - event key type
 *  @param  [in] pv_key_info            - event key information
 *  @param  [out] pz_event_info_len     - pointer of event information length
 *  @param  [out] pe_cond               - pointer of event condition
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_get_event_info_len (
                    HANDLE_T            h_event,
                    EVCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_info,
                    SIZE_T*             pz_event_info_len,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  get event info function

 *  @param  [in] h_event                - handle of evctx object
 *  @param  [in] e_key_type             - event key type
 *  @param  [in] pv_key_info            - event key information
 *  @param  [out] pz_event_info_len     - pointer of event information length
 *  @param  [out] pv_event_info         - pointer of event information
 *  @param  [out] pe_cond               - pointer of event condition
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_get_event_info (
                    HANDLE_T            h_event,
                    EVCTX_KEY_TYPE_T    e_key_type,
                    VOID*               pv_key_info,
                    SIZE_T*             pz_event_info_len,
                    VOID*               pv_event_info,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  get event brief info length function

 *  @param  [in] h_event                - handle of evctx object
 *  @param  [in] pt_iso_639_lang        - pointer of iso 639 language structure
 *  @param  [out] pz_brief_info_len     - pointer of event information length
 *  @param  [out] pe_cond               - pointer of event condition
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_get_brief_info_len (
                    HANDLE_T            h_event,
                    ISO_639_LANG_T*     pt_iso_639_lang,
                    SIZE_T*             pz_brief_info_len,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief  get event brief info function

 *  @param  [in] h_event                - handle of evctx object
 *  @param  [in] pt_iso_639_lang        - pointer of iso 639 language structure
 *  @param  [in] pt_iso_639_lang        - pointer of iso 639 language structure
 *  @param  [out] pz_brief_info_len     - pointer of event information length
 *  @param  [out] pv_brief_info         - pointer of brief event information
 *  @param  [out] pe_cond               - pointer of event condition
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_get_brief_info (
                    HANDLE_T            h_event,
                    ISO_639_LANG_T*     pt_iso_639_lang,
                    SIZE_T*             pz_brief_info_len,
                    VOID*               pv_brief_info,
                    EVCTX_COND_T*       pe_cond);


/*----------------------------------------------------------------------------*/
/*! @brief Init evctx 
 
 *  
 *  @note   

 *  @see
 *  @return		BOOL
 *  @retval		TRUE   - Success
 */
/*----------------------------------------------------------------------------*/

extern BOOL x_evctx_is_inited (VOID);


/*----------------------------------------------------------------------------*/
/*! @brief  set evctx country code function

 *  @param  [in] e_brdcst_type          - broadcast type
 *  @param  [in] pt_country_code        - pointer of country code
 *  @param  [in] ui2_region_id          - region id
 *  
 *  @note   

 *  @see
 *  @return		INT32
 *  @retval		EVCTXR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_evctx_set_country_code (
                    BRDCST_TYPE_T       e_brdcst_type,
                    ISO_3166_COUNT_T*   pt_country_code,
                    UINT16              ui2_region_id);

#endif /* _X_EVCTX_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_EVCTX */
/*----------------------------------------------------------------------------*/

