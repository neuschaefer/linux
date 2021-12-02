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
 * $RCSfile: drv_cust_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains specific implementation.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_rm.h"
#include "u_drv_cust.h"

/* Panel name */
#include "panel.h"
#include "x_assert.h"
#include "sif_if.h"
#include "sif_sw_drvif.h"
#include "pdwnc_if.h"
#include "storage_if.h"
#include "c_model.h"
#include "drv_tvd.h"
#include "drv_hdmi.h"
#include "mute_if.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "api_srch_if.h"
#include "x_pdwnc.h"
#include "x_util.h"
#include "x_os.h"
#include "x_dram.h"
#ifdef CC_AUD_SETUP_MELODY
#include "aud_if.h"
#endif
#if defined(CC_SUPPORT_HDMI_FBOOTING) || defined(CC_AUD_SETUP_MELODY)
#include "eepdef.h"
#include "api_eep.h"
#endif

#include "nand_if.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
#ifdef CUSTOMER_FACTORY_SERVICE_MODE_ENABLE
extern INT32 PDWNC_SetupAliveCheck(UINT32 u4Mask, UINT32 u4ModeOnOff);
static UINT32  ui4_alive_check_ctrl = 0;
#endif /* CUSTOMER_FACTORY_SERVICE_MODE_ENABLE */

static PANEL_SELECTION_CONFIG_T _rPanelSelCfg = { PANEL_SELECTION_MODE_LAST, 0 };
static BOOL _fgVideoLogoFinished = TRUE;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
#ifdef CC_Support_Sil9285
extern UINT8 vSiI9287EDIDWrite(UINT8*,UINT8*) ;

extern UINT8  vSiI9287EDIDread(UINT8, UINT8*);

extern UINT8  vSiI9287HDCPKSVread(UINT8*);
#endif

extern void MW_VDP_ENABLE_CB(BOOL fEnable);

extern INT32 d_custom_misc_get_loader_info(
    UINT8*     szVersion,
    UINT32*    pu4VersionSize,
    UINT32*    pu4CheckSum
);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
INT32 DRVCUSTOM_Eeprom_Write(UINT32 u4Offset, UINT8 u1Data)
{
    INT32 i4Ret;
    STORG_DEV_T *prDev;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);
    if (prDev == NULL) { return -1; }
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE, NULL, NULL, NULL,
                    (UINT64)u4Offset, 1, (void *)&u1Data, NULL);
    return i4Ret;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

INT32 d_custom_misc_set (
    DRV_CUSTOM_MISC_TYPE_T    e_misc_type,
    VOID*                     pv_set_info,
    SIZE_T                    z_size,
    BOOL                      b_store )
{
    INT32 i4Val, i4Ret=-1;
    UINT8 u1CHannelID; UINT16 u2ClkDiv; UINT8 u1DevAddr; UINT8 u1WordAddrNum ; UINT32 u4WordAddr; UINT8 *pu1Buf ; UINT16 u2ByteCntl;
    PANEL_SELECTION_CONFIG_T *prPanel;
    UINT32 u4Addr;

    // Check input parameters
    if((pv_set_info == NULL) || (z_size == 0))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    // Check pz_size

    switch (e_misc_type)
    {
        case DRV_CUSTOM_MISC_TYPE_TUNER_SIF:
        {
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_HDMI_EDID:
        {
            break;
        }
        default:
            break;
    }

    // Implementation
    switch (e_misc_type)
    {
        case DRV_CUSTOM_MISC_TYPE_TUNER_SIF:
        {
             ASSERT( (((MISC_TYPE_TUNER_SIF_T *)pv_set_info)->ui1_offset_len) ==0);
             ASSERT( z_size == 1 );
             i4Val = SIF_TunerWriteNoSubAddr(0x100,
    	                 (UINT8)(((MISC_TYPE_TUNER_SIF_T *)pv_set_info)->ui4_addr),
                        (UINT8 *)&(((MISC_TYPE_TUNER_SIF_T *)pv_set_info)->ui1_data),
                       1);
		ASSERT( (i4Val ==1) || (i4Val==0) );	//0:fail, 1:successfuly get one byte
		i4Ret = (i4Val? RMR_OK: -1);
            break;
        }
        //#ifdef CC_SCPOS_FORCE_UNMUTE
		case DTV_CUSTOM_MISC_TYPE_FORCE_MUTE_OFF:
            _vDrvVideoForceUnMute(0, *(BOOL*)pv_set_info);
			break;
	//#endif

	    case DRV_CUSTOM_MISC_TYPE_AUTO_SEARCH :
		{
			UINT32 u4AutoSearchType;
			switch(*(UINT32*)pv_set_info)
			{
				case AUTOSEARCH_CHANGE_CHANNEL:
					u4AutoSearchType = SV_CHANGE_CHANNEL_MODE;
					break;
				case AUTOSEARCH_SCAN:
					u4AutoSearchType = SV_SCAN_MODE;
					break;
				case AUTOSEARCH_FINE_TUNE:
					u4AutoSearchType = SV_FINE_TUNE_MODE;
					break;
				default:
					i4Ret = -1 ;
					return i4Ret;
			}
            vSetAutoSearchType(u4AutoSearchType);
            i4Ret = RMR_OK;
	        break;
		}

        //////////////d_custom_misc_set::DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED//////////////////////////////
        case DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED:
        {
            u1CHannelID = ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->ui1_channel;
            u2ClkDiv    = ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->ui2_clkdiv;
            u1DevAddr   = ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->ui4_addr;
            u1WordAddrNum = ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->ui1_offset_len;
            u4WordAddr = ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->ui4_offset;
            pu1Buf = ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->pui1_data;
            u2ByteCntl= ((MISC_TYPE_SIF_ADVANCED_T *)pv_set_info)->ui1_data_len;

            i4Val =SIF_X_Write(
                              u1CHannelID,
                              u2ClkDiv,
                              u1DevAddr,
                              u1WordAddrNum ,
                              u4WordAddr,
                              pu1Buf,
                              u2ByteCntl
                              );
		ASSERT( (i4Val ==1) || (i4Val==0) );	//0:fail, 1:successfuly get one byte
		i4Ret = (i4Val? RMR_OK: -1);
            break;
        }
#if 0
        case DRV_CUSTOM_MISC_TYPE_LIGHT_GUIDE:
        {
            UINT32 u4LightGuide;

            u4LightGuide = *((UINT32 *)pv_set_info);
            if (u4LightGuide >= 100)
            {
                PDWNC_WRITE32(REG_RW_PDWNC_MISC, PDWNC_READ32(REG_RW_PDWNC_MISC) & (~(1U << 2)));
                GPIO_SetOut(202, 1);
            }
            else if (u4LightGuide >= 50)
            {
                GPIO_SetOut(202, 0);
                PDWNC_WRITE32(REG_RW_PDWNC_MISC, PDWNC_READ32(REG_RW_PDWNC_MISC) | (1U << 2));
                PDWNC_WRITE32(REG_RW_PDWNC_MISC, PDWNC_READ32(REG_RW_PDWNC_MISC) & (~PD_MISC_SYSLED_CFG));
            }
            else if (u4LightGuide > 0)
            {
                GPIO_SetOut(202, 0);
                PDWNC_WRITE32(REG_RW_PDWNC_MISC, PDWNC_READ32(REG_RW_PDWNC_MISC) | (1U << 2));
                PDWNC_WRITE32(REG_RW_PDWNC_MISC, PDWNC_READ32(REG_RW_PDWNC_MISC) | PD_MISC_SYSLED_CFG);
            }
            else // if (u4LightGuide == 0)
            {
                PDWNC_WRITE32(REG_RW_PDWNC_MISC, PDWNC_READ32(REG_RW_PDWNC_MISC) & (~(1U << 2)));
                GPIO_SetOut(202, 0);
            }
            break;
        }
#endif
        case DRV_CUSTOM_MISC_TYPE_PALNEL_MODE:
        {
            UINT32 u4Value ;
            prPanel = (PANEL_SELECTION_CONFIG_T *)pv_set_info;

            if ((DRVCUST_PanelQuery(ePanelIndexOffset1, &u4Value) == 0))
            {
                i4Ret = EEPROM_Write(u4Value, (UINT32)(&(prPanel->ui4_panel_id)), 1);
            }
            else
            {
                return RMR_DRV_INV_SET_INFO ;
            }
#ifdef PANEL_INDEX_FLAG_OFFSET
            i4Ret = DRVCUSTOM_Eeprom_Write(PANEL_INDEX_FLAG_OFFSET, (UINT8)((prPanel->e_selection_mode==PANEL_SELECTION_MODE_AUTO) ? 0 : 1));
            if (i4Ret != 0)
            {
                return RMR_DRV_INV_SET_INFO;
            }
#endif
            _rPanelSelCfg.e_selection_mode = prPanel->e_selection_mode;
            _rPanelSelCfg.ui4_panel_id = prPanel->ui4_panel_id;
            i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_ALIVE_CHECK_CTRL:
        {
#ifdef CUSTOMER_FACTORY_SERVICE_MODE_ENABLE
            UINT32 ui4_value = 0;

            if ( z_size != sizeof(UINT32) )
            {
                return RMR_DRV_INV_SET_INFO;
            }

            if ( (UINT32)pv_set_info & MISC_TYPE_ALIVE_CHECK_CTRL_FACTORY_MODE )
            {
                ui4_value |= FACTORY_MODE;
            }
            if ( (UINT32)pv_set_info & MISC_TYPE_ALIVE_CHECK_CTRL_SDM_MODE )
            {
                ui4_value |= SDM_MODE;
            }
            if ( (UINT32)pv_set_info & MISC_TYPE_ALIVE_CHECK_CTRL_CSM_MODE )
            {
                ui4_value |= CSM_MODE;
            }
            if ( (UINT32)pv_set_info & MISC_TYPE_ALIVE_CHECK_CTRL_USB_MODE )
            {
                ui4_value |= USB_MODE;
            }
            if ( (UINT32)pv_set_info & MISC_TYPE_ALIVE_CHECK_CTRL_SAM_MODE )
            {
                ui4_value |= SAM_MODE;
            }

            ui4_alive_check_ctrl = ui4_value;
            PDWNC_SetupAliveCheck( MISC_TYPE_ALIVE_CHECK_CTRL_MASK_ALL, ui4_value );
#endif /* CUSTOMER_FACTORY_SERVICE_MODE_ENABLE */

            i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_HDMI_EDID:
        {
        	  #ifdef CC_Support_Sil9285
#ifndef CC_PHI_DREAMCATCHER_DANGERIOUS
        	  i4Val= vSiI9287EDIDWrite(((MISC_TYPE_HDMI_EDID_T *)pv_set_info)->aui1_NVRAMboottable_buf,((MISC_TYPE_HDMI_EDID_T *)pv_set_info)->aui1_edid_buf);
        	  i4Ret = (i4Val? RMR_OK: -1);
#endif
        	 #else
        	 #endif
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_HDMI_HPD_LOW:
        {
            vHDMIHPDAdjust(*(UINT32*)pv_set_info);
            i4Ret = RMR_OK;
            break;
        }

       #ifdef CC_BYPASS_AVI_INFOFRAME
       case DRV_CUSTOM_MISC_TYPE_HDMI_BYPASS_AVIINFO:
        {
            vHDMIDVIBypassAVIinfo(*(UINT8*)pv_set_info);
            i4Ret = RMR_OK;
            break;
        }
       #endif

       case DRV_CUSTOM_SET_CRNT_INP_SRC_INFO:
        {
#ifdef CC_SUPPORT_HDMI_FBOOTING
            fgApiEepromWriteByte(EEP_HDMI_CRNT_SOURCE,((DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T *)pv_set_info)->e_type);
            fgApiEepromWriteByte(EEP_HDMI_CRNT_PORT,((DRV_CUSTOM_MISC_CRNT_INP_SRC_INFO_T *)pv_set_info)->ui1_port_num);
#endif
            i4Ret = RMR_OK;
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_TVD_MCDONE_CNT:
        {
            vTvdSetModeDetTime(*(UINT8*)pv_set_info);
            i4Ret = RMR_OK;
            break;
        }

        case DRV_CUSTOM_MISC_SET_IMAGE_BANK:
        case DRV_CUSTOM_MISC_SET_8032UP_BANK:
        {
            DTVCFG_T    rDtvCfg;
            UINT32*     pui4_bank;

            if (EEPDTV_GetCfg(&rDtvCfg))
            {
                return RMR_DRV_SET_FAILED;
            }

            pui4_bank = (UINT32*)pv_set_info;

            if (e_misc_type == DRV_CUSTOM_MISC_SET_IMAGE_BANK)
            {
                if (*pui4_bank)
                {
                    rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_IMAGE_BANK);
                }
                else
                {
                    rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_IMAGE_BANK);
                }
            }
            else
            {
                if (*pui4_bank)
                {
                    rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_8032UP_BANK);
                }
                else
                {
                    rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_8032UP_BANK);
                }
            }
            if (EEPDTV_SetCfg(&rDtvCfg))
            {
                return RMR_DRV_SET_FAILED;
            }
            i4Ret = RMR_OK;
            break;
        }
		case DRV_CUSTOM_SET_COUNTRY:
        {
            SetTargetCountry(*(UINT8*)pv_set_info);
            i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_OSD_STATUS_CHG:
        {
#ifdef DRVCUST_OSD_STATUS_CHANGE
            BOOL    b_osd_on = FALSE;

            b_osd_on = *((BOOL *)pv_set_info);

            return DRVCUST_OSD_STATUS_CHANGE(b_osd_on);
#else
            return RMR_OK;
#endif
        }

        case  DRV_CUSTOM_DRAM_CFG_SET:
        {
            DRV_CUST_DRAM_CFG_T *prDdrCfg;
            prDdrCfg = (DRV_CUST_DRAM_CFG_T *)pv_set_info;
            if (DDR_SetCustCfg((DRAM_CFG_T *)prDdrCfg))
            {
                i4Ret = RMR_OK;
            }
            else
            {
                i4Ret = -1;
            }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS:
        {
            _fgVideoLogoFinished = *((BOOL *)pv_set_info);
            i4Ret = RMR_OK;
            break;
        }

        /* CEC Configuration Set */
        case DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_SET:
        {
            i4Ret = -1;
            if ((z_size == 1) && (u4Addr = DRVCUST_InitGet(eEepromCECEnableOffset)) != 0x00)
            {
                i4Ret = EEPROM_Write(u4Addr, (UINT32)pv_set_info, 1);
                if (i4Ret == 0)
                {
                    i4Ret = RMR_OK;
                }
            }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_SET:
        {
            i4Ret = -1;
            if ((z_size == 4) && (u4Addr = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x00)
            {
                i4Ret = EEPROM_Write(u4Addr, (UINT32)pv_set_info, 4);
                if (i4Ret == 0)
                {
                    i4Ret = RMR_OK;
                }
            }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_SET:
        {
            i4Ret = -1;
            if ((z_size == 1) && (u4Addr = DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) != 0x00)
            {
                i4Ret = EEPROM_Write(u4Addr, (UINT32)pv_set_info, 1);
                if (i4Ret == 0)
                {
                    i4Ret = RMR_OK;
                }
            }
            break;
        }

		 case DRV_CUSTOM_MISC_TYPE_NAND_ERASE:
        {
            UINT8    ui1_part_id = *((UINT8 *)pv_set_info);
            INT32    i4_ret;

            /* erase NAND flash by ID */
            i4_ret = NANDPART_Erase(ui1_part_id);

            i4Ret = (0 == i4_ret) ? RMR_OK : RMR_DRV_ERROR;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_NAND_WRITE:
        {
            MISC_TYPE_NAND_FLASH_WRITE_T*    pt_write_info = (MISC_TYPE_NAND_FLASH_WRITE_T*)pv_set_info;
            UINT64                           ui8_offset = 0;
            INT32                            i4_ret;

            ui8_offset = ((((UINT64)pt_write_info->ui4_part_id) << 32) | pt_write_info->ui4_offset);

            /* erase NAND flash by ID */
            i4_ret = NANDPART_Write(ui8_offset, (UINT32)pt_write_info->aui1_mem_buff, pt_write_info->ui4_size);

            i4Ret = (0 == i4_ret) ? RMR_OK : RMR_DRV_ERROR;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_NAND_BULK_WRITE:
        {
            MISC_TYPE_NAND_FLASH_BULK_WRITE_T* pt_write_info = (MISC_TYPE_NAND_FLASH_BULK_WRITE_T *)pv_set_info;
            UINT64                             ui8_offset = 0;
            INT32                              i4_ret;

            ui8_offset = ((((UINT64)pt_write_info->ui4_part_id) << 32) | pt_write_info->ui4_offset);

            /* erase NAND flash by ID */
            i4_ret = NANDPART_BulkWrite(ui8_offset, pt_write_info->ui1_value, pt_write_info->ui4_size);

            i4Ret = (0 == i4_ret) ? RMR_OK : RMR_DRV_ERROR;
            break;
        }

		case DRV_CUSTOM_MISC_TYPE_VDP_CB_ENABLE_SET:
        {
            MW_VDP_ENABLE_CB(*((BOOL*) pv_set_info));
            i4Ret = RMR_OK;
            break;
        }
        default:
            return RMR_DRV_INV_SET_INFO;
    }
    return i4Ret;
}

INT32 d_custom_misc_get (
    DRV_CUSTOM_MISC_TYPE_T    e_misc_type,
    VOID*                     pv_get_info,
    SIZE_T*                   pz_size )
{
    INT32 i4Val, i4Ret=-1;
    UINT8	u1AddrLen;
    #ifndef CC_Support_Sil9285
    UINT8 u1tmp;
    #endif
    UINT8 u1CHannelID; UINT16 u2ClkDiv; UINT8 u1DevAddr; UINT8 u1WordAddrNum ; UINT32 u4WordAddr; UINT8 *pu1Buf ; UINT16 u2ByteCntl;
    PANEL_SELECTION_CONFIG_T *prPanel;
    UINT32 u4Addr;
    UINT8 au1Data[4]={0,0,0,0};
    UINT32 u4Val;

    // Check input parameters
    if((pv_get_info == NULL) || (pz_size == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    // Check pz_size

    switch (e_misc_type)
    {
        case DRV_CUSTOM_MISC_TYPE_PANEL_NAME:
        {
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_TUNER_SIF:
        {
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_HDMI_EDID:
        {
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_PALNEL_MODE:
        {
            if (pz_size == NULL)
            {
                return RMR_DRV_INV_GET_INFO;
            }
            else if ((*pz_size < sizeof(PANEL_SELECTION_CONFIG_T)) || (pv_get_info==NULL))
            {
                *pz_size = sizeof(PANEL_SELECTION_CONFIG_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            *pz_size = sizeof(PANEL_SELECTION_CONFIG_T);
            break;
        }
        default:
            break;
    }

    // Implementation
    switch (e_misc_type)
    {
        case DRV_CUSTOM_MISC_TYPE_PANEL_NAME:
        {
            x_strncpy((CHAR *)pv_get_info, GetCurrentPanelName(), *pz_size);
            i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_TUNER_SIF:
        {
//             ASSERT( (((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui1_offset_len) ==0);
	    u1AddrLen	= (((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui1_offset_len) ;

	    if( u1AddrLen ==0 )
	    {
             i4Val = SIF_TunerReadNoSubAddr(0x100,
    	                 (UINT8)(((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui4_addr),
                        (UINT8 *)&(((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui1_data),
                       1);
		ASSERT( (i4Val ==1) || (i4Val==0) );	//0:fail, 1:successfuly get one byte
		*pz_size = (SIZE_T) i4Val;
		i4Ret = (i4Val? RMR_OK: -1);
	    }
	    else if( u1AddrLen > 0  )
	    {
	      i4Val = SIF_X_Read(SIF_BUS_TUNER_I2C, 0x200,
    	                 (UINT8)(((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui4_addr),
			   u1AddrLen ,
    	                 (UINT8)(((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui4_addr),
                        (UINT8 *)&(((MISC_TYPE_TUNER_SIF_T *)pv_get_info)->ui1_data),
			   1);
		ASSERT( (i4Val ==1) || (i4Val==0) );	//0:fail, 1:successfuly get one byte
		*pz_size = (SIZE_T) i4Val;
		i4Ret = (i4Val? RMR_OK: -1);
	    }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_ALIVE_CHECK_CTRL:
            {
#ifdef CUSTOMER_FACTORY_SERVICE_MODE_ENABLE
                UINT32   ui4_value = 0;

                if ( !pv_get_info || !pz_size || *pz_size != sizeof(UINT32) )
                {
                    return RMR_DRV_INV_GET_INFO;
                }

                if ( ui4_alive_check_ctrl & FACTORY_MODE )
                {
                    ui4_value |= MISC_TYPE_ALIVE_CHECK_CTRL_FACTORY_MODE;
                }
                if ( ui4_alive_check_ctrl & SDM_MODE )
                {
                    ui4_value |= MISC_TYPE_ALIVE_CHECK_CTRL_SDM_MODE;
                }
                if ( ui4_alive_check_ctrl & CSM_MODE )
                {
                    ui4_value |= MISC_TYPE_ALIVE_CHECK_CTRL_CSM_MODE;
                }
                if ( ui4_alive_check_ctrl & SAM_MODE )
                {
                    ui4_value |= MISC_TYPE_ALIVE_CHECK_CTRL_SAM_MODE;
                }
                if ( ui4_alive_check_ctrl & USB_MODE )
                {
                    ui4_value |= MISC_TYPE_ALIVE_CHECK_CTRL_USB_MODE;
                }
                *(UINT32*)pv_get_info = ui4_value;
#endif /* CUSTOMER_FACTORY_SERVICE_MODE_ENABLE */

                i4Ret = RMR_OK;
            }
            break;

        //////////////d_custom_misc_set::DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED//////////////////////////////
        case DRV_CUSTOM_MISC_TYPE_SIF_ADVANCED:
        {
            u1CHannelID = ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->ui1_channel;
            u2ClkDiv    = ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->ui2_clkdiv;
            u1DevAddr   = ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->ui4_addr;
            u1WordAddrNum = ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->ui1_offset_len;
            u4WordAddr = ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->ui4_offset;
            pu1Buf = ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->pui1_data;
            u2ByteCntl= ((MISC_TYPE_SIF_ADVANCED_T *)pv_get_info)->ui1_data_len;

            i4Val =SIF_X_Read(
                              u1CHannelID,
                              u2ClkDiv,
                              u1DevAddr,
                              u1WordAddrNum ,
                              u4WordAddr,
                              pu1Buf,
                              u2ByteCntl
                              );
		ASSERT( (i4Val ==1) || (i4Val==0) );	//0:fail, 1:successfuly get one byte
		*pz_size = (SIZE_T) i4Val;
		i4Ret = (i4Val? RMR_OK: -1);
            break;
        }
                case DRV_CUSTOM_MISC_TYPE_HDMI_EDID:
        {

        	 #ifdef CC_Support_Sil9285
        	 i4Val =vSiI9287EDIDread((((MISC_TYPE_HDMI_EDID_T *)pv_get_info)->ui1_hdmi_comp_id), (((MISC_TYPE_HDMI_EDID_T *)pv_get_info)->aui1_edid_buf));
        	 i4Ret = (i4Val? RMR_OK: -1);
        	 #else
        	 u1tmp = 0;
		i4Val = SifEDIDReadByte(
    	                 (UINT8)(((EDID_T *)pv_get_info)->u1EDID_ID),
    	                 (UINT8)(((EDID_T *)pv_get_info)->u1SubAddr),
			    &u1tmp);

             (((EDID_T *)pv_get_info)->ui1_data) = u1tmp;
		i4Ret = (i4Val? RMR_OK: -1);
		#endif
            break;
        }

	 case DRV_CUSTOM_MISC_TYPE_HDMI_KSV_VALUE:
        {
        	 #ifdef CC_Support_Sil9285
        	 i4Val=vSiI9287HDCPKSVread((((MISC_TYPE_HDMI_EDID_T *)pv_get_info)->aui1_hdmi_ksv)) ;
        	 i4Ret = (i4Val? RMR_OK: -1);
        	 #else
        	 #endif
            break;
        }

         case DRV_CUSTOM_MISC_TYPE_HDMI_HPD_LOW:
        {
            *(UINT8*)pv_get_info = (UINT8)bHDMIGetHPDAdjust();
            i4Ret = RMR_OK;
            break;
        }

        #ifdef CC_BYPASS_AVI_INFOFRAME
        case DRV_CUSTOM_MISC_TYPE_HDMI_BYPASS_AVIINFO:
        {
            *(UINT8*)pv_get_info = (UINT8)bHDMIGetDVIBypassAVIinfo();
            i4Ret = RMR_OK;
            break;
        }
        #endif

        case DRV_CUSTOM_MISC_TYPE_HDMI_DEEPCOLOR_STATUS:
        {
            *(UINT8*)pv_get_info = (UINT8)bHDMIDeepColorStatus();
            i4Ret = RMR_OK;
            break;
        }


        case DRV_CUSTOM_MISC_TYPE_TVD_MCDONE_CNT:
        {
            *(UINT8*)pv_get_info = bTvdGetModeDetTime();
            i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_MISC_TYPE_PALNEL_MODE:
        {
            prPanel = (PANEL_SELECTION_CONFIG_T *)pv_get_info;
            if (_rPanelSelCfg.e_selection_mode == PANEL_SELECTION_MODE_LAST)
            {
                prPanel->e_selection_mode = (GetFlagPanelIdInEeprom() ? PANEL_SELECTION_MODE_MANUAL : PANEL_SELECTION_MODE_AUTO);
                if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
                {
                    prPanel->ui4_panel_id = PANEL_GetEepromPanelIndex();
                }
                else
                {
                    prPanel->ui4_panel_id = GetCurrentPanelIndex();
                }
            }
            else
            {
                prPanel->e_selection_mode = _rPanelSelCfg.e_selection_mode;
                prPanel->ui4_panel_id = _rPanelSelCfg.ui4_panel_id;
            }
            i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_GET_NTPV_EEPROM_OFFSET:
        {
            UINT16 u2Tmp ;

            u2Tmp = DRVCUST_OptGet(eEepromNptvDriverOffset);
            if(u2Tmp != 0 )
            {
                   i4Ret = RMR_DRV_INV_GET_INFO ;
            }
            else
            {
                *(UINT16*)pv_get_info = u2Tmp ;
                i4Ret = RMR_OK;
            }

            break;
        }

        case DRV_CUSTOM_GET_COUNTRY:
        {

            *(UINT8*)pv_get_info =  GetTargetCountry();
                i4Ret = RMR_OK;
            break;
        }
        case DRV_CUSTOM_MISC_QUERY_IMAGE_BANK:
        case DRV_CUSTOM_MISC_QUERY_8032UP_BANK:
        {
            DTVCFG_T    rDtvCfg;

            if (EEPDTV_GetCfg(&rDtvCfg))
            {
                return RMR_DRV_GET_FAILED;
            }
            if (e_misc_type == DRV_CUSTOM_MISC_QUERY_IMAGE_BANK)
            {
                *(UINT8*)pv_get_info = ((rDtvCfg.u1Flags2 & DTVCFG_FLAG2_IMAGE_BANK) ? 1 : 0);
            }
            else
            {
                *(UINT8*)pv_get_info = ((rDtvCfg.u1Flags2 & DRV_CUSTOM_MISC_QUERY_8032UP_BANK) ? 1 : 0);
            }
            i4Ret = RMR_OK;
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_STARTUP_SOUND_STATUS:
        {
            #ifdef CC_AUD_SETUP_MELODY
            UINT8 i=0;
            while ((!AUD_IsSetupMelodyFinish()) && (bApiEepromReadByte(EEP_BOOT_MUSIC_ONOFF) & AUD_POWER_ON_MUSIC_MASK))
            {
                x_thread_delay(500);
                i++;
                if(i==12)
                {
                    i=0;
                    Printf("Start Up Aud timeout!\n");
                    break;
                }
            }
            AUD_StopSetupMelody(AUD_DEC_MAIN);
            #endif
            //*(BOOL*)pv_get_info = TRUE;
            i4Ret = RMR_OK;
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS:
        {
            UINT8 i=0;
            while(!_fgVideoLogoFinished)
            {
                x_thread_delay(500);
                i++;
                if(i>=20)
                {
                    i=0;
                    Printf("Start Up Video Logo timeout!\n");
                    break;
                }
            }
            _fgVideoLogoFinished = TRUE;
            //*(BOOL*)pv_get_info = TRUE;
            i4Ret = RMR_OK;
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_GET_T8032_VER:
        {
            UINT32 u4Val;

            if (*pz_size < 4)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            if (PDWNC_ReadSwVersion(&u4Val)==0)
            {
                x_snprintf((CHAR *)pv_get_info, *pz_size, "%03d", u4Val);
                i4Ret = RMR_OK;
            }
            break;
        }

        case  DRV_CUSTOM_DRAM_CFG_GET:
        {
            DRV_CUST_DRAM_CFG_T *prDdrCfg;
            prDdrCfg = (DRV_CUST_DRAM_CFG_T *)pv_get_info;
            if (DDR_GetCustCfg((DRAM_CFG_T *)prDdrCfg))
            {
                i4Ret = RMR_OK;
            }
            else
            {
                i4Ret = -1;
            }
            break;
        }

        /* CEC Configuration Get */
        case DRV_CUSTOM_MISC_TYPE_CEC_ENABLE_GET:
        {
            i4Ret = -1;
            if ((u4Addr = DRVCUST_InitGet(eEepromCECEnableOffset)) != 0x00)
            {
                u4Val = 0;
                i4Ret = EEPROM_Read(u4Addr, (UINT32)&u4Val, 1);
                if (i4Ret == 0)
                {
                    *((UINT8*)pv_get_info) = (UINT8)(u4Val & 0xFF);
                    *pz_size = 1;
                    i4Ret = RMR_OK;
                }
            }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_CEC_MENU_LANG_GET:
        {
            i4Ret = -1;
            if ((u4Addr = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x00)
            {
                u4Val = 0;
                i4Ret = EEPROM_Read(u4Addr, (UINT32)au1Data, 4);
                if (i4Ret == 0)
                {
                    x_memcpy((void *)pv_get_info, (void *)au1Data, 4);
                    *pz_size = 4;
                    i4Ret = RMR_OK;
                }
            }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_CEC_ONE_TOUCH_PLAY_EN_GET:
        {
            i4Ret = -1;
            if ((u4Addr = DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) != 0x00)
            {
                u4Val = 0;
                i4Ret = EEPROM_Read(u4Addr, (UINT32)&u4Val, 1);
                if (i4Ret == 0)
                {
                    *((UINT8*)pv_get_info) = (UINT8)(u4Val & 0xFF);
                    *pz_size = 1;
                    i4Ret = RMR_OK;
                }
            }
            break;
        }

        case DRV_CUSTOM_MISC_TYPE_LOADER_INFO:
        {
            UINT32 u4VerSz;
            UINT32 u4ChkSum;

            i4Ret = d_custom_misc_get_loader_info(
                        ((DRV_CUSTOM_MISC_LOADER_INFO_T*)pv_get_info)->s_version,
                        &u4VerSz,
                        &u4ChkSum);
            break;
        }

        default:
            return RMR_DRV_INV_GET_INFO;
    }
    return i4Ret;
}

INT32 d_custom_misc_get_min_max (
    DRV_CUSTOM_MISC_TYPE_T    e_misc_type,
    UINT16*                   pui2_min,
    UINT16*                   pui2_max)
{
    return RMR_OK;
}

INT32 d_custom_cust_spec_set (
    DRV_CUSTOM_CUST_SPEC_TYPE_T    e_cust_spec_type,
    VOID*                          pv_set_info,
    SIZE_T                         z_size,
    BOOL                           b_store
)
{
#ifdef DRV_CUSTOM_CUST_SPEC_SET
    return DRV_CUSTOM_CUST_SPEC_SET(e_cust_spec_type,
                                    pv_set_info,
                                    z_size,
                                    b_store);
#else

    return RMR_OK;
#endif

}

INT32 d_custom_cust_spec_get (
    DRV_CUSTOM_CUST_SPEC_TYPE_T    e_cust_spec_type,
    VOID*                          pv_get_info,
    SIZE_T*                        pz_size
)
{
#ifdef DRV_CUSTOM_CUST_SPEC_GET
    return DRV_CUSTOM_CUST_SPEC_GET(e_cust_spec_type,
                                    pv_get_info,
                                    pz_size);
#else

    return RMR_OK;
#endif
}

