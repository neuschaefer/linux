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
/*! @file x_cc_isdb_common.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 */
/*----------------------------------------------------------------------------*/
#ifndef _X_CC_ISDB_COMMON_H_
#define _X_CC_ISDB_COMMON_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_dbg.h"
#include "x_os.h"

#include "u_gl.h"


/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_WGL_CCI
*  @{  
*/
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define CC_ISDB_OK                       ((INT32)   0)    /**< The routine was successful. */
#define CC_ISDB_INV_ARG                  ((INT32)  -1)    /**< The argument is invalid. */

#define CC_ISDB_MAX_INSTANCE                CC_ISDB_VALUE_MAX_INSTANCE         /**<  The MAX CCI widget number */
#define CC_ISDB_ADD_EXTERN_SIZE             fg_g_cc_isdb_extern_size         /**< Flag to enlarge font */
#define CC_ISDB_EQUAL_WIDTH_FLAG            fg_g_cc_isdb_equal_width        /**< Flag to make character equal width */
#define CC_ISDB_AUTO_LINE_FEED_FLAG         fg_g_cc_isdb_auto_line_feed    /**< Flag to auto line break */
#define CC_ISDB_AUTO_LINE_EQUAL_FLAG        CC_ISDB_VALUE_AUTO_LINE_EQUAL_FLAG   /**< Flag to auto line break */
#define CC_ISDB_ROLL_MODE_FLAG              fg_g_cc_isdb_roll_up_mode         /**< Flag to support roll up function */
#define CC_ISDB_ES_SEPARATE_LANG            CC_ISDB_VALUE_ES_SEPARATE_LANG        /**< Flag to filter language from ES */
#define CC_ISDB_SCDB_SEPARATE_LANG          CC_ISDB_VALUE_SCDB_SEPARATE_LANG     /**< Flag to filter language from SCDB */

#if 1
#define CC_ISDB_BRA_SUPPORT    /**< ISDB closed caption for Brazil */
#else
#define CC_ISDB_JPN_SUPPORT    /**< ISDB closed caption for Japanese */
#endif

#ifdef  CC_ISDB_BRA_SUPPORT
#define CC_ISDB_VALUE_MAX_INSTANCE                ((UINT16)4)    /**<  The MAX CCI widget number */
#define CC_ISDB_VALUE_ADD_EXTERN_SIZE             ((BOOL) FALSE)    /**< Flag to enlarge font */
#define CC_ISDB_VALUE_EQUAL_WIDTH_FLAG            ((BOOL) FALSE)    /**< Flag to make character equal width */
#define CC_ISDB_VALUE_AUTO_LINE_FEED_FLAG         ((BOOL) TRUE)    /**< Flag to auto line break */
#define CC_ISDB_VALUE_AUTO_LINE_EQUAL_FLAG        ((BOOL) FALSE)    /**< Flag to auto line break */
#define CC_ISDB_VALUE_ROLL_MODE_FLAG              ((BOOL) TRUE)    /**< Flag to support roll up function */
#define CC_ISDB_VALUE_ES_SEPARATE_LANG            ((BOOL) FALSE)    /**< Flag to filter language from ES */
#define CC_ISDB_VALUE_SCDB_SEPARATE_LANG          ((BOOL) TRUE)    /**< Flag to filter language from SCDB */
#endif

#ifdef  CC_ISDB_JPN_SUPPORT
#define CC_ISDB_VALUE_MAX_INSTANCE                ((UINT16)4)    /**<  The MAX CCI widget number */
#define CC_ISDB_VALUE_ADD_EXTERN_SIZE             ((BOOL) FALSE)    /**< Flag to enlarge font */
#define CC_ISDB_VALUE_EQUAL_WIDTH_FLAG            ((BOOL) TRUE)    /**< Flag to make character equal width */
#define CC_ISDB_VALUE_AUTO_LINE_FEED_FLAG         ((BOOL) TRUE)    /**< Flag to auto line break */
#define CC_ISDB_VALUE_AUTO_LINE_EQUAK_FLAG        ((BOOL) TRUE)    /**< Flag to auto line break */
#define CC_ISDB_VALUE_ROLL_MODE_FLAG              ((BOOL) TRUE)    /**< Flag to support roll up function */
#define CC_ISDB_VALUE_ES_SEPARATE_LANG            ((BOOL) TRUE)    /**< Flag to filter language from ES */
#define CC_ISDB_VALUE_SCDB_SEPARATE_LANG          ((BOOL) FALSE)    /**< Flag to filter language from SCDB */
#endif

#define CC_ISDB_DRCS_NUM_MAX        ((UINT16)376)   /* 188*2 */
#define CC_ISDB_DRCS_MIN_UNICODE    ((UINT16)0xE000)
#define IS_DRCS_CODE( char_code)    ((CC_ISDB_DRCS_MIN_UNICODE <= char_code) && (char_code < CC_ISDB_DRCS_MIN_UNICODE + CC_ISDB_DRCS_NUM_MAX))


#ifdef  NDEBUG  /* release version */
    
#else           /* debug version */
    #define CC_ISDB_DEBUG    /**< ISDB Closed Caption has extern log and information for debug */
#endif

extern UINT32                       WGL_CCI_CLUT_SIZE;
extern GL_COLOR_T                   at_g_cci_clut[];
extern BOOL                         fg_g_cc_isdb_extern_size;
extern BOOL                         fg_g_cc_isdb_equal_width;
extern BOOL                         fg_g_cc_isdb_auto_line_feed;
extern BOOL                         fg_g_cc_isdb_roll_up_mode;


#define CC_ISDB_ASSERT( _expr )      { if (! (_expr)) { x_dbg_stmt( "[file:%s][line:%d]", __FILE__, __LINE__ );DBG_ABORT (0);} }    /**< when CCI abort, it will print the position */


/*--------------------------memory debug--------------------------*/
#ifdef CC_ISDB_DEBUG  /* for memory manage */
    
/*------------------------------------------------------------------*/
/*! @brief Allocate a block memory for CCI
 *  @param [in]     z_size                      - Memory block size
 *  @param [in]     pc_file                     - The file name
 *  @param [in]     ui4_line                    - The line number
 *  @param [out]
 *  @note
 *  @see
 *  @return     Return the allocated memory address.
 *  @retval      point                  - Memory address
 */
/*------------------------------------------------------------------*/
    VOID*   cc_isdb_act_alloc( SIZE_T z_size, CHAR* pc_file, UINT32 ui4_line);
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]     pv_mem                      - Memory block point
 *  @param [in]     pc_file                     - The file name
 *  @param [in]     ui4_line                    - The line number
 *  @param [out]
 *  @note
 *  @see
 *  @return None
 */
/*------------------------------------------------------------------*/
    VOID    cc_isdb_act_free ( VOID* pv_mem,  CHAR* pc_file, UINT32 ui4_line );
    
    #define CC_ISDB_ALLOC(p) cc_isdb_act_alloc(p,__FILE__,__LINE__)    /**< Allocate memory for CCI */
    #define CC_ISDB_FREE(p)  cc_isdb_act_free (p,__FILE__,__LINE__)    /**< Free CCI memory */
    
/*------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
    VOID    cc_isdb_reset_memory_log ( VOID );
/*------------------------------------------------------------------*/
/*! @brief Print the CCI memory condition
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
    VOID    cc_isdb_printf_memory_log( VOID );
      
#else   /* CC_ISDB_DEBUG */
    
    #define CC_ISDB_ALLOC    x_mem_alloc    /**< Directly allocate to OS memory */
    #define CC_ISDB_FREE     x_mem_free    /**< Directly free OS memory */
      
#endif  /* CC_ISDB_DEBUG */


/*------------------------------------------------------------------*/
/*! @struct CC_ISDB_RECT
 *  @brief  This structure contains the rectangle data for CCI
 *  @code
 *  typedef struct _CC_ISDB_RECT
 *  {
 *      UINT16                  ui2_left;
 *      UINT16                  ui2_top;
 *      UINT16                  ui2_right;
 *      UINT16                  ui2_bottom;
 *  }   CC_ISDB_RECT;
 *  @endcode
 *  @li@c  ui2_left                                          - The left postion
 *  @li@c  ui2_top                                           - The top postion
 *  @li@c  ui2_right                                         - The right postion
 *  @li@c  ui2_bottom                                        - The bottom postion
 */
/*------------------------------------------------------------------*/
typedef struct _CC_ISDB_RECT
{
    UINT16                  ui2_left;
    UINT16                  ui2_top;
    UINT16                  ui2_right;
    UINT16                  ui2_bottom;
}   CC_ISDB_RECT;


#define INTERSECT_ISDB_RECT(rc_out, rc1, rc2)                           \
    do {                                                                \
        (rc_out).ui2_left   = MAX((rc1).ui2_left,     (rc2).ui2_left);  \
        (rc_out).ui2_right  = MIN((rc1).ui2_right,    (rc2).ui2_right); \
        (rc_out).ui2_top    = MAX((rc1).ui2_top,      (rc2).ui2_top);   \
        (rc_out).ui2_bottom = MIN((rc1).ui2_bottom,   (rc2).ui2_bottom);\
    } while (0)    /**< intersect two rectangles as rc_out */    /**< calculate the intersection */

#define UNION_ISDB_RECT(rc_out, rc1, rc2)                               \
    do {                                                                \
        (rc_out).ui2_left   = MIN((rc1).ui2_left,     (rc2).ui2_left);  \
        (rc_out).ui2_right  = MAX((rc1).ui2_right,    (rc2).ui2_right); \
        (rc_out).ui2_top    = MIN((rc1).ui2_top,      (rc2).ui2_top);   \
        (rc_out).ui2_bottom = MAX((rc1).ui2_bottom,   (rc2).ui2_bottom);\
    } while (0)    /**< union two rectangles as rc_out */    /**< calculate the union */


/*--------------------------list structure--------------------------*/
/*------------------------------------------------------------------*/
/*! @enum CC_ISDB_LIST_FIND_T
 *  @brief This enumeration contains the notify function return value of CCI list user
 *  @code
 *  typedef enum
 *  {
 *      CC_ISDB_LIST_NO_FOUND    = 0,    
 *      CC_ISDB_LIST_FOUND_REP,          
 *      CC_ISDB_LIST_FOUND,              
 *      CC_ISDB_LIST_DELETED,            
 *  }   CC_ISDB_LIST_FIND_T;
 *  @endcode
 *  @li@c  CC_ISDB_LIST_NO_FOUND                             - user do not need the node, so we need to check the next node
 *  @li@c  CC_ISDB_LIST_FOUND_REP                            - user found the node, but he wants check the next node
 *  @li@c  CC_ISDB_LIST_FOUND                                - user got what he wants
 *  @li@c  CC_ISDB_LIST_DELETED                              - user delete what he wants
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CC_ISDB_LIST_NO_FOUND    = 0,    /* user do not need the node, so we need to check the next node */
    CC_ISDB_LIST_FOUND_REP,          /* user found the node, but he wants check the next node */
    CC_ISDB_LIST_FOUND,              /* user got what he wants */
    CC_ISDB_LIST_DELETED,            /* user delete what he wants */
}   CC_ISDB_LIST_FIND_T;


/*------------------------------------------------------------------*/
/*! @struct CC_ISDB_LIST_FIND_TAG_T
 *  @brief This structure contains the user private data
 *  @code
 *  typedef struct
 *  {
 *      VOID*                           pv_tag1;
 *      VOID*                           pv_tag2;
 *      VOID*                           pv_tag3;
 *      VOID*                           pv_tag4;
 *  }   CC_ISDB_LIST_FIND_TAG_T;
 *  @endcode
 *  @li@c  pv_tag1                                           - The first private data
 *  @li@c  pv_tag2                                           - The second private data
 *  @li@c  pv_tag3                                           - The third private data
 *  @li@c  pv_tag4                                           - The fourth private data
 */
/*------------------------------------------------------------------*/
typedef struct
{
    VOID*                           pv_tag1;
    VOID*                           pv_tag2;
    VOID*                           pv_tag3;
    VOID*                           pv_tag4;
}   CC_ISDB_LIST_FIND_TAG_T;


/*------------------------------------------------------------------*/
/*! @brief Notify the found node
 *  @param [in]     pv_data                     - The element data
 *  @param [in]     pt_find_tag                 - The user tag
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef CC_ISDB_LIST_FIND_T (* FUNC_CC_ISDB_LIST_FIND )( 
            VOID*                       pv_data,
            CC_ISDB_LIST_FIND_TAG_T*    pt_find_tag );


#define CC_ISDB_LIST_ELE_NUM        ((UINT32)5)    /**< the element number of per list block */


/*------------------------------------------------------------------*/
/*! @enum CC_ISDB_LIST_DES_TYPE
 *  @brief This enumeration contains the way of free the list element data
 *  @code
 *  typedef enum
 *  {
 *      CC_ISDB_LIST_DES_NORMAL     = 0,
 *      CC_ISDB_LIST_DES_NO_NFY,
 *      CC_ISDB_LIST_DES_DIRECT_FREE,
 *  }   CC_ISDB_LIST_DES_TYPE;
 *  @endcode
 *  @li@c  CC_ISDB_LIST_DES_NORMAL                           - The list user will free the element data himself
 *  @li@c  CC_ISDB_LIST_DES_NO_NFY                           - The list element data is not memory point
 *  @li@c  CC_ISDB_LIST_DES_DIRECT_FREE                      - The list element data is memory point
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CC_ISDB_LIST_DES_NORMAL     = 0,
    CC_ISDB_LIST_DES_NO_NFY,
    CC_ISDB_LIST_DES_DIRECT_FREE,
}   CC_ISDB_LIST_DES_TYPE;


/* because the list user maybe add NULL as data, we can not judge the empty position with NULL */
/*------------------------------------------------------------------*/
/*! @struct CC_ISDB_LIST
 *  @brief This structure contains the main body of CCI list
 *  @code
 *  typedef struct _CC_ISDB_LIST
 *  {
 *      UINT32                          ui4_attach_data;
 *      UINT32                          ui4_ele_num;    
 *      UINT32                          ui4_max_free;   
 *      UINT32*                         pui4_free_node; 
 *      UINT32                          ui4_free_node_num;
 *      UINT32*                         pui4_head_node;
 *      UINT32*                         pui4_last_node;
 *      UINT32                          ui4_fst_empty;  
 *  }   CC_ISDB_LIST;
 *  @endcode
 *  @li@c  ui4_attach_data                                   - The attached data to list for user
 *  @li@c  ui4_ele_num                                       - Element number in one node
 *  @li@c  ui4_max_free                                      - MAX number of free node in list
 *  @li@c  pui4_free_node                                    - Free node in list
 *  @li@c  ui4_free_node_num                                 - Free node number
 *  @li@c  pui4_head_node                                    - The firt node of the list
 *  @li@c  pui4_last_node                                    - The last node of the list
 *  @li@c  ui4_fst_empty                                     - Index of the first empty element position in last node
 */
/*------------------------------------------------------------------*/
typedef struct _CC_ISDB_LIST
{
    UINT32                          ui4_attach_data;
    UINT32                          ui4_ele_num;    /* element number in one node */
    UINT32                          ui4_max_free;   /* MAX number of free node in list */

    UINT32*                         pui4_free_node; 
    UINT32                          ui4_free_node_num;
    UINT32*                         pui4_head_node;
    UINT32*                         pui4_last_node;
    UINT32                          ui4_fst_empty;  /* index of the first empty element position in last node */
}   CC_ISDB_LIST;


/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*--------------------------list function----------------------------*/
/*------------------------------------------------------------------*/
/*! @brief Create a CCI list
 *  @param [in]     pt_list             - The list structrue
 *  @param [in]     ui4_ele_num         - The element number of node
 *  @param [in]     ui4_max_free        - The MAX free node number
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      CC_ISDB_OK               - The routine was successful.
 *  @retval      CC_ISDB_INV_ARG          - Argument is NULL.
 */
/*------------------------------------------------------------------*/
INT32 x_cc_isdb_list_create(
            CC_ISDB_LIST*               pt_list,
            UINT32                      ui4_ele_num,
            UINT32                      ui4_max_free );


/*------------------------------------------------------------------*/
/*! @brief Attach user data to the list
 *  @param [in]     pt_list             - The list structrue
 *  @param [in]     ui4_attach_data     - The user data
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      CC_ISDB_OK               - The routine was successful.
 *  @retval      CC_ISDB_INV_ARG          - Argument is NULL.
 */
/*------------------------------------------------------------------*/
INT32 x_cc_isdb_list_attach_data(
            CC_ISDB_LIST*               pt_list,
            UINT32                      ui4_attach_data );


/*------------------------------------------------------------------*/
/*! @brief Get user data from the list
 *  @param [in]     pt_list             - The list structrue
 *  @param [in]     ui4_attach_data     - The attached data
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      CC_ISDB_OK               - The routine was successful.
 *  @retval      CC_ISDB_INV_ARG          - Argument is NULL.
 */
/*------------------------------------------------------------------*/
INT32 x_cc_isdb_list_get_attach(
            CC_ISDB_LIST*               pt_list,
            UINT32*                     pui4_attach_data );


/*------------------------------------------------------------------*/
/*! @brief Add new element to the list
 *  @param [in]     pt_list             - The list structrue
 *  @param [in]     pv_data             - The new element data
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      CC_ISDB_OK               - The routine was successful.
 *  @retval      CC_ISDB_INV_ARG          - Argument is NULL.
 */
/*------------------------------------------------------------------*/
INT32 x_cc_isdb_list_add_data(
            CC_ISDB_LIST*               pt_list,
            VOID*                       pv_data );


/*------------------------------------------------------------------*/
/*! @brief Find some element from the list
 *  @param [in]     pt_list             - The list structrue
 *  @param [in]     pf_find             - The notification function for finding element
 *  @param [in]     pt_find_tag         - The notification function tag
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      CC_ISDB_OK               - The routine was successful.
 *  @retval      CC_ISDB_INV_ARG          - Argument is NULL.
 */
/*------------------------------------------------------------------*/
INT32 x_cc_isdb_list_find_data(
            CC_ISDB_LIST*               pt_list,
            FUNC_CC_ISDB_LIST_FIND      pf_find,
            CC_ISDB_LIST_FIND_TAG_T*    pt_find_tag );


/*------------------------------------------------------------------*/
/*! @brief Destory a CCI list
 *  @param [in]     pt_list             - The list structrue
 *  @param [in]     t_type              - The way
 *  @param [in]     pf_delete           - The notification function for deleting element
 *  @param [in]     pt_delete_tag       - The notification function tag
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      CC_ISDB_OK               - The routine was successful.
 *  @retval      CC_ISDB_INV_ARG          - Argument is NULL.
 */
/*------------------------------------------------------------------*/
INT32 x_cc_isdb_list_destory(
            CC_ISDB_LIST*               pt_list,
            CC_ISDB_LIST_DES_TYPE       t_type,
            FUNC_CC_ISDB_LIST_FIND      pf_delete,
            CC_ISDB_LIST_FIND_TAG_T*    pt_delete_tag );


/*------------------------------------------------------------------*/
/*! @brief Calculate a intersection rectangle 
 *  @param [in]     pt_rect1            - The first rectangle
 *  @param [in]     pt_rect2            - The second rectangle
 *  @param [out]    pt_rect_out         - The calculation result
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
VOID SUPER_INTERSECT_RECT( 
                GL_RECT_T*                      pt_rect_out,
                GL_RECT_T*                      pt_rect1,
                GL_RECT_T*                      pt_rect2 );


/*------------------------------------------------------------------*/
/*! @brief Calculate a union rectangle 
 *  @param [in]     pt_rect1            - The first rectangle
 *  @param [in]     pt_rect2            - The second rectangle
 *  @param [out]    pt_rect_out         - The calculation result
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
VOID SUPER_UNION_RECT( 
                GL_RECT_T*                      pt_rect_out,
                GL_RECT_T*                      pt_rect1,
                GL_RECT_T*                      pt_rect2 );


/*------------------------------------------------------------------*/
/*!@brief  This function is used to set the ISDB closed caption extern size flag.
 *  @param [in]    fg_extern_size       - The extern size flag
 */
/*------------------------------------------------------------------*/
extern VOID x_cc_isdb_set_extern_size( BOOL  fg_extern_size );


/*------------------------------------------------------------------*/
/*!@brief  This function is used to set the ISDB closed caption equal width flag.
 *  @param [in]    fg_equal_width       - The equal width flag
 */
/*------------------------------------------------------------------*/
extern VOID x_cc_isdb_set_equal_width ( BOOL  fg_equal_width );


/*------------------------------------------------------------------*/
/*!@brief  This function is used to set the ISDB closed caption auto line feed flag.
 *  @param [in]    fg_auto_line_feed    - The auto line feed flag
 */
/*------------------------------------------------------------------*/
extern VOID x_cc_isdb_set_auto_line_feed( BOOL  fg_auto_line_feed );


/*------------------------------------------------------------------*/
/*!@brief  This function is used to set the ISDB closed caption roll up flag.
 *  @param [in]    fg_roll_up_mode      - The roll up flag
 */
/*------------------------------------------------------------------*/
extern VOID x_cc_isdb_set_roll_up_mode( BOOL  fg_roll_up_mode );


#endif /* _X_CC_ISDB_COMMON_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                /* end of groupMW_WGL_CCI */
/*----------------------------------------------------------------------------*/


