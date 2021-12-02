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
 * $RCSfile: x_edb_dvb_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains exported definitions and delcarations of
 *         Event Context DVB engine.
 *---------------------------------------------------------------------------*/

#ifndef _X_EDB_DVB_ENG_H_
#define _X_EDB_DVB_ENG_H_

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/
#include "u_edb_dvb_eng.h"



/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/

typedef struct _EDB_DVB_ENG_CONFIG_T
{
    EDB_DVB_CONFIG_T      t_eng_cfg_flag;
} EDB_DVB_ENG_CONFIG_T;


/*-----------------------------------------------------------------------------
  functions declaraions
  ----------------------------------------------------------------------------*/

extern INT32 x_edb_dvb_eng_init (const EDB_DVB_ENG_CONFIG_T*   pt_config);

/*
   INT32 x_edb_dvb_get_info_len (
   HANDLE_T                h_event,
   EDB_DVB_KEY_TYPE_T    e_dvb_key_type,
   VOID*                   pv_dvb_key_info,
   SIZE_T*                 pz_event_info_len,
   EDB_COND_T*           pe_cond);


   INT32 x_edb_dvb_get_info (
   HANDLE_T                h_event,
   EDB_DVB_KEY_TYPE_T    e_dvb_key_type,
   VOID*                   pv_dvb_key_info,
   SIZE_T*                 pz_event_info_len,
   VOID*                   pv_event_info,
   EDB_COND_T*           pe_cond);
   */                    




#endif /* _X_EDB_DVB_ENG_H_ */


