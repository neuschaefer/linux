/* 

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014 Intel Corporation.

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

  Copyright(c) 2014 Intel Corporation. All rights reserved.

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

/*-----------------------------------------------------------------------------
* File Name: time_capture_drv.h
*------------------------------------------------------------------------------
*/
#ifndef  _TIME_CAPTURE_DRV_H_
#define  _TIME_CAPTURE_DRV_H_

#define TIME_CAPTURE_INTERFACE_DRIVER_DEV_NAME "/dev/time_capture"

/*-------------------------------------------------------------------------------------------
* 
* Time Capture
* 
* This module is used for measuring the boot time of eventes in the system.
* In order to add a new event one must insert a new time capturing enum
* and a title to the TIME_CAPTURE_EVENT_e enum and the eventsToString[] array below
* respectively, both on the same index, while incrementing TIME_CAPTURE_EVENTS_COUNT by one.
* 
* Afterwards one needs to insert a line in the code where the time capturing
* would be done.
* 
* For user space applications one should use the time_capture lib (part of ticc)
* include "time_capture_api.h" header and use the following:
* TC_TimeCapture(TIME_CAPTURE_EVENT_e event) command in the place measure is required.
* 
* For kernel space time measuring one should use the 
* TC_setEventTime(TIME_CAPTURE_EVENT_e event) command in the place measure is required.
* 
* 
*--------------------------------------------------------------------------------------------
*/

/* Events enum */
typedef enum
{
    TIME_CAPTURE_EVENT_RCS,
    TIME_CAPTURE_EVENT_PCD,
    TIME_CAPTURE_EVENT_DOCSIS_SCAN,
    TIME_CAPTURE_EVENT_DOCSIS_PROV,
    TIME_CAPTURE_EVENT_CONF_FILE,
    TIME_CAPTURE_EVENT_IP_PROV,
    TIME_CAPTURE_EVENT_DOCSIS_OP,
    TIME_CAPTURE_EVENT_VOICE,
    TIME_CAPTURE_EVENT_DIAL_TONE,
    TIME_CAPTURE_EVENT_GW_IPV4,
    TIME_CAPTURE_EVENT_GW_IPV6,
    TIME_CAPTURE_EVENT_WIFI,
    TIME_CAPTURE_EVENT_VFE_VMA,
    TIME_CAPTURE_EVENTS_COUNT
} TIME_CAPTURE_EVENT_e;   

#ifdef __KERNEL__

/* Titles of events are shown on proc display */
static const char* eventsToString[] = 
{
    "RCS script start",
    "PCD Start",
    "Docsis Scan start",
    "Docsis IP provisioning start",
    "Docsis Config file downloaded",
    "Docsis IP provisioning successful",
    "Docsis Operational",
    "Voice Provisioned",
    "Dial tone gained",
    "GW got IPv4 address",
    "GW got IPv6 address",
    "WiFi initialized",
    "VFE_VMA ready"
};


/**************************************************************************/
/*! \fn int TC_setEventTime(TIME_CAPTURE_EVENT_e event)
 **************************************************************************
 *  \brief  take a time stamp measure and add it to TIME CAPTURE times db
 *
 *  \param[in] event - enum of time event in the system
 *  \return 0 if ok -1 if error
 **************************************************************************/
extern int TC_setEventTime(TIME_CAPTURE_EVENT_e event);

#endif /* __KERNEL__ */


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

#define TIME_CAPTURE_MODULE_ID                         (0x87)
#define TIME_CAPTURE_EVENT_CMD                         _IOW  (TIME_CAPTURE_MODULE_ID, 1, unsigned long)

#endif /* _TIME_CAPTURE_DRV_H_ */

