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
 * $RCSfile: io_ddigfxosd.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtgfxosd_ddi.c
 *  This file contains implementation of exported APIs of Gfx and Osd module for DDI.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mttype.h"
#include "gfx_if.h"
#include "mtgfx.h"
#include "gfxosd_ddi.h"
#include "osd_drvif.h"
#include "drvcust_if.h"
#include "fbm_drvif.h"
#include <linux/ddi_ioctl.h>

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
	UINT32 DataBuffer[256];
}DDI_OSD_PALETTE_T;

typedef struct
{
    INT32 vosdId;
    INT32 x;
    INT32 y;
    INT32 width;
    INT32 height;
    INT32 colorFormat;
    INT32 stride;
    INT32 pVOSDAddr;
}DDI_VOSD_INFO_T;

typedef struct
{
	UINT32 RegionID;
	GFXOSD_FORMAT_T			format;				
	GFXOSD_PIXELDEPTH_T		pxlDepth; 			
	UINT16					width;
	UINT16					height;	
	
}DDI_PHYOSD_SETTING_T;

enum
{
    PhyOSD0=0,
    PhyOSD1,
    MaxNumOfPhyOSD
}; 
static UINT32 PhyOSD0_list = 0;
static UINT32 PhyOSD1_list = 0;

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//static GFXOSD_VOSD_INFO_T _arVirtualOSD[4];
//static GFXOSD_INFO_T _arOSDInfo[2];
static DDI_PHYOSD_SETTING_T _aPOSDSetting[MaxNumOfPhyOSD];
static UINT32 _au4BackBuffer[MaxNumOfPhyOSD];
static UINT32 _au4FrontBuffer[MaxNumOfPhyOSD];
static UINT32 _au4VirtualOSDAddr[4];
static DDI_VOSD_INFO_T _ar4VOSDInfo[4];
static UINT32 _u4WorkingFrameBuffer;
static UINT32 _u4WorkingFrameBufferVirtual;
static UINT8 InitializeFlag;
//static DDI_OSD_PALETTE_T _au4VOSDPalette[4];
//static DDI_OSD_PALETTE_T _au4POSDPalette[MaxNumOfPhyOSD]; //we only have to maintain POSD palette
static UINT32* _apu4Palette[2];
static UINT32* _apu4PaletteVirt[2];


//-----------------------------------------------------------------------------
// Define definitions
//-----------------------------------------------------------------------------
#define ROP_SRC_OVER (7)
#define OSD_PALETTE_SIZE (1024)
#define OSD_BMP_ALIGN_SIZE (15)

#define GFX_DRIVER_ENABLE

#define GFX_FRAME_SIZE 32*1024*1024
#define GFX_FRAME_SIZE_ORDER 25

//#define OSD_SMALL_BUFFER



#ifdef OSD_SMALL_BUFFER
    #define BMP_WIDTH 960
    #define BMP_HEIGHT 540
    #define VOSD_BYTE_PER_PIXEL 2
    #define OSD_BYTE_PER_PIXEL 2
    #define OSD_PALETTE_SIZE 0
    #define NUMBER_OF_VOSD 2
    #define NUMBER_OF_POSD 2
    #define VOSD_CM GFXOSD_ARGB4444
    //#define OSD_CM GFXOSD_ARGB4444
    #define OSD1_CM GFXOSD_ARGB4444
    #define OSD2_CM GFXOSD_ARGB4444
#else
    #define BMP_WIDTH 1366
    #define BMP_HEIGHT 768
    #define VOSD_BYTE_PER_PIXEL 4
    #define OSD_BYTE_PER_PIXEL 4
    #define OSD_PALETTE_SIZE 1024
    #define NUMBER_OF_VOSD 4
    #define NUMBER_OF_POSD 2
    #define VOSD_CM GFXOSD_ARGB8888
    //#define OSD_CM GFXOSD_ARGB1555
    #define OSD1_CM GFXOSD_ARGB8888
    #define OSD2_CM GFXOSD_8BPP_PAL
#endif

#define VOSD_BUFFER_SIZE ((BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL)))
#define POSD_BACKBUFFER_SIZE ((BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL)))
#define POSD_FRONTBUFFER_SIZE ((BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL)))
#define PALETTE_SIZE 1024

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define OSD_BMP_ALIGN(X) (((X) + OSD_BMP_ALIGN_SIZE) & (~OSD_BMP_ALIGN_SIZE))

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_ci argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
    
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)             \
        if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                           sizeof(type)))                        \
        {                                                       \
            printk("mtal_ioctl_osd argument error\n");      \
            return MTR_ERR_INV;                                          \
        } 

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                    \

#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                          
    
#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }          
    #define USR_SPACE_ACCESS_VALIDATE_10ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_10ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \

#define COPY_FROM_USER_10ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_10ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_osd argument error\n");      \
        return MTR_ERR_INV;                                          \
    }        \
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


static UINT32 _GfxGetColorMode(GFXOSD_FORMAT_T rColorMode)
{
    switch (rColorMode)
    {
    case GFXOSD_ARGB8888:
        return (UINT32)CM_ARGB8888_DIRECT32;
        
    case GFXOSD_ARGB4444:
        return (UINT32)CM_ARGB4444_DIRECT16;
        
    case GFXOSD_ARGB1555:
        return (UINT32)CM_ARGB1555_DIRECT16;
        
    case GFXOSD_8BPP_PAL:
        return (UINT32)CM_RGB_CLUT8;
        
    default:
        return (UINT32)CM_ARGB8888_DIRECT32;
    }
}

static UINT32 _GfxGetBytePerPixel(GFXOSD_FORMAT_T rColorMode)
{
    switch (rColorMode)
    {
    case GFXOSD_ARGB8888:
        return (UINT32)4;
        
    case GFXOSD_ARGB4444:
        return (UINT32)2;
        
    case GFXOSD_ARGB1555:
        return (UINT32)2;
        
    case GFXOSD_8BPP_PAL:
        return (UINT32)1;

    default:
        return (UINT32)4;
    }

}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static MT_RESULT_T _MTGFXOSD_DDI_PreInit(unsigned long arg)
{
    MTGFX_BUFPROP_T rProp;
	UINT32 i,j;
	UINT32 TmpBuf, region0,region1;

	if(InitializeFlag==1)
	{
		return MTR_OK;
	}
	InitializeFlag=1;
    //get the memory from kernel
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eFBMemSize) & ~PAGE_MASK) == 0);      
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eDirectFBMemSize) & ~PAGE_MASK) == 0);      
    rProp.u4Size = DRVCUST_OptGet(eFBMemSize);
    rProp.u4Addr = BSP_GetFbmMemAddr() - (DRVCUST_OptGet(eFBMemSize) + DRVCUST_OptGet(eDirectFBMemSize)); 

    TmpBuf = OSD_BMP_ALIGN(rProp.u4Addr);

    _u4WorkingFrameBuffer = TmpBuf;

   TmpBuf += GFX_FRAME_SIZE;//(BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL));
    
    //VOSD initial
    for(i=0 ; i<NUMBER_OF_VOSD ; i++)
    {
    	_au4VirtualOSDAddr[i] = TmpBuf;
    	
    	TmpBuf += (BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL));    	
    }

    //Physical OSD : front/back buffer initial
    for(i=0 ; i<NUMBER_OF_POSD ; i++)
    {
    	_au4BackBuffer[i] = TmpBuf;
    	TmpBuf += (BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*OSD_BYTE_PER_PIXEL));	
    }
    for(i=0 ; i<NUMBER_OF_POSD ; i++)
    {
    	_au4FrontBuffer[i] = TmpBuf;  	
    	TmpBuf += (BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*OSD_BYTE_PER_PIXEL));
    	
    }

	
    //clear palette
    /*
    for(i=0 ; i<2 ; i++)
    {
    	for(j=0;j<256;j++)
    		_au4POSDPalette[i].DataBuffer[j]=0;
    }
    */
	_apu4Palette[0] = BSP_AllocAlignedDmaMemory(PALETTE_SIZE, 0x10);
    	_apu4Palette[1] = BSP_AllocAlignedDmaMemory(PALETTE_SIZE, 0x10);	
     _apu4PaletteVirt[0] = VIRTUAL(_apu4Palette[0]);
    _apu4PaletteVirt[1] = VIRTUAL(_apu4Palette[1]);
    x_memset(_apu4PaletteVirt[0],0,PALETTE_SIZE);
    x_memset(_apu4PaletteVirt[1],0,PALETTE_SIZE);
    
    //reset OSD driver
    OSD_Init();
    GFX_Init();
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_CreatePhysicalOSD(unsigned long arg)
{
		MTAL_IOCTL_8ARG_T rArg;
		UINT32 u4POSD_id,u4x,u4y,u4Width,u4Height,u4ColorMode,u4Pitch;
		UINT32 regionId;
		UINT32* pu4Address;
		
		USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);
    
    u4POSD_id = (UINT32)rArg.ai4Arg[0];
    u4x = (UINT32)rArg.ai4Arg[1];
    u4y = (UINT32)rArg.ai4Arg[2];
    u4Width = (UINT32)rArg.ai4Arg[3];
    u4Height = (UINT32)rArg.ai4Arg[4];
    u4ColorMode = (UINT32)rArg.ai4Arg[5];
    u4Pitch = (UINT32)rArg.ai4Arg[6];
    //pu4Address = (UINT32*)rArg.ai4Arg[7];
    
    if( (u4POSD_id < MaxNumOfPhyOSD)&&((u4Pitch&0x0000000f)==0) )
    {
        UINT32 u4RgnList;
        OSD_RGN_Create(&regionId, u4Width, u4Height, (void*)_au4FrontBuffer[u4POSD_id],
                       _GfxGetColorMode(u4ColorMode), u4Pitch, u4x, u4y, u4Width, u4Height);

        OSD_RGN_LIST_Create(&u4RgnList);        
        
        if(u4ColorMode==GFXOSD_8BPP_PAL)
        {
        		//OSD_RGN_Set(regionId,OSD_RGN_PAL_ADDR,&_au4POSDPalette[u4POSD_id].DataBuffer[0]);
        		OSD_RGN_Set(regionId,OSD_RGN_PAL_ADDR,_apu4Palette[u4POSD_id]);
        }
        OSD_RGN_Set(regionId, OSD_RGN_MIX_SEL,0x1);

	 if(u4POSD_id==PhyOSD0)
	 {
// change for 5395
//	 	OSD_RGN_Insert(regionId,0);
//           OSD_PLA_FlipTo(PhyOSD0+1,0);
	 	OSD_RGN_Insert(regionId,u4RgnList);
              PhyOSD0_list = u4RgnList;
		OSD_PLA_FlipTo(PhyOSD0+1, u4RgnList); 	
	 }
	 else
	 {
// change for 5395
//	 	OSD_RGN_Insert(regionId,1);
//	 	OSD_PLA_FlipTo(PhyOSD1+1,1);
	 	OSD_RGN_Insert(regionId,u4RgnList);
              PhyOSD1_list = u4RgnList;
		OSD_PLA_FlipTo(PhyOSD1+1, u4RgnList); 		 	
	 }
        _aPOSDSetting[u4POSD_id].RegionID=regionId;
        _aPOSDSetting[u4POSD_id].format=u4ColorMode;
        _aPOSDSetting[u4POSD_id].pxlDepth=u4Pitch;
        _aPOSDSetting[u4POSD_id].width=u4Width;
        _aPOSDSetting[u4POSD_id].height=u4Height;

        rArg.ai4Arg[7] = _u4WorkingFrameBufferVirtual + GFX_FRAME_SIZE +4 * VOSD_BUFFER_SIZE
        				+2*POSD_BACKBUFFER_SIZE+u4POSD_id*POSD_FRONTBUFFER_SIZE;
        x_memset(rArg.ai4Arg[7],0,POSD_FRONTBUFFER_SIZE);
    	COPY_TO_USER_ARG(arg, rArg,MTAL_IOCTL_8ARG_T);
        return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
   
}

static MT_RESULT_T _MTGFXOSD_DDI_SetPhysicalOSDPalette(unsigned long arg)
{
		MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4POSD_id;
    UINT32* pu4POSD_palette;
    UINT8 i;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    u4POSD_id = (UINT32)rArg.ai4Arg[0];
    pu4POSD_palette = (UINT32*)rArg.ai4Arg[1];
        
    if((u4POSD_id < MaxNumOfPhyOSD)&&(pu4POSD_palette!=NULL))
    {
    	//COPY_FROM_USER_ARG(pu4POSD_palette,_au4POSDPalette[u4POSD_id].DataBuffer[0],DDI_OSD_PALETTE_T);
    	memcpy(_apu4PaletteVirt[u4POSD_id],pu4POSD_palette, PALETTE_SIZE);
    	 return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
    
}

static MT_RESULT_T _MTGFXOSD_DDI_CreateVirtualOSD(unsigned long arg)
{
	MTAL_IOCTL_8ARG_T rArg;
    UINT32 u4VOSD_id;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_ARG(arg, rArg,MTAL_IOCTL_8ARG_T);
   
    u4VOSD_id = (UINT32)rArg.ai4Arg[0];

    if((u4VOSD_id < 4))
    {
        _ar4VOSDInfo[u4VOSD_id].x = rArg.ai4Arg[1];
        _ar4VOSDInfo[u4VOSD_id].y = rArg.ai4Arg[2]; 
        _ar4VOSDInfo[u4VOSD_id].width = rArg.ai4Arg[3];
        _ar4VOSDInfo[u4VOSD_id].height = rArg.ai4Arg[4];
        _ar4VOSDInfo[u4VOSD_id].colorFormat = rArg.ai4Arg[5];
        _ar4VOSDInfo[u4VOSD_id].stride = OSD_BMP_ALIGN(rArg.ai4Arg[6]);
        _ar4VOSDInfo[u4VOSD_id].pVOSDAddr = _u4WorkingFrameBufferVirtual + GFX_FRAME_SIZE + u4VOSD_id* VOSD_BUFFER_SIZE;
        
    	rArg.ai4Arg[7] = _u4WorkingFrameBufferVirtual + GFX_FRAME_SIZE + u4VOSD_id* VOSD_BUFFER_SIZE;
    	x_memset(rArg.ai4Arg[7],0,VOSD_BUFFER_SIZE);
    	COPY_TO_USER_ARG(arg, rArg,MTAL_IOCTL_8ARG_T);
    	return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
    return MTR_OK;

}

static MT_RESULT_T _MTGFXOSD_DDI_ScalePhysicalOSD(unsigned long arg)
{
    MTAL_IOCTL_10ARG_T rArg;
    UINT32 u4POSD_id,u4srcx,u4srcy,u4srcw,u4srch,u4dstx,u4dsty,u4dstw,u4dsth;
    UINT32 u4setx,u4sety,regionID;

    USR_SPACE_ACCESS_VALIDATE_10ARG(arg);
    COPY_FROM_USER_10ARG(arg, rArg);
    
    u4POSD_id = (UINT32)rArg.ai4Arg[0];
    u4srcx = (UINT32)rArg.ai4Arg[1];
    u4srcy = (UINT32)rArg.ai4Arg[2];
    u4srcw = (UINT32)rArg.ai4Arg[3];
    u4srch = (UINT32)rArg.ai4Arg[4];
    u4dstx = (UINT32)rArg.ai4Arg[5];
    u4dsty = (UINT32)rArg.ai4Arg[6];
    u4dstw = (UINT32)rArg.ai4Arg[7];
    u4dsth = (UINT32)rArg.ai4Arg[8];

	regionID=_aPOSDSetting[u4POSD_id].RegionID;
    if( u4POSD_id < MaxNumOfPhyOSD)
    {
    	if( ( u4srcw< u4dstw)&&(u4srch< u4dsth) )
    	{
    	/*
    	    OSD_RGN_SetDisplayWidth(_aPOSDSetting[u4POSD_id].RegionID,_aPOSDSetting[u4POSD_id].width);
    	    OSD_RGN_SetDisplayHeight(_aPOSDSetting[u4POSD_id].RegionID,_aPOSDSetting[u4POSD_id].height);
    	    OSD_SC_Scale(u4POSD_id,1,_aPOSDSetting[u4POSD_id].width,_aPOSDSetting[u4POSD_id].height,u4Width,u4Height);
	*/
    OSD_RGN_Set(regionID,OSD_RGN_CLIP_H,u4srcx);
    OSD_RGN_Set(regionID,OSD_RGN_CLIP_V,u4srcy);

    u4setx=(u4dstx*u4srcw)/u4dstw;
    u4sety=(u4dsty*u4srch)/u4dsth;
    OSD_RGN_Set(regionID,OSD_RGN_POS_X,u4setx);
    OSD_RGN_Set(regionID,OSD_RGN_POS_Y,u4sety);
    OSD_RGN_Set(regionID,OSD_RGN_BMP_W,u4setx+u4srcw);
    OSD_RGN_Set(regionID,OSD_RGN_BMP_H,u4sety+u4srch);
    
    OSD_RGN_SetDisplayWidth(regionID,u4setx+u4srcw);
    OSD_RGN_SetDisplayHeight(regionID,u4sety+u4srch);
    OSD_RGN_Set(regionID,OSD_RGN_BMP_W,u4srcw);
    OSD_RGN_Set(regionID,OSD_RGN_BMP_H,u4srch);

    //OSD_SC_SetSpeicalScale(u4POSD_id,u4srcw,u4srch,u4dstw,u4dsth,u4setx+u4srcw);
    	    OSD_SC_Scale(u4POSD_id,1,u4srcw,u4srch,u4dstw,u4dsth);    
    	}
    	else
    	{
    	    OSD_RGN_SetDisplayWidth(regionID,u4dstw);
    	    OSD_RGN_SetDisplayHeight(regionID,u4dsth);
    	    OSD_SC_Scale(u4POSD_id,1,0,0,0,0);
    	    OSD_RGN_Set(_aPOSDSetting[u4POSD_id].RegionID,OSD_RGN_POS_X,u4dstx);
        OSD_RGN_Set(_aPOSDSetting[u4POSD_id].RegionID,OSD_RGN_POS_Y,u4dsty);  
    	}
    	
        return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
		
}

static MT_RESULT_T _MTGFXOSD_DDI_EnablePhysicalOSD(unsigned long arg)
{
		if( (UINT32)arg < MaxNumOfPhyOSD)
    {
        OSD_PLA_Enable((arg+1),1);
        return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
}

static MT_RESULT_T _MTGFXOSD_DDI_DisablePhysicalOSD(unsigned long arg)
{
    if( (UINT32)arg < MaxNumOfPhyOSD)
    {
        OSD_PLA_Enable((arg+1),0);
        return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
}

static MT_RESULT_T _MTGFXOSD_DDI_FillRectVirtualOSD(unsigned long arg)
{
   MTAL_IOCTL_8ARG_T rArg;
    MTAL_IOCTL_8ARG_T* prArg;
    INT32 vosdId;
    INT32 opType;
    
    VOID* pvDst;
    VOID* pvSrc;
    UINT32 u4DestX,u4DestY,u4DestPitch;
    UINT32 u4SrcX,u4SrcY,u4SrcPitch;
    UINT32 u4Width,u4Height;
    UINT32 u4RectColor;
    UINT32 u4ColorMode;
    UINT32 u4Alpha;

    prArg = (MTAL_IOCTL_8ARG_T*)arg;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_8ARG_T);
    COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_8ARG_T);

    vosdId = rArg.ai4Arg[0];
    u4DestX = rArg.ai4Arg[1];
    u4DestY = rArg.ai4Arg[2];
    u4Width = rArg.ai4Arg[3];
    u4Height = rArg.ai4Arg[4];
    u4RectColor = rArg.ai4Arg[5];
    opType = rArg.ai4Arg[6];

    pvDst = _au4VirtualOSDAddr[vosdId];
    pvSrc = _au4VirtualOSDAddr[vosdId];
    u4ColorMode = _GfxGetColorMode(_ar4VOSDInfo[vosdId].colorFormat);
    u4DestPitch = (_ar4VOSDInfo[vosdId].stride);
    u4SrcPitch = (_ar4VOSDInfo[vosdId].stride);

#ifdef GFX_DRIVER_ENABLE
    //call native driver
    u4Alpha = opType & 0xFF;
    
    switch(opType)
    {
        case GFXOSD_OP_DEFAULT:
        case GFXOSD_OP_COPY_DEFAULT:    
            GFX_Lock();
            GFX_SetDst(pvDst,u4ColorMode,u4DestPitch);
            GFX_SetColor(u4RectColor);
            GFX_Fill(u4DestX, u4DestY, u4Width, u4Height);
            GFX_Unlock();
        break;

        case GFXOSD_OP_BLEND_ZERO:        
        case GFXOSD_OP_BLEND_DEFAULT:
            GFX_Lock();
        #if 1

            GFX_SetSrc(pvSrc, u4ColorMode, u4SrcPitch);    
            GFX_SetDst(pvDst, u4ColorMode, u4DestPitch);        
            GFX_SetColor(u4RectColor);    
            GFX_ComposeLoop(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Width, u4Height, 
                u4Alpha, ROP_SRC_OVER, (UINT32)FALSE);
        #else
            GFX_SetSrc(pvSrc, u4ColorMode, u4SrcPitch); 
            GFX_SetDst(pvDst, u4ColorMode, u4DestPitch);   
            GFX_SetAlpha(u4Alpha);
            GFX_Blend(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Alpha, u4Height);
        #endif
            GFX_Unlock();
        break;  

        default:
            return MTR_NOT_SUPPORTED;
        break;
        
    }

    GFX_Flush();
    GFX_Wait();
#endif

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_FillRectPhysicalOSD(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;
    MTAL_IOCTL_8ARG_T* prArg;
    INT32 osdId;
    INT32 opType;
    
    VOID* pvDst;
    VOID* pvSrc;
    UINT32 u4DestX,u4DestY,u4DestPitch;
    UINT32 u4SrcX,u4SrcY,u4SrcPitch;
    UINT32 u4Width,u4Height;
    UINT32 u4RectColor;
    UINT32 u4ColorMode;
    UINT32 u4Alpha;

    prArg = (MTAL_IOCTL_8ARG_T*)arg;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_8ARG_T);
    COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_8ARG_T);

    osdId = rArg.ai4Arg[0];
    u4DestX = rArg.ai4Arg[1];
    u4DestY = rArg.ai4Arg[2];
    u4Width = rArg.ai4Arg[3];
    u4Height = rArg.ai4Arg[4];
    u4RectColor = rArg.ai4Arg[5];
    opType = rArg.ai4Arg[6];

    pvDst = _au4FrontBuffer[osdId];
    pvSrc = _au4FrontBuffer[osdId];
    u4ColorMode = _GfxGetColorMode( _aPOSDSetting[osdId].format);
    u4DestPitch = OSD_BMP_ALIGN( _aPOSDSetting[osdId].width * _GfxGetBytePerPixel(_aPOSDSetting[osdId].format));
    u4SrcPitch = u4DestPitch;
        
#ifdef GFX_DRIVER_ENABLE
    //call native driver
    u4Alpha = opType & 0xFF;
    
    switch(opType)
    {
        case GFXOSD_OP_DEFAULT:
        case GFXOSD_OP_COPY_DEFAULT:    
            GFX_Lock();
            GFX_SetDst(pvDst,u4ColorMode,u4DestPitch);
            GFX_SetColor(u4RectColor);
            GFX_Fill(u4DestX, u4DestY, u4Width, u4Height);
            GFX_Unlock();
        break;

        case GFXOSD_OP_BLEND_ZERO:        
        case GFXOSD_OP_BLEND_DEFAULT:
            GFX_Lock();
    #if 1

            GFX_SetSrc(pvSrc, u4ColorMode, u4SrcPitch);    
            GFX_SetDst(pvDst, u4ColorMode, u4DestPitch);        
            GFX_SetColor(u4RectColor);    
            GFX_ComposeLoop(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Width, u4Height, 
                u4Alpha, ROP_SRC_OVER, (UINT32)FALSE);
    #else
            GFX_SetSrc(pvSrc, u4ColorMode, u4SrcPitch); 
            GFX_SetDst(pvDst, u4ColorMode, u4DestPitch);   
            GFX_SetAlpha(u4Alpha);
            GFX_Blend(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Alpha, u4Height);
    #endif
            GFX_Unlock();
        break;  

        default:
            return MTR_NOT_SUPPORTED;
        break;
        
    }

    GFX_Flush();
    GFX_Wait();
#endif

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_BlitAddr(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    MTAL_IOCTL_3ARG_T* prArg;
    GFXOSD_FRAME_T rDst;
    GFXOSD_FRAME_T rSrc;

    VOID* pvDst;
    VOID* pvSrc;
    UINT32 u4DstX,u4DstY,u4DstPitch;
    UINT32 u4SrcX,u4SrcY,u4SrcPitch;
    //UINT32 u4Width,u4Height;
    UINT32 u4RectColor;
    //UINT32 u4ColorMode;
    UINT32 u4SrcCM;
    UINT32 u4DstCM;
    UINT32 u4Alpha;
    UINT32 u4SrcWidth,u4SrcHeight;
    UINT32 u4DstWidth,u4DstHeight;
    
    BOOL fgSwMode = FALSE;
    GFXOSD_OP_TYPE_T opType;
    UINT32 vosdId;
    
    prArg = (MTAL_IOCTL_3ARG_T*)arg;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_3ARG_T);
    COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_3ARG_T);

    //prDst = (GFXOSD_FRAME_T*)rArg.ai4Arg[0];
    //prSrc = (GFXOSD_FRAME_T*)rArg.ai4Arg[1];
    opType = (GFXOSD_OP_TYPE_T)rArg.ai4Arg[2];
    
    COPY_FROM_USER_ARG((GFXOSD_FRAME_T*)rArg.ai4Arg[0],rDst,GFXOSD_FRAME_T);
    COPY_FROM_USER_ARG((GFXOSD_FRAME_T*)rArg.ai4Arg[1],rSrc,GFXOSD_FRAME_T);

    if(rDst.pAddr == NULL)
    {
        u4DstX = rDst.x;//_arVirtualOSD[rDst.vosdId].dispX;
        u4DstY =rDst.y; //_arVirtualOSD[rDst.vosdId].dispY;
        u4DstPitch = _ar4VOSDInfo[rDst.vosdId].stride;
        //pvDst = _ar4VOSDInfo[rDst.vosdId].pAddr;
        pvDst = _au4VirtualOSDAddr[rDst.vosdId];
        u4DstWidth =  rDst.width;//_arVirtualOSD[rDst.vosdId].width;
        u4DstHeight = rDst.height;//_arVirtualOSD[rDst.vosdId].height;
        u4DstCM = _GfxGetColorMode(_ar4VOSDInfo[rDst.vosdId].colorFormat);
    }
    else
    {
        u4DstX = rDst.x;
        u4DstY = rDst.y;
        u4DstPitch = OSD_BMP_ALIGN(rDst.stride);
        pvDst = _u4WorkingFrameBuffer+(rDst.pAddr -_u4WorkingFrameBufferVirtual) ;
        u4DstWidth = rDst.width;
        u4DstHeight = rDst.height;
        u4DstCM = _GfxGetColorMode(rDst.format);
    }
    
    if(rSrc.pAddr == NULL)
    {
        u4SrcX = rSrc.x; //_arVirtualOSD[rSrc.vosdId].dispX;
        u4SrcY = rSrc.y; //_arVirtualOSD[rSrc.vosdId].dispY;
        u4SrcPitch = _ar4VOSDInfo[rSrc.vosdId].stride;
        //pvSrc = _ar4VOSDInfo[rSrc.vosdId].pAddr;
        pvSrc = _au4VirtualOSDAddr[rSrc.vosdId];
        u4SrcWidth = rSrc.width; //_arVirtualOSD[rSrc.vosdId].width;
        u4SrcHeight = rSrc.height; //_arVirtualOSD[rSrc.vosdId].height;
        u4SrcCM = _GfxGetColorMode(_ar4VOSDInfo[rSrc.vosdId].colorFormat);
    }
    else
    {
        u4SrcX = rSrc.x;
        u4SrcY = rSrc.y;
        u4SrcPitch = OSD_BMP_ALIGN(rSrc.stride);
        pvSrc = _u4WorkingFrameBuffer + (rSrc.pAddr -_u4WorkingFrameBufferVirtual);
        u4SrcWidth = rSrc.width;
        u4SrcHeight = rSrc.height;
        u4SrcCM = _GfxGetColorMode(rSrc.format);
    }
    
    //ASSERT(_arVirtualOSD[rSrc.vosdId].format == _arVirtualOSD[rDst.vosdId].format);
    
    //u4ColorMode = _GfxGetColorMode(_arVirtualOSD[rSrc.vosdId].format);

#ifdef GFX_DRIVER_ENABLE

    u4Alpha = opType & 0xFF;
    
    switch(opType)
    {
        case GFXOSD_OP_DEFAULT:
        case GFXOSD_OP_COPY_DEFAULT:  
            GFX_Lock();
            GFX_SetSrc(pvSrc, (UINT32)u4SrcCM, u4SrcPitch);
            GFX_SetDst(pvDst, (UINT32)u4DstCM, u4DstPitch);
            //need to be confirmed
            GFX_SetBltOpt(0, 0, 0);
            
            //GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight);
            if(u4SrcCM==CM_RGB_CLUT8)	//for 8bpp color mode, we can only use Gfx strech blit
            {
                GFX_StretchBlt(u4SrcX,u4SrcY,u4SrcWidth,u4SrcHeight,
                				u4DstX,u4DstY,u4DstWidth,u4DstHeight);
            }
            else
            {
            	GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight);
            }
            GFX_Unlock();
        break;
        case GFXOSD_OP_BLEND_ZERO :
        case GFXOSD_OP_BLEND_DEFAULT :
        case GFXOSD_OP_COPY_ZERO :
            GFX_Lock();
        #if 1
            GFX_SetSrc(pvSrc, u4SrcCM, u4SrcPitch);    
            GFX_SetDst(pvDst, u4DstCM, u4DstPitch);        
            GFX_SetColor(u4RectColor);    
            GFX_ComposeLoop(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight, 
                u4Alpha, ROP_SRC_OVER, (UINT32)FALSE);
        #else
            GFX_SetSrc(pvSrc, u4SrcCM, u4SrcPitch); 
            GFX_SetDst(pvDst, u4DstCM, u4DstPitch);   
            GFX_SetAlpha(u4Alpha);
            GFX_Blend(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight);
        #endif
            GFX_Unlock();
        break;
         
        default:
            return MTR_NOT_SUPPORTED;
        break;
        
    }

    GFX_Flush();
    GFX_Wait();
#endif

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_AllocMemory(unsigned long arg)
{
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_FreeMemory(unsigned long arg)
{
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_SetVirtualAddress(unsigned long arg)
{
    _u4WorkingFrameBufferVirtual = (UINT32)(arg);
    
    return MTR_OK;
}
static MT_RESULT_T _MTGFXOSD_DDI_GetFrameBufferInfo(unsigned long arg)
{
    GFXOSD_BUFFER_INFO_T rArg;
    GFXOSD_BUFFER_INFO_T* prArg;
    
    prArg = (GFXOSD_BUFFER_INFO_T*)(arg);

    USR_SPACE_ACCESS_VALIDATE_ARG(prArg,GFXOSD_BUFFER_INFO_T);
	COPY_FROM_USER_ARG(prArg,rArg,GFXOSD_BUFFER_INFO_T);

	rArg.u4BufferSize = GFX_FRAME_SIZE + NUMBER_OF_VOSD*VOSD_BUFFER_SIZE
					+NUMBER_OF_POSD*POSD_BACKBUFFER_SIZE+NUMBER_OF_POSD*POSD_FRONTBUFFER_SIZE;
	rArg.u4BufferSizeOrder = GFX_FRAME_SIZE_ORDER;
	
	rArg.u4PhysicalAddr = _u4WorkingFrameBuffer;

	USR_SPACE_ACCESS_VALIDATE_ARG(prArg,GFXOSD_BUFFER_INFO_T);
    COPY_TO_USER_ARG(prArg,rArg,GFXOSD_BUFFER_INFO_T);
    
    return MTR_OK;
}

/********************************************************
*                     Old GfxOsd DDI
********************************************************/
#ifdef OLD_GFXOSD_DDI
static MT_RESULT_T _MTGFXOSD_DDI_Lock(void)
{
    //GFX_Lock();
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_Unlock(void)
{
    //GFX_Unlock();
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_Initialize(void)
{
	MTGFX_BUFPROP_T rProp;
	UINT32 i;
	UINT32 TmpBuf;

	if(InitializeFlag==1)
	{
		return MTR_OK;
	}
	InitializeFlag=1;
    //get the memory from kernel
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eFBMemSize) & ~PAGE_MASK) == 0);      
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eDirectFBMemSize) & ~PAGE_MASK) == 0);      
    rProp.u4Size = DRVCUST_OptGet(eFBMemSize);
    rProp.u4Addr = BSP_GetFbmMemAddr() - (DRVCUST_OptGet(eFBMemSize) + DRVCUST_OptGet(eDirectFBMemSize)); 

    TmpBuf = OSD_BMP_ALIGN(rProp.u4Addr);

    _u4WorkingFrameBuffer = TmpBuf;

   TmpBuf += GFX_FRAME_SIZE;//(BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL));
    
    //VOSD initial
    for(i=0 ; i<NUMBER_OF_VOSD ; i++)
    {
    	_arVirtualOSD[i].pAddr = TmpBuf;

    	TmpBuf += (BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL)) + OSD_PALETTE_SIZE;
    	
    	_arVirtualOSD[i].stride = OSD_BMP_ALIGN(BMP_WIDTH*VOSD_BYTE_PER_PIXEL);
    	
    	_arVirtualOSD[i].width = BMP_WIDTH;
    	_arVirtualOSD[i].height = BMP_HEIGHT;
    	_arVirtualOSD[i].format = VOSD_CM;
		_arVirtualOSD[i].alphaValue = 0xf0;
    }

    //Physical OSD : front/back buffer initial
    for(i=0 ; i<2 ; i++)
    {
        #ifndef OSD_SMALL_BUFFER
    	_au4BackBuffer[i] = TmpBuf;
    	TmpBuf += (BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*OSD_BYTE_PER_PIXEL));
    	#endif
    	
    	_au4FrontBuffer[i] = TmpBuf;
    	
    	TmpBuf += (BMP_HEIGHT*OSD_BMP_ALIGN(BMP_WIDTH*OSD_BYTE_PER_PIXEL));
    	
	if(i == 0)
	    _arOSDInfo[i].format = OSD1_CM;
	else
	    _arOSDInfo[i].format = OSD2_CM;
	
    	_arOSDInfo[i].width = BMP_WIDTH;
    	_arOSDInfo[i].height = BMP_HEIGHT;
	if(i == 0)
    		_arOSDInfo[i].pxlDepth = GFXOSD_32BPP;
	else
		_arOSDInfo[i].pxlDepth = GFXOSD_8BPP;
    }
    //Working Frame buffer

    
    //reset OSD driver
    OSD_Init();

    GFX_Init();
    
    //create region for front buffers 
    OSD_RGN_Create(&region0, BMP_WIDTH, BMP_HEIGHT, (void*)_au4FrontBuffer[0],
                       _GfxGetColorMode(OSD1_CM), OSD_BMP_ALIGN(BMP_WIDTH*_GfxGetBytePerPixel(OSD1_CM)), 0, 0, PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
                       
    OSD_RGN_Create(&region1, BMP_WIDTH, BMP_HEIGHT, (void*)_au4FrontBuffer[1],
                       _GfxGetColorMode(OSD2_CM), OSD_BMP_ALIGN(BMP_WIDTH*_GfxGetBytePerPixel(OSD2_CM)), 0, 0, PANEL_GetPanelWidth(), PANEL_GetPanelHeight());

    
    //use palette0 as default palette and initial palette0
    for(i=0;i<256;i++)
    {
    	_au4VOSDPalette[1].DataBuffer[i]=0;
    }
    OSD_RGN_Set(region1,OSD_RGN_PAL_ADDR,&_au4VOSDPalette[1].DataBuffer[0]);

    
    OSD_RGN_Set(region0, OSD_RGN_MIX_SEL,0x1);
    //OSD_PLA_SetFading(1, 0xF0);
    OSD_RGN_Set(region1, OSD_RGN_MIX_SEL,0x1);
    
    OSD_RGN_Insert(region0,0);
    OSD_RGN_Insert(region1,1);

    //OSD_PLA_FlipTo(0,0);
    //OSD_PLA_FlipTo(1,1);  
    OSD_PLA_FlipTo(1,0);
    OSD_PLA_FlipTo(2,1);  
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_Finalize(void)
{
   //reset OSD driver
    OSD_Init();
    
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_GetOSDInfo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4PhyOSD_id;
    GFXOSD_INFO_T* pu4PhyOSD_Info;
	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
   
    u4PhyOSD_id = (UINT32)rArg.ai4Arg[0];
    pu4PhyOSD_Info = (GFXOSD_INFO_T*)rArg.ai4Arg[1];
    
    if(u4PhyOSD_id < 2)
    {
        COPY_TO_USER_ARG(pu4PhyOSD_Info, _arOSDInfo[u4PhyOSD_id],GFXOSD_INFO_T);
    return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
}


static MT_RESULT_T _MTGFXOSD_DDI_Enable(unsigned long arg)
{
    
    if( (UINT32)arg < 2)
    {
        OSD_PLA_Enable((arg+1),1);
        return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
    
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_Disable(unsigned long arg)
{
    
    if( (UINT32)arg < 2)
    {
        OSD_PLA_Enable((arg+1),0);
        return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
    
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_GetVirtualOSDInfo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VOSD_id;
    GFXOSD_VOSD_INFO_T* pu4VOSD_Info;
    GFXOSD_VOSD_INFO_T _rTemp;
	
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
   
    u4VOSD_id = (UINT32)rArg.ai4Arg[0];
    pu4VOSD_Info = (GFXOSD_VOSD_INFO_T*)rArg.ai4Arg[1];
    
    x_memcpy((void*)&_rTemp,(void*)&(_arVirtualOSD[u4VOSD_id]),sizeof(GFXOSD_VOSD_INFO_T));
    _rTemp.pAddr = _u4WorkingFrameBufferVirtual + GFX_FRAME_SIZE + u4VOSD_id* VOSD_BUFFER_SIZE;
    
    if((u4VOSD_id < 4)&&(pu4VOSD_Info!=NULL))
    {
        COPY_TO_USER_ARG(pu4VOSD_Info, _rTemp,GFXOSD_VOSD_INFO_T);
    	return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
}

static MT_RESULT_T _MTGFXOSD_DDI_SetVirtualOSDInfo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VOSD_id;
    GFXOSD_VOSD_INFO_T* pu4VOSD_Info;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
   
    u4VOSD_id = (UINT32)rArg.ai4Arg[0];
    pu4VOSD_Info = (GFXOSD_VOSD_INFO_T*)rArg.ai4Arg[1];

    if((u4VOSD_id < 4)&&(pu4VOSD_Info!=NULL))
    {
    	_arVirtualOSD[u4VOSD_id].bEnable=pu4VOSD_Info->bEnable;
    	_arVirtualOSD[u4VOSD_id].osdId=pu4VOSD_Info->osdId;
    	_arVirtualOSD[u4VOSD_id].viewX=pu4VOSD_Info->viewX;
    	_arVirtualOSD[u4VOSD_id].viewY=pu4VOSD_Info->viewY;
    	_arVirtualOSD[u4VOSD_id].viewWidth=pu4VOSD_Info->viewWidth;
    	_arVirtualOSD[u4VOSD_id].viewHeight=pu4VOSD_Info->viewHeight;
    	_arVirtualOSD[u4VOSD_id].format=pu4VOSD_Info->format;
    	_arVirtualOSD[u4VOSD_id].pxlDepth=pu4VOSD_Info->pxlDepth;
    	_arVirtualOSD[u4VOSD_id].width=pu4VOSD_Info->width;
    	_arVirtualOSD[u4VOSD_id].height=pu4VOSD_Info->height;
    	_arVirtualOSD[u4VOSD_id].stride= OSD_BMP_ALIGN(pu4VOSD_Info->stride);
    	_arVirtualOSD[u4VOSD_id].alphaValue=pu4VOSD_Info->alphaValue;
    	_arVirtualOSD[u4VOSD_id].pAddr=pu4VOSD_Info->pAddr;
    	return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
}


static MT_RESULT_T  _MTGFXOSD_DDI_EnableVirtualOSD(unsigned long arg)
{
    GFXOSD_VOSD_ID_T rArg;
	//GFXOSD_VOSD_ID_T* prArg;
	
    UINT32 ux, uy;
    
	//prArg = (GFXOSD_VOSD_ID_T*)arg;
	
	//USR_SPACE_ACCESS_VALIDATE_ARG(prArg,GFXOSD_VOSD_ID_T);
	//COPY_FROM_USER_ARG(prArg,rArg,GFXOSD_VOSD_ID_T);

    rArg = (GFXOSD_VOSD_ID_T)(arg);
    
    if(_arVirtualOSD[rArg].bEnable)
    {
        return MTR_OK;
    }

    _arVirtualOSD[rArg].bEnable = TRUE;
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_DisableVirtualOSD(unsigned long arg)
{
   	GFXOSD_VOSD_ID_T rArg;
	//GFXOSD_VOSD_ID_T* prArg;
    UINT32 ux, uy;
    
	//prArg = (GFXOSD_VOSD_ID_T*)arg;
	
	//USR_SPACE_ACCESS_VALIDATE_ARG(prArg,GFXOSD_VOSD_ID_T);
	//COPY_FROM_USER_ARG(prArg,rArg,GFXOSD_VOSD_ID_T);

    rArg = (GFXOSD_VOSD_ID_T)(arg);

    if(!_arVirtualOSD[rArg].bEnable)
    {
        return MTR_OK;
    }

    _arVirtualOSD[rArg].bEnable = FALSE;

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_IsVirtualOSDEnabled(unsigned long arg)
{
    GFXOSD_VOSD_ID_T rArg;
	//GFXOSD_VOSD_ID_T* prArg;
	//prArg = (GFXOSD_VOSD_ID_T*)arg;
	
    //USR_SPACE_ACCESS_VALIDATE_ARG(prArg,GFXOSD_VOSD_ID_T);
	//COPY_FROM_USER_ARG(prArg,rArg,GFXOSD_VOSD_ID_T);

    rArg = (GFXOSD_VOSD_ID_T)(arg);
    
    if(!_arVirtualOSD[rArg].bEnable)
    {
        return MTR_NOT_OK;
    }
    
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_SetVirtualOSDPalette(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT32 u4VOSD_id;
    UINT32* pu4VOSD_palette;
    UINT8 i;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    u4VOSD_id = (UINT32)rArg.ai4Arg[0];
    pu4VOSD_palette = (UINT32*)rArg.ai4Arg[1];
        
    if((u4VOSD_id < 4)&&(pu4VOSD_palette!=NULL))
    {
    	COPY_FROM_USER_ARG(rArg.ai4Arg[1],_au4VOSDPalette[u4VOSD_id].DataBuffer[0],DDI_OSD_PALETTE_T);
    	 return MTR_OK;
    }
    else
    {
        return MTR_ERR_INV;
    }
}

static MT_RESULT_T _MTGFXOSD_DDI_WritePixel(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
	MTAL_IOCTL_4ARG_T* prArg;
	INT32 vosdId,x,y,color;
	UINT8* pu1DstAddr;
	
	prArg = (MTAL_IOCTL_4ARG_T*)arg;
    
	USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_4ARG_T);
	COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_4ARG_T);

	vosdId = rArg.ai4Arg[0];
    x = rArg.ai4Arg[1];
    y = rArg.ai4Arg[2];
    color = rArg.ai4Arg[3];

    pu1DstAddr = VIRTUAL(_arVirtualOSD[vosdId].pAddr);

    pu1DstAddr[y*_arVirtualOSD[vosdId].stride + x] = color;

    
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_ReadPixel(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
	MTAL_IOCTL_4ARG_T* prArg;
	INT32 vosdId,x,y,color;
	INT32* pColor;
	UINT8* pu1DstAddr;
	
	prArg = (MTAL_IOCTL_4ARG_T*)arg;
    
	USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_4ARG_T);
	COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_4ARG_T);

	vosdId = rArg.ai4Arg[0];
    x = rArg.ai4Arg[1];
    y = rArg.ai4Arg[2];
    pColor = rArg.ai4Arg[3];
    pu1DstAddr = VIRTUAL(_arVirtualOSD[vosdId].pAddr);
    color = pu1DstAddr[y*_arVirtualOSD[vosdId].stride + x];

    USR_SPACE_ACCESS_VALIDATE_ARG(pColor,INT32);
    COPY_TO_USER_ARG(pColor,color,INT32);

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_FillRect(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;
	MTAL_IOCTL_8ARG_T* prArg;
    INT32 vosdId;
    INT32 opType;
    
    VOID* pvDst;
    VOID* pvSrc;
    UINT32 u4DestX,u4DestY,u4DestPitch;
    UINT32 u4SrcX,u4SrcY,u4SrcPitch;
    UINT32 u4Width,u4Height;
    UINT32 u4RectColor;
    UINT32 u4ColorMode;
    UINT32 u4Alpha;

	prArg = (MTAL_IOCTL_8ARG_T*)arg;
	
	USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_8ARG_T);
	COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_8ARG_T);

    vosdId = rArg.ai4Arg[0];
    u4DestX = rArg.ai4Arg[1];
    u4DestY = rArg.ai4Arg[2];
    u4Width = rArg.ai4Arg[3];
    u4Height = rArg.ai4Arg[4];
    u4RectColor = rArg.ai4Arg[5];
    opType = rArg.ai4Arg[6];

    pvDst = _arVirtualOSD[vosdId].pAddr;
    pvSrc = _arVirtualOSD[vosdId].pAddr;
    u4ColorMode = _GfxGetColorMode(_arVirtualOSD[vosdId].format);
    u4DestPitch = (_arVirtualOSD[vosdId].stride);
    u4SrcPitch = (_arVirtualOSD[vosdId].stride);

#ifdef GFX_DRIVER_ENABLE
    //call native driver
    u4Alpha = opType & 0xFF;
    
    switch(opType)
    {
        case GFXOSD_OP_DEFAULT:
        case GFXOSD_OP_COPY_DEFAULT:    
            GFX_Lock();
            GFX_SetDst(pvDst,u4ColorMode,u4DestPitch);
            GFX_SetColor(u4RectColor);
            GFX_Fill(u4DestX, u4DestY, u4Width, u4Height);
            GFX_Unlock();
        break;

        case GFXOSD_OP_BLEND_ZERO:        
        case GFXOSD_OP_BLEND_DEFAULT:
            GFX_Lock();
            #if 1

            GFX_SetSrc(pvSrc, u4ColorMode, u4SrcPitch);    
            GFX_SetDst(pvDst, u4ColorMode, u4DestPitch);        
            GFX_SetColor(u4RectColor);    
            GFX_ComposeLoop(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Width, u4Height, 
                u4Alpha, ROP_SRC_OVER, (UINT32)FALSE);
            #else
            GFX_SetSrc(pvSrc, u4ColorMode, u4SrcPitch); 
            GFX_SetDst(pvDst, u4ColorMode, u4DestPitch);   
            GFX_SetAlpha(u4Alpha);
            GFX_Blend(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Alpha, u4Height);
            #endif
            GFX_Unlock();
        break;  

        default:
            return MTR_NOT_SUPPORTED;
        break;
        
    }

    GFX_Flush();
    GFX_Wait();
#endif

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_Clear(unsigned long arg)
{
    GFXOSD_VOSD_ID_T rArg;
    //GFXOSD_VOSD_ID_T* prArg;
    VOID* pvDst;
    UINT32 u4DestX,u4DestY,u4DestPitch;
    UINT32 u4Width,u4Height;
    UINT32 u4ColorMode;

    //prArg = (GFXOSD_VOSD_ID_T*)arg;
    
    //USR_SPACE_ACCESS_VALIDATE_ARG(prArg,GFXOSD_VOSD_ID_T);
	//COPY_FROM_USER_ARG(prArg,rArg,GFXOSD_VOSD_ID_T);
    rArg = (GFXOSD_VOSD_ID_T)(arg);

    pvDst = _arVirtualOSD[rArg].pAddr;
    u4ColorMode = _GfxGetColorMode(_arVirtualOSD[rArg].format);
    u4DestPitch = _arVirtualOSD[rArg].stride;
    u4Width = _arVirtualOSD[rArg].width;
    u4Height = _arVirtualOSD[rArg].height;

#ifdef GFX_DRIVER_ENABLE    
	//call native driver
	GFX_Lock();
    GFX_SetDst(pvDst,u4ColorMode,u4DestPitch);
    GFX_SetColor(0);
    GFX_Fill(0, 0, u4Width, u4Height);
    GFX_Unlock();

    GFX_Flush();
    GFX_Wait();
#endif

    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_UpdateScreen(unsigned long arg)
{
    MTAL_IOCTL_6ARG_T rArg;
	MTAL_IOCTL_6ARG_T* prArg;
	GFXOSD_VOSD_ID_T vosdId;
    UINT32 u4SrcX,u4SrcY;
    UINT32 u4Width,u4Height;
    BOOL fgImmediate;
    UINT8 u1Idx;
    
	prArg = (MTAL_IOCTL_6ARG_T*)arg;
	USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_6ARG_T);
	COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_6ARG_T);

    vosdId = (GFXOSD_VOSD_ID_T)rArg.ai4Arg[0];
    u4SrcX = 0;//rArg.ai4Arg[1];
    u4SrcY = 0;//.ai4Arg[2];
    u4Width = BMP_WIDTH;//rArg.ai4Arg[3];
    u4Height = BMP_HEIGHT;//rArg.ai4Arg[4];
	fgImmediate = rArg.ai4Arg[5];

#ifdef GFX_DRIVER_ENABLE
	//if(!fgImmediate)
	    GFX_Lock();
	for(u1Idx =0 ; u1Idx<NUMBER_OF_VOSD; u1Idx++)
	{
	    VOID* pvSrc = _arVirtualOSD[u1Idx].pAddr;
//#ifndef OSD_SMALL_BUFFER
//	    VOID* pvDst = _au4BackBuffer[_arVirtualOSD[u1Idx].osdId];
//#else
        VOID* pvDst = _au4FrontBuffer[_arVirtualOSD[u1Idx].osdId];       
//#endif
//	    VOID* pvDst2 = _au4FrontBuffer[_arVirtualOSD[u1Idx].osdId];
	    
	    UINT32 u4ColorMode = _GfxGetColorMode(_arVirtualOSD[u1Idx].format);
	    UINT32 u4SrcPitch = _arVirtualOSD[u1Idx].stride;
	    UINT32 u4DstPitch = OSD_BMP_ALIGN(_arOSDInfo[_arVirtualOSD[u1Idx].osdId].width * _GfxGetBytePerPixel(_arOSDInfo[_arVirtualOSD[u1Idx].osdId].format));

	    if(_arVirtualOSD[u1Idx].bEnable)
	    {
	    	if(u4ColorMode==CM_RGB_CLUT8)
	    	{
	    		
	    		OSD_RGN_Set(OSD_PLA_GetFirstRegion(_arVirtualOSD[u1Idx].osdId),OSD_RGN_PAL_ADDR,&_au4VOSDPalette[u1Idx].DataBuffer[0]);
	    	}
	        //call navtive driver to perform bitblt operation
            GFX_SetSrc(pvSrc, (UINT32)u4ColorMode, u4SrcPitch);
            GFX_SetDst(pvDst, (UINT32)u4ColorMode, u4DstPitch);
            GFX_SetBltOpt(0, 0, 0);
            GFX_BitBlt(u4SrcX, u4SrcY, u4SrcX, u4SrcY, u4Width, u4Height);
/*#ifndef OSD_SMALL_BUFFER
            //copy back buffer to front buffer
            GFX_SetSrc(pvDst, (UINT32)_GfxGetColorMode(_arOSDInfo[_arVirtualOSD[u1Idx].osdId].format), u4DstPitch);
            GFX_SetDst(pvDst2, (UINT32)_GfxGetColorMode(_arOSDInfo[_arVirtualOSD[u1Idx].osdId].format), u4DstPitch);
            GFX_SetBltOpt(0, 0, 0);
            GFX_BitBlt(0, 0, 0, 0, _arOSDInfo[_arVirtualOSD[u1Idx].osdId].width, _arOSDInfo[_arVirtualOSD[u1Idx].osdId].height);
#endif
*/
	    }
	}
	//if(!fgImmediate)
        GFX_Unlock();
	GFX_Flush();
	GFX_Wait();
#endif

    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_AllocateFrame(unsigned long arg)
{
    //implement memory management in user mode
    return MTR_OK;
}

static MT_RESULT_T _MTGFXOSD_DDI_FreeFrame(unsigned long arg)
{
    //implement memory management in user mode
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_Blit(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
	MTAL_IOCTL_3ARG_T* prArg;
	GFXOSD_FRAME_T rDst;
	GFXOSD_FRAME_T rSrc;

	VOID* pvDst;
	VOID* pvSrc;
    UINT32 u4DstX,u4DstY,u4DstPitch;
    UINT32 u4SrcX,u4SrcY,u4SrcPitch;
    //UINT32 u4Width,u4Height;
    UINT32 u4RectColor;
    //UINT32 u4ColorMode;
    UINT32 u4SrcCM;
    UINT32 u4DstCM;
    UINT32 u4Alpha;
    UINT32 u4SrcWidth,u4SrcHeight;
    UINT32 u4DstWidth,u4DstHeight;
    
    BOOL fgSwMode = FALSE;
    GFXOSD_OP_TYPE_T opType;
    UINT32 vosdId;
    
	prArg = (MTAL_IOCTL_3ARG_T*)arg;
	
	USR_SPACE_ACCESS_VALIDATE_ARG(prArg,MTAL_IOCTL_3ARG_T);
	COPY_FROM_USER_ARG(prArg,rArg,MTAL_IOCTL_3ARG_T);

    //prDst = (GFXOSD_FRAME_T*)rArg.ai4Arg[0];
    //prSrc = (GFXOSD_FRAME_T*)rArg.ai4Arg[1];
    opType = (GFXOSD_OP_TYPE_T)rArg.ai4Arg[2];
    
    COPY_FROM_USER_ARG((GFXOSD_FRAME_T*)rArg.ai4Arg[0],rDst,GFXOSD_FRAME_T);
    COPY_FROM_USER_ARG((GFXOSD_FRAME_T*)rArg.ai4Arg[1],rSrc,GFXOSD_FRAME_T);

    if(rDst.pAddr == NULL)
    {
        u4DstX = rDst.x;//_arVirtualOSD[rDst.vosdId].dispX;
        u4DstY =rDst.y; //_arVirtualOSD[rDst.vosdId].dispY;
        u4DstPitch = _arVirtualOSD[rDst.vosdId].stride;
        pvDst = _arVirtualOSD[rDst.vosdId].pAddr;
        u4DstWidth =  rDst.width;//_arVirtualOSD[rDst.vosdId].width;
        u4DstHeight = rDst.height;//_arVirtualOSD[rDst.vosdId].height;
        u4DstCM = _GfxGetColorMode(_arVirtualOSD[rDst.vosdId].format);
    }
    else
    {
        u4DstX = rDst.x;
        u4DstY = rDst.y;
        u4DstPitch = rDst.stride;
        pvDst = _u4WorkingFrameBuffer+(rDst.pAddr -_u4WorkingFrameBufferVirtual) ;
        u4DstWidth = rDst.width;
        u4DstHeight = rDst.height;
        u4DstCM = _GfxGetColorMode(rDst.format);
    }
    
    if(rSrc.pAddr == NULL)
    {
        u4SrcX = rSrc.x; //_arVirtualOSD[rSrc.vosdId].dispX;
        u4SrcY = rSrc.y; //_arVirtualOSD[rSrc.vosdId].dispY;
        u4SrcPitch = _arVirtualOSD[rSrc.vosdId].stride;
        pvSrc = _arVirtualOSD[rSrc.vosdId].pAddr;
        u4SrcWidth = rSrc.width; //_arVirtualOSD[rSrc.vosdId].width;
        u4SrcHeight = rSrc.height; //_arVirtualOSD[rSrc.vosdId].height;
        u4SrcCM = _GfxGetColorMode(_arVirtualOSD[rSrc.vosdId].format);
    }
    else
    {
        u4SrcX = rSrc.x;
        u4SrcY = rSrc.y;
        u4SrcPitch = rSrc.stride;
        pvSrc = _u4WorkingFrameBuffer + (rSrc.pAddr -_u4WorkingFrameBufferVirtual);
        u4SrcWidth = rSrc.width;
        u4SrcHeight = rSrc.height;
        u4SrcCM = _GfxGetColorMode(rSrc.format);
    }
    
    //ASSERT(_arVirtualOSD[rSrc.vosdId].format == _arVirtualOSD[rDst.vosdId].format);
    
    //u4ColorMode = _GfxGetColorMode(_arVirtualOSD[rSrc.vosdId].format);

#ifdef GFX_DRIVER_ENABLE

    u4Alpha = opType & 0xFF;
    
    switch(opType)
    {
        case GFXOSD_OP_DEFAULT:
        case GFXOSD_OP_COPY_DEFAULT:  
            GFX_Lock();
            GFX_SetSrc(pvSrc, (UINT32)u4SrcCM, u4SrcPitch);
            GFX_SetDst(pvDst, (UINT32)u4DstCM, u4DstPitch);
            //need to be confirmed
            GFX_SetBltOpt(0, 0, 0);
            GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight);
            GFX_Unlock();
        break;
        case GFXOSD_OP_BLEND_ZERO :
        case GFXOSD_OP_BLEND_DEFAULT :
        case GFXOSD_OP_COPY_ZERO :
            GFX_Lock();
            #if 1
            GFX_SetSrc(pvSrc, u4SrcCM, u4SrcPitch);    
            GFX_SetDst(pvDst, u4DstCM, u4DstPitch);        
            GFX_SetColor(u4RectColor);    
            GFX_ComposeLoop(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight, 
                u4Alpha, ROP_SRC_OVER, (UINT32)FALSE);
            #else
            GFX_SetSrc(pvSrc, u4SrcCM, u4SrcPitch); 
            GFX_SetDst(pvDst, u4DstCM, u4DstPitch);   
            GFX_SetAlpha(u4Alpha);
            GFX_Blend(u4SrcX, u4SrcY, u4DstX, u4DstY, u4SrcWidth, u4SrcHeight);
            #endif
            GFX_Unlock();
        break;
         
        default:
            return MTR_NOT_SUPPORTED;
        break;
        
    }

    GFX_Flush();
    GFX_Wait();
#endif

    return MTR_OK;
    
}
#endif //#ifdef OLD_GFXOSD_DDI

static MT_RESULT_T _MTGFXOSD_DDI_StartTrueMotionDemo(unsigned long arg)
{  
    return MTR_OK;
}


static MT_RESULT_T _MTGFXOSD_DDI_StopTrueMotionDemo(unsigned long arg)
{
    return MTR_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int ddi_ioctl_gfxosd(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
       int i4Ret = 0;

	//printk("ddi_ioctl_gfxosd cmd: 0x%08x, DDI_IO_GFXOSD_LOCK: 0x%08x\n", cmd,DDI_IO_GFXOSD_LOCK);
	switch (cmd)
	{

#ifdef OLD_GFXOSD_DDI
		case DDI_IO_GFXOSD_LOCK:
			return _MTGFXOSD_DDI_Lock();
		case DDI_IO_GFXOSD_UNLOCK:
			return _MTGFXOSD_DDI_Unlock();
		case DDI_IO_GFXOSD_INIT:
		    return _MTGFXOSD_DDI_Initialize();
		case DDI_IO_GFXOSD_FINAL:
		    return _MTGFXOSD_DDI_Finalize();
		case DDI_IO_GFXOSD_GETOSDINFO:
		    return _MTGFXOSD_DDI_GetOSDInfo( arg);
		case DDI_IO_GFXOSD_ENABLE:
		    return _MTGFXOSD_DDI_Enable( arg);
		case DDI_IO_GFXOSD_DISABLE:
			return _MTGFXOSD_DDI_Disable( arg);
		case DDI_IO_GFXOSD_GETVIRTUALOSDINFO:
			return _MTGFXOSD_DDI_GetVirtualOSDInfo( arg);
		case DDI_IO_GFXOSD_SETVIRTUALOSDINFO:
		    return _MTGFXOSD_DDI_SetVirtualOSDInfo( arg);
		case DDI_IO_GFXOSD_ENABLEVIRTUALOSD:
		    return _MTGFXOSD_DDI_EnableVirtualOSD(arg);
		case DDI_IO_GFXOSD_DISABLEVIRTUALOSD:
		    return _MTGFXOSD_DDI_DisableVirtualOSD(arg);
		case DDI_IO_GFXOSD_ISVIRTUALOSDENABLED:  
		    return _MTGFXOSD_DDI_IsVirtualOSDEnabled( arg);
		case DDI_IO_GFXOSD_SETVIRTUALOSDPALETTE:
		    return _MTGFXOSD_DDI_SetVirtualOSDPalette( arg);
		case DDI_IO_GFXOSD_WRITEPIXEL:
		    return _MTGFXOSD_DDI_WritePixel( arg);
		case DDI_IO_GFXOSD_READPIXEL:
			return _MTGFXOSD_DDI_ReadPixel( arg);
		case DDI_IO_GFXOSD_FILLRECT:
		    return _MTGFXOSD_DDI_FillRect( arg);
		case DDI_IO_GFXOSD_CLEAR:
		    return _MTGFXOSD_DDI_Clear( arg);
		case DDI_IO_GFXOSD_UPDATESCREEN:
		    return _MTGFXOSD_DDI_UpdateScreen( arg);
		case DDI_IO_GFXOSD_ALLOCATEFRAME:
		    return _MTGFXOSD_DDI_AllocateFrame( arg);
		case DDI_IO_GFXOSD_FREEFRAME:
			return _MTGFXOSD_DDI_FreeFrame( arg);
		case DDI_IO_GFXOSD_BLIT:
			return _MTGFXOSD_DDI_Blit( arg);

	#endif //#ifdef OLD_GFXOSD_DDI
	
		case DDI_IO_GFXOSD_STARTTRUEMOTIONDEMO:
		    return _MTGFXOSD_DDI_StartTrueMotionDemo( arg);
		case DDI_IO_GFXOSD_STOPTRUEMOTIONDEMO:
		    return _MTGFXOSD_DDI_StopTrueMotionDemo( arg);
		    
		case DDI_IO_GFXOSD_PREINIT:
		    return _MTGFXOSD_DDI_PreInit(arg);
		case DDI_IO_GFXOSD_CREATEPHYSICALOSD:
		    return _MTGFXOSD_DDI_CreatePhysicalOSD(arg);
		case DDI_IO_GFXOSD_SETPHYSICALOSDPALETTE:
		    return _MTGFXOSD_DDI_SetPhysicalOSDPalette(arg);
		case DDI_IO_GFXOSD_CREATEVIRTUALOSD:
		    return _MTGFXOSD_DDI_CreateVirtualOSD(arg);
		case DDI_IO_GFXOSD_SCALEPHYSICALOSD:
		    return _MTGFXOSD_DDI_ScalePhysicalOSD(arg);
		case DDI_IO_GFXOSD_ENABLEPHYSICALOSD:
		    return _MTGFXOSD_DDI_EnablePhysicalOSD(arg);
		case DDI_IO_GFXOSD_DISABLEPHYSICALOSD:
		    return _MTGFXOSD_DDI_DisablePhysicalOSD(arg);
		case DDI_IO_GFXOSD_FILLRECTVIRTUALOSD:
		    return _MTGFXOSD_DDI_FillRectVirtualOSD(arg);
		case DDI_IO_GFXOSD_FILLRECTPHYSICALOSD:
		    return _MTGFXOSD_DDI_FillRectPhysicalOSD(arg);
		case DDI_IO_GFXOSD_BLITADDR:
		    return _MTGFXOSD_DDI_BlitAddr(arg);
		case DDI_IO_GFXOSD_ALLOCMEMORY:
		    return _MTGFXOSD_DDI_AllocMemory(arg);
		case DDI_IO_GFXOSD_FREEMEMORY:
		    return _MTGFXOSD_DDI_FreeMemory(arg);
		    
		case DDI_IO_GFXOSD_SETVIRTUALADDRESS:
		    return _MTGFXOSD_DDI_SetVirtualAddress(arg);
		case DDI_IO_GFXOSD_GETFRAMEBUFFERINFO:
		    return _MTGFXOSD_DDI_GetFrameBufferInfo(arg);
		default:
		    break;
	}

	return i4Ret;
}
