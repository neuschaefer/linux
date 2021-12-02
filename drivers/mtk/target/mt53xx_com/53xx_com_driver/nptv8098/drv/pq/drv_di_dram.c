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
 * $RCSfile: drv_di.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 * MIB Help 264 Decoder to Write DRAM
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////
#include "hw_di.h"
#include "hw_sw.h"
#include "hw_ycproc.h"

#include "vdo_misc.h"

#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_tdnavi.h"
#include "fbm_drvif.h"

#include "x_assert.h"
// Frame Number usage can be reduced under output Sync mode if input frame rate = output frame rate
#define MIB_REDUCED_INTERLACE_FRAME_NUM (1) 

/**
 * @brief Set DI Dram dram number
 * @param u1VdpId VDP_1/VDP_2
 */
static void _vDrvDISetFrameNum(UINT8 u1VdpId)
{
    UINT32 u4IsFSInput=IS_FRAME_SEQ(u1VdpId);    
    UINT8 u1YFieldNum, u1CFieldNum;

    switch (_arMDDiPrm[u1VdpId].eDataMode)
    {
        //Non FRC
        case E(I,NR_OFF   ,DI_BOB ,FRC_NON):
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;        
        //Scaler FRC
        case E(I,NR_OFF   ,DI_BOB ,FRC_SCL):
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;
        case E(I,NR_OFF   ,DI_Y3D ,FRC_SCL)://This Mode only used under Sub            
        case E(I,NR_YC    ,DI_Y3D ,FRC_SCL):
            u1YFieldNum = 4;
            u1CFieldNum = 3;
            break;  
        case E(I,NR_OFF   ,DI_YC3D,FRC_SCL)://This Mode only used under sub
        case E(I,NR_YC    ,DI_YC3D,FRC_SCL):
            u1YFieldNum = 4;
            u1CFieldNum = 4;
            break;                   
        //MIB FRC
        case E(I,NR_OFF   ,DI_BOB ,FRC_MIB):
            u1YFieldNum = 2;
            u1CFieldNum = 2;
            break;            
        case E(I,NR_Y_ONLY,DI_OFF ,FRC_MIB):
            u1YFieldNum = 2;
            u1CFieldNum = 2;
            break;          
        case E(I,NR_YC    ,DI_OFF ,FRC_MIB):
            u1YFieldNum = 2;
            u1CFieldNum = 2;
            break;        
        case E(I,NR_OFF   ,DI_Y3D ,FRC_MIB):
            u1YFieldNum = 5;
            u1CFieldNum = 4;
            break;   
        case E(I,NR_OFF   ,DI_YC3D,FRC_MIB):
            u1YFieldNum = 5;
            u1CFieldNum = 5;
            break;         
        case E(I,NR_YC    ,DI_Y3D ,FRC_MIB):
            u1YFieldNum = 5;
            u1CFieldNum = 4;
            break;        
        case E(I,NR_YC    ,DI_YC3D,FRC_MIB):
            u1YFieldNum = 5;
            u1CFieldNum = 5;
            break;         
        //Scaler FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_SCL):
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;            
        case E(P,NR_Y_ONLY,DI_OFF ,FRC_SCL):
            if(u4IsFSInput||IS_SD_HEIGHT(u1VdpId))
            {
                u1YFieldNum = 2;
                u1CFieldNum = 0;
            }
            else
            {
                u1YFieldNum = 1;
                u1CFieldNum = 0;
            }
            break;         
        case E(P,NR_YC    ,DI_OFF ,FRC_SCL):
            if(u4IsFSInput||IS_SD_HEIGHT(u1VdpId))
            {
                u1YFieldNum = 2;
                u1CFieldNum = 2;                
            }
            else
            {
                u1YFieldNum = 1;
                u1CFieldNum = 1;                
            }
            break;         
        //MIB FRC
        case E(P,NR_OFF   ,DI_OFF ,FRC_MIB):
            u1YFieldNum = 2;
            u1CFieldNum = 2;
            break;        
        case E(P,NR_Y_ONLY,DI_OFF ,FRC_MIB):
            u1YFieldNum = 2;
            u1CFieldNum = 2;
            break;         
        case E(P,NR_YC    ,DI_OFF ,FRC_MIB):
            u1YFieldNum = 2;
            u1CFieldNum = 2;
            break;            
        default:
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;
    }

    if(IS_FLIP_MIRROR(u1VdpId))
    {
        u1YFieldNum++;
        u1CFieldNum++;
    }
#if MIB_REDUCED_INTERLACE_FRAME_NUM
    else if(IS_INTERLACE(u1VdpId)&&IS_MIB_FRC(u1VdpId))
    {
        //Frame Number usage can be reduced if input frame rate = output frame rate,
        //And not flip mode
        u1YFieldNum--;
        u1CFieldNum--;
    }    
#endif
    //Set the number of frame buffer
    u1YFieldNum = u1YFieldNum ? (u1YFieldNum - 1) : 0;
    u1CFieldNum = u1CFieldNum ? (u1CFieldNum - 1) : 0;
    vDrvDISetFrmNum(u1VdpId, u1YFieldNum, u1CFieldNum, u1CFieldNum);
}

/**
 * @brief Calculate Msb page number
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Width
 * @param u2Height
 * @param u1YNum
 * @param u1CNum
 */
static void _vDrvDICalMsbPageNum(UINT8 u1VdpId, UINT32 u4Width, UINT32 u4Height, UINT8 u1YNum, UINT8 u1CNum)
{
    UINT32 u4HPxlPerPg, u4VPxlPerPg, u4VMsbCPgNum;
    
    u4HPxlPerPg = 1 << (8);
    u4VPxlPerPg = 1 << (3);  

    _arMDDiPrm[u1VdpId].rPgNum.u4HMsbPgNum = MPY_AND_CEIL(u4Width, 1, u4HPxlPerPg);  
    _arMDDiPrm[u1VdpId].rPgNum.u4VMsbYPgNum = MPY_AND_CEIL((u4Height * u1YNum), 1, u4VPxlPerPg);    
    u4VMsbCPgNum = MPY_AND_CEIL((u4Height * u1CNum), 1, u4VPxlPerPg);
    _arMDDiPrm[u1VdpId].rPgNum.u4VMsbCPgNum = IS_420TO422(u1VdpId) ? MPY_AND_CEIL(u4VMsbCPgNum, 1, 2) : u4VMsbCPgNum;
}

/**
 * @brief Calculate Lsb page number
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Width
 * @param u2Height
 * @param u1YNum
 * @param u1CNum
 */
static void _vDrvDICalLsbPageNum(UINT8 u1VdpId, UINT32 u4Width, UINT32 u4Height, UINT8 u1YNum, UINT8 u1CNum)
{
    UINT32 u4HPxlPerPg = 256;
    UINT32 u4VPxlPerPg = 32;
    UINT32 u4VLsbCPgNum;
    UINT8 u18bit = (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0; 
    
    _arMDDiPrm[u1VdpId].rPgNum.u4HLsbPgNum = u18bit ? 0 : (MPY_AND_CEIL(u4Width, 1, u4HPxlPerPg));
    _arMDDiPrm[u1VdpId].rPgNum.u4VLsbYPgNum = u18bit ? 0 : (MPY_AND_CEIL((u4Height * u1YNum), 1, u4VPxlPerPg));
    u4VLsbCPgNum = u18bit ? 0 : (MPY_AND_CEIL((u4Height * u1CNum), 1, u4VPxlPerPg));

    _arMDDiPrm[u1VdpId].rPgNum.u4VLsbCPgNum =  IS_420TO422(u1VdpId) ? 
        MPY_AND_CEIL(u4VLsbCPgNum, 1, 2) : u4VLsbCPgNum;
}

/**
 * @brief Calculate buffer address 
 * @param u1VdpId VDP_1/VDP_2
 * @param u2Width
 * @param u2Height
 * @param u4StartAddr
 * @param u4EndAddr
 */
static void _vDrvDICalBufferAddress(UINT8 u1VdpId, UINT32 u4Width, UINT32 u4Height, 
        UINT32 u4StartAddr, UINT32 u4EndAddr)
{
    UINT16 u1AlignBit1, u1AlignBit2;
    UINT32 u4YAddrLsb, u4UAddrMsb, u4UAddrLsb, u4VAddrMsb, u4VAddrLsb;
    UINT32 u4FlgMVAddr, u4FlgLMAddr, u4FlgEndAddr;
    UINT32 u4FlgMVPerLine, u4FlgLMPerLine;
    UINT32 u4u4HMsbPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4HMsbPgNum;
    UINT32 u4u4HLsbPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4HLsbPgNum;
    UINT32 u4VMsbCPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4VMsbCPgNum;
    UINT32 u4VLsbCPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4VLsbCPgNum;

    // Calculate data address
    u4YAddrLsb = MAKE_ALIGN_TO((u4StartAddr + (u4u4HMsbPgNum * _arMDDiPrm[u1VdpId].rPgNum.u4VMsbYPgNum)), 3, ((u4StartAddr%8)+2)%8);
    u4UAddrMsb = MAKE_ALIGN_TO((u4YAddrLsb + (u4u4HLsbPgNum * _arMDDiPrm[u1VdpId].rPgNum.u4VLsbYPgNum)), 3, ((u4StartAddr%8)+4)%8);
    u4UAddrLsb = MAKE_ALIGN_TO((u4UAddrMsb + (u4u4HMsbPgNum * u4VMsbCPgNum)), 3, ((u4StartAddr%8)+6)%8);

    if ((bIsScalerInput444(u1VdpId) && (IS_PROG_DISPMODE(u1VdpId))))
    {
        u4VAddrMsb = MAKE_ALIGN_TO((u4UAddrLsb + (u4u4HLsbPgNum * u4VLsbCPgNum)), 3, ((u4StartAddr%8)+0)%8);
        u4VAddrLsb = MAKE_ALIGN_TO((u4VAddrMsb + (u4u4HMsbPgNum * u4VMsbCPgNum)), 3, ((u4StartAddr%8)+2)%8);
    }
    else
    {
        u4VAddrMsb = u4UAddrMsb;
        u4VAddrLsb = u4UAddrLsb;
    }

    // Calculate flag address
    u4FlgMVAddr = (u4VAddrLsb + (u4u4HLsbPgNum * u4VLsbCPgNum)) << 11;

    u1AlignBit1 = IS_OVERSAMPLE(u1VdpId) ? 512 : 256;
    u1AlignBit2 = IS_OVERSAMPLE(u1VdpId) ? 16 : 8;
    // Motion Vector Flag
    u4FlgMVPerLine = MPY_AND_CEIL(u4Width, 1, u1AlignBit1) * u1AlignBit1 / u1AlignBit2;
    u4FlgLMAddr = u4FlgMVAddr + MPY_AND_CEIL((u4FlgMVPerLine * u4Height * 16 / 8), 1, 16) * 16;        
    // Local Motion Flag
    if(IS_INTERLACE(VDP_1))
    {
        u4FlgLMPerLine = MPY_AND_CEIL(u4Width, 1, u1AlignBit1) * u1AlignBit1 / 8;                
    }
    else
    {
        u4FlgLMPerLine = 0;
    }        
    u4FlgEndAddr = u4FlgLMAddr + MPY_AND_CEIL((u4FlgLMPerLine * MPY_AND_CEIL(u4Height, 1, 4) * 16 / 8), 1, 16) * 16;                    

    if (u4FlgEndAddr > u4EndAddr)
    {
        u4FlgEndAddr = u4EndAddr;
    }
    if (u4FlgMVAddr > u4EndAddr)
    {
        u4FlgMVAddr = u4EndAddr;
    }
    
    vDrvDISetBufferProtect(u1VdpId, u4StartAddr, (u4FlgMVAddr >> 11), (u4FlgMVAddr >> 4), (u4FlgEndAddr >> 4));

    vDrvDISetBufferAddr(u1VdpId, u4StartAddr, u4YAddrLsb, u4UAddrMsb, u4UAddrLsb, 
            u4VAddrMsb, u4VAddrLsb, (u4FlgMVAddr >> 4), (u4FlgLMAddr >> 4));    
}

/**
 * @brief Set DI Buffer
 * @param prFbmPool fbm configure
 */
void vDrvDISetFrameBuf(UINT8 u1VdpId, const MDDI_FBM_POOL_T * prFbmPool)
{
    UINT8 u1YFldNum, u1CFldNum;
    UINT32 u4Width, u4Height;
    UINT32 u4StartAddr = prFbmPool->u4Addr >> 11;
    UINT32 u4EndAddr = (prFbmPool->u4Addr + prFbmPool->u4Size);

    //UINT32 u4IsFSInput=IS_FRAME_SEQ(u1VdpId)&&IS_INTERLACE(u1VdpId);

    PATH_CHECK(u1VdpId);
    
    u4Height = wDrvVideoInputHeight(u1VdpId);
    
    if(u4Height > _arMDDiPrm[u1VdpId].u2BufHeight)
    { // 1600 x 1200 Case , FBM use 1920 x 1080      
        u4Width = (_arMDDiPrm[u1VdpId].u2BufWidth + 255)/ 256 * 256 * _arMDDiPrm[u1VdpId].u2BufHeight/ u4Height;
        u4Width = u4Width / 256 * 256;
    }
    else
    {
        u4Width  = _arMDDiPrm[u1VdpId].u2BufWidth;
        u4Width = (u4Width / 256 + 1) * 256;        
        u4Height = _arMDDiPrm[u1VdpId].u2BufHeight;
    }

    u4Width=MAX(u4Width,_arMDDiPrm[u1VdpId].u2SigWidth);

    u4Height = IS_INTERLACE(u1VdpId) ? (u4Height>>1) :u4Height;
    u4Height = IS_420TO422(u1VdpId) ? (((u4Height+1)>>1)<<1) : u4Height;
    
    MDDI_WRITE_FLD(u1VdpId, MCVP_DRAM_0B,MIN(u4Width /16,127), DA_DRAM_LINE_LENGTH);     

    vDrvDISetBitRes(u1VdpId, (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0);
    _vDrvDISetFrameNum(u1VdpId);


    u1YFldNum = MDDI_READ_FLD(u1VdpId, MCVP_DRAM_0A, DA_FRAME_NUM_Y) + 1;
    u1CFldNum = MDDI_READ_FLD(u1VdpId, MCVP_DRAM_0A, DA_FRAME_NUM_U) + 1;
   
    _vDrvDICalMsbPageNum(u1VdpId, u4Width, u4Height, u1YFldNum, u1CFldNum);
    _vDrvDICalLsbPageNum(u1VdpId, u4Width, u4Height, u1YFldNum, u1CFldNum);
    _vDrvDICalBufferAddress(u1VdpId, u4Width, u4Height, u4StartAddr, u4EndAddr);
}


#define DRAM_ALIGN_WIDTH 256
#define DRAM_ALIGN_HEIGHT 8
EXTERN void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr);
EXTERN void HalInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);

void vDrvDIDumpDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height,UINT32 *pu4Data)
{
    UINT32 u4Addr = MDDI_READ_FLD(u1VdpId,MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y) << 11;
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_0B, DA_DRAM_LINE_LENGTH)*16;
    UINT32 u4FrmNum  = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_0A, DA_FRAME_NUM_Y)+1;

	UINT32 u4HPageMsb;
	UINT32 blkNumMsb, lineNumMsb, posMsb;
    UINT32 u4Tmp;
    UINT32 i,j;

    if(MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00, EN_NRW)==SV_OFF)
    {
        return;
    }

    width = width &(~0x3);
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;
    
    for(i = y; i < y+height ; i++)
    {
        u4Tmp = i * u4FrmNum / DRAM_ALIGN_HEIGHT * u4HPageMsb;
        lineNumMsb = i *   u4FrmNum % DRAM_ALIGN_HEIGHT;
        
        for(j = x ; j < x + width ; j+=4)
        {   
           blkNumMsb = u4Tmp + j /DRAM_ALIGN_WIDTH;
           posMsb = j % DRAM_ALIGN_WIDTH;            
            *pu4Data++ =  *(UINT32 *)VIRTUAL((u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT + lineNumMsb*DRAM_ALIGN_WIDTH + posMsb));
        }
    }

}

void vDrvDIFlushDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height)
{  
    UINT32 u4Addr = MDDI_READ_FLD(u1VdpId,MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y) << 11;
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_0B, DA_DRAM_LINE_LENGTH)*16;
    UINT32 u4FrmNum  = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_0A, DA_FRAME_NUM_Y)+1;
    
    UINT32 u4AddrTmp;
    UINT32 u4Step;
    UINT32 u4HPageMsb;
    UINT32 blkNumMsb;
    UINT32 u4Tmp;
    UINT32 i;

    if(MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00, EN_NRW)==SV_OFF)
    {
        return;
    }

    u4Step = u4FrmNum==1 ? 8 :
             u4FrmNum==2 ? 4 :
             u4FrmNum==3 ? 2 :
             u4FrmNum==4 ? 2 : 1;   
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;

    for(i = y&(~(DRAM_ALIGN_HEIGHT-1)); i < y+height ; i+=u4Step)
    {
        u4Tmp = i * u4FrmNum/ DRAM_ALIGN_HEIGHT * u4HPageMsb;
        blkNumMsb = u4Tmp + x/DRAM_ALIGN_WIDTH ;          
        u4AddrTmp = VIRTUAL(u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT);
        HalInvalidateDCacheMultipleLine(u4AddrTmp,DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT*2);
    }
}

////////////////////////////////////////////////////////////////////////////////
//DI DRAM Dump for NCSTool
////////////////////////////////////////////////////////////////////////////////

UINT32 DiReg06, DiReg07, DiReg12, DiReg16;

void vDrvDIDumpInit(UINT8 bPath)
{
    UINT32 u4SubOffset = (bPath == VDP_2) ? 0x800 : 0;
    UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
    UINT32 u4SrcWidth = MDDI_READ_FLD(bPath, MCVP_KC_0A, HDEW);
    UINT32 u4SrcHeight = MDDI_READ_FLD(bPath, MCVP_KC_0A, VDEW);
    UINT32 u4DumpSize = u4SrcWidth*u4SrcHeight*4;
    FBM_POOL_T* prPool;    
    
    if (bIsInterlace || (bPath == VDP_2)) // sub path always interlace, progresive will bypass Pscan
    {
        u4SrcHeight = u4SrcHeight/2;
    }

    LOG(4, "PSCAN Dump Start \n");    
    LOG(4, "W %d  H %d  Size %d \n", u4SrcWidth, u4SrcHeight, u4DumpSize);
    
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PQ_TOOL, NULL);
        
    if (u4DumpSize > prPool->u4Size)
    {
        LOG(0, "PSCAN Dump buffer too small  %d  %d\n", u4DumpSize, prPool->u4Size);
        vRegWriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
        vRegWriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
        return;
    }
    
    vRegWrite4B(SWRW_01, prPool->u4Addr);
    vRegWrite4B(SWRW_02, prPool->u4Size);
    vRegWriteFldAlign(SWRW_03, u4SrcWidth, DUMP_CTRL_WIDTH);
    vRegWriteFldAlign(SWRW_03, u4SrcHeight, DUMP_CTRL_HEIGHT);
    vRegWriteFldAlign(SWRW_03, DUMP_FMT_YCBCR_444_10BIT, DUMP_CTRL_FORMAT);    
    vRegWriteFldAlign(SWRW_04, u4DumpSize, DUMP_CTRL_SIZE); 
    
	DiReg12 = u4RegRead4B(MCVP_DRAM_12+u4SubOffset);
	DiReg06 = u4RegRead4B(MCVP_DRAM_0A+u4SubOffset);
    DiReg07 = u4RegRead4B(MCVP_DRAM_0B+u4SubOffset);
	DiReg16 = u4RegRead4B(MCVP_DRAM_18+u4SubOffset);	

	MDDI_WRITE_FLD(bPath, MCVP_DRAM_14, 0, DA_NR_SNAKE_MAPPING);
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_0A, 0, DA_SNAKE_MAPPING);
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_18, 0, DA_SC_SNAKE_MAPPING);    
    MDDI_WRITE_FLD(bPath, MCVP_DRAM_0B, u4SrcWidth>>4, DA_DRAM_LINE_LENGTH);
}

void vDrvDIDumpStart(UINT8 bPath)
{
    UINT8* Y_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_Y)<<11);
    UINT8* Y_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0B, DA_ADDR_BASE_LSB_Y)<<11);
    UINT8* U_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_U)<<11);
    UINT8* U_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_U)<<11);
    UINT8* V_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0E, DA_ADDR_BASE_MSB_V)<<11);
    //UINT8* V_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_V)<<11);
    UINT8 bYFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_FRAME_NUM_Y);
    UINT8 bCFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_FRAME_NUM_U);
    UINT8 bIs10bit = MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_BIT_SEL_WY) ? 0 : 1; 
    UINT8 bIs444 = MDDI_READ_FLD(bPath, MCVP_KC_1E, M444);     
	UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
    
	UINT32 u4SrcWidth = RegReadFldAlign(SWRW_03, DUMP_CTRL_WIDTH);
    UINT32 u4SrcHeight = RegReadFldAlign(SWRW_03, DUMP_CTRL_HEIGHT);
    UINT32* pBuf = (UINT32 *)VIRTUAL(u4RegRead4B(SWRW_01));
    
    UINT32 u4HPxlPerPgMsb = 1 << 8;
    UINT32 u4VPxlPerPgMsb = 1 << 3;  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT32 HPageMsb = (u4SrcWidth+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (u4SrcWidth+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb;
	UINT32 blkNumLsb, lineNumLsb, posLsb, Lsbshf;
    UINT32 Y0 = 0, Y1 = 0, Cb = 0, Cr = 0;
    UINT32 YMsb, YLsb, CMsb, CLsb;
    UINT32 R, G, B;  
    UINT32 x, y;

    vDrvDIFreezeOnOff(bPath, SV_ON);
    
    if ((bYFrmNum == 0) && bIsInterlace)
    {
        LOG(0, "PSCAN 0W0R mode");
        return;
    }
    else
    {
        bYFrmNum++;
        bCFrmNum++;
    }
    
    if (bIs444 == 0)
    {   
        for (y=0; y < u4SrcHeight; y++)
        {
            for (x=0; x < u4SrcWidth; x++)
            {
                blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
                lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;

                blkNumLsb = y*bYFrmNum/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
                lineNumLsb = y*bYFrmNum%u4VPxlPerPgLsb;
                
                posMsb = x%u4HPxlPerPgMsb;          
                posLsb = x%u4HPxlPerPgLsb;  
                Lsbshf = (x%4)*2;
                
                YMsb = (*(Y_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb))<<2;               
                CMsb = (*(U_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb))<<2;    
                
                if (bIs10bit)
                {
                    YLsb = ((*(Y_LSB_Addr+blkNumLsb*2048+lineNumLsb*u4HPxlPerPgLsb/4+posLsb/4))>>Lsbshf)&0x3; 
                    CLsb = ((*(U_LSB_Addr+blkNumLsb*2048+lineNumLsb*u4HPxlPerPgLsb/4+posLsb/4))>>Lsbshf)&0x3; 
                }
                else
                {                   
                    YLsb = 0;   
                    CLsb = 0;   
                }
                
                if ((x % 2) == 0)
                {
                    Y0 = YMsb+YLsb;
                    Cb = CMsb+CLsb; 
                }
                else
                {
                    Y1 = YMsb+YLsb;
                    Cr = CMsb+CLsb; 

                    pBuf[y*u4SrcWidth+x-1] = (Y0<<20) | (Cr<<10) | Cb;
                    pBuf[y*u4SrcWidth+x] = (Y1<<20) | (Cr<<10) | Cb; 
                }
            }
        }
        
        vRegWriteFldAlign(SWRW_03, 3, DUMP_CTRL_FORMAT);    
    }
    else
    {       
        for (y=0; y < u4SrcHeight; y++)
        {
            for (x=0; x < u4SrcWidth; x++)
            {
                blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
                lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;
                
                posMsb = x%u4HPxlPerPgMsb;          
                
                G = (*(Y_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));         
                B = (*(U_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));               
                R = (*(V_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));

                pBuf[y*u4SrcWidth+x] = (R<<20) | (G<<10) | B;
            }
        }
        
        vRegWriteFldAlign(SWRW_03, 1, DUMP_CTRL_FORMAT);    
    }    
}

void vDrvDIDumpEnd(UINT8 bPath)
{
    UINT32 u4SubOffset = (bPath == VDP_2) ? 0x800 : 0;
    
    vRegWrite4B(MCVP_DRAM_12+u4SubOffset, DiReg12);
	vRegWrite4B(MCVP_DRAM_0A+u4SubOffset, DiReg06);
    vRegWrite4B(MCVP_DRAM_0B+u4SubOffset, DiReg07);
	vRegWrite4B(MCVP_DRAM_18+u4SubOffset, DiReg16);

    vDrvDIFreezeOnOff(bPath, SV_OFF);
}




////////////////////////////////////////////////////////////////////////////////
//DI DRAM Dump for Customer utillity
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//Should Be integrate with NCSTOOL DRAM DUMP Later
////////////////////////////////////////////////////////////////////////////////


UINT8 bDiSnakeMapping = 0;
DI_DRAM_DUMP_INTO_T rDIDramDunpInfo;

UINT8 u1DrvDIInitDump(UINT8 u1VdpId)
{    
    
	UINT16 u2Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	


    bDiSnakeMapping = RegReadFldAlign(MCVP_DRAM_0A+u2Offset, DA_SNAKE_MAPPING);
    
    //1. disable snake mapping first;
	vRegWriteFldAlign(MCVP_DRAM_12+u2Offset, 0, DA_NR_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_0A+u2Offset, 0, DA_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_18+u2Offset, 0, DA_SC_SNAKE_MAPPING);
	
	vDrvDIGetDumpInfo(u1VdpId, &rDIDramDunpInfo);
    //Print Dram dump info
    LOG(5,"MSB Y Addr(0x%x)\n",rDIDramDunpInfo.u4MsbY);
    LOG(5,"LSB Y Addr(0x%x)\n",rDIDramDunpInfo.u4LsbY);
    LOG(5,"MSB U Addr(0x%x)\n",rDIDramDunpInfo.u4MsbU);
    LOG(5,"LSB U Addr(0x%x)\n",rDIDramDunpInfo.u4LsbU);
    LOG(5,"MSB V Addr(0x%x)\n",rDIDramDunpInfo.u4MsbV);
    LOG(5,"LSB V Addr(0x%x)\n",rDIDramDunpInfo.u4LsbV);
    
    LOG(5,"FB size(0x%x)\n",rDIDramDunpInfo.u4Size);
    LOG(5,"width(%d)\n",rDIDramDunpInfo.u4Width);
    LOG(5,"height(%d)\n",rDIDramDunpInfo.u4Height);
    LOG(5,"Y frame number(%d)\n",rDIDramDunpInfo.u1FrameNumY);
    LOG(5,"U frame number(%d)\n",rDIDramDunpInfo.u1FrameNumU);
    LOG(5,"is 10 bit(%d)\n",rDIDramDunpInfo.bIs10Bits);
    LOG(5,"is 444(%d)\n",rDIDramDunpInfo.bIs444);    

	return SV_TRUE;
}

UINT8 u1DrvDIReleaseDump(UINT8 u1VdpId)
{
	UINT16 u2Offset = (u1VdpId==VDP_1) ? 0x800 : 0;	

	//recover snake mapping
	vRegWriteFldAlign(MCVP_DRAM_12+u2Offset, bDiSnakeMapping, DA_NR_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_0A+u2Offset, bDiSnakeMapping, DA_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_18+u2Offset, bDiSnakeMapping, DA_SC_SNAKE_MAPPING);    

    return SV_TRUE;
}

void vDrvDIGetDumpInfo(UINT8 u1VdpId, DI_DRAM_DUMP_INTO_T* rDramDumpInfo)
{
	UINT8 u1Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	
    UINT32 u4LineLength;
	//1. base addr
	rDramDumpInfo->u4MsbY = (RegReadFldAlign(MCVP_DRAM_0A+u1Offset, DA_ADDR_BASE_MSB_Y) << 11);
	rDramDumpInfo->u4LsbY = (RegReadFldAlign(MCVP_DRAM_0B+u1Offset, DA_ADDR_BASE_LSB_Y) << 11);
	rDramDumpInfo->u4MsbU = (RegReadFldAlign(MCVP_DRAM_0A+u1Offset, DA_ADDR_BASE_MSB_U) << 11);
	rDramDumpInfo->u4LsbU = (RegReadFldAlign(MCVP_DRAM_0B+u1Offset, DA_ADDR_BASE_LSB_U) << 11);
	rDramDumpInfo->u4MsbV = (RegReadFldAlign(MCVP_DRAM_0E+u1Offset, DA_ADDR_BASE_MSB_V) << 11);
	rDramDumpInfo->u4LsbV = (RegReadFldAlign(MCVP_DRAM_0D+u1Offset, DA_ADDR_BASE_LSB_V) << 11);

    u4LineLength = RegReadFldAlign(MCVP_DRAM_0B+u1Offset, DA_DRAM_LINE_LENGTH);
    rDramDumpInfo->u4Width = u4LineLength << 4;
	//4. height
	if(RegReadFldAlign(MCVP_KC_00+u1Offset, I2P_EN)==1 || u1VdpId == SV_VP_PIP) 
    // sub path always interlace, progresive will bypass Pscan
	{
		rDramDumpInfo->u4Height = RegReadFldAlign(MCVP_KC_0A+u1Offset, VDEW)/2;
	}
	else
	{
		rDramDumpInfo->u4Height = RegReadFldAlign(MCVP_KC_0A+u1Offset, VDEW);
	}	
	//5. 10/8 bit
	rDramDumpInfo->bIs10Bits = RegReadFldAlign(MCVP_DRAM_0A+u1Offset, DA_BIT_SEL_WY) ? 0 : 1; // 0: 8bit / 1: 10bit
    //6. 444/422
	rDramDumpInfo->bIs444 = RegReadFldAlign(MCVP_KC_1E+u1Offset, M444);  
    //7. frame buffer size	
	rDramDumpInfo->u4Size = (RegReadFldAlign(MCVP_DRAM_0E+u1Offset, DA_WADDR_HI_LIMIT) << 11) - (RegReadFldAlign(MCVP_DRAM_0F+u1Offset, DA_WADDR_LO_LIMIT) << 11);	

    //8. block size
	rDramDumpInfo->u1BlockSize = 1;
    //9. Y Frame number
	rDramDumpInfo->u1FrameNumY = RegReadFldAlign(MCVP_DRAM_0A+u1Offset, DA_FRAME_NUM_Y);
    //10. U Frame number
	rDramDumpInfo->u1FrameNumU = RegReadFldAlign(MCVP_DRAM_0A+u1Offset, DA_FRAME_NUM_U);
}

void vDrvDIDumpDram(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height)
{
    UINT16 yIndex = 0;
    UINT16 xIndex = 0;
    DI_BUFFER_DATA value;

    HAL_TIME_T startTime;
    HAL_TIME_T endTime;
    HAL_TIME_T durationTime;
    
    HAL_GetTime(&startTime);
    
    for(yIndex = y; yIndex < (y+height); yIndex ++)
    {
        for(xIndex = x; xIndex < (x + width); xIndex++)
        {
            vDrvDIGetPixelValue(u1VdpId, xIndex, yIndex, &value);
            Printf("(%d,%d) = (%d,%d,%d)\n", xIndex,yIndex, value.u4YG, value.u4UB, value.u4VR);
        }
    }
    HAL_GetTime(&endTime);

    HAL_GetDeltaTime(&durationTime, &startTime, &endTime);
    Printf("Dump duration: %d.%d\n", durationTime.u4Seconds, durationTime.u4Micros);
    
}

void vDrvDIGetPixelValue(UINT8 u1VdpId, UINT16 x, UINT16 y, DI_BUFFER_DATA* value)
{   
	UINT8 u1Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	    
	UINT32 width = rDIDramDunpInfo.u4Width;
	UINT8 FrmNumY = 0;
    UINT32 u4HPxlPerPgMsb = 1 << 8;
    UINT32 u4VPxlPerPgMsb = 1 << 3;  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT8 bIsInterlace = RegReadFldAlign(MCVP_KC_00+u1Offset, I2P_EN);
	UINT32 HPageMsb = (width+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (width+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb;
	UINT32	blkNumLsb, lineNumLsb, posLsb, Lsbshf, pLsbM16;

    if(rDIDramDunpInfo.u4MsbY == 0)
    {
        return;
    }
    
	if ((rDIDramDunpInfo.u1FrameNumY == 0) && bIsInterlace) // 0W0R
	{
		Printf("PSCAN 0W0R \n");
	}
	else 
	{
		FrmNumY = rDIDramDunpInfo.u1FrameNumY + 1;
	}

	if (rDIDramDunpInfo.bIs444 == 0)    //422
	{			
		UINT32 Y0 = 0, Cb = 0; 
		UINT32 YMsb, YLsb, CMsb, CLsb;		
		
		blkNumMsb = y * FrmNumY / u4VPxlPerPgMsb * HPageMsb + x/u4HPxlPerPgMsb;
		lineNumMsb = y * FrmNumY % u4VPxlPerPgMsb;

		blkNumLsb = y*FrmNumY/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
		lineNumLsb = y*FrmNumY%u4VPxlPerPgLsb;

		//y offset
		posMsb = x%u4HPxlPerPgMsb;
		posLsb = x%u4HPxlPerPgLsb/4;
		pLsbM16 = posLsb % 16;
		posLsb = posLsb - pLsbM16 + (3 - pLsbM16/4)*4 + posLsb%4;
		Lsbshf = (x%4)*2;

		//read MSB
		YMsb = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb)))<<2;				
		CMsb = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb)))<<2;	

		//read LSB
		if (rDIDramDunpInfo.bIs10Bits)
		{
			YLsb = ((*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbY + blkNumLsb*2048 + lineNumLsb*u4HPxlPerPgLsb/4 + posLsb)))>>Lsbshf)&0x3;	
			CLsb = ((*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbU + blkNumLsb*2048 + lineNumLsb*u4HPxlPerPgLsb/4 + posLsb)))>>Lsbshf)&0x3;
		}
		else
		{					
			YLsb = 0;	
			CLsb = 0;	
		}

        Y0 = YMsb+YLsb;
        Cb = CMsb+CLsb;

        value->u4YG = Y0;
        value->u4UB= Cb;
        value->u4VR= 0;
	}
	else    //444
	{		
		UINT32 R, G, B;
		
		blkNumMsb = y*FrmNumY/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
		lineNumMsb = y*FrmNumY%u4VPxlPerPgMsb;
		
		posMsb = x%u4HPxlPerPgMsb;			
		
		G = *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb));			
		B = *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb));				
		R = *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbV + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb));

		value->u4YG= G;
		value->u4UB= B;
		value->u4VR= R;
	}
}


void vDrvDIWritePixelValue(UINT8 u1VdpId, UINT16 x, UINT16 y, DI_BUFFER_DATA* value)
{    
	UINT8 u1Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	    
	UINT32 width = rDIDramDunpInfo.u4Width;	
	UINT8 FrmNumY = 0;
    UINT32 u4HPxlPerPgMsb = 1 << 8;
    UINT32 u4VPxlPerPgMsb = 1 << 3;  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT8 bIsInterlace = RegReadFldAlign(MCVP_KC_00+u1Offset, I2P_EN);
    UINT16 u2I;
	UINT32 HPageMsb = (width+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (width+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb, TmpLineNum;
	UINT32	blkNumLsb, lineNumLsb, posLsb, Lsbshf, pLsbM16;

	if(rDIDramDunpInfo.u4MsbY == 0)
    {
        return;
    }
	    
	if ((rDIDramDunpInfo.u1FrameNumY == 0) && bIsInterlace) // 0W0R
	{
		Printf("PSCAN 0W0R \n");
	}
	else 
	{
		FrmNumY = rDIDramDunpInfo.u1FrameNumY + 1;
	}


	if (rDIDramDunpInfo.bIs444 == 0)    //422
	{			
		UINT32 Y0, Cb; 
		UINT32 YMsb, YLsb, CMsb, CLsb;		
	
		blkNumMsb = y * FrmNumY / u4VPxlPerPgMsb * HPageMsb + x/u4HPxlPerPgMsb;
		lineNumMsb = y * FrmNumY % u4VPxlPerPgMsb;

		blkNumLsb = y*FrmNumY/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
		lineNumLsb = y*FrmNumY%u4VPxlPerPgLsb;

		//y offset
		posMsb = x%u4HPxlPerPgMsb;
		posLsb = x%u4HPxlPerPgLsb/4;
		pLsbM16 = posLsb % 16;
		posLsb = posLsb - pLsbM16 + (3 - pLsbM16/4)*4 + posLsb%4;
		Lsbshf = (x%4)*2;

              Y0 = value->u4YG ;
              Cb = value->u4UB;
              
              YLsb = Y0 & 0x3;
              CLsb = Cb & 0x3;

              YMsb = Y0 >> 2;
              CMsb = Cb >> 2;
		//write MSB
		      for (u2I = 0; u2I < FrmNumY; u2I++)
		      {
		          TmpLineNum = (lineNumMsb + u2I) % u4VPxlPerPgMsb;
                  *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)YMsb;
                  *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)CMsb;
		      }
  
		//write LSB
		if (rDIDramDunpInfo.bIs10Bits)
		{
		    for (u2I = 0; u2I < FrmNumY; u2I++)
		    {
		        UINT32 u4Y,u4C,u4Clr = ~(0x3 << Lsbshf);
		        TmpLineNum = (lineNumLsb + u2I) % u4VPxlPerPgMsb;
			    u4Y = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbY + blkNumLsb*2048 + TmpLineNum*u4HPxlPerPgLsb/4 + posLsb)));	
			    u4C = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbU + blkNumLsb*2048 + TmpLineNum*u4HPxlPerPgLsb/4 + posLsb)));	
		
		   
                YLsb = (YLsb << Lsbshf);
                CLsb = (CLsb << Lsbshf);

                u4Y &= u4Clr;
                u4C &= u4Clr;
                u4Y |= YLsb;
                u4C |= CLsb;  

                *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbY + blkNumLsb*2048 + TmpLineNum * u4HPxlPerPgLsb/4 + posLsb)) = (UINT8)u4Y;
                *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbU + blkNumLsb*2048 + TmpLineNum * u4HPxlPerPgLsb/4 + posLsb)) = (UINT8)u4C;
		    }
		}

	}
	else    //444
	{		
		UINT32 R, G, B;
		
		blkNumMsb = y*FrmNumY/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
		lineNumMsb = y*FrmNumY%u4VPxlPerPgMsb;
		
		posMsb = x%u4HPxlPerPgMsb;			

		G = value->u4YG;
		B = value->u4UB;
		R = value->u4VR;

		for (u2I = 0; u2I < FrmNumY; u2I++)
		{
		    TmpLineNum = (lineNumMsb + u2I) % u4VPxlPerPgMsb;
            *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)G;
            *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)B;
            *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbV + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)R;
		}    
	}
}

