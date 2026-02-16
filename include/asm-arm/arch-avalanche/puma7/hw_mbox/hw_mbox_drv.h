/*
 *
 * hw_mbox_drv.h
 * Description:
 * HW_MBOX driver API.
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

#ifndef _HW_MBOX_DRV_H_
#define _HW_MBOX_DRV_H_
/************************************************************************/
/*     INCLUDES                                                         */
/************************************************************************/

#include "hw_mbox_defs.h"   /* HW_MBOX definitions and configuration */

#ifdef __KERNEL__
/************************************************************************/
/*     HW_MBOX Kernel INTERFACE FUNCTIONS Prototypes:                   */
/************************************************************************/

/**************************************************************************/
/*! \fn int hwMbox_isReady(void)
 **************************************************************************
 *  \brief Returns whether HW MBOX driver is initailized or not.
 *  \return [ 0 on initialized / -1 on NOT initialized ].
 **************************************************************************/
int hwMbox_isReady(void);

/**************************************************************************/
/*! \fn int hwMbox_getChannelConfig(hw_mbox_Masters_e peer,
                                    hw_mbox_type_e mboxType,
                                    Bool isPeerCommander,
                                    hw_mbox_channelConfig_t *cfg)
 **************************************************************************
 *  \brief Get channel configuration.
 *  \param[in] peer - channel peer .
 *  \param[in] mboxType - channel type.
 *  \param[in] isPeerCommander - indicates whether the peer is the commander
 *                               of the channel.
 *  \param[out] cfg - channel configuration, if found.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_getChannelConfig(hw_mbox_Masters_e peer,
                            hw_mbox_type_e mboxType,
                            Bool isPeerCommander,
                            hw_mbox_channelConfig_t *cfg);

/**************************************************************************/
/*! \fn int hwMbox_sendCommand(hw_mbox_Masters_e subject,
                               Uint32 *cmd,
                               Uint8 *dataBuf, Uint32 dataLen)
 **************************************************************************
 *  \brief Send a command to <subject> in a blocking manner using the HW_MBOX.
 *  \attention will only return upon reply/ACK from <subject> (or error).
 *  \param[in]     subject - To which Master should the opcode be sent.
 *  \param[in,out] cmd - Command to send, upon returning will hold the reply.
 *  \param[in,out] dataBuf - pointer to a Buffer containing more data to be
 *                           sent, upon returning will hold the reply data
 *                           Buffer (copies as much as the buffer can hold).
 *  \param[in]     dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendCommand(hw_mbox_Masters_e subject,
                       Uint32 *cmd,
                       Uint8 *dataBuf, Uint32 dataLen);

/**************************************************************************/
/*! \fn int hwMbox_recvCommand(hw_mbox_Masters_e commander,
                               Uint32 *cmd,
                               Uint8 *dataBuf, Uint32 dataLen)
 **************************************************************************
 *  \brief Receive a command from <commander> using the HW_MBOX.
 *  \attention will only return upon arrival of a command from <commander>
 *             (or error).
 *  \note To reply call HW_MBOX_sendReply [if a meaningful response is expected].
 *  \note To ACK call HW_MBOX_sendAck [to simply ACK the command].
 *  \warning The subsequent call to HW_MBOX_sendReply/HW_MBOX_sendAck is extermely
 *           important as until it is called the channel is not released!!!
 *  \param[in]  commander - From which Master should a command be received.
 *  \param[out] cmd - Command that was received.
 *  \param[out] dataBuf - pointer to a Buffer containing more data that was sent.
 *  \param[in]  dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_recvCommand(hw_mbox_Masters_e commander,
                       Uint32 *cmd,
                       Uint8 *dataBuf, Uint32 dataLen);

/**************************************************************************/
/*! \fn int hwMbox_sendReplyCommand(hw_mbox_Masters_e commander,
                                    Uint32 reply,
                                    Uint8 *dataBuf, Uint32 dataLen)
 **************************************************************************
 *  \brief Send back a reply to <commander>, from which a command has been
 *         received using the HW_MBOX.
 *  \note should be called when a meaningful response is expected.
 *  \attention should only be used after a command has been received from
 *             <commander> using hwMbox_recvCommand.
 *  \param[in] commander - To which Master should the reply be sent.
 *  \param[in] reply - reply to send.
 *  \param[in] dataBuf - pointer to a Buffer containing more data to be sent.
 *  \param[in] dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendReplyCommand(hw_mbox_Masters_e commander,
                            Uint32 reply,
                            Uint8 *dataBuf, Uint32 dataLen);

/**************************************************************************/
/*! \fn int hwMbox_sendAckCommand(hw_mbox_Masters_e commander)
 **************************************************************************
 *  \brief Send back an ACK to <commander>, from which a command has been
 *         received using the HW_MBOX.
 *  \note should be called when no meaningful response is expected in order
 *        to simply ACK the command.
 *  \note Using this function leaves the command register’s value unchanged
 *        (writes the same value that was received).
 *  \attention should only be used after a command has been received from
 *             <commander> using hwMbox_recvCommand.
 *  \param[in] commander - To which Master should the reply be sent.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendAckCommand(hw_mbox_Masters_e commander);

/**************************************************************************/
/*! \fn int hwMbox_sendOpcode(hw_mbox_Masters_e subject,
                              Uint32 tag,
                              Uint8 *dataBuf, Uint32 *dataLen,
                              Uint32 opcodeDataLen,
                              Uint32 *replyDataLen)
 **************************************************************************
 *  \brief Send an opcode to <subject> in a blocking manner using the HW_MBOX.
 *  \attention will only return upon reply from <subject> (or error).
 *  \attention specified buffer should be able to hold generic messages that
 *             sent on opcode channels, meaning size should be at least
 *             HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES.
 *  \param[in]     subject - To which Master should the opcode be sent.
 *  \param[in]     tag - indicates opcode destination.
 *  \param[in,out] dataBuf - pointer to a Buffer containing opcode data to be sent,
 *                           upon returning will hold the reply data Buffer.
 *  \param[in]     dataLen - Length in Bytes of buffer specified in <dataBuf>.
 *  \param[in]     opcodeDataLen - Length in Bytes of opcode data to send.
 *  \param[out]    replyDataLen - upon returning will hold the length in Bytes
 *                                of returned reply data.
 *  \note if function returns -1, and replyDataLen outputs > 0 value then
 *        the user supplied buffer is populated with a <hwMbox_opcodeMessage_t>
 *        of type HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS containing extended error
 *        information.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendOpcode(hw_mbox_Masters_e subject,
                      Uint32 tag,
                      Uint8 *dataBuf, Uint32 dataLen,
                      Uint32 opcodeDataLen,
                      Uint32 *replyDataLen);

/**************************************************************************/
/*! \fn int hwMbox_sendReplyOpcode(hw_mbox_Masters_e commander,
                                   Uint8 *dataBuf, Uint32 *dataLen)
 **************************************************************************
 *  \brief Send a reply to an opcode received from <commander> using the HW_MBOX.
 *  \attention should only be used after an opcode has been received from <commander>.
 *  \param[in] commander - To which Master should the reply be sent.
 *  \param[in] dataBuf - pointer to a Buffer containing data to be sent.
 *  \param[in] dataLen - Length in Bytes of data to be sent from buffer
 *                       specified in <dataBuf>.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendReplyOpcode(hw_mbox_Masters_e commander,
                           Uint8 *dataBuf, Uint32 dataLen);

/**************************************************************************/
/*! \fn int hwMbox_sendAckOpcode(hw_mbox_Masters_e commander)
 **************************************************************************
 *  \brief Send back an ACK to <commander>, from which an opcode has been
 *         received using the HW_MBOX.
 *  \attention should only be used after an opcode has been received from
 *             <commander> using HW_MBOX_recvCommand.
 *  \param[in] commander - To which Master should the ACK be sent.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_sendAckOpcode(hw_mbox_Masters_e commander);

/**************************************************************************/
/*! \fn int hwMbox_registerRecvOpcode(hw_mbox_Masters_e commander,
                                      hwMboxRecvOpcodeCallback pOpcodeCallbackFunc,
                                      Uint32 tag,
                                      Uint32 token)
 **************************************************************************
 *  \brief Register for Receive Opcode from <commander> using the HW_MBOX.
 *  \attention This function should be called once per tag.
 *  \param[in] commander - From which Master should the opcode be received.
 *  \param[in] pOpcodeCallbackFunc - pointer to the callback function to be
 *                                   invoked upon receiving an opcode from
 *                                   <commander>.
 *  \param[in] tag - indicates the destination.
 *  \param[in] token - 32 bit value saved in DB that can be used by application.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_registerRecvOpcode(hw_mbox_Masters_e commander,
                              hwMboxRecvOpcodeCallback pOpcodeCallbackFunc,
                              Uint32 tag,
                              Uint32 token);

/**************************************************************************/
/*! \fn int hwMbox_unregisterRecvOpcode(hw_mbox_Masters_e commander, Uint32 tag)
 **************************************************************************
 *  \brief Unregister Receive Opcode from <commander> using the HW_MBOX.
 *  \attention This function should be called once per tag.
 *  \param[in] commander - From which Master should the opcode be received.
 *  \param[in] tag - indicates the destination.
 *  \return [ 0 on success / error otherwise ].
 **************************************************************************/
int hwMbox_unregisterRecvOpcode(hw_mbox_Masters_e commander, Uint32 tag);
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

#include <linux/ioctl.h>

#define HW_MBOX_MODULE_ID                   (0x8C)

/* IOCTL CMDs:
   Note: All of them return a HW_MBOX_STATUS_e value.
*/
#define HW_MBOX_CHN_DEV_IOCTL_LOCK_CMD      _IOR(HW_MBOX_MODULE_ID, 1, int)
#define HW_MBOX_CHN_DEV_IOCTL_UNLOCK_CMD    _IOR(HW_MBOX_MODULE_ID, 2, int)

#endif /* ! _HW_MBOX_DRV_H_ */
