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
 * $RCSfile: extmjc_msg_hdlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC
#include "x_typedef.h"
#include "x_assert.h"
#include "x_gpio.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"
#include "osd_drvif.h"
#include "nptv_if.h"
#include "sif_if.h"
#include "extmjc_if.h"
#include "extmjc_msg_hdlr.h"
#include "u_drv_cust.h"
#include "extmjc_debug.h"
#include "drv_lvds.h"
#include "srm_drvif.h"

#ifdef CC_MTK_LOADER //defined(CC_5360_LOADER) ||defined(CC_5391_LOADER)
#define CC_53XX_LOADER
#define LOG(x,fmt...) Printf(fmt);
#ifdef x_crit_start
#undef x_crit_start
#define x_crit_start() 0
#endif
#ifdef x_crit_end
#undef x_crit_end
#define x_crit_end(x) UNUSED(x)
#endif
#define CRIT_STATE_T UINT8
#else
#include "x_os.h"
#endif

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
/* Message Type Sync with MT8280 extmjc_msg.h */
#define CONFIG_MT8281 1
#define CHIP_MT8530 0

enum{
#if CONFIG_MT8281
EXTMJC_MSG_SET_MJC_UI                       = 0x6B,
EXTMJC_MSG_SET_MJC_BYPASS_WINDOW_DIRECTLY   = 0x42,
EXTMJC_MSG_MJC_SET_SPREAD_SPECTRUM          = 0x75,
EXTMJC_MSG_MJC_LVDS_ON_DELAY                = 0x76,


#if CHIP_MT8530
EXTMJC_MSG_MT8530_MJC_ON_OFF                = 0x71,
EXTMJC_MSG_MT8530_MJC_VIDEO_FMT             = 0x72,
EXTMJC_MSG_MT8530_MJC_EFFECT                = 0x73,
EXTMJC_MSG_MT8530_MJC_GET_INPUT_STATUS      = 0x74,
#endif
#else
EXTMJC_MSG_SET_MJC_SETTING_EX               = 0x22,
EXTMJC_MSG_SET_MJC_DEMO                     = 0x68,
#endif
EXTMJC_MSG_GET_MJC_VERSION                  = 0x05,
EXTMJC_MSG_SET_BLUE_STRETCH                 = 0x10,
EXTMJC_MSG_SET_COLOR_TEMPERATURE            = 0x14,
EXTMJC_MSG_GET_GAMMA_SETTING                = 0x15,
EXTMJC_MSG_SET_GAMMA_SETTING                = 0x16,
EXTMJC_MSG_SET_GAMMA_ARRAY                  = 0x17,
EXTMJC_MSG_SET_COLOR_TEMPERATURE_EX         = 0x1A,
EXTMJC_MSG_SET_LVDS_MUTE                    = 0x20,
EXTMJC_MSG_RESET_LVDS_RX			        = 0x21,
EXTMJC_MSG_SET_VCR_MODE                     = 0x26,
EXTMJC_MSG_SET_INPUT_SOURCE_TYPE            = 0x28,
EXTMJC_MSG_RX_TX_RESET                      = 0x44,
EXTMJC_MSG_SET_OSD_ENABLE                   = 0x48,
EXTMJC_MSG_SET_OSD_MODE                     = 0x4A,
EXTMJC_MSG_SET_OSD_INFO                     = 0x4C,
EXTMJC_MSG_SET_OSD_FADING                   = 0x4D,
EXTMJC_MSG_GET_VIDEO_FMT                    = 0x51,
EXTMJC_MSG_SET_VIDEO_FMT                    = 0x52,
EXTMJC_MSG_FIX_LVDS_MODE                    = 0x53,
EXTMJC_MSG_GET_VIDEO_OVERSCAN               = 0x54,
EXTMJC_MSG_SET_VIDEO_OVERSCAN               = 0x55,
EXTMJC_MSG_SET_3D_FMT                       = 0x56,
EXTMJC_MSG_SET_SRC_TIMING                   = 0x57,
EXTMJC_MSG_SET_PWM                          = 0x58,
EXTMJC_MSG_MJC_COLOR_TEST_PATTERN           = 0x63,
EXTMJC_MSG_MJC_SET_GPIO_TEST                = 0x65,
EXTMJC_MSG_MJC_GET_DRAM_TEST                = 0x66,
EXTMJC_MSG_MJC_SET_DRAM_TEST                = 0x67,
EXTMJC_MSG_MJC_SET_SRAM                     = 0x69,
EXTMJC_MSG_MJC_GET_SRAM                     = 0x6A,
EXTMJC_MSG_SET_MODE_CHANGE                  = 0x6C,
EXTMJC_MSG_GET_MJC_DELAY                    = 0x6D,
EXTMJC_MSG_GET_MJC_MIRRORFLIP               = 0x6E,
EXTMJC_MSG_SET_MJC_FRZ						= 0x6F,

EXTMJC_MSG_SET_CSHD_SBM                     = 0x70,

EXTMJC_MSG_SET_ALL_MUTE                     = 0x77,
EXTMJC_MSG_SET_PANEL_POWERSEQ               = 0x78,


EXTMJC_MSG_CUSTOM_CMD                       = 0x80
};


#define EXTMJC_POWER_STATUS_OFF                     0x00
#define EXTMJC_POWER_STATUS_ON                      0x01


#define EXTMJC_PING_TIMER_PERIOD                    2000 //ms
//#define DRV_SUPPORT_EXTMJC_NORMAL_OSD 1
#if CONFIG_MT8281
//#define EXTMJCTRI 	0x200
//#define EXTMJCMSG 	0x201
#define EXTMJCP0 	0x202
#define EXTMJCP1 	0x203
#define EXTMJCP2 	0x204
#define EXTMJCP3 	0x205
#define EXTMJCP4 	0x206
#define EXTMJCP5 	0x207
#define EXTMJCP6 	0x208
#define EXTMJCP7 	0x209
#define EXTMJCP8 	0x20a
#define EXTMJCP9 	0x20b
#define EXTMJC_PING     0x20c

#define EXTMJCTRI 	0x80e0
#define EXTMJCMSG 	0x80e4
#define EXTMJCPARA0 	0x80e4
#define EXTMJCPARA1 	0x80e8
#define EXTMJCPARA2 	0x80ec
#define ETXTMJCPASS0		0x81c0
#define ETXTMJCPASS1		0x81c4
#define ETXTMJCPASSCFG		0x81c8
//Copy from dram.map
/* 8032 ICU Mode Dram */
#define UP_DRAM_BASE 0x0
#define UP_DRAM_SIZE 0x40000 // 256K // allocate it to be overwritten by hw blocks
#define XDATA_CACHE_DRAM_BASE (UP_DRAM_BASE + UP_DRAM_SIZE)
#define XDATA_CACHE_DRAM_SIZE 0x40000

//OSD
#define OSD_DRAM_BASE (XDATA_CACHE_DRAM_BASE + XDATA_CACHE_DRAM_SIZE) //0x80000
#define OSD_DRAM_HEADER0	 OSD_DRAM_BASE						//0x80000
#define OSD_DRAM_HEADER1	 OSD_DRAM_HEADER0	+ 0x400			//0x80400
#define OSD_DRAM_PALETTE0	 OSD_DRAM_HEADER1	+ 0x400			//0x80800
#define OSD_DRAM_PALETTE1	 OSD_DRAM_PALETTE0	+ 0x400			//0x80c00
#define OSD_DRAM_PALETTE2	 OSD_DRAM_PALETTE1	+ 0x400			//0x81000
#define OSD_DRAM_PALETTE3	 OSD_DRAM_PALETTE2	+ 0x400			//0x81400
#define OSD_DRAM_BITMAP0	 OSD_DRAM_PALETTE3	+ 0xc00			//0x82000
#define OSD_DRAM_BITMAP1	 OSD_DRAM_BITMAP0	+ 0x1FE000		//0x280000
#define OSD_DRAM_BITMAP2	 OSD_DRAM_BITMAP1	+ 0x1FE000		//0x47E000
#define OSD_DRAM_SIZE 0x60e000 //0x410000				// (4096 + 64) x 1024

// 0x10000 space for usage
/* DTV rs232 driver DRAM */
#define DTV_RS232_DRAM_BASE (OSD_DRAM_BASE + OSD_DRAM_SIZE) //0x68E000
#define DTV_RS232_DRAM_SIZE 0x400 // 1K

#define GAMMA_DRAM_BASE	(DTV_RS232_DRAM_BASE + DTV_RS232_DRAM_SIZE) //0x68E400
#define GAMMA_DRAM_SIZE	0x2400									//(1024 x 3 = 3072 Bytes) x 3 [bright/middle/dark]
#define GAMMA_DRAM_DEC_BASE	(GAMMA_DRAM_BASE + GAMMA_DRAM_SIZE) //0x690800
#define GAMMA_DRAM_DEC_SIZE	0x800							    // 1024 x 2byte [decompressed gamma table]

#define PANEL_DRAM_BASE	(GAMMA_DRAM_BASE + GAMMA_DRAM_SIZE + GAMMA_DRAM_DEC_SIZE) //0x6910000
#define PANEL_DRAM_SIZE	0x400									// 1KB

#else
#define EXTMJCTRI 0xff
#define EXTMJCMSG 0x51
#define EXTMJCP0 0x52
#define EXTMJCP1 0x53
#define EXTMJCP2 0x54
#define EXTMJCP3 0x56
#define EXTMJCP4 0x108
#define EXTMJCP5 0x109
#define EXTMJCP6 0x10a
#define EXTMJCP7 0x10b
#define EXTMJCP8 0x10c
#define EXTMJCP9 0x10d
#endif

#define EXTMJCDO 0x10
#define EXTMJCACK 0x20
#define EXTMJCPING 0x40
//#define MT8280_REG_OSD_DMA_B0               0x70B0
#define MT8280_REG_OSD_DMA_C0               0x70C0
#define MT8280_REG_OSD_DMA_C2               0x70C2
#define MT8280_REG_OSD_DMA_C4               0x70C4
#define MT8280_REG_OSD_DMA_C5               0x70C5

#define MT8280_REG_DMA_FIFO_RESET           0x70C4
#define MT8280_REG_I2C_DMA                  0x70C5
#define MT8280_MEM_PANEL_ADDRESS            0x
#if CONFIG_MT8281
#define MT8280_REG_DMA_ADDRESS              0x70B4
#else
#define MT8280_REG_DMA_ADDRESS              0x70C8
#endif

#define MT8280_REG_OSD_DMA_B0               0x2660

typedef struct _EXTMJC_STATUS_T
{
    /* power status */
    UINT8               ui1_power_status;

    /* mjc */
    UINT8               ui1_mjc_mode;

    /* color temperature ex */
    UINT16              aui2_color_temp_ex[15];
    BOOL                b_color_temp_ex_pass;

    /* gamma */
    BOOL                b_init_gamma;
    UINT8               ui1_gamma;

    /* OSD */
    BOOL                b_osd_enable;

    BOOL                b_init_osd_info;
    BOOL                b_osd_enable_scaler;
    UINT16              ui2_osd_width;
    UINT16              ui2_osd_height;

    /* LVDS */
    BOOL                b_lvds_enable;

    /* PIP status */
    UINT8               ui1_pip_mode;

    BOOL                b_set_main_pos;
    UINT16              ui2_main_vid_left_top_x;
    UINT16              ui2_main_vid_left_top_y;
    UINT16              ui2_main_vid_right_bottom_x;
    UINT16              ui2_main_vid_right_bottom_y;

    BOOL                b_set_sub_pos;
    UINT16              ui2_sub_vid_left_top_x;
    UINT16              ui2_sub_vid_left_top_y;
    UINT16              ui2_sub_vid_right_bottom_x;
    UINT16              ui2_sub_vid_right_bottom_y;

} EXTMJC_STATUS_T;

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
HANDLE_T                        gh_extmjc_req_wait_resp_lock = NULL_HANDLE;
HANDLE_T                        gh_extmjc_req_lock = NULL_HANDLE;
BOOL                            gb_extmjc_req_success;
EXTMJC_STATUS_T                 gt_extmjc_status;


UINT8                           gui1_extmjc_resp_id = 0xFF;
UINT8                           gui1_extmjc_resp_type;
UINT16                          gui2_extmjc_resp_body_len;
UINT8                           gaui1_extmjc_resp_body[50];

//BOOL                            gb_extmjc_enable_alive_check = FALSE;
BOOL                            gb_extmjc_enable_alive_check = TRUE;
UINT32                          gui4_extmjc_ping_failed_cnt = 0;
UINT32                          gui4_extmjc_ping_okay_cnt = 0;

UINT32                          _u48282UiUnmteTimeout = 0;

#define EXTMJC_I2C_CHECKSUM_ENABLE 0
#if EXTMJC_I2C_CHECKSUM_ENABLE
BOOL                            gui1_extmjc_checksum_enable = FALSE;
#endif

#ifndef CC_53XX_LOADER
HANDLE_T                        gh_extmjc_ping_timer = NULL_HANDLE;
HANDLE_T                        gh_extmjc_cmd_bin_sema = NULL_HANDLE;
HANDLE_T                        gh_extmjc_cmd_gpio_ack_bin_sema = NULL_HANDLE;
BOOL                            gb_extmjc_cmd_gpio_ack_bin_sema = FALSE;
#define EXTMJC_SEMA_LOCK(x) \
do { \
   VERIFY (x_sema_lock(x, X_SEMA_OPTION_WAIT) == OSR_OK); \
}while(0)

#define EXTMJC_SEMA_LOCK_TIMEOUT(x,t) (x_sema_lock_timeout(x, t))

#define EXTMJC_SEMA_UNLOCK(x) \
do { \
   VERIFY (x_sema_unlock(x) == OSR_OK); \
}while(0)

#else
#define EXTMJC_SEMA_LOCK(x)
#define EXTMJC_SEMA_LOCK_TIMEOUT(x,t)
#define EXTMJC_SEMA_UNLOCK(x)
#endif


/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/

#if 0
static VOID extmjc_send_common_setting(VOID);
#endif

INT32 drv_extmjc_wait_cmd_ready(BOOL b_unlock_sema);
//static VOID extmjc_send_color_temp_setting(VOID);
//static void drv_extmjc_fix_lvds_mode(BOOL b_enable);



/*-----------------------------------------------------------------------------
 * extern methods declarations
 *---------------------------------------------------------------------------*/
UINT8 u1ExtmjcReadRegByI2C(UINT16 addr, UINT8* pdata)
{
    UINT8 u1Dev;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    INT32 i4ReturnValue;
    UINT8 u1AddrNum;
    UINT8 u1FailCount;
    static UINT32 u4ContFailCount = 0;

    u4ClkDiv = 0x100;
    u1Dev = 0x0c;
    u1AddrNum = 0x02;
    u4Addr = addr;
    //u4Count = 0x01;
    u4Count = 0x04;

    if( (addr & 0x0003) !=0 )
    {
    	Printf("!!!extmjc I2C error: address not align: 0x%x!\n",addr);
    	return 0;
    }

    for(u1FailCount=0; u1FailCount<10;u1FailCount++)
    {
        //i4ReturnValue = SIF_ReadMultipleSubAddr(u4ClkDiv, u1Dev, u1AddrNum , u4Addr, pdata, u4Count);
        i4ReturnValue = SIF_X_Read(EXTMJC_I2C_BUS, u4ClkDiv, u1Dev, u1AddrNum , u4Addr, pdata, u4Count);
        if (i4ReturnValue > 0)
            break;
        if ((u4ContFailCount % 1000)==0)
            Printf("Extmjc:SIF_Read 0x%04x Failed u4ContFailCount=%d\n",u4Addr,u4ContFailCount);
        u4ContFailCount++;
        if (u4ContFailCount>10) //due to continous failed, do not retry
            break;
        HAL_Delay_us(10);
    }

    if (i4ReturnValue>0)
    {
        u4ContFailCount = 0;
        return 0;
    }
    else
    {
            // fail.
        return 1;
    }

}

//-----------------------------------------------------------------------------
/** u1ExtmjcWriteRegByI2C
 *
 *
 *  @param  u2addr    [IN]
 *             EXTMJC subaddress
 *  @param  u1data    [IN]
 *             EXTMJC data
 *  @retval 0  Succeed
 *  @retval others           Fail
 */
//-----------------------------------------------------------------------------
UINT8 u1ExtmjcWriteRegByI2C(UINT16 u2addr, UINT32 u1data)
{
    // refer to _SifWriteMultiCmd
    UINT8  u1Dev;
    UINT32 u4ClkDiv;
    UINT32 u4Addr;
    UINT32 u4Count;
    INT32 i4ReturnValue;
    UINT8 u1AddrNum;
    UINT8 u1FailCount;
    #if EXTMJC_I2C_CHECKSUM_ENABLE
    UINT8 u1Buf[2];
    #else
    UINT32 u4Buf;
    #endif
    static UINT32 u4ContFailCount = 0;

    if( (u2addr & 0x0003) !=0 )
    {
    	Printf("!!!extmjc I2C error: address not align: 0x%x!\n",u2addr);
    	return 1;
    }

    u4ClkDiv = 0x100;
    u1Dev = 0x0c;
    u1AddrNum = 0x02;
    u4Addr = u2addr;
#if EXTMJC_I2C_CHECKSUM_ENABLE
    if (gui1_extmjc_checksum_enable == TRUE)
    {
        u1Buf[0] = u1data;
        u1Buf[1] = ((u2addr>>8) & 0xff) ^ (u2addr & 0xff) ^ u1Buf[0];
        u4Count = 0x02;
    }
    else
    {
        u1Buf[0] = u1data;
        u4Count = 0x01;
    }
#else
    //u1Buf[0] = u1data;
    //u4Count = 0x01;
    u4Buf = u1data;
    u4Count = 0x04;
#endif

    // printf("write reg by i2c add:0x%02x data:0x%02x\n", u2addr, u1data);
    for(u1FailCount=0; u1FailCount<10;u1FailCount++)
    {
        //i4ReturnValue = SIF_WriteMultipleSubAddr(u4ClkDiv, u1Dev, u1AddrNum ,u4Addr, (UINT8 *)u1Buf, u4Count);
        #if EXTMJC_I2C_CHECKSUM_ENABLE
        i4ReturnValue = SIF_X_Write(EXTMJC_I2C_BUS, u4ClkDiv, u1Dev, u1AddrNum ,u4Addr, (UINT8 *)u1Buf, u4Count);
        #else
        i4ReturnValue = SIF_X_Write(EXTMJC_I2C_BUS, u4ClkDiv, u1Dev, u1AddrNum ,u4Addr, (UINT8 *)(&u4Buf), u4Count);
        #endif
        if (i4ReturnValue == u4Count)
            break;
        if ((u4ContFailCount % 1000)==0)
            Printf("Extmjc:SIF_Write 0x%04x Failed u1FailCount=%08d i4ReturnValue=%d u4Count=%d\n",u4Addr,u4ContFailCount,i4ReturnValue,u4Count);
        u4ContFailCount++;
        if (u4ContFailCount>10) //due to continous failed, do not retry
            break;
        HAL_Delay_us(10);
    }
    if (i4ReturnValue == u4Count)
    {
        u4ContFailCount = 0;
        return 0;
    }
    else
    {
        // fail
        return 1;
    }

}

// Send command to 8283 through I2C
// u1Msg : msg type
// pPara : pointer to parameters of msg
// u1Size : size of pPara in bytes
UINT8 u1ExtmjcSendI2CMsg(UINT8 u1Msg,UINT8* pPara,UINT8 u1Size)
{
	UINT32 u4ParaBuf[3]={0,0,0};
	UINT8* pBuf;

	if(pPara == NULL)
	{
		Printf("Extmjc error: input para null!\n");
		return 1;
	}
	if( (u1Size>10)||(u1Size==0))
	{
		Printf("Extmjc error: para size incorrect:%d!\n",u1Size);
		return 1;
	}

	pBuf = (UINT8*)u4ParaBuf;
	*pBuf = u1Msg;
	
	pBuf ++;
	x_memcpy(pBuf, (const void *)pPara, u1Size);
	
	if(u1Size<4) // 1~3 parameters
	{
		u1ExtmjcWriteRegByI2C(EXTMJCMSG,u4ParaBuf[0]);
	}
	else if(u1Size<8) // 4~7parameters
	{
		u1ExtmjcWriteRegByI2C(EXTMJCMSG,u4ParaBuf[0]);
		u1ExtmjcWriteRegByI2C(EXTMJCPARA1,u4ParaBuf[1]);
	}
	else // 8~10parameters 
	{
		u1ExtmjcWriteRegByI2C(EXTMJCMSG,u4ParaBuf[0]);
		u1ExtmjcWriteRegByI2C(EXTMJCPARA1,u4ParaBuf[1]);
		u1ExtmjcWriteRegByI2C(EXTMJCPARA2,u4ParaBuf[2]);
	}

	return 0;
}

UINT16 u2ExtmjcWriteRegByI2CNBytes(UINT32 u4Reg, UINT8 *pData,UINT32 u4Size)
{
    UINT16 len;
#if EXTMJC_I2C_CHECKSUM_ENABLE
    UINT32 i;

    if (gui1_extmjc_checksum_enable == TRUE)
    {
        for (i=0;i<u4Size;i++)
        {
            u1ExtmjcWriteRegByI2C(u4Reg,pData[i]);
        }
        len = u4Size;
    }
    else
    {
        //len = SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, u4Reg, pData, u4Size);
        len = SIF_X_Write(EXTMJC_I2C_BUS, 0x100, 0xc, 0x2, u4Reg, pData, u4Size);
    }
#else
    //len = SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, u4Reg, pData, u4Size);
    len = SIF_X_Write(EXTMJC_I2C_BUS, 0x100, 0xc, 0x2, u4Reg, pData, u4Size);
#endif
//    Printf("u2ExtmjcWriteRegByI2CNBytes:size=%d written=%d\n",u4Size,len);
    return len;
}

UINT8 u1ExtmjcWriteRegByI2C4Bytes(UINT32 u4Reg, UINT32 u4Data)
{
    if (u1ExtmjcWriteRegByI2C(u4Reg + 3, ((u4Data>>24) & 0x00ff))>0)
        return 1;
    if (u1ExtmjcWriteRegByI2C(u4Reg + 2, ((u4Data>>16) & 0x00ff))>0)
        return 1;
    if (u1ExtmjcWriteRegByI2C(u4Reg + 1, ((u4Data>>8) & 0x00ff))>0)
        return 1;
    if (u1ExtmjcWriteRegByI2C(u4Reg + 0, (u4Data & 0x00ff))>0)
        return 1;

    return 0;
}

INT32 drv_extmjc_i2c_checksum_enable(BOOL flag)
{
#if EXTMJC_I2C_CHECKSUM_ENABLE
    if (flag)
    {
        //enable checksum
        u1ExtmjcWriteRegByI2C(0x11a,0x11);
        gui1_extmjc_checksum_enable = TRUE;
        u1ExtmjcWriteRegByI2C(0x11a,0x01);
    }
    else
    {
        //disable checksum
        u1ExtmjcWriteRegByI2C(0x11a,0x00);
        gui1_extmjc_checksum_enable = FALSE;
    }
#endif
    return (DRV_EXTMJCR_OK);
}

INT32 drv_extmjc_reset(VOID)
{
    INT32 i4Gpio, i4GpioPolarity,i4LVDSGpio;
    UINT8 ui1_err;
    UINT32 ui4_I2cCount;
	UINT32  u4Data ;

    Printf("Extmjc Inited drv_extmjc_is_enable()=%d\n",drv_extmjc_is_enable());
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_EXTMJCR_OK);
    }

    i4Gpio = (INT32)DRVCUST_InitGet(eMt8280ResetGpio);
    i4GpioPolarity = (INT32)DRVCUST_InitGet(eMt8280ResetGpioPolarity);

    // Setup 8280
    if (i4Gpio != 0xFFFF)  // here needs to setup 8280.
    {
#ifndef CC_MT8281_SLT
	#ifdef DRV_SUPPORT_MT8283
	Printf("MT8283 init here!!\n");
	i4LVDSGpio = (INT32)DRVCUST_InitGet(eMt8283LVDSPowerGpio);
	GPIO_SetOut(i4LVDSGpio,1); //Power On MT8283
	#endif
        GPIO_SetOut(i4Gpio, i4GpioPolarity);
        HAL_Delay_us(100*1000);
        GPIO_SetOut(i4Gpio, !i4GpioPolarity);
        HAL_Delay_us(20*1000);
#else
        GPIO_SetOut(8,0); //Power On MT8282 5V
        GPIO_SetOut(6,0); //Power On MT8280_0 5V
        GPIO_SetOut(7,0); //Power On MT8280_1 5V

        GPIO_SetOut(i4Gpio, i4GpioPolarity);
        HAL_Delay_us(10*1000);
        GPIO_SetOut(i4Gpio, !i4GpioPolarity);  //Release Reset MT8282
        HAL_Delay_us(10*1000);
        GPIO_SetOut(0,1); //Release Reset MT8280_0
        GPIO_SetOut(1,1); //Release Reset MT8280_1
#endif

	#ifdef DRV_SUPPORT_MT8283
	//return (DRV_EXTMJCR_OK);
	#endif
    }
    Printf("Checking Extmjc Chip ready\n");
    for(ui4_I2cCount=0;ui4_I2cCount<30;ui4_I2cCount++) //dummy read,ensure MT8281 is ready
    {
		//if(u1ExtmjcWriteRegByI2C(ETXTMJCPASS0,0x12345678)==0)
		if(u1ExtmjcReadRegByI2C(ETXTMJCPASS0,(UINT8*)&u4Data)==0)
    		break;
    	Printf("EXTMJC FAILED I2C\n");
    	HAL_Delay_us(100*1000);
    }
    if (ui4_I2cCount>=30)
        Printf("Extmjc I2C read Failed\n");
    else
        Printf("Extmjc I2C enable count =%d\n",ui4_I2cCount);
    //enable I2C read/write
    drv_extmjc_i2c_access_enable_password(&ui1_err);
    drv_extmjc_i2c_accessable_group(0xffff,&ui1_err);
    Printf("Extmjc Inited\n");

    return (DRV_EXTMJCR_OK);
}

UINT32 u4ExtmjcGetSfCrcChecksum(UINT32 u4_size)
{
    UINT32 u4_src_address;
    UINT32 u4_length;
    UINT8 u1_data;
    UINT32 u4_ret;

    u1ExtmjcWriteRegByI2C(0x62b,0);
    u1ExtmjcWriteRegByI2C(0x62a,0);
    u1ExtmjcWriteRegByI2C(0x629,0);
    u1ExtmjcWriteRegByI2C(0x628,0);

    //enable callculate checksum
//    u1ExtmjcWriteRegByI2C(0x62e,0x40); //when uP read Flash
    u1ExtmjcWriteRegByI2C(0x62e,0x08);   //By designer Jerick suggest change to 0x08 Select DMA checksum
    u4_src_address = 0;
    while(u4_src_address<u4_size)
    {
        //DMA flash src address
        u1ExtmjcWriteRegByI2C(0x36,BYTE0(u4_src_address));
        u1ExtmjcWriteRegByI2C(0x37,BYTE1(u4_src_address));
        u1ExtmjcWriteRegByI2C(0x38,BYTE2(u4_src_address));

        //DMA dram dst address
        u1ExtmjcWriteRegByI2C(0x39,0);
        u1ExtmjcWriteRegByI2C(0x3a,0);
        u1ExtmjcWriteRegByI2C(0x3b,0);

        //DMA length
        u4_length = u4_size-u4_src_address;
        if (u4_length > 0x8000)
            u4_length=0x8000;
        u1ExtmjcWriteRegByI2C(0x3c,BYTE0(u4_length));
        u1ExtmjcWriteRegByI2C(0x3d,BYTE1(u4_length));

        //DMA trigger start
        u1ExtmjcWriteRegByI2C(0x33,0x40);
        while (1)
        {
            u1ExtmjcReadRegByI2C(0x33,&u1_data);
            if ((u1_data & 0x40) == 0)
                break;
        }
        u4_src_address+=u4_length;
    }
    u1ExtmjcReadRegByI2C(0x62b,&u1_data);
    u4_ret = u1_data;
    u1ExtmjcReadRegByI2C(0x62a,&u1_data);
    u4_ret = (u4_ret<<8) | u1_data;
    u1ExtmjcReadRegByI2C(0x629,&u1_data);
    u4_ret = (u4_ret<<8) | u1_data;
    u1ExtmjcReadRegByI2C(0x628,&u1_data);
    u4_ret = (u4_ret<<8) | u1_data;
    return u4_ret;
}

INT32 drv_extmjc_i2c_mem_write(UINT32 u4SrcAddress,UINT32 u4DstAddress,UINT32 u4Size)
{
    UINT32 len,ret;
    INT32 u4FailCount;
    UINT8 u1Data;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_EXTMJCR_OK);
    }
    for(u4FailCount=0; u4FailCount<5;u4FailCount++)
    {
        u1ExtmjcReadRegByI2C(MT8280_REG_OSD_DMA_B0,&u1Data);
        //disable OSD DMA frame buffer control
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_B0, u1Data & 0xFE);
        //set OSD DMA starting address
        u1ExtmjcWriteRegByI2C4Bytes(MT8280_REG_DMA_ADDRESS, u4DstAddress);
        //OSD FIFO source from I2C
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C5, 0x7);
        // MT8281 OSD DMA software reset
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C2, 0x0);
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C2, 0x1);
        //OSD DMA enable
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C0, 0x3);
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C0, 0x2);
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C0, 0x3);
        //Set OSD FIFO Threshold
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C4, 0xf0);
        len = (u4Size>>4)<<4;
        if ((u4Size-len)>0)
            len+=16;

        #if CONFIG_MT8281
        //MT8281
        ret = u2ExtmjcWriteRegByI2CNBytes(0x9981,(UINT8*)u4SrcAddress, len);
        //ret = SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, 0x9981, (UINT8*)u4SrcAddress, len);
        #else
        //MT8280
        ret = u2ExtmjcWriteRegByI2CNBytes(0x2534,(UINT8*)u4SrcAddress, len);
        //ret = SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, 0x2534, (UINT8*)u4SrcAddress, len);
        #endif
        //OSD FIFO source from LVDS RX
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C5, 0x3);
        //restore OSD DMA frame buffer control
        u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_B0, u1Data);

        if (ret == len)
            return 0;
    }
    // fail;
    return 1;
}

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void _I2CWriteMem(UINT8* u1SrcAddress, UINT32 u4Size)
{
    UINT32 i = 0;

    LOG(6, "_I2CWriteMem copy to 8280(0x%x) size=%d\n",
        u1SrcAddress, u4Size);

    while (u4Size > 0)
    {

        if (u4Size < 256)
        {
            i = u4Size;
        }
        else
        {
            i = 256;
        }

        u4Size -= i;
        u2ExtmjcWriteRegByI2CNBytes(0x9981,u1SrcAddress,i);
//        SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, 0x2534, (UINT8*)u1SrcAddress, i);
        u1SrcAddress += i;
        //x_thread_delay(1);
    }
}

void drv_extmjc_gamma_write(UINT8 *pArray,UINT32 u4ChannelSize,UINT8 u1DesIdx)
{
	UINT32 u4TmpSize;
	UINT8 u1Tmp=3;
	CRIT_STATE_T csState;

    if (FALSE == drv_extmjc_is_enable())
    {
        return;
    }

    printf("extmjc gamma wr %d bytes to Des %d\n", u4ChannelSize,u1DesIdx);
    
    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);
    csState = x_crit_start();

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        goto EXTMJC_EXIT;
    }

    u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_GAMMA_ARRAY,&u1DesIdx,1);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        goto EXTMJC_EXIT;
    }
    
    if((u4ChannelSize&0xf) !=0) // size must be 16 bytes alignment
    {
    	u4TmpSize =( (u4ChannelSize>>4)<<4)+16;
    }
    else
    {
    	u4TmpSize=u4ChannelSize;
    }
    printf("size to write is %d\n",u4TmpSize);
    u2ExtmjcWriteRegByI2CNBytes(0x8268,pArray, u4TmpSize); // I2C writing to reg 0x8268 will write to DRAM

    if(u1DesIdx==2) // the last array is transfered, send command to 8283 to restore original setting
    {
    	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_GAMMA_ARRAY,&u1Tmp,1);
    	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    }
    EXTMJC_EXIT:
    x_crit_end(csState);
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return;
   
}

#if 0
void drv_extmjc_gamma_write(UINT8 *pDark[],UINT8 *pMiddle[],
                                        UINT8 *pBright[],UINT32 u4ChannelSize)
{
	UINT8 u1OrgData, bChannel;
	CRIT_STATE_T csState;

    if (FALSE == drv_extmjc_is_enable())
    {
        return;
    }
    Printf("\r\nGamma send address 0x%08x\n",GAMMA_DRAM_BASE);
	csState = x_crit_start();
    drv_extmjc_i2c_checksum_enable(FALSE);

    u1ExtmjcReadRegByI2C(MT8280_REG_OSD_DMA_B0,&u1OrgData);
    //disable OSD DMA frame buffer control
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_B0, u1OrgData & 0xFE);
    //set OSD DMA starting address
    u1ExtmjcWriteRegByI2C4Bytes(MT8280_REG_DMA_ADDRESS, GAMMA_DRAM_BASE);
    //OSD FIFO source from I2C
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C5, 0x7);
    // MT8281 OSD DMA software reset
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C2, 0x0);
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C2, 0x1);
    //OSD DMA enable
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C0, 0x3);
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C0, 0x2);
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C0, 0x3);
    //Set OSD FIFO Threshold
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C4, 0xf0);

    for (bChannel=0;bChannel<3;bChannel++)
        u2ExtmjcWriteRegByI2CNBytes(0x9981,pDark[bChannel], u4ChannelSize);
    for (bChannel=0;bChannel<3;bChannel++)
        u2ExtmjcWriteRegByI2CNBytes(0x9981,pMiddle[bChannel], u4ChannelSize);
    for (bChannel=0;bChannel<3;bChannel++)
        u2ExtmjcWriteRegByI2CNBytes(0x9981,pBright[bChannel], u4ChannelSize);

    //OSD FIFO source from LVDS RX
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_C5, 0x3);
    //restore OSD DMA frame buffer control
    u1ExtmjcWriteRegByI2C(MT8280_REG_OSD_DMA_B0, u1OrgData);

    drv_extmjc_i2c_checksum_enable(TRUE);
    x_crit_end(csState);
}
#endif

#define EXTMJC_SENT_PANEL_TIMEOUT 5
#define EXTMJC_SENT_PANEL_HW_REG_CNT 30
extern VOID extmjc_send_cmd_fail_handler(VOID);
INT32 drv_extmjc_send_panel(UINT8 *pPanel,UINT32 len)
{
    UINT8 u1Rery;
    UINT8 u1Data = 0;
    UINT8 i,j,k,l;
    UINT16 u1SendCnt;
    HAL_TIME_T rTime1,rTime2,rTimeDiff;
    static BOOL bInited = FALSE;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_EXTMJCR_OK);
    }

    if (bInited == TRUE)
        return 0;

    u1SendCnt=0;
    for (u1Rery=0;u1Rery<1;u1Rery++)
    {
        if ((INT32)DRVCUST_InitGet(eMT8280SendPanel) != 0)
        {
            bInited = TRUE;
            HAL_GetTime(&rTime1);
            Printf("EXTMJC:Send %03d Times Panel from MT53xx 0x%08x To MT8282 0x%08x\n",u1SendCnt,(UINT32)pPanel,PANEL_DRAM_BASE);
            k = (len+(EXTMJC_SENT_PANEL_HW_REG_CNT-1))/EXTMJC_SENT_PANEL_HW_REG_CNT;
            for (j=1;j<=k;j++)
            {
                do
                {
                    HAL_Delay_us(1000);
                    if (u1ExtmjcReadRegByI2C(EXTMJCTRI,(UINT8*)(&u1Data))==1)
                        goto EXTMJC_RESET;

                    HAL_GetTime(&rTime2);
                    HAL_GetDeltaTime(&rTimeDiff, &rTime1, &rTime2);
                    if (rTimeDiff.u4Seconds>=EXTMJC_SENT_PANEL_TIMEOUT)
                        goto EXTMJC_RESET;
                }while (u1Data != 0xF0);
                for (l=0;l<EXTMJC_SENT_PANEL_HW_REG_CNT;l++)
                {
                    u1ExtmjcWriteRegByI2C(EXTMJCP0+l,pPanel[l+((j-1)*EXTMJC_SENT_PANEL_HW_REG_CNT)]);
                }
                u1ExtmjcWriteRegByI2C(EXTMJCMSG,k-j);
                u1ExtmjcWriteRegByI2C(EXTMJCTRI,0xF1);
            }
            for(i=0;i<(((k*EXTMJC_SENT_PANEL_HW_REG_CNT)+3)/4);i++)
                Printf("EXTMJC:0x%02x 0x%02x 0x%02x 0x%02x\n",pPanel[(i*4)],pPanel[(i*4)+1],pPanel[(i*4)+2],pPanel[(i*4)+3]);
        }

        //wait MT8282 system ready
        HAL_GetTime(&rTime1);
        do
        {
            HAL_Delay_us(1000);
            HAL_GetTime(&rTime2);
            HAL_GetDeltaTime(&rTimeDiff, &rTime1, &rTime2);
            if (rTimeDiff.u4Seconds>=EXTMJC_SENT_PANEL_TIMEOUT)
                goto EXTMJC_RESET;
            u1ExtmjcReadRegByI2C(EXTMJCTRI,&u1Data);
        }while (u1Data != 0x20);
        Printf("EXTMJC: Command ready to trigger\n");
        return 0;
EXTMJC_RESET:
        Printf("EXTMJC: Command not ready\n");
#if 0
        extmjc_send_cmd_fail_handler();
        printf("EXTMJC: Download built-in MT8282 image from 0x%08x(size=0x%08x)\n",
            (0x28000000+0x740000),64*1024);
        d_custom_extmjc_upgrade_open();
        d_custom_extmjc_upgrade_erase();
        d_custom_extmjc_upgrade_write(0, ((UINT8 *)(0x28000000+0x740000)), 64*1024);
        d_custom_extmjc_upgrade_close();
        drv_extmjc_reset();
#endif
    }
    return 0;
}
//-----------------------------------------------------------------------------
/** drv_extmjc_wait_cmd_ready
 *
 *  wait last I2C message trigger command ready,
 *  need to enclose in the gh_extmjc_cmd_bin_sema semaphore ,
 *  ensure no one can trigger I2C message coammand
 *
 *  @param  flag    [IN]
 *             TRUE:need to unlock gh_extmjc_cmd_gpio_ack_bin_sema semaphore (usually using in "get data" command)
 *             FALSE:no need to unlock gh_extmjc_cmd_gpio_ack_bin_sema semaphore
 *  @param
 *
 *  @retval 0                Succeed
 *  @retval others           Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_wait_cmd_ready(BOOL b_unlock_sema)
{
    //UINT8   u1ack = 0;
    UINT32   u1ack = 0;
    UINT16  u2_hangup_count;
#ifndef CC_53XX_LOADER
    INT32 i4_ret;

    if (gb_extmjc_cmd_gpio_ack_bin_sema == TRUE)
    {
        i4_ret = EXTMJC_SEMA_LOCK_TIMEOUT(gh_extmjc_cmd_gpio_ack_bin_sema,5000); //5 seconds
        if (i4_ret == OSR_TIMEOUT)
        {
            printf("EXTMJC semaphore timeout force I2C read\n");
            u1ExtmjcReadRegByI2C(EXTMJCTRI, (UINT8*)(&u1ack));
            gb_extmjc_cmd_gpio_ack_bin_sema = FALSE;
            if (u1ack != EXTMJCACK)
            {
                return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
            }
            return (DRV_CUSTOM_EXTMJCR_OK); //using I2C command to polling in next time
        }
        if (b_unlock_sema)
        {
            EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_gpio_ack_bin_sema);
        }
    }
    else
#endif
    {
        u2_hangup_count = 0;
        do
        {
            u2_hangup_count++;
            if (u1ExtmjcReadRegByI2C(EXTMJCTRI, (UINT8*)(&u1ack))>0)
                return (DRV_CUSTOM_EXTMJCR_REG_FAIL);

            //if (u2_hangup_count==1000) //wait 5 seconds
            if (u2_hangup_count==16) //wait 80 msec
            {
                //UINT8 u1reg200,u1reg201,u1reg202;
                UINT32 u4RegTri,u4RegMsg;
                HAL_TIME_T rtime0;

                //u1ExtmjcReadRegByI2C(EXTMJCTRI, &u1reg200);
                //u1ExtmjcReadRegByI2C(EXTMJCMSG, &u1reg201);
                //u1ExtmjcReadRegByI2C(EXTMJCP0, &u1reg202);
                u1ExtmjcReadRegByI2C(EXTMJCTRI, (UINT8*)&u4RegTri);
                u1ExtmjcReadRegByI2C(EXTMJCMSG, (UINT8*)&u4RegMsg);
                HAL_GetTime(&rtime0);
                //printf("Extmjc:Time(%08ds) u2_hangup_count overflow 0x200=0x%02x 0x201=%02x 0x202=%02x\n",
                //    rtime0.u4Seconds,u1reg200,u1reg201,u1reg202);
                printf("Extmjc:Time(%08ds) u2_hangup_count overflow RegTri=0x%02x RegMsg=0x%02x\n",
                    rtime0.u4Seconds,u4RegTri,u4RegMsg);
                return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
            }
            x_thread_delay(5); //wait 5ms
        }while(u1ack != EXTMJCACK);
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_is_enable
 *  Indicate whether extmjc is enable
 *
 *  @param  NULL
 *
 *  @retval TRUE	        Existed
 *  @retval FALSE	        Not existed
 */
//-----------------------------------------------------------------------------
BOOL d_custom_extmjc_is_enable(VOID)
{
    return (drv_extmjc_is_enable());
}


//-----------------------------------------------------------------------------
/** drv_extmjc_is_enable
 *  Indicate whether extmjc is enable
 *
 *  @param  NULL
 *
 *  @retval TRUE	        Existed
 *  @retval FALSE	        Not existed
 */
//-----------------------------------------------------------------------------
BOOL drv_extmjc_is_enable(VOID)
{
    return (IS_COMPANION_CHIP_ON());
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_enable_alive_check
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
VOID d_custom_extmjc_enable_alive_check(BOOL b_enable)
{
    gb_extmjc_enable_alive_check = b_enable;
    return ;
}


//-----------------------------------------------------------------------------
/** d_custom_extmjc_get_alive_check
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
BOOL d_custom_extmjc_get_alive_check(VOID)
{
    return (gb_extmjc_enable_alive_check);
}


//-----------------------------------------------------------------------------
/** drv_extmjc_set_mjc_onoff
 *  Set mjc on/off
 *
 *  @param  b_mjc_on        [In] Turn on or off mjc
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_mjc_mode(
    UINT8       ui1_mjc_mode,
    UINT8*      pui1_msg_error
    )
{
    INT32 i4_ret;

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_color_temperature_ex
 *  Set color temperature
 *
 *  @param  pt_color_temperature    [In] Contains information of color temperature
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_color_temperature_ex(
    UINT8       ui1_path,
    UINT8       ui1_index,
    UINT16      ui2_value
    )
{
    INT32 i4_ret;

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_color_temperature
 *  Set color temperature separately
 *
 *  @param  pt_color_temperature    [In] Contains information of color temperature
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_color_temperature(
    UINT8       ui1_index,
    UINT16      ui2_value
    )
{
    INT32 i4_ret;
    UINT8 aui1_req[3];

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (ui1_index >= E_EXTMJC_CT_MAX)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_INV_ARG;
        goto EXTMJC_EXIT;
    }

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    aui1_req[0] = ui1_index;
    aui1_req[1] = ui2_value >> 8;
    aui1_req[2] = ui2_value & 0xFF;

    LOG(1,"drv_extmjc_set_color_temperature (%d, %d)\n", ui1_index, ui2_value);

    u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_COLOR_TEMPERATURE, aui1_req, 3);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return (i4_ret);
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_blue_stretch
 *  Set color temperature separately
 *
 *  @param  pt_color_temperature    [In] Contains information of color temperature
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_blue_stretch(
    UINT8       ui1_onoff,
    UINT8       ui1_rgain,
    UINT8       ui1_ggain,
    UINT8       ui1_bgain,
    UINT8       ui1_lumathr,
    UINT8       ui1_chromathr
    )
{
    INT32 i4_ret;
    UINT8 aui1_req[6];

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    aui1_req[0] = ui1_onoff;
    aui1_req[1] = ui1_rgain;
    aui1_req[2] = ui1_ggain;
    aui1_req[3] = ui1_bgain;
    aui1_req[4] = ui1_lumathr;
    aui1_req[5] = ui1_chromathr;

    printf("drv_extmjc_set_blue_stretch ui1_onoff = %d\n", ui1_onoff);

    u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_BLUE_STRETCH, aui1_req, 6);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return (i4_ret);
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_gamma
 *  Set gamma value
 *
 *  @param  ui1_gamma       [In] Gamma value.
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_gamma(
    UINT8       ui1_gamma,
    UINT8*      pui1_msg_error
    )
{
    //UINT8       ui1_req;
    INT32 i4_ret;
    UINT8 u1Gamma=0;


    if ((DRV_EXTMJC_GAMMA_OFF != ui1_gamma) &&
        (DRV_EXTMJC_GAMMA_DARK != ui1_gamma) &&
        (DRV_EXTMJC_GAMMA_MIDDLE != ui1_gamma) &&
        (DRV_EXTMJC_GAMMA_BRIGHT != ui1_gamma))
    {
        LOG(1, "EXTMJC Set wrong gamma=%d\n", ui1_gamma);
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if ((gt_extmjc_status.ui1_gamma == ui1_gamma) &&
        (TRUE == gt_extmjc_status.b_init_gamma) )
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        i4_ret = DRV_CUSTOM_EXTMJCR_OK;
        goto EXTMJC_EXIT;
    }

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    gt_extmjc_status.ui1_gamma = ui1_gamma;
    gt_extmjc_status.b_init_gamma = TRUE;

    printf("EXTMJC Set gamma=%d\n", ui1_gamma);

	//u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_GAMMA_SETTING);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, ui1_gamma);
	u1Gamma=ui1_gamma;
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_GAMMA_SETTING,&u1Gamma,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return (i4_ret);
#if 0
    INT32       i4_ret;
    UINT8       ui1_req;

    if ((DRV_EXTMJC_GAMMA_OFF != ui1_gamma) &&
        (DRV_EXTMJC_GAMMA_DARK != ui1_gamma) &&
        (DRV_EXTMJC_GAMMA_MIDDLE != ui1_gamma) &&
        (DRV_EXTMJC_GAMMA_BRIGHT != ui1_gamma))
    {
        LOG(1, "Set wrong gamma=%d\n", ui1_gamma);
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    if ((gt_extmjc_status.ui1_gamma == ui1_gamma) &&
        (TRUE == gt_extmjc_status.b_init_gamma) )
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    gt_extmjc_status.ui1_gamma = ui1_gamma;
    gt_extmjc_status.b_init_gamma = TRUE;

    printf("Set gamma=%d\n", ui1_gamma);

    ui1_req = ui1_gamma;
    i4_ret = extmjc_send_withresp_req(EXTMJC_MSG_SET_GAMMA_SETTING,
                                      1, /* ui2_req_body_len */
                                      &ui1_req /* pui1_req_body */
                                     );

    if (DRV_EXTMJCR_OK != i4_ret)
    {
        return (i4_ret);
    }

    /* parsing response data */
    if (1 != gui2_extmjc_resp_body_len)
    {
        LOG(1, "Set gamma resp len wrong =%d\n", gui2_extmjc_resp_body_len);
        return (DRV_EXTMJCR_INV_RESP);
    }

    if (NULL != pui1_msg_error)
    {
        *pui1_msg_error = gaui1_extmjc_resp_body[0];
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_pip
 *  Set PIP status
 *
 *  @param  ui1_frame_rate      [In] Frame rate
 *  @param  ui1_color_domain    [In] Color domain
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_mjc_bypass_window_directly(
    UINT8       ui1_mjc_bypasswindow_idx,
    UINT16      ui2_mjc_width,
    UINT16      ui2_mjc_height,
    UINT16      ui2_mjc_h_start,
    UINT16      ui2_mjc_v_start,
    UINT8*      pui1_msg_error
    )
{
    UINT8       aui1_req[9];
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set mjc bypass window ui1_mjc_bypasswindow_idx=%d, ui2_mjc_width=%d, ui2_mjc_height=%d, ui2_mjc_h_start=%d ui2_mjc_v_start=%d\n", ui1_mjc_bypasswindow_idx, ui2_mjc_width, ui2_mjc_height, ui2_mjc_h_start, ui2_mjc_v_start);
    aui1_req[0] = ui1_mjc_bypasswindow_idx;
    aui1_req[1] = ui2_mjc_width & 0xFF;
    aui1_req[2] = (ui2_mjc_width >> 8) & 0xFF;
    aui1_req[3] = ui2_mjc_height & 0xFF;
    aui1_req[4] = (ui2_mjc_height >> 8) & 0xFF;
    aui1_req[5] = ui2_mjc_h_start & 0xFF;
    aui1_req[6] = (ui2_mjc_h_start>> 8) & 0xFF;
	aui1_req[7] = ui2_mjc_v_start & 0xFF;
    aui1_req[8] = (ui2_mjc_v_start>> 8) & 0xFF;

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    u1ExtmjcSendI2CMsg( EXTMJC_MSG_SET_MJC_BYPASS_WINDOW_DIRECTLY,aui1_req,9);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return (i4_ret);
}



//-----------------------------------------------------------------------------
/** drv_extmjc_set_pip
 *  Set PIP status
 *
 *  @param  ui1_frame_rate      [In] Frame rate
 *  @param  ui1_color_domain    [In] Color domain
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_video_fmt(
    UINT8       ui1_lvds_mode,
    UINT8       ui1_refresh_rate,
    UINT16      ui2_h_total,
    UINT16      ui2_v_total,
    UINT8       ui1_mjc_on_off,
    UINT16      ui2_width,
    UINT16      ui2_height,
    UINT8*      pui1_msg_error
    )
{
#if 1
    UINT8       aui1_req[10];
    INT32 i4_ret;
//    if ( (ui1_color_domain != DRV_EXTMJC_VIDEO_FMT_COLOR_DOMAIN_YUV) &&
//         (ui1_color_domain != DRV_EXTMJC_VIDEO_FMT_COLOR_DOMAIN_RGB))

    if ( (ui1_lvds_mode > 7))
    {
        LOG(0, "Set wrong lvdsmode=%d\n", ui1_lvds_mode);
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set video lvdsmode=%d, Refresh Rate=%d, Htotal=%d, VTotal=%d ui2_width=%d ui2_height=%d\n", ui1_lvds_mode, ui1_refresh_rate, ui2_h_total, ui2_v_total,ui2_width,ui2_height);
    //aui1_req[0] = ui1_lvds_mode;
    aui1_req[0] = (ui1_lvds_mode&0xF) |((ui1_mjc_on_off & 0xF)<<4);
    aui1_req[1] = ui1_refresh_rate;
    aui1_req[2] = (ui2_h_total >> 8) & 0xFF;
    aui1_req[3] = ui2_h_total & 0xFF;
    aui1_req[4] = (ui2_v_total >> 8) & 0xFF;
    aui1_req[5] = ui2_v_total & 0xFF;
    aui1_req[6] = (ui2_width >> 8) & 0xFF;
    aui1_req[7] = ui2_width & 0xFF;
    aui1_req[8] = (ui2_height >> 8) & 0xFF;
    aui1_req[9] = ui2_height & 0xFF;
    //aui1_req[6] = ui1_mjc_on_off;

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    /*
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_VIDEO_FMT);
    u1ExtmjcWriteRegByI2C(EXTMJCP0, aui1_req[0]);
    u1ExtmjcWriteRegByI2C(EXTMJCP1, aui1_req[1]);
    u1ExtmjcWriteRegByI2C(EXTMJCP2, aui1_req[2]);
    u1ExtmjcWriteRegByI2C(EXTMJCP3, aui1_req[3]);
    u1ExtmjcWriteRegByI2C(EXTMJCP4, aui1_req[4]);
    u1ExtmjcWriteRegByI2C(EXTMJCP5, aui1_req[5]);
    u1ExtmjcWriteRegByI2C(EXTMJCP6, aui1_req[6]);
    */
    u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_VIDEO_FMT,aui1_req,10);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return (i4_ret);

#else
    INT32       i4_ret;
    UINT8       aui1_req[6];

    if ( (ui1_color_domain != DRV_EXTMJC_VIDEO_FMT_COLOR_DOMAIN_YUV) &&
         (ui1_color_domain != DRV_EXTMJC_VIDEO_FMT_COLOR_DOMAIN_RGB))
    {
        LOG(0, "Set wrong color domain=%d\n", ui1_color_domain);
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }


    printf("Set video color domain=%d, Refresh Rate=%d, Htotal=%d, VTotal=%d\n", ui1_color_domain, ui1_refresh_rate, ui2_h_total, ui2_v_total);
    aui1_req[0] = ui1_color_domain;
    aui1_req[1] = ui1_refresh_rate;
    aui1_req[2] = (ui2_h_total >> 8) & 0xFF;
    aui1_req[3] = ui2_h_total & 0xFF;
    aui1_req[4] = (ui2_v_total >> 8) & 0xFF;
    aui1_req[5] = ui2_v_total & 0xFF;

    i4_ret = extmjc_send_withresp_req(EXTMJC_MSG_SET_VIDEO_FMT,
                                      6, /* ui2_req_body_len */
                                      aui1_req /* pui1_req_body */
                                      );
    if (DRV_CUSTOM_EXTMJCR_OK != i4_ret)
    {
        return (i4_ret);
        }

    /* parsing response data */
    if (1 != gui2_extmjc_resp_body_len)
    {
        LOG(1, "Set LVDS resp len wrong =%d\n", gui2_extmjc_resp_body_len);
        return (DRV_EXTMJCR_INV_RESP);
    }

    if (NULL != pui1_msg_error)
    {
        *pui1_msg_error = gaui1_extmjc_resp_body[0];
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_SrcTiming
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_SrcTiming(
    UINT8                u1SrcTiming,
    UINT8*              pui1_msg_error
    )
{
    //UINT8       ui1_req;
    INT32 i4_ret;


   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set Src timing=%d\n", u1SrcTiming);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
	
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_SRC_TIMING,&u1SrcTiming,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_3D_fmt
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_3D_fmt(
    UINT8                u13D_fmt,
    UINT32                u4Para0,
    UINT32                u4Para1,
    UINT8*              pui1_msg_error
    )
{
    //UINT8       ui1_req;
    UINT8       aui1_req[9];
    INT32 i4_ret;


   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set 3D fmt=%d\n", u13D_fmt);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    aui1_req[0] = u13D_fmt;
    aui1_req[1] = (UINT8)(u4Para0&0xff);
    aui1_req[2] = (UINT8)((u4Para0&0xff00)>>8);
    aui1_req[3] = (UINT8)((u4Para0&0xff0000)>>16);
    aui1_req[4] = (UINT8)((u4Para0&0xff000000)>>24);
    aui1_req[5] = (UINT8)(u4Para1&0xff);
    aui1_req[6] = (UINT8)((u4Para1&0xff00)>>8);
    aui1_req[7] = (UINT8)((u4Para1&0xff0000)>>16);
    aui1_req[8] = (UINT8)((u4Para1&0xff000000)>>24);
	
	//u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_3D_FMT,&u13D_fmt,1);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_3D_FMT,aui1_req,9);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_PWM
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_PWM(
     UINT8                u1Src,
    UINT8                u1Ratio,
    UINT8                u1DutyStart,
    UINT8                u1DutyHigh,
    UINT8                u1DutyAll,
    UINT8*              pui1_msg_error
    )
{
    UINT8       aui1_req[5];
    INT32 i4_ret;


   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set PWM: Src(%d),Ratio(0x%x),Start(0x%x),High(0x%x),All(0x%x)\n", u1Src,u1Ratio,u1DutyStart,u1DutyHigh,u1DutyAll);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    aui1_req[0] = u1Src;
    aui1_req[1] = u1Ratio;
    aui1_req[2] = u1DutyStart;
    aui1_req[3] = u1DutyHigh;
    aui1_req[4] = u1DutyAll;
	
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_PWM,aui1_req,5);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_Mode_Change
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_Mode_Change(
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    //UINT8       ui1_req;
    #ifndef CC_53XX_LOADER
    SRM_MM_MODE_T eMM_Mode;
    #endif


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        	*pui1_msg_error = 0;
    	 }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    #ifndef CC_53XX_LOADER
    eMM_Mode = SRM_GetMmMode();
        if(eMM_Mode==SRM_MM_MODE_TYPE_THUMBNAIL)
        {	
        	Printf("--3D_thmb mode--\n");
        	return (DRV_CUSTOM_EXTMJCR_OK);
        }
    #endif

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_MODE_CHANGE);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;

}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_get_delay
 *
 *  @param  pui1_frm_delay      [In] frame delay
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                   Succeed
 *  @retval others              Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_get_delay(
    UINT8*                                  pui1_frm_delay,
    UINT8*                                  pui1_msg_error
    )
{
    INT32 i4_ret;
    UINT32 u4GetValue;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        *pui1_frm_delay = 0;
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_GET_MJC_DELAY);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    if (drv_extmjc_wait_cmd_ready(TRUE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    //u1ExtmjcReadRegByI2C(EXTMJCP0, pui1_frm_delay);
    u1ExtmjcReadRegByI2C(EXTMJCPARA0, (UINT8 *)(&u4GetValue));
    *pui1_frm_delay = (UINT8)(((u4GetValue & 0x0000ff00) >> 8) * 10);

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

#if 0
//-----------------------------------------------------------------------------
/** drv_extmjc_fix_lvds_mode
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
static void drv_extmjc_fix_lvds_mode(BOOL b_enable)
{
#if 0
    INT32       i4_ret;
    UINT8       ui1_req;
    UINT8 		ui1_msg_id;

    printf("Fix LVDS mode =%d \n", b_enable);

    ui1_req= (UINT8)b_enable;
    i4_ret = extmjc_prc_send_withresp_req(11, /* retry count */
                                          500,/* time out (ms) */
                                          EXTMJC_MSG_FIX_LVDS_MODE,
                                          1,
                                          &ui1_req,
                                          &ui1_msg_id);

    if (DRV_CUSTOM_EXTMJCR_OK != i4_ret)
    {
   	 printf("Fix LVDS mode fail %d\n", i4_ret);
    }
#endif
    UINT8       u1ack;
    UINT8       ui1_req;
    INT32 i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Fix LVDS mode =%d \n", b_enable);

    ui1_req= (UINT8)b_enable;

	u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_FIX_LVDS_MODE);
	u1ExtmjcWriteRegByI2C(EXTMJCP0, ui1_req);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return (i4_ret);
}
#endif


//-----------------------------------------------------------------------------
/** drv_extmjc_set_pip
 *  Set PIP status
 *
 *  @param  pt_pip          [In] PIP status
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_pip(
    DRV_EXTMJC_PIP_T*   pt_pip,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
}


//-----------------------------------------------------------------------------
/** drv_extmjc_set_ping
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_ping(
    UINT8*              pui1_msg_error
    )
{
#if 0
    UINT8 u1ret = 0;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    if (u1ExtmjcReadRegByI2C(EXTMJC_PING,&u1ret) != 0)
    {
        gui4_extmjc_ping_failed_cnt++;
        return DRV_CUSTOM_EXTMJCR_REG_FAIL;
    }
    if (u1ret != EXTMJCACK)
    {
        printf("EXTMJC:Ping [0x%04x]=0x%02x\n",EXTMJC_PING,u1ret);
        gui4_extmjc_ping_failed_cnt++;
        return DRV_CUSTOM_EXTMJCR_REG_FAIL;
    }
    gui4_extmjc_ping_failed_cnt=0;
    u1ExtmjcWriteRegByI2C(EXTMJC_PING, EXTMJCPING);

    return DRV_CUSTOM_EXTMJCR_OK;
#else
    INT32 i4_ret;


     if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
	{
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        gui4_extmjc_ping_failed_cnt++;
        goto EXTMJC_EXIT;
    }

    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCPING);

    if (drv_extmjc_wait_cmd_ready(TRUE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        gui4_extmjc_ping_failed_cnt++;
        goto EXTMJC_EXIT;
    }

    gui4_extmjc_ping_failed_cnt=0;
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
#endif
}

INT32 drv_extmjc_set_RX_reset(
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    //UINT8       ui1_req;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        	*pui1_msg_error = 0;
    	 }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_RESET_LVDS_RX);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;

}


//-----------------------------------------------------------------------------
/** drv_extmjc_set_lvds_enable
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_lvds_enable(
    BOOL                b_enable,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    UINT8       ui1_req;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

   // printf("@@@Set LVDS enable=%d\n", b_enable);

    if (gt_extmjc_status.b_lvds_enable == b_enable)
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        i4_ret = DRV_CUSTOM_EXTMJCR_OK;
        goto EXTMJC_EXIT;
    }

    gt_extmjc_status.b_lvds_enable = b_enable;

    if (TRUE == b_enable)
    {
        ui1_req = 1;
    }
    else
    {
        ui1_req = 0;
    }

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_LVDS_MUTE);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, ui1_req);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_LVDS_MUTE,&ui1_req,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
#if 0
    i4_ret = extmjc_send_withresp_req(EXTMJC_MSG_SET_LVDS_ENABLE,
                                      1, /* ui2_req_body_len */
                                      &ui1_req /* pui1_req_body */
                                     );

    if (DRV_CUSTOM_EXTMJCR_OK != i4_ret)
    {
        return (i4_ret);
    }

    /* parsing response data */
    if (1 != gui2_extmjc_resp_body_len)
        {
        LOG(1, "Set lvds enable resp len wrong =%d\n", gui2_extmjc_resp_body_len);
        return (DRV_CUSTOM_EXTMJCR_INV_RESP);
        }

    if (NULL != pui1_msg_error)
    {
        *pui1_msg_error = gaui1_extmjc_resp_body[0];
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
#endif

}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_osd_enable
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_osd_enable(
    BOOL                b_osd_enable,
    UINT8*              pui1_msg_error
    )
{
    UINT8       ui1_req;
    INT32 i4_ret;


   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set osd enable=%d\n", b_osd_enable);

    if (TRUE == b_osd_enable)
    {
        ui1_req = 1;
    }
    else
    {
        ui1_req = 0;
    }

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
	//u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_OSD_ENABLE);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, ui1_req);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_OSD_ENABLE,&ui1_req,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
#if 0
    i4_ret = extmjc_send_withresp_req(EXTMJC_MSG_SET_OSD_ENABLE,
                                      1, /* ui2_req_body_len */
                                      &ui1_req /* pui1_req_body */
                                     );

    if (DRV_CUSTOM_EXTMJCR_OK != i4_ret)
    {
        return (i4_ret);
    }

    /* parsing response data */
    if (1 != gui2_extmjc_resp_body_len)
    {
        LOG(1, "Set osd mode resp len wrong =%d\n", gui2_extmjc_resp_body_len);
        return (DRV_CUSTOM_EXTMJCR_INV_RESP);
    }

    if (NULL != pui1_msg_error)
    {
        *pui1_msg_error = gaui1_extmjc_resp_body[0];
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_osd_mode
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_osd_mode(
    UINT8               ui1_osd_mode,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;


    if ((ui1_osd_mode != DRV_EXTMJC_OSD_MODE_NORMAL) &&
		(ui1_osd_mode != DRV_EXTMJC_OSD_MODE_GRAPHIC) &&
        (ui1_osd_mode != DRV_EXTMJC_OSD_MODE_DMA))
    {
        LOG(1, "Set wrong osd mode=%d\n", ui1_osd_mode);
        i4_ret = DRV_CUSTOM_EXTMJCR_INV_ARG;
        goto EXTMJC_EXIT;
    }



   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set osd mode=%d\n", ui1_osd_mode);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
	//u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_OSD_MODE);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, ui1_osd_mode);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_OSD_MODE,&ui1_osd_mode,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
#if 0
    INT32       i4_ret;
    UINT8       ui1_req;

    ui1_req = ui1_osd_mode;
    i4_ret = extmjc_send_withresp_req(EXTMJC_MSG_SET_OSD_MODE,
                                      1, /* ui2_req_body_len */
                                      &ui1_req /* pui1_req_body */
                                     );

    if (DRV_CUSTOM_EXTMJCR_OK != i4_ret)
    {
        return (i4_ret);
    }

    /* parsing response data */
    if (1 != gui2_extmjc_resp_body_len)
    {
        LOG(1, "Set osd mode resp len wrong =%d\n", gui2_extmjc_resp_body_len);
        return (DRV_CUSTOM_EXTMJCR_INV_RESP);
    }

    if (NULL != pui1_msg_error)
    {
        *pui1_msg_error = gaui1_extmjc_resp_body[0];
    }

    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}

INT32 drv_extmjc_set_osd_fading(
    UINT8               ui1_osd_fading
    )
{
   if (FALSE == drv_extmjc_is_enable())
    {
        return 0;
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set osd fading=%d\n", ui1_osd_fading);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        goto EXTMJC_EXIT;
    }
	
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_OSD_FADING,&ui1_osd_fading,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return 0;

}


//-----------------------------------------------------------------------------
/** drv_extmjc_set_vcr_mode
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_vcr_mode(
    UINT8               ui1_vcr_mode,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_input_source_type
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_input_source_type(
    UINT8               ui1_input_source_type,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_osd_info
 *  MJC chipset DRAM Test
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_osd_info(
    BOOL                b_enable_scaler,
    UINT16              ui2_osd_width,
    UINT16              ui2_osd_height,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_reset_rx_tx
 *  reset Mt8282 rx & tx fifo
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_reset_rx_tx(
    UINT32 u4resetCnt,
    UINT8* pui1_msg_error
    )
{
    INT32       i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_RX_TX_RESET);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, u4resetCnt);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_RX_TX_RESET,(UINT8*)&u4resetCnt,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_spread_spectrum
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------

INT32 drv_extmjc_set_spread_spectrum( UINT8 u1Percentage)
{
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_SET_SPREAD_SPECTRUM);
    //u1ExtmjcWriteRegByI2C(EXTMJCP0, u1Percentage);
    u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_SET_SPREAD_SPECTRUM,&u1Percentage,1);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
        EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
        return i4_ret;

}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_spread_spectrum
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------

INT32 drv_extmjc_set_lvds_on_delay( UINT8 u1OnDelay)
{
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_LVDS_ON_DELAY);
    //u1ExtmjcWriteRegByI2C(EXTMJCP0, u1OnDelay);
    u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_LVDS_ON_DELAY,&u1OnDelay,1);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
        EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
        return i4_ret;

}

//-----------------------------------------------------------------------------
/** drv_extmjc_set_osd_enable
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_CSHD_mode(
    UINT8                u1CSHD_Mode,
    UINT16                u2OverScanSize,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    UINT8       aui1_req[3];

   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set CSHD=%d, overscan=%d\n", u1CSHD_Mode,u2OverScanSize);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    aui1_req[0] = u1CSHD_Mode;
    aui1_req[1] = (UINT8)(u2OverScanSize&0xff);
    aui1_req[2] = (UINT8)((u2OverScanSize&0xff00)>>8);
	
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_CSHD_SBM,aui1_req,3);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}


INT32 drv_extmjc_set_MJC_MirrorFlip(
    UINT8                u1MirrorFlip,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    UINT8 u1MirrorFlipSet=0;


   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set mirrorflip=%d\n", u1MirrorFlip);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

	// set flag format according to 8283
    if(u1MirrorFlip==1)
    	u1MirrorFlipSet=2;
    else if(u1MirrorFlip==2)
	u1MirrorFlipSet=1;
    else
    	u1MirrorFlipSet=u1MirrorFlip;
    
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_GET_MJC_MIRRORFLIP,&u1MirrorFlipSet,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}


INT32 drv_extmjc_setPanelPowerSequence( BOOL fgPowerOn )
{
    INT32 i4_ret;
    UINT8 ui1_req=0;


   if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("setPanelPowerSequence=%d\n", fgPowerOn);

    if (TRUE == fgPowerOn)
    {
        ui1_req = 1;
    }
    else
    {
        ui1_req = 0;
    }
    
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_PANEL_POWERSEQ,&ui1_req,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * d interface methods implementations
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
/** d_custom_extmjc_get_version
 *  Set window position
 *
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *  @param  pt_extmjc_version       [Out] version of extmjc
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_get_version(
    UINT8*                                  pui1_msg_error,
    DRV_CUSTOM_EXTMJC_VERSION_T*            pt_extmjc_version
    )
{

    UINT8       u1ret[2];
//    UINT16     u2version;
    INT32 i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_GET_MJC_VERSION);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    if (drv_extmjc_wait_cmd_ready(TRUE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    u1ExtmjcReadRegByI2C(EXTMJCP0, u1ret);
    u1ExtmjcReadRegByI2C(EXTMJCP1, (u1ret+1));

    if (pt_extmjc_version !=NULL &&
        pt_extmjc_version->ui1_size>=2)
    {
        pt_extmjc_version->pui1_version[0] = u1ret[0];
        pt_extmjc_version->pui1_version[1] = u1ret[1];
        pt_extmjc_version->ui1_len=2;
    }
    else
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

//    u2version=(u1ret[1]<<8)|u1ret[0];
    printf("MJC version %d.%02d = %d\n", u1ret[1],u1ret[0]);

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}


//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_mjc_setting
 *  Set window position
 *
 *  @param
 *  @param
 *  @param
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_mjc_setting(
                                    UINT8       ui1_mjc_effect,
                                    UINT8       ui1_60_120,
                                    UINT8       ui1_demo_mode,
                                    UINT8*      pui1_msg_error
                                    )
{
    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    return DRV_CUSTOM_EXTMJCR_OK;
}
//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_fastmute
 *  Set window position
 *
 *  @param  ui1_on_off              [In] Mute Disable/Enable.
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_fastmute(
    BOOL              fgMute,
    UINT8*              pui1_msg_error
    )
{

	UINT8       ui1_req;
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set mute=%d\n", fgMute);

    if (TRUE == fgMute)
    {
        ui1_req = 1;
    }
    else
    {
        ui1_req = 0;
    }

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
	
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_ALL_MUTE,&ui1_req,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
    
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_fastmute_timeout(
 *  Set window position
 *
 *  @param  Counter              [In] Mute timeout count.
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
//extern UINT32 _u48280Unmte;
INT32 d_custom_extmjc_set_fastmute_timeout(
    UINT32             Counter,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

 //Printf("\n DelayCount:%x",Counter);
   _u48282UiUnmteTimeout = Counter;

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return (i4_ret);
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_mute
 *  Set window position
 *
 *  @param  ui1_on_off              [In] Mute Disable/Enable.
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_mute(
    BOOL              ui1_on_off,
    UINT8*              pui1_msg_error
    )
{
    INT32 i4_ret;
    //static BOOL u1MuteInited=FALSE;
    //static BOOL u1Mute=FALSE;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    #if 0
    if (u1MuteInited == FALSE || u1Mute!=ui1_on_off)
    {
        u1MuteInited = TRUE;
        u1Mute=ui1_on_off;
        if(u1Mute==TRUE)
        {
            u1ExtmjcWriteRegByI2C(0x205c,0x80);
        }
        else
        {
            u1ExtmjcWriteRegByI2C(0x205c,0x0);
        }
    }
    #endif
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return (i4_ret);
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_window_position
 *  Set window position
 *
 *  @param  pat_window_position     [In] Points to an array containing window positions.
 *  @param  ui1_window_num          [In] Number of windows in pat_window_position
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_osd_mode(
    UINT8               ui1_osd_mode,
    UINT8*              pui1_msg_error
    )
{
    INT32       i4_ret;

    if ((ui1_osd_mode != DRV_CUSTOM_EXTMJC_OSD_MODE_NORMAL) &&
		(ui1_osd_mode != DRV_CUSTOM_EXTMJC_OSD_MODE_DMA) &&
        (ui1_osd_mode != DRV_CUSTOM_EXTMJC_OSD_MODE_GRAPHIC))
    {
        LOG(1, "Set wrong osd mode=%d\n", ui1_osd_mode);
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

#ifndef DRV_SUPPORT_EXTMJC_MODE0
    if (DRV_CUSTOM_EXTMJC_OSD_MODE_NORMAL == ui1_osd_mode)
    {
        i4_ret = drv_extmjc_set_osd_enable(FALSE, pui1_msg_error);
        OSD_DMA_SetMode(OSD_OP_NORMAL);
        i4_ret = drv_extmjc_set_osd_mode(ui1_osd_mode, pui1_msg_error);

    }

    if (DRV_CUSTOM_EXTMJC_OSD_MODE_DMA == ui1_osd_mode)
    {
        i4_ret = drv_extmjc_set_osd_mode(ui1_osd_mode, pui1_msg_error);
        OSD_DMA_SetMode(OSD_OP_DMA);
        i4_ret = drv_extmjc_set_osd_enable(TRUE, pui1_msg_error);
    }

    if (DRV_CUSTOM_EXTMJC_OSD_MODE_GRAPHIC == ui1_osd_mode)
    {
        i4_ret = drv_extmjc_set_osd_mode(ui1_osd_mode, pui1_msg_error);
        OSD_DMA_SetMode(OSD_OP_GFX);
        i4_ret = drv_extmjc_set_osd_enable(TRUE, pui1_msg_error);
    }
#endif //DRV_SUPPORT_EXTMJC_MODE0
    return (i4_ret);
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_window_position
 *  Set window position
 *
 *  @param  pat_window_position     [In] Points to an array containing window positions.
 *  @param  ui1_window_num          [In] Number of windows in pat_window_position
 *  @param  pui1_msg_error          [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_window_position(
    DRV_CUSTOM_EXTMJC_WINDOW_POSITION_T*    pat_window_position,
    UINT8                                   ui1_window_num,
    UINT8*                                  pui1_msg_error
    )
{
#if 1
    INT32       i4_ret;

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
    return i4_ret;
#else
    INT32       i4_ret;
    UINT8       aui1_req[50];
    INT32       i;
    UINT16      ui2_len;

    if (((NULL == pat_window_position) && (0 != ui1_window_num)) ||
        (5 < ui1_window_num))
    {
        LOG(1, "Set window position null or too many windows\n");
        return (DRV_CUSTOM_EXTMJCR_INV_ARG);
    }

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    aui1_req[0] = ui1_window_num;

    ui2_len = 1;
    for (i = 0; i < ui1_window_num; i++)
    {
        aui1_req[ui2_len + 0] = (pat_window_position[i].ui2_left_top_x >> 8) & 0xFF;
        aui1_req[ui2_len + 1] = pat_window_position[i].ui2_left_top_x & 0xFF;

        aui1_req[ui2_len + 2] = (pat_window_position[i].ui2_left_top_y >> 8) & 0xFF;
        aui1_req[ui2_len + 3] = pat_window_position[i].ui2_left_top_y & 0xFF;

        aui1_req[ui2_len + 4] = (pat_window_position[i].ui2_right_bottom_x >> 8) & 0xFF;
        aui1_req[ui2_len + 5] = pat_window_position[i].ui2_right_bottom_x & 0xFF;

        aui1_req[ui2_len + 6] = (pat_window_position[i].ui2_right_bottom_y >> 8) & 0xFF;
        aui1_req[ui2_len + 7] = pat_window_position[i].ui2_right_bottom_y & 0xFF;

        ui2_len += 8 ;
    }
    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_color_pattern
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_color_pattern(
    DRV_CUSTOM_EXTMJC_COLOR_PATTERN_T       t_color_pattern,
    UINT8*                                  pui1_msg_error
    )
{
    INT32       i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set color pattern test %d\n", t_color_pattern);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_COLOR_TEST_PATTERN);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, t_color_pattern);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_COLOR_TEST_PATTERN,&t_color_pattern,1);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_get_gpio
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_get_gpio(
    UINT8                                   ui1_gpio_id,
    BOOL*                                   pb_onoff,
    UINT8*                                  pui1_msg_error
    )
{
#if 0
    INT32 i4_ret;
    UINT8 u1_ret;
#endif
    printf("%s:%d extmjc_get_gpio not support! wait mt8280 code ready\n",__FILE__,__LINE__);
    return DRV_CUSTOM_EXTMJCR_OK;
#if 0

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("get gpio = %d status\n", ui1_gpio_id);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_GET_GPIO_TEST);
    u1ExtmjcWriteRegByI2C(EXTMJCP0, ui1_gpio_id);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    if (drv_extmjc_wait_cmd_ready(TRUE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    u1ExtmjcReadRegByI2C(EXTMJCP4, &u1_ret);
    *pb_onoff = u1_ret;
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
#endif
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_gpio
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_gpio(
    UINT8                                   ui1_gpio_id,
    BOOL                                    b_onoff,
    UINT8*                                  pui1_msg_error
    )
{
#if 1
    UINT8       aui1_req[2];
    INT32 i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("Set gpio=%d onoff=%d test\n", ui1_gpio_id, b_onoff);
    aui1_req[0] = ui1_gpio_id;
    if (TRUE == b_onoff)
    {
        aui1_req[1] = 1;
    }
    else
    {
        aui1_req[1] = 0;
    }

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_SET_GPIO_TEST);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, aui1_req[0]);
	//u1ExtmjcWriteRegByI2C(EXTMJCP1, aui1_req[1]);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_SET_GPIO_TEST,aui1_req,2);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
#endif
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_ui
 *  Set window status
 *
 *  @param  ui2_level           [In] off / low / middle / high
 *  @param  ui2_mode            [In] mjc mode : norm / 60Hz / no_film
 *  @param  ui2_demo            [In] demo mode : left / right / all
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_ui(
    UINT8                                   ui2_level,
    UINT8                                   ui2_mode,
    UINT8                                   ui2_demo,
    UINT8*                                  pui1_msg_error
    )
{

    INT32 i4_ret;
    UINT8 ui2_intp_left, ui2_intp_right;
    UINT8       aui1_req[4];

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    switch (ui2_demo)
    {
        case VDP_ARG_MJC_DEMO_RIGHT:
            ui2_intp_left = MJC_CTRL_INTP_OFF;
            ui2_intp_right = (ui2_mode == VDP_ARG_MJC_MODE_NO_DEBLUR)? MJC_CTRL_INTP_HFBK : MJC_CTRL_INTP_NORM;
            break;
        case VDP_ARG_MJC_DEMO_LEFT:
            ui2_intp_left = (ui2_mode == VDP_ARG_MJC_MODE_NO_DEBLUR)? MJC_CTRL_INTP_HFBK : MJC_CTRL_INTP_NORM;
            ui2_intp_right = MJC_CTRL_INTP_OFF;
            break;
        default:
            if (ui2_level == MJC_CTRL_EFCT_OFF)
            {
                ui2_intp_left = MJC_CTRL_INTP_OFF;
                ui2_intp_right = MJC_CTRL_INTP_OFF;;
            }
            else
            {
                ui2_intp_left = MJC_CTRL_INTP_NORM;
                ui2_intp_right = MJC_CTRL_INTP_NORM;;
            }
            break;
    }

    /*
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_SET_MJC_UI);
    u1ExtmjcWriteRegByI2C(EXTMJCP0, ui2_level);
    u1ExtmjcWriteRegByI2C(EXTMJCP1, ui2_intp_left);
    u1ExtmjcWriteRegByI2C(EXTMJCP2, ui2_intp_right);
    u1ExtmjcWriteRegByI2C(EXTMJCP3, ui2_mode);
    */
    aui1_req[0]=ui2_level;
    aui1_req[1]=ui2_intp_left;
    aui1_req[2]=ui2_intp_right;
    aui1_req[3]=ui2_mode;
    u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_MJC_UI,aui1_req,4);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_mjc_frz
 *  Set window freez
 *
 *  @param  ui1_fgfrz           [In] 0 / l 
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_mjc_frz(
    UINT8                                   ui1_fgfrz,
    UINT8*                                  pui1_msg_error
    )
{

    INT32 i4_ret;
    UINT8       aui1_req[1];

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
    aui1_req[0]=ui1_fgfrz;
    u1ExtmjcSendI2CMsg(EXTMJC_MSG_SET_MJC_FRZ, aui1_req, 1);
    u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}


//-----------------------------------------------------------------------------

/** d_custom_extmjc_set_demo_mode
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_demo_mode(
    UINT8                                   ui1_demo_mode,
    UINT16                                  ui2_middle_line_pos,
    UINT8*                                  pui1_msg_error
    )
{
//    ui2_middle_line_pos = 0;
    Printf("Extmjc demo mode = %d\n", ui1_demo_mode);

    switch (ui1_demo_mode)
    {
        case DRV_CUSTOM_EXTMJC_DEMO_MODE_NORMAL:
            d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_NORM, MJC_CTRL_INTP_NORM, NULL);
            break;
        case DRV_CUSTOM_EXTMJC_DEMO_MODE_LEFT_MJC:  // left on, right half fall back
            d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_NORM, MJC_CTRL_INTP_HFBK, NULL);
            break;
        case DRV_CUSTOM_EXTMJC_DEMO_MODE_RIGHT_MJC: // righ on, left half fall back
	        d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_HFBK, MJC_CTRL_INTP_NORM, NULL);
	        break;
	    case DRV_CUSTOM_EXTMJC_DEMO_MODE_BOTH_MJC:
	        d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_NORM, MJC_CTRL_INTP_NORM, NULL);
	        break;
	    case DRV_CUSTOM_EXTMJC_DEMO_MODE_LEFT_DNM:  // left on, right fall back
	        d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_NORM, MJC_CTRL_INTP_OFF, NULL);
	        break;
	    case DRV_CUSTOM_EXTMJC_DEMO_MODE_RIGHT_DNM: // right on, left fall back
	        d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_OFF, MJC_CTRL_INTP_NORM, NULL);
	        break;
	    case DRV_CUSTOM_EXTMJC_DEMO_MODE_BOTH_DNM:  // left/right half fall back
	        d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_HFBK, MJC_CTRL_INTP_HFBK, NULL);
	        break;
	    case DRV_CUSTOM_EXTMJC_DEMO_MODE_DISABLE:
	    default:
	        d_custom_extmjc_set_ui(VDP_ARG_MJC_EFFECT_HIGH, MJC_CTRL_INTP_OFF, MJC_CTRL_INTP_OFF, NULL);
	        break;
    }

    return (DRV_CUSTOM_EXTMJCR_OK);
}


//-----------------------------------------------------------------------------
/** d_custom_extmjc_sram_read
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_sram_read(
    UINT16                                  ui2_sram_addr,
    UINT8*                                  pui1_msg_error
    )
{

    UINT8       u1ret[4];
    UINT8       aui1_req[4];
    INT32 i4_ret;
    //UINT32     ui4_addr;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

     EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    //printf("get dram addr = %d\n", ui4_addr);

    aui1_req[1] = (ui2_sram_addr & 0x0000FF00) >> 8;
    aui1_req[0] = (ui2_sram_addr & 0x000000FF);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    //u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_GET_SRAM);
	//u1ExtmjcWriteRegByI2C(EXTMJCP0, aui1_req[0]);
	//u1ExtmjcWriteRegByI2C(EXTMJCP1, aui1_req[1]);
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_GET_SRAM,aui1_req,2);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    if (drv_extmjc_wait_cmd_ready(TRUE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
	{
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
		}

	u1ExtmjcReadRegByI2C(EXTMJCP4, u1ret);
       u1ExtmjcReadRegByI2C(EXTMJCP5, (u1ret+1));
       u1ExtmjcReadRegByI2C(EXTMJCP6, (u1ret+2));
       u1ExtmjcReadRegByI2C(EXTMJCP7, (u1ret+3));

    printf("read extmjc sram addr=0x%x, read sram data B3-B0 (hex)=0x%x,0x%x,0x%x,0x%x\n",
            ui2_sram_addr,u1ret[3], u1ret[2], u1ret[1], u1ret[0]);


    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
#if 0 //keep it
    UINT8 u1ret[4];

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

	u1ExtmjcReadRegByI2C(ui2_sram_addr, u1ret);
       u1ExtmjcReadRegByI2C(ui2_sram_addr+1, (u1ret+1));
       u1ExtmjcReadRegByI2C(ui2_sram_addr+2, (u1ret+2));
       u1ExtmjcReadRegByI2C(ui2_sram_addr+3, (u1ret+3));

    printf("read extmjc sram addr=0x%x, read sram data B3-B0 (hex)=0x%x,0x%x,0x%x,0x%x\n",
            ui2_sram_addr,u1ret[3], u1ret[2], u1ret[1], u1ret[0]);

    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_sram_write
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_sram_write(
    UINT16                                  ui2_sram_addr,
    UINT32                                  ui4_sram_data,
    UINT8*                                  pui1_msg_error
    )
{

    UINT8       aui1_req[8];
    INT32 i4_ret;
    //UINT32     ui4_addr, ui4_data;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

     EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    aui1_req[1] = (ui2_sram_addr & 0x0000FF00) >> 8;
    aui1_req[0] = (ui2_sram_addr & 0x000000FF);
    aui1_req[7] = (ui4_sram_data & 0xFF000000) >> 24;
    aui1_req[6] = (ui4_sram_data & 0x00FF0000) >> 16;
    aui1_req[5] = (ui4_sram_data & 0x0000FF00) >> 8;
    aui1_req[4] = (ui4_sram_data & 0x000000FF);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
	/*
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_SET_SRAM);
	u1ExtmjcWriteRegByI2C(EXTMJCP0, aui1_req[0]);
	u1ExtmjcWriteRegByI2C(EXTMJCP1, aui1_req[1]);
	u1ExtmjcWriteRegByI2C(EXTMJCP4, aui1_req[4]);
       u1ExtmjcWriteRegByI2C(EXTMJCP5, aui1_req[5]);
       u1ExtmjcWriteRegByI2C(EXTMJCP6, aui1_req[6]);
       u1ExtmjcWriteRegByI2C(EXTMJCP7, aui1_req[7]);
       */
       u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_SET_SRAM,aui1_req,8);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    printf("write extmjc sram addr=0x%x, write sram data (hex)=0x%x\n",
            ui2_sram_addr,ui4_sram_data);

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;

#if 0 //keep it
    UINT8 aui1_req[5];
    UINT8 u1ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    aui1_req[3] = (ui4_sram_data & 0xFF000000) >> 24;
    aui1_req[2] = (ui4_sram_data & 0x00FF0000) >> 16;
    aui1_req[1] = (ui4_sram_data & 0x0000FF00) >> 8;
    aui1_req[0] = (ui4_sram_data & 0x000000FF);

    printf("write extmjc sram addr=0x%x, sram data B3-B0 (hex)=0x%x,0x%x,0x%x,0x%x\n",
            ui2_sram_addr,aui1_req[3], aui1_req[2], aui1_req[1], aui1_req[0]);

	u1ExtmjcWriteRegByI2C(ui2_sram_addr, aui1_req[0]);
	u1ExtmjcWriteRegByI2C(ui2_sram_addr+1, aui1_req[1]);
	u1ExtmjcWriteRegByI2C(ui2_sram_addr+2, aui1_req[2]);
	u1ExtmjcWriteRegByI2C(ui2_sram_addr+3, aui1_req[3]);

    return (DRV_CUSTOM_EXTMJCR_OK);
#endif
}


//-----------------------------------------------------------------------------
/** d_custom_extmjc_get_dram
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_get_dram(
    UINT32                                  ui4_addr,
    UINT32*                                 pui4_data,
    UINT8*                                  pui1_msg_error
    )
{
    UINT8       u1ret[4];
    UINT8       aui1_req[4];
    INT32 i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
            *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    aui1_req[3] = (ui4_addr & 0xFF000000) >> 24;
    aui1_req[2] = (ui4_addr & 0x00FF0000) >> 16;
    aui1_req[1] = (ui4_addr & 0x0000FF00) >> 8;
    aui1_req[0] = (ui4_addr & 0x000000FF);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    /*
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_GET_DRAM_TEST);
	u1ExtmjcWriteRegByI2C(EXTMJCP0, aui1_req[0]);
	u1ExtmjcWriteRegByI2C(EXTMJCP1, aui1_req[1]);
	u1ExtmjcWriteRegByI2C(EXTMJCP2, aui1_req[2]);
	u1ExtmjcWriteRegByI2C(EXTMJCP3, aui1_req[3]);
	*/
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_GET_DRAM_TEST,aui1_req,4);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    if (drv_extmjc_wait_cmd_ready(TRUE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
	{
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
		}

	u1ExtmjcReadRegByI2C(EXTMJCP4, u1ret);
       u1ExtmjcReadRegByI2C(EXTMJCP5, (u1ret+1));
       u1ExtmjcReadRegByI2C(EXTMJCP6, (u1ret+2));
       u1ExtmjcReadRegByI2C(EXTMJCP7, (u1ret+3));
       if (pui4_data != NULL)
       {
              *pui4_data = (((UINT32)u1ret[3])<<24)|(((UINT32)u1ret[2])<<16)|
				(((UINT32)u1ret[1])<<8)|(((UINT32)u1ret[0]));
       }
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//-----------------------------------------------------------------------------
/** d_custom_extmjc_set_dram
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 d_custom_extmjc_set_dram(
    UINT32                                  ui4_addr,
    UINT32                                  ui4_data,
    UINT8*                                  pui1_msg_error
    )
{
    UINT8       aui1_req[8];
    INT32 i4_ret;


    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    printf("set dram addr=0x%x data=0x%x\n", ui4_addr, ui4_data);

    aui1_req[3] = (ui4_addr & 0xFF000000) >> 24;
    aui1_req[2] = (ui4_addr & 0x00FF0000) >> 16;
    aui1_req[1] = (ui4_addr & 0x0000FF00) >> 8;
    aui1_req[0] = (ui4_addr & 0x000000FF);
    aui1_req[7] = (ui4_data & 0xFF000000) >> 24;
    aui1_req[6] = (ui4_data & 0x00FF0000) >> 16;
    aui1_req[5] = (ui4_data & 0x0000FF00) >> 8;
    aui1_req[4] = (ui4_data & 0x000000FF);

    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }
	/*
    u1ExtmjcWriteRegByI2C(EXTMJCMSG, EXTMJC_MSG_MJC_SET_DRAM_TEST);
	u1ExtmjcWriteRegByI2C(EXTMJCP0, aui1_req[0]);
	u1ExtmjcWriteRegByI2C(EXTMJCP1, aui1_req[1]);
	u1ExtmjcWriteRegByI2C(EXTMJCP2, aui1_req[2]);
	u1ExtmjcWriteRegByI2C(EXTMJCP3, aui1_req[3]);
	u1ExtmjcWriteRegByI2C(EXTMJCP4, aui1_req[4]);
       u1ExtmjcWriteRegByI2C(EXTMJCP5, aui1_req[5]);
       u1ExtmjcWriteRegByI2C(EXTMJCP6, aui1_req[6]);
       u1ExtmjcWriteRegByI2C(EXTMJCP7, aui1_req[7]);
       */
       u1ExtmjcSendI2CMsg(EXTMJC_MSG_MJC_SET_DRAM_TEST,aui1_req,8);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}


//-----------------------------------------------------------------------------
/** drv_extmjc_sflash_ProtEn
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_sflash_ProtEn(
    UINT8*                                  pui1_msg_error
    )
{
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    printf("drv_extmjc_sflash_ProtEn\n");

        u1ExtmjcWriteRegByI2C(0x60d, 0x1c);
        u1ExtmjcWriteRegByI2C(0x600, 0x20);

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

    return i4_ret;
}


//-----------------------------------------------------------------------------
/** drv_extmjc_sflash_ProtDis
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_sflash_ProtDis(
    UINT8*                                  pui1_msg_error
    )
{
    INT32 i4_ret;

    if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
        }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    printf("drv_extmjc_sflash_ProtDis\n");

        u1ExtmjcWriteRegByI2C(0x60d, 0x0);
        u1ExtmjcWriteRegByI2C(0x600, 0x20);
    u1ExtmjcWriteRegByI2C(0x809, 0x0);

    i4_ret = DRV_CUSTOM_EXTMJCR_OK;

    return i4_ret;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_cpu_run
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_cpu_run(
    UINT8*                                  pui1_msg_error
    )
{

    u1ExtmjcWriteRegByI2C(0x809, 0x0);

    return DRV_CUSTOM_EXTMJCR_OK;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_cpu_run
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_cpu_stop(
    UINT8*                                  pui1_msg_error
    )
{
	u1ExtmjcWriteRegByI2C(0x55,0x4);
	u1ExtmjcWriteRegByI2C(0xff,0xff);
	u1ExtmjcWriteRegByI2C(0x125,0x1);
	u1ExtmjcWriteRegByI2C(0xa2,0x1);
    	//u1ExtmjcWriteRegByI2C(0x809, 0x1); // Old Way

    return DRV_CUSTOM_EXTMJCR_OK;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_i2c_access_enable_password
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_i2c_access_enable_password(
    UINT8*                                  pui1_msg_error
    )
{
	u1ExtmjcWriteRegByI2C(ETXTMJCPASS0, 0x8dca6c85);
	u1ExtmjcWriteRegByI2C(ETXTMJCPASS1, 0x1ec76748);

    return  DRV_CUSTOM_EXTMJCR_OK;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_i2c_access_disable_password
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_i2c_access_disable_password(
    UINT8*                                  pui1_msg_error
    )
{
	u1ExtmjcWriteRegByI2C(ETXTMJCPASS0, 0x33912782);
	u1ExtmjcWriteRegByI2C(ETXTMJCPASS1, 0x01231850);

    return DRV_CUSTOM_EXTMJCR_OK;
}

//-----------------------------------------------------------------------------
/** drv_extmjc_i2c_access_disable_password
 *  Set window status
 *
 *  @param
 *  @param  pui1_msg_error      [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_i2c_accessable_group(
    UINT16                                  ui2_accessable_group,
    UINT8*                                  pui1_msg_error
    )
{
	// only bim[8] and osd[2] is enable
	/*
	bit 0 : TCMGPR0
	bit 1 : TCMGPR1
	bit 2 : OSD
	bit 3 : SCPIP0
	bit 4 : SCPIP1
	bit 5 : POST_PROC
	bit 6 : MJC
	bit 7 : DRAM
	bit 8 : BIM
	bit 9 : TCON0
	bit10: TCON1
	bit11: OD
	bit12: RS232
	bit13: CKGEN
	bit14: LVDSRX
	*/
	u1ExtmjcWriteRegByI2C(ETXTMJCPASSCFG, 0x8000FEFB);

    return DRV_CUSTOM_EXTMJCR_OK;
}

#ifndef CC_53XX_LOADER
//-----------------------------------------------------------------------------
/** drv_extmjc_set_osd_enable
 *
 *
 *  @param  pui1_msg_error  [Out] Error code of the command from the MJC chipset
 *
 *  @retval 0                Succeed
 *  @retval others            Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_set_osd_turn_on(VOID)
{
#ifndef DRV_SUPPORT_EXTMJC_MODE0
    INT32       i4_ret;
    UINT8       ui1_msg_error;
#endif

    if (FALSE == drv_extmjc_is_enable())
    {
        return DRV_CUSTOM_EXTMJCR_OK;
    }


    printf("Set osd turn on\n");

#ifndef DRV_SUPPORT_EXTMJC_MODE0
    if (FALSE == gt_extmjc_status.b_osd_enable)
    {
        if (fgApiVideoIsVgaTiming(SV_VP_MAIN))
        {
            OSD_BASE_SetYuvOutput(FALSE);
        }
        else
        {
            UINT32 LVDSMode;
            LVDSMode = vDrvGetCurLvdsMode();
            if((LVDSMode==0)||(LVDSMode==2))
            {
            	OSD_BASE_SetYuvOutput(FALSE);
            }
            else
            {
              //OSD_BASE_SetYuvOutput(TRUE);
              OSD_BASE_SetYuvOutput(FALSE); // RGB to YUV do by LVDS TX Transform
        }
      }

#if DRV_SUPPORT_EXTMJC_DMA_OSD
        printf("Force to OSD normal mode to avoid OSD shift problem\n");
        OSD_DMA_SetMode(OSD_OP_NORMAL);
#endif

#if DRV_SUPPORT_EXTMJC_DMA_OSD
        i4_ret = drv_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &ui1_msg_error);
#endif
#ifdef DRV_SUPPORT_EXTMJC_GRAPHIC_OSD
        i4_ret = drv_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_GRAPHIC, &ui1_msg_error);
#endif

#if 1
        /* I2C mode */
        OSD_DMA_EnableI2C(TRUE);
#endif

#if DRV_SUPPORT_EXTMJC_DMA_OSD
        OSD_DMA_SetMode(OSD_OP_DMA);
#endif

#ifdef DRV_SUPPORT_EXTMJC_GRAPHIC_OSD
        OSD_DMA_SetMode(OSD_OP_GFX);
#endif

        i4_ret = drv_extmjc_set_osd_enable(TRUE, &ui1_msg_error);

        gt_extmjc_status.b_osd_enable = TRUE;
        return (i4_ret);
    }
#endif

    return DRV_CUSTOM_EXTMJCR_OK;
}

static void _extmjc_cmd_gpio_ack_notify_isr(INT32 i4Gpio, BOOL fgStatus)
{
    UINT32 u4_extmjc_ack_gpio;
    UINT32 u4_extmjc_gpio_polarity;

    u4_extmjc_ack_gpio = (INT32)DRVCUST_InitGet(eMT8280CmdAckGpio);
    u4_extmjc_gpio_polarity = (UINT32)DRVCUST_InitGet(eMT8280CmdAckGpioPolarity);
    if (u4_extmjc_ack_gpio != i4Gpio)
        return;

    if (GPIO_Input(u4_extmjc_ack_gpio) == u4_extmjc_gpio_polarity)
        EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_gpio_ack_bin_sema);
}


static VOID extmjc_ping_timer_fct(
    HANDLE_T        pt_tm_handle,
    VOID*           pv_tag)
{
    EXTMJC_MSG_T        t_msg;

    if (gb_extmjc_enable_alive_check == TRUE)
    {
        t_msg.ui1_msg_type = EXTMJC_MSG_TYPE_PING;
        extmjc_send_msg(&t_msg);
    }

    x_timer_start(gh_extmjc_ping_timer,
                  EXTMJC_PING_TIMER_PERIOD,
                  X_TIMER_FLAG_ONCE,
                  extmjc_ping_timer_fct,
                  NULL);

    return ;
}
#endif
VOID extmjc_msg_hdlr_init(VOID)
{
#ifndef CC_53XX_LOADER
    UINT32 u4_extmjc_ack_gpio;

    if (FALSE == drv_extmjc_is_enable())
    {
        return;
    }

    VERIFY(OSR_OK == x_sema_create(&gh_extmjc_cmd_bin_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));
    u4_extmjc_ack_gpio = (UINT32)DRVCUST_InitGet(eMT8280CmdAckGpio);
    if (u4_extmjc_ack_gpio != 0xFFFFFFFF)
    {
        if (OSR_OK == x_sema_create(&gh_extmjc_cmd_gpio_ack_bin_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK))
        {
            gb_extmjc_cmd_gpio_ack_bin_sema = TRUE;
            GPIO_Reg(u4_extmjc_ack_gpio, GPIO_TYPE_INTR_BOTH, _extmjc_cmd_gpio_ack_notify_isr);
            printf("register EXTMJC GPIO=%d ISR\n",u4_extmjc_ack_gpio);
        }
    }

    if (OSR_OK != x_timer_create(&gh_extmjc_ping_timer))
    {
        ASSERT(FALSE);
        return;
    }
    x_timer_start(gh_extmjc_ping_timer,
                  EXTMJC_PING_TIMER_PERIOD,
                  X_TIMER_FLAG_ONCE,
                  extmjc_ping_timer_fct,
                  NULL);
#endif
}

//-----------------------------------
// extmjc custom API
//-----------------------------------
INT32 drv_extmjc_custom_cmd(
    UINT32                u4Para0,
    UINT32                u4Para1,
    UINT8*              pui1_msg_error
    )
{
    UINT8       aui1_req[8];
    INT32 i4_ret;


   if (FALSE == drv_extmjc_is_enable())
    {
        if (NULL != pui1_msg_error)
        {
        *pui1_msg_error = 0;
    }
        return (DRV_CUSTOM_EXTMJCR_OK);
    }

    EXTMJC_SEMA_LOCK(gh_extmjc_cmd_bin_sema);

    LOG(1,"extmjc custom cmd:para0(%d),para1(%d)\n", u4Para0,u4Para1);
    if (drv_extmjc_wait_cmd_ready(FALSE) == DRV_CUSTOM_EXTMJCR_REG_FAIL)
    {
        i4_ret = DRV_CUSTOM_EXTMJCR_REG_FAIL;
        goto EXTMJC_EXIT;
    }

    aui1_req[0] = (UINT8)(u4Para0&0xff);
    aui1_req[1] = (UINT8)((u4Para0&0xff00)>>8);
    aui1_req[2] = (UINT8)((u4Para0&0xff0000)>>16);
    aui1_req[3] = (UINT8)((u4Para0&0xff000000)>>24);
    aui1_req[4] = (UINT8)(u4Para1&0xff);
    aui1_req[5] = (UINT8)((u4Para1&0xff00)>>8);
    aui1_req[6] = (UINT8)((u4Para1&0xff0000)>>16);
    aui1_req[7] = (UINT8)((u4Para1&0xff000000)>>24);
	
	u1ExtmjcSendI2CMsg(EXTMJC_MSG_CUSTOM_CMD,aui1_req,8);
	u1ExtmjcWriteRegByI2C(EXTMJCTRI, EXTMJCDO);
    i4_ret = DRV_CUSTOM_EXTMJCR_OK;
EXTMJC_EXIT:
    EXTMJC_SEMA_UNLOCK(gh_extmjc_cmd_bin_sema);
    return i4_ret;
}

//----------------------------------
// New d_interface extmjcc set/get API
//----------------------------------
INT32 d_custom_extmjc_set (
    DRV_CUSTOM_EXTMJC_TYPE_T    e_extmjc_type,
    VOID*                       pv_set_info,
    SIZE_T                      z_size,
    BOOL                        b_store
)
{
	INT32 i4ret=DRV_CUSTOM_EXTMJCR_OK;
	UINT8 u1MsgErr;

	switch(e_extmjc_type)
	{
		
		case DRV_CUSTOM_EXTMJC_TYPE_SET_OSD_MODE:
			if(pv_set_info!=NULL)
			{
				Printf("-*-*Set OSD mode:%d!!\n",((EXTMJC_TYPE_SET_OSD_MODE_T*)pv_set_info)->ui1_osd_mode);
				i4ret=d_custom_extmjc_set_osd_mode(((EXTMJC_TYPE_SET_OSD_MODE_T*)pv_set_info)->ui1_osd_mode,&u1MsgErr);				
			}
			break;

		default:
			Printf("d_custom_extmjc_set command not support: %d\n",(UINT32)e_extmjc_type);
			break;
	}
	return i4ret;	
}

 INT32 d_custom_extmjc_get (
    DRV_CUSTOM_EXTMJC_TYPE_T    e_extmjc_type,
    VOID*                       pv_get_info,
    SIZE_T*                     pz_size
)
 {
 	switch(e_extmjc_type)
	{
 		case DRV_CUSTOM_EXTMJC_TYPE_IS_ENABLE:
 			if(pv_get_info!=NULL)
 			{
 				*(BOOL*)pv_get_info=d_custom_extmjc_is_enable();
 			}
 			else
 			{
 				return DRV_CUSTOM_EXTMJCR_INV_ARG;
 			}
			break;

		default:
			Printf("d_custom_extmjc_get command not support: %d\n",(UINT32)e_extmjc_type);
			break;
 	}
 	return DRV_CUSTOM_EXTMJCR_OK;	
 }
#endif /* DRV_SUPPORT_EXTMJC */

