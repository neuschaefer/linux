/*
 *
 * hw_mbox_socket.h
 * Description:
 * contains AF_HWMBOX family sockets api for the puma7 HW_MBOX controller.
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015-2016 Intel Corporation.

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

  Copyright(c) 2015-2016 Intel Corporation. All rights reserved.

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

#ifndef _HW_MBOX_SOCKET_H_
#define _HW_MBOX_SOCKET_H_

/*! \enum hwMbox_socket_type
    \brief This enum denotes HW_MBOX socket types.
*/
enum hwMbox_socket_type {
    SOCK_COMMANDER = 0,
    SOCK_SUBJECT,
};

/*! \define HW_MBOX_SOCK_TYPE_GET_STR( _sockTypeVal )
    \brief Use this define to get string for HW_MBOX socket types.
    \param[in] _sockTypeVal - HW_MBOX socket type.
    \return string corresponding to <_sockTypeVal>.
*/
#define HW_MBOX_SOCK_TYPE_GET_STR( _sockTypeVal ) \
    ( (( _sockTypeVal ) == SOCK_COMMANDER) ? "COMMANDER" : \
      (( _sockTypeVal ) == SOCK_SUBJECT)   ? "SUBJECT"   : \
      "__ERROR_UNKNOWN_HW_MBOX_SOCKET_TYPE__" )

#ifdef __KERNEL__
/************************************
|          KERNEL's part            |
************************************/

#include <linux/types.h>
#include <linux/socket.h>

/*! \struct sockaddr_hwMbox
    \brief This structure denotes socket's address for HW_MBOX.
*/
struct sockaddr_hwMbox {
    __kernel_sa_family_t        shwMbox_family;
    __u8                        shwMbox_mid;
    __u16                       shwMbox_appid;
    __u8                        shwMbox_zero[sizeof(struct sockaddr) - (sizeof(__kernel_sa_family_t) + sizeof(__u8) + sizeof(__u16))];
} __attribute__((packed));

/*! \struct hwMbox_stat
    \brief HW_MBOX socket's statistics.
*/
struct hwMbox_stat {
    __u32       sent_cnt;
    __u32       sent_bytes;
    __u32       recv_cnt;
    __u32       recv_bytes;
};

/*! \struct hwMbox_sock
    \brief This structure denotes HW_MBOX socket's internals.
*/
struct hwMbox_sock {
    struct sock         sk;
    struct hwMbox_stat  stat;
    __u8                mid;
    __u8                cid;
    __u16               appid;
    __u16               owner_pid;
    __s32               chnIntr;
};

/*! \enum hwMbox_socket_state_e
    \brief This enum denotes HW_MBOX socket states.
*/
typedef enum hwMbox_socket_state {
    HW_MBOX_SOCK_STATE_CREATED = 0,
    HW_MBOX_SOCK_STATE_BOUND,
    HW_MBOX_SOCK_STATE_RECEIVING,
    HW_MBOX_SOCK_STATE_RECEIVED,
    HW_MBOX_SOCK_STATE_RECV_ERROR,
    HW_MBOX_SOCK_STATE_SENDING,
    HW_MBOX_SOCK_STATE_SENT,
    HW_MBOX_SOCK_STATE_SEND_ERROR
} hwMbox_socket_state_e;

/*! \define HW_MBOX_SOCK_TYPE_GET_STR( _sockStateVal )
    \brief Use this define to get string for HW_MBOX socket state.
    \note Value is stored in sock->state.
    \param[in] _sockStateVal - HW_MBOX socket state.
    \return string corresponding to <_sockStateVal>.
*/
#define HW_MBOX_SOCK_STATE_STR( _sockStateVal ) \
    ( ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_CREATED)    ? ("created"   ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_BOUND)      ? ("bound"     ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_RECEIVING)  ? ("receiving" ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_RECEIVED)   ? ("received"  ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_RECV_ERROR) ? ("recv err"  ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_SENDING)    ? ("sending"   ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_SENT)       ? ("sent"      ) : \
      ((hwMbox_socket_state_e)( _sockStateVal ) == HW_MBOX_SOCK_STATE_SEND_ERROR) ? ("send err"  ) : \
      "__ERROR_UNKNOWN_HW_MBOX_SOCKET_STATE__" )

/*! \fn struct hwMbox_sock * hwMbox_sk(struct sock *sk)
   \brief This is a typecast function.
   \param[in] sk - pointer to struct sock.
   \return pointer to struct HwMbox_sock.
*/
static inline struct hwMbox_sock * hwMbox_sk(struct sock *sk)
{
    return (struct hwMbox_sock *)sk;
}

#else /* __KERNEL__ */
/************************************
|           USER's part             |
************************************/

#include <sys/socket.h>

#define AF_HWMBOX       41
#define PF_HWMBOX       AF_HWMBOX

/*! \struct sockaddr_hwMbox
    \brief This structure denotes socket's address for HW_MBOX.
*/
struct sockaddr_hwMbox {
    sa_family_t       shwMbox_family;
    unsigned char     shwMbox_mid;
    unsigned short    shwMbox_appid;
    unsigned char     shwMbox_zero[sizeof(struct sockaddr) - (sizeof(sa_family_t) + sizeof(unsigned char) + sizeof(unsigned short))];
} __attribute__((packed));

#endif /* ! __KERNEL__ */
#endif /* ! _HW_MBOX_SOCKET_H_ */
