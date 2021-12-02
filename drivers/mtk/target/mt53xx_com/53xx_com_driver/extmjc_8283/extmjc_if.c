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
 * $RCSfile: extmjc_if.c,v $
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

#include "extmjc_if.h"
#include "extmjc_debug.h"
#include "extmjc_msg_hdlr.h"
//#include "extmjc_prc.h"
//#include "extmjc_prc_8226_xls.h"
#include "drvcust_if.h"
#include "drv_lvds.h"
#include "osd_drvif.h"
#include "nptv_if.h"
#include "sif_if.h"
#include "x_pdwnc.h"
#include "srm_drvif.h"
#include "vdo_misc.h"
#include "drv_display.h"
#include "drv_video.h"
#include "drv_lvds.h"
#include "source_table.h"
#include "drv_tve.h"
#ifdef DRV_SUPPORT_EXTMJC_MODE7
#include "video_def.h"
#endif
#include "panel.h"

#ifdef DRV_SUPPORT_EXTMJC
#ifdef CC_MTK_LOADER //defined(CC_5360_LOADER) ||defined(CC_5391_LOADER) 
#define CC_53XX_LOADER
#endif

#ifndef CC_53XX_LOADER
#define DBG_LEVEL 0
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define EXTMJC_THREAD_PRIORITY          70
#define EXTMJC_THREAD_STACK_SIZE        4096
#define EXTMJC_QUEUE_SIZE               32
#define EXTMJC_MSG_PRIORITY             0


typedef struct _REDO_TWO_CHIP_CONN_INFO_T
{
    UINT8 u1Path;
    #ifndef CC_UP8032_ATV
    BOOL fgEnable;
    #else
    UINT8 fgEnable;
    #endif
    UINT8 u1ColorDomain;
    UINT8 u1RefreshRate;
    UINT32 u4HTotal;
    UINT32 u4VTotal;
    UINT8 u1SrcTiming;
} REDO_TWO_CHIP_CONN_INFO_T;


extern VOID extmjc_ping_handler(VOID);

/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
//EXTMJC_PRC_CONFIG_T t_extmjc_prc_cfg;
HANDLE_T	h_extmjc_thread = NULL;
HANDLE_T	h_extmjc_que = NULL;

//extern UINT32 _u48280Unmte;
UINT32 _u48280Unmte = 0;

/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/

static VOID extmjc_thread(VOID* pv_arg)
{
    EXTMJC_MSG_T        t_msg;
    SIZE_T              z_msg_size;
    UINT16              ui2_index;

    z_msg_size = sizeof(EXTMJC_MSG_T);
    printf("extmjc_thread running \n");
    while (TRUE)
    {
        if (OSR_OK != x_msg_q_receive(&ui2_index,
                                      (VOID*) (&t_msg),
                                      &z_msg_size,
                                      &h_extmjc_que,
                                      1,
                                      X_MSGQ_OPTION_WAIT))
        {
            LOG(1, "ERR: RTX thread rec message failed\n");
            continue;
        }
		switch (t_msg.ui1_msg_type)
		{
			case EXTMJC_MSG_TYPE_TX_EMPTY:
				printf("EXTMJC_MSG_TYPE_TX_EMPTY\n");
				break;
			case EXTMJC_MSG_TYPE_TX_RETRY:
				printf("EXTMJC_MSG_TYPE_TX_RETRY\n");
				break;
			case EXTMJC_MSG_TYPE_PING:
			    extmjc_ping_handler();
				//printf("EXTMJC_MSG_TYPE_PING\n");
				break;
			case EXTMJC_MSG_TYPE_PING_FAIL:
			    //no such message
				break;
			default:
				break;
		}
    }
}
/*-----------------------------------------------------------------------------
 * extern methods declarations
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
INT32 extmjc_send_msg(EXTMJC_MSG_T* pt_msg)
{
    INT32 i4_ret;
	if (h_extmjc_que == NULL)
    {
        return (DRV_EXTMJCR_OK);
    }
    i4_ret = x_msg_q_send(h_extmjc_que, 
                          (VOID *) pt_msg, 
                          sizeof (EXTMJC_MSG_T), 
                          EXTMJC_MSG_PRIORITY);
    
    return (i4_ret);
}

//-----------------------------------------------------------------------------
/** drv_extmjc_init
 *  Init external mjc module
 *
 *  @retval 0	            Succeed
 *  @retval others	        Fail
 */
//-----------------------------------------------------------------------------
INT32 drv_extmjc_init(VOID)
{
	static BOOL fgInit=0;

    //Printf("**drv_extmjc_init:%d\n",fgInit);
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_EXTMJCR_OK);
    }

    if(fgInit)
    {
    	return (DRV_EXTMJCR_OK);
    }

    /* Init Process Thread */
    if (OSR_OK != x_msg_q_create( &h_extmjc_que,
                                  "extmjc_que",
                                  sizeof(EXTMJC_MSG_T), /* message size */
                                  EXTMJC_QUEUE_SIZE))
    {
        ASSERT(FALSE);
    }
    
    if (OSR_OK != x_thread_create( &h_extmjc_thread,
                                   "extmjc_thread",
                                   EXTMJC_THREAD_STACK_SIZE, /* z_stack_size */
                                   EXTMJC_THREAD_PRIORITY, /* ui1_priority */
                                   extmjc_thread, /* thread API */
                                   0, 
                                   NULL))
    {
        ASSERT(FALSE);
    }
    extmjc_msg_hdlr_init();
    fgInit=1;
    
    return (DRV_EXTMJCR_OK);
}

/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/

UINT8 drv_extmjc_gamma_send(VOID)
{
//	UINT8 bData;
//	UINT8 bChannel;
	OSD_OP_MODE_T OSD_OP_MODE;
//	UINT32 DMAAddress,u4Size,u4MallocAddr;
	//UINT32 ui4_start_time = x_os_get_sys_tick();
	//CRIT_STATE_T csState;
	HAL_TIME_T rCurTime, rDeltaTime,rStartTime;
	HAL_GetTime(&rStartTime);

	OSD_OP_MODE = OSD_DMA_GetMode();
	if(OSD_OP_MODE != OSD_OP_DMA)
	{
		OSD_DMA_SetMode(OSD_OP_DMA);
	}
	//vApiRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
	//vApiRedoTwoChipConnection( 0, 1);
	//--------------------
  // set current LVDS mode
  Printf("\nSetCurrent LVDS Mode");
	vDrvExtmjcRedoTwoChipConnection(SV_VP_MAIN,SV_TRUE);
       OSD_DMA_EnableI2C(TRUE);
	

#if 0
    	//vDrvSetCurLvdsMode(0x3);
	
	//---------------------------------------------------------
	// Gamma Table 64 Alignment (1024*3)
	u4Size = (3072 + OSD_DMA_ALIGN_SIZE);
       u4MallocAddr = (UINT32) x_mem_alloc(u4Size);
       VERIFY(NULL != x_memset((void*)u4MallocAddr, 0, u4Size));
	DMAAddress =  OSD_DMA_ALIGN(u4MallocAddr);
//	Printf("\nDMA Address(64):%8X",DMAAddress);

//       x_memcpy((void *)DMAAddress, GAMMA_1024_DARK, 1024);
//	OSD_DMA_Copy(0x490400,DMAAddress,1024);



	// Dark Gamma(GAMMA_1024_DARK)
	      for (bChannel = SV_RGB_R; bChannel <= SV_RGB_B; bChannel++)
	{
    		x_memcpy((void *)(DMAAddress +  bChannel*1024), GAMMA_1024_DARK[bChannel], 1024);
    		
	}
       OSD_DMA_Copy(0x00490400 , DMAAddress, 3072);

	// Middle Gamma(GAMMA_1024_MIDDLE)
	for (bChannel = SV_RGB_R; bChannel <= SV_RGB_B; bChannel++)
	{
    		x_memcpy((void *)(DMAAddress + bChannel*1024), GAMMA_1024_MIDDLE[bChannel], 1024);
    		
	}
    	   OSD_DMA_Copy((0x00490400+  0xc00) , DMAAddress, 3072);

#ifndef CC_DRV_IGNORE_BRIGHT_GAMMA
	// Bright Gamma9(GAMMA_1024_BRIGHT)
	for (bChannel = SV_RGB_R; bChannel <= SV_RGB_B; bChannel++)
	{
    		x_memcpy((void *)(DMAAddress + bChannel*1024) , GAMMA_1024_BRIGHT[bChannel], 1024);
    		
	}
	OSD_DMA_Copy((0x00490400 + 0x1800), DMAAddress, 3072);
#endif
	//---------------------------------------------------------
	Printf("Free Mem\n");
	x_mem_free((void*)u4MallocAddr);

#endif	

	OSD_DMA_SetMode(OSD_OP_MODE);

	Printf("Time End\n");
	//x_crit_end(csState);
	//Printf("Time End1\n");
	//Printf( "\r\n vApiVideoExternalGammaInit() cost %u ms\r\n\r\n", (x_os_get_sys_tick()-ui4_start_time)*5 );
	HAL_GetTime(&rCurTime);
  HAL_GetDeltaTime(&rDeltaTime, &rStartTime, &rCurTime);

  Printf("cost %u ms\r\n\r\n",rDeltaTime.u4Micros);

	//CRC Check---- Need Added
	//---------------------
	return (DRV_EXTMJCR_OK);
}

void vDrvExtmjcRedoTwoChipConnection(UINT8 u1Path,BOOL fgEnable)
{
    if (!IS_COMPANION_CHIP_ON())
    {
        return;
    }
    
    INT32 i4Ret;
    UINT8 u1MsgErr;
    #ifndef CC_MT8281_SLT
    //static SRM_TV_MODE_T _ePoPMode;
    #endif    
    static REDO_TWO_CHIP_CONN_INFO_T t_new_conn_info={ SV_VP_MAIN, SV_TRUE, 0xff , 0xff ,0, 0,0xff};
    static UINT8 _u1IsMJCOn = 0xff;
    static UINT8 _u1inited =0;

    // 5382p + 8280
    if ((FALSE == fgEnable) && (t_new_conn_info.fgEnable == SV_TRUE))
    {
        drv_extmjc_set_lvds_enable(SV_FALSE, &u1MsgErr);
        LOG(2,"drv_extmjc_set_lvds_enable: 0\n");
        // Change 538x and 8280 OSD to normal mode
        #ifndef DRV_SUPPORT_EXTMJC_MODE0
        i4Ret =d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_NORMAL, &u1MsgErr);
        #endif //DRV_SUPPORT_EXTMJC_MODE0
        t_new_conn_info.u1Path          = u1Path;
        t_new_conn_info.fgEnable        = fgEnable;
        LOG(2,"[vApiRedoTwoChipConnection]: OSD normal mode\n");
    }
    else if (TRUE == fgEnable)
    {
        UINT32  u4MjcVTotal;
        UINT32  u4MjcHTotal;
        UINT32  u4PanelWidth;
        UINT32  u4PanelHeight;
        UINT8   u1MjcRefreshRate;
        UINT8   u1LvdsMode;
        UINT8   u1PreLvdsMode;  
        UINT8 u1IsMJCOn;
        UINT8 u1ModeChanged;
        UINT8 u1SrcTypTmg;
        UCHAR ucEnable;

        SRM_MM_MODE_T eMM_Mode;

        if(_u1inited==0)
        {
        	#if defined(CC_FLIP_MIRROR_SUPPORT) && defined(CC_MT5395)
        	UINT8 u1Config;

		u1Config= u1GetFlipMirrorConfig();
		drv_extmjc_set_MJC_MirrorFlip(u1Config,&u1MsgErr);
        	#endif
        	_u1inited=1;
        }

        u1MjcRefreshRate = bOUTPUT_REFRESH_RATE;//bDrvVideoGetRefreshRate(SV_VP_MAIN);
        u1PreLvdsMode = vDrvGetCurLvdsMode();
        u4PanelWidth=PANEL_GetPanelWidth();
        u4PanelHeight=PANEL_GetPanelHeight();
        #ifndef CC_MT8281_SLT
        //_ePoPMode = SRM_GetTvMode();
        TVE_GetEnable(TVE_1, &ucEnable);

        eMM_Mode=SRM_GetMmMode();
        if(eMM_Mode==SRM_MM_MODE_TYPE_THUMBNAIL)
        {	
        	//Printf("--Redo_thmb mode--\n");
        	return;
        }

        //if((_ePoPMode == SRM_TV_MODE_TYPE_NORMAL) && (bIsScalerInput444(SV_VP_MAIN)==SV_FALSE))
        if (((_bSrcSubNew == SV_VS_MAX) || (ucEnable == SV_TRUE)) && (bIsScalerInput444(SV_VP_MAIN) == SV_FALSE))
        {
            #ifdef DRV_SUPPORT_EXTMJC_MODE0
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidNormal;
            #else
            #ifdef DRV_SUPPORT_EXTMJC_MODE3
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidYuv422TwoPort;
            #else
            #ifdef DRV_SUPPORT_EXTMJC_MODE7
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidYuv422TwoPort8Bit;
            #else
            u1IsMJCOn = 1;
            u1LvdsMode = eLvdsVidRgb444OnePort;
            #endif //DRV_SUPPORT_EXTMJC_MODE7
            #endif //DRV_SUPPORT_EXTMJC_MODE3
            #endif //DRV_SUPPORT_EXTMJC_MODE0
        }
        else 
        #endif    
        {
            #ifdef DRV_SUPPORT_EXTMJC_MODE0
            u1IsMJCOn = 0;
            u1LvdsMode = eLvdsVidNormal;
            #else        
            #ifdef DRV_SUPPORT_EXTMJC_MODE3
            u1IsMJCOn = 0;
            u1LvdsMode = eLvdsVidRgb444TwoPort;            
            #else
            u1IsMJCOn = 0;
            u1LvdsMode = eLvdsVidRgb444OnePort;            
            #endif //DRV_SUPPORT_EXTMJC_MODE3
            #endif //DRV_SUPPORT_EXTMJC_MODE0
        }


#ifdef DRV_SUPPORT_EXTMJC_MODE7
	 if(fgIsMainVga())
	 {
	 	u1LvdsMode = eLvdsVidNormal;
	 	u1IsMJCOn = 0;
	 }else
	 {
	 	u1LvdsMode = eLvdsVidYuv422TwoPort8Bit;
	 }
#endif	 
        

        //u4MjcHTotal = wDrvGetOutputHTotal() -1;
        //u4MjcVTotal = wDrvGetOutputVTotal() -1;
        u4MjcHTotal = wDrvGetOutputHTotal();
        u4MjcVTotal = wDrvGetOutputVTotal();

        u1SrcTypTmg = bDrvVideoIsMainSrcTV();

         if ((u4MjcHTotal !=t_new_conn_info.u4HTotal)   ||
            (u4MjcVTotal !=t_new_conn_info.u4VTotal)   ||
            (u1SrcTypTmg !=t_new_conn_info.u1SrcTiming)   ||
            (t_new_conn_info.u1RefreshRate != u1MjcRefreshRate) ||
            (_u1IsMJCOn  != u1IsMJCOn) ||
            (u1PreLvdsMode != u1LvdsMode)
            )
        {
            u1ModeChanged = 1;
        }
        else
        {
            u1ModeChanged = 0;        
        }

        if (u1ModeChanged)
        {
//            _u48280Unmte = 15;
//            LOG(2,"drv_extmjc_set_lvds_enable(PIP): mode changd done : 0\n");
//            drv_extmjc_set_lvds_enable(SV_FALSE, &u1MsgErr);
            //vDrvExtmjcSetMute(15);
		UINT8 u1MsgErr;
		d_custom_extmjc_set_fastmute(TRUE, &u1MsgErr);
        }

        vDrvSetCurLvdsMode(u1LvdsMode);

        if ((u1LvdsMode == eLvdsVidYuv422TwoPort) || (u1LvdsMode == eLvdsVidYuv422TwoPort8Bit) || (u1LvdsMode == eLvdsVidYuv422OnePort8Bit))
        {
            vDrvSetUniColorDomain(eColorDomainYUV,u1LvdsMode);
            vDrvSetLvdsColorDomain(eColorDomainYUV);
        }
        else
        {
            vDrvSetUniColorDomain(eColorDomainRGB,u1LvdsMode);
            vDrvSetLvdsColorDomain(eColorDomainRGB);
        }

        if (u1ModeChanged)
        {
           UINT8 u1MsgErr;	
            // Change 8280 LVDS mode
            d_custom_extmjc_set_fastmute(TRUE, &u1MsgErr);
            //i4Ret = drv_extmjc_set_video_fmt(u1LvdsMode, u1MjcRefreshRate, (UINT16) u4MjcHTotal, (UINT16) u4MjcVTotal, u1IsMJCOn, &u1MsgErr);
            i4Ret = drv_extmjc_set_video_fmt(u1LvdsMode, u1MjcRefreshRate, (UINT16) u4MjcHTotal, (UINT16) u4MjcVTotal, u1IsMJCOn, (UINT16) u4PanelWidth, (UINT16) u4PanelHeight,&u1MsgErr);
            Printf("Reset538x LVDS: (Enable(%d), Path(%d), u1LvdsMode(%d), "
                "rate(%d), H(%d), V(%d), W(%d), , H(%d) )\r\n",
                (UINT8)fgEnable, (UINT8)u1Path, (UINT8)u1LvdsMode,
                (UINT32)u1MjcRefreshRate, (UINT32)u4MjcHTotal, (UINT32)u4MjcVTotal, (UINT32)u4PanelWidth, (UINT32)u4PanelHeight);

		// set source type and timing
		//u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(SV_VP_MAIN);
		// check if input main source TV
		//u1SrcTypTmg = bDrvVideoIsMainSrcTV();
		i4Ret = drv_extmjc_set_SrcTiming(u1SrcTypTmg,&u1MsgErr);
		
		i4Ret = drv_extmjc_set_Mode_Change(&u1MsgErr);
		
            t_new_conn_info.u1RefreshRate   = u1MjcRefreshRate;
            t_new_conn_info.u4HTotal        = u4MjcHTotal;
            t_new_conn_info.u4VTotal        = u4MjcVTotal;
            _u1IsMJCOn = u1IsMJCOn;
            //d_custom_extmjc_set_fastmute_timeout(0x10,&u1MsgErr);
            d_custom_extmjc_set_fastmute(FALSE, &u1MsgErr);
        }



#ifdef DRV_SUPPORT_EXTMJC_MODE7
	 if(u1PreLvdsMode!=u1LvdsMode)
	 {
	 	INT32   i4_ret;
        	UINT8   ui1_err;

	 	u1ModeChanged = 1;
	 	if(u1LvdsMode == eLvdsVidNormal)
	 	{
              	//Printf("\n Enter VGA");
			i4_ret = d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_NORMAL, &ui1_err);
	 	}

	 	if(u1LvdsMode==eLvdsVidYuv422TwoPort8Bit)
	 	{
	 		//Printf("\n Exit VGA");
			i4_ret = d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &ui1_err);
	 	}
		UNUSED(i4_ret);
	 	//Printf("\n[@@@] Reconnect EXTMJC !!!!!");
	 }
#endif	 
        

        if(t_new_conn_info.fgEnable==SV_FALSE)
        {
            #ifndef DRV_SUPPORT_EXTMJC_MODE0
            // Change 538x and 8280 OSD to DMA mode
            i4Ret = d_custom_extmjc_set_osd_mode(DRV_EXTMJC_OSD_MODE_DMA, &u1MsgErr);
            LOG(2,"[vApiRedoTwoChipConnection]: OSD DMA mode\n");
            #endif //DRV_SUPPORT_EXTMJC_MODE0                
            t_new_conn_info.u1Path          = u1Path;
            t_new_conn_info.fgEnable        = fgEnable;
        }
    }

    UNUSED(u1MsgErr);
    UNUSED(i4Ret);
}

void vDrvExtmjcNormProc(void)
{
    UINT8 u1MsgErr;

    if (!IS_COMPANION_CHIP_ON())
    {
        return;
    }

    // delay unmute
    if (_u48280Unmte)
    {
        LOG(3, "8280 delay unmute counter: %d\n", _u48280Unmte);
        _u48280Unmte--;

        if (_u48280Unmte==0)
        {
            d_custom_extmjc_set_mute(FALSE,&u1MsgErr);
            LOG(2,"d_custom_extmjc_set_mute(FALSE,&u1MsgErr)\n");
        }
    }

    
}

void vDrvExtmjcSetMute(UINT32 u4Count)
{
    UINT8 u1MsgErr;

    if (!IS_COMPANION_CHIP_ON())
    {
        return;
    }

    if (_u48280Unmte > u4Count)
    {
        // do nothing
        return;
    }

    if (_u48280Unmte == 0)
    {
        d_custom_extmjc_set_mute(TRUE,&u1MsgErr);        
    }
    _u48280Unmte = u4Count;

    LOG(2,"d_custom_extmjc_set_mute(%d)\n", _u48280Unmte);

}

#endif
#else

INT32 drv_extmjc_init(VOID)
{
    return (DRV_EXTMJCR_OK);
}
#endif /* DRV_SUPPORT_EXTMJC */


