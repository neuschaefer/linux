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
 * $RCSfile: mm_util.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description: 
 *         This file exports all glu API for media utility 
 *---------------------------------------------------------------------------*/
#ifndef MM_UTIL_H_
#define MM_UTIL_H_

/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/ 
#ifndef __KERNEL__
#include "inc/u_common.h"
#include "mutil/minfo/u_minfo.h"
#include "cl/x_cl.h"
#include "dbg/u_dbg.h"

#else
#include "u_common.h"
#include "u_dbg.h"
#include "minfo/u_minfo.h"
#include "x_cl.h"

#endif
#include "mm_util_memory.h"
#ifndef CC_DRIVER_PROGRAM
#define ENABLE_CMOV 1
#else
#define ENABLE_CMOV 0
#endif

#ifndef DEBUG
#define MUTIL_DBG_LOG_MW_0(msg)  DBG_LOG_MW_0(msg)
#else
#define MUTIL_DBG_LOG_MW_0(msg)
#endif

#define ENABLE_MM_CLI_TEST 0
#define DLNA_SUPPORTED_MP4_MOOV_SIZE_MAX        (1*1024*1024)
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
*---------------------------------------------------------------------------*/

#define MIDXBULD_SYSTEM_CLOCK_FREQUENCY          90000


/*return code definition*/
#define MM_UTILR_NO_SUPPORT         -4
#define MM_UTILR_OUT_OF_MEM         -3
#define MM_UTILR_INV_ARG            -2
#define MM_UTILR_FAIL               -1 
#define MM_UTILR_OK                 0
#define MM_UTILR_ERR                1
#define BYTE3(arg)        (*((UINT8 *)&(arg) + 3))
#define BYTE2(arg)        (*((UINT8 *)&(arg) + 2))
#define BYTE1(arg)        (*((UINT8 *)&(arg) + 1))
#define BYTE0(arg)        (* (UINT8 *)&(arg))


#define LOAD_BYTE(arg1, arg2)  BYTE0(arg2) = *((UINT8 *)(arg1)); 

/* big endian */
#define LOADB_WORD(arg1, arg2)  BYTE1(arg2) = *((UINT8 *)(arg1)); \
                                BYTE0(arg2) = *((UINT8 *)(arg1) + 1)

/* little endian */
#define LOADL_WORD(arg1, arg2)  BYTE1(arg2) = *((UINT8 *)(arg1) + 1); \
                                BYTE0(arg2) = *((UINT8 *)(arg1))   
                             
/* big endian */
#define LOADB_DWRD(p, arg2)  BYTE3(arg2) = *((UINT8 *)(p)); \
                             BYTE2(arg2) = *((UINT8 *)(p) + 1); \
                             BYTE1(arg2) = *((UINT8 *)(p) + 2); \
                             BYTE0(arg2) = *((UINT8 *)(p) + 3)

/* little endian */  
#define LOADL_DWRD(p, arg2)  BYTE3(arg2) = *((UINT8 *)(p) + 3); \
                             BYTE2(arg2) = *((UINT8 *)(p) + 2); \
                             BYTE1(arg2) = *((UINT8 *)(p) + 1); \
                             BYTE0(arg2) = *((UINT8 *)(p))
                                  
#define LOADL_QWRD(p, arg2) ( (arg2) = ((UINT64)(p)[7] << 56)   \
                                        | ((UINT64)(p)[6] << 48)   \
                                        | ((UINT64)(p)[5] << 40)   \
                                        | ((UINT64)(p)[4] << 32)   \
                                        | ((UINT64)(p)[3] << 24)   \
                                        | ((UINT64)(p)[2] << 16)   \
                                        | ((UINT64)(p)[1] << 8)    \
                                        | (UINT64)(p)[0] )           

#define COPY_CDATA(pv_src, ui4_src_len, pv_dest, pui4_dest_len)      \
do                                                                  \
{                                                                   \
    if(  (pv_src  == NULL)                                          \
       ||(pv_dest == NULL)                                          \
       ||(ui4_src_len == 0)                                         \
       ||(pui4_dest_len == NULL)                                    \
       ||(*pui4_dest_len < 2))                                      \
    {                                                               \
        if ((NULL != pui4_dest_len) && (0 == ui4_src_len))          \
        {                                                           \
            *pui4_dest_len = 0;                                     \
        }                                                           \
        break;                                                      \
    }                                                               \
    if(ui4_src_len >= *pui4_dest_len)                               \
    {                                                               \
        x_memcpy(pv_dest, pv_src, (*pui4_dest_len - 1));            \
    }                                                               \
    else                                                            \
    {                                                               \
        x_memcpy(pv_dest, pv_src, ui4_src_len);                     \
        *pui4_dest_len = ui4_src_len;                               \
    }                                                               \
}while(0);


/*use for width char  ui4_src_len */
#define COPY_WCDATA(pv_src, ui4_src_len, pv_dest, pui4_dest_len)    \
do                                                                  \
{                                                                   \
    if(  (pv_src  == NULL)                                          \
       ||(pv_dest == NULL)                                          \
       ||(ui4_src_len == 0)                                         \
       ||(pui4_dest_len == NULL)                                    \
       ||(*pui4_dest_len < 4))                                      \
    {                                                               \
        if ((NULL != pui4_dest_len) && (0 == ui4_src_len))          \
        {                                                           \
            *pui4_dest_len = 0;                                     \
            if(pv_dest)                                             \
            {                                                       \
                *((UINT16*)pv_dest) = 0;                            \
            }                                                       \
        }                                                           \
        break;                                                      \
    }                                                               \
    if(ui4_src_len >= *pui4_dest_len/2)                             \
    {                                                               \
        x_memcpy(pv_dest, pv_src, (*pui4_dest_len/2-1)*2);          \
    }                                                               \
    else                                                            \
    {                                                               \
        x_memcpy(pv_dest, pv_src, ui4_src_len*2);                   \
        *pui4_dest_len = ui4_src_len;                               \
    }                                                               \
}while(0);

#define COPY_DATA  COPY_WCDATA


#ifdef __NO_FM__

#define FM_SEEK_BGN             ((UINT8) 1)
#define FM_SEEK_CUR             ((UINT8) 2)
#define FM_SEEK_END             ((UINT8) 3)
#define FM_SEEK_CHK             ((UINT8) 0x80)


#define FMR_OK                  ((INT32)  0)
#define FMR_EOF                 ((INT32) -13)

#endif

#ifdef __KERNEL__

/* mfmtrecg default debug level */
#ifndef DBG_INIT_LEVEL_MW_MFMTRECG
#define DBG_INIT_LEVEL_MW_MFMTRECG   DBG_LEVEL_NONE
#endif

/* minfo default debug level */
#ifndef DBG_INIT_LEVEL_MW_MINFO
#define DBG_INIT_LEVEL_MW_MINFO   DBG_LEVEL_NONE
#endif

/* midxbuld default debug level */
#ifndef DBG_INIT_LEVEL_MW_MIDXBULD
#define DBG_INIT_LEVEL_MW_MIDXBULD   DBG_LEVEL_NONE
#endif


#endif

/***************Begin Date related structure and operations********************/
#if 0
typedef struct _MM_TM_T
{
    UINT16 ui2_year;       /* This is a absolute year value e.g. 2008*/
    UINT8  ui1_month;      /* 1-12, month of the year. */
    UINT8  ui1_day;        /* 1-31, day of the month.  */
    UINT8  ui1_week;       /* 0-6, Sunday to Saturday. */
    UINT8  ui1_hour;       /* 0-23 */
    UINT8  ui1_min;        /* 0-59 */
    UINT8  ui1_sec;        /* 0-59, permit 61 */

    BOOL   b_gmt;          /* TRUE:  Universal Time Coordinate(UTC)
                                     Greenwich Mean Time(GMT)
                              FALSE: Local Time
                           */
    BOOL   b_dst;          /* TRUE:  DayLight-Saving-Time on
                              FALSE: Day-Light_Saving Time off
                           */
}MM_TM_T ;
#endif
typedef INT64 MM_TIME_T;


#define MM_GMT_ASF_EPOCH_YEAR_M    1601 
#define MM_GMT_AVI_EPOCH_YEAR_M    1970 
#define MM_GMT_MP4_EPOCH_YEAR_M    1904 
#define MM_GMT_MKV_EPOCH_YEAR_M    2001 

/*----------------------------------------------------------------
Number of seconds in a regular year and leap year
-----------------------------------------------------------------*/
#define   MM_SEC_PER_YR_M         31536000
#define   MM_SEC_PER_LEAP_YR_M    31622400

/*----------------------------------------------------------
  Number of seconds per day, hour, and minute.
------------------------------------------------------------*/

#define   MM_SEC_PER_DAY_M        86400
#define   MM_SEC_PER_HR_M         3600
#define   MM_SEC_PER_MIN_M        60


/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
#define   MM_IS_LEAP_YEAR(year)    \
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
  
#define MM_DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
/*
The lapsed leap day. for B.C year ,it is leap day count between y and 0
for A.D year it is leap day count between 1 and y
*/
#define MM_LEAP_LAPSED_NUM_BY(y) (MM_DIV(y, 4) - MM_DIV(y, 100) + MM_DIV(y, 400))

/***************End Date related structure and operations**********************/






/*The operations are GMT, not local time, no DST offset*/
extern BOOL _mm_is_leap_year(INT64 i8_year);
/*extern INT64 _mm_leap_lapsed_days_by(INT64 i8_year);*/
extern INT32 _mm_gmtime(MM_TIME_T t_time, UINT32 ui4_epoch_year, MM_TM_T* pt_tm);
extern VOID _mm_asctime(const MM_TM_T* pt_tm);


extern UINT64 _mm_div64(UINT64 ui8_dividend, 
                        UINT64 ui8_divider, 
                        UINT64* pui8_remainder);
extern VOID _mm_lang_to_639_2(UINT16          ui2_lang,  
                              ISO_639_LANG_T* pt_iso_639_lang);
extern VOID _mm_lang_639_2char_to_3char(ISO_639_LANG_T*  pt_iso_639_2char,  
                                        ISO_639_LANG_T*  pt_iso_639_3char);
#if 0
#ifdef __KERNEL__
typedef enum
{
    CL_NFY_HANDLE_CLOSED_TYPE
} CL_NFY_EVENT_TYPE_T;
/* the callback function used in x_cl_open_eng, x_cl_open_archive, 
   x_open_entry_by_name and x_cl_open_entry_by_index. */
typedef VOID (*x_cl_nfy_fct) (
              HANDLE_T              h_handle,   /* (I) The handle will be destoried */
              VOID*                 pv_nfy_tag, /* (I)The client data specified by the client */
              CL_NFY_EVENT_TYPE_T   e_event     /* (I)usually is CL_NFY_EVENT_TYPE_T */
              );
#endif
#endif

extern INT32 x_init_zlib_eng(
             const CHAR*    ps_name,  
             x_cl_nfy_fct   pf_nfy,   
             VOID*          pv_tag,
             HANDLE_T*      ph_engine );

extern INT32 x_do_zlib_decompress(
             HANDLE_T                 h_engine, 
             const VOID*              pv_src_buf, 
             SIZE_T                   z_src_len,  
             SIZE_T                   z_skip_len,
             UINT32                   ui4_param_type, 
             VOID*                    pv_param, 
             VOID*                    pv_dest_buf, 
             SIZE_T*                  pz_dest_len,
             HANDLE_T*                ph_prev_state);

extern INT32 x_close_zlib_eng(HANDLE_T h_handle);

#endif /* MM_UTIL_H_ */

