/*****************************************************************************
* Copyright 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#ifndef _VC_AUDIO_DEFS_H_
#define _VC_AUDIO_DEFS_H_

#include "interface/vchi/vchi.h"

// FourCC code used for VCHI connection
#define VC_AUDIO_SERVER_NAME  MAKE_FOURCC("AUDS")

#define VC_AUDIO_UPLINK_SERVICE_FOURCC     VCHIQ_MAKE_FOURCC('A','U','D','U')
#define VC_AUDIO_UPLINK_SERVICE_VERSION    1

// Maximum message length
#define VC_AUDIO_MAX_MSG_LEN  (sizeof( VC_AUDIO_MSG_T ))

#define MAX_AUDIO_STREAMS 8

// List of screens that are currently supported
// All message types supported for HOST->VC direction
typedef enum
{
  VC_AUDIO_MSG_TYPE_RESULT,      // Generic result
  VC_AUDIO_MSG_TYPE_COMPLETE,    // Generic result
  VC_AUDIO_MSG_TYPE_CONFIG,      // Configure audio
  VC_AUDIO_MSG_TYPE_CONTROL,     // Configure audio
  VC_AUDIO_MSG_TYPE_OPEN,        // Configure audio
  VC_AUDIO_MSG_TYPE_CLOSE,       // Configure audio
  VC_AUDIO_MSG_TYPE_START,       // Configure audio
  VC_AUDIO_MSG_TYPE_STOP,        // Configure audio
  VC_AUDIO_MSG_TYPE_WRITE,       // Configure audio
  VC_AUDIO_MSG_TYPE_READ,        // Configure audio
  VC_AUDIO_MSG_TYPE_LOOPAUDIO_ALLOC,
  VC_AUDIO_MSG_TYPE_MAX
} VC_AUDIO_MSG_TYPE;

// configure the audio
typedef struct
{
  uint32_t channels;
  uint32_t samplerate;
  uint32_t bps;
  uint32_t direction;   // VC_AUDIO_DIRECTION_T
} VC_AUDIO_CONFIG_T;

typedef struct
{
  uint32_t volume;
  uint32_t dest;
} VC_AUDIO_CONTROL_T;

// audio
typedef enum
{
   DIRECTION_DOWNLINK   = 2,
   DIRECTION_UPLINK     = 4
} VC_AUDIO_DIRECTION_T;

typedef struct
{
   uint32_t dummy;
} VC_AUDIO_OPEN_T;

// audio
typedef struct
{
  uint32_t dummy;
} VC_AUDIO_CLOSE_T;

// audio
typedef struct
{
  uint32_t dummy;  
} VC_AUDIO_START_T;

// audio
typedef struct
{
  uint32_t draining;
} VC_AUDIO_STOP_T;

// configure the write audio samples
typedef struct
{
  uint32_t count;      // in bytes
  uint32_t silence;
} VC_AUDIO_WRITE_T;

typedef struct
{
  uint32_t count;      // in bytes
} VC_AUDIO_READ_T;

typedef struct
{
  uint32_t res_handle;
  void    *res_mem;
} VC_LOOPAUDIO_ALLOC_RESULT_T;

#define VC_LOOPAUDIO_BUFFER_SIZE (64*1024)
typedef struct 
{
    uint32_t sidx4;
    uint32_t written;
    uint32_t lflags;   // flags written by linux
    uint32_t vflags;   // flags written by videocore
    char buffer[VC_LOOPAUDIO_BUFFER_SIZE];
} VC_LOOPAUDIO_MAILBOX_T;

// lflags
#define VC_LOOPAUDIO_LOOP_MUTE     (1<<0)
#define VC_LOOPAUDIO_HDMI_MUTE     (1<<1)
#define VC_LOOPAUDIO_LOOP_48KHZ    (1<<2)

// Generic result for a request (VC->HOST)
typedef struct
{
  int32_t success;   // Success value
} VC_AUDIO_RESULT_T;

// Generic result for a request (VC->HOST)
typedef struct
{
  int32_t count;      // Success value
} VC_AUDIO_COMPLETE_T;

// Message header for all messages in HOST->VC direction
typedef struct
{
  int32_t type;      // Message type (VC_AUDIO_MSG_TYPE)
  union
  {
   VC_AUDIO_CONFIG_T config;
   VC_AUDIO_CONTROL_T control;
   VC_AUDIO_OPEN_T open;
   VC_AUDIO_CLOSE_T close;
   VC_AUDIO_START_T start;
   VC_AUDIO_STOP_T stop;
   VC_AUDIO_WRITE_T write;
   VC_AUDIO_READ_T read;
   VC_AUDIO_RESULT_T result;
   VC_AUDIO_COMPLETE_T complete;
   VC_LOOPAUDIO_ALLOC_RESULT_T loopaudio;
  } u;
} VC_AUDIO_MSG_T;

#endif // _VC_AUDIO_DEFS_H_
