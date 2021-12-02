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
 * $RCSfile: io_mtscart.c,v $
 * $Revision: #2 $
 * $Author: muran.xiang $
 * $SWAuthor: wen han $
 * $MD5HEX:  $
 *---------------------------------------------------------------------------*/

/** @file io_mtscart.c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mtscart.h"
#include <linux/mtal_ioctl.h>
#include "general.h"
#include "cb_low.h"
#include "drv_scart.h"
#include "vdo_misc.h"
#include "api_notify.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                            \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    }     \

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtscart argument error\n");      \
        return MTR_ERR_INV;                                          \
    } \

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

/**
 * @Set Scart Input Mode
 * @param  eScartMode
 	   MTSCART_AUTO,	   MTSCART_CVBS_RGB,   MTSCART_CVBS,
	   MTSCART_RGB,	   MTSCART_SV
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
static MT_RESULT_T _MTVDECEX_SCART_SetInputMode(unsigned long arg)
{
    INT32    iArg;
    MTSCART_INPUT_MODE_T eScartMode;
    UINT8    bDrvUIScartMode=SV_SCART_AUTO;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, iArg);

    eScartMode = (MTSCART_INPUT_MODE_T)iArg;

    MTSCART_PRINT(" -eScartMode = %d\n ",eScartMode);

    //add driver implementation code here
    switch(eScartMode)
    {
        case MTSCART_AUTO:
            bDrvUIScartMode = SV_SCART_AUTO;
        break;
        case MTSCART_CVBS_RGB:
            bDrvUIScartMode = SV_SCART_CVBS_RGB;
        break; 
        case MTSCART_CVBS:
            bDrvUIScartMode = SV_SCART_CVBS;
        break; 
        case MTSCART_RGB:
            bDrvUIScartMode = SV_SCART_RGB;
        break;
        case MTSCART_SV:
            bDrvUIScartMode = SV_SCART_SV;
        break;
        default:
            bDrvUIScartMode = SV_SCART_AUTO;
        break;        
    }
    vScartUIChangeInputMode(bDrvUIScartMode);

    return MTR_OK;
}

/**
* @brief callback function for pin8 status change
*@retval none
*/
static void _MTVDECEX_SCART_Pin8ChgCb(UINT8 u1Source, UINT8 u1Status)
{
    MTSCART_CB_T cbArg; 
    cbArg.u4Arg1 = u1Source - SV_VS_SCART1; // conver source id to scart id

    if(cbArg.u4Arg1 < MTSCART_ID_MAX)
    {
        switch(u1Status)
    	{
    		case SV_WS_16W_9H:
    			cbArg.u4Arg2 = MTSCART_PIN_8_ASPECT_16_9;
    			break;
	    	case SV_WS_4W_3H:
	    		cbArg.u4Arg2 =MTSCART_PIN_8_ASPECT_4_3;
	    		break;
	    	default:
	    		cbArg.u4Arg2 =MTSCART_PIN_8_NO_SIGNAL;
	    		break;
    	}
    }
    else 
    {
    	cbArg.u4Arg2 =MTSCART_PIN_8_NO_SIGNAL;
    }

    _CB_PutEvent(CB_MTAL_TVDECEX_SCART_PIN8CHG_TRIGGER, sizeof(cbArg), (void*)&cbArg);
}
/**
* @brief Register input video signal change callback function
* @param pfnFct input video signal decteion done callback function
*@retval MTR_OK          Success.
*@retval MTR_NOT_OK  Fail.
*/
static MT_RESULT_T _MTVDECEX_SCART_RegPin8ChgNfyCb(unsigned long arg)
{
    //add driver implementation code here
    NPTV_SetScartNfy(_MTVDECEX_SCART_Pin8ChgCb);
    vStartDetectFS();
    return MTR_OK;
}

/**
 * @Get Scart Pin8 Status
 * @param eScartId MTSCART_1, MTSCART_2
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
static MT_RESULT_T _MTVDECEX_SCART_GetPin8Status(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T    rArg;
    MTSCART_ID_T eScartId;
    MTSCART_PIN_8_T *pePin8Status = NULL;
    MTSCART_PIN_8_T ePin8Status;
    UINT8 u1DrvPin8Status;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    eScartId = (MTSCART_ID_T)rArg.ai4Arg[0];
    pePin8Status = (MTSCART_PIN_8_T*)rArg.ai4Arg[1];

    if(eScartId < MTSCART_ID_MAX)
    {
        u1DrvPin8Status = bDrvGetScartPin8ASR(SV_VS_SCART1+eScartId);
        switch(u1DrvPin8Status)
    	{
    		case SV_WS_16W_9H:
    			ePin8Status = MTSCART_PIN_8_ASPECT_16_9;
    			break;
	    	case SV_WS_4W_3H:
	    		ePin8Status =MTSCART_PIN_8_ASPECT_4_3;
	    		break;
	    	default:
	    		ePin8Status =MTSCART_PIN_8_NO_SIGNAL;
	    		break;
    	}
    }
    else 
    {
    	ePin8Status =MTSCART_PIN_8_NO_SIGNAL;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(pePin8Status, MTSCART_PIN_8_T);
    COPY_TO_USER_ARG(pePin8Status, ePin8Status, MTSCART_PIN_8_T);

    MTSCART_PRINT(" - eScartId = %d, ePin8Status = %d\n",
                   (int)eScartId,
                   (int)ePin8Status);
	
    return MTR_OK;
}

/**
 * @Get Scart Input Mode
 * @param  eScartMode
 	   MTSCART_CVBS_RGB, MTSCART_CVBS, MTSCART_RGB, MTSCART_SV
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
static MT_RESULT_T _MTVDECEX_SCART_GetInputMode(unsigned long arg)
{
    INT32    iArg;
    MTSCART_INPUT_MODE_T eScartMode;
    MTSCART_INPUT_MODE_T *peScartMode;
    UINT8 u1DrvScartMode;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, iArg);

    peScartMode = (MTSCART_INPUT_MODE_T*)iArg;

    u1DrvScartMode = bDrvGetScartInputMode();
    switch(u1DrvScartMode)
    {
            case SCART_COLOR_CVBS:
                    eScartMode = MTSCART_CVBS;
                    break;
            case SCART_COLOR_SV: 
                    eScartMode = MTSCART_SV;
                    break;
            case SCART_COLOR_RGB:
                    eScartMode = MTSCART_RGB;
                    break;
            case SCART_COLOR_MIX: 
            default:
                    eScartMode = MTSCART_CVBS_RGB;
                    break;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(peScartMode, MTSCART_INPUT_MODE_T);
    COPY_TO_USER_ARG(peScartMode, eScartMode, MTSCART_INPUT_MODE_T);

    return MTR_OK;
}
static MT_RESULT_T _MTVDECEX_SetScartMatrixEnable(unsigned long arg)
{
    UINT8 u1OnOff;
    u1OnOff = (UINT8)arg;
    MTSCART_PRINT(" - u1OnOff = %d\n",u1OnOff);

    bDrvSetScartMatrixEnable(u1OnOff);
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetScartMatrixYoffset(unsigned long arg)
{
    UINT8 u1Value;
    u1Value = (UINT8)arg;
    MTSCART_PRINT(" - u1Value = %d\n",u1Value);

    if(u1Value > 0x3f)
    {
        MTSCART_PRINT(" u1Value error input value (> 0x3F).\n");
        return MTR_NOT_OK;
    }
    bDrvSetScartMatrixYoffset(u1Value);
    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_SetScartMatrixRgbOffset(unsigned long arg)
{
    UINT8 u1Value;
    u1Value = (UINT8)arg;
    MTSCART_PRINT(" - u1Value = %d\n",u1Value);

    if(u1Value > 0x1f)
    {
        MTSCART_PRINT(" u1Value error input value (> 0x1F).\n");
        return MTR_NOT_OK;
    }
    bDrvSetScartMatrixRgbOffset(u1Value);
    return MTR_OK;
}


static MT_RESULT_T _MTVDECEX_SetScartRgbPorch(unsigned long arg)
{

    UINT16 u2Value;
    u2Value = (UINT16)arg;
    MTSCART_PRINT(" - ScartRgbPorch(%d)\n", u2Value);

    vHdtvSetPorch(SV_VP_MAIN, SV_HPORCH_CURRENT, u2Value);

    return MTR_OK;
}

static MT_RESULT_T _MTVDECEX_GetScartRgbPorch(unsigned long arg)
{
    INT32  iArg;
    UINT16 u2Value;
    UINT16 *pu2Value;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, iArg);

    pu2Value = (UINT16*)(MTSCART_INPUT_MODE_T*)iArg;

    u2Value = wHdtvGetPorch(SV_VP_MAIN, SV_HPORCH_CURRENT);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu2Value, UINT16);
    COPY_TO_USER_ARG(pu2Value, u2Value, UINT16);

    MTSCART_PRINT(" - GetScartRgbPorch %d\n", u2Value);

    return MTR_OK;
}



/**
 * @Get Scart HW Input Mode Status
 * @param  eScartMode
 	   MTSCART_CVBS_RGB, MTSCART_CVBS, MTSCART_RGB, MTSCART_SV
 * @retval MTR_OK
 * @retval MTR_NOT_OK
 */
static MT_RESULT_T _MTVDECEX_SCART_GetHWInputMode(unsigned long arg)
{
    INT32    iArg;
    MTSCART_INPUT_MODE_T eScartMode;
    MTSCART_INPUT_MODE_T *peScartMode;
    UINT8 u1DrvScartMode;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg);
    COPY_FROM_USER_1ARG(arg, iArg);

    peScartMode = (MTSCART_INPUT_MODE_T*)iArg;

    u1DrvScartMode = bDrvGetScartAutoModeStatus();
    switch(u1DrvScartMode)
    {
            case SCART_COLOR_CVBS:
                    eScartMode = MTSCART_CVBS;
                    break;
            case SCART_COLOR_SV: 
                    eScartMode = MTSCART_SV;
                    break;
            case SCART_COLOR_RGB:
                    eScartMode = MTSCART_RGB;
                    break;
            case SCART_COLOR_MIX: 
            default:
                    eScartMode = MTSCART_CVBS_RGB;
                    break;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG(peScartMode, MTSCART_INPUT_MODE_T);
    COPY_TO_USER_ARG(peScartMode, eScartMode, MTSCART_INPUT_MODE_T);

    return MTR_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int mtal_ioctl_mtscart(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
       INT32 i4Ret = MTR_OK;

	switch (cmd)
	{
		case MTAL_IO_SCART_SETINPMODE:
                        i4Ret = _MTVDECEX_SCART_SetInputMode(arg);
			break ;

		case MTAL_IO_SCART_REGPIN8FUNC:
                        i4Ret = _MTVDECEX_SCART_RegPin8ChgNfyCb(arg);
			break;

		case MTAL_IO_SCART_GETPIN8STE:
                        i4Ret = _MTVDECEX_SCART_GetPin8Status(arg);
			break;

		case MTAL_IO_SCART_GETINPMODE:
                        i4Ret = _MTVDECEX_SCART_GetInputMode(arg);
			break ;

		case MTAL_IO_SCART_GETHWINPMODE:
                        i4Ret = _MTVDECEX_SCART_GetHWInputMode(arg);
			break ;

		case MTAL_IO_SCART_SETSCARTMATRIXENABLE:
                        i4Ret = _MTVDECEX_SetScartMatrixEnable(arg);
			break;
			
		case MTAL_IO_SCART_SETSCARTMATRIXYOFFSET:
                        i4Ret = _MTVDECEX_SetScartMatrixYoffset(arg);
			break;
			
		case MTAL_IO_SCART_SETSCARTMATRIXRGBOFFSET:
                        i4Ret = _MTVDECEX_SetScartMatrixRgbOffset(arg);
			break;
			
    case MTAL_IO_SCART_SETSCARTRGBPORCH:
                        i4Ret =  _MTVDECEX_SetScartRgbPorch(arg);
     break;
     
    case MTAL_IO_SCART_GETSCARTRGBPORCH:
                        i4Ret =  _MTVDECEX_GetScartRgbPorch(arg);
     break;
     
		default:
                        i4Ret = MTR_PARAMETER_ERROR;
		    break;
	}

	return i4Ret;
}
