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
 * $RCSfile: pcmcia_if.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
 
 //-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#include "x_hal_926.h"
LINT_EXT_HEADER_END

#include "pcmcia_if.h"
#include "pcmcia_ctrl.h"
#include "pcmcia_hw.h"
#include "pcmcia_isr.h"
#include "pcmcia_util.h"
#include "pcmcia_slt.h"

#include "CableCard/ci_if.h"
#include "CableCard/pod_if.h"
#include "CableCard/mpod_if.h"
#include "CableCard/podci_ctrl.h"
#include "CableCard/podci_cis.h"

#include "SPI/spi_if.h"

#include "x_tuner.h"
#include "tuner_if.h"
#include "dmx_if.h"

#ifdef PCMCIA_ENABLED

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* For performance test */
//#define PCMCIA_MEASURE_TIME


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Work Around
/* Used only for driver build. Add an delay after resetting data channel */
#define PCMCIA_WORK_AROUND_DRIVER_BUILD_FOR_CAM026_CAM023


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Exported variables
//-----------------------------------------------------------------------------
#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI)
    PCMCIA_CARD_TYPE ePcmciaCardType = PCMCIA_CARD_TYPE_CI;
#else
    PCMCIA_CARD_TYPE ePcmciaCardType = PCMCIA_CARD_TYPE_POD;
#endif

#if defined(CC_INTERNAL_CI) || defined(CC_INTERNAL_POD)
    PCMCIA_CHIP_TYPE ePcmciaChipType = PCMCIA_CHIP_TYPE_INTERNAL;
#else
    PCMCIA_CHIP_TYPE ePcmciaChipType = PCMCIA_CHIP_TYPE_EXTERNAL;
#endif

PCMCIA_EXTERNAL_IC_VERSION eExternalIcVersion = MT8295_VERSION_NONE;
BOOL        fgPcmciaIsInitedByCli = FALSE;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL fgPcmciaTsEnabled = FALSE;
static PCMCIA_TS_TYPE u4TsType = PCMCIA_TS_TYPE_DVB_T;
static HANDLE_T _hAutoRouteSema = (HANDLE_T)NULL;
TS_FMT_T _rTsFmt;
static BOOL fgIsExternalDemod = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// This is a dirty work around solution. 
// The TS should not be routed by pcmcia driver. 
// It's better to set TS routing path by middleware.
// (Don't put this function into pcmcia_ctrl.c.)
//-----------------------------------------------------------------------------

static void _PCMCIA_AutoSetTsRoutingPath_5396(void)
{
    // Route TS based on the following attributes : 
    //      *. CamConnectiviey, 
    //      *. DVB-C or DVB-T, 
    //      *. External/Internal CI, 
    //      *. Enable/Disable TS set by middleware.
    // The following parameters should be set to route TS : 
    //      P1. Demod : internal/external, parallel/serial
    //      P2. PCMCIA : parallel/serial, bypass
    //      P3. DMX : Set front end.
    //BOOL fgCamConnected = FALSE;
    BOOL fgTSGoThroughCam = FALSE;
    UNUSED(_PCMCIA_AutoSetTsRoutingPath_5396);

    fgTSGoThroughCam = fgPcmciaTsEnabled;
    if (_rTsFmt.fgExternalDemod == BOOL_TRUE)
    {
        fgIsExternalDemod = TRUE;
    }
    else
    {
        fgIsExternalDemod = FALSE;
    }

    PCMCIA_SetTsPath(fgIsExternalDemod, fgTSGoThroughCam);

}

static void _PCMCIA_AutoSetTsRoutingPath(void)
{
    if ( _hAutoRouteSema == (HANDLE_T)NULL )
    {
        VERIFY(OSR_OK == x_sema_create(&_hAutoRouteSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) );
    }
    VERIFY(OSR_OK == x_sema_lock(_hAutoRouteSema, X_SEMA_OPTION_WAIT));

    _PCMCIA_AutoSetTsRoutingPath_5396();

    VERIFY(OSR_OK == x_sema_unlock(_hAutoRouteSema));
}


void PCMCIA_SetTsPath(BOOL fgExternalDemod, BOOL fgThroughCard)
{
    // Route TS based on the following attributes : 
    //      *. CamConnectivity, 
    //      *. External/Internal Demod, 
    //      *. External/Internal CI, 
    //      *. Enable/Disable TS set by middleware.
    // The following parameters should be set to route TS : 
    //      P1. Demod : internal/external, parallel/serial
    //      P2. PCMCIA : parallel/serial, bypass
    //      P3. DMX : Set front end.
    BOOL fgCamConnected = FALSE;
    BOOL fgIsInternalCi = TRUE;

    fgCamConnected   = PCMCIA_DetectCableCard();
    fgIsInternalCi   = ((ePcmciaChipType==PCMCIA_CHIP_TYPE_INTERNAL)?TRUE:FALSE);

    LOG(1, "PCMCIA_SetTsPath: fgExternalDemod=%d, fgThroughCard=%d, fgCamConnected=%d\n", 
            fgExternalDemod, fgThroughCard, fgCamConnected);

    if ( fgIsInternalCi )   // Internal CI
    {
        if ( fgExternalDemod )
        {
            // Todo:Set External Demod pinmux
            if ( fgThroughCard && fgCamConnected )   // CAM Connected
            {
                // P1. External Demod 
                PCMCIA_EnableTS(FALSE);
                PCMCIA_SetInputTs(PCMCIA_TS_EXTERNAL_DEMOD_P);
                PCMCIA_SetParallelTsOnOff(TRUE);
                PCMCIA_EnableTS(TRUE);

                // P3. DMX : Set front end -------------------------------------
                LOG(1, "    Set d.sfe %d\n", DMX_FE_CI);
                DMX_SetFrontEnd(DMX_FE_CI);
            }
            else    // CAM not Connected
            {
                PCMCIA_EnableTS(FALSE);
                
                // P3. DMX : Set front end -------------------------------------
                LOG(1, "    Set d.sfe %d\n", DMX_FE_EXT_P);
                DMX_SetFrontEnd(DMX_FE_EXT_P);
            }
        }
        else // Internal demod
        {
            // Todo: set internal demod pinmux. 
            if ( fgThroughCard && fgCamConnected )   // CAM Connected
            {
                // P1. Internal Demod 
                PCMCIA_EnableTS(FALSE);
                PCMCIA_SetInputTs(PCMCIA_TS_INTERNAL_DEMOD_P);
                PCMCIA_SetParallelTsOnOff(TRUE);
                PCMCIA_EnableTS(TRUE);

                // P3. DMX : Set front end -------------------------------------
                LOG(0, "    Set d.sfe %d\n", DMX_FE_CI);
                DMX_SetFrontEnd(DMX_FE_CI);
            }
            else    // CAM not Connected
            {
                PCMCIA_EnableTS(FALSE);

                // P3. DMX : Set front end -------------------------------------
                LOG(1, "    Set d.sfe %d\n", DMX_FE_INTERNAL);
                DMX_SetFrontEnd(DMX_FE_INTERNAL);
            }
        }
    }
    else    // External CI
    {
        if ( fgExternalDemod )
        {
            if (_rTsFmt.SerOrPar == SP_PARALLEL)
            {
                PCMCIA_SetParallelTsOnOff(TRUE);
                LOG(1, "Parallel TS input\n");
            }
            else
            {
                PCMCIA_SetParallelTsOnOff(FALSE);
                LOG(1, "Serial TS input\n");
            }
            if ( fgThroughCard && fgCamConnected )   // CAM Connected
            {
                PCMCIA_EnableTS(TRUE);
            }
            else    // CAM not Connected
            {
                PCMCIA_EnableTS(FALSE);
            }
            // P3. DMX : Set front end -------------------------------------
            LOG(1, "    Set d.sfe %d 0\n", DMX_FE_EXT_S);
            DMX_SetFrontEndEx(0, DMX_FE_EXT_S);
        }
        else // Internal demod
        {
            LOG(0, "Internal demod and external ci ???\n");
        }
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** PCMCIA_Init **/
//-----------------------------------------------------------------------------
INT32 PCMCIA_Init(void)
{
    INT32 i4Result;

    i4Result = PCMCIA_Initialize();
    LOG(0, "PCMCIA_Init : %d\n", i4Result);
#ifdef CC_USE_DDI
    if (eExternalIcVersion == MT8295_VERSION_BNE)
    {
        //FIXME, it is customization....
//        _rTsFmt.SerOrPar = SP_PARALLEL;
//        PCMCIA_UpdateTsStatus();
    }
#endif
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_LowLevelInit (Just for CLI)
 *      Only invoked by pcmcia_cmd.c. This API will not only initialize 8295,
 *      but does personality change & nego buffer size.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_LowLevelInit(void)
{
    INT32 i4Result;
    
    i4Result = PODCI_LowLevelInit();
    LOG(3, "PCMCIA_LowLevelInit : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_RawRead32 (Just for CLI)
 *  Read 8295 register.
 */
//-----------------------------------------------------------------------------
UINT32 PCMCIA_RawRead32(UINT32 offset)
{
    UINT32 u4Result;

    u4Result = PCMCIAHW_READ32(offset);
    LOG(9, "PCMCIA_RawRead32(%d) : %d\n", offset, u4Result);    
    return u4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_RawWrite32 (Just for CLI)
 *  Write 8295 register.
 */
//-----------------------------------------------------------------------------
UINT32 PCMCIA_RawWrite32(UINT32 offset, UINT32 value)
{
    UINT32 u4Result;

    u4Result = (UINT32)PCMCIAHW_WRITE32(offset, value);
    LOG(9, "PCMCIA_RawWrite32(%d) : %d\n", offset, u4Result);
    return u4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_Read (Not used)
 *      This API reads a number of bytes from CAM attribute memory. (address 0 for data)
 *      Not used.
 *      If we want to remove this API, pcmcia_mw.c & io_mtci.c should be modified.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_Read(POD_IO_STATE_T eIoState, UINT8* pu1Data, UINT16* pu2DataLen)
{
    INT32 i4Result = 0 ;
    ASSERT((pu1Data != NULL) && (pu2DataLen != NULL));
	
    i4Result = PODCI_ReadCard(eIoState, pu1Data, pu2DataLen);
    LOG(3, "PCMCIA_Read : %d\n", i4Result);
    
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_Write (Not used)
 *      This API writes a number of bytes to CAM attribute memory. (address 0 for data)
 *      Not used.
 *      If we want to remove this API, pcmcia_mw.c & io_mtci.c should be modified.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_Write(POD_IO_STATE_T eIoState, const UINT8* pu1Data, UINT16 u2DataLen)
{
    INT32 i4Result = 0;
    ASSERT(pu1Data != NULL);
	  i4Result = PODCI_WriteCard(eIoState,  pu1Data, u2DataLen);
//    i4Result = PODCI_WriteCardReg(eIoState, 0, pu1Data, u2DataLen);
    //ASSERT(0);
    LOG(3, "PCMCIA_Write : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ReadReg
 *      This API reads a number of bytes from CAM attribute memory.
 *      address 0 for data, address 1 for command/status.
 *      (Doesn't handle control/status/size(LS/MS) register.)
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_ReadReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr,
                     UINT8* pu1Data, UINT16 u2DataLen)
{
    INT32 i4Result;
    ASSERT((pu1Data != (UINT8*)NULL));

    if ( u2DataLen > 128 )
    {
        eIoState = POD_DMA;
    }

    i4Result = PODCI_ReadCardReg(eIoState, u4RegAddr, pu1Data, u2DataLen);
    LOG(7, "PCMCIA_ReadReg(%d) : %d\n", u4RegAddr, i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_WriteReg
 *      This API writes a number of bytes to CAM common memory.
 *      address 0 for data, address 1 for command/status.
 *      (Doesn't handle control/status/size(LS/MS) register.)
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_WriteReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr,
                      const UINT8* pu1Data, UINT16 u2DataLen)
{
    INT32 i4Result;
    ASSERT(pu1Data != NULL);

    i4Result = PODCI_WriteCardReg(eIoState, u4RegAddr, pu1Data, u2DataLen);
    LOG(7, "PCMCIA_WriteReg(%d) : %d\n", u4RegAddr, i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ReadCor
 *      This API reads a value to CAM attribute memory.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_ReadCor(UINT16 u2CorAddress)
{
    INT32 i4Result;

    i4Result = PODCI_ReadCor(u2CorAddress);
    LOG(8, "PCMCIA_ReadCor : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_WriteCor
 *      This API writes a value parsed from CIS to CAM attribute memory.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_WriteCor(UINT16 u2CorAddress, UINT8 u1CorValue)
{
    INT32 i4Result;

    /* Disable BVD1 and BVD2 interrupts since the pins are used as TS data pins after personality change */
    PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & ~PC_BVD1_IE & ~PC_BVD2_IE);
    //PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)PODCI);
    PCMCIA_PcmciaToPod(TRUE);
    i4Result = PODCI_WriteCor(u2CorAddress, u1CorValue);
    LOG(3, "PCMCIA_WriteCor : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_CamPwrControl (Just for CLI)
 *      ManualControl  : CI 5V is controlled by CLI.
 *      !ManualControl : CI 5V is controlled by PCMCIA_DetectCard()
 */
//-----------------------------------------------------------------------------
void PCMCIA_CamPwrControl(BOOL bManualControl)
{
    PCMCIA_CamPwrCtrl(bManualControl);
    LOG(3, "PCMCIA_CamPwrControl : %d\n", bManualControl);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_CamPwrOnOff (Just for CLI)
 *      ManualControl  : CI 5V is controlled by CLI.
 *      !ManualControl : CI 5V is controlled by PCMCIA_DetectCard()
 */
//-----------------------------------------------------------------------------
void PCMCIA_CamPwrOnOff(BOOL bPwrOn)
{
    PCMCIA_CamPwrONOFF(bPwrOn);
    LOG(3, "PCMCIA_CamPwrOnOff : %d\n", bPwrOn);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_Reset
 *      Reset CAM.
 *  @param b_set, TRUE: Pull high reset pin, FALSE: Pull low reset pin.
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_Reset(BOOL b_set)
{
    INT32 i4Result;
    
    i4Result = PCMCIA_ResetCableCardOnOff(b_set);
    LOG(3, "PCMCIA_Reset(%d) : %d\n", b_set, i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ResetCam
 *      Reset CAM
 */
//-----------------------------------------------------------------------------
void PCMCIA_ResetCam(void)         //reset pod/ci card
{
    PCMCIA_ResetCableCard();
    LOG(3, "PCMCIA_ResetCam\n");
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ResetPcmciaHw (Just for CLI)
 *      Reset CAM
 */
//-----------------------------------------------------------------------------
void PCMCIA_ResetPcmciaHw(void)
{
    PCMCIA_ResetPcmciaHW();
    LOG(3, "PCMCIA_ResetPcmciaHw Done\n");
    return;    
}

//-----------------------------------------------------------------------------
/** PCMCIA_ThreadNoAssert (Just for CLI)
 *      bNoAssert = TRUE  : Pcmcia monitor thread will not assert
 *      bNoAssert = FALSE : Pcmcia monitor thread will assert (default setting)
 */
//-----------------------------------------------------------------------------
void PCMCIA_ThreadNoAssert(BOOL bNoAssert)
{
    PCMCIA_ThdNoAsrt(bNoAssert);
    LOG(3, "PCMCIA_ThreadNoAssert : %d\n", bNoAssert);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_StopThread (Just for CLI)
 */
//-----------------------------------------------------------------------------
void PCMCIA_StopThread(void)
{
    PCMCIA_StopThd();
    LOG(3, "PCMCIA_StopThread\n");
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_EnableIsr
 *
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_EnableIsr(void)
{
    INT32 i4Result;
    
    i4Result = PCMCIA_EnableISR();
    LOG(5, "PCMCIA_EnableIsr : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_GetIfType (Just for CLI)
 *      This API gets the operation mode.
 */
//-----------------------------------------------------------------------------
PCMCIA_IF_TYPE_T PCMCIA_GetIfType(void)
{
    PCMCIA_IF_TYPE_T eResult;

    eResult = PCMCIAHW_GetIfType();
    LOG(3, "PCMCIA_GetIfType : %d\n", eResult);
    return eResult;
}

//-----------------------------------------------------------------------------
/** PCMCIA_SetIfType (Just for CLI)
 *      This API sets the operation mode.
 */
//-----------------------------------------------------------------------------
void PCMCIA_SetIfType(PCMCIA_IF_TYPE_T eOpMode)
{
    PCMCIAHW_SetIfType(eOpMode);
    if ( eOpMode == PCMCIA_IF_TYPE_NFI )
    {
        LOG(3, "PCMCIA_SetIfType : NFI\n"); 
    }
    else
    {
        LOG(3, "PCMCIA_SetIfType : SPI\n"); 
    }
}

//-----------------------------------------------------------------------------
/** PCMCIA_GetPodciAccessMode (Just for CLI)
 *  
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_GetPodciAccessMode(POD_ACCESS_MODE_T* peAccessMode)
{
    INT32 i4Result;

    i4Result = PODCI_GetPodCiAccessMode(peAccessMode);
    LOG(3, "PCMCIA_GetPodciAccessMode : %d\n", *peAccessMode);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_SetPodCiAccessMode (Just for CLI)
 *  
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode)
{   
    LOG(9, "PCMCIA_SetPodCiAccessMode : %d\n", (UINT32)eAccessMode);
    return PODCI_SetPodCiAccessMode(eAccessMode);
}

INT32 PCMCIA_GetDaFrIntStatus(POD_ACCESS_MODE_T    eAccessMode,
                            UINT8*               pu1DaFrSts)
{
	return PODCI_GetDaFrIntStatus(	eAccessMode,pu1DaFrSts);
}
BOOL PCMCIA_ResetChannel(void)
{
	return PODCI_ResetChannel();
}


//-----------------------------------------------------------------------------
/** PCMCIA_NegotiatedBufSize (For CLI & Linux MTAL)
 *  
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_NegotiatedBufSize(UINT16* pu2BufSize)
{
    INT32 i4Result;

    VERIFY(PODCI_SetPodCiAccessMode(POD_DATA_CHANNEL) == POD_OK);

    i4Result = PODCI_NegotiateBufSize(pu2BufSize);
    LOG(0, "PCMCIA_NegotiatedBufSize : %d\n", *pu2BufSize);
    if (!i4Result)
    {
        LOG(0, "PCMCIA_NegotiatedBufSize Failed\n");
        return POD_BUF_SIZE_NGO_FAILED;
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** PCMCIA_WaitForModuleStatus (Just for CLI)
 *  
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_WaitForModuleStatus(UINT8 u1StatusBit, UINT16 u2TimeOut)
{
    INT32 i4Result;

    i4Result = PODCI_WaitForModuleStatus(u1StatusBit, u2TimeOut);
    LOG(3, "PCMCIA_WaitForModuleStatus : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ParseCis (Just for CLI)
 *      This API parses CIS.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_ParseCis(UINT8* pu1CisMap, CIS_INFO_T* prCisInfo)
{
    INT32 i4Result;
    
    i4Result = PODCI_ParseCis(pu1CisMap, prCisInfo);
    LOG(3, "PCMCIA_ParseCis : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_CheckCIS (For CLI & Linux MTAL)
 *      This API checks CIS.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_CheckCIS(UINT8* pu1Cis, UINT16 *addrcor, UINT8 *cor)
{
    INT32 i4Result;

    i4Result = PODCI_CheckCIS(pu1Cis, addrcor, cor);
    LOG(3, "PCMCIA_CheckCIS : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_CheckCIPLUSCapability (For CLI & Linux MTAL)
 *      This API checks CI+ capability from CIS.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_CheckCIPLUSCapability(UINT8 *pui1CIPlusCap)
{
    INT32 i4Result;
    
    i4Result = PODCI_CheckCIPLUSCapability(pui1CIPlusCap);
    LOG(0, "PODCI_CheckCIPLUSCapability : %d, %d\n", i4Result, *pui1CIPlusCap);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_GetBypass
 *      This API return 8295 bypass status.
 * @return : 1: bypass, 0:non-bypass
 */
//-----------------------------------------------------------------------------
UINT32 PCMCIA_GetBypass()
{
    if ( PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & 0x80000000 )
    {
        LOG(3, "PCMCIA_GetBypass : Bypass is On\n");
        return 1;
    }
    else
    {
        LOG(3, "PCMCIA_GetBypass : Bypass is Off\n");
        return 0;
    }
}

//-----------------------------------------------------------------------------
/** PCMCIA_SetBypass
 *      This API can bypass 8295.
 */
//-----------------------------------------------------------------------------
void PCMCIA_SetBypass(BOOL fgBypass)
{
    LOG(3, "PCMCIA_SetBypass : %d\n", fgBypass);
    PODCI_SetBypass(fgBypass);
}


void PCMCIA_SetBypassTsRouting(BOOL fgBypass)
{
   PCMCIA_SetTsRouting(fgBypass, &_rTsFmt);
}

//-----------------------------------------------------------------------------
/** PCMCIA_PcmciaToPod
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_PcmciaToPod(BOOL fgPcmciaToPod)
{
    INT32 i4Result;
    static INT32 bCurrentSetting = -1;

    if ( bCurrentSetting == (INT32)fgPcmciaToPod )
    {
        return 0;
    }

    i4Result = PCMCIA_PcmciaToPodCi(fgPcmciaToPod);
    
    if ( bCurrentSetting != (INT32)fgPcmciaToPod )
    {
        LOG(3, "PCMCIA_PcmciaToPod(%d) : %d\n", fgPcmciaToPod, i4Result);
        bCurrentSetting = (INT32)fgPcmciaToPod;
    }
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_PcmciaEnableTS
 *      
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_PcmciaEnableTS(BOOL fgPcmciaEnableTS)
{
    INT32 i4Result;
    UNUSED(fgPcmciaTsEnabled);

    fgPcmciaTsEnabled = fgPcmciaEnableTS;

    i4Result = PCMCIA_EnableTS(fgPcmciaEnableTS);
    LOG(3, "PCMCIA_PcmciaEnableTS(%d) : %d\n", fgPcmciaEnableTS, i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_DetectCard
 *      
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_DetectCard(void)
{
    INT32 i4Result;

    i4Result = PCMCIA_DetectCableCard();
    LOG(9, "PCMCIA_DetectCard\n");
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_TestCardReady
 *      Test card ready status.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_TestCardReady(void)
{
    INT32 i4Result;
    static INT32 i4CardReady = FALSE;
    
    i4Result = (PCMCIAHW_QueryPbusSts((UINT32)PC_RDY_IRQ) ? 1 : 0);
    if ( i4CardReady != i4Result )
    {
        LOG(3, "PCMCIA_TestCardReady : %d\n", i4Result);
        i4CardReady = i4Result;
    }
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_DetectPodActive
 *      Detect if slot is in POD/CI mode.
 */
//-----------------------------------------------------------------------------
BOOL PCMCIA_DetectPodActive(void)
{
    BOOL i4Result;
    i4Result = PCMCIA_DetectPodCiActive();
    LOG(7, "PCMCIA_DetectPodActive : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ExternalDemodOnOff (Just for CLI)
 *      Detect if slot is in POD/CI mode.
 */
//-----------------------------------------------------------------------------
void PCMCIA_ExternalDemodOnOff(BOOL fgOnOff)
{
    PCMCIA_SetExtDemodOnOff(fgOnOff);
    LOG(3, "PCMCIA_ExternalDemodOnOff(%d)\n", fgOnOff);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ParallelTsOnOff (Just for CLI)
 *      Detect if Ts setting is parallel or serial.
 *      Note: For 8295B, if we want to set TS to parallel, only register 0x88 
 *            bit 30 should be turn-on. We could ignore register 0x94 bit 14.
 *            This means when setting parallel ts, external demod is always used.
 */
//-----------------------------------------------------------------------------
void PCMCIA_ParallelTsOnOff(BOOL fgOnOff)
{
    PCMCIA_SetParallelTsOnOff(fgOnOff);
    LOG(3, "PCMCIA_ExternalDemodOnOff(%d)\n", fgOnOff);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_RegIsr
 *      Register 8295 Isr. The prototype is copied from PDWNC_RegIsr()
 */
//-----------------------------------------------------------------------------
void PCMCIA_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld)
{
    PCMCIA_RegISR(u2IntNo, pfnIsr, ppfnOld);
    LOG(3, "PCMCIA_RegIsr(%d)\n", u2IntNo);
    return;    
}


//-----------------------------------------------------------------------------
/** PCMCIA_RegCamStatusCbk
 *      Register callback for cam status.
 */
//-----------------------------------------------------------------------------
void PCMCIA_RegCamStatusCbk(x_pcmcia_nfy_fct pfn)
{
    PCMCIA_RegCAMStatusCbk(pfn);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ExternalIcSetTsClkRate
 *      Set MT8295 TS CLK Rate, could be 63(default)/72/81/94/108/135/162
 */
//-----------------------------------------------------------------------------
void PCMCIA_ExternalIcSetTsClkRate(UINT32 u4ClkRate)
{
    PCMCIA_ExternalICSetTsClkRate(u4ClkRate);
    LOG(3, "PCMCIA_ExternalIcSetTsClkRate : %dMhz\n", u4ClkRate);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_EnableDemodTs (Just for CLI)
 *      This API is just for debug purpose
 */
//-----------------------------------------------------------------------------
void PCMCIA_EnableDemodTs(void)
{
    LOG(1, "PCMCIA_EnableDemodTs\n");
    LOG(1, "    TunerSetTsTristate(BOOL_TRUE)\n");
    TunerSetTsTristate(BOOL_TRUE);
}

//-----------------------------------------------------------------------------
/** PCMCIA_DisableDemodTs (Just for CLI)
 *      This API is just for debug purpose
 */
//-----------------------------------------------------------------------------
void PCMCIA_DisableDemodTs(void)
{
    LOG(1, "PCMCIA_DisableDemodTs\n");
    LOG(1, "    TunerSetTsTristate(BOOL_FALSE)\n");
    TunerSetTsTristate(BOOL_FALSE);
}

//-----------------------------------------------------------------------------
/** PCMCIA_AutoSetTsRoutingPath
 */
//-----------------------------------------------------------------------------
void PCMCIA_AutoSetTsRoutingPath(void)
{
    _PCMCIA_AutoSetTsRoutingPath();
}

//-----------------------------------------------------------------------------
/** PCMCIA_SLT
 */
//-----------------------------------------------------------------------------
void PCMCIA_SLT(UINT8 u1TsIndex, UINT8 u1TsClkRate, UINT8 u1TsDataWidth)
{
    LOG(0, "PCMCIA SLT Start\n");
    PCMCIA_SltStart(u1TsIndex, u1TsClkRate, u1TsDataWidth);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_Demo
 *      Invoked by demo program initialization flow.
 *      PCMCIA driver will create a thread to monitor the CI CAM status.
 *      Once the CI CAM is inserted, PCMCIA driver will change TS path to go
 *      through CI CAM.
 */
//-----------------------------------------------------------------------------
void PCMCIA_Demo(pcmcia_cam_connectivity_nfy pfnCbk)
{
    LOG(0, "PCMCIA Demo Start\n");
    PCMCIA_DemoStart(pfnCbk);
    return;    
}

//-----------------------------------------------------------------------------
/** PCMCIA_SetTsType
 *      Set TS type (DVB-T or DVB-C), the pcmcia driver will config the TS 
 *      routing path.
 *  @param u4TypeValue : 0:DVB-T, 1:DVB-C
 */
//-----------------------------------------------------------------------------
void PCMCIA_SetTsType(PCMCIA_TS_TYPE eTypeValue)
{
    u4TsType = eTypeValue;
    LOG(1, "PCMCIA_SetTsType : %d\n", u4TsType);
    _PCMCIA_AutoSetTsRoutingPath();
}

//-----------------------------------------------------------------------------
/** PCMCIA_GetTsType
 *      Get TS type (DVB-T or DVB-C).
 *  @return : 0:DVB-T, 1:DVB-C
 */
//-----------------------------------------------------------------------------
PCMCIA_TS_TYPE PCMCIA_GetTsType()
{
    LOG(1, "PCMCIA_GetTsType : %d\n", u4TsType);
    return u4TsType;
}

void PCMCIA_UpdateTsStatus()
{
    LOG(1, "PCMCIA_UpdateTsStatus\n");
    //PCMCIA_Init();
    TunerGetTsFmt(&_rTsFmt);

    PCMCIA_AutoSetTsRoutingPath();
}

//-----------------------------------------------------------------------------
/** PCMCIA_CiCamWorkaround
 */
//-----------------------------------------------------------------------------
void PCMCIA_CiCamWorkaround(CI_CAM_WORKAROUND eCiCamWorkaround, void* vParam)
{
    LOG(1, "PCMCIA_CiCamWorkaround : %d\n", eCiCamWorkaround);
    
    switch ( eCiCamWorkaround )
    {
        case CI_CAM_WORKAROUND1:
        {
            UCHAR uParam[2];
            BOOL bWorkaroundOn = *((BOOL*)vParam);
            UNUSED(uParam);
            uParam[0] = 0;
            if ( bWorkaroundOn == TRUE )
            {
                uParam[1] = (UCHAR)1;
            }
            else
            {
                uParam[1] = (UCHAR)0;
            }
            TunerSetCiCardType(&(uParam[0]));
        }
        break;
            
        case CI_CAM_WORKAROUND2:
            break;

        case CI_CAM_WORKAROUND3:
        break;
            
        default: 
            break;
    }
}

#ifdef CC_FAST_INIT
INT32 CI_GPIO_Suspend(void)
{
    PCMCIA_GPIO_Suspend();
    return 0;    
}
INT32 CI_GPIO_Resume(void)
{
    PCMCIA_GPIO_Resume();
    return 0;    
}

INT32 CI_GPIO_ResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType)
{
    return PCMCIA_GPIO_Reg(i4Gpio, eType, NULL);
}
#endif

INT32 CI_MT8295_GPIO_Range_Check(INT32 i4GpioNum)
{
    return PCMCIA_MT8295_GPIO_Range_Check(i4GpioNum);
}

INT32 CI_GPIO_Init(void)
{
    return PCMCIA_GPIO_Init();
}

INT32 CI_GPIO_Range_Check(INT32 i4GpioNum)
{
    return PCMCIA_GPIO_Range_Check(i4GpioNum);  
}

INT32   CI_MT5135_GPIO_Range_Check(INT32 i4GpioNum)
{
    return PCMCIA_MT5135_GPIO_Range_Check(i4GpioNum);
}   
    
INT32 CI_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity)
{
    return PCMCIA_GPIO_Enable(i4GpioNum, pi4Polarity);
}

INT32 CI_GPIO_Input(INT32 i4GpioNum)
{
    return PCMCIA_GPIO_Input(i4GpioNum);
}

INT32 CI_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow)
{
    return PCMCIA_GPIO_Output(i4GpioNum, pi4DriveHighLow);
}

INT32 CI_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet)
{
    return PCMCIA_GPIO_Intrq(i4Gpio, pfgSet);
}

INT32 CI_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    return PCMCIA_GPIO_Reg(i4Gpio, eType, pfnCallback);
}

INT32 CI_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    return PCMCIA_GPIO_Query(i4Gpio, pi4Intr, pi4Fall, pi4Rise);
}

#ifndef CC_MTK_LOADER
//#include "x_chip_id.h"
INT32 PCMCIA_BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size)
{
    return 0;//BIM_GetDeviceData(pu1DeviceID, u1Size);
}

INT32 PCMCIA_BIM_GetDeviceID(BYTE *pu1DeviceID, BYTE u1Size)
{
    return 0;//BIM_GetDeviceID(pu1DeviceID, u1Size);
}

INT32 PCMCIA_BIM_GetSecureID(BYTE *pu1SecureID, BYTE u1Size)
{
    return 0;//BIM_GetSecureID(pu1SecureID, u1Size);
}

// For debug 2 bit by cli.
void PCMCIA_Set2Bit(BOOL fg2Bit)
{
    _PCMCIA_Set2Bit(fg2Bit);
}

BOOL PCMCIA_Get2Bit(void)
{
    BOOL fg2bit = _PCMCIA_Get2Bit();
    return fg2bit;
}

void PcmciaSetDriving(UINT32 u4Driving)
{
    PCMCIA_SetDriving(u4Driving);
    return;   
} 

void PcmciaSetTsClock(UINT32 u4TsClock)
{
    PCMCIA_SetCiTsClock(u4TsClock);
    return;   
} 

void PcmciaSetSysClock(UINT32 u4SysClock)
{
    PCMCIA_SetCiSysClock(u4SysClock);
    return;   
} 

#ifdef CC_FAST_INIT
void    PCMCIA_pm_suspend(void)
{
    PCMCIA_Suspend();
}

void    PCMCIA_pm_resume(void) 
{
    PCMCIA_Initialize();
    return;
}
#endif
#endif //CC_MTK_LOADER

#else // PCMCIA_ENABLED

#ifndef CC_MTK_LOADER
INT32   PCMCIA_Init(void){return 0;}
INT32   PCMCIA_LowLevelInit(void){return 0;}
UINT32  PCMCIA_RawRead32(UINT32 offset){return 0;}
UINT32  PCMCIA_RawWrite32(UINT32 offset, UINT32 value){return 0;}
INT32   PCMCIA_Read(POD_IO_STATE_T eIoState, UINT8* pu1Data, UINT16* pu2DataLen){return 0;}
INT32   PCMCIA_Write(POD_IO_STATE_T eIoState, const UINT8* pu1Data, UINT16 u2DataLen){return 0;}
INT32   PCMCIA_ReadReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr, UINT8* pu1Data, UINT16 pu2DataLen){return 0;}
INT32   PCMCIA_WriteReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr, const UINT8* pu1Data, UINT16 u2DataLen){return 0;}
INT32   PCMCIA_ReadCor(UINT16 u2CorAddress){return 0;}
INT32   PCMCIA_WriteCor(UINT16 u2CorAddress, UINT8 u1CorValue){return 0;}
void    PCMCIA_CamPwrControl(BOOL bManualControl){return;}
void    PCMCIA_CamPwrOnOff(BOOL bPwrOn){return;}
INT32   PCMCIA_Reset(BOOL b_set){return 0;}
void    PCMCIA_ResetCam(void){return;}
void    PCMCIA_ResetPcmciaHw(void){return;}
void    PCMCIA_ThreadNoAssert(BOOL bNoAssert){return;}
void    PCMCIA_StopThread(void){return;}
INT32   PCMCIA_EnableIsr(void){return 0;}
PCMCIA_IF_TYPE_T     PCMCIA_GetIfType(void){return PCMCIA_IF_TYPE_SPI;}
void                 PCMCIA_SetIfType(PCMCIA_IF_TYPE_T eOpMode){return;}
INT32   PCMCIA_GetPodCiAccessMode(POD_ACCESS_MODE_T* peAccessMode){return 0;}
INT32   PCMCIA_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode){return 0;}
INT32 PCMCIA_GetDaFrIntStatus(POD_ACCESS_MODE_T  eAccessMode, UINT8* pu1DaFrSts){return 0;}
BOOL PCMCIA_ResetChannel(void){return 0;}


INT32   PCMCIA_NegotiatedBufSize(UINT16* pu2BufSize){return 0;}
INT32   PCMCIA_WaitForModuleStatus(UINT8 u1StatusBit, UINT16 u2TimeOut){return 0;}
INT32   PCMCIA_ParseCis(UINT8* pu1CisMap, CIS_INFO_T* prInfo){return 0;}
INT32   PCMCIA_CheckCIS(UINT8* pu1Cis, UINT16 *addrcor, UINT8 *cor){return 0;}
INT32   PCMCIA_CheckCIPLUSCapability(UINT8 *pui1CIPlusCap){return 0;}
UINT32  PCMCIA_GetBypass(void){return 0;}
void    PCMCIA_SetBypass(BOOL fgBypass){return;}
void    PCMCIA_SetBypassTsRouting(BOOL fgBypass){return;};
INT32   PCMCIA_PcmciaToPod(BOOL fgPcmciaToPod){return 0;}
INT32   PCMCIA_PcmciaEnableTS(BOOL fgPcmciaEnableTS){return 0;}
INT32   PCMCIA_DetectCard(void){return 0;}
INT32   PCMCIA_TestCardReady(void){return 0;}
BOOL    PCMCIA_DetectPodActive(void){return 0;}
void    PCMCIA_ExternalDemodOnOff(BOOL fgOnOff){return;}
void    PCMCIA_ParallelTsOnOff(BOOL fgOnOff){return;}
void    PCMCIA_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld){return;}
void    PCMCIA_ExternalIcSetTsClkRate(UINT32 u4ClkRate){return;}
void    PCMCIA_EnableDemodTs(void){return;}
void    PCMCIA_DisableDemodTs(void){return;}
void    PCMCIA_AutoSetTsRoutingPath(void){return;}
void    PCMCIA_SLT(UINT8 u1TsIndex, UINT8 u1TsClkRate, UINT8 u1TsDataWidth){return;}
void    PCMCIA_Demo(pcmcia_cam_connectivity_nfy pfnCbk){return;}
void            PCMCIA_SetTsType(PCMCIA_TS_TYPE eTypeValue){return;}
PCMCIA_TS_TYPE  PCMCIA_GetTsType(void){return PCMCIA_TS_TYPE_DVB_T;}
void    PCMCIA_UpdateTsStatus(void){return;}
void    PCMCIA_SetTsPath(BOOL fgExternalDemod, BOOL fgThroughCard){return;}
void    PCMCIA_CiCamWorkaround(CI_CAM_WORKAROUND eCiCamWorkaround, void* vParam){return;}
#endif //MTK_LOADER

#ifdef CC_FAST_INIT
INT32   CI_GPIO_Suspend(void){return 0;}
INT32   CI_GPIO_Resume(void){return 0;}
INT32   CI_GPIO_ResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType){return 0;}
#endif

INT32   CI_MT8295_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
/*
INT32   CI_GPIO_Init(void){return -1;}
INT32   CI_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
//INT32   CI_MT8295_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
INT32   CI_MT5135_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
INT32   CI_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity){return 0;}
INT32   CI_GPIO_Input(INT32 i4GpioNum){return 0;}
INT32   CI_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow){return 0;}
INT32   CI_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet){return 0;}
INT32   CI_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback){return 0;}
INT32   CI_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise){return 0;}
*/
#ifndef CC_MTK_LOADER
INT32   PCMCIA_BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size){return 0;}
INT32   PCMCIA_BIM_GetDeviceID(BYTE *pu1DeviceID, BYTE u1Size){return 0;}
INT32   PCMCIA_BIM_GetSecureID(BYTE *pu1SecureID, BYTE u1Size){return 0;}


void PCMCIA_Set2Bit(BOOL fg2Bit){return;}
BOOL PCMCIA_Get2Bit(void){return 0;}

void    PcmciaSetDriving(UINT32 u4Drivin) {return;}
void    PcmciaSetTsClock(UINT32 u4TsClock){return;}
void    PcmciaSetSysClock(UINT32 u4SysClock) {return;}
#ifdef CC_FAST_INIT
void    PCMCIA_pm_suspend(void) {return;}
void    PCMCIA_pm_resume(void) {return;}
#endif
#endif // CC_MTK_LOADER

#if defined(CC_EXTERNAL_IC_MT5135)
INT32 CI_GPIO_Init(void)
{
    return PCMCIA_GPIO_Init();
}

INT32 CI_GPIO_Range_Check(INT32 i4GpioNum)
{
    return PCMCIA_GPIO_Range_Check(i4GpioNum);  
}

INT32   CI_MT5135_GPIO_Range_Check(INT32 i4GpioNum)
{
    return PCMCIA_MT5135_GPIO_Range_Check(i4GpioNum);
}   
    
INT32 CI_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity)
{
    return PCMCIA_GPIO_Enable(i4GpioNum, pi4Polarity);
}

INT32 CI_GPIO_Input(INT32 i4GpioNum)
{
    return PCMCIA_GPIO_Input(i4GpioNum);
}

INT32 CI_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow)
{
    return PCMCIA_GPIO_Output(i4GpioNum, pi4DriveHighLow);
}

INT32 CI_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet)
{
    return PCMCIA_GPIO_Intrq(i4Gpio, pfgSet);
}

INT32 CI_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    return PCMCIA_GPIO_Reg(i4Gpio, eType, pfnCallback);
}

INT32 CI_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    return PCMCIA_GPIO_Query(i4Gpio, pi4Intr, pi4Fall, pi4Rise);
}
#else
INT32   CI_GPIO_Init(void){return -1;}
INT32   CI_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
//INT32   CI_MT8295_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
INT32   CI_MT5135_GPIO_Range_Check(INT32 i4GpioNum){return 0;}
INT32   CI_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity){return 0;}
INT32   CI_GPIO_Input(INT32 i4GpioNum){return 0;}
INT32   CI_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow){return 0;}
INT32   CI_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet){return 0;}
INT32   CI_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback){return 0;}
INT32   CI_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise){return 0;}  
#endif

#endif // PCMCIA_ENABLED

