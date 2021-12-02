/********************************************************************************************
 *     LEGAL DISCLAIMER 
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *     
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
#ifndef _GRAPHICS3D_IF_C_
#define _GRAPHICS3D_IF_C_

#define T32_HOST 1

#include "x_drv_cli.h"
#include "x_serial.h"
#include "x_printf.h"
#include "x_os.h"
//#include "x_rtos.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_stl_lib.h"
#include "x_common.h"
#include "x_bim.h"
#include "x_rand.h"
//#include "drv_config.h"
//#include "drv_graphics3d.h"
//#include "drv_if_graphics3d.h"

#include "x_typedef.h"
#include "x_printf.h"
#include "x_debug.h"
//#include "drv_fs.h"
//#include <linux/string.h>
#include "x_hal_arm.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_timer.h"

#include "g3d_emu_if.h"
#include "x_bim.h"

#include "fbm_drvif.h"

#include "musb_if.h"
#include "x_blk_dev.h"
#include "x_fm.h"
#include "osd_drvif.h"

#define STATIC_MEMORY_DISPATCH  1
#define NON_CACHEABLE_DRAM_OFFSET (0x0)
#define ASSERT_AT_COMPARE_FAIL (0)

#define TEST_G3D_INTERRUPT (1)
#define PRINT_2ND_COMPARE (0)
#define PRINT_SKIP_MEM_ADDR (0)
#define TEST_DRAM_CHB (0)   
#define TEST_PROCESSING_TIME (0)   
#define LOAD_FROM_NAND (1)
#define MOUNT_USB (0)

#if MOUNT_USB
extern INT32 fm_init(VOID);
extern INT32 x_fm_fat_init(VOID *pv_data);
#endif

#if LOAD_FROM_NAND
#define BYTES_PER_READ 512
#define MAX_FILE_SIZE 18 * 1024 * 1024
#define MAX_LENGTH_FILE_NAME 128

static UINT32 _u4ImgSize = 0;

static UINT32 _u4MaliBufferBase = 0;
static UINT32 _u4MaliMMUBase = 0;
static UINT32 _u4MaliTmpBufBase = 0;
#if MOUNT_USB
static HANDLE_T _hMonitorMsgq = NULL;
static HANDLE_T _hMonitorThread = NULL;

static HANDLE_T _hMountSema = NULL;
static CHAR _szMountPath[MAX_LENGTH_FILE_NAME];
static CHAR _szDevPath[MAX_LENGTH_FILE_NAME];
#endif
#ifndef __KERNEL__
static HANDLE_T _hImgFile = 0;
#endif
static CHAR _szCurrFile[MAX_LENGTH_FILE_NAME];
#endif

/*-----------------------------------------------------------------------------
                    function extension
 ----------------------------------------------------------------------------*/
#define x_alloc_aligned_dma_mem BSP_AllocAlignedDmaMemory
#define x_free_aligned_dma_mem BSP_FreeAlignedDmaMemory

/*-----------------------------------------------------------------------------
                    data define
 ----------------------------------------------------------------------------*/
    CHAR    compname[10][256];
    CHAR    goldname[10][256];
    UINT32  cmpcnt = 0;
    UINT32  goldcnt = 0;    
    BOOL    fgMakeFileParsed = FALSE;
    INT32   u4DumpMemCount[10];
    INT32   u4DumpRegCount[10];
    CHAR *GPMmuBuffer = NULL;
    CHAR *pGPMmuBuffer = NULL;
#if TEST_DRAM_CHB    
    CHAR *pGPPhyMemBuff = (CHAR*)0x47a00000; // 15M;  NULL;
#else
    CHAR *pGPPhyMemBuff = (CHAR*)0x07a00000; // 15M;  NULL;
#endif
    CHAR *pAlignedGPMemBuff = NULL;
    CHAR *PPMmuBuffer = NULL;
    CHAR *pPPMmuBuffer = NULL;
//    CHAR *pPPPhyMemBuff = (CHAR*)0x01800000;
#if TEST_DRAM_CHB  
    CHAR *pPPPhyMemBuff = (CHAR*)0x47a00000;
#else
    CHAR *pPPPhyMemBuff = (CHAR*)0x07a00000;
#endif
    CHAR *pAlignedPPMemBuff = NULL;    
    BOOL  fgIsGPMmu = FALSE;
    BOOL  fgIsPPMmu = FALSE;

#if     STATIC_MEMORY_DISPATCH
    CHAR *pcStaticTotalBuffer = NULL;
    CHAR *pHexFileBuffer = NULL;
    CHAR *pCofigFileBuffer = NULL;
    CHAR *pListFileBuffer = NULL;
#endif
    BOOL  fgIsPPOutBuff = FALSE;
    CHAR *pPPOutBuffAddr = NULL;

    BOOL fgMaliMemStartFrom0 = TRUE;

   static UINT32 u4GPDTE = 0;
   static UINT32 u4PPDTE = 0;

static CHAR _szEmuListName[256] = "Z:\\g3d_emulation\\g3d_emu_list.txt";

static HAL_TIME_T _rTimeGPTrigger, _rTimePPFinish, _rActiveTime;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
typedef struct
{
  INT32 handle;
  INT32 offset;
  UINT32 filesize;
}
FILE;

/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

#define MALI_REG_OFFSET   0x40000

#define HEX_DEFAULT 0
#define HEX_LOAD    1
#define HEX_COMP_REG     2
#define HEX_COMP_MEM    3

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
//t32 host command
enum
{
	E_T32_NOP,
	E_T32_EXEC,
	E_T32_RESTART,
	E_T32_EXIT,
	E_T32_EXEC_RELOAD_SYM,
	E_T32_DETECT_HOST,
	E_T32_END
};

#if T32_HOST
extern INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
#endif

/*-----------------------------------------------------------------------------
                    code start
 ----------------------------------------------------------------------------*/

#define vWriteReg(dAddr, dVal)  *(volatile UINT32 *)(IO_BASE + dAddr) = dVal
#define u4ReadReg(dAddr)         *(volatile UINT32 *)(IO_BASE + dAddr)

#if TEST_DRAM_CHB    
#define vWriteMem(dAddr, dVal)  *(volatile UINT32 *)(dAddr) = dVal
#define u4ReadMem(dAddr)         *(volatile UINT32 *)(dAddr)
#else
#define vWriteMem(dAddr, dVal)  *(volatile UINT32 *)(DRAM_A_BASE + dAddr) = dVal
#define u4ReadMem(dAddr)         *(volatile UINT32 *)(DRAM_A_BASE + dAddr)
#endif

#if TEST_G3D_INTERRUPT
static x_os_isr_fct _pfnOldIsr;
static HANDLE_T _hG3dIsrSema = 0;
static HANDLE_T _hG3dPpIsrSema = 0;
#endif

void *x_memset_2(void *s, int c, UINT32 n)
{
    UINT32 i;
    UINT8* ptr = (UINT8*)s;    
    for(i = 0; i < n; i++)
    {
        *(ptr + i) = (UINT8)c;
    }
    return (void*)ptr;
}


void vMaliWriteReg(UINT32 u4VDecID, UINT32 u4Addr, UINT32 u4Val)
{
        vWriteReg(MALI_REG_OFFSET + u4Addr, u4Val);
}

UINT32 u4MaliReadReg(UINT32 u4VDecID, UINT32 u4Addr)
{
        return (u4ReadReg(MALI_REG_OFFSET + u4Addr));
}


void vMaliWriteMem(UINT32 u4VDecID, UINT32 u4Addr, UINT32 u4Val)
{
        vWriteMem(u4Addr, u4Val);
}

UINT32 u4MaliReadMem(UINT32 u4VDecID, UINT32 u4Addr)
{
        return (u4ReadMem(u4Addr));
}

#if LOAD_FROM_NAND

INT32 _ReadFile(const CHAR* szFileName)
{

    UINT32 u4Read = 0;
    FM_FILE_INFO_T  rFileInfo;
    UINT8 *pu1Buf;

    rFileInfo.ui8_size = 0;
    _u4ImgSize = 0;

#ifndef __KERNEL__
    // Open JPEG file
    if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, szFileName, FM_READ_ONLY, 0777, FALSE, &_hImgFile))
    {
        printf("Open file %s fail!\n", szFileName);
        return -1024;
    }

    // Get file info
    if (FMR_OK != x_fm_get_info_by_handle(_hImgFile, &rFileInfo))
    {
        printf("Get file %s info fail!\n", szFileName);
        return -1024;
    }
#endif

    // Get file size
    if (rFileInfo.ui8_size > MAX_FILE_SIZE - 512)
    {
        printf("File %s too large!\n", szFileName);
        return -1024;
    }

    // Read file into buffer, max 4 MB
    pu1Buf = (UINT8*)_u4MaliTmpBufBase;

    while (1)
    {
#ifndef __KERNEL__
        if (FMR_OK != x_fm_read(_hImgFile, pu1Buf, BYTES_PER_READ, &u4Read))
        {
            printf("Read file %s fail!\n", szFileName);
            x_fm_close(_hImgFile);
            return -1024;
        }
#endif
        //LOG(5, "Read %d\n", _u4ImgSize);

        _u4ImgSize += u4Read;
        pu1Buf += u4Read;

        if (u4Read < BYTES_PER_READ)
        {
            break;
        }

    }

    return 0;

}

#if MOUNT_USB
static VOID  _MusbCallback(UINT32 u4Tag, INT32 i4Status)
{
    UINT32 u4Msg;

    if (i4Status == BLKDEV_STATUS_READY)
    {
        u4Msg = i4Status;
        x_msg_q_send(_hMonitorMsgq, &u4Msg, sizeof(UINT32), 200);
    }
    else if (i4Status == BLKDEV_STATUS_ABSENT)
    {
        u4Msg = i4Status;
        x_msg_q_send(_hMonitorMsgq, &u4Msg, sizeof(UINT32), 200);
    }
    return;
}


static INT32 _Mount(void)
{
    INT32 i4_ret;
    UINT32 ui4_part_ns;

    UINT32 ui4_cnt;

    /*  1. dev point creation */

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/dev", 0666);

    if (i4_ret == FMR_OK)
    {
        printf("/dev is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            printf("/dev is already existed.  That is OK.\n");
        }
        else
        {
            printf("make /dev fail\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  2. mount point creation */

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        printf("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            printf("/mnt is already existed.  That is OK.\n");
        }
        else
        {
            printf("make /mnt fail\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  3. make entry for USB */

    i4_ret = x_fm_make_entry(FM_ROOT_HANDLE,
                             "/dev/usb",
                             FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0660,
                             FM_DRVT_USB_MASS_STORAGE,
                             0);

    if (i4_ret == FMR_OK)
    {
        printf("/dev/usb is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            printf("/dev/usb is already existed.  That is OK.\n");
        }
        else
        {
            printf("make /dev/usb fail\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  4. parse drive  */

    i4_ret = x_fm_parse_drive(FM_ROOT_HANDLE,
                              "/dev/usb",
                              (VOID *) NULL);

    if (i4_ret != FMR_OK)
    {
        printf("parse drive error.");
    }

    /*  5. get partition ns */

    i4_ret = x_fm_get_part_ns(FM_ROOT_HANDLE,
                              (const CHAR *) "/dev/usb",
                              &ui4_part_ns);

    if (i4_ret != FMR_OK)
    {
        printf("x_fm_get_part_ns fails.\n");
        printf("return value:%d\n", i4_ret);
        goto func_exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_part_ns; ui4_cnt++)
    {
        FM_PART_INFO_T t_part_info;

        CHAR ps_dev_name[32];
        CHAR ps_mnt_name[32];

        i4_ret = x_fm_get_part_info(FM_ROOT_HANDLE,
                                    (const CHAR *) "/dev/usb",
                                    ui4_cnt,
                                    &t_part_info);

        if (i4_ret != FMR_OK)
        {
            printf("x_fm_get_part_info fails.\n");
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }

        x_strcpy((CHAR *) ps_dev_name, "/dev/");
        x_strcpy((CHAR *) ps_mnt_name, "/mnt/");

        x_strcat((CHAR *) ps_dev_name, (const CHAR *) t_part_info.ps_part_name);
        x_strcat((CHAR *) ps_mnt_name, (const CHAR *) t_part_info.ps_part_name);

        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mnt_name, 0666);

        if (i4_ret == FMR_OK)
        {
            printf("%s is created.\n", ps_mnt_name);
        }
        else if (i4_ret != FMR_OK)
        {
            if (i4_ret == FMR_EXIST)
            {
                printf("%s is already existed.  That is OK.\n", ps_mnt_name);
            }
            else
            {
                printf("make %s fail.\n", ps_mnt_name);
                printf("return value:%d\n", i4_ret);
                goto func_exit;
            }
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_name, FM_ROOT_HANDLE, ps_mnt_name);

        if (i4_ret == FMR_OK)
        {
            x_strncpy(_szMountPath, ps_mnt_name, x_strlen(ps_mnt_name));
            x_strncpy(_szDevPath, ps_dev_name, x_strlen(ps_dev_name));
            printf("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);
        }
        if (i4_ret != FMR_OK)
        {
            printf("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);
            printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

func_exit:

    return 0;
}

static INT32 _Umount(void)
{
    INT32 i4_ret;
    HANDLE_T h_dir;

    i4_ret = x_fm_umount(FM_ROOT_HANDLE, _szMountPath);

    if (i4_ret == FMR_OK)
    {
        printf("%s is unmounted.\n", _szMountPath);
        x_memset(_szMountPath, 0, MAX_LENGTH_FILE_NAME);
    }
    if (i4_ret != FMR_OK)
    {
        printf("ERR: unmount %s fail.\n", _szMountPath);
        printf("return value:%d\n", i4_ret);
        goto func_exit;
    }

    i4_ret = x_fm_set_dir_path(FM_ROOT_HANDLE, "/dev", &h_dir);

    if (i4_ret == FMR_OK)
    {
        printf("/dev is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        printf("/dev is not created.\n");
    }

    i4_ret = x_fm_release_drive(h_dir, "usb");

    if (i4_ret == FMR_OK)
    {
        printf("/dev/usb is released.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        printf("Release drive error.\n");
    }

    x_fm_close(h_dir);

#if 0
    i4_ret = x_fm_delete_entry(FM_ROOT_HANDLE,
                               "/dev/usb",
                               FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0660);

    if (i4_ret == FMR_OK)
    {
        printf("/dev/usb is deleted.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        printf("delete /dev/usb fail\n");
        printf("return value:%d\n", i4_ret);
        goto func_exit;
    }
#endif

func_exit:

    return i4_ret;
}

static VOID _MonitorThread(VOID* pvArg)
{
    UINT16 u2Idx;
    UINT32 u4Msg;
    SIZE_T rSize = sizeof(UINT32);
    while (1)
    {
        x_msg_q_receive(&u2Idx, &u4Msg, &rSize, &_hMonitorMsgq, 1, X_MSGQ_OPTION_WAIT);
        if (u4Msg == BLKDEV_STATUS_READY)
        {
            VERIFY(_Mount() == 0);
            x_sema_unlock(_hMountSema);
        }
        else if (u4Msg == BLKDEV_STATUS_ABSENT)
        {
            // Send virtual EXIT key event when MM is playing before umount USB disk, or system crash
            VERIFY(_Umount() == 0);
        }
    }
}

#endif
INT32 _LoadPattern(void)
{

    UINT32* pu4SrcPtr = (UINT32*)_u4MaliTmpBufBase;
    UINT32* pu4DstPtr = (UINT32*)_u4MaliBufferBase;    
    UINT32 u4LowerBound;
    UINT32 u4UpperBound;    
    UINT32 u4Size;        
    UNUSED(u4UpperBound);
    while(1)
    {
        u4LowerBound = *pu4SrcPtr;
        if(u4LowerBound == 0x0)
        {
            break;
        }
        pu4SrcPtr++;        
        u4UpperBound = *pu4SrcPtr;        
        pu4SrcPtr++;
        u4Size = *pu4SrcPtr;
        pu4SrcPtr++;        
        pu4DstPtr = (UINT32*)(_u4MaliBufferBase + (u4LowerBound - 0x7a00000));
        x_memcpy(pu4DstPtr, pu4SrcPtr, u4Size);
        pu4SrcPtr+=(u4Size/4);        
    }
    return 0;
}
#endif
#if TEST_G3D_INTERRUPT
static VOID _Gfx3dIsr(UINT16 u2Vector)
{
    if(u2Vector == VECTOR_GFX3D_GP)
    {
        UINT32 u4Gp_Irq_Raw;
        UINT32 u4Gp_Irq_Clear;        
        UINT32 u4Gp_Irq_Mask;                
        UINT32 u4Gp_Irq_Status;  
        UNUSED(u4Gp_Irq_Raw);        UNUSED(u4Gp_Irq_Clear);        UNUSED(u4Gp_Irq_Mask);        UNUSED(u4Gp_Irq_Status);
        u4Gp_Irq_Raw = u4MaliReadReg(0, 0x24); 
        u4Gp_Irq_Clear = u4MaliReadReg(0, 0x28); 
        u4Gp_Irq_Mask = u4MaliReadReg(0, 0x2c); 
        u4Gp_Irq_Status = u4MaliReadReg(0, 0x30);
        IO_WRITE32(0xF0040000, 0x28, 0xffffffff);        
#if !TEST_PROCESSING_TIME && !defined(__MODEL_slt__)             
        Printf("[G3D] GP interrupts!\n");          
        Printf("[G3D] GP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", 
        	u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status);     
#endif        
        x_sema_unlock(_hG3dIsrSema);
    }
    else if(u2Vector == VECTOR_GFX3D_PP)
    {
        UINT32 u4Pp_Irq_Raw;
        UINT32 u4Pp_Irq_Clear;        
        UINT32 u4Pp_Irq_Mask;                
        UINT32 u4Pp_Irq_Status;            
        UNUSED(u4Pp_Irq_Raw);        UNUSED(u4Pp_Irq_Clear);        UNUSED(u4Pp_Irq_Mask);        UNUSED(u4Pp_Irq_Status);        
        HAL_GetTime(&_rTimePPFinish);        
        HAL_GetDeltaTime(&_rActiveTime, &_rTimeGPTrigger, &_rTimePPFinish);        
        u4Pp_Irq_Raw = u4MaliReadReg(0, 0x9020); 
        u4Pp_Irq_Clear = u4MaliReadReg(0, 0x9024);
        u4Pp_Irq_Mask = u4MaliReadReg(0, 0x9028);
        u4Pp_Irq_Status = u4MaliReadReg(0, 0x902c);
        IO_WRITE32(0xF0049000, 0x24, 0xffffffff);    
#ifndef __MODEL_slt__
        Printf("[G3D] PP interrupts!\n");            
        Printf("[G3D] PP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", 
        	u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status);                       
        Printf("[G3D] Total frame time: %d sec %d micro!\n", _rActiveTime.u4Seconds, _rActiveTime.u4Micros);                    
#endif        
        x_sema_unlock(_hG3dPpIsrSema);        
    }
    else if(u2Vector == VECTOR_GFX3D_GPMMU)
    {
        IO_WRITE32(0xF0043000, 0x18, 0xffffffff);        
        Printf("[G3D] GP MMU interrupts!\n");    
    }
    else if(u2Vector == VECTOR_GFX3D_PPMMU)
    {
        IO_WRITE32(0xF0044000, 0x18, 0xffffffff);            
        Printf("[G3D] PP MMU interrupts!\n");    
    }    
    else if(u2Vector == VECTOR_GFX3D_PMU)
    {
        IO_WRITE32(0xF0042000, 0x18, 0xffffffff);                
        Printf("[G3D] PM interrupts!\n");    
    }
    else
    {
        Printf("[G3D] Unhook interrupts!\n");    
        ASSERT(0);
    }
}
#endif

INT32 i4Graphics3D_Drv_Init(G3D_EMU_CONF_T rConf)
{
   fgMaliMemStartFrom0 = rConf.fgMemoryStartFromZero;
   if(rConf.szEmuListName)
   {
       x_strncpy(_szEmuListName, rConf.szEmuListName, x_strlen(rConf.szEmuListName));
   }

#if !LOAD_FROM_NAND
   if(!u4GPDTE)
   {
//       u4GPDTE = 0x00E00000;      
//       u4GPDTE = (UINT32) (x_alloc_aligned_dma_mem(4 * 1024 * 1024, 0x1000));      // for gl bench, simmark
//       u4GPDTE = (UINT32) (x_alloc_aligned_dma_mem(176 * 1024 * 1024, 0x1000));      // for 3dmm11
       u4GPDTE = (UINT32) (x_alloc_aligned_dma_mem(228 * 1024 * 1024, 0x1000));      // for 3dmm11       
   }
   if(!u4PPDTE)
   {
//       u4GPDTE = 0x00E00000;      
       u4PPDTE = (UINT32) (x_alloc_aligned_dma_mem(4 * 1024 * 1024, 0x1000));      
   }
   Printf("GP DTE: 0x%8x\n", u4GPDTE);
   Printf("PP DTE: 0x%8x\n", u4PPDTE);
#endif

   // Set GRA3D clock to vdec pll
   IO_WRITE32(0xF000D000, 0x314, 0xD);
   
#if TEST_G3D_INTERRUPT
   if(_hG3dIsrSema == 0)
   {
       VERIFY(x_reg_isr(VECTOR_GFX3D_GP, _Gfx3dIsr, &_pfnOldIsr) == OSR_OK);
       VERIFY(x_reg_isr(VECTOR_GFX3D_PP, _Gfx3dIsr, &_pfnOldIsr) == OSR_OK);
       VERIFY(x_reg_isr(VECTOR_GFX3D_GPMMU, _Gfx3dIsr, &_pfnOldIsr) == OSR_OK);   
       VERIFY(x_reg_isr(VECTOR_GFX3D_PPMMU, _Gfx3dIsr, &_pfnOldIsr) == OSR_OK);      
       x_sema_create(&_hG3dIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
       x_sema_create(&_hG3dPpIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);       
   }
#endif

#if LOAD_FROM_NAND
    {
        UINT32 i;

        if(_u4MaliTmpBufBase == 0)
        {        
            _u4MaliTmpBufBase = (UINT32)x_alloc_aligned_dma_mem(18 * 1048576, 4);
        }       
#if 0        
        prPool = FBM_GetPoolInfo(FBM_POOL_TYPE_3DGFX_BUFFER);        
        _u4MaliMMUBase = prPool->u4Addr;
        _u4MaliBufferBase = prPool->u4Addr + 4 * 1024 * 1024;
#else
       if(_u4MaliMMUBase == 0)
       {
           _u4MaliMMUBase = (UINT32)x_alloc_aligned_dma_mem(4 * 1048576, 0x1000);
           _u4MaliBufferBase = (UINT32)x_alloc_aligned_dma_mem(18 * 1048576, 0x1000);
       }
#endif
#ifndef __MODEL_slt__
        Printf("3D temp buffer: 0x%8x\n", _u4MaliTmpBufBase);	
        Printf("3D mali MMU buffer: 0x%8x\n", _u4MaliMMUBase);                
        Printf("3D mali GP/PP buffer: 0x%8x\n", _u4MaliBufferBase);        
#endif
        // Setting up MMU
        x_memset((void*)_u4MaliMMUBase, 0, 4 * 1024 * 1024);
        for(i = 0; i < 13; i++)
        {
            *(((UINT32*)_u4MaliMMUBase)+i) = _u4MaliMMUBase + 0x1001 + (i * 0x1000);   
        }
        for(i = 0; i < 4608; i++)
        {        
      	     *(UINT32*)(_u4MaliMMUBase + 0x1000 + (i*4)) = _u4MaliBufferBase + (i * 0x1000) + 7;
        }	

        // Clear buffer
        x_memset((void*)_u4MaliTmpBufBase, 0x0, 18 * 1048576);

#if MOUNT_USB
        if(_hMountSema == NULL)
        {
            UINT32 u4Tag;
            x_sema_create(&_hMountSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    
            x_msg_q_create(&_hMonitorMsgq, "MonMsgq", sizeof(UINT32), 256);
        
            x_thread_create(&_hMonitorThread, "MonThd", 2048, 200, _MonitorThread, 0, NULL);
    
            MUSB_Init(1);
        
            if (fm_init() != FMR_OK)
            {
                Printf("\nError: Init FM fail\n");
            }
            if (x_fm_fat_init(NULL) != FMR_OK)
            {
                Printf("\nError: Init FAT fail\n");
            }
        
            MUSB_SetHotswapCallback(u4Tag, _MusbCallback);
        }
#endif        
    }
#endif

   return 0;
}

INT32 i4Graphics3D_Drv_Uninit(void)
{
   x_free_aligned_dma_mem(_u4MaliTmpBufBase);       
   return 0;
}

INT32 i4Graphics3D_Drv_Stop( UINT32              u4CmdBufHandle)
{
   return 0;
}


UINT32 M2P(UINT32 u4Mali)
{
    if(fgMaliMemStartFrom0)
    {
        return u4Mali + (UINT32)pGPPhyMemBuff;          
    }
    else
    {
        return u4Mali;
    }
}

/*static inline UINT32 P2M(UINT32 u4Phys)
{
    ASSERT(u4Phys < 0x1000000);
    return u4Phys - 0x1000000;
}*/

BOOL InitMaliGPMMU(UINT32 u4DTE, UINT32 u4PTE, UINT32 u4MemBase, UINT32 u4MemSize)
{
    UINT32 i;
    vMaliWriteReg(0, 0x3008, 0x1);      
    vMaliWriteReg(0, 0x3000, u4DTE + NON_CACHEABLE_DRAM_OFFSET);
    x_memset((void*)(u4DTE + NON_CACHEABLE_DRAM_OFFSET), 0, 4096);
    for(i = 0 ; i < (u4MemSize - 1) / 0x400000 + 1; i++)
    {
        *((UINT32*)u4DTE + i + NON_CACHEABLE_DRAM_OFFSET) = ((u4PTE + NON_CACHEABLE_DRAM_OFFSET) + (i * 0x1000)) | 0x1;
    }
    for(i = 0; i < 1024 * ((u4MemSize - 1) / 0x400000 + 1); i++)
    {
        *((UINT32*)u4PTE + i + NON_CACHEABLE_DRAM_OFFSET) = ((u4MemBase + NON_CACHEABLE_DRAM_OFFSET) + (i * 0x1000)) | 0x7;
        // for deal with 0x1100000 only
        /*
        if(((u4MemBase + (i * 0x1000)) | 0x7) > 0x10000000)
        {
            *((UINT32*)u4PTE + i) = ((u4MemBase + (i * 0x1000)) | 0x7) + 0x800000 - 0x10000000;
        }  
        */
    }
    vMaliWriteReg(0, 0x3008, 0x0);    
    return TRUE;
}

BOOL InitMaliPPMMU(UINT32 u4DTE, UINT32 u4PTE, UINT32 u4MemBase, UINT32 u4MemSize)
{
    UINT32 i;
    vMaliWriteReg(0, 0x4008, 0x1);      
    vMaliWriteReg(0, 0x4000, (u4DTE + NON_CACHEABLE_DRAM_OFFSET));
    x_memset((void*)(u4DTE + NON_CACHEABLE_DRAM_OFFSET), 0, 4096);
    for(i = 0 ; i < (u4MemSize - 1) / 0x400000 + 1; i++)
    {
        *((UINT32*)u4DTE + i + NON_CACHEABLE_DRAM_OFFSET) = ((u4PTE + NON_CACHEABLE_DRAM_OFFSET) + (i * 0x1000)) | 0x1;
    }
    for(i = 0; i < 1024 * ((u4MemSize - 1) / 0x400000 + 1); i++)
    {
        *((UINT32*)u4PTE + i + NON_CACHEABLE_DRAM_OFFSET) = ((u4MemBase + NON_CACHEABLE_DRAM_OFFSET) + (i * 0x1000)) | 0x7;
        // for deal with 0x1100000 only
        /*
        if(((u4MemBase + (i * 0x1000)) | 0x7) > 0x10000000)
        {
            *((UINT32*)u4PTE + i) = ((u4MemBase + (i * 0x1000)) | 0x7) + 0x800000 - 0x10000000;
        }  
        */
    }
    vMaliWriteReg(0, 0x4008, 0x0);    
    return TRUE;
}

#if !T32_HOST
BOOL fgReadFile(void* pvAddr, UINT32 u4Size, CHAR *strFileName, UINT32 u4OffSet, INT32 *piFd, UINT32 *pu4RealReadSize, UINT32 *pu4FileLength)
{
    FILE* pFile = NULL;
    UINT32 u4FileSize = 0;
    UINT32 u4ReadSize = u4Size;
    
    if ((NULL == pvAddr) || (0 == u4Size) || (NULL == strFileName) || (NULL == pu4RealReadSize))
      return FALSE;

    *pu4RealReadSize = 0;

    if((pFile = fopen(strFileName, "rb")) == NULL)
        return FALSE;
    
    //u4FileSize = u4fsize(pFile);
    u4FileSize = pFile->filesize;
    *pu4FileLength = u4FileSize;

    if (u4OffSet >= u4FileSize)
    {
        fclose(pFile);
        vVDecOutputDebugString("\n read offset(%d) > filesize(%d)", u4OffSet, u4FileSize);
        return FALSE;
    }

    if(fseek (pFile, u4OffSet, SEEK_SET))
    {
        fclose(pFile);
        vVDecOutputDebugString("\n seek fail: %s", strFileName);
        return FALSE;
    }

    if (u4Size > (u4FileSize - u4OffSet))
        u4ReadSize = u4FileSize - u4OffSet;

    *pu4RealReadSize = fread((void *)pvAddr, 1, u4ReadSize, pFile);
    fclose(pFile);

    if (u4ReadSize != *pu4RealReadSize)
    {
        Printf("\n read fail: %s", strFileName);
        return FALSE;
    }

    return TRUE;
}
#endif

UINT32 u4StrToInt (CHAR *adr1)
{    
    UINT32 value, idx, i;
    UINT32 oft = 0;

    value=0; idx=0;
    for(i=(8-x_strlen(adr1));i<8;i++)
    {
        if(adr1[idx]=='0') oft = 0;        
        else if(adr1[idx]=='1') oft = 1;
        else if(adr1[idx]=='2') oft = 2;
        else if(adr1[idx]=='3') oft = 3;
        else if(adr1[idx]=='4') oft = 4;
        else if(adr1[idx]=='5') oft = 5;
        else if(adr1[idx]=='6') oft = 6;
        else if(adr1[idx]=='7') oft = 7;
        else if(adr1[idx]=='8') oft = 8;
        else if(adr1[idx]=='9') oft = 9;
        else if(adr1[idx]=='a' || adr1[idx]=='A' ) oft = 10;
        else if(adr1[idx]=='b' || adr1[idx]=='B' ) oft = 11;
        else if(adr1[idx]=='c' || adr1[idx]=='C' ) oft = 12;
        else if(adr1[idx]=='d' || adr1[idx]=='D' ) oft = 13;
        else if(adr1[idx]=='e' || adr1[idx]=='E' ) oft = 14;
        else if(adr1[idx]=='f' || adr1[idx]=='F' ) oft = 15;
        else if(adr1[idx]=='x') oft = 0;        
        idx++;
        value = value + ((oft & 0xf) << ((7-i)*4));
    }

    return value;
}

UINT32 u4ConfigAddToRealAddr(UINT32 value)
{
    UINT32 new_adr0 = 0;

    if( (value>>16) == 0 )           new_adr0 = value & 0xFFF;
    else if( (value>>16) == 0x0100 ) new_adr0 = 0x1000 + (value & 0xFFF );
    else if( (value>>16) == 0x2000 ) new_adr0 = 0x2000 + (value & 0xFFF );
    else if( (value>>16) == 0x1000 ) new_adr0 = 0x8000 + (value & 0x1FFF);
    else if( (value>>16) == 0x1001 ) new_adr0 = 0xA000 + (value & 0x1FFF);
    else if( (value>>16) == 0x1002 ) new_adr0 = 0xC000 + (value & 0x1FFF);
    else if( (value>>16) == 0x1003 ) new_adr0 = 0xE000 + (value & 0x1FFF);
    else if( (value>>16) == 0x0200 ) new_adr0 = 0x3000 + (value & 0xFFF );
    else if( (value>>16) == 0x1200 ) new_adr0 = 0x4000 + (value & 0xFFF );
    else if( (value>>16) == 0x1201 ) new_adr0 = 0x5000 + (value & 0xFFF );
    else if( (value>>16) == 0x1202 ) new_adr0 = 0x6000 + (value & 0xFFF );
    else if( (value>>16) == 0x1203 ) new_adr0 = 0x7000 + (value & 0xFFF );

    return new_adr0;
}

BOOL fgG3DBufGetLine(CHAR *pBuf, UINT32 *pu4PrsSz, CHAR *pline, UINT32 u4RealSz)
{
    CHAR *pucLine;
    CHAR ucCurr;
    UINT32 idx;
    
    while(*pu4PrsSz < u4RealSz) 
    {  
        pucLine = ((CHAR*) pBuf) + (*pu4PrsSz);
        for (idx = 0; idx < 256; idx++)
        {
            ucCurr = *pucLine;
            if (ucCurr == '\n')
            {
                (*pu4PrsSz) ++;
                break;
            }
            else if (ucCurr == '\0')
            {
                if(idx == 0)
                {
                    return FALSE;
                }
                else
                {
                    break;
                }
            }
            
            (*pline) = ucCurr;
            (*pu4PrsSz) ++;
            pucLine++;
            pline++;
        }
        (*pline) = '\0';
        //Printf("line is : %s\n", pline);
        return TRUE;
    }
    return FALSE;
}

BOOL  fgIsValueEqual(UINT32 golden, UINT32 result)
{
#if 0   
    UINT32 g0, g1, g2, g3, g4, g5, g6, g7;
    UINT32 r0,  r1, r2,  r3, r4,  r5, r6,  r7;

    if (golden != result)
    {
        g0 = (golden >> 0x28) && 0xf;
        g1 = (golden >> 0x24) && 0xf;
        g2 = (golden >> 0x20) && 0xf;
        g3 = (golden >> 0x16) && 0xf;
        g4 = (golden >> 0x12) && 0xf;
        g5 = (golden >> 0x8) && 0xf;
        g6 = (golden >> 0x4) && 0xf;
        g7 = (golden            ) && 0xf;	

        r0 = (golden >> 0x28) && 0xf;
        r1 = (golden >> 0x24) && 0xf;
        r2 = (golden >> 0x20) && 0xf;
        r3 = (golden >> 0x16) && 0xf;
        r4 = (golden >> 0x12) && 0xf;
        r5 = (golden >> 0x8) && 0xf;
        r6 = (golden >> 0x4) && 0xf;
        r7 = (golden            ) && 0xf;	

        if (
          ( g0 != 0x2E &&  g0 != 0x58 && g0 !=0x78 && g0 != r0)  ||
          ( g1 != 0x2E &&  g1 != 0x58 && g1 !=0x78 && g1 != r1)  ||
          ( g2 != 0x2E &&  g2 != 0x58 && g2 !=0x78 && g2 != r2)  ||
          ( g3 != 0x2E &&  g3 != 0x58 && g3 !=0x78 && g3 != r3)  ||
          ( g4 != 0x2E &&  g4 != 0x58 && g4 !=0x78 && g4 != r4)  ||
          ( g5 != 0x2E &&  g5 != 0x58 && g5 !=0x78 && g5 != r5)  ||
          ( g6 != 0x2E &&  g6 != 0x58 && g6 !=0x78 && g6 != r6)  ||
          ( g7 != 0x2E &&  g7 != 0x58 && g7 !=0x78 && g7 != r7)  )
        {
           return FALSE;
        }

        return TRUE;

    }
#else
    if (golden != result)
    {
        return FALSE;    
    }
#endif
     return TRUE;
}

BOOL  fgIsValueEqual_2(CHAR* adr1, UINT32 val)
{
    UINT32 idx, i;
    CHAR str[15];

    x_snprintf(str, 15, "%08x", val);
    str[8] = '\0';
#if PRINT_2ND_COMPARE
    printf("2nd compare: %s vs %s\n", adr1, str);
#endif
    idx=0;
    for(i=(8-x_strlen(adr1));i<8;i++)
    {
        if(adr1[idx] != str[idx] && adr1[idx] != '.' && adr1[idx] != 'x')
        {
            return FALSE;
        }
    }
    return TRUE;
}

void vG3DEmuParseMemHex (CHAR *pMemName, UINT32 u4Offset, INT32 hex_action, UINT32 u4RdCount, UINT32 u4CompCnt, UINT32 u4CompAddr)
{
    CHAR str0 [256];
    CHAR mem_addr [256];
    UINT32 value;
    CHAR val0 [10];
    UINT32  r, i, j;
    CHAR *pMemBuf = NULL;
    INT32  iFd;
    UINT32 u4PrsSz = 0;
    UINT32 u4RealMemSize;
    UINT32 u4MemLength;
    UINT32 u4MemSz;
    CHAR line [256];    
    UINT32 addr =0; 
    UINT32 reg_addr;
    BOOL fgret;
    UINT32 cnt = 0;
    UINT32 mem_value = 0;
    INT32  u4SkipLine = u4RdCount / 16;
    INT32  u4CompLine = u4CompCnt / 16;
    BOOL   fgOpenSuccess = FALSE;
    UINT32 u4LineCnt = 0;

    x_strncpy(str0, pMemName, x_strlen(pMemName));
    Printf("\n%s ", str0);
    UNUSED(iFd);UNUSED(u4MemLength);UNUSED(u4MemSz);UNUSED(u4LineCnt);
#if STATIC_MEMORY_DISPATCH
//    pMemBuf = (CHAR*)  (0x2000000);
//    pMemBuf = (CHAR*)  (pHexFileBuffer);
    pMemBuf = (CHAR *) x_alloc_aligned_dma_mem(69 * 1024 * 1024, 1024);   
#else
    pMemBuf = (CHAR *) x_alloc_aligned_dma_mem(1024 * 1024, 1024);    
#endif
    if (pMemBuf == NULL)
    {
       Printf("Allocate Memory for MemHex File Fail: %s\n ", pMemName);
       ASSERT(0);
       return;
    }
#if T32_HOST
   x_memset_2(pMemBuf, (int)'\0', 69* 1024 * 1024);
   u4RealMemSize= 69 * 1024 * 1024;
   u4MemLength = 69 * 1024 * 1024;
#ifndef __KERNEL__
    T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pMemName, pMemBuf);   
#endif
//    u4RealMemSize = u4MemLength = x_strlen(pMemName);    
    fgOpenSuccess = TRUE;
#else    
    fgOpenSuccess = fgReadFile((void*) pMemBuf, 15*1024*1024, pMemName, 0, &iFd, &u4RealMemSize, &u4MemLength);
#endif
    if (fgOpenSuccess == FALSE)
    {
       Printf("Open MemHex Fail: %s\n ", pMemName);
       ASSERT(0);
       return;
    }

    switch(hex_action)
    {
              case HEX_LOAD:
                  Printf("LoadMem: \n");
                  break;
              case HEX_COMP_MEM:
                  Printf("CompMem: \n");                
                  break;
              case HEX_COMP_REG:
             	    reg_addr = u4ConfigAddToRealAddr(addr);
                  Printf("CompReg: \n");
                  break;

              default:
                  break;
    }
        
    u4MemSz = 0;
    while( (u4PrsSz < u4RealMemSize) && (u4CompLine >0) ) 
    {                
        fgret = fgG3DBufGetLine(pMemBuf, &u4PrsSz, line, u4RealMemSize);
        if (!fgret)
            break;
                            
        if (line[0] == '#')
        {
            //This is comment line. Do nothing.
        }
        else
#if TEST_DRAM_CHB
        if (line[0] == '0' || line[0] == '4')
#else
        if (line[0] == '0')
#endif        	
        {
            u4CompLine--;
            
            if (u4SkipLine > 0)
            {
                u4SkipLine --;
            }
            else
            {
            
            //Get Addr
            for (i=0; i < x_strlen(line); i++)
            {
                if (line[i] != ':' && line[i] != ' ')
                {
                    mem_addr[i] = line[i];
                }
                else
                {
                    mem_addr[i] = '\0';
                    break;
                }
            }

            addr = u4StrToInt(mem_addr);
            addr = addr + u4Offset;

            while(line[i] == ':' || line[i] == ' ')
            {        
                i++;
            }                        
            
            cnt = 0;
            for (j=i; j < x_strlen(line); )
            {
              for (r=0; r < 8; r++)
              {
                val0[r] = line[j];
                j++;
              }
              val0[8] = '\0';
              value = u4StrToInt(val0);
              //Printf("0x%08x ", value);              

              switch(hex_action)
              {
              case HEX_LOAD:                  
                  if ( (cnt == 0) &&  ((addr &0x3FFFF) == 0x0) )
                  {
                      Printf("HEX_LOAD_MEM: Mem[0x%08x] = 0x%08x \n", addr + (cnt*4), value);
                  }
                  else
                  if ( (cnt == 0) &&  ((addr &0xFFF) == 0x0) )
                  {
                      Printf("HEX_LOAD_MEM: ..\n");
                  }

                  /*
                  if(x_strncmp("xxxxxxxx", val0, 8) == 0)
                  {
                      Printf("Skip mem load addr: 0x%08x : 0x%08x -- 0x%08x\n", addr, value, mem_value);
                  }
                  else
                  */
                  {
             	        vMaliWriteMem(0, M2P(addr + (cnt*4)), value);
                  }
                  break;
              case HEX_COMP_MEM:
                  if ( (u4CompAddr == 0x0) ||
                  	   (u4CompAddr != 0x0 &&  ( (addr + (cnt*4)) >= u4CompAddr ) ) )
                  {
                      if ( (cnt == 0) &&  ((addr &0x3FFFF) == 0x0) )
                      {
                          Printf("HEX_COMP_MEM: Mem[0x%08x] = ", addr + (cnt*4));                  
                      }
                      mem_value = u4MaliReadMem(0, M2P(addr + (cnt*4)));

                      if ( (cnt == 0) &&  ((addr &0x3FFFF) == 0x0) )
                      {
                          Printf("0x%08x \n", mem_value);                  
                      }
                      if(x_strncmp("........", val0, 8) == 0)
                      {
#if PRINT_SKIP_MEM_ADDR                      
                          Printf("Skip mem addr: 0x%08x : 0x%08x -- 0x%08x\n", addr, value, mem_value);
#endif
                      }
                      else if (fgIsValueEqual (value, mem_value)  != TRUE)
                      {
                         if(!fgIsValueEqual_2(val0, mem_value))
                         {
                             Printf("CompMem Mismatch: 0x%08x : 0x%08x -- 0x%08x\n", addr, value, mem_value);
                             // ASSERT(0);                      
                             Printf("ASSERT!!!\n");  
                             goto ParseMemHex_EXIT;                          
#if ASSERT_AT_COMPARE_FAIL                            
                             while(1);
#endif               
                         }
                      }
                  }
                  else
                  {
                      //Skip Comp Mem
                      if ( (cnt == 0) &&  ((addr &0x3FFFF) == 0x0) )
                      {
                          Printf("HEX_COMP_MEM: Mem[0x%08x] : Skip\n", addr + (cnt*4));
                      }
                      else 
                      if ( (cnt == 0) &&  ((addr &0xFFF) == 0x0) )
                      {
                         Printf("HEX_COMP_MEM: .. : Skip\n", addr + (cnt*4));
                      }
                  }
                  break;
              case HEX_COMP_REG:              	
                  reg_addr = u4ConfigAddToRealAddr(addr);

                  if ( (cnt == 0) &&  ((reg_addr &0x3FFFF) == 0x0) )
                  {
                      Printf("HEX_COMP_REG: Mem[0x%08x] = ", reg_addr + (cnt*4));
                  }
                  else
                  if ( (cnt == 0) &&  ((addr &0xFFF) == 0x0) )
                  {
                      Printf("HEX_COMP_REG: ..\n");
                  }

                  mem_value = u4MaliReadReg(0, reg_addr + (cnt*4));
                  
                  if ( (cnt == 0) &&  ((reg_addr &0x3FFFF) == 0x0) )
                  {
                      Printf("0x%08x \n",  mem_value);
                  }

                  if(x_strncmp("........", val0, 8) == 0)
                  {
                      Printf("Skip reg addr: 0x%08x : 0x%08x -- 0x%08x\n", addr, value, mem_value);
                  }
                  else if (fgIsValueEqual (value, mem_value)  != TRUE)
                  {
                      Printf("CompReg Mismatch: 0x%08x : 0x%08x -- 0x%08x\n", addr, value, mem_value);
                      // ASSERT(0);                      
                      Printf("ASSERT!!!\n");    
                      goto ParseMemHex_EXIT;
#if ASSERT_AT_COMPARE_FAIL                                        
                      while(1);
#endif                      
                  }
                  break;

              default:
                  break;
              }
              cnt++;

              if (cnt==4)
              {
                  break;
              }

              while((line[j] == ':' || line[j] == ' ') && j < x_strlen(line) )
              {        
                j++;
              }
            }        
            //Printf("\n");            
            }
        }                                            
    } // 

ParseMemHex_EXIT:

    Printf("HEX_LAST_ADR: ADDR= 0x%08x\n", addr); 

#if     STATIC_MEMORY_DISPATCH
    x_free_aligned_dma_mem((UINT32)pMemBuf);
#else
    x_free_aligned_dma_mem((UINT32)pMemBuf);
#endif
}

BOOL fgRealChar(char p)
{
	if (p == 0x5F || p == 0x2E 
		|| (p >= 0x30 && p <= 0x39)
		|| (p >= 0x41 && p <= 0x5A)
		|| (p >= 0x61 && p <= 0x7A) )
	{
		return TRUE;
	}

	return FALSE;
}

void vG3DEmuParseMakefile (CHAR *pMakeName, CHAR *dumpname, INT32 u4CompType)
{
    CHAR str0 [256];    
    UINT32 i, j;
    CHAR *pMakeBuf = NULL;
    INT32  iFd;
    UINT32 u4PrsSz = 0;
    UINT32 u4RealSize;
    UINT32 u4Length;
    UINT32 u4Sz;
    CHAR line [256];        
    BOOL fgret;
    BOOL     fgDiffDump = FALSE;
    BOOL     fgDiffResult = FALSE;
    BOOL     fgKeepSearch = TRUE;
    INT32 i4StartIdx = 0;
    BOOL  fgOpenSuccess = FALSE;

    cmpcnt = 0;
    goldcnt = 0;    
    UNUSED(u4Length);UNUSED(u4Sz);UNUSED(iFd);

    x_strncpy(str0, pMakeName, x_strlen(pMakeName));
    Printf("\n%s ", str0);

#if STATIC_MEMORY_DISPATCH
//    pMakeBuf = (CHAR*) (0x11100000);
    pMakeBuf = (CHAR *) x_alloc_aligned_dma_mem(100 * 1024, 1024);   
#else
    pMakeBuf = (CHAR *) x_alloc_aligned_dma_mem(100 * 1024, 1024);    
#endif

    if (pMakeBuf == NULL)
    {
       Printf("Allocate Memory for Make File Fail: %s\n ", pMakeName);
       ASSERT(0);
       return;
    }
#if T32_HOST
   x_memset_2(pMakeBuf, (int)'\0', 100 * 1024);
   u4RealSize= 100 * 1024;
   u4Length = 100 * 1024;
#ifndef __KERNEL__
    T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", pMakeName, pMakeBuf);   
#endif
    //u4RealSize = u4Length = x_strlen(pMakeName);   
    fgOpenSuccess = TRUE;
#else
    fgOpenSuccess = fgReadFile((void*) pMakeBuf, 100*1024, pMakeName, 0, &iFd, &u4RealSize, &u4Length);
#endif
    if (fgOpenSuccess == FALSE)
    {
       Printf("Open Make Fail: %s\n ", pMakeName);
       ASSERT(0);
       return;
    }

    
    u4Sz = 0;
    while( u4PrsSz < u4RealSize) 
    {
        fgret = fgG3DBufGetLine(pMakeBuf, &u4PrsSz, line, u4RealSize);
        if (!fgret)
            break;

        if (line[0] == 'D' && line[1] == 'U' && line[2] == 'M' && line[3] == 'P' && line[4] == 'S')
        {
            i4StartIdx = 0;
            for (i=5; i < x_strlen(line); i++)
            {
                if (line[i] == '=')
                {
                   i4StartIdx = i+1;
                   break;
                }
                else
                if (line[i] == '+' && line[i+1] == '=')
                {
                   i4StartIdx = i+2;
                   break;
                }
            }
                        
            j = 0;
            for (i=i4StartIdx; i < x_strlen(line); i++)
            {
                if( j < 4 || j > 255)
                {
                    break;
                }
                if ( (line[i] == 0 || line[i] == '\n' || line[i] == '\r' || line[i] == '#')  &&  j > 0)
                {
                    goldname[goldcnt][j] = 0;

                    if (  (goldname[goldcnt][j-4] == '.')
                    	&& (goldname[goldcnt][j-3] == 'h')
                    	&& (goldname[goldcnt][j-2] == 'e')
                    	&& (goldname[goldcnt][j-1] == 'x') )
                    {
                       //do nothing.
                    }
                    else
                    {
                       //concate .hex
                       x_strncat(goldname[goldcnt], "_result.hex", x_strlen("_result.hex"));
                    }
                    goldcnt++;
                    j = 0;
                    break;
                }
                else
                if ( line[i] == ' ')
                {
                   if ( j > 0 )
                   {
                      goldname[goldcnt][j] = 0;

                      if (  (goldname[goldcnt][j-4] == '.')
                      	&& (goldname[goldcnt][j-3] == 'h')
                      	&& (goldname[goldcnt][j-2] == 'e')
                     	&& (goldname[goldcnt][j-1] == 'x') )
                      {
                         //do nothing.
                      }
                      else
                      {
                         //concate .hex
                         x_strncat(goldname[goldcnt], "_result.hex", x_strlen("_result.hex"));
                      }
                      goldcnt++;
                      j = 0;
                   }
                }
                else
                if (fgRealChar( line[i] ))
                {
                  goldname[goldcnt][j] = line[i];
                  j++;
                }
            }      

            if (j > 0)
            {
                    goldname[goldcnt][j] = 0;
                    
                    if (  (goldname[goldcnt][j-4] == '.')
                    	&& (goldname[goldcnt][j-3] == 'h')
                    	&& (goldname[goldcnt][j-2] == 'e')
                    	&& (goldname[goldcnt][j-1] == 'x') )
                    {
                       //do nothing.
                    }
                    else
                    {
                       //concate .hex
                       x_strncat(goldname[goldcnt], "_result.hex", x_strlen("_result.hex"));
                    }
                    goldcnt++;
                    j = 0;
            }
            
        }
        else
        if (line[0] == 'I' && line[1] == 'M' && line[2] == 'A' && line[3] == 'G' && line[4] == 'E' && line[5] == 'S' && line[6] == '=')
        {
            j = 0;
            for (i=7; i < x_strlen(line); i++)
            {
                if (line[i] == 0 || line[i] == '\n' || line[i] == '\r' || ((line[i] == ' ') && ( j > 0 )))
                {
                    goldname[goldcnt][j] = 0;

                    if (  (goldname[goldcnt][j-4] == '.')
                    	&& (goldname[goldcnt][j-3] == 'h')
                    	&& (goldname[goldcnt][j-2] == 'e')
                    	&& (goldname[goldcnt][j-1] == 'x') )
                    {
                       //do nothing.
                    }
                    else
                    {
                       //concate .hex
                       x_strncat(goldname[goldcnt], "_result.hex", x_strlen("_result.hex"));
                    }
                    goldcnt++;
                    j = 0;
                    break;
                }
                else
                {
                  goldname[goldcnt][j] = line[i];
                  j++;
                }
            }      

            if (j > 0)
            {
                    goldname[goldcnt][j] = 0;
                    
                    if (  (goldname[goldcnt][j-4] == '.')
                    	&& (goldname[goldcnt][j-3] == 'h')
                    	&& (goldname[goldcnt][j-2] == 'e')
                    	&& (goldname[goldcnt][j-1] == 'x') )
                    {
                       //do nothing.
                    }
                    else
                    {
                       //concate .hex
                       x_strncat(goldname[goldcnt], "_result.hex", x_strlen("_result.hex"));
                    }
                    goldcnt++;
                    j = 0;
            }
        }
        else
        if (line[0] == 'D' &&
			line[1] == 'I' &&
			line[2] == 'F' &&
			line[3] == 'F' &&
			line[4] == '_' &&
			line[5] == 'D' &&
			line[6] == 'U' &&
			line[7] == 'M' &&
			line[8] == 'P' )
		{
		     i4StartIdx = 0;
                   for (i=9; i < x_strlen(line); i++)
                   {
                        if (line[i] == '=')
                        {
                           i4StartIdx = i+1;
                           break;
                        }
                    }
		      		      
			fgDiffDump = TRUE;
	              fgDiffResult = FALSE;
			fgKeepSearch = FALSE;
			j=0;
			for (i=i4StartIdx; i < x_strlen(line); i++)
			{
				if (line[i] == 0 || line[i] == '\n' || line[i] == '\r' )
				{
					if (j > 0)
					{
					    compname[cmpcnt][j] = 0;				
				           cmpcnt++;
					}
					j = 0;
					break;
				}
				else
				if (line[i] == ' ')
				{				
					if (j > 0)
					{					  
					  compname[cmpcnt][j] = 0;
					  j = 0;
					  cmpcnt++;
					}
				}
				else			
				if (line[i] == '\\')
				{
					//do nothing.
					fgKeepSearch = TRUE;
				}
				else
				if (fgRealChar( line[i] ))
				{
				  compname[cmpcnt][j] = line[i];
				  j++;
				}
			}

			if (j > 0)
                     {
                         compname[cmpcnt][j] = 0;				
			    cmpcnt++;
                    }
		}
		else
		if (line[0] == 'D' &&
			line[1] == 'I' &&
			line[2] == 'F' &&
			line[3] == 'F' &&
			line[4] == '_' &&
			line[5] == 'R' &&
			line[6] == 'E' &&
			line[7] == 'S' &&
			line[8] == 'U' &&
			line[9] == 'L' &&
			line[10] == 'T' )
		{		
			//line[11] = ' '
			//line[12] = '='
			//line[13] = ' '
			i4StartIdx = 0;
			for (i=11; i < x_strlen(line); i++)
                    {
                        if (line[i] == '=')
                        {
                           i4StartIdx = i+1;
                           break;
                        }
                     }
					      
			fgDiffDump = FALSE;
                     fgDiffResult = TRUE;
			fgKeepSearch = FALSE;
			j=0;
			for (i=i4StartIdx; i < x_strlen(line); i++)
			{
				if (line[i] == 0 || line[i] == '\n' || line[i] == '\r' )
				{
					if (j > 0)
					{
					 goldname[goldcnt][j] = 0;
					 goldcnt++;
					 j = 0;
					}
					break;
				}
				else
				if (line[i] == ' ')
				{					
					if (j > 0)
					{					  
					  goldname[goldcnt][j] = 0;
					  j=0;
					  goldcnt++;
					}
				}
				else			
				if (line[i] == '\\')
				{
					//do nothing.
					fgKeepSearch = TRUE;
				}
				else
				if (fgRealChar( line[i] ))
				{
				  goldname[goldcnt][j] = line[i];
				  j++;
				}
			}

			if (j > 0)
                     {
                         goldname[goldcnt][j] = 0;
			    goldcnt++;
			    j = 0;
                    }
		}
		else
		if (fgDiffDump == TRUE)
		{		
			fgKeepSearch = FALSE;
			j=0;
			for (i=0; i < x_strlen(line); i++)
			{
				if (line[i] == 0 || line[i] == '\n' || line[i] == '\r' )
				{
					if (j > 0)
					{
						compname[cmpcnt][j] = 0;					
						cmpcnt++;
					}
					j = 0;
					fgDiffDump = fgKeepSearch;
					break;
				}
				else
				if (line[i] == ' ')
				{					
					if ( j > 0 )
					{					  
					  compname[cmpcnt][j] = 0;
					  cmpcnt++;
					  j = 0;
					}
				}
				else			
				if (line[i] == '\\')
				{
					//do nothing.
					fgKeepSearch = TRUE;
				}
				else
				if (fgRealChar( line[i] ))
				{
				  compname[cmpcnt][j] = line[i];
				  j++;
				}
			}

			if (j > 0)
                     {
                         compname[cmpcnt][j] = 0;
 			    cmpcnt++;
			    j = 0;
                    }
		}
		else
		if (fgDiffResult == TRUE)
		{		
			fgKeepSearch = FALSE;
			j=0;
			for (i=0; i < x_strlen(line); i++)
			{
				if (line[i] == 0 || line[i] == '\n' || line[i] == '\r' )
				{
					if (j > 0)
					{
					  goldname[goldcnt][j] = 0;
					  goldcnt++;
					}
					j=0;
					fgDiffResult = fgKeepSearch;
					break;
				}
				else
				if (line[i] == ' ')
				{				
					if (j > 0)
					{					  
					  goldname[goldcnt][j] = 0;					  
					  goldcnt++;
					  j=0;
					}
				}
				else			
				if (line[i] == '\\')
				{
					//do nothing.
					fgKeepSearch = TRUE;
				}
				else
				if (fgRealChar( line[i] ))
				{
				  goldname[goldcnt][j] = line[i];
				  j++;
				}
			}

			if (j > 0)
                     {
                        goldname[goldcnt][j] = 0;					  
 			   goldcnt++;
			   j = 0;
                    }
		}                                       
    } //    

#if     STATIC_MEMORY_DISPATCH
    x_free_aligned_dma_mem((UINT32)pMakeBuf);
#else
    x_free_aligned_dma_mem((UINT32)pMakeBuf);
#endif
}

void vG3DEmuCompareHex (CHAR *pMakeName, CHAR *dumpname, INT32 u4CompType, CHAR *pucPathName, UINT32 u4Size, UINT32 u4CompAddr)
{
   CHAR str1[100] = "";
   INT32 argc = 2;
   UINT32 u4PrsSz = 0;
   INT32  regval;
   UINT32 u4offset = 0;
   CHAR load_filename [256];
   CHAR make_filename [256];   
   CHAR fullgold_filename [256];
   BOOL fgIRQ_GP, fgIRQ_Pp, fgIRQ_GpMmu, fgIRQ_PpMmu;
   INT32 i;
   BOOL  fgCompared = FALSE;
   INT32 u4waitcnt = 0;
   CHAR *pch;
   UNUSED(argc);UNUSED(u4PrsSz);UNUSED(regval);UNUSED(u4offset);UNUSED(load_filename);UNUSED(fgIRQ_GP);UNUSED(fgIRQ_Pp);UNUSED(fgIRQ_GpMmu);UNUSED(fgIRQ_PpMmu);UNUSED(u4waitcnt);
   if (u4Size == 0)
   	u4Size = 0x7FFFFFFF;
   
   x_strncpy(make_filename, pucPathName, x_strlen(pucPathName));
   if(x_strlen(make_filename) + 8 > 255)
   {
       Printf("make_filename too long\n");
   }
   else 
   {
       x_strncat(make_filename, "Makefile", x_strlen("Makefile"));
   }
   
                     if (fgMakeFileParsed == FALSE)
                     {
                       vG3DEmuParseMakefile(make_filename, dumpname, u4CompType);
                       fgMakeFileParsed = TRUE;

                     }

                     fgCompared = FALSE;
                     if (cmpcnt >0 && goldcnt > 0)
                     {
                         for (i=0; i < cmpcnt; i++)
                         {
                            if ( x_strcmp(dumpname, compname[i]) == 0)
                            {
                               x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                               x_strncat(fullgold_filename, goldname[i], x_strlen(goldname[i]));
                               vG3DEmuParseMemHex (fullgold_filename, 0, u4CompType, u4DumpMemCount[i], (u4Size), u4CompAddr);

                               fgCompared = TRUE;
                               u4DumpMemCount[i] += (u4Size);
                               break;
                           }
                         }
                     }
                     else
                     if (cmpcnt == 0 && goldcnt > 0)
                     {
                        pch = x_strrchr(str1, '.');
                        for (i=0; i < (pch - str1); i++)
                        {
                           dumpname[i] = str1[i];
                        }
                        dumpname[i] = 0;
                        x_strncat(dumpname, "_result.hex", x_strlen("_result.hex"));
                        
                         for (i=0; i < goldcnt; i++)
                         {
                            if ( x_strcmp(dumpname, goldname[i]) == 0)
                            {
                               x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                               x_strncat(fullgold_filename, goldname[i], x_strlen(goldname[i]));
                               vG3DEmuParseMemHex (fullgold_filename, 0, u4CompType, u4DumpMemCount[i], (u4Size), u4CompAddr);

                               fgCompared = TRUE;
                               u4DumpMemCount[i] += (u4Size);
                               break;
                           }
                         }
                     }
                     else
                     if (cmpcnt > 0 && goldcnt == 0)
                     {
                        Printf("Mismatch\n");
                     }
                     
                     if (fgCompared == FALSE)
                     {
                          x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                          x_strncat(fullgold_filename, "result.hex", x_strlen("result.hex"));
                          vG3DEmuParseMemHex (fullgold_filename, 0, u4CompType, u4DumpMemCount[0], (u4Size), u4CompAddr);
                          u4DumpMemCount[0] += (u4Size);
                     }
}

void vG3DEmuParseConfig ( void *pConfig, UINT32 u4ConfigSz, CHAR *pucPathName)
{
   CHAR config[500] = {'\0'};
   CHAR cmd[256],line[256];
   CHAR adr0[50], adr1[50];
   CHAR val0[24], val1[24], val2[24];
   INT32  once, new_adr, new_adr0, new_adr1;
   INT32  r = 0, value, idx;
   INT32  addr0 = 0, addr1 = 0, len;
   INT32  line_number=0; 
   CHAR str0[100] = " ",str1[100] = " ";
   INT32 argc = 2;
   CHAR dumpname [256];   
   UINT32 u4PrsSz = 0;
   INT32  regval;
   UINT32 u4offset = 0;
   CHAR load_filename [256];
   CHAR make_filename [256];   
   CHAR fullgold_filename [256];
   BOOL fgIRQ_GP, fgIRQ_Pp, fgIRQ_GpMmu, fgIRQ_PpMmu;
   INT32 i;
   BOOL  fgCompared = FALSE;
   INT32 u4waitcnt = 0;
   CHAR *pch;
   UINT32 u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status;
   UINT32 u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status;
   UNUSED(config);   UNUSED(idx);   UNUSED(len);
   UNUSED(fgIRQ_GP); UNUSED(fgIRQ_Pp); UNUSED(fgIRQ_GpMmu); UNUSED(fgIRQ_PpMmu);   
   fgMakeFileParsed = FALSE;

   for (i=0; i<10; i++)
   {
       u4DumpMemCount[i] = 0;
       u4DumpRegCount[i] = 0;
   }
   
   if(argc==2)
   {
      // config translation
      Printf("*************************\n");
      Printf(" MALI to MTK Integration \n");
      Printf("*************************\n\n");  
      
      Printf("a) converting config.txt  --->  mali.pat\n");
      Printf("b) converting { \n");
      
      once=0;
      while(u4PrsSz < u4ConfigSz) 
      {         
         BOOL fgRet;
         fgRet = fgG3DBufGetLine((CHAR*) pConfig, &u4PrsSz, line, u4ConfigSz);         
         if(!fgRet)
         {
             break;
         }
         
         line_number = line_number + 1;
         
#ifndef __KERNEL__
         r = x_sscanf(line, "%s\r\n", cmd);
#endif
#if !TEST_PROCESSING_TIME               
         Printf("cmd is : %s\n", cmd);
#endif

         //if(c!=NULL)
         {
            if(r==1)
            { 
               if(x_strcmp(cmd, "reset")==0)
               {
                  Printf("reset cmd\n");

                  //GP software reset:
                  //write 0020[10]=1
                  //wait reset complete interrupt 0030[19]
                  regval = u4MaliReadReg(0, 0x0020);
                  regval |= (0x1<<10);
                  vMaliWriteReg(0, 0x0020, regval);
                  u4waitcnt = 0;
                  while(u4waitcnt < 5)
                  {
                       x_thread_delay(2);
                       if (u4MaliReadReg(0, 0x0030)  &  (0x1<<19))
                       {
                            break;
                       }

                       u4waitcnt++;
                  }

                  //PP software reset:
                  //write 900c[7]=1
                  //wait reset complete interrupt 902c[12]
                  vMaliWriteReg(0, 0x9028, 0xFFF);
                  
                  regval = u4MaliReadReg(0, 0x900c);
                  regval |= (0x1<<7);
                  vMaliWriteReg(0, 0x900c, regval);
                  u4waitcnt = 0;
                  while(u4waitcnt < 5)
                  {
                       x_thread_delay(2);
                       if (u4MaliReadReg(0, 0x0902c)  &  (0x1<<12))
                       {
                            break;
                       }
                       u4waitcnt++;
                  }
                  
               }  
               else if(x_strcmp(cmd, "wait")==0)
               {
                  val0[0] = 0;
#ifndef __KERNEL__
                  r = x_sscanf(line, " wait %s %s %s", str0, str1, val0);
#endif
                  if(x_strcmp(str1, "irq")==0)
                  {
                        Printf( "\nwait(mali_%s_risc==1);\n\n", str1);

#if TEST_G3D_INTERRUPT                        
//                        if(x_sema_lock_timeout(_hG3dIsrSema, 5000) != OSR_OK)
                        if(x_sema_lock_timeout(_hG3dIsrSema, 30000) != OSR_OK)
                        {
                            u4Gp_Irq_Raw = u4MaliReadReg(0, 0x24); 
                            u4Gp_Irq_Clear = u4MaliReadReg(0, 0x28); 
                            u4Gp_Irq_Mask = u4MaliReadReg(0, 0x2c); 
                            u4Gp_Irq_Status = u4MaliReadReg(0, 0x30);
                            Printf( "GP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status);
                        }
                        if(x_sema_lock_timeout(_hG3dPpIsrSema, 30000) != OSR_OK)
                        {
                            u4Pp_Irq_Raw = u4MaliReadReg(0, 0x9020); 
                            u4Pp_Irq_Clear = u4MaliReadReg(0, 0x9024);
                            u4Pp_Irq_Mask = u4MaliReadReg(0, 0x9028);
                            u4Pp_Irq_Status = u4MaliReadReg(0, 0x902c);
                            Printf( "PP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status);                                                             
                        }                        
#else
                        u4waitcnt = 0;
                        while (1)
                        {
                           //irq_gp = 0x68[0]
                           //fgIRQ_GP = (u4MaliReadReg(0, 0x68) & 0x1)? TRUE : FALSE;
                           fgIRQ_GP = ((u4MaliReadReg(0, 0x24) & 0x3)==0x3)? TRUE : FALSE;
                           
                           //irq_pp_0 = 0x9008[6]
                           fgIRQ_Pp = (u4MaliReadReg(0, 0x9008) & (0x1 << 6))? TRUE : FALSE;

                           fgIRQ_GpMmu =  (u4MaliReadReg(0, 0x3020) & (0x3))? TRUE : FALSE;

                           fgIRQ_PpMmu =  (u4MaliReadReg(0, 0x5020) & (0x3))? TRUE : FALSE;

                           if (fgIRQ_GP || fgIRQ_Pp || fgIRQ_GpMmu || fgIRQ_PpMmu)
                           	  break;

                           x_thread_delay(10);
                           u4waitcnt++;

                           if (u4waitcnt > 500 )
                           {
                                 Printf( "Wait IRQ Fail!!!!\n");
                                 u4Gp_Irq_Raw = u4MaliReadReg(0, 0x24); 
                                 u4Gp_Irq_Clear = u4MaliReadReg(0, 0x28); 
                                 u4Gp_Irq_Mask = u4MaliReadReg(0, 0x2c); 
                                 u4Gp_Irq_Status = u4MaliReadReg(0, 0x30);
                                 u4Pp_Irq_Raw = u4MaliReadReg(0, 0x9020); 
                                 u4Pp_Irq_Clear = u4MaliReadReg(0, 0x9024);
                                 u4Pp_Irq_Mask = u4MaliReadReg(0, 0x9028);
                                 u4Pp_Irq_Status = u4MaliReadReg(0, 0x902c);
                                 Printf( "GP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status);
                                 Printf( "PP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status);                                 
                                 break;
                           }
                        }                       
#endif
                  }
                  else if(x_strcmp(str1, "clk")==0)
                  {
                    Printf( "\nrepeat(%s) @(%s mali_%s);\n\n", val0, str0, str1);
                    value = 0;
                    if ( val0[0] != 0)
                       value = u4StrToInt(val0);

                    if (value == 0)
                    	   value = 50;
                    
                     x_thread_delay(value);
                  }
                  else if(x_strcmp(str0, "irq")==0)
                  {
#ifndef __KERNEL__
                     r = x_sscanf(line, " wait irq %s %s", val0, val1);
#endif
#if TEST_G3D_INTERRUPT                        
//                        if(x_sema_lock_timeout(_hG3dIsrSema, 5000) != OSR_OK)
                        if(x_sema_lock_timeout(_hG3dIsrSema, 30000) != OSR_OK)
                        {
                            u4Gp_Irq_Raw = u4MaliReadReg(0, 0x24); 
                            u4Gp_Irq_Clear = u4MaliReadReg(0, 0x28); 
                            u4Gp_Irq_Mask = u4MaliReadReg(0, 0x2c); 
                            u4Gp_Irq_Status = u4MaliReadReg(0, 0x30);
                            Printf( "GP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status);
                        }
                        if(x_sema_lock_timeout(_hG3dPpIsrSema, 30000) != OSR_OK)
                        {
                            u4Pp_Irq_Raw = u4MaliReadReg(0, 0x9020); 
                            u4Pp_Irq_Clear = u4MaliReadReg(0, 0x9024);
                            u4Pp_Irq_Mask = u4MaliReadReg(0, 0x9028);
                            u4Pp_Irq_Status = u4MaliReadReg(0, 0x902c);
                            Printf( "PP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status);                                                             
                        }                        
#else                     
                     u4waitcnt = 0;                     
                     while (1)
                     {
                           //irq_gp = 0x68[0]
                           //fgIRQ_GP = (u4MaliReadReg(0, 0x68) & 0x1)? TRUE : FALSE;
                           fgIRQ_GP = ((u4MaliReadReg(0, 0x24) & 0x3) == 0x3)? TRUE : FALSE;
                        
                            //irq_pp_0 = 0x9008[6]
                           fgIRQ_Pp = (u4MaliReadReg(0, 0x9008) & (0x1 << 6))? TRUE : FALSE;

                            fgIRQ_GpMmu =  (u4MaliReadReg(0, 0x3020) & (0x3))? TRUE : FALSE;

                           fgIRQ_PpMmu =  (u4MaliReadReg(0, 0x5020) & (0x3))? TRUE : FALSE;

                           if (fgIRQ_GP || fgIRQ_Pp || fgIRQ_GpMmu || fgIRQ_PpMmu)
                           	  break;

                           x_thread_delay(10);
                           u4waitcnt++;

                           if (u4waitcnt > 100)
                           {
                                 Printf( "Wait IRQ Fail!!!!\n");
                                 u4Gp_Irq_Raw = u4MaliReadReg(0, 0x24); 
                                 u4Gp_Irq_Clear = u4MaliReadReg(0, 0x28); 
                                 u4Gp_Irq_Mask = u4MaliReadReg(0, 0x2c); 
                                 u4Gp_Irq_Status = u4MaliReadReg(0, 0x30);
                                 u4Pp_Irq_Raw = u4MaliReadReg(0, 0x9020); 
                                 u4Pp_Irq_Clear = u4MaliReadReg(0, 0x9024);
                                 u4Pp_Irq_Mask = u4MaliReadReg(0, 0x9028);
                                 u4Pp_Irq_Status = u4MaliReadReg(0, 0x902c);
                                 Printf( "GP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status);
                                 Printf( "PP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status);
                                 break;
                           }
                     }          
#endif

                     if(r==1)
                     {
                        Printf(  "\n$display(\"Waiting for IRQ %s ...\");", val0);
                        Printf(  "\nwait((IRQ & 'h%s));\n\n", val0);
                     }
                     else 
                     if(r==2)
                     {
                           Printf(  "\n$display(\"Waiting for IRQs %s == %s ...\");", val0, val1);
                           Printf(  "\nwait(((IRQ & 'h%s)=='h%s));\n\n", val0, val1);
                     }
                  }
               }
               else if(x_strcmp(cmd, "clear_mem")==0)
               {
#ifndef __KERNEL__
                  r = x_sscanf(line, " clear_mem %s %s %s", str0, str1, val0);
#endif                  
                  if(r==2) 
                  {
                     Printf( "    top.clear_mem('h%s, 'h%s, 0);\n", str0, str1);
                     value = 0;        
                  }
                  else
                  {
                     Printf( "    top.clear_mem('h%s, 'h%s, 'h%s);\n", str0, str1, val0);
                     value = u4StrToInt(val0);                      
                  }
                  new_adr0 = u4StrToInt(str0);
                  new_adr1 = u4StrToInt(str1);    
                  for(i = new_adr0; i < new_adr1; i+=4)
                  {
                      *(UINT32*)M2P((UINT32)i) = (UINT32)value;
                  }
               }
               else if(x_strcmp(cmd, "writereg")==0)
               {
#ifndef __KERNEL__
                  r = x_sscanf(line, " writereg %s %s", adr0, val0);
#endif
                  value=0;    idx=0;                  
                  value = u4StrToInt(adr0);
                  new_adr = u4ConfigAddToRealAddr(value);
                  regval = u4StrToInt(val0);          

                  if (new_adr == 0x8104 || new_adr == 0x8204 || new_adr == 0x8304)
                  {
                      fgIsPPOutBuff = TRUE;
                      pPPOutBuffAddr = (CHAR*)regval;
                      
                      Printf( "PP Output Buffer =  0x%08x\n", regval);;
                  }

#if !TEST_PROCESSING_TIME                   
                  Printf( "writereg 0x%08x : 0x%08x\n", new_adr, regval);
#endif

                  if(new_adr == 0x0020)
                  {
                      HAL_GetTime(&_rTimeGPTrigger);
                  }
                  
                  vMaliWriteReg(0, new_adr, regval);
               }
               else if(x_strcmp(cmd, "dump_mem")==0)
               {
#ifndef __KERNEL__
                  r = x_sscanf(line, " dump_mem %x %x %s %s", &addr0, &addr1, str0, str1);
#endif
                  //len = addr1 - addr0;
                  Printf( " Dump Memory Start\n");
                  
                  if(r==2)
                  {
                     ;//Printf( "    mali_testbench.DUT.u_dramc.SaveMemory(`STDOUT, 32'h%08x, 32'h%08x );\n", addr0, len);      
                  }
                  else if(r==4)
                  {
                     if(x_strcmp(str0, ">")==0) 
                     {
                        Printf( " str0 is > \n");           
                     }
                     else if(x_strcmp(str0, ">>")==0) 
                     {
                        Printf( " str0 is >> \n");           
                     }

                     new_adr0 = addr0;
                     new_adr1 = addr1;
                     //new_adr0 = u4ConfigAddToRealAddr(addr0);
                     //new_adr1 = u4ConfigAddToRealAddr(addr1);                     
                  
                     Printf("Dump_mem: 0x%08x ~ 0x%08x", new_adr0, new_adr1);
                     Printf( " str1 is %s \n", str1);
                     x_strncpy (dumpname, str1, x_strlen(str1));

                     x_strncpy(make_filename, pucPathName, x_strlen(pucPathName));
                     x_strncat(make_filename, "Makefile", x_strlen("Makefile"));
                     if (fgMakeFileParsed == FALSE)
                     {
                       vG3DEmuParseMakefile(make_filename, dumpname, HEX_COMP_MEM);
                       fgMakeFileParsed = TRUE;

                     }

                     fgCompared = FALSE;
                     if (cmpcnt >0 && goldcnt > 0)
                     {
                         for (i=0; i < cmpcnt; i++)
                         {
                            if (x_strcmp(dumpname, compname[i]) == 0)
                            {
                               x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                               x_strncat(fullgold_filename, goldname[i], x_strlen(goldname[i]));
                               vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_MEM, u4DumpMemCount[i], (new_adr1 - new_adr0), 0x0);
                               fgCompared = TRUE;
                               u4DumpMemCount[i] += (new_adr1 - new_adr0);
                               break;
                           }
                         }
                     }
                     else
                     if (cmpcnt == 0 && goldcnt > 0)
                     {
                        pch = x_strrchr(str1, '.');
                        for (i=0; i < (pch - str1); i++)
                        {
                           dumpname[i] = str1[i];
                        }
                        dumpname[i] = 0;
                        x_strncat(dumpname, "_result.hex", x_strlen("_result.hex"));
                        
                         for (i=0; i < goldcnt; i++)
                         {
                            if ( x_strcmp(dumpname, goldname[i]) == 0)
                            {
                               x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                               x_strncat(fullgold_filename, goldname[i], x_strlen(goldname[i]));
                               vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_MEM, u4DumpMemCount[i], (new_adr1 - new_adr0), 0x0);
                               fgCompared = TRUE;
                               u4DumpMemCount[i] += (new_adr1 - new_adr0);
                               break;
                           }
                         }
                     }
                     else
                     if (cmpcnt > 0 && goldcnt == 0)
                     {
                        Printf("Mismatch\n");
                     }
                     
                     if (fgCompared == FALSE)
                     {
                          x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                          x_strncat(fullgold_filename, "result.hex", x_strlen("result.hex"));
                          vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_MEM, u4DumpMemCount[0], (new_adr1 - new_adr0), 0x0);
                          u4DumpMemCount[0] += (new_adr1 - new_adr0);
                     }
                  }
                  Printf( " Dump Memory End\n");
               }
               else if(x_strcmp(cmd, "dump_written_mem")==0)
               {
#ifndef __KERNEL__
                  r = x_sscanf(line, " dump_written_mem %x %x %s %s", &addr0, &addr1, str0, str1);
#endif
                  
                  if(r<=0)
                  {
#ifndef __KERNEL__
                     r = x_sscanf(line, " dump_written_mem %s %s", str0, str1);
#endif
                     if(x_strcmp(str0, ">")==0)
                        Printf( "  file_tmp = $fopen(\"%s\", \"w\");\n", str1);                                                
                     else if(x_strcmp(str0, ">>")==0)
                        Printf( "  file_tmp = $fopen(\"%s\", \"a\");\n", str1);
                     else
                     	break;
                     	
                     new_adr0 = 0;
                     new_adr1 = 0;                  
                     x_strncpy (dumpname, str1, x_strlen(str1));
// result.hex
                     goldname[0][0] = 'r';
                     goldname[0][1] = 'e';
                     goldname[0][2] = 's';
                     goldname[0][3] = 'u';
                     goldname[0][4] = 'l';
                     goldname[0][5] = 't';
                     goldname[0][6] = '.';
                     goldname[0][7] = 'h';
                     goldname[0][8] = 'e';
                     goldname[0][9] = 'x';
                     goldname[0][10] = 0;
                     x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                     x_strncat(fullgold_filename, goldname[0], x_strlen(goldname[0]));

                     // vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_MEM, 0, 0x80000000, ((UINT32) pPPOutBuffAddr)+0x05100000);
                     vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_MEM, 0, 0x80000000, ((UINT32) pPPOutBuffAddr));                     
                     //vG3DEmuCompareHex (make_filename, dumpname, HEX_COMP_MEM, pucPathName, (new_adr1 - new_adr0));
                     
                  }
                  else if(r==4)
                  {
                     if(x_strcmp(str0, ">")==0) 
                        Printf( "  file_tmp = $fopen(\"%s\", \"w\");\n", str1);                        
                     else if(x_strcmp(str0, ">>")==0) 
                        Printf( "  file_tmp = $fopen(\"%s\", \"a\");\n", str1);    
                      else
                     	break;

                     value=0;
                     value = u4StrToInt(adr0);
                     new_adr0 = u4ConfigAddToRealAddr(value);
                     value = u4StrToInt(adr1);
                     new_adr1 = u4ConfigAddToRealAddr(value);

                     x_strncpy (dumpname, str1, x_strlen(str1));
                     // vG3DEmuCompareHex (make_filename, dumpname, HEX_COMP_MEM, pucPathName, (new_adr1 - new_adr0), (UINT32) pPPOutBuffAddr);
                     vG3DEmuCompareHex (make_filename, dumpname, HEX_COMP_MEM, pucPathName, (new_adr1 - new_adr0), (UINT32) pPPOutBuffAddr);                     
                  }
               }
               else if(x_strcmp(cmd, "dump_reg")==0)
               {
#ifndef __KERNEL__
                  r = x_sscanf(line, " dump_reg %s %s %s %s", adr0, adr1, str0, str1);                  
#endif
                  value=0;
                  value = u4StrToInt(adr0);
                  new_adr0 = u4ConfigAddToRealAddr(value);
                  value = u4StrToInt(adr1);
                  new_adr1 = u4ConfigAddToRealAddr(value);
                  
                  Printf("Dump_reg: 0x%08x ~ 0x%08x", new_adr0, new_adr1);

                  if(r==2)
                  {
                     ;//Printf( "    mali_testbench.DUT.u_dramc.SaveMemory(`STDOUT, 32'h%08x, 32'h%08x );\n", addr0, len);      
                  }
                  else if(r==4)
                  {
                     if(x_strcmp(str0, ">")==0) 
                     {
                        Printf( " str0 is > \n");           
                     }
                     else if(x_strcmp(str0, ">>")==0) 
                     {
                        Printf( " str0 is >> \n");
                     }

                     Printf( " str1 is %s \n", str1);
                     
                     x_strncpy (dumpname, str1, x_strlen(str1));

                     x_strncpy(make_filename, pucPathName, x_strlen(pucPathName));
                     x_strncat(make_filename, "Makefile", x_strlen("Makefile"));
                     if (fgMakeFileParsed == FALSE)
                     {
                         vG3DEmuParseMakefile(make_filename, dumpname, HEX_COMP_REG);
                         fgMakeFileParsed = TRUE;
                     }

                     fgCompared = FALSE;
                     if (cmpcnt >0 && goldcnt > 0)
                     {
                         for (i=0; i < cmpcnt; i++)
                         {
                            if ( x_strcmp(dumpname, compname[i]) == 0)
                            {                                       
                                  x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                                  x_strncat(fullgold_filename, goldname[i], x_strlen(goldname[i]));
                                  vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_REG, u4DumpRegCount[i], (new_adr1 - new_adr0), 0x0);
                                  fgCompared = TRUE;
                                  u4DumpRegCount[i] += (new_adr1 - new_adr0);
                                  break;
                             }
                        }
                    }
                    else
                    if ( cmpcnt == 0 && goldcnt > 0 )
                    {
                        pch = x_strrchr(str1, '.');
                        for (i=0; i < (pch - str1); i++)
                        {
                           dumpname[i] = str1[i];
                        }
                        dumpname[i] = 0;
                        x_strncat(dumpname, "_result.hex", x_strlen("_result.hex"));
                        
                         for (i=0; i < goldcnt; i++)
                         {
                            if ( x_strcmp(dumpname, goldname[i]) == 0)
                            {
                               x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                               x_strncat(fullgold_filename, goldname[i], x_strlen(goldname[i]));
                               vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_REG, u4DumpRegCount[i], (new_adr1 - new_adr0), 0x0);
                               fgCompared = TRUE;
                               u4DumpRegCount[i] += (new_adr1 - new_adr0);
                               break;
                           }
                         }
                    }
                    else
                    if ( cmpcnt > 0 && goldcnt == 0 )
                    {
                        Printf("Mismatch\n");
                    }
                    	
                     
                    if (fgCompared == FALSE)
                    {
                        x_strncpy(fullgold_filename, pucPathName, x_strlen(pucPathName));
                        x_strncat(fullgold_filename, "result.hex", x_strlen("result.hex"));
                        vG3DEmuParseMemHex (fullgold_filename, 0, HEX_COMP_REG, u4DumpMemCount[0], (new_adr1 - new_adr0), 0x0);
                        u4DumpRegCount[0] += (new_adr1 - new_adr0);
                    }
                 }
               }
               else if(x_strcmp(cmd, "outstanding")==0)
               {
#ifndef __KERNEL__
                  r = x_sscanf(line, " outstanding %s %s %s", val0, val1, val2 );
#endif
                  Printf( "$display(\"Setting outstanding transactions support: %s reads, %s writes, XX total\");\n", val0, val1, val2);
                  //fp_axi_w = fopen("axi_setting.v", "w");
                  Printf( "RISCWrite(`SVOAHB_00_ADDR + 'hE004, 'h%s%s);\n", val0, val1);
                  //fclose(fp_axi_w);
               }
               else if(x_strcmp(cmd, "quit")==0 && !once)
               {
                  //Printf( "$finish;\n");
                  //once=1;
               }
               // Memory convertion
               else if(x_strcmp(cmd, "load_mem")==0)
               {
                  val0[0] = 0;
#ifndef __KERNEL__
                  r = x_sscanf(line, " load_mem %s %s", str0, val0);
#endif
                                   
                  x_strncpy( load_filename, pucPathName, x_strlen(pucPathName));
                  if(x_strlen(load_filename) + x_strlen(pucPathName) > 255)
                  {
                      Printf("load_filename too large!\n");
                  }
                  else
                  {
                      x_strncat(load_filename, str0, x_strlen(str0));
                  }

                  u4offset = 0;
                  if (val0[0] != 0)
                  {
                      u4offset= u4StrToInt(val0);
                  }

                  vG3DEmuParseMemHex (load_filename, u4offset, HEX_LOAD, 0, 0xfffffff, 0x0);
                }
                
    } // r=1
   
  }  // c != NULL  
 }  //end of while
 
 
 Printf("  }\n");
 
 Printf("\n\n");   
  } //end of if
  
} //end of func.


  

INT32 i4Graphics3D_Drv_RegRW(void)
{
   INT32  i4Idx = 0;
   INT32  i4WrData = 0;
   INT32  i4RdData = 0;

   //i4WrData = 0xab00;
   //vMaliWriteReg(0, 4, i4WrData);
   
   while (1)
   {
   for ( i4Idx = 0; i4Idx < 256; i4Idx++)
   {
        if ( (i4Idx & 0xf) == 0x0)
        {
            Printf("i4Idx = %d\n", i4Idx);
        }
   
        //i4WrData = i4Idx << 8;
        //Printf("i4WrData = %d\n", i4WrData);
        //vMaliWriteReg(0, 4, i4WrData);
                             //vWriteReg(0x8100 + 0, i4WrData);
        //Printf("i4WrData = %d\n", i4WrData);
        i4RdData = u4MaliReadReg(0, 4);
                            //i4RdData = u4ReadReg(0x8100 + 0);
        Printf("i4RdData = %d\n", i4RdData);
        if (i4WrData != i4RdData)
        {
            Printf("Mali W != R!!\n");
        }        
   }
   }

}

static UINT32 _u4RgnList = 0;
static UINT32 _u4Rgn = 0;
INT32 i4Graphics3D_Display(void)
{
   OSD_Reset();
   OSD_RGN_LIST_Create(&_u4RgnList);
   OSD_RGN_Create(&_u4Rgn, 1280, 720,
           (void*)(_u4MaliBufferBase + 0x381f00),
           OSD_CM_ARGB8888_DIRECT32, 1280 * 4,
           0, 0, 1920, 1080); 
   OSD_RGN_Insert(_u4Rgn, _u4RgnList);
   
   OSD_RGN_Set(_u4Rgn, OSD_RGN_BIG_ENDIAN, 0);
   OSD_RGN_Set(_u4Rgn, OSD_RGN_MIX_SEL, OSD_BM_PLANE);
   
    /* insert region to list*/
   OSD_RGN_Insert(_u4Rgn, _u4RgnList);

    /* flip plan to list */
   OSD_PLA_FlipTo(OSD_PLANE_2, _u4RgnList);
   OSD_PLA_Enable(OSD_PLANE_2, TRUE);
   return 0;
}

INT32 i4Graphics3D_Drv_Emu(void)
{
   CHAR    *pConfigBuffer;   
   CHAR    *pG3dEmuList;
//   CHAR    ucEmuListName [256] = "d:\\g3d_emulation\\g3d_emu_list.txt";
   CHAR    ucEmuListName [256] = "Z:\\g3d_emulation\\g3d_emu_list.txt";
   INT32    i4EmuFd = 0;
   UINT32  u4EmuReadSize = 0;
   UINT32  u4EmuFileSize = 0;
   UINT32  line_number = 0;
   UINT32  u4PrsSz = 0;
   CHAR    line[256];
   CHAR    cmd[256];
   
   CHAR     ucPathName [256] = "D:\\g3d\0";
   CHAR     ucFileName [256] = "config.txt\0";
   CHAR     ucStringName [256];
   INT32    i4ConfigFd = 0;
   UINT32  u4ConfigReadSize = 0;
   UINT32  u4ConfigFileSize = 0;
   BOOL     fgRet = FALSE;
   INT32    r;
   CHAR    ucTemp[20] = {'\0'};
   UINT32 u4PTE;   
   UNUSED(u4GPDTE);   UNUSED(u4PTE);   UNUSED(u4PPDTE);
   UNUSED(ucEmuListName);   UNUSED(i4EmuFd);   UNUSED(u4EmuFileSize);   UNUSED(i4ConfigFd);  UNUSED(u4ConfigFileSize);
   UNUSED(pConfigBuffer);UNUSED(u4EmuReadSize);
   UNUSED(line_number);
   UNUSED(line);   
   UNUSED(u4PrsSz);      
   UNUSED(cmd);    
   UNUSED(ucPathName);       
   UNUSED(ucFileName);       	
   UNUSED(ucStringName);
   UNUSED(u4ConfigReadSize);   
   UNUSED(fgRet);      
   UNUSED(r);     
   UNUSED(ucTemp);        
   UNUSED(pG3dEmuList);
#if LOAD_FROM_NAND
    {
        UINT32 u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status;
        UINT32 u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status;
        UNUSED(u4Gp_Irq_Raw);        UNUSED(u4Gp_Irq_Clear);        UNUSED(u4Gp_Irq_Mask);        UNUSED(u4Gp_Irq_Status);
        UNUSED(u4Pp_Irq_Raw);        UNUSED(u4Pp_Irq_Clear);        UNUSED(u4Pp_Irq_Mask);        UNUSED(u4Pp_Irq_Status);        
        //reset
        //IO_WRITE32(0xF0040000, 0x20, 0x20);
        //IO_WRITE32(0xF0048000, 0x100c, 0x20);
        //# MMU page tables
        //load_mem gp_7A_52_42_00.hex
        //# MMU registers
        //writereg 02000008 00000006
        IO_WRITE32(0xF0043000, 8, 6);
        //writereg 02000008 00000001
        IO_WRITE32(0xF0043000, 8, 1);
        //writereg 02000000 0084a000
        IO_WRITE32(0xF0043000, 0, _u4MaliMMUBase);
        //writereg 02000008 00000004
        IO_WRITE32(0xF0043000, 8, 4);
        //writereg 02000008 00000000
        IO_WRITE32(0xF0043000, 8, 0);
        //writereg 12000008 00000006
        IO_WRITE32(0xF0044000, 8, 6);
        //writereg 12000008 00000001
        IO_WRITE32(0xF0044000, 8, 1);
        //writereg 12000000 0084a000
        IO_WRITE32(0xF0044000, 0, _u4MaliMMUBase);
        //writereg 12000008 00000004
        IO_WRITE32(0xF0044000, 8, 4);
        //writereg 12000008 00000000
        IO_WRITE32(0xF0044000, 8, 0);
        //# MMU end
        //load_mem mc1.hex
#if MOUNT_USB
        if(x_sema_lock_timeout(_hMountSema, 10000) != OSR_OK)
        {
            Printf("Cannot mount USB!!\n");
        }    
        x_strncpy(_szCurrFile, _szMountPath, x_strlen(_szMountPath));
        x_strncat(_szCurrFile, "/", x_strlen("/"));
        x_strncat(_szCurrFile, "mc1.bin", x_strlen("mc1.bin"));
#else
        x_strncpy(_szCurrFile, "/mnt/usb_0/mc1.bin", x_strlen("/mnt/usb_0/mc1.bin"));
#endif    
        _ReadFile(_szCurrFile);
        _LoadPattern();
    //writereg 0000002c 00707bfe # vs_plbu_job
        IO_WRITE32(0xF0040000, 0x2c, 0x00707bfe);
    //writereg 00000028 00707bfe # vs_plbu_job
        IO_WRITE32(0xF0040000, 0x28, 0x00707bfe);
    //writereg 00000000 001ac260 # vs_plbu_job
        IO_WRITE32(0xF0040000, 0x0, 0x001ac260);
    //writereg 00000004 001adb20 # vs_plbu_job
        IO_WRITE32(0xF0040000, 0x4, 0x001adb20);
    //writereg 00000008 001af580 # vs_plbu_job
        IO_WRITE32(0xF0040000, 0x8, 0x001af580);
    //writereg 0000000c 001b0ed8 # vs_plbu_job
        IO_WRITE32(0xF0040000, 0xc, 0x001b0ed8);
    //writereg 00000020 00000003 # vs_plbu_job
        IO_WRITE32(0xF0040000, 0x20, 0x3);
    //writereg 10001028 00000fff # render_job
        IO_WRITE32(0xF0048000, 0x1028, 0xfff);
    //writereg 10001024 00000fff # render_job
        IO_WRITE32(0xF0048000, 0x1024, 0xfff);
    //writereg 10000000 003655c0 # render_job
        IO_WRITE32(0xF0048000, 0x0, 0x003655c0);
    //writereg 10000004 00705f00 # render_job
        IO_WRITE32(0xF0048000, 0x4, 0x00705f00);
    //writereg 1000000c 00000022 # render_job
        IO_WRITE32(0xF0048000, 0xc, 0x22);
    //writereg 10000010 00ffffff # render_job
        IO_WRITE32(0xF0048000, 0x10, 0xffffff);
    //writereg 10000018 ff7f3f01 # render_job
        IO_WRITE32(0xF0048000, 0x18, 0xff7f3f01);
    //writereg 1000001c ff7f3f01 # render_job
        IO_WRITE32(0xF0048000, 0x1c, 0xff7f3f01);
    //writereg 10000020 ff7f3f01 # render_job
        IO_WRITE32(0xF0048000, 0x20, 0xff7f3f01);
    //writereg 10000024 ff7f3f01 # render_job
        IO_WRITE32(0xF0048000, 0x24, 0xff7f3f01);
    //writereg 10000030 0109de40 # render_job
        IO_WRITE32(0xF0048000, 0x30, 0x0109de40);
    //writereg 10000034 00010001 # render_job
        IO_WRITE32(0xF0048000, 0x34, 0x10001);
    //writereg 10000040 00000001 # render_job
        IO_WRITE32(0xF0048000, 0x40, 0x1);
    //writereg 10000044 0000059f # render_job
        IO_WRITE32(0xF0048000, 0x44, 0x59f);
    //writereg 10000048 00000077 # render_job
        IO_WRITE32(0xF0048000, 0x48, 0x77);
    //writereg 1000004c 00000001 # render_job
        IO_WRITE32(0xF0048000, 0x4c, 0x1);
    //writereg 10000050 10010002 # render_job
        IO_WRITE32(0xF0048000, 0x50, 0x10010002);
    //writereg 10000054 00000c0c # render_job
        IO_WRITE32(0xF0048000, 0x54, 0xc0c);
    //writereg 10000100 00000002 # render_job
        IO_WRITE32(0xF0048000, 0x100, 0x2);
    //writereg 10000104 00381f00 # render_job
        IO_WRITE32(0xF0048000, 0x104, 0x381f00);
    //writereg 10000108 00000003 # render_job
        IO_WRITE32(0xF0048000, 0x108, 0x3);
    //writereg 10000114 00000280 # render_job
        IO_WRITE32(0xF0048000, 0x114, 0x280);
    //writereg 1000100c 00000040 # render_job
        IO_WRITE32(0xF0048000, 0x100c, 0x40);
    //wait irq 00010001 00010001 # join
        if(x_sema_lock_timeout(_hG3dIsrSema, 30000) != OSR_OK)
        {
            u4Gp_Irq_Raw = u4MaliReadReg(0, 0x24); 
            u4Gp_Irq_Clear = u4MaliReadReg(0, 0x28); 
            u4Gp_Irq_Mask = u4MaliReadReg(0, 0x2c); 
            u4Gp_Irq_Status = u4MaliReadReg(0, 0x30);
#ifndef __MODEL_slt__            
            Printf( "GP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Gp_Irq_Raw, u4Gp_Irq_Clear, u4Gp_Irq_Mask, u4Gp_Irq_Status);
#endif
        }
        if(x_sema_lock_timeout(_hG3dPpIsrSema, 30000) != OSR_OK)
        {
            u4Pp_Irq_Raw = u4MaliReadReg(0, 0x9020); 
            u4Pp_Irq_Clear = u4MaliReadReg(0, 0x9024);
            u4Pp_Irq_Mask = u4MaliReadReg(0, 0x9028);
            u4Pp_Irq_Status = u4MaliReadReg(0, 0x902c);
#ifndef __MODEL_slt__            
            Printf( "PP IRQ: Raw=0x%x, Clear = 0x%x, Mask=0x%x, Status = 0x%x \n", u4Pp_Irq_Raw, u4Pp_Irq_Clear, u4Pp_Irq_Mask, u4Pp_Irq_Status);                                                             
#endif
        }           
    }

#else

   x_strncpy(ucEmuListName, _szEmuListName, x_strlen(_szEmuListName));
   
#if STATIC_MEMORY_DISPATCH
   //pcStaticTotalBuffer = x_mem_alloc(38*1024*1024);
   Printf("Physical GP MMU Table Buffer : 0x%x !!\n", (INT32) (PHYSICAL ((INT32) pcStaticTotalBuffer)) ); 

   pHexFileBuffer   = (CHAR*)0x14400000;   //  15M
//   pGPPhyMemBuff = (CHAR*)0x1000000; // 15M
   pListFileBuffer    = (CHAR*)0x11C00000; //   1M
   pCofigFileBuffer = (CHAR*)0x11E00000;    //  1M
   GPMmuBuffer =     (CHAR*)0x12000000;

   // for yn's mmu
   // pGPPhyMemBuff = (CHAR*)0x5500000; // 15M

   pG3dEmuList = (CHAR*) (x_alloc_aligned_dma_mem(100*1024, 1024)); 
   pConfigBuffer = (CHAR*) (x_alloc_aligned_dma_mem(1024*1024, 1024));  

//   HalDisableDCache();

   if(u4GPDTE == NULL)
   {
//       u4GPDTE = 0x00E00000;      
       //u4GPDTE = (UINT32) (x_alloc_aligned_dma_mem(4 * 1024 * 1024, 0x1000));   

       //u4GPDTE = 0x06600000;   
   }
#if TEST_DRAM_CHB
   InitMaliGPMMU(u4GPDTE+0x40000000, u4GPDTE + 4096+0x40000000, (UINT32)pGPPhyMemBuff, (15 + 15 + 1 + 1 + 4) * 1024  * 1024);
#else
   InitMaliGPMMU(u4GPDTE, u4GPDTE + 4096, (UINT32)pGPPhyMemBuff, (15 + 15 + 1 + 1 + 4) * 1024  * 1024);
#endif
   //InitMaliGPMMU(u4GPDTE, u4GPDTE + 4096, (UINT32)pGPPhyMemBuff, 8 * 1024  * 1024);
   if(u4PPDTE == NULL)
   {
//       u4PPDTE = 0x00F00000;      
       //u4PPDTE = (UINT32) (x_alloc_aligned_dma_mem(4 * 1024 * 1024, 0x1000));   

       //u4PPDTE = 0x06600000;          
   }   
#if TEST_DRAM_CHB
   InitMaliPPMMU(u4PPDTE+0x40000000, u4PPDTE + 4096+0x40000000, (UINT32)pPPPhyMemBuff, (15 + 15 + 1 + 1 + 4) * 1024  * 1024);   
#else   
   InitMaliPPMMU(u4PPDTE, u4PPDTE + 4096, (UINT32)pPPPhyMemBuff, (15 + 15 + 1 + 1 + 4) * 1024  * 1024);   
   //InitMaliPPMMU(u4PPDTE, u4PPDTE + 4096, (UINT32)pPPPhyMemBuff, 8 * 1024  * 1024);
#endif   
#else
          
//GP MMU Part
   if (fgIsGPMmu)
   {
   GPMmuBuffer = x_mem_alloc(2*1024*1024);
   Printf("Physical GP Mmu Buffer : 0x%x !!\n", (INT32) (PHYSICAL ((INT32)GPMmuBuffer)) );
   if ( ( ((UINT32) GPMmuBuffer) & 0xFFFFF) == 0)
   	pGPMmuBuffer = (CHAR*) ((PHYSICAL ((INT32)GPMmuBuffer)) );
   else
        pGPMmuBuffer = (CHAR*) ((PHYSICAL ((INT32)GPMmuBuffer + 0x100000)) & 0xFFF00000);
   
   Printf("Aligned GP MMu Buffer : 0x%x !!\n", (INT32) pGPMmuBuffer );
   
   
   pGPPhyMemBuff = x_mem_alloc(10*1024*1024);
   Printf("Physical GP Mem Buffer : 0x%x !!\n", (INT32) (PHYSICAL ((INT32)pGPPhyMemBuff)) );

   if ( ( ((UINT32)pGPPhyMemBuff) & 0xFFFFF) == 0)
   	pAlignedGPMemBuff = (CHAR*) ((PHYSICAL ((INT32)pGPPhyMemBuff)) );
   else
        pAlignedGPMemBuff = (CHAR*) ((PHYSICAL ((INT32)pGPPhyMemBuff + 0x100000)) & 0xFFF00000);
   
   Printf("Aligned GP Mem Buffer : 0x%x !!\n", (INT32) pAlignedGPMemBuff );
   }

// PP MMU Part
   if (fgIsPPMmu)
   {
   PPMmuBuffer = x_mem_alloc(2*1024*1024);
   Printf("Physical PP Mmu Buffer : 0x%x !!\n", (INT32) (PHYSICAL ((INT32)PPMmuBuffer)) );
   if ( ( ((UINT32)PPMmuBuffer) & 0xFFFFF) == 0)
   	pPPMmuBuffer = (CHAR*) ((PHYSICAL ((INT32)PPMmuBuffer)) );
   else
        pPPMmuBuffer = (CHAR*) ((PHYSICAL ((INT32)PPMmuBuffer + 0x100000)) & 0xFFF00000);
   Printf("Aligned PP MMu Buffer : 0x%x !!\n", (INT32) pPPMmuBuffer );
   


   pPPPhyMemBuff = x_mem_alloc(10*1024*1024);
   Printf("Physical PP Mem Buffer : 0x%x !!\n", (INT32) (PHYSICAL ((INT32)pPPPhyMemBuff)) );
   
   if ( ( ((UINT32)pPPPhyMemBuff) & 0xFFFFF) == 0)
   	pAlignedPPMemBuff = (CHAR*) ((PHYSICAL ((INT32)pPPPhyMemBuff)) );
   else
        pAlignedPPMemBuff = (CHAR*) ((PHYSICAL ((INT32)pPPPhyMemBuff + 0x100000)) & 0xFFF00000);
   Printf("Aligned PP Mem Buffer : 0x%x !!\n", (INT32) pAlignedPPMemBuff );
   }

   pG3dEmuList = (CHAR*) (x_alloc_aligned_dma_mem(100*1024, 1024));
   Printf("pG3dEmuList Buffer : 0x%x !!\n", (INT32) (PHYSICAL ((INT32)pG3dEmuList)) );
      
   pConfigBuffer = (CHAR*) (x_alloc_aligned_dma_mem(100*1024, 1024)); 
   
#endif   

#if T32_HOST
   x_memset_2(pG3dEmuList, (int)'\0', 100 * 1024);
   u4EmuReadSize = 100 * 1024;
   u4EmuFileSize = 100 * 1024;
//   T32_HostExec(E_T32_EXEC, "d.load.binary \"d:\\g3d_emulation\\g3d_emu_list.txt\" 0x%08x /ny", pG3dEmuList);  
#ifndef __KERNEL__
   T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", ucEmuListName, pG3dEmuList);   
#endif
   //u4EmuReadSize = u4EmuFileSize = x_strlen(pG3dEmuList);   
   fgRet = 1;
#else
   fgRet = fgReadFile(pG3dEmuList, 100*1024, &ucEmuListName[0], 0, &i4EmuFd, &u4EmuReadSize, &u4EmuFileSize);
#endif
   if (fgRet == 0)
   {
        Printf("Read list file : %s FAIL!!\n", ucFileName);
        return 0;
   }

   u4PrsSz = 0;
   
   while(u4PrsSz < u4EmuReadSize) 
   {         
       fgRet = fgG3DBufGetLine((CHAR*) pG3dEmuList, &u4PrsSz, line, u4EmuReadSize);
       if(!fgRet)
       {
           break;
       }
       
       line_number = line_number + 1;

#ifndef __KERNEL__
       r = x_sscanf(line, "%s\r\n", cmd);
#endif
       Printf("PathName is : %s\n", cmd);
         
       if(r==1 && x_strcmp(cmd, "#")==0)
       { 
          //This line is comment. Do nothing.
       }
       else
       {
       x_strncpy(ucPathName, cmd, x_strlen(cmd)); 
       sprintf(ucTemp, "\\");
       x_strncat(ucPathName, ucTemp, x_srrlen(ucTemp));
       //strcat(ucPathName, "\\");
       
       x_strncpy(ucStringName, ucPathName, x_strlen(ucPathName));
       x_strncat(ucStringName, ucFileName, x_strlen(ucFileName));

       Printf("\n\n [3D] Parsing Config = %s\n", ucStringName); 

#if T32_HOST
       x_memset(pConfigBuffer, (int)'\0', 1024 * 1024);
       u4ConfigFileSize = 1024 * 1024;
       u4ConfigReadSize = 1024 * 1024;
#ifndef __KERNEL__
       T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%08x /ny", &ucStringName[0], pConfigBuffer);   
#endif
       //u4ConfigReadSize = u4ConfigFileSize = x_strlen(pConfigBuffer);
       fgRet = 1;
#else
       fgRet = fgReadFile(pConfigBuffer, 100*1024, &ucStringName[0], 0, &i4ConfigFd, &u4ConfigReadSize, &u4ConfigFileSize);
#endif

       if (!fgRet)
      {
           Printf("Read %s file FAIL!!\n", ucFileName);
       }

       fgIsPPOutBuff = FALSE;
       pPPOutBuffAddr = NULL;
       vG3DEmuParseConfig ( pConfigBuffer, u4ConfigReadSize, &ucPathName[0]);

       x_thread_delay(1000);
       }
   }

#endif // LOAD_FROM_NAND

#if STATIC_MEMORY_DISPATCH

   //x_mem_free(pcStaticTotalBuffer);

#else
   x_free_aligned_dma_mem(pConfigBuffer);
   x_free_aligned_dma_mem(pG3dEmuList);

   if (fgIsGPMmu)
   {
   x_mem_free (pPPPhyMemBuff); 
   x_mem_free(PPMmuBuffer);
   }

   if (fgIsPPMmu)
   {
   x_mem_free (pGPPhyMemBuff); 
   x_mem_free(GPMmuBuffer);
   }
#endif

   return 0;
}


#endif
