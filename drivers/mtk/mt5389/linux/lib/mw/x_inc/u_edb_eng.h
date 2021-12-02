
/*******************************************************************************
 * LEGAL DISCLAIMER
 *    
 * (Header of MediaTek Software/Firmware Release or Documentation)
 *    
 * BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
 * AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK 
 * SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO
 * BUYER ON AN "AS-IS" BASISONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL 
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH 
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO
 * LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES 
 * MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM.
 * 
 * BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE 
 * LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, 
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO 
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 * 
 * THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
 * THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS 
 * PRINCIPLES.
 ******************************************************************************/
/*------------------------------------------------------------------------------
 * Copyright (c) 2009, Mediatek Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/*! @file u_edb_eng.h 
 * $RCSfile: ,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $

 *
 *  @par Description:
 *         This contains Event database library macros, definitons, structures,
 *         and API's, which are shared with other modules, and customized engines.
 *---------------------------------------------------------------------------*/

#ifndef _U_EDB_ENG_H_
#define _U_EDB_ENG_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_EDB
 *  @{
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/
#include "u_edb.h"

/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @brief  edb create object function

 *  @param  [in] h_edb_eng              - handle of edb engine
 *  @param  [in] pt_edb_cmd            - edb command
 *  @param  [out] ppv_eng_obj_data  - pointer of edb engine obj data
 *  @param  [out] pe_cond               - pointer of edb engine condition
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_create_fct) (
        HANDLE_T            h_edb_eng,
        EDB_COMMAND_T*      pt_edb_cmd,
        VOID**              ppv_eng_obj_data,
        EDB_COND_T*         pe_cond);

/*----------------------------------------------------------------------------*/
/*! @brief  edb delete object function

 *  @param  [in] pv_eng_obj_data  - engine object to delete
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_delete_fct) (
        VOID*               pv_eng_obj_data);

/*----------------------------------------------------------------------------*/
/*! @brief  edb get object information length function

 *  @param  [in] h_edb_eng                 - handle of edb engine
 *  @param  [in] e_key_type                - event type to query
 *  @param  [in] pv_key_info               - pointer event key information
 *  @param  [out] pz_event_info_len     - pointer of event info length
 *  @param  [out] pe_cond                  - pointer of edb condition
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_get_event_info_len_fct) (
        HANDLE_T            h_edb_eng,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        EDB_COND_T*         pe_cond);

/*----------------------------------------------------------------------------*/
/*! @brief  edb get object information function

 *  @param  [in] h_edb_eng                - handle of edb engine
 *  @param  [in] e_key_type               - event type to query
 *  @param  [in] pv_key_info              - pointer event key information
 *  @param  [in] pz_event_info_len      - pointer of event info length
 *  @param  [out] pz_event_info_len     - pointer of event info length
 *  @param  [out] pv_event_info         - pointer of event info information
 *  @param  [out] pe_cond               - pointer of edb engine condition
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_get_event_info_fct) (
        HANDLE_T            h_edb_eng,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info,
        EDB_COND_T*       pe_cond);

/*----------------------------------------------------------------------------*/
/*! @brief  edb task function
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_task_fct) ( VOID );

/*----------------------------------------------------------------------------*/
/*! @brief  edb set function

 *  @param  [in] pt_set_cmd         - pointer to the set command
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_set_fct) (
        HANDLE_T            h_edb_eng,
        EDB_COMMAND_T*      pt_set_cmd);

/*----------------------------------------------------------------------------*/
/*! @brief  edb get function

 *  @param  [in] pt_get_cmd         - pointer to the get command
 *  
 *  @note   EDB Engine should implement such function pointer and register to edb
 *  @see
 *  @return		INT32
 *  @retval		EDBR_OK           - Success
 */
/*----------------------------------------------------------------------------*/
typedef INT32 (*x_edb_eng_get_fct) (
        HANDLE_T            h_edb_eng,
        EDB_COMMAND_T*      pt_get_cmd);


/*------------------------------------------------------------------*/
/*! @struct EDB_ENG_FCT_TBL_T
 *  @brief  Structure for the EDB engine function table

 *  @code
 * typedef struct _EDB_ENG_FCT_TBL_T
 * {
 *     x_edb_create_event_obj_fct        pf_create_event_obj;
 *     x_edb_delete_event_obj_fct        pf_delete_event_obj;
 *     x_edb_get_event_obj_info_len_fct  pf_get_event_obj_info_len;
 *     x_edb_get_event_obj_info_fct      pf_get_event_obj_info;
 *     x_edb_engine_task_fct             pf_engine_task;
 *     x_edb_set_fct                     pf_set;
 *     x_edb_get_fct                     pf_get;
 * } EDB_ENG_FCT_TBL_T;
 *  @endcode

 *  @li@c   pf_create_event_obj         - edb create fucntion pointer
 *  @li@c   pf_delete_event_obj         - edb delete fucntion pointer     
 *  @li@c   pf_get_event_obj_info_len   - edb get info length fucntion pointer
 *  @li@c   pf_get_event_obj_info       - edb get info fucntion pointer
 *  @li@c   pf_engine_task              - edb task fucntion pointer
 *  @li@c   pf_set                      - edb set fucntion pointer                  
 *  @li@c   pf_get                      - edb get fucntion pointer                  
 */
/*------------------------------------------------------------------*/
/* Structure for the EDB engine function table*/
typedef struct _EDB_ENG_FCT_TBL_T
{
    x_edb_eng_create_fct                   pf_create_eng_obj;
    x_edb_eng_delete_fct                   pf_delete_eng_obj;
    x_edb_eng_get_event_info_len_fct       pf_get_event_info_len;
    x_edb_eng_get_event_info_fct           pf_get_event_info;
    x_edb_eng_task_fct                     pf_engine_task;
    x_edb_eng_set_fct                      pf_set;
    x_edb_eng_get_fct                      pf_get;
} EDB_ENG_FCT_TBL_T;

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_EDB */
/*----------------------------------------------------------------------------*/


#endif /* _U_EDB_ENG_H_ */
