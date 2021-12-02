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
 * $Author: jiangbo.wang $
 * $Date: 2013/01/24 $
 * $RCSfile: osd_base_if.c,v $
 * $Revision: #17 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_base_if.c
 *  osd base header file, includes general osd setting
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"
#ifndef CC_MTK_LOADER
#include "drvcust_if.h"
//#include "ostg_if.h"
#include "x_os.h"
#endif
#include "fbm_drvif.h"

LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "panel.h"
LINT_EXT_HEADER_END

#include "eeprom_if.h"


#ifdef CC_TDTV_3D_OSD_ENABLE
#include "drv_tdtv_drvif.h"
#endif

#include "drv_display.h"

#ifndef OSD_LOGO_FADING_ENABLE
#define OSD_LOGO_FADING_ENABLE (1)
#endif


typedef void (* pfn_DFBMalloc)(UINT32 *u4PhyAddr, UINT32 *u4VirAddr, INT32 u4Size);
typedef void (* pfn_DFBFree)(UINT32 *u4PhyAddr, UINT32 *u4VirAddr);
EXTERN void OSD_SetMemCallback(pfn_DFBMalloc pfnMalloc, pfn_DFBFree pfnFree);


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
BOOL gfgOSD_Mirror=FALSE;
BOOL gfgOSD_Flip=FALSE;

BOOL gfgOSD_Loging = FALSE;
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4Clock = OSD_CK_XTAL;
static BOOL _fgYuvOutput = FALSE;
//#ifdef CC_MT5363
//static BOOL gfgSwap=FALSE;
//BOOL gfgSwap=FALSE;
//#endif
#ifndef CC_MTK_LOADER
static BOOL fgLogoInit=FALSE;
static HANDLE_T _hOsdWaitVsyncSema;
static UINT32 _u4FlipCnt = 0;
static BOOL _fgVsyncEn=FALSE;
static HANDLE_T _hOsdFlipToSema;
#endif

#ifdef CC_TDTV_3D_OSD_ENABLE
BOOL gfgOsd_Sbs = FALSE;
BOOL gfgOsd_Tbb = FALSE;
UINT8 _u13DOsdShift = 0;
#endif

#ifdef CC_TDTV_3D_OSD_ENABLE
static UINT8 _u1Plane2RightRegion = 0;
static UINT8 _u1Plane3RightRegion = 0;

#endif

static UINT32 _fgOsdInit = 0;
static BOOL OSD2_3DPrLshift_ENABLE_FLAG = FALSE;

pfn_DFBMalloc pfn_malloc = NULL;
pfn_DFBFree   pfn_free = NULL;

// #define USE_OSD_CK_REG

void OSD_SetMemCallback(pfn_DFBMalloc pfnMalloc, pfn_DFBFree pfnFree)
{
     pfn_malloc = pfnMalloc;
     pfn_free   = pfnFree;
}


void OSD_LockFlipTo(void)
{
#if !defined(CC_MTK_LOADER)    
    VERIFY(x_sema_lock(_hOsdFlipToSema, X_SEMA_OPTION_WAIT) ==
               OSR_OK);      
#endif               
}


void OSD_UnLockFlipTo(void)
{
#if !defined(CC_MTK_LOADER)    
    	VERIFY(x_sema_unlock(_hOsdFlipToSema) == OSR_OK);
#endif    
}


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _OsdBaseInit(void)
{
    IGNORE_RET(_OSD_BASE_SetVsWidth(1));
    IGNORE_RET(_OSD_BASE_SetOsd1HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd2HStart(0));
    //IGNORE_RET(_OSD_BASE_SetOsd3HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd1VStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd2VStart(0));
    //#ifdef CC_MT5363
    IGNORE_RET(_OSD_BASE_SetOsd3HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd3VStart(0));
    //#endif
  
	OSD_Base_SetHStart();

    return (INT32)OSD_RET_OK;
}

#include "drv_display.h"
static void _OsdGetMirrorFlipSetting(void)
{

	UINT8 u1Config;
	u1Config= u1GetFlipMirrorConfig(); 


    //if(EEPDTV_GetCfg(&rDtvCfg))                                                                                                                                   
   //{
   //     Printf(" osd Get DTV config fail\n");
   // }
    if(u1Config & SYS_FLIP_CONFIG_ON)
    {
        gfgOSD_Flip=TRUE;
        Printf(" osd flip en\n");
    }
    if(u1Config & SYS_MIRROR_CONFIG_ON)
    {
        gfgOSD_Mirror=TRUE;
        Printf(" osd mirror en\n");                                                                                                                              
     }	
    return;
}

static void _OsdSetMirrorFlipSetting(void)
{
	static UINT32 _fgOsdCnt = 10 ;
	
	if(_fgOsdCnt > 0 && gfgOSD_Flip == FALSE && gfgOSD_Mirror == FALSE)
	{
	   _OsdGetMirrorFlipSetting();
	   
	   _fgOsdCnt-- ;
	   Printf("[%s]<%d>_OsdGetMirrorFlipSetting : _fgOsdCnt = %d\n", __FUNCTION__,__LINE__,_fgOsdCnt);
	}
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

INT32 OSD_BASE_SetClock(UINT32 u4Clock)
{
	#ifdef USE_OSD_CK_REG
    if (_u4Clock != u4Clock)
    {
        _u4Clock = u4Clock;
        IO_WRITE32(OSD_CK_REG, 0, u4Clock);
    }
	#endif
	
    return (INT32)OSD_RET_OK;
}


void OSD_WaitVsync(void)
{
    #if !defined(CC_MTK_LOADER)    

    if (_fgVsyncEn)
    {    
        _u4FlipCnt++;
        
        VERIFY(x_sema_lock(_hOsdWaitVsyncSema, X_SEMA_OPTION_WAIT) ==
                   OSR_OK);
    	_u4FlipCnt --;		
    }
        
    #endif               
}


void OSD_UnlockVsyncSema(void)
{
#if !defined(CC_MTK_LOADER)    
    if (!_fgVsyncEn)
    {
        _fgVsyncEn = TRUE;
    }
    
    if (_u4FlipCnt > 0)
    {
    	VERIFY(x_sema_unlock(_hOsdWaitVsyncSema) == OSR_OK);
    }
#endif    
}

INT32 OSD_Base_SetHStart(void)
{
	UINT32	u4HSyncValue	= 0x0;
	INT32	i4HAdjValue 	= 0x0;
	INT32 i4HAdjCSRValue    = 0x0;
	
	if(!_fgOsdInit)
	{
	   return (INT32)OSD_RET_OK;
	}
	
	u4HSyncValue = wDrvGetHsyncBp();
	//Printf("OSD_Base_SetHStart test osd sync with HsyncBp ********* %d,  %d\n", u4HSyncValue,PANEL_GetHSyncWidth());

    #if defined(CC_MT5368)
    {
        i4HAdjValue = 0x294;
    }
    #elif defined(CC_MT5389)
    {
    	i4HAdjValue = 0x1ae;
		i4HAdjCSRValue = 0x1aa;
    }
	#else
    {
        #if defined(CC_MT5396)
        {
            i4HAdjValue = -2;
			i4HAdjCSRValue = 0x0;
        }
        #endif
    }
    #endif
	
	_OSD_BASE_SetScrnHStartOsd1(u4HSyncValue + i4HAdjValue);
	_OSD_BASE_SetScrnHStartOsd2(u4HSyncValue + i4HAdjValue);
	_OSD_BASE_SetScrnHStartOsd3(u4HSyncValue + i4HAdjValue);

    #if defined(CC_MT5389)||defined(CC_MT5396)
    _OSD_BASE_SetScrnHStartCSR(u4HSyncValue + i4HAdjCSRValue);
    #else
    UNUSED(i4HAdjCSRValue);
    #endif
	
	VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());

	return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 OSD_BASE_SetYuvOutput(BOOL fgYuvOutput)
{
    _fgYuvOutput = fgYuvOutput;
    IGNORE_RET(_OSD_BASE_SetYuvOut(fgYuvOutput));
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}

#if defined(CC_FAST_INIT)
#if 0
static UINT32 pm_save_fmtreg[1+(0xb8-0)/4];
static UINT32 pm_save_osd1reg[1+(0x178-0x100)/4];
static UINT32 pm_save_osd2reg[1+(0x284-0x200)/4];
static UINT32 pm_save_osd3reg[1+(0x384-0x300)/4];
static UINT32 pm_save_osd2screg[1+(0x4d8-0x400)/4];
static UINT32 pm_save_osd3screg[1+(0x5d8-0x500)/4];
static UINT32 pm_save_csrreg[1+(0x73c-0x700)/4];
#endif

static UINT32* pm_save_fmt_regs;
static UINT32* pm_save_osd1_regs;
static UINT32* pm_save_osd2_regs;
static UINT32* pm_save_osd3_regs;
static UINT32* pm_save_osd2sc_regs;
static UINT32* pm_save_osd3sc_regs;
static UINT32* pm_save_csr_regs;

#define OSD_FMT_REG_SIZE (4+(0xb8-0))
#define OSD1_REG_SIZE (4+(0x178-0x100))
#define OSD2_REG_SIZE (4+(0x284-0x200))
#define OSD3_REG_SIZE (4+(0x384-0x300))
#define OSD2SC_REG_SIZE (4+(0x4d8-0x400))
#define OSD3SC_REG_SIZE (4+(0x5d8-0x500))
#define OSD_CSR_REG_SIZE (4+(0x73c-0x700))
void OSD_pm_suspend(void)
{
    pm_save_fmt_regs = x_mem_alloc(OSD_FMT_REG_SIZE);
    pm_save_osd1_regs = x_mem_alloc(OSD1_REG_SIZE);
    pm_save_osd2_regs = x_mem_alloc(OSD2_REG_SIZE);
    pm_save_osd3_regs = x_mem_alloc(OSD3_REG_SIZE);
    pm_save_osd2sc_regs = x_mem_alloc(OSD2SC_REG_SIZE);
    pm_save_osd3sc_regs = x_mem_alloc(OSD3SC_REG_SIZE);
    pm_save_csr_regs = x_mem_alloc(OSD_CSR_REG_SIZE);

    #if 0
    x_memcpy((void*)pm_save_fmtreg,(void*)(OSD_BASE+0x0),sizeof(pm_save_fmtreg));
    x_memcpy((void*)pm_save_osd1reg,(void*)(OSD_BASE+0x100),sizeof(pm_save_osd1reg));
    x_memcpy((void*)pm_save_osd2reg,(void*)(OSD_BASE+0x200),sizeof(pm_save_osd2reg));
    x_memcpy((void*)pm_save_osd3reg,(void*)(OSD_BASE+0x300),sizeof(pm_save_osd3reg));
    x_memcpy((void*)pm_save_osd2screg,(void*)(OSD_BASE+0x400),sizeof(pm_save_osd2screg));
    x_memcpy((void*)pm_save_osd3screg,(void*)(OSD_BASE+0x500),sizeof(pm_save_osd3screg));
    x_memcpy((void*)pm_save_csrreg,(void*)(OSD_BASE+0x700),sizeof(pm_save_csrreg));
    #else
    x_memcpy((void*)pm_save_fmt_regs,(void*)(OSD_BASE+0x0),OSD_FMT_REG_SIZE);
    x_memcpy((void*)pm_save_osd1_regs,(void*)(OSD_BASE+0x100),OSD1_REG_SIZE);
    x_memcpy((void*)pm_save_osd2_regs,(void*)(OSD_BASE+0x200),OSD2_REG_SIZE);
    x_memcpy((void*)pm_save_osd3_regs,(void*)(OSD_BASE+0x300),OSD3_REG_SIZE);
    x_memcpy((void*)pm_save_osd2sc_regs,(void*)(OSD_BASE+0x400),OSD2SC_REG_SIZE);
    x_memcpy((void*)pm_save_osd3sc_regs,(void*)(OSD_BASE+0x500),OSD3SC_REG_SIZE);
    x_memcpy((void*)pm_save_csr_regs,(void*)(OSD_BASE+0x700),OSD_CSR_REG_SIZE);
    #endif
}

void OSD_pm_resume(void)
{
    //update clock
    _u4Clock = OSD_CK_XTAL;
    OSD_BASE_SetClock(OSD_CK_OCLK); //enable OSD clock

    //restore osd reg
    #if 0
    x_memcpy((void*)(OSD_BASE+0x0+8),(void*)(pm_save_fmtreg)+8,sizeof(pm_save_fmtreg)-8);
    x_memcpy((void*)(OSD_BASE+0x100+4),(void*)(pm_save_osd1reg)+4,sizeof(pm_save_osd1reg)-4);
    x_memcpy((void*)(OSD_BASE+0x200+4),(void*)(pm_save_osd2reg)+4,sizeof(pm_save_osd2reg)-4);
    x_memcpy((void*)(OSD_BASE+0x300+4),(void*)(pm_save_osd3reg)+4,sizeof(pm_save_osd3reg)-4);
    x_memcpy((void*)(OSD_BASE+0x400),(void*)(pm_save_osd2screg),sizeof(pm_save_osd2screg));
    x_memcpy((void*)(OSD_BASE+0x500),(void*)(pm_save_osd3screg),sizeof(pm_save_osd3screg));
    x_memcpy((void*)(OSD_BASE+0x700),(void*)(pm_save_csrreg),sizeof(pm_save_csrreg));

    IO_WRITE32(OSD_BASE, 0x100, pm_save_osd1reg[0]);
    IO_WRITE32(OSD_BASE, 0x200, pm_save_osd2reg[0]);
    IO_WRITE32(OSD_BASE, 0x300, pm_save_osd3reg[0]);
    IO_WRITE32(OSD_BASE, 0, pm_save_fmtreg[0]|1); //update shadow
    #else
    x_memcpy((void*)(OSD_BASE+0x0+8),(void*)(pm_save_fmt_regs)+8,OSD_FMT_REG_SIZE-8);
    x_memcpy((void*)(OSD_BASE+0x100+4),(void*)(pm_save_osd1_regs)+4,OSD1_REG_SIZE-4);
    x_memcpy((void*)(OSD_BASE+0x200+4),(void*)(pm_save_osd2_regs)+4,OSD2_REG_SIZE-4);
    x_memcpy((void*)(OSD_BASE+0x300+4),(void*)(pm_save_osd3_regs)+4,OSD3_REG_SIZE-4);
    x_memcpy((void*)(OSD_BASE+0x400),(void*)(pm_save_osd2sc_regs),OSD2SC_REG_SIZE);
    x_memcpy((void*)(OSD_BASE+0x500),(void*)(pm_save_osd3sc_regs),OSD3SC_REG_SIZE);
    x_memcpy((void*)(OSD_BASE+0x700),(void*)(pm_save_csr_regs),OSD_CSR_REG_SIZE);

    IO_WRITE32(OSD_BASE, 0x100, pm_save_osd1_regs[0]);
    IO_WRITE32(OSD_BASE, 0x200, pm_save_osd2_regs[0]);
    IO_WRITE32(OSD_BASE, 0x300, pm_save_osd3_regs[0]);
    IO_WRITE32(OSD_BASE, 0, pm_save_fmt_regs[0]|1); //update shadow
    #endif

    x_mem_free(pm_save_fmt_regs);
    x_mem_free(pm_save_osd1_regs);
    x_mem_free(pm_save_osd2_regs);
    x_mem_free(pm_save_osd3_regs);
    x_mem_free(pm_save_osd2sc_regs);
    x_mem_free(pm_save_osd3sc_regs);
    x_mem_free(pm_save_csr_regs);
}
#endif

//-----------------------------------------------------------------------------
/** osd global initialization function
 *  @param  void
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_Init(void)
{
    Printf("gfgOSD_Loging = %d \n", gfgOSD_Loging);
    while(gfgOSD_Loging)
    {
         Printf("gfgOSD_Loging = %d \n", gfgOSD_Loging);
         x_thread_delay(100);
    }

    if (_fgOsdInit == 0)
    {
        IGNORE_RET(OSD_Reset());

#if !defined(CC_MTK_LOADER)
        // Create OSD Flip Semphore
        if (x_sema_create
            (&_hOsdWaitVsyncSema, X_SEMA_TYPE_BINARY,
             X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ASSERT(0);
        }

        if(x_sema_create
            (&_hOsdFlipToSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }
#endif          

        _OsdGetMirrorFlipSetting();

        _fgOsdInit = 1;

        OSD_Base_SetHStart();

        Printf("OSD_Init Done!\n");
    }

    return (INT32)OSD_RET_OK;
}


INT32 OSD_Reset(void)
{
    // to choose osd clock as oclk
    IGNORE_RET(OSD_BASE_SetClock(OSD_CK_OCLK));

    // reset the whole OSD
    IGNORE_RET(_OSD_BASE_SetReg(NULL));

    IGNORE_RET(_OsdBaseInit());
    IGNORE_RET(_OSD_BASE_SetYuvOut(_fgYuvOutput));
    IGNORE_RET(OSD_UpdateTiming());
    
    IGNORE_RET(_OSD_BASE_SetHsEdge(0));
    IGNORE_RET(_OSD_BASE_SetVsEdge(1));
   #if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
    #ifdef CC_3D_OSD_SUPPORT
    IGNORE_RET(_OSD_3D_SetReg(NULL));
    #endif
    #endif  	

    _OSD_RGN_InitApi();
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_1));
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_2));
    //#ifdef CC_MT5363
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_3));
    //#endif
    IGNORE_RET(OSD_RGN_LIST_Init());
    IGNORE_RET(OSD_RGN_Init());

    // adjust scaler to fit new resolution and also reset LPF
    // defaule enable scaler for better bandwidth utilization
    #ifdef CC_MT5396
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_1, TRUE, 0, 0, 0, 0));
	#endif
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_2, TRUE, 0, 0, 0, 0));
    //IGNORE_RET(OSD_SC_SetLpf((UINT32)OSD_SCALER_2, FALSE));
    //#ifdef CC_MT5363
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_3, TRUE, 0, 0, 0, 0));
    //#endif
    //IGNORE_RET(OSD_SC_SetLpf((UINT32)OSD_SCALER_3, FALSE));	

    IO_WRITE32(OSD_INT_ENABLE_REG, 0, OSD1_CFUD_IEN |
               OSD2_CFUD_IEN | OSD2_PFUD_IEN | OSD2_VUTO_IEN | OSD2_VDTO_IEN |
               OSD3_CFUD_IEN | OSD3_PFUD_IEN | OSD3_VUTO_IEN | OSD3_VDTO_IEN);

    #ifdef DRV_SUPPORT_EXTMJC
    OSD_DMA_Init();
    #endif


    // update 3x3 matrix for RGB-to-YUV (OSD2)
    /*
    IO_WRITE32(OSD_BASE, 0x260, 0x00420081);
    IO_WRITE32(OSD_BASE, 0x264, 0x00190010);
    IO_WRITE32(OSD_BASE, 0x268, 0x1026104a);
    IO_WRITE32(OSD_BASE, 0x26c, 0x00700080);
    IO_WRITE32(OSD_BASE, 0x270, 0x0070105e);
    IO_WRITE32(OSD_BASE, 0x274, 0x10120080);
	*/

    // update 3x3 matrix for YUV-to-RGB 
    IO_WRITE32(OSD_BASE, 0x260, 0x01000000);
    IO_WRITE32(OSD_BASE, 0x264, 0x0164034e);
    IO_WRITE32(OSD_BASE, 0x268, 0x01001058);
    IO_WRITE32(OSD_BASE, 0x26c, 0x10b40086);
    IO_WRITE32(OSD_BASE, 0x270, 0x010001c4);
    IO_WRITE32(OSD_BASE, 0x274, 0x0000031e);

    IO_WRITE32(OSD_BASE, 0x360, 0x01000000);
    IO_WRITE32(OSD_BASE, 0x364, 0x0164034e);
    IO_WRITE32(OSD_BASE, 0x368, 0x01001058);
    IO_WRITE32(OSD_BASE, 0x36c, 0x10b40086);
    IO_WRITE32(OSD_BASE, 0x370, 0x010001c4);
    IO_WRITE32(OSD_BASE, 0x374, 0x0000031e);

#if !(defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)) 
    //#ifdef CC_MT5363
    OSD_SetTVEOut(1);
    //#endif
#endif
#if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
    IO_WRITE32(OSD_BASE, 0,0);
#endif
    return (INT32)OSD_RET_OK;
}


INT32 OSD_UpdateTiming(void)
{
#ifdef CC_MT5391_FPGA
    IGNORE_RET(_OSD_BASE_SetScrnHSize(720));
    IGNORE_RET(_OSD_BASE_SetScrnVSize(480));
    // osd2 hstart=0xb, osd vstart=0x5
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x350b0500, 0xffffff00);
    // osd3 hstart=0x1b
    IO_WRITE32MSK(OSD_BASE, 0x38, 0x091d, 0xffff);
#else
    IGNORE_RET(_OSD_BASE_SetScrnHSize(PANEL_GetPanelWidth()));
    IGNORE_RET(_OSD_BASE_SetScrnVSize(PANEL_GetPanelHeight()));
    //#ifdef CC_MT5363
     #if defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x1A0510, 0x00ffff00);
    #else
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x1A0410, 0x00ffff00);
    #endif
    //IO_WRITE32MSK(OSD_BASE, 0x38, 0x090c, 0xffff);
    IO_WRITE32MSK(OSD_BASE, 0x38, 0x1A, 0xffff);
    //IO_WRITE32MSK(OSD_BASE, 0x24, 0x00860000, 0xffffffff);
    //IO_WRITE32MSK(OSD_BASE, 0x28, 0x00860000, 0xffffffff);
    //#else
    /*
    	#ifdef CC_MT5301A
    	IO_WRITE32MSK(OSD_BASE, 0x2c, 0x251a0310, 0xffffff00);
    	#else
    	IO_WRITE32MSK(OSD_BASE, 0x2c, 0x250b0410, 0xffffff00);
    	#endif
    */
    //#endif
    //IO_WRITE32MSK(OSD_BASE, 0x38, 0x091d, 0xffff);
#endif

    // default OSD2 is top plane
    //OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_2);

    // write hardware
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** set whether osd1/osd2 is top plane
 *  @param  u4TopPlane OSD_PLANE_1/OSD_PLANE_2/OSD_PLANE_NONE(video plane is on top)
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_SetTopPlane(UINT32 u4TopPlane)
{
    // this function should be call in vsync, and update HW immediately
    //UNUSED(u4TopPlane);
	_OsdSetMirrorFlipSetting();
	//Printf("======[%s] <%d> =====\n",__FUNCTION__,__LINE__);
//#ifdef CC_MT5363
#if 0
    UINT32 u4X_tmp1,u4Y_tmp1,u4X_tmp2,u4Y_tmp2;
    if(u4TopPlane == (UINT32)OSD_PLANE_2)
    {
    	if(gfgSwap==TRUE)
    	{
    		return (INT32)OSD_RET_OK;
    	}
    	 OSD_PLA_Swap(1);
        OSD_SC_Swap(1);
        OSD_BASE_GetOsdPosition(OSD_PLANE_2,&u4X_tmp1,&u4Y_tmp1);
        OSD_BASE_GetOsdPosition(OSD_PLANE_3,&u4X_tmp2,&u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_2,u4X_tmp2,u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_3,u4X_tmp1,u4Y_tmp1);
	 gfgSwap=TRUE;
    }
    else	//OSD_PLANE_3 Top
    {
    	 if(gfgSwap==FALSE)
    	{
    		return (INT32)OSD_RET_OK;
    	}
    	 OSD_PLA_Swap(0);
        OSD_SC_Swap(0);
        OSD_BASE_GetOsdPosition(OSD_PLANE_2,&u4X_tmp1,&u4Y_tmp1);
        OSD_BASE_GetOsdPosition(OSD_PLANE_3,&u4X_tmp2,&u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_2,u4X_tmp2,u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_3,u4X_tmp1,u4Y_tmp1);
        gfgSwap=FALSE;
    }
#else
    //if(u4TopPlane == (UINT32)OSD_PLANE_1)
    if(u4TopPlane == (UINT32)OSD_PLANE_2)
    {
        _OSD_BASE_SetOSD12Ex(1);
    }
    else
    {
        _OSD_BASE_SetOSD12Ex(0);
    }
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
 #endif
    return (INT32)OSD_RET_OK;

#if 0
    _OSD_AlwaysUpdateReg(TRUE);

    if (u4TopPlane == (UINT32)OSD_PLANE_NONE)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(0));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x20<<16, 0x00ff0000);	//set OSD2 H auto-position not top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1D, 0x000000ff);	//set OSD3 H auto-position not top
    }
    else if (u4TopPlane == (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(1));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x20<<16, 0x00ff0000);	//set OSD2 H auto-position not top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1D, 0x000000ff);	//set OSD3 H auto-position not top
    }
    else if (u4TopPlane == (UINT32)OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(2));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x1E<<16, 0x00ff0000);	//set OSD2 H auto-position top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1D, 0x000000ff);	//set OSD3 H auto-position not top
    }
    else if (u4TopPlane == (UINT32)OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(4));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x20<<16, 0x00ff0000);	//set OSD2 H auto-position not top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1B, 0x000000ff);	//set OSD3 H auto-position top
    }
    else
    {
        return (INT32)OSD_RET_INV_ARG;
    }

    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    _OSD_AlwaysUpdateReg(FALSE);
    return (INT32)OSD_RET_OK;
    
#endif

}

INT32 OSD_BASE_SetPlaneSwitchOrder(UINT32 u4TopPlane, UINT32 u4MidPlane, UINT32 u4BotPlane)
{
   // default define: plan1 top , plane3  Mid; plane2 Bot,  need update when IC back
   if(u4TopPlane == OSD_PLANE_2)
	{
	  if(u4MidPlane == OSD_PLANE_1)
	  {
	      _OSD_BASE_SetOSD1Switch(0x2);
		  _OSD_BASE_SetOSD3Switch(0x1);
		  _OSD_BASE_SetOSD2Switch(0x3);
	  }
	  else
	  	{
	  	  _OSD_BASE_SetOSD1Switch(0x2);
		  _OSD_BASE_SetOSD3Switch(0x3);
		  _OSD_BASE_SetOSD2Switch(0x1);
	  	}
   	}
    else if(u4TopPlane == OSD_PLANE_1)
	{
	  if(u4MidPlane == OSD_PLANE_2)
	  {
	      _OSD_BASE_SetOSD1Switch(0x1);
		  _OSD_BASE_SetOSD3Switch(0x2);
		  _OSD_BASE_SetOSD2Switch(0x3);
	  }
	  else
	  	{
	  	  _OSD_BASE_SetOSD1Switch(0x1);
		  _OSD_BASE_SetOSD3Switch(0x3);
		  _OSD_BASE_SetOSD2Switch(0x2);
	  	}
   	 }
	 else if(u4TopPlane == OSD_PLANE_3)
	{
	  if(u4MidPlane == OSD_PLANE_1)
	  {
	      _OSD_BASE_SetOSD1Switch(0x3);
		  _OSD_BASE_SetOSD3Switch(0x1);
		  _OSD_BASE_SetOSD2Switch(0x2);
	  }
	  else
	  	{
	  	  _OSD_BASE_SetOSD1Switch(0x3);
		  _OSD_BASE_SetOSD3Switch(0x2);
		  _OSD_BASE_SetOSD2Switch(0x1);
	  	}
   	 }

    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;


}



INT32 OSD_BASE_SetDisplayMode(UINT32 u4DisplayMode)
{
    if (BSP_IsFPGA())
    {
        OSD_VERIFY_DISPLAY_MODE(u4DisplayMode);

        if (u4DisplayMode == (UINT32)OSD_DM_480P) // 2
        {
            // 720x480
            IO_WRITE32MSK(SCPOS_BASE, 0x24, ((UINT32)0x359 << 20) | 0x20c,
                          0xfff007ff);
            PANEL_SetPanelWidth(720);
            PANEL_SetPanelHeight(480);
        }
        else if (u4DisplayMode == (UINT32)OSD_DM_1080P) // 7
        {
            // 1920x1080
            IO_WRITE32MSK(SCPOS_BASE, 0x24, ((UINT32)0x897 << 20) | 0x464,
                          0xfff007ff);
            PANEL_SetPanelWidth(1920);
            PANEL_SetPanelHeight(1080);
        }
    }

    UNUSED(u4DisplayMode);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** set osd plane position
 *  @param  u4Plane plane id
 *  @param  u4X x position
 *  @param  u4Y y position
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_SetOsdPosition(UINT32 u4Plane, UINT32 u4X, UINT32 u4Y)
{

    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
    #if 0
    if(gfgSwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif

    if (u4Plane == (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_SetOsd1HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd1VStart(u4Y));
    }
    else if (u4Plane == (UINT32)OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_SetOsd2HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd2VStart(u4Y));
        //OSD_SC_UpdateDstSize((UINT32)OSD_SCALER_2);
    }
    //#ifdef CC_MT5363
    else if (u4Plane == (UINT32)OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_SetOsd3HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd3VStart(u4Y));
        //OSD_SC_UpdateDstSize((UINT32)OSD_SCALER_3);
    }
    //#endif

    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32)OSD_RET_OK;
}

VOID OSD_BASE_GetPanelSize(UINT32 *u4Width, UINT32 *u4Height)
{
     if(u4Width)
	 	*u4Width = PANEL_GetPanelWidth();

	 if(u4Height)
	 	*u4Height = PANEL_GetPanelHeight();
}

//-----------------------------------------------------------------------------
/** get osd plane position
 *  @param  u4Plane plane id
 *  @param  pu4X x position pointer
 *  @param  pu4Y y position pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_GetOsdPosition(UINT32 u4Plane, UINT32* pu4X, UINT32* pu4Y)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
    #if 0
    if(gfgSwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif

    if (u4Plane == (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_GetOsd1HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd1VStart(pu4Y));
    }
    else if (u4Plane == (UINT32)OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_GetOsd2HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd2VStart(pu4Y));
    }
    //#ifdef CC_MT5363
    else if (u4Plane == (UINT32)OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_GetOsd3HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd3VStart(pu4Y));
    }
    //#endif
    return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** dump osd debug info
 *  @param  void
 *  @return void
 */
//-----------------------------------------------------------------------------
void OSD_BASE_Dump(void)
{
    UINT32 u4IntStatus;
    u4IntStatus = IO_READ32(OSD_INT_STATUS_REG, 0);

    Printf("OSD INTERRUPT=");
    if (u4IntStatus == 0)
    {
        Printf("none\n");
    }
    else
    {
        if ((u4IntStatus & OSD_INT) > 0)
        {
            Printf("(OSD_INT)");
        }
        if ((u4IntStatus & OSD1_CFUD_IID) > 0)
        {
            Printf("(OSD1_CFUD_IID)");
        }
        if ((u4IntStatus & OSD2_CFUD_IID) > 0)
        {
            Printf("(OSD2_CFUD_IID)");
        }
        if ((u4IntStatus & OSD2_PFUD_IID) > 0)
        {
            Printf("(OSD2_PFUD_IID)");
        }
        if ((u4IntStatus & OSD2_VUTO_IID) > 0)
        {
            Printf("(OSD2_VUTO_IID)");
        }
        if ((u4IntStatus & OSD2_VDTO_IID) > 0)
        {
            Printf("(OSD2_VDTO_IID)");
        }
        if ((u4IntStatus & OSD3_CFUD_IID) > 0)
        {
            Printf("(OSD3_CFUD_IID)");
        }
        if ((u4IntStatus & OSD3_PFUD_IID) > 0)
        {
            Printf("(OSD3_PFUD_IID)");
        }
        if ((u4IntStatus & OSD3_VUTO_IID) > 0)
        {
            Printf("(OSD3_VUTO_IID)");
        }
        if ((u4IntStatus & OSD3_VDTO_IID) > 0)
        {
            Printf("(OSD3_VDTO_IID)");
        }
        Printf("\n");
        IO_WRITE32(OSD_INT_STATUS_REG, 0, 0);
    }
}

//Set 5363 SD TVE output config

INT32 OSD_SetTVEOut(BOOL fgEnable)
{
	Printf("OSD_SetTVEOut: %x\n",fgEnable);
	if(fgEnable)
	{
		//IO_WRITE32(OSD_BASE, 0x8, 0x40128);
		IO_WRITE32(OSD_BASE, 0xc0, 0xa);
		IO_WRITE32(OSD_BASE, 0x10, 0x20766610);
		IO_WRITE32(OSD_BASE, 0,8);
		IO_WRITE32(OSD_BASE, 0x100,0);
		IO_WRITE32(OSD_BASE, 0x4,2);
		IO_WRITE32(OSD_BASE, 0x4,3);
		IO_WRITE32(OSD_BASE, 0,0);
		IO_WRITE32(OSD_BASE, 0x100,1);
		IO_WRITE32(OSD_BASE, 0,8);
		IO_WRITE32(OSD_BASE, 0xc8, 0x03600271);
		IO_WRITE32(OSD_BASE, 0xcc, 0x03600271);
		//IO_WRITE32(OSD_BASE, 0x20, 0x001e000c);
		//IO_WRITE32(OSD_BASE, 0x20, 0x0019002b);
		IGNORE_RET(_OSD_BASE_SetOsd1HStart(0x19));
              IGNORE_RET(_OSD_BASE_SetOsd1VStart(0x2b));
              _OSD_BASE_UpdateHwReg();
		//IO_WRITE32(OSD_BASE, 0,8);
	}
	else
	{
		//IO_WRITE32MSK(OSD_BASE, 0x8, 0, 0x40128);
		IO_WRITE32MSK(OSD_BASE, 0xc0, 0, 0xa);
		IO_WRITE32(OSD_BASE, 0x100,0);
		IO_WRITE32MSK(OSD_BASE, 0xc8, 0,0x03600271);
		IO_WRITE32MSK(OSD_BASE, 0xcc, 0,0x03600271);
		//IO_WRITE32MSK(OSD_BASE, 0x20, 0,0x001e000c);
		IO_WRITE32MSK(OSD_BASE, 0x20, 0,0x0019002b);
		IO_WRITE32(OSD_BASE, 0,8);
	}
	
    return (INT32)OSD_RET_OK;
}

INT32 OSD_SetPowerDwn(BOOL fgEnable)
{
	Printf("OSD_SetPowerDwn: %x\n",fgEnable);
	#ifdef USE_OSD_CK_REG
	if(fgEnable)
	{	
		IO_WRITE32(OSD_CK_REG, 0, (_u4Clock |0x80));
	}
	else
	{
		IO_WRITE32(OSD_CK_REG, 0, _u4Clock );
	}
	#else
	UNUSED(_u4Clock);
	#endif
	
	return (INT32)OSD_RET_OK;
}

#ifndef CC_MTK_LOADER

#include "linux/file_ops.h"


UINT8 bDrvLogoColorMode = 0xd;
UINT8 *pLogoBuffer = NULL;
UINT32 u4LogoBufferLength = 0;
UINT32 u4LogoWidth = 712;
UINT32 u4LogoHeight = 400;
extern UINT8 bIsBootLogoEnabled(void);
UINT8 bLogoTableResume(void)
{
    UINT32 i = 0;
    UINT32 u4Configuration[3];
    UINT32 u4Len = 0;
    UINT64 u8Offset = 0;
    UINT32 u4Pitch = 0;
    UINT32 u4ValidLen = 0;
    struct file *file = file_open("/ro/logo.bin", O_RDONLY, 0);

    Printf("step1____\n");
    if (!file)
    {
        Printf("logo exit: a\n");
        return 0;//fail
    }
    Printf("step2____\n");

    u8Offset= 0;
    u4Len = file_read(file, &u4Configuration[0], 12, &u8Offset);
    Printf(" u4Len1 = %d \n",u4Len);

    if (u4Len != 12)
    {
        Printf("logo exit: b\n");
        return 0;
    }
    else
    {
        u4LogoBufferLength = u4Configuration[0];
        u4LogoWidth = u4Configuration[1];
        u4LogoHeight = u4Configuration[2];
    }
    Printf("step3____\n");
    u8Offset= 12;
    
    OSD_GET_PITCH_SIZE(bDrvLogoColorMode, u4LogoWidth, u4Pitch);
    u4ValidLen = u4Pitch;
    u4Pitch = (u4Pitch + 0xF) & (~0xF);
    u4LogoBufferLength = u4Pitch * u4LogoHeight;
    
    pLogoBuffer = (UINT8 *)x_mem_alloc(u4LogoBufferLength);
#if 0    
    u4Len = file_read(file, pLogoBuffer, u4LogoBufferLength, &u8Offset);
    Printf(" u4Len2 = %d \n",u4Len);
#else
    for(i = 0; i < u4LogoHeight; i++)
    {
         u8Offset = 12 + i * u4ValidLen;
         u4Len = file_read(file, (UINT8 *)((UINT32) pLogoBuffer + i * u4Pitch), u4ValidLen, &u8Offset);
        // Printf(" u4Len2 = %d \n",u4Len);    
    }

#endif

    if (u4Len != u4LogoBufferLength)
    {
        Printf("logo exit: c\n");
        return 0;
    }
    else
    {
#if 0//debug
        for(i = 0; i<100; i++)
        {
            printf("1 pTmp[%d] = 0x%x\n", i , pLogoBuffer[i]);
        }
        for(i = u4LogoBufferLength-100; i<u4LogoBufferLength; i++)
        {
            printf("2 pTmp[%d] = 0x%x\n", i , pLogoBuffer[i]);
        }
#endif
        
    }
    Printf("step4____\n");

    file_close(file);
    return 1;
}

void vLogoTableDelete(void)
{
    x_mem_free(pLogoBuffer);
}
#endif

void OSD_SetFlipMirror(BOOL flip, BOOL mirror)
{
    Printf("~~~~~~ %s %d flip= %d, mirror = %d\n",__FUNCTION__,__LINE__, flip,mirror);
    OSD_PLA_EnableFlipMirror(OSD_PLANE_2, flip, mirror);

    OSD_PLA_EnableFlipMirror(OSD_PLANE_3, flip, mirror);
}

INT32 OSD_ShowLogo(void)
{
#ifndef CC_MTK_LOADER
    static UINT32 u4RegionList;
    //UINT32 u4Region=0, u4BmpPitch,u4Width,u4Height,u4ColorMode;
    static UINT32 u4Region=0;
    UINT32 u4BmpPitch,u4Width,u4Height,u4ColorMode,u4DataSize;
    
    UINT32 u4PanelWidth, u4PanelHeight;
    UINT32 u4OutWidth, u4OutHeight,u4OutX,u4OutY;
    UINT32 i;
    UINT32 u4BmpAddr;//,u4PalAddr;
    UINT32 u4LogoBuf, u4Virtual;
    //for restoring original setting
    //UINT32 u4OsdPlane = OSD_PLANE_2;
    //UINT32 u4Scaler = OSD_SCALER_2;
    UINT32 u4OsdPlane = OSD_PLANE_3;
    UINT32 u4Scaler = OSD_SCALER_3;
    INT32 i4OriRegionList=(-1);
    UINT8 u1OriPLA_Enable;
    UINT32 u4OriSC_Enable = 0;
    UINT32 u4OriSrcW = 0;
    UINT32 u4OriSrcH = 0;
    UINT32 u4OriDstW = 0;
    UINT32 u4OriDstH = 0;
    UINT32 u4OriIs16Bpp = 0;
    UINT8 u1tmpFading;
    //UINT32 u4tmpScaler;
    
    INT32 ret;
   // FBM_POOL_T* prFbmPool;

    if(pfn_malloc == NULL || pfn_free == NULL)
    {
        Printf("malloc/free function not register \n");
        return (INT32)OSD_RET_OK; 
    }


    if((DRVCUST_OptGet(eDrvLogoEnable)==0)||(bIsBootLogoEnabled()==0))
    {
    	 Printf("Driver Logo Disabled!, %d, %d\n", DRVCUST_OptGet(eDrvLogoEnable), bIsBootLogoEnabled());
        return (INT32)OSD_RET_OK;
    }
    //Printf("---OSD Show Logo---\n");
    
    //get original setting
    i4OriRegionList = OSD_PLA_GetFirstRegionList(u4OsdPlane);
        if(i4OriRegionList < 0)
        {
        	/*
            i4OriRegionList = OSD_PLA_GetFirstRegionList(OSD_PLANE_3);
            if(i4OriRegionList >= 0)
            {
                u4OsdPlane = OSD_PLANE_3;
                u4Scaler = OSD_SCALER_3;
            }
            */
            Printf("---plane3 empty!\n");
        }
        i4OriRegionList = OSD_PLA_GetFirstRegionList(u4OsdPlane);
        if(OSD_PLA_GetEnable(u4OsdPlane, &u1OriPLA_Enable) < 0)
        {
            Printf("OSD_PLA_GetEnable fail\n");
        }
        UNUSED(OSD_SC_GetScalerInfo(u4Scaler, &u4OriSC_Enable,
                       &u4OriSrcW, &u4OriSrcH,
                       &u4OriDstW, &u4OriDstH,
                       &u4OriIs16Bpp));
        UNUSED(u4OriIs16Bpp);
    
    if(fgLogoInit==TRUE)
    {
    	//ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    	gfgOSD_Loging = TRUE;
    	ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
    }
    else
    {
       // power on panel
       //vApiPanelPowerSequence(TRUE);
       OSD_Init();
       gfgOSD_Loging = TRUE;
	   OSD_PLA_Enable(u4OsdPlane, FALSE);

    	u4PanelWidth = PANEL_GetPanelWidth();
    	u4PanelHeight = PANEL_GetPanelHeight();

    	Printf("Panel %ld %ld \n", u4PanelWidth, u4PanelHeight);

    	ret = OSD_RGN_LIST_Create(&u4RegionList);
    		if (ret != OSD_RET_OK) return 1;

        bLogoTableResume();
    	u4BmpPitch = 0;
    	u4ColorMode=bDrvLogoColorMode;//DRVCUST_OptGet(eDrvLogoColorMode);
    	u4Width=u4LogoWidth;//DRVCUST_OptGet(eDrvLogoWidth);
    	u4Height=u4LogoHeight;//DRVCUST_OptGet(eDrvLogoHeight);
       u4BmpAddr=(UINT32)pLogoBuffer;//DRVCUST_OptGet(eDrvLogoBmpAddr);

       Printf("showlogo:0x%x, 0x%x, 0x%x, 0x%x____________________________\n", u4ColorMode,u4Width, u4Height, u4BmpAddr );
       //if(u4ColorMode==OSD_CM_RGB_CLUT8)
       //{
       //	u4PalAddr=DRVCUST_OptGet(eDrvLogoPalAddr);
       //}
    
    	OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.

        u4BmpPitch = (u4BmpPitch + 0xF) & (~0xF);

	    u4DataSize=u4BmpPitch*u4Height;
        if(u4DataSize == 0)
        {
            Printf("u4DataSize(0)\n");
            return FALSE;
        }
	
	#if 0
    	prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        if(!prFbmPool)
        {
            Printf("prFbmPool null\n", prFbmPool);
            return FALSE;
        }
        if(u4DataSize > prFbmPool->u4Size)
        {
            Printf("u4DataSize(%d) too big\n", u4DataSize);
            u4DataSize = prFbmPool->u4Size;
        }
        x_memcpy((VOID*)VIRTUAL(prFbmPool->u4Addr), (void*)u4BmpAddr, u4DataSize);
        u4BmpAddr = prFbmPool->u4Addr;
    #endif        
#if 1
        //u4LogoBuf = (UINT32)BSP_AllocAlignedDmaMemory(u4DataSize, 16);
        pfn_malloc(&u4LogoBuf, &u4Virtual, u4DataSize);
        if (u4LogoBuf == 0)
        {
            gfgOSD_Loging = FALSE; //memory allocated fails,set gfgOSD_Loging to false.
            fgLogoInit = TRUE; /*set the logo init to true,ignore the logo.*/
            Printf("u4LogoBuf null\n");
            return FALSE;
        }

        x_memcpy((void*)u4Virtual, (void*)u4BmpAddr, u4DataSize);

		HalFlushInvalidateDCacheMultipleLine((UINT32)(VIRTUAL(u4LogoBuf)),u4DataSize);
        x_memset(u4BmpAddr, 0, u4LogoBufferLength);
        u4BmpAddr = u4LogoBuf;    
  #endif      
    	ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)u4BmpAddr,
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);

    	//if(u4ColorMode==OSD_CM_RGB_CLUT8)
       //{
       //	OSD_RGN_Set(u4Region, OSD_RGN_PAL_ADDR, u4PalAddr);
       //}
    	if (ret != OSD_RET_OK) return 2;
    	ret = OSD_RGN_Insert(u4Region, u4RegionList);
    	if (ret != OSD_RET_OK) return 3;
    	//ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    	ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
    	if (ret != OSD_RET_OK) return 4;

	#if 1   // original bmp size.
	    if((u4PanelWidth>1366) && (u4Width <= 1366) && (u4Height <= 768))
	    {
      u4OutWidth = (u4Width*u4PanelWidth)/1366;
	    u4OutHeight = (u4Height*u4PanelHeight)/768;
	    //Printf("Out: %d x %d \n", u4OutWidth, u4OutHeight);
	    
	    u4OutX =((u4PanelWidth - u4OutWidth) >> 1);
	    u4OutY =((u4PanelHeight - u4OutHeight) >> 1);
	    //ret = OSD_SC_Scale(OSD_SCALER_2, 1, u4Width, u4Height, u4OutWidth, u4OutHeight);
	    ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4OutWidth, u4OutHeight);
	    if (ret != OSD_RET_OK) return 5;	
	    
	    //Printf("---suspect: scaler: %d,swap: %d\n",u4Scaler,gfgSwap);
	    #if 0
	    if(gfgSwap)
	    {
	    		if(u4Scaler==OSD_SCALER_2)
	    			u4tmpScaler=OSD_SCALER_3;
	    		else
	    			u4tmpScaler=OSD_SCALER_2;
	    }
	    else
	    {
	    		u4tmpScaler=u4Scaler;
	    }
	    #endif
	    /*
	    	_OSD_SC_SetSrcHSize(u4tmpScaler, u4PanelWidth);
        _OSD_SC_SetVscHSize(u4tmpScaler, u4PanelWidth);
        _OSD_SC_SetDstHSize(u4tmpScaler, u4PanelWidth);
        _OSD_SC_SetSrcVSize(u4tmpScaler, u4PanelHeight);
        _OSD_SC_SetDstVSize(u4tmpScaler, u4PanelHeight);
        _OSD_SC_UpdateHwReg(u4tmpScaler);
        */
	    
        _OSD_SC_SetSrcHSize(u4Scaler, 1366);
        _OSD_SC_SetVscHSize(u4Scaler, 1366);
        _OSD_SC_SetDstHSize(u4Scaler, u4PanelWidth);
        _OSD_SC_SetSrcVSize(u4Scaler, 768);
        _OSD_SC_SetDstVSize(u4Scaler, u4PanelHeight);

        _OSD_SC_UpdateHwReg(u4Scaler);
        
	    
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4OutX*1366)/u4PanelWidth);
    	if (ret != OSD_RET_OK) return 6;
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4OutY*768)/u4PanelHeight);
    	if (ret != OSD_RET_OK) return 7;
    	}
    	else
    	{
        _OSD_SC_SetSrcHSize(u4Scaler, u4PanelWidth);
        _OSD_SC_SetVscHSize(u4Scaler, u4PanelWidth);
        _OSD_SC_SetDstHSize(u4Scaler, u4PanelWidth);
        _OSD_SC_SetSrcVSize(u4Scaler, u4PanelHeight);
        _OSD_SC_SetDstVSize(u4Scaler, u4PanelHeight);
        _OSD_SC_UpdateHwReg(u4Scaler);  
		
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4PanelWidth - u4Width) >> 1);
    	if (ret != OSD_RET_OK) return 5;
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4PanelHeight - u4Height) >> 1);
    	if (ret != OSD_RET_OK) return 6;

        OSD_SC_Scale(u4Scaler, 1, u4PanelWidth, u4PanelHeight, u4PanelWidth, u4PanelHeight);
      
      }
	#else   // scale size.
    	ret = OSD_SC_Scale(OSD_SCALER_2, 1, u4Width, u4Height, u4PanelWidth, u4PanelHeight);
    	if (ret != OSD_RET_OK) return 7;
	#endif
	fgLogoInit=TRUE;
    }
    //ret = OSD_PLA_Enable(OSD_PLANE_2, TRUE);
    ret = OSD_PLA_Enable(u4OsdPlane, TRUE);
    if (ret != OSD_RET_OK) return 8;

#if 1
    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0xff); 
    x_thread_delay(3000);
#else
#if OSD_LOGO_FADING_ENABLE
		OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);   
#else
		OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0xff); 
#endif

    OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, (UINT32)OSD_BM_REGION);   
 
    OSD_PLA_GetFading(OSD_PLANE_2,&u1tmpFading);
    Printf("P3 fading: 0x%x\n", u1tmpFading);
    OSD_PLA_SetFading(OSD_PLANE_2,0);

    x_thread_delay(2000);
 #if OSD_LOGO_FADING_ENABLE
	//OSD_PLA_SetFading(OSD_PLANE_2,0);
	for(i=0;i<16;i++)
	{
		x_thread_delay(62);
		//OSD_PLA_SetFading(OSD_PLANE_2,((i+1)*16-1));
		OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, ((i+1)*16-1));   
	}
	x_thread_delay(1000);
	for(i=16;i>0;i--)
	{
		//OSD_PLA_SetFading(OSD_PLANE_2,(i*16-1));
		OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, (i*16-1));   
		x_thread_delay(62);
	}
	//OSD_PLA_SetFading(OSD_PLANE_2,0xff);
#endif 
	OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);   
	
	OSD_PLA_SetFading(OSD_PLANE_2,u1tmpFading);
#endif    
	//restoring original setting
	//OSD_PLA_Enable(u4OsdPlane, FALSE);
    //OSD_PLA_FlipToNone(u4OsdPlane);
    //x_thread_delay(3000);
#if 0      
        if(u4OriSC_Enable != 0)
        {
            UNUSED(OSD_SC_Scale(u4Scaler, u4OriSC_Enable,
                           u4OriSrcW, u4OriSrcH,
                           u4OriDstW, u4OriDstH));
        }
        
        if(i4OriRegionList >= 0)
        {
            OSD_PLA_FlipTo(u4OsdPlane, (UINT32)i4OriRegionList);
        }
        else
        {
            OSD_PLA_FlipToNone(u4OsdPlane);
        }
        if(u1OriPLA_Enable)
        {
            OSD_PLA_Enable(u4OsdPlane, TRUE);
        }
#endif
    //BSP_FreeAlignedDmaMemory((void*)VIRTUAL(u4LogoBuf));

    gfgOSD_Loging = FALSE;

    x_memset((void*)VIRTUAL(u4LogoBuf), 0, u4DataSize);
    HalFlushInvalidateDCacheMultipleLine((UINT32)(VIRTUAL(u4LogoBuf)),u4DataSize);
    OSD_PLA_Enable(u4OsdPlane, FALSE);
    OSD_WaitVsync();
    pfn_free(&u4LogoBuf, &u4Virtual);//BSP_FreeAlignedDmaMemory(u4LogoBuf);
    vLogoTableDelete();
#endif
    return (INT32)OSD_RET_OK;
}

// Get OSD FIFO underflow status
BOOL OSD_GetUnderflowStatus(void)
{
	UINT32 fgUnderflow;
	
	fgUnderflow = IO_READ32(OSD_BASE, 0xFC);

	if(fgUnderflow!=0)
		return TRUE;
	else
		return FALSE;
}
// Clear OSD FIFO underflow status
void OSD_ClearUnderflowStatus(void)
{
	IO_WRITE32(OSD_BASE, 0x034, 0x0);
	IO_WRITE32(OSD_BASE, 0x0FC, 0x4);
}


INT32 OSD_BASE_EnableAutoLR(UINT32 u4Enable)
{

    _OSD_BASE_SetAutoLR(u4Enable);
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());    
    return (INT32)OSD_RET_OK;
}


INT32 OSD_BASE_GetLRStatus(UINT32* pu4Status)
{

    _OSD_BASE_GetLRFlag(pu4Status);
    return (INT32)OSD_RET_OK;
}



INT32 OSD_BASE_SetPlaneSwitch(UINT32 u4Enable)
{
    UINT32 u4Rgn2, u4Rgn3;
    UINT32  u4BmpH2, u4BmpH3;
    u4Rgn2 = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
    if (u4Rgn2 >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE  get region 2 fail \n");
        //return (INT32)OSD_RET_OUT_OF_REGION;
    }
    else
    {
        IGNORE_RET(OSD_RGN_Get(u4Rgn2, (INT32)OSD_RGN_BMP_H, &u4BmpH2));
	Printf("test  u4BmpH2 = %ld\n", u4BmpH2);
        if (u4Enable == TRUE)
        {
            IGNORE_RET(OSD_RGN_Set(u4Rgn2, (INT32)OSD_RGN_DISP_H, u4BmpH2 / 2));
            _OSD_RGN_SetVStep(u4Rgn2, 0x2000);
        }
        else
        {
            IGNORE_RET(OSD_RGN_Set(u4Rgn2, (INT32)OSD_RGN_DISP_H, u4BmpH2));
            _OSD_RGN_SetVStep(u4Rgn2, 0x1000);
        }
    }

    u4Rgn3 = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
    if (u4Rgn3 >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE  get region 3  fail \n");
        //  return (INT32)OSD_RET_OUT_OF_REGION;
    }
    else
    {
        IGNORE_RET(OSD_RGN_Get(u4Rgn3, (INT32)OSD_RGN_BMP_H, &u4BmpH3));
	Printf("test  u4BmpH3 = %ld\n", u4BmpH3);
        if (u4Enable == TRUE)
        {
            IGNORE_RET(OSD_RGN_Set(u4Rgn3, (INT32)OSD_RGN_DISP_H, u4BmpH3 / 2));
            _OSD_RGN_SetVStep(u4Rgn3, 0x2000);
        }
        else
        {
            IGNORE_RET(OSD_RGN_Set(u4Rgn3, (INT32)OSD_RGN_DISP_H, u4BmpH3));
            _OSD_RGN_SetVStep(u4Rgn3, 0x1000);
        }
    }

    _OSD_BASE_SetPlaneSwitch(u4Enable);
   _OSD_BASE_SetLineDup(u4Enable);
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_SetLineDup(UINT32 u4Enable)
{
    _OSD_BASE_SetLineDup(u4Enable);
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}


INT32 OSD_BASE_SetPlaneLineShift(UINT32 u4Plane, BOOL fgEnable,  INT32 i4Value)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
    #if 0
    if(gfgPLASwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif
    #ifdef CC_3D_OSD_SUPPORT
	switch(u4Plane)
		{
		   case OSD_PLANE_1:
		   		if(fgEnable)
		   		{
		   		  OSD_3D_SetOsd1LShiftEn(TRUE);
				  _OSD_3D_SetOsd1ShiftValue((i4Value>0)?i4Value:(0-i4Value));
		   		}
				else//OSD_3DPrLshift_ENABLE_FLAG
				{
				  OSD_3D_SetOsd1LShiftEn(FALSE);
				  _OSD_3D_SetOsd1ShiftValue(0);
				}
		   	break;
	       case OSD_PLANE_2:
		   		if(fgEnable)
		   		{
		   		  
		   		  OSD_3D_SetOsd2LShiftEn(TRUE);
				  
				  _OSD_3D_SetOsd2ShiftValue((i4Value>0)?i4Value:(0-i4Value));
		   		}
				else
				{
				  OSD_3D_SetOsd2LShiftEn(FALSE);

				  _OSD_3D_SetOsd2ShiftValue(0);
				}
		   	break;
		   case OSD_PLANE_3:
		   		if(fgEnable)
		   		{
		   		  OSD_3D_SetOsd3LShiftEn(TRUE);
				  _OSD_3D_SetOsd3ShiftValue((i4Value>0)?i4Value:(0-i4Value));
		   		}
				else
				{
				  OSD_3D_SetOsd3LShiftEn(FALSE);
				  _OSD_3D_SetOsd3ShiftValue(0);
				}
		   	break;
		   default:
				break;
		}
    #endif
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    #ifdef CC_3D_OSD_SUPPORT
	VERIFY((INT32)OSD_RET_OK == _OSD_3D_UpdateHwReg());
	#endif
	HalFlushDCache();

    return (INT32)OSD_RET_OK;
}

#ifdef CC_3D_OSD_SUPPORT
INT32 OSD_BASE_Get3DOsd2LShiftEn(UINT32 *pu4Value)
{
    _OSD_3D_GetOsd2LShiftEn(pu4Value);
    return (INT32)OSD_RET_OK;
}


INT32 OSD_BASE_Set3DPrLineShift( UINT32 u4Enable, UINT32 u4Shift)
{
    UINT32 u4Plane2Region = 0, u4Plane3Region = 0, u4Plane1Region = 0;
	INT32 u4PlaneOutputXShift = 0, u4PlaneOutputX = 0;
	static UINT32 u4Enable_tag = 0;
	static UINT32 u4Plane1OutputH_tag = 0, u4Plane2OutputH_tag = 0, u4Plane3OutputH_tag = 0;

	printf("u4Enable_tag = 0x%x\n",u4Enable_tag);

	if( !(u4Enable_tag) )
	{
		_OSD_BASE_GetScrnHStartOsd1(&u4Plane1OutputH_tag);
		_OSD_BASE_GetScrnHStartOsd2(&u4Plane2OutputH_tag);
		_OSD_BASE_GetScrnHStartOsd3(&u4Plane3OutputH_tag);

		u4Enable_tag = 1;
		printf(">>u4Enable_tag = 0x%x\n",u4Enable_tag);
	}


	if( u4Shift < 0x80 )
	{
		u4PlaneOutputXShift = - u4Shift / 2;
		_OSD_3D_SetOsd2LShiftInv((UINT32)FALSE);	
	}
	else if( u4Shift > 0x80 )
	{
		u4PlaneOutputXShift = ( u4Shift - 0x80 ) / 2;
		_OSD_3D_SetOsd2LShiftInv((UINT32)TRUE);
	}

    u4Plane2Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2); 
	if (u4Plane2Region >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE get region fail \n");
        //return (INT32)OSD_RET_OK;
    }
    else if (u4Plane2Region < OSD_MAX_NUM_RGN)
    {  
        if (u4Enable == 1)
        {
           u4PlaneOutputX = u4Plane2OutputH_tag + u4PlaneOutputXShift;
		   _OSD_BASE_SetScrnHStartOsd2(u4PlaneOutputX);
		   
           OSD_RGN_Set(u4Plane2Region, (INT32)OSD_RGN_STEP_V, 0x2000);
		   OSD_BASE_SetPlaneLineShift(OSD_PLANE_2,TRUE, u4Shift);
        }
        else
        {
           OSD_RGN_Set(u4Plane2Region, (INT32)OSD_RGN_STEP_V, 0x1000);
		   OSD_BASE_SetPlaneLineShift(OSD_PLANE_2,FALSE, 0);
		   
		   _OSD_BASE_SetScrnHStartOsd2(u4Plane2OutputH_tag);
		   u4Enable_tag = 0;
        }
    }

	 u4Plane3Region = OSD_PLA_GetFirstRegion(OSD_PLANE_3); 
	if (u4Plane3Region >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE get region fail \n");
       // return (INT32)OSD_RET_OK;
    }
    else if (u4Plane3Region < OSD_MAX_NUM_RGN)
    {  
        if (u4Enable == 1)
        {
           u4PlaneOutputX = u4Plane3OutputH_tag + u4PlaneOutputXShift;
		   _OSD_BASE_SetScrnHStartOsd3(u4PlaneOutputX);
		   
           OSD_RGN_Set(u4Plane3Region, (INT32)OSD_RGN_STEP_V, 0x2000);
		   OSD_BASE_SetPlaneLineShift(OSD_PLANE_3, TRUE, u4Shift);
        }
        else
        {
           OSD_RGN_Set(u4Plane3Region, (INT32)OSD_RGN_STEP_V, 0x1000);
		   OSD_BASE_SetPlaneLineShift(OSD_PLANE_3, FALSE, 0);

		   _OSD_BASE_SetScrnHStartOsd3(u4Plane3OutputH_tag);
		   u4Enable_tag = 0;
        }
    }

	u4Plane1Region = OSD_PLA_GetFirstRegion(OSD_PLANE_1); 
	if (u4Plane1Region >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE get region fail \n");
       // return (INT32)OSD_RET_OK;
    }
    else if (u4Plane1Region < OSD_MAX_NUM_RGN)
    {  
        if (u4Enable == 1)
        {
           u4PlaneOutputX = u4Plane1OutputH_tag + u4PlaneOutputXShift;
		   _OSD_BASE_SetScrnHStartOsd1(u4PlaneOutputX);
		   
           OSD_RGN_Set(u4Plane1Region, (INT32)OSD_RGN_STEP_V, 0x2000);
		   OSD_BASE_SetPlaneLineShift(OSD_PLANE_1,TRUE, u4Shift);
        }
        else
        {
           OSD_RGN_Set(u4Plane1Region, (INT32)OSD_RGN_STEP_V, 0x1000);
		   OSD_BASE_SetPlaneLineShift(OSD_PLANE_1, FALSE, 0);

		   _OSD_BASE_SetScrnHStartOsd1(u4Plane1OutputH_tag);
		   u4Enable_tag = 0;
        }
    }
	
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
	HalFlushDCache();

    return (INT32)OSD_RET_OK;
}


INT32 OSD_BASE_Set3DShutterLineShift( UINT32 u4Enable, UINT32 u4Shift)
{
    UINT32 u4Plane2Region = 0, u4Plane3Region = 0, u4Plane1Region = 0;
	INT32 u4PlaneOutputXShift = 0, u4PlaneOutputX = 0;
	static UINT32 u4Enable_tag = 0;
	static UINT32 u4Plane1OutputH_tag = 0, u4Plane2OutputH_tag = 0, u4Plane3OutputH_tag = 0;

	printf("u4Enable_tag = 0x%x\n",u4Enable_tag);

	if( !(u4Enable_tag) )
	{
		_OSD_BASE_GetScrnHStartOsd1(&u4Plane1OutputH_tag);
		_OSD_BASE_GetScrnHStartOsd2(&u4Plane2OutputH_tag);
		_OSD_BASE_GetScrnHStartOsd3(&u4Plane3OutputH_tag);

		u4Enable_tag = 1;
	}	
	if( u4Shift < 0x80 )
	{
		u4PlaneOutputXShift = - u4Shift / 2;
	}
	else if( u4Shift > 0x80 )
	{
		u4PlaneOutputXShift = ( u4Shift - 0x80 ) / 2;
	}

	
    u4Plane2Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2); 
	if (u4Plane2Region >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE get region fail \n");
        //return (INT32)OSD_RET_OK;
    }
    else if (u4Plane2Region < OSD_MAX_NUM_RGN)
    {  
        if (u4Enable == 1)
        {
           u4PlaneOutputX = u4Plane2OutputH_tag + u4PlaneOutputXShift;
		   _OSD_BASE_SetScrnHStartOsd2(u4PlaneOutputX);
		   
		   _OSD_3D_SetShutterShiftEn(TRUE);
		   _OSD_3D_SetTgenLREn(TRUE);
		   _OSD_3D_SetOsd2ShiftValue((u4Shift>0)?u4Shift:(0-u4Shift));
        }
        else
        {
		   _OSD_3D_SetShutterShiftEn(FALSE);
		   _OSD_3D_SetTgenLREn(FALSE);
		   _OSD_3D_SetOsd2ShiftValue(0);

		   _OSD_BASE_SetScrnHStartOsd2(u4Plane2OutputH_tag);
		   u4Enable_tag = 0;
        }
    }

	 u4Plane3Region = OSD_PLA_GetFirstRegion(OSD_PLANE_3); 
	if (u4Plane3Region >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE get region fail \n");
       // return (INT32)OSD_RET_OK;
    }
   else if (u4Plane3Region < OSD_MAX_NUM_RGN)
    {  
        if (u4Enable == 1)
        {
        	u4PlaneOutputX = u4Plane3OutputH_tag + u4PlaneOutputXShift;
		   _OSD_BASE_SetScrnHStartOsd3(u4PlaneOutputX);
		   
		   _OSD_3D_SetShutterShiftEn(TRUE);
		   _OSD_3D_SetTgenLREn(TRUE);
		   _OSD_3D_SetOsd2ShiftValue((u4Shift>0)?u4Shift:(0-u4Shift));
        }
        else
        {
		   _OSD_3D_SetShutterShiftEn(FALSE);
		   _OSD_3D_SetTgenLREn(FALSE);
		   _OSD_3D_SetOsd2ShiftValue(0);

		   _OSD_BASE_SetScrnHStartOsd3(u4Plane3OutputH_tag);
		   u4Enable_tag = 0;
        }
    }

	u4Plane1Region = OSD_PLA_GetFirstRegion(OSD_PLANE_1); 
	if (u4Plane1Region >= (UINT32)OSD_MAX_NUM_RGN)
    {
        Printf(" OSD_PLANE get region fail \n");
       // return (INT32)OSD_RET_OK;
    }
   else if (u4Plane1Region < OSD_MAX_NUM_RGN)
    {  
        if (u4Enable == 1)
        {
           u4PlaneOutputX = u4Plane1OutputH_tag + u4PlaneOutputXShift;
		   _OSD_BASE_SetScrnHStartOsd1(u4PlaneOutputX);
		   
		   _OSD_3D_SetShutterShiftEn(TRUE);
		   _OSD_3D_SetTgenLREn(TRUE);
		   _OSD_3D_SetOsd2ShiftValue((u4Shift>0)?u4Shift:(0-u4Shift));
        }
        else
        {
		   _OSD_3D_SetShutterShiftEn(FALSE);
		   _OSD_3D_SetTgenLREn(FALSE);
		   _OSD_3D_SetOsd2ShiftValue(0);

		   _OSD_BASE_SetScrnHStartOsd1(u4Plane1OutputH_tag);
		   u4Enable_tag = 0;
        }
    }
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    VERIFY((INT32)OSD_RET_OK == _OSD_3D_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}


INT32 OSD_BASE_SetLineShift(UINT32 u4Enable, UINT32 u4Shift )
{

   switch(u4Enable)
   	{
   	 case 0:
	 	OSD_BASE_Set3DPrLineShift(FALSE, 0);
		OSD_BASE_Set3DShutterLineShift(FALSE, 0);
	 	break;
	 case 1:
	 	OSD_BASE_Set3DPrLineShift(TRUE, u4Shift);
		break;
	 case 2:
	 	OSD_BASE_Set3DShutterLineShift(TRUE, u4Shift);
		break;
	 case 3:
	 	OSD_BASE_Set3DPrLineShift(FALSE, 0);
	 	break;
	 case 4:
	 	OSD_BASE_Set3DShutterLineShift(FALSE, 0);
		break;
	default:
			break;
	
   	}  
    return (INT32)OSD_RET_OK;
}

BOOL OSD_BASE_Get_3DOSD2PLsEn()
{
	return OSD2_3DPrLshift_ENABLE_FLAG;
}

void OSD_BASE_Set_3DOSD2PLsEn(BOOL u4Enable)
{
	OSD2_3DPrLshift_ENABLE_FLAG = u4Enable;
}
INT32 OSD_3D_SetOsd2LShiftEn(UINT32 u4Value)
{
	#ifndef CONFIG_OSD_COMPRESSION 
		return _OSD_3D_SetOsd2LShiftEn(u4Value);
	#else
		OSD_BASE_Set_3DOSD2PLsEn(u4Value);
		return (INT32)OSD_RET_OK;
	#endif
}
INT32 OSD_3D_SetOsd1LShiftEn(UINT32 u4Value)
{
	return _OSD_3D_SetOsd1LShiftEn(u4Value);
}
INT32 OSD_3D_SetOsd3LShiftEn(UINT32 u4Value)
{
	return _OSD_3D_SetOsd3LShiftEn(u4Value);
}
#endif

BOOL OSD_BASE_IsDisplaying(void)
{
 BOOL fgEnable = 0;
 UINT32 u4Enabe1 =0, u4Enable2=0;
  _OSD_PLA_GetEnable(OSD_PLANE_2, &u4Enabe1);
  _OSD_PLA_GetEnable(OSD_PLANE_3, &u4Enable2);
  
  _OSD_SC_SetAlphaCountEn(OSD_SCALER_2, TRUE);
  IGNORE_RET(_OSD_SC_UpdateHwReg(OSD_SCALER_2));
  _OSD_SC_SetAlphaCountEn(OSD_SCALER_3, TRUE);
  IGNORE_RET(_OSD_SC_UpdateHwReg(OSD_SCALER_3));
  
//  printf("u4Enabe1 =%d, u4Enabe2=%d, value1=0x%x, value2=0x%x\n",u4Enabe1, u4Enable2, IO_READ32(0xF0021000, 0x5F8),IO_READ32(0xF0021000, 0x6F8));
 if((u4Enabe1 && IO_READ32(0xF0021000, 0x5F8)) || (u4Enable2 && IO_READ32(0xF0021000, 0x6F8)))
 {
   fgEnable = TRUE;
 }
 else
 {
   fgEnable = FALSE;
 }
  _OSD_SC_SetAlphaCountEn(OSD_SCALER_2, FALSE);
  _OSD_SC_SetAlphaCountEn(OSD_SCALER_3, FALSE);	
  IGNORE_RET(_OSD_SC_UpdateHwReg(OSD_SCALER_2));
  IGNORE_RET(_OSD_SC_UpdateHwReg(OSD_SCALER_3));
  
  return fgEnable;
}



#ifdef CC_TDTV_3D_OSD_ENABLE
#ifdef CC_3D_OSD_SUPPORT

INT32 OSD_BASE_Enabe3DShutter( UINT32 u4Enable, UINT32 u4Shift)
{
    UINT32 u4Plane2Region = 0, u4Plane3Region = 0;
    UINT32 u4Plane2RightRegion = 0, u4Plane3RightRegion = 0;
    UINT32 u4u4LeftRgnAddr;
    UINT32 u4u4RightRgnAddr;
    UINT32 u4RegionPosX = 0;

    if (u4Enable == 0)
    {
        if (_u1Plane2RightRegion != 0)
        {   
            OSD_VERIFY_REGION(_u1Plane2RightRegion);
            OSD_RGN_Delete(_u1Plane2RightRegion);
            _u1Plane2RightRegion = 0;
	     _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_2, 0);
	     OSD_PLA_Set3DMode(OSD_PLANE_2, u4Enable);
	     VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_2));
        }
        if (_u1Plane3RightRegion != 0)
        {   
            OSD_VERIFY_REGION(_u1Plane3RightRegion);
            OSD_RGN_Delete(_u1Plane3RightRegion);
            _u1Plane3RightRegion = 0;
	     _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_3, 0);
	     OSD_PLA_Set3DMode(OSD_PLANE_3, u4Enable);
	     VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_3));
        }
        return (INT32)OSD_RET_OK;
    }

    u4Plane2Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
    u4Plane3Region = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
    if ((u4Plane2Region >= (UINT32)OSD_MAX_NUM_RGN) && (u4Plane3Region >= (UINT32)OSD_MAX_NUM_RGN))
    {
        Printf(" OSD_PLANE get region fail \n");
        return (INT32)OSD_RET_OK;
    }
    if (u4Plane2Region < OSD_MAX_NUM_RGN)
    {
        UINT32 u4BmpW, u4BmpH, u4DispW, u4DispH, u4ColorMode;
        UINT32 u4BmpAddr, u4BmpPitch;
        UINT32 u4ret;

        if (_u1Plane2RightRegion != 0)
        {
            Printf(" OSD Plane 2 3D  Shutter have enable l \n");
        }
        else
        {
            OSD_VERIFY_REGION(u4Plane2Region);

            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_W, &u4BmpW));
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_H, &u4BmpH));
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_DISP_W, &u4DispW));
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_DISP_H, &u4DispH));
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_COLORMODE, &u4ColorMode));
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_ADDR, &u4BmpAddr));
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_PITCH, &u4BmpPitch));

            u4ret = OSD_RGN_Create(&u4Plane2RightRegion, u4BmpW, u4BmpH, (void *)u4BmpAddr,
                                   u4ColorMode, u4BmpPitch, 0, 0, u4DispW, u4DispH);
            if (u4ret != OSD_RET_OK)
            {
                Printf("OSD create region fail \n");
                return 2;
            }  

            VERIFY((INT32)OSD_RET_OK ==
                   _OSD_RGN_GetAddress((UINT32)u4Plane2Region, &u4u4LeftRgnAddr));

            VERIFY((INT32)OSD_RET_OK ==
                   _OSD_RGN_GetAddress((UINT32)u4Plane2RightRegion, &u4u4RightRgnAddr));	
		
              x_memcpy((void*)u4u4RightRgnAddr, (void*)u4u4LeftRgnAddr, sizeof(OSD_RGN_UNION_T));
			  	
            if (u4Shift < 0x40 && u4Shift > 0)
            {
                IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_POS_X, &u4RegionPosX));
                u4RegionPosX = u4RegionPosX +   u4Shift;
		  Printf(" OSD Plane test 000000	 %d l \n", u4RegionPosX);
                IGNORE_RET(OSD_RGN_Set(u4Plane2RightRegion, (INT32)OSD_RGN_POS_X, u4RegionPosX));
	    }
			
            VERIFY((INT32)OSD_RET_OK ==
                   _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_2, u4u4RightRgnAddr));
			
            OSD_PLA_Set3DMode(OSD_PLANE_2, u4Enable);
            VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_2));
			
	    _u1Plane2RightRegion = u4Plane2RightRegion;
        }
    }

    if (u4Plane3Region < OSD_MAX_NUM_RGN)
    {
        UINT32 u4BmpW, u4BmpH, u4DispW, u4DispH, u4ColorMode;
        UINT32 u4BmpAddr, u4BmpPitch;
        UINT32 u4ret;

        if (_u1Plane3RightRegion != 0)
        {
            Printf(" OSD Plane 3 3D  Shutter have enable l \n");
        }
        else
        {
            OSD_VERIFY_REGION(u4Plane3Region);

            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_W, &u4BmpW));
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_H, &u4BmpH));
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_DISP_W, &u4DispW));
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_DISP_H, &u4DispH));
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_COLORMODE, &u4ColorMode));
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_ADDR, &u4BmpAddr));
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_PITCH, &u4BmpPitch));

            u4ret = OSD_RGN_Create(&u4Plane3RightRegion, u4BmpW, u4BmpH, (void *)u4BmpAddr,
                                   u4ColorMode, u4BmpPitch, 0, 0, u4DispW, u4DispH);
            if (u4ret != OSD_RET_OK)
            {
                Printf("OSD create region fail \n");
                return 2;
            }
            VERIFY((INT32)OSD_RET_OK ==
                   _OSD_RGN_GetAddress((UINT32)u4Plane3Region, &u4u4LeftRgnAddr));

            VERIFY((INT32)OSD_RET_OK ==
                   _OSD_RGN_GetAddress((UINT32)u4Plane3RightRegion, &u4u4RightRgnAddr));

            x_memcpy((void*)u4u4LeftRgnAddr, (void*)&u4u4RightRgnAddr, sizeof(OSD_RGN_UNION_T));

            if (u4Shift < 0x40 && u4Shift > 0)
            {
                IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_POS_X, &u4RegionPosX));
                u4RegionPosX = u4RegionPosX +   u4Shift;
                IGNORE_RET(OSD_RGN_Set(u4Plane3RightRegion, (INT32)OSD_RGN_POS_X, u4RegionPosX));
            }

            VERIFY((INT32)OSD_RET_OK ==
                   _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_3, u4u4RightRgnAddr));
            OSD_PLA_Set3DMode(OSD_PLANE_3, u4Enable);
            VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_3));
	    _u1Plane3RightRegion = u4Plane3RightRegion;
        }
    }
    return (INT32)OSD_RET_OK;
}


INT32 OSD_BASE_Set3DStatus(void)
{
       S_TDTV_OSD_CONFIG sOsdCfg=eDrvTDTVOSDModeQuery();
	if(_u13DOsdShift !=0)     //for cli test use
	{
		return (INT32)OSD_RET_OK;	
	}
		
	  switch(sOsdCfg.eOSDType)
	   {
	     case E_TDTV_OSD_2D:
	     		OSD_BASE_Enabe3DShutter(FALSE, 0);
		 	gfgOsd_Sbs = 0;
			gfgOsd_Tbb = 0;
			_u13DOsdShift = 0;
		 	break;
            case E_TDTV_OSD_FS:
			gfgOsd_Sbs = 0;
			gfgOsd_Tbb = 0;
			_u13DOsdShift = 0;
			OSD_BASE_Enabe3DShutter(TRUE, 0);
			break;
            case E_TDTV_OSD_SBS:
			OSD_BASE_Enabe3DShutter(FALSE, 0);
			gfgOsd_Sbs = 1;
			gfgOsd_Tbb = 0;
			_u13DOsdShift = 0;
			break;
            case E_TDTV_OSD_TB:
			OSD_BASE_Enabe3DShutter(FALSE, 0);
			gfgOsd_Sbs = 0;
			gfgOsd_Tbb = 1;
			_u13DOsdShift = 0;
			break;
		default:
			OSD_BASE_Enabe3DShutter(FALSE, 0);
		 	gfgOsd_Sbs = 0;
			gfgOsd_Tbb = 0;
			_u13DOsdShift = 0;
			break;
	   }
     return (INT32)OSD_RET_OK;
}
#endif
#endif
