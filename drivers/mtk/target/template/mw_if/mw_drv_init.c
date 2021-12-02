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

#include "d_drv_cust.h"
#include "d_drv_cust_util.h"

#include "avc_combi_mw.h"
#include "sif_if.h"
#include "eeprom_if.h"

#include "mw_drv_post_init.h"

//prototypes, will move to header file later
extern VOID x_cli_start(void);
extern INT32 MW_IRRC_Init(void);
extern INT32 MW_SOCKET_USB_Init(void);
extern INT32 MW_BLOCK_Init(void);
extern INT32 MW_CHAR_Init(void);
extern INT32 MW_OSD_Init(void);
extern INT32 MW_VDP_Init( DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
extern INT32 MW_PMX_Init( DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
extern INT32 MW_GFX_Init(void);
extern INT32 MW_MPV_Init(void);
extern void SIF_Init(void);
extern INT32 Tuner_Init(void);
extern BOOL MW_DMX_Init(void);
extern INT32 MW_TvdInit( DRV_CUSTOM_DIR_T* pt_drv_cust_dir );
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
extern INT32 MW_JpgInit(void);
extern INT32 MW_CiInit(void);
extern INT32 buf_agt_init(void);
extern void ReadPanelIndexFromEeprom(void);
extern BOOL  drv_extmjc_is_enable(void);

/* For external MJC, 8226, 8280 */
#ifdef DRV_SUPPORT_EXTMJC
extern INT32 drv_extmjc_init(void);
typedef enum
{
    OSD_OP_NORMAL,
    OSD_OP_DMA,
    OSD_OP_GFX
} OSD_OP_MODE_T;

extern void OSD_DMA_SetMode(OSD_OP_MODE_T eMode);
#endif

INT32 x_drv_init(GEN_CONFIG_T* pt_config);

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

//===========================================================================
// Public functions

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

    BIM_SetTimeLog(6);
    VERIFY(GPIO_Init() == 0);
    SIF_Init();

    DRVCUST_BootupCheck();

    i4_ret = MW_PCDInit();
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    LoadPanelIndex();
#define CC_SRM_ON
#ifdef CC_SRM_ON
    SRM_Init();
#endif

    if(pt_config==NULL)
    {
        return INITR_FAIL;
    }
    // Start Frame Buffer Manager
    FBM_Init();

    // Init DBS
    if (DBS_Init())
    {
    }
    
    // this must be in front of Tuner_Init(). 
    // Tuner_Init() will use EEPNPTV_READ().
    // It should perform storage init before EEPNPTV_READ.
    // Or it will cause race condition.
    i4_ret = MW_CHAR_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
    
    i4_ret = Tuner_Init();
    if (i4_ret != 0)
    {
        //return INITR_FAIL;
    }

    // initail HDCP SRAM.
    i4_ret = EEPHDCP_Init();
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_IRRC_Init();
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = RS_232_Init();
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    /* External MJC */
#ifdef DRV_SUPPORT_EXTMJC
    i4_ret = drv_extmjc_init();
    if (i4_ret != 0)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
#endif

    i4_ret = MW_SOCKET_USB_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);   
        return i4_ret;
    }

    i4_ret = MW_BLOCK_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    /* VDP init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_VID_PLANE,
        DRVT_VID_PLANE,
        pt_config->pv_config );
    i4_ret = MW_VDP_Init(pt_drv_cust_dir);
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_GFX_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_MPV_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_RtcInit();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    if (!MW_DMX_Init())
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_AUD_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = buf_agt_init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_FpdDevInit();
    i4_ret = MW_IndctrDevInit();
    
    /* NPTV driver init */
    /* TV decoder init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_TV_DEC,
        DRVT_TV_DEC,
        pt_config->pv_config );
    i4_ret = MW_TvdInit( pt_drv_cust_dir );
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }
    
    /* Plane Mixer init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_PLA_MXR,
        DRVT_PLA_MXR,
        pt_config->pv_config );

    DRVCUST_SetDrvInit(1);

//    i4_ret = MW_PMX_Init( pt_drv_cust_dir );
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    i4_ret = MW_OSD_Init();
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

#ifdef DRV_SUPPORT_EXTMJC
    if (TRUE == drv_extmjc_is_enable())
    {
    #ifdef DRV_SUPPORT_EXTMJC_DMA_OSD
        OSD_DMA_SetMode(OSD_OP_DMA);
    #endif
    }
#endif

    DRVCUST_SetDrvInit(0);

    /* AV Connector init */
    pt_drv_cust_dir = d_drv_cust_get_dir_by_type(
        DRVT_AVC_COMP_VIDEO,
        DRVT_AVC_SPDIF,
        pt_config->pv_config );
    
    i4_ret = MW_SrcInit( pt_drv_cust_dir );
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
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

	/* Ci init */
#ifndef CI_SUPPORT_DVBT_BYPASS
//	i4_ret = MW_CiInit();
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
    if(i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    #ifdef JPG_ENABLE
    MW_JpgInit(); 
    #endif

    x_cli_start();
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
    if (i4_ret != RMR_OK)
    {
        Printf("drv_init:%d\n", __LINE__);
        return INITR_FAIL;
    }

    BIM_SetTimeLog(7);
    return INITR_OK;
}

