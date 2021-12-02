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
 * $RCSfile: io_mtvfe.c,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/

/** @file io_mtvfe.c
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtvfe.h"
#include <linux/mtal_ioctl.h>
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "nptv_if.h"
#include "x_linux.h"
#include <linux/wait.h>
#include "hw_vdoin.h"
#include "hw_hdtv.h"
#include "api_notify.h"
#include <linux/delay.h>

#define  EEPROM_SHADOW_ISSUPPORTED 0
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvfe argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                           

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvfe argument error\n");      \
        return MTR_ERR_INV;                                          \
    }  

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                           

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_pal argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static DECLARE_WAIT_QUEUE_HEAD(mtvfel_autocolor_wq);
static BOOL MTVFE_AutoColorResult;
static BOOL MTVFE_AutoColorDone;

static void MTVFE_AutoColorNotify(UINT16 ui2_id, BOOL fgSuccess)
{
	MTVFE_AutoColorResult=fgSuccess;
	MTVFE_AutoColorDone=1;
//	wake_up_interruptible(&mtvfel_autocolor_wq);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**Reset ADC module to default.
*@param  void
*@retval void
*/
static MT_RESULT_T _MTVFE_ADCSettingReset(void)
{
    UINT8 i;
    MTVFE_PRINT("- void\n");

    /* skip reset TV adc currently*/
    // vDrvTVADCDefaultSetting();

     /* this cause screen crash */
    //vDrvHDTVADCDefaultSetting(); 
    for(i=HDADC1_CHANNEL_B_Y;i<=HDADC3_CHANNEL_R_PR;i++)
    {
    	bDrvSetHDADCOffset(i,0x80);
    	bDrvSetHDADCGain(i,0);
    }
    return MTR_OK;
}

/** Do ADC calibration(autocolor)
*@param  void
*@retval void
*/
static MT_RESULT_T _MTVFE_ADCCalibration(void)
{
    INT32 i;
    MTVFE_PRINT("- void\n");
    //set notify function
    NPTV_SetVgaAutocolorNfy(MTVFE_AutoColorNotify);

    //issue auto color
    vDrvIntAutoColorStart();
    MTVFE_AutoColorDone=0;

    /*
	if(interruptible_sleep_on_timeout(&mtvfel_autocolor_wq,15*HZ)==0)
	{
		return MTR_NOT_OK;
	}
     */
     for(i=0;i<750;i++)
     {
     	msleep(20);
     	if(MTVFE_AutoColorDone) break;
     }

	return MTVFE_AutoColorResult?MTR_OK:MTR_NOT_OK;
}


/**Read ADC_Offset
*@param eChannelSel	HDADC Channel
*@retval Offset Value
*/
static MT_RESULT_T _MTVFE_GetHDADCOffset(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVFE_HDADC_CHANNEL_T eChannelSel;
    UINT8 u1AdcOffset;
    MT_RESULT_T eRet=MTR_OK;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eChannelSel = (MTVFE_HDADC_CHANNEL_T)rArg.ai4Arg[0];
	
    switch(eChannelSel)
    {
    	case HDADC1_CHANNEL_B_Y:
    		u1AdcOffset=vCust_B_Offset_Read();
    		break;
    	case HDADC2_CHANNEL_G_PB:
  		u1AdcOffset=vCust_G_Offset_Read();
    		break;
    	case HDADC3_CHANNEL_R_PR:
    		u1AdcOffset=vCust_R_Offset_Read();
    		break;
    	default:
    		u1AdcOffset=0;
    		eRet=MTR_ERR_INV;
    		break;
    }

    MTVFE_PRINT("- eChannelSel = %d, ADCOffset = %d\n",
		(int)eChannelSel,(int)u1AdcOffset);    
    rArg.ai4Arg[0] = (INT32)u1AdcOffset;
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_2ARG_T);
    return eRet;
}

/**Read ADC_Gain
*@param eChannelSel	HDADC Channel
*@retval Gain Value
*/
static MT_RESULT_T _MTVFE_GetHDADCGain(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVFE_HDADC_CHANNEL_T eChannelSel;
    UINT8 u1AdcGain;
    MT_RESULT_T eRet=MTR_OK;
	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
	
    eChannelSel = (MTVFE_HDADC_CHANNEL_T)rArg.ai4Arg[0];
	
    switch(eChannelSel)
    {
    	case HDADC1_CHANNEL_B_Y:
    		u1AdcGain=vCust_B_Gain_Read();
    		break;
    	case HDADC2_CHANNEL_G_PB:
		u1AdcGain=vCust_G_Gain_Read();
    		break;
    	case HDADC3_CHANNEL_R_PR:
    		u1AdcGain=vCust_R_Gain_Read();
    		break;
    	default:
    		u1AdcGain=0;
    		eRet=MTR_ERR_INV;
    		break;
    }	

    MTVFE_PRINT("- eChannelSel = %d, ADCGain = %d\n",
		(int)eChannelSel,(int)u1AdcGain);    
    rArg.ai4Arg[0] = (INT32)u1AdcGain;
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_2ARG_T);
    return eRet;
}


/**Write ADC_Offset
*@param eChannelSel	HDADC Channel
*@param u2Offset		Offset Value
*@retval void
*/
static MT_RESULT_T _MTVFE_SetHDADCOffset(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVFE_HDADC_CHANNEL_T eChannelSel;
    UINT16 u2Offset;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    eChannelSel = (MTVFE_HDADC_CHANNEL_T)rArg.ai4Arg[0];
    u2Offset = (UINT16)rArg.ai4Arg[1];

    MTVFE_PRINT("- eChannelSel = %d, u2Offset = %d\n",
		(int)eChannelSel, (int)u2Offset);

    u2Offset &= 0xff;// limit 8 bit

    switch(eChannelSel)
    {
    	case HDADC1_CHANNEL_B_Y:
    		bDrvSetHDADCOffset(0,u2Offset);
    		vCust_B_Offset_Update(u2Offset);
    		break;
    	case HDADC2_CHANNEL_G_PB:
    		bDrvSetHDADCOffset(1,u2Offset);
		vCust_G_Offset_Update(u2Offset);
    		break;
    	case HDADC3_CHANNEL_R_PR:
	    	bDrvSetHDADCOffset(2,u2Offset);
    		vCust_R_Offset_Update(u2Offset);
    		break;
    	default:
    		return MTR_ERR_INV;
    }		
    vCust_Gain_OFFSet_Update();

    return MTR_OK;
}

/**Write ADC_Gain
*@param eChannelSel	HDADC Channel
*@param u2Gain		Gain Value
*@retval void
*/
static MT_RESULT_T _MTVFE_SetHDADCGain(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVFE_HDADC_CHANNEL_T eChannelSel=HDADC1_CHANNEL_B_Y;
    UINT16 u2Gain=0;
	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    MTVFE_PRINT("- eChannelSel = %d, u2Gain = %d\n",
		(int)eChannelSel,
		(int)u2Gain);
	
    eChannelSel = (MTVFE_HDADC_CHANNEL_T)rArg.ai4Arg[0];
    u2Gain = (UINT16)rArg.ai4Arg[1];

    u2Gain &= 0xff;// limit 8 bit
	
    switch(eChannelSel)
    {
    	case HDADC1_CHANNEL_B_Y:
    		bDrvSetHDADCGain(0,u2Gain);
    		vCust_B_Gain_Update(u2Gain);
    		break;
    	case HDADC2_CHANNEL_G_PB:
		bDrvSetHDADCGain(1,u2Gain);
    		vCust_G_Gain_Update(u2Gain);
    		break;
    	case HDADC3_CHANNEL_R_PR:
	    	bDrvSetHDADCGain(2,u2Gain);
    		vCust_R_Gain_Update(u2Gain);
    		break;
    	default:
    		return MTR_ERR_INV;
    }	


    return MTR_OK;
}

/**Get maximum Value in a frame
*@param eChannelSel	HDADC Channel
*@retval Max value
*/
static MT_RESULT_T _MTVFE_GetADCMaxValue(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVFE_HDADC_CHANNEL_T eChannelSel;
    UINT16 u2AdcMaxVal;
    MT_RESULT_T eRet=MTR_OK;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
	
    eChannelSel = (MTVFE_HDADC_CHANNEL_T)rArg.ai4Arg[0];
	
    switch(eChannelSel)
    {
    	case HDADC1_CHANNEL_B_Y:
		u2AdcMaxVal=bDrvOnChipGetADCMaxValueRGBYPBPR(2);
    		break;
    	case HDADC2_CHANNEL_G_PB:
		u2AdcMaxVal=bDrvOnChipGetADCMaxValueRGBYPBPR(1);
    		break;
    	case HDADC3_CHANNEL_R_PR:
    		u2AdcMaxVal=bDrvOnChipGetADCMaxValueRGBYPBPR(0);
    		break;
    	default:
    		u2AdcMaxVal=0;
    		eRet=MTR_ERR_INV;
    		break;
    }

    MTVFE_PRINT("- eChannelSel = %d, ADCMaxValue = %d\n",
		(int)eChannelSel,
		(int)u2AdcMaxVal);
	
    rArg.ai4Arg[0] = (INT32)u2AdcMaxVal;
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_2ARG_T);
    return eRet;
}

/**Get balnking Value
*@param eChannelSel	HDADC Channel
*@retval Blank value
*/
static MT_RESULT_T _MTVFE_GetBlankVal(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTVFE_HDADC_CHANNEL_T eChannelSel;
    UINT16 u2AdcBlankVal;
    MT_RESULT_T eRet=MTR_OK;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);
	
    eChannelSel = (MTVFE_HDADC_CHANNEL_T)rArg.ai4Arg[0];
	
    //add driver implementation code here	
    vRegWriteFldAlign(HDTV_03, AS_BLANK_ALWAYS, HDTV_BLAK_SET);
    msleep(1);
    switch(eChannelSel)
    {
    	case HDADC1_CHANNEL_B_Y:
		u2AdcBlankVal=bDrvGetBlankVal(2);
    		break;
    	case HDADC2_CHANNEL_G_PB:
		u2AdcBlankVal=bDrvGetBlankVal(1);
    		break;
    	case HDADC3_CHANNEL_R_PR:
    		u2AdcBlankVal=bDrvGetBlankVal(0);
    		break;
    	default:
    		u2AdcBlankVal=0;
    		eRet=MTR_ERR_INV;
    		break;
    }
    vRegWriteFldAlign(HDTV_03, AS_BLANK_RESET, HDTV_BLAK_SET);
    MTVFE_PRINT("- eChannelSel = %d, BlankVal = %d\n",
		(int)eChannelSel,
		(int)u2AdcBlankVal);    
    rArg.ai4Arg[0] = (INT32)u2AdcBlankVal;
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_2ARG_T);
    return eRet;
}


static MT_RESULT_T _MTVFE_SetSlicerLevel(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 bVTL;
    UINT8 bVTH;


    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg,rArg);

    bVTL = (UINT8)rArg.ai4Arg[0];
    bVTH = (UINT8)rArg.ai4Arg[1];

    MTVFE_PRINT("- bVTL = 0x%x, bVTH = 0x%d\n",bVTL,bVTH);
    MTAL_SOGY_VTL_Set(bVTL);
    MTAL_SOGY_VTH_Set(bVTH);
    return MTR_OK;
}

static MT_RESULT_T _MTVFE_GetGainStorage(unsigned long arg)
{
    MTVDECEX_SRC_T eType;
    UINT32 *pu4Value;
    UINT32  u4Value;
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 bSrcType=0xFF;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eType = (MTVDECEX_SRC_T )rArg.ai4Arg[0];
    pu4Value = (UINT32 *)rArg.ai4Arg[1];

    switch(eType)
    {
        case MTVDECEX_SRC_COMP1:
        case MTVDECEX_SRC_COMP2:
        case MTVDECEX_SRC_COMP3:
        case MTVDECEX_SRC_COMP4:
            bSrcType = 0;
            u4Value = vCust_Current_Use_ADC_Read(bSrcType);
            break;
        case MTVDECEX_SRC_PC1:
            bSrcType = 1;
            u4Value = vCust_Current_Use_ADC_Read(bSrcType);
            break;
        case MTVDECEX_SRC_SCART1:
        case MTVDECEX_SRC_SCART2:
        case MTVDECEX_SRC_SCART3:
        case MTVDECEX_SRC_SCART4:
            bSrcType = 2;
            u4Value = vCust_Current_Use_ADC_Read(bSrcType);
            break;
        default:
            bSrcType = 0xFF;
            u4Value = 0;
            break;
    }    
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4Value,UINT32);
    COPY_TO_USER_ARG(pu4Value,u4Value,UINT32);

    MTVDECEX_PRINT(" - eType = %d, u4Value = %d\n",
		(int)eType,
		(int)u4Value);
    if(bSrcType==0xFF)
        return MTR_NOT_OK;
    else
        return MTR_OK;
}

static MT_RESULT_T _MTVFE_EBGainOverRule(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 bOnOff;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    bOnOff = (UINT8)rArg.ai4Arg[0];

    if(bOnOff)
    	vCust_EBGain_OverRule_Update(0x00);//use OTP
    else
	vCust_EBGain_OverRule_Update(0xAA);//use external ADC
    return MTR_OK;

}
static MT_RESULT_T _MTVFE_Embedded_Gain(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    UINT8 Type;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    Type = (UINT8)rArg.ai4Arg[0];

    	vCust_Embedded_Gain(Type);//0:YPbPr  1:VGA  2:SCART 
    return MTR_OK;

}
int mtal_ioctl_mtvfe(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
       int i4Ret = MTR_OK;
	   
	switch (cmd) 
	{				
		case MTAL_IO_VFE_ADCSETTINGRESET:
			return _MTVFE_ADCSettingReset();
		case MTAL_IO_VFE_ADCCALIBRATION:
			return _MTVFE_ADCCalibration();
		case MTAL_IO_VFE_GETADCMAXVALUE:
			return _MTVFE_GetADCMaxValue(arg);
		case MTAL_IO_VFE_GETBLANKVAL:
			return _MTVFE_GetBlankVal(arg);
		case MTAL_IO_VFE_GETHDADCGAIN:
			return _MTVFE_GetHDADCGain(arg);
		case MTAL_IO_VFE_GETHDADCOFFSET:
			return _MTVFE_GetHDADCOffset(arg);
		case MTAL_IO_VFE_SETHDADCGAIN:
			return _MTVFE_SetHDADCGain(arg);
		case MTAL_IO_VFE_SETHDADCOFFSET:
			return _MTVFE_SetHDADCOffset(arg);
		case MTAL_IO_VFE_SETSYNCSLICELEVEL:
		       return _MTVFE_SetSlicerLevel(arg);
		case MTAL_IO_VFE_GET_GAIN_STORAGE:
        	       return _MTVFE_GetGainStorage(arg);
 		case MTAL_IO_VFE_EBGAIN_GAIN_OVERRULE:
        	       return _MTVFE_EBGainOverRule(arg);
 		case MTAL_IO_VFE_Embedded_GAIN:
        	       return _MTVFE_Embedded_Gain(arg);
        	break;         
		default:
			return MTR_NOT_SUPPORTED;
	}
	return i4Ret;
}

