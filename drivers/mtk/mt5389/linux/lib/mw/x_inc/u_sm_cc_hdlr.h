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
 /*---------------------------------------------------------------------------*/
/*! @file u_sm_cc_hdlr.h  
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/11 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: d185ada3f0c3c6d7a83fbdb7558fecd3 $
 *
 * @par Description: 
 *         This header file contains Closed Caption Stream handler specific 
 *         definitions, which are exported to applications.
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_STRM_MNGR_CC_HDLR Closed Caption Handler
  *  @ingroup groupMW_STRM_MNGR
  *  @brief Provides external function TV Application
  *  %MW Application API would be used by TV Application, lot of these APIs would
  *   be wrapped from Exported API
  *  @see groupMW_STRM_MNGR
  *  @{ 
  */  
/*----------------------------------------------------------------------------*/

     
#ifdef MW_CC_SUPPORT_CUST_1
     /*here we include the .h files in cc_hdlr_c1 directory*/
#include "u_sm_cc_hdlr.h"
#else

#ifndef _U_SM_CC_HDLR_H_
#define _U_SM_CC_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_sys_name.h"
#include "u_handle.h"
#include "u_sm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

#define SM_CC_FONT_STYLE_0         SN_FONT_MONO_SP_WO_SERF      
/**<Closed Caption Font Style 0, it is the default Font Style and it is defined the same as Font Style 3. */

#define SM_CC_FONT_STYLE_1         SN_FONT_MONO_SP_SERF         
/**<Closed Caption Font Style1, that is monospaced with serifs.*/

#define SM_CC_FONT_STYLE_2         SN_FONT_PROP_SP_SERF         
/**<Closed Caption Font Style2, that is proportionally spaced with serifs. */

#define SM_CC_FONT_STYLE_3         SN_FONT_MONO_SP_WO_SERF      
/**<Closed Caption Font Style3, that is monospaced without serifs.*/

#define SM_CC_FONT_STYLE_4         SN_FONT_PROP_SP_WO_SERF      
/**<Closed Caption Font Style4, that is proportionally spaced without serifs. */

#define SM_CC_FONT_STYLE_5         SN_FONT_CASUAL               
/**<Closed Caption Font Style5, that is casual font type.*/

#define SM_CC_FONT_STYLE_6         SN_FONT_CURSIVE              
/**<Closed Caption Font Style6, that is cursive font type.*/

#define SM_CC_FONT_STYLE_7         SN_FONT_SMALL_CAPITALS       
/**<Closed Caption Font Style7, that is small capitals.*/


/* command sets *************************************************************/
/* primary commands */
#define SM_CC_CMD_CODE_SET_VS_HANDLE                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))           
/**<This command is mandatory. Argument h_obj of struct SM_COMMAND_T contains a video stream handle.*/

#define SM_CC_CMD_CODE_USE_OSD_DISP                         \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))           
/**<This command is optional. Argument h_obj of struct SM_COMMAND_T contains a canvas handle.*/

#define SM_CC_CMD_CODE_SET_CC_FILTER                        \
      ((SM_CMD_CODE_T)(SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))           
/**<This command is mandatory. Argument pv_data of struct SM_COMMAND_T references to struct SM_CC_FILTER_T */


/*------------------------------------------------------------------*/
/*! @brief     Indicates the closed caption aspect ration information.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CC_ASPECT_RATIO_UNKNOWN = 0,
  *      SM_CC_ASPECT_RATIO_4_3,
  *      SM_CC_ASPECT_RATIO_16_9, 
  *      SM_CC_ASPECT_RATIO_OTHER
  *  } SM_CC_ASPECT_RATIO_T; 
  *  @endcode 
  *  @li@c  SM_CC_ASPECT_RATIO_UNKNOWN     -The aspect ratio is unknown.
  *  @li@c  SM_CC_ASPECT_RATIO_4_3               -The aspect ratio is 4:3.
  *  @li@c  SM_CC_ASPECT_RATIO_16_9             -The aspect ratio is 16:9.
  *  @li@c  SM_CC_ASPECT_RATIO_OTHER          -The aspect ratio is neither 4:3 nor 16:9
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CC_ASPECT_RATIO_UNKNOWN = 0,
    SM_CC_ASPECT_RATIO_4_3,
    SM_CC_ASPECT_RATIO_16_9,
    SM_CC_ASPECT_RATIO_OTHER
}   SM_CC_ASPECT_RATIO_T;


/*------------------------------------------------------------------*/
/*! @brief     Indicates the closed caption filter type.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CC_TYPE_IGNORE          = 0xFF,
  *      SM_CC_TYPE_NTSC_LINE21  = 0,
  *      SM_CC_TYPE_DTVCC           = 1,
  *      SM_CC_TYPE_AUTO_SEL       = 2
  *  } SM_CC_TYPE_T; 
  *  @endcode 
  *  @li@c  SM_CC_TYPE_IGNORE           -The filter type is IGNORE.
  *  @li@c  SM_CC_TYPE_NTSC_LINE21   -The filter type is NTSC LINE21.
  *  @li@c  SM_CC_TYPE_DTVCC            -The filter type is DTVCC.
  *  @li@c  SM_CC_TYPE_AUTO_SEL        -The filter type is AUTO SELECT.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CC_TYPE_IGNORE      = 0xFF,
    SM_CC_TYPE_NTSC_LINE21 = 0,
    SM_CC_TYPE_DTVCC       = 1,
    SM_CC_TYPE_AUTO_SEL    = 2
} SM_CC_TYPE_T;


/*------------------------------------------------------------------*/
/*! @brief     Indicates the NTSC closed caption data type.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_NTSC_DATA_TYPE_UNKNOWN          = 0xFF,
  *      SM_NTSC_DATA_TYPE_CC1  = 0,
  *      SM_NTSC_DATA_TYPE_CC2,
  *      SM_NTSC_DATA_TYPE_CC3,
  *      SM_NTSC_DATA_TYPE_CC4,
  *      SM_NTSC_DATA_TYPE_T1,
  *      SM_NTSC_DATA_TYPE_T2,
  *      SM_NTSC_DATA_TYPE_T3,
  *      SM_NTSC_DATA_TYPE_T4,  
  *  } SM_NTSC_DATA_TYPE_T; 
  *  @endcode 
  *  @li@c  SM_NTSC_DATA_TYPE_UNKNOWN  -The NTSC data type is unknown.
  *  @li@c  SM_NTSC_DATA_TYPE_CC1           -The NTSC data type is CC1.
  *  @li@c  SM_NTSC_DATA_TYPE_CC2           -The NTSC data type is CC2.
  *  @li@c  SM_NTSC_DATA_TYPE_CC3           -The NTSC data type is CC3.
  *  @li@c  SM_NTSC_DATA_TYPE_CC4           -The NTSC data type is CC4.
  *  @li@c  SM_NTSC_DATA_TYPE_T1              -The NTSC data type is T1.
  *  @li@c  SM_NTSC_DATA_TYPE_T2              -The NTSC data type is T2.
  *  @li@c  SM_NTSC_DATA_TYPE_T3              -The NTSC data type is T3.
  *  @li@c  SM_NTSC_DATA_TYPE_T4              -The NTSC data type is T4.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_NTSC_DATA_TYPE_UNKNOWN = 0xFF,
    SM_NTSC_DATA_TYPE_CC1 = 0,
    SM_NTSC_DATA_TYPE_CC2,
    SM_NTSC_DATA_TYPE_CC3,
    SM_NTSC_DATA_TYPE_CC4,
    SM_NTSC_DATA_TYPE_T1,
    SM_NTSC_DATA_TYPE_T2,
    SM_NTSC_DATA_TYPE_T3,
    SM_NTSC_DATA_TYPE_T4
} SM_NTSC_DATA_TYPE_T;

#define SM_NTSCCC_FLAG( cc_type )       MAKE_BIT_MASK_8( cc_type )  
/**<Mask for NTSC line21 closed caption type.*/

#define SM_DTVCC_FLAG( svc_num )        MAKE_BIT_MASK_64( svc_num ) 
/**<Mask for DTV closed caption service.*/


/*------------------------------------------------------------------*/
/*! @brief The structure of closed caption auto select property.
  *  @code 
  *  typedef struct _SM_CC_AUTO_SEL_T 
  *  { 
  *      BOOL          b_prefer_dtvcc;
  *      UINT8         ui1_ntsccc_mask; 
  *      UINT64        ui8_dtvcc_mask;
  *  } SM_CC_AUTO_SEL_T; 
  *  @endcode 
  *  @li@c  b_prefer_dtvcc            -Indicated dtv cc is perfered or not.
  *  @li@c  ui1_ntsccc_mask         -Contains the mask of NTSC cc.
  *  @li@c  ui8_dtvcc_mask          -Contains the mask of dtv cc.
  */
/*------------------------------------------------------------------*/
typedef struct _SM_CC_AUTO_SEL_T
{
    BOOL          b_prefer_dtvcc;
    UINT8         ui1_ntsccc_mask; 
    UINT64        ui8_dtvcc_mask;
} SM_CC_AUTO_SEL_T;

/* pv_data of SM_CC_CMD_CODE_SET_CC_FILTER */
/*------------------------------------------------------------------*/
/*! @brief The structure of closed caption filter property.
  *  @code 
  *  typedef struct  
  *  { 
  *         SM_CC_TYPE_T     e_cc_type;
  *         union
  *          {
  *                 SM_NTSC_DATA_TYPE_T    e_data_type; 
  *                 UINT8                               ui1_service_num;
  *                 SM_CC_AUTO_SEL_T         t_auto_sel;
  *      } u; 
  *  } SM_CC_FILTER_T; 
  *  @endcode 
  *  @li@c  e_cc_type            -Indicated CC filter type.
  *  @li@c  e_data_type         -Contains the NTSC line21 cc data type.
  *  @li@c  ui1_service_num  -Contains the service number of digital cc.
  *  @li@c  t_auto_sel            -Contains the auto select cc parameter.
  */
/*------------------------------------------------------------------*/
typedef struct
{
    SM_CC_TYPE_T     e_cc_type;
    
    union
    {
        SM_NTSC_DATA_TYPE_T  e_data_type;     /* SM_CC_TYPE_NTSC_LINE21 */
        UINT8                ui1_service_num; /* SM_CC_TYPE_DTVCC, std:1-6, ext:7-63 */
        SM_CC_AUTO_SEL_T     t_auto_sel;
    } u;
} SM_CC_FILTER_T;


/* set/get type groups *******************************************************/
#define SM_GRP_CCH                      (( SM_GRP_LAST_ENTRY + 7 ) << SM_CODE_BITS)
/**<This group definition is for Closed Caption Stream Handler specific set/get types.*/


#define SM_GRP_NTSC_CC                  (( SM_GRP_LAST_ENTRY + 8 ) << SM_CODE_BITS)
/**<This group definition is for NTSC CC specific set/get types.*/


#define SM_GRP_DTV_CC                   (( SM_GRP_LAST_ENTRY + 9 ) << SM_CODE_BITS)
/**<This group definition is for DTV CC specific set/get types.*/

/* set types *****************************************************************/
#define SM_CCH_SET_TYPE_IGNORE          ((SM_SET_TYPE_T)(SM_GRP_CCH | 0))
/**<Ingore this set type please*/

/*#define SM_CCH_SET_TYPE_EXT_DATA_ON     ((SM_SET_TYPE_T)(SM_GRP_NTSC_CC | 0))
#define SM_CCH_SET_TYPE_EXT_DATA_OFF    ((SM_SET_TYPE_T)(SM_GRP_NTSC_CC | 1))
*/
#define SM_CCH_SET_TYPE_NTSC_CC_VBI_PSR_CB  ((SM_SET_TYPE_T)(SM_GRP_NTSC_CC | 3))
/**<Argument pv_set_data of API (*sm_set_fct) references a type SM_CCH_NTSC_CC_VBI_PSR_CB_IINFO_T, which contains the callback function to deliver VBI data to video handler.*/

/* move to SCC API */
#define SM_CCH_SET_TYPE_CC_FLTR             ((SM_SET_TYPE_T)(SM_GRP_CCH    | 0))
/**<Argument pv_set_data of API (*sm_set_fct) references a type SM_CC_FILTER_T, which contains the setting of CC Filter attributes.*/


#define SM_CCH_SET_TYPE_CC_GLOBAL_VISIBLE   ((SM_SET_TYPE_T)(SM_GRP_CCH    | 1))
/**<Argument pv_set_data of API (*sm_set_fct) references a boolean value, which contains the cc frame is visible or invisible.*/


#define SM_CCH_SET_TYPE_DTVCC_DUMP_SRVC_ON  ((SM_SET_TYPE_T)(SM_GRP_DTV_CC | 0))
/**<Ingore this set type please*/


#define SM_CCH_SET_TYPE_DTVCC_DUMP_SRVC_OFF ((SM_SET_TYPE_T)(SM_GRP_DTV_CC | 1))
/**<Ingore this set type please*/


#define SM_CCH_SET_TYPE_DTVCC_ATTRIBS       ((SM_SET_TYPE_T)(SM_GRP_DTV_CC | 2))
/**<Argument pv_set_data of API (*sm_set_fct) references a type SM_CCH_DTVCC_ATTRIBS_INFO_T, which contains the setting of DTVCC stream attributes.*/

/* get types *****************************************************************/
#define SM_CCH_GET_TYPE_IGNORE              ((SM_GET_TYPE_T)(SM_GRP_CCH | 0))
/**<Ingore this get type please*/

#define SM_CCH_GET_TYPE_EXISTED_CC_IND      ((SM_GET_TYPE_T)(SM_GRP_CCH | 1))
/**<Argument pv_get_data of API (*sm_get_fct) references a type SM_CCH_EXISTED_CC_IND_T, which contains the existed cc indicator of  CC Data.*/


#define SM_CCH_GET_TYPE_SELECTED_CC_FLTR    ((SM_GET_TYPE_T)(SM_GRP_CCH | 2))
/**<Argument pv_get_data of API (*sm_get_fct) references a type SM_CC_FILTER_T, which contains the selected CC filter. For AUTO CC Engine, it is the real CC filter.*/


#define SM_CCH_GET_TYPE_CC_FLTR             ((SM_GET_TYPE_T)(SM_GRP_CCH | 3))
/**<Argument pv_get_data of API (*sm_get_fct) references a type SM_CC_FILTER_T, which contains the selected CC filter by CC Handler.*/

#define SM_CCH_GET_TYPE_DTVCC_ATTRIBS       ((SM_GET_TYPE_T)(SM_GRP_DTV_CC | 0))
/**<Argument pv_get_data of API (*sm_get_fct) references a type SM_CCH_DTVCC_ATTRIBS_INFO_T, which contains the setting of DTVCC stream attributes.*/

/* SM_CCH_SET_TYPE_EXT_DATA_ON data info *************************************/
/*typedef INT32 (*x_sm_cch_ntsc_ext_data_fct)( HANDLE_T     h_stream,
                                               CHAR*        pc_buff );
*/

/* SM_CCH_SET_TYPE_NTSC_CC_VBI_PSR_CB data info ******************************/


/* CLI execution function */
/*------------------------------------------------------------------*/
/*! @brief  This type defines the cc handler ntsc vbi parser callback function prototype.
  *  @param [in] pv_tag           Contain the tag of user's private data.
  *  @param [in] ui1_char_1     Contain the vbi char1.
  *  @param [in] ui1_char_2     Contain the vbi char2. 
  */
/*------------------------------------------------------------------*/
typedef VOID (*x_sm_cch_ntsc_vbi_psr_cb_fct)( VOID*        pv_tag,
                                              UINT8        ui1_char_1,
                                              UINT8        ui1_char_2 );


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption vbi parser callback information
  *  @code 
  *  typedef struct  
  *  { 
  *      VOID*                           pv_tag;
  *      x_sm_cch_ntsc_vbi_psr_cb_fct    pf_vbi_psr_cb;
  *  } SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T; 
  *  @endcode 
  *  @li@c  pv_tag                  -Contains the private user's tag data.
  *  @li@c  pf_vbi_psr_cb        -Contains the function pointer of vbi parser.
  */
/*------------------------------------------------------------------*/
typedef struct
{
    VOID*                           pv_tag;
    x_sm_cch_ntsc_vbi_psr_cb_fct    pf_vbi_psr_cb;
} SM_CCH_NTSC_CC_VBI_PSR_CB_INFO_T;

#if 0
/* SM_CCH_SET_TYPE_DTVCC_DUMP_SRVC_ON data info ******************************/
typedef struct
{
    UINT8     ui1_srvc_num;      /* allow:1~63 */
    UINT8     ui1_len;    
} SM_CCH_DTVCC_SRVC_HDR_T;


/*------------------------------------------------------------------*/
/*! @brief  This type defines the cc handler digital cc service data function prototype.
  *  @param [in] h_stream         Contain the tag of user's private data.
  *  @param [in] pt_svc_hdr       Contain the vbi char1.
  *  @param [in] pc_buff            Contain the vbi char1.
  *  @param [out] 
  *  @note 
  *  @see 
  *  @return 
  *  @retval      - 
  */
/*------------------------------------------------------------------*/
typedef INT32 (*x_sm_cch_dtvcc_srvc_data_fct)( 
    HANDLE_T                 h_stream,
    SM_CCH_DTVCC_SRVC_HDR_T* pt_svc_hdr,
    CHAR*                    pc_buff );
#endif

/* SM_CCH_GET_TYPE_DTVCC_ATTRIBS data info ***********************************/
#define SM_CCH_DTVCC_ATT_FLAG_FONT_STYLE_AS_BRDCST         MAKE_BIT_MASK_32(0)
/**<Definition the digital closed caption of font style flag*/

#define SM_CCH_DTVCC_ATT_FLAG_FONT_SIZE_AS_BRDCST          MAKE_BIT_MASK_32(1)
/**<Definition the digital closed caption of font size flag*/

#define SM_CCH_DTVCC_ATT_FLAG_BG_COLOR_AS_BRDCST           MAKE_BIT_MASK_32(2)
/**<Definition the digital closed caption of background color flag*/

#define SM_CCH_DTVCC_ATT_FLAG_BG_COLOR_OPC_AS_BRDCST       MAKE_BIT_MASK_32(3)
/**<Definition the digital closed caption of background color opacity flag*/

#define SM_CCH_DTVCC_ATT_FLAG_FG_COLOR_AS_BRDCST           MAKE_BIT_MASK_32(4)
/**<Definition the digital closed caption of foreground color flag*/

#define SM_CCH_DTVCC_ATT_FLAG_FG_COLOR_OPC_AS_BRDCST       MAKE_BIT_MASK_32(5)
/**<Definition the digital closed caption of foreground color opacity flag*/

#define SM_CCH_DTVCC_ATT_FLAG_EDGE_COLOR_AS_BRDCST         MAKE_BIT_MASK_32(6)
/**<Definition the digital closed caption of edge color flag*/

#define SM_CCH_DTVCC_ATT_FLAG_EDGE_COLOR_OPC_AS_BRDCST     MAKE_BIT_MASK_32(7)
/**<Definition the digital closed caption of edeg color opactity flag*/

#define SM_CCH_DTVCC_ATT_FLAG_EDGE_TYPE_AS_BRDCST          MAKE_BIT_MASK_32(8)
/**<Definition the digital closed caption of edge type flag*/

#define SM_CCH_DTVCC_ATT_FLAG_WND_COLOR_AS_BRDCST          MAKE_BIT_MASK_32(9)
/**<Definition the digital closed caption of window color flag*/

#define SM_CCH_DTVCC_ATT_FLAG_WND_COLOR_OPC_AS_BRDCST      MAKE_BIT_MASK_32(10)
/**<Definition the digital closed caption of window color opacity flag*/

#define SM_CCH_DTVCC_ATT_FLAG_WND_EDGE_COLOR_AS_BRDCST     MAKE_BIT_MASK_32(11)
/**<Definition the digital closed caption of window edge color flag*/

#define SM_CCH_DTVCC_ATT_FLAG_WND_EDGE_COLOR_OPC_AS_BRDCST MAKE_BIT_MASK_32(12)
/**<Definition the digital closed caption of window edge color opacity flag*/

#define SM_CCH_DTVCC_ATT_FLAG_WND_EDGE_TYPE_AS_BRDCST      MAKE_BIT_MASK_32(13)
/**<Definition the digital closed caption of windo edge type flag*/

#define SM_CCH_DTVCC_ATT_FLAG_WND_JUSTIFY_AS_BRDCST        MAKE_BIT_MASK_32(14)
/**<Definition the digital closed caption of window justify flag*/

#define SM_CCH_DTVCC_ATT_FLAG_ALL_AS_BRDCST                ((UINT32)0xFFFFFFFF)
/**<Definition the digital closed caption of all flag*/

#define SM_CCH_DTVCC_ATT_FLAG_ALL_USER_DEFFINE             ((UINT32)0x0)
/**<Definition the digital closed caption of all user define flag*/



/*------------------------------------------------------------------*/
/*! @brief     Indicates the DTV closed caption font style type.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_DTVCC_FONT_STYLE_MONO_SP_WITH_SERIFS            = 1,
  *      SM_CCH_DTVCC_FONT_STYLE_PROP_SP_WITH_SERIFS             = 2,
  *      SM_CCH_DTVCC_FONT_STYLE_MONO_SP_WITHOUT_SERIFS      = 3,
  *      SM_CCH_DTVCC_FONT_STYLE_PROP_SP_WITHOUT_SERIFS       = 4,
  *      SM_CCH_DTVCC_FONT_STYLE_CASUAL                                    = 5,
  *      SM_CCH_DTVCC_FONT_STYLE_CURSIVE                                   = 6,
  *      SM_CCH_DTVCC_FONT_STYLE_SMALL_CAPITALS                       = 7
  *  } SM_CCH_DTVCC_FONT_STYLE_T; 
  *  @endcode 
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_MONO_SP_WITH_SERIFS         -The MON_SP_WITH_SERIFS font style.
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_PROP_SP_WITH_SERIFS          -The PROP_SP_WITH_SERIFS font style.
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_MONO_SP_WITHOUT_SERIFS   -The MONO_SP_WITHOUT_SERIFS font style.
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_PROP_SP_WITHOUT_SERIFS    -The PROP_SP_WITHOUT_SERIFS font style.
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_CASUAL                                 -The CASUAL font style.
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_CURSIVE                                -The CURSIVE font style.
  *  @li@c  SM_CCH_DTVCC_FONT_STYLE_SMALL_CAPITALS                    -The SMALL_CAPITALS font style.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_DTVCC_FONT_STYLE_MONO_SP_WITH_SERIFS    = 1,
    SM_CCH_DTVCC_FONT_STYLE_PROP_SP_WITH_SERIFS    = 2,
    SM_CCH_DTVCC_FONT_STYLE_MONO_SP_WITHOUT_SERIFS = 3,
    SM_CCH_DTVCC_FONT_STYLE_PROP_SP_WITHOUT_SERIFS = 4,
    SM_CCH_DTVCC_FONT_STYLE_CASUAL                 = 5,
    SM_CCH_DTVCC_FONT_STYLE_CURSIVE                = 6,
    SM_CCH_DTVCC_FONT_STYLE_SMALL_CAPITALS         = 7
} SM_CCH_DTVCC_FONT_STYLE_T;


/*------------------------------------------------------------------*/
/*! @brief     Indicates the DTV closed caption font size.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_DTVCC_FONT_SIZE_SMALL = 0,
  *      SM_CCH_DTVCC_FONT_SIZE_MEDIUM,
  *      SM_CCH_DTVCC_FONT_SIZE_LARGE
  *  } SM_CCH_DTVCC_FONT_SIZE_T; 
  *  @endcode 
  *  @li@c  SM_CCH_DTVCC_FONT_SIZE_SMALL    -The small font size.
  *  @li@c  SM_CCH_DTVCC_FONT_SIZE_MEDIUM  -The medium font size.
  *  @li@c  SM_CCH_DTVCC_FONT_SIZE_LARGE    -The large font size.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_DTVCC_FONT_SIZE_SMALL = 0,
    SM_CCH_DTVCC_FONT_SIZE_MEDIUM,
    SM_CCH_DTVCC_FONT_SIZE_LARGE
} SM_CCH_DTVCC_FONT_SIZE_T;


/*------------------------------------------------------------------*/
/*! @brief     Indicates the DTV closed caption font edge type.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_DTVCC_EDGE_TYPE_NONE            = 0,
  *      SM_CCH_DTVCC_EDGE_TYPE_RAISED,
  *      SM_CCH_DTVCC_EDGE_TYPE_DEPRESSED,
  *      SM_CCH_DTVCC_EDGE_TYPE_UNIFORM,
  *      SM_CCH_DTVCC_EDGE_TYPE_LEFT_DROP_SHADOW,
  *      SM_CCH_DTVCC_EDGE_TYPE_RIGHT_DROP_SHADOW
  *  } SM_CCH_DTVCC_EDGE_TYPE_T; 
  *  @endcode 
  *  @li@c  SM_CCH_DTVCC_EDGE_TYPE_NONE                           -The NONE font edge style.
  *  @li@c  SM_CCH_DTVCC_EDGE_TYPE_RAISED                        -The RAISED font edge style.
  *  @li@c  SM_CCH_DTVCC_EDGE_TYPE_DEPRESSED                  -The DEPRESSED font edge style.
  *  @li@c  SM_CCH_DTVCC_EDGE_TYPE_UNIFORM                      -The UNIFORM font edge style.
  *  @li@c  SM_CCH_DTVCC_EDGE_TYPE_LEFT_DROP_SHADOW    -The LEFT_DROP_SHADOW font edge style.
  *  @li@c  SM_CCH_DTVCC_EDGE_TYPE_RIGHT_DROP_SHADOW -The RIGHT_DROP_SHADOW font edge style.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_DTVCC_EDGE_TYPE_NONE = 0,
    SM_CCH_DTVCC_EDGE_TYPE_RAISED,
    SM_CCH_DTVCC_EDGE_TYPE_DEPRESSED,
    SM_CCH_DTVCC_EDGE_TYPE_UNIFORM,
    SM_CCH_DTVCC_EDGE_TYPE_LEFT_DROP_SHADOW,
    SM_CCH_DTVCC_EDGE_TYPE_RIGHT_DROP_SHADOW
} SM_CCH_DTVCC_EDGE_TYPE_T;


/*------------------------------------------------------------------*/
/*! @brief     Indicates the DTV closed caption opacity type.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_DTVCC_OPACITY_SOLID = 0,
  *      SM_CCH_DTVCC_OPACITY_FLASH = 1,
  *      SM_CCH_DTVCC_OPACITY_TRANSLUCENT = 2,
  *      SM_CCH_DTVCC_OPACITY_TRANSPARENT = 3
  *  } SM_CCH_DTVCC_OPACITY_T; 
  *  @endcode 
  *  @li@c  SM_CCH_DTVCC_OPACITY_SOLID                -The SOLID opacity type.
  *  @li@c  SM_CCH_DTVCC_OPACITY_FLASH                -The flash opacity type.
  *  @li@c  SM_CCH_DTVCC_OPACITY_TRANSLUCENT    -The translucent opacity type.
  *  @li@c  SM_CCH_DTVCC_OPACITY_TRANSPARENT    -The transparent opacity type.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_DTVCC_OPACITY_SOLID = 0,
    SM_CCH_DTVCC_OPACITY_FLASH = 1,
    SM_CCH_DTVCC_OPACITY_TRANSLUCENT = 2,
    SM_CCH_DTVCC_OPACITY_TRANSPARENT = 3
} SM_CCH_DTVCC_OPACITY_T;


/*------------------------------------------------------------------*/
/*!@brief the structure of digital closed caption color property.
  *  @code 
  *  typedef struct  
  *  { 
  *      UINT8                     ui1_red;
  *      UINT8                     ui1_green;
  *      UINT8                     ui1_blue;
  *      SM_CCH_DTVCC_OPACITY_T    e_opacity;
  *  } SM_CCH_DTVCC_COLOR_T; 
  *  @endcode 
  *  @li@c  ui1_red            -Contains the red color value.
  *  @li@c  ui1_green         -Contains the green color value.
  *  @li@c  ui1_blue           -Contains the blue color value.
  *  @li@c  e_opacity         -Contains the opacity value.
  */  
/*------------------------------------------------------------------*/
typedef struct
{
    UINT8                     ui1_red;
    UINT8                     ui1_green;
    UINT8                     ui1_blue;
    SM_CCH_DTVCC_OPACITY_T    e_opacity;
} SM_CCH_DTVCC_COLOR_T;



/*------------------------------------------------------------------*/
/*! @brief     Indicates the DTV closed caption justify type.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_DTVCC_JUSTIFY_LEFT = 0,
  *      SM_CCH_DTVCC_JUSTIFY_RIGHT,
  *      SM_CCH_DTVCC_JUSTIFY_CENTER,
  *      SM_CCH_DTVCC_JUSTIFY_FULL
  *  } SM_CCH_DTVCC_JUSTIFY_T; 
  *  @endcode 
  *  @li@c  SM_CCH_DTVCC_JUSTIFY_LEFT       -The justify type is LEFT.
  *  @li@c  SM_CCH_DTVCC_JUSTIFY_RIGHT    -The justify type is RIGHT.
  *  @li@c  SM_CCH_DTVCC_JUSTIFY_CENTER  -The justify type is CENTER.
  *  @li@c  SM_CCH_DTVCC_JUSTIFY_FULL       -The justify type is FULL.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_DTVCC_JUSTIFY_LEFT = 0,
    SM_CCH_DTVCC_JUSTIFY_RIGHT,
    SM_CCH_DTVCC_JUSTIFY_CENTER,
    SM_CCH_DTVCC_JUSTIFY_FULL
} SM_CCH_DTVCC_JUSTIFY_T;


/*------------------------------------------------------------------*/
/*! @brief the structure of digital closed caption attribute information.
  *  @code 
  *  typedef struct  
  *  { 
  *         UINT32                                         ui4_att_flag;
  *         SM_CCH_DTVCC_FONT_STYLE_T    e_font_style;
  *         SM_CCH_DTVCC_FONT_SIZE_T      e_font_size;
  *         SM_CCH_DTVCC_COLOR_T            t_font_bg_color;
  *         SM_CCH_DTVCC_COLOR_T            t_font_fg_color;
  *         SM_CCH_DTVCC_COLOR_T            t_font_edge_color;
  *         SM_CCH_DTVCC_EDGE_TYPE_T     e_font_edge_type;
  *         SM_CCH_DTVCC_COLOR_T            t_wnd_color;
  *         SM_CCH_DTVCC_COLOR_T            t_wnd_edge_color;
  *         SM_CCH_DTVCC_EDGE_TYPE_T      e_wnd_edge_type;
  *         SM_CCH_DTVCC_JUSTIFY_T            e_wnd_jusify;
  *  } SM_CCH_DTVCC_ATTRIBS_INFO_T; 
  *  @endcode 
  *  @li@c  ui4_att_flag             -Contains the attribute flag to indicates it is valid or not.
  *  @li@c  e_font_style            -Contains the font style attribute.
  *  @li@c  e_font_size             -Contains the font size attribute.
  *  @li@c  t_font_bg_color       -Contains the font's background color attribute.
  *  @li@c  t_font_fg_color        -Contains the font's foreground color attribute.
  *  @li@c  t_font_edge_color    -Contains the font's edge color attribute.
  *  @li@c  e_font_edge_type    -Contains the font's edge type attribute.
  *  @li@c  t_wnd_color             -Contains the window's color attribute.
  *  @li@c  t_wnd_edge_color    -Contains the window's edge color attribute.
  *  @li@c  e_wnd_edge_type    -Contains the window's edge type attribute.
  *  @li@c  e_wnd_jusify           -Contains the window's justify attribute.
  */  
/*------------------------------------------------------------------*/
typedef struct
{
    UINT32                    ui4_att_flag;
    SM_CCH_DTVCC_FONT_STYLE_T e_font_style;
    SM_CCH_DTVCC_FONT_SIZE_T  e_font_size;
    SM_CCH_DTVCC_COLOR_T      t_font_bg_color;
    SM_CCH_DTVCC_COLOR_T      t_font_fg_color;
    SM_CCH_DTVCC_COLOR_T      t_font_edge_color;
    SM_CCH_DTVCC_EDGE_TYPE_T  e_font_edge_type;
    SM_CCH_DTVCC_COLOR_T      t_wnd_color;
    SM_CCH_DTVCC_COLOR_T      t_wnd_edge_color;
    SM_CCH_DTVCC_EDGE_TYPE_T  e_wnd_edge_type;
    SM_CCH_DTVCC_JUSTIFY_T    e_wnd_jusify;
} SM_CCH_DTVCC_ATTRIBS_INFO_T;

/* SM_CCH_GET_TYPE_EXISTED_CC_IND ********************************************/
typedef struct _SM_CC_AUTO_SEL_T SM_CCH_EXISTED_CC_IND_T;   
/**<the type definition of existed closed caption indicator*/


/* CC Handler Config */
/*------------------------------------------------------------------*/
/*! @brief     Indicates how to process failed cc parity error char.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_PARITY_ERR_HDLR_NORMAL = 0,
  *      SM_CCH_PARITY_ERR_HDLR_IGNORE,
  *      SM_CCH_PARITY_ERR_HDLR_SP,
  *      SM_CCH_PARITY_ERR_HDLR_TSP
  *  } SM_CCH_PARITY_ERR_HDLR_T; 
  *  @endcode 
  *  @li@c  SM_CCH_PARITY_ERR_HDLR_NORMAL    -Process the char normally.
  *  @li@c  SM_CCH_PARITY_ERR_HDLR_IGNORE    -Ingore the parity error check
  *  @li@c  SM_CCH_PARITY_ERR_HDLR_SP            -Replace the char with SP.
  *  @li@c  SM_CCH_PARITY_ERR_HDLR_TSP          -Replace the char with TSP.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_PARITY_ERR_HDLR_NORMAL = 0,
    SM_CCH_PARITY_ERR_HDLR_IGNORE,
    SM_CCH_PARITY_ERR_HDLR_SP,
    SM_CCH_PARITY_ERR_HDLR_TSP
} SM_CCH_PARITY_ERR_HDLR_T;

#define SM_CCH_DTVCC_DELAY_BUFF_LEN_MAX      ((UINT8)26)    
/**<the definition of maximum digital closed caption delay beffer length*/


/*------------------------------------------------------------------*/
/*! @brief     Indicates the order of closed caption widget.
  *  @code 
  *  typedef enum 
  *  { 
  *      SM_CCH_WGT_ORDER_BOTTOMMOST = 0,
  *      SM_CCH_WGT_ORDER_TOPMOST
  *  } SM_CCH_WGT_ORDER_T; 
  *  @endcode 
  *  @li@c  SM_CCH_WGT_ORDER_BOTTOMMOST     - BOTTOMMOST order.
  *  @li@c  SM_CCH_WGT_ORDER_TOPMOST            -TOPMOST order.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    SM_CCH_WGT_ORDER_BOTTOMMOST = 0,
    SM_CCH_WGT_ORDER_TOPMOST
} SM_CCH_WGT_ORDER_T;

#define SM_CCH_NTSC_TEXT_MODE_ROWS           8              
/**<The row count of ntxt text mode*/


/*------------------------------------------------------------------*/
/*! @brief the structure of closed caption configuration.
  *  @code 
  *  typedef struct  _SM_CCH_CONFIG_T
  *  { 
  *         THREAD_DESCR_T                     t_thrd_descr;
  *         UINT8                                       ui1_width_percentage;
  *         UINT8                                        ui1_height_percentage;
  *         SM_CCH_PARITY_ERR_HDLR_T   e_parity_err_hdlr;
  *         UINT8                                        ui1_dtvcc_delay_buff_len;
  *         SM_CCH_WGT_ORDER_T            e_wgt_order;
  *         BOOL                                         b_black_bac_only;
  *         UINT8                                        ui1_text_mode_rows;
  *  } SM_CCH_CONFIG_T; 
  *  @endcode 
  *  @li@c  t_thrd_descr                   -Contains the thread descriptor information.
  *  @li@c  ui1_width_percentage      -Contains the width percentage of cc safe area.
  *  @li@c  ui1_height_percentage     -Contains the height percentage of cc safe area.
  *  @li@c  e_parity_err_hdlr             -Contains the parity error handler.
  *  @li@c  ui1_dtvcc_delay_buff_len  -Contains the dtv cc delay buffer length.
  *  @li@c  e_wgt_order                    -Contains the widget's order type.
  *  @li@c  b_black_bac_only            -Contains the black bac only attribute of NTSC cc.
  *  @li@c  ui1_text_mode_rows        -Contains the thex mode rows configuration.
  */  
/*------------------------------------------------------------------*/
typedef struct _SM_CCH_CONFIG_T
{
    THREAD_DESCR_T            t_thrd_descr;
    UINT8                     ui1_width_percentage;  /* percentage of canvas */
    UINT8                     ui1_height_percentage; /* percentage of canvas */
    SM_CCH_PARITY_ERR_HDLR_T  e_parity_err_hdlr;
    UINT8                     ui1_dtvcc_delay_buff_len;  /* seconds, 3~26 */
    SM_CCH_WGT_ORDER_T        e_wgt_order;
    BOOL                      b_black_bac_only;
    UINT8                     ui1_text_mode_rows;
} SM_CCH_CONFIG_T;


#define SM_CCH_CONFIG_INIT                                                   \
    {                                                                        \
        {DEFAULT_STACK_SIZE, DEFAULT_PRIORITY, DEFAULT_NUM_MSGS},            \
        0, 0, SM_CCH_PARITY_ERR_HDLR_NORMAL,                                 \
        SM_CCH_DTVCC_DELAY_BUFF_LEN_MAX,                                     \
        SM_CCH_WGT_ORDER_TOPMOST,                                            \
        FALSE,                                                               \
        SM_CCH_NTSC_TEXT_MODE_ROWS                                           \
    }
/**<The init value of closed caption handler configuration*/

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
#endif
#endif /* _U_SM_CC_HDLR_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_STRM_MNGR_CC_HDLR */
/*----------------------------------------------------------------------------*/




