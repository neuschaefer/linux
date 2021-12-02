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

#ifndef _U_EDB_H_
#define _U_EDB_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EDB Event DataBase
 *
 *  @ingroup groupMW
 *  @brief Event Context is used to get Event Inforamtion (EPG data) from stream.
 *
 *  %Event DataBase is designed for user to retrive event information (EPG data)
 *   that tranmitted via transport stream.
 *
 *   Event DataBase is consitant of a framework, and a set of difference sub 
 *   engine. Those engines are designed for retrieve different event information
 *   table for differenct DTV standard. E.g. there would be a DVB sub engine, 
 *   which is used to retrieve and parse EIT table that defined in DVB standard. 
 *   And there also be a ATSC sub engine, which is used to retrieve and parse 
 *   EIT table that defined in ATSC standard.
 *
 *  @see groupMW_EDB
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_lnk_list.h"


#define EDB_DVB_BRDCST_GRP                          ( (MAKE_BIT_MASK_8(BRDCST_TYPE_DVB)) | (MAKE_BIT_MASK_8(BRDCST_TYPE_DTMB)) )
#define EDB_NAME_MAX_SIZE                           ((SIZE_T)  16)
#define EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS    "\n"                /*between extennded text and items*/
#define EDB_SEPARATOR_OF_SHORT_TEXT_AND_ITEMS_IF_THERE_IS_NO_EXTENDED_TEXT    "\n"   
#define EDB_SEPARATOR_OF_ITEM_DESC_AND_ITEM_TEXT    "-"                 /*between item descript and item text*/
#define EDB_SEPARATOR_OF_ITEM_AND_ITEM              "\n"                /*between item descript and item text*/
#define EDB_SEPARATOR_OF_MULT_LANG                  "\n"                /*between each language*/
#define EDB_TRANCATE_SPACE_BUFFER                   ((UINT16)(1024*8))
#define EDB_EVENT_DETAIL_BUFFER                     ((UINT16)(1024*8))
#define EDB_MIN_SED_COMPARE_LENGTH		            ((SIZE_T)(   0x7))


#define EDB_EXTENDED_STRATAGE2


typedef  UINT32 EDB_NFY_REASON_T;

/* Notification Reason used in (*x_edb_nfy_fct)() */
#define EDB_REASON_UNKNOWN                      ((UINT32) 0x00000000)

#define EDB_REASON_HANDLE_CLOSED                MAKE_BIT_MASK_32(0)
#define EDB_REASON_EVENT_NOT_FOUND              MAKE_BIT_MASK_32(1)
#define EDB_REASON_EVENT_EXPIRED                MAKE_BIT_MASK_32(2)
#define EDB_REASON_DT_TIMEOUT                   MAKE_BIT_MASK_32(3)
#define EDB_REASON_DT_DISCONT                   MAKE_BIT_MASK_32(4)
#define EDB_REASON_SVCTX_GET_FAILED             MAKE_BIT_MASK_32(5)
#define EDB_REASON_NOW_EVENT_UPDATED            MAKE_BIT_MASK_32(6)
#define EDB_REASON_NEX_EVENT_UPDATED            MAKE_BIT_MASK_32(7)
#define EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED  MAKE_BIT_MASK_32(8)

/* Return values */
#define EDBR_OK                                 ((INT32)    0)
#define EDBR_ALREADY_INIT                       ((INT32)   -1)
#define EDBR_CMD_NOT_SUPPORT                    ((INT32)   -2)
#define EDBR_DUP_BRDCST_TYPE                    ((INT32)   -3)
#define EDBR_ENG_NOT_FOUND                      ((INT32)   -4)
#define EDBR_EVENT_UNAVAIL                      ((INT32)   -5)
#define EDBR_INFO_NOT_FOUND                     ((INT32)   -6)
#define EDBR_INTERNAL_ERROR                     ((INT32)   -7)
#define EDBR_INV_ARG                            ((INT32)   -8)
#define EDBR_INV_COMMAND                        ((INT32)   -9)
#define EDBR_INV_COND                           ((INT32)  -10)
#define EDBR_INV_HANDLE                         ((INT32)  -11)
#define EDBR_NOT_INIT                           ((INT32)  -12)
#define EDBR_OUT_OF_HANDLES                     ((INT32)  -13)
#define EDBR_OUT_OF_MEM                         ((INT32)  -14)
#define EDBR_SVC_UNAVAIL                        ((INT32)  -15)
#define EDBR_OUT_OF_RESOURCE                    ((INT32)  -16)
#define EDBR_HANDLE_NOT_SUPPORT                 ((INT32)  -17)
#define EDBR_ALREADY_EXIST                      ((INT32)  -18)
#define EDBR_VERSION_NOT_CHANGE                 ((INT32)  -19)
#define EDBR_PARSE_ERROR                        ((INT32)  -20)
#define EDBR_HASH_NOT_EXIST                     ((INT32)  -21)
#define EDBR_HASH_PARSE_BREAK                   ((INT32)  -22)



extern UINT16          g_edb_monitor_on_id;
extern UINT16          g_edb_monitor_ts_id;
extern UINT16          g_edb_monitor_svc_id;

#define MONITOR_ON_ID                           (g_edb_monitor_on_id)/*((UINT16)0x1013)*/
#define MONITOR_TS_ID                           (g_edb_monitor_ts_id)/*((UINT16)0x502 )*/
#define MONITOR_SVC_ID                          (g_edb_monitor_svc_id)/*((UINT16)0x520 )*/


/* Command codes used to load events */

typedef UINT32  EDB_COMMAND_CODE_T;

#define EDB_CMD_END                             ((EDB_COMMAND_CODE_T) 0)
#define EDB_CMD_OPEN_DATA                       ((EDB_COMMAND_CODE_T) 1)/*(EDB_ENG_OPEN_DATA_T*) to setup a source control engine 
                                                                          repository with (ui2_svl_id, e_brdcst_type, ps_tuner_name). 
                                                                          This command can be hooked several times for different 
                                                                          source control engine repositories.*/
#define EDB_CMD_CFG_PF_ONLY                     ((EDB_COMMAND_CODE_T) 2)/*(BOOL) to ask to load and cache present/following events only (default: false)*/
#define EDB_CMD_CFG_ACTUAL_ONLY                 ((EDB_COMMAND_CODE_T) 3)/*(BOOL) to ask to load and cache events from EIT actual only (default: false)*/
#define EDB_CMD_CFG_MAX_DAYS                    ((EDB_COMMAND_CODE_T) 4)/*(UINT16)to restrict cache days. For EU models, the days of EIT loading will
                                                                          be also restricted. (default: 8 days)*/
#define EDB_CMD_CFG_PREF_LANG                   ((EDB_COMMAND_CODE_T) 5)/*(EDB_PREF_LANG_T*) to provide preference language (default: (1, eng)
                                                                           EDB_CMD_COUNTRY_CODE: (CHAR*) to provide country code for 
                                                                           mapping correct character set (default: GBR)*/
#define EDB_CMD_CFG_COUNTRY_CODE                ((EDB_COMMAND_CODE_T) 6)
#define EDB_CMD_CFG_ACTIVE_WIN                  ((EDB_COMMAND_CODE_T) 7)
#define EDB_CMD_CFG_EVENT_MIN_SECS              ((EDB_COMMAND_CODE_T) 8)
#define EDB_CMD_CFG_FAKE_EVENT_INSERTION_ENABLE ((EDB_COMMAND_CODE_T) 9)
#define EDB_CMD_CFG_FAKE_EVENT_MIN_SECS         ((EDB_COMMAND_CODE_T) 10)
#define EDB_CMD_CFG_TIME_CONFLICT_ALLOW         ((EDB_COMMAND_CODE_T) 11)
#define EDB_CMD_CFG_TIME_PARTIAL_OVERLAP_ALLOW  ((EDB_COMMAND_CODE_T) 12)
#define EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR      ((EDB_COMMAND_CODE_T) 13)
#define EDB_CMD_CFG_DVBC_OPERATOR               ((EDB_COMMAND_CODE_T) 14)


#define EDB_CMD_DO_CURRENT_SERVICE              ((EDB_COMMAND_CODE_T) 15)
#define EDB_CMD_DO_RESTART                      ((EDB_COMMAND_CODE_T) 16)
#define EDB_CMD_DO_CLEAN                        ((EDB_COMMAND_CODE_T) 17)
#define EDB_CMD_DO_ENABLE                       ((EDB_COMMAND_CODE_T) 18)
#define EDB_CMD_DO_TUNER_CHANGE                 ((EDB_COMMAND_CODE_T) 19)
#define EDB_CMD_DO_ASU_MODE                     ((EDB_COMMAND_CODE_T) 20)
#define EDB_CMD_DO_FLASH_EVENTS                 ((EDB_COMMAND_CODE_T) 21)
#define EDB_CMD_DO_SYNC_EVENTS                  ((EDB_COMMAND_CODE_T) 22)
#define EDB_CMD_RISKTV                          ((EDB_COMMAND_CODE_T) 23)
#define EDB_CMD_CHANNEL_SCAN_ACTIVE             ((EDB_COMMAND_CODE_T) 24)


#define EDB_CMD_MAX_NUM                         ((EDB_COMMAND_CODE_T) 25) 



#define EDB_MAX_NUM_PREF_LANG                   ((UINT8)    4)
#define EDB_MAX_NUM_CA_SYSTEM                   ((UINT8)    4)

#define EDB_MAX_LENGTH_EVENT_TITLE              ((UINT8)   60)
#define EDB_MAX_LENGTH_EVENT_GUIDANCE			((UINT8)   75)
#define EDB_MAX_NUM_RATING_INFO                 ((UINT8)    4)
#define EDB_MAX_NUM_CATEGORY                    ((UINT8)    8)
#define EDB_MAX_LEN_ITEM_DESC_CHAR              ((UINT8)  255)
#define EDB_MAX_LEN_ITEM_CHAR                   ((UINT8)  255)
#define EDB_S_MAX_NUM_SEG_IN_SUB_TBL            ((UINT8)   32)
#define EDB_MAX_PER_CH_TBL                      ((UINT8)    2)   /*16 for 64 days,2 for 8 days*/
#define MAX_COMPONENT_INFO                      ((UINT8)    8)

#define EDB_MIN_MEMORY_SIZE                     ((SIZE_T)1024*1024*5)/**/
#define EDB_MAX_MEMORY_SIZE                     (edb_get_memory_size())/**/
#ifdef MW_EPG_CACHE_NVM_SUPPORT 
#define EDB_MAX_MEMORY_SIZE_FOR_FLASH_POOL      (edb_get_flash_size_for_pool())/**/
#endif


#if 0/*EDB memory can be config at sys_start*/
#ifdef MW_EDB_WITH_SMALL_MEMORY
#define EDB_MAX_MEMORY_SIZE                     ((SIZE_T)1024*1024*5)/**/
#else
#define EDB_MAX_MEMORY_SIZE                     ((SIZE_T)1024*1024*12)/**/
#endif
#endif

#define EDB_MEMORY_THRESHOLD                    ((SIZE_T)1024*150)/**/
#ifdef MW_EPG_CACHE_NVM_SUPPORT 
#define EDB_MEMORY_THRESHOLD_FOR_SINGLE_EVNET   ((SIZE_T)1024*20)/**/
#endif

#define EDB_MAX_ENG_DATA                        ((UINT8)        10)/**/
#define EDB_MAX_INTER_CH_LOCATE                 ((UINT16) 1024 * 2)/*enough ??*/
#define EDB_POOL_HASH_CHANNEL_TABLE_SIZE        ((UINT16) 100     )
#define EDB_POOL_MAX_CHANNEL_EVENT              ((UINT16) 96 * 8  )  /*(On day 96) * (8 days)*/
#define EDB_POOL_CHANNEL_HASH_EVENT_TABLE_SIZE  ((UINT16) 50 )  

#define EDB_EVENT_MAX_DES_SIZE                  ((UINT16)      0xFF)  

#define MAX_EMPTY_CELL_SUPPORT                  ((UINT8)  32      )
#define EVENT_EMPTY_CELL_START_EVENT_ID         ((UINT16) 65500   )



#if 0
typedef UINT8  EDB_EVENT_TYPE_T;
#define EDB_EVENT_TYPE_PRESENT           ((UINT8)  0)
#define EDB_EVENT_TYPE_FOLLOW            ((UINT8)  1)
#define EDB_EVENT_TYPE_SCHEDULE          ((UINT8)  2)

typedef enum
{
    EDB_EVENT_BRDCST_TYPE_DVB = 0,
    EDB_EVENT_BRDCST_TYPE_ATSC
}EDB_EVENT_BRDCST_TYPE_T; /*TODO*/
#endif




/* EDB States */
/*------------------------------------------------------------------*/
/*! @enum EDB_COND_T
 *  @brief EDB engine condition
 *  @code
 *  typedef enum
 *  {
 *      EDB_COND_UNCONFIG = 0,
 *      EDB_COND_STOPPED,
 *      EDB_COND_STARTED
 *  } EDB_COND_T;
 *
 *  @endcode
 *  @li@c  
 *  @li@c  
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EDB_COND_UNCONFIG = 0,
    EDB_COND_STOPPED,
    EDB_COND_STARTED
} EDB_COND_T;



typedef enum
{
    EDB_KEY_TYPE_UNKNOWN = 0,
    EDB_KEY_TYPE_EVENT_ID,   
    EDB_KEY_TYPE_START_TIME,
    EDB_KEY_TYPE_DURATION,    
    EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
    EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
    /*EDB_KEY_TYPE_EVENT_ITEM_DESC_BY_ID,*/
    EDB_KEY_TYPE_PRESENT_EVENT,
    EDB_KEY_TYPE_FOLLOWING_EVENT,
    EDB_KEY_TYPE_SINGLE_EVENT_BY_ID,
    EDB_KEY_TYPE_SINGLE_EVENT_BY_UTC,
    EDB_KEY_TYPE_EVENT_IDS_BY_RANGE,
    EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC, /*the utc is given for searching these two events*/
    EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC,
    EDB_KEY_TYPE_EVENT_IDS_BY_FILTER,
    EDB_KEY_TYPE_EVENT_NUM_BY_RANGE_ASYNC,
    EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC_ASYNC,
    EDB_KEY_TYPE_RATING_LIST_BY_ID,
    EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC, /*the utc is given for checking the validity of current PF*/
    EDB_KEY_TYPE_LINKAGE,
    EDB_KEY_TYPE_GUIDANCE_TEXT_BY_ID,
    EDB_KEY_TYPE_SINGLE_POOL_SERIAL_EVENT_BY_ID,
    EDB_KEY_TYPE_EVENT_IDS_BY_DESIRED_STRING,
    EDB_KEY_TYPE_DETAIL_LANGUAGE,
    EDB_KEY_TYPE_PREVIOUS_EVENT_BY_UTC,/*the utc is given for searching the previous event*/
#if 0    
    EDB_KEY_TYPE_COMMON,
    EDB_KEY_TYPE_EVENT_DETAIL,
    EDB_KEY_TYPE_EVENT_EXTENDED,
    EDB_KEY_TYPE_TIME_SHIFTED_EVENT ,
    EDB_KEY_TYPE_COMPONENT ,         
    EDB_KEY_TYPE_CA_IDENTIFIER,    
    EDB_KEY_TYPE_CONTENT,         
    EDB_KEY_TYPE_PARENTAL_RATING, 
#endif    
    EDB_KEY_TYPE_TOTAL_NUMBER     /* total number of key types */
} EDB_KEY_TYPE_T;






/*----------------------------------------------------------------------------*/
/*! @brief  EDB notify callback function

 *  @param  [in] h_edb,       - The handle of the edb
 *  @param  [in] e_reason,    - The notify reason of edb
 *  @param  [in] pv_nfy_tag,  - The tag provided when creating edb
 *  @param  [in] ui4_data1    - When e_reason is 
 *                              EDB_REASON_NOW_EVENT_UPDATED or EDB_REASON_NEX_EVENT_UPDATED
 *                                  ui4_data1 is ui2_svl_id of current svl and 
 *                                  ui4_data2 is ui4_channel_id of current service.
 *                              EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED
 *                                  ui4_data1 is ui2_svl_id of current svl 
 *                                  ui4_data2 is ui4_channel_id of updated service.
 * 
 *  @param  [in] ui4_data2    -
 *
 *
 *  @note   User should implement such function pointer and register to EDB
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/
typedef VOID (*x_edb_nfy_fct) (
        HANDLE_T                h_edb,
        EDB_NFY_REASON_T        e_reason,
        VOID*                   pv_nfy_tag,
        UINT32                  ui4_data1,
        UINT32                  ui4_data2);







/*----------------------------------------------------------------------------*/
/*! @brief  EDB query notify function

 *  @param  [in] ui2_svl_id,              - Svl ID
 *  @param  [in] ui4_channel_id,          - The input channel id when invoking this query
 *  @param  [in] t_base_utc,              - The input start time when invoking this query
 *  @param  [in] ui2_event_num,           - The number of events in this time of query
 *  @param  [in] ui2_channel_check_sum,   - The checksum of events in this time of query
 *  @param  [in] pv_tag                   - The input tag address when invoking this query
 *
 *
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/

typedef VOID (*x_edb_query_nfy_fct)(
        UINT16   ui2_svl_id,
        UINT32   ui4_channel_id,
        TIME_T   t_base_utc,
        UINT16   ui2_event_num,
        UINT16   ui2_channel_check_sum,
        VOID*    pv_tag
        );


/*------------------------------------------------------------------*/
/*! @struct EDB_ENG_OPEN_DATA_T
 *
 *  @code
 * typedef struct  _EDB_ENG_OPEN_DATA_T
 * {
 *     BRDCST_TYPE_T   e_brdcst_type;
 *     UINT16          ui2_svl_id;
 *     CHAR*           ps_tuner_name;  
 * } EDB_ENG_OPEN_DATA_T;  
 * 
 *  @endcode
 * 
 *  @li@c    
 *  @li@c    
 */
/*------------------------------------------------------------------*/
typedef struct  _EDB_ENG_OPEN_DATA_T
{
    BRDCST_TYPE_T   e_brdcst_type;
    UINT16          ui2_svl_id;
    CHAR*           ps_tuner_name;  
} EDB_ENG_OPEN_DATA_T; 



/*------------------------------------------------------------------*/
/*! @struct EDB_PREF_LANG_T
 *
 *  @code
 * typedef struct  EDB_PREF_LANG_T
 * {
 *     UINT8          ui1_num;
 *     ISO_639_LANG_T t_lang[EDB_MAX_NUM_PREF_LANG]; 
 * } EDB_PREF_LANG_T; 
 * 
 *  @endcode
 * 
 *  @li@c    ui1_num                   -The number of preferred languages
 *  @li@c    t_time_duration           -The array contains preferred languages 
 */
/*------------------------------------------------------------------*/
typedef struct  EDB_PREF_LANG_T
{
    UINT8          ui1_num;
    ISO_639_LANG_T t_lang[EDB_MAX_NUM_PREF_LANG]; 
} EDB_PREF_LANG_T; 




/*------------------------------------------------------------------*/
/*! @struct EDB_CH_ID_T
 * 
 *  @code
 * 
 * typedef struct _EDB_CH_ID_T                 
 * {
 *     BRDCST_TYPE_T       e_brdcst_type;
 *     UINT16                 ui2_svl_id;
 *     UINT32                 ui4_channel_id;
 * } EDB_CH_ID_T;                           @endcode
 *  @endcode
 * 
 *  @li@c    e_brdcst_type                  - Broadcast type.
 *  @li@c    ui2_svl_id                     - svl id
 *  @li@c    ui4_channel_id                 - channel id
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_ID_T
{
    BRDCST_TYPE_T           e_brdcst_type;
    UINT16                  ui2_svl_id;
    UINT32                  ui4_channel_id;
} EDB_CH_ID_T;


#define EDB_MAX_NUM_ACTIVE_CHANNEL  ((UINT8) 32)

/*------------------------------------------------------------------*/
/*! @struct  EDB_CACHE_ACTIVE_WIN_T
 * 
 *  @code
 * 
 *typedef struct _EDB_ACTIVE_WIN_T
 *{
 *    UINT8     ui1_num_channel;
 *    EDB_CH_ID_T at_channel_id[EDB_MAX_NUM_ACTIVE_CHANNEL];
 *    TIME_T    t_start_time;
 *    TIME_T    t_duration;
 *} EDB_CACHE_ACTIVE_WIN_T; 
 *   
 *  @endcode
 * 
 *  @li@c      ui1_num_channel            - The number of active channels 
 *  @li@c      at_channel_id              - The array contains the id of active channels
 *  @li@c      t_start_time               - The start time of active window
 *  @li@c      t_duration                 - the duration of active window
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CACHE_ACTIVE_WIN_T
{
    UINT8           ui1_num_channel;
    EDB_CH_ID_T     at_channel_id[EDB_MAX_NUM_ACTIVE_CHANNEL];
    TIME_T          t_start_time;
    TIME_T          t_duration;
} EDB_CACHE_ACTIVE_WIN_T; 



/*------------------------------------------------------------------*/
/*! @struct  EDB_CH_EVT_ID_T

 *  @code
 * 
 * typedef struct _EDB_CH_EVT_ID_T            
 * {
 *     BRDCST_TYPE_T       e_brdcst_type;
 *     UINT16                 ui2_svl_id;
 *     UINT32                 ui4_channel_id;
 *     UINT16                 ui2_event_id;
 * } EDB_CH_EVT_ID_T;
 *   
 *  @endcode
 * 
 *  @li@c     e_brdcst_type            - Broadcast type. 
 *  @li@c     ui2_svl_id               - Svl ID
 *  @li@c     ui4_channel_id           - Channel ID
 *  @li@c     ui2_event_id             - Event ID
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_EVT_ID_T
{
    BRDCST_TYPE_T          e_brdcst_type;
    UINT16                 ui2_svl_id;
    UINT32                 ui4_channel_id;
    UINT16                 ui2_event_id;
    BOOL                   b_auto_event_id;
} EDB_CH_EVT_ID_T;


typedef enum 
{
    EDB_DESIRED_STRING_MATCH_TITLE,
    EDB_DESIRED_STRING_MATCH_TITLE_AND_DETAIL
}EDB_DESIRED_STRING_MATCH_TYPE_T;

typedef struct _EDB_DESIRED_STRING_T
{
    BRDCST_TYPE_T                       e_brdcst_type;
    UINT16                              ui2_svl_id;
    CHAR*                               ps_desired_string;
    SIZE_T                              z_string_length;
    EDB_DESIRED_STRING_MATCH_TYPE_T     e_match_type;
} EDB_DESIRED_STRING_T;


/*------------------------------------------------------------------*/
/*! @struct  EDB_BUCKET_INFO_T

 *  @code
 * 
 * typedef struct _EDB_BUCKET_INFO_T             
 * {
 *     UINT16                   ui2_checksum
 *     UINT16*                  pui2_num_events;
 *     EDB_CH_EVT_ID_T*      pui2_ch_event_id;
 * } EDB_BUCKET_INFO_T;
 * 
 *   
 *  @endcode
 * 
 *  @li@c     ui2_checksum            - The checksum of the events in this bucket
 *  @li@c     pui2_num_events         - [In] the max num of event for this bucket
 *                                      [out] the actual num of events for this query
 *
 *  @li@c     pui2_ch_event_id        - The start pointer address of the array to put ID of events.
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_BUCKET_INFO_T
{
    UINT16                   ui2_checksum;
    UINT16*                  pui2_num_events;
    EDB_CH_EVT_ID_T*         pui2_ch_event_id;
} EDB_BUCKET_INFO_T;








#define     EDB_EVENT_GOT_EVENT_ID              MAKE_BIT_MASK_32(0)
#define     EDB_EVENT_GOT_START_TIME            MAKE_BIT_MASK_32(1)
#define     EDB_EVENT_GOT_DURATION              MAKE_BIT_MASK_32(2)
#define     EDB_EVENT_GOT_CAPTION               MAKE_BIT_MASK_32(3)
#define     EDB_EVENT_GOT_FREE_CA_MODE          MAKE_BIT_MASK_32(4)
#define     EDB_EVENT_GOT_LINKAGE               MAKE_BIT_MASK_32(5)
#define     EDB_EVENT_GOT_EVENT_TITLE           MAKE_BIT_MASK_32(6)
#define     EDB_EVENT_GOT_EVENT_DETAIL          MAKE_BIT_MASK_32(7)
#define     EDB_EVENT_GOT_EVENT_EXTENDED        MAKE_BIT_MASK_32(8)
#define     EDB_EVENT_GOT_TIME_SHIFT            MAKE_BIT_MASK_32(9)
#define     EDB_EVENT_GOT_COMPONENT             MAKE_BIT_MASK_32(10)
#define     EDB_EVENT_GOT_CA_IDENTIFIER         MAKE_BIT_MASK_32(11)
#define     EDB_EVENT_GOT_CONTENT               MAKE_BIT_MASK_32(12)
#define     EDB_EVENT_GOT_RATING                MAKE_BIT_MASK_32(13)
#define     EDB_EVENT_GOT_GUIDANCE              MAKE_BIT_MASK_32(14)



typedef struct _EDB_RATING_T
{
    UINT16                  ui2_dimension;
    UINT16                  ui2_rating_value;
} EDB_RATING_T;

typedef struct _EDB_ITEM_T
{
    UINT8       ui1_item_desc_len;
    CHAR        ac_item_desc_char[EDB_MAX_LEN_ITEM_DESC_CHAR];
    UINT8       ui1_item_len;
    CHAR        ac_item_char[EDB_MAX_LEN_ITEM_CHAR];
} EDB_ITEM_T;


typedef struct _EDB_COMPONENT_DESCRIPTOR_T
{
    UINT8           ui1_stream_content;
    UINT8           ui1_component_type;
    UINT8           ui1_component_tag;
    ISO_639_LANG_T  t_lang;
} EDB_COMPONENT_INFO_T;

typedef struct _EDB_EVENT_LINKAGE_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
}EDB_EVENT_LINKAGE_INFO_T;

#define MAX_EVENT_LINKAGE_INFO      ((UINT8)4)

#define GUIDANCE_MODE_NOT_DEFINE 				((UINT8)0)
#define UNSUITABLE_UNTIL_AFTER_THE_WATERSHED 	((UINT8)1)

typedef struct _EDB_EVENT_INFO_T
{
    UINT16                      ui2_svl_id;
    UINT32                      ui4_channel_id;
    UINT16                      ui2_event_id;
    TIME_T                      t_start_time;
    TIME_T                      t_duration;
    BOOL                        b_caption;
    UINT16                      ui2_event_mask;
    BOOL                        b_free_ca_mode;
    UINT8                       ui1_event_title_len;
    CHAR                        ac_event_title[EDB_MAX_LENGTH_EVENT_TITLE + 1];
	UINT8						ui1_guidance_mode;
	UINT8                       ui1_event_guidance_len;
	CHAR						ac_event_guidance[EDB_MAX_LENGTH_EVENT_GUIDANCE + 1];
    UINT8                       ui1_num_rating;
    UINT16                      aui2_ca_system_id[EDB_MAX_NUM_CA_SYSTEM];
    UINT8                       ui1_num_event_category;
    UINT8                       aui1_event_category[EDB_MAX_NUM_CATEGORY];
    UINT8                       ui1_num_comp_info;
    EDB_COMPONENT_INFO_T        at_comp_info[MAX_COMPONENT_INFO];
    UINT8                       ui1_num_linkage_info;
    EDB_EVENT_LINKAGE_INFO_T    at_linkage_info[MAX_EVENT_LINKAGE_INFO];
    UINT16                      ui2_checksum;    

#if 0
    EDB_RATING_T           at_rating_info[EDB_MAX_NUM_RATING_INFO];
#endif
} EDB_EVENT_INFO_T;

typedef enum 
{
    EDB_RATING_NORNAL,
    EDB_RATING_NO_EIT,
    EDB_RATING_NO_DESC
}EDB_RATING_TYPE_T;



typedef struct _EDB_RATING_LIST_T
{
    UINT16                  ui2_rating_region;
    UINT16                  ui2_rated_dimensions;
    CHAR*                   ps_country_code;
    EDB_RATING_T*           pt_ratings;

    struct _EDB_RATING_LIST_T*  pt_next;

    EDB_RATING_T            t_rating;
    ISO_3166_COUNT_T        t_country_code;
    EDB_RATING_TYPE_T       t_rating_type;
} EDB_RATING_LIST_T;



/*----------------------------------------------------------------------------*/
/*! @brief  EDB filter function

 *  @param  [in] pt_event_info      - The pointer of event info which is used as criterions to filter events.
 *  @param  [in] pv_tag             - The input tag address when invoking this query
 *
 *
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/

typedef BOOL (*x_edb_filter_fct)(
        EDB_EVENT_INFO_T*    pt_event_info,
        VOID*                       pv_tag
        );



/*------------------------------------------------------------------*/
/*! @struct EDB_CH_TIME_FILTER_T 

 *  @code
 * 
 * typedef struct _EDB_CH_TIME_FILTER_T            
 * {
 *     BRDCST_TYPE_T               e_brdcst_type;
 *     UINT16                      ui2_svl_id;
 *     UINT32                      ui4_channel_id;
 *     TIME_T                      t_start_time;
 *     TIME_T                      t_end_time;
 *     x_edb_filter_fct            pf_filter_fct;  
 *     VOID*                       pv_filter_tag;  
 *     x_edb_query_nfy_fct         pf_nfy_fct;     
 *     VOID*                       pv_nfy_tag;     
 * }EDB_CH_TIME_FILTER_T; 
 *   
 *  @endcode
 * 
 *  @li@c   e_brdcst_type       - Broadcast type  
 *  @li@c   ui2_svl_id          - Svl list id
 *  @li@c   ui4_channel_id      - Channel ID
 *  @li@c   t_start_time        - indicates the specific UTC instead start time.
 *  @li@c   t_end_time          - indicates the specific UTC instead end time.
 *  @li@c   pf_filter_fct       - The function to provide the additional filter criterions. If the function point is NULL, it means that additional filter is not required.
 *  @li@c   pv_filter_tag       - The tag to be input when invoking the filter function
 *  @li@c   pf_nfy_fct          - When the type of key is ASYNC, the result will be provide with this notify function.
 *  @li@c   pv_nfy_tag          - The tag to be input when invoking the notify function.
 *                                      
 *
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_TIME_FILTER_T
{
    BRDCST_TYPE_T               e_brdcst_type;
    UINT16                      ui2_svl_id;
    UINT32                      ui4_channel_id;
    TIME_T                      t_start_time;
    TIME_T                      t_end_time;
    x_edb_filter_fct            pf_filter_fct;
    VOID*                       pv_filter_tag;
    x_edb_query_nfy_fct         pf_nfy_fct;
    VOID*                       pv_nfy_tag;
} EDB_CH_TIME_FILTER_T;




typedef struct _EDB_COMMAND_T
{
    EDB_COMMAND_CODE_T    e_code;
    union
    {
        EDB_ENG_OPEN_DATA_T*            pt_eng_open_data;   /*EDB_CMD_OPEN_DATA */
        BOOL                            b_bool;             /*EDB_CMD_CFG_PF_ONLY EDB_CMD_CFG_ACTUAL_ONLY */
        UINT16                          ui2_number;         /*EDB_CMD_CFG_MAX_DAYS  EDB_CMD_CFG_EVENT_MIN_SECS  EDB_CMD_DO_CURRENT_SERVICE EDB_CMD_DO_ENABLE*/
        EDB_PREF_LANG_T*                pt_pref_lang;       /*EDB_CMD_CFG_PREF_LANG*/
        ISO_3166_COUNT_T                t_country_code;     /*EDB_CMD_CFG_COUNTRY_CODE*/
        EDB_CACHE_ACTIVE_WIN_T*         pt_active_win;      /*EDB_CMD_CFG_ACTIVE_WIN*/
        EDB_CH_ID_T*                    pt_ch_id;           /**/
        CHAR                            s_separator[4];     /*EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR*/
        UINT32                 t_operator;
    } u;
} EDB_COMMAND_T;









/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_EDB               */
/*----------------------------------------------------------------------------*/

#endif /* _U_EDB_H_ */


