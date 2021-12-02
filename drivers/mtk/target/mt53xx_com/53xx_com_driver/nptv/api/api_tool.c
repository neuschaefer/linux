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
#include "gdma_if.h"
#include "drv_scpos.h"
#include "api_eep.h"

// === HW INCLUDE =============================================================================

// === DEFINE =============================================================================

#ifdef SUPPORT_USBCDC
#include "musb_if.h"
#include "mu_cdc_if.h"
#endif

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

// === FUNCTION PROTOTYPE =================================================================
INT8 FlashWrite(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size);
UINT32 FlashGetBlkSize(void);
// === Global FUNCTION =================================================================
void CopyScalerDRAM(void);
void vSetUSBDump(BOOL bOnOff);
void NptvScalerVideoDump(INT32 i4Argc, const CHAR **szArgv);
// === STATIC FUNCTION ====================================================================
#ifdef SUPPORT_USBCDC
static UINT32 _MMAP_Allocate(UINT32 dwAddr, UINT32 dwSize);
static UINT32 _MMAP_Addr(UINT32 dwAddr);
static void _vUsbDownloadProc(MUSB_CMD_SET rCmd);
#endif
// === EXTERN FUNCTION ====================================================================
EXTERN void vSendUSBCLICmd(UINT8 *sCliCmd, UINT32 dwSize);
EXTERN UINT32 StrToInt(const CHAR* pszStr);
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
static UINT32 Dump_MMAP[MAX_MMAP_SIZE];
static UINT16 wDump_MMAP_Size;
static UINT16 wDump_MMAP_RealSize;

#endif /* SUPPORT_USBCDC */

// === END OF STATIC VARIABLE =============================================================


// =================================================================
// USB CLI
// =================================================================

typedef struct
{
    UINT32 WriteIndex;       /* @field Current Write index. */
    UINT32 MaxSize;      	 /* @field Length of buffer */
    UINT8 *Buffer; 		 	 /* @field Start of buffer */
} USB_CLI_QUEUE;


#define USBCLI_BUFSIZE 1024*4
static UINT8 UsbCLIBuffer[USBCLI_BUFSIZE];
USB_CLI_QUEUE rUSbCLIQueue;
static HANDLE_T hUsbCLISema;

void vUsbCLIInit()
{
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
        x_sema_lock(hUsbCLISema, X_SEMA_OPTION_WAIT);
        
        ASSERT(rUSbCLIQueue.WriteIndex < rUSbCLIQueue.MaxSize);
        
        rUSbCLIQueue.Buffer[rUSbCLIQueue.WriteIndex++] = u1Char;
        
        if (rUSbCLIQueue.WriteIndex == rUSbCLIQueue.MaxSize)
        {
            rUSbCLIQueue.WriteIndex = 0;
        }
        
        x_sema_unlock(hUsbCLISema);
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
    UINT8 pbData[4096];
	
    UNUSED(pvArgv);

    vUsbCLIInit();

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
			LOG(6, "MUSB_GET_INPUT_INFO Start\n");

			// For Scaler Real Time Dump
			if(dwScalerCpyAddr < dwScalerCpyEndAddr)
			{
				LOG(0, "MUSB_GET_INPUT_INFO, Addr 0x%x, Size 0x%x\n", 
					dwScalerCpyAddr, rDramDumpInfo.dwSize);
				rDramDumpInfo.dwAddr = _MMAP_Addr(dwScalerCpyAddr);
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
            LOG(6, "MUSB_GET_INPUT_INFO Final\n");

            break;

		case MUSB_PUT_DRAM:
			LOG(0, "MUSB: Put DRAM Addr:%x, Size:%x\n", rCmd.dwAddr, rCmd.dwData);

			xfercount = MUSB_CdcSyncRx((UINT8*)(rCmd.dwAddr),
						rCmd.dwData);
			LOG(0, "MUSB: Put DRAM Finish\n");
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


static void _vUsbDownloadProc(MUSB_CMD_SET rCmd)
{
    UINT32 dwSize, dwOffset;
    INT32 i4FlashSize, i4Size;
    UINT8 *pBinBuf, *pBinBuf2;    
    UINT32 dwBlkSize;
    UINT32 dwRxUnitSize = 0x2000;    
    HAL_TIME_T TimeStart, TimeNow, TimeDelta;
    UINT32 _u4FlashTotalSize;
	UINT32 xfercount;
	UINT8 bOk = SV_TRUE;
	
	dwBlkSize = FlashGetBlkSize();    
	#if defined(CC_NAND_ENABLE)    		
		_u4FlashTotalSize = DRVCUST_OptGet(eNANDFlashTotalSize);
	#else
		_u4FlashTotalSize = DRVCUST_OptGet(eNorFlashTotalSize);
	#endif
    		
	dwOffset = (rCmd.dwAddr & 0xFFFF) << 16;

	// unit is 0x1000
	dwRxUnitSize = (rCmd.dwAddr >> 16) << 12;
	dwSize = rCmd.dwData;
	i4FlashSize = rCmd.dwData;

	UNUSED(pBinBuf2);

	HAL_GetTime(&TimeStart);

	LOG(0, "MUSB: Offset:%x, Size:%x, Flash Size:%x\n", 
		dwOffset, dwSize, _u4FlashTotalSize);
	LOG(0, "MUSB: Unitsize:%x\n", dwRxUnitSize);

	if((i4FlashSize&(dwBlkSize-1)) != 0)
	{
		i4FlashSize &= ~(dwBlkSize-1);
		i4FlashSize += dwBlkSize;
	}

	pBinBuf = (UINT8 *)x_mem_alloc(i4FlashSize);
	assert(pBinBuf);
	x_memset(pBinBuf, 0xFF, i4FlashSize);

	pBinBuf2 = pBinBuf;
	i4Size = dwSize;

	while(i4Size > 0)
	{
		int rsize = (i4Size >= dwRxUnitSize ? dwRxUnitSize: i4Size);
		xfercount = MUSB_CdcSyncRx(pBinBuf2, rsize);

		if(xfercount != rsize)
		{
			LOG(0, "MUSB: Read error read %x, expected %x\n",
				xfercount, rsize);
			bOk = SV_FALSE;
			return;
		}
		i4Size -= rsize;
		pBinBuf2 += rsize;
	}

    xfercount = MUSB_CdcSyncRx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));

    LOG(4, "Read cmd:%d, Size:%d\n", rCmd.dwCmd, xfercount);
    if(xfercount != sizeof(MUSB_CMD_SET))
    {
        LOG(0, "\nRead error count = %d, xfercount = %d.\n",
        	sizeof(MUSB_CMD_SET), xfercount);
        bOk = SV_FALSE;
    }

	if(rCmd.dwCmd != MUSB_BURN_IMAGE || rCmd.dwAddr != 0)
	{
        bOk = SV_FALSE;
        return;
	}

	if(bOk == SV_FALSE)
	{
		LOG(0, "MUSB: Image is wrong during transfering\n");
		return;
	}

	if((dwOffset + dwSize) > _u4FlashTotalSize)
	{
		LOG(0, "MUSB: Image is larger than flash size\n");
		return;
	}

	if(dwOffset == 0xFFFF0000){
		return;
	}

	// disable ISR
	vRegWrite4B(0x240c, 0xFFFFFFFF);
	// disable watchdog
	vRegWrite4B(0x8000, 0x0);

	HAL_GetTime(&TimeNow);
	HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
	LOG(0, "Elapse time %d sec\n", TimeDelta.u4Seconds);

	// Write
	pBinBuf2 = pBinBuf;
	i4Size = i4FlashSize;
	
	while(i4Size > 0)
	{
		if(FlashWrite(dwOffset, (UINT32)pBinBuf2, dwBlkSize) == 0)
		{
			rCmd.dwData = (UINT32)(i4Size - dwBlkSize);
    		xfercount = MUSB_CdcSyncTx((UINT8 *)&rCmd, sizeof(MUSB_CMD_SET));
    		if(xfercount != sizeof(MUSB_CMD_SET))
    		{
        		LOG(0, "USB Write error count = %d, xfercount = %d.\n",
        			sizeof(MUSB_CMD_SET), xfercount);
    		}
			LOG(0,"0x%x\n", rCmd.dwData);
		}else{
			return;
		}
		dwOffset += dwBlkSize;
		i4Size -= dwBlkSize;
		pBinBuf2 += dwBlkSize;
	}

	x_mem_free(pBinBuf);

	HAL_GetTime(&TimeNow);
	HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeNow);
	LOG(0, "Elapse time %d sec\n", TimeDelta.u4Seconds);
	
	if(i4Size > 0)
	{
		LOG(0, "Flash Upgrade Failed\n\n");
	}else
	{			
		LOG(0,"Finish Flash Upgrade!\n\n");
	}
}


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

#endif // SUPPORT_USBCDC

#define USB_EEPROM_MAGIC 0x73

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
		
		LOG(0, "USB Dump Enabled!\n");	

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
	UINT32 dwAddr, dwSize;
	UINT32 dwIndex;
	
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
		
		GDMA_Init();
		
		vGetScalerDRAMInfo(&rDramDumpInfo);		
		dwScalerfDRAMSize = (fgHalf ? rDramDumpInfo.dwSize/2 : rDramDumpInfo.dwSize);

		for(dwIndex=0; dwIndex < dwFrameCount; dwIndex++)
		{
			dwAddr = (UINT32)x_mem_alloc(dwScalerfDRAMSize);
			if(dwAddr == 0)
			{
				LOG(3, "Memory Allocation Failed\n");
				break;
			}				
			_MMAP_Allocate(dwAddr, dwScalerfDRAMSize);							
		}		
		wDump_MMAP_RealSize = wDump_MMAP_Size;
		dwScalerCpyAddr = (UINT32)0;
		dwScalerCpyEndAddr = dwScalerfDRAMSize* wDump_MMAP_Size;
		
		LOG(0, "Frame Number=%d, Dump Address=0x%08X, End Adddress=0x%08X\n", 
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
				x_mem_free((void*)Dump_MMAP[dwIndex]);											
				Dump_MMAP[dwIndex] = NULL;
			}
		}						
		wDump_MMAP_Size = 0;			
		wDump_MMAP_RealSize = 0;
		GDMA_Stop();				
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
		LOG(0, "New Frame=%d, Total Frame=%d\n", dwIndex, wDump_MMAP_Size);
	}
	else	
	{		
    	LOG(0, "scaler_dd [on/off]\n");
	}
#endif
}

void CopyScalerDRAM(void)
{
#ifdef SUPPORT_USBCDC
	static int count=0;
	HAL_TIME_T StartTime, EndTime, DeltaTime;
	tagDramDumpInfo_T rDramDumpInfo;		
	
	if(fgScalerVideoDump && wDump_MMAP_Size)
	{		
		vGetScalerDRAMInfo(&rDramDumpInfo);		
		HAL_GetTime(&StartTime);		
		GDMA_MemCpy((void*)(_MMAP_Addr(dwScalerCpyAddr)), (void*)(rDramDumpInfo.dwAddr), dwScalerfDRAMSize);	
		HAL_GetTime(&EndTime);	
		HAL_GetDeltaTime(&DeltaTime, &StartTime, &EndTime);			
		
		LOG(0, "%d: Memcpy from 0x%x to 0x%x in size 0x%x at time %d:%d, during %d us\n", 
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
			LOG(0, "End of scaler dramp dump\n");			
		}
	}	
#endif	
}

UINT32 FlashGetBlkSize(void)
{
#if defined(CC_NAND_ENABLE)
    NANDDEV_T _NandDev;
#endif
    
	if(DRVCUST_OptGet(eFlashPqUseNandFalsh))
	{
#if defined(CC_NAND_ENABLE)
        if(NAND_GetInfo(&_NandDev) != 0)
        {
    		return 0;
        }
        else
        {
    		return _NandDev.u4BlkSz;
        }
#else
        return 0;
#endif
	}
    else
	{
		return 0x10000;
	}
}


#if defined(CC_NAND_ENABLE)
static UINT32 rbBuf128KB[32*1024];
static INT8 NandFlashWrite(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size)
{
    UINT32 u4BlkSize, i;
    UINT32 *pu4Cmp;

    NANDDEV_T _NandDev;
    NAND_CONTROL_T rNewCtrl;
    UINT32 u4BlkIdx, u4PgIdx, u4PgPerBlk;
    UINT32 u4WriteSize = 0;
	INT32 _u4ImageStartAddr;
	INT32 i4Size = u4Size;

	_u4ImageStartAddr = u4Offset;

    if(NAND_GetInfo(&_NandDev) != 0)
    {
    	LOG(0, "Cannot NAND_GetInfo\n");
		return -1;
    }

    // Set new control
    rNewCtrl.fgIsr = FALSE;
    rNewCtrl.fgDMA = FALSE;
    NAND_SetControl(NAND_CTRL_FLAGS_DMA | NAND_CTRL_FLAGS_ISR, &rNewCtrl);

    u4BlkSize = _NandDev.u4BlkSz;
    u4PgPerBlk = _NandDev.u4BlkSz / _NandDev.u4PgSz;

    if ( (_u4ImageStartAddr & (u4BlkSize-1) ))
    {
        LOG(0, "image write address is not %d Byte alignment\n", u4BlkSize);
        return -1;
    }
    
    while(i4Size > 0)
    {
        if((_u4ImageStartAddr+i4Size) >= _NandDev.u4TotalSize)
        {
            LOG(0, "Offset too large!\n");
            return -1;
        }

        u4PgIdx = _u4ImageStartAddr / _NandDev.u4PgSz;
        u4BlkIdx = _u4ImageStartAddr / u4BlkSize;
        
        if ( (_u4ImageStartAddr & (u4BlkSize-1) ) == 0)
        {
            if(NAND_IsBadBlk(u4BlkIdx))
            {
                NAND_MarkBadBlk(u4BlkIdx);
                _u4ImageStartAddr += u4BlkSize;
                LOG(3, "Bad block %d, addr 0x%x\n", u4BlkIdx, u4BlkIdx * u4BlkSize);
                continue;
            }
            
            if(NAND_Erase(u4PgIdx, u4PgPerBlk) != 0)
            {
                NAND_MarkBadBlk(u4BlkIdx);
                _u4ImageStartAddr += u4BlkSize;
                continue;
            }
            
            if(NAND_Read((UINT64)_u4ImageStartAddr, (UINT32)rbBuf128KB, u4BlkSize) != 0)
            {
                NAND_MarkBadBlk(u4BlkIdx);
                _u4ImageStartAddr += u4BlkSize;
                continue;
            }
            
            for (i = 0; i < u4BlkSize/4; i++)
            {
                if (0xFFFFFFFF != rbBuf128KB[i])
                {
                    break;
                }
            }
            
            if ( i != (u4BlkSize/4) )
            {
                NAND_MarkBadBlk(u4BlkIdx);
                _u4ImageStartAddr += u4BlkSize;
                LOG(0, "erase block is not 0xFF\n");
                continue;
            }

        }


        if (i4Size >= u4BlkSize)
        {
            u4WriteSize = u4BlkSize;
        }
        else
        {
            if ( i4Size < (u4BlkSize/2) )
            {
                u4WriteSize = (u4BlkSize /2);
            }
            else if ( (i4Size > (u4BlkSize/2)) && (i4Size && (u4BlkSize -1) ))
            {
                u4WriteSize = (u4BlkSize);
            }
            else
            {
                u4WriteSize = i4Size;
            }
        }
        if(NAND_Write((UINT64)_u4ImageStartAddr, u4MemPtr, u4WriteSize) != 0)
        {
            NAND_MarkBadBlk(u4BlkIdx);
            _u4ImageStartAddr += u4BlkSize;
            LOG(0, "Nand flash write fail: Offset:0x%06x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
            continue;
        }

        if(NAND_Read((UINT64)_u4ImageStartAddr, (UINT32)rbBuf128KB, u4WriteSize) != 0)
        {
            NAND_MarkBadBlk(u4BlkIdx);
            _u4ImageStartAddr += u4BlkSize;
            LOG(0, "Nand flash read fail: Offset:0x%06x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
            continue;
        }
        
        pu4Cmp = (UINT32 *)u4MemPtr;
        for (i = 0; i < u4WriteSize/4; i++)
        {
            if (pu4Cmp[i] != rbBuf128KB[i])
            {
                break;
            }
        }
        if ( i != (u4WriteSize/4) )
        {
            NAND_MarkBadBlk(u4BlkIdx);
            _u4ImageStartAddr += u4BlkSize;
            LOG(0, "cmpare fail\n");
            continue;
        }
        LOG(3, "Nand flash write complete: Offset:0x%06x, WriteSize:0x%x\n", _u4ImageStartAddr, u4WriteSize);

        u4MemPtr += u4WriteSize;
        _u4ImageStartAddr += u4WriteSize;
        i4Size -= u4WriteSize;				
    }
    
    return 0;
}

#endif // CC_NAND_ENABLE

#if !defined(CC_NAND_ENABLE)
static INT8 NorFlashWrite(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size)
{
	NOR_EraseBlk(u4Offset);
	return NOR_Write(u4Offset, u4MemPtr, u4Size);
}
#endif 


INT8 FlashWrite(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Size)
{

#if defined(CC_NAND_ENABLE)
	return NandFlashWrite(u4Offset, u4MemPtr, u4Size);
#else
	return NorFlashWrite(u4Offset, u4MemPtr, u4Size);	

#endif
}
