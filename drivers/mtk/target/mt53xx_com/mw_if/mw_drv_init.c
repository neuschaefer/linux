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
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_start_common.h"

#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_sys_name.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_bim.h"

#include "srm_drvif.h"
#include "fbm_drvif.h"
#include "drv_dbase.h"
#include "panel.h"
#include "nptv_if.h"
#include "osd_drvif.h"

#include "d_drv_cust.h"
#include "d_drv_cust_util.h"

#include "avc_combi_mw.h"
#include "sif_if.h"
#include "eeprom_if.h"
#include "x_pdwnc.h"
#ifndef  CC_DISCARD_SUPPORT_PCMCIA
#include "pcmcia_if.h"
#endif
#include "mw_drv_post_init.h"

#include "cust_drv_mw.h"
#include "ostg_if.h"
#include "pdwnc_if.h"
#if defined(CC_MT5363)
#ifdef LINUX_TURNKEY_SOLUTION
#include "pdwnc_drvif.h"
//#include "mw_if_t8032fw.h"
#endif /* LINUX_TURNKEY_SOLUTION */
#endif

#ifdef CC_AUD_SETUP_MELODY
#include "aud_if.h"
#endif

#ifdef CC_SUPPORT_HDMI_FBOOTING
#include "drv_hdmi.h"
#include "drv_dvi.h"
#endif

#include "eepdef.h"
#include "api_eep.h"

// Enable DC on/off stress test by RTC
//#define CC_DC_ONOFF_TEST_ENABLE

//prototypes, will move to header file later
extern VOID x_cli_start(void);
extern INT32 MW_IRRC_Init(void);
extern INT32 MW_SOCKET_USB_Init(void);
extern INT32 MW_BLOCK_Init(void);
extern INT32 MW_SICD_USB_Init(void);
extern INT32 MW_CHAR_Init(void);
extern INT32 MW_OSD_Init(void);
extern INT32 MW_VDP_Init( DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
extern INT32 MW_PMX_Init( DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
extern INT32 MW_GFX_Init(void);
extern INT32 MW_MPV_Init(void);
extern void SIF_Init(void);
extern INT32 Tuner_Init(void);
extern BOOL MW_DMX_Init(void);
#ifdef TIME_SHIFT_SUPPORT
extern BOOL MW_PVR_Init(void);
extern BOOL MW_Rec_DMX_Init(void);
#endif  // TIME_SHIFT_SUPPORT
extern INT32 MW_TvdInit( DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
extern INT32 MW_CEC_Init(void);
extern INT32 RS_232_Init(void);
extern INT32 MW_RtcInit(void);
extern BOOL MW_DMX_Init(void);
extern INT32 MW_AUD_Init(void);
extern INT32 MW_PCDInit(void);
extern INT32 MW_FpdDevInit(void);
extern INT32 MW_IndctrDevInit(void);
extern INT32 MW_SrcInit(DRV_CUSTOM_DIR_T* pt_drv_cust_dir);
extern VOID _MW_Nptv_SetConnectivity(void);
extern INT32 MW_PostInit(void);
extern INT32 MW_DRM_Init(void);
extern INT32 MW_JpgInit(void);
#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI) || defined(CC_INTERNAL_POD) || defined(CC_EXTERNAL_POD)
extern INT32 MW_PcmciaInit(void);
#endif
extern INT32 MW_StcCtrl_Init(void);
extern INT32 buf_agt_init(void);
extern void ReadPanelIndexFromEeprom(void);
extern BOOL  drv_extmjc_is_enable(void);

#ifdef CC_SUPPORT_TVE
extern INT32 MW_TveInit(void);
extern INT32 MW_VdoOutInit(void);
#endif /*CC_SUPPORT_TVE*/

/* For external MJC, 8226, 8280 */
#ifdef DRV_SUPPORT_EXTMJC
extern INT32 drv_extmjc_init(void);
extern UINT8 drv_extmjc_gamma_send(VOID);
extern void OSD_DMA_EnableI2C(BOOL fgEnable);
#endif

extern void CEC_Init(void);

#ifdef CHIP_SPEC_SUPPORT
extern INT32 MW_CHIP_SPEC_Init(void);
#endif
/*
typedef enum
{
    OSD_OP_NORMAL,
    OSD_OP_DMA,
    OSD_OP_GFX
} OSD_OP_MODE_T;
extern void OSD_DMA_SetMode(OSD_OP_MODE_T eMode);
extern INT32 OSD_ShowLogo(void);
*/
#define AUD_THREAD_NAME             "StartSound"
#define AUD_THREAD_STACK_SIZE       2048
#define AUD_THREAD_PRIORITY         100

#define DRV_PARALLEL_INIT
#define DRV_PARALLEL_INIT_DMX

#ifdef DRV_PARALLEL_INIT_DMX
HANDLE_T hThread_dmx = NULL;
#endif

INT32 x_drv_init(GEN_CONFIG_T* pt_config);

#define DRV_SET_TIMESTAMP(module)     x_os_drv_set_timestamp(module)

typedef enum{
    DRIVER_INIT_MW_IRRC = 0,
    DRIVER_INIT_GPIO,
    DRIVER_INIT_SIF,
    DRIVER_INIT_MW_PCD,
    DRIVER_INIT_SRM,
    DRIVER_INIT_FBM,
    DRIVER_INIT_DBS,
    DRIVER_INIT_MW_CHAR,
    DRIVER_INIT_Tuner,
    DRIVER_INIT_EEPHDCP,
    DRIVER_INIT_RS_232,
    DRIVER_INIT_MW_SOCKET_USB,
    DRIVER_INIT_MW_BLOCK,
    DRIVER_INIT_MW_SICD_USB,
    DRIVER_INIT_MW_VDP,
    DRIVER_INIT_MW_GFX,
    DRIVER_INIT_MW_MPV,
    DRIVER_INIT_MW_Rtc,
    DRIVER_INIT_MW_StcCtrl,
    DRIVER_INIT_MW_Tve,
    DRIVER_INIT_MW_VdoOut,
    DRIVER_INIT_MW_DMX,
    DRIVER_INIT_MW_DMX1,
    DRIVER_INIT_MW_DMX2,
    DRIVER_INIT_MW_AUD,
    DRIVER_INIT_buf_agt,
    DRIVER_INIT_MW_FpdDev,
    DRIVER_INIT_MW_IndctrDev,
    DRIVER_INIT_MW_Tvd,
    DRIVER_INIT_MW_CEC,
    DRIVER_INIT_MW_PMX,
    DRIVER_INIT_MW_OSD,
    DRIVER_INIT_MW_Src,
    DRIVER_INIT_MW_Ci,
    DRIVER_INIT_MW_DrvCustPost,
    DRIVER_INIT_MW_Jpg,
    DRIVER_INIT_x_cli_start,
    DRIVER_INIT_MW_Post,
    DRIVER_INIT_MW_DRM,
    DRIVER_INIT_avc_combi_init,
    DRIVER_INIT_OSD_DMA_SetMode,
    DRIVER_INIT_LoadPanelIndex,
    DRIVER_INIT_custom_drv_init,
    DRIVER_INIT_drv_extmjc,
    DRIVER_INIT_TargetCountry,
    DRIVER_INIT_PanelPowerOn,
    DRIVER_INIT_END
}DRIVER_INIT_T;

static CHAR _aszDriverInitStr[][20] =
{
    "MW_IRRC_Init",
    "GPIO_Init",
    "SIF_Init",
    "MW_PCDInit",
    "SRM_Init",
    "FBM_Init",
    "DBS_Init",
    "MW_CHAR_Init",
    "Tuner_Init",
    "EEPHDCP_Init",
    "RS_232_Init",
    "MW_SOCKET_USB_Init",
    "MW_BLOCK_Init",
    "MW_SICD_USB_Init",
    "MW_VDP_Init",
    "MW_GFX_Init",
    "MW_MPV_Init",
    "MW_RtcInit",
    "MW_StcCtrl_Init",
    "MW_TveInit",
    "MW_VdoOutInit",
    "MW_DMX_Init",
    "MW_DMX_Init1",
	"MW_DMX_Init2",
    "MW_AUD_Init",
    "buf_agt_init",
    "MW_FpdDevInit",
    "MW_IndctrDevInit",
    "MW_TvdInit",
    "MW_CEC_Init",
    "MW_PMX_Init",
    "MW_OSD_Init",
    "MW_SrcInit",
    "MW_PcmciaInit",
    "MW_DrvCustPostInit",
    "MW_JpgInit",
    "x_cli_start",
    "MW_PostInit",
    "MW_DRM_Init",
    "avc_combi_init",
    "OSD_DMA_SetMode",
    "LoadPanelIndex",
    "custom_drv_init",
    "drv_extmjc_init",
    "Target_Country",
    "PanelPowerOn",
};

#ifdef CC_DC_ONOFF_TEST_ENABLE
#define SYSTEM_SHUTDOWN_DELAY       5000
#define WAKEUP_TIMER                3       // system wake up timer by RTC

static HANDLE_T _hShutdownHandle = NULL;

static void RebootHandle(void *pvArg)
{
    UINT64 u8Time;
    
    UNUSED(pvArg);
   
    x_thread_delay(SYSTEM_SHUTDOWN_DELAY);
   
    RTC_GetTimeDate(&u8Time);
    u8Time += WAKEUP_TIMER;
    RTC_SetAlertTime(u8Time);
        
    // Enable RTC wakeup            
    vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_RTC_WAKEN);
    
    PDWNC_EnterPowerDown(WAK_IRRX | WAK_RTC, (UINT32)PDWNC_POWER_DOWN_UNKNOWN);
    return;
}
#endif /* CC_DC_ONOFF_TEST_ENABLE */

#if 0
//===========================================================================
// Constant and type definitions

typedef struct _DRV_MODULE_REG_T
{
    DRV_COMP_REG_T* prComp;
    UINT16 u2Num;
    UINT16 u2Max;
} DRV_MODULE_REG_T;

//===========================================================================
// Local functions

static INT32 _InitModule(DRV_MODULE_REG_T* prModule, DRV_TYPE_T eDrvType, UINT16 u2StartId, UINT16 u2Num)
{
    DRV_COMP_REG_T *prComp;
    UINT16 u2Idx;

    if (prModule == NULL)
    {
        return INITR_FAIL;
    }

    prComp = prModule->prComp;
    //Printf("mod.comp = 0x%08x\n", prComp);
    for (u2Idx = 0; u2Idx < u2Num; u2Idx++)
    {
        //Printf("\t%d:%d\n", eDrvType, u2StartId + u2Idx);
        prComp[u2Idx].e_type            = eDrvType;
        prComp[u2Idx].e_id_type         = ID_TYPE_IND;
        prComp[u2Idx].u.t_ind.pv_tag    = NULL;
        prComp[u2Idx].u.t_ind.ui1_port  = ALL_PORTS;
        prComp[u2Idx].u.t_ind.ui2_id    = u2StartId + u2Idx;
    }
    prModule->u2Max = u2Num;
    prModule->u2Num = u2Num;
    return INITR_OK;
}

static INT32 _RegCompConn(DRV_COMP_REG_T* prOutComp, UINT16 u2OutNum, DRV_COMP_REG_T* prInComp, UINT16 u2InNum, CONN_DIR_TYPE_T eDir)
{
    INT32 i4Ret;
    UINT16 u2Loop;

    if (prOutComp   != NULL &&
        prInComp    != NULL &&
        u2OutNum    >  0    &&
        u2InNum     >  0)
    {
        for (u2Loop = 0; u2Loop < u2InNum; u2Loop++)
        {
            i4Ret = x_rm_reg_conn_list(&prInComp[u2Loop], prOutComp, u2OutNum, eDir, FALSE);
            if (i4Ret)
            {
                return i4Ret;
            }
        }
        return INITR_OK;
    }
    return INITR_FAIL;
}

static INT32 _RegModuleConn(DRV_MODULE_REG_T* prOutModule, DRV_MODULE_REG_T* prInModule, CONN_DIR_TYPE_T eDir)
{
    if (prOutModule != NULL &&
        prInModule  != NULL)
    {
        return _RegCompConn(prOutModule->prComp, prOutModule->u2Num,
                            prInModule->prComp,  prInModule->u2Num,
                            eDir);
    }
    return INITR_FAIL;
}
#endif

static DRVP_TYPE_T drv_cust_post_init_parse(
    const DRV_CUSTOM_T*     pt_drv_cust,
    VOID*                   pv_tag )
{
    INT32     i4_ret;
//    SIZE_T    z_num_drvs = 0;

    if ( !pt_drv_cust )
    {
        return DRVP_ERROR;
    }

    switch( pt_drv_cust->e_custom_type )
    {
    case DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME:
        {
            DRV_REG_GRP_NAME_T*  pt_reg_grp_name =
                (DRV_REG_GRP_NAME_T*)pt_drv_cust->pv_arg;

            i4_ret = d_drv_cust_reg_grp_name( pt_reg_grp_name );
            if ( i4_ret != 0 )
            {
                return DRVP_ERROR;
            }
        }
        break;

    case DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL:
        {
            DRV_REG_COMP_EXCL_T*  pt_reg_comp_excl =
                (DRV_REG_COMP_EXCL_T*)pt_drv_cust->pv_arg;

            i4_ret = d_drv_cust_reg_comp_excl( pt_reg_comp_excl );
            if ( i4_ret != 0 )
            {
                return DRVP_ERROR;
            }
        }
        break;

#ifdef MW_PIP_POP_SUPPORT
    case DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL_AP:
        break;
#endif

    default:
        return DRVP_ERROR;
    }

    return DRVP_NEXT;
}

static INT32 _MW_DrvCustPostInit( DRV_CUSTOM_DIR_T*  pt_drv_cust_dir )
{
    INT32   i4_ret;

    if ( !pt_drv_cust_dir )
    {
        return INITR_OK;
    }

    i4_ret = d_drv_cust_parse(
        pt_drv_cust_dir->pt_custom,
        drv_cust_post_init_parse,
        NULL );
    if ( i4_ret != 0 )
    {
        return -1;
    }

    return INITR_OK;
}

//For Start-up sound playback
#ifdef CC_AUD_SETUP_MELODY
static void _StartAudThread(void *pvArgs)
{
    //Unmute Audio
    ADAC_Enable(TRUE);
    ADAC_Mute(FALSE);
    ADAC_CodecMute(FALSE);

    x_thread_delay(50); // For PWM first power on pop noise.
    ADAC_GpioCodecMute(FALSE);
	ADAC_SpeakerEnable(FALSE);
	if(!(Aud_IsHeadphoneExist()))
	{
		x_thread_delay(20);
	    ADAC_SpeakerEnable(TRUE);
	}
    //Play Setup Melody
    AUD_PlaySetupMelody(AUD_DEC_MAIN, 3, 1);
}
#endif

#if defined(CC_MT5363)
#ifdef LINUX_TURNKEY_SOLUTION
static INT32 _SetT8032Normal(void)
{
    UINT32 u4Val;

    VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_ENGINE_ARM) == 0);

//    PDWNC_WRITE32(REG_RW_UP_CFG, PDWNC_READ32(REG_RW_UP_CFG) & ~(ENGINE_EN));

    // Install T8032 ISR
    //VERIFY(_PDWNC_RegT8032Isr() == PDWNC_OK);

    // Install GPIO ISR
    // VERIFY(_PDWNC_InitGpio() == 0);

    u4Val = PDWNC_ReadWakeupReason();
    if((u4Val != PDWNC_WAKE_UP_REASON_AC_POWER) && (u4Val != PDWNC_WAKE_UP_REASON_WATCHDOG))
    {
        Printf("T8032 init wakeup case app stage...\n");
        VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
    }
    else
    {
        Printf("T8032 init AC on/off case app stage...\n");
        VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
    }

    return 0;
}
#endif /* LINUX_TURNKEY_SOLUTION */
#endif

static void _LedBlinkingThread(void *pvArgs)
{
    static UINT32 _fgLed = 0;
    UINT32 u4BackLightGpio, u4BackLightOnVal, u4BlinkingLedGpio, u4BlinkingLedOnVal, u4LedBlinkingPeriod;    
    if (DRVCUST_InitQuery(eBackLightGpio, &u4BackLightGpio) == 0)
    {
        UNUSED(DRVCUST_InitQuery(eBackLightOnVal, &u4BackLightOnVal));
        UNUSED(DRVCUST_InitQuery(eBootBlinkingLed, &u4BlinkingLedGpio));            
        UNUSED(DRVCUST_InitQuery(eBootBlinkingLedPeriod, &u4LedBlinkingPeriod));
        Printf("[LedBlinking] Gpio_(%d), toggle (%d)ms\n",u4BlinkingLedGpio, u4LedBlinkingPeriod);
        while(!((GPIO_Output(u4BackLightGpio, NULL) == u4BackLightOnVal) && (GPIO_Enable(u4BackLightGpio, NULL) == 1)))
        {
            _fgLed = !_fgLed;
            GPIO_SetOut(u4BlinkingLedGpio, _fgLed);
            x_thread_delay(u4LedBlinkingPeriod);
        }
        Printf("[LedBlinkikng] Panel is turned on, stop blinking led!!!\n");
        UNUSED(DRVCUST_InitQuery(eBootBlinkingLedOnVal, &u4BlinkingLedOnVal));            
        GPIO_SetOut(u4BlinkingLedGpio, u4BlinkingLedOnVal);
    }
    return;    
}

#ifdef DRV_PARALLEL_INIT
static UINT32 _u4ParallelInitDone = 0;
static void _ParallelInitThread(void *pvArgs)
{
    INT32 i4_ret;
    _u4ParallelInitDone = 0;
    i4_ret = MW_AUD_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("MW_AUD_Init Failed\n");
        _u4ParallelInitDone = 0xff;
        return;
    }
    _u4ParallelInitDone = 1;
}

static void _ParallelInitThread2(void *pvArgs)
{
    #ifdef CC_AUD_SETUP_MELODY
    HANDLE_T hThread;
    #endif
    BOOL fgFinished = TRUE;

// power on panel
    // only OSD logo init should trigger panel init.
    //if (DRVCUST_OptGet(eDrvLogoEnable))
    //{
      //      vApiPanelPowerSequence(TRUE);
    #ifdef CC_AUD_SETUP_MELODY
            if (bApiEepromReadByte(EEP_BOOT_MUSIC_ONOFF) & AUD_POWER_ON_MUSIC_MASK)
            {
    
            x_thread_create(&hThread, AUD_THREAD_NAME,
                                   AUD_THREAD_STACK_SIZE,
                                   AUD_THREAD_PRIORITY,
                                   _StartAudThread, 0, NULL);
            }
    #endif
    #if 0
            if (DRVCUST_OptGet(eDrvLogoVdecEnable))
            {
                MPV_ShowLogo();
            }
            else
            {
                OSD_ShowLogo();
            }
    #endif
            UNUSED(d_custom_misc_set(DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
                &fgFinished, sizeof(fgFinished), FALSE));

   // }
        DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_PanelPowerOn]);

}

#endif
#ifdef DRV_PARALLEL_INIT_DMX
static void _Parallel2InitThread(void *pvArgs)
{
    if (!MW_DMX_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return;
    }
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DMX]);

    #ifdef TIME_SHIFT_SUPPORT
    if (!MW_Rec_DMX_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return;
    }
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DMX1]);

    if (!MW_PVR_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return;
    }
    #endif  // TIME_SHIFT_SUPPORT
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DMX2]);
}
#endif

//===========================================================================
// Public functions

#ifdef CC_FAST_INIT
#if 0 // def CC_FAST_INIT_USE_SOFTWARE_SUSPEND
extern int hibernate(void); // software suspend
#else // if defined(CC_FAST_INIT_USE_TUXONICE)
extern int toi_try_hibernate(void); // tuxonice
#endif
static void do_hibernation(void)
{
    #if 0 // def CC_FAST_INIT_USE_SOFTWARE_SUSPEND
    hibernate();
    #else // if defined(CC_FAST_INIT_USE_TUXONICE)
    toi_try_hibernate();
    #endif
}
#endif
/*----------------------------------------------------------------------------
 * Function: x_drv_init
 *
 * Description:
 *      Drivers initialization
 *
 * Inputs:
 *      pt_config: The configuration parameters
 *
 * Outputs: -
 *
 * Returns:
 *      INITR_OK: Successful
 *      INITR_FAIL: Failed
 *---------------------------------------------------------------------------*/
INT32 x_drv_init(GEN_CONFIG_T* pt_config)
{
    INT32              i4_ret;
    DRV_CUSTOM_DIR_T*  pt_drv_cust_dir;
    DTVCFG_T rDtvCfg;
#if defined(CC_MT5363)
#ifdef LINUX_TURNKEY_SOLUTION
    UINT32 u4Val;
    UINT32 u4Offset, u4Size;
#endif /* LINUX_TURNKEY_SOLUTION */
#endif
    #ifdef DRV_PARALLEL_INIT
    HANDLE_T hThread2;
    //#ifdef CC_AUD_SETUP_MELODY
    //UINT32 i=0;
    //#endif
    #else
    #ifdef CC_AUD_SETUP_MELODY
    HANDLE_T hThread;
    //UINT32 i=0;
    #endif    
    #endif
    BOOL fgFinished;
    HANDLE_T hLedBlinkingThread;
    UINT32 i4LedBlinking;
    BIM_SetTimeLog(4);
	
//#if (!defined(CC_MT5363)) && (!defined(CC_MT5392B)) && defined(LINUX_TURNKEY_SOLUTION)
     //   PDWNC_CreateWatchDogThread();
//#endif

    custom_drv_init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_custom_drv_init]);

    #ifndef COUNTRY_FROM_EEPROM
    {
        #if (defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368))
        UINT32 u4Country;
        // Init target country
        if (DRVCUST_InitQuery(eCountryDefaultSetting, &u4Country) == 0)
        {
            // get target country settnig from driver customization
            SetTargetCountry(u4Country);
            Printf("drv_init as 0x%X model\n", u4Country);
        }
        else
        #endif    
        {
            #ifdef CC_ISDB_SUPPORT
                SetTargetCountry(COUNTRY_BZ);
                Printf("drv_init as BZ model\n");
            #elif defined(CC_TWN_SUPPORT)
                SetTargetCountry(COUNTRY_TW);
                Printf("drv_init as TW model\n");
            #else
                #ifdef CC_DVBT_SUPPORT
                // init as EU model
                SetTargetCountry(COUNTRY_EU);
                Printf("drv_init as EU model\n");
                #else
                SetTargetCountry(COUNTRY_US);
                Printf("drv_init as US model\n");
                #endif
            #endif
        }
        DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_TargetCountry]);
    }
    #endif //COUNTRY_FROM_EEPROM

    i4_ret = MW_IRRC_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_IRRC]);
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    VERIFY(GPIO_Init() == 0);
    CEC_Init();
#if 0
    PDWNC_SetT8032ToggleLed(0);

    if((DRVCUST_InitQuery(eFlagBootBlinkingLedSupport, &i4LedBlinking) == 0))
    {
        if ((!DRVCUST_InitGet(eLoaderLogoNumber)) && (i4LedBlinking))
        {
            x_thread_create(&hLedBlinkingThread, "DrvLedBlinking",
                                   2048,
                                   100,
                                   _LedBlinkingThread, 0, NULL);
      
        }
    }
#endif
#if defined(CC_MT5363)
#ifdef LINUX_TURNKEY_SOLUTION
    // Setup 8032 uP
    u4Offset = DRVCUST_InitGet(eT8032uPOffset);
    u4Size = DRVCUST_InitGet(eT8032uPSize);
#if 0
    PDWNC_InitT8032(u4Offset, u4Size);
    _SetT8032Normal();
#endif
    if((i4_ret = PDWNC_ReadSwVersion(&u4Val)) == PDWNC_OK)
    {
        Printf("T8032 FW version: %d\n", u4Val);
    }
#endif /* LINUX_TURNKEY_SOLUTION */
#endif
    //VERIFY(GPIO_Init() == 0);
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_GPIO]);

#if defined(CC_MT5363)
#ifdef LINUX_TURNKEY_SOLUTION
    if((i4_ret = PDWNC_ReadSwVersion(&u4Val)) == PDWNC_OK)
    {
        Printf("T8032 FW version: %d\n", u4Val);
    }
#endif /* LINUX_TURNKEY_SOLUTION */
#endif

    SIF_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_SIF]);

    DRVCUST_BootupCheck();     // Can use GPIO and I2C only in this function.

#if 0//def CC_SUPPORT_HDMI_FBOOTING
    vHDMIInit();
    vDviInitial();
    Printf("HDMI fast booting init...\n");
#endif

    i4_ret = MW_PCDInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_PCD]);
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    LoadPanelIndex();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_LoadPanelIndex]);

#ifdef SUPPORT_MULTI_PORTMAP
    vDrvLoadPortmapFromEeprom();
#endif


///#define CC_SRM_ON
#ifdef CC_SRM_ON
    SRM_Init();
    SRM_Pause();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_SRM]);
#endif

    if(pt_config==NULL)
    {
        return INITR_FAIL;
    }
    // Start Frame Buffer Manager
    FBM_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_FBM]);

    // Init DBS
    if (DBS_Init())
    {
    }
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_DBS]);
    
    // this must be in front of Tuner_Init().
    // Tuner_Init() will use EEPNPTV_READ().
    // It should perform storage init before EEPNPTV_READ.
    // Or it will cause race condition.
    i4_ret = MW_CHAR_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_CHAR]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI) || defined(CC_INTERNAL_POD) || defined(CC_EXTERNAL_POD)
        i4_ret = PCMCIA_Init();
#endif

    i4_ret = Tuner_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_Tuner]);
    if (i4_ret != 0)
    {
        //return INITR_FAIL;
    }
	
#ifdef DRV_PARALLEL_INIT_DMX	
    x_thread_create(&hThread_dmx, "DrvParallel2Init",  2048, 65,  _Parallel2InitThread, 0, NULL);
#endif

#ifdef DRV_PARALLEL_INIT
    x_thread_create(&hThread2, "DrvParallelInit",
                               2048,
                               100,
                               _ParallelInitThread, 0, NULL);
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_AUD]);
#else
    i4_ret = MW_AUD_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_AUD]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif

    // initail HDCP SRAM.
    i4_ret = EEPHDCP_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_EEPHDCP]);
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

#ifndef LINUX_TURNKEY_SOLUTION
    i4_ret = RS_232_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_RS_232]);
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif /* LINUX_TURNKEY_SOLUTION */

    /* External MJC */

#ifdef DRV_SUPPORT_EXTMJC
    i4_ret = drv_extmjc_init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_drv_extmjc]);
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif

    i4_ret = MW_SOCKET_USB_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_SOCKET_USB]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return i4_ret;
    }

    i4_ret = MW_BLOCK_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_BLOCK]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_SICD_USB_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_SICD_USB]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return i4_ret;
    }


    /* VDP init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_VID_PLANE,
        DRVT_VID_PLANE,
        pt_config->pv_config );
#if 1
    i4_ret = MW_VDP_Init(pt_drv_cust_dir);
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_VDP]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif
    i4_ret = MW_GFX_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_GFX]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_MPV_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_MPV]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_RtcInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Rtc]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_StcCtrl_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_StcCtrl]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

#ifndef DRV_PARALLEL_INIT_DMX
    if (!MW_DMX_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DMX]);

#ifdef TIME_SHIFT_SUPPORT
    if (!MW_Rec_DMX_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DMX1]);

    if (!MW_PVR_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif  // TIME_SHIFT_SUPPORT
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DMX]);
#endif


    i4_ret = buf_agt_init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_buf_agt]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_FpdDevInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_FpdDev]);

    i4_ret = MW_IndctrDevInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_IndctrDev]);

    /* NPTV driver init */
    /* TV decoder init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_TV_DEC,
        DRVT_TV_DEC,
        pt_config->pv_config );
    i4_ret = MW_TvdInit( pt_drv_cust_dir );
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Tvd]);
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
    // Resume SRM
    SRM_Resume();
#ifdef CC_SUPPORT_TVE	


    i4_ret = MW_TveInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Tve]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

 
i4_ret = MW_VdoOutInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_VdoOut]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }


#endif/* CC_SUPPORT_TVE */

#if 1

    i4_ret = MW_CEC_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_CEC]);
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif 


    /* Plane Mixer init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_PLA_MXR,
        DRVT_PLA_MXR,
        pt_config->pv_config );

    DRVCUST_SetDrvInit(1);

    i4_ret = MW_PMX_Init( pt_drv_cust_dir );
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_PMX]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_OSD_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_OSD]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

#ifdef DRV_SUPPORT_EXTMJC
    if (TRUE == drv_extmjc_is_enable())
    {
    #ifdef DRV_SUPPORT_EXTMJC_DMA_OSD
//alvin        OSD_DMA_SetMode(OSD_OP_DMA);
//        DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_OSD_DMA_SetMode]);
    #endif
    }
#endif

    DRVCUST_SetDrvInit(0);

    /* AV Connector init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_AVC_COMP_VIDEO,
        DRVT_AVC_SPDIF,
        pt_config->pv_config );

    #ifdef DRV_PARALLEL_INIT
    // wait until parallel init is done
    while (1)
    {
        if (_u4ParallelInitDone)
        {
            DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_AUD]);
            if (_u4ParallelInitDone == 0xff)
            {
                Printf("drv_init:%d\n", __LINE__);
                return INITR_FAIL;
            }
            break;
        }
        x_thread_delay(20);
    }
    #endif

    i4_ret = MW_SrcInit( pt_drv_cust_dir );
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Src]);
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    /* COMBI init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_AVC_COMBI,
        DRVT_AVC_COMBI,
        pt_config->pv_config );

    i4_ret = avc_combi_init( pt_drv_cust_dir );
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_avc_combi_init]);
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

	/* Ci init */
#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI) || defined(CC_INTERNAL_POD) || defined(CC_EXTERNAL_POD)
    i4_ret = MW_PcmciaInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Ci]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif    /* ifndef CI_SUPPORT_DVBT_BYPASS */

    /* Post init */
    /* Driver customization post init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_UNKNOWN,
        DRVT_UNKNOWN,
        pt_config->pv_config );

    i4_ret = _MW_DrvCustPostInit( pt_drv_cust_dir );
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DrvCustPost]);
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    #ifdef JPG_ENABLE
    MW_JpgInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Jpg]);
    #endif

#ifndef LINUX_TURNKEY_SOLUTION
    x_cli_start();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_x_cli_start]);
#endif /* LINUX_TURNKEY_SOLUTION */

#if 0
    // Connection List, Temp
    if (1)
    {
        static DRV_MODULE_REG_T _rModuleOsd;
        static DRV_COMP_REG_T _arCompOsd[3];

        static DRV_MODULE_REG_T _rModuleVdp;
        static DRV_COMP_REG_T _arCompVdp[2];

        static DRV_MODULE_REG_T _rModulePmx;
        static DRV_COMP_REG_T _arCompPmx[1];

        _rModuleOsd.prComp = _arCompOsd;
        _InitModule(&_rModuleOsd, DRVT_OSD_PLANE, 0, 3);

        _rModuleVdp.prComp = _arCompVdp;
        _InitModule(&_rModuleVdp, DRVT_VID_PLANE, 0, 2);

        _rModulePmx.prComp = _arCompPmx;
        _InitModule(&_rModulePmx, DRVT_PLA_MXR, 0, 1);

        // Register pmx0 <--> vdp
        i4_ret = (_RegCompConn(_arCompPmx, 1,
                        _arCompVdp, 2,
                        CONN_DIR_OUT_TO_INP));
        if (i4_ret != RMR_OK)
        {
            Printf("drv_init:%d\n", __LINE__);
            return INITR_FAIL;
        }

        // Register pmx0 <--> osd
        i4_ret = (_RegCompConn(_arCompPmx, 1,
                        _arCompOsd, 3,
                        CONN_DIR_OUT_TO_INP));
        if (i4_ret != RMR_OK)
        {
            Printf("drv_init:%d\n", __LINE__);
            return INITR_FAIL;
        }
    }
#endif

    i4_ret = MW_PostInit();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_Post]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_DRM_Init();
    DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_MW_DRM]);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    #ifdef DRV_SUPPORT_EXTMJC
    if (TRUE == drv_extmjc_is_enable())
    {
    	//vApiRedoTwoChipConnection(0,1);
       //OSD_DMA_EnableI2C(TRUE);
       //OSD_DMA_SetMode(OSD_OP_DMA);
    	i4_ret = drv_extmjc_gamma_send();
    	if(i4_ret != RMR_OK)
    	{
        	Printf("drv_init:%d\n", __LINE__);
        	return INITR_FAIL;
    	}
    	#ifndef __KERNEL__
    	OSD_DMA_SetMode(OSD_OP_NORMAL);
    	#endif
    #ifdef DRV_SUPPORT_EXTMJC_DMA_OSD
//alvin        OSD_DMA_SetMode(OSD_OP_DMA);
//        DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_OSD_DMA_SetMode]);
    #endif
    }
#endif

#ifdef CHIP_SPEC_SUPPORT
    i4_ret = MW_CHIP_SPEC_Init();
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif
    x_memset(&rDtvCfg, 0, sizeof(DTVCFG_T));

    #ifdef DRV_PARALLEL_INIT

    i4_ret = EEPDTV_GetCfg(&rDtvCfg);
    ASSERT(i4_ret==0); 
    if (i4_ret!=0)
    {
        Printf("x_drv_init: DTV EEPROM read failure.\n");
    }    
    if ((rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT) && (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_RTC))
    {
        Printf("x_drv_init: Quiet Boot\n");
    }
    else if (rDtvCfg.u1Flags2 & DTVCFG_FLAG2_DISABLE_LOGO)
    {
                Printf("x_drv_init: Disable Logo\n");
    }
    else    
    {
    fgFinished = FALSE;
    UNUSED(d_custom_misc_set(DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
        &fgFinished, sizeof(fgFinished), FALSE));
    x_thread_create(&hThread2, "DrvPanelPowerOnParallelInit",
                               2048,
                               100,
                               _ParallelInitThread2, 0, NULL);
    }                                   
    #else
    i4_ret = EEPDTV_GetCfg(&rDtvCfg);
    ASSERT(i4_ret==0); 
    if (i4_ret!=0)
    {
        Printf("x_drv_init: DTV EEPROM read failure.\n");
    }    
    if ((rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT) && (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_RTC))
    {
        Printf("x_drv_init: Quiet Boot\n");
    }
    else    
    {    
    // only OSD logo init should trigger panel init.
    //if (DRVCUST_OptGet(eDrvLogoEnable))
    //{
    //    vApiPanelPowerSequence(TRUE);
#ifdef CC_AUD_SETUP_MELODY
        if (bApiEepromReadByte(EEP_BOOT_MUSIC_ONOFF) & AUD_POWER_ON_MUSIC_MASK)
        {

        x_thread_create(&hThread, AUD_THREAD_NAME,
                               AUD_THREAD_STACK_SIZE,
                               AUD_THREAD_PRIORITY,
                               _StartAudThread, 0, NULL);
        }
#endif
#if 0
        if (DRVCUST_OptGet(eDrvLogoVdecEnable))
        {
            MPV_ShowLogo();
        }
        else
        {
            OSD_ShowLogo();
        }
#endif
   // }
    }
   DRV_SET_TIMESTAMP(_aszDriverInitStr[DRIVER_INIT_PanelPowerOn]);
   #endif /* DRV_PARALLEL_INIT */

// app will call this at d_custom_misc_get
#if 0//def CC_AUD_SETUP_MELODY
    if (bApiEepromReadByte(EEP_BOOT_MUSIC_ONOFF) & AUD_POWER_ON_MUSIC_MASK)
    {
    while (!AUD_IsSetupMelodyFinish())
    {
        x_thread_delay(500);
        i++;
        if(i==6)
        {
            i=0;
            Printf("Start Up Aud timeout!\n");
            break;
        }
    }
    AUD_StopSetupMelody(AUD_DEC_MAIN);
    }
#endif

// app will call this at d_custom_misc_get
#if 0 // remove this after AP call DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS
    {
        SIZE_T szSize = 0;
        fgFinished = TRUE;
        UNUSED(d_custom_misc_get(DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
            &fgFinished, &szSize));
        UNUSED(szSize);
    }
#endif

#ifdef CC_FAST_INIT
    #if 1
    printk("(yjdbg) x_drv_init: end of x_drv_init\n");
    do_hibernation(); // kernel panic
    #endif
#endif

#ifdef CC_DC_ONOFF_TEST_ENABLE
    // Start DC on/off stress test
    UNUSED(x_thread_create(&_hShutdownHandle, "ShutdownHandle", 0x800, 100, RebootHandle, 0, NULL));
#endif /* CC_DC_ONOFF_TEST_ENABLE */

    UNUSED(fgFinished);
    BIM_SetTimeLog(5);
    return INITR_OK;
}

