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

#ifndef VC_VCHI_FB_H
#define VC_VCHI_FB_H

void vc_vchiq_fb_init(void);
void vc_vchiq_fb_deinit(void);
int32_t vc_vchiq_fb_open(SCRN_INFO_T* scrn_info);
int32_t vc_vchiq_fb_close(SCRN_INFO_T* scrn_info);
int32_t vc_vchiq_fb_get_scrn_info(SCRN_INFO_T* scrn_info, VC_FB_SCRN_INFO_T *info);
int32_t vc_vchiq_fb_alloc(SCRN_INFO_T *scrn_info, VC_FB_ALLOC_T* alloc, VC_FB_ALLOC_RESULT_T *alloc_result);
int32_t vc_vchiq_fb_free(SCRN_INFO_T* scrn_info);
int32_t vc_vchiq_fb_pan(SCRN_INFO_T* scrn_info, VC_FB_PAN_T* pan);
//int32_t vc_vchi_fb_swap(uint32_t res_handle, uint32_t active_frame, int32_t connection);
int32_t vc_vchiq_fb_cfg(SCRN_INFO_T *scrn_info, VC_FB_CFG_T *cfg,VC_FB_CFG_RESULT_T *cfg_result);
#endif // VC_VCHI_FB_H
