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
/*****************************************************/
/* Various timer routines.                           */
/* Al Aburto, aburto@nosc.mil, 18 Feb 1997           */
/*                                                   */
/* t = dtime() outputs the current time in seconds.  */
/* Use CAUTION as some of these routines will mess   */
/* up when timing across the hour mark!!!            */
/*                                                   */
/* For timing I use the 'user' time whenever         */
/* possible. Using 'user+sys' time is a separate     */
/* issue.                                            */
/*                                                   */
/* Example Usage:                                    */
/* [timer options added here]                        */
/* main()                                            */
/* {                                                 */
/* double starttime,benchtime,dtime();               */
/*                                                   */
/* starttime = dtime();                              */
/* [routine to time]                                 */
/* benchtime = dtime() - starttime;                  */
/* }                                                 */
/*                                                   */
/* [timer code below added here]                     */
/*****************************************************/

/***************************************************************/
/* Timer options. You MUST uncomment one of the options below  */
/* or compile, for example, with the '-DUNIX' option.          */
/***************************************************************/
/* #define Amiga       */
/* #define UNIX */
/* #define UNIX_Old    */
/* #define VMS         */
/* #define BORLAND_C   */
/* #define MSC         */
/* #define MAC         */
/* #define IPSC        */
/* #define FORTRAN_SEC */
/* #define GTODay      */
/* #define CTimer      */
/* #define UXPM        */
/* #define MAC_TMgr    */
/* #define PARIX       */
/* #define POSIX       */
/* #define WIN32       */
/* #define POSIX1      */
/* #define RISCOS      */
#define MTK_OSAI
/***********************/

/*********************************/
/* Timer code.                   */
/*********************************/
/*******************/
/*  Amiga dtime()  */
/*******************/
#ifdef Amiga
#include <ctype.h>
#define HZ 50

double dtime()
{
 double q;

 struct tt
       {
	long  days;
	long  minutes;
	long  ticks;
       } tt;

 DateStamp(&tt);

 q = ((double)(tt.ticks + (tt.minutes * 60L * 50L))) / (double)HZ;

 return q;
}
#endif

/*****************************************************/
/*  UNIX dtime(). This is the preferred UNIX timer.  */
/*  Provided by: Markku Kolkka, mk59200@cc.tut.fi    */
/*  HP-UX Addition by: Bo Thide', bt@irfu.se         */
/*****************************************************/
#ifdef UNIX
#include <time.h>
#include <sys/resource.h>

#ifdef hpux
#include <sys/syscall.h>
#define getrusage(a,b) syscall(SYS_getrusage,a,b)
#endif

struct rusage rusage;

double dtime()
{
 double q;

 getrusage(RUSAGE_SELF,&rusage);

 q = (double)(rusage.ru_utime.tv_sec);
 q = q + (double)(rusage.ru_utime.tv_usec) * 1.0e-06;

 return q;
}
#endif

/***************************************************/
/*  UNIX_Old dtime(). This is the old UNIX timer.  */
/*  Make sure HZ is properly defined in param.h !! */
/***************************************************/
#ifdef UNIX_Old
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

#ifndef HZ
#define HZ 60
#endif

struct tms tms;

double dtime()
{
 double q;

 times(&tms);

 q = (double)(tms.tms_utime) / (double)HZ;

 return q;
}
#endif

/*********************************************************/
/*  VMS dtime() for VMS systems.                         */
/*  Provided by: RAMO@uvphys.phys.UVic.CA                */
/*  Some people have run into problems with this timer.  */
/*********************************************************/
#ifdef VMS
#include time

#ifndef HZ
#define HZ 100
#endif

struct tbuffer_t
      {
       int proc_user_time;
       int proc_system_time;
       int child_user_time;
       int child_system_time;
      };

struct tbuffer_t tms;

double dtime()
{
 double q;

 times(&tms);

 q = (double)(tms.proc_user_time) / (double)HZ;

 return q;
}
#endif

/******************************/
/*  BORLAND C dtime() for DOS */
/******************************/
#ifdef BORLAND_C
#include <ctype.h>
#include <dos.h>
#include <time.h>

#define HZ 100
struct time tnow;

double dtime()
{
 double q;

 gettime(&tnow);

 q = 60.0 * (double)(tnow.ti_min);
 q = q + (double)(tnow.ti_sec);
 q = q + (double)(tnow.ti_hund)/(double)HZ;

 return q;
}
#endif

/**************************************/
/*  Microsoft C (MSC) dtime() for DOS */
/**************************************/
#ifdef MSC
#include <time.h>
#include <ctype.h>

#define HZ CLOCKS_PER_SEC
clock_t tnow;

double dtime()
{
 double q;

 tnow = clock();

 q = (double)tnow / (double)HZ;

 return q;
}
#endif

/*************************************/
/*  Macintosh (MAC) Think C dtime()  */
/*************************************/
#ifdef MAC
#include <time.h>

#define HZ 60

double dtime()
{
 double q;

 q = (double)clock() / (double)HZ;

 return q;
}
#endif

/************************************************************/
/*  iPSC/860 (IPSC) dtime() for i860.                       */
/*  Provided by: Dan Yergeau, yergeau@gloworm.Stanford.EDU  */
/************************************************************/
#ifdef IPSC
extern double dclock();

double dtime()
{
 double q;

 q = dclock();

 return q;
}
#endif

/**************************************************/
/*  FORTRAN dtime() for Cray type systems.        */
/*  This is the preferred timer for Cray systems. */
/**************************************************/
#ifdef FORTRAN_SEC

fortran double second();

double dtime()
{
 double q;

 second(&q);

 return q;
}
#endif

/***********************************************************/
/*  UNICOS C dtime() for Cray UNICOS systems.  Don't use   */
/*  unless absolutely necessary as returned time includes  */
/*  'user+system' time.  Provided by: R. Mike Dority,      */
/*  dority@craysea.cray.com                                */
/***********************************************************/
#ifdef CTimer
#include <time.h>

double dtime()
{
 double    q;
 clock_t   clock(void);

 q = (double)clock() / (double)CLOCKS_PER_SEC;

 return q;
}
#endif

/********************************************/
/* Another UNIX timer using gettimeofday(). */
/* However, getrusage() is preferred.       */
/********************************************/
#ifdef GTODay
#include <sys/time.h>

struct timeval tnow;

double dtime()
{
 double q;

 gettimeofday(&tnow,NULL);
 q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

 return q;
}
#endif

/*****************************************************/
/*  Fujitsu UXP/M timer.                             */
/*  Provided by: Mathew Lim, ANUSF, M.Lim@anu.edu.au */
/*****************************************************/
#ifdef UXPM
#include <sys/types.h>
#include <sys/timesu.h>
struct tmsu rusage;

double dtime()
{
 double q;

 timesu(&rusage);

 q = (double)(rusage.tms_utime) * 1.0e-06;

 return q;
}
#endif

/**********************************************/
/*    Macintosh (MAC_TMgr) Think C dtime()    */
/*   requires Think C Language Extensions or  */
/*    #include <MacHeaders> in the prefix     */
/*  provided by Francis H Schiffer 3rd (fhs)  */
/*         skipschiffer@genie.geis.com        */
/**********************************************/
#ifdef MAC_TMgr
#include <Timer.h>
#include <stdlib.h>

static TMTask   mgrTimer;
static Boolean  mgrInited = false;
static double   mgrClock;

#define RMV_TIMER RmvTime( (QElemPtr)&mgrTimer )
#define MAX_TIME  1800000000L
/* MAX_TIME limits time between calls to */
/* dtime( ) to no more than 30 minutes   */
/* this limitation could be removed by   */
/* creating a completion routine to sum  */
/* 30 minute segments (fhs 1994 feb 9)   */

static void Remove_timer( )
{
 RMV_TIMER;
 mgrInited = false;
}

double  dtime( )
{
 if( mgrInited ) {
   RMV_TIMER;
   mgrClock += (MAX_TIME + mgrTimer.tmCount)*1.0e-6;
 } else {
   if( _atexit( &Remove_timer ) == 0 ) mgrInited = true;
   mgrClock = 0.0;
 }

 if ( mgrInited )
   {
    mgrTimer.tmAddr = NULL;
    mgrTimer.tmCount = 0;
    mgrTimer.tmWakeUp = 0;
    mgrTimer.tmReserved = 0;
    InsTime( (QElemPtr)&mgrTimer );
    PrimeTime( (QElemPtr)&mgrTimer, -MAX_TIME );
   }
 return( mgrClock );
}
#endif

/***********************************************************/
/*  Parsytec GCel timer.                                   */
/*  Provided by: Georg Wambach, gw@informatik.uni-koeln.de */
/***********************************************************/
#ifdef PARIX
#include <sys/time.h>

double dtime()
{
 double q;

 q = (double) (TimeNowHigh()) / (double) CLK_TCK_HIGH;

 return q;
}
#endif

/************************************************/
/*  Sun Solaris POSIX dtime() routine           */
/*  Provided by: Case Larsen, CTLarsen.lbl.gov  */
/************************************************/
#ifdef POSIX
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/rusage.h>

#ifdef __hpux
#include <sys/syscall.h>
#endif

struct rusage rusage;

double dtime()
{
 double q;

 getrusage(RUSAGE_SELF,&rusage);

 q = (double)(rusage.ru_utime.tv_sec);
 q = q + (double)(rusage.ru_utime.tv_nsec) * 1.0e-09;

 return q;
}
#endif


/****************************************************/
/*  Windows NT (32 bit) dtime() routine             */
/*  Provided by: Piers Haken, piersh@microsoft.com  */
/****************************************************/
#ifdef WIN32
#include <windows.h>

double dtime(void)
{
 double q;

 q = (double)GetTickCount() * 1.0e-03;

 return q;
}
#endif

/*****************************************************/
/* Time according to POSIX.1  -  <J.Pelan@qub.ac.uk> */
/* Ref: "POSIX Programmer's Guide"  O'Reilly & Assoc.*/
/*****************************************************/
#ifdef POSIX1
#define _POSIX_SOURCE 1
#include <unistd.h>
#include <limits.h>
#include <sys/times.h>

struct tms tms;

double dtime()
{
 double q;
 times(&tms);
 q = (double)tms.tms_utime / (double)CLK_TCK;
 return q;
}
#endif
/*****************************************************/
/* RiscOS implementation of dtime(): Guy Griffin     */
/* uses clock()					     */
/*****************************************************/
#ifdef RISCOS
#include <time.h>
#include <ctype.h>

#define HZ CLOCKS_PER_SEC
clock_t time_now;

double dtime(void)
{
    register double q;

    time_now = clock();
    q = (double)time_now / (double)HZ;
    return (q);
}
#endif
/*****************************************************/
/* MTK OSAI implementation of dtime(): Xavier Wang   */
/* uses HAL_GetTime()                                */
/*****************************************************/
#ifdef MTK_OSAI

#include "x_timer.h"

#if 0
double dtime(void)
{
    HAL_TIME_T rTime;
    double fTime;

    HAL_GetTime(&rTime);

    fTime = rTime.u4Seconds + ((double)rTime.u4Micros / 1000000.0);

    return fTime;
}
#else //long version
unsigned int dtime(void)
{
    HAL_TIME_T rTime;
    long fTime;

    HAL_GetTime(&rTime);

    fTime = rTime.u4Seconds * 1000000 + rTime.u4Micros;

    return fTime;
}
#endif
#endif
