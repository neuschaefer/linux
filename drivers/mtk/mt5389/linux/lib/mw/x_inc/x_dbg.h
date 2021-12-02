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
 * $RCSfile: x_dbg.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains debug related definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/


 
 
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_DBG
  *  @{  
  */
/*----------------------------------------------------------------------------*/
#ifndef _X_DBG_H_
#define _X_DBG_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_dbg.h"
#include "u_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Macro to be used for level control debug statements. Note that */
/* the extern API declaration in the macro DBG_LEVEL_STMT was     */
/* placed there so that that API's external declaration is not    */
/* visible outside the macro's namespace.                         */
#undef DBG_LEVEL_STMT


#define DBG_LEVEL_STMT(_dbg_level, _stmt_level, _stmt)         \
{                                                              \
    if(x_dbg_check_lvl(_dbg_level,_stmt_level,__FILE__,__FUNCTION__,__LINE__,__TIME__,__DATE__))\
    {                                                            \
        x_dbg_ctrl_stmt _stmt;                                   \
    }                                                           \
}

/**<Macro to be used for level control debug statements.  Note that  
 the extern API declaration in the macro DBG_LEVEL_STMT was
 placed there so that that API's external declaration is not
 visible outside the macro's namespace. */


/* Debug setup types. */

/*------------------------------------------------------------------*/
/*! @brief     Indicates the output type of debug log information.
  *  @code 
  *  typedef enum 
  *  { 
  *      DBG_OUTPUT_TYPE_IGNORE = 0, 
  *      DBG_OUTPUT_TYPE_SERIAL_PORT
  *  } DBG_OUTPUT_TYPE_T; 
  *  @endcode 
  *  @li@c  DBG_OUTPUT_TYPE_IGNORE      -Ingore the debug log
  *  @li@c  DBG_OUTPUT_TYPE_SERIAL_PORT -Output the debug log to serial port.
  */
/*------------------------------------------------------------------*/
typedef enum
{
    DBG_OUTPUT_TYPE_IGNORE = 0,
    DBG_OUTPUT_TYPE_SERIAL_PORT
}   DBG_OUTPUT_TYPE_T;


/* Callback function definitions. */

/*------------------------------------------------------------------*/
/*! @brief  This type defines the debug log output callback function prototype.
  *  @param [in] ps_stmt           Contain the content to be output.
  */
/*------------------------------------------------------------------*/
typedef VOID (*x_dbg_output_fct) (const CHAR*  ps_stmt);


/*------------------------------------------------------------------*/
/*! @brief  This type defines the debug trace record callback function prototype.
  *  @param [in] b_trace_ena        Contain the switch of enable/disable trace.
  *  @param [in] ui4_code           Contain the code of trace info.
  *  @param [in] pv_data            Contain the data.
  *  @param [in] z_len              Contain the length of data. 
  */
/*------------------------------------------------------------------*/
typedef VOID (*x_dbg_trace_rec_fct) (BOOL         b_trace_ena,
                                     UINT32       ui4_code,
                                     const VOID*  pv_data,
                                     SIZE_T       z_len);
                                     
                                     
/*------------------------------------------------------------------*/
/*! @brief  This type defines the debug data input callback function prototype.
  *  @param [in] ui1_inp_data           Contain the input data.
  *  @param [in] pv_tag                 Contain the tag information.
  */
/*------------------------------------------------------------------*/
typedef VOID (*x_dbg_data_inp_fct) (UINT8  ui1_inp_data,
                                    VOID*  pv_tag);


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* The following API's are exported by the Middleware. */
/*------------------------------------------------------------------*/
/*! @brief  This API dumps the content of the trace buffer. Note that the
*           trace buffer will be disabled duriung the dumping and
*           re-enabled (if required) afterwards.
*  @param [in]   pf_output     References a rotine which will handle the outputing of the trace record.
*  @retval      -DBGR_OK                    Routine successful.
*  @retval      -DBGR_NOT_INIT              Debug library not initialized.
*  @retval      -DBGR_NO_TRACE_BUFFER       No trace buffer allocated.
*  @retval      -DBGR_NO_OUTPUT_DEVICE      No output device available.
*  @retval      -DBGR_DUMP_IN_PROGRESS      Trace buffer dump is in progress.
*  @retval      -DBGR_NOT_ENABLED           Debug library not compiled with define DEBUG.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_dump_trace_buff (x_dbg_output_fct  pf_output);

/*------------------------------------------------------------------*/
/*! @brief  This API clears the trace buffer. Essentially, it simply resets the read and write indices.
*  @retval      -DBGR_OK               Routine successful.
*  @retval      -DBGR_NOT_INIT         Debug library not initialized.
*  @retval      -DBGR_NO_TRACE_BUFFER  No trace buffer allocated.
*  @retval      -DBGR_NOT_ENABLED      Debug library not compiled with define DEBUG.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_flush_trace_buff (VOID);

/*------------------------------------------------------------------*/
/*! @brief  This API returns the trace buffer state.
*  @retval      -TRUE             The control flag of trace buffer is enable.
*  @retval      -FALSE            The control flag of trace buffer is disable.
*/
/*------------------------------------------------------------------*/
extern BOOL x_dbg_get_trace_buff_ctrl (VOID);

/*------------------------------------------------------------------*/
/*! @brief  This API registers a record trace callback function. To prevent
*           simmultanous access of the callback function pointer, a critical
*           section start / stop is used.
*  @param [in]   pf_trace_rec     References the record trace callback fucntion.
*  @retval      -DBGR_OK             Routine successful.
*  @retval      -DBGR_NOT_INIT       Debug library not initialized.
*  @retval      -DBGR_NOT_ENABLED    Debug library not compiled with define DEBUG
*  @retval      -DBGR_REG_CB_ACTIVE  A callback is still active.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_reg_trace_rec_cb (x_dbg_trace_rec_fct  pf_trace_rec);

/*------------------------------------------------------------------*/
/*! @brief  This API registers a data input callback function with the debug library.
*  @param [in]   pf_data_inp     References the data receive callback function.
*  @param [in]   pv_tag          Contains some tag value which is returned with the callback function.
*  @retval      -DBGR_OK             Routine successful.
*  @retval      -DBGR_NOT_INIT       Debug library not initialized.
*  @retval      -DBGR_NOT_ENABLED    Debug library not compiled with define DEBUG
*  @retval      -DBGR_REG_CB_ACTIVE  A callback is still active.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_reg_data_inp_cb (x_dbg_data_inp_fct  pf_data_inp,
									VOID*        	    pv_tag);
/*------------------------------------------------------------------*/
/*! @brief  This API enables or disables the trace buffer recording. This
*           API may be called from within an interrupt context.
*  @param [in]   b_trace_ena        Set to TRUE of the trace buffer recording shall be enabled else FALSE.
*  @retval      -DBGR_OK             Routine successful.
*  @retval      -DBGR_NOT_INIT       Debug library not initialized.
*  @retval      -DBGR_NOT_ENABLED    Debug library not compiled with define DEBUG
*  @retval      -DBGR_REG_CB_ACTIVE  A callback is still active.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_set_trace_buff_ctrl (BOOL  b_trace_ena);

/*------------------------------------------------------------------*/
/*! @brief  This API output the debug string as specified format.
*  @param [in]   ps_format     The specified output format.
*  @param [in]   ...                References to parameters of output.
*  @retval      -DBGR_OK                     Routine successful.
*  @retval      -DBGR_INV_ARG            The Input arguments are invalid.
*  @retval      -DBGR_NOT_INIT           The DBG moudle hasn't be initilized.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_stmt (const CHAR*  ps_format, ...);

/*------------------------------------------------------------------*/
/*! @brief  This API output the debug string as specified format by control method.
*  @param [in]   ps_format     The specified output format.
*  @param [in]   ...                References to parameters of output.
*  @retval      -DBGR_OK                      Routine successful.
*  @retval      -DBGR_INV_ARG            The Input arguments are invalid.
*  @retval      -DBGR_NOT_INIT           The DBG moudle hasn't be initilized.
*/
/*------------------------------------------------------------------*/
extern INT32 x_dbg_ctrl_stmt (const CHAR*  ps_format, ...);

/*------------------------------------------------------------------*/
/*! @brief  This API output  specia message (data ,time ,line)with the log level
*  @param [in]   ui4_dbg_lvl     The  module's current log level.
*  @param [in]   ui4_stmt_lvl     current dbg outout level
*  @param [in]   ps_file            file name
*  @param [in]   ps_func          function name
*  @param [in]   ui2_line           line number
*  @param [in]   ps_time          time
*  @param [in]   ps_date          date
*  @retval      TRUE                 need to printf dbg message.
*  @retval      FALSE               no need to printf dbg message.
*/
/*------------------------------------------------------------------*/
extern BOOL x_dbg_check_lvl(UINT32 ui4_dbg_lvl, 
                        UINT32 ui4_stmt_lvl,
                        const CHAR    *ps_file,
                        const CHAR    *ps_func,
                        UINT16  ui2_line,
                        CHAR   *ps_time,
                        CHAR   *ps_date);


#endif /* _X_DBG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_DBG*/
/*----------------------------------------------------------------------------*/




