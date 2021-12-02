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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_papi_fe_vbi.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe_vbi.c
 *  PAPI kernel driver implementation: VBI slicer
 */


/**************************************************************************
 * Header Files
 *************************************************************************/
#include "papi.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>

#include "drv_vbi.h"
#include "api_notify.h"
#include "video_def.h"

/**************************************************************************
 * Local Constant/Configure Definitions
 *************************************************************************/
/// Common error code
#define TM_FAIL                -0x800

#define VBI_CB_TTX_DATA         0x01
#define VBI_CB_WSS_DATA         0x02
#define VBI_CB_VPS_DATA         0x04
#define VBI_CB_CC_DATA          0x08
#define VBI_CB_NO_TTX_DATA      0x10
#define VBI_CB_NO_WSS_DATA      0x20
#define VBI_CB_NO_VPS_DATA      0x40
#define VBI_CB_NO_CC_DATA       0x80

//WSS NO DATA Defined
#define WSS_NO_DATA_NOTIFY      0x00

//CC NO DATA Defined
#define CC_NO_DATA              0xFF

// X Valued for TimeOut ThresHold
#define X_ThresHold               30

#define MAX_VBI_LINES    36 /* line 6-22 and line 318-335 for TTX, line 23 for WSSB */

#define F1_NTSC_START      263
#define F1_PAL_START       313

#define AV_NTSC_F0_DIFF    (AV_NML_NTSC_CC_SEL0 - 21)    
#define AV_NTSC_F1_DIFF    (AV_NML_NTSC_CC_SEL1 - 21)

#define AV_PAL_F0_DIFF     (AV_NML_PAL_WSS_SEL0 -23)
#define AV_PAL_F1_DIFF     (AV_NML_PAL_WSS_SEL1 -23)

#define VBI_LINE_RANGE      30
/**************************************************************************
 * Local Type definitions
 *************************************************************************/
typedef struct 
{
    UINT32 u4LineNum;
    tmVbiSlice2_VbiData_t E_LineType;
}VBILine;

typedef struct
{
    UINT8 u1TotalLine;
    VBILine Line[MAX_VBI_LINES];
}VBILineSet;

/**************************************************************************
 * Local Macro definitions
 *************************************************************************/

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return TM_FAIL; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return TM_FAIL; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return TM_FAIL; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_FAIL; \
    }

/**************************************************************************
 * Imported functions (optional)
 *************************************************************************/

/**************************************************************************
 * Imported variables (optional)
 *************************************************************************/

/**************************************************************************
 * Local/Static variables
 *************************************************************************/
static UINT32 _u4SubScriptions = 0;
static Bool _bEn_Auto_Slicing;
static VBI_WSS_TIMEOUT_INFO _WSSTimeOutInfo = {FALSE, FALSE, X_ThresHold, WSS_NO_DATA_NOTIFY};
static VBI_CC_TIMEOUT_INFO  _CCTimeOutInfo = {FALSE, X_ThresHold, CC_NO_DATA, CC_NO_DATA };

static VBI_LINE_INFO _VbiLineInfo =
{
    FALSE,                                                                     //bEnMenuLineSet
    {.u1F0_SET = AV_NML_NTSC_CC_SEL0, .u1F1_SET = AV_NML_NTSC_CC_SEL1},        //AV_NTSC_CC
    {.u1F0_SET = AV_NML_PAL_CC_SEL0,  .u1F1_SET = AV_NML_PAL_CC_SEL1},         //AV_PAL_CC
    {.u1F0_SET = AV_NML_NTSC_WSS_SEL0,.u1F1_SET = AV_NML_NTSC_WSS_SEL1},       //AV_NTSC_WSS
    {.u1F0_SET = AV_NML_PAL_WSS_SEL0, .u1F1_SET = AV_NML_PAL_WSS_SEL1},        //AV_PAL_WSS
    {.u1F0_SET = AV_NML_PAL_VPS_SEL,  .u1F1_SET = AV_NML_PAL_VPS_SEL},         //AV_PAL_VPS
    {.u1F0_START = AV_NML_PAL_TT0_START,.u1F0_END = AV_NML_PAL_TT0_END,\
     .u1F1_START = AV_NML_PAL_TT1_START,.u1F1_END = AV_NML_PAL_TT1_END},       //AV_PAL_TT
    {.u1F0_START = AV_NML_NTSC_CC2X0_START,.u1F0_END = AV_NML_NTSC_CC2X0_END,\
     .u1F1_START = AV_NML_NTSC_CC2X1_START,.u1F1_END = AV_NML_NTSC_CC2X1_END}, //AV_NTSC_CC2X
    {.u1F0_SET = HDTV_NTSC_CC_SEL0, .u1F1_SET = HDTV_NTSC_CC_SEL1},            //HDTV_NTSC_CC
    {.u1F0_SET = HDTV_PAL_CC_SEL0,  .u1F1_SET = HDTV_PAL_CC_SEL1},             //HDTV_PAL_CC
    {.u1F0_SET = HDTV_NTSC_WSS_SEL0,.u1F1_SET = HDTV_NTSC_WSS_SEL1},           //HTTV_NTSC_WSS
    {.u1F0_SET = HDTV_PAL_WSS_SEL0, .u1F1_SET = HDTV_PAL_WSS_SEL1},            //HDTV_PAL_WSS
    {.u1F0_SET = HDTV_NTSC_WSS_PSCAN_SEL,.u1F1_SET = HDTV_NTSC_WSS_PSCAN_SEL}, //HTTV_NTSC_WSS_PSCAN
    {.u1F0_SET = HDTV_PAL_WSS_PSCAN_SEL, .u1F1_SET = HDTV_PAL_WSS_PSCAN_SEL},  //HDTV_PAL_WSS_PSAN
    {.u1F0_START = HDTV_PAL_TT0_START,.u1F0_END = HDTV_PAL_TT0_END,\
     .u1F1_START = HDTV_PAL_TT1_START,.u1F1_END = HDTV_PAL_TT1_END},           //HDTV_PAL_TT
};


/**************************************************************************
 * Global variables
 *************************************************************************/

/**************************************************************************
 * Local Functions
 *************************************************************************/
static NPTV_PFN_TTX_NFY _TTX_Cb_driver(UINT8 u1Path, UINT32 u4PktsAvail, UINT32 readptr)
{
    PAPI_FE_VBI_CB_T rFeVBICb = {0};
    
    /* no TTX data for nrFramesNoData consecutive frames */
    if (u4PktsAvail == 0) //??To DO for NO TTX Data later. 
    {
         rFeVBICb.u4Type = VBI_CB_NO_TTX_DATA;
         _CB_PutEvent(CB_PAPI_FE_VBI_TRIGGER, sizeof (PAPI_FE_VBI_CB_T),&rFeVBICb);             
    }
    else        
    {    
         rFeVBICb.u4Type = VBI_CB_TTX_DATA;
         rFeVBICb.CbData.TTXInfo.u4TTX_Type = tmVbiSlice2_EuroTeletext;
         rFeVBICb.CbData.TTXInfo.u4Path = u1Path;
         rFeVBICb.CbData.TTXInfo.u4PktsAvail = u4PktsAvail;
         rFeVBICb.CbData.TTXInfo.u4ReadPtr = readptr;
         
         _CB_PutEvent(CB_PAPI_FE_VBI_TRIGGER, sizeof (PAPI_FE_VBI_CB_T),&rFeVBICb);         
    }
        
    return 0;
}

/*  For WSS525 Notify Currently */
static NPTV_PFN_WSS_NFY _USWSS_Cb_driver(UINT8 u1Path, UINT32 u4Data)
{
    PAPI_FE_VBI_CB_T rFeVBICb ={0};

    /* no WSS data for nrFramesNoData consecutive frames */
    if (u4Data == WSS_NO_DATA_NOTIFY)
    {
         rFeVBICb.u4Type = VBI_CB_NO_WSS_DATA;
    }
    else
    {
         rFeVBICb.u4Type = VBI_CB_WSS_DATA;
         rFeVBICb.CbData.WSSInfo.u4Path = u1Path;
         rFeVBICb.CbData.WSSInfo.u4WSS_Type = tmVbiSlice2_UsWss;
         rFeVBICb.CbData.WSSInfo.u4WSSData = u4Data;
    }
    
    _CB_PutEvent(CB_PAPI_FE_VBI_TRIGGER, sizeof (PAPI_FE_VBI_CB_T),&rFeVBICb);    

    return 0;
}

/*  For WSS625 Notify Currently */
static NPTV_PFN_WSS_NFY _EUWSS_Cb_driver(UINT8 u1Path, UINT32 u4Data)
{
    PAPI_FE_VBI_CB_T rFeVBICb ={0};

    /* no WSS data for nrFramesNoData consecutive frames */
    if (u4Data == WSS_NO_DATA_NOTIFY)
    {
         rFeVBICb.u4Type = VBI_CB_NO_WSS_DATA;
    }
    else
    {
         rFeVBICb.u4Type = VBI_CB_WSS_DATA;
         rFeVBICb.CbData.WSSInfo.u4Path = u1Path;
         rFeVBICb.CbData.WSSInfo.u4WSS_Type = tmVbiSlice2_EuroWss;
         rFeVBICb.CbData.WSSInfo.u4WSSData = u4Data;
    }
    
    _CB_PutEvent(CB_PAPI_FE_VBI_TRIGGER, sizeof (PAPI_FE_VBI_CB_T),&rFeVBICb);    

    return 0;
}


static NPTV_PFN_VPS_NFY _VPS_Cb_driver(UINT8 au1Data[])
{
    PAPI_FE_VBI_CB_T rFeVBICb = {0};

    /* no VPS data for nrFramesNoData consecutive frames */
    if ((au1Data[0] == 0xff) && (au1Data[1] == 0xff) && (au1Data[2] == 0xff) && (au1Data[3] == 0xff))
    {
         rFeVBICb.u4Type = VBI_CB_NO_VPS_DATA;
    }
    else
    {
         rFeVBICb.u4Type = VBI_CB_VPS_DATA;
         memcpy ( (UINT8 *)rFeVBICb.CbData.VPSInfo.u1VPSData, au1Data, 13);
    }
    
    _CB_PutEvent(CB_PAPI_FE_VBI_TRIGGER, sizeof (PAPI_FE_VBI_CB_T),&rFeVBICb);    

    return 0;
}


static NPTV_PFN_CC_NFY _CC_Cb_driver(UINT8 u1Path, UINT8 u1data1, UINT8 u1data2, UINT8 u1field)
{

    PAPI_FE_VBI_CB_T rFeVBICb = {0};

    //Check Timing for define USCC / EUCC
    switch (bDrvVideoGetTiming(u1Path))
    {
        case MODE_525I:
        case MODE_525I_OVERSAMPLE:
            rFeVBICb.CbData.CCInfo.u4CC_Type = tmVbiSlice2_UsClosedCaption;
            break;
            
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            rFeVBICb.CbData.CCInfo.u4CC_Type = tmVbiSlice2_EuroClosedCaption;
            break;		
        	  
        default:
        	  return  0;
    };

    if ((u1data1 == CC_NO_DATA) && (u1data1 == CC_NO_DATA))
    {
         rFeVBICb.u4Type = VBI_CB_NO_CC_DATA;
    }
    else
    {
        rFeVBICb.u4Type = VBI_CB_CC_DATA;
    }
    rFeVBICb.CbData.CCInfo.u4Path = u1Path;
    rFeVBICb.CbData.CCInfo.u4Field   = u1field;
    rFeVBICb.CbData.CCInfo.u1CCData0 = u1data1;
    rFeVBICb.CbData.CCInfo.u1CCData1 = u1data2;

    _CB_PutEvent(CB_PAPI_FE_VBI_TRIGGER, sizeof (PAPI_FE_VBI_CB_T),&rFeVBICb);         
   
    return 0;
}



/**
* TVD: WST + VPS + WSS
* YPbPr: WSS only
**/
static INT32 _VBI_EnableAutoSlicing(u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4retval;

    UINT32 u4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    _bEn_Auto_Slicing = (Bool)rArg.au4Arg[0];
    pu4retval = (UINT32 *)rArg.au4Arg[1];

    /* API function implementation should be here */

    if(_bEn_Auto_Slicing)
    {
        NPTV_VBI_SetCcNfy(_CC_Cb_driver);
        NPTV_VBI_SetWss525Nfy(_USWSS_Cb_driver);
        NPTV_VBI_SetWssNfy(_EUWSS_Cb_driver);
        printk("papi_fe_vbi: Enable CC & WSS callback functions\n");
    }
    else
    {
        NPTV_VBI_SetCcNfy(NULL);
        NPTV_VBI_SetWss525Nfy(NULL);
        NPTV_VBI_SetWssNfy(NULL);        
        printk("papi_fe_vbi: Disable CC & WSS callback functions\n");
    }
    u4retval = TM_OK;
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));

    return TM_OK;
}

static INT32 _VBI_GetAutoSlicingEnabled(u4Arg)
{
    IOCTL_2ARG_T rArg;
    Bool* pbEnabled;
    UINT32* pu4retval;
    UINT32 u4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    pbEnabled = (BOOL *)rArg.au4Arg[0];
    pu4retval = (UINT32 *)rArg.au4Arg[1];

    /* API function implementation should be here */
    u4retval = TM_OK;
    COPY_TO_USER(pbEnabled, &_bEn_Auto_Slicing, sizeof (Bool));
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return TM_OK;
}

static INT32 _VBI_SetLine(VBILine *pVbiLine)
{
    UINT8 u1RealLine;

    switch (pVbiLine->E_LineType) 
    {
        case tmVbiSlice2_VbiNone:
            break;
        case tmVbiSlice2_VbiEuroTeletext:
        case tmVbiSlice2_VbiUsTeletext:
        case tmVbiSlice2_VbiTeletext:
        case tmVbiSlice2_VbiNabts:
        case tmVbiSlice2_VbiMoji:
        case tmVbiSlice2_VbiVps: 
        case tmVbiSlice2_VbiGemstar1x:
        case tmVbiSlice2_VbiGemstar2x:
        case tmVbiSlice2_VbiCgms:
            printk("Not Support VBI [Line][Type] = [%d][%d]\n",pVbiLine->u4LineNum, pVbiLine->E_LineType);
            return TM_ERR_NOT_STOPPED;
        
        case tmVbiSlice2_VbiEuroClosedCaption:
            if (pVbiLine->u4LineNum > F1_PAL_START) // PAL CC Field 1
            {
                u1RealLine = (pVbiLine->u4LineNum - F1_PAL_START) + AV_PAL_F1_DIFF;
                
                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_PAL_CC.u1F1_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }
            else // PAL CC Field 0 
            {
                u1RealLine = pVbiLine->u4LineNum + AV_PAL_F0_DIFF;

                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_PAL_CC.u1F0_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }

            }
            break;
            
        case tmVbiSlice2_VbiUsClosedCaption:
            if (pVbiLine->u4LineNum > F1_NTSC_START) // US CC Field 1
            {
                u1RealLine = (pVbiLine->u4LineNum - F1_NTSC_START) + AV_NTSC_F1_DIFF;
                
                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_NTSC_CC.u1F1_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }
            else // US CC Field 0 
            {
                u1RealLine = pVbiLine->u4LineNum + AV_NTSC_F0_DIFF;

                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_NTSC_CC.u1F0_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }
            
            break;

        case tmVbiSlice2_VbiEuroWss:
            if (pVbiLine->u4LineNum > F1_PAL_START) // EU WSS Field 1
            {
                u1RealLine = (pVbiLine->u4LineNum - F1_PAL_START) + AV_PAL_F1_DIFF;
                
                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_PAL_WSS.u1F1_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }
            else // EU WSS Field 0 
            {
                u1RealLine = pVbiLine->u4LineNum + AV_PAL_F0_DIFF;

                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_PAL_WSS.u1F0_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }

            break;
            
        case tmVbiSlice2_VbiUsWss:
            if (pVbiLine->u4LineNum > F1_NTSC_START) // US WSS Field 1
            {
                u1RealLine = (pVbiLine->u4LineNum - F1_NTSC_START) + AV_NTSC_F1_DIFF;
                
                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_NTSC_WSS.u1F1_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }
            else // US WSS Field 0 
            {
                u1RealLine = pVbiLine->u4LineNum + AV_NTSC_F1_DIFF;

                if ((u1RealLine < VBI_LINE_RANGE) && (u1RealLine > 0))
                {
                    _VbiLineInfo.AV_NTSC_WSS.u1F0_SET = u1RealLine;
                }
                else
                {
                    goto EXIT;
                }
            }
            
            break;

    }
    return TM_OK;

    
EXIT: 
    printk("Invalid Line setting Line VBI [Line][Type] = [%d][%d]\n",pVbiLine->u4LineNum, pVbiLine->E_LineType);

    return TM_ERR_BAD_PARAMETER;
}


static INT32 _VBI_SetVbiLines(u4Arg)
{
    UINT16 u2nrVbiLines;
    INT32 u4Ret, u4Ret2 = TM_OK;
    UINT32 u4i;

    VBILineSet VbiLineSet;

    // Copy arguments from user space
    COPY_FROM_USER(&VbiLineSet, u4Arg, sizeof (VBILineSet));

    for (u4i = 0; u4i < VbiLineSet.u1TotalLine; u4i++)
    {
        u4Ret = _VBI_SetLine( &VbiLineSet.Line[u4i] );
        if (u4Ret != TM_OK)
        {
            u4Ret2 = u4Ret;
        }
    }
    
    //Set VBI lines 
    _VbiLineInfo.bEnMenuLineSet = TRUE;
    VBI_SetLineInfo (&_VbiLineInfo);

EXIT:
    return u4Ret2;
}

static INT32 _VBI_SetSlicermode(u4Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32* pu4retval;
    UINT32 u4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    // Get arguments
    _u4SubScriptions = (UINT32)rArg.au4Arg[0];
    pu4retval = (UINT32 *)rArg.au4Arg[1];

    /* API function implementation should be here */
    /* Turn OFF all the notifications */
    VBI_StartNotifyCCOnOff(SV_VP_MAIN, FALSE, NULL);
    VBI_StartNotifyUSWSSOnOff(SV_VP_MAIN, FALSE, NULL);
    VBI_StartNotifyWSSOnOff(SV_VP_MAIN, FALSE, NULL);

    //Disable WSS TimeOut Mechanism
    _WSSTimeOutInfo.bEnWSS525TOut = FALSE;
    _WSSTimeOutInfo.bEnWSS625TOut = FALSE;

    //Disable CC TimeOut Mechanis 
    _CCTimeOutInfo.bEnCCTimeOut = FALSE;
    
    /* Turn ON which data type is set */
    if(_u4SubScriptions & papi_fe_vbisl_Wss)
    {
        VBI_StartNotifyUSWSSOnOff(SV_VP_MAIN, TRUE, NULL);        
        VBI_StartNotifyWSSOnOff(SV_VP_MAIN, TRUE, NULL);
        _WSSTimeOutInfo.bEnWSS525TOut = TRUE;
        _WSSTimeOutInfo.bEnWSS625TOut = TRUE;
    }

    if(_u4SubScriptions & papi_fe_vbisl_Cc)
    {
        VBI_StartNotifyCCOnOff(SV_VP_MAIN, TRUE, NULL);
        _CCTimeOutInfo.bEnCCTimeOut = TRUE;
    }

    VBI_SetWSSTimeOutInfo(&_WSSTimeOutInfo);
    VBI_SetCCTimeOutInfo(&_CCTimeOutInfo);

    u4retval = TM_OK;
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return TM_OK;

}

static INT32 _VBI_GetVbiLines(u4Arg)
{
    IOCTL_6ARG_T rArg;
    UINT16 u2size;
    UINT32* pu4ActNrVbiLines;
    UINT32* pu4Lines;
    tmVbiSlice2_VbiDataSet_t* ptDatatypes;
    UINT32* pu4retval;

    UINT32 u4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_6ARG_T));
    u2size = (UINT16)rArg.au4Arg[0];
    pu4ActNrVbiLines = (UINT32 *)rArg.au4Arg[1];
    pu4Lines = (UINT32 *)rArg.au4Arg[2];
    ptDatatypes = (tmVbiSlice2_VbiDataSet_t *)rArg.au4Arg[3];
    pu4retval = (UINT32 *)rArg.au4Arg[4];
    
    u4retval = TM_OK;
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return TM_OK;
}

/**************************************************************************
 * Global Functions
 *************************************************************************/

int papi_ioctl_fe_vbi(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_FE_VBISL_ENABLEAUTOSLICING:
        _VBI_EnableAutoSlicing(arg);
        break;
    case PAPI_IO_FE_VBISL_GETAUTOSLICINGENABLED:
        _VBI_GetAutoSlicingEnabled(arg);
        break;
    case PAPI_IO_FE_VBISL_GETVBILINES:
        _VBI_GetVbiLines(arg);
        break;
    case PAPI_IO_FE_VBISL_SETSLICERMODE:
        _VBI_SetSlicermode(arg);
        break;
    case PAPI_IO_FE_VBISL_SETVBILINES:
        _VBI_SetVbiLines(arg);
        break;

    default:
        printk("papi_ioctl_fe_vbi: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

