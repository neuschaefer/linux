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
 * $Author: muran.xiang $
 * $Date: 2012/08/22 $
 * $RCSfile: io_ice_filesystem.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_filesystem.c
 *  ICE filesystem driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "sv_const.h"
#include "cb_data.h"
#include "ice_debug.h"
#include "storage_if.h"
#include "drvcust_if.h"
#include "cb_data.h"
#include "mt53xx_cb.h"
#include "fbm_drvif.h"
#include "hw_di.h"
#include "hw_scpos.h"
#include <linux/mtd/mt53xx_part_oper.h>
#include "eeprom_if.h"
#include "nand_if.h"

extern UINT32 _u4BypassNptvMloop;
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


#ifndef PAGE_SIZE
#define PAGE_SIZE (4*1024)
#endif

#define COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,    \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error8\n");      \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error7\n");      \
        return MTR_ERR_INV;                                          \
    }  

#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_ci argument error6\n");     \
        return MTR_ERR_INV;                                          \
    } 


UINT32  BytesToPages(UINT32 size)
{
    UINT32 i = 0;
  //  UINT32 howmanypages = 0;
    
    for(i=0;((((UINT32)1)<<i)*PAGE_SIZE)<size; i ++)
    {
        
    }
    return (((UINT32)1)<<i);
    
}

UINT32  BytesToOrder(UINT32 size)
{
    UINT32 i = 0;
   // UINT32 howmanypages = 0;
    
    for(i=0;((((UINT32)1)<<i)*PAGE_SIZE)<size; i ++)
    {
        
    }
    return i;
    
}

void SetPagesReserved(unsigned long p, UINT32 howmanypages)
{
    UINT32 i = 0;
    
    for(i = 0; i<howmanypages; i++)
    {
        SetPageReserved(virt_to_page(p));
        p+=PAGE_SIZE;
    }
}

void ClearPagesReserved(unsigned long p, UINT32 howmanypages)
{
    UINT32 i = 0;
    
    for(i = 0; i<howmanypages; i++)
    {
        ClearPageReserved(virt_to_page(p));
        p+=PAGE_SIZE;
    }
}

/*
******************************************************
size: how many bytes you want to malloc from kernel;
pKernel: kernel space address to the memory that you malloc;
pUser: user space address to the memory that you malloc;
ret val: frost_true is success; frost_false is fail;
******************************************************
*/
int _ice_MMapMallocFromKernel(unsigned long arg)
{
    MTAL_IOCTL_3ARG_T rArg;
    unsigned long p = 0;
   // UINT32 i = 0;
    UINT32 size = 0;
    UINT32 howmanypages = 0;
    UINT32 order = 0;
    
    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);
    size = rArg.ai4Arg[0] ;
    howmanypages = BytesToPages(size);
    order = BytesToOrder(size);
    p = __get_free_pages(GFP_KERNEL, order);
    if(p == 0)
    {
        printf("free page alloc error____________________________________\n");
        rArg.ai4Arg[1] = 0;
        rArg.ai4Arg[2] = 0;//malloc result, 0 fail, 1 success
        COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_3ARG_T);
        return MTR_OK;
    }
    SetPagesReserved(p, howmanypages);
    printf("p = 0x%08x, order= %d, howmanypages = %d++++++++++++++++++++++++++++++++++\n", p, order, howmanypages);
    rArg.ai4Arg[1] = p;//kernel allocated memory addess;
    rArg.ai4Arg[2] = 1;//malloc result, 0 fail, 1 success
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_3ARG_T);
    return MTR_OK;
}

/*
******************************************************
size: how many bytes you want to free from kernel;
pKernel: kernel space address to the memory that you want to free;
ret val: frost_true is success; frost_false is fail;
******************************************************
*/
int _ice_MMapMallocFree(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8  * pKernel = NULL;
    UINT32 size = 0;
    UINT32 howmanypages = 0;
    UINT32 order = 0;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg); 

    size = rArg.ai4Arg[0];
    pKernel = (UINT8  *)rArg.ai4Arg[1];
    howmanypages = BytesToPages(size);
    order = BytesToOrder(size);
    
    ClearPagesReserved((unsigned long)pKernel, howmanypages);
    free_pages((unsigned long)pKernel, order); 
    
    return MTR_OK;
}

int _ice_UpgradeFileToNandPartition(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT8  * pKernel = NULL;
   // UINT32 i = 0;
    UINT32 size = 0;
    UINT32 FileType = 0;
    UINT32 dWhichPartition = 19;
    UINT32 u4Offset = 0;
    INT32 i4_ret = 0;
    
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    size = (UINT32)rArg.ai4Arg[0];
    pKernel = (UINT8  *)rArg.ai4Arg[1];
    FileType = (UINT32)rArg.ai4Arg[2];
    u4Offset = (UINT32)rArg.ai4Arg[3];
    switch(FileType)
    {
        case 0://pqf.bin
            dWhichPartition= DRVCUST_OptGet(eFlashPq2ndpartBasePartition);
            break;
        case 1://pcf.bin
            dWhichPartition= DRVCUST_OptGet(eFlashPqBasePartition);
            break;
        case 2://aq.bin
            dWhichPartition= DRVCUST_OptGet(eAudioFlashAQPartition);
            break;
        case 3://ptf.bin
            dWhichPartition= DRVCUST_PanelGet(ePanelFlashTablePartitionID);
            break;
        case 4://hdmiedid.bin
            dWhichPartition= DRVCUST_InitGet(eFlashHdmiEdidyPartitionID);
            break;
        case 5://vgaedid.bin
            dWhichPartition= DRVCUST_InitGet(eFlashVgaEdidPartitionID);
            break;
        case 6://ci.bin
            size = 131072;//for nand block size aligned;
            dWhichPartition= DRVCUST_OptGet(eFlashCIKeySetPartitionID);
            break;
        case 7://hdcpkey.bin
            size = 131072;//for nand block size aligned;
            dWhichPartition= DRVCUST_OptGet(eFlashHdcpKeyPartitionID);
            break;
#if 0           
        case 8://uImage
            dWhichPartition= 5;//fix me, change to customize.
            i4NFBPartitionWrite(dWhichPartition, u4Offset, pKernel, size);
            return MTR_OK;
        case 9://rootfs
            dWhichPartition= 7;//fix me, change to customize.
            i4NFBPartitionWrite(dWhichPartition, u4Offset, pKernel, size);
            return MTR_OK;
        case 10://basic
            dWhichPartition= 10;//fix me, change to customize.
            i4NFBPartitionWrite(dWhichPartition, u4Offset, pKernel, size);
            return MTR_OK;
#endif

        default:
            //check OAD download first.
            if (WRITE_PARTITION == (FileType&0xff00))
            {
                UINT32 u4AllPartitionNum = 0;
                
                dWhichPartition = FileType&0xff;

                i4_ret = DRVCUST_OptQuery(eNANDFlashPartEnd, &u4AllPartitionNum);
                if (-1 == i4_ret)
                {
                    printf("\n---Get eNANDFlashPartEnd Error!!! ---\n");
                    return MTR_NOT_OK;
                }

                if (dWhichPartition >= u4AllPartitionNum)
                {
                    printf("\n---Error, invalid partition number!!! ---\n");
                    return MTR_NOT_OK;
                }
 
               // i4_ret = i4NFBPartitionWrite(dWhichPartition, u4Offset, pKernel, size);
                if (0 == i4_ret)
                { 
                    printf("\n---Write partition complete ok!!! ret =  %d.---\n", i4_ret);
                    return MTR_OK;
                }
                else
                {
                    printf("\n---Write partition Error!!! ret = %d.---\n", i4_ret);
                    return MTR_NOT_OK;
                }
            }
            else
            {
                Printf("\n---Invalid FileType %d----.\n", FileType);
                return MTR_NOT_OK;
            }
            //not need. break;
            
    }
    Printf("dWhichPartition = %d, size = %d, pKernel = 0x%x, FileType = %d_________________________________\n", dWhichPartition, size, pKernel, FileType);
#if 0//test
    for(i = 0; i< size; i++)
    {
        printf("0x%x, 0x%x\n", *(pKernel + i), i);
    }
#endif
    STORG_SyncNandWriteBlock(dWhichPartition, 0, pKernel, size);
    return MTR_OK;
}

int _ice_ReadFileFromNandPartition(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT8  * pKernel = NULL;
  //  UINT32 i = 0;
    UINT32 size = 0;
    UINT8 FileType = 0;
    UINT32 dWhichPartition = 19;
    UINT32 u4Offset = 0;  
    
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    size = (UINT32)rArg.ai4Arg[0];
    pKernel = (UINT8  *)rArg.ai4Arg[1];
    FileType = (UINT8)rArg.ai4Arg[2];
    u4Offset = (UINT32)rArg.ai4Arg[3];
    switch(FileType)
    {
        case 0://pqf.bin
            dWhichPartition= DRVCUST_OptGet(eFlashPq2ndpartBasePartition);
            break;
        case 1://pcf.bin
            dWhichPartition= DRVCUST_OptGet(eFlashPqBasePartition);
            break;
        case 2://aq.bin
            dWhichPartition= DRVCUST_OptGet(eAudioFlashAQPartition);
            break;
        case 3://ptf.bin
            dWhichPartition= DRVCUST_PanelGet(ePanelFlashTablePartitionID);
            break;
        case 4://hdmiedid.bin
            dWhichPartition= DRVCUST_InitGet(eFlashHdmiEdidyPartitionID);
            break;
        case 5://vgaedid.bin
            dWhichPartition= DRVCUST_InitGet(eFlashVgaEdidPartitionID);
            break;
        case 6://ci.bin
            dWhichPartition= DRVCUST_OptGet(eFlashCIKeySetPartitionID);
            break;
        case 7://hdcpkey.bin
            dWhichPartition= DRVCUST_OptGet(eFlashHdcpKeyPartitionID);
            break;
        case 8://uImage
            dWhichPartition= 5;//fix me, change to customize.
        //    i4NFBPartitionRead(dWhichPartition, u4Offset, pKernel, size);
            return MTR_OK;
        case 9://rootfs
            dWhichPartition= 7;//fix me, change to customize.
          //  i4NFBPartitionRead(dWhichPartition, u4Offset, pKernel, size);
            return MTR_OK;
        case 10://basic
            dWhichPartition= 10;//fix me, change to customize.
          //  i4NFBPartitionRead(dWhichPartition, u4Offset, pKernel, size);
            return MTR_OK;

        default:
            return MTR_NOT_OK;
    }
    STORG_SyncNandRead(dWhichPartition, 0, pKernel, size);
#if 0//for test
    for(i = 0; i< size; i++)
    {
        *(pKernel+i)  = i;//for test
    }
#endif
    return MTR_OK;
}


int _ice_ReadProductionCredentialsSizeFromNand(unsigned long arg)
{
    UINT32 dProductionCredentialsSize = 0;
    UINT32 dWhichPartition = 19;
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pProductionCredentialsSize = (UINT8 *)arg;
    UINT8 buffer[2] = {0, 0};

    dWhichPartition= DRVCUST_OptGet(eFlashCIKeySetPartitionID);

    STORG_SyncNandRead(dWhichPartition, 0, (UINT8 *)&dProductionCredentialsSize, 2);
    buffer[0] = dProductionCredentialsSize&0xff;
    buffer[1] = (dProductionCredentialsSize>>8)&0xff;
    dProductionCredentialsSize = ((((UINT32)buffer[0])<<8)|buffer[1] );
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pProductionCredentialsSize, 4);
    COPY_TO_USER_ARG_K(pProductionCredentialsSize, dProductionCredentialsSize, 4);
    return i4Ret;
}

/*
if you want to send data from driver to user, you can use bUpdateDataToUser in driver layer;
user related function is ice_DrvCbHandler. please seprate it in the switch case through bDataType in ice_DrvCbHandler body.
there are three param for bUpdateDataToUser;
1. bData, a pointer to data buffer;
2. wDataSize, how many data to be send;
3. bDataType. Data type, 0: HDCP key upgrade; 1~0xff: future use.
*/
UINT8 bUpdateDataToUser(UINT8 * bData, UINT16 wDataSize, UINT8 bDataType)
{
    UINT16 i = 0;
    UINT8 * buffer;
    UINT32 size;
    size = wDataSize+3;
    buffer = (UINT8 *) x_mem_alloc (size);
    if(!buffer)
    {
        return 0;
    }
    *(buffer+0) = bDataType;
    *(buffer+1) = (UINT8 )(wDataSize & 0xff);
    *(buffer+2) = (UINT8 )((wDataSize>>8) & 0xff);

    for(i = 3; i<size; i++)
    {
        *(buffer+i) = *(bData+i-3);
    }
    if(CBR_OK != _CB_PutEvent(CB_DRV_NFY, size, (void*)buffer))
    {
        x_mem_free(buffer);
        buffer = NULL;
        return 0;
    }
    x_mem_free(buffer);
    buffer = NULL;
    return 1;
}

/*
used for factory mass production upgrade.
relaced function in user layer is frost_bool ice_UpdateHDCPKeyToFileSystem(UINT8 * bKey, UINT16 size)
*/
UINT8 _ice_UpdateHDCPKeyToFileSystem(UINT8 * bKey, UINT16 wDataSize)
{
    if(1 == bUpdateDataToUser(bKey, wDataSize, 0))
    {
        return 1;
    }
    else
    {
        printf("hdcpkey update fail!");
        return 0;
    }
}


UINT8 bIsFBMHooked = 0xff;//0xff means not enable; 0x1 means enable;
void _ice_HookFBM_EnableDisableScalerAndPscanWriteDram(UINT8 bEnable)
{
    if(bEnable == 0)
    {
    #if 0
        _u4BypassNptvMloop = 0x8;//disable scaler mainloop
        vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_W_EN_1);//disable scaler_main write dram
        vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_W_EN_2);//disable scaler_sub write dram
        vRegWriteFldAlign(MCVP_MULTI_00, 0, EN_SRCW);//disable pscan_main write dram
        vRegWriteFldAlign(MCVP_MULTI_00+0x800, 0, EN_SRCW);//disable pscan_sub write dram
        vRegWriteFldAlign(OUTSTG_OS_17, 1, OS_17_MUTE_MAIN);//mute main
        vRegWriteFldAlign(OUTSTG_OS_17, 1, OS_17_MUTE_PIP);//mute sub.
		#endif
		_u4BypassNptvMloop = 0x18;//disable scaler mainloop

        vRegWriteFldAlign(MCVP_MULTI_00, 0, EN_DRAM_RW);
		vRegWriteFldAlign(MCVP_MULTI_00+0x800, 0, EN_DRAM_RW);
		vScpipSetDramWrite(0, 0);
		vScpipSetDramWrite(1, 0);
		vScpipSetDramRead(0, 0);
		vScpipSetDramRead(1, 0);
		vRegWriteFldAlign(MUTE_00, 1, R_M_MUTE_FRONT_EN);
		vRegWriteFldAlign(MUTE_00, 1, R_P_MUTE_FRONT_EN);
        bIsFBMHooked = 1;
    }
    else
    {
    
#if 0
        _u4BypassNptvMloop = 0x0;//disable scaler mainloop
        vRegWriteFldAlign(MDDI_MULTI_00, 1, EN_SRCW);//disable pscan_main write dram
        vRegWriteFldAlign(MDDI_MULTI_00+0x800, 1, EN_SRCW);//disable pscan_sub write dram
        vRegWriteFldAlign(OUTSTG_OS_17, 0, OS_17_MUTE_MAIN);//mute main
        vRegWriteFldAlign(OUTSTG_OS_17, 0, OS_17_MUTE_PIP);//mute sub.
		
#endif
		_u4BypassNptvMloop = 0x0;//enable scaler mainloop

        vRegWriteFldAlign(MCVP_MULTI_00, 1, EN_DRAM_RW);
		vRegWriteFldAlign(MCVP_MULTI_00+0x800, 1, EN_DRAM_RW);
		vScpipSetDramWrite(0, 1);
		vScpipSetDramWrite(1, 1);
		vScpipSetDramRead(0, 1);
		vScpipSetDramRead(1, 1);
		vRegWriteFldAlign(MUTE_00, 1, R_M_MUTE_FRONT_EN);
		vRegWriteFldAlign(MUTE_00, 1, R_P_MUTE_FRONT_EN);

        bIsFBMHooked = 0xff;  
    }
}

int _ice_HookFBM(unsigned long arg)
{
    FBM_POOL_T* prPool_START;
    FBM_POOL_T* prPool_END;
    MTAL_IOCTL_2ARG_T rArg;
    
    _ice_HookFBM_EnableDisableScalerAndPscanWriteDram(0);

    prPool_START = FBM_GetPoolInfo(FBM_POOL_TYPE_SCPOS_MAIN);
//    prPool_END = FBM_GetPoolInfo(FBM_POOL_TYPE_DSP);
    
//    rArg.ai4Arg[0] = prPool_START->u4Addr + prPool_START->u4Size;//start from the end of MPEG
    rArg.ai4Arg[0] = prPool_START->u4Addr;//prPool_START->u4Addr + prPool_START->u4Size - 0xa00000;
    rArg.ai4Arg[1] = prPool_START->u4Size;//prPool_END->u4Addr - rArg.ai4Arg[0];
//end at the beginning of DSP.
    printf("rArg.ai4Arg[0]= 0x%x++++++++++++++++++++++++++++++++\n", rArg.ai4Arg[0]);
    printf("rArg.ai4Arg[1]= 0x%x++++++++++++++++++++++++++++++++\n", rArg.ai4Arg[1]);
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_2ARG_T);

    return MTR_OK;
}

int _ice_ReleaseFBMHook(unsigned long arg)
{
    _ice_HookFBM_EnableDisableScalerAndPscanWriteDram(1);
    return MTR_OK;
}

static MT_RESULT_T _ICE_FileSystem_GetOADPartition(unsigned long arg)
{
    UINT32 *pu4KernelPartitionMain = NULL;
    UINT32 *pu4KernelPartitionSlave = NULL;
    UINT32 *pu4RootfsPartitionMain = NULL;
    UINT32 *pu4RootfsPartitionSlave = NULL;
    UINT32 *pu4BasicPartitionMain = NULL;
    UINT32 *pu4BasicPartitionSlave = NULL;
    UINT32 u4KernelPartitionMain = 0;
    UINT32 u4KernelPartitionSlave = 0;
    UINT32 u4RootfsPartitionMain = 0;
    UINT32 u4RootfsPartitionSlave = 0;
    UINT32 u4BasicPartitionMain = 0;
    UINT32 u4BasicPartitionSlave = 0;
    UINT32 u4TmpData = 0;
    MTAL_IOCTL_6ARG_T rArg ;
    DTVCFG_T rDtvCfg ;
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_6ARG(arg);
    COPY_FROM_USER_6ARG(arg, rArg);

    pu4KernelPartitionMain = (UINT32*)rArg.ai4Arg[0];
    pu4KernelPartitionSlave = (UINT32*)rArg.ai4Arg[1];
    pu4RootfsPartitionMain = (UINT32*)rArg.ai4Arg[2];
    pu4RootfsPartitionSlave = (UINT32*)rArg.ai4Arg[3];
    pu4BasicPartitionMain = (UINT32*)rArg.ai4Arg[4];
    pu4BasicPartitionSlave = (UINT32*)rArg.ai4Arg[5];

    i4Ret = DRVCUST_OptQuery(eKernelAPartNum, &u4KernelPartitionMain);
    if (-1 == i4Ret)
    {
        printf("\n---Get eKernelAPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eKernelBPartNum, &u4KernelPartitionSlave);
    if (-1 == i4Ret)
    {
        printf("\n---Get eKernelBPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eRootfsAPartNum, &u4RootfsPartitionMain);
    if (-1 == i4Ret)
    {
        printf("\n---Get eRootfsAPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eRootfsBPartNum, &u4RootfsPartitionSlave);
    if (-1 == i4Ret)
    {
        printf("\n---Get eRootfsPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eBasicAPartNum, &u4BasicPartitionMain);
    if (-1 == i4Ret)
    {
        printf("\n---Get eBasicAPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eBasicBPartNum, &u4BasicPartitionSlave);
    if (-1 == i4Ret)
    {
        printf("\n---Get eBasicBPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }    

    i4Ret  = EEPDTV_GetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---GetOADPartition. Get EEPROM partition settings Error!!! ---\n");
        return MTR_NOT_OK;
    }        

    if (DTVCFG_FLAG3_KERNEL_BANK == (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_KERNEL_BANK))
    {
        u4TmpData = u4KernelPartitionMain;
        u4KernelPartitionMain = u4KernelPartitionSlave;
        u4KernelPartitionSlave = u4TmpData;
    }

    if (DTVCFG_FLAG3_ROOTFS_BANK == (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_ROOTFS_BANK))
    {
        u4TmpData = u4RootfsPartitionMain;
        u4RootfsPartitionMain = u4RootfsPartitionSlave;
        u4RootfsPartitionSlave = u4TmpData;
    }
        
    if (DTVCFG_FLAG3_BASIC_BANK == (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_BASIC_BANK))
    {
        u4TmpData = u4BasicPartitionMain;
        u4BasicPartitionMain = u4BasicPartitionSlave;
        u4BasicPartitionSlave = u4TmpData;
    }
                
    USR_SPACE_ACCESS_VALIDATE_ARG(pu4KernelPartitionMain, UINT32);
    COPY_TO_USER_ARG(pu4KernelPartitionMain, u4KernelPartitionMain, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4KernelPartitionSlave, UINT32);
    COPY_TO_USER_ARG(pu4KernelPartitionSlave, u4KernelPartitionSlave, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4RootfsPartitionMain, UINT32);
    COPY_TO_USER_ARG(pu4RootfsPartitionMain, u4RootfsPartitionMain, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4RootfsPartitionSlave, UINT32);
    COPY_TO_USER_ARG(pu4RootfsPartitionSlave, u4RootfsPartitionSlave, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4BasicPartitionMain, UINT32);
    COPY_TO_USER_ARG(pu4BasicPartitionMain, u4BasicPartitionMain, UINT32);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4BasicPartitionSlave, UINT32);
    COPY_TO_USER_ARG(pu4BasicPartitionSlave, u4BasicPartitionSlave, UINT32);

    return MTR_OK;
}

static MT_RESULT_T _ICE_FileSystem_SetOADKernelPartition(unsigned long arg)
{
    UINT32 u4KernelPartitionMain = 0;
    UINT32 u4KernelPartitionSlave = 0;
    UINT32 u4SetPartitionMain = 0;
    UINT32 u4SetPartitionSlave = 0;    
    MTAL_IOCTL_2ARG_T rArg ;
    DTVCFG_T rDtvCfg;
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4SetPartitionMain = (UINT32)rArg.ai4Arg[0];
    u4SetPartitionSlave = (UINT32)rArg.ai4Arg[1];

    i4Ret = DRVCUST_OptQuery(eKernelAPartNum, &u4KernelPartitionMain);
    if (-1 == i4Ret)
    {
        printf("\n---Get eKernelAPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eKernelBPartNum, &u4KernelPartitionSlave);
    if (-1 == i4Ret)
    {
        printf("\n---Get eKernelBPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_GetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Set OAD Kernel Parition. Get EEPROM partition settings Error!!! ---\n");
        return MTR_NOT_OK;
    }        

    if ((u4KernelPartitionMain == u4SetPartitionMain) && (u4KernelPartitionSlave == u4SetPartitionSlave))
    {
        //to reduce eeprom write times.
        if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_KERNEL_BANK)
        {
            rDtvCfg.u1Flags3 &= ~((UINT8)DTVCFG_FLAG3_KERNEL_BANK);
        }
        else
        {
            //the same as current settings, return directly!
            return MTR_OK;
        }
    }
    else if ((u4KernelPartitionMain == u4SetPartitionSlave) && (u4KernelPartitionSlave == u4SetPartitionMain))
    {
        //to reduce eeprom write times.
        if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_KERNEL_BANK)
        {
            //the same as current settings, return directly!
            return MTR_OK;

        }
        else
        {
            rDtvCfg.u1Flags3 |= ((UINT8)DTVCFG_FLAG3_KERNEL_BANK);
        }
    }
    else
    {
        //do nothing.
        printf("\n---Set OAD Kernel Parition. Parition number don't match!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_SetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Set OAD Kernel Parition. Set EEPROM partition Error!!! ---\n");
        return MTR_NOT_OK;
    }        
    
    return MTR_OK;
}

static MT_RESULT_T _ICE_FileSystem_SetOADRootfsPartition(unsigned long arg)
{
    UINT32 u4RootfsPartitionMain = 0;
    UINT32 u4RootfsPartitionSlave = 0;
    UINT32 u4SetPartitionMain = 0;
    UINT32 u4SetPartitionSlave = 0;    
    MTAL_IOCTL_2ARG_T rArg ;
    DTVCFG_T rDtvCfg ;
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4SetPartitionMain = (UINT32)rArg.ai4Arg[0];
    u4SetPartitionSlave = (UINT32)rArg.ai4Arg[1];

    i4Ret = DRVCUST_OptQuery(eRootfsAPartNum, &u4RootfsPartitionMain);
    if (-1 == i4Ret)
    {
        printf("\n---Get eRootfsAPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eRootfsBPartNum, &u4RootfsPartitionSlave);
    if (-1 == i4Ret)
    {
        printf("\n---Get eRootfsBPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_GetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Set OAD rootfs Parition. Get EEPROM partition settings Error!!! ---\n");
        return MTR_NOT_OK;
    }        

    if ((u4RootfsPartitionMain == u4SetPartitionMain) && (u4RootfsPartitionSlave == u4SetPartitionSlave))
    {
        //to reduce eeprom write times.
        if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_ROOTFS_BANK)
        {
            rDtvCfg.u1Flags3 &= ~((UINT8)DTVCFG_FLAG3_ROOTFS_BANK);
        }
        else
        {
            //the same as current settings, return directly!
            return MTR_OK;
        }
    }
    else if ((u4RootfsPartitionMain == u4SetPartitionSlave) && (u4RootfsPartitionSlave == u4SetPartitionMain))
    {
        //to reduce eeprom write times.
        if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_ROOTFS_BANK)
        {
            //the same as current settings, return directly!
            return MTR_OK;
        }
        else
        {
            rDtvCfg.u1Flags3 |= ((UINT8)DTVCFG_FLAG3_ROOTFS_BANK);
        }
    }
    else
    {
        //do nothing.
        printf("\n---Set OAD rootfs Parition. Parition number don't match!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_SetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Set OAD rootfs Parition. Set EEPROM partition Error!!! ---\n");
        return MTR_NOT_OK;
    }        
    
    return MTR_OK;
}

static MT_RESULT_T _ICE_FileSystem_SetOADBasicPartition(unsigned long arg)
{
    UINT32 u4BasicPartitionMain = 0;
    UINT32 u4BasicPartitionSlave = 0;
    UINT32 u4SetPartitionMain = 0;
    UINT32 u4SetPartitionSlave = 0;    
    MTAL_IOCTL_2ARG_T rArg ;
    DTVCFG_T rDtvCfg ;
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    u4SetPartitionMain = (UINT32)rArg.ai4Arg[0];
    u4SetPartitionSlave = (UINT32)rArg.ai4Arg[1];

    i4Ret = DRVCUST_OptQuery(eBasicAPartNum, &u4BasicPartitionMain);
    if (-1 == i4Ret)
    {
        printf("\n---Get eBasicAPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret = DRVCUST_OptQuery(eBasicBPartNum, &u4BasicPartitionSlave);
    if (-1 == i4Ret)
    {
        printf("\n---Get eBasicBPartNum Error!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_GetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Set OAD basic Parition. Get EEPROM partition settings Error!!! ---\n");
        return MTR_NOT_OK;
    }        

    if ((u4BasicPartitionMain == u4SetPartitionMain) && (u4BasicPartitionSlave == u4SetPartitionSlave))
    {
        //to reduce eeprom write times.
        if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_BASIC_BANK)
        {
            rDtvCfg.u1Flags3 &= ~((UINT8)DTVCFG_FLAG3_BASIC_BANK);
        }
        else
        {
            //the same as current settings, return directly!
            return MTR_OK;
        }
    }
    else if ((u4BasicPartitionMain == u4SetPartitionSlave) && (u4BasicPartitionSlave == u4SetPartitionMain))
    {
        //to reduce eeprom write times.
        if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_BASIC_BANK)
        {
            //the same as current settings, return directly!
            return MTR_OK;
        }
        else
        {
            rDtvCfg.u1Flags3 |= ((UINT8)DTVCFG_FLAG3_BASIC_BANK);
        }
    }
    else
    {
        //do nothing.
        printf("\n---Set OAD basic Parition. Parition number don't match!!! ---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_SetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Set OAD basic Parition. Set EEPROM partition Error!!! ---\n");
        return MTR_NOT_OK;
    }        
    
    return MTR_OK;
}

static MT_RESULT_T _ICE_FileSystem_GetBasicEEPSetting(unsigned long arg)
{
    UINT32  *p4Arg = NULL;
    UINT32 u4BasicToggled = FALSE;
    DTVCFG_T rDtvCfg ;    
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_1ARG(arg); 
    
    p4Arg = (UINT32 *)arg;

    if (NULL == p4Arg)
    {
        printf("\n---Error!! -Get invaild arg from user.!!! _ICE_FileSystem_GetBasicEEPSetting().---\n");
        return MTR_NOT_OK;
    }

    i4Ret  = EEPDTV_GetCfg(&rDtvCfg);
    if (i4Ret != 0)
    {
        printf("\n---Get OAD basic EEP Settings. Get EEPROM partition settings Error!!! ---\n");
        return MTR_NOT_OK;
    }        
    
    if (rDtvCfg.u1Flags3 & DTVCFG_FLAG3_BASIC_BANK)
    {
        u4BasicToggled = 1;//true
    }
    else
    {
        u4BasicToggled = 0;//false
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(p4Arg,UINT32);
    COPY_TO_USER_ARG(p4Arg, u4BasicToggled, UINT32);

    return MTR_OK;
}

static MT_RESULT_T _ICE_FileSystem_ErasePartition(unsigned long arg)
{
    INT32 i4Ret = 0;
    UINT32 u4AllPartitionNum = 0xff;
    UINT32 u1PartitionNumber = 0xff;
    UINT32 Arg = 0;

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, UINT32);
    COPY_FROM_USER_1ARG(arg, Arg);

    u1PartitionNumber = (UINT8)Arg;

    i4Ret = DRVCUST_OptQuery(eNANDFlashPartEnd, &u4AllPartitionNum);
    if (-1 == i4Ret)
    {
        printf("\n---Get eNANDFlashPartEnd Error!!! ---\n");
        return MTR_NOT_OK;
    }

    if (u1PartitionNumber >= u4AllPartitionNum)
    {
        printf("\n---Error, invalid partition number!!!u4PartitionNumber=%d, u4AllPartitionNum=%d. ---\n", u1PartitionNumber, u4AllPartitionNum);
        return MTR_NOT_OK;
    }

    printf("\n---Begin erase partition :%d. ---\n", u1PartitionNumber);
    i4Ret = NANDPART_Erase(u1PartitionNumber);
    printf("\n---End erase partition :%d. ret = %d---\n", u1PartitionNumber, i4Ret);
    if (0 == i4Ret)
    {
        return MTR_OK;
    }
    else
    {
        return MTR_NOT_OK;
    } 
        
}

static MT_RESULT_T _ICE_OADSavePkgAttributes(unsigned long arg)
{
    UINT32 buffer[2];
    UINT32 * pBuffer = (UINT32 *)arg;
    
    UINT32 * pOADPkgPhyAddress = (UINT8 *)arg;
    UINT32 u4OADPkgPhyAddress = 0;
    int i;

    UINT32 u4OADPkgPhyAddressOffset = 0;
    UINT32 u4OADPkgPhyAddressOld = 0;

    USR_SPACE_ACCESS_VALIDATE_ARG_K(pOADPkgPhyAddress, 4);
    COPY_FROM_USER_ARG_K(pOADPkgPhyAddress, u4OADPkgPhyAddress, 4);

    i = DRVCUST_InitQuery(eOADPkgPhyAddressOffset, (UINT32 *)(void *)&u4OADPkgPhyAddressOffset);
    if(1 ==EEPROM_Read(u4OADPkgPhyAddressOffset, (UINT32)&u4OADPkgPhyAddressOld, 4))
    {
        printf("_ICE_OADSavePkgAttributes: eep error#1\n");
        return MTR_NOT_OK; 
    }
    if(u4OADPkgPhyAddressOld != u4OADPkgPhyAddress)
    {
        if(1 ==EEPROM_Write(u4OADPkgPhyAddressOffset, (UINT32)&u4OADPkgPhyAddress, 4))
        {
            printf("_ICE_OADSavePkgAttributes: eep error#2\n");
            return MTR_NOT_OK; 
        } 
    }
    return MTR_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_filesystem(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
            case ICE_IO_MMAPMALLOCFROMKERNEL:
                    i4Ret = _ice_MMapMallocFromKernel(arg);
                    break;
            case ICE_IO_MMAPMALLOCFREE:
                    i4Ret = _ice_MMapMallocFree(arg);
                    break;
            case ICE_IO_UPGRADE_PROFILE_TO_NAND:
                    i4Ret = _ice_UpgradeFileToNandPartition(arg);
                    break;
            case ICE_IO_READ_PROFILE_FROM_NAND:
                    i4Ret = _ice_ReadFileFromNandPartition(arg);
                    break;
            case ICE_IO_READ_PRODUCTION_CREDENTIALS_SIZE_FROM_NAND:
                    i4Ret = _ice_ReadProductionCredentialsSizeFromNand(arg);
                    break;
            case ICE_IO_HOOKFBM:
                i4Ret = _ice_HookFBM(arg);
                break;
            case ICE_IO_RELEASEFBMHOOK:
                i4Ret = _ice_ReleaseFBMHook(arg);
                break;
            case ICE_IO_FILESYSTEM_GET_OAD_PARTITION:
                i4Ret = _ICE_FileSystem_GetOADPartition(arg);
                break;
            case ICE_IO_FILESYSTEM_SET_OAD_KERNEL_PARTITION:
                i4Ret = _ICE_FileSystem_SetOADKernelPartition(arg);
                break;
            case ICE_IO_FILESYSTEM_SET_OAD_ROOTFS_PARTITION:
                i4Ret = _ICE_FileSystem_SetOADRootfsPartition(arg);
                break;
            case ICE_IO_FILESYSTEM_SET_OAD_BASIC_PARTITION:
                i4Ret = _ICE_FileSystem_SetOADBasicPartition(arg);
                break;
            case ICE_IO_FILESYSTEM_GET_BASIC_EEP_SETTING:
                i4Ret = _ICE_FileSystem_GetBasicEEPSetting(arg);
                break;
            case ICE_IO_FILESYSTEM_ERASE_PARTITION:
                i4Ret = _ICE_FileSystem_ErasePartition(arg); 
                break;
            case ICE_IO_FILESYSTEM_OAD_SAVE_PKG_ATRRIBUTES:
                i4Ret = _ICE_OADSavePkgAttributes(arg); 
                break;
            default:
                    break;
    }
    return i4Ret;
}
