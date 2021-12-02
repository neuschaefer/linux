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
 * Description:
 *         The main routine of boot loader
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"
#include "x_typedef.h"
#include "x_hal_5381.h"
#include "loader_imghdr.h"
#include "loader_if.h"
#include "eeprom_if.h"
#include "c_model.h"

#ifdef CC_NAND_BOOT
#include "nand_if.h"
#endif

#ifdef CC_EMMC_BOOT
#include "msdc_if.h"
#endif

#include "zip.h"
#include "x_ckgen.h"
#include "x_bim.h"

#ifdef CC_LZMA_DECODE
#include "../lzma/LzmaEnc.h"
#include "../lzma/LzmaLib.h"
#endif

#ifdef CC_LOADER_JFFS2_SUPPORT
#include "jffs2.h"
#endif

//#define ENABLE_DUALREAD

#if (defined(ENABLE_DUALREAD))
static UINT8 _u1DualReadCmd = 0x3B;
#define SFLASH_PRGDATA3_REG     ((UINT32)0x3C)
#define SFLASH_DUAL_REG         ((UINT32)0xC0)
#endif
extern _pucFileBuf ;
extern u4UsbKernelSize;
#define FIRMWARE_FLASH_OFFSET           LOADER_MAX_SIZE

#ifdef CC_LOAD_KERNEL
#define htol(A) ( (((UINT32)(A) & 0xff000000 ) >> 24) |  \
			    	(((UINT32)(A) & 0x00ff0000 ) >> 8) | \
					(((UINT32)(A) & 0x0000ff00 ) << 8) | \
					(((UINT32)(A) & 0x000000ff ) << 24))
					
char rootDev[512] ="root=/dev/mtdblock2 rootwait lpj=1814528 console=ttyMT0,115200n1 quiet mtdparts=";
char upgradeStr[]=" upgrade_vestel_files";
UINT8 _UsbKernelEN=0;
UINT8 _BootNormal =0;
UINT8 _TryUpgradeAutoPkg=0;

void	(*theKernel)(UINT32 zero, UINT32 arch,	UINT32 kernel_entry);
#define ATAG_CORE  0x54410001
#define ATAG_MEM   0x54410002
#define ATAG_CMDLINE   0x54410009
#define ATAG_STEP	0x1
#define MACHINE_ID	5389
#define default_environment_size 200

typedef struct image_header {
	UINT32	ih_magic;	/* Image Header Magic Number	*/
	UINT32	ih_hcrc;	/* Image Header CRC Checksum	*/
	UINT32	ih_time;	/* Image Creation Timestamp	*/
	UINT32	ih_size;	/* Image Data Size		*/
	UINT32	ih_load;	/* Data	 Load  Address		*/
	UINT32	ih_ep;		/* Entry Point Address		*/
	UINT32	ih_dcrc;	/* Image Data CRC Checksum	*/
	UINT8   ih_os;		/* Operating System		*/
	UINT8		ih_arch;	/* CPU architecture		*/
	UINT8		ih_type;	/* Image Type			*/
	UINT8		ih_comp;	/* Compression Type		*/
	UINT8		ih_name[32];	/* Image Name		*/
} image_header_t;
image_header_t *hdr;

#endif


#if 0
#define HalInstructionMemoryBarrier()
#define HalDisableCaches()
#define HalDisableMMU()
#define Printf(...)
#define SerWaitTxBufClear()
#define SerEnd()
#endif

#if defined(CC_NAND_BOOT)
#define CHIP_FlashCopyToDRAMLZHS(u4PartId, u4Dest, u4Offset, u4Len)  Loader_ReadNandFlash(u4PartId, u4Offset, (void *)(u4Dest), u4Len)
#elif defined(CC_EMMC_BOOT)
#define CHIP_FlashCopyToDRAMLZHS(u4PartId, u4Dest, u4Offset, u4Len)  Loader_ReadMsdc(u4PartId, u4Offset, (void *)(u4Dest), u4Len)
#else
#define CHIP_FlashCopyToDRAMLZHS(u4PartId, u4Dest, u4Offset, u4Len)
#endif

static UINT32 _u4DualOffset = 0;
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT)
static UINT32 _u415MBImageAddr = 0;
#define MAX_COMPRESS_IMAGE_SIZE 0x1000000
#endif

#ifdef CC_LOADER_JFFS2_SUPPORT
JFFS2_ROOT_T *pJffs2RwRoot = 0;

void JFFS2_MountRwPartition()
{
    if (!pJffs2RwRoot)
        JFFS2_Mount(&pJffs2RwRoot, 65536, (NOR_FLASH_RW_SIZ)/65536, (void*)(NOR_PART_OFFSET_4+PBI_A_BASE));
}
#endif


#ifdef CC_LOAD_KERNEL
static int DRVCUST_QueryPart(int i, int *pData, int type)
{
    UINT32 u4Data = 0;
    
    if (type == 0)
    {
        u4Data = DRVCUST_InitGet(eNorFlashPartEnd);
    }
    else if(type == 1)
    {
        u4Data = DRVCUST_InitGet((QUERY_TYPE_T)(eNorFlashPartSize0 + i));
    }
    else if(type == 2)
    {
        u4Data = DRVCUST_InitGet((QUERY_TYPE_T)(eNorFlashPartName0 + i));
    }
    else if(type == 3)
    {
#if 1//def CC_MTD_ENCRYPT_SUPPORT
        u4Data = DRVCUST_InitGet((QUERY_TYPE_T)(eNorFlashPartEncrypt0 + i));
#else
        u4Data = 0;
#endif
    }
    else
    {
        ASSERT(0);
        return -1;
    }

    *pData = u4Data;
    return 0;
}

static void set_cust_env(void)
{
    int i = 0, size = 0, i4PartNum, encrypt = 0;
    char *p1 = NULL, *p2 = NULL;
    char *name = NULL;
    char buf[64] = {0};
    char *mtdparts = NULL;

    mtdparts = (char *)x_mem_alloc(default_environment_size);
    x_memset(mtdparts, 0, default_environment_size);

    DRVCUST_QueryPart(0, &i4PartNum, 0);

    // generate mtdparts string
    while (i < i4PartNum)
    {
        DRVCUST_QueryPart(i, &size, 1);
        DRVCUST_QueryPart(i, (int *)&name, 2);
        DRVCUST_QueryPart(i, &encrypt, 3);

        if (!size)
        {
            break;
        }

        if (i == 0)
        {
#if defined(CC_EMMC_BOOT)
            x_strcat(mtdparts, "mt53xx-emmc:");
#else
            x_strcat(mtdparts, "mt53xx-nor:");
#endif
        }
        else
        {
            x_strcat(mtdparts, ",");
        }

        if ((size % (1024 * 1024)) == 0)
        {
            sprintf(buf, "%dM(%s)", size /1024 /1024, name);
        }
        else if ((size % 1024) == 0)
        {
            sprintf(buf, "%dk(%s)", size /1024, name);
        }
        else
        {
           
		   Printf("\n not alligemnt in 1k or  1M error  \n");
		   ASSERT(0);
        }

        x_strcat(mtdparts, buf);

        i++;
    }

    x_strcat(rootDev, mtdparts);
	
    printf (" Environment is  %s\n", rootDev);
    
}


UINT8 * bEEPromBuf = NULL;
UINT32 u4EEPromBufSize = 0;

UINT8 * bMB82Developer = NULL;
UINT32 u4MB82DeveloperSize = 0;

UINT8 * bMB82Autotest = NULL;
UINT32 u4MB82AutotestSize = 0;

extern UINT32 _u4Upgraded;
static void _LoadBootKernel(UINT32 u4ImageAddr)
{
    LDR_DATA_T   *prLdrData;

	UINT32 *tagAddr,PhyRamStart,BootArgStart,BootArgEnd,KernelStart,kernel_entry,kernel_size;
	UINT32 LimitRam=LINUX_KERNEL_MEM_SIZE ;
        char *bootargs = rootDev;

        BIM_SetTimeLog(2);

	PhyRamStart = 0x00000000;
	BootArgStart =PhyRamStart + 0x100;
	BootArgEnd = BootArgStart + 0x6ff;
	KernelStart =0;
	kernel_entry =0;
	kernel_size =0;

#ifdef CC_LOADER_JFFS2_SUPPORT
        JFFS2_MountRwPartition();
     
	if (pJffs2RwRoot)
	{
	        char *buf;
	        UINT32 rsize;	
	        if (!JFFS2_Read(pJffs2RwRoot, "bootargs", &buf, &rsize))
	        {
	            bootargs = buf;
	            buf[rsize]=0;
	            printf("bootargs====> %s\n", buf);
                buf[rsize]=' ';
	        }
	        if (!JFFS2_Read(pJffs2RwRoot, "eeprom_1", &bEEPromBuf, &u4EEPromBufSize))
	        {
	            printf("loader eeprom dump succeed.");
	        }


			
		 if (!JFFS2_Read(pJffs2RwRoot, "MB82_DEVELOPER", &bMB82Developer, &u4MB82DeveloperSize))
	        {
	            printf("===>Find MB82_DEVELOPER in rw patition !!!");
		     _UsbKernelEN=1 ;
		      printf("\n===>Find MB82_DEVELOPER  and _UsbKernelEN=%d\n",_UsbKernelEN);
	        }
		    else
		    {
		        printf("\n===>There is no MB82_DEVELOPER,will check MB82_AUTOTEST !!!");
		        if (!JFFS2_Read(pJffs2RwRoot, "MB82_AUTOTEST", &bMB82Autotest, &u4MB82AutotestSize))
	            {
	            printf("\n===>Find MB82_AUTOTEST,will try to upgrade auto_full_pkg !!!\n");
				_TryUpgradeAutoPkg=1;
		        _UsbKernelEN=1 ;		        
	            }
				else
				{
				printf("\n===>There is no MB82_AUTOTEST,will Boot Normally!!!");
                 _UsbKernelEN=0;//boot normally;				
				}
		    }

	}

#endif
	set_cust_env();

#if (defined(ENABLE_DUALREAD))
		if (_u1DualReadCmd == 0x3B)
		{
			IO_WRITE8(0xf0008700, SFLASH_PRGDATA3_REG, _u1DualReadCmd);
			IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x1);
		}
		else if (_u1DualReadCmd == 0xBB)
		{
			IO_WRITE8(0xf0008700, SFLASH_PRGDATA3_REG, _u1DualReadCmd);
			IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x3);
	
		}
		else
		{
			IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x0);
	
		}
#endif

	x_memset(BootArgStart,0,BootArgEnd -BootArgStart);
	tagAddr=BootArgStart;
	//; ATAG_CORE
	*tagAddr = 0x2;
	tagAddr=tagAddr+ATAG_STEP;
	*tagAddr  = ATAG_CORE;
	tagAddr=tagAddr+ATAG_STEP;
	
	
	//; ATAG_MEM
	*tagAddr = 0x4;
	tagAddr=tagAddr+ATAG_STEP;
	*tagAddr = ATAG_MEM;
	tagAddr=tagAddr+ATAG_STEP;
	*tagAddr =LimitRam;
	tagAddr=tagAddr+ATAG_STEP;
	*tagAddr =PhyRamStart; // start
	tagAddr=tagAddr+ATAG_STEP;
	
	*tagAddr = 0x100;
	tagAddr=tagAddr+ATAG_STEP;
	*tagAddr = ATAG_CMDLINE;
	tagAddr=tagAddr+ATAG_STEP;

	#if 0//defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT)
	//loader kernel
	CHIP_FlashCopyToDRAMLZHS(4,u4ImageAddr,0,64);
	
	hdr = (image_header_t *)u4ImageAddr;
	KernelStart = htol(hdr->ih_load);
	kernel_entry =htol(hdr->ih_ep);
	kernel_size = htol( hdr->ih_size) + sizeof(image_header_t);

	Printf("\nhdr->ih_load = 0x%x \n",htol(hdr->ih_load));
	Printf("\nhdr->ih_ep = 0x%x \n", htol(hdr->ih_ep));
	Printf("\nhdr->ih_size = 0x%x \n",htol( hdr->ih_size));
	Printf("\nkernel_size = 0x%x \n", LimitRam);
	
	CHIP_FlashCopyToDRAMLZHS(4,KernelStart,0,kernel_size);
	
	Printf("\n nand or emmc boot \n");
	#else
	u4ImageAddr = PBI_A_BASE + DRVCUST_InitGet(eNorFlashPartSize0);

	hdr = (image_header_t *)u4ImageAddr;
	KernelStart = htol(hdr->ih_load);
	kernel_entry =htol(hdr->ih_ep);
	kernel_size = htol( hdr->ih_size) + sizeof(image_header_t);

	Printf("\nhdr->ih_load = 0x%x \n",htol(hdr->ih_load));
	Printf("\nhdr->ih_ep = 0x%x \n", htol(hdr->ih_ep));
	Printf("\nhdr->ih_size = 0x%x \n",htol( hdr->ih_size));
	Printf("\nkernel_size = 0x%x \n", LimitRam);

	
/*#if (defined(ENABLE_DUALREAD))
    if (_u1DualReadCmd == 0x3B)
    {
        IO_WRITE8(0xf0008700, SFLASH_PRGDATA3_REG, _u1DualReadCmd);
        IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x1);
    }
    else if (_u1DualReadCmd == 0xBB)
    {
        IO_WRITE8(0xf0008700, SFLASH_PRGDATA3_REG, _u1DualReadCmd);
        IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x3);

    }
    else
    {
        IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x0);

    }
#endif*/

	if (_UsbKernelEN==0)
	{
		
	     if((kernel_size  < LimitRam) && (kernel_size > 524288))// empty flash kernel_size will have 0xffffffff+N--->0x0000000x problem. that blocks upgrade.
	    {
	        Printf("\n====>copy kernel to dram normal_case :kernel_size=%d\n",kernel_size);
	        x_memcpy((void *)KernelStart, (void *)u4ImageAddr, (UINT32)kernel_size); //assume loader will be smaller than 128KB

		 }
	    else //don't block upgrade.
	    {
	        x_thread_delay(2000);
	    }
	     Printf("\n NOR boot \n");
	}	
	//close nor flash dual read
#if (defined(ENABLE_DUALREAD))
	IO_WRITE8(0xf0008700, SFLASH_PRGDATA3_REG, 0x0);
	IO_WRITE8(0xf0008700, SFLASH_DUAL_REG, 0x0);	
#endif  
	//NANDHW_EnablePinmux();
	#endif

	
	theKernel = (void (*)(UINT32, UINT32, UINT32))kernel_entry;
	
	// Disable MMU and caches
	HalFlushDCache();
	HalDisableCaches();
	HalDisableMMU();
	
	SerTransparent();
    prLdrData = LDR_GetLdrData();
    LDR_CheckUpgrade(prLdrData);
if (_UsbKernelEN==1)	
{
  if(_BootNormal==1)
  {
   Printf("\n===>can not find kernel image in usb,so boot normally \n");	
    if(kernel_size  < LimitRam)
    {
    Printf("\n====>copy kernel to dram normal_case :kernel_size=%d\n",kernel_size);
        x_memcpy((void *)KernelStart, (void *)u4ImageAddr, (UINT32)kernel_size); 
    }
    else //don't block upgrade.
    {
        x_thread_delay(2000);
    }
  }
else
  {
	if(u4UsbKernelSize  < LimitRam)
    {
       Printf("\n====>copy kernel to dram usb_case :u4UsbKernelSize=%d\n",u4UsbKernelSize);
	 x_memcpy((void *)KernelStart, (void *)_pucFileBuf, (UINT32)u4UsbKernelSize); //memcpy kernel to 0x80000
    }

 if (_pucFileBuf)
    {
    Printf("\n====>will FREE _pucFileBuf=%x \n",_pucFileBuf);
        x_mem_free(_pucFileBuf);
        _pucFileBuf = NULL;
    }
   	}
     Printf("\n NOR boot \n");
}   


    if (_u4Upgraded == 0)
    {
	    if(1 == bPDWNC_ShouldDriverEnterStandby())
	    {
	        PDWNC_SetT8032LEDBlinkOff(1);
	        x_thread_delay(80);
	        PDWNC_SetT8032LEDTurnOn(1);
	        x_thread_delay(100);//east.yan20120120.    
	        LDR_EnterStandby(prLdrData);
	    }
   	}
    x_strcpy(tagAddr,bootargs);    

    if(1 == PDWNC_GetT8032IROKPressedStatus())//for upgrade.
    {
        PDWNC_SetT8032IROKPressedHookOrNot(0);
        x_strcat(tagAddr,upgradeStr);    
        tagAddr=tagAddr+ sizeof(upgradeStr);
    }
    tagAddr=tagAddr+ strlen(bootargs)+1;

    // ; ATAG_NONE
    *tagAddr= 0x0;
    tagAddr=tagAddr+ATAG_STEP;
    *tagAddr= 0x0;
    tagAddr=tagAddr+ATAG_STEP;

    PDWNC_SetT8032LEDBlinkOn(1);

    BIM_SetTimeLog(3);

    theKernel (0,MACHINE_ID, BootArgStart);
	while(1)
	{
	   printf("should not be here \n");
	}
	
}
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _LdrSetImageBank(LDR_DATA_T *prLdrData, UINT8 u1Image)
{
    UINT32 i4Ret;

    if (u1Image==0)
    {
        prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_IMAGE_BANK));
    }
    else if (u1Image==1)
    {
        prLdrData->rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_IMAGE_BANK);
    }
    i4Ret = EEPDTV_SetCfg(&prLdrData->rDtvCfg);
    if (i4Ret)
    {
        printf("eeprom write failed, return %d\n", i4Ret);
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** _LdrImageDecompressOther(): decompress image to dram on the offset of entry.
 *  This function is going to search the target device and find out the compressed
 *  image. Then copy to dram (if necessary) and decompress the data from dram
 *  to dram by LZHS hardware.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
static UINT32 _LdrImageDecompressOther(LDR_DATA_T *prLdrData)
{
    IMAGE_HEADER_T* prHeader;
    UINT32 u4FirmwareExecAddr = 0;
    UINT32 u4IfSecondImage = 0;

    u4IfSecondImage = prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_IMAGE_BANK;    // 0 for the first image, 1 for the second one

    Printf("\nTry the other image, DTVCFG_FLAG2_IMAGE_BANK=%d\n", u4IfSecondImage);

#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
    UINT32 u4ImageAddr;
    static BOOL _fgNorClkChange = FALSE;

    if (_fgNorClkChange == FALSE)
    {
        _fgNorClkChange = TRUE;
    }

    if (_u4DualOffset && u4IfSecondImage)
    {
        u4ImageAddr = PBI_A_BASE + _u4DualOffset;
    }
    else
    {
        u4ImageAddr = PBI_A_BASE + FIRMWARE_FLASH_OFFSET;
    }

    Printf("NOR load image from: 0x%x\n", (u4ImageAddr - PBI_A_BASE));

#else /* nand or emmc boot */

    UINT32 u4ImageAddr;

    if (_u415MBImageAddr == 0)
    {
        _u415MBImageAddr = (UINT32)x_mem_alloc(MAX_COMPRESS_IMAGE_SIZE);
    }
    u4ImageAddr = _u415MBImageAddr;
    u4ImageAddr = (u4ImageAddr & 0xfffffff0) + 0x00000010; //16-bytes alignment

    if (_u4DualOffset && u4IfSecondImage)
    {
        Printf("\nFlash load lzhs header from SysImgB(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr, 0, (2*1024));
    }
    else
    {
        Printf("\nFlash load lzhs header from SysImgA(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr, 0, (2*1024));
    }
#endif

    // Check if valid signature
    prHeader = (IMAGE_HEADER_T *)u4ImageAddr;
    if ((prHeader->u4Signature != IMAGE_SIGNATURE) && (prHeader->u4Signature != IMAGE_SIGNATURE_UPGRADE))
    {
        // Not a valid image
        Printf("No valid image found! 0x%08x: 0x%08x 0x%08x\n", u4ImageAddr, prHeader->u4Signature, IMAGE_SIGNATURE);
        return 1;
    }

    UINT32 u4ImgaeHeaderSize = (prHeader->u4Signature == IMAGE_SIGNATURE_UPGRADE) ? sizeof(IMAGE_UPGRADE_HEADER_T) : sizeof (IMAGE_HEADER_T);
    u4ImageAddr += u4ImgaeHeaderSize;

    if(prHeader->fgCompress == COMPRESS_TYPE_LZHS)
    {
        UINT32 u4DstSize;
        UINT8 u1Checksum;

        prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;

        u4DstSize = (*((UINT32 *)u4ImageAddr));
        u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) /* nand or emmc boot */
        UINT32 u4ImageLzhsSize = (*((UINT32 *)(u4ImageAddr + 4))) + 0x10 + u4ImgaeHeaderSize;

    #if defined(CC_SECURE_BOOT_ALL)
        u4ImageLzhsSize += 768;
    #endif

        if(u4ImageLzhsSize > MAX_COMPRESS_IMAGE_SIZE)
		{
		    Printf("alloc Compress Image buffer 0x%x is too small than Image lzhs 0x%x \n", MAX_COMPRESS_IMAGE_SIZE, u4ImageLzhsSize);
		    ASSERT(0);
		}

        if(_u4DualOffset && u4IfSecondImage)
        {
            Printf("\nFlash load image from SysImgB(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
        else
        {
            Printf("\nFlash load image from SysImgA(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
#endif

        if(LZHS_Dec(u4ImageAddr + 0x10, u4FirmwareExecAddr, u4DstSize, &u1Checksum) != 0)
        {
            Printf("LZHS decode fail!\n");
            return 1;
        }
    }
    else
    {
        #if 0
        // Copy the image to destination address
        Printf("Copying image...\n");
        x_memcpy((void *)u4FirmwareExecAddr, (void *)u4ImageAddr, prHeader->u4Size);
        #endif
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** LDR_ImageDecompress(): decompress image to dram on the offset of entry.
 *  This function is going to search the target device and find out the compressed
 *  image. Then copy to dram (if necessary) and decompress the data from dram
 *  to dram by LZHS hardware.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_ImageDecompress(LDR_DATA_T *prLdrData)
{
    IMAGE_HEADER_T* prHeader;
    UINT32 u4FirmwareExecAddr = 0;
    UINT32 u4IfSecondImage = 0;

    _u4DualOffset = DRVCUST_InitGet(eLoaderDualBootOffset);
    u4IfSecondImage = prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_IMAGE_BANK;    // 0 for the first image, 1 for the second one

// 1. read lzhs header
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
    UINT32 u4ImageAddr;

    if (_u4DualOffset && u4IfSecondImage)
    {
        u4ImageAddr = PBI_A_BASE + _u4DualOffset;
    }
    else
    {
        u4ImageAddr = PBI_A_BASE + FIRMWARE_FLASH_OFFSET;
    }

    Printf("\nNOR load lzhs header from:0x%x\n", (u4ImageAddr - PBI_A_BASE));
	#ifdef CC_LOAD_KERNEL
	_LoadBootKernel(u4ImageAddr);
	#endif

#else /* nand or emmc boot */

    UINT32 u4ImageAddr;

    if (_u415MBImageAddr == 0)
    {
        _u415MBImageAddr = (UINT32)x_mem_alloc(MAX_COMPRESS_IMAGE_SIZE);
    }
    u4ImageAddr = _u415MBImageAddr;
    u4ImageAddr = (u4ImageAddr & 0xfffffff0) + 0x00000010; //16-bytes alignment

#ifdef CC_LOAD_UBOOT
#ifdef CC_LOAD_KERNEL

	_LoadBootKernel(u4ImageAddr);

#else
    Printf("\nFlash load lzhs header from 0x%x to dram(0x%x), size=2048\n", LOADER_MAX_SIZE, u4ImageAddr);
    CHIP_FlashCopyToDRAMLZHS(0, u4ImageAddr, LOADER_MAX_SIZE, (2*1024));
	#endif

#else /* CC_LOAD_UBOOT */

    if (_u4DualOffset && u4IfSecondImage)
    {
        Printf("\nFlash load lzhs header from SysImgB(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr, 0, (2*1024));
    }
    else
    {
        Printf("\nFlash load lzhs header from SysImgA(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr, 0, (2*1024));
    }
#endif /* CC_LOAD_UBOOT */

#endif //nor boot

// 2. read uboot/sys compressed image for lzds decoding
#ifdef CC_LOAD_UBOOT
    UINT32 u4DstSize;
    UINT8 u1Checksum;

    UNUSED(prHeader);
    UNUSED(_LdrSetImageBank);
    UNUSED(_LdrImageDecompressOther);
    UNUSED(u4IfSecondImage);

    prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = 0x800000;
    Printf("Decompression uboot to 0x%08x...\n", u4FirmwareExecAddr);

    u4DstSize = (*((UINT32 *)u4ImageAddr));
    u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) /* nand or emmc boot */
    UINT32 u4ImageLzhsSize;
    u4ImageLzhsSize = (*((UINT32 *)(u4ImageAddr + 4))) + 0x10;

    #if defined(CC_SECURE_BOOT_ALL)
    u4ImageLzhsSize += 768;
    #endif

    Printf("\nFlash load image from 0x%x to dram(0x%x), size=0x%x\n", LOADER_MAX_SIZE, u4ImageAddr, u4ImageLzhsSize);
    CHIP_FlashCopyToDRAMLZHS(0, u4ImageAddr, LOADER_MAX_SIZE, u4ImageLzhsSize);

#ifdef CC_TRUSTZONE_SUPPORT
    do
    {
        //u4ImageLzhsSize = (*(UINT32 *)(u4ImageAddr + 4) + 0x20 + 256) & 0xfffffff0 ;
        //CHIP_FlashCopyToDRAMLZHS(0, LINUX_KERNEL_MEM_SIZE + DRVCUST_InitGet(eTzCodeBase), LOADER_MAX_SIZE+u4ImageLzhsSize, DRVCUST_InitGet(eTzCodeLen));

        UINT32 u4TzNandOffset, u4TzLzhsDstDram, u4TzLzhsSrcDram, u4TzPlainSize, u4TzLzhsSize;
        UINT8 u1TzChksum;
        // uboot=uboot_lzhs_header(16)+uboot_lzhs+padding(16)+signature(256)
        u4TzNandOffset = (*(UINT32 *)(u4ImageAddr + 4) + 0x20 + 256) & 0xfffffff0 ;
#if defined(CC_SECURE_BOOT_CHECK_CUSTKEY)
        //+custkey(256)
        u4TzNandOffset += 256;
#endif
        u4TzNandOffset += LOADER_MAX_SIZE;
        u4TzLzhsDstDram = LINUX_KERNEL_MEM_SIZE + DRVCUST_InitGet(eTzCodeBase);
        u4TzLzhsSrcDram = LINUX_KERNEL_MEM_SIZE + 0x100000;
        //copy first 4k first to check header
        CHIP_FlashCopyToDRAMLZHS(0, u4TzLzhsSrcDram, u4TzNandOffset, 0x1000);
        u4TzPlainSize = *((UINT32*)u4TzLzhsSrcDram);
        u4TzLzhsSize = *((UINT32*)(u4TzLzhsSrcDram+4));
        u1TzChksum = *((UINT8*)(u4TzLzhsSrcDram+8));
        //copy rest data
        CHIP_FlashCopyToDRAMLZHS(0, u4TzLzhsSrcDram+0x1000, u4TzNandOffset+0x1000, u4TzLzhsSize);
        if(LZHS_Dec(u4TzLzhsSrcDram + 0x10, u4TzLzhsDstDram, u4TzPlainSize, &u1TzChksum) != 0)
        {
            Printf("TZ LZHS decode fail! src=0x%x,dst=0x%x,psize=0x%x,lsize=0x%x,chk=0x%x\n",
                u4TzLzhsSrcDram + 0x10, u4TzLzhsDstDram, u4TzPlainSize, u4TzLzhsSize, u1TzChksum);
            return 1;
        }
    }
    while (0);
#endif

#endif

    if(LZHS_Dec(u4ImageAddr + 0x10, u4FirmwareExecAddr, u4DstSize, &u1Checksum) != 0)
    {
        Printf("LZHS decode fail!\n");
        return 1;
    }

#else // nucleus

    // Check if valid signature
    prHeader = (IMAGE_HEADER_T *)u4ImageAddr;
    if ((prHeader->u4Signature != IMAGE_SIGNATURE) && (prHeader->u4Signature != IMAGE_SIGNATURE_UPGRADE))
    {
        // Not a valid image
        Printf("No valid image found! 0x%08x: 0x%08x 0x%08x\n", u4ImageAddr, prHeader->u4Signature, IMAGE_SIGNATURE);

        if (_u4DualOffset)
        {
            _LdrSetImageBank(prLdrData, !u4IfSecondImage);
            return _LdrImageDecompressOther(prLdrData);
        }
        else
        {
            return 1;
        }
    }

    UINT32 u4ImgaeHeaderSize = (prHeader->u4Signature == IMAGE_SIGNATURE_UPGRADE) ? sizeof(IMAGE_UPGRADE_HEADER_T) : sizeof (IMAGE_HEADER_T);
    u4ImageAddr += u4ImgaeHeaderSize;

#ifdef CC_LZMA_DECODE
    UINT32 u4DstSize, u4DstAddr, u4SrcSize, u4SrcAddr;
    LZMA_FILE_HEADER* prLzmaHdr = (LZMA_FILE_HEADER *)u4ImageAddr;

    u4DstSize = prLzmaHdr->org_size;
    u4DstAddr = prHeader->u4FirmwareExecAddr;
    u4SrcAddr = u4ImageAddr + sizeof(LZMA_FILE_HEADER);
    u4SrcSize = prLzmaHdr->compressed_size;
    prLdrData->rLdrHdr.u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;
    UNUSED(u4FirmwareExecAddr);

    Printf("LZMA decode, src{addr=0x%08x, len=%d(0x%08x)}, dst{addr=0x%08x, len=%d(0x%08x)}\n",
        u4SrcAddr, u4SrcSize, u4SrcSize, u4DstAddr, u4DstSize, u4DstSize);
    UINT32 u4Ret = (UINT32)LzmaUncompress((unsigned char *)u4DstAddr, (size_t *)&u4DstSize,
        (unsigned char *)u4SrcAddr, (size_t *)&u4SrcSize,
        (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));

    if (u4Ret != SZ_OK)
    {
        Printf("LZMA fail, code=%d\n", u4Ret);
        return 1;
    }

#else

    if(prHeader->fgCompress == COMPRESS_TYPE_LZHS)
    {
        UINT32 u4DstSize;
        UINT8 u1Checksum;

        prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;
        Printf("Decompression image to 0x%08x...\n", u4FirmwareExecAddr);

        u4DstSize = (*((UINT32 *)u4ImageAddr));
        u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));

    #if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) /* nand or emmc boot */
        UINT32 u4ImageLzhsSize = (*((UINT32 *)(u4ImageAddr + 4))) + 0x10 + u4ImgaeHeaderSize;

        #if defined(CC_SECURE_BOOT_ALL)
        u4ImageLzhsSize += 768;
        #endif

		if(u4ImageLzhsSize > MAX_COMPRESS_IMAGE_SIZE)
		{
    		Printf("alloc Compress Image buffer 0x%x is too small than Image lzhs 0x%x \n", MAX_COMPRESS_IMAGE_SIZE, u4ImageLzhsSize);
    		ASSERT(0);
		}

        if(_u4DualOffset && u4IfSecondImage)
        {
            Printf("\nFlash load image from SysImgB(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
        else
        {
            Printf("\nFlash load image from SysImgA(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
    #endif /* nand or emmc boot */

        if(LZHS_Dec(u4ImageAddr + 0x10, u4FirmwareExecAddr, u4DstSize, &u1Checksum) != 0)
        {
            Printf("LZHS decode fail!\n");
            if (_u4DualOffset)
            {
                _LdrSetImageBank(prLdrData, !u4IfSecondImage);
                return _LdrImageDecompressOther(prLdrData);
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        #if 0
        // Copy the image to destination address
        Printf("Copying image...\n");
        x_memcpy((void *)u4FirmwareExecAddr, (void *)u4ImageAddr, prHeader->u4Size);
        #endif
    }
#endif // CC_LZMA_DECODE
#endif // nucleus

    return 0;
}

