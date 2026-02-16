/*
 *
 * p_unit_api.h
 * Description:
 * power control unit device common type declarations
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

 *  Copyright(c) 2012-2015 Intel Corporation. All rights reserved.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2012-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*------------------------------------------------------------------------------
* File Name: p_unit_api.h
*------------------------------------------------------------------------------
*/
//! \file
#ifndef  P_UNIT_API_H
#define  P_UNIT_API_H


#define P_UNIT_INTERFACE_DRIVER_DEV_NAME "/dev/p_unit"

/* ************************************************ */

/* P_UnitResetType_e describe the reset type */
/* Alighn to P-Unit spec 1.1.9 */
typedef enum
{
    RESET_COLD_BOOT,
    RESET_COLD_RESET,
    RESET_WARM,
    RESET_ATOM_NAGRA_BBU_EXIT,
    RESET_ATOM,
    RESET_DOCSIS,
    RESET_FATAL,
    RESET_ATOM_TO_CSTATE,
    RESET_REASONS_MAX,

} P_UnitResetType_e;

#define P_UNIT_RESET_STR(v)         \
    static char * v[] =             \
    {                               \
        "RESET_COLD_BOOT ",         \
        "RESET_COLD_RESET",         \
        "RESET_WARM      ",         \
        "RESET_ATOM_NAGRA_BBU_EXIT",\
        "RESET_ATOM_ONLY ",         \
        "RESET_DOCSIS    ",         \
        "RESET_FATAL     ",         \
        "RESET_ATOM_TO_CSTATE",     \
    }

/* P_UnitResetOriginator_e describe the reset Initiator */
/* Alighn to P-Unit spec 1.1.9 */
typedef enum
{
    RESET_ORIGIN_HW,
    RESET_ORIGIN_8051_BUTTON,
    RESET_ORIGIN_ATOM,
    RESET_ORIGIN_ATOM_WATCHDOG,
    RESET_ORIGIN_DOCSIS,
    RESET_ORIGIN_DOCSIS_WATCHDOG,
    RESET_ORIGIN_BBU,
    RESET_ORIGIN_AEP_WD,
    RESET_ORIGIN_REBOOT_WDT,
    RESET_ORIGIN_MAX,
}P_UnitResetOriginator_e;

#define P_UNIT_RESET_ORIGIN_STR(v)      \
    static char * v[] =                 \
    {                                   \
        "RESET_ORIGIN_HW             ", \
        "RESET_ORIGIN_8051_BUTTON    ", \
        "RESET_ORIGIN_ATOM           ", \
        "RESET_ORIGIN_ATOM_WATCHDOG  ", \
        "RESET_ORIGIN_DOCSIS         ", \
        "RESET_ORIGIN_DOCSIS_WATCHDOG", \
        "RESET_ORIGIN_BBU            ", \
        "RESET_ORIGIN_AEP WD         ", \
        "RESET_ORIGIN_REBOOT_WDT     ", \
    }
/* ************************************************ */


/* ************************************************ */
typedef enum
{
    WATCHDOG_DO_COLD_RESET,
    WATCHDOG_DO_WARM_RESET,
    WATCHDOG_DO_CPU_RESET,
    WATCHDOG_DO_NOTHING,

} P_UnitWatchdogAction_e;

#define P_UNIT_WATCHDOG_ACT_STR(v)  \
    static char * v[] =             \
    {                               \
        "WATCHDOG_DO_COLD_RESET",   \
        "WATCHDOG_DO_WARM_RESET",   \
        "WATCHDOG_DO_CPU_RESET ",   \
        "WATCHDOG_DO_NOTHING   ",   \
    }
/* ************************************************ */


/* ***************************************************** */
/* notification Id upon APP cpu state change will notify to user */
typedef enum
{
    APP_REBOOT_STARTED,
    APP_SUSPEND_DONE,
    APP_LAST_INTERRUPT,

} P_UnitInterrupts_e;

/* notification Id upon APP cpu state change will notify to user */
typedef enum
{
    EVENT_NOT_REGISTERED,
    EVENT_REGISTERED,
} P_UnitEventRegister_e;

/* Struct received from user, to notify on pUnit interrupts */
typedef struct
{
    unsigned int destCtx;                       /* destination to send the event*/
    int eventId;                                /* event id to send */
    P_UnitEventRegister_e isEventRegistered;    /* save if event registered */
} P_UnitInterruptsEvent_t;

/* ************************************************ */

/********************************************************************************************************/
/* IOCTL commands:

   If you are adding new ioctl's to the kernel, you should use the _IO
   macros defined in <linux/ioctl.h> _IO macros are used to create ioctl numbers:

    _IO(type, nr)         - an ioctl with no parameter.
   _IOW(type, nr, size)  - an ioctl with write parameters (copy_from_user), kernel would actually read data from user space
   _IOR(type, nr, size)  - an ioctl with read parameters (copy_to_user), kernel would actually write data to user space
   _IOWR(type, nr, size) - an ioctl with both write and read parameters

   'Write' and 'read' are from the user's point of view, just like the
    system calls 'write' and 'read'.  For example, a SET_FOO ioctl would
    be _IOW, although the kernel would actually read data from user space;
    a GET_FOO ioctl would be _IOR, although the kernel would actually write
    data to user space.

    The first argument to _IO, _IOW, _IOR, or _IOWR is an identifying letter
    or number from the SoC_ModuleIds_e enum located in this file.

    The second argument to _IO, _IOW, _IOR, or _IOWR is a sequence number
    to distinguish ioctls from each other.

   The third argument to _IOW, _IOR, or _IOWR is the type of the data going
   into the kernel or coming out of the kernel (e.g.  'int' or 'struct foo').

   NOTE!  Do NOT use sizeof(arg) as the third argument as this results in
   your ioctl thinking it passes an argument of type size_t.

*/

#define SOC_P_UNIT_MODULE_ID                    (0xDA)
#define P_UNIT_RESET_SOC_REG_CMD                _IO  (SOC_P_UNIT_MODULE_ID, 1)
#define P_UNIT_GET_LAST_RESET_REASON_CMD        _IOR (SOC_P_UNIT_MODULE_ID, 2, unsigned long)
#define P_UNIT_GET_LAST_RESET_DURATION_CMD      _IOR (SOC_P_UNIT_MODULE_ID, 3, unsigned long)
#define P_UNIT_SET_WATCHDOG_ACTION_CMD          _IOW (SOC_P_UNIT_MODULE_ID, 4, unsigned long)
#define P_UNIT_SET_APP_CPU_EXIT_BBU_MODE        _IO  (SOC_P_UNIT_MODULE_ID, 5)
#define P_UNIT_UPGRADE_START                    _IO  (SOC_P_UNIT_MODULE_ID, 6)
#define P_UNIT_UPGRADE_DONE                     _IO  (SOC_P_UNIT_MODULE_ID, 7)
#define P_UNIT_SET_EVENTS                       _IOW (SOC_P_UNIT_MODULE_ID, 8, P_UnitInterruptsEvent_t *)
#define P_UNIT_GET_RESET_DURATION_SHADOW_CMD    _IOR (SOC_P_UNIT_MODULE_ID, 9, unsigned long) 
#define P_UNIT_CLR_RESET_DURATION_SHADOW_CMD    _IO  (SOC_P_UNIT_MODULE_ID, 10)
#define P_UNIT_RESET_APP_CPU_ONLY               _IO  (SOC_P_UNIT_MODULE_ID, 11)
#define P_UNIT_RESET_APP_CPU_TO_CSTATE          _IO  (SOC_P_UNIT_MODULE_ID, 12)


#ifdef __KERNEL__
/**************************************************************************/
/*! \fn  int p_unit_reset_soc ( void )
 **************************************************************************
 *  \brief This function is used to reset the entire SoC
 *  input:
 *  \return . fail / success
 **************************************************************************/
int p_unit_reset_soc( void );

/**************************************************************************/
/*! \fn  int p_unit_get_reset_reason ( unsigned long * )
 **************************************************************************
 *  \brief This function returns the last SoC reset reason
 *  input: None
 *  output: unsigned long  -  the reason encoded as P_UnitResetType_e
 *  \return . fail / success
 **************************************************************************/
int p_unit_get_reset_reason   ( unsigned long * reason   );


/**************************************************************************/
/*! \fn  int p_unit_get_reset_duration ( unsigned long * )
 **************************************************************************
 *  \brief This function returns the last SoC reset duration in usec
 *  input: None
 *  output: unsigned long  -  the last reset duration in msec
 *  \return . fail / success
 **************************************************************************/
int p_unit_get_reset_duration ( unsigned long * duration );

/**************************************************************************/
/*! \fn  int p_unit_get_reset_duration_shadow_reg ( unsigned long * )
 **************************************************************************
 *  \brief This function returns the last SoC shadow reset duration in msec
 *  input: None
 *  output: unsigned long  -  the last reset duration in msec
 *  \return . fail / success
 **************************************************************************/
int p_unit_get_reset_duration_shadow_reg ( unsigned long * duration );

/**************************************************************************/
/*! \fn  int p_unit_clear_reset_duration_shadow_reg ( void )
 **************************************************************************
 *  \brief This function clears the last SoC shadow reset duration
 *  input: None
 *  output: None  -  the last reset duration shadow reg is cleared
 *  \return . fail / success
 **************************************************************************/
int p_unit_clear_reset_duration_shadow_reg ( void );

/**************************************************************************/
/*! \fn  int p_unit_set_watchdog_action ( P_UnitWatchdogAction_e )
 **************************************************************************
 *  \brief This function configures watchdog behavior on expiry
 *  input: P_UnitWatchdogAction_e
 *  output: None
 *  \return . fail / success
 **************************************************************************/
int p_unit_set_watchdog_action( P_UnitWatchdogAction_e action );
#endif /* __KERNEL__ */
#endif /* P_UNIT_API_H */

