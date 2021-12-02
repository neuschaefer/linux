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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
  
 
 //-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_timer.h"
#include "x_debug.h"
#include "x_ckgen.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END

#include "x_hal_arm.h"
#include "pcmcia_if.h"
#include "spi_if.h"
#include "spi_hw.h"
#include "spi_isr.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static HANDLE_T _hSpiIfEntrySema = (HANDLE_T)NULL;
#endif
static BOOL fgSpiIfInited = FALSE;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** SPI_Init
 *  
 *  @param   
 *  @return
 */
//-------------------------------------------------------------------------
void SPI_Init(void)
{
    if ( fgSpiIfInited == TRUE )
    {
        return;
    }
#ifndef CC_MTK_LOADER
    if ( _hSpiIfEntrySema == (HANDLE_T)NULL )
    {
        VERIFY(OSR_OK == x_sema_create(&_hSpiIfEntrySema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) );
    }
#endif

    SPIHW_Init();
#ifndef CC_MTK_LOADER
    SPI_EnableISR();
#endif
    fgSpiIfInited = TRUE;
}

//-------------------------------------------------------------------------
/** SPI_Reset
 *  
 *  @param   
 *  @return
 */
//-------------------------------------------------------------------------
void SPI_Reset(void)
{
    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER    
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    SPIHW_Reset();
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
}

void SPI_SetPll(void)
{
    // 0xf000d210[3:0]
    // 2 - 72 MHz
    // 0 - 13.5 MHz
    // 7 - 13.5
    // 6 - 27
    // 5 - 36
    // 4 - 54
    UINT32 u4SpiClk = 0;
    UINT8 u1Val;
#ifndef CC_MTK_LOADER
    DRVCUST_OptQuery(eCISpiClock, &u4SpiClk);
#endif
    switch (u4SpiClk)
    {
        case 27:
            u1Val = 0x6;
            break;
        case 36:
            u1Val = 0x5;
            break;
        case 54:
            u1Val = 0x4;
            break;
        case 72:
            u1Val = 0x2;
            break;
        case 14: // 13.5
            u1Val = 0x0;
            break;
        default:
            u4SpiClk = 27;
            u1Val = 0x6;
            break;
    }
#ifdef CC_MTK_LOADER
    Printf("SPI_SetPll(), spi clock: %d Mhz\n", u4SpiClk);
#else
    LOG(0, "SPI_SetPll(), spi clock: %d Mhz\n", u4SpiClk);
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    CKGEN_WRITE32( 0x210, (CKGEN_READ32(0x210) & 0xFFFFFFF0) | u1Val ); // SPI clock    
    IO_WRITE32(0xf0008314, 0, (IO_READ32(0xf0008314, 0) & 0xCFFFFFFF) | (1<<28)); // SPI delay
    //CKGEN_WRITE32( 0x314, (CKGEN_READ32(0x314) & 0xFFEFFFFF) | 0x100000); // SPI clock pin driving
    CKGEN_WRITE32( 0x390, (CKGEN_READ32(0x390) & 0xFFFFFFF8) | 0x7 ); // SPI pin driving 
    // Just write 0x390 to set E4=1 is OK. Since 0x3bc make nand un-correctable ECC, mark it.
    //CKGEN_WRITE32( 0x3bc, (CKGEN_READ32(0x3bc) & 0xFFFFFFF8) | 0x7 ); // SPI pin driving  
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
}

//-------------------------------------------------------------------------
/** SPI_SDAWrite
 *  
 *  @param   
 *  @return
 */
//-------------------------------------------------------------------------
INT32 SPI_SDAWrite(const UINT32* pu4Data, UINT32 u4Addr)
{
    INT32 i4Ret;

    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER    
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    i4Ret = SPIHW_SDAWrite(pu4Data, u4Addr);
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif

    return i4Ret;
}

static INT32 SPI_SDAWriteNoLock(const UINT32* pu4Data, UINT32 u4Addr)
{
    INT32 i4Ret;

    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
    i4Ret = SPIHW_SDAWrite(pu4Data, u4Addr);

    return i4Ret;
}


//-------------------------------------------------------------------------
/** SPI_SDARead
 *  
 *  @param   
 *  @return
 */
//-------------------------------------------------------------------------
INT32 SPI_SDARead(UINT32* pu4Data, UINT32 u4Addr)
{
    INT32 i4Ret;
    
    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    i4Ret = SPIHW_SDARead(pu4Data, u4Addr);
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif

    return i4Ret;
}

static INT32 SPI_SDAReadNoLock(UINT32* pu4Data, UINT32 u4Addr)
{
    INT32 i4Ret;
    
    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
    i4Ret = SPIHW_SDARead(pu4Data, u4Addr);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SPI_DMAWrite
 *  
 *  @param   
 *  @return
 */
//-------------------------------------------------------------------------
INT32 SPI_DMAWrite(const UINT32* pu4Data, UINT32 u4Addr, UINT32 u4DataLen)
{
    INT32 i4Ret;

    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    HalDmaBufferStart((void*)pu4Data, u4DataLen*4, HAL_DMA_TO_DEVICE);
    i4Ret = SPIHW_DMAWrite(pu4Data, u4Addr, u4DataLen);
    HalDmaBufferDone((void*)pu4Data, u4DataLen*4, HAL_DMA_TO_DEVICE);
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SPI_DMARead
 *  
 *  @param   
 *  @return
 */
//-------------------------------------------------------------------------
INT32 SPI_DMARead(UINT32* pu4Data, UINT32 u4Addr, UINT32 u4DataLen)
{
    INT32 i4Ret;

    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));    
#endif
    HalDmaBufferStart((void*)pu4Data, u4DataLen*4, HAL_DMA_FROM_DEVICE);
    i4Ret = SPIHW_DMARead(pu4Data, u4Addr, u4DataLen);
    HalDmaBufferDone((void*)pu4Data, u4DataLen*4, HAL_DMA_FROM_DEVICE);
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
    return i4Ret;
}

INT32 SPI_DemodSingleRead(UINT8* pu1Data, UINT32 u4Addr)
{
    UINT32 u4DataTmp;
    UINT32 u4ByteIndex = (u4Addr%4);
    UINT32 u4WaitLoopCount = 0xffff;
    UINT8 u1ByteEn=0;

    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    // Set WDATA0 ~ WDATA3
    switch ( u4ByteIndex )
    {
        case 0: 
            u1ByteEn = 0x1;
            break;
        case 1: 
            u1ByteEn = 0x2;
            break;
        case 2: 
            u1ByteEn = 0x4;
            break;
        case 3: 
            u1ByteEn = 0x8;
            break;
    }

    // Set CTRL0 ~ CTRL3
    u4DataTmp = 0x0;
    u4DataTmp |= (u4Addr>>2);
    u4DataTmp |= (0x00ff0000 & (u1ByteEn <<16));
    u4DataTmp |= SPI_DEMOD_REG_PROXY_READ_ENABLE;
    SPI_SDAWriteNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_CTRL);
    
    // check result
    while(u4WaitLoopCount >0)
    {
        u4WaitLoopCount--;
        SPI_SDAReadNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_STATUS);
        u4DataTmp = ((u4DataTmp>>2)&0x3);
        if ( u4DataTmp == SPI_DEMOD_REG_PROXY_STATUS_READ_ERROR )
        {
#ifdef CC_MTK_LOADER
            Printf("SPI_DemodSingleRead read error !\n");
#else
            LOG(0, "SPI_DemodSingleRead read error !\n");
            VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
            return -1;
        }
        else if ( u4DataTmp == SPI_DEMOD_REG_PROXY_STATUS_READ_READY )
        {
            break;
        }
        else if ( u4DataTmp == SPI_DEMOD_REG_PROXY_STATUS_READ_OK )
        {
/* print "SPI_DemodSingleRead timeout !" below.
            LOG(0, "SPI_DemodSingleRead OK !? \n");
*/
        }
    }

    // set return value
    if ( u4DataTmp == SPI_DEMOD_REG_PROXY_STATUS_READ_READY )
    {
        switch ( u4ByteIndex )
        {
            case 0: 
                SPI_SDAReadNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_RDATA);
                *pu1Data = u4DataTmp;
                break;
            case 1: 
                SPI_SDAReadNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_RDATA);
                *pu1Data = ((u4DataTmp>>8)&0x000000ff);
                break;
            case 2: 
                SPI_SDAReadNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_RDATA);
                *pu1Data = ((u4DataTmp>>16)&0x000000ff);
                break;
            case 3: 
                SPI_SDAReadNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_RDATA);
                *pu1Data = ((u4DataTmp>>24)&0x000000ff);
                break;
        }
#ifndef CC_MTK_LOADER
        VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
        return 0;
    }
    else
    {
#ifdef CC_MTK_LOADER
        Printf("SPI_DemodSingleRead timeout !\n");
#else
        LOG(0, "SPI_DemodSingleRead timeout !\n");
        VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
        return -1;
    }
}

INT32 SPI_DemodSingleWrite(const UINT8* pu1Data, UINT32 u4Addr)
{
    UINT32 u4DataTmp;
    UINT32 u4ByteIndex = (u4Addr%4);
    UINT32 u4WaitLoopCount = 0xffff;
    UINT8 u1ByteEn=0;

    if ( fgSpiIfInited == FALSE )
    {
        SPI_Init();
    }
    
#ifndef CC_MTK_LOADER
    VERIFY(OSR_OK == x_sema_lock(_hSpiIfEntrySema, X_SEMA_OPTION_WAIT));
#endif
    // Set WDATA0 ~ WDATA3
    switch ( u4ByteIndex )
    {
        case 0: 
            u4DataTmp = (0xff & (*pu1Data));
            u1ByteEn = 0x1;
            break;
        case 1: 
            u4DataTmp = (0xff00 & ((*pu1Data)<<8));
            u1ByteEn = 0x2;
            break;
        case 2: 
            u4DataTmp = (0xff0000 & ((*pu1Data)<<16));
            u1ByteEn = 0x4;
            break;
        case 3: 
            u4DataTmp = (0xff000000 & ((*pu1Data)<<24));
            u1ByteEn = 0x8;
            break;
    }
    SPI_SDAWriteNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_WDATA);

    // Set CTRL0 ~ CTRL3
    u4DataTmp = 0x0;
    u4DataTmp |= (u4Addr>>2);
    u4DataTmp |= (0x00ff0000 & (u1ByteEn <<16));
    u4DataTmp |= SPI_DEMOD_REG_PROXY_WRITE_ENABLE;
    SPI_SDAWriteNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_CTRL);
    

    // check result
    while(u4WaitLoopCount >0)
    {
        u4WaitLoopCount--;
        SPI_SDAReadNoLock(&u4DataTmp, SPI_DEMOD_REG_PROXY_STATUS);
        u4DataTmp = (u4DataTmp&0x3);
        if ( u4DataTmp == SPI_DEMOD_REG_PROXY_STATUS_WRITE_OK )
        {
#ifndef CC_MTK_LOADER
            VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
            return 0;
        }
        else if ( u4DataTmp == SPI_DEMOD_REG_PROXY_STATUS_WRITE_ERROR )
        {
#ifdef CC_MTK_LOADER
            Printf("SPI_DemodSingleWrite write error !\n");
#else
            LOG(0, "SPI_DemodSingleWrite write error !\n");
            VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
            return -1;
        }
    }
#ifdef CC_MTK_LOADER
    Printf("SPI_DemodSingleWrite timeout !\n");
#else
    LOG(0, "SPI_DemodSingleWrite timeout !\n");
    VERIFY(OSR_OK == x_sema_unlock(_hSpiIfEntrySema));
#endif
    return -1;
}


