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
// USB Tool Driver
// Author: Barz Hsu
// 2010/03/19


#ifndef __KERNEL__
#ifndef CC_USB_DISABLE
#define SUPPORT_USBCDC
#endif
#endif

// === INCLUDE =============================================================================
#include "x_assert.h"
#include "nptv_debug.h"
#include "sv_const.h"
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#include "eepdef.h"
//#include "gdma_if.h"
#include "drv_scpos.h"
#include "api_eep.h"
#include "gfx_drvif.h"
#include "gfx_if.h"


// === HW INCLUDE =============================================================================
#include "hw_sw.h"

// === DEFINE =============================================================================

#ifdef SUPPORT_USBCDC
#include "musb_if.h"
#include "mu_cdc_if.h"
#endif

#define USB_EEPROM_MAGIC 0x73

enum {
    // Register Access
    MUSB_READ_REG,
    MUSB_WRITE_REG,

    // Burn Image
    MUSB_BURN_IMAGE,

    // Get DRAM
    MUSB_GET_DRAM,

    // Get Input Info
    MUSB_GET_INPUT_INFO,
    MUSB_PUT_DRAM,

    // MISC
    MUSB_GET_CHIP_ID,
    
    // CLI
    MUSB_RECV_CLI,
    MUSB_SEND_CLI,       
};


typedef struct
{
    UINT32 dwCmd;
    UINT32 dwAddr;
    UINT32 dwData;
}MUSB_CMD_SET;

typedef struct
{
    UINT32 WriteIndex;       /* @field Current Write index. */
    UINT32 MaxSize;      	 /* @field Length of buffer */
    UINT8 *Buffer; 		 	 /* @field Start of buffer */
} USB_CLI_QUEUE;


// === FUNCTION PROTOTYPE =================================================================
// ==========================================================
UINT32 FlashGetBlkSize(UINT8 u1FlashType);
INT8 FlashWrite(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size, UINT8 fgBoost);
INT8 FlashRead(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size);
INT8 FlashErase(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4Size);
UINT32 CalcCheckSum(UINT32 u4MemPtr, UINT32 u4Size);
// ==========================================================


void vSWRegInit(void);
void vSWRegMainLoop(void);

// === Global FUNCTION =================================================================
void CopyScalerDRAM(void);
void vSetUSBDump(BOOL bOnOff);
void NptvScalerVideoDump(INT32 i4Argc, const CHAR **szArgv);
UINT32 CalcCheckSum(UINT32 u4MemPtr, UINT32 u4Size);

// === STATIC FUNCTION ====================================================================
#ifdef SUPPORT_USBCDC
static UINT32 _MMAP_Allocate(UINT32 dwAddr, UINT32 dwSize);
static UINT32 _MMAP_Addr(UINT32 dwAddr);
static UINT32 _MMAP_Addr_64Align(UINT32 dwAddr);
static void _vUsbDownloadProc(MUSB_CMD_SET rCmd);
#endif

// === EXTERN FUNCTION ====================================================================
EXTERN void vSendUSBCLICmd(UINT8 *sCliCmd, UINT32 dwSize);
EXTERN UINT32 StrToInt(const CHAR* pszStr);
EXTERN INT32 Loader_GetNandFlashBlockSize(void);
EXTERN INT32 Loader_ReadNandFlash(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize);
EXTERN INT32 Loader_WriteNandFlash(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize);
EXTERN void SerTransparent(void);
EXTERN void* BSP_AllocateReservedAlign(UINT32 u4Size, UINT32 u4Align);


// === GLOBAL VARIABLE ====================================================================


// === EXTERN VARIABLE ====================================================================


// === STATIC VARIABLE ====================================================================

#ifdef SUPPORT_USBCDC
static HANDLE_T phDumpThreadHdl;
static BOOL fgScalerVideoDump = FALSE;
static UINT32 dwScalerCpyAddr = 0;
static UINT32 dwScalerCpyEndAddr = 0; 
static UINT32 dwScalerfDRAMSize;
static const  UINT32 MAX_MMAP_SIZE=1024;
static UINT32 *Dump_MMAP;
static UINT16 wDump_MMAP_Size;
static UINT16 wDump_MMAP_RealSize;

#endif /* SUPPORT_USBCDC */

static const int USBCLI_BUFSIZE = 1024*4;
static UINT8 *UsbCLIBuffer;
USB_CLI_QUEUE rUSbCLIQueue;
static HANDLE_T hUsbCLISema;

// Download Tool 
static HANDLE_T phFlashEraseThreadHdl;


// === END OF STATIC VARIABLE =============================================================


// =================================================================
// USB CLI
// =================================================================

void vUsbCLIInit(void)
{
	// Allocate Memory
	UsbCLIBuffer = (UINT8*)x_mem_alloc(USBCLI_BUFSIZE);
	ASSERT(UsbCLIBuffer != NULL);
	
    rUSbCLIQueue.WriteIndex = 0;
    //pQueue->Read = 0;
    rUSbCLIQueue.Buffer = UsbCLIBuffer;
    rUSbCLIQueue.MaxSize = USBCLI_BUFSIZE;
    VERIFY(OSR_OK == x_sema_create(&hUsbCLISema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));
}


void vUsbCLIPush(UINT8 u1Char)
{
    if (rUSbCLIQueue.Buffer)
    {
//        x_sema_lock(hUsbCLISema, X_SEMA_OPTION_WAIT);
        
        ASSERT(rUSbCLIQueue.WriteIndex < rUSbCLIQueue.MaxSize);
        
        rUSbCLIQueue.Buffer[rUSbCLIQueue.WriteIndex++] = u1Char;
        
        if (rUSbCLIQueue.WriteIndex == rUSbCLIQueue.MaxSize)
        {
            rUSbCLIQueue.WriteIndex = 0;
        }
        
//        x_sema_unlock(hUsbCLISema);
    }
}

#ifdef SUPPORT_USBCDC
static void _vUsbCLIClear()
{
    rUSbCLIQueue.WriteIndex= 0;
}

static UINT32 _GetUsbCLISize(void)
{
    return (rUSbCLIQueue.WriteIndex);
}

static UINT8* _GetUsbCLIPtr(void)
{
    return (rUSbCLIQueue.Buffer);
}
#endif

// =================================================================
// End of USB CLI
// =================================================================


#ifdef SUPPORT_USBCDC
//-------------------------------------------------------------------------
/** _MUSB_DumpThread
 *  USB CLI CDC Device 2 DMA channel test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL.
 */
//-------------------------------------------------------------------------

static VOID _NPTV_DumpThread(VOID *pvArgv)
{
    UINT32 txcount = 0;
    INT32 xfercount = 0;
    UINT32 dwSize;
    MUSB_CMD_SET rCmd;
    tagDramDumpInfo_T rDramDumpInfo;
    UINT8 *pbData;
	
    UNUSED(pvArgv);

	// Fix klocwork
	x_memset((void*)&rDramDumpInfo, 0, sizeof(tagDramDumpInfo_T));

    vUsbCLIInit();

	// Allocate Memory
	pbData = (UINT8*)x_mem_alloc(4096);
	ASSERT(pbData != NULL);
	
    while(1)
    {
		if(MUSB_CdcConfigOK() < 0)
		{
			x_thread_delay(500);
			continue;
		}

        xfercount = MUSB_CdcSyncRx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));
        LOG(6, "Read cmd:%d, Size:%d\n", rCmd.dwCmd, xfercount);
        if (xfercount != sizeof(MUSB_CMD_SET))
        {
            LOG(0, "\nRead error count = %d, xfercount = %d.\n",
            	sizeof(MUSB_CMD_SET), xfercount);
            continue;
        }

        switch(rCmd.dwCmd)
        {
        case MUSB_READ_REG:
            rCmd.dwData = u4IO32Read4B(rCmd.dwAddr);
            LOG(6, "MUSB_READ_REG Addr: %x, Value:%x\n", rCmd.dwAddr, rCmd.dwData);
            xfercount = MUSB_CdcSyncTx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));

            if(xfercount !=  sizeof(MUSB_CMD_SET))
            {
                LOG(0, "MUSB: Write error txcount = %d, xfercount = %d.\n", txcount, xfercount);
            }
            break;

        case MUSB_WRITE_REG:
            LOG(6, "MUSB_WRITE_REG Addr: %x, Value:%x\n", rCmd.dwAddr, rCmd.dwData);
			vIO32Write4B(rCmd.dwAddr, rCmd.dwData);
            break;

        case MUSB_GET_DRAM:
			LOG(6, "MUSB_GET_DRAM Addr:%x, Size:%x\n", rCmd.dwAddr, rCmd.dwData);

			xfercount = MUSB_CdcSyncTx((UINT8 *)rCmd.dwAddr, rCmd.dwData);

			if (rCmd.dwData != xfercount)
			{
			    LOG(0, "\nWrite error dwSize = %d, xfercount = %d.\n", rCmd.dwData, xfercount);
			}
			LOG(6, "MUSB_GET_DRAM Final\n");
            break;

        case MUSB_GET_INPUT_INFO://get scalar info and dump scalar
            vGetScalerDRAMInfo(&rDramDumpInfo);
			LOG(3, "MUSB_GET_INPUT_INFO Start\n");

			// For Scaler Real Time Dump
			if(dwScalerCpyAddr < dwScalerCpyEndAddr)
			{
				LOG(0, "MUSB_GET_INPUT_INFO, Addr 0x%x, Size 0x%x\n", 
					dwScalerCpyAddr, rDramDumpInfo.dwSize);
				rDramDumpInfo.dwAddr = _MMAP_Addr_64Align(dwScalerCpyAddr);
				if(rDramDumpInfo.dwSize == dwScalerfDRAMSize*2)
				{
					rDramDumpInfo.dwSize = dwScalerfDRAMSize;
					rDramDumpInfo.dwMVL >>= 1;
				}	
				dwScalerCpyAddr += dwScalerfDRAMSize;					
			}
			
            txcount = sizeof(rDramDumpInfo);
            xfercount = MUSB_CdcSyncTx((UINT8 *)(&rDramDumpInfo), txcount);
            if (txcount != xfercount)
            {
                LOG(0, "\nWrite error txcount = %d, xfercount = %d.\n", txcount, xfercount);
            }
            LOG(3, "MUSB_GET_INPUT_INFO Final\n");

            break;

		case MUSB_PUT_DRAM:
			LOG(6, "MUSB: Put DRAM Addr:%x, Size:%x\n", rCmd.dwAddr, rCmd.dwData);

			xfercount = MUSB_CdcSyncRx((UINT8*)(rCmd.dwAddr),
						rCmd.dwData);
			LOG(6, "MUSB: Put DRAM Finish\n");
			break;

		case MUSB_BURN_IMAGE:	
			_vUsbDownloadProc(rCmd);			
			break;

		case MUSB_GET_CHIP_ID:
            rCmd.dwData = u4IO32Read4B(0xF00081FC);
            xfercount = MUSB_CdcSyncTx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));

            if(xfercount !=  sizeof(MUSB_CMD_SET))
            {
                LOG(0, "MUSB: Write error txcount = %d, xfercount = %d.\n", txcount, xfercount);
            }
			break;
			
        case MUSB_RECV_CLI:
//			x_sema_lock(hQSema, X_SEMA_OPTION_WAIT);

			rCmd.dwData = _GetUsbCLISize();
			MUSB_CdcSyncTx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));
			if (rCmd.dwData > 0)
			{
			    MUSB_CdcSyncTx(_GetUsbCLIPtr(), rCmd.dwData+12);
    			_vUsbCLIClear();
    			// LOG(0, "Send %d bytes to usb.\n", rCmd.dwData);
    	    }
//    	    x_sema_unlock(hQSema);

			break;

        case MUSB_SEND_CLI:
	       // CLI command length is stored in rCmd.dwData
	    	dwSize = rCmd.dwData;
            ASSERT(dwSize < 4096);

	  	    xfercount = MUSB_CdcSyncRx(pbData, dwSize);

            if(xfercount != dwSize)
            {
                LOG(0, "\nRead error count = %d, xfercount = %d.\n",
                    dwSize, xfercount);
                //bOk = SV_FALSE;
            }
            vSendUSBCLICmd(pbData, dwSize);
			break;

        default:
            ASSERT(0);
        }
    }
}

//=========================================================
static void _vUsbDownloadProc(MUSB_CMD_SET rCmd)
{
    HAL_TIME_T TimeStart, TimeNow, TimeDelta;
	UINT32 xfercount;
    UINT32 dwSize, dwOffset;
	UINT32 u4PartID;
    UINT8 *pBinBuf, *pBinBuf_Cur;    
    UINT32 dwBlkSize;	
	UINT8 u1FlashType;
	UINT32 index;

	// Get Start Time Stamp
	HAL_GetTime(&TimeStart);

	// Parsing USB Command
	dwOffset = rCmd.dwAddr & 0xFFFFFF00;
	u4PartID = rCmd.dwAddr & 0xFF;	
	dwSize = rCmd.dwData & 0xFFFFFF00;	
	u1FlashType = rCmd.dwData & 0xFF;	

	// Get Flash Information
    UINT32 _u4FlashTotalSize;	
#if defined(CC_NAND_ENABLE) 		
		_u4FlashTotalSize = DRVCUST_OptGet(eNANDFlashTotalSize);
#else
		_u4FlashTotalSize = DRVCUST_OptGet(eNorFlashTotalSize);
#endif

	if(u1FlashType == 0)
	{
#if defined(CC_NAND_ENABLE) 
		u1FlashType = 2;
#else
		u1FlashType = 1;
#endif
	}
	
	if(u1FlashType==2 && u4PartID==0xFF)
	{
		// Get NAND partition ID	
		DRVCUST_OptQuery(eNANDFlashPartIdSysImgA, (UINT32*)&u4PartID);						
	}
	

	dwBlkSize = FlashGetBlkSize(u1FlashType);  	

	
	Printf("USB_Upgrade: Offset:%x, Size:%x, Flash Size:%x\n", 
		dwOffset, dwSize, _u4FlashTotalSize);

	if((dwOffset + dwSize) > _u4FlashTotalSize)
	{
		Printf("USB_Upgrade: Image is larger than flash size\n");
		return;
	}
	

	// Allocate Memory
	pBinBuf = (UINT8*)x_mem_alloc(dwSize);
	ASSERT(pBinBuf);
	x_memset(pBinBuf, 0xFF, dwSize);

	// Receive Data
	xfercount = MUSB_CdcSyncRx(pBinBuf, dwSize);

	if(xfercount != dwSize)
	{
		Printf("USB_Upgrade: Read error read %x, expected %x\n",
			xfercount, dwSize);
		return;
	}

	
	// disable ISR
	vRegWrite4B(0x240c, 0xFFFFFFFF);
	// disable watchdog
	vRegWrite4B(0x8000, 0x0);

	// Show Transfer Time
	HAL_GetTime(&TimeNow);
	HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
	Printf("USB_Upgrade: Transfer binfile time %d sec\n", TimeDelta.u4Seconds);

	// Get CheckSum
	CalcCheckSum((UINT32)pBinBuf, dwSize);

	xfercount = MUSB_CdcSyncRx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));
	if(xfercount != sizeof(MUSB_CMD_SET))
	{
		Printf("Failed\n");
		return;
	}else
	{
		Printf("Okay\n");
	}

	// Upgrade
	pBinBuf_Cur = pBinBuf;

	INT32 i4Result;
	

	for(index=0; index < dwSize; )
	{

		if(u1FlashType==2)
		{
#ifdef CC_NAND_ENABLE
			i4Result = Loader_WriteNandFlash(u4PartID, dwOffset, (void*)pBinBuf_Cur, dwBlkSize);
#else
            i4Result = -1;
#endif
		}
		else
		{

			NOR_EraseBlk(dwOffset);
			i4Result = NOR_Write(dwOffset, (UINT32)pBinBuf_Cur, dwBlkSize);		
		}
			
		if(i4Result == 0)
		{
			Printf("USB_Upgrade: Progress 0x%08X/0x%08X\n", index, dwSize);
			
			index += dwBlkSize;
			dwOffset += dwBlkSize;
			pBinBuf_Cur += dwBlkSize;
		
			rCmd.dwData = index;

    		xfercount = MUSB_CdcSyncTx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));
			
    		if(xfercount != sizeof(MUSB_CMD_SET))
    		{
        		Printf("USB_Upgrade: USB Write error count = %d, xfercount = %d.\n",
        			sizeof(MUSB_CMD_SET), xfercount);
				
				Printf("USB_Upgrade: Flash Upgrade Failed\n\n");						
				return;
    		}
		
		}else{
			Printf("USB_Upgrade: Flash Upgrade Failed\n\n");		
			return;
		}
		

	}
	x_mem_free(pBinBuf);

	// Show End Time Stamp
	HAL_GetTime(&TimeNow);
	HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
	Printf("USB_Upgrade: Elapse time %d sec\n", TimeDelta.u4Seconds);
			
	Printf("USB_Upgrade: Finish!\n");

}
//=========================================================



static UINT32 _MMAP_Allocate(UINT32 dwAddr, UINT32 dwSize)
{		
	UINT32 dwCount=0;
	UINT32 dwEndAddr = dwAddr + dwSize;
	while((dwAddr+dwScalerfDRAMSize) <= dwEndAddr)
	{
		if(wDump_MMAP_Size >= MAX_MMAP_SIZE)
		{
			break;
		}
		Dump_MMAP[wDump_MMAP_Size++] = dwAddr;
		dwAddr += dwScalerfDRAMSize;	
		dwCount++;
	}
	return dwCount;
}

static UINT32 _MMAP_Addr(UINT32 dwAddr)
{	
	UINT32 dwIndex;
	UINT32 dwRealAddr;
	
	ASSERT(dwScalerfDRAMSize != 0);
	dwIndex = dwAddr / dwScalerfDRAMSize;
	ASSERT(dwIndex  < wDump_MMAP_Size);	
	
	if(wDump_MMAP_Size >= MAX_MMAP_SIZE)
	{
		return 0;
	}	
	
	dwRealAddr = Dump_MMAP[dwIndex]; 
	LOG(3, "MMAP Address=0x%08X, Index=%d, Read Address=0x%08X\n", dwAddr, dwIndex, dwRealAddr);
	return dwRealAddr;
}


static UINT32 _MMAP_Addr_64Align(UINT32 dwAddr)
{
	UINT32 dwAlignAddr, dwBaseAddr;

	dwAlignAddr = _MMAP_Addr(dwAddr);
	dwBaseAddr = (dwAlignAddr >> 6) << 6;

	if(dwAlignAddr != dwBaseAddr)
	{
		return 	(dwBaseAddr+64);
	}else
	{
		return dwBaseAddr;
	}	
}


#endif // SUPPORT_USBCDC

void vSetUSBDump(BOOL bOnOff)
{
#ifdef SUPPORT_USBCDC
    static BYTE fgInit = SV_FALSE;   
    char *company=NULL;

	if(fgInit){
		return;
	}

	// Customers don't need to keep USB setting
	DRVCUST_OptQuery(eCustomerName, (UINT32*)&company);		
	if(bOnOff == SV_FALSE && !x_strcmp(company, "mtk"))
	{
    	bOnOff= (bApiEepromReadByte(EEP_MUSB_ENABLE) == USB_EEPROM_MAGIC);
	}
		
	if(bOnOff)
	{
		
		Printf("USB Dump Enabled!\n");	

    	(void)MUSB_Init(MUSB_DEVICE_MODE);
    	(void)MUSB_InitDeviceMode(MUSB_DEVICE_MODE_CDC);

    	x_thread_delay(1000);

    	/* back ground reading. */
    	x_thread_create(&phDumpThreadHdl, "USBDRAMDump",
        	8092,  50, _NPTV_DumpThread, 0, NULL);
    	fgInit = SV_TRUE;    	   	
    }
	else
	{
		MUSB_Init(MUSB_HOST_MODE);
	}
#endif

}


void NptvScalerVideoDump(INT32 i4Argc, const CHAR **szArgv)
{  
#ifdef SUPPORT_USBCDC
	tagDramDumpInfo_T rDramDumpInfo;
	UINT32 dwFrameCount = 20;
	BOOL fgHalf = FALSE;
	UINT32 dwAddr, dwSize, dwEndAddr;
	UINT32 dwIndex;

	// Fix klocwork
	x_memset((void*)&rDramDumpInfo, 0, sizeof(tagDramDumpInfo_T));
	
	if(x_strcmp(szArgv[1], "on") == 0)
	{
		if(i4Argc >= 3)
		{
			dwFrameCount = StrToInt(szArgv[2]);
		}
		if(i4Argc >= 4)
		{
			fgHalf = StrToInt(szArgv[3]);
		}			

		
		vGetScalerDRAMInfo(&rDramDumpInfo);		
		dwScalerfDRAMSize = (fgHalf ? rDramDumpInfo.dwSize/2 : rDramDumpInfo.dwSize);

		// Allocate Memory
		Dump_MMAP = (UINT32*)x_mem_alloc(MAX_MMAP_SIZE*4);	
		ASSERT(Dump_MMAP != NULL);
		
		for(dwIndex=0; dwIndex < dwFrameCount; dwIndex++)
		{
			dwSize = (rDramDumpInfo.dwWINC<<4)+64;
			dwAddr = (UINT32)x_mem_calloc(1, dwSize);
			dwEndAddr = dwAddr+dwSize;
			
			Printf("%d: Allocate Memory 0x%08x~0x%08x\n", dwIndex, dwAddr, dwEndAddr);			
			
			if(dwAddr == 0)
			{
				Printf("Memory Allocation Failed\n");
				break;
			}				
			_MMAP_Allocate(dwAddr, dwScalerfDRAMSize);							
		}		
		wDump_MMAP_RealSize = wDump_MMAP_Size;
		dwScalerCpyAddr = (UINT32)0;
		dwScalerCpyEndAddr = dwScalerfDRAMSize* wDump_MMAP_Size;
		
		Printf("Frame Number=%d, Dump Address=0x%08X, End Adddress=0x%08X\n", 
			dwIndex, dwScalerCpyAddr, dwScalerCpyEndAddr);
		

	}
	else if(x_strcmp(szArgv[1], "off") == 0)
	{		
		dwScalerCpyAddr = 0;
		dwScalerCpyEndAddr = 0;
		
		for(dwIndex=0; dwIndex < wDump_MMAP_RealSize; dwIndex++)
		{
			if(dwIndex < MAX_MMAP_SIZE)
			{
				Printf("%d: Free Memory 0x%08x\n", dwIndex, Dump_MMAP[dwIndex]);				
				x_mem_free((void*)Dump_MMAP[dwIndex]);											
				Dump_MMAP[dwIndex] = NULL;
			}
		}				
		
		// Free memory
		x_mem_free((void*)Dump_MMAP);
		Dump_MMAP = NULL;		
		
		wDump_MMAP_Size = 0;			
		wDump_MMAP_RealSize = 0;
			
	}else if(x_strcmp(szArgv[1], "start") == 0)	
	{
		dwScalerCpyAddr = 0;			
		fgScalerVideoDump = TRUE;			
	}else if(x_strcmp(szArgv[1], "add") == 0 && i4Argc >= 4)
	{
		dwAddr = StrToInt(szArgv[2]);
		dwSize = StrToInt(szArgv[3]);			
		dwIndex = _MMAP_Allocate(dwAddr, dwSize);				
		dwScalerCpyEndAddr = dwScalerfDRAMSize* wDump_MMAP_Size;			
		Printf("New Frame=%d, Total Frame=%d\n", dwIndex, wDump_MMAP_Size);
	}
	else	
	{		
    	Printf("scaler_dd [on/off]\n");
	}
#endif
}

void vScpipSetDumpCtrl(UINT8 bOnOff);

void CopyScalerDRAM(void)
{
#ifdef SUPPORT_USBCDC
/*
	static int count=0;
	HAL_TIME_T StartTime, EndTime, DeltaTime;
	tagDramDumpInfo_T rDramDumpInfo;		
	
	if(fgScalerVideoDump && wDump_MMAP_Size)
	{		
		vGetScalerDRAMInfo(&rDramDumpInfo);		
		HAL_GetTime(&StartTime);		

		_GFX_DMA((UINT32*)_MMAP_Addr(dwScalerCpyAddr), (UINT32*)rDramDumpInfo.dwAddr, dwScalerfDRAMSize/4);
		GFX_Flush();
		GFX_Wait();
		
		HAL_GetTime(&EndTime);	
		HAL_GetDeltaTime(&DeltaTime, &StartTime, &EndTime);			
		
		Printf("%d: Memcpy from 0x%x to 0x%x in size 0x%x at time %d:%d, during %d us\n", 
					count++, rDramDumpInfo.dwAddr, 
					dwScalerCpyAddr,  dwScalerfDRAMSize, 
					StartTime.u4Seconds, StartTime.u4Micros,
					DeltaTime.u4Micros);		
		
		dwScalerCpyAddr += dwScalerfDRAMSize;

		if(dwScalerCpyAddr >= dwScalerCpyEndAddr)
		{
			dwScalerCpyAddr = 0;	
			fgScalerVideoDump = FALSE;
			count = 0;
			Printf("End of scaler dramp dump\n");			
		}
	}	
*/	
//@6896 review this. Dram is totally diff. with 5395
	static int count=0;
	//static UINT32 dwScalerOrigAddr;
	UINT32 dwAddr;
	tagDramDumpInfo_T rDramDumpInfo;

	// Fix klocwork
	x_memset((void*)&rDramDumpInfo, 0, sizeof(tagDramDumpInfo_T));

	if(fgScalerVideoDump && wDump_MMAP_Size)
	{		
		if(dwScalerCpyAddr < dwScalerCpyEndAddr)
		{	
			HAL_TIME_T rTime;
			HAL_GetTime(&rTime);
			printf("Copy Scaler DRAM Time: %d:%d\n", rTime.u4Seconds, rTime.u4Micros/1000);

		
			if(count == 0)
			{
				// Initial Dump Setting
				vScpipSetDumpCtrl(SV_TRUE);
				//dwScalerOrigAddr = RegReadFldAlign(SCPIP_PIP1_05, PIP1_05_WSTARTP_1);	
			}

			dwAddr = _MMAP_Addr_64Align(dwScalerCpyAddr) >> 4;
	
		 	vGetScalerDRAMInfo(&rDramDumpInfo);	

			rDramDumpInfo.u2W_POS = (rDramDumpInfo.u2W_POS+1)%3;
			dwAddr -= rDramDumpInfo.u2W_POS*rDramDumpInfo.dwWINC;
			
			//vRegWriteFldAlign(SCPIP_PIP1_05, dwAddr, PIP1_05_WSTARTP_1);	
			
			Printf("%d: Scaler Dump into 0x%08x, W_POS=%d, WINC=%x\n", count, dwAddr << 4, rDramDumpInfo.u2W_POS, rDramDumpInfo.dwWINC);		
			
			dwScalerCpyAddr += dwScalerfDRAMSize;
			count++;
		}
		else
		{
			dwScalerCpyAddr = 0;	
			fgScalerVideoDump = FALSE;
			count = 0;

			// Restore Dump Setting
			//vRegWriteFldAlign(SCPIP_PIP1_05, dwScalerOrigAddr, PIP1_05_WSTARTP_1);
			vScpipSetDumpCtrl(SV_FALSE);
			
			Printf("End of scaler dramp dump\n");			
		}
	}
#endif	
}


// =======================================================================

UINT32 FlashGetBlkSize(UINT8 u1FlashType)
{    
	if(u1FlashType != 1 && u1FlashType != 2)
	{
		Printf("Not support flash type!\n");
		return 0;
	}


	if(u1FlashType==2)
	{
#ifdef CC_NAND_ENABLE        
		return Loader_GetNandFlashBlockSize();
#else
        return 0xFFFFFFFF;
#endif
	}else
	{
		return 0x10000;		
	}
}

static UINT32 _u4Offset_EraseFlash;
static UINT32 _u4Size_EraseFlash;
static UINT8 _fgDone_EraseFlash;

static VOID _FlashEraseThread(VOID *pvArgv)
{
    HAL_TIME_T TimeStart, TimeNow, TimeDelta;

	// Get Current Time
	HAL_GetTime(&TimeStart);

#ifndef CC_NOR_DISABLE
	Printf("Erasing from 0x%08X with size 0x%08X\n", _u4Offset_EraseFlash, _u4Size_EraseFlash);	
	NOR_EraseAddr(_u4Offset_EraseFlash, _u4Size_EraseFlash);
	_fgDone_EraseFlash = 1;
#endif
	
	HAL_GetTime(&TimeNow);
	HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow); 
	Printf("Elapse time %d sec\n", TimeDelta.u4Seconds);	
	Printf("Finish Erasing!\n");

}

INT8 FlashErase(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4Size)
{
	UNUSED(u4PartID);
	
	if(u1FlashType == 1)
	{
		_fgDone_EraseFlash = 0;
		_u4Offset_EraseFlash = u4Offset;
		_u4Size_EraseFlash = u4Size;
	
		x_thread_create(&phFlashEraseThreadHdl, "FlashEraseThread",
			2048,  50, _FlashEraseThread, 0, NULL);	
	}

	return 0;
}


INT8 FlashWrite(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size, UINT8 fgBoost)
{
    UINT32 dwBlkSize;  
    HAL_TIME_T TimeStart, TimeNow, TimeDelta;
    // fix gcc warning
	// Write
	UINT32 _Offset = 0;
	UINT32 _MemPtrCur = u4MemPtr;
	INT32 i4Result = 0;
	
	if(u1FlashType != 1 && u1FlashType != 2)
	{
		Printf("Not support flash type!\n");
		return -1;
	}
			
	// Get BlkSize
	dwBlkSize = FlashGetBlkSize(u1FlashType);
	
	// Disable ISR
	vRegWrite4B(0x240c, 0xFFFFFFFF);
	// Disable watchdog
	vRegWrite4B(0x8000, 0x0);

	// Get Current Time
	HAL_GetTime(&TimeStart);
	
	if(fgBoost && u1FlashType==1)	// NOR Fast Mode
	{
		UINT32 _timeout_counter=0;		
		while(!_fgDone_EraseFlash)
		{
			x_thread_delay(500);

			_timeout_counter+= 500;

			Printf("Waiting NOR Flash Erasing...\n");

			// Set timeout 40 sec
			if(_timeout_counter >  40000)
			{
				Printf("NOR Flash Erasing Timeout!\n");
				fgBoost = 0;
			}
		}
	}


	for(_Offset=u4Offset; _Offset < (u4Offset+u4Size);)
	{
		
		if(u1FlashType==2)
		{
#ifdef CC_NAND_ENABLE			
			i4Result = Loader_WriteNandFlash(u4PartID, _Offset, (void*)_MemPtrCur, dwBlkSize);
#else
            i4Result = -1;
#endif
		}
		else
		{
#ifndef CC_NOR_DISABLE
			if(!fgBoost)
			{
				NOR_EraseBlk(_Offset);
			}
			i4Result = NOR_Write(_Offset, _MemPtrCur, dwBlkSize);		
#endif
		}
		
		if(i4Result != 0)
		{
			Printf("Flash Upgrade Error!\n");
			return i4Result;		
		}
		
		_Offset+=dwBlkSize;
		_MemPtrCur+=dwBlkSize;	

		if(u1FlashType==2)
		{
			Printf("NANDFlash Upgrade Progress %08X/%08X, MemPtr=%08X\n", _Offset, u4Size, _MemPtrCur);	
		}else
		{
			Printf("NORFlash Upgrade Progress %08X/%08X, MemPtr=%08X\n", _Offset, u4Size, _MemPtrCur);
		}		
	}

	HAL_GetTime(&TimeNow);
	HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
	
	Printf("Elapse time %d sec\n", TimeDelta.u4Seconds);
	Printf("Finish!\n");
	
	return i4Result;
}


INT8 FlashRead(UINT8 u1FlashType, UINT32 u4PartID, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size)
{
	INT32 i4Result = 0;

	if(u1FlashType != 1 && u1FlashType != 2)
	{
		Printf("Not support flash type!\n");
		return -1;
	}
	

	if(u1FlashType==2)
	{
		Printf("NANDFlash Backup Offset=0x%08X, MemPtr=0x%08X, Size=0x%08X\n", 
			u4Offset, u4MemPtr, u4Size);
#ifdef CC_NAND_ENABLE			
		i4Result = Loader_ReadNandFlash(u4PartID, u4Offset, (VOID*)u4MemPtr, u4Size);
#else
        i4Result = -1;
#endif
	}
	else
	{
#ifndef CC_NOR_DISABLE
		Printf("NORFlash Backup Offset=0x%08X, MemPtr=0x%08X, Size=0x%08X\n", 
			u4Offset, u4MemPtr, u4Size);
		
		i4Result = NOR_Read(u4Offset, u4MemPtr, u4Size);	
#endif
	}

	return i4Result;	
}


UINT32 CalcCheckSum(UINT32 u4MemPtr, UINT32 u4Size)
{
	UINT32 i;
	UINT32 u4CheckSum = 0;
	UINT32 *pu4Data;

	pu4Data = (UINT32*)u4MemPtr;
	
	for(i=0; i < u4Size; i+=4)
	{
		u4CheckSum += (*pu4Data);		
		pu4Data++;
	}


	Printf("CheckSum=%08X\n", u4CheckSum);
	return u4CheckSum;
}

//======================================================================
void vSWRegInit(void)
{
    UINT32 i;
    // fix gcc warning
    UINT32 pName = 0; //NULL;
    
	// Enable tool to switch UART mode
	vIO32WriteFldAlign(TOOL_UART_MODE, 1, TOOL_UART_SUPPORT_MODE_CHG);
	vIO32WriteFldAlign(TOOL_UART_MODE, 1, TOOL_SUPPORT_CUST_NAME);
    vIO32WriteFldAlign(TOOL_UART_MODE, 1, TOOL_SUPPORT_MEMMAP);
    
    if (DRVCUST_OptQuery(eCustomerName, &pName) == 0)
    {
	    for (i=0; i<8; i++)
	    {
            vIO32Write4B(TOOL_CUST_NAME+4*i, *((UINT32*)pName+i));
	    }
    }
}

void vSWRegMainLoop(void)
{
    UINT32 u4Data;
    
	if(IO32ReadFldAlign(TOOL_UART_MODE, TOOL_UART_ENTER_T_MODE) == 1)
	{
		// Enter Transparent Mode
		HAL_Delay_us(5000);		
	
		SerTransparent();
		vIO32WriteFldAlign(TOOL_UART_MODE, 0, TOOL_UART_ENTER_T_MODE);
	}

	if (IO32ReadFldAlign(TOOL_UART_MODE, TOOL_SUPPORT_MEMMAP) == 1)
	{	    
	    u4Data = (u4RegRead4B(0x2700) & 0x3);
	    
        if (u4Data == 0x2)
        {
            vRegWrite4B(0x2700, SW_DRAM_BASE_PHY);
        }
        else if (u4Data == 0x3)
	    {
            vRegWrite4B(0x2700, SW_DRAM_BASE);
	    }
	}
}
	

