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
 * $RCSfile: musb_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_cmd.c
 *  MUSB module CLI test function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN 

#define MUSB_EHSET

#include "drvcust_if.h"

#include "musb_if.h"
#include "mu_cdi.h"
#include "mu_impl.h"
#include "mu_cdc_if.h"
#include "mu_hfi_if.h"
#include "mu_hdrdf.h"
#include "mu_audio.h"
#include "mu_hid_if.h"

#include "x_debug.h"
#include "x_util.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_typedef.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

LINT_EXT_HEADER_END 

#ifndef CC_USB_DISABLE

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
//#define MUSB_ISO_EMULATION
//#define MUSB_AUD
//#define MUSB_HID
//#define MUSB_CDC
//#define MUSB_TWO_PORT
//#define MUSB_QMU
//#define MUSB_POWERDOWN
#ifdef MUSB_QMU
#define MUSB_ISO_EMULATION
#endif
#define MGC_Printf Printf
//#define MGC_Printf
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MUSB_REG_LENGTH      (0x210)

/* One MUSB phy setting rule two ports all */ 
#define MUSB_P0_BASE  (0x20029000)
#define MUSB_P1_BASE  (0x2002E000)

/* Macro for the size of fixed array */
#define MUSB_ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define MGC_AUD_TST_BUF_SIZE   (100000) //(1076084)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef struct {
    INT32   reg;    /** the register offset */
    INT32   msb;    /** the MSB of setting in register */
    INT32   lsb;    /** the LSB of setting in register */
    char*   name;   /** the name of finetune setting */
    char*   descr;  /** Description. Can be NULL if no description */
} MUSB_FINETUNE_T;


 typedef struct _MUSB_DEV_T
 {
    UINT32 lun;   
    UINT32 sector_start_offset;    
    UINT32 sector_num;
    INT32 i4Argc;
    UINT32 option;
  }MUSB_DEV_T;



//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
#ifdef MUSB_ISO_EMULATION
/*
 * force host to send and receive isochronous packet.
 */
extern void MGC_Host_SendISOPkt(UINT32 bEnd, UINT32 pktsize, 
    UINT8 *buf, UINT32 framenum, UINT32 balance, UINT32 interval);
extern void MGC_Host_GetISOPkt(UINT32 bEnd, UINT32 pktsize, 
    UINT8 *buf, UINT32 framenum, UINT32 balance, UINT32 interval);

/*
 * force device to send and receive isochronous packet.
 */
extern void MGC_Dev_SendISOPkt(UINT32 bEnd, UINT32 pktsize, 
    UINT8 *buf, UINT32 framenum, UINT32 balance);
extern UINT32 MGC_Dev_GetISOPkt(UINT32 bEnd, UINT8 *buf, UINT32 framenum);
#ifdef MUSB_QMU
//extern int TestQMUBulkLoopback(int i4Argc, const char **szArgv);
//extern int _MUSB_QmuInit(int i4Agc, const char **szArgv);
//extern void MGC_QMU_Host_GetPkt(UINT8 bRxEnd, UINT32 u4type, UINT32 framenum, UINT8 ishost);
//extern void MGC_QMU_Host_SendPkt(UINT8 bTxEnd, UINT32 u4type, UINT32 framenum, UINT8 ishost);
#endif
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
/*
static REG_TEST_T arMUSBRgtList [] =
{
    { MUSB_COREBASE+MGC_O_HDRC_FADDR, eRD_WR, 1, 0x0000007F, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_POWER, eWR_ONLY, 1, 0x000000E5, 1, 0x20 },
    { MUSB_COREBASE+MGC_O_HDRC_POWER, eRD_ONLY, 1, 0x000000FF, 1, 0x20 },
    { MUSB_COREBASE+MGC_O_HDRC_INTRTX, eRD_ONLY, 2, 0x0000FFFF, 1, 0x0000 },
    { MUSB_COREBASE+MGC_O_HDRC_INTRRX, eRD_ONLY, 2, 0x0000FFFF, 1, 0x0000 },
    { MUSB_COREBASE+MGC_O_HDRC_INTRTXE, eRD_WR, 2, 0x0000001F, 1, 0x001F },
    { MUSB_COREBASE+MGC_O_HDRC_INTRRXE, eRD_WR, 2, 0x0000001E, 1, 0x001E },
    { MUSB_COREBASE+MGC_O_HDRC_INTRUSB, eRD_ONLY, 1, 0x000000FF, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_INTRUSBE, eRD_WR, 1, 0x000000FF, 1, 0x06 },
    { MUSB_COREBASE+MGC_O_HDRC_FRAME, eRD_ONLY, 1, 0x000000FF, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_INDEX, eRD_WR, 1, 0x0000000F, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_TESTMODE, eRD_WR, 1, 0x000000BF, 1, 0x00 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_TXMAXP, eRD_WR, 2, 0x0000FFFF, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_CSR0, eRD_ONLY, 2, 0x0000FFFF, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_CSR0, eWR_ONLY, 2, 0x000009FF, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_RXMAXP, eRD_WR, 2, 0x0000FFFF, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_RXCSR, eRD_ONLY, 2, 0x0000FF6F, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_RXCSR, eWR_ONLY, 2, 0x0000E8FD, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_RXCOUNT, eRD_ONLY, 2, 0x00001FFF, 1, 0x0000 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_TXTYPE, eRD_WR, 1, 0x0000003F, 1, 0x00 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_TXINTERVAL, eRD_WR, 1, 0x000000FF, 1, 0x00 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_RXTYPE, eRD_WR, 1, 0x0000003F, 1, 0x00 },
    { MUSB_COREBASE+0x10 + MGC_O_HDRC_RXINTERVAL, eRD_WR, 1, 0x000000FF, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_DEVCTL, eRD_ONLY, 1, 0x000000FF, 1, 0x98 },
    { MUSB_COREBASE+MGC_O_HDRC_TXFIFOSZ, eRD_WR, 1, 0x0000001F, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_RXFIFOSZ, eRD_WR, 1, 0x0000001F, 1, 0x00 },
    { MUSB_COREBASE+MGC_O_HDRC_TXFIFOADD, eRD_WR, 2, 0x00001FFF, 1, 0x0000 },
    { MUSB_COREBASE+MGC_O_HDRC_RXFIFOADD, eRD_WR, 2, 0x00001FFF, 1, 0x0000 },

    // End.
    { 0xffff, eNO_TYPE, -1, 0, 0, 0 }
};
*/

static MUSB_FINETUNE_T _arFinetune[] = {
    /* Register   MSB LSB  Name           Description */
    { M_REG_PHYC4, 6, 4, "HS_TERMC", "0: 480mV, 1: 460mV, 2:440mV, ..." },
    { M_REG_PHYC2, 6, 4, "IADJ", "Depend on ACD designer" },
    { M_REG_PHYC4,  17,  16, "TX_RISE", "0: min rt/ft, 3: min rt/ft" }
};

#ifdef MUSB_CDC
static UINT32 *_pu4MusbMemCmpPool = NULL;
static UINT8 *_pu1MusbAsyncRxBuf = NULL;
#endif
static void *_pvMusbMemPool = NULL;
static UINT32 u4VbusLoopCount = 0;
static UINT32 u4VbusTotalLoop = 0;
static UINT32 u4VbusOffTime = 0;
static UINT32 u4VbusNewOffTime = 0;
static UINT32 u4VbusOnTime = 0;
static UINT32 u4VbusOffIncGapTime = 0;
static HANDLE_T phVbusTimerHdl;
static HANDLE_T phVbusThreadHdl;
static HANDLE_T phVbusSemaHdl;
static BOOL fgVbusStatus = FALSE;

#ifdef MUSB_AUD
static MUSB_AUDParam rAudPlayParm;
static MUSB_AudStreamIrp rAudStreamOutIrp;
static MUSB_AudStreamIrp rAudStreamInIrp;
static MUSB_AudIsoPtr rAudIsoPtr;
static UINT8 *MGC_aAudRecBuf;
#endif

static HANDLE_T phUSBThreadHdl;
static UINT32 u4ReadErrCount = 0;
static UINT32 u4WriteErrCount = 0;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _MUSBSetModeCmd
 *  USB set host or device mode function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBSetModeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
  
    if (i4Argc < 2)
    {
        MGC_Printf("musb.i 1  : init usb to host mode.\n");
        MGC_Printf("musb.i 0  : init usb to dmx capture device.\n");
        MGC_Printf("musb.i 2  : init usb to usb to uart device.\n");
        MGC_Printf("musb.i 3  : init usb to msd device.\n");        
        return 0;
    }

    ASSERT(szArgv);

    // operation mode. host or device.
    u4Mode = StrToInt(szArgv[1]);

    switch (u4Mode)
    {
        case 0: /* dmx capture device mode. */
            (void)MUSB_Init(MUSB_DEVICE_MODE);
            (void)MUSB_InitDeviceMode(MUSB_DEVICE_MODE_CAP);
            break;
        case 1: /* host mode. */
            (void)MUSB_Init(MUSB_HOST_MODE);
            break;
        case 2: /* usb to uart device mode. */
            (void)MUSB_Init(MUSB_DEVICE_MODE);
            (void)MUSB_InitDeviceMode(MUSB_DEVICE_MODE_CDC);
            break;
        case 3: /* msd device mode. */
            (void)MUSB_Init(MUSB_DEVICE_MODE);
            (void)MUSB_InitDeviceMode(MUSB_DEVICE_MODE_MSD);
            break;
            
        default:
            break;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBToggleVbus
 *  USB turn on/off Vbus.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBToggleVbus(INT32 i4Argc, const CHAR **szArgv)
{
    static INT32 fgVbus = FALSE;
    UINT32 u4VbusGpio;
    UINT32 u4VbusGpioPolarity;

    fgVbus = !fgVbus;
    
    // turn on usb vbus control.
    if ((0 == DRVCUST_InitQuery(eUSBVbus0Gpio, &u4VbusGpio)) &&
        (0 == DRVCUST_InitQuery(eUSBVbus0EnablePolarity, &u4VbusGpioPolarity)))
    {       
        if (GPIO_Output((INT32)u4VbusGpio, (INT32 *)&fgVbus) 
            != (INT32)fgVbus)
        {
          MGC_Printf("GPIO_Output fail.\n");
          return 0;        
        }

        MGC_Printf("Set Vbus Gpio%d=%d, Polarity=%d.\n", u4VbusGpio, fgVbus, u4VbusGpioPolarity);
    }    
    else
    {
        MGC_Printf("Vbus no driver customization pararmeter.\n");
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _MUSB_VbusOffTimer
 *  Use timer to stress test Vbus.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static void _MUSB_VbusOffTimer(HANDLE_T pt_tm_handle, void *pv_tag)
{
    VERIFY(OSR_OK == x_sema_unlock(phVbusSemaHdl));
}

//-------------------------------------------------------------------------
/** _MUSB_VbusThread
 *  Vbus test thread. 
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval   void
 */
//-------------------------------------------------------------------------
static VOID _MUSB_VbusThread(VOID *pvArgv)
{    
    UINT32 u4VbusGpio;
    UINT32 u4VbusGpioPolarity;
    UINT32 u4Time;

    UNUSED(pvArgv);

    while(1)
    {
        x_sema_lock(phVbusSemaHdl, X_SEMA_OPTION_WAIT);

        if ((0 == DRVCUST_InitQuery(eUSBVbus0Gpio, &u4VbusGpio)) &&
            (0 == DRVCUST_InitQuery(eUSBVbus0EnablePolarity, &u4VbusGpioPolarity)))
        {      
            if (fgVbusStatus)
            {
                u4VbusNewOffTime += u4VbusOffIncGapTime;
                if (u4VbusNewOffTime > 20*1000)
                {
                    u4VbusNewOffTime = u4VbusOffTime;
                }

                u4VbusLoopCount ++;

                if (u4VbusLoopCount > u4VbusTotalLoop)    
                {
                    MGC_Printf("[VBUS] %d times test end.\n", u4VbusLoopCount);
                    MGC_Printf("***************************************.\n");
                    /*
                    VERIFY(GPIO_Output((INT32)u4VbusGpio, (INT32 *)&u4VbusGpioPolarity) 
                        == (INT32)u4VbusGpioPolarity);                    
                    */    
                    continue;
                }

                u4Time = u4VbusNewOffTime;
            }
            else
            {
                u4VbusGpioPolarity = !u4VbusGpioPolarity;

                u4Time = u4VbusOnTime;                
            }
            
            VERIFY(GPIO_Output((INT32)u4VbusGpio, (INT32 *)&u4VbusGpioPolarity) 
                == (INT32)u4VbusGpioPolarity);

            MGC_Printf("***************************************.\n");
            MGC_Printf("[VBUS-%d] Set Vbus Gpio%d=%d, wait %d ms...\n\n", 
                u4VbusLoopCount, u4VbusGpio, u4VbusGpioPolarity, u4Time);

            // set timer to set vbus on.
            UNUSED(x_timer_start(phVbusTimerHdl, u4Time, 
                X_TIMER_FLAG_ONCE, _MUSB_VbusOffTimer, NULL));            
        }    
        else
        {
            MGC_Printf("[VBUS] Vbus no driver customization pararmeter.\n");
        }

        // toggle fgVbusStatus.
        fgVbusStatus = !fgVbusStatus;            
    }
}

//-------------------------------------------------------------------------
/** _MUSBVbusStress
 *  Use timer to stress test Vbus.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBVbusStress(INT32 i4Argc, const CHAR **szArgv)
{
    static BOOL fgInit = FALSE;
    UINT32 u4VbusGpio;
    UINT32 u4VbusGpioPolarity;
    
    if (i4Argc < 4)
    {
        MGC_Printf("Arg: [VBSOffTime, VBSOnTime, VBSOffIncGapTime, LoopCount].\n");
        return 0;
    }
    
    u4VbusOffTime = StrToInt(szArgv[1]);
    u4VbusOnTime = StrToInt(szArgv[2]);
    u4VbusOffIncGapTime = StrToInt(szArgv[3]);
    u4VbusTotalLoop = StrToInt(szArgv[4]);
    u4VbusLoopCount = 0;
    u4VbusNewOffTime = u4VbusOffTime;
    
    if (!fgInit)
    {
        UNUSED(x_timer_create(&phVbusTimerHdl));

        VERIFY(OSR_OK == x_sema_create(&phVbusSemaHdl, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

        /* back ground reading. */
        UNUSED(x_thread_create(&phVbusThreadHdl, "USBVbusTest",
                                8092,  50,
                                _MUSB_VbusThread, 0,
                                NULL));
        fgInit = TRUE;
    }

    UNUSED(x_timer_stop(phVbusTimerHdl));    
    
    // turn on usb vbus control.
    if ((0 == DRVCUST_InitQuery(eUSBVbus0Gpio, &u4VbusGpio)) &&
        (0 == DRVCUST_InitQuery(eUSBVbus0EnablePolarity, &u4VbusGpioPolarity)))
    {
        VERIFY(GPIO_Output((INT32)u4VbusGpio, (INT32 *)&u4VbusGpioPolarity) 
            == (INT32)u4VbusGpioPolarity);

        MGC_Printf("[VBUS-%d] Set Vbus Gpio%d=%d, wait %d ms...\n\n", 
            u4VbusLoopCount, u4VbusGpio, u4VbusGpioPolarity, u4VbusNewOffTime);

        fgVbusStatus = FALSE;
        UNUSED(x_timer_start(phVbusTimerHdl, u4VbusNewOffTime, 
            X_TIMER_FLAG_ONCE, _MUSB_VbusOffTimer, NULL));
    }    
    else
    {
        MGC_Printf("[VBUS] Vbus no driver customization pararmeter.\n");
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBSetSpeedCmd
 *  USB set host speed function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBSetSpeedCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Speed;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    uint16_t i = 0;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [high speed = 1, full speed = 0].\n");
        return 0;
    }

    ASSERT(szArgv);

    // operation mode. host or device.
    u4Speed = StrToInt(szArgv[1]);
    
    for (i=0; i<MUSB_CountPorts(); i++)
    {
        pUsbPort = MUSB_GetPort(i);
        ASSERT(pUsbPort);
        if (pUsbPort)
        {
            pPort = (MGC_Port *)pUsbPort->pPrivateData;
            ASSERT(pPort);
            if (pPort)
            {
                pPort->bWantHighSpeed = (u4Speed > 0);
                MGC_Printf("Set Port%d = %s.\n", i, ((u4Speed > 0) ? ("High Speed"): ("Full Speed")));
            }
        }
    }
    
    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBSetModeCmd
 *  USB set host or device mode function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBSLTCheckCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);

    UNUSED(szArgv);

    if (MUSB_CheckSLT() == 0)
    {
        MGC_Printf("USB SLT pass.\n");
    }
    else
    {
        MGC_Printf("USB SLT fail.\n");
    }

    return 1;
}

#ifndef __KERNEL__
//-------------------------------------------------------------------------
/** _MUSBHostMsdRWCmd
 *  USB CLI MSD host write/read disk speed test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBHostMsdRWCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 sector_start_offset = 0;
    UINT32 total_test_sector_num = 0;
    UINT32 lun = 0;
    UINT32 test_block_size = 0x10000;
    UINT64 test_total_size = 0;
    UINT32 total_disk_sector_num = 0;
    INT32 block_size = 0;
    UINT8 *SrcBuf;
    UINT8 *RxBuf;
    UINT32 *SrcBuf32bit;
    UINT32 *RxBuf32bit;
    UINT32 i;
    UINT64 u8Rate;
    UINT64 u8Position; /*test position %*/
    UINT32 read_test = 1;
    UINT32 write_test = 1;    
    UINT32 comp_test = 1;    
    UINT32 index = 0;    

    if (!_pvMusbMemPool)
    {
        _pvMusbMemPool = x_mem_alloc(0x40000);
        if ( _pvMusbMemPool == NULL )
        {
            LOG(0, "_MUSBHostMsdRWCmd allocate memory failed!\n");
            return 0;
        }
    }
    SrcBuf = (UINT8 *)_pvMusbMemPool;
    RxBuf = (UINT8 *)((UINT32)SrcBuf + 0x20000);
    
    if (i4Argc < 5)
    {
        MGC_Printf("Arg: [LUN], [W], [R], [C], [TestBlockSize].\n");
        return 0;
    }

    ASSERT(szArgv);
    lun = StrToInt(szArgv[1]);
    write_test = StrToInt(szArgv[2]);
    read_test = StrToInt(szArgv[3]);
    comp_test = StrToInt(szArgv[4]);
    test_block_size = StrToInt(szArgv[5]);

    MGC_Printf("LUN= 0x%X.\n", lun);

    //initial Src data.
#if 0    
    for (i = 0; i < (0x10000 + 64); i++)
    {
        // kill device data content.
        *(SrcBuf + i) = (option == 2) ? (0xFF) : (i & 0xFF);
        *(RxBuf + i) = 0;
    }
#else
    SrcBuf32bit = (UINT32 *)SrcBuf;
    RxBuf32bit = (UINT32 *)RxBuf;
    for (i = 0; i < ((0x20000 + 64)/4); i++)
    {
        // kill device data content.
        SrcBuf32bit[i] = i;
        RxBuf32bit[i] = 0;
    }
    SrcBuf = (UINT8 *)SrcBuf32bit;
    RxBuf = (UINT8 *)RxBuf32bit;   
#endif

    // check device LUN.    
    if (0 > MUSB_SetLun(lun))
    {
        return 0;
    }

    MGC_Printf("Total Lun = %d.\n", MUSB_GetTotalLun());

    // check device status.
    if (HFI_STATUS_READY != MUSB_GetStatus())
    {
        MGC_Printf("Device Status error !");    
        return 0;
    }

    // show device model name.
    MGC_Printf("Name: %s.\n", MUSB_GetModel());

    // check device block size.
    block_size = MUSB_GetBlockSize();
    if (block_size < 0)
    {
        MGC_Printf("block size error !");        
        return 0;
    }

    MGC_Printf("block_size = %d bytes.\n", block_size);

    if (MUSB_GetAccessType() != MUSB_HFI_ACCESS_RANDOM)
    {
        MGC_Printf("Device in Write Protected mode.\n");
        return 0;
    }

    MGC_Printf("Write dram addr = 0x%08X.\n", (UINT32)SrcBuf);
    MGC_Printf("Read dram addr = 0x%08X.\n", (UINT32)RxBuf);

    if (write_test)
    {
        /* 
         * check write performance.
         */   
        total_disk_sector_num = MUSB_GetSize();
        test_total_size = 0;
        sector_start_offset = 0;
        total_test_sector_num = 0;
        // clear total time stamp.
        MUSB_InitTimeMonitor();
        
        while (1)
        {
            test_total_size += test_block_size;
            total_test_sector_num += (test_block_size/block_size);
            if (0 > MUSB_Write(sector_start_offset*block_size, (UINT32)SrcBuf, test_block_size))
            {
                MGC_Printf("Write error !");
                return 0;
            }

            // update test size.
            sector_start_offset = (UINT32)(test_total_size/block_size);
                  
            // show disk speed information.
            if (total_test_sector_num >= (total_disk_sector_num /100))
            {
                u8Rate = 1000 * block_size;
                u8Rate = u8Rate * total_test_sector_num;
                u8Rate = u8Rate / MUSB_GetTimeMonitor();

                u8Position = test_total_size/block_size;
                u8Position = u8Position*100;
                u8Position = u8Position/total_disk_sector_num;
                MGC_Printf("%d%% : Write rate = %d KB/s.\n", (UINT32)u8Position, (UINT32)u8Rate);

                total_test_sector_num = 0;
                // clear total time stamp.
                MUSB_InitTimeMonitor();            
            }

            if (sector_start_offset >= total_disk_sector_num)
            {            
                return 0;                
            }            
        }
    }

    if (read_test)
    {
        /* 
         * check read performance.
         */   
        total_disk_sector_num = MUSB_GetSize();
        test_total_size = 0;
        sector_start_offset = 0;
        total_test_sector_num = 0;
        // clear total time stamp.
        MUSB_InitTimeMonitor();
        
        while (1)
        {
            test_total_size += test_block_size;
            total_test_sector_num += (test_block_size/block_size);
            if (0 > MUSB_Read(sector_start_offset*block_size, (UINT32)RxBuf, test_block_size))
            {
                MGC_Printf("Read error !");
                return 0;
            }

            // update test size.
            sector_start_offset = (UINT32)(test_total_size/block_size);
                  
            // show disk speed information.
            if (total_test_sector_num >= (total_disk_sector_num /100))
            {
                u8Rate = 1000 * block_size;
                u8Rate = u8Rate * total_test_sector_num;
                u8Rate = u8Rate / MUSB_GetTimeMonitor();

                u8Position = test_total_size/block_size;
                u8Position = u8Position*100;
                u8Position = u8Position/total_disk_sector_num;
                MGC_Printf("%d%% : Read rate = %d KB/s.\n", (UINT32)u8Position, (UINT32)u8Rate);
                
                total_test_sector_num = 0;
                // clear total time stamp.
                MUSB_InitTimeMonitor();            
            }

            if (sector_start_offset >= total_disk_sector_num)
            {            
                return 0;                
            }            
        }
    }

    if (comp_test)
    {
        /* 
         * check read/write compare.
         */   
        total_disk_sector_num = MUSB_GetSize();
        test_total_size = 0;
        sector_start_offset = 0;
        total_test_sector_num = 0;
        
        while (1)
        {
            index ++;
            MGC_Printf("%d: data test.\n", index);
            
            test_total_size += test_block_size;
            total_test_sector_num += (test_block_size/block_size);
            if (0 > MUSB_Write(sector_start_offset*block_size, (UINT32)SrcBuf, test_block_size))
            {
                MGC_Printf("Write error !");
                return 0;
            }

            if (0 > MUSB_Read(sector_start_offset*block_size, (UINT32)RxBuf, test_block_size))
            {
                MGC_Printf("Read error !");
                return 0;
            }

            for (i=0; i<test_block_size; i++)
            {
                if (SrcBuf[i] != RxBuf[i])
                {
                    MGC_Printf("[%d]: Err %d, %d.\n", i, SrcBuf[i], RxBuf[i]);
                    ASSERT(SrcBuf[i] == RxBuf[i]);
                    return 0;
                }
            }

            // update test size.
            sector_start_offset = (UINT32)(test_total_size/block_size);
                  
            // show disk speed information.
            if (total_test_sector_num >= (total_disk_sector_num /100))
            {

                u8Position = test_total_size/block_size;
                u8Position = u8Position*100;
                u8Position = u8Position/total_disk_sector_num;
                MGC_Printf("%d%% : data test ok.\n", (UINT32)u8Position);

                total_test_sector_num = 0;
            }

            if (sector_start_offset >= total_disk_sector_num)
            {            
                return 0;                
            }            
        }
    }
    
    return 0;
}

static void _MUSBHostMsdThread(void *pvArgv)
{

        UINT32 sector_start_offset = 0;
        UINT32 sector_offset = 0;
        UINT32 sector_num = 0;
        UINT32 readonly = 0;
        UINT32 option = 0;
        UINT32 stress = 0;
        UINT32 lun = 0;
        UINT32 test_size = 0x10000;
        UINT32 test_total_size = 0;
        INT32 block_size = 0;
        UINT8 *SrcBuf;
        UINT8 *RxBuf;
        UINT32 *SrcBuf32bit;
        UINT32 *RxBuf32bit;
        UINT32 offset = 64;
        UINT32 i;
        UINT64 u8WriteRate;
        UINT64 u8ReadRate;
        UINT32 templun = 0;
        MUSB_DEV_T     *pt_musb = NULL;

        ASSERT(pvArgv != NULL);
        pt_musb = *(MUSB_DEV_T **)pvArgv;
        
        if (!_pvMusbMemPool)
        {
            _pvMusbMemPool = x_mem_alloc(0x40000);
            if ( _pvMusbMemPool == NULL )
            {
                LOG(0, "_MUSBHostMsdCmd allocate memory failed!\n");
                return;
            }
        }
        SrcBuf = (UINT8 *)_pvMusbMemPool;
        RxBuf = (UINT8 *)((UINT32)SrcBuf + 0x20000);
    
        lun = pt_musb->lun;
        templun = lun;
        sector_start_offset = pt_musb->sector_start_offset;//StrToInt(szArgv[2]);
        sector_num = pt_musb->sector_num;//StrToInt(szArgv[3]);

        MGC_Printf("[MUSB]LUN= 0x%X.\n", lun);
        MGC_Printf("[MUSB]sector_start_offset = 0x%X.\n", sector_start_offset);
        MGC_Printf("[MUSB]sector_num = 0x%X.\n", sector_num);
    
        if (pt_musb->i4Argc == 5)
    {
            option = pt_musb->option;//StrToInt(szArgv[4]);
            MGC_Printf("[MUSB]Option = %d.\n", option);
            stress = (option != 2) ? 1: 0;
            readonly = (option == 1) ? 1: 0;
    }
    else
    {
            MGC_Printf("[MUSB]Write and read test.\n");
            stress = 0;
    }
    
        if ((sector_start_offset + 0x80) < sector_num)
        {
            sector_num -= 0x80;
        }
        else
        {
            return;
        }
    
        //initial Src data.
#if 0    
        for (i = 0; i < (0x10000 + 64); i++)
        {
            // kill device data content.
            *(SrcBuf + i) = (option == 2) ? (0xFF) : (i & 0xFF);
            *(RxBuf + i) = 0;
        }
#else
        SrcBuf32bit = (UINT32 *)SrcBuf;
        RxBuf32bit = (UINT32 *)RxBuf;
        for (i = 0; i < ((0x10000 + 64)/4); i++)
        {
            // kill device data content.
            SrcBuf32bit[i] = (option == 2) ? (0xFFFFFFFF) : (i);
            RxBuf32bit[i] = 0;
        }
        SrcBuf = (UINT8 *)SrcBuf32bit;
        RxBuf = (UINT8 *)RxBuf32bit;   
#endif
    
        // check device LUN.    
        if (0 > MUSB_SetLun(lun))
        {
            return;
        }
    
        MGC_Printf("[MUSB]Total Lun = %d.\n", MUSB_GetTotalLun());
    
        // check device status.
        if (HFI_STATUS_READY != MUSB_GetStatus())
        {
            MGC_Printf("\n[MUSB]Device Status error !");    
            return;
        }
    
        // show device model name.
        MGC_Printf("\n[MUSB]lun=%d, Name: %s.\n", lun, MUSB_GetModel());
    
        // check device block size.
        block_size = MUSB_GetBlockSize();
        if (block_size < 0)
        {
            MGC_Printf("\n[MUSB]block size error !");        
            return;
        }
    
        MGC_Printf("\n[MUSB]lun= %d, block_size = %d bytes.\n", lun, block_size);

if ((readonly) || (MUSB_GetAccessType() != MUSB_HFI_ACCESS_RANDOM))
{
    readonly = 1;
            MGC_Printf("\n[MUSB]lun=%d, Device in Write Protected mode.\n", lun);
}

sector_offset = sector_start_offset;

test_size = block_size;

// clear total time stamp.
MUSB_InitTimeMonitor();

        /* 
         * check write speed.
         */
        test_total_size = 0;
        test_size = block_size;
        if (!readonly)
        {
            while (1)
            {
                    test_total_size += test_size;                       
                    if (0 > MUSB_Write(sector_start_offset*block_size, (UINT32)SrcBuf, test_size))
                    {
                        MGC_Printf("\n[MUSB]lun = %d, Write error !", templun);
                        return;
                    }                  
                // update test size.
                test_size += block_size;
    
                if (test_size > 65536)
                {   
                #ifdef __linux__
                    {
                        u8WriteRate = test_total_size * 1000;
    //                    do_div(u8WriteRate, MUSB_GetTimeMonitor());
                    }
                #else
                        u8WriteRate = ((UINT64)(test_total_size * 1000))/(MUSB_GetTimeMonitor());
                #endif
                    MGC_Printf("\n[MUSB]lun = %d, Write rate = %d KB/s.\n", templun, (UINT32)u8WriteRate);
    
                    break;                
                }            
            }
        }     
    
        // clear total time stamp.
        MUSB_InitTimeMonitor();
        /* 
         * check read speed.
         */
        test_total_size = 0;
        test_size = block_size;
        while (1)
        {
            test_total_size += test_size;
    
            if (0 > MUSB_Read(sector_start_offset*block_size, (UINT32)RxBuf, test_size))
            {
                MGC_Printf("\n[MUSB]lun=%d, Read error !", templun);
                return;
            }        
               
            // update test size.
            test_size += block_size;
    
            if (test_size > 65536)
            {
            #ifdef __linux__
                {
                    u8ReadRate = test_total_size * 1000;
    //                do_div(u8ReadRate, MUSB_GetTimeMonitor());
                }
            #else
                    u8ReadRate = ((UINT64)(test_total_size * 1000))/(MUSB_GetTimeMonitor());
            #endif
                MGC_Printf("\n[MUSB]lun=%d, Read rate = %d KB/s.\n", templun,(UINT32)u8ReadRate);
    
                break;                
            }            
        }
        //initial Src data.
#if 0    
        for (i = 0; i < (0x10000 + 64); i++)
        {
            // kill device data content.
            *(SrcBuf + i) = (option == 2) ? (0xFF) : (i & 0xFF);
            *(RxBuf + i) = 0;
        }
#else
        SrcBuf32bit = (UINT32 *)SrcBuf;
        RxBuf32bit = (UINT32 *)RxBuf;
        for (i = 0; i < ((0x10000 + 64)/4); i++)
        {
            // kill device data content.
            SrcBuf32bit[i] = (option == 2) ? (0xFFFFFFFF) : (i);
            RxBuf32bit[i] = 0;
        }
        SrcBuf = (UINT8 *)SrcBuf32bit;
        RxBuf = (UINT8 *)RxBuf32bit;   
#endif
    
        test_size = block_size;
    
    
        // Read only
        if ( readonly )
        {
            UINT32 u4Counter = 0;
            MGC_Printf("[MUSB]Read Only Stress Test Start ... \n");
            MGC_Printf("[MUSB]Write one time, read/compare forever.\n");
            MGC_Printf("[MUSB]There will be one line log for every 100000 tries.\n");
            if (0 > MUSB_Write(sector_offset*block_size, (UINT32)(SrcBuf + offset), test_size))
            {
                u4WriteErrCount ++;
                MGC_Printf("[MUSB]lun=%d, Write error !.\n", templun);
                return;
            }
    
                x_memset((void *)(RxBuf + offset), 0, test_size);
    
            while(1)
            {
                u4Counter++;
                if ( u4Counter == 100000 )
                {
                    MGC_Printf("\n[MUSB]loop %d times.\n", u4Counter);
                    u4Counter = 0;
                }
                // Read the data from the same sector which has written already
                    if (0 > MUSB_Read(sector_offset*block_size, (UINT32)(RxBuf + offset), test_size))
                    {
                        u4ReadErrCount++;
                        MGC_Printf("\n[MUSB]lun=%d, Read error !\n", templun);
                        return;
                    }
                
                    // compare this data with what was written to find
                    // out if there is an error in reading 
                    {
                        for (i = 0; i < test_size; i++)
                        {
                            if (SrcBuf[i + offset] != RxBuf[i + offset])
                            {
                                MGC_Printf("\n[MUSB]error: test_size = 0x%X, i = 0x%X, src = 0x%X, rx = 0x%X.\n",
                                    test_size, i,
                                    SrcBuf[i + offset], RxBuf[i + offset]);
                                return;
                        }
                            }
                        }
            }
        }
    
        // R/W
        while (1)
        {        
                /*
                 * Write a blcok of date into a sector
                 */
                if (stress)
                {
                    MGC_Printf("\n[MUSB][%d] offset = 0x%X, test_size = 0x%X.\n", templun, sector_offset, test_size);
                }
            
                if (!readonly)
                {
                    if (0 > MUSB_Write(sector_offset*block_size, (UINT32)(SrcBuf + offset), test_size))
                    {
                        MGC_Printf("\n[MUSB]lun =%d, Write error !.\n", templun);
                        return;
                    }
                    
                    x_memset((void *)(RxBuf + offset), 0, test_size);
                }
    
                /*
                 * Read the data from the same sector which has written already
                 */
                if (0 > MUSB_Read(sector_offset*block_size, (UINT32)(RxBuf + offset), test_size))
                {
                    MGC_Printf("\n[MUSB]lun=%d, Read error !.\n", templun);
                    return;
                }
    
                /* 
                 * compare this data with what was written to find
                 * out if there is an error in reading 
                 */
                if (!readonly)
                {
                    for (i = 0; i < test_size; i++)
                    {
                        if (SrcBuf[i + offset] != RxBuf[i + offset])
                        {
                            MGC_Printf("\n[MUSB]lun=%d,error: test_size = 0x%X, i = 0x%X, src = 0x%X, rx = 0x%X.\n",
                                templun,test_size, i,
                                SrcBuf[i + offset], RxBuf[i + offset]);
                            return;
                    }
                        }
                    }
            sector_offset += (test_size /block_size);
    
            if (sector_offset >= sector_num)
            {
                sector_offset = sector_start_offset;
            }
    
            // update test size.
            test_size += block_size;
    
            if (test_size > 65536)
            {
                if (!stress)
                {
                    MGC_Printf("\n[MUSB]Write/Read data check ok !\n");
                    break;                
                }
                test_size = block_size;
            }
    
            offset++;
            if (offset > 64)
            {
                offset = 1;
            }
        }
    
        return;
    }

//-------------------------------------------------------------------------
/** _MUSBHostMsdCmd
 *  USB CLI MSD host test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBHostMsdCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 sector_start_offset = 0;
    UINT32 sector_num = 0;
    UINT32 lun = 0;
    UINT32 option = 0;
    MUSB_DEV_T     *pt_musb = NULL;
        
    if (i4Argc < 4)
    {
        MGC_Printf("[MUSB]Arg: [LUN] [StartSector] [NumSector] [Option: RW=0/RO=1/KILL=2].\n");
        return 0;
    }
    pt_musb = x_mem_calloc(1, sizeof(MUSB_DEV_T));
    if (pt_musb == NULL)
    {
        MGC_Printf("[MUSB]Memory allocate fail.\n");
        return 0;
    }
    u4ReadErrCount = 0;
    u4WriteErrCount = 0;

    lun = StrToInt(szArgv[1]);
    sector_start_offset = StrToInt(szArgv[2]);
    sector_num = StrToInt(szArgv[3]);
    option = StrToInt(szArgv[4]);
    
    MGC_Printf("[MUSB]LUN= 0x%X.\n", lun);
    MGC_Printf("[MUSB]sector_start_offset = 0x%X.\n", sector_start_offset);
    MGC_Printf("[MUSB]sector_num = 0x%X.\n", sector_num);

    pt_musb->lun = lun;   
    pt_musb->sector_start_offset = sector_start_offset;    
    pt_musb->sector_num = sector_num; 
    pt_musb->i4Argc = i4Argc;
    pt_musb->option = option;

    if(x_thread_create(&phUSBThreadHdl, "[MUSB]USBLoop times",
                                                      8092, 20, _MUSBHostMsdThread, sizeof(MUSB_DEV_T *), 
                                                      (VOID *)&pt_musb) != OSR_OK)
    {
        Printf("[MUSB]\nError: USB HM test thread fail\n");
        return 1;
    }


    return 0;

}
#endif

static INT32 _MUSBHostMsdResultCmd(INT32 i4Argc, const CHAR **szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);
    MGC_Printf("\nUSB Burn-in Result.\n");

    if(u4ReadErrCount)
    {
        MGC_Printf("\n[USB]Burn-in Read Device Fail Fail Fail Fail Count = %d.\n", u4ReadErrCount);
    }
    else if(u4WriteErrCount)
    {
        MGC_Printf("\n[USB]Burn-in Write Device Fail Fail Fail Fail Count = %d.\n", u4WriteErrCount);
    }
    else
    {
        MGC_Printf("\n[USB]Burn-in Result OK OK OK OK OK OK.\n");
    }
    return 1;
}


//-------------------------------------------------------------------------
/** _MUSBHostCompilanceCmd
 *  USB CLI MSD host compilance test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBHostCompilanceCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Case = 0;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [test case: 1 ~ 8].\n");
        MGC_Printf("**************************************\n");
        MGC_Printf("Case 1: SE0_NAK.\n");
        MGC_Printf("Case 2: TEST J.\n");
        MGC_Printf("Case 3: TEST K.\n");        
        MGC_Printf("Case 4: TEST PACKET.\n");        
        MGC_Printf("Case 5: FORCE ENABLE.\n");        
        MGC_Printf("Case 6: SUSPEND, WAIT 15 SECS, RESUME.\n");        
        MGC_Printf("Case 7: WAIT 15 SECS, PERFORM SETUP PHASE OF GET_DESC.\n");        
        MGC_Printf("Case 8: PERFORM SETUP PHASE OF GET_DESC, WAIT 15 SECS, PERFORM IN DATA PHASE.\n\n");
        MGC_Printf("**************************************\n\n");
        return 0;
    }

    ASSERT(szArgv);

    u4Case = StrToInt(szArgv[1]);
    if ((u4Case < 1) || (u4Case > 8))
    {
        MGC_Printf("test case out of boundary 1 ~ 8.\n");
        return 0;
    }

    u4Case += 0x0100;

    if (0 != MUSB_SetEhset(TRUE, u4Case))
    {
        MGC_Printf("MUSB_SetEhset fail.\n");
        return -1;
    }

    MGC_Printf("MUSB_SetEhset ok.\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBSuspendCmd
 *  USB CLI Suspend test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBSuspendCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    INT32 i4Ret;
  
    UNUSED(i4Argc);
    UNUSED(szArgv);
    i4Ret = MUSB_Suspend();

    UNUSED(i4Ret);           
    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBResumeCmd
 *  USB CLI Remote Wake up Device test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBResumeCmd(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Ret;
    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = MUSB_Resume();

    UNUSED(i4Ret);           

    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBPllStressTestCmd
 *  USB CLI Remote Wake up Device test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBPllStressTestCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4LoopTime=1000000;
    LOG(0, "Attention! All usb ports should be connected with a MSD.\n");
    UNUSED(i4Argc);
    UNUSED(szArgv);

    CLI_Parser("gpio.init");
    CLI_Parser("musb.i 1");
    x_thread_delay(15000);

    while(u4LoopTime)
    {
        u4LoopTime--;

        // only verified on MT5365
        // Change USB ECLK clock source to others (0xd25c[0:3])
        CLI_Parser("w 0xf000d25c 0x10004");

        // disable USB PLL (0x29404[0])
        CLI_Parser("w 0xf0029800 0x7300");
        CLI_Parser("w 0xf002e800 0x7300");
        CLI_Parser("w 0xf0029404 0x804202");
        CLI_Parser("w 0xf002e404 0x804202");
        x_thread_delay(10);
        
        // enable USB PLL (0x29404[0])
        CLI_Parser("w 0xf0029404 0x804203");
        CLI_Parser("w 0xf002e404 0x804203");
        CLI_Parser("w 0xf0029800 0x7000");
        CLI_Parser("w 0xf002e800 0x7000");

        // Change USB ECLK clock source to USB PLL (0xd25c[0:3])
        CLI_Parser("w 0xf000d25c 0x10001");

        // init usb
        CLI_Parser("musb.i 1");
        x_thread_delay(7000);

        // Do SLT test
        CLI_Parser("musb.slt");
    }
    
    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBUsbSwitchCmd
 *  USB CLI usb switch 1 port to 2 port test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
#if 0
static INT32 _MUSBUsbSwitchCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 u4ResetGpio = 4;
    UINT32 u4SelectGpio = 57;
    UINT32 u4SelectGpioValue = 0;    
    UINT32 u4GpioValue;
   
    if (i4Argc < 4)
    {
        MGC_Printf("USB switch: 1 port to 2 ports CLI.\n");
        MGC_Printf("Arg: [ResetGpio SelectGpio SelectGpioValue].\n");
        return 0;
    }

    ASSERT(szArgv);

    u4ResetGpio = StrToInt(szArgv[1]);
    u4SelectGpio = StrToInt(szArgv[2]);
    u4SelectGpioValue = StrToInt(szArgv[3]);
   
    if (u4SelectGpioValue) // usb card reader.
    {
        // reset = low.
        u4GpioValue = 0;
        VERIFY(GPIO_Output((INT32)u4ResetGpio, (INT32 *)&u4GpioValue) 
            == (INT32)u4GpioValue);

        // select = high.
        u4GpioValue = 1;
        VERIFY(GPIO_Output((INT32)u4SelectGpio, (INT32 *)&u4GpioValue) 
            == (INT32)u4GpioValue);

        // GL827 USB 2.0 Single Slot Card Reader Controller spec page 37, 6.6.8 Reset Timing.
        // Reset Deassertion to respond USB command ready = 72 ms.
        x_thread_delay(100);

        // reset = high.
        u4GpioValue = 1;
        VERIFY(GPIO_Output((INT32)u4ResetGpio, (INT32 *)&u4GpioValue) 
            == (INT32)u4GpioValue);            
    }
    else // usb port.
    {
        // reset = low.
        u4GpioValue = 0;
        VERIFY(GPIO_Output((INT32)u4ResetGpio, (INT32 *)&u4GpioValue) 
            == (INT32)u4GpioValue);

        // select = high.
        u4GpioValue = 0;
        VERIFY(GPIO_Output((INT32)u4SelectGpio, (INT32 *)&u4GpioValue) 
            == (INT32)u4GpioValue);

        // GL827 USB 2.0 Single Slot Card Reader Controller spec page 37, 6.6.8 Reset Timing.
        // Reset Deassertion to respond USB command ready = 72 ms.
        x_thread_delay(100);
        
        // reset = high.
        u4GpioValue = 1;
        VERIFY(GPIO_Output((INT32)u4ResetGpio, (INT32 *)&u4GpioValue) 
            == (INT32)u4GpioValue);            
    }
    
    return 0;
}
#endif
#ifdef MUSB_CDC
//-------------------------------------------------------------------------
/** _MUSBDevDMACmd
 *  USB CLI CDC Device DMA test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBDevDMACmd(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 test_size = 65536;
    UINT32 rxcount = 513;
    INT32 xfercount = 0;
    UINT32 txcount = 0;
    UINT8 *RxBuf;
    UINT32 offset = 0;

    if (!_pvMusbMemPool)
    {   
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    RxBuf = (UINT8 *)_pvMusbMemPool;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [start_count].\n");
        return 0;
    }

    ASSERT(szArgv);

    rxcount = StrToInt(szArgv[1]);

    while (1)
    {
        // check data by PC.
        MGC_Printf("\nRX = %d, offset = %d.\n", rxcount, offset);
        xfercount = MUSB_CdcSyncRx((UINT8 *)(RxBuf + offset), rxcount);
        if (rxcount != xfercount)
        {
            MGC_Printf("\nRead error count = %d, xfercount = %d.\n", rxcount, xfercount);
            return 0;
        }
        txcount = rxcount;
        /*        
        if (!(xfercount % maxpktsize))
        {
            txcount++;
        }
        */
        // check data by PC.
        //MGC_Printf("\nTX = %d, offset = %d.\n", txcount, offset);
        xfercount = MUSB_CdcSyncTx((UINT8 *)(RxBuf + offset), txcount);
        if (txcount != xfercount)
        {
            MGC_Printf("\nWrite error txcount = %d, xfercount = %d.\n", txcount, xfercount);
            return 0;
        }

        rxcount++;
        rxcount = (rxcount > test_size) ? 1 : rxcount;

        // set addres to round 0~16.
        offset++;
        if (offset > 32)
        {
            offset = 0;
        }
    }
}

static void _MUSB_CdcAsyncRxComplete(UINT32 dwActualLength)
{
    UINT8* pSrcBuffer;    
    UINT8* pDstBuffer;
    UINT32 i;
    static UINT32 offset = 0;
    UINT32 count = 0;
    INT32 xfercount;

    // check data by PC.
    MGC_Printf("\nRX = %d, offset = %d.\n", dwActualLength, offset);
       
    // compare data.
    pSrcBuffer = (UINT8*)_pu4MusbMemCmpPool;
    pDstBuffer = (UINT8*)_pu1MusbAsyncRxBuf;
    for (i=0; i<dwActualLength; i++)
    {
        if (pSrcBuffer[i] != pDstBuffer[i + offset])
        {
            MUSB_ASSERT(pSrcBuffer[i] == pDstBuffer[i + offset]);
            MGC_Printf("\nRx compare error.\n");
            return;
        }
    }

    // set addres to round 0~16.
    offset++;
    offset = (offset > 32) ? 1 : offset;

    pDstBuffer += offset;
    count = dwActualLength + 1;
    if (count > 0x20000)
    {
        count = 1;
    }
    
    xfercount = MUSB_CdcAsyncRx(pDstBuffer, count, _MUSB_CdcAsyncRxComplete);
    if (xfercount < 0)
    {
        MGC_Printf("\nAsync rx error count = %d, %d.\n", count, xfercount);
    }    
}
//-------------------------------------------------------------------------
/** _MUSBDevDMA2CHCmd
 *  USB CLI CDC Device 2 DMA channel test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBDevDMA2CHCmd(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 xfercount = 0;
    UINT32 count = 0;
    UINT32 test_size = 0x20000;
    UINT8 *RxBuf;
    UINT8 *TxBuf;
    UINT32 *TxInitBuf;
    UINT32 *RxInitBuf;
    UINT32 offset = 0;
    UINT32 i = 0;

    if (!_pvMusbMemPool)
    {   
        _pvMusbMemPool = x_mem_alloc(0x60000);
        ASSERT(_pvMusbMemPool);

        _pu4MusbMemCmpPool = x_mem_alloc(0x30000);
        ASSERT(_pu4MusbMemCmpPool);

        for (i = 0; i < 0x30000/4; i++)
        {
            _pu4MusbMemCmpPool[i] = i;
        }        
    }
    RxBuf = (UINT8 *)_pvMusbMemPool;
    TxBuf = RxBuf + 0x30000;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [start_count].\n");
        return 0;
    }

    ASSERT(szArgv);

    count = StrToInt(szArgv[1]);

    // check data 
    TxInitBuf = (UINT32 *)TxBuf;
    RxInitBuf = (UINT32 *)RxBuf;

    for (i = 0; i < test_size/4; i++)
    {
        RxInitBuf[i] = 0x5AC35391;
        //TxInitBuf[i] = i  | 0x50000000;
        TxInitBuf[i] = i;
    }

    _pu1MusbAsyncRxBuf = RxBuf;
    xfercount= MUSB_CdcAsyncRx(RxBuf, count, _MUSB_CdcAsyncRxComplete);
    if (xfercount < 0)
    {
        MGC_Printf("\nAsync rx error count = %d, %d.\n", count, xfercount);
        return 0;
    }
    
    while (1)
    {
        // check data by PC.
        MGC_Printf("\nTX = %d, offset = %d.\n", count, offset);

        xfercount = MUSB_CdcSyncTx((UINT8 *)(TxBuf + offset), count);
        if ((xfercount < 0) || (xfercount < count))
        {
            MGC_Printf("\nwrite error count = %d, xfercount = %d.\n", count, xfercount);
            return 0;
        }

        count++;
        count = (count > test_size) ? 1 : count;

        // set addres to round 0~16.
        offset++;
        offset = (offset > 32) ? 1 : offset;
    }
}
#endif /* #ifdef MUSB_CDC  */

#ifdef MUSB_ISO_EMULATION
//-------------------------------------------------------------------------
/** _USBHostISOTest
 *  USB CLI Isochronous host IN/OUT test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _USBHostISOTest(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 pktsize = 3072;
    UINT8 * src;
    UINT32 framenum = 1;
    UINT32 i;

    if (!_pvMusbMemPool)
    {   
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    src = (UINT8 *)_pvMusbMemPool;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [MaxPktSize].\n");
        return 0;
    }
    
    pktsize = StrToInt(szArgv[1]);
    
    // initialize buffer.
    for (i = 0; i < (pktsize * framenum); i ++)
    {
        *(src + i) = i & 0xFF;
    }

    // test data is balance in multi-packet.
    for (i = 1; i <= pktsize; i ++)
    {
        CLI_Parser_Arg("musb.his %d 7 1", i);
        // In fpga, cpu clock is slower, we need to wait device is ready to send.
        //x_thread_delay(10);
        CLI_Parser_Arg("musb.hig %d 7 1", i);
    }           

    MGC_Printf("Pass: %d packets are balance data between multi-packet.\n", pktsize);
    
    // test data is not balance in multi-packet.
    for (i = 1; i <= pktsize; i ++)
    {
        CLI_Parser_Arg("musb.his %d 7 0", i);
        // In fpga, cpu clock is slower, we need to wait device is ready to send.
        //x_thread_delay(10);
        CLI_Parser_Arg("musb.hig %d 7 0", i);
    }           

    MGC_Printf("Pass: %d packets are not balance data between multi-packet.\n", pktsize);
    
    return 0;
}
//-------------------------------------------------------------------------
/** _USBDevISOTest
 *  USB CLI Isochronous device IN/OUT test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _USBDevISOTest(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 ep = 2;
    UINT32 pktsize = 0;
    UINT8* buf;
    UINT32 framenum = 1;
    UINT32 loop = 0;
    UINT32 i;
    UINT32 balance;

    if (!_pvMusbMemPool)
    {
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    buf = (UINT8 *)_pvMusbMemPool;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [loop].\n");
        return 0;
    }
    
    loop = StrToInt(szArgv[1]);

    // test data is balance in multi-packet.
    balance = 1;
    MGC_Printf("balance = %d test start...\n", balance);

    for (i=0; i<loop; i++)
    {
        pktsize = MGC_Dev_GetISOPkt(ep, (UINT8 *)buf, framenum);

        MGC_Printf("pktsize = %d, buf = 0x%08X.\n", pktsize, (UINT32)buf);
        
        MGC_Dev_SendISOPkt(ep, pktsize, (UINT8 *)buf, framenum, balance);

        buf += pktsize;        
    }        

    // test data is not balance in multi-packet.
    balance = 0;
    MGC_Printf("balance = %d test start...\n", balance);
    
    for (i=0; i<loop; i++)
    {
        pktsize = MGC_Dev_GetISOPkt(ep, (UINT8 *)buf, framenum);

        MGC_Printf("pktsize = %d, buf = 0x%08X.\n", pktsize, (UINT32)buf);
        
        MGC_Dev_SendISOPkt(ep, pktsize, (UINT8 *)buf, framenum, balance);

        buf += pktsize;        
    }            
        
    return 0;
}
//-------------------------------------------------------------------------
/** _USBHostISOSendCmd
 *  USB CLI Isochronous host send packet command test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _USBHostISOSendCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 ep = 2;
    UINT32 pktsize = 1024;
    UINT32 * src;
    UINT32 framenum = 1;
    UINT32 interval;
    UINT32 balance;

    if (!_pvMusbMemPool)
    {
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    src = (UINT32 *)_pvMusbMemPool;

    if (i4Argc < 3)
    {
        MGC_Printf("Arg: [pktsize] [interval] [balance].\n");
        return 0;
    }

    pktsize = StrToInt(szArgv[1]);
    interval = StrToInt(szArgv[2]);
    balance = StrToInt(szArgv[3]);

    MGC_Host_SendISOPkt(ep, pktsize, (UINT8 *)src, framenum, balance, interval);

    MGC_Printf("ISO send %d bytes, interval = %d.\n", pktsize, interval);
    return 0;
}
//-------------------------------------------------------------------------
/** _USBHostISOGetCmd
 *  USB CLI Isochronous host get packet command test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _USBHostISOGetCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 ep = 2;
    UINT32 pktsize = 1024;
    UINT8* src;
    UINT8 * dst;
    UINT32 framenum = 1;
    UINT32 i;
    UINT32 interval;
    UINT32 balance;

    if (!_pvMusbMemPool)
    {
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    src = (UINT8 *)_pvMusbMemPool;
    dst = (UINT8 *)((UINT32)src + 0x20000);

    if (i4Argc < 3)
    {
        MGC_Printf("Arg: [pktsize] [interval] [balance].\n");
        return 0;
    }

    pktsize = StrToInt(szArgv[1]);
    interval = StrToInt(szArgv[2]);
    balance = StrToInt(szArgv[3]);

    // initialize buffer.
    for (i = 0; i < (pktsize * framenum); i ++)
    {
        *(dst + i) = 0;
    }

    MGC_Printf("ISO get %d bytes, interval = %d.\n", pktsize, interval);
    
    MGC_Host_GetISOPkt(ep, pktsize, (UINT8 *)dst, framenum, balance, interval);
    
    for (i = 0; i < (pktsize * framenum); i ++)
    {
        if (*(src + i) != *(dst + i))
        {
            MGC_Printf("Err: Src[%]=%d, Dst[%d]=%d.\n", i, *(src + i), i, *(dst + i));
            return 0;
        }
    }
        
    return 0;
}
#endif

#ifdef MUSB_QMU

static INT32 _MUSB_Qmu_HostTest(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 ep = 2;
    UINT32 pktsize = 1024;
    UINT32 * src;
    UINT32 framenum = 1;
    UINT32 interval;
    UINT32 balance;

    if (!_pvMusbMemPool)
    {   
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    src = (UINT32 *)_pvMusbMemPool;

    if (i4Argc < 3)
    {
        MGC_Printf("Arg: [pktsize] [interval] [balance].\n");
        return 0;
    }
    
    pktsize = StrToInt(szArgv[1]);
    interval = StrToInt(szArgv[2]);
    balance = StrToInt(szArgv[3]);

    MGC_Host_SendISOPkt(ep, pktsize, (UINT8 *)src, framenum, balance, interval);

    MGC_Printf("ISO send %d bytes, interval = %d.\n", pktsize, interval);



}

static INT32 _MUSB_Qmu_SendCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    UINT32 ep = 2;
    UINT32 pktsize = 1024;
    UINT32 * src;
    UINT32 framenum = 1;
    UINT32 interval;
    UINT32 balance;
    
    if (!_pvMusbMemPool)
    {
        _pvMusbMemPool = x_mem_alloc(0x40000);
        ASSERT(_pvMusbMemPool);
    }
    src = (UINT32 *)_pvMusbMemPool;

    if (i4Argc < 3)
    {
        MGC_Printf("Arg: [pktsize] [interval] [balance].\n");
        return 0;
    }           

    pktsize = StrToInt(szArgv[1]);
    interval = StrToInt(szArgv[2]);
    balance = StrToInt(szArgv[3]);

    MGC_QMU_Host_SendPkt(ep, pktsize, (UINT8 *)src, framenum, balance, interval);

    MGC_Printf("ISO send %d bytes, interval = %d.\n", pktsize, interval);
     return 0;
}


//-------------------------------------------------------------------------
/** _USBHostISOGetCmd
 *  USB CLI Isochronous host get packet command test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBQmuSendCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    uint32_t u4type;
    uint8_t ishost;
    uint8_t txend;
    uint32_t framenum;

    if (i4Argc < 3)
    {
        MGC_Printf("Arg: [tx endpoint_num] [framenum]\n"); //[transfer type] [TX/RX].\n");
        MGC_Printf("[tx endpoint_num] 1, 2,3, 4\n");
        MGC_Printf("[framenum]\n");
        return 0;
    }

    txend = StrToInt(szArgv[1]);
    u4type = 0x01; //StrToInt(szArgv[2]);
    ishost = 0x01; //StrToInt(szArgv[3]);
    framenum = StrToInt(szArgv[2]);
    
    MGC_QMU_Host_SendPkt(txend, u4type, framenum, ishost);    
       
    return 0;
}



//-------------------------------------------------------------------------
/** _USBHostISOGetCmd
 *  USB CLI Isochronous host get packet command test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBQmuGetCmd(INT32 i4Argc, const CHAR **szArgv)
{       
    uint32_t u4type;
    uint8_t ishost;
    uint8_t rxend;
    uint32_t framenum;

    if (i4Argc < 2)
    {
        MGC_Printf("Arg: [rx endpoint_num] [framenum]\n"); //[transfer type] [TX/RX].\n");
        MGC_Printf("[rx endpoint_num] 1, 2,3,4\n");
        //MGC_Printf("[transfer type] 1: ISO; 2: BULK\n");
        //MGC_Printf("[TX/RX] 0: RX; not 1: TX\n");
        MGC_Printf("[framenum number]\n");
        return 0;
    }

    rxend = StrToInt(szArgv[1]);
    u4type = 0x01; //StrToInt(szArgv[2]);
    ishost = 0;//StrToInt(szArgv[3]);
    framenum = StrToInt(szArgv[2]);
    MGC_QMU_Host_GetPkt(rxend, u4type, framenum, ishost);    
       
    return 0;
}


#endif

#ifdef MUSB_POWERDOWN
static INT32 _MUSBPowerDownCmd(INT32 i4Argc, const CHAR **szArgv)
{
    uint32_t u4Case;
    if(i4Argc < 2)
    {
        MGC_Printf("Cmd: [PowerDown Type].\n");
        MGC_Printf("Arg: [test case: 1 ~ 3].\n");
        MGC_Printf("**************************************\n");
        MGC_Printf("Case 1: Power Down and HotPlug Wakeup.\n");
        MGC_Printf("Case 2: Power Down and Software Wakeup .\n");
        MGC_Printf("Case 3: Power Down and Remote Wakeup.\n");        
        MGC_Printf("**************************************\n\n");
        return 0;
    }
    u4Case = StrToInt(szArgv[1]);
    if ((u4Case < 1) || (u4Case > 3))
    {
        MGC_Printf("test case out of boundary 1 ~ 8.\n");
        return 0;
    }    

    MUSB_PowerDownSet(u4Case);
	return 0;
}
#endif

//-------------------------------------------------------------------------
/** _MUSBHWPhyGet
 *  Query MUSB Phy finetune setting.
 *  @param  i4idx		The index of phy finetune setting
 *                      (according to _finetune structure)
 *  @retval  u4value 	the value of phy setting
 */
//-------------------------------------------------------------------------
static INT32 _MUSBHWPhyGet(INT32 i4idx)
{
    UINT32 u4value;
    UINT32 u4mask;

    ASSERT(i4idx < MUSB_ARRAY_SIZE(_arFinetune));
    u4mask = ( 1 << (_arFinetune[i4idx].msb-_arFinetune[i4idx].lsb+1) ) - 1;
    u4value = MGC_PHY_Read32(MUSB_P0_BASE, _arFinetune[i4idx].reg);
    u4value = (u4value >> _arFinetune[i4idx].lsb) & u4mask;
    return u4value;
}

//-------------------------------------------------------------------------
/** _MUSBHWPhySet
 *  Write MUSB Phy finetune setting.
 *  @param  i4idx		The index of phy finetune setting
 *                      (according to _arFinetune structure)
 *  @param  u4value		The value to be written to the phy setting
 *  @retval  u4regvalue	The written value of phy setting
 *                      (Input u4value will be truncated if it exceeds
 *                      the bits of setting)
 */
//-------------------------------------------------------------------------
static INT32 _MUSBHWPhySet(INT32 i4idx, UINT32 u4value)
{
    UINT32 u4mask;
    UINT32 u4reg;

    ASSERT(i4idx < MUSB_ARRAY_SIZE(_arFinetune));
    u4mask = ( 1 << (_arFinetune[i4idx].msb-_arFinetune[i4idx].lsb+1) ) - 1;
    u4value &= u4mask;
    u4reg = MGC_PHY_Read32(MUSB_P0_BASE, _arFinetune[i4idx].reg);
    u4reg = (u4reg & ~(u4mask<<_arFinetune[i4idx].lsb)) | (u4value<<_arFinetune[i4idx].lsb);
    MGC_PHY_Write32(MUSB_P0_BASE, _arFinetune[i4idx].reg, u4reg);
    return u4value;
}

//-------------------------------------------------------------------------
/** _MUSBPhyFinetune
 *  MUSB Hardware Phy finetune setting.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBPhyFinetune(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Val, u4Index, u4Help;
    u4Help = 1;
   
    if (i4Argc == 3)
    {
        u4Index = StrToInt(szArgv[1]);
        u4Val = StrToInt(szArgv[2]);
        if (u4Index >= MUSB_ARRAY_SIZE(_arFinetune)) {
            MGC_Printf("index is out of range\n");
        } else {
            u4Val = _MUSBHWPhySet(u4Index, u4Val);
            MGC_Printf("Set %s to %d\n", _arFinetune[u4Index].name, u4Val);
            u4Help = 0;
        }
    }
    if (u4Help) {
        MGC_Printf("Args: [index] [value]\n");
        MGC_Printf("index Bits Setting     Value\n");
        MGC_Printf("=========================\n");
        for (u4Index = 0; u4Index < MUSB_ARRAY_SIZE(_arFinetune); u4Index++)
        {
            MGC_Printf("%-3d [%2d] %-11s 0x%-9x-> %s\n", u4Index,
                _arFinetune[u4Index].msb - _arFinetune[u4Index].lsb + 1,
                _arFinetune[u4Index].name, _MUSBHWPhyGet(u4Index),
                _arFinetune[u4Index].descr ? _arFinetune[u4Index].descr : "");
        }
        return 0;
    }

    return 1;
}

#ifdef MUSB_AUD
//-------------------------------------------------------------------------
/** _MUSBAudTest
 *  USB CLI Audio host test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBAudTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Index;
    UINT32 u4Mode = 1;  /* 1: Play, 2: Rec*/
    UINT32 u4Ch;
    UINT32 u4Bits;
    UINT32 u4Freq;

    if (!MGC_aAudRecBuf)
    {
        MGC_aAudRecBuf = x_mem_alloc(MGC_AUD_TST_BUF_SIZE);
        ASSERT(MGC_aAudRecBuf);
    }

    if (i4Argc < 5)
    {
        MGC_Printf("Arg: [Index] [Mode] [Ch] [Bits] [Freq].\n");
        return 0;
    }

    u4Index = StrToInt(szArgv[1]);
    u4Mode = StrToInt(szArgv[2]);
    u4Ch = StrToInt(szArgv[3]);
    u4Bits = StrToInt(szArgv[4]);
    u4Freq = StrToInt(szArgv[5]);

    MGC_Printf("Index=%d, Mode=%d, Ch=%d, Bits=%d, Freq=%d.\n",
        u4Index, u4Mode, u4Ch, u4Bits, u4Freq);

    // test code.
    rAudPlayParm.wFormatTag = MUSB_AUD_PCM;
    rAudPlayParm.bNrChannels= u4Ch;
    rAudPlayParm.bBitResolution = u4Bits;
    rAudPlayParm.dwSamFreq = u4Freq;

    if (u4Mode == 1)  /* audio stream set */
    {
        rAudIsoPtr.bEmpty= FALSE;
    
        rAudStreamOutIrp.pBuffer = MGC_aAudRecBuf;
        rAudStreamOutIrp.dwBufSize = MGC_AUD_TST_BUF_SIZE;
        rAudStreamOutIrp.dwThreshold = 0;
        rAudStreamOutIrp.prAudIsoPtr = &rAudIsoPtr;
        rAudStreamOutIrp.pfIrpComplete = NULL;
        rAudStreamOutIrp.pCompleteParam = NULL;

        MGC_Printf("Audio Stream Set...\n");        
        MUSB_AudStreamSet(u4Index, &rAudPlayParm, &rAudStreamOutIrp);
    }
    else if (u4Mode == 2)  /* audio stream get */
    {
        rAudIsoPtr.dwRp = 0;
        rAudIsoPtr.dwWp = 0;
        rAudIsoPtr.dwWEp = 0;
        rAudIsoPtr.bEmpty= TRUE;

        rAudStreamInIrp.pBuffer = MGC_aAudRecBuf;
        rAudStreamInIrp.dwBufSize = MGC_AUD_TST_BUF_SIZE;
        rAudStreamInIrp.dwThreshold = 0;
        rAudStreamInIrp.prAudIsoPtr = &rAudIsoPtr;
        rAudStreamInIrp.pfIrpComplete = NULL;
        rAudStreamInIrp.pCompleteParam = NULL;

        MGC_Printf("Audio Stream Get...\n");        
        MUSB_AudStreamGet(u4Index, &rAudPlayParm, &rAudStreamInIrp);
    }
    else if (u4Mode == 3)  /* loop test */
    {
        rAudIsoPtr.dwRp = 0;
        rAudIsoPtr.dwWp = 0;
        rAudIsoPtr.dwWEp = 0;
        rAudIsoPtr.bEmpty= TRUE;

        rAudStreamInIrp.pBuffer = MGC_aAudRecBuf;
        rAudStreamInIrp.dwBufSize = MGC_AUD_TST_BUF_SIZE;
        rAudStreamInIrp.dwThreshold = 0;
        rAudStreamInIrp.prAudIsoPtr = &rAudIsoPtr;
        rAudStreamInIrp.pfIrpComplete = NULL;
        rAudStreamInIrp.pCompleteParam = NULL;

        MGC_Printf("Audio Stream Get...\n");        
        MUSB_AudStreamGet(u4Index, &rAudPlayParm, &rAudStreamInIrp);
        //x_thread_delay(100);

        rAudStreamOutIrp.pBuffer = MGC_aAudRecBuf;
        rAudStreamOutIrp.dwBufSize = MGC_AUD_TST_BUF_SIZE;
        rAudStreamOutIrp.dwThreshold = 0;
        rAudStreamOutIrp.prAudIsoPtr = &rAudIsoPtr;
        rAudStreamOutIrp.pfIrpComplete = NULL;
        rAudStreamOutIrp.pCompleteParam = NULL;

        MGC_Printf("Audio Stream Set...\n");        
        MUSB_AudStreamSet(u4Index, &rAudPlayParm, &rAudStreamOutIrp);        
    }        
    
    return 1;
}

//-------------------------------------------------------------------------
/** _MUSBAudVolumeTest
 *  USB CLI Audio host volume test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBAudVolumeTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Index;
    UINT8   u1IsPlay = 0; 
    UINT32 u4Volume;

    if (i4Argc < 3)
    {
        MGC_Printf("Arg: [Index] [u1IsPlay] [Volume].\n");
        return 0;
    }

    u4Index = StrToInt(szArgv[1]);
    u1IsPlay = StrToInt(szArgv[2]);
    u4Volume = StrToInt(szArgv[3]);

    MGC_Printf("Index=%d, IsPlay=%d, Volume=%d.\n", u4Index, u1IsPlay, u4Volume);

    if (0 != MUSB_AudVolumeSet(u4Index, (UINT8)u1IsPlay, (UINT16)u4Volume))
    {
        MGC_Printf("Error !");
    }    
    return 1;
}
#endif /* #ifdef MUSB_AUD */

#ifdef MUSB_HID
static void _MUSBHidJoyStickInsertNfy(uint8_t bJoystickId, uint8_t bInsert)
{
    MGC_Printf("Joystick-%d: %s.\n", bJoystickId, 
        ((bInsert) ? ("Inserted"): ("Removed")));
}

static void _MUSBHidJoyStickDataNfy(uint8_t bJoystickId, JOYSTICK_INFO_STRUCT *pJoyStick)
{
    MGC_Printf("Joystick-%d: X=%d, Y=%d, Button=0x%X.\n", 
        bJoystickId, pJoyStick->dwX, pJoyStick->dwY, pJoyStick->dwButton);
}
//-------------------------------------------------------------------------
/** _MUSBHidTest
 *  USB CLI Hid host test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBHidTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4DeviceId;
    UINT32 u4PortId;
    UINT32 u4DeviceNum;
    UINT32 u4PortNum;
    UINT32 u4Status;
    INT32 i4Return;
    JOYSTICK_INFO_STRUCT rJoyStickData;

    // Set function test.
    i4Return = MUSB_HidIoctl(0, MUSB_HID_IOCTL_SET_JOYSTICK_INSERT_NOTIFY, 
        0, (void *)_MUSBHidJoyStickInsertNfy);
    if (i4Return < 0)
    {
        MGC_Printf("Hid call error = %d.\n", i4Return);    
        return;
    }        

    i4Return = MUSB_HidIoctl(0, MUSB_HID_IOCTL_SET_JOYSTICK_DATA_NOTIFY, 
        0, (void *)_MUSBHidJoyStickDataNfy);
    if (i4Return < 0)
    {
        MGC_Printf("Hid call error = %d.\n", i4Return);    
        return;
    }        

    // Get function test.    
    i4Return = MUSB_HidIoctl(0, MUSB_HID_IOCTL_GET_DEVICE_NUM, 0, &u4DeviceNum);
    if (i4Return < 0)
    {
        MGC_Printf("Hid call error = %d.\n", i4Return);    
        return;
    }
    MGC_Printf("Hid device number = %d.\n", u4DeviceNum);

    for (u4DeviceId=0; u4DeviceId<u4DeviceNum; u4DeviceId++)
    {
        u4Status = 0;
        i4Return = MUSB_HidIoctl(u4DeviceId, MUSB_HID_IOCTL_GET_DEVICE_STATUS, 0, &u4Status);
        if (i4Return < 0)
        {
            MGC_Printf("Hid call error = %d.\n", i4Return);    
            return;
        }        

        u4PortNum = 0;
        i4Return = MUSB_HidIoctl(u4DeviceId, MUSB_HID_IOCTL_GET_DEVICE_PORT, 0, &u4PortNum);
        if (i4Return < 0)
        {
            MGC_Printf("Hid call error = %d.\n", i4Return);    
            return;
        }        

        MGC_Printf("Hid device-%d, Joystick num = %d, Status=%s.\n",
            u4DeviceId, u4PortNum, ((u4Status) ? ("Yes"): ("No")));        
    }

    // Get function test.    
    i4Return = MUSB_HidIoctl(0, MUSB_HID_IOCTL_GET_JOYSTICK_NUM, 0, &u4DeviceNum);
    if (i4Return < 0)
    {
        MGC_Printf("Hid call error = %d.\n", i4Return);    
        return;
    }
    MGC_Printf("Hid Joystick number = %d.\n", u4DeviceNum);

    for (u4PortId=0; u4PortId<u4DeviceNum; u4PortId++)
    {
        i4Return = MUSB_HidIoctl(u4PortId, MUSB_HID_IOCTL_GET_JOYSTICK_DATA, 
            0, &rJoyStickData);
        if (i4Return < 0)
        {
            MGC_Printf("Hid call error = %d.\n", i4Return);    
            return;
        }        
        
        MGC_Printf("Hid Joystick-%d: X=%d, Y=%d, Button=%d.\n",
            u4PortId, rJoyStickData.dwX, rJoyStickData.dwY, rJoyStickData.dwButton);            
    }            
}
#endif /* #ifdef MUSB_HID */

#if 0//def MUSB_QMU
static INT32 _MUSBTestQMUBulkLoopback(INT32 i4Argc, const CHAR **szArgv)
{
    return TestQMUBulkLoopback(i4Argc, szArgv);
}

static INT32 MUSB_QmuInit(INT32 i4Argc, const CHAR **szArgv)
{
    return _MUSB_QmuInit(i4Argc, szArgv);
}
#endif
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
/* Declare the debug on/off/level and RegTest functions */

LINT_EXT_HEADER_BEGIN 

CLIMOD_DEBUG_FUNCTIONS(MUSB)

/*CLIMOD_REGTST_FUNCTIONS(MUSB)*/
/* End of Declare */

static CLI_EXEC_T _arMUSBCmdTbl [] =
{
#ifndef __KERNEL__
    /*CLIMOD_REGTST_CLIENTRY(MUSB),*/
    { "debug_on", "d_on", _MUSBCLI_DebugOn, NULL, "MUSB.d_on", CLI_GUEST },
    { "debug_off", "d_off", _MUSBCLI_DebugOff, NULL, "MUSB.d_off", CLI_GUEST },
    { "debug_level", "d_l", _MUSBCLI_DebugLevel, NULL, "MUSB.d_l", CLI_GUEST },
#endif
    /*system init*/
    { "init", "i", _MUSBSetModeCmd, NULL, "MUSB init", CLI_GUEST },
#ifndef __KERNEL__
    /*Set vbus*/
    { "vbus", "v", _MUSBToggleVbus, NULL, "MUSB Vbus", CLI_GUEST },
    { "vbusst", "vst", _MUSBVbusStress, NULL, "MUSB Vbus stress test", CLI_GUEST },

    /*system setupt*/
    { "speed", "speed", _MUSBSetSpeedCmd, NULL, "MUSB speed", CLI_GUEST },

    /*SLT check*/
    { "slt", "slt", _MUSBSLTCheckCmd, NULL, "MUSB slt", CLI_GUEST },

    /*host mode test cli*/
    { "hmsd", "hw", _MUSBHostMsdRWCmd, NULL, "MUSB host msd rw speed test", CLI_GUEST },
    { "hmsd", "hm", _MUSBHostMsdCmd, NULL, "MUSB host msd test", CLI_GUEST },
    { "hmresult", "hr", _MUSBHostMsdResultCmd, NULL, "MUSB host msd test result", CLI_GUEST},
    /*host compilance test*/
    { "htst", "ht", _MUSBHostCompilanceCmd, NULL, "MUSB host compilance test", CLI_GUEST },
    { "suspend", "s", _MUSBSuspendCmd, NULL, "MUSB bus suspend", CLI_GUEST },       
    { "resume", "r", _MUSBResumeCmd, NULL, "MUSB bus resume", CLI_GUEST },

    /* usb pll stress test*/
    { "usbpllst", "upst", _MUSBPllStressTestCmd, NULL, "MUSB USBPLL stress test", CLI_GUEST },

    /*USB switch test*/
    //{ "switch", "sw", _MUSBUsbSwitchCmd, NULL, "MUSB switch test", CLI_GUEST },       

    /*USB register tunning test*/
    { "phy", "phy", _MUSBPhyFinetune, NULL, "MUSB phy finetune setting", CLI_SUPERVISOR },
#endif
#ifdef MUSB_AUD
    /*Audio class test clii*/
    { "aud", "a", _MUSBAudTest, NULL, "MUSB Audio test", CLI_GUEST },
    { "vol", "vol", _MUSBAudVolumeTest, NULL, "MUSB Audio Volume test", CLI_GUEST },
#endif /* #ifdef MUSB_AUD */

#ifdef MUSB_HID
    /*HID class test clii*/
    { "hid", "hid", _MUSBHidTest, NULL, "MUSB Hid test", CLI_GUEST },
#endif /* #ifdef MUSB_HID */

/*device mode test cli*/
#ifdef MUSB_CDC
    { "dma", "d", _MUSBDevDMACmd, NULL, "MUSB dma test, 1 ~ 65536", CLI_GUEST },
    { "dma2", "d2", _MUSBDevDMA2CHCmd, NULL, "MUSB dma 2 ch test, 1 ~ 65536", CLI_GUEST },
#endif /* #ifdef MUSB_CDC  */

/*Isochroous mode test cli*/
#ifdef MUSB_ISO_EMULATION
    { "hiso", "hiso", _USBHostISOTest, NULL, "USB host isochronous IN/OUT packet", CLI_GUEST },   
    { "diso", "diso", _USBDevISOTest, NULL, "USB device isochronous IN/OUT packet test", CLI_GUEST },      
    { "his", "his", _USBHostISOSendCmd, NULL, "USB host isochronous send packet", CLI_GUEST },   
    { "hig", "hig", _USBHostISOGetCmd, NULL, "USB host isochronous get packet test", CLI_GUEST },  
#endif 
#ifdef MUSB_QMU
    {"qmus", "qmus", _MUSBQmuSendCmd, NULL, "USB QMU host send packet tst", CLI_GUEST},
    {"qmug", "qmug", _MUSBQmuGetCmd, NULL, "USB QMU host get packet test", CLI_GUEST},
//    {"qmui", "qmui", MUSB_QmuInit, NULL, "USB QMU Init", CLI_GUEST},

      //  { "qdebugLevel", "qdl", _MUSBTestQDebugLevel, NULL, "QMU Set Debug Level", CLI_GUEST }, 
       // { "qbulklb", "qbl", _MUSBTestQMUBulkLoopback, NULL, "QMU random data loopback test using bulk transfer", CLI_GUEST }, 
#endif
#ifdef MUSB_POWERDOWN
    {"powerdown", "pd", _MUSBPowerDownCmd, NULL, "USB Power Down and Hot Plug Test", CLI_GUEST},
#endif 
    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

#else

static CLI_EXEC_T _arMUSBCmdTbl[] =
{
    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

#endif


CLI_MAIN_COMMAND_ITEM(rMUSB)
{
    "musb", NULL, NULL, _arMUSBCmdTbl, "MUSB command", CLI_GUEST
};

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
LINT_EXT_HEADER_END

