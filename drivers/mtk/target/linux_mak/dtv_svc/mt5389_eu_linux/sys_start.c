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
 * $RCSfile: sys_start.c,v $
 * $Revision: #13 $
 * $Date: 2012/12/07 $
 * $Author: pan.huang $
 *
 * Description:
 *         This file contains system start, custom & application
 *         initialization.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef SYS_FAST_BOOT
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#endif
#include <unistd.h>
#include "x_evctx.h"
#include "x_evctx_dvb_eng.h"
#include "x_evctx_analog_eng.h"
#include "x_rrctx.h"
#include "x_rrctx_vchip_eng.h"
#ifdef SYS_EDB_SUPPORT
#include "x_edb.h"
#include "x_edb_dvb_eng.h"
#endif
#include "sys_start.h"
#include "sys_custom.h"
#include "x_dvb_si_eng.h"
#include "x_appman.h"
#include "x_brdcst.h"
#include "x_analog_brdcst.h"
#include "x_avc_handler.h"
#include "x_common.h"
#ifdef SYS_UK_FREEVIEWHD_SUPPORT
#include "x_cl_huffman_eng.h"
#endif
#include "x_dbg.h"
#include "x_dt.h"
#include "x_fe.h"
#include "x_fm.h"
#include "x_dm.h"
#include "x_os.h"
#include "x_handle.h"
#include "x_plane_mxr.h"
#include "x_psi_eng.h"
#include "x_rm_dev_types.h"
#include "x_rs_232.h"
#include "x_sb.h"
#include "x_sb_dvb_eng.h"
#include "x_sb_dvbc_eng.h"
#include "x_sb_pal_secam_eng.h"
#include "x_vbi_fltr.h"
#include "x_scc.h"
#include "x_sm.h"
#include "x_start.h"
#include "x_svl.h"
#include "x_tsl.h"
#include "x_fm_fat.h"
#ifdef FM_SUPPORT_NTFS    /* NTFS support */
#include "x_fm_ntfs.h"
#endif
#include "x_tv_enc.h"
#include "x_vid_plane.h"
#include "x_wgl_bdr_null.h"
#include "x_wgl_bdr_transp.h"
#include "x_wgl_bdr_uniform.h"
#include "x_wgl_bdr_depressed.h"
#include "x_wgl_bdr_raised.h"
#include "x_wgl_bdr_drop_shadow_left.h"
#include "x_wgl_bdr_drop_shadow_right.h"
#include "x_wgl_bdr_timg.h"
#include "x_wgl_button.h"
#include "x_wgl_edit.h"
#include "x_wgl_frame.h"
#include "x_wgl_icon.h"
#include "x_wgl_lb.h"
#include "x_wgl_progress.h"
#include "x_wgl_sets.h"
#include "x_wgl_text.h"
#include "x_wgl_hts.h"
#include "x_wgl_sbtl.h"
#include "x_wgl_ttx.h"
#include "x_wgl_ttx_sys_init.h"

#include "x_vbif_hdlr_ttx.h"
#include "x_vbif_hdlr_eum.h"
#include "x_vbif_vc_vbi.h"
#include "x_vbif_vc_pes.h"

#include "u_svctx.h"
#include "x_ch_hdlr.h"
#include "x_inp_hdlr.h"
#include "x_dlm.h"

/* custom header files */
#include "am/a_am.h"
#include "amb/a_amb.h"
#include "app_util/a_common.h"
#include "app_util/a_cfg.h"
#include "fonts/fontdata.h"
#include "scan_maps/scan_maps.h"
#include "scan_maps/scan_maps_pal_secam.h"
#include "scan_maps/dvbc_scan_maps.h"
#include "x_sm_sbtl_hdlr.h"
#include "x_sm_ttx_hdlr.h"
#include "x_sect_eng.h"

#ifdef SYS_MHEG5_SUPPORT
#include "x_mheg5.h"
#include "x_mheg5_OceanBlue.h"
#endif /* SYS_MHEG5_SUPPORT */

#ifdef DVBT_CI_ENABLE
#include "x_ci.h"
#ifdef MW_CI_OCEANBLUE_SUPPORT
#include "x_ci_oceanblue_gl.h"
#else
#include "x_ci_wipro_gl.h"
#endif
#endif

#ifdef SYS_OAD_SUPPORT
#include "x_dsmcc_eng.h"
#include "x_dsmcc_oc.h"
#include "x_ssum.h"
#endif /* SYS_OAD_SUPPORT */

#include "res/fonts/a_font_custom.h"

#include "x_playback_handler.h"
#include "u_gl.h"
#include "u_gl_nc.h"

#ifdef ENABLE_MULTIMEDIA
#include "x_mm_hdlr.h"
#ifdef MW_MMP_VIDEO_SUPPORT
#include "x_mm_hdlr_uop_avi.h"
#include "x_mm_hdlr_uop_ps.h"
#include "x_mm_hdlr_uop_ts.h"
#include "x_mm_hdlr_uop_asf.h"
#include "x_mm_hdlr_uop_mp4.h"
#include "x_mm_hdlr_uop_mkv.h"
#include "x_mm_hdlr_uop_es.h"
#include "x_mm_hdlr_uop_flv.h"
#include "x_mm_hdlr_uop_ogg.h"
#endif

#ifdef MW_MP3_SUPPORT
#include "x_mm_hdlr_uop_mp3.h"
#include "x_mfmtrecg_mp3.h"
#include "x_minfo_mp3.h"
#include "x_midxbuld_mp3.h"
#endif

#include "x_mfmtrecg.h"
#include "x_minfo.h"
#include "x_midxbuld.h"

#ifdef MW_MMP_VIDEO_SUPPORT
#include "x_mfmtrecg_avi.h"
#include "x_minfo_avi.h"
#include "x_midxbuld_avi.h"

#include "x_mfmtrecg_ts.h"
/*#include "x_mfmtrecg_es.h"*/
#include "x_mfmtrecg_ps.h"
#include "x_minfo_ps.h"

#include "x_mfmtrecg_asf.h"
#include "x_minfo_asf.h"
#include "x_midxbuld_asf.h"

#include "x_mfmtrecg_mp4.h"
#include "x_minfo_mp4.h"
#include "x_midxbuld_mp4.h"

#include "x_mfmtrecg_mkv.h"
#include "x_minfo_mkv.h"
#include "x_midxbuld_mkv.h"

#include "x_mfmtrecg_aud_es.h"
#include "x_mfmtrecg_vid_es.h"
#include "x_minfo_aud_es.h"

#include "x_mfmtrecg_flv.h"
#include "x_minfo_flv.h"
#include "x_midxbuld_flv.h"

#include "x_mfmtrecg_ogg.h"
#include "x_minfo_ogg.h"

#endif

#include "x_wgl_sbtl.h"
#include "x_sm_mm_sbtl_handler.h"
#include "x_sbtl_divx.h"
#include "x_sbtl_text.h"
#include "x_sbtl_vobsub.h"
#include "x_sbtl_mkv.h"
#include "x_sbtl_mp4.h"
#include "x_divx_drm.h"
#endif
#include "x_sbtl_lyric.h"
#include "u_img.h"
#include "x_img_jpg_eng.h"
#include "x_img_gif_eng.h"
#include "x_img_png_eng.h"
#include "x_img_bmp_eng.h"
#include "x_img_webp_eng.h"
#include "x_img_tfx.h"

#ifdef SYS_MHEG5_SUPPORT
#include "nav/mheg5_comp/a_mheg5.h"
#endif /* SYS_MHEG5_SUPPORT */

#ifdef INET_SUPPORT
#include "x_net_config.h"
#endif

#ifdef SYS_RECORD_SUPPORT
#include "x_rec_hdlr.h"
#include "x_sm_sess_rcrd.h"
#include "x_sm_rcrd_hdlr.h"
#include "x_rec_util.h"
#endif

#ifdef SYS_HBBTV_SUPPORT
#include "x_hbbtv.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef SYS_FAST_BOOT
#define SH_EXE "sh"

#define ETH_MOD_SH          "/etc/scripts/ins_eth_mod.sh"
#define MOUNT_3RD_SH    "/etc/scripts/mount_3rd.sh"
#define MOUNT_PERM_SH       "/etc/scripts/mount_perm.sh"
#define USB_CORE_MOD_SH     "/etc/scripts/ins_usb_core_mod.sh"

#ifdef INET_SUPPORT
#ifdef SYS_NETWORK_LAZY_INIT
#define WIFI_MOD_SH "/etc/scripts/ins_wifi_mod.sh"
#endif
#endif

#define KERNEL_MOD_SH "/etc/scripts/ins_all_mod.sh"

#endif


#ifdef ABORT_CUSTOM
#undef ABORT_CUSTOM
#endif

#ifdef ABORT_APPL
#undef ABORT_APPL
#endif

#define ABORT_CUSTOM(_cat, _val) dbg_abort ((DBG_MOD_CUSTOM_INIT | _cat | _val))
#define ABORT_APPL(_cat, _val)   dbg_abort ((DBG_MOD_APPL_INIT   | _cat | _val))

/* Some debug abort values. */
#define DBG_ABRT_PSI_ENG_INIT                     ((UINT32) 0x00000000)
#define DBG_ABRT_DVB_SI_ENG_INIT                  ((UINT32) 0x00000001)
#define DBG_ABRT_BRDCST_ENG_INIT                  ((UINT32) 0x00000002)
#define DBG_ABRT_FONTS_INIT                       ((UINT32) 0x00000003)
#define DBG_ABRT_WIDGET_INIT                      ((UINT32) 0x00000004)
#define DBG_ABRT_MAIN_DISP_INIT                   ((UINT32) 0x00000005)
#define DBG_ABRT_RAW_DEV_INIT                     ((UINT32) 0x00000006)
#define DBG_ABRT_CLOSED_CAPTION_INIT              ((UINT32) 0x00000007)
#define DBG_ABRT_SVCTX_HDLRS_INIT                 ((UINT32) 0x00000008)
#define DBG_ABRT_RRCTX_ENG_INIT                   ((UINT32) 0x00000009)
#define DBG_ABRT_VBI_PSR_INIT                     ((UINT32) 0x0000000A)
#define DBG_ABRT_VBI_FLTR_INIT                    ((UINT32) 0x0000000B)
#define DBG_ABRT_SUBTITLE_INIT                    ((UINT32) 0x0000000C)  
#define DBG_ABRT_TELETEXT_INIT                    ((UINT32) 0x0000000D)  
#define DBG_ABRT_DT_BRDCST_INIT                   ((UINT32) 0x0000000E)

#ifdef SYS_MHEG5_SUPPORT
#define DBG_ABRT_MHEG5_ENG_INIT                   ((UINT32) 0x0000000F)  
#endif /* SYS_MHEG5_SUPPORT */

#define DBG_ABRT_SECT_ENG_INIT                    ((UINT32) 0x00000010)  
#define DBG_ABRT_CI_INIT                          ((UINT32) 0x00000011)  

#ifdef SYS_OAD_SUPPORT
#define DBG_ABRT_DSMCC_INIT                       ((UINT32) 0x00000012)
#define DBG_ABRT_SSU_INIT                         ((UINT32) 0x00000013)  
#endif /* SYS_OAD_SUPPORT */

#define DBG_ABRT_CEC_MNGR_INIT                    ((UINT32) 0x00000014)
#define DBG_ABRT_DLM_ENG_INIT                     ((UINT32) 0x00000015)

#define DBG_ABRT_MFMTRECG_INIT         	          ((UINT32) 0x00000016)
#define DBG_ABRT_MSVCTX_INIT            	  ((UINT32) 0x00000017)
#define DBG_ABRT_MINFO_INIT              	  ((UINT32) 0x00000018)
#define DBG_ABRT_MIDXBULD_INIT          	  ((UINT32) 0x00000019)
#define DBG_ABRT_JPG_INIT                 	  ((UINT32) 0x0000001A)
#define DBG_ABRT_TFX_INIT                 	  ((UINT32) 0x0000001B)
#define DBG_ABRT_MM_INIT                          ((UINT32) 0x0000001C)
#define DBG_ABRT_BMP_INIT                         ((UINT32) 0x0000001D)
#define DBG_ABRT_PNG_INIT                         ((UINT32) 0x0000001E)
#define DBG_ABRT_RECORD_INIT                      ((UINT32) 0x0000001F)
#define DBG_ABRT_NETWORK_INIT                     ((UINT32) 0x00000020)
#define DBG_ABRT_GIF_INIT                         ((UINT32) 0x00000021)
#define DBG_ABRT_WEBP_INIT                        ((UINT32) 0x00000022)
#define DBG_ABRT_HBBTV_INIT                       ((UINT32) 0x00000023)

#define DBG_ABRT_CREATE_SEMA                      ((UINT32) 0x00000100)
#define DBG_ABRT_LOCK_SEMA                        ((UINT32) 0x00000101)
#define DBG_ABRT_UNLOCK_SEMA                      ((UINT32) 0x00000102)
#define DBG_ABRT_DELETE_SEMA                      ((UINT32) 0x00000103)
#define DBG_ABRT_CREATE_APP_MNGR                  ((UINT32) 0x00000104)
#define DBG_ABRT_APP_MNGR_START_FAILURE           ((UINT32) 0x00000105)
#define DBG_ABRT_CANNOT_ALLOC_APP_REG_CTRL_BLOCK  ((UINT32) 0x00000106)
#define DBG_ABRT_CREATE_NAV_APP                   ((UINT32) 0x00000107)
#define DBG_ABRT_CREATE_MENU_APP                  ((UINT32) 0x00000108)
#define DBG_ABRT_CREATE_EPG_APP                   ((UINT32) 0x00000109)
#define DBG_ABRT_CREATE_EAS_APP                   ((UINT32) 0x0000010A)
#define DBG_ABRT_START_EAS_APP                    ((UINT32) 0x0000010B)
#define DBG_ABRT_CREATE_POD_APP                   ((UINT32) 0x0000010C)
#define DBG_ABRT_START_POD_APP                    ((UINT32) 0x0000010D)
#define DBG_ABRT_CREATE_APP_FAILED                ((UINT32) 0x0000010E)

#ifdef SYS_UK_FREEVIEWHD_SUPPORT
#define DBG_ABRT_HUFFMAN_INIT                     ((UINT32) 0x00000111)
#endif

#define R_INIT_MM_OK                              ((UINT32) 0x0)
#define R_INIT_MFMTRECG_FAIL                      ((UINT32) 0x1)
#define R_INIT_MINFO_FAIL                         ((UINT32) 0x2)
#define R_INIT_MIDXBULD_FAIL                      ((UINT32) 0x3)
#define R_INIT_MSVCTX_FAIL                        ((UINT32) 0x4)
#define R_INIT_DRM_FAIL                           ((UINT32) 0x5)

/* DivX DRM Configuration */
#define DIVX_DRM_MEMORY_OFFSET                    ((UINT16)0)               /* Offset at MW partition. */
#define DIVX_DRM_MEMORY_SIZE_BYTE                 ((UINT16)56)              /* Maximum available size is 64. */
#define DIVX_DRM_MODEL_ID_LOW_BYTE                ((UINT8)(0x31))		    /* 0x31 is only simulation mode. */
#define DIVX_DRM_MODEL_ID_HIGH_BYTE               ((UINT8)(0x30 & 0xF0))    /* Only half of the byte counts. 0x30 is only for simulation. */

#ifdef DVB_ENABLE_HD_SUBTITLE
    #define SM_SBTL_TTX_SERVICE_PARTITION_SIZE        ((UINT32) 0x000C0000) /* 744 K Bytes */
#else
    #define SM_SBTL_TTX_SERVICE_PARTITION_SIZE        ((UINT32) 0x00030000) /* 196 K Bytes */
#endif /* DVB_ENABLE_HD_SUBTITLE */

#define SM_SBTL_TTX_SERVICE_NAME                  "sbtl_ttx_strm"

#define SYS_FILESYSTEM_MOUNT_ENTRY                "/mnt"

#ifdef SYS_FLASH_FONT_SUPPORT
    #define SYS_FLASH_FONT_DIR_NAME               "fonts"
#endif /* SYS_FLASH_FONT_SUPPORT */

#define CI_FILE_CREDENTIALS_CUST (SN_DEV_PATH "/" SN_MTD_BLK_17)

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

typedef struct _APP_MNGR_INIT_SEQ_T
{
    HANDLE_T  h_sema;  /* Semaphore handle for the application manager sequencing. */

    BOOL  b_ok;        /* Boolean state indicating application manager init successfull. */
}   APP_MNGR_INIT_SEQ_T;

static GEN_CONFIG_T t_g_drv_config = {0};

UINT8   gaui1_sbtl_ttx_strm_partition[SM_SBTL_TTX_SERVICE_PARTITION_SIZE];

#ifdef SYS_TTX_SUPPORT
#ifdef SYS_MHEG5_SUPPORT
static HANDLE_T     h_mheg5_mem_part = NULL_HANDLE;
#endif /* SYS_MHEG5_SUPPORT */
#endif

#ifdef DVBT_CI_ENABLE
/* CI CA config*/
#define   MAX_CA_SRC_DRV_TYPE     2
static DRV_TYPE_T  at_ca_src_type[MAX_CA_SRC_DRV_TYPE] =
{
    DRVT_TUNER_CAB_DIG,
    DRVT_TUNER_TER_DIG
};
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern VOID init_drv_custom( GEN_CONFIG_T* pt_drv_config ); 
extern VOID init_input_src_table( GEN_CONFIG_T* pt_drv_config ); 
extern INT32 it_sb_cli_init(VOID);
#ifdef SYS_MHEG5_SUPPORT
extern INT32 mheg5_init(THREAD_DESCR_T* pt_thread_desc);
#endif /* SYS_MHEG5_SUPPORT */

#ifdef INET_SUPPORT
#ifdef DLNA_SUPPORT
extern VOID x_net_dlna_reg_cli(VOID);
#endif
#ifdef MW_WIFI_SUPPORT
extern INT32 x_wifi_cli_init(VOID);
#endif
#endif
static BOOL init_psi_eng (VOID);
static BOOL init_brdcst_eng (VOID);


#if 0
static BOOL app_mngr_nfy_fct (HANDLE_T          h_app_mngr,
                              VOID*             pv_tag,
                              APP_NFY_REASON_T  e_nfy_reason);


#ifndef NO_FONTS_INIT
static BOOL init_fonts (VOID);
#endif
static BOOL init_vbi_fltr (VOID);
static BOOL init_widgets (VOID);
static BOOL init_main_disp (VOID);
static BOOL init_raw_dev (VOID);
static BOOL init_sect_eng(VOID);

#ifdef SYS_MHEG5_SUPPORT
static BOOL init_mheg5_eng (VOID);
#endif /* SYS_MHEG5_SUPPORT */

#ifdef SYS_OAD_SUPPORT
static BOOL init_dsmcc(VOID);
static BOOL init_ssum(VOID);
#endif /* SYS_OAD_SUPPORT */

static BOOL init_cec_mngr (VOID);
/*-----------------------------------------------------------------------------
 * Name: app_mngr_nfy_fct
 *
 * Description: This notify function is called by the application manager
 *              and signals either successfull or failed start. If the start
 *              is successfull, this routine will set the boolean 'b_ok' in
 *              structure APP_MNGR_INIT_SEQ_T to TRUE.
 *
 * Inputs:  h_app_mngr    Contains the application manager handle.
 *          pv_tag        References a structure of type APP_MNGR_INIT_SEQ_T.
 *          e_nfy-reason  Contains the notification reason.
 *
 * Outputs: -
 *
 * Returns: Always FALSE.
 ----------------------------------------------------------------------------*/
static BOOL app_mngr_nfy_fct (HANDLE_T          h_app_mngr,
                              VOID*             pv_tag,
                              APP_NFY_REASON_T  e_nfy_reason)
{
    if ((pv_tag != NULL)                           &&
        ((e_nfy_reason == APP_NFY_INIT_OK)       ||
         (e_nfy_reason == APP_NFY_INIT_FAILED)))
    {
        if (e_nfy_reason == APP_NFY_INIT_OK)
        {
            ((APP_MNGR_INIT_SEQ_T*) pv_tag)->b_ok = TRUE;
        }

        if (x_sema_unlock (((APP_MNGR_INIT_SEQ_T*) pv_tag)->h_sema) != OSR_OK)
        {
            ABORT_APPL (DBG_CAT_SEMAPHORE, DBG_ABRT_UNLOCK_SEMA);
        }
    }

    return (FALSE);
}

#endif

/*-----------------------------------------------------------------------------
 * Name: init_psi_eng
 *
 * Description: This API initializes the PSI engine.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_psi_eng (VOID)
{
    BOOL         b_success;
    PSI_CONFIG_T t_psi_config;


    b_success = FALSE;

    x_memset (((VOID*) &t_psi_config), 0, sizeof (PSI_CONFIG_T));

    /* TODO: TSDT settings may be changed if needed */
    t_psi_config.ui1_num_pat_filters = PSI_ENG_NUM_PAT_FILTERS;
    t_psi_config.ui1_num_pmt_filters = PSI_ENG_NUM_PMT_FILTERS;
    t_psi_config.ui1_num_cat_filters = PSI_ENG_NUM_CAT_FILTERS;
    t_psi_config.ui1_num_tsdt_filters = PSI_ENG_NUM_TSDT_FILTERS;

    t_psi_config.z_pat_buff_size = PSI_ENG_PAT_BUFF_SIZE;
    t_psi_config.z_pmt_buff_size = PSI_ENG_PMT_BUFF_SIZE;
    t_psi_config.z_cat_buff_size = PSI_ENG_CAT_BUFF_SIZE;
    t_psi_config.z_tsdt_buff_size = PSI_ENG_TSDT_BUFF_SIZE;

    t_psi_config.ui2_num_pat_tbl_objs     = PSI_ENG_NUM_PAT_TBL_OBJS;
    t_psi_config.ui2_num_pmt_tbl_objs     = PSI_ENG_NUM_PMT_TBL_OBJS;
    t_psi_config.ui2_num_cat_tbl_objs     = PSI_ENG_NUM_CAT_TBL_OBJS;
    t_psi_config.ui2_num_tsdt_tbl_objs    = PSI_ENG_NUM_TSDT_TBL_OBJS;
    t_psi_config.ui2_num_mon_pat_tbl_objs = PSI_ENG_NUM_MON_PAT_TBL_OBJS;

    if (x_psi_init (NULL, &(t_psi_config)) == TMR_OK)
    {
        b_success = TRUE;
    }

    return (b_success);
}


/*-----------------------------------------------------------------------------
 * Name: init_dvb_si_eng
 *
 * Description: This API initializes the DVB-SI engine.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
 #if 0
static BOOL init_dvb_si_eng (VOID)
{
    BOOL                        b_success;
    DVB_SI_CONFIG_T             t_dvb_si_config;
    SB_DVB_INIT_DATA_T          t_sb_dvb_config;
    SB_PAL_SECAM_INIT_DATA_T    t_sb_pal_secam_config;
    EVCTX_DVB_ENG_CONFIG_T      t_evctx_config;
    EVCTX_ANALOG_ENG_CONFIG_T   t_evctx_analog_config;
    SB_DVBC_INIT_DATA_T          t_sb_dvbc_config;
#ifdef MW_EDB_SUPPORT
    EDB_DVB_ENG_CONFIG_T        t_edb_dvb_config        = {0};
#endif


#ifdef MW_EDB_SUPPORT
    t_edb_dvb_config.t_eng_cfg_flag                 = EDB_DVB_CONFIG_PARSE_EXTEND_ITEMS|
                                                       EDB_DVB_CONFIG_USE_MAX_EVENT_NAME;
#endif
    b_success = FALSE;

    x_memset (((VOID*) &t_dvb_si_config), 0, sizeof (DVB_SI_CONFIG_T));

    t_dvb_si_config.ui1_num_bat_filters             = DVB_SI_ENG_NUM_BAT_FILTERS;
    t_dvb_si_config.ui1_num_nit_filters             = DVB_SI_ENG_NUM_NIT_FILTERS;
    t_dvb_si_config.ui1_num_sdt_filters             = DVB_SI_ENG_NUM_SDT_FILTERS;
    t_dvb_si_config.ui1_num_eit_filters             = DVB_SI_ENG_NUM_EIT_FILTERS;
    t_dvb_si_config.ui1_num_tdt_filters             = DVB_SI_ENG_NUM_TDT_FILTERS;
    t_dvb_si_config.ui1_num_tot_filters             = DVB_SI_ENG_NUM_TOT_FILTERS;
    t_dvb_si_config.ui1_num_rst_filters             = DVB_SI_ENG_NUM_RST_FILTERS;
#ifdef SSUM_SUPPORT_SSU_ENHANCED_PROFILE
    t_dvb_si_config.ui1_num_unt_filters             = DVB_SI_ENG_NUM_UNT_FILTERS;
#endif	/* #endif SSU_SUPPORT_ENHANCED_PROFILE */
    t_dvb_si_config.ui1_num_eit_sec_filters         = DVB_SI_ENG_NUM_EIT_SEC_FILTERS;
    t_dvb_si_config.ui1_num_ait_filters             = DVB_SI_ENG_NUM_AIT_FILTERS;

    t_dvb_si_config.z_bat_buff_size                 = DVB_SI_ENG_BAT_BUFF_SIZE;
    t_dvb_si_config.z_nit_buff_size                 = DVB_SI_ENG_NIT_BUFF_SIZE;
    t_dvb_si_config.z_sdt_buff_size                 = DVB_SI_ENG_SDT_BUFF_SIZE;
    t_dvb_si_config.z_eit_buff_size                 = DVB_SI_ENG_EIT_BUFF_SIZE;
    t_dvb_si_config.z_tdt_buff_size                 = DVB_SI_ENG_TDT_BUFF_SIZE;
    t_dvb_si_config.z_tot_buff_size                 = DVB_SI_ENG_TOT_BUFF_SIZE;
    t_dvb_si_config.z_rst_buff_size                 = DVB_SI_ENG_RST_BUFF_SIZE;
#ifdef SSUM_SUPPORT_SSU_ENHANCED_PROFILE
    t_dvb_si_config.z_unt_buff_size                 = DVB_SI_ENG_UNT_BUFF_SIZE;
#endif	/* #endif SSU_SUPPORT_ENHANCED_PROFILE */
    t_dvb_si_config.z_ait_buff_size                 = DVB_SI_ENG_AIT_BUFF_SIZE;

    t_dvb_si_config.ui2_num_bat_tbl_objs            = DVB_SI_ENG_NUM_BAT_TBL_OBJS;
    t_dvb_si_config.ui2_num_nit_tbl_objs            = DVB_SI_ENG_NUM_NIT_TBL_OBJS;
    t_dvb_si_config.ui2_num_sdt_tbl_objs            = DVB_SI_ENG_NUM_SDT_TBL_OBJS;
    t_dvb_si_config.ui2_num_eit_tbl_objs            = DVB_SI_ENG_NUM_EIT_TBL_OBJS;
    t_dvb_si_config.ui2_num_tdt_tbl_objs            = DVB_SI_ENG_NUM_TDT_TBL_OBJS;
    t_dvb_si_config.ui2_num_tot_tbl_objs            = DVB_SI_ENG_NUM_TOT_TBL_OBJS;
    t_dvb_si_config.ui2_num_rst_tbl_objs            = DVB_SI_ENG_NUM_RST_TBL_OBJS;
    t_dvb_si_config.ui2_num_txt_tbl_objs            = DVB_SI_ENG_NUM_TXT_TBL_OBJS;
#ifdef SSUM_SUPPORT_SSU_ENHANCED_PROFILE
    t_dvb_si_config.ui2_num_unt_tbl_objs            = DVB_SI_ENG_NUM_UNT_TBL_OBJS;
#endif	/* #endif SSU_SUPPORT_ENHANCED_PROFILE */
    t_dvb_si_config.ui2_num_ait_tbl_objs            = DVB_SI_ENG_NUM_AIT_TBL_OBJS;

    t_dvb_si_config.ui2_num_mon_bat_tbl_objs        = DVB_SI_ENG_NUM_MON_BAT_TBL_OBJS;
    t_dvb_si_config.ui2_num_mon_nit_tbl_objs        = DVB_SI_ENG_NUM_MON_NIT_TBL_OBJS;
    t_dvb_si_config.ui2_num_mon_sdt_tbl_objs        = DVB_SI_ENG_NUM_MON_SDT_TBL_OBJS;
#ifdef SSUM_SUPPORT_SSU_ENHANCED_PROFILE
    t_dvb_si_config.ui2_num_mon_unt_tbl_objs        = DVB_SI_ENG_NUM_MON_UNT_TBL_OBJS;
#endif	/* #endif SSU_SUPPORT_ENHANCED_PROFILE */
    t_dvb_si_config.ui2_num_mon_ait_tbl_objs        = DVB_SI_ENG_NUM_MON_AIT_TBL_OBJS;

    t_dvb_si_config.ui1_num_eit_tbl_objs_per_filter = DVB_SI_ENG_NUM_EIT_TBL_OBJS_PER_FILTER;
    t_dvb_si_config.ui4_eit_tbl_filter_timeout      = DVB_SI_ENG_EIT_TBL_FILTER_TIMEOUT;

    x_memset (&t_sb_dvb_config, 0, sizeof(SB_DVB_INIT_DATA_T));
    t_sb_dvb_config.t_eng_cfg_flag                  = SB_DVB_CONFIG_START_CH_NUM_FOR_NON_LCN_CH |
                                                      SB_DVB_CONFIG_APPLY_SMALLER_FOR_SINGLE_HD_SIMULCAST_LCN;
#ifdef SYS_MHEG5_SUPPORT
    t_sb_dvb_config.t_eng_cfg_flag                 |= SB_DVB_CONFIG_SUPPORT_MHEG5_SERVICES;
#endif /* SYS_MHEG5_SUPPORT */
    t_sb_dvb_config.t_eng_cfg_flag                 |= SB_DVB_CONFIG_NON_STANDARD_BITSTREAM;

    t_sb_dvb_config.t_eng_cfg_flag                 |= SB_DVB_CONFIG_FIN_ANTENNA_READY_HD_SUPPORT;

#ifdef SYS_UK_FREEVIEWHD_SUPPORT
    t_sb_dvb_config.t_eng_cfg_flag                 |= SB_DVB_CONFIG_DTG_HD_FREEVIEW_SUPPORT;
#endif
    
    t_sb_dvb_config.ui2_start_ch_num_for_non_lcn    = sys_cust_get_start_ch_num_for_non_lcn();                                                  
    t_sb_dvb_config.pf_scan_maps                    = scan_maps_fct;

    x_memset (&t_sb_pal_secam_config, 0, sizeof(SB_PAL_SECAM_INIT_DATA_T));
    t_sb_pal_secam_config.t_eng_cfg_flag            = SB_PAL_SECAM_CONFIG_START_CH_NUM;
    t_sb_pal_secam_config.ui2_start_ch_num          = 900;
    t_sb_pal_secam_config.pf_scan_maps              = scan_maps_pal_secam_fct;

    t_evctx_config.ui2_num_event_objs               = EVCTX_DVB_ENG_NUM_EVENT_OBJS;

    t_evctx_analog_config.ui2_num_event_objs = EVCTX_ANALOG_ENG_NUM_EVENT_OBJS;
    t_evctx_analog_config.ui4_timeout = EVCTX_ANALOG_TIMEOUT;

    x_memset (&t_sb_dvbc_config, 0, sizeof(SB_DVBC_INIT_DATA_T));
    t_sb_dvbc_config.t_eng_cfg_flag = SB_DVBC_CONFIG_START_CH_NUM_FOR_NON_LCN_CH |
                                                      SB_DVBC_CONFIG_SUPPORT_MHEG5_SERVICES;
    t_sb_dvbc_config.ui2_start_ch_num_for_non_lcn = sys_cust_get_start_ch_num_for_non_lcn();                                                  
    t_sb_dvbc_config.pf_scan_maps = dvbc_scan_maps_fct;


    if ((x_dvb_si_init (NULL, &(t_dvb_si_config))         == TMR_OK)    &&
        (x_sb_dvb_eng_init (&t_sb_dvb_config)             == SBR_OK)    &&
        (x_sb_dvbc_eng_init (&t_sb_dvbc_config)           == SBR_OK)    &&
        (x_sb_pal_secam_eng_init (&t_sb_pal_secam_config) == SBR_OK)    &&
        (x_evctx_dvb_eng_init(&t_evctx_config)            == EVCTXR_OK) &&
		(x_evctx_analog_eng_init(&t_evctx_analog_config)  == EVCTXR_OK)
#ifdef MW_EDB_SUPPORT
        && (x_edb_dvb_eng_init(&t_edb_dvb_config)            == EDBR_OK) 
#endif
        )
    {
        b_success = TRUE;
    }

    return (b_success);
}
 #endif
/*-----------------------------------------------------------------------------
 * Name: init_svctx_hdlrs
 *
 * Description: This API initializes the service context engine.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_svctx_hdlrs (VOID)
{


#ifdef ENABLE_MULTIMEDIA
    if (x_mm_hdlr_reg_handler() != SVCTXR_OK) 
    {
        return FALSE;
    }

#ifdef MW_MMP_VIDEO_SUPPORT
    /*intialuop_handler for multimedai playback*/ 
    if(x_mm_hdlr_uop_avi_init()!= SVCTXR_OK)
    {
        return FALSE;
    }

    /*intialuop_handler for multimedai playback*/ 
    if(x_mm_hdlr_uop_flv_init()!= SVCTXR_OK)
    {
        return FALSE;
    }

    if(x_mm_hdlr_uop_ps_init()!= SVCTXR_OK)
    {
        return FALSE;
    }

    if(x_mm_hdlr_uop_ts_init()!= SVCTXR_OK)
    {
        return FALSE;
    }
    
    /*intiate uop_handler for multimedai playback --asf */
    if(x_mm_hdlr_uop_asf_init() != SVCTXR_OK)
    {
        return FALSE;
    } 

    /*intiate uop_handler for multimedai playback --mp4*/ 
    if(x_mm_hdlr_uop_mp4_init() != SVCTXR_OK)
    {
        return FALSE;
    }
    /*intiate uop_handler for multimedai playback --mkv*/
    if(x_mm_hdlr_uop_mkv_init() != SVCTXR_OK)
    {
        return FALSE;
    }

    /*intiate uop_handler for multimedai playback --rm
    if(x_mm_hdlr_uop_rm_init() != SVCTXR_OK)
    {
        return FALSE;
    }*/

    /*intiate uop_handler for multimedai playback --es*/
    if(x_mm_hdlr_uop_es_init() != SVCTXR_OK)
    {
        return FALSE;
    }
    
    /*intiate uop_handler for multimedai playback --ogg*/
    if(x_mm_hdlr_uop_ogg_init() != SVCTXR_OK)
    {
        return FALSE;
    }
    
    /*intiate uop_handler for multimedai playback --avs
    if(x_mm_hdlr_uop_avs_init() != SVCTXR_OK)
    {
        return FALSE;
    }*/
#endif

#ifdef MW_MP3_SUPPORT
    /*intiate uop_handler for multimedai playback --mp3*/
    if(x_mm_hdlr_uop_mp3_init() != SVCTXR_OK)
    {
        return FALSE;
    }
#endif

    /*if (x_mm_hdlr_reg_handler() != SVCTXR_OK) 
    {
        return FALSE;
    }*/

#ifdef SYS_RECORD_SUPPORT
    if (x_rec_hdlr_reg_handler() != SVCTXR_OK) 
    {
        return FALSE;
    }
#endif
#endif
    return (TRUE);
}
/*-----------------------------------------------------------------------------
 * Name: init_rrctx_eng
 *
 * Description: This API initializes the rating region context engine.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_rrctx_eng (VOID)
{
    BOOL b_success = FALSE;

    if (x_rrctx_vchip_eng_init() == RRCTXR_OK)
    {
        b_success = TRUE;
    }

    return (b_success);
}
/*-----------------------------------------------------------------------------
 * Name: init_brdcst_eng
 *
 * Description: This API initializes the broadcast handler engine.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_brdcst_eng (VOID)
{
    THREAD_DESCR_T t_thread_descr;

    t_thread_descr.z_stack_size = 0;
    t_thread_descr.ui1_priority = 0;
    t_thread_descr.ui2_num_msgs = 0;

#ifdef MW_CM_PLAYBACK_SUPPORT
    if (x_playback_reg_handler (PLAYBACK_HANDLER_DEFAULT_NAME, &t_thread_descr) != CMR_OK)
    {
        return FALSE;
    }
#endif

    return (TRUE);
}

/*-----------------------------------------------------------------------------
 * Name: init_dt_brdcst
 *
 * Description: This API initializes the closed caption handler.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
#if 0
static BOOL init_dt_brdcst (VOID)
{
    BOOL     b_success = FALSE;

    if (( x_dt_reg_dvb() == DTR_OK ) &&
        ( x_dt_set_config( DT_USE_DST_AT_SPECIFIED_TIME |
                           DT_DVB_MATCH_CNTRY_AND_REG_CODE |
                           DT_DVB_MATCH_CNTRY_CODE |
                           DT_DVB_MATCH_ANY_CNTRY_OR_REG_CODE        
                           ) == DTR_OK ))

    {
        b_success = TRUE;
    }

    return b_success;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: init_fonts
 *
 * Description: This API initializes the fonts.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
 #if 0
#ifndef NO_FONTS_INIT
static BOOL init_fonts (VOID)
{
#ifndef SYS_FLASH_FONT_TEST
    return a_font_custom_init();
#else
    BOOL b_success = FALSE;

    UINT32 ui4_font_size_ttx20_bdf         = GET_UINT32_FROM_PTR_BIG_END (&(font_file_ttx20_bdf [0]));

    if ((x_fe_add_file_font(SN_FONT_DEFAULT,
            SYS_FILESYSTEM_MOUNT_ENTRY "/" SYS_FLASH_FONT_DIR_NAME "/" "courbd.ttf", 26, 30, 36) == FER_OK)

        && (x_fe_add_file_font(SN_FONT_DEFAULT_BIG,
            SYS_FILESYSTEM_MOUNT_ENTRY "/" SYS_FLASH_FONT_DIR_NAME "/" "courbd.ttf", 40, 50, 60) == FER_OK)

        && (x_fe_add_mem_font("fnt_ttx20_bdf",
            &(font_file_ttx20_bdf[4]), ui4_font_size_ttx20_bdf, 20, 20, 20) == FER_OK)
        )

    {
        x_dbg_stmt ("Flash FONT init OK\n");
        b_success = TRUE;
    }
    else
    {
        x_dbg_stmt ("Flash FONT init FAIL\n");
        b_success = FALSE;
    }

    return (b_success);
#endif /* SYS_FLASH_FONT_TEST */
}
#endif
/*-----------------------------------------------------------------------------
 * Name: init_vbi_fltr
 *
 * Description: This API initializes necessary components of the vbi filter.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_vbi_fltr (VOID)
{
    VBIF_HDLR_EUM_CFG_T     t_cfg_hdlr_eum;
    BOOL                    b_success;
    
    b_success = FALSE;
    
    t_cfg_hdlr_eum.ui4_flag = 0;
    
    if (x_vbif_vc_vbi_init   () == VBIFR_OK  &&
        x_vbif_vc_pes_init   () == VBIFR_OK  &&
        x_vbif_hdlr_ttx_init () == VBIFR_OK  && 
        x_vbif_hdlr_eum_init (&t_cfg_hdlr_eum) == VBIFR_OK)
    {
        b_success = TRUE;
    }
    
    return b_success;
}    
#endif

/*-----------------------------------------------------------------------------
 * Name: init_widgets
 *
 * Description: This API initializes the widgets.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_widgets (VOID)
{
    BOOL b_success;


    b_success = FALSE;

    if (wgl_init_default_sys_resource () == WGLR_OK)
    {
        if (
#if 0
            (x_wgl_button_init      (WGL_BTN_INIT_CTRL_LOGIC     | WGL_BTN_INIT_DEFAULT_CNT)     == WGLR_OK)  &&
            (x_wgl_eb_init          (WGL_EB_INIT_CTRL_LOGIC      | WGL_EB_INIT_DEFAULT_CNT)      == WGLR_OK)  &&
            (x_wgl_frame_init       (WGL_FM_INIT_CTRL_LOGIC      | WGL_FM_INIT_DEFAULT_CNT)      == WGLR_OK)  &&
            (x_wgl_icon_init        (WGL_ICON_INIT_CTRL_LOGIC    | WGL_ICON_INIT_DEFAULT_CNT)    == WGLR_OK)  &&
            (x_wgl_pg_init          (WGL_PG_INIT_CTRL_LOGIC      | WGL_PG_INIT_DEFAULT_CNT)      == WGLR_OK)  &&
            (x_wgl_text_init        (WGL_TEXT_INIT_CTRL_LOGIC    | WGL_TEXT_INIT_DEFAULT_CNT)    == WGLR_OK)  &&
            (x_wgl_listbox_init     (WGL_LISTBOX_INIT_CTRL_LOGIC | WGL_LISTBOX_INIT_DEFAULT_CNT) == WGLR_OK)  &&
            (x_wgl_hts_init         (WGL_HTS_INIT_CTRL_LOGIC     | WGL_HTS_INIT_DEFAULT_CNT)     == WGLR_OK)  &&
            (x_wgl_bdr_null_init    ()                                                           == WGLR_OK)  &&
            (x_wgl_bdr_transp_init  ()                                                           == WGLR_OK)  &&
            (x_wgl_bdr_uniform_init ()                                                           == WGLR_OK)  &&
            (x_wgl_bdr_depressed_init()                                                          == WGLR_OK)  &&
            (x_wgl_bdr_raised_init  ()                                                           == WGLR_OK)  &&
            (x_wgl_bdr_drop_shadow_left_init()                                                   == WGLR_OK)  &&
            (x_wgl_bdr_drop_shadow_right_init()                                                  == WGLR_OK)  &&
            (x_wgl_bdr_timg_init()                                                               == WGLR_OK)
#endif
            (x_wgl_frame_init       (WGL_FM_INIT_CTRL_LOGIC      | WGL_FM_INIT_DEFAULT_CNT)      == WGLR_OK)  &&
            (x_wgl_text_init        (WGL_TEXT_INIT_CTRL_LOGIC    | WGL_TEXT_INIT_DEFAULT_CNT)    == WGLR_OK)
            )
        {
            b_success = TRUE;
        }
    }

    return (b_success);
}

/*-----------------------------------------------------------------------------
 * Name: init_subtitle
 *
 * Description: This API initializes the subtitle stream handler and widget.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
#ifdef SYS_SUBTITLE_SUPPORT
static BOOL init_subtitle(HANDLE_T h_partition)
{
    WGL_SBTL_SYS_INIT_T     t_sys_init;
    SM_SBTLH_CONFIG_T       t_sbtlh_config;


    if (sys_cust_get_color_mode() == GL_COLORMODE_ARGB_CLUT8)
    {
        t_sys_init.ui4_flag                = WGL_SBTL_SYS_INIT_FLAG_PALETTE_MODE ;
        t_sys_init.e_clr_mode_frm_buf      = GL_COLORMODE_ARGB_CLUT8;
        t_sys_init.ui1_cnvs_plt_sbtl_start = sys_cust_get_ttx_sbtl_plt_range_start();
        t_sys_init.ui1_cnvs_plt_sbtl_end   = sys_cust_get_ttx_sbtl_plt_range_end();
    }
    else    /* GL_COLORMODE_ARGB_D4444 */
    {
        t_sys_init.ui4_flag           = 0;
        t_sys_init.e_clr_mode_frm_buf = GL_COLORMODE_ARGB_D8888;
    }
    
    if (WGLR_OK != x_wgl_sbtl_init(&t_sys_init))
    {
        return (FALSE);
    }
    
    t_sbtlh_config.h_partition = h_partition;
    t_sbtlh_config.t_thrd_descr.ui1_priority = 128;
    t_sbtlh_config.t_thrd_descr.ui2_num_msgs = 50;
    t_sbtlh_config.t_thrd_descr.z_stack_size = 8192;
    t_sbtlh_config.ui1_color_depth = 8;
    t_sbtlh_config.ui1_max_service_cnt = 1;
    t_sbtlh_config.ui4_page_width = 720;
    t_sbtlh_config.ui4_page_height = 576;
    t_sbtlh_config.ui1_wgl_order = WGL_APP_BOTTOMMOST;

    if (SMR_OK != x_sm_sbtl_hdlr_init(&t_sbtlh_config))
    {
        return (FALSE);
    }
    
    return (TRUE);
}
#endif /* SYS_SUBTITLE_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name: init_teletext
 *
 * Description: This API initializes the subtitle stream handler and widget.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
#ifdef SYS_TTX_SUPPORT
static WGL_TTX_DIMAP_ITEM_T         at_g_ttx_dimap_items[2];
static WGL_TTX_DIMAP_FONT_CANDT_T   at_g_ttx_font_candts_no_sp[1];
static WGL_TTX_DIMAP_FONT_CANDT_T   at_g_ttx_font_candts_sp[1];

#define DUMMY_ITEM                  { 0, 0, FALSE, FALSE }

static WGL_TTX_CGID_INFO_T          at_g_cgid_tbl_1_normal[4] = 
                                    {
                                        /* 0: the whole char */
                                        { 0, 0, FALSE, FALSE },
                                        /* 1: no this char_grid_id */
                                        DUMMY_ITEM,
                                        /* 2: no this char_grid_id */
                                        DUMMY_ITEM,
                                        /* 3: no this char_grid_id */
                                        DUMMY_ITEM
                                    };

static WGL_TTX_CGID_INFO_T          at_g_cgid_tbl_1_dbl_w[4] = 
                                    {
                                        /* 0: left part of char */
                                        { 0, 0, FALSE, FALSE},
                                        /* 1: no this char_grid_id */
                                        DUMMY_ITEM,
                                        /* 2: right */
                                        { 0, 0, TRUE,  FALSE},
                                        /* 3: no this char_grid_id */
                                        DUMMY_ITEM
                                    };

static WGL_TTX_CGID_INFO_T          at_g_cgid_tbl_1_dbl_h[4] = 
                                    {
                                        /* 0: top part of char */
                                        { 0, 0, FALSE, FALSE},
                                        /* 1: bottom part of char */
                                        { 0, 0, FALSE, TRUE},
                                        /* 2: no this char_grid_id */
                                        DUMMY_ITEM,
                                        /* 3: no this char_grid_id */
                                        DUMMY_ITEM
                                    };

static WGL_TTX_CGID_INFO_T          at_g_cgid_tbl_1_dbl_wh[4] = 
                                    {
                                        /* 0: left-top part of char */
                                        { 0, 0, FALSE, FALSE},
                                        /* 1: left-bottom part of char */
                                        { 0, 0, FALSE, TRUE},
                                        /* 2: right-top part of char */
                                        { 0, 0, TRUE,  FALSE},
                                        /* 3: right-bottom part of char */
                                        { 0, 0, TRUE,  TRUE}
                                    };

static WGL_TTX_CGID_TBL_T           t_g_tbl_cgid_info = 
                                    {
                                        4,
                                        {
                                            at_g_cgid_tbl_1_normal,
                                            at_g_cgid_tbl_1_dbl_w,
                                            at_g_cgid_tbl_1_dbl_h,
                                            at_g_cgid_tbl_1_dbl_wh
                                        }
                                    };

#define  VGRID_NUM                  28
#define  LG_HGRID_W                 ((UINT32) 720/56)
#define  LG_VGRID_H                 ((UINT32) 576/VGRID_NUM)

static BOOL ttx_fnt_get_private_code_proc (
    const UTF16_T                       w2_char_std,
    UTF16_T*                            pw2_char_prvt)
{
    BOOL    b_found;
    
    b_found = TRUE;
    switch (w2_char_std)
    {
        case 0x25A0:
            *pw2_char_prvt = 0xE65F;
        break;
        
        case 0x20A4:
            *pw2_char_prvt = 0xE800;
        break;
        
        default:
            *pw2_char_prvt = w2_char_std;
            b_found        = FALSE;
    }
    
    return b_found;
}

static BOOL init_teletext(HANDLE_T h_partition)
{
    WGL_TTX_SYS_INIT_T      t_init;
    SM_TTXH_CONFIG_T        t_ttxh_config;

    /* teletext widget configuration */
    at_g_ttx_font_candts_no_sp[0].ui1_flag              = 0;
    at_g_ttx_font_candts_no_sp[0].ps_font_name          = "fnt_ttx20_bdf";
    at_g_ttx_font_candts_no_sp[0].e_font_size           = FE_FNT_SIZE_MEDIUM;
    at_g_ttx_font_candts_no_sp[0].pf_get_private_code   = ttx_fnt_get_private_code_proc;
    at_g_ttx_font_candts_no_sp[0].i2_margin_upper       = 0;
    at_g_ttx_font_candts_no_sp[0].i2_margin_lower       = 0;
    at_g_ttx_font_candts_no_sp[0].ui1_num_hgrid_char    = 1;
    at_g_ttx_font_candts_no_sp[0].ui1_num_vgrid_char    = 1;
    at_g_ttx_font_candts_no_sp[0].pt_tbl_cgid_info      = & t_g_tbl_cgid_info;
    
    at_g_ttx_font_candts_sp[0].ui1_flag                 = 0;
    at_g_ttx_font_candts_sp[0].ps_font_name             = "fnt_ttx20_bdf";
    at_g_ttx_font_candts_sp[0].e_font_size              = FE_FNT_SIZE_MEDIUM;
    at_g_ttx_font_candts_sp[0].pf_get_private_code      = ttx_fnt_get_private_code_proc;
    at_g_ttx_font_candts_sp[0].i2_margin_upper          = 0;
    at_g_ttx_font_candts_sp[0].i2_margin_lower          = 0;
    at_g_ttx_font_candts_sp[0].ui1_num_hgrid_char       = 1;
    at_g_ttx_font_candts_sp[0].ui1_num_vgrid_char       = 1;
    at_g_ttx_font_candts_sp[0].pt_tbl_cgid_info         = & t_g_tbl_cgid_info;
    

    if (sys_cust_get_color_mode() == GL_COLORMODE_ARGB_CLUT8)
    {
        t_init.ui4_flag               = WGL_TTX_SYS_INIT_FLAG_PALETTE_MODE | 
                                        WGL_TTX_SYS_INIT_FLAG_PALETTE_SUPPORT_PARTIAL_MIX_MODE;
        t_init.ui1_cnvs_plt_ttx_start = sys_cust_get_ttx_sbtl_plt_range_start();
        t_init.ui1_cnvs_plt_ttx_end   = sys_cust_get_ttx_sbtl_plt_range_end();
    }
    else    /* GL_COLORMODE_ARGB_D4444 */
    {
        t_init.ui4_flag               = 0;
    }
                                   
    t_init.t_dimap.ui2_num_item   = 2;
    t_init.t_dimap.pat_items      = at_g_ttx_dimap_items;
    
    /* for no-side-pnl */
    t_init.t_dimap.pat_items[0].t_key.t_sz_vp_logical.ui4_width     = LG_HGRID_W * ((UINT32) WGL_TTX_HGRID_NUM_NO_SIDE_PNL);
    t_init.t_dimap.pat_items[0].t_key.t_sz_vp_logical.ui4_height    = LG_VGRID_H * VGRID_NUM;
    t_init.t_dimap.pat_items[0].t_key.e_num_hgrid                   = WGL_TTX_HGRID_NUM_NO_SIDE_PNL;
    t_init.t_dimap.pat_items[0].t_key.ui1_num_vgrid                 = VGRID_NUM;
    
    t_init.t_dimap.pat_items[0].t_info.t_sz_grid_logical.ui4_width  = LG_HGRID_W;
    t_init.t_dimap.pat_items[0].t_info.t_sz_grid_logical.ui4_height = LG_VGRID_H;
    t_init.t_dimap.pat_items[0].t_info.ui2_num_candts               = 1;
    t_init.t_dimap.pat_items[0].t_info.pat_candts                   = at_g_ttx_font_candts_no_sp;
    t_init.t_dimap.pat_items[0].t_info.ui2_num_bmp                  = 0;
    t_init.t_dimap.pat_items[0].t_info.pat_bmps                     = NULL;
    
    /* for with-side-pnl */
    t_init.t_dimap.pat_items[1].t_key.t_sz_vp_logical.ui4_width     = LG_HGRID_W * ((UINT32) WGL_TTX_HGRID_NUM_WITH_SIDE_PNL);
    t_init.t_dimap.pat_items[1].t_key.t_sz_vp_logical.ui4_height    = LG_VGRID_H * VGRID_NUM;
    t_init.t_dimap.pat_items[1].t_key.e_num_hgrid                   = WGL_TTX_HGRID_NUM_WITH_SIDE_PNL;
    t_init.t_dimap.pat_items[1].t_key.ui1_num_vgrid                 = VGRID_NUM;
    
    t_init.t_dimap.pat_items[1].t_info.t_sz_grid_logical.ui4_width  = LG_HGRID_W;
    t_init.t_dimap.pat_items[1].t_info.t_sz_grid_logical.ui4_height = LG_VGRID_H;
    t_init.t_dimap.pat_items[1].t_info.ui2_num_candts               = 1;
    t_init.t_dimap.pat_items[1].t_info.pat_candts                   = at_g_ttx_font_candts_sp;
    t_init.t_dimap.pat_items[1].t_info.ui2_num_bmp                  = 0;
    t_init.t_dimap.pat_items[1].t_info.pat_bmps                     = NULL;

    if (WGLR_OK != x_wgl_ttx_init (& t_init))
    {
        return (FALSE);
    }

    /* Teletext configuration */
    t_ttxh_config.h_partition = h_partition;

    t_ttxh_config.b_enable_2_5 = TRUE;
    t_ttxh_config.b_enable_top = TRUE;
    t_ttxh_config.b_parse_mip = TRUE;
    t_ttxh_config.b_rolling_header = TRUE;
    t_ttxh_config.t_default_char_set_designation.ui1_default_designation = 0;
    t_ttxh_config.t_default_char_set_designation.ui1_modified_designation = 0;
    t_ttxh_config.t_default_char_set_designation.ui1_second_designation = 0;
    
    t_ttxh_config.t_cache_policy.ui2_page_cache_cnt = 500;
    
    t_ttxh_config.ui1_ttx_service_max_cnt = 1;
    t_ttxh_config.ui4_max_part_size = 1024 * 1024;
    
    t_ttxh_config.t_thrd_descr.ui1_priority = 128;
    t_ttxh_config.t_thrd_descr.ui2_num_msgs = 128;
    t_ttxh_config.t_thrd_descr.z_stack_size = 8192;
    
    t_ttxh_config.ui1_wgl_order = WGL_APP_BOTTOMMOST;
    
    if (SMR_OK != x_sm_ttx_hdlr_init(&t_ttxh_config))
    {
        return (FALSE);
    }

#ifdef SYS_MHEG5_SUPPORT
    if (sys_cust_enable_ttx_share_buf())
    {
        x_sm_ttx_cache_init(h_mheg5_mem_part);
    }
#endif /* SYS_MHEG5_SUPPORT */

    return (TRUE);
}
#endif /* SYS_TTX_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name: init_main_disp
 *
 * Description: This API initializes the main display.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_main_disp (VOID)
{
    BOOL                        b_success;
    INT16                       i2_i = 0;
    SM_COMMAND_T                at_scc_cmds [32];
    SCC_BG_COLOR_T              t_disp_bg;
    SCC_BG_COLOR_T              t_video_bg;

    b_success = FALSE;

    /* display */
    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [i2_i++].u.ui4_data = SCC_CMD_TYPE_DISP;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_NAME;
    at_scc_cmds [i2_i++].u.ps_text  = SN_PRES_MAIN_DISPLAY;

    t_disp_bg.ui1_blue  = 0;
    t_disp_bg.ui1_green = 0;
    t_disp_bg.ui1_red   = 0;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_DISP_BACKGROUND;
    at_scc_cmds [i2_i++].u.pv_data  = (VOID*) &t_disp_bg;

    /* video MAIN */
    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [i2_i++].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_NAME;
    at_scc_cmds [i2_i++].u.ps_text  = SN_PRES_MAIN_DISPLAY;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_VIDEO_MODE;
    at_scc_cmds [i2_i++].u.ui4_data = (UINT32) SCC_VID_DEINT;

    t_video_bg.ui1_blue  = 0;
    t_video_bg.ui1_green = 0;
    t_video_bg.ui1_red   = 0;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_VIDEO_BACKGROUND;
    at_scc_cmds [i2_i++].u.pv_data  = (VOID*) &t_video_bg;

    /* video SUB */
    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [i2_i++].u.ui4_data = SCC_CMD_TYPE_VIDEO;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_NAME;
    at_scc_cmds [i2_i++].u.ps_text  = SN_PRES_SUB_DISPLAY;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_VIDEO_MODE;
    at_scc_cmds [i2_i++].u.ui4_data = (UINT32) SCC_VID_NORMAL;

    t_video_bg.ui1_blue  = 0;
    t_video_bg.ui1_green = 0;
    t_video_bg.ui1_red   = 0;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_VIDEO_BACKGROUND;
    at_scc_cmds [i2_i++].u.pv_data  = (VOID*) &t_video_bg;

    /* audio */
    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_GRP_TYPE;
    at_scc_cmds [i2_i++].u.ui4_data = SCC_CMD_TYPE_AUDIO;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_NAME;
    at_scc_cmds [i2_i++].u.ps_text  = SN_PRES_MAIN_DISPLAY;

    at_scc_cmds [i2_i].e_code       = SCC_CMD_CODE_AUDIO_OUT_PORT;
    at_scc_cmds [i2_i++].u.ui4_data = (UINT32) SCC_AUD_OUT_PORT_2_CH | SCC_AUD_OUT_PORT_5_1_CH | SCC_AUD_OUT_PORT_SPDIF;

    at_scc_cmds [i2_i].e_code       = SM_CMD_CODE_END;


    if (x_scc_comp_grp_set (&(at_scc_cmds [0])) == SMR_OK)
    {
        b_success = TRUE;
    }

    return (b_success);
}

#ifdef SYS_FLASH_FONT_SUPPORT
/* callback function only for passing parameter */
static VOID _fm_mnt_nfy_fct(
    FM_MNT_COND_T t_cond,
    CHAR  *ps_part_name,
    UINT32  ui4_part_idx,
    CHAR *ps_mnt_path,
    VOID *pv_tag)
{
    return;
}
#endif /* SYS_FLASH_FONT_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name: init_raw_dev
 *
 * Description: This API mounts the raw file system devices.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_raw_dev (VOID)
{
    BOOL b_success = FALSE;
    HANDLE_T h_dev_dir;

#ifdef SYS_FLASH_FONT_SUPPORT
    FM_MNT_CB_T t_mnt_cb;
#endif /* SYS_FLASH_FONT_SUPPORT */

    /* The flash settting is in .cfg   
      DEFINES += -DSN_NAND_FLASH_1=\"mtd4\"
      DEFINES += -DSN_NAND_FLASH_2=\"mtdc\"
      DEFINES += -DSN_NAND_FLASH_4=\"mtdd\"
      DEFINES += -DSN_NAND_FLASH_5=\"mtd5\"
      
      The acturally partion define: 
      #define PARTITION_TURNKEY "mt53xx-nand:" \
                                "2M(uboot),2M(uboot_env),2M(sigA),2M(sigB),"  \
                                "2M(kernelA),"   \
                                "2M(kernelB),"   \
                                "10M(rootfsA),"   \
                                "10M(rootfsB),"   \
                                "25M(basic)," \
                                "176M(rw_area)," \
                                "5M(perm)," \
                                "128k(reserved)," \
                                "2M(channelsA),2M(channelsB),"   \
                                "2M(pq),2M(aq)," \                                
                                "3M(logo)," \
                                "128k(ci)"
   */

    if ( x_fm_open_dir(FM_ROOT_HANDLE, SN_DEV_PATH, &h_dev_dir) == FMR_OK)
    {
        /* Mount all NOR Flash devices and EEPROM devices. */
        /* This piece of code must be enhanced so that we can cycle through all  */
        /* possible raw file system devices and mount them correctly. Also, this */
        /* is the place wer we can mount other devices, which are used to store  */
        /* the configuration database etc.                                       */
        if ((x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_0,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  0) == FMR_OK) && /* Loader*/
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_2,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  2) == FMR_OK) && /* SigA */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_3,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  3) == FMR_OK) && /* SigB */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_6,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  6) == FMR_OK) && /* rootfs A */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_7,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  7) == FMR_OK) && /* rootfs B */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_1,              (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  4) == FMR_OK) && /* Kernel A */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_APP_IMAGE_1,    (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  4) == FMR_OK) && /* Kernel A */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_2,              (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 12) == FMR_OK) && /* channels B */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_CHANNEL_LIST_1, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 12) == FMR_OK) && /* channels A */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_5,              (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  5) == FMR_OK) && /* Kernel B */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_APP_IMAGE_2,    (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH,  5) == FMR_OK) && /* Kernel B */
            #ifdef DUAL_CHANNEL_SUPPORT
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_4,              (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 13) == FMR_OK) && /* channels B */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_CHANNEL_LIST_2, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 13) == FMR_OK) && /* channels B */
            #endif
            #ifndef DISABLE_BASIC_PART
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_8,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_VIRT | 0666), DRVT_NAND_FLASH, 8) == FMR_OK) && /* basic */
            #endif
            #ifndef SYS_FAST_BOOT
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_9,            (FM_MODE_TYPE_BLK | FM_MODE_DEV_VIRT | 0666), DRVT_NAND_FLASH, 9) == FMR_OK) && /* 3rd */
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_10,           (FM_MODE_TYPE_BLK | FM_MODE_DEV_VIRT | 0666), DRVT_NAND_FLASH, 10) == FMR_OK) && /* perm */
            #endif /* SYS_FAST_BOOT */
            #ifdef SUPPORT_FLASH_PQ
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_14,           (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 14) == FMR_OK) &&
            #endif
            #ifdef SUPPORT_FLASH_AQ
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_15,           (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 15) == FMR_OK) &&
            #endif
            #ifdef CI_PLUS_SUPPORT
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_17,           (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 17) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_CIPLUS_1,       (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 17) == FMR_OK) &&
            #endif            
#ifdef SYS_FLASH_FONT_SUPPORT
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_FONT_1, (FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0666), DRVT_NAND_FLASH, 8) == FMR_OK) &&
#endif /* SYS_FLASH_FONT_SUPPORT */

            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_EEPROM_0,    (FM_MODE_TYPE_CHR | FM_MODE_DEV_TRUE | 0666), DRVT_EEPROM,    0) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_EEPROM_3,    (FM_MODE_TYPE_CHR | FM_MODE_DEV_TRUE | 0666), DRVT_EEPROM,    3) == FMR_OK) &&
            (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_EEPROM_4,    (FM_MODE_TYPE_CHR | FM_MODE_DEV_TRUE | 0666), DRVT_EEPROM,    4) == FMR_OK))
        {
            b_success = TRUE;
        }

#ifdef SYS_FLASH_FONT_SUPPORT
        t_mnt_cb.pf_mnt_fct = _fm_mnt_nfy_fct;
        t_mnt_cb.pv_tag = (VOID *) NULL;

        x_fm_parse_drive(FM_ROOT_HANDLE,
                         SN_DEV_PATH "/" SN_FLASH_FONT_1,
                         (VOID *) &t_mnt_cb);
#endif /* SYS_FLASH_FONT_SUPPORT */

        if (b_success == TRUE)
        {
            do
            {
                b_success = FALSE;
#ifndef DISABLE_BASIC_PART
                /* attach basic dir to MTD Block 8 */
                if (x_fm_create_dir(FM_ROOT_HANDLE, "/basic", 0666) != FMR_OK)
                {
                    break;
                }
                if (x_fm_attach_file_system(FM_ROOT_HANDLE,
                                            SN_DEV_PATH "/" SN_MTD_BLK_8,
                                            FM_ROOT_HANDLE,
                                            "/basic") != FMR_OK)
                {
                    break;
                }
#endif

#ifndef SYS_FAST_BOOT
                if (x_fm_create_dir(FM_ROOT_HANDLE, "/perm", 0666) != FMR_OK)
                {

                    break;
                }

                /* attach 3rd dir to MTD Block 10 */
                if (x_fm_attach_file_system(FM_ROOT_HANDLE,
                                        SN_DEV_PATH "/" SN_MTD_BLK_10,
                                        FM_ROOT_HANDLE,
                                        "/perm") != FMR_OK)
                {
                    break;
                }

                if (x_fm_create_dir(FM_ROOT_HANDLE, "/3rd", 0666) != FMR_OK)
                {
                    break;
                }
                /* attach 3rd dir to MTD Block 9 */
                if (x_fm_attach_file_system(FM_ROOT_HANDLE,
                                        SN_DEV_PATH "/" SN_MTD_BLK_9,
                                        FM_ROOT_HANDLE,
                                        "/3rd") != FMR_OK)
                {
                    break;
                }
		#ifdef MHEG5_NVM_SUPPORT
                x_mheg5_init_tps();
                #endif
#endif /* SYS_FAST_BOOT */
                b_success = TRUE;
            } while(0);
        }
    }
    x_fm_fat_init(NULL);
#ifdef FM_SUPPORT_NTFS
    x_fm_ntfs_init(NULL);
#endif  /* #endif FM_SUPPORT_NTFS */

    x_fm_create_dir(FM_ROOT_HANDLE, SYS_FILESYSTEM_MOUNT_ENTRY, 0666);

#ifdef SYS_FLASH_FONT_SUPPORT
    x_fm_create_dir(FM_ROOT_HANDLE, SYS_FILESYSTEM_MOUNT_ENTRY "/" SYS_FLASH_FONT_DIR_NAME, 0666);
#endif /* SYS_FLASH_FONT_SUPPORT */

    x_fm_close(h_dev_dir);

#ifdef SYS_FLASH_FONT_SUPPORT
    x_fm_mount(FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_FLASH_FONT_1 "_0", FM_ROOT_HANDLE,
        SYS_FILESYSTEM_MOUNT_ENTRY "/" SYS_FLASH_FONT_DIR_NAME);
#endif /* SYS_FLASH_FONT_SUPPORT */

    return (b_success);
}

static BOOL init_sect_eng(VOID)
{
    BOOL            b_success = FALSE;
    SECT_CONFIG_T   t_sect_cfg;

    t_sect_cfg.ui1_num_filters     = 8;
    t_sect_cfg.ui1_num_filter_objs = 8;
    t_sect_cfg.z_buff_size         = 96 * 1024;
    t_sect_cfg.ui2_num_reqs        = 255;
    
    if (TMR_OK == x_sect_init(
        "sect_eng",
        &t_sect_cfg))
    {
        b_success = TRUE;
    }

    return (b_success);
}

/*-----------------------------------------------------------------------------
 * Name: init_cec_mngr
 *
 * Description: This API initializes the CEC Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/

#if 0
static BOOL init_cec_mngr (VOID)
{
    THREAD_DESCR_T t_cecm_thread_descr = {DEFAULT_STACK_SIZE,
                                          DEFAULT_PRIORITY,
                                          DEFAULT_NUM_MSGS};

    if (x_cecm_init(&t_cecm_thread_descr) != CMR_OK)
    {
        return FALSE;
    }

    return TRUE;
}
#endif

#ifdef SYS_MHEG5_SUPPORT
static BOOL get_dtv_default_overscan(SCC_VID_OVER_SCAN_CLIPPER_T* pt_ovs_clip)
{
    extern APP_CFG_CUST_OVERSCAN_TBL_T  at_sys_overscan_table_main[];
    
    APP_CFG_CUST_OVERSCAN_TBL_T* pt_ovs_cfg = &(at_sys_overscan_table_main[0]);

    while (1)
    {
        if (INP_SRC_TYPE_UNKNOWN == pt_ovs_cfg->e_src_type)
        {
            return FALSE;
        }
        
        if ((INP_SRC_TYPE_TV   == pt_ovs_cfg->e_src_type) &&
            (DEV_TUNER_TER_DIG == pt_ovs_cfg->e_video_type))
        {
            pt_ovs_clip->ui4_top    = pt_ovs_cfg->ui4_top;
            pt_ovs_clip->ui4_bottom = pt_ovs_cfg->ui4_bottom;
            pt_ovs_clip->ui4_left   = pt_ovs_cfg->ui4_left;
            pt_ovs_clip->ui4_right  = pt_ovs_cfg->ui4_right;

            return TRUE;
        }

        pt_ovs_cfg++;
    }
}

static BOOL init_mheg5_eng (VOID)
{
    BOOL                        b_success = FALSE;
    SCC_VID_OVER_SCAN_CLIPPER_T t_ovs_clip = {0, 0, 0, 0};

    get_dtv_default_overscan(&t_ovs_clip);
    
    if (MHEG5R_OK == x_mheg5_OceanBlue_stack_init(
        "OceanBlue_MHEG5",
        SN_MAIN_TUNER_GRP,
        MHEG5_DISPLAY_ASPECT_RATIO_16_9,
        &t_ovs_clip))
    {

#ifdef MHEG5_NVM_SUPPORT
        x_mheg5_init_tps();
#endif
        b_success = TRUE;
    }

    return (b_success);
}
#endif /* SYS_MHEG5_SUPPORT */

#ifdef DVBT_CI_ENABLE
/*-----------------------------------------------------------------------------
 * Name: init_common_interface
 *
 * Description: This API initializes the common interface and its engine.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_common_interface (VOID)
{
    BOOL b_success = FALSE;
#ifdef CI_PLUS_SUPPORT
    CIPLUS_CONFIG_T t_ciplus_config;
    CHAR* aui1_aes_key = NULL;
    CHAR* aui1_aes_iv= NULL;    
    BOOL b_ret = FALSE;
    UINT8 ui1_desc_algs;
    UINT8 ui1_search_pattern1_len;
    UINT8 ui1_search_pattern2_len;
#endif

#ifdef CI_PLUS_SUPPORT
    x_memset(&t_ciplus_config, 0x00, sizeof(CIPLUS_CONFIG_T));

    b_ret = sys_cust_get_fix_encrypt_mode (&t_ciplus_config.b_fix_key,
                                           &ui1_desc_algs,
                                           (UINT8 *)&t_ciplus_config.u_data.aes_128_cbc.z_key_len,
                                           (UINT8 *)&t_ciplus_config.u_data.aes_128_cbc.z_iv_len);    
    if (b_ret && t_ciplus_config.b_fix_key)
    {
        aui1_aes_key = x_mem_alloc(sizeof(UINT8)*(t_ciplus_config.u_data.aes_128_cbc.z_key_len+1));
        aui1_aes_iv = x_mem_alloc(sizeof(UINT8)*(t_ciplus_config.u_data.aes_128_cbc.z_iv_len+1));
        if ((aui1_aes_key == NULL) ||
            (aui1_aes_iv== NULL))
    {
            return FALSE;
    }
        x_memset(aui1_aes_key, 0x00, (t_ciplus_config.u_data.aes_128_cbc.z_key_len+1));
        x_memset(aui1_aes_iv, 0x00, (t_ciplus_config.u_data.aes_128_cbc.z_iv_len+1));        
        t_ciplus_config.t_desc_alg = (CIPLUS_CRED_DESC_ALG_T)ui1_desc_algs;

        sys_cust_get_fix_key (aui1_aes_key, 
                              aui1_aes_iv);
        x_memcpy(t_ciplus_config.u_data.aes_128_cbc.aui1_key, aui1_aes_key, t_ciplus_config.u_data.aes_128_cbc.z_key_len);
        x_memcpy(t_ciplus_config.u_data.aes_128_cbc.aui1_iv, aui1_aes_iv, t_ciplus_config.u_data.aes_128_cbc.z_iv_len);
    }

    sys_cust_get_custom_code_len (&t_ciplus_config.b_custom_code,
                                  &t_ciplus_config.ui1_len_code,
                                  &t_ciplus_config.ui1_max_len_code);
    if (t_ciplus_config.b_custom_code)
    {
        t_ciplus_config.pc_custom_code = x_mem_alloc(sizeof(CHAR)*(t_ciplus_config.ui1_len_code+1));
        x_memset(t_ciplus_config.pc_custom_code, 0x00, (t_ciplus_config.ui1_len_code+1));
        b_ret = sys_cust_get_custom_code_info (t_ciplus_config.ui1_len_code,
                                               t_ciplus_config.pc_custom_code);
        if (!b_ret)
        {
            return FALSE;
        }
    }

    b_ret = sys_cust_get_search_mode_pattern_len (&t_ciplus_config.t_search_mode_data.t_search_mode,
                                                  &t_ciplus_config.ui1_max_search_pattern_len,
                                                  &ui1_search_pattern1_len,
                                                  &ui1_search_pattern2_len);
    if (!b_ret)
    {
        return FALSE;
    }

    switch(t_ciplus_config.t_search_mode_data.t_search_mode)
    {
        case CI_FILE_SEARCH_MODE_FULL:
            /* set search mode 1 */
            t_ciplus_config.t_search_mode_data.u_para.t_fullname.ui2_filename_len = ui1_search_pattern1_len;
            t_ciplus_config.t_search_mode_data.u_para.t_fullname.pc_filename = x_mem_alloc(sizeof(CHAR)*(ui1_search_pattern1_len+1));
            if (t_ciplus_config.t_search_mode_data.u_para.t_fullname.pc_filename == NULL)
            {
                return FALSE;
            }
            x_memset(t_ciplus_config.t_search_mode_data.u_para.t_fullname.pc_filename, 0x00, (ui1_search_pattern1_len+1));
            b_ret = sys_cust_get_search_mode_pattern_info (t_ciplus_config.t_search_mode_data.t_search_mode,
                                                           ui1_search_pattern1_len,
                                                           t_ciplus_config.t_search_mode_data.u_para.t_fullname.pc_filename, 
                                                           0,
                                                           NULL);
            if (!b_ret)
            {
                return FALSE;
            }            
            break;
        
        case CI_FILE_SEARCH_MODE_PREFIX_EXT:
            /* set search mode 2 */
            t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.ui2_prefix_len = ui1_search_pattern1_len;
            t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.ui2_extension_len = ui1_search_pattern2_len;
            t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_prefix = x_mem_alloc(sizeof(CHAR)*(ui1_search_pattern1_len+1));
            t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_extension = x_mem_alloc(sizeof(CHAR)*(ui1_search_pattern2_len+1));
            if ((t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_prefix == NULL) ||
                 t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_extension == NULL)
            {
                return FALSE;
            }
            x_memset(t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_prefix, 0x00, (ui1_search_pattern1_len+1));
            x_memset(t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_extension, 0x00, (ui1_search_pattern2_len+1));            
            b_ret = sys_cust_get_search_mode_pattern_info (t_ciplus_config.t_search_mode_data.t_search_mode,
                                                           ui1_search_pattern1_len,
                                                           t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_prefix, 
                                                           ui1_search_pattern2_len,
                                                           t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_extension);
            if (!b_ret)
            {
                return FALSE;
            }            
            break;
        
        case CI_FILE_SEARCH_MODE_PREFIX:
            /* set search mode 3 */
            t_ciplus_config.t_search_mode_data.u_para.t_prefix.ui2_prefix_len = ui1_search_pattern1_len;
            t_ciplus_config.t_search_mode_data.u_para.t_prefix.pc_prefix = x_mem_alloc(sizeof(CHAR)*(ui1_search_pattern1_len+1));
            if (t_ciplus_config.t_search_mode_data.u_para.t_prefix.pc_prefix == NULL)
            {
                return FALSE;
            }
            x_memset(t_ciplus_config.t_search_mode_data.u_para.t_prefix.pc_prefix, 0x00, (ui1_search_pattern1_len+1));
            b_ret = sys_cust_get_search_mode_pattern_info (t_ciplus_config.t_search_mode_data.t_search_mode,
                                                           ui1_search_pattern1_len,
                                                           t_ciplus_config.t_search_mode_data.u_para.t_prefix.pc_prefix, 
                                                           0,
                                                           NULL);
            if (!b_ret)
            {
                return FALSE;
            } 
            break;

        case CI_FILE_SEARCH_MODE_EXT:
            /* set search mode 4 */
            t_ciplus_config.t_search_mode_data.u_para.t_ext.ui2_extension_len = ui1_search_pattern1_len;
            t_ciplus_config.t_search_mode_data.u_para.t_ext.pc_extension = x_mem_alloc(sizeof(CHAR)*(ui1_search_pattern1_len+1));
            if (t_ciplus_config.t_search_mode_data.u_para.t_ext.pc_extension == NULL)
            {
                return FALSE;
            }
            x_memset(t_ciplus_config.t_search_mode_data.u_para.t_ext.pc_extension, 0x00, (ui1_search_pattern1_len+1));
            b_ret = sys_cust_get_search_mode_pattern_info (t_ciplus_config.t_search_mode_data.t_search_mode,
                                                           ui1_search_pattern1_len,
                                                           t_ciplus_config.t_search_mode_data.u_para.t_ext.pc_extension, 
                                                           0,
                                                           NULL);
            if (!b_ret)
            {
                return FALSE;
            } 
        break;

        default:
            return FALSE;
    }
        
    //t_ciplus_config.pc_dst_path = CI_FILE_CREDENTIALS;    
    t_ciplus_config.pc_dst_path = CI_FILE_CREDENTIALS_CUST;   

    /* pre-fix to rename ci+ credential files after written down */
    sys_cust_get_rename_prefix_len(&t_ciplus_config.ui1_rename_prefix_len,
                                   &t_ciplus_config.ui1_max_rename_prefix_len);
    t_ciplus_config.pc_rename_prefix = x_mem_alloc(sizeof(CHAR)*(t_ciplus_config.ui1_rename_prefix_len+1));
    if (t_ciplus_config.pc_rename_prefix == NULL)
    {
        return FALSE;
    }
    x_memset(t_ciplus_config.pc_rename_prefix, 0x00, (t_ciplus_config.ui1_rename_prefix_len+1));
    b_ret = sys_cust_get_rename_prefix_info (t_ciplus_config.ui1_rename_prefix_len,
                                             t_ciplus_config.pc_rename_prefix);
    if (!b_ret)
    {
        return FALSE;
    }    
    t_ciplus_config.b_rename_after_write = TRUE;

    /* set the maxium of serial number */
    sys_cust_get_max_serial_num_len(&t_ciplus_config.ui1_max_serial_num_len);
    
    x_ciplus_config(&t_ciplus_config);

    /* free temporatory allocated sources */
    x_mem_free(aui1_aes_key);
    x_mem_free(aui1_aes_iv);
    x_mem_free(t_ciplus_config.pc_custom_code);
    x_mem_free(t_ciplus_config.pc_rename_prefix);
    switch(t_ciplus_config.t_search_mode_data.t_search_mode)
    {
        case CI_FILE_SEARCH_MODE_FULL:
            x_mem_free(t_ciplus_config.t_search_mode_data.u_para.t_fullname.pc_filename);
            break;
        case CI_FILE_SEARCH_MODE_PREFIX_EXT:
            x_mem_free(t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_prefix);
            x_mem_free(t_ciplus_config.t_search_mode_data.u_para.t_prefix_ext.pc_extension);
            break;
        case CI_FILE_SEARCH_MODE_PREFIX:
            x_mem_free(t_ciplus_config.t_search_mode_data.u_para.t_prefix.pc_prefix);
            break;
        case CI_FILE_SEARCH_MODE_EXT:
            x_mem_free(t_ciplus_config.t_search_mode_data.u_para.t_ext.pc_extension);
            break;
        default:
            /* Impossible case */
            return FALSE;
    }
#endif

    x_ci_config(MAX_NUM_PM_SVCS, 
                MAX_NUM_PM_COMPS,
                MAX_CA_SRC_DRV_TYPE,
                at_ca_src_type);

#ifdef MW_CI_OCEANBLUE_SUPPORT
    if ((CIR_OK == x_ci_init(sys_cust_get_ci_sw_version())) &&
        (CIR_OK == x_ci_oceanblue_eng_init()))

    {
        b_success = TRUE;
    }
#else
    if ((CIR_OK == x_ci_init(sys_cust_get_ci_sw_version())) &&
        (CIR_OK == x_ci_wipro_eng_init()))

    {
        b_success = TRUE;
    }
#endif

    return (b_success);
}
#endif

#ifdef SYS_OAD_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: init_dsmcc
 *
 * Description: This API initializes the dsmcc engine
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_dsmcc (VOID)
{
    BOOL b_success = FALSE;
    DSMCC_CONFIG_T t_config;

    t_config.ui1_num_dsi_filters = 2;
    t_config.ui1_num_dii_filters = 1;
    t_config.ui1_num_ddb_filters = 1;
    t_config.ui1_num_ddl_filters = 1;
    t_config.z_dsi_buff_size = 128*1024;
    t_config.z_dii_buff_size = 64*1024;    
    t_config.z_ddb_buff_size = 64*1024;    
    t_config.z_ddl_buff_size = 64*1024;    
    t_config.ui2_num_dsi_tbl_objs = 2;    
    t_config.ui2_num_dii_tbl_objs = 1;    
    t_config.ui2_num_ddb_tbl_objs = 16;    
    t_config.ui2_num_ddl_tbl_objs = 1;    
    t_config.ui2_num_mon_tbl_objs = 1;        

    if ((TMR_OK == x_dsmcc_init("dsmcc_eng", &t_config)) &&
        (TMR_OK == x_dsmcc_oc_init("dsmcc_oc")))

    {
        b_success = TRUE;
    }

    return (b_success);
}
#endif /* SYS_OAD_SUPPORT */

/*-----------------------------------------------------------------------------
 * Name: init_divx_drm
 *
 * Description: This API initializes the divx drm.
 *
 * Inputs: None.
 *
 * Outputs: None.
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
#if defined(ENABLE_MULTIMEDIA) && defined(ENABLE_DIVX_DRM)
static BOOL init_divx_drm(VOID)
{
    DIVX_DRM_INIT_CONFIG_T t_divx_drm_init_config;

    t_divx_drm_init_config.ui2_divx_drm_memory_offset = DIVX_DRM_MEMORY_OFFSET;
    t_divx_drm_init_config.ui2_divx_drm_memory_size_byte = DIVX_DRM_MEMORY_SIZE_BYTE;
    t_divx_drm_init_config.ui1_divx_drm_model_id_low_byte = DIVX_DRM_MODEL_ID_LOW_BYTE;
    t_divx_drm_init_config.ui1_divx_drm_model_id_high_byte = DIVX_DRM_MODEL_ID_HIGH_BYTE;    

    if (DRMR_OK != x_divx_drm_init(&t_divx_drm_init_config))
    {
        return FALSE;
    }

    return TRUE;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: sys_cust_mm_init
 *
 * Description: This API initializes the Multimedia modules.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
 
static UINT32 sys_cust_mm_init(VOID)
{
#ifdef ENABLE_MULTIMEDIA
#ifdef MW_MMP_VIDEO_SUPPORT
    WGL_SBTL_SYS_INIT_T     t_sys_init;
#endif

#ifdef MW_MP3_SUPPORT
    
    if (x_minfo_mp3_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
    
    if (x_midxbuld_mp3_init() != MIDXBULDR_OK)
    {
        return R_INIT_MIDXBULD_FAIL;
    }
    
#endif

#ifdef MW_MMP_VIDEO_SUPPORT
    if (x_mfmtrecg_vid_es_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
/*
    if (x_mfmtrecg_avs_init() != MFMTRECGR_OK)
    {        
        return R_INIT_MFMTRECG_FAIL;
    }
    */
    if (x_mfmtrecg_aud_es_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }

    if (x_minfo_aud_es_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
#ifdef MW_MP3_SUPPORT
    if (x_mfmtrecg_mp3_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
#endif
    if (x_mfmtrecg_ps_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
    
    if (x_minfo_ps_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }

    if (x_mfmtrecg_ts_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }

    if (x_mfmtrecg_avi_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
    
    if (x_minfo_avi_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
    
    if (x_midxbuld_avi_init() != MIDXBULDR_OK)
    {
        return R_INIT_MIDXBULD_FAIL;
    }
    
    if (x_mfmtrecg_flv_init()!= MFMTRECGR_OK)
    {
        return FALSE;
    }
    
    if (x_minfo_flv_init()!= MFMTRECGR_OK)
    {
        return FALSE;          
    }
    
    if (x_midxbuld_flv_init()!= MFMTRECGR_OK)
    {
        return FALSE;          
    }
    
    if (x_mfmtrecg_asf_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
    
    if (x_minfo_asf_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
    
    if (x_midxbuld_asf_init() != MIDXBULDR_OK)
    {
        return R_INIT_MIDXBULD_FAIL;
    }
    
    if (x_mfmtrecg_mp4_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
    
    if (x_minfo_mp4_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
    
    if (x_midxbuld_mp4_init() != MIDXBULDR_OK)
    {
        return R_INIT_MIDXBULD_FAIL;
    }
    
    if (x_mfmtrecg_mkv_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }
    
    if (x_minfo_mkv_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
    if (x_midxbuld_mkv_init() != MIDXBULDR_OK)
    {
        return R_INIT_MIDXBULD_FAIL;
    }


/*
    if (x_mfmtrecg_aif_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }

    if (x_minfo_aif_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }

    if (x_mfmtrecg_rm_init() != MFMTRECGR_OK)
    {        
        return R_INIT_MFMTRECG_FAIL;
    }

    if (x_minfo_rm_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }

    if (x_midxbuld_rm_init() != MIDXBULDR_OK)
    {
        return R_INIT_MIDXBULD_FAIL;
    }
  */      
    if (x_mfmtrecg_ogg_init() != MFMTRECGR_OK)
    {
        return R_INIT_MFMTRECG_FAIL;
    }

    if (x_minfo_ogg_init() != MINFOR_OK)
    {
        return R_INIT_MINFO_FAIL;
    }
    
    x_sm_mm_sbtl_hdlr_init();
    x_sbtl_divx_init();
    x_sbtl_text_init();
    x_sbtl_vobsub_init();
    x_sbtl_mkv_init();
    x_sbtl_mp4_init();
    x_sbtl_lyric_init();
#endif /* MW_MMP_VIDEO_SUPPORT */
#if defined(APP_COLOR_MODE_PALLET)
    t_sys_init.ui4_flag                = WGL_SBTL_SYS_INIT_FLAG_PALETTE_MODE ;
    t_sys_init.e_clr_mode_frm_buf      = GL_COLORMODE_ARGB_CLUT8;
    t_sys_init.ui1_cnvs_plt_sbtl_start = sys_cust_get_ttx_sbtl_plt_range_start();
    t_sys_init.ui1_cnvs_plt_sbtl_end   = sys_cust_get_ttx_sbtl_plt_range_end();
#else
    t_sys_init.ui4_flag           = 0;
    t_sys_init.e_clr_mode_frm_buf = GL_COLORMODE_ARGB_D8888;
#endif
    
    x_wgl_sbtl_init(&t_sys_init);
    
#ifdef ENABLE_DIVX_DRM
    if (!init_divx_drm())
    {
        return R_INIT_DRM_FAIL;
    }
#endif /* ENABLE_DIVX_DRM */

#endif /* ENABLE_MULTIMEDIA */

    return R_INIT_MM_OK;
}

#ifdef SYS_RECORD_SUPPORT

typedef struct _PVR_BUF_RES_T
{
    BOOL                        b_allocted;
    
    VOID*                       pv_buf_physi;
    SIZE_T                      z_size_physi;

    REC_UTIL_PBA_BUF_INFO_T     t_pba_buf;
    
} PVR_BUF_RES_T;
    
static PVR_BUF_RES_T   at_g_pvr_bufs[1];

static BOOL pvr_buf_query_avail (
    VOID*                           pv_tag,
    const REC_UTIL_PBA_PARAM_T*     pt_param,
    SIZE_T*                         pz_max_size)
{
    *pz_max_size = pt_param->z_size;
        
     return TRUE;
}

static BOOL pvr_buf_alloc (
    VOID*                           pv_tag,
    const REC_UTIL_PBA_PARAM_T*     pt_param,
    REC_UTIL_PBA_BUF_INFO_T*        pt_buf_info)
{
    const UINT32    ui4_num_res = (UINT32) (sizeof (at_g_pvr_bufs) / sizeof (PVR_BUF_RES_T));
    UINT32          ui4_idx_buf;
    PVR_BUF_RES_T*  pt_buf_res;
    
    pt_buf_res = NULL;
    
    for (ui4_idx_buf = 0; ui4_idx_buf < ui4_num_res; ui4_idx_buf ++)
    {
        if (!at_g_pvr_bufs[ui4_idx_buf].b_allocted)
        {
            pt_buf_res = & at_g_pvr_bufs[ui4_idx_buf];
            break;
        }
    }
    
    if (pt_buf_res == NULL)
    {
        return FALSE;
    }
    
    pt_buf_res->z_size_physi = pt_param->z_size + pt_param->ui4_mem_align;
    
    pt_buf_res->pv_buf_physi = x_share_mem_alloc (pt_buf_res->z_size_physi);
    if (pt_buf_res->pv_buf_physi == NULL ||
        (((UINT32) pt_buf_res->pv_buf_physi) + ((UINT32) pt_buf_res->z_size_physi)) >= 0x40000000 )
    {
        return FALSE;
    }
    
    pt_buf_res->t_pba_buf.ui4_buf_id = ui4_idx_buf;
    pt_buf_res->t_pba_buf.pv_buf = 
            (VOID *) (((((UINT32) pt_buf_res->pv_buf_physi) + (pt_param->ui4_mem_align - 1)) / 
                        pt_param->ui4_mem_align) * pt_param->ui4_mem_align);
    pt_buf_res->t_pba_buf.z_size = pt_param->z_size;
    pt_buf_res->b_allocted = TRUE;

    *pt_buf_info = pt_buf_res->t_pba_buf;
    
    return TRUE;
}

static BOOL pvr_buf_free (
    VOID*                           pv_tag,
    const REC_UTIL_PBA_BUF_INFO_T*  pt_buf_info)
{
    const UINT32    ui4_num_res = (UINT32) (sizeof (at_g_pvr_bufs) / sizeof (PVR_BUF_RES_T));
    PVR_BUF_RES_T*  pt_buf_res;
    
    if (pt_buf_info->ui4_buf_id >= ui4_num_res)
    {
        return FALSE;
    }
    
    pt_buf_res = & at_g_pvr_bufs[pt_buf_info->ui4_buf_id];
    
    if (!(pt_buf_res->b_allocted                              &&
          pt_buf_info->pv_buf == pt_buf_res->t_pba_buf.pv_buf && 
          pt_buf_info->z_size == pt_buf_res->t_pba_buf.z_size))
    {
        return FALSE;
    }
    
    x_share_mem_free (pt_buf_res->pv_buf_physi, pt_buf_res->z_size_physi);
    
    x_memset ((VOID *) pt_buf_res, 0, sizeof (PVR_BUF_RES_T));
    pt_buf_res->b_allocted = FALSE;
    
    return TRUE;
}

static BOOL init_record_sink (VOID)
{
    static REC_UTIL_BITRATE_MAP_ITEM_T  at_g_brm[] = 
    {
        {
			BRDCST_TYPE_UNKNOWN,
            BRDCST_MEDIUM_DIG_TERRESTRIAL,
            /* 1179648 */      /* SD-only: 9Mbps */
            2621440            /* SD + HD + H264: 20Mbps */
        },
        {
            BRDCST_TYPE_UNKNOWN,
            BRDCST_MEDIUM_DIG_CABLE,
            /* 1179648 */      /* SD-only: 9Mbps */
            2621440            /* SD + HD + H264: 20Mbps */
        },
        {
            BRDCST_TYPE_DVB,
            BRDCST_MEDIUM_DIG_TERRESTRIAL,
            /* 1179648 */      /* SD-only: 9Mbps */
            2621440            /* SD + HD + H264: 20Mbps */
        },
        {
            BRDCST_TYPE_DVB,
            BRDCST_MEDIUM_DIG_CABLE,
            /* 1179648 */      /* SD-only: 9Mbps */
            2621440            /* SD + HD + H264: 20Mbps */
        }
    };
    
    const UINT32             ui4_num_res = (UINT32) (sizeof (at_g_pvr_bufs) / sizeof (PVR_BUF_RES_T));
    UINT32                   ui4_i;
    REC_UTIL_SYS_CONFIG_T    t_ru_cfg;
    INT32                    i4_ret;

    /* init record session */
    i4_ret = x_sm_sess_rcrd_init ();
    if ( i4_ret != SMR_OK )
    {
        return FALSE;
    }

    /* init record stream */
    i4_ret = x_sm_rcrd_strm_hldr_init ();
    if ( i4_ret != SMR_OK )
    {
        return FALSE;
    }
    
    /* init pvr buf res */
    x_memset ((VOID *) at_g_pvr_bufs, 0, sizeof (at_g_pvr_bufs));
    for (ui4_i = 0; ui4_i < ui4_num_res; ui4_i ++)
    {
        at_g_pvr_bufs[ui4_i].b_allocted = FALSE;
    } 
    
    /* init record utility */
    t_ru_cfg.ui4_strct_ver         = 1;
    t_ru_cfg.ui4_flags             = 0;
    t_ru_cfg.ui4_write_fail_rate   = 0;
    t_ru_cfg.ui2_bitrate_map_items = (UINT16) (sizeof (at_g_brm) / sizeof (REC_UTIL_BITRATE_MAP_ITEM_T));
    t_ru_cfg.at_bitrate_map        = at_g_brm;
    t_ru_cfg.ui4_pvr_buf_depth     = 1500;
    t_ru_cfg.pv_tag_pvr_buf_cb     = NULL;
    t_ru_cfg.t_fct_tbl_pvr_buf.pf_query_avail_pvr_buf = pvr_buf_query_avail;
    t_ru_cfg.t_fct_tbl_pvr_buf.pf_alloc_pvr_buf       = pvr_buf_alloc;
    t_ru_cfg.t_fct_tbl_pvr_buf.pf_free_pvr_buf        = pvr_buf_free;
    
    i4_ret = x_rec_util_init (&t_ru_cfg);
    if (i4_ret != 0)
    {
        return FALSE;
    }
    
    return TRUE;
}    
#endif

#ifdef SYS_OAD_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: init_ssum
 *
 * Description: This API initializes the SSU engine
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE if successfull else FALSE.
 ----------------------------------------------------------------------------*/
static BOOL init_ssum (VOID)
{
    BOOL b_success = FALSE;

    if (SSUMR_OK == x_ssum_init())

    {
        b_success = TRUE;
    }

    return (b_success);
}
#endif

#ifdef MW_FAST_BOOT_PRE_CONNECT

INT32 x_custom_init_boot (GEN_CONFIG_T*  pt_config)
{
    x_os_set_timestamp("init_psi_eng");
    if (! (init_psi_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_PSI_ENG_INIT);
    }

    x_os_set_timestamp("init_raw_dev");

    if (! (init_raw_dev ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_RAW_DEV_INIT);
    }

#ifdef DVBT_CI_ENABLE
    x_os_set_timestamp("init_common_interface");
    if (! (init_common_interface()))
    {
        x_dbg_stmt("init_common_interface failed \n");
        x_os_set_timestamp("init_common_interface failed");
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_CI_INIT);
    }
#endif /* DVBT_CI_ENABLE */

    x_os_set_timestamp("init_brdcst_eng");
    if (! (init_brdcst_eng ()))
    {
        x_dbg_stmt("init_brdcst_eng failed\n");
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_BRDCST_ENG_INIT);
    }

    if(0)//a_cfg_ch_lst_load_tsl_svl() != APP_CFGR_OK)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_BRDCST_ENG_INIT);
    }
    return (INITR_OK);
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_custom_init
 *
 * Description: Customization init function.
 *
 * Inputs:  pt_config  References the configuration structure.
 *
 * Outputs: -
 *
 * Returns: Always INITR_OK
 ----------------------------------------------------------------------------*/
INT32 x_custom_init (GEN_CONFIG_T*  pt_config)
{
#if defined(SYS_TTX_SUPPORT) || defined(SYS_SUBTITLE_SUPPORT)
    HANDLE_T                h_partition;
    INT32                   i4_ret;
#endif
#if defined(SYS_TTX_SUPPORT) || defined(SYS_SUBTITLE_SUPPORT)
#ifdef SYS_MHEG5_SUPPORT
    VOID*       pv_mem_part = NULL;
#endif /* SYS_MHEG5_SUPPORT */
#endif 
#ifndef MW_FAST_BOOT_PRE_CONNECT
    if (! (init_psi_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_PSI_ENG_INIT);
    }
#endif
    if (0)//! (init_dvb_si_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_DVB_SI_ENG_INIT);
    }
#ifndef MW_FAST_BOOT_PRE_CONNECT
    if (! (init_brdcst_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_BRDCST_ENG_INIT);
    }
#endif

#ifdef SYS_OAD_SUPPORT
    if ( ! (init_dsmcc()) )
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_DSMCC_INIT);
    }
        
    if ( ! (init_ssum()) )
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_SSU_INIT);
    } 
#endif /* SYS_OAD_SUPPORT */

    if (! (init_widgets ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_WIDGET_INIT);
    }
#if 0
    if (! (init_main_disp ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_MAIN_DISP_INIT);
    }
#endif
#if defined(SYS_TTX_SUPPORT) || defined(SYS_SUBTITLE_SUPPORT)
    /* create memory partition */
    i4_ret = x_mem_part_create(&h_partition,
                               SM_SBTL_TTX_SERVICE_NAME,
                               (VOID*) gaui1_sbtl_ttx_strm_partition,
                               SM_SBTL_TTX_SERVICE_PARTITION_SIZE,
                               0);
    if (OSR_OK != i4_ret)
    {
        return (FALSE);
    }
#ifdef SYS_MHEG5_SUPPORT    
    if (sys_cust_enable_ttx_share_buf())
    {
        if ((pv_mem_part = x_mem_alloc (((SIZE_T) MHEG5_MEM_PARTITION_SIZE))) != NULL)
        {
            if (x_mem_part_create (&h_mheg5_mem_part, "ttx_share_buf", pv_mem_part, ((SIZE_T) MHEG5_MEM_PARTITION_SIZE), 0) != OSR_OK)
            {
                ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_TELETEXT_INIT);
            }
        }
        else
        {
            ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_TELETEXT_INIT);
        }
    }
#endif /* SYS_MHEG5_SUPPORT */
#endif /* SYS_TTX_SUPPORT */

#ifdef SYS_SUBTITLE_SUPPORT
    if (! (init_subtitle(h_partition)))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_SUBTITLE_INIT);
    }
#endif /* SYS_SUBTITLE_SUPPORT */

#ifdef SYS_TTX_SUPPORT
    if (! (init_teletext(h_partition)))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_TELETEXT_INIT);
    }
#endif /* SYS_TTX_SUPPORT */
#ifndef MW_FAST_BOOT_PRE_CONNECT
    if (! (init_raw_dev ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_RAW_DEV_INIT);
    }
#endif
#ifndef NO_FONTS_INIT
    if (0)//! (init_fonts ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_FONTS_INIT);
    }
#endif
    if (! (init_svctx_hdlrs ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_SVCTX_HDLRS_INIT);
    }

    if (! (init_rrctx_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_RRCTX_ENG_INIT);
    }
    
#if 0
    if (! (init_cec_mngr ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_CEC_MNGR_INIT);
    }    
    if (! (init_vbi_fltr ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_VBI_FLTR_INIT);
    }
#endif

    if (! (init_sect_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_SECT_ENG_INIT);
    }
#ifndef MW_FAST_BOOT_PRE_CONNECT
#ifdef DVBT_CI_ENABLE
    if (! (init_common_interface()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_CI_INIT);
    }
#endif /* DVBT_CI_ENABLE */
#endif
#ifdef SYS_MHEG5_SUPPORT
    if (mheg5_init(&(pt_config->t_mheg5_thread))!= INITR_OK)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_MHEG5_ENG_INIT);
    }

    if (! (init_mheg5_eng ()))
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_MHEG5_ENG_INIT);
    }

    if (sys_cust_enable_ttx_share_buf())
    {
        //a_nav_mheg5_set_mem_partition(h_mheg5_mem_part);
    }
#endif /* SYS_MHEG5_SUPPORT */

    if (x_dlm_init() != 0)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_DLM_ENG_INIT);
    }

     if (sys_cust_mm_init() != R_INIT_MM_OK)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_MM_INIT);        
    }

#ifdef MW_JPG_SUPPORT
    if (IMGR_OK != x_img_jpg_init())
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_JPG_INIT);
    }
    
    if (x_img_tfx_init() != IMGR_OK)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_TFX_INIT);
    }
#endif

#ifdef MW_BMP_SUPPORT
    if (IMGR_OK != x_img_bmp_init())
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_BMP_INIT);
    }
#endif

#ifdef MW_PNG_SUPPORT   
    if (IMGR_OK != x_img_png_init())
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_PNG_INIT);
    }
#endif

#ifdef MW_GIF_SUPPORT
    if (IMGR_OK != x_img_gif_init())
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_GIF_INIT);
    }
#endif

#ifdef MW_WEBP_SUPPORT
    if (IMGR_OK != x_img_webp_init())
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_WEBP_INIT);
    }
#endif

#ifdef INET_SUPPORT
#ifdef DLNA_SUPPORT
    x_net_dlna_reg_cli();
#endif

    x_net_ip_reg_cli();

#ifdef MW_WIFI_SUPPORT
    x_wifi_cli_init();
#endif
#endif

#ifdef SYS_RECORD_SUPPORT
    if (!init_record_sink ())
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_RECORD_INIT);
    }
#endif
#ifdef SYS_UK_FREEVIEWHD_SUPPORT
    if (x_cl_huffman_init() != TRUE)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_HUFFMAN_INIT);
    }
#endif

#ifdef SYS_HBBTV_SUPPORT
    if (x_hbbtv_init () != HBBTVR_OK)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_HBBTV_INIT);
    }
#endif
	printf("x_custom_init is over %d \n",__LINE__);
    return (INITR_OK);
}


/*-----------------------------------------------------------------------------
 * Name: x_appl_init
 *
 * Description: Application init function.
 *
 * Inputs:  pt_config  References the configuration structure.
 *
 * Outputs: -
 *
 * Returns: Always INITR_OK
 ----------------------------------------------------------------------------*/
#ifndef NO_X_APPL_INIT
INT32 x_appl_init (GEN_CONFIG_T*  pt_config)
{
#if 0
    HANDLE_T              h_app_mngr;
    APP_MNGR_INIT_SEQ_T   t_app_mngr_init_seq;
    AMB_REGISTER_INFO_T   t_amb_reg_info;
    AM_INIT_CFG_T         t_am_init_cfg;
    extern BOOL a_appl_init (VOID);

    init_input_src_table(&t_g_drv_config);
    
    /* Create the application manager synchronization semaphore. */
    if (x_sema_create (&t_app_mngr_init_seq.h_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        ABORT_APPL (DBG_CAT_SEMAPHORE, DBG_ABRT_CREATE_SEMA);
    }

    /* Initialize application manager */
    x_memset (& t_am_init_cfg, 0, sizeof (AM_INIT_CFG_T));

    t_am_init_cfg.e_lifecycle_mode    = AM_LIFECYCLE_STACK;
    t_am_init_cfg.e_btn_cond          = IOM_NFY_COND_BTN_DOWN;
    t_am_init_cfg.ui2_first_key_delay = FIRST_KEY_REP_DELAY;
    t_am_init_cfg.ui2_next_key_delay  = NEXT_KEY_REP_DELAY;

    a_am_init (& t_am_init_cfg, & t_amb_reg_info);

    t_app_mngr_init_seq.b_ok = FALSE;

    /* Start application manager */
    if (x_app_start (&(t_amb_reg_info.t_fct_tbl),
                     &(t_amb_reg_info.t_desc),
                     t_amb_reg_info.s_name,
                     app_mngr_nfy_fct,
                     ((VOID*) &t_app_mngr_init_seq),
                     &h_app_mngr) != AEER_OK)
    {
        ABORT_APPL (DBG_CAT_INIT, DBG_ABRT_CREATE_APP_MNGR);
    }

    /* And now wait until the application manager has signaled that it */
    /* has successfully started.                                       */
    if (x_sema_lock (t_app_mngr_init_seq.h_sema, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        ABORT_APPL (DBG_CAT_SEMAPHORE, DBG_ABRT_LOCK_SEMA);
    }

    /* If the application manager could not start successfully, abort.*/
    if (! (t_app_mngr_init_seq.b_ok))
    {
        ABORT_APPL (DBG_CAT_INIT, DBG_ABRT_APP_MNGR_START_FAILURE);
    }

    /* Free the application manager sequencing semaphore. */
    if (x_sema_delete (t_app_mngr_init_seq.h_sema) != OSR_OK)
    {
        ABORT_APPL (DBG_CAT_SEMAPHORE, DBG_ABRT_DELETE_SEMA);
    }

    if (a_appl_init() == 0)
    {
        ABORT_APPL (DBG_CAT_INIT, DBG_ABRT_CREATE_APP_FAILED);
    }
#endif
    return (INITR_OK);
}
#endif

#ifdef SYS_FAST_BOOT
VOID init_post_routie(VOID)
{
#ifdef INET_SUPPORT
#ifdef SYS_NETWORK_LAZY_INIT
    INT32   i4_ret = 0;
#endif
#endif

    /* Turn-on printk debug level to KERN_DEBUG */
#ifndef ENABLE_EFENCE_DTV_SVC
    system("echo 7 > /proc/sys/kernel/printk");
#endif
    /* delay for a while */
    x_thread_delay(2000);

    /* mount /perm partion */
    if (access(MOUNT_PERM_SH, F_OK) == 0)
    {
#ifndef ENABLE_EFENCE_DTV_SVC        
        system(SH_EXE" "MOUNT_PERM_SH);
#endif        
        if (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_10, (FM_MODE_TYPE | FM_MODE_DEV_VIRT | 0666), DRVT_FLASH, 10) == FMR_OK)/* perm */
        {
            x_fm_create_dir(FM_ROOT_HANDLE, "/perm", 0666);

            /* attach perm dir to MTD Block 10 */
            x_fm_attach_file_system(FM_ROOT_HANDLE,
                                    SN_DEV_PATH "/" SN_MTD_BLK_10,
                                    FM_ROOT_HANDLE,
                                    "/perm");
        }
        else
        {
            ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_RAW_DEV_INIT);
        }
    }

    /* mount /3rd partion */
    if (access(MOUNT_3RD_SH, F_OK) == 0)
    {
#ifndef ENABLE_EFENCE_DTV_SVC
        system(SH_EXE" "MOUNT_3RD_SH);
#endif
        if (x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_MTD_BLK_9, (FM_MODE_TYPE | FM_MODE_DEV_VIRT | 0666), DRVT_FLASH, 9) == FMR_OK)/* 3rd */
        {
            x_fm_create_dir(FM_ROOT_HANDLE, "/3rd", 0666);
            
            /* attach 3rd dir to MTD Block 9 */
            x_fm_attach_file_system(FM_ROOT_HANDLE,
                                    SN_DEV_PATH "/" SN_MTD_BLK_9,
                                    FM_ROOT_HANDLE,
                                    "/3rd");
            
            x_fm_create_dir(FM_ROOT_HANDLE, "/3rd/upgrade", 0666);
	    #ifdef MHEG5_NVM_SUPPORT
            x_mheg5_init_tps();
            #endif
        }
        else
        {
            ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_RAW_DEV_INIT);
        }
    }
    
    /* insert USB Core module */
    if (access(USB_CORE_MOD_SH, F_OK) == 0)
    {
#ifndef ENABLE_EFENCE_DTV_SVC
        system(SH_EXE" "USB_CORE_MOD_SH);
#endif
    }
    
#ifdef INET_SUPPORT
#ifdef SYS_NETWORK_LAZY_INIT

    /* insert ethernet driver */
    if (access(ETH_MOD_SH, F_OK) == 0)
    {
#ifndef ENABLE_EFENCE_DTV_SVC
        system(SH_EXE" "ETH_MOD_SH);
#endif
    }

    if (access(WIFI_MOD_SH, F_OK) == 0)
    {
#ifndef ENABLE_EFENCE_DTV_SVC
        system(SH_EXE" "WIFI_MOD_SH);
#endif
    }

    i4_ret = x_net_network_ready(NULL);
    if (i4_ret != 0)
    {
        ABORT_CUSTOM (DBG_CAT_INIT, DBG_ABRT_NETWORK_INIT);
    }
#endif /* SYS_NETWORK_LAZY_INIT */
#endif /* INET_SUPPORT */
    
    /* insert kernel modules */
    if (access(KERNEL_MOD_SH, F_OK) == 0)
    {
#ifndef ENABLE_EFENCE_DTV_SVC
        system(SH_EXE" "KERNEL_MOD_SH);
#endif
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name: x_sys_start
 *
 * Description: System entry point.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_sys_start (VOID*   pv_mem_addr,
                  SIZE_T  z_mem_size,
                  VOID*   pv_arg)
{	  
		X_CONFIG_T t_config;
	
		x_start_def_config (&t_config);
	
		t_config.pv_mem_addr	 = pv_mem_addr;
		t_config.z_mem_size 	 = z_mem_size;
		t_config.ui2_num_handles = MAX_NUM_HANDLES;
		t_config.ui2_num_comps	 = MAX_NUM_RM_COMPS;
		t_config.ps_model_name	 = MODEL_NAME;
	
		t_config.t_dt_config.t_src_type  = DT_SYNC_WITH_DVB_TDT;
		t_config.t_dt_config.t_src_desc  = DT_SRC_TYPE_MPEG_2_BRDCST;
		t_config.t_dt_config.pv_src_info = SB_TUNER_NAME;
	
		t_config.ui2_cdb_num_db 	 = MAX_NUM_CDB_DB;
		
#ifndef NO_SVL_INIT
		t_config.ui2_svl_max_num	 = MAX_NUM_SVLS;
#endif
	
#ifndef NO_TSL_INIT 
		t_config.ui2_tsl_max_num	 = MAX_NUM_TSLS;
#endif
		t_config.ui2_cm_num_handlers = MAX_NUM_CM_HANDLERS;
		t_config.ui2_pm_num_ca		 = MAX_NUM_PM_CAS;
		t_config.ui2_pm_num_service  = MAX_NUM_PM_SVCS;
		t_config.ui2_pm_num_comps	 = MAX_NUM_PM_COMPS;
		
#ifndef NO_FLM_INIT
		t_config.ui2_flm_max_num	 = MAX_NUM_FLS;
		t_config.ui2_flm_max_size	 = MAX_NUM_FL_ENTRIES;
#endif
#ifndef NO_ATV_MANAGER_INIT
		t_config.t_atv_mngr_cfg.ui1_version 					 = ATV_MNGR_VERSION;
		t_config.t_atv_mngr_cfg.ui1_noresp_req_handler_max_cnt	 = ATV_MNGR_MAX_NUM_NORESP_REQ_HANDLERS;
		t_config.t_atv_mngr_cfg.ui1_withresp_req_handler_max_cnt = ATV_MNGR_MAX_NUM_WITHRESP_REQ_HANDLERS;
		t_config.t_atv_mngr_cfg.ui1_resp_handler_max_cnt		 = ATV_MNGR_MAX_NUM_RESP_HANDLERS;
		t_config.t_atv_mngr_cfg.ui1_tx_delay_time				 = ATV_MNGR_DELAY_TIME;
		t_config.t_atv_mngr_cfg.ui1_tx_delay_byte_cnt			 = ATV_MNGR_DELAY_BYTE_CNT;
		t_config.t_atv_mngr_cfg.ui1_max_reliable_msg_cnt		 = ATV_MNGR_MAX_NUM_RELIABLE_MSGS;
		t_config.t_atv_mngr_cfg.ui1_noresp_req_buf_cnt			 = ATV_MNGR_NORESP_REQ_BUFF_CNT;
		t_config.t_atv_mngr_cfg.ui1_withresp_req_buf_cnt		 = ATV_MNGR_WITHRESP_REQ_BUFF_CNT;
		t_config.t_atv_mngr_cfg.t_rs_232_setup_info.e_data_len	 = ATV_RS_232_DATA_LEN_8;
		t_config.t_atv_mngr_cfg.t_rs_232_setup_info.e_parity	 = ATV_RS_232_PARITY_NONE;
		t_config.t_atv_mngr_cfg.t_rs_232_setup_info.e_speed 	 = ATV_RS_232_SPEED_115200;
		t_config.t_atv_mngr_cfg.t_rs_232_setup_info.e_stop_bit	 = ATV_RS_232_STOP_BIT_1;
	
		x_memset (((VOID*) &(t_config.t_atv_mngr_cfg.s_hardware_name [0])), 0, (SYS_NAME_LEN + 1));
#endif 
#ifndef NO_WGL_INIT
	
		t_config.ui4_width					= sys_cust_get_screen_width();
		t_config.ui4_height 				= sys_cust_get_screen_height();
		t_config.b_auto_scale_view			= TRUE;
		t_config.e_colormode				= sys_cust_get_color_mode();
		t_config.b_compressed_compose		= TRUE;  /* for new UI PNG effect */
	
		if (sys_cust_get_color_mode() == GL_COLORMODE_ARGB_CLUT8)
		{
#if defined(APP_COLOR_MODE_PALLET)
		   t_config.t_color_key 			   = *(sys_cust_get_default_colorkey()); 
		   sys_cust_get_default_clut(&(t_config.pt_clut), &(t_config.ui2_clut_sz));
#endif
		}
	
		t_config.ui2_num_widgets			= MAX_NUM_WIDGETS;
		t_config.ui2_key_first_repeat_delay = FIRST_KEY_REP_DELAY;
		t_config.ui2_key_repeat_delay		= NEXT_KEY_REP_DELAY;
		t_config.ui2_svctx_max_num			= MAX_NUM_SVC_CNTXS;
	#endif
	
		
#ifndef NO_FONT_INIT
		t_config.ui4_hori_dpi				= HORZ_DPI;
		t_config.ui4_vert_dpi				= VERT_DPI;
#endif
	
#ifndef NO_DM_INIT
		t_config.t_dm_init.ui4_automnt_cond_ns = 0;
		t_config.t_dm_init.pt_automnt_cond = NULL;
#endif
	
#ifndef NO_WGL_INIT
		t_config.ui1_max_floated_widget = 10;
#endif
	
#ifndef NO_EDB_INIT
#ifdef MW_EDB_SUPPORT
		t_config.z_edb_mem_size = 12*1024*1024;//Default 12M
#endif
#endif
#ifdef WGL_ANIM_SUPPORT
		t_config.b_support_compose_anim = TRUE;
#endif
	
		/* Setup driver customization */
		init_drv_custom( &t_config.t_drv_config );
		t_g_drv_config = t_config.t_drv_config;
	
		x_start (&t_config);
	
#ifdef SYS_FAST_BOOT
	//	  init_post_routie();
#endif
		
}
