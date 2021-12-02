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
 * $RCSfile: extmjc_if.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

#ifndef _DRV_EXTMJC_H_
#define _DRV_EXTMJC_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define DRV_EXTMJCR_OK                       ((INT32)     0)
#define DRV_EXTMJCR_INV_ARG                  ((INT32)     -1)
#define DRV_EXTMJCR_TX_FAIL                  ((INT32)     -2)
#define DRV_EXTMJCR_INV_RESP                 ((INT32)     -3)
#define DRV_EXTMJCR_REG_FAIL                 ((INT32)     -4)
#define DRV_EXTMJCR_CONNECT_NOT_READY        ((INT32)     -5)

#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_RED_OFFSET11	 0
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_GREEN_OFFSET11	 1
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_BLUE_OFFSET11	 2
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_RED_GAIN1		 3
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_GREEN_GAIN1      4
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_BLUE_GAIN1		 5
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_RED_OFFSET12	 6
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_GREEN_OFFSET12	 7
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_BLUE_OFFSET12	 8
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_RED_GAIN2		 9
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_GREEN_GAIN2		 10
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_BLUE_GAIN2		 11
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_RED_OFFSET2		 12
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_GREEN_OFFSET2	 13
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_BLUE_OFFSET2	 14
#define DRV_EXTMJC_COLOR_TEMPERATURE_EX_IDX_BYPASS           0xFF


#define DRV_EXTMJC_GAMMA_OFF                    0
#define DRV_EXTMJC_GAMMA_DARK                   1
#define DRV_EXTMJC_GAMMA_MIDDLE                 2
#define DRV_EXTMJC_GAMMA_BRIGHT                 3

#define DRV_EXTMJC_LVDS_MODE_NORMAL             0
#define DRV_EXTMJC_LVDS_MODE_YUV                1
#define DRV_EXTMJC_LVDS_MODE_RGB                2

#define DRV_EXTMJC_VIDEO_FMT_COLOR_DOMAIN_YUV   1
#define DRV_EXTMJC_VIDEO_FMT_COLOR_DOMAIN_RGB   2

#define DRV_EXTMJC_PIP_MODE_NORMAL              0
#define DRV_EXTMJC_PIP_MODE_PIP                 1
#define DRV_EXTMJC_PIP_MODE_POP                 2

#define DRV_EXTMJC_OSD_MODE_NORMAL              0
#define DRV_EXTMJC_OSD_MODE_DMA                 1
#define DRV_EXTMJC_OSD_MODE_GRAPHIC             2

#define DRV_EXTMJC_MODE_OFF                     0
#define DRV_EXTMJC_MODE_MJC                     1
#define DRV_EXTMJC_MODE_DNM                     2

#define DRV_EXTMJC_VCR_MODE_ON                  1
#define DRV_EXTMJC_VCR_MODE_OFF                 2

#define DRV_EXTMJC_INPUT_SOURCE_TYPE_TV         0
#define DRV_EXTMJC_INPUT_SOURCE_TYPE_AV         1
#define DRV_EXTMJC_INPUT_SOURCE_TYPE_SV         2
#define DRV_EXTMJC_INPUT_SOURCE_TYPE_YP         3       
#define DRV_EXTMJC_INPUT_SOURCE_TYPE_VGA        4
#define DRV_EXTMJC_INPUT_SOURCE_TYPE_DVI        5
#define DRV_EXTMJC_INPUT_SOURCE_TYPE_MPEG       6

#define DRV_CUSTOM_EXTMJCR_OK                                   ((INT32)    0)    /**<        */
#define DRV_CUSTOM_EXTMJCR_INV_ARG                              ((INT32)   -1)    /**<        */
#define DRV_CUSTOM_EXTMJCR_TX_FAIL                              ((INT32)   -2)    /**<        */
#define DRV_CUSTOM_EXTMJCR_INV_RESP                             ((INT32)   -3)    /**<        */
#define DRV_CUSTOM_EXTMJCR_REG_FAIL                             ((INT32)   -4)    /**<        */
#define DRV_CUSTOM_EXTMJCR_CONNECT_NOT_READY                    ((INT32)   -5)    /**<        */

#define DRV_CUSTOM_EXTMJC_OSD_MODE_NORMAL                       ((UINT8)    0)    /**<        */
#define DRV_CUSTOM_EXTMJC_OSD_MODE_DMA                          ((UINT8)    1)    /**<        */
#define DRV_CUSTOM_EXTMJC_OSD_MODE_GRAPHIC                      ((UINT8)    2)    /**<        */

#define DRV_CUSTOM_EXTMJC_DEMO_MODE_NORMAL                      ((UINT8)    0)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_LEFT_MJC                    ((UINT8)    1)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_RIGHT_MJC                   ((UINT8)    2)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_BOTH_MJC                    ((UINT8)    3)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_LEFT_DNM                    ((UINT8)    4)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_RIGHT_DNM                   ((UINT8)    5)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_BOTH_DNM                    ((UINT8)    6)    /**<        */
#define DRV_CUSTOM_EXTMJC_DEMO_MODE_DISABLE                     ((UINT8)    7)    /**<        */

#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_OFF                    ((UINT8)    0)    /**<        */
#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_LOW                    ((UINT8)    1)    /**<        */
#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_MIDDLE                 ((UINT8)    2)    /**<        */
#define DRV_CUSTOM_EXTMJC_SETTING_EFFECT_HIGH                   ((UINT8)    3)    /**<        */

#define DRV_CUSTOM_EXTMJC_SETTING_60                            ((UINT8)    0)    /**<        */
#define DRV_CUSTOM_EXTMJC_SETTING_120                           ((UINT8)    1)    /**<        */

#define DRV_CUSTOM_EXTMJC_SETTING_DEMO_OFF                      ((UINT8)    0)    /**<        */
#define DRV_CUSTOM_EXTMJC_SETTING_DEMO_RIGHT                    ((UINT8)    1)    /**<        */
#define DRV_CUSTOM_EXTMJC_SETTING_DEMO_LEFT                     ((UINT8)    2)    /**<        */

/* message */
#define EXTMJC_MSG_TYPE_TX_EMPTY    0
#define EXTMJC_MSG_TYPE_TX_RETRY    1
#define EXTMJC_MSG_TYPE_PING        2
#define EXTMJC_MSG_TYPE_PING_FAIL   3

typedef struct _EXTMJC_MSG_T
{
    /* */
    UINT8                           ui1_msg_type;
    VOID*                           pt_obj;
} EXTMJC_MSG_T;
typedef struct DRV_EXTMJC_PIP
{
    UINT8       ui1_pip_mode;

    BOOL        b_main;
    
    UINT16      ui2_vid_left_top_x;
    UINT16      ui2_vid_left_top_y;
    UINT16      ui2_vid_right_bottom_x;
    UINT16      ui2_vid_right_bottom_y;
} DRV_EXTMJC_PIP_T;

typedef struct DRV_EXTMJC_GAMMA_TEST
{
    BOOL        b_onoff;
    UINT8       ui1_red;
    UINT8       ui1_green;
    UINT8       ui1_blue;
} DRV_EXTMJC_GAMMA_TEST_T;


/*----------------------------------------------------------------------------*/
/*! @struct DRV_CUSTOM_EXTMJC_VERSION_T
 *  @brief  This structure contains the external MJC version information
 *
 *  @code
 *  typedef struct DRV_CUSTOM_EXTMJC_VERSION
 *  {
 *      UINT8       ui1_size;
 *      UINT8       ui1_len;
 *      UINT8*      pui1_version;
 *  } DRV_CUSTOM_EXTMJC_VERSION_T;
 *  @endcode
 *
 *  @li@c  ui1_size     -
 *  @li@c  ui1_len      -
 *  @li@c  pui1_version -
 */
/*----------------------------------------------------------------------------*/
typedef struct DRV_CUSTOM_EXTMJC_VERSION
{
    UINT8       ui1_size;
    UINT8       ui1_len;
    UINT8*      pui1_version;
} DRV_CUSTOM_EXTMJC_VERSION_T;

/*----------------------------------------------------------------------------*/
/*! @struct DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T
 *  @brief  This structure contains the external MJC window postion information
 *
 *  @code
 *  typedef struct DRV_CUSTOM_EXTMJC_WINDOW_POSITION
 *  {
 *      UINT16      ui2_left_top_x;
 *      UINT16      ui2_left_top_y;
 *      UINT16      ui2_right_bottom_x;
 *      UINT16      ui2_right_bottom_y;
 *  } DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T;
 *  @endcode
 *
 *  @li@c  ui2_left_top_x                                    -
 *  @li@c  ui2_left_top_y                                    -
 *  @li@c  ui2_right_bottom_x                                -
 *  @li@c  ui2_right_bottom_y                                -
 */
/*----------------------------------------------------------------------------*/
typedef struct DRV_CUSTOM_EXTMJC_WINDOW_POSITION
{
    UINT16      ui2_left_top_x;
    UINT16      ui2_left_top_y;
    UINT16      ui2_right_bottom_x;
    UINT16      ui2_right_bottom_y;
} DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T;

/*----------------------------------------------------------------------------*/
/*! @enum  DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T
 *  @brief
 *
 *  @code
 *  typedef enum DRV_CUSTOM_EXTMJC_COLOR_PATTERN
 *  {
 *      DRV_CUSTOM_EXTMJC_COLOR_PATTERN_RED   = 0,
 *      DRV_CUSTOM_EXTMJC_COLOR_PATTERN_GREEN = 1,
 *      DRV_CUSTOM_EXTMJC_COLOR_PATTERN_BLUE  = 2,
 *      DRV_CUSTOM_EXTMJC_COLOR_PATTERN_WHITE = 3,
 *      DRV_CUSTOM_EXTMJC_COLOR_PATTERN_NONE  = 0xFF
 *  } DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T;
 *  @endcode
 *
 *  @li@c  DRV_CUSTOM_EXTMJC_COLOR_PATTERN_RED               -
 *  @li@c  DRV_CUSTOM_EXTMJC_COLOR_PATTERN_GREEN             -
 *  @li@c  DRV_CUSTOM_EXTMJC_COLOR_PATTERN_BLUE              -
 *  @li@c  DRV_CUSTOM_EXTMJC_COLOR_PATTERN_WHITE             -
 *  @li@c  DRV_CUSTOM_EXTMJC_COLOR_PATTERN_NONE              -
 */
/*----------------------------------------------------------------------------*/
typedef enum DRV_CUSTOM_EXTMJC_COLOR_PATTERN
{
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_RED   = 0,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_GREEN = 1,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_BLUE  = 2,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_WHITE = 3,
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_NONE  = 0xFF
} DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T;

extern UINT32 _u48282UiUnmteTimeout;


/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

extern INT32 drv_extmjc_init(VOID);
extern INT32 drv_extmjc_reset(VOID);
extern  UINT8 drv_extmjc_gamma_send(VOID);
extern INT32 extmjc_send_msg(EXTMJC_MSG_T* pt_msg);
extern UINT8 u1ExtmjcReadRegByI2C(
    UINT16 addr, 
    UINT8* pdata
    );

extern UINT8 u1ExtmjcWriteRegByI2C(
    UINT16 u2addr, 
    UINT32 u1data
    );

extern UINT16 u2ExtmjcWriteRegByI2CNBytes(UINT32 u4Reg, UINT8 *pData,UINT32 u4Size);

extern UINT32 u4ExtmjcGetSfCrcChecksum(UINT32 u4_size);

extern INT32 drv_extmjc_i2c_checksum_enable(BOOL flag);

extern INT32 drv_extmjc_i2c_mem_write(UINT32 u4SrcAddress,UINT32 u4DstAddress,UINT32 u4Size);
extern void drv_extmjc_gamma_write(UINT8 **pDark,UINT8 **pMiddle,
                                        UINT8 **pBright,UINT32 u4ChannelSize);
extern INT32 drv_extmjc_send_panel(UINT8 *pPanel,UINT32 len);

extern BOOL drv_extmjc_is_enable(VOID);

extern INT32 drv_extmjc_set_mjc_mode(
    UINT8       ui1_mjc_mode,
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_set_color_temperature_ex(
    UINT8       ui1_path,
    UINT8       ui1_index,
    UINT16      ui2_value
    );

extern INT32 drv_extmjc_set_gamma(
    UINT8       ui1_gamma,
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_set_video_fmt(
    UINT8       ui1_lvds_mode,
    UINT8       ui1_refresh_rate,
    UINT16      ui2_h_total,
    UINT16      ui2_v_total,
    UINT8       ui1_mjc_on_off,
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_set_lvds_mode(
    UINT8       ui1_lvds_mode,
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_set_pip(
    DRV_EXTMJC_PIP_T*   pt_pip,
    UINT8*              pui1_msg_error
    );


extern INT32 drv_extmjc_set_ping(
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_set_RX_reset(
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_set_lvds_enable(
    BOOL                b_enable,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_set_osd_turn_on(VOID);

extern INT32 drv_extmjc_set_osd_enable(
    BOOL                b_osd_enable,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_set_osd_mode(
    UINT8               ui1_osd_mode,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_set_osd_info(
    BOOL                b_enable_scaler,
    UINT16              ui2_osd_width,
    UINT16              ui2_osd_height,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_reset_rx_tx(
    UINT32 u4resetCnt,
    UINT8* pui1_msg_error
    );
    
extern INT32 drv_extmjc_set_vcr_mode(
    UINT8               ui1_vcr_mode,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_set_input_source_type(
    UINT8               ui1_input_source_type,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_sram_read(
    UINT16              ui2_sram_addr,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_sram_write(
    UINT16                                  ui2_sram_addr,
    UINT32                                  ui4_sram_data,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_get_dram(
    UINT32                                  ui4_addr,
    UINT32*                                 pui4_data,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_dram(
    UINT32                                  ui4_addr,
    UINT32                                  ui4_data,
    UINT8*                                  pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_fastmute(
    BOOL              ui1_on_off,
    UINT8*              pui1_msg_error
    );
    
INT32 d_custom_extmjc_set_fastmute_timeout(
    UINT32             Counter,
    UINT8*              pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_mjc_setting(
    UINT8       ui1_mjc_effect,
    UINT8       ui1_60_120,
    UINT8       ui1_demo_mode,
    UINT8*      pui1_msg_error
    );

INT32 d_custom_extmjc_set_mute(
    BOOL                ui1_on_off,
    UINT8*              pui1_msg_error
    );

extern INT32 d_custom_extmjc_set_osd_mode(
    UINT8               ui1_osd_mode,
    UINT8*              pui1_msg_error
    );

extern INT32 drv_extmjc_sflash_ProtDis(
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_sflash_ProtEn(
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_i2c_access_enable_password(
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_i2c_access_disable_password(
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_cpu_run(
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_cpu_stop(
    UINT8*      pui1_msg_error
    );

extern INT32 drv_extmjc_i2c_accessable_group(
    UINT16      ui2_accessable_group,
    UINT8*      pui1_msg_error
    );

void vDrvExtmjcRedoTwoChipConnection(UINT8 u1Path,BOOL fgEnable);
void vDrvExtmjcNormProc(void);
void vDrvExtmjcSetMute(UINT32 u4Count);

// merged from u_drv_cust.h
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern BOOL d_custom_extmjc_is_enable(
    VOID
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern VOID d_custom_extmjc_enable_alive_check(
    BOOL                           b_enable
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern BOOL d_custom_extmjc_get_alive_check(
    VOID
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_get_version(
    UINT8*                         pui1_msg_error,
    DRV_CUSTOM_EXTMJC_VERSION_T*   pt_extmjc_version
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
/*
extern INT32 d_custom_extmjc_set_mjc_setting(
    UINT8       ui1_mjc_effect,
    UINT8       ui1_60_120,
    UINT8       ui1_demo_mode,
    UINT8*      pui1_msg_error
);
*/
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
/*
extern INT32 d_custom_extmjc_set_osd_mode(
    UINT8                                   ui1_osd_mode,
    UINT8*                                  pui1_msg_error
    );
*/
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_set_window_position(
    DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T*    pat_window_position,
    UINT8                                   ui1_window_num,
    UINT8*                                  pui1_msg_error
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_set_color_pattern(
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T       t_color_pattern,
    UINT8*                                  pui1_msg_error
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_get_gpio(
    UINT8     ui1_gpio_id,
    BOOL*     pb_onoff,
    UINT8*    pui1_msg_error
    );

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_set_gpio(
    UINT8     ui1_gpio_id,
    BOOL      b_onoff,
    UINT8*    pui1_msg_error
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
/*
extern INT32 d_custom_extmjc_get_dram(
    UINT32     ui4_addr,
    UINT32*    pui4_data,
    UINT8*     pui1_msg_error
);
*/

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
/*
extern INT32 d_custom_extmjc_set_dram(
    UINT32    ui4_addr,
    UINT32    ui4_data,
    UINT8*    pui1_msg_error
);
*/
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_set_demo_mode(
    UINT8     ui1_demo_mode,
    UINT16    ui2_middle_line_pos,
    UINT8*    pui1_msg_error
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_upgrade_open(
    VOID
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_upgrade_erase(
    VOID
);

/* The maximun data size is 128 */
/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_upgrade_write(
    UINT32    ui4_offset,
    UINT8*    pui1_data,
    UINT32    ui4_data_size
);

/*----------------------------------------------------------------------------*/
/*! @brief
 *  @param [in]
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*----------------------------------------------------------------------------*/
extern INT32 d_custom_extmjc_upgrade_close(
    VOID
);


#endif /* _DRV_EXTMJC_H_ */
