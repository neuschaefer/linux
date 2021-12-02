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
 * rfc_ebml.h,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2008/10/20
 * lianming lin
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: lianming lin $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file defines ebml related structures/enums/macros/
           Author: lianming mcn07123 
           History:
           (1) 2008-10-20  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/

#ifndef _RFC_EBML_H_
#define _RFC_EBML_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/u_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "unicode/x_uc_str.h"
#include "mutil/mm_util.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "x_mm_common.h"
#include "u_handle.h"
#include "x_uc_str.h"
#include "../../mm_util.h"

#endif
/*-----------------------------------------------------------------------------

                    macros, defines, typedefs, enums
----------------------------------------------------------------------------*/
#ifndef _EBML_KERNEL_NO_VFP_
#define _EBML_KERNEL_NO_VFP_
#endif

#define EBML_NAME_MAX_M                  (16)
#define EBML_BASE_MAX_HEADER_LEN         (12)
#define EBML_INVALID_OFFSET              (0xFFFFFFFFFFFFFFFFll)
#define EBML_INVALID_VINT                (0xFFFFFFFFFFFFFFFFll)
#define EBML_INVALID_SVINT               (0xFFFFFFFFFFFFFFFFll)

/*return code definition*/
#define EBMLR_OK                           (0) 
#define EBMLR_ERR                          (1)
#define EBMLR_NULL_PTR                     (-1)
#define EBMLR_INVALID_VINT                 (-2)

/*ID compare*/
#define EBMLR_VINT_COMP_EQUAL                (0) 
#define EBMLR_VINT_COMP_LESS                 (-1) 
#define EBMLR_VINT_COMP_MORE                 (1) 
#define EBMLR_VINT_COMP_UNKNOWN              (-2) 


typedef INT32 (*ebml_copybytes_fct)   (VOID* pv_obj, 
                                       VOID* pv_buf, 
                                       SIZE_T z_buf_leng, 
                                       SIZE_T z_copy_size, 
                                       UINT32* pui4_size);
typedef INT32 (*ebml_input4bytes_fct) (VOID* pv_obj, 
                                       UINT32* pui4_data);
typedef INT32 (*ebml_input3bytes_fct) (VOID* pv_obj, 
                                       UINT32* pui4_data);
typedef INT32 (*ebml_input2bytes_fct) (VOID* pv_obj, 
                                       UINT16* pui2_data);
typedef INT32 (*ebml_input1byte_fct)  (VOID* pv_obj, 
                                       UINT8*  pui1_data);
typedef INT32 (*ebml_setpos_fct)      (VOID* pv_obj, 
                                       INT64 i8_offset, 
                                       UINT8 ui1_whence);
typedef INT32 (*ebml_getpos_fct)      (VOID* pv_obj, 
                                       UINT64* pui8_cur_pos);

/*ebml input callback functions table*/
typedef struct _EBML_INPUT_FCTS_T
{
    ebml_copybytes_fct   f_copybytes;
    ebml_setpos_fct      f_setpos;
    ebml_getpos_fct      f_getpos;
    ebml_input4bytes_fct f_input4bytes;
    ebml_input3bytes_fct f_input3bytes;
    ebml_input2bytes_fct f_input2bytes;
    ebml_input1byte_fct  f_input1byte; 
}EBML_INPUT_FCTS_T;
/*ebml input stream ,may be network or file */
typedef struct _EBML_INPUT_STREAM_T
{
    /*this defines the stream source, it can be minfo obj, midxbuld obj,etc.*/
    VOID*             pv_obj; 
    /*input operations*/
    EBML_INPUT_FCTS_T t_input_fcts;
}EBML_INPUT_STREAM_T;
/*ebml library object, one application can apply one object*/
typedef struct _EBML_LIB_T
{
    CHAR                ai1_name[EBML_NAME_MAX_M]; /*for example "matroska"*/
    UINT8               ui1_ver; /*lib version ,it is now 1.0*/
    HANDLE_T            h_handle; /*ebml lib handle*/
    BOOL                b_buf_input; /*directly operate memory data*/
    EBML_INPUT_STREAM_T t_input_stream;

    /*operations:
    ebml_lib_open/ebml_lib_close/ebml_get_app_name
    */
}EBML_LIB_T;


/*define ebml variable int type*/
typedef struct _EBML_VINT_T
{
    UINT8  ui1_id_len;
    UINT8  aui1_data[8]; /*no more than 8 bytes*/
    /*operations:
    ebml_vint/ebml_vint_compare/ebml_buf_vint
    */
}EBML_VINT_T;

/*define signed variable int*/
typedef EBML_VINT_T EBML_SVINT_T;

/*define ebml id */
typedef EBML_VINT_T EBML_ID_T;
/*operations:
ebml_id/embl_buf_id/ebml_id_compare/EBML_ELE_ID_M/EBML_PELE_ID_M
*/

/*define ebml size */
typedef EBML_VINT_T EBML_SIZE_T;
/*operations:
ebml_size/ebml_buf_size/EBML_ELE_SIZE_M/EBML_PELE_SIZE_M
*/


/*ebml basic structure, all other elements definition must contain this 
  value at first order*/
typedef struct _EBML_ELE_BASIC_T
{
    EBML_ID_T                 t_id;
    EBML_SIZE_T               t_size;
    UINT64                    ui8_ele_offset;
    struct _EBML_ELE_BASIC_T* pt_parent;
    struct _EBML_ELE_BASIC_T* pt_child_list;
    
    /*operations:
     ebml_ele_base/ebml_buf_ele_base
     /EBML_ELE_BASE_OFFSET_M/EBML_PELE_BASE_OFFSET_M
     /EBML_ELE_BASE_DATA_OFFSET_M/EBML_PELE_BASE_DATA_OFFSET_M
     /EBML_ELE_BASE_ID_M/EBML_PELE_BASE_ID_M
     /EBML_ELE_BASE_SIZE_M/EBML_PELE_BASE_SIZE_M/
    */
}EBML_ELE_BASIC_T;


/************begin: ebml supported value type definition **********************/
typedef UINT8         EBML_UINT8;
typedef INT8          EBML_INT8;
typedef UINT16        EBML_UINT16;
typedef INT16         EBML_INT16;
typedef UINT32        EBML_UINT32;
typedef INT32         EBML_INT32;
typedef UINT64        EBML_UINT64;
typedef INT64         EBML_INT64;

typedef UINT64        EBML_UINT;
typedef INT64         EBML_INT;

#ifdef _EBML_KERNEL_NO_VFP_
    typedef UINT32        EBML_FLOAT32;
    typedef UINT64        EBML_DOUBLE64;
/*now can only support 64bits*/
    typedef UINT64        EBML_FLOAT;
    /*typedef long long INT32   EBML_DOUBLE_80;*/
    typedef struct _IEEE_FLOAT32
    {
        UINT8  ui1_sign;
        UINT8  ui1_exponent;
        UINT32 ui4_mantissa;
    }IEEE_FLOAT32;
    typedef struct _IEEE_DOUBLE64
    {
        UINT8  ui1_sign;
        UINT16 ui2_exponent;
        UINT64 ui8_mantissa;
    }IEEE_DOUBLE64;

    #define EBML_NORMAL_NUMBER_M                0
    #define EBML_POS_INFINITY_M                 1
    #define EBML_NEG_INFINITY_M                 2
    #define EBML_NOT_A_NUMBER_M                 3
    #define EBML_SMALL_NUMBER_M                 4
    #define EBML_NORMAL_NUMBER_UNDER_FLOW_M     5
    #define EBML_NORMAL_NUMBER_OVER_FLOW_M      6
    #define EBML_NORMAL_NUMBER_NUME_SCALE_M     7
    #define EBML_NORMAL_NUMBER_DENO_SCALE_M     8
    
    typedef struct _FLOAT_FRACTION_T
    {
        UINT8  ui1_sign;     /*0 is positive 1 is negative*/
        UINT8  ui1_type;
        UINT64 aui8_nume_scale[2];
        UINT64 aui8_deno_scale[2];
        UINT64 ui8_numerator;
        UINT64 ui8_denominator;
        UINT64 ui8_int_part;
        UINT64 ui8_scale_part;
    }FLOAT_FRACTION_T;
#else
    typedef float        EBML_FLOAT32;
    typedef double       EBML_DOUBLE64;
    /*now can only support 64bits*/
    typedef double       EBML_FLOAT;
#endif

typedef UINT8         EBML_BYTE;
 /*distance in nasoseconds to the beginning of 2001-01-01 00:00:00 UTC*/
typedef INT64         EBML_DATE; 

typedef CHAR          EBML_CHAR;
typedef UINT16        EBML_WCHAR;


/************end: ebml supported value type definition***********************/


/************begin EBML basic elements definition*****************************/
/*1. EBML Version*/
typedef struct _EBML_VERSION_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_UINT        ui_ver;
}EBML_VERSION_T;
/*2. EBML Read Version*/
typedef struct _EBML_READ_VERSION_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_UINT        ui_ver;
}EBML_READ_VERSION_T;
/*3. EBML Max Id Width*/
typedef struct _EBML_MAX_ID_WIDTH_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_UINT        ui_width;
}_EBML_MAX_ID_WIDTH_T;
/*4. EBML Max Size Width*/
typedef struct _EBML_MAX_SIZE_WIDTH_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_UINT        ui_width;
}_EBML_MAX_SIZE_WIDTH_T;
/*5. EBML Doc Type*/
typedef struct _EBML_DOC_TYPE_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_CHAR*       pui1_type;
}EBML_DOC_TYPE_T;
/*6. EBML DocTypeVersion*/
typedef struct _EBML_DOC_TYPE_VER_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_UINT        ui_ver;
}EBML_DOC_TYPE_VER_T;
/*7. EBML DocTypeReadVersion*/
typedef struct _EBML_DOC_TYPE_R_VER_T
{
    EBML_ELE_BASIC_T t_base;
    EBML_UINT        ui_ver;
}EBML_DOC_TYPE_R_VER_T;
/*8. EBML Header*/
typedef struct _EBML_HEADER_T
{
    EBML_ELE_BASIC_T t_base;
}EBML_HEADER_T;
/*9. EBML CRC32*/
typedef struct _EBML_CRC32_T
{
    EBML_ELE_BASIC_T t_base;
}EBML_CRC32_T;
/*10. EBML VOID*/
typedef struct _EBML_VOID_T
{
    EBML_ELE_BASIC_T t_base;
}EBML_VOID_T;


/************end  EBML basic elements definition******************************/



/******************************************************************************
                       data declarations
*******************************************************************************/



/******************************************************************************
                       functions declarations
*******************************************************************************/


/**************begin  EBML_LIB related operations******************/
extern INT32 _ebml_lib_open(CHAR*                pi1_name, 
                            EBML_INPUT_STREAM_T* pt_input_stream, 
                            HANDLE_T*            ph_ebml);
extern VOID _ebml_get_app_name(HANDLE_T h_ebml, CHAR* pi1_name);
extern VOID _ebml_lib_close(HANDLE_T h_ebml);
/**************end defining EBML_LIB related operations********************/


/***********begin  EBML_VINT_T related operations***************/
extern INT32 _ebml_vint(HANDLE_T h_ebml, UINT64 ui8_offset, EBML_VINT_T* pt_this);
extern INT32 _ebml_buf_vint(VOID* pv_buf, UINT32 ui4_len, EBML_VINT_T* pt_this);
extern BOOL _ebml_vint_is_valid(EBML_VINT_T* pt_vint);
extern INT32 _ebml_vint_compare(EBML_VINT_T* pt_vint1, EBML_VINT_T* pt_vint2);
extern UINT64 _ebml_vint_get_value(EBML_VINT_T* pt_vint);

/***********end defining EBML_VINT_T related operations*****************/


#define _ebml_svint       _ebml_vint
#define _ebml_buf_svint   _ebml_buf_vint
extern INT64 _ebml_svint_get_value(EBML_SVINT_T* pt_vint);


/***********begin  EBML_ID_T related operations***************/
extern INT32 _ebml_id(HANDLE_T h_ebml,  UINT64 ui8_offset, EBML_ID_T* pt_this);
extern INT32 _ebml_buf_id(VOID* pv_buf, UINT32 ui4_len, EBML_ID_T* pt_this);
extern UINT32 _ebml_id_get_value(EBML_ID_T* pt_vint);
extern INT32 _ebml_id_compare(EBML_VINT_T* pt_vint, UINT32 ui4_id_value);

/***********end  EBML_ID_T related operations*****************/


/***********begin  EBML_SIZE_T related operations***************/
extern INT32 _ebml_size(HANDLE_T h_ebml,  UINT64 ui8_offset, EBML_SIZE_T* pt_this);
extern INT32 _ebml_buf_size(VOID* pv_buf, UINT32 ui4_len, EBML_SIZE_T* pt_this);
extern UINT64 _ebml_size_get_value(EBML_SIZE_T* pt_vint);

/***********end  EBML_SIZE_T related operations*****************/


/***********begin  EBML_ELE_BASIC_T related operations***************/
extern INT32 _ebml_ele_base(HANDLE_T          h_ebml, 
                            UINT64            ui8_offset, 
                            EBML_ELE_BASIC_T* pt_this);
extern INT32 _ebml_buf_ele_base(VOID*             pv_buf, 
                                UINT32            ui4_len, 
                                UINT64            ui8_offset,
                                EBML_ELE_BASIC_T* pt_this);
extern UINT64 _ebml_buf_ele_get_data_offset(EBML_ELE_BASIC_T* pt_this);
extern UINT32 _ebml_buf_ele_get_ele_size(EBML_ELE_BASIC_T* pt_this);
extern UINT32 _ebml_buf_ele_get_head_size(EBML_ELE_BASIC_T* pt_this);

/***********end  EBML_ELE_BASIC_T related operations*****************/


/***********begin: ebml supported value type related operations ***************/
extern INT32 _ebml_uint(HANDLE_T    h_ebml,
                        UINT64      ui8_offset, 
                        UINT32      ui4_size, 
                        EBML_UINT*  pui8_value);
extern INT32 _ebml_buf_uint(VOID*      pv_buf, 
                            UINT32     ui4_len, 
                            UINT32     ui4_size,
                            EBML_UINT* pui8_value);

extern INT32 _ebml_buf_luint(VOID*      pv_buf, 
                            UINT32     ui4_len, 
                            UINT32     ui4_size,
                            EBML_UINT* pui8_value1,
                            EBML_UINT* pui8_value2);

extern INT32 _ebml_int(HANDLE_T    h_ebml,
                       UINT64      ui8_offset, 
                       UINT32      ui4_size, 
                       EBML_INT*   pi8_value);
extern INT32 _ebml_buf_int(VOID*      pv_buf, 
                           UINT32     ui4_len, 
                           UINT32     ui4_size,
                           EBML_INT*  pi8_value);
   
extern INT32 _ebml_float(HANDLE_T    h_ebml, 
                         UINT64      ui8_offset,
                         UINT32      ui4_size,
                         EBML_FLOAT* pf8_value);

extern INT32 _ebml_buf_float(VOID*       pv_buf,  
                             UINT32      ui4_len,
                             UINT32      ui4_size,
                             EBML_FLOAT* pf4_value);

extern INT32 _ebml_float32_to_fra(EBML_FLOAT32      f4_value, 
                                  FLOAT_FRACTION_T* pt_fraction);
extern INT32 _ebml_double64_to_fra(EBML_DOUBLE64     f8_value, 
                                   FLOAT_FRACTION_T* pt_fraction);
extern INT32 _ebml_float_to_fraction(EBML_FLOAT        f8_value, 
                              UINT32            ui4_size,
                              FLOAT_FRACTION_T* pt_fraction);

extern INT32 _ebml_date(HANDLE_T   h_ebml, 
                        UINT64     ui8_offset,
                        UINT32     ui4_size,
                        EBML_DATE* pi8_date);

extern INT32 _ebml_buf_date(VOID*      pv_buf,
                            UINT32     ui4_len,
                            UINT32     ui4_size,
                            EBML_DATE* pi8_date);


extern EBML_CHAR* _ebml_string(HANDLE_T h_ebml, 
                               UINT64   ui8_offset,
                               UINT32   ui4_size);

extern EBML_CHAR* _ebml_buf_string(VOID* pv_buf, 
                                   UINT32 ui4_len,
                                   UINT32 ui4_size);

extern INT32 _ebml_string_free(EBML_CHAR** ppi1_str);


extern EBML_WCHAR* _ebml_utf8_string(HANDLE_T h_ebml, 
                                     UINT64   ui8_offset,
                                     UINT32   ui4_size);
   
extern EBML_WCHAR* _ebml_buf_utf8_string(VOID*   pv_buf, 
                                         UINT32  ui4_len,
                                         UINT32  ui4_size);

extern INT32 _ebml_utf16_string_free(EBML_WCHAR** ppui2_str);

extern EBML_CHAR* _ebml_get_str_from_uft16(EBML_WCHAR* pui2_str);

extern UINT32 _ebml_get_ele_id(EBML_ELE_BASIC_T* pt_element);

extern UINT64 _ebml_get_ele_size(EBML_ELE_BASIC_T* pt_element);

extern UINT8 _ebml_get_ele_head_len(EBML_ELE_BASIC_T* pt_element);
 
/***********end: ebml supported value type related operations *****************/

#endif /*_RFC_EBML_H_*/


