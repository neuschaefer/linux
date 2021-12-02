/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2006, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: ,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/


#ifndef _X_EDB_H_
#define _X_EDB_H_

#include "u_sys_name.h"
#include "u_edb.h"
#include "u_edb_eng.h"










/*----------------------------------------------------------------------------*/
/*! @brief The name of edb, which is to identify a unique event database. (e.g.  "EDB_MAIN") 
 * 
 *  @param  [in]   ps_edb_name   - The name of edb, which is to identify a unique event database. (e.g. "EDB_MAIN") 
 *  @param  [in]   pt_cmd        - A command list. To create an event database, APP has to give corresponding parameters with this command list.
 *  @param  [in]   pv_nfy_tag    - Notify tag
 *  @param  [in]   pf_nfy        - Notify fucntion
 *  @param  [out]  ph_edb_client - Contains the handle of edb
 *  
 *  @note   
 *  @see
 *  @return     INT32
 *  @retval     EDBR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_edb_create (
        const CHAR*          ps_edb_name,
        EDB_COMMAND_T*       pt_cmd,
        VOID*                pv_nfy_tag,
        x_edb_nfy_fct        pf_nfy,
        HANDLE_T*            ph_edb_client);

extern INT32 x_edb_destroy (HANDLE_T    h_edb_client);


/*----------------------------------------------------------------------------*/
/*! @brief 
 * 
 *  @param  [in]   h_edb_client        - The handle of cache
 *  @param  [in]   pv_nfy_tag   - the pointer of tag will be used when edb selector invokes x_edb_nfy_fct
 *  @param  [in]   pf_nfy       - (x_edb_nfy_fct(h_edb_client, e_cond, pv_tag, ui4_data1, ui4_data2)) Callback to notify app some kinds of condition change
 *  
 *  @note   
 *  @see
 *  @return     INT32
 *  @retval     EDBR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_edb_set_listener (
        HANDLE_T             h_edb_client,
        VOID*                pv_nfy_tag,
        x_edb_nfy_fct        pf_nfy);


/*----------------------------------------------------------------------------*/
/*! @brief 
 * 
 *  @param  [in]   h_edb_client        - The handle of cache
 *  @param  [in]   pt_set_cmd   - A set command to adjust the configuration of cache
 *                                 EDB_CMD_PF_ONLY:         (BOOL) to ask to load and cache present/following events only (default: false)
 *                                 EDB_CMD_ACTUAL_ONLY:     (BOOL) to ask to load and cache events from EIT actual only (default: false)
 *                                 EDB_CMD_MAX_DAYS:        (UINT16)to restrict cache days. For EU models, the days of EIT loading will be also restricted. (default: 8 days)
 *                                 EDB_CMD_PREF_LANG:       (EDB_PREF_LANG_T*) to provide preference language (default: (1, eng)
 *                                 EDB_CMD_COUNTRY_CODE:    (CHAR*) to provide country code for mapping correct character set (default:GBR
 *                                 EDB_CMD_ENABLE:          (BOOL) to stop/restart the event cache.
 *                                 EDB_CMD_ACTIVE_WIN:      (EDB_CACHE_ACTIVE_WIN_T*)  to setup a active 2-D window, in which the events won¡¯t be deleted
 *                                 EDB_CMD_LOCK:            (BOOL) lock/unlock the cache (read lock) to prevent an update during a series of get actions. 
 *                                                          Must unlock after get actions are done.
 *                                 EDB_CMD_EVENT_MIN_SECS:  (UINT32) to specify the minimum duration of an event. 
 *                                                           Only the event with duration larger than this value can be retrieved.
 *                                 EDB_CMD_FAKE_EVENT_INSERTION_ENABLE: (BOOL) to enable/disable the function of fake event insertion.
 *                                 EDB_CMD_FAKE_EVENT_MIN_SECS:        	(UINT32) to specify the minimum duration of a fake event. 
 *                                                                       If the empty gap larger than this value, insert a fake event into the gap.
 *                                 EDB_CMD_TIME_CONFLICT_ALLOW:        	(BOOL) to enable/disable the allowance of time conflict between events.
 *                                 EDB_CMD_EVENT_DETAIL_SEPARATOR:      (CHAR*) to specify a string as a separator between short detail and extended detail text.
 *                                 EDB_CMD_CURRENT_SERVICE:        		(EDB_CH_ID_T*) to inform currently selected service.
 *                                 EDB_CMD_RESTART:                     when SVL update use this command delete old channel request and load new channel data. 
 *                                 EDB_CMD_END:                          to indicate the end of command list.
 *  
 *  @note                          Suggest the set commands  be called in ACFG
 *  @see
 *  @return     INT32
 *  @retval     EDBR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_edb_set (
        HANDLE_T            h_edb_client,
        EDB_COMMAND_T*      pt_set_cmd);

/*----------------------------------------------------------------------------*/
/*! @brief 
 * 
 *  @param  [in]   h_edb_client        - The handle of cache
 *  @param  [in]   e_key_typ    - The specific key type:
 *                                 EDB_KEY_TYPE_EVENT_DETAIL_BY_ID - get length of specific event detail (short + separator + extended). pv_key_info: (EDB_CH_EVT_ID_T *) 
 *                                 EDB_KEY_TYPE_EVENT_IDS_BY_RANGE - get size of bucket which contains event ids
 *   @param  [in]  pv_key_info  - (EDB_CH_TIME_FILTER_T*)pv_event_info_len: N*sizeof(EDB_BUCKET_INFO)
 *                                 EDB_KEY_TYPE_EVENT_ITEM_DESC_BY_ID - get length of itemized description. pv_key_info: (EDB_CH_EVT_ID_T *)
 *   @param  [out] pv_event_info_len - (N*sizeof(EDB_ITEM_T))
 *
 *  
 *  @note   
 *  @see
 *  @return     INT32
 *  @retval     EDBR_OK   - Success
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_edb_get_info_len (
        HANDLE_T            h_edb_client,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len);

/*----------------------------------------------------------------------------*/
/*! @brief 
 * 
 *  @param  [in]        h_edb_client        - The handle of cache
 *  @param  [in]        e_key_typ    - The specific key type:
 *                                     EDB_KEY_TYPE_PRESENT_EVENT- get present event information. 
 *                                                                   pv_key_info: (EDB_CH_ID_T*)
 *                                                                   pv_event_info: (EDB_EVENT_INFO_T*)
 *                                     
 *                                     EDB_KEY_TYPE_FOLLOWING_EVENT - get following event information. 
 *                                                                      pv_key_info: (EDB_CH_ID_T*)
 *                                                                      pv_event_info: (EDB_EVENT_INFO_T*)
 *                                     
 *                                     EDB_KEY_TYPE_SINGLE_EVENT_BY_ID- get specific event information. 
 *                                                                      pv_key_info: (EDB_CH_EVT_ID_T *)
 *                                                                      pv_event_info: (EDB_EVENT_INFO_T*)
 *                                     
 *                                     EDB_KEY_TYPE_EVENT_DETAIL_BY_ID- get specific event detail (short + separator + extended).
 *                                                                      pv_key_info: (EDB_CH_EVT_ID_T *)
 *                                                                      pv_event_info: (CHAR*) 
 *                                     
 *                                     EDB_KEY_TYPE _SINGLE_EVENT_BY_UTC- get specific event information with UTC. pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                                                      pv_event_info: (EDB_EVENT_INFO_T*)
 *                                     
 *                                     EDB_KEY_TYPE_EVENT_IDS_BY_RANGE - get event_ids by range of time in one channel.
 *                                                                      pv_key_info: (EDB_CH_TIME_FILTER_T*) pv_event_info: (EDB_BUCKET_INFO*)
 *                                     
 *                                     EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC- get information of current and next events with UTC. 
 *                                                                              pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                                                              pv_event_info: (EDB_EVENT_INFO_T*), need buffer size 2x(EDB_EVENT_INFO_T*)
 *                                     
 *                                     EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC- get information of current and previous events with UTC. 
 *                                                                                 pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                                                                 pv_event_info: (EDB_EVENT_INFO_T*), need buffer size 2x(EDB_EVENT_INFO_T*)
 *                                     
 *                                     EDB_KEY_TYPE_EVENT_IDS_BY_FILTER- with specific filter, get event ids among all channels. pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                                                      pv_event_info: (EDB_BUCKET_INFO*)
 *                                     
 *                                     EDB_CMD_LARGE_QUERY:- get the status of large query. pv_event_info: (BOOL*)
 *                                     

 *                                     EDB_KEY_TYPE_EVENT_ITEM_DESC_BY_ID- get itemized description. pv_key_info: (EDB_CH_EVT_ID_T *)
 *                                                                          pv_event_info: (EDB_ITEM_T*)
 *                                     
 *                                     EDB_KEY_TYPE_EVENT_NUM_BY_RANGE_ASYNC- async get number of events and checksum by range and 
 *                                                                             the result will be provided with x_edb_query_nfy_fct.
 *                                                                             pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                     
 *                                     EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC_ASYNC- async get number and checksum of now/next events by specific time 
 *                                                                                   and the result will be provided with x_edb_query_nfy_fct.
 *                                                                             pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                     
 *                                     EDB_KEY_TYPE_RATING_LIST_BY_ID - get rating list by event id
 *                                                                          pv_key_info: (EDB_CH_EVT_ID_T *)
 *                                                                          pv_event_info: (EDB_PARSE_RATING_T*)
 *                                     
 *                                     EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC- get number and checksum of now/next events in specific channel and time.
 *                                                                              pv_key_info: (EDB_CH_TIME_FILTER_T *)
 *                                                                              pv_event_info: (EDB_BUCKET_INFO*)
 *
 *  @param  [in]        pv_key_info  - the auxiliary information for the key type. Please see the description of e_key_type                                
 *  @param  [in/out]    pv_event_info_len - Contains the length of provided buffer. 
 *                                          After successfully acquired the information, 
 *                                          it will contain the actual length of pv_key_info.
 *
 *  @parm   [out]       pv_event_info - Contains the required event information
 *  @note   
 *  @see
 *  @return     INT32
 *  @retval     EDBR_OK   - Success
 */
/*----------------------------------------------------------------------------*/

extern INT32 x_edb_get_info (
        HANDLE_T            h_edb_client,
        EDB_KEY_TYPE_T      e_key_type,
        VOID*               pv_key_info,
        SIZE_T*             pz_event_info_len,
        VOID*               pv_event_info);

extern HANDLE_T edb_get_msg_q_hdl (VOID);
extern UINT16 x_edb_get_dbg_level(VOID);

extern BOOL x_edb_set_dbg_level(
        UINT16              ui2_level); 

extern SIZE_T edb_get_memory_size(VOID);
extern VOID edb_set_memory_size(SIZE_T z_size);



extern VOID x_edb_pool_lock_rd(VOID);

extern VOID x_edb_pool_unlock_rd(VOID);








#endif /* _X_EDB_H_ */
