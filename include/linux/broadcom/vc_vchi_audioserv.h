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

#ifndef _VC_AUDIO__H_
#define _VC_AUDIO_H_

#include <vc_vchi_audioserv_defs.h>

void vc_vchiq_alsa_init(void);
void vc_vchiq_alsa_deinit(void);
int32_t vc_vchiq_alsa_open(vc_alsa_stream_t *alsa_stream);
int32_t vc_vchiq_alsa_close(vc_alsa_stream_t *alsa_stream);
int32_t vc_vchiq_alsa_set_params(vc_alsa_stream_t *alsa_stream, uint32_t channels, uint32_t samplerate, uint32_t bps);
int32_t vc_vchiq_alsa_set_ctls_chan(vc_alsa_stream_t *alsa_stream, vc_alsa_chip_t *chip);
int32_t vc_vchiq_alsa_setup(vc_alsa_stream_t *alsa_stream);
int32_t vc_vchiq_alsa_start(vc_alsa_stream_t *alsa_stream);
int32_t vc_vchiq_alsa_stop(vc_alsa_stream_t *alsa_stream);
int32_t vc_vchiq_alsa_set_ctls(vc_alsa_chip_t *chip);
int32_t vc_vchiq_alsa_write(vc_alsa_stream_t *alsa_stream, uint32_t count, void *src);
uint32_t vc_vchiq_alsa_retrieve_buffers(vc_alsa_stream_t *alsa_stream);
void vc_vchiq_alsa_flush_buffers(vc_alsa_stream_t *alsa_stream);
void vc_vchiq_alsa_flush_playback_buffers(vc_alsa_stream_t *alsa_stream);

#endif