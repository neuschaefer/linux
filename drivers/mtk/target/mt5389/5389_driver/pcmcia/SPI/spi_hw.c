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
#include "x_hal_926.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_timer.h"
#include "x_debug.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END

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
HANDLE_T _hSpiIsrSema = (HANDLE_T)NULL;
#endif
static BOOL fgSpiInited = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

// SPIHW_Init only once in loader.
// In TV image, just create semaphore and set flag.
void SPIHW_Init(void)
{
#ifdef CC_MTK_LOADER
    UINT32 u4SpiPin = 0;
#endif
    if ( fgSpiInited == TRUE )
    {
        return;
    }

#ifndef CC_MTK_LOADER
    if ( _hSpiIsrSema == (HANDLE_T)NULL )
    {
        VERIFY(OSR_OK == x_sema_create(&_hSpiIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) );
    }
#endif

#ifdef CC_MTK_LOADER
    DRVCUST_InitQuery(eCISpiPinmux, &u4SpiPin);
    if (u4SpiPin == 0)
    {
    Printf(0, "SPIHW_Init ----- Set SPI PinMux0\n");
    BSP_PinSet( PIN_GPIO0, PINMUX_FUNCTION4 );
    BSP_PinSet( PIN_GPIO1, PINMUX_FUNCTION4 );
    BSP_PinSet( PIN_GPIO2, PINMUX_FUNCTION4 );
    }
    else
    {
    CKGEN_WRITE32(0x1d4, (CKGEN_READ32(0x1d4) | (0xF << 16)));
    CKGEN_WRITE32(0x6e4, (CKGEN_READ32(0x6e4) | 2));
    Printf(0, "SPIHW_Init ----- Set SPI PinMux1:\n");
    BSP_PinSet( PIN_TXVN_1, PINMUX_FUNCTION3 );
    BSP_PinSet( PIN_TXVP_1, PINMUX_FUNCTION3 );
    BSP_PinSet( PIN_TXVN_0, PINMUX_FUNCTION3 );
    Printf(0, " 0xF000D1D4[19:16]=0xF:0x%x\n", CKGEN_READ32(0x1d4));
    Printf(0, " 0xF000D6E4[1]=1:0x%x\n", CKGEN_READ32(0x6e4));
    Printf(0, " 0xF000D400[20:18]=3:0x%x\n", CKGEN_READ32(0x400));
    }

    //IO_WRITE32(0xf000d514, 0, IO_READ32(0xf000d514, 0)|0x4000); // TS CLK should be set as GPO, but pinmux is function 1 for SPI.    
    SPI_WRITE32(SPI_CONTROL ,_SPI_READ_WAIT_STATE(16));
    Printf("SPIHW_Init ----- Set SPI PinMux\n");
    Printf("SPI dummy delay:0x%x\n", SPI_READ32(SPI_CONTROL));
#endif
    fgSpiInited = TRUE;
    return;

}
void SPIHW_Reset(void)
{
    if ( fgSpiInited == FALSE )
    {
        SPIHW_Init();
    }
    
    SPI_WRITE32(SPI_TRIGGER ,SPI_READ32(SPI_TRIGGER)| _SPI_RESET);
    while(SPI_READ32(SPI_TRIGGER)& _SPI_TRANS_START);
    HAL_Delay_us(1);
    SPI_WRITE32(SPI_TRIGGER ,SPI_READ32(SPI_TRIGGER)| _SPI_RESET);
}
INT32 SPIHW_SDAWrite(const UINT32* pu4Data, UINT32 u4Addr)
{
    if ( fgSpiInited == FALSE )
    {
        SPIHW_Init();
    }

    SPI_WRITE32(SPI_SDA_WRITE_DATA, *pu4Data);
    SPI_WRITE32(SPI_TRIGGER, _SPI_WRITE_EN|_SPI_ADDRESS(u4Addr));
    SPI_WRITE32(SPI_TRIGGER, _SPI_TRANS_START|SPI_READ32(SPI_TRIGGER));
    while(SPI_READ32(SPI_TRIGGER)& _SPI_TRANS_START);

    return 0;
}
INT32 SPIHW_SDARead(UINT32* pu1Data, UINT32 u4Addr)
{
    if ( fgSpiInited == FALSE )
    {
        SPIHW_Init();
    }
    
    SPI_WRITE32(SPI_TRIGGER, _SPI_ADDRESS(u4Addr));
    SPI_WRITE32(SPI_TRIGGER, _SPI_TRANS_START|SPI_READ32(SPI_TRIGGER));
    while(SPI_READ32(SPI_TRIGGER)& _SPI_TRANS_START);
    *pu1Data = SPI_READ32(SPI_SDA_READ_DATA);

    return 0;
}
INT32 SPIHW_DMAWrite(const UINT32* pu4Data, UINT32 u4Addr, UINT32 u4DataLen)
{
    if ( fgSpiInited == FALSE )
    {
        SPIHW_Init();
    }
    
    SPI_WRITE32(SPI_DMA_ADDR, _SPI_ADDR((UINT32)pu4Data));

    SPI_WRITE32(SPI_TRIGGER, _SPI_DMA_EN|_SPI_WRITE_EN
                |_SPI_ADDRESS(u4Addr)| _SPI_LEN(u4DataLen));

    SPI_WRITE32(SPI_TRIGGER, _SPI_TRANS_START|SPI_READ32(SPI_TRIGGER));

#ifndef CC_MTK_LOADER
//    while(SPI_READ32(SPI_TRIGGER)& _SPI_TRANS_START);
    VERIFY(OSR_OK == x_sema_lock(_hSpiIsrSema, X_SEMA_OPTION_WAIT));
#endif
    return 0;
}
INT32 SPIHW_DMARead(UINT32* pu4Data, UINT32 u4Addr, UINT32 u4DataLen)
{
    if ( fgSpiInited == FALSE )
    {
        SPIHW_Init();
    }
    
    SPI_WRITE32(SPI_DMA_ADDR, _SPI_ADDR((UINT32)pu4Data));

    SPI_WRITE32(SPI_TRIGGER, _SPI_DMA_EN
                 | _SPI_ADDRESS(u4Addr)| _SPI_LEN(u4DataLen));

    SPI_WRITE32(SPI_TRIGGER, _SPI_TRANS_START|SPI_READ32(SPI_TRIGGER));

#ifndef CC_MTK_LOADER
//    while(SPI_READ32(SPI_TRIGGER)& _SPI_TRANS_START);
    VERIFY(OSR_OK == x_sema_lock(_hSpiIsrSema, X_SEMA_OPTION_WAIT));
#endif
    return 0;
}


