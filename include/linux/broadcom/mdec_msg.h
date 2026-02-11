/*
 * Copyright (c) 2012 Broadcom Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  mdec_msg.h
 *
 *  Definitions of messages between Linux driver and VideoCore service.
 */
#ifndef	_MDEC_MSG_H_
#define	_MDEC_MSG_H_

/** The service name in FOURCC format **/
#define MDEC_FOURCC     VCHIQ_MAKE_FOURCC('M', 'D', 'E', 'C')

/** Use the fourcc encoding to enumerate the commands.  The argument
    is a string of at least four characters, but only the first four
    characters are significant.
**/
#define MDEC_CMD(s)     VCHIQ_MAKE_FOURCC((s)[0],(s)[1],(s)[2],(s)[3])

/** Command codes between the client and server **/
/* Initialisation */
#define MDEC_CMD_FILE                     VCHIQ_MAKE_FOURCC('F','I','L','E')
#define MDEC_CMD_SETUP                    VCHIQ_MAKE_FOURCC('S','E','U','P')

/* Transport controls */
#define MDEC_CMD_PLAY                     VCHIQ_MAKE_FOURCC('P','L','A','Y')
#define MDEC_CMD_STOP                     VCHIQ_MAKE_FOURCC('S','T','O','P')
#define MDEC_CMD_PAUSE                    VCHIQ_MAKE_FOURCC('P','A','U','S')
#define MDEC_CMD_STEP                     VCHIQ_MAKE_FOURCC('S','T','E','P')
#define MDEC_CMD_CLEAR                    VCHIQ_MAKE_FOURCC('C','L','E','R')

/* Settings */
#define MDEC_CMD_SET_CLOCK                VCHIQ_MAKE_FOURCC('S','E','T','C')
#define MDEC_CMD_SET_VOLUME               VCHIQ_MAKE_FOURCC('S','E','T','V')
#define MDEC_CMD_SET_SOURCE_REGION        VCHIQ_MAKE_FOURCC('S','E','T','S')
#define MDEC_CMD_SET_DEST_REGION          VCHIQ_MAKE_FOURCC('S','E','T','D')
#define MDEC_CMD_SET_MUTED                VCHIQ_MAKE_FOURCC('M','U','T','E')
#define MDEC_CMD_SET_TRANSPARENCY         VCHIQ_MAKE_FOURCC('S','E','T','T')
#define MDEC_CMD_SET_FADE                 VCHIQ_MAKE_FOURCC('S','E','F','A')
#define MDEC_CMD_SET_SYNC_OFFSET          VCHIQ_MAKE_FOURCC('S','E','T','O')

/* Query */
#define MDEC_CMD_GET_PROPERTY             VCHIQ_MAKE_FOURCC('G','E','T','P')
#define MDEC_CMD_GET_FRAME                VCHIQ_MAKE_FOURCC('G','E','T','F')
#define MDEC_CMD_GET_STATE                VCHIQ_MAKE_FOURCC('G','E','T','S')
#define MDEC_CMD_GET_FADE                 VCHIQ_MAKE_FOURCC('G','E','F','A')

/* Bulk media data */
#define MDEC_CMD_SEND_AUDIO_DATA          VCHIQ_MAKE_FOURCC('T','X','A','D')
#define MDEC_CMD_SEND_VIDEO_DATA          VCHIQ_MAKE_FOURCC('T','X','V','D')

/** Media Dec server response status **/
#define MDEC_RESPONSE_ACK                 VCHIQ_MAKE_FOURCC('R','_','A','K')
#define MDEC_RESPONSE_NAK                 VCHIQ_MAKE_FOURCC('R','_','N','K')

/** Media Dec stream states **/
#define MDEC_STATE_IDLE                   VCHIQ_MAKE_FOURCC('S','_','I','D')
#define MDEC_STATE_ERROR                  VCHIQ_MAKE_FOURCC('S','_','E','R')
#define MDEC_STATE_ENABLE                 VCHIQ_MAKE_FOURCC('S','_','E','N')
#define MDEC_STATE_PLAY                   VCHIQ_MAKE_FOURCC('S','_','P','L')

typedef enum {
    MDEC_SERVER_MSG_RESPONSE,
    MDEC_SERVER_MSG_QUERY,
    MDEC_SERVER_MSG_CALLBACK
} mdec_server_msg_t;

typedef struct {
    uint32_t type; /* from mdec_server_msg_t */
    uint32_t response;
    union {
       MDEC_WAIT_FOR_CALLBACK_T callback;   
       MDEC_GET_PROPERTY_T      property;
       uint32_t                 uv; 
       uint8_t                  buf[24];   /* Simple buffer for generic copy */       
    } u;
} MDEC_SERVER_CTRL_MESSAGE_T;

/******************************************************************************/
#endif /* _MDEC_MSG_H_ */
/******************************************************************************/
