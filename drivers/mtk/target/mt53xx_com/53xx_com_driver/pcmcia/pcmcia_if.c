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


#ifdef PCMCIA_ENABLED

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

MT8295_VERSION eMt8295Version = MT8295_VERSION_NONE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// This is a dirty work around solution. 
// Ths TS should not be routed by pcmcia driver. 
// It's better to set TS routing path by middleware.
// (Don't put this function into pcmcia_ctrl.c --> It's just work around.)
// (Don't do pcmcia functions here, do only DMX/Demod setting in this function.)
//-----------------------------------------------------------------------------
#include "dmx_if.h"
#include "pi_dvbt_if.h"
static UINT32 u4TsType = 0;
static void _PCMCIA_AutoSetTsRoutingPath(void)
{
    // Route TS based on the attributes : CamConnectiviey, DVB-C or DVB-T, External/Internal CI
    BOOL fgCamConnected = FALSE;
    BOOL fgIsDvbC = FALSE;
    BOOL fgIsInternalCi = FALSE;

    fgCamConnected = PCMCIA_DetectCableCard();
    fgIsDvbC = ((u4TsType==1)?TRUE:FALSE);
    fgIsInternalCi = ((ePcmciaChipType==PCMCIA_CHIP_TYPE_INTERNAL)?TRUE:FALSE);

    if ( fgIsInternalCi )   // Internal CI
    {
        if ( fgIsDvbC ) // DVB-C    --> TODO
        {
            LOG(0, "TODO\n");
            ASSERT(0);
            if ( fgCamConnected )   // CAM Connected
            {
            }
            else    // CAM not Connected
            {
            }
        }
        else    // DVB-T
        {
            if ( fgCamConnected )   // CAM Connected
            {
                DVBT_SetTSInterface(TRUE);
                DMX_SetFrontEnd((DMX_FRONTEND_T)9);
                LOG(0, "    Set d.sfe 9\n");
            }
            else    // CAM not Connected
            {
                DVBT_SetTSInterface(FALSE);
                DMX_SetFrontEnd((DMX_FRONTEND_T)1);
                LOG(0, "    Set d.sfe 1\n");
            }
        }
    }
    else    // External CI
    {
        if ( fgIsDvbC ) // DVB-C
        {
            DMX_SetFrontEnd((DMX_FRONTEND_T)3);
                LOG(0, "    Set d.sfe 3\n");
        }
        else    // DVB-T
        {
            if ( fgCamConnected )   // CAM Connected
            {
                DMX_SetFrontEnd((DMX_FRONTEND_T)3);
                LOG(0, "    Set d.sfe 3\n");
            }
            else    // CAM not Connected
            {
                DMX_SetFrontEnd((DMX_FRONTEND_T)1);
                LOG(0, "    Set d.sfe 1\n");
            }
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
    LOG(3, "PCMCIA_Init : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_LowLevelInit
 *      Only invoked by pcmcia_cmd.c. This API will not only initialize 8295,
 *      but does personality change & nego buffer size.
 *  @param void
 *  @return INT32
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
/** POD_RawRead8
 *  Read 8295 register.
 */
//-----------------------------------------------------------------------------
UINT8 PCMCIA_RawRead8(UINT32 offset)
{
    UINT8 u1Result;

    u1Result = PCMCIAHW_READ8(offset);
    LOG(9, "PCMCIA_RawRead8(%d) : %d\n", offset, u1Result);
    
    return u1Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_RawRead32
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
/** PCMCIA_RawWrite32
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
/** PCMCIA_Read
 *      This API reads a number of bytes from CAM common memory. (address 0 for data)
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_Read(POD_IO_STATE_T eIoState, UINT8* pu1Data, UINT16* pu2DataLen)
{
    INT32 i4Result;

    i4Result = PODCI_ReadCard(eIoState, pu1Data, pu2DataLen);
    
    LOG(3, "PCMCIA_Read : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_Write
 *      This API writes a number of bytes to CAM common memory. (address 0 for data)
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_Write(POD_IO_STATE_T eIoState, const UINT8* pu1Data, UINT16 u2DataLen)
{
    INT32 i4Result;
    ASSERT(pu1Data != NULL);

    i4Result = PODCI_WriteCard(eIoState, pu1Data, u2DataLen);
    LOG(3, "PCMCIA_Write : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ReadReg
 *      This API writes a number of bytes from CAM common memory.
 *      address 0 for data, address 1 for command/status.
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_ReadReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr,
                     UINT8* pu1Data, UINT16 u2DataLen)
{
    INT32 i4Result;
    ASSERT((pu1Data != NULL) && (u2DataLen != NULL));

    i4Result = PODCI_ReadCardReg(eIoState, u4RegAddr, pu1Data, u2DataLen);
    LOG(7, "PCMCIA_ReadReg(%d) : %d\n", u4RegAddr, i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_WriteReg
 *      This API writes a number of bytes to CAM common memory.
 *      address 0 for data, address 1 for command/status.
 *  @param
 *  @param
 *  @return   VOID
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
 *  @param
 *  @param
 *  @return   VOID
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
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_WriteCor(UINT16 u2CorAddress, UINT8 u1CorValue)
{
    INT32 i4Result;

    /* Disable BVD1 and BVD2 interrupts since the pins are used as TS data pins after personality change */
    PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & ~PC_BVD1_IE & ~PC_BVD2_IE);
    PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)POD);
    i4Result = PODCI_WriteCor(u2CorAddress, u1CorValue);
    LOG(3, "PCMCIA_WriteCor : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_CamPwrControl
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
/** PCMCIA_CamPwrOnOff
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
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_ResetCam(void)         //reset pod/ci card
{
    PCMCIA_ResetCableCard();
    LOG(3, "PCMCIA_ResetCam\n");
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ResetPcmciaHw
 *      Reset CAM
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_ResetPcmciaHw(void)
{
    PCMCIA_ResetPcmciaHW();
    LOG(3, "PCMCIA_ResetPcmciaHw Done\n");
    return;    
}

//-----------------------------------------------------------------------------
/** PCMCIA_ResetChannel
 *  
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
BOOL PCMCIA_ResetChannel(void)
{
    INT32 i4Result;
    
    i4Result = PODCI_ResetChannel();
    LOG(3, "PCMCIA_ResetChannel : %d\n", i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ThreadNoAssert
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
/** PCMCIA_StopThread
 */
//-----------------------------------------------------------------------------
void PCMCIA_StopThread(void)
{
    PCMCIA_StopThd();
    LOG(3, "PCMCIA_StopThread\n");
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_GetPodState
 *  
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_GetPodState(UINT32* pu4CurPodState)
{
    INT32 i4Result;

    i4Result = PCMCIA_GetState(pu4CurPodState);
    LOG(7, "PCMCIA_GetPodState : %d\n", *pu4CurPodState);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_RefreshPodState
 *  
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_RefreshPodState(void)
{
    LOG(3, "PCMCIA_RefreshPodState\n");
    PCMCIA_RefreshState();
}

//-----------------------------------------------------------------------------
/** PCMCIA_EnableIsr
 *  
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_GetIfType
 *  
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_SetIfType
 *      This API sets the operation mode.
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_GetPodciAccessMode
 *  
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_SetPodCiAccessMode
 *  
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode)
{   
    LOG(9, "PCMCIA_SetPodCiAccessMode : %d\n", (UINT32)eAccessMode);
    return PODCI_SetPodCiAccessMode(eAccessMode);
}

//-----------------------------------------------------------------------------
/** PCMCIA_NegotiatedBufSize
 *  
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_NegotiatedBufSize(UINT16* pu2BufSize)
{
    INT32 i4Result;

    VERIFY(PODCI_SetPodCiAccessMode(POD_DATA_CHANNEL) == POD_OK);
    if (!PODCI_ResetChannel())
    {
        LOG(0, "PCMCIA_NegotiatedBufSize: POD_CHANNEL_RESET_FAILED\n");
        return POD_CHANNEL_RESET_FAILED;
    }

    i4Result = PODCI_NegotiateBufSize(pu2BufSize);
    LOG(3, "PCMCIA_NegotiatedBufSize : %d\n", i4Result);
    if (!i4Result)
    {
        LOG(0, "PCMCIA_NegotiatedBufSize Failed\n");
        return POD_BUF_SIZE_NGO_FAILED;
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** PCMCIA_WaitForModuleStatus
 *  
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_Dump
 *  
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_Dump(CHAR* pszTitle, UINT8* pu1Msg, UINT32 u4MsgLen)
{    
    PODCI_Dump(pszTitle, pu1Msg, u4MsgLen);
    LOG(3, "PCMCIA_Dump\n");
}

//-----------------------------------------------------------------------------
/** PCMCIA_ParseCis
 *      This API parses CIS.
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_ParseCis
 *      This API parses CIS.
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_AllocDmaBuf
 *      This API allocates a DMA buffer for DMA read and write.
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_AllocDmaBuf(void)
{    
    PODCI_AllocDmaBuf();
    LOG(3, "PCMCIA_AllocDmaBuf\n");
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
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_SetBypass(BOOL fgBypass)
{
    LOG(3, "PCMCIA_SetBypass : %d\n", fgBypass);
    PODCI_SetBypass(fgBypass);
}

//-----------------------------------------------------------------------------
/** PCMCIA_PcmciaToPod
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_PcmciaToPod(BOOL fgPcmciaToPod)
{
    INT32 i4Result;
    static BOOL bCurrentSetting = FALSE;

    i4Result = PCMCIA_PcmciaToPodCi(fgPcmciaToPod);
    
    if ( bCurrentSetting != fgPcmciaToPod )
    {
        LOG(3, "PCMCIA_PcmciaToPod(%d) : %d\n", fgPcmciaToPod, i4Result);
        bCurrentSetting = fgPcmciaToPod;
    }
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_PcmciaEnableTS
 *      
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_PcmciaEnableTS(BOOL fgPcmciaEnableTS)
{
    INT32 i4Result;

    i4Result = PCMCIA_EnableTS(fgPcmciaEnableTS);
    LOG(3, "PCMCIA_PcmciaEnableTS(%d) : %d\n", fgPcmciaEnableTS, i4Result);
    return i4Result;
}

//-----------------------------------------------------------------------------
/** PCMCIA_DetectCard
 *      
 *  @param
 *  @param
 *  @return   VOID
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
 *  @param
 *  @param
 *  @return   VOID
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
 *  @param
 *  @param
 *  @return   VOID
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
/** PCMCIA_ExternalDemodOnOff
 *      Detect if slot is in POD/CI mode.
 *  @param
 *  @param
 *  @return   VOID
 */
//-----------------------------------------------------------------------------
void PCMCIA_ExternalDemodOnOff(BOOL fgOnOff)
{
    PCMCIA_SetExtDemodOnOff(fgOnOff);
    LOG(3, "PCMCIA_ExternalDemodOnOff(%d)\n", fgOnOff);
    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_ParallelTsOnOff
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
/** PCMCIA_MT8295PinSet
 *      Set MT8295 PINMUX.
 */
//-----------------------------------------------------------------------------
void PCMCIA_MT8295PinSet(UINT32 i4PinSel, UINT32 i4Func)
{
    PCMCIA_Mt8295PinSet(i4PinSel, i4Func);
    LOG(3, "PCMCIA_MT8295PinSet(%d, %d)\n", i4PinSel, i4Func);
    return;    
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
void PCMCIA_SLT(void)
{
    LOG(0, "PCMCIA SLT Start\n");
    PCMCIA_SltStart();
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
void PCMCIA_SetTsType(UINT32 u4TypeValue)
{
    UINT32 u4RegValue;
    UNUSED(u4RegValue);
    
    if ( u4TypeValue == 0 ) // DVB-T
    {
        u4TsType = 0;
        LOG(0, "PCMCIA_SetTsType : DVB-T\n");
        
        PCMCIA_ParallelTsOnOff(FALSE);
        LOG(0, "    Set Parallel TS Off\n");
        
        _PCMCIA_AutoSetTsRoutingPath();
    }
    else    // DVB-C
    {
        u4TsType = 1;
        LOG(0, "PCMCIA_SetTsType : DVB-C\n");

        PCMCIA_ParallelTsOnOff(TRUE);
        LOG(0, "    Set Parallel TS On\n");
        
        _PCMCIA_AutoSetTsRoutingPath();
    }

    if ( PCMCIA_GetBypass() )
    {
        LOG(0, "    8295 Bypass is On\n");
    }
    else
    {
        LOG(0, "    8295 Bypass is Off\n");
    }

    return;
}

//-----------------------------------------------------------------------------
/** PCMCIA_GetTsType
 *      Get TS type (DVB-T or DVB-C).
 *  @return : 0:DVB-T, 1:DVB-C
 */
//-----------------------------------------------------------------------------
UINT32 PCMCIA_GetTsType()
{
    if ( u4TsType == 0 )
    {
        LOG(9, "PCMCIA_GetTsType : DVB-T\n");
    }
    else
    {
        LOG(9, "PCMCIA_GetTsType : DVB-C\n");
    }

    return u4TsType;
}

#else
INT32   PCMCIA_Init(void){return 0;}
INT32   PCMCIA_LowLevelInit(void){return 0;}
UINT8   PCMCIA_RawRead8(UINT32 offset){return 0;}
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
BOOL    PCMCIA_ResetChannel(void){return 0;}
void    PCMCIA_ThreadNoAssert(BOOL bNoAssert){return;}
void    PCMCIA_StopThread(void){return;}
INT32   PCMCIA_GetPodState(UINT32* pu4ModuleState){return 0;}
void    PCMCIA_RefreshPodState(void){return;}
INT32   PCMCIA_EnableIsr(void){return 0;}
PCMCIA_IF_TYPE_T     PCMCIA_GetIfType(void){return PCMCIA_IF_TYPE_SPI;}
void                 PCMCIA_SetIfType(PCMCIA_IF_TYPE_T eOpMode){return;}
INT32   PCMCIA_GetPodCiAccessMode(POD_ACCESS_MODE_T* peAccessMode){return 0;}
INT32   PCMCIA_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode){return 0;}
INT32   PCMCIA_NegotiatedBufSize(UINT16* pu2BufSize){return 0;}
INT32   PCMCIA_WaitForModuleStatus(UINT8 u1StatusBit, UINT16 u2TimeOut){return 0;}
void    PCMCIA_Dump(CHAR* pszTitle, UINT8* pu1Msg, UINT32 u4MsgLen){return;}
INT32   PCMCIA_ParseCis(UINT8* pu1CisMap, CIS_INFO_T* prInfo){return 0;}
INT32   PCMCIA_CheckCIS(UINT8* pu1Cis, UINT16 *addrcor, UINT8 *cor){return 0;}
void    PCMCIA_AllocDmaBuf(void){return;}
UINT32  PCMCIA_GetBypass(void){return 0;}
void    PCMCIA_SetBypass(BOOL fgBypass){return;}
INT32   PCMCIA_PcmciaToPod(BOOL fgPcmciaToPod){return 0;}
INT32   PCMCIA_PcmciaEnableTS(BOOL fgPcmciaEnableTS){return 0;}
INT32   PCMCIA_DetectCard(void){return 0;}
INT32   PCMCIA_TestCardReady(void){return 0;}
BOOL    PCMCIA_DetectPodActive(void){return 0;}
void    PCMCIA_ExternalDemodOnOff(BOOL fgOnOff){return;}
void    PCMCIA_ParallelTsOnOff(BOOL fgOnOff){return;}
void    PCMCIA_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld){return;}
void    PCMCIA_MT8295PinSet(UINT32 i4PinSel, UINT32 i4Func){return;}
void    PCMCIA_AutoSetTsRoutingPath(void){return;}
void    PCMCIA_SLT(void){return;}
void    PCMCIA_Demo(pcmcia_cam_connectivity_nfy pfnCbk){return;}
void    PCMCIA_SetTsType(UINT32 u4TypeValue){return;}
UINT32  PCMCIA_GetTsType(void){return 0;}
#endif

