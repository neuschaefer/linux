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
/*-----------------------------------------------------------------------------*/
/*! @file u_evctx.h 
 * $RCSfile: u_evctx.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/5 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: e5a2644d77eac094a5d2aa1496612a57 $
 *
 *  @par Description:
 *         This contains Event Context library exported macros, definitons,
 *         and structures.
 *---------------------------------------------------------------------------*/

#ifndef _U_EVCTX_H_
#define _U_EVCTX_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EVCTX Event Context
 *
 *  @ingroup groupMW
 *  @brief Event Context is used to get Event Inforamtion (EPG data) from stream.
 *
 *  %Event Context is designed for user to retrive event information (EPG data)
 *   that tranmitted via transport stream.
 *
 *   Event Context is consitant of a framework, and a set of difference sub 
 *   engine. Those engines are designed for retrieve different event information
 *   table for differenct DTV standard. E.g. there would be a DVB sub engine, 
 *   which is used to retrieve and parse EIT table that defined in DVB standard. 
 *   And there also be a ATSC sub engine, which is used to retrieve and parse 
 *   EIT table that defined in ATSC standard.
 *
 *  @see groupMW_EVCTX
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_device_type.h"
#include "u_handle.h"
#include "u_svl.h"
#include "u_tsl.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Notification Reason used in (*x_evctx_nfy_fct)() */
#define EVCTX_REASON_UNKNOWN            ((UINT32) 0x00000000)
#define EVCTX_REASON_HANDLE_CLOSED      MAKE_BIT_MASK_32(0)
#define EVCTX_REASON_EVENT_NOT_FOUND    MAKE_BIT_MASK_32(1)
#define EVCTX_REASON_EVENT_EXPIRED      MAKE_BIT_MASK_32(2)
#define EVCTX_REASON_DT_TIMEOUT         MAKE_BIT_MASK_32(3)
#define EVCTX_REASON_DT_DISCONT         MAKE_BIT_MASK_32(4)
#define EVCTX_REASON_SVCTX_GET_FAILED   MAKE_BIT_MASK_32(5)


/* Return values */
#define EVCTXR_OK                       ((INT32)    0)
#define EVCTXR_ALREADY_INIT             ((INT32)   -1)
#define EVCTXR_CMD_NOT_SUPPORT          ((INT32)   -2)
#define EVCTXR_DUP_BRDCST_TYPE          ((INT32)   -3)
#define EVCTXR_ENG_NOT_FOUND            ((INT32)   -4)
#define EVCTXR_EVENT_UNAVAIL            ((INT32)   -5)
#define EVCTXR_INFO_NOT_FOUND           ((INT32)   -6)
#define EVCTXR_INTERNAL_ERROR           ((INT32)   -7)
#define EVCTXR_INV_ARG                  ((INT32)   -8)
#define EVCTXR_INV_COMMAND              ((INT32)   -9)
#define EVCTXR_INV_COND                 ((INT32)  -10)
#define EVCTXR_INV_HANDLE               ((INT32)  -11)
#define EVCTXR_NOT_INIT                 ((INT32)  -12)
#define EVCTXR_OUT_OF_HANDLES           ((INT32)  -13)
#define EVCTXR_OUT_OF_MEM               ((INT32)  -14)
#define EVCTXR_SVC_UNAVAIL              ((INT32)  -15)
#define EVCTXR_OUT_OF_RESOURCE          ((INT32)  -16)
#define EVCTXR_HANDLE_NOT_SUPPORT       ((INT32)  -17)

/*------------------------------------------------------------------*/
/*! @enum    EVCTX_COND_T
 *  @brief        Type of Event Context Condition
 
 *  @code
 * typedef enum
 * {
 *     EVCTX_COND_UNAVAIL = 0,
 *     EVCTX_COND_TRANSITION,
 *     EVCTX_COND_AVAIL,
 *     EVCTX_COND_ACTIVE,      
 *     EVCTX_COND_EXPIRED,     
 *     EVCTX_COND_UPDATED      
 * } EVCTX_COND_T; 
 *  @endcode

 *  @li@c   EVCTX_COND_UNAVAIL          - The evctx condition is unavailable
 *  @li@c   EVCTX_COND_TRANSITION       - The evctx condition is in transition condition
 *  @li@c   EVCTX_COND_AVAIL            - The evctx condition is available
 *  @li@c   EVCTX_COND_ACTIVE           - The evctx condition is active
 *  @li@c   EVCTX_COND_EXPIRED          - The evctx condition is expired
 *  @li@c   EVCTX_COND_UPDATED          - The evctx condition is updated
 */
/*------------------------------------------------------------------*/
/* EVCTX object conditions */
typedef enum
{
    EVCTX_COND_UNAVAIL = 0,
    EVCTX_COND_TRANSITION,
    EVCTX_COND_AVAIL,
    EVCTX_COND_ACTIVE,      /* the event becomes the current event */
    EVCTX_COND_EXPIRED,     /* the event becomes expired */
    EVCTX_COND_UPDATED      /* the content of event is updated */
} EVCTX_COND_T;

/*------------------------------------------------------------------*/
/*! @enum    EVCTX_CATEGORY_T
 *  @brief        Type of Event Context Category
 
 *  @code
 * typedef enum
 * {
 *     EVCTX_CATEGORY_UNKNOWN = 0,
 *     EVCTX_CATEGORY_MOVIE_DRAMA,
 *     EVCTX_CATEGORY_NEWS,       
 *     EVCTX_CATEGORY_SHOW,       
 *     EVCTX_CATEGORY_SPORTS,
 *     EVCTX_CATEGORY_CHILDREN,   
 *     EVCTX_CATEGORY_MUSIC,      
 *     EVCTX_CATEGORY_ARTS,       
 *     EVCTX_CATEGORY_SOC_POL_EC, 
 *     EVCTX_CATEGORY_ED_SC_FACT, 
 *     EVCTX_CATEGORY_LEISURE,    
 *     EVCTX_CATEGORY_SPECIAL,    
 *     EVCTX_CATEGORY_COMEDY,     
 *     EVCTX_CATEGORY_DRAMA,      
 *     EVCTX_CATEGORY_DOCUMENTARY 
 * 
 * #ifdef MW_ISDB_BRA_SUPPORT    
 *     ,
 *     EVCTX_CATEGORY_BRA_NEWS_1 = 0     ,
 *     EVCTX_CATEGORY_BRA_SPORTS_2       ,
 *     EVCTX_CATEGORY_BRA_EDUCATION_3    ,
 *     EVCTX_CATEGORY_BRA_SOAP_OPERA_4   ,
 *     EVCTX_CATEGORY_BRA_MINI_SERIES_5  ,
 *     EVCTX_CATEGORY_BRA_SERIES_6       ,
 *     EVCTX_CATEGORY_BRA_VARIETY_7      ,
 *     EVCTX_CATEGORY_BRA_REALITY_SHOW_8 ,
 *     EVCTX_CATEGORY_BRA_INFOMATION_9   ,
 *     EVCTX_CATEGORY_BRA_COMICAL_10     ,
 *     EVCTX_CATEGORY_BRA_CHILDREN_11    ,
 *     EVCTX_CATEGORY_BRA_EROTIC_12      ,
 *     EVCTX_CATEGORY_BRA_MOVIE_13       ,
 *     EVCTX_CATEGORY_BRA_RA_TE_SA_PR_14 ,
 *     EVCTX_CATEGORY_BRA_DEBATE_INTERVIEW_15 ,
 *     EVCTX_CATEGORY_BRA_OTHER_16,
 *     EVCTX_CATEGORY_BRA_UNKNOWN
 * #endif
 * } EVCTX_CATEGORY_T;
 *  @endcode

 *  @li@c   EVCTX_CATEGORY_UNKNOWN                  - Unkown Category
 *  @li@c   EVCTX_CATEGORY_MOVIE_DRAMA              - Movie Drama Category
 *  @li@c   EVCTX_CATEGORY_NEWS                     - News Category
 *  @li@c   EVCTX_CATEGORY_SHOW                     - Show Category
 *  @li@c   EVCTX_CATEGORY_SPORTS                   - SPORTS Category
 *  @li@c   EVCTX_CATEGORY_CHILDREN                 - CHILDREN Category
 *  @li@c   EVCTX_CATEGORY_MUSIC                    - MUSIC Category
 *  @li@c   EVCTX_CATEGORY_ARTS                     - ARTS Category
 *  @li@c   EVCTX_CATEGORY_SOC_POL_EC               - SOC_POL_EC Category
 *  @li@c   EVCTX_CATEGORY_ED_SC_FACT               - ED_SC_FACT Category
 *  @li@c   EVCTX_CATEGORY_LEISURE                  - LEISURE Category
 *  @li@c   EVCTX_CATEGORY_SPECIAL                  - SPECIAL Category
 *  @li@c   EVCTX_CATEGORY_COMEDY                   - COMEDY Category
 *  @li@c   EVCTX_CATEGORY_DRAMA                    - DRAMA Category
 *  @li@c   EVCTX_CATEGORY_DOCUMENTARY              - DOCUMENTARY Category
 *  @li@c   EVCTX_CATEGORY_SPECIAL                  - SPECIAL Category
 *  @li@c   EVCTX_CATEGORY_BRA_NEWS_1 = 0           - BRA_NEWS_1 Category
 *  @li@c   EVCTX_CATEGORY_BRA_SPORTS_2             - BRA_SPORTS_2 Category
 *  @li@c   EVCTX_CATEGORY_BRA_EDUCATION_3          - BRA_EDUCATION_3 Category
 *  @li@c   EVCTX_CATEGORY_BRA_SOAP_OPERA_4         - BRA_SOAP_OPERA_4 Category
 *  @li@c   EVCTX_CATEGORY_BRA_MINI_SERIES_5        - BRA_MINI_SERIES_5 Category
 *  @li@c   EVCTX_CATEGORY_BRA_SERIES_6             - BRA_SERIES_6 Category
 *  @li@c   EVCTX_CATEGORY_BRA_VARIETY_7            - BRA_VARIETY_7 Category
 *  @li@c   EVCTX_CATEGORY_BRA_REALITY_SHOW_8       - BRA_REALITY_SHOW_8 Category
 *  @li@c   EVCTX_CATEGORY_BRA_INFOMATION_9         - BRA_INFOMATION_9 Category
 *  @li@c   EVCTX_CATEGORY_BRA_COMICAL_10           - BRA_COMICAL_10 Category
 *  @li@c   EVCTX_CATEGORY_BRA_CHILDREN_11          - BRA_CHILDREN_11 Category
 *  @li@c   EVCTX_CATEGORY_BRA_EROTIC_12            - BRA_EROTIC_12 Category
 *  @li@c   EVCTX_CATEGORY_BRA_MOVIE_13             - BRA_MOVIE_13 Category
 *  @li@c   EVCTX_CATEGORY_BRA_RA_TE_SA_PR_14       - BRA_RA_TE_SA_PR_14 Category
 *  @li@c   EVCTX_CATEGORY_BRA_DEBATE_INTERVIEW_15  - BRA_DEBATE_INTERVIEW_15 Category
 *  @li@c   EVCTX_CATEGORY_BRA_OTHER_16             - BRA_OTHER_16 Category
 *  @li@c   EVCTX_CATEGORY_BRA_UNKNOWN              - BRA_UNKNOWN Category
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EVCTX_CATEGORY_UNKNOWN = 0,
    EVCTX_CATEGORY_MOVIE_DRAMA,
    EVCTX_CATEGORY_NEWS,        /* News/Current affairs */
    EVCTX_CATEGORY_SHOW,        /* Show/Game show */
    EVCTX_CATEGORY_SPORTS,
    EVCTX_CATEGORY_CHILDREN,    /* Children/Youth programmes */
    EVCTX_CATEGORY_MUSIC,       /* Music/Ballet/Dance */
    EVCTX_CATEGORY_ARTS,        /* Arts/Culture (without music) */
    EVCTX_CATEGORY_SOC_POL_EC,  /* Social/Political issues/Economics */
    EVCTX_CATEGORY_ED_SC_FACT,  /* Education/Science/Factual topics */
    EVCTX_CATEGORY_LEISURE,     /* Leisure hobbies */
    EVCTX_CATEGORY_SPECIAL,      /* Special characteristics */
    /*For AUS used<<<*/
    EVCTX_CATEGORY_COMEDY,      /* AUS Comedy */
    EVCTX_CATEGORY_DRAMA,       /* AUS Drama */
    EVCTX_CATEGORY_DOCUMENTARY  /* AUS Documentary */
    /*For AUS used>>>*/


#if 0    
#ifdef MW_DVBT_AUS_SUPPORT
    EVCTX_CATEGORY_SPECIAL,     /* Special characteristics */
    EVCTX_CATEGORY_COMEDY,      /* AUS Comedy */
    EVCTX_CATEGORY_DRAMA,       /* AUS Drama */
    EVCTX_CATEGORY_DOCUMENTARY  /* AUS Documentary */
#else /* MW_DVBT_AUS_SUPPORT */
    EVCTX_CATEGORY_SPECIAL      /* Special characteristics */
#endif /* MW_DVBT_AUS_SUPPORT */
#endif

#ifdef MW_ISDB_BRA_SUPPORT    /*for ISDB Brazil*/
    ,
    EVCTX_CATEGORY_BRA_NEWS_1 = 0        ,
    EVCTX_CATEGORY_BRA_SPORTS_2       ,
    EVCTX_CATEGORY_BRA_EDUCATION_3    ,
    EVCTX_CATEGORY_BRA_SOAP_OPERA_4   ,
    EVCTX_CATEGORY_BRA_MINI_SERIES_5  ,
    EVCTX_CATEGORY_BRA_SERIES_6       ,
    EVCTX_CATEGORY_BRA_VARIETY_7      ,
    EVCTX_CATEGORY_BRA_REALITY_SHOW_8 ,
    EVCTX_CATEGORY_BRA_INFOMATION_9   ,
    EVCTX_CATEGORY_BRA_COMICAL_10     ,
    EVCTX_CATEGORY_BRA_CHILDREN_11    ,
    EVCTX_CATEGORY_BRA_EROTIC_12      ,
    EVCTX_CATEGORY_BRA_MOVIE_13       ,
    EVCTX_CATEGORY_BRA_RA_TE_SA_PR_14 ,
    EVCTX_CATEGORY_BRA_DEBATE_INTERVIEW_15 ,
    EVCTX_CATEGORY_BRA_OTHER_16,
    EVCTX_CATEGORY_BRA_UNKNOWN
#endif
} EVCTX_CATEGORY_T;

#define EVCTX_SUB_TYPE_ALL          ((UINT16)  0xffff)


/* Command codes used to load events */
typedef UINT16  EVCTX_COMMAND_CODE_T;

#define EVCTX_CMD_END               ((EVCTX_COMMAND_CODE_T) 0)
#define EVCTX_CMD_SVL_ID            ((EVCTX_COMMAND_CODE_T) 1)
#define EVCTX_CMD_SVL_NAME          ((EVCTX_COMMAND_CODE_T) 2)
#define EVCTX_CMD_SVL_REC_ID        ((EVCTX_COMMAND_CODE_T) 3)
#define EVCTX_CMD_SRC_INFO          ((EVCTX_COMMAND_CODE_T) 4)
#define EVCTX_CMD_BRDCST_TYPE       ((EVCTX_COMMAND_CODE_T) 5)
#define EVCTX_CMD_COUNTRY_CODE      ((EVCTX_COMMAND_CODE_T) 6)
#define EVCTX_CMD_REGION_ID         ((EVCTX_COMMAND_CODE_T) 7)
#define EVCTX_CMD_EVENT_CATEGORY    ((EVCTX_COMMAND_CODE_T) 8)
#define EVCTX_CMD_EVENT_SUB_TYPE    ((EVCTX_COMMAND_CODE_T) 9)
#define EVCTX_CMD_DEV_TYPE          ((EVCTX_COMMAND_CODE_T) 10)
#define EVCTX_CMD_TABLE_OBJ         ((EVCTX_COMMAND_CODE_T) 11)
#define EVCTX_CMD_NO_TABLE_TIMEOUT  ((EVCTX_COMMAND_CODE_T) 12)
#define EVCTX_CMD_CONN_HANDLE       ((EVCTX_COMMAND_CODE_T) 13)
#define EVCTX_CMD_SVL_REC           ((EVCTX_COMMAND_CODE_T) 14)
#define EVCTX_CMD_TSL_REC           ((EVCTX_COMMAND_CODE_T) 15)


/* Event Offset types */
typedef UINT8   EVCTX_OFFSET_TYPE_T;

#define EVCTX_OFFSET_TYPE_INDEX     ((EVCTX_OFFSET_TYPE_T)  0)
#define EVCTX_OFFSET_TYPE_TIME      ((EVCTX_OFFSET_TYPE_T)  1)

/* Table source types */
typedef UINT16   EVCTX_TABLE_SOURCE_TYPE_T;

#define EVCTX_TABLE_SOURCE_TYPE_P     ((EVCTX_TABLE_SOURCE_TYPE_T)  0)
#define EVCTX_TABLE_SOURCE_TYPE_F     ((EVCTX_TABLE_SOURCE_TYPE_T)  1)
#define EVCTX_TABLE_SOURCE_TYPE_S     ((EVCTX_TABLE_SOURCE_TYPE_T)  2)



/*------------------------------------------------------------------*/
/*! @enum    EVCTX_KEY_TYPE_T
 *  @brief   Key types to get event information
 
 *  @code
 * typedef enum
 * {
 *     EVCTX_KEY_TYPE_UNKNOWN = 0,
 *     EVCTX_KEY_TYPE_EVENT_ID,
 *     EVCTX_KEY_TYPE_START_TIME,          
 *     EVCTX_KEY_TYPE_DURATION,            
 *     EVCTX_KEY_TYPE_EVENT_TITLE,         
 *     EVCTX_KEY_TYPE_EVENT_TEXT,          
 *     EVCTX_KEY_TYPE_SVC_NAME,            
 *     EVCTX_KEY_TYPE_SVC_TEXT,            
 *     EVCTX_KEY_TYPE_CAPTION,             
 *     EVCTX_KEY_TYPE_RATING,              
 *     EVCTX_KEY_TYPE_ENG_SPECIFIC,        
 *     EVCTX_KEY_TYPE_EVENT_CATEGORY,      
 *     EVCTX_KEY_TYPE_EVENT_TITLE_STYLE,   
 *     EVCTX_KEY_TYPE_EVENT_TEXT_STYLE,    
 *     EVCTX_KEY_TYPE_EVENT_EXT_TEXT,      
 *     EVCTX_KEY_TYPE_EVENT_EXT_TEXT_STYLE,
 *     EVCTX_KEY_TYPE_CA_SYSTEMS,          
 *     EVCTX_KEY_TYPE_EVENT_LINK,          
 * 
 *     EVCTX_KEY_TYPE_NUM_EVENTS,          
 *     EVCTX_KEY_TYPE_IDX_EVENT_ID,        
 *     EVCTX_KEY_TYPE_IDX_START_TIME,      
 *     EVCTX_KEY_TYPE_IDX_DURATION,        
 *     EVCTX_KEY_TYPE_IDX_EVENT_TITLE,     
 *     EVCTX_KEY_TYPE_IDX_EVENT_TEXT,      
 *     EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT,  
 *     EVCTX_KEY_TYPE_IDX_CAPTION,         
 *     EVCTX_KEY_TYPE_IDX_RATING,          
 *     EVCTX_KEY_TYPE_IDX_EVENT_CATEGORY,  
 *     EVCTX_KEY_TYPE_IDX_EVENT_TITLE_STYLE,   
 *     EVCTX_KEY_TYPE_IDX_EVENT_TEXT_STYLE,    
 *     EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT_STYLE,
 *     EVCTX_KEY_TYPE_IDX_CA_SYSTEMS,      
 *     EVCTX_KEY_TYPE_IDX_EVENT_LINK,
 * 
 *     EVCTX_KEY_TYPE_ELAPSED_TIME,        
 *     
 * #ifdef MW_ISDB_SUPPORT
 *     EVCTX_KEY_TYPE_DIGITAL_COPY_CONTROL,
 *     EVCTX_KEY_TYPE_AUDIO_COMPONENT,
 *     EVCTX_KEY_TYPE_DATA_CONTENTS,
 *     EVCTX_KEY_TYPE_CA_CONTRACT_INFO,
 *     EVCTX_KEY_TYPE_EVENT_GROUP,
 *     EVCTX_KEY_TYPE_COMPONENT_GROUP,
 *     EVCTX_KEY_TYPE_EVENT_LOCATION,
 *     EVCTX_KEY_TYPE_EVENT_EXT_ITEM_TEXT,
 *     EVCTX_KEY_TYPE_COMPONENT,
 *     EVCTX_KEY_TYPE_SERIES,
 * 
 *     EVCTX_KEY_TYPE_IDX_DIGITAL_COPY_CONTROL,
 *     EVCTX_KEY_TYPE_IDX_AUDIO_COMPONENT,
 *     EVCTX_KEY_TYPE_IDX_DATA_CONTENTS,
 *     EVCTX_KEY_TYPE_IDX_CA_CONTRACT_INFO,
 *     EVCTX_KEY_TYPE_IDX_EVENT_GROUP,
 *     EVCTX_KEY_TYPE_IDX_COMPONENT_GROUP,
 *     EVCTX_KEY_TYPE_IDX_EVENT_LOCATION,
 *     EVCTX_KEY_TYPE_IDX_EVENT_EXT_ITEM_TEXT,
 *     EVCTX_KEY_TYPE_IDX_COMPONENT,
 *     EVCTX_KEY_TYPE_IDX_SERIES,
 * #endif
 *     
 *     EVCTX_KEY_TYPE_TOTAL_NUMBER
 *  @endcode

 *  @li@c   EVCTX_KEY_TYPE_UNKNOWN                    - UNKNOWN        
 *  @li@c   EVCTX_KEY_TYPE_EVENT_ID                   - EVENT ID
 *  @li@c   EVCTX_KEY_TYPE_START_TIME                 - START TIME              
 *  @li@c   EVCTX_KEY_TYPE_DURATION                   - DURATION                
 *  @li@c   EVCTX_KEY_TYPE_EVENT_TITLE                - EVENT TITLE             
 *  @li@c   EVCTX_KEY_TYPE_EVENT_TEXT                 - EVENT TEXT              
 *  @li@c   EVCTX_KEY_TYPE_SVC_NAME                   - SVC NAME                
 *  @li@c   EVCTX_KEY_TYPE_SVC_TEXT                   - SVC TEXT                
 *  @li@c   EVCTX_KEY_TYPE_CAPTION                    - CAPTION                 
 *  @li@c   EVCTX_KEY_TYPE_RATING                     - RATING                  
 *  @li@c   EVCTX_KEY_TYPE_ENG_SPECIFIC               - ENG SPECIFIC            
 *  @li@c   EVCTX_KEY_TYPE_EVENT_CATEGORY             - EVENT CATEGORY          
 *  @li@c   EVCTX_KEY_TYPE_EVENT_TITLE_STYLE          - EVENT TITLE_STYLE       
 *  @li@c   EVCTX_KEY_TYPE_EVENT_TEXT_STYLE           - EVENT TEXT_STYLE        
 *  @li@c   EVCTX_KEY_TYPE_EVENT_EXT_TEXT             - EVENT EXT_TEXT          
 *  @li@c   EVCTX_KEY_TYPE_EVENT_EXT_TEXT_STYLE       - EVENT EXT_TEXT_STYLE    
 *  @li@c   EVCTX_KEY_TYPE_CA_SYSTEMS                 - CA SYSTEMS              
 *  @li@c   EVCTX_KEY_TYPE_EVENT_LINK                 - EVENT LINKAGE
 *  @li@c   EVCTX_KEY_TYPE_NUM_EVENTS                 - NUM EVENTS              
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_ID               - IDX EVENT_ID            
 *  @li@c   EVCTX_KEY_TYPE_IDX_START_TIME             - IDX START_TIME          
 *  @li@c   EVCTX_KEY_TYPE_IDX_DURATION               - IDX DURATION            
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_TITLE            - IDX EVENT TITLE         
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_TEXT             - IDX EVENT TEXT          
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT         - IDX EVENT EXT TEXT      
 *  @li@c   EVCTX_KEY_TYPE_IDX_CAPTION                - IDX CAPTION             
 *  @li@c   EVCTX_KEY_TYPE_IDX_RATING                 - IDX RATING              
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_CATEGORY         - IDX EVENT CATEGORY      
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_TITLE_STYLE      - IDX EVENT TITLE STYLE   
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_TEXT_STYLE       - IDX EVENT TEXT STYLE    
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT_STYLE   - IDX EVENT EXT TEXT STYLE
 *  @li@c   EVCTX_KEY_TYPE_IDX_CA_SYSTEMS             - IDX CA_SYSTEMS    
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_LINK             - EVENT LINKAGE
 *  @li@c   EVCTX_KEY_TYPE_ELAPSED_TIME               - ELAPSED TIME            
 *  @li@c   EVCTX_KEY_TYPE_DIGITAL_COPY_CONTROL       - DIGITAL COPY CONTROL    
 *  @li@c   EVCTX_KEY_TYPE_AUDIO_COMPONENT            - AUDIO COMPONENT         
 *  @li@c   EVCTX_KEY_TYPE_DATA_CONTENTS              - DATA CONTENTS           
 *  @li@c   EVCTX_KEY_TYPE_CA_CONTRACT_INFO           - CA CONTRACT_INFO        
 *  @li@c   EVCTX_KEY_TYPE_EVENT_GROUP                - EVENT GROUP             
 *  @li@c   EVCTX_KEY_TYPE_COMPONENT_GROUP            - COMPONENT GROUP         
 *  @li@c   EVCTX_KEY_TYPE_EVENT_LOCATION             - EVENT LOCATION          
 *  @li@c   EVCTX_KEY_TYPE_EVENT_EXT_ITEM_TEXT        - EVENT EXT ITEM TEXT     
 *  @li@c   EVCTX_KEY_TYPE_COMPONENT                  - COMPONENT               
 *  @li@c   EVCTX_KEY_TYPE_SERIES                     - SERIES                  
 *  @li@c   EVCTX_KEY_TYPE_IDX_DIGITAL_COPY_CONTROL   - IDX DIGITAL COPY CONTROL
 *  @li@c   EVCTX_KEY_TYPE_IDX_AUDIO_COMPONENT        - IDX AUDIO COMPONENT     
 *  @li@c   EVCTX_KEY_TYPE_IDX_DATA_CONTENTS          - IDX DATA CONTENTS       
 *  @li@c   EVCTX_KEY_TYPE_IDX_CA_CONTRACT_INFO       - IDX CA CONTRACT INFO    
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_GROUP            - IDX EVENT GROUP         
 *  @li@c   EVCTX_KEY_TYPE_IDX_COMPONENT_GROUP        - IDX COMPONENT GROUP     
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_LOCATION         - IDX EVENT LOCATION      
 *  @li@c   EVCTX_KEY_TYPE_IDX_EVENT_EXT_ITEM_TEXT    - IDX EVENT EXT ITEM TEXT 
 *  @li@c   EVCTX_KEY_TYPE_IDX_COMPONENT              - IDX COMPONENT           
 *  @li@c   EVCTX_KEY_TYPE_IDX_SERIES                 - IDX SERIES              
 *  @li@c   EVCTX_KEY_TYPE_TOTAL_NUMBER               - TOTAL NUMBER
 */
/*------------------------------------------------------------------*/
/* Key types to get event information */
typedef enum
{
    EVCTX_KEY_TYPE_UNKNOWN = 0,
    EVCTX_KEY_TYPE_EVENT_ID,            /* UINT16 */
    EVCTX_KEY_TYPE_START_TIME,          /* TIME_T */
    EVCTX_KEY_TYPE_DURATION,            /* TIME_T */
    EVCTX_KEY_TYPE_FREE_CA_MODE,        /* UINT8  */
    EVCTX_KEY_TYPE_EVENT_TITLE,         /* UTF-8 string */
    EVCTX_KEY_TYPE_EVENT_TEXT,          /* UTF-8 string */
    EVCTX_KEY_TYPE_SVC_NAME,            /* UTF-8 string */
    EVCTX_KEY_TYPE_SVC_TEXT,            /* UTF-8 string */
    EVCTX_KEY_TYPE_STREAM_CONTENT,      /* UINT8 */
    EVCTX_KEY_TYPE_COMPONENT_TYPE,      /* UINT8 */
    EVCTX_KEY_TYPE_CAPTION,             /* BOOLEAN */
    EVCTX_KEY_TYPE_RATING,              /* protocol-defined structure */
    EVCTX_KEY_TYPE_ENG_SPECIFIC,        /* engine specific information */
    EVCTX_KEY_TYPE_EVENT_CATEGORY,      /* Event type/category */
    EVCTX_KEY_TYPE_EVENT_TITLE_STYLE,   /* STYLE_STR_T */
    EVCTX_KEY_TYPE_EVENT_PRIVATE,       /* Event private data */
    EVCTX_KEY_TYPE_EVENT_TEXT_STYLE,    /* STYLE_STR_T */
    EVCTX_KEY_TYPE_EVENT_EXT_TEXT,      /* UTF-8 string */
    EVCTX_KEY_TYPE_EVENT_EXT_TEXT_STYLE,/* STYLE_STR_T */
    EVCTX_KEY_TYPE_CA_SYSTEMS,          /* UINT16 array */
    EVCTX_KEY_TYPE_EVENT_LINK,          /* protocol-defined structure*/

#ifdef MW_ISDB_SUPPORT /* for ISDB  */
            EVCTX_KEY_TYPE_DIGITAL_COPY_CONTROL,
            EVCTX_KEY_TYPE_AUDIO_COMPONENT,
            EVCTX_KEY_TYPE_DATA_CONTENTS,
            EVCTX_KEY_TYPE_CA_CONTRACT_INFO,
            EVCTX_KEY_TYPE_EVENT_GROUP,
            EVCTX_KEY_TYPE_COMPONENT_GROUP,
            EVCTX_KEY_TYPE_EVENT_LOCATION,
            EVCTX_KEY_TYPE_EVENT_EXT_ITEM_TEXT,
            EVCTX_KEY_TYPE_COMPONENT,
            EVCTX_KEY_TYPE_SERIES,
#endif

    EVCTX_KEY_TYPE_NUM_EVENTS,          /* UINT16 */
    EVCTX_KEY_TYPE_IDX_EVENT_ID,        /* UINT16 */
    EVCTX_KEY_TYPE_IDX_START_TIME,      /* TIME_T */
    EVCTX_KEY_TYPE_IDX_DURATION,        /* TIME_T */
    EVCTX_KEY_TYPE_IDX_FREE_CA_MODE,        /* UINT8  */
    EVCTX_KEY_TYPE_IDX_EVENT_TITLE,     /* UTF-8 string */
    EVCTX_KEY_TYPE_IDX_EVENT_TEXT,      /* UTF-8 string */
    EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT,  /* UTF-8 string */
    EVCTX_KEY_TYPE_IDX_CAPTION,         /* BOOLEAN */
    EVCTX_KEY_TYPE_IDX_STREAM_CONTENT,  /* UINT8 */
    EVCTX_KEY_TYPE_IDX_COMPONENT_TYPE,  /* UINT8 */
    EVCTX_KEY_TYPE_IDX_RATING,          /* protocol-defined structure */
    EVCTX_KEY_TYPE_IDX_EVENT_CATEGORY,  /* Event type/category */
    EVCTX_KEY_TYPE_IDX_EVENT_PRIVATE,  /* Event private data*/
    EVCTX_KEY_TYPE_IDX_EVENT_TITLE_STYLE,   /* STYLE_STR_T */
    EVCTX_KEY_TYPE_IDX_EVENT_TEXT_STYLE,    /* STYLE_STR_T */
    EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT_STYLE,/* STYLE_STR_T */
    EVCTX_KEY_TYPE_IDX_CA_SYSTEMS,      /* UINT16 array */
    EVCTX_KEY_TYPE_IDX_EVENT_LINK,          /* protocol-defined structure*/
    EVCTX_KEY_TYPE_IDX_SOURCE_TABLE,          /* to check the source of events. EIT-pf or EIT-s*/    
    
    EVCTX_KEY_TYPE_ELAPSED_TIME,        /* TIME_T */
    
#ifdef MW_ISDB_SUPPORT /* for ISDB  */
    EVCTX_KEY_TYPE_IDX_DIGITAL_COPY_CONTROL,    /* For ISDB */
    EVCTX_KEY_TYPE_IDX_AUDIO_COMPONENT,
    EVCTX_KEY_TYPE_IDX_DATA_CONTENTS,
    EVCTX_KEY_TYPE_IDX_CA_CONTRACT_INFO,
    EVCTX_KEY_TYPE_IDX_EVENT_GROUP,
    EVCTX_KEY_TYPE_IDX_COMPONENT_GROUP,
    EVCTX_KEY_TYPE_IDX_EVENT_LOCATION,
    EVCTX_KEY_TYPE_IDX_EVENT_EXT_ITEM_TEXT,
    EVCTX_KEY_TYPE_IDX_COMPONENT,
    EVCTX_KEY_TYPE_IDX_SERIES,
#endif
    
    EVCTX_KEY_TYPE_TOTAL_NUMBER     /* total number of key types */
} EVCTX_KEY_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct EVCTX_KEY_TYPE_MASK_T
 *  @brief  Key mask structure
 
 *  @code
 * typedef struct _EVCTX_KEY_TYPE_MASK_T
 * {
 *     UINT8   at_masks[(EVCTX_KEY_TYPE_TOTAL_NUMBER+7)/8];
 * } EVCTX_KEY_TYPE_MASK_T;
 *  @endcode
 
 *  @li@c    at_masks            - array of masks
 */
/*------------------------------------------------------------------*/
/* key type mask definitions & macros */
typedef struct _EVCTX_KEY_TYPE_MASK_T
{
    UINT8   at_masks[(EVCTX_KEY_TYPE_TOTAL_NUMBER+7)/8];
} EVCTX_KEY_TYPE_MASK_T;

#define EVCTX_INIT_KEY_TYPE_MAKE(_pt_mask)              \
    {                                                   \
        UINT32      ui4_mask_idx;                       \
                                                        \
        for (ui4_mask_idx = 0 ; ui4_mask_idx < (UINT32) ((EVCTX_KEY_TYPE_TOTAL_NUMBER+7)/8); ui4_mask_idx++)   \
        {                                               \
            (_pt_mask)->at_masks[ui4_mask_idx] = 0;     \
        }                                               \
    }
    
#define EVCTX_ADD_KEY_TYPE_MASK(_pt_mask, _e_key_type)  \
    {                                                   \
        (_pt_mask)->at_masks[(_e_key_type)/8] |= (((UINT8)1) << ((_e_key_type)%8));    \
    }
    
#define EVCTX_DEL_KEY_TYPE_MASK(_pt_mask, _e_key_type)  \
    {                                                   \
        (_pt_mask)->at_masks[(_e_key_type)/8] &= ~((((UINT8)1) << ((_e_key_type)%8))); \
    }

#define EVCTX_CHK_KEY_TYPE_MASK(_pt_mask, _e_key_type)  \
    ((_pt_mask)->at_masks[(_e_key_type)/8] & (((UINT8)1) << ((_e_key_type)%8)))


/*------------------------------------------------------------------*/
/*! @struct    EVCTX_COMMAND_T
 *  @brief  Command structure used to load events
 
 *  @code
 * typedef struct _EVCTX_COMMAND_T
 * {
 *     EVCTX_COMMAND_CODE_T    e_code;
 *     union
 *     {
 *         UINT16              ui2_number;
 *         const CHAR*         ps_name;
 *         BRDCST_TYPE_T       e_brdcst_type;
 *         EVCTX_CATEGORY_T    e_evt_type;
 *         DEVICE_TYPE_T       e_dev_type;
 *         HANDLE_T            h_obj;
 *         SVL_REC_T*          pt_svl_rec;
 *         TSL_REC_T*          pt_tsl_rec;
 *     } u;
 * } EVCTX_COMMAND_T; *  @endcode
 *  @endcode

 *  @li@c    e_code          - command code
 *  @li@c    u               - union of command data
 */
/*------------------------------------------------------------------*/
/* Command structure used to load events */
typedef struct _EVCTX_COMMAND_T
{
    EVCTX_COMMAND_CODE_T    e_code;
    union
    {
        UINT16              ui2_number;
        const CHAR*         ps_name;
        BRDCST_TYPE_T       e_brdcst_type;
        EVCTX_CATEGORY_T    e_evt_type;
        DEVICE_TYPE_T       e_dev_type;
        HANDLE_T            h_obj;
        SVL_REC_T*          pt_svl_rec;
        TSL_REC_T*          pt_tsl_rec;
    } u;
} EVCTX_COMMAND_T;

/*------------------------------------------------------------------*/
/*! @struct    EVCTX_OFFSET_T
 *  @brief  Event offset structure
 
 *  @code
 * typedef struct _EVCTX_OFFSET_T
 * {
 *     EVCTX_OFFSET_TYPE_T     e_offset_type;
 *     union
 *     {
 *         INT16               i2_index;
 *         TIME_T              t_time_delta;
 *     } u_offset;
 *     TIME_T                  t_time_start;
 * } EVCTX_OFFSET_T;
 *  @endcode
 
 *  @li@c    e_offset_type       - the evctx offset type
 *  @li@c    u_offset            - union of offset data
 *  @li@c    t_time_start        - the start time
 */
/*------------------------------------------------------------------*/
/* Event offset structure */
typedef struct _EVCTX_OFFSET_T
{
    EVCTX_OFFSET_TYPE_T     e_offset_type;
    union
    {
        INT16               i2_index;
        TIME_T              t_time_delta;
    } u_offset;
    TIME_T                  t_time_start;
} EVCTX_OFFSET_T;

/*------------------------------------------------------------------*/
/*! @struct EVCTX_BRIEF_INFO_T
 *  @brief  Event Brief information structure
 
 *  @code
 * typedef struct _EVCTX_BRIEF_INFO_T
 * {
 *     TIME_T      t_time_start;
 *     TIME_T      t_time_duration;
 *     CHAR*       ps_event_title;
 * } EVCTX_BRIEF_INFO_T;
 *  @endcode
 
 *  @li@c    t_time_start              - the event start time
 *  @li@c    t_time_duration           - the event duration
 *  @li@c    ps_event_title            - the event title
 */
/*------------------------------------------------------------------*/
/* Brief information structure */
typedef struct _EVCTX_BRIEF_INFO_T
{
    TIME_T      t_time_start;
    TIME_T      t_time_duration;
    CHAR*       ps_event_title;
} EVCTX_BRIEF_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct EVCTX_EVT_CAT_T
 *  @brief  Event type/category structure
 
 *  @code
 * typedef struct _EVCTX_EVT_CAT_T
 * {
 *     EVCTX_CATEGORY_T    e_type;
 *     UINT16              ui2_data;
 * } EVCTX_EVT_CAT_T;
 *  @endcode
 
 *  @li@c    e_type             - the event category type
 *  @li@c    ui2_data           - the event category data
 */
/*------------------------------------------------------------------*/
/* Event type/category structure */
typedef struct _EVCTX_EVT_CAT_T
{
    EVCTX_CATEGORY_T    e_type;
    UINT16              ui2_data;
} EVCTX_EVT_CAT_T;

/*------------------------------------------------------------------*/
/*! @struct EVCTX_EVT_CAT_LIST_T
 *  @brief  Event category list  structure
 
 *  @code
 * typedef struct _EVCTX_EVT_CAT_LIST_T
 * {
 *     BRDCST_TYPE_T       e_brdcst_type;
 *     UINT16              ui2_num_types;
 *     EVCTX_EVT_CAT_T*    pt_types;
 * } EVCTX_EVT_CAT_LIST_T;
 *  @endcode
 
 *  @li@c    e_brdcst_type      - broadcast type
 *  @li@c    ui2_num_types      - number of event type/category in the list
 *  @li@c    pt_types           - pointer to the array of event type/category structure
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_EVT_CAT_LIST_T
{
    BRDCST_TYPE_T       e_brdcst_type;
    UINT16              ui2_num_types;
    EVCTX_EVT_CAT_T*    pt_types;
} EVCTX_EVT_CAT_LIST_T;

/*------------------------------------------------------------------*/
/*! @struct EVCTX_RATING_T
 *  @brief  Event rating
 
 *  @code
 * typedef struct _EVCTX_RATING_T
 * {
 *     UINT16      ui2_dimension;
 *     UINT16      ui2_rating_value;
 * } EVCTX_RATING_T;
 *  @endcode
 
 *  @li@c    ui2_dimension      - the rating dimension
 *  @li@c    ui2_rating_value   - the rating value
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_RATING_T
{
    UINT16      ui2_dimension;
    UINT16      ui2_rating_value;
} EVCTX_RATING_T;


/*
    pt_ratings is a pointer to an array of EVCTX_RATING_T type data.
    ui2_rated_dimensions contains the number of elements in the pt_ratings
    array.
    
    The following is an illustration of the layout, which a real instance
    of EVCTX_RATING_LIST_T may be.
    
    Arrows in the left side shows that the pt_ratings array is allocated
    exactly below the pt_ratings field, and followed by the rating 
    description string.
    
    Arrows in the right side shows how to calculate the total size of
    an instance of EVCTX_RATING_LIST_T data.
    
        ui2_rating_region           <---
        ui2_rated_dimensions            | sizeof(EVCTX_RATING_LIST_T)
------- ps_rating_description           |
| ----- pt_ratings                      |
| |  -- pt_next                     <---                   +
| |  |
| ----->  pt_ratings[0]                      <---
|    |    pt_ratings[1]                          | ui2_rated_dimensions *
|    |    pt_ratings[2]                          | sizeof(EVCTX_RATING_T)
|    |        :                                  |
|    |    pt_ratings[ui2_rated_dimensions-1] <---          +
--------> "TV-PG-L-S"                        <---  x_strlen(pt_rating_description)+1
     |
     |
     -> ui2_rating_region   (the next region)
        ui2_rated_dimensions
        pt_rating_description
        pt_ratings
        pt_next
*/

/*------------------------------------------------------------------*/
/*! @struct EVCTX_RATING_LIST_T
 *  @brief  Event rating list
 
 *  @code
 * typedef struct _EVCTX_RATING_LIST_T
 * {
 *     UINT16              ui2_rating_region;
 *     UINT16              ui2_rated_dimensions;
 *     CHAR*               ps_rating_descrption;
 *     EVCTX_RATING_T*     pt_ratings;
 * 
 *     struct _EVCTX_RATING_LIST_T*   pt_next;
 * } EVCTX_RATING_LIST_T;
 *  @endcode
 
 *  @li@c    ui2_rating_region      - the rating region
 *  @li@c    ui2_rated_dimensions   - number of dimensions in the list
 *  @li@c    ps_rating_descrption   - pointer of description
 *  @li@c    pt_ratings             - pointer to the array of event rating structure
 *  @li@c    pt_next                - pointer of next rating list
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_RATING_LIST_T
{
    UINT16              ui2_rating_region;
    UINT16              ui2_rated_dimensions;
    CHAR*               ps_rating_descrption;
    EVCTX_RATING_T*     pt_ratings;

    struct _EVCTX_RATING_LIST_T*   pt_next;
} EVCTX_RATING_LIST_T;

/*------------------------------------------------------------------*/
/*! @struct EVCTX_CA_SYSTEM_ID_LIST_T
 *  @brief  Event CA SYSTEM ID LIST
 
 *  @code
 * typedef struct _EVCTX_CA_SYSTEM_ID_LIST_T
 * {
 *     UINT16              ui2_num_ca_ids;
 *     UINT16*             pui2_ca_ids;
 * } EVCTX_CA_SYSTEM_ID_LIST_T;
 *  @endcode
 
 *  @li@c    ui2_num_ca_ids      - Number of CA id in the list
 *  @li@c    pui2_ca_ids         - pointer to the array of CA id
 */
/*------------------------------------------------------------------*/

typedef struct _EVCTX_CA_SYSTEM_ID_LIST_T
{
    UINT16              ui2_num_ca_ids;
    UINT16*             pui2_ca_ids;
} EVCTX_CA_SYSTEM_ID_LIST_T;



/*------------------------------------------------------------------*/
/*! @struct EVCTX_EVENT_LINKAGE_T
 *  @brief  Event linkage 
 
 *  @code
 *  typedef struct _EVCTX_EVENT_LINKAGE_T
 *  {
 *      UINT16      ui2_on_id;
 *      UINT16      ui2_ts_id;
 *      UINT16      ui2_svc_id;
 *       
 *      UINT16      ui2_target_event_id;
 *      UINT8       ui1_target_listed;
 *      UINT8       ui1_event_simulcast;
 *      UINT8       ui1_reserved;
 *  } EVCTX_EVENT_LINKAGE_T;
 *  @endcode
 
 *  @li@c    ui2_on_id              - The network_id of the originating delivery system
 *  @li@c    ui2_ts_id              - Transport stream id
 *  @li@c    ui2_svc_id             - Service id
 *  @li@c    ui2_target_event_id    - Identifies the event_id of the event (the target event),
 *  @li@c    ui1_target_listed      - When target_listed is set to 1 (one), the service shall be included in the SDT
 *  @li@c    ui1_event_simulcast    - Set to 1 (one) when the target event and the source event are being simulcast.It shall be set to 0 (zero) when the events are offset in time.
 *  @li@c    ui1_reserved           - Rreserved
 */
/*------------------------------------------------------------------*/

typedef struct _EVCTX_EVENT_LINKAGE_T
{
    UINT16      ui2_on_id;
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;
    
    UINT16      ui2_target_event_id;
    UINT8       ui1_target_listed;
    UINT8       ui1_event_simulcast;
    UINT8       ui1_reserved;
} EVCTX_EVENT_LINKAGE_T;



/*------------------------------------------------------------------*/
/*! @struct EVCTX_EVENT_LINKAGE_LIST_T
 *  @brief  Event linkage LIST
 
 *  @code
 * typedef struct _EVCTX_EVENT_LINKAGE_LIST_T
 * {
 *     UINT32                  ui4_num_event_linkage; 
 *     EVCTX_EVENT_LINKAGE_T*  pt_event_linkage;
 * } EVCTX_EVENT_LINKAGE_LIST_T;
 *  @endcode
 
 *  @li@c    ui4_num_event_linkage      - Number event linkage in EIT
 *  @li@c    pt_event_linkage           - Pointer to the array of event linkage
 */
/*------------------------------------------------------------------*/

typedef struct _EVCTX_EVENT_LINKAGE_LIST_T
{
    UINT32                  ui4_num_event_linkage;    
    EVCTX_EVENT_LINKAGE_T*  pt_event_linkage;
} EVCTX_EVENT_LINKAGE_LIST_T;



/*------------------------------------------------------------------*/
/*! @struct EVCTX_IDX_LANG_T
 *  @brief  Event Language ID structure
 
 *  @code
 * typedef struct _EVCTX_IDX_LANG_T
 * {                               
 *     UINT16          ui2_idx;    
 *     ISO_639_LANG_T* pt_lang;    
 * } EVCTX_IDX_LANG_T;             
 *  @endcode
 
 *  @li@c    ui2_idx   - language index
 *  @li@c    pt_lang   - pointer of language structure
 */
/*------------------------------------------------------------------*/
typedef struct _EVCTX_IDX_LANG_T
{
    UINT16          ui2_idx;
    ISO_639_LANG_T* pt_lang;
} EVCTX_IDX_LANG_T;

#ifdef MW_ISDB_SUPPORT /* for isdb */
/******************************************************
* Audio Component Descriptor
******************************************************/
typedef struct _EVCTX_AUDIO_COMPONENT_T
{
    UINT8 ui1_stream_content;
    UINT8 ui1_component_type;
    UINT8 ui1_component_tag;
    UINT8 ui1_stream_type;
    UINT8 ui1_simulcast_grp_tag;
    BOOL  b_es_multi_lingual_flag;
    BOOL  b_main_component_flag;
    UINT8 ui1_quality_indicator;
    UINT8 ui1_sample_rate;
    ISO_639_LANG_T t_audio_lang1;
    CHAR* ps_audio_name1;
    ISO_639_LANG_T t_audio_lang2;
    CHAR* ps_audio_name2; 
    struct _EVCTX_AUDIO_COMPONENT_T* pt_next;
} EVCTX_AUDIO_COMPONENT_T;


/******************************************************
* Digital Copy Control Descriptor
******************************************************/
typedef struct _EVCTX_COMPONENT_CONTROL_LIST_T
{
    UINT8 ui1_component_tag;
    UINT8 ui1_digital_recording_ctrl_data;
    BOOL  b_max_bitrate_flag;
    UINT8 ui1_max_bitrate;     /* Only effect when  is b_max_bitrate_flag true*/
    UINT8 ui1_copy_ctrl_type;
    UINT8 ui1_APS_ctrl_data; /* Only effect when  ui1_copy_ctrl_type is true*/
    struct _EVCTX_COMPONENT_CONTROL_LIST_T* pt_next;
} EVCTX_COMPONENT_CONTROL_LIST_T;

typedef struct _EVCTX_DIGITAL_COPY_CONTROL_T
{
    UINT8 ui1_digital_recording_ctrl_data;
    BOOL  b_max_bitrate_flag;    
    UINT8 ui1_max_bitrate;     /* Only effect when  b_max_bitrate_flag is true*/
    UINT8 ui1_copy_ctrl_type;
    UINT8 ui1_APS_ctrl_data;    /* Only effect when  ui1_copy_ctrl_type is true*/
    BOOL  b_component_control_flag;
    EVCTX_COMPONENT_CONTROL_LIST_T* pt_component_ctrl_list;
} EVCTX_DIGITAL_COPY_CONTROL_T;


/******************************************************
* CA Contract Info Descriptor
******************************************************/
typedef struct _EVCTX_CA_CONTRACT_INFO_T
{
    UINT16 ui2_CA_system_id;
    UINT8   ui1_CA_uint_id;
    UINT8   ui1_component_num;
    UINT8* pui1_component_tags;
    UINT8   ui1_contract_ver_info_len;
    UINT8* pui1_contract_ver_info;
    CHAR*  ps_fee_name;
} EVCTX_CA_CONTRACT_INFO_T;


/******************************************************
* Event Group Descriptor
******************************************************/
typedef struct _EVCTX_SERVICE_EVENT_ID_T
{
    UINT16 ui2_service_id;
    UINT16 ui2_event_id;
} EVCTX_SERVICE_EVENT_ID_T;

typedef struct _EVCTX_ON_TS_SVC_EVT_ID_T
{
    UINT16 ui2_on_id;
    UINT16 ui2_ts_id;
    UINT16 ui2_service_id;    
    UINT16 ui2_event_id;    
} EVCTX_ON_TS_SVC_EVT_ID_T;

typedef struct _EVCTX_EVENT_GROUP_T
{
    UINT8 ui1_group_type;
    UINT8 ui1_event_count;
    UINT16  ui2_current_ts_id;
    EVCTX_SERVICE_EVENT_ID_T* pt_svc_event_ids;
    UINT8 ui1_other_network_event_count;
    EVCTX_ON_TS_SVC_EVT_ID_T* pt_on_ts_svc_event_ids;
} EVCTX_EVENT_GROUP_T;


/******************************************************
* Component Group Descriptor
******************************************************/
typedef struct _EVCTX_CA_UNIT_INFO_LIST_T
{
    UINT8   ui1_ca_unit_id;
    UINT8   ui1_component_num;
    UINT8* pui1_component_tags;
    struct _EVCTX_CA_UNIT_INFO_LIST_T* pt_next;
} EVCTX_CA_UNIT_INFO_LIST_T;

typedef struct _EVCTX_COMPONENT_GROUP_INFO_LIST_T
{
    UINT8 ui1_component_group_id;
    INT16 i2_total_bit_rate;     /* Only effect when i2_total_bit_rate is greater than or equal to 0*/
    CHAR* ps_description;
    EVCTX_CA_UNIT_INFO_LIST_T* pt_ca_unit_info_list;
    struct _EVCTX_COMPONENT_GROUP_INFO_LIST_T* pt_next;
} EVCTX_COMPONENT_GROUP_INFO_LIST_T;

typedef struct _EVCTX_COMPONENT_GROUP_T
{
    UINT8 ui1_component_group_type;
    BOOL  b_total_bit_rate_flag;
    EVCTX_COMPONENT_GROUP_INFO_LIST_T* pt_component_group_list;
} EVCTX_COMPONENT_GROUP_T;

/******************************************************
* Component Descriptor
******************************************************/
typedef struct _EVCTX_COMPONENT_T
{
    UINT8 ui1_stream_content;
    UINT8 ui1_component_type;
    UINT8 ui1_component_tag;
    ISO_639_LANG_T t_component_lang;
    CHAR* ps_component_name;
} EVCTX_COMPONENT_T;

/******************************************************
* Series Decriptor
******************************************************/
typedef struct _EVCTX_SERIES_T
{
    UINT16 ui2_series_id;
    UINT8   ui1_repeat_label;
    UINT8   ui1_program_pattern;
    BOOL    b_expire_date_valid_flag;
    UINT16 ui2_expire_date;
    UINT16 ui2_episode_num;
    UINT16 ui2_last_episode_num;
    CHAR*  ps_series_name;
} EVCTX_SERIES_T;


/******************************************************
* Extended Event Descriptor
*****************************************************/
typedef struct _EVCTX_EXT_EVENT_ITEM_T
{
    CHAR* ps_item_description;
    CHAR* ps_item_text;
    struct _EVCTX_EXT_EVENT_ITEM_T* pt_next;
} EVCTX_EXT_EVENT_ITEM_T;

typedef struct _EVCTX_EXT_EVENT_ITEM_LIST_T
{
    UINT8 ui1_item_num;
    ISO_639_LANG_T* pt_lang;
    EVCTX_EXT_EVENT_ITEM_T* pt_item;
} EVCTX_EXT_EVENT_ITEM_LIST_T;


/******************************************************
* Data Contents Descriptor
******************************************************/
typedef struct _EVCTX_ARIB_CAROUSEL_INFO_LIST_T
{
    UINT8 ui1_component_tag;
    BOOL b_event_section_flag;
    BOOL b_component_size_flag;
    UINT32 ui4_component_size;            /* Only effect when b_component_size_flag is true*/
    BOOL b_default_transaction_id_flag;
    UINT32 ui4_transaction_id;            /* Only effect when b_default_transaction_id_flag is true*/
    BOOL b_default_timeout_DII_flag;
    UINT32 ui4_timeout_value_dii;        /* Only effect when b_default_timeout_DII_flag is true*/
    BOOL b_default_leak_rate_flag;
    UINT32 ui4_leak_rate;                 /* Only effect when b_default_leak_rate_flag is true*/
    struct _EVCTX_ARIB_CAROUSEL_INFO_LIST_T* pt_next;
} EVCTX_ARIB_CAROUSEL_INFO_LIST_T;

typedef struct _EVCTX_CONTENT_ID_INFO_T
{
    UINT32 ui4_content_id;
    UINT16 ui2_content_version;
} EVCTX_CONTENT_ID_INFO_T;

typedef struct _EVCTX_BXML_VERSION_INFO_T
{
    UINT16 ui2_bxml_major_version;
    UINT16 ui2_bxml_minor_version;
} EVCTX_BXML_VERSION_INFO_T;

typedef struct _EVCTX_BML_VERSION_INFO_T
{
    UINT16 ui2_bml_major_version;
    UINT16 ui2_bml_minor_version;
    EVCTX_BXML_VERSION_INFO_T* pt_bxml_version_info;
} EVCTX_BML_VERSION_INFO_T;

typedef struct _EVCTX_TRANSMISSION_FORMAT_INFO_T
{
    EVCTX_ARIB_CAROUSEL_INFO_LIST_T* pt_arib_carousel_info_list;
    BOOL b_ondemand_retrieval_flag;
    BOOL b_file_storable_flag;
} EVCTX_TRANSMISSION_FORMAT_INFO_T;

typedef struct _EVCTX_ARIB_BXML_INFO_T
{
    UINT8 ui1_transmission_format;
    EVCTX_TRANSMISSION_FORMAT_INFO_T* pt_transmission_format_info;
    BOOL  b_auto_start_flag;
    UINT8 ui1_document_resolution;
    BOOL  b_use_xml;
    BOOL  b_default_version_flag;
    BOOL  b_independent_flag;
    BOOL  b_style_for_tv_flag;
    EVCTX_BML_VERSION_INFO_T* pt_default_version_info;
    BOOL  b_content_id_flag;
    EVCTX_CONTENT_ID_INFO_T* pt_content_id_info;
    BOOL  b_associated_contents_flag;
    BOOL  b_update_flag;
    ISO_639_LANG_T t_lang;
} EVCTX_ARIB_BXML_INFO_T;

typedef struct _EVCTX_DATA_CONTENTS_T
{
    UINT16 ui2_data_component_id;
    UINT8  ui1_entry_component;
    EVCTX_ARIB_BXML_INFO_T t_arib_bxml_info;
    UINT8  ui1_component_ref_num;
    UINT8  pui1_conponent_ref;
    ISO_639_LANG_T t_lang;
    CHAR* ps_content_title;
} EVCTX_DATA_CONTENTS_T;
/* end dyin */
#endif


/*----------------------------------------------------------------------------*/
/*! @brief  evctx notify callback function

 *  @param  [in] h_event        - handle of event object
 *  @param  [in] e_cond         - event condition
 *  @param  [in] pv_nfy_tag     - Notification Tag
 *  @param  [in] ui4_data       - Notification Data
 *  
 *  @note   User should implement such function pointer and register to evctx
 *  @see
 *  @return		VOID
 *  @retval		VOID
 */
/*----------------------------------------------------------------------------*/
/* Callback function type, used to notify applications of 
   the condition changes */
typedef VOID (*x_evctx_nfy_fct) (
    HANDLE_T          h_event,
    EVCTX_COND_T      e_cond,
    VOID*             pv_nfy_tag,
    UINT32            ui4_data);

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_EVCTX               */
/*----------------------------------------------------------------------------*/

#endif /* _U_EVCTX_H_ */

 /*! @page pageMW_EVCTX Event Context Detailed Description
 *  @par
 *  @section secMW_EVCTX_1  1. Introduction
 *  @par
 * 	 Event Context(EVCTX) describes component, which is responsible for retrieving event information by interfacing with Table Manager.
 *  @par
 * 	 This document must be read in conjunction with the document, SW Design Middleware - SW Design Middleware-SVL Rev 1.x.doc and SW Design Middleware-TSL Rev 1.y.doc for more details about serivce lists and transport stream lists.
 *  @par
 *  @section secMW_EVCTX_2  2. Concept
 *  @par
 *  @subsection ssecMW_EVCTX_2_1  2.1. Conditions/states of an event context
 *  @par
 * 	 @image html  Evctx_state_diagram.gif "Figure 2.1 State diagram"
 *  @par
 * 	 Figure 2.1 shows the state or condition diagram of an event context.  The following table describes the individual edges in more details.  All these condition changes will be notified to the application through the API (*x_evctx_nfy_fct) ().
 *  @par
 * 	 <table>
 * 	 <tr>
 * 	 <th>Item</th><th>
 * 	 Descriptionn
 * 	 </th>
 * 	 </tr>
 * 	 <tr>
 * 	 <td>A</td><td>
 * 	 Tables necessary for the requested event(s) are loaded.
 * 	 </td>
 * 	 </tr>
 * 	 <tr>
 * 	 <td>B</td><td>
 * 	 The event becomes active (the current event), updated, or expired.
 * 	 </td>
 * 	 </tr>
 * 	 <tr>
 * 	 <td>C</td><td>
 * 	 The event is not found, or it is handle was closed.
 * 	 </td>
 * 	 </tr>
 * 	 <tr>
 * 	 <td>D</td><td>
 * 	 Failed to load tables.
 * 	 </td>
 * 	 </tr>
 * 	 </table>
 * 	 <center><b></b></center>
 *  @par
 *  @section secMW_EVCTX_3  3. Brief Design
 *  @par
 *      EVCTX component consists of a selector and customizable engines.  The selector provides protocol-agnostic APIs, while the underneath engines provide ways to retrieve events based on different SI protocols.
 *  @par
 *      Each underneah engine interfaces with Table Manager and SVL/TSL databases. Some engines may inteface with other middleware components if necessary.
 *  @par
 *      @image html  Event_context.gif "Figure 3 Event Context"
 *  @par
 *  @subsection ssecMW_EVCTX_3_1  3.1. Functional blocks
 *  @par
 *      The EVCTX component consists essentially of the following functional blocks.
 *      -   Selector
 *      -   EVCTX engine
 *
 *  @par
 *  @subsubsection sssecMW_EVCTX_3_1_1  3.1.1. Selector
 *  @par
 *      The selector passes the protocol-agnostic function calls to a proper EVCTX engine, monitors each engine?¡¥s internal states, and gets the requested information back to applications
 *  @par
 *  @subsubsection sssecMW_EVCTX_3_1_2  3.1.2. Selector
 *  @par
 *      An EVCTX engine should have the capabitlity of interfacing with Table Manager, and SVL/TSL databases.  By registration, a function table (see section 6.2.1) is duplicated in an engine control block (section 5.2.3).  All engine control blocks are maintained in a double-linked list.
 *  @par
 *  @subsection ssecMW_EVCTX_3_2  3.2. EVCTX control structures
 *  @par
 *      Before an application can get an event information, it must create a handle to the respective EVCTX object with the internal handle type EVCTXT_EVENT (see section 7.3.1).  Every event context handle refers to its own EVCTX_BASE_CTRL_T structure (by pv_obj field in the HANDLE_CTRL_T struct). The application  requesting the event context must specify its own notify function with the function type (*x_evctx_nfy_fct), which is called whenever the event context has a state change.
 *  @par
 *      The structure EVCTX_BASE_CTRL_T also contains the engine pointer and condition field (see section 5.2.1).  Figure 7 shows the internal structures of the EVCTX object.
 *  @par
 *      @image html  Evctx_object.gif "Figure 3.2 A handle refers to the EVCTX object"
 *  @par
 *  @subsection ssecMW_EVCTX_3_3  3.3. Notification
 *  @par
 *      The EVCTX component has one kind of notifications:
 *      -   Condition change
 *
 *  @par
 *  @subsubsection sssecMW_EVCTX_3_3_1  3.3.1. Condition change
 *  @par
 *      The callback function (*x_evctx_nfy_fct)() is specified when calling the APIs x_evctx_load_current_event(), x_evctx_load_next_event(), and x_evctx_load_schedule_event(). Every edge in figure 5 is accompanied with a condition change notification.
 *  @par
 *  @subsection ssecMW_EVCTX_3_4  3.4. Program flow
 *  @par
 *      The program flow for loading the current event with an ATSC engine.
 *  @par
 *      @image html  Program_flow_load_current_event.gif "Figure 3.41 Program flow of load current event"
 *  @par
 *      Illustrates the proceeding procedures which are designed to retrieve event information.
 *  @par
 *      @image html  Program_flow_Evctx.gif "Figure 3.42 Program flow of Evctx Content"
 *  @par
 *      <table>
 *      <tr>
 *      <th>Item</th><th>
 *      Descriptionn
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>1</td><td>
 *      The application loads the current event with Event Context using the API x_evctx_load_current_event. The API immediately returns with a handle if no error.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>2</td><td>
 *      Once the EVCTX Selector has determined the engine, it will call the API (*evctx_create_event_obj_fct).
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>3</td><td>
 *      Convert pt_svc_cmd (EVCTX_COMMAND_T type) to SVL id and SVL rec id, get the corresponding SVL record, and then get the Transport Stream ID from the TSL record.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>4</td><td>
 *      Use the API x_atsc_si_load_mgt_by_ts_descr with the argument TS_ID (retrieved by step 3) to load the MGT.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>5</td><td>
 *      Callback function is called with the MGT handle when the MGT is available (loaded).  Send the message to the EVCTX Selector about the MGT information.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>6</td><td>
 *      Callback function is called with the MGT handle when the MGT is available (loaded).  Send the message to the EVCTX Selector about the MGT information.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>7</td><td>
 *      The EVCTX selector determines the message and pass it to the ATSC engine using the API (*evctx_engine_task).
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>8</td><td>
 *      A loop which calls the API x_atsc_si_get_mgt_entry to determine whether TVCT or CVCT is present.  In this example, TVCT is present. Therefore, use the API x_atsc_si_load_tvct with the argument MGT handle (retrieved by step 4) to load it.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>9</td><td>
 *      Callback function is called with the TVCT handle when the TVCT is available.  Send the message to the EVCTX Selector about the TVCT information.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>10</td><td>
 *      Callback function is called with the TVCT handle when the TVCT is available.  Send the message to the EVCTX Selector about the TVCT information.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>11</td><td>
 *      The EVCTX Selector determines the message and pass it to the ATSC engine using the API (*evctx_engine_task).
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>12</td><td>
 *      Use the API x_atsc_si_load_eit_by_src_id with arguments TVCT handle (retrieved by step 8) and current time to load the EIT0.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>13</td><td>
 *      Callback function is called with the EIT0 handle when the EIT0 is available.  Send the message to the EVCTX Selector about the EIT0 information.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>14</td><td>
 *      Callback function is called with the EIT0 handle when the EIT0 is available.  Send the message to the EVCTX Selector about the EIT0 information.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>15</td><td>
 *      The EVCTX Selector determines the message and pass it to the ATSC engine using the API (*evctx_engine_task).
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>16</td><td>
 *      Use the callback function (*x_evctx_nfy_fct) to notfiy the application of the condition change.  When required tables are successfully loaded in this example, the EVCTX_COND_AVAIL condition is sent.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>17</td><td>
 *      Use x_evctx_get_event_info_len and x_evctx_get_event_info to get any type of event information.  The types are described in section 7.1.6.  When calling the API x_evctx_get_event_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_event_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>18</td><td>
 *      Use x_evctx_get_event_info_len and x_evctx_get_event_info to get any type of event information.  The types are described in section 7.1.6.  When calling the API x_evctx_get_event_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_event_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>19</td><td>
 *      Use x_evctx_get_event_info_len and x_evctx_get_event_info to get any type of event information.  The types are described in section 7.1.6.  When calling the API x_evctx_get_event_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_event_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>20</td><td>
 *      Use x_evctx_get_event_info_len and x_evctx_get_event_info to get any type of event information.  The types are described in section 7.1.6.  When calling the API x_evctx_get_event_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_event_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>21</td><td>
 *      The API x_evctx_get_brief_info_len is a help function, which calls 3 times of the API x_evctx_get_event_info_len for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>22</td><td>
 *      The API x_evctx_get_brief_info_len is a help function, which calls 3 times of the API x_evctx_get_event_info_len for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>23</td><td>
 *      The API x_evctx_get_brief_info_len is a help function, which calls 3 times of the API x_evctx_get_event_info_len for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>24</td><td>
 *      The API x_evctx_get_brief_info_len is a help function, which calls 3 times of the API x_evctx_get_event_info_len for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>25</td><td>
 *      The API x_evctx_get_brief_info is a help function, which calls 3 times of the API x_evctx_get_event_info for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.  When calling the API x_evctx_get_brief_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_brief_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>26</td><td>
 *      The API x_evctx_get_brief_info is a help function, which calls 3 times of the API x_evctx_get_event_info for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.  When calling the API x_evctx_get_brief_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_brief_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>27</td><td>
 *      The API x_evctx_get_brief_info is a help function, which calls 3 times of the API x_evctx_get_event_info for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.  When calling the API x_evctx_get_brief_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_brief_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>28</td><td>
 *      The API x_evctx_get_brief_info is a help function, which calls 3 times of the API x_evctx_get_event_info for key types EVCTX_KEY_TYPE_START_TIME, EVCTX_KEY_TYPE_DURATION, and EVCTX_KEY_TYPE_EVENT_TITLE, respectively.  When calling the API x_evctx_get_brief_info, the application must provide memory chunk with the size bigger than or equal to the length output by the API x_evctx_get_brief_info_len.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>29</td><td>
 *      Use the API x_evctx_delete_event to delete the event.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>30</td><td>
 *      Use the API x_evctx_delete_event to delete the event.
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b></b></center>
 *  @par
 *  @subsection ssecMW_EVCTX_3_5  3.5. Event Context engine list
 *  @par
 *      By registration, EVCTX component maintains an engine list, as illustrated in figure 10.  Each node in the list represents an event context engine, which consists of a broadcast type and a function table.  See section 5.2.3 for more details about the EVCTX_ENG_CTRL_T structure, and section 6.2.1 for details about function table structure EVCTX_ENG_FCT_TBL_T.
 *  @par
 *      @image html  Evctx_engine_list.gif "Figure 3.5 Event Context engine list "
 *  @par
 *  @par
 *  @section secMW_EVCTX_4  4. Typical Case
 *  @par
 *  @subsection ssecMW_EVCTX_4_1  4.1. Priority A
 *  @par
 *  @subsubsection sssecMW_EVCTX_4_1_1  4.1.1. How to load the "current" event information? What is the lifecycle of EVCTX event object handle?
 *      c_evctx_load_schedule_event() to load a schedule and current event object.
 *      1.When offset type is EVCTX_OFFSET_TYPE_INDEX, index is 0, it is a current event.
 *      2.When offset type is EVCTX_OFFSET_TYPE_TIME, start time is current time, it is a current event.
 *  @par
 *      When the event is load, the EVCTX event object handle is available.
 *  @par
 *      @include evctx/a1.txt
 *  @par
 *  @subsubsection sssecMW_EVCTX_4_1_2  4.1.2. How to load the "next" event information?
 *      c_evctx_load_next_event() to load a next event object.
 *  @par
 *      @include evctx/a2.txt
 *  @par
 *  @subsubsection sssecMW_EVCTX_4_1_3  4.1.3. How to load the "scheduled" event information?
 *      c_evctx_load_schedule_event() to load a schedule and current event object.
 *      1.When offset type is EVCTX_OFFSET_TYPE_INDEX, index is more than 1, it is a schedule event.
 *      2.When offset type is EVCTX_OFFSET_TYPE_TIME, start time is user set, it is a schedule event.
 *  @par
 *      @include evctx/a3.txt
 *  @par
 *  @subsubsection sssecMW_EVCTX_4_1_4  4.1.4. WWhat kind of information are able to provide from one EVCTX event object handle? How to get such information?
 *      c_evctx_get_event_info() is uesd to load the information of event.
 *  @par
 *      EVCTX_KEY_TYPE_EVENT_ID,
 *      EVCTX_KEY_TYPE_START_TIME,
 *      EVCTX_KEY_TYPE_DURATION,
 *      EVCTX_KEY_TYPE_EVENT_TITLE,
 *      EVCTX_KEY_TYPE_EVENT_TEXT,
 *      EVCTX_KEY_TYPE_SVC_NAME,
 *      EVCTX_KEY_TYPE_SVC_TEXT,
 *      EVCTX_KEY_TYPE_CAPTION,
 *      EVCTX_KEY_TYPE_RATING,
 *      EVCTX_KEY_TYPE_ENG_SPECIFIC,
 *      EVCTX_KEY_TYPE_EVENT_CATEGORY,
 *      EVCTX_KEY_TYPE_EVENT_TITLE_STYLE,
 *      EVCTX_KEY_TYPE_EVENT_TEXT_STYLE,
 *      EVCTX_KEY_TYPE_EVENT_EXT_TEXT,
 *      EVCTX_KEY_TYPE_EVENT_EXT_TEXT_STYLE,
 *      EVCTX_KEY_TYPE_CA_SYSTEMS,
 *      EVCTX_KEY_TYPE_EVENT_LINK,
 *  @par
 *      EVCTX_KEY_TYPE_DIGITAL_COPY_CONTROL,
 *      EVCTX_KEY_TYPE_AUDIO_COMPONENT,
 *      EVCTX_KEY_TYPE_DATA_CONTENTS,
 *      EVCTX_KEY_TYPE_CA_CONTRACT_INFO,
 *      EVCTX_KEY_TYPE_EVENT_GROUP,
 *      EVCTX_KEY_TYPE_COMPONENT_GROUP,
 *      EVCTX_KEY_TYPE_EVENT_LOCATION,
 *      EVCTX_KEY_TYPE_EVENT_EXT_ITEM_TEXT,
 *      EVCTX_KEY_TYPE_COMPONENT,
 *      EVCTX_KEY_TYPE_SERIES,
 *  @par
 *      EVCTX_KEY_TYPE_NUM_EVENTS,
 *      EVCTX_KEY_TYPE_IDX_EVENT_ID,
 *      EVCTX_KEY_TYPE_IDX_START_TIME,
 *      EVCTX_KEY_TYPE_IDX_DURATION,
 *      EVCTX_KEY_TYPE_IDX_EVENT_TITLE,
 *      EVCTX_KEY_TYPE_IDX_EVENT_TEXT,
 *      EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT,
 *      EVCTX_KEY_TYPE_IDX_CAPTION,
 *      EVCTX_KEY_TYPE_IDX_RATING,
 *      EVCTX_KEY_TYPE_IDX_EVENT_CATEGORY,
 *      EVCTX_KEY_TYPE_IDX_EVENT_TITLE_STYLE,
 *      EVCTX_KEY_TYPE_IDX_EVENT_TEXT_STYLE,
 *      EVCTX_KEY_TYPE_IDX_EVENT_EXT_TEXT_STYLE,
 *      EVCTX_KEY_TYPE_IDX_CA_SYSTEMS,
 *      EVCTX_KEY_TYPE_IDX_EVENT_LINK,
 *      EVCTX_KEY_TYPE_IDX_SOURCE_TABLE,
 *      EVCTX_KEY_TYPE_ELAPSED_TIME,
 *  @par
 *      EVCTX_KEY_TYPE_IDX_DIGITAL_COPY_CONTROL,
 *      EVCTX_KEY_TYPE_IDX_AUDIO_COMPONENT,
 *      EVCTX_KEY_TYPE_IDX_DATA_CONTENTS,
 *      EVCTX_KEY_TYPE_IDX_CA_CONTRACT_INFO,
 *      EVCTX_KEY_TYPE_IDX_EVENT_GROUP,
 *      EVCTX_KEY_TYPE_IDX_COMPONENT_GROUP,
 *      EVCTX_KEY_TYPE_IDX_EVENT_LOCATION,
 *      EVCTX_KEY_TYPE_IDX_EVENT_EXT_ITEM_TEXT,
 *      EVCTX_KEY_TYPE_IDX_COMPONENT,
 *      EVCTX_KEY_TYPE_IDX_SERIES,
 *  @par
 *      EVCTX_KEY_TYPE_TOTAL_NUMBER
 *  @par
 *      @include evctx/a4.txt
 *  @par
 *  @subsubsection sssecMW_EVCTX_4_1_5  4.1.5. How to free one event object handle?
 *      c_evctx_delete_event() is used to free one event object handle.
 *  @par
 *      @include evctx/a5.txt
 */


