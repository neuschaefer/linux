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
 * $RCSfile: mm_util.c,v $
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
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/ 
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "inc/u_sys_name.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"

#else

#include "u_common.h"
#include "u_sys_name.h"
#include "x_util.h"
#include "x_os.h"
#include "x_dbg.h"
#include "u_dbg.h"

#endif
#include "mm_util.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
*---------------------------------------------------------------------------*/
#define wBigTwoCC(a,b)    ((((UINT16)a)<<8)|(b))

#define ISO_639_1_LANG_NUM    201

typedef struct _MM_LANG_639_T
{
    CHAR s_two_cc[3];
    CHAR s_three_cc[4];
}MM_LANG_639_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static MM_LANG_639_T _a_lang_639_array[ISO_639_1_LANG_NUM]=
{
    {"aa", "aar"},
    {"ab", "abk"},
    {"af", "afr"},
    {"ak", "aka"},
    {"sq", "alb"},
    {"am", "amh"},
    {"ar", "ara"},
    {"an", "arg"},
    {"hy", "arm"},
    {"as", "asm"},
    {"av", "ava"},
    {"ae", "ave"},
    {"ay", "aym"},
    {"az", "aze"},
    {"ba", "bak"},
    {"bm", "bam"},
    {"eu", "baq"},
    {"be", "bel"},
    {"bn", "ben"},
    {"bh", "bih"},
    {"bi", "bis"},
    {"bo", "tib"},
    {"bs", "bos"},
    {"br", "bre"},
    {"bg", "bul"},
    {"my", "bur"},
    {"ca", "cat"},
    {"cs", "cze"},
    {"ch", "cha"},
    {"ce", "che"},
    {"zh", "chi"},
    {"cu", "chu"},
    {"cv", "chv"},
    {"kw", "cor"},
    {"co", "cos"},
    {"cr", "cre"},
    {"cy", "wel"},
    {"cs", "cze"},
    {"da", "dan"},
    {"de", "ger"},
    {"dv", "div"},
    {"nl", "dut"},
    {"dz", "dzo"},
    {"el", "gre"},
    {"en", "eng"},
    {"eo", "epo"},
    {"et", "est"},
    {"eu", "baq"},
    {"ee", "ewe"},
    {"fo", "fao"},
    {"fa", "per"},
    {"fj", "fij"},
    {"fi", "fin"},
    {"fr", "fre"},
    {"fy", "fry"},
    {"ff", "ful"},
    {"ka", "geo"},
    {"de", "ger"},
    {"gd", "gla"},
    {"ga", "gle"},
    {"gl", "glg"},
    {"gv", "glv"},
    {"el", "gre"},
    {"gn", "grn"},
    {"gu", "guj"},
    {"ht", "hat"},
    {"ha", "hau"},
    {"he", "heb"},
    {"hz", "her"},
    {"hi", "hin"},
    {"ho", "hmo"},
    {"hr", "hrv"},
    {"hu", "hun"},
    {"hy", "arm"},
    {"ig", "ibo"},
    {"is", "ice"},
    {"io", "ido"},
    {"ii", "iii"},
    {"iu", "iku"},
    {"ie", "ile"},
    {"ia", "ina"},
    {"id", "ind"},
    {"ik", "ipk"},
    {"is", "ice"},
    {"it", "ita"},
    {"jv", "jav"},
    {"ja", "jpn"},
    {"kl", "kal"},
    {"kn", "kan"},
    {"ks", "kas"},
    {"ka", "geo"},
    {"kr", "kau"},
    {"kk", "kaz"},
    {"km", "khm"},
    {"ki", "kik"},
    {"rw", "kin"},
    {"ky", "kir"},
    {"kv", "kom"},
    {"kg", "kon"},
    {"ko", "kor"},
    {"kj", "kua"},
    {"ku", "kur"},
    {"lo", "lao"},
    {"la", "lat"},
    {"lv", "lav"},
    {"li", "lim"},
    {"ln", "lin"},
    {"lt", "lit"},
    {"lb", "ltz"},
    {"lu", "lub"},
    {"lg", "lug"},
    {"mk", "mac"},
    {"mh", "mah"},
    {"ml", "mal"},
    {"mi", "mao"},
    {"mr", "mar"},
    {"ms", "may"},
    {"mg", "mlg"},
    {"mt", "mlt"},
    {"mo", "mol"},
    {"mn", "mon"},
    {"mi", "mao"},
    {"ms", "may"},
    {"my", "bur"},
    {"na", "nau"},
    {"nv", "nav"},
    {"nr", "nbl"},
    {"nd", "nde"},
    {"ng", "ndo"},
    {"ne", "nep"},
    {"nl", "dut"},
    {"nn", "nno"},
    {"nb", "nob"},
    {"no", "nor"},
    {"ny", "nya"},
    {"oc", "oci"},
    {"oj", "oji"},
    {"or", "ori"},
    {"om", "orm"},
    {"os", "oss"},
    {"pa", "pan"},
    {"fa", "per"},
    {"pi", "pli"},
    {"pl", "pol"},
    {"pt", "por"},
    {"ps", "pus"},
    {"qu", "que"},
    {"rm", "roh"},
    {"ro", "rum"},
    {"rn", "run"},
    {"ru", "rus"},
    {"sg", "sag"},
    {"sa", "san"},
    {"si", "sin"},
    {"sk", "slo"},
    {"sl", "slv"},
    {"se", "sme"},
    {"sm", "smo"},
    {"sn", "sna"},
    {"sd", "snd"},
    {"so", "som"},
    {"st", "sot"},
    {"es", "spa"},
    {"sq", "alb"},
    {"sc", "srd"},
    {"sr", "srp"},
    {"ss", "ssw"},
    {"su", "sun"},
    {"sw", "swa"},
    {"sv", "swe"},
    {"ty", "tah"},
    {"ta", "tam"},
    {"tt", "tat"},
    {"te", "tel"},
    {"tg", "tgk"},
    {"tl", "tgl"},
    {"th", "tha"},
    {"bo", "tib"},
    {"ti", "tir"},
    {"to", "ton"},
    {"tn", "tsn"},
    {"ts", "tso"},
    {"tk", "tuk"},
    {"tr", "tur"},
    {"tw", "twi"},
    {"ug", "uig"},
    {"uk", "ukr"},
    {"ur", "urd"},
    {"uz", "uzb"},
    {"ve", "ven"},
    {"vi", "vie"},
    {"vo", "vol"},
    {"cy", "wel"},
    {"wa", "wln"},
    {"wo", "wol"},
    {"xh", "xho"},
    {"yi", "yid"},
    {"yo", "yor"},
    {"za", "zha"},
    {"zh", "chi"},
    {"zu", "zul"}
};


/* How many days come before each month (0-12).  */
static const UINT32 gaui4_days_before_mon[2][13] =
{
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

/*
static const CHAR* gapi1_week[7]=
{
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
*/

/*----------------------------------------------------------------------------
 * Function: _mm_div64()
 *
 *---------------------------------------------------------------------------*/
UINT64 _mm_div64(UINT64 ui8_dividend, UINT64 ui8_divider, UINT64* pui8_remainder)
{
#ifdef __KERNEL__
    INT64 i8_divider = (INT64)ui8_divider;
    if(i8_divider < 0)
    {
        // We don't support negative division
        return 0;
    }
    if(ui8_divider >> 32)
    {
        return u8Div6464(ui8_dividend,ui8_divider,pui8_remainder);
    }
    return u8Div6432(ui8_dividend,ui8_divider,pui8_remainder);
    
#else

    UINT64 ui8_temp = 0;

    if (ui8_divider)
    {
        ui8_temp = ui8_dividend/ui8_divider;
    }
    
    if (pui8_remainder)
    {
        *pui8_remainder = ui8_dividend - (ui8_temp*ui8_divider);
    }
    return (ui8_temp);
    
#endif
}
 
VOID _mm_lang_to_639_2( UINT16          ui2_lang,  
                        ISO_639_LANG_T* pt_iso_639_lang )
{
    UINT16      ui2_idx;
    UINT16      ui2_curr_lang;
    CHAR*       psz_language;

    if(pt_iso_639_lang == NULL)
    {
        return;
    }

    psz_language = (CHAR*)pt_iso_639_lang;
    for(ui2_idx = 0; ui2_idx < ISO_639_1_LANG_NUM; ui2_idx ++)
    {
        ui2_curr_lang = wBigTwoCC(_a_lang_639_array[ui2_idx].s_two_cc[0], 
                                  _a_lang_639_array[ui2_idx].s_two_cc[1]);
        if(ui2_lang == ui2_curr_lang)
        {
            psz_language[0] = _a_lang_639_array[ui2_idx].s_three_cc[0];
            psz_language[1] = _a_lang_639_array[ui2_idx].s_three_cc[1];
            psz_language[2] = _a_lang_639_array[ui2_idx].s_three_cc[2];
            psz_language[3] = 0;
            /*x_strncpy(psz_language, 
                      (CHAR*)_a_lang_639_array[ui2_idx].s_three_cc,
                      ISO_639_LANG_LEN);*/
            break;
        }
    }
    psz_language[3] = 0;
}

VOID _mm_lang_639_2char_to_3char(ISO_639_LANG_T*  pt_iso_639_2char,  
                                 ISO_639_LANG_T*  pt_iso_639_3char)
{
    UINT16      ui2_idx;

    if((NULL == pt_iso_639_2char) || (NULL == pt_iso_639_3char))
    {
        return;
    }

    x_memset(pt_iso_639_3char, 0, ISO_639_LANG_LEN);
    for(ui2_idx = 0; ui2_idx < ISO_639_1_LANG_NUM; ++ui2_idx)
    {
        if (0 == x_strncmp((CHAR*)pt_iso_639_2char, 
                           (CHAR*)_a_lang_639_array[ui2_idx].s_two_cc, 
                           2))
        {
            x_strncpy((CHAR*)pt_iso_639_3char, 
                      (CHAR*)_a_lang_639_array[ui2_idx].s_three_cc, 
                      3);
            break;
        }
    }
    return;
}

BOOL _mm_is_leap_year(INT64 i8_year)
{
    UINT64 ui8_remainder4;
    UINT64 ui8_remainder100;
    UINT64 ui8_remainder400;
    UINT64 ui8_year;

    if (i8_year < 0)
    {
        ui8_year = (UINT64)((-1) * i8_year);
    }
    else
    {
        ui8_year = (UINT64)i8_year;
    }
    _mm_div64((UINT64)ui8_year, 
             4,
             (UINT64*)&ui8_remainder4);
    
    _mm_div64((UINT64)ui8_year, 
             100,
             (UINT64*)&ui8_remainder100);
    
    _mm_div64((UINT64)ui8_year, 
             400,
             (UINT64*)&ui8_remainder400);

    return (((ui8_remainder4 == 0) && 
             (ui8_remainder100 != 0 || ui8_remainder400 == 0)));
}
#if 0
/*
The lapsed leap day. for B.C year ,it is leap day count between y and 0
for A.D year it is leap day count between 1 and y
*/
INT64 _mm_leap_lapsed_days_by(INT64 i8_year)
{
    INT64 i8_remainder;
    INT64 i8_time4;
    INT64 i8_time100;
    INT64 i8_time400;
    

    i8_time4 = (INT64)_mm_div64((UINT64)i8_year, 
                                4,
                                (UINT64*)&i8_remainder);
    i8_time4 -= (i8_remainder < 0);
    
    
    i8_time100 = (INT64)_mm_div64((UINT64)i8_year, 
                                  100,
                                  (UINT64*)&i8_remainder);
    i8_time100 -= (i8_remainder < 0);

    
    i8_time400 = (INT64)_mm_div64((UINT64)i8_year, 
                                 400,
                                 (UINT64*)&i8_remainder);
    i8_time400 -= (i8_remainder < 0);

    return (i8_time4 - i8_time100 + i8_time400);

}

/*-----------------------------------------------------------------------------
 * Name:  _mm_gmtime
 *
 * Description: This function converts MM_TIME_T to MM_TM 
 *
 * Inputs:  t_time :      offsets by second
 *          i4_epoch_year:reference year, e.g. 1970.1.1. 00:00:00  
 *
 * Outputs: pt_tm         broken down time format MM_TM
 *
 * Returns: MM_UTILR_OK    Success. The routine was successful and the operation 
 *                         has been completed.
 *          MM_UTILR_ERR   Fail
 * Note: Here we do not 
 ----------------------------------------------------------------------------*/

INT32 _mm_gmtime(MM_TIME_T t_time, INT32 i4_epoch_year, MM_TM_T* pt_tm)
{
    MM_TIME_T      t_temp;
    INT64          i8_days;
    INT64          i8_remainder; 
    INT64          i8_year;
    INT64          i8_guess_year;
    const INT32*   pi4_days_before_mon;
    UINT8          ui1_idx;
    UINT32         ui4_epoch_week_day;
    
    if (NULL == pt_tm)  
    {
        /*DBG_ERROR(("_mm_gmtime: pt_tm is NULL\r\n"));*/
        return MM_UTILR_ERR;
    }

    /*init pt_tm*/
    pt_tm->ui2_year  = i4_epoch_year;
    pt_tm->ui1_month = 0;
    pt_tm->ui1_day   = 0;
    pt_tm->ui1_week  = 0;
    pt_tm->ui1_hour  = 0;
    pt_tm->ui1_min   = 0;
    pt_tm->ui1_sec   = 0;
    pt_tm->b_gmt     = FALSE;
    pt_tm->b_dst     = FALSE;
    t_temp           = t_time;
    
    /*
     ****|current prior|***************************************************
    ***************************|epoch point|*******************************
    ************************************************|current after|********
    */
    
    /*1. calculate the day offset to the epoch, it may be negative*/
    i8_days =(INT64)_mm_div64((UINT64)t_temp, 
                              MM_SEC_PER_DAY_M,
                              (UINT64*)&i8_remainder);
    /*if it is negative we must -1 because the time is from left to right*/
    if (i8_remainder < 0)
    {
        i8_remainder += MM_SEC_PER_DAY_M; 
        --i8_days;
    }
    
    /********now i8_remainder is positive*******************/

    /*2.get the hours of the day*/
    t_temp = i8_remainder;
    pt_tm->ui1_hour = (UINT8)_mm_div64((UINT64)t_temp, 
                                       MM_SEC_PER_HR_M, 
                                       (UINT64*)&i8_remainder);
    /*3.get the minute */
    t_temp = i8_remainder;
    pt_tm->ui1_min = (UINT8)_mm_div64((UINT64)t_temp, 
                                      MM_SEC_PER_MIN_M, 
                                      (UINT64*)&i8_remainder);

    /*4.get second of the minute*/
    pt_tm->ui1_sec = (UINT8)i8_remainder;

    /*5.get the weekday.To get epoch day's weekday*/
    switch (i4_epoch_year)
    {
        case MM_GMT_ASF_EPOCH_YEAR_M:
            ui4_epoch_week_day = 1;  /*1601-1-1*/
            break;
        case MM_GMT_AVI_EPOCH_YEAR_M:/*1970-1-1*/
            ui4_epoch_week_day = 4;
            break;
        case MM_GMT_MP4_EPOCH_YEAR_M:/*1904-1-1*/
            ui4_epoch_week_day = 5;
            break;
        case MM_GMT_MKV_EPOCH_YEAR_M:/*2001-1-1*/
            ui4_epoch_week_day = 1;
            break;
        default:
            ui4_epoch_week_day = 4; /*1970-1-1*/
    }
    
    _mm_div64(i8_days + ui4_epoch_week_day, 7, (UINT64*)&i8_remainder);
    /*if it is negative we must get the positive,time from left to right*/
    if (i8_remainder < 0)
    {
        i8_remainder += 7;
    }
    pt_tm->ui1_week = (UINT8)i8_remainder;
    
    /*6.Now begin to calculate the year*/
    i8_year = pt_tm->ui2_year;
    /*we must take account of A.D and B.C two cases*/
    while ((i8_days < 0) 
            || i8_days >= (_mm_is_leap_year(i8_year) ? 366 : 365))
    {
        /*we firstly assume that all year is 365 days then adjust it*/
        i8_guess_year = (INT64)_mm_div64((UINT64)i8_days, 365, (UINT64*)&i8_remainder);
        i8_guess_year += i8_year - (i8_remainder < 0);
        
        /*adjust it by leap day*/
        i8_days -=((i8_guess_year - i8_year) * 365
                   + _mm_leap_lapsed_days_by(i8_guess_year - 1)
                   - _mm_leap_lapsed_days_by(i8_year - 1));
        i8_year = i8_guess_year;
    }
    pt_tm->ui2_year = (UINT16)i8_year;
    
    pi4_days_before_mon = &gaui1_days_before_mon[MM_IS_LEAP_YEAR(pt_tm->ui2_year)][0];
    for (ui1_idx = 11; i8_days < pi4_days_before_mon[ui1_idx]; --ui1_idx)
    {
        continue;
    }
    
    i8_days -= pi4_days_before_mon[ui1_idx];
    pt_tm->ui1_month = ui1_idx + 1;
    pt_tm->ui1_day = (UINT8)(i8_days + 1);
    
    return MM_UTILR_OK;
    
}
#endif


/*-----------------------------------------------------------------------------
 * Name:  _mm_gmtime
 *
 * Description: This function converts MM_TIME_T to MM_TM 
 *
 * Inputs:  t_time :      offsets by second
 *          ui4_epoch_year:reference year, e.g. 1970.1.1. 00:00:00  
 *
 * Outputs: pt_tm         broken down time format MM_TM
 *
 * Returns: MM_UTILR_OK    Success. The routine was successful and the operation 
 *                         has been completed.
 *          MM_UTILR_ERR   Fail
 * Note:    ui4_epoch_year > 0
 ----------------------------------------------------------------------------*/
INT32 _mm_gmtime(MM_TIME_T t_time, UINT32 ui4_epoch_year, MM_TM_T* pt_tm)
{
    UINT8           ui1_idx;
    UINT32          ui4_epoch_week_day;

    UINT64          ui8_days;
    UINT64          ui8_remainder; 
    UINT64          ui8_year;
    UINT64          ui8_time;
    BOOL            b_leap_year;
    const UINT32*   pui4_days_before_mon;

    
    if (NULL == pt_tm)  
    {
        /*DBG_ERROR(("_mm_gmtime: pt_tm is NULL\r\n"));*/
        return MM_UTILR_ERR;
    }

    /*init pt_tm*/
    pt_tm->ui2_year  = (UINT16)ui4_epoch_year;
    pt_tm->ui1_month = 0;
    pt_tm->ui1_day   = 0;
    pt_tm->ui1_week  = 0;
    pt_tm->ui1_hour  = 0;
    pt_tm->ui1_min   = 0;
    pt_tm->ui1_sec   = 0;
    pt_tm->b_gmt     = FALSE;
    pt_tm->b_dst     = FALSE;
    
    /*
     ****|current prior|***************************************************
    ***************************|epoch point|*******************************
    ************************************************|current after|********
    */
    
    if (t_time >= 0)  /*after reference epoch*/
    {
        ui8_time = (UINT64)t_time;
        
        /*1. calculate the day offset to the epoch, it may be negative*/
        ui8_days =_mm_div64(ui8_time, 
                            MM_SEC_PER_DAY_M,
                            &ui8_remainder);

        /*2.get the hours of the day*/
        ui8_time = ui8_remainder;
        pt_tm->ui1_hour = (UINT8)_mm_div64(ui8_time, 
                                           MM_SEC_PER_HR_M, 
                                           &ui8_remainder);
        /*3.get the minute */
        ui8_time = ui8_remainder;
        pt_tm->ui1_min = (UINT8)_mm_div64(ui8_time, 
                                          MM_SEC_PER_MIN_M, 
                                          &ui8_remainder);

        /*4.get second of the minute*/
        pt_tm->ui1_sec = (UINT8)ui8_remainder;

        /*5.get the weekday.To get epoch day's weekday*/
        switch (ui4_epoch_year)
        {
            case MM_GMT_ASF_EPOCH_YEAR_M:
                ui4_epoch_week_day = 1;  /*1601-1-1*/
                break;
            case MM_GMT_AVI_EPOCH_YEAR_M:/*1970-1-1*/
                ui4_epoch_week_day = 4;
                break;
            case MM_GMT_MP4_EPOCH_YEAR_M:/*1904-1-1*/
                ui4_epoch_week_day = 5;
                break;
            case MM_GMT_MKV_EPOCH_YEAR_M:/*2001-1-1*/
                ui4_epoch_week_day = 1;
                break;
            default:
                ui4_epoch_week_day = 4; /*1970-1-1*/
        }
        
        _mm_div64(ui8_days + ui4_epoch_week_day, 7, (UINT64*)&ui8_remainder);
        /*if it is negative we must get the positive,time from left to right*/
        
        pt_tm->ui1_week = (UINT8)ui8_remainder;
        
        /*6.Now begin to calculate the year*/
        ui8_year = (UINT64)ui4_epoch_year;
        b_leap_year = _mm_is_leap_year((INT64)ui8_year);
        while(ui8_days >= (UINT64)(b_leap_year ? 366 : 365))
        {
            if (TRUE == b_leap_year)
            {
                ui8_days -= 366;
            }
            else
            {
                ui8_days -= 365;
            }
            ++ui8_year;
            b_leap_year = _mm_is_leap_year((INT64)ui8_year);
        }
        
        pt_tm->ui2_year = (UINT16)ui8_year;
        
        pui4_days_before_mon = 
                 &gaui4_days_before_mon[_mm_is_leap_year((INT64)(pt_tm->ui2_year))][0];
        for (ui1_idx = 11; ui8_days < pui4_days_before_mon[ui1_idx]; --ui1_idx)
        {
            continue;
        }
        
        ui8_days -= pui4_days_before_mon[ui1_idx];
        pt_tm->ui1_month = (UINT8)(ui1_idx + 1);
        pt_tm->ui1_day = (UINT8)(ui8_days + 1);    
    }
    else /*before epoch*/
    {
        ui8_time = (UINT64)((-1)* t_time);
        
        /*1. calculate the day offset to the epoch, it may be negative*/
        ui8_days = _mm_div64(ui8_time, 
                             MM_SEC_PER_DAY_M,
                             &ui8_remainder);
        
        /*if it is negative we must -1 because the time is from left to right*/
        if (ui8_remainder > 0)
        {
            ui8_remainder = MM_SEC_PER_DAY_M - ui8_remainder; 
            ++ui8_days;
        }
        
        /********now i8_remainder is positive*******************/

        /*2.get the hours of the day*/
        ui8_time = ui8_remainder;
        pt_tm->ui1_hour = (UINT8)_mm_div64(ui8_time, 
                                           MM_SEC_PER_HR_M, 
                                           &ui8_remainder);
        /*3.get the minute */
        ui8_time = ui8_remainder;
        pt_tm->ui1_min = (UINT8)_mm_div64(ui8_time, 
                                          MM_SEC_PER_MIN_M, 
                                          &ui8_remainder);

        /*4.get second of the minute*/
        pt_tm->ui1_sec = (UINT8)ui8_remainder;

        /*5.get the weekday.To get epoch day's weekday*/
        switch (ui4_epoch_year)
        {
            case MM_GMT_ASF_EPOCH_YEAR_M:
                ui4_epoch_week_day = 1;  /*1601-1-1*/
                break;
            case MM_GMT_AVI_EPOCH_YEAR_M:/*1970-1-1*/
                ui4_epoch_week_day = 4;
                break;
            case MM_GMT_MP4_EPOCH_YEAR_M:/*1904-1-1*/
                ui4_epoch_week_day = 5;
                break;
            case MM_GMT_MKV_EPOCH_YEAR_M:/*2001-1-1*/
                ui4_epoch_week_day = 1;
                break;
            default:
                ui4_epoch_week_day = 4; /*1970-1-1*/
        }
        
        _mm_div64(ui8_days, 7, &ui8_remainder);
        if (ui4_epoch_week_day >= (UINT32)ui8_remainder)
        {
            pt_tm->ui1_week = (UINT8)(ui4_epoch_week_day - ui8_remainder);
        }
        else
        {
            pt_tm->ui1_week = (UINT8)(ui4_epoch_week_day + 7 - ui8_remainder);
        }
        
        /*6.Now begin to calculate the year*/

        ui8_year = (UINT64)(ui4_epoch_year - 1);
        b_leap_year = _mm_is_leap_year((INT64)ui8_year);
        while(ui8_days >= (UINT64)(b_leap_year ? 366 : 365))
        {
            if (TRUE == b_leap_year)
            {
                ui8_days -= 366;
            }
            else
            {
                ui8_days -= 365;
            }
            --ui8_year;
            b_leap_year = _mm_is_leap_year((INT64)ui8_year);
        }

        if (ui8_days > 0)
        {
            ui8_days = (b_leap_year ? 366 : 365) - ui8_days;
        }
        pt_tm->ui2_year = (UINT16)ui8_year;
        
        pui4_days_before_mon = 
             &gaui4_days_before_mon[_mm_is_leap_year((INT64)(pt_tm->ui2_year))][0];
        for (ui1_idx = 11; ui8_days < pui4_days_before_mon[ui1_idx]; --ui1_idx)
        {
            continue;
        }
        
        ui8_days -= pui4_days_before_mon[ui1_idx];
        pt_tm->ui1_month = (UINT8)(ui1_idx + 1);
        pt_tm->ui1_day = (UINT8)(ui8_days + 1);     
    }
    
    return MM_UTILR_OK;
    
}



VOID _mm_asctime(const MM_TM_T* pt_tm)
{
    if (NULL == pt_tm)
    {
        /**/
        return;
    }
    #if 0
    DBG_INFO(("UTC Time:%u-%u-%u %u:%u:%u %s\r\n",
              pt_tm->ui2_year, 
              pt_tm->ui1_month,
              pt_tm->ui1_day,
              pt_tm->ui1_hour,
              pt_tm->ui1_min,
              pt_tm->ui1_sec,
              gapi1_week[pt_tm->ui1_week]));
    return;
    #endif
    
}


INT32 x_init_zlib_eng(
             const CHAR*    ps_name,  
             x_cl_nfy_fct   pf_nfy,   
             VOID*          pv_tag,
             HANDLE_T*      ph_engine )
{
#ifndef __KERNEL__
    return x_cl_open_eng(SN_CL_ZIP, NULL, NULL, ph_engine);
#else
    return -1;
#endif
}

INT32 x_do_zlib_decompress(
             HANDLE_T                 h_engine, 
             const VOID*              pv_src_buf, 
             SIZE_T                   z_src_len,  
             SIZE_T                   z_skip_len,
             UINT32                   ui4_param_type, 
             VOID*                    pv_param, 
             VOID*                    pv_dest_buf, 
             SIZE_T*                  pz_dest_len,
             HANDLE_T*                ph_prev_state)
{
#ifndef __KERNEL__
    return x_cl_decompress (h_engine, 
                            pv_src_buf, 
                            z_src_len,  
                            z_skip_len,
                            ui4_param_type, 
                            pv_param,  
                            pv_dest_buf, 
                            pz_dest_len,
                            ph_prev_state);
#else
    return -1;
#endif
}

INT32 x_close_zlib_eng(HANDLE_T h_handle)
{
#ifndef __KERNEL__
    return x_cl_close(h_handle);
#else
    return -1;
#endif
}


