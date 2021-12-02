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
/*! @file u_cli.h  
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/11 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: d3f4bd3088d0839e70c155f2e911dd7a $
 *
 * @par Description:
 *         This header file contains CLI related definitions, which are
 *         known to applications and middleware.
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_CLI Command Line Interface  
  *  @ingroup groupMW
  *  @see groupMW
  *  @{ 
  */  
/*----------------------------------------------------------------------------*/


#ifndef _U_CLI_H_
#define _U_CLI_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* CLI API return values */
#define CLIR_INV_CMD_USAGE          ((INT32)   1)   /**<CLI command usage is invalid*/
#define CLIR_OK                     ((INT32)   0)   /**<Routine successful*/
#define CLIR_NOT_INIT               ((INT32)  -1)   /**<The component has not been initialized.*/
#define CLIR_ALREADY_INIT           ((INT32)  -2)   /**<The component has already been initialized.*/
#define CLIR_NOT_ENABLED            ((INT32)  -3)   /**<CLI is not enabled.*/
#define CLIR_INV_ARG                ((INT32)  -4)   /**<One or more of the arguments contain invalid data.*/
#define CLIR_INV_CMD_TBL            ((INT32)  -5)   /**<The command table is invalid*/
#define CLIR_CMD_TOO_LONG           ((INT32)  -6)   /**<CLI command is too long.*/
#define CLIR_ALIAS_TOO_LONG         ((INT32)  -7)   /**<Alias is too long.*/
#define CLIR_CMD_TBL_FULL           ((INT32)  -8)   /**<Command table is full.*/
#define CLIR_ALIAS_TBL_FULL         ((INT32)  -9)   /**<CLI alias table is full.*/
#define CLIR_CMD_NOT_FOUND          ((INT32) -10)   /**<CLI command not found.*/
#define CLIR_DIR_NOT_FOUND          ((INT32) -11)   /**<CLI directory not found.*/
#define CLIR_CMD_EXEC_ERROR         ((INT32) -12)   /**<CLI command execution failed.*/
#define CLIR_UNKNOWN_CMD            ((INT32) -13)   /**<Unknown CLI command.*/
#define CLIR_OUT_OF_MEM             ((INT32) -14)   /**<There is no enough memory when executing the CLI command*/
#define CLIR_DUP_KEY                ((INT32) -15)   /**<The key is duplicated*/
#define CLIR_MODULE_NAME_TOO_LONG   ((INT32) -16)   /**<Module name is too long.*/
#define CLIR_LOG_TBL_FULL			((INT32) -17)	/**<CLI log table is full.*/
#define CLIR_MODULE_UNREGIST		((INT32) -18)	/**<CLI module  is unregist.*/

/* ASCII key definiton */
#define CLI_ASCII_KEY_CTRL_B        ((INT8) 0x02)   /**<The key definition of CTRL+B*/

/* Definition of get & set debug level command and help strings */
#define CLI_GET_DBG_LVL_STR         "gdl"           /**<gdl string definition*/
#define CLI_SET_DBG_LVL_STR         "sdl"           /**<sdl string definition*/



#define CLI_GET_DBG_LVL_HELP_STR    "Get debug level (e=error, a=api, i=info, n=none)"  
/**<gdl help string definition*/



#define CLI_SET_DBG_LVL_HELP_STR    "Set debug level (e=error, a=api, i=info, n=none)"  
/**<sdl help string definition*/

/* Definition of group debug level control */
#define CLI_GRP_NONE                ((UINT64)   0x00000000)     /**<The group mask of null*/
#define CLI_GRP_PIPE                ((UINT64)   0x00000001)     /**<The group mask of pipe*/
#define CLI_GRP_GUI                 ((UINT64)   0x00000002)     /**<The group mask of gui*/
#define CLI_GRP_EPG                 ((UINT64)   0x00000004)     /**<The group mask of epg*/
#define CLI_GRP_DRV                 ((UINT64)   0x00000008)     /**<The group mask of drv*/
#define CLI_GRP_MAX                 ((UINT64)   0x00000010)     /**<The group mask of max*/

/* Definition of get & set time measurement level command and help strings */
#define CLI_GET_TMS_LVL_STR         "gtl"           /**<gtl string definition*/
#define CLI_SET_TMS_LVL_STR         "stl"           /**<stl string definition*/



#define CLI_GET_TMS_LVL_HELP_STR    "Get TMS level (r=real-time, o=off-line, n=none)"   
/**<gtl help string definition*/


#define CLI_SET_TMS_LVL_HELP_STR    "Set TMS level (r=real-time, o=off-line, n=none)"   
/**<stl help string definition*/

/* Definition of command table terminator */


#define END_OF_CLI_CMD_TBL          {NULL, NULL, NULL, NULL, NULL, CLI_HIDDEN}  

#define MODULE_NAME_MAX_LENTH			5		/* Module name's max lenth when regrestto log command */

/**<Definition of command table terminator */

/* CLI command access right */
/* New entries must be added to the end of the enumeration before CLI_HIDDEN */

/*------------------------------------------------------------------*/
/*! @brief     Indicates the access right of a cli command/directory.
  *  @code 
  *  typedef enum 
  *  { 
  *      CLI_SUPERVISOR = 0, 
  *      CLI_ADMIN,
  *      CLI_GUEST, 
  *      CLI_HIDDEN
  *  } CLI_ACCESS_RIGHT_T; 
  *  @endcode 
  *  @li@c  CLI_SUPERVISOR      -Commands used by MTK only
  *  @li@c  CLI_ADMIN               -Commands used by customer only.
  *  @li@c  CLI_GUEST               -Commands used by end user only.
  *  @li@c  CLI_HIDDEN             -Hide commands from help list.
  */
/*------------------------------------------------------------------*/
typedef enum
{
    CLI_SUPERVISOR = 0, /* Commands used by MTK only */    
    CLI_ADMIN,          /* Commands used by customer only */
    CLI_GUEST,          /* Commands used by end user only */
    CLI_HIDDEN          /* Hide commands from help list */
}   CLI_ACCESS_RIGHT_T;

#define CLI_MODE_SUPERVISOR         ((CLI_ACCESS_RIGHT_T) 0)    /**<The supervisor cli mode*/
#define CLI_MODE_ADMIN              ((CLI_ACCESS_RIGHT_T) 1)    /**<The admin cli mode*/
#define CLI_MODE_GUEST              ((CLI_ACCESS_RIGHT_T) 2)    /**<The guest cli mode*/

/* CLI category definition */
/*------------------------------------------------------------------*/
/*! @brief     Indicates the catetory of cli command.
  *  @code 
  *  typedef enum 
  *  { 
  *      CLI_CAT_ROOT = 0, 
  *      CLI_CAT_BASIC,
  *      CLI_CAT_APP, 
  *      CLI_CAT_MW
  *      CLI_CAT_DRV
  *      CLI_CAT_TEST
  *      CLI_CAT_MTK_TOOL
  *      CLI_CAT_MAX
  *  } CLI_CAT_T; 
  *  @endcode 
  *  @li@c  CLI_CAT_ROOT           -Command category is ROOT.
  *  @li@c  CLI_CAT_BASIC          -Command category is BASIC.
  *  @li@c  CLI_CAT_APP              -Command category is APP.
  *  @li@c  CLI_CAT_MW               -Command category is MW.
  *  @li@c  CLI_CAT_DRV             -Command category is DRV.
  *  @li@c  CLI_CAT_TEST            -Command category is TEST.
  *  @li@c  CLI_CAT_MTK_TOOL    -Command category is MTK_TOOL.
  *  @li@c  CLI_CAT_MAX             -The MAX indicator of category.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    CLI_CAT_ROOT = 0,
    CLI_CAT_BASIC,    
    CLI_CAT_APP,    
    CLI_CAT_MW,
    CLI_CAT_DRV,    /* For driver module use only */         
    CLI_CAT_TEST,   /* For test use only */             
    CLI_CAT_MTK_TOOL,        
    CLI_CAT_MAX
}   CLI_CAT_T;

/* CLI password calculation definition */
/*------------------------------------------------------------------*/
/*! @brief     Indicates the method to generate the cli password.
  *  @code 
  *  typedef enum 
  *  { 
  *      CLI_PASSWD_ODD = 0, 
  *      CLI_PASSWD_EVEN,
  *      CLI_PASSWD_TWO_DIGITS_SUM_DEC, 
  *      CLI_PASSWD_TWO_DIGITS_SUM_HEX
  *  } CLI_PASSWD_T; 
  *  @endcode 
  *  @li@c  CLI_PASSWD_ODD                                  -Password generated by odd digital num.
  *  @li@c  CLI_PASSWD_EVEN                                 -Password generaeted by even digital num.
  *  @li@c  CLI_PASSWD_TWO_DIGITS_SUM_DEC     -Password generated by sum two digital num in dec.
  *  @li@c  CLI_PASSWD_TWO_DIGITS_SUM_HEX     -Password generated by sum two digital num in hex.
  */  
/*------------------------------------------------------------------*/
typedef enum
{
    CLI_PASSWD_ODD = 0,
    CLI_PASSWD_EVEN,    
    CLI_PASSWD_TWO_DIGITS_SUM_DEC,
    CLI_PASSWD_TWO_DIGITS_SUM_HEX
}   CLI_PASSWD_T;


/* CLI execution function */
/*------------------------------------------------------------------*/
/*! @brief  This type defines the cli execute function prototype.
  *  @param [in] i4_argc        Contain the number of arguments.
  *  @param [in] pps_argv     Contain the points to the argument array.
  */
/*------------------------------------------------------------------*/
typedef INT32 (*x_cli_exec_fct)(INT32        i4_argc, 
                                const CHAR** pps_argv);
                              
/* CLI command table structure */
/*------------------------------------------------------------------*/
/*! @brief This structure contains the content of a cli command/path.
  *  @code 
  *  typedef struct _CLI_EXEC_T 
  *  { 
  *      CHAR*                             ps_cmd_str;
  *      CHAR*                             ps_cmd_abbr_str;
  *      x_cli_exec_fct                   pf_exec_fct; 
  *      struct _CLI_EXEC_T*         pt_next_level;
  *      CHAR*                              ps_cmd_help_str; 
  *      CLI_ACCESS_RIGHT_T      e_access_right;    
  *  } SM_OBSVR_FILTER_T; 
  *  @endcode 
  *  @li@c  ps_cmd_str                 -Command string. 
  *  @li@c  ps_cmd_abbr_str         -Command abbreviation string. 
  *  @li@c  pf_exec_fct                 -Execution function.
  *  @li@c  pt_next_level              -Next level command table. 
  *  @li@c  ps_cmd_help_str         -Command help string.
  *  @li@c  e_access_right            -Command access right.
  */
/*------------------------------------------------------------------*/
typedef struct _CLI_EXEC_T
{
    CHAR*                   ps_cmd_str;         /* Command string */
    CHAR*                   ps_cmd_abbr_str;    /* Command abbreviation string */
    x_cli_exec_fct          pf_exec_fct;        /* Execution function */
    struct _CLI_EXEC_T*     pt_next_level;      /* Next level command table */
    CHAR*                   ps_cmd_help_str;    /* Command help string */
    CLI_ACCESS_RIGHT_T      e_access_right;     /* Command access right */
}   CLI_EXEC_T;


/* log callback function */
/*------------------------------------------------------------------*/
/*! @brief  This type defines the cli execute function prototype.
  *  @param [in] i4_custom        Contain the number of arguments.
  *  @param [in] pv_tag     Contain the points to the regist tag.
  */
/*------------------------------------------------------------------*/
typedef INT32 (*x_log_callback_fct)(UINT32        i4_custom, 
                                VOID* pv_tag);

/* log  table structure */
/*------------------------------------------------------------------*/
/*! @brief This structure contains the content of a log command
  *  @code 
  *	typedef struct _CLI_LOG_STRUCTURE_T
  *	{
  *   	CHAR                   	s_module_name;		
  *    	VOID*                   	pv_tag;    			
  *		x_cli_exec_fct     		pfExecFun;        	
  *		x_cli_exec_fct 		pfCallbackFun;      
  *	} CLI_LOG_STRUCTURE_T;
  *  @endcode 
  *  @li@c  s_module_name             	-module name. 
  *  @li@c  pv_tag         			-customize tag . 
  *  @li@c  pfExecFun                	-Execution function.
  *  @li@c  pfCallbackFun              	-callback function. 
  */
/*------------------------------------------------------------------*/
typedef struct _CLI_LOG_STRUCTURE_T
{
    CHAR                   	s_module_name[MODULE_NAME_MAX_LENTH];		/* module name*/
    VOID*                   pv_tag;    			/* customize tag */
    x_cli_exec_fct     		pfExecFun;        	/* Execution function */
    x_cli_exec_fct 			pfCallbackFun;      /* callback function */
}   CLI_LOG_STRUCTURE_T;

/* UI Hot Keys callback function type */
/*------------------------------------------------------------------*/
/*! @brief  This type defines the cli ui hotkey callback function. It will be executed from key received context.
  *  @param [in] ui2_hotkeys    Indicates the sent hotkey.
  *  @param [in] pv_tag             Contains the user's private tag. 
  */
/*------------------------------------------------------------------*/
typedef INT32 (*x_cli_ui_hotkey_fct) (UINT16    ui2_hotkeys,
                                      VOID*     pv_tag);

/* UI Hot Keys */
#define CLI_UI_HOTKEY_AIR_DIG_SCAN      ((UINT16)   0x1681)     /**<The hot key definition of air digital scan*/
#define CLI_UI_HOTKEY_CAB_DIG_SCAN      ((UINT16)   0x1682)     /**<The hot key definition of cable digital scan*/
#define CLI_UI_HOTKEY_ANA_SCAN          ((UINT16)   0x1683)     /**<The hot key definition of air scan*/
#define CLI_UI_HOTKEY_DIG_EVCTX         ((UINT16)   0x8881)     /**<The hot key definition of digital evctx*/
#define CLI_UI_HOTKEY_ANA_EVCTX         ((UINT16)   0x8882)     /**<The hot key definition of analog evctx*/

#endif /* _U_CLI_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_CLI */
/*----------------------------------------------------------------------------*/


