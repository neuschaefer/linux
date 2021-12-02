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

#ifndef __PNG_H
#define __PNG_H

#include "x_common.h"
#include "x_img_dec.h"
#include "png_com_hdr.h"
//#include <string.h>
#include "png_hal_dec.h"
//#include "drv_imgresz.h"
//#include "dram_model.h"
//#include "chip_ver.h"
//#include "sys_config.h"
#include "x_typedef.h"
#include "x_os.h"
#include "fbm_drvif.h"
#include "img_lib_if.h"

#if 0//CONFIG_SYS_MEM_PHASE3
#define PNG_MEMORY_PHASE_III  1
#define PNG_SUPPORT_BROKEN_MEM 1
#else
#define PNG_MEMORY_PHASE_III 0
#define PNG_SUPPORT_BROKEN_MEM 0
#define PNG_SUPPORT_DISCONTINUE_SRC 1
#endif

#define PngCopyMemory(destination, source, length) x_memcpy((destination), (source), (length))
#define PngZeroMemory(destination, length)         x_memset((destination), 0, (length))

#define PNG_INVALID64      0xFFFFFFFFFFFFFFFF
#define PNG_INVALID32      0xFFFFFFFF
#define PNG_INVALID16      0xFFFF
#define PNG_INVALID8       0xFF

typedef void* PNGHANDLE;

#define PNG_INST_NUM                1           ///< the instance number
#define PNG_PIC_INFO_CACHE_NUM      10          ///< the picture info cache number

#define PNG_FIFO_OFFSET 0
#define PNG_FIFO_SIZE               512*1024    ///< fifo size, 512K
#define PNG_FIFO_THRESHOLD          4*1024      ///< fifo threshold, reach, then start decode
#define PNG_CLR_BUF_OFFSET (PNG_FIFO_OFFSET + PNG_FIFO_SIZE)
#define PNG_CLR_BUF_SIZE            4096*768   ///< color buffer size 3M
#define PNG_CLR_BUF_NUM             1
#define PNG_CLR_BUF_HEIGHT          16

#define PNG_PLTE_OFFSET (PNG_CLR_BUF_OFFSET + PNG_CLR_BUF_SIZE)
#define PNG_PLTE_SIZE 256*3
#define PNG_TRNS_OFFSET (PNG_PLTE_OFFSET + PNG_PLTE_SIZE)
#define PNG_TRNS_SIZE 256

#define PNG_LZ77_OFFSET  (PNG_TRNS_OFFSET + PNG_TRNS_SIZE)
#define PNG_LZ77_SIZE       32*1024

#define PNG_LINE_BUF_OFFSET (PNG_LZ77_OFFSET + PNG_LZ77_SIZE)
#define PNG_LINE_BUF_SIZE   512*1024

#define PNG_DRV_RTN_STOP    1
#define PNG_DRV_RTN_OK      0
#define PNG_DRV_RTN_FAIL    (-1)

#define PNG_ERR_TYPE_IO_ERROR               -1
#define PNG_ERR_TYPE_TRNS_TIME_OUT          -2
#define PNG_ERR_TYPE_UNSUPPORT_CLR_MD       -3
#define PNG_ERR_TYPE_CLR_BUF_NO_INIT        -4
#define PNG_ERR_TYPE_NO_AVAILABLE_CLR_BUF   -5
#define PNG_ERR_TYPE_BITSTREAM_ERR          -6
#define PNG_ERR_TYPE_UPDATE_RDPOINT_ERR     -7
#define PNG_ERR_TYPE_UPDATE_IMGDMA_ERR      -8
#define PNG_ERR_TYPE_DECODE_ERR             -9
#define PNG_ERR_TYPE_NOT_IDAT               -10
#define PNG_ERR_TYPE_FIFO_NO_INIT           -11
#define PNG_ERR_TYPE_NO_AVAILABLE_FIFO_NUF  -12
#define PNG_ERR_TYPE_SIGN_ERR               -13
#define PNG_ERR_TYPE_HEADERN_ERR            -14
#define PNG_ERR_TYPE_HW_SET_PLT_ERR         -15
#define PNG_ERR_TYPE_HW_SET_TRNS_ERR        -16
#define PNG_ERR_TYPE_HW_SET_BKGD_ERR        -17
#define PNG_ERR_TYPE_HW_ENABLED_ERR         -18
#define PNG_ERR_TYPE_HW_SET_PICINFO_ERR     -19
#define PNG_ERR_TYPE_HW_SCALE_ERROR         -20
#define PNG_ERR_TYPE_FILE_END_ERROR         -21
#define PNG_ERR_TYPE_INEXISTENCE_CASE       -22
#define PNG_ERR_TYPE_8530_NOT_IEND              -23
#define PNG_ERR_TYPE_NO_AVAILABLE_TEMPLINE_BUF   -24



#define PNG_DRV_EV_INITIAL         ((EV_GRP_EVENT_T) 0)
#define PNG_DRV_EV_DECODE          ((EV_GRP_EVENT_T)(1) << 0)
#define PNG_DRV_EV_SCALE_DONE      ((EV_GRP_EVENT_T)(1) << 1)
#define PNG_DRV_EV_STOP            ((EV_GRP_EVENT_T)(1) << 2)
#define PNG_DRV_EV_SCALE_ABORT     ((EV_GRP_EVENT_T)(1) << 3)
#define PNG_DRV_EV_DEC_ABORT       ((EV_GRP_EVENT_T)(1) << 4)
#define PNG_DRV_EV_TIME_OUT        ((EV_GRP_EVENT_T)(1) << 5)
#define PNG_DRV_EV_FILLED          ((EV_GRP_EVENT_T)(1) << 6)
#define PNG_DRV_EV_SCALE_ERR       ((EV_GRP_EVENT_T)(1) << 7)
#define PNG_DRV_EV_DEL_THREAD      ((EV_GRP_EVENT_T)(1) << 8)

/// Png State Machine
typedef enum
{
    PNG_STATE_IDLE,
    PNG_STATE_START,
    PNG_STATE_INIT,
    PNG_STATE_PRS_HDR,
    PNG_STATE_INIT_HAL,
    PNG_STATE_DECODING,
    PNG_STATE_MAX
} PNG_STATE;

/// Png Decoding State Machine
typedef enum
{
    PNG_DEC_STATE_IDLE,
    PNG_DEC_STATE_DEC,
    PNG_DEC_STATE_WAIT,
    PNG_DEC_STATE_CHK_IDAT,
    PNG_DEC_STATE_SRC_RESUME,
    PNG_DEC_STATE_DST_RESUME,
    PNG_DEC_STATE_DO_SCALE,
    PNG_DEC_STATE_WAIT_SCALE,
    PNG_DEC_STATE_NEXT_DEC,
    PNG_DEC_STATE_MAX
} PNG_DEC_STATE;


/// Png Cache State
typedef enum
{
    PNG_CACHE_INVALID,
    PNG_CACHE_INUSING,
    PNG_CACHE_VALID,
    PNG_CACHE_DEC_FAIL
} PNG_CACHE_STATE;

/// Png Hal decode mode
typedef enum
{
    PNG_DRV_DECMD_PIC,
    PNG_DRV_DECMD_CLRBUF,
    PNG_DRV_DECMD_BROCKEN,
} PNG_DRV_DEC_MODE;

typedef struct _PNG_PIC_INFO_T
{
    UINT32              u4ImgId;                ///< picture id
} PNG_PIC_INFO_T;

typedef struct _PNG_FIFO_INFO_T
{
    UINT32              u4WrPoint;              ///< current fifo write point
    UINT32              u4RdPoint;              ///< current fifo read point
    UINT32              u4FifoStart;            ///< fifo start address
    UINT32              u4FifoEnd;              ///< fifo size
    UINT32              u4PreRdPoint;           ///< previous read point
    UINT32              u4FileOfst;             ///< the offset of file that has been read
    UINT32              u4ValidData;            ///< the number of valid data in fifo
    BOOL                fgEoRange;              ///< end of file
} PNG_FIFO_INFO_T;

typedef struct _PNG_BLOCK_INFO_T
{
    UINT32             u4TotalBlk;
    UINT32             u4BlkSize;
    UINT32             u4LastBlkSize;
    HANDLE_T           hTemp;
} PNG_BLOCK_INFO_T;

typedef struct _IMGRESZ_DRV_TICKET_T
{
    UINT32 u4Ticket;
}IMGRESZ_DRV_TICKET_T;

#define IMGDMA_OK 0
#define IMGDMA_DRV_FAIL 1
#define IMGDMA_DRV_EOF 2
#define IMGDMA_DRV_IO_ERR 3

typedef struct _PNG_INST_T
{
    UINT16              u2CompId;               ///< the Png Component Id
    HANDLE_T            hPngInst;               ///< the instance id
    HANDLE_T            hPngEvent;              ///< the event id
    HANDLE_T            hPngSema;               ///< the semapfore id
    INT32               i4PngTrnsTimerStatus;
    PNG_FIFO_INFO_T     tFifoInfo;
    UINT32              u4HwInstId;

    UINT32              u4ImgId;
    void                *pbImgBuf;
    UINT32              u4ImgSize;

    UINT32              u4FrmIndex;         /* frame index */
    UINT32              u4SrcX;             /* x offset in the source image in pixels */
    UINT32              u4SrcY;             /* y offset in the source image in pixels */
    UINT32              u4SrcWidth;         /* width to be decoded in pixels */
    UINT32              u4SrcHeight;        /* height to be decoded in pixels */
    VOID                *pvDst;             /* destination starting address */
    UINT32              u4DstX;             /* x offset in the destination in pixels */
    UINT32              u4DstY;             /* y offset in the destination in pixels */
    UINT32              u4DstWidth;         /* expected output width in pixels */
    UINT32              u4DstHeight;        /* expected output height in pixels */
    UINT32              u4DstPitch;         /* pitch of the destination image */
    UINT32              u4ScaleRowH;
    UINT32              u4ScaleRowY;
    GFX_COLORMODE_T     eDstCm;             /* destination color mode */
    ENUM_IMG_DISP_COLOR_MODE eDispFinalColorMode;
    ENUM_IMG_DISP_COLOR_MODE eDispColorMode;   /*display color mode */
    ENUM_IMG_ROTATE_PHASE        eRotate;            /* rotation option */

    PNG_STATE           ePngState;
    UINT32              u4Progress;
    PNG_HDR_INFO_T      tPngHdr;

    PNG_DEC_STATE       ePngDecState;
    UINT32              u4ClrBufAddr;
    UINT32              aru4ClrBufAddr[PNG_CLR_BUF_NUM];
    UINT32              u4PicTotalSz;
    UINT32              u4ClrBufSz;
    UINT32              u4ClrBufPitch;
    UINT32              u4ClrPixelNum;
    UINT32              u4IDATCount;
    UINT32              u4RemainChunkSz;
    UINT32              u4CurrSrcStrmSz;
    UINT32              u4TotalSlice;
    UINT32              u4SliceCount;
    UINT32              u4SliceHeight;
    BOOL                fgUseClrBuf;
    PNG_DRV_DEC_MODE    ePngDecMd;
    BOOL                fgRedecode;         /*redecode flag, if alder 32 error, set this flag true.*/
    INT32               i4PngErrorType;
    BOOL fgActive;
    BOOL fgAbort;
    UINT32 u4NfyStatus; 
    VOID                *pvDst_OnVideoPlane;             /* video plane destination starting address */    
    UINT32 u4CbCrOffset;            ///* video plane cbcr destination starting address */    
} PNG_INST_T;

void* _PNG_Init(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag);
INT32 _PNG_DecPrepare(PNGHANDLE hCurImg, IMG_PARAM_T *prInfo);
INT32 _PNG_DecOneSegment(IMGHANDLE hCurImg, UINT32 u4ImgID,IMG_PARAM_T *prInfo);
void PNG_SetFileSize(PNGHANDLE hDec,UINT32 u4FileSize);
void _PNG_SetFillBsBufEnd(PNGHANDLE hCurImg, UINT32 u4Entry1Size,
                          UINT32 u4Entry2Size, BOOL bIsEOF);
INT32 PNG_MwFillBuffer(PNGHANDLE hDec, void **ppvBuf,UINT32 *pu4Size, UINT32
                       u4AddSize);
void PNG_FreeInstance(PNGHANDLE hDec);

ENUM_IMG_RET _PNG_Stop(PNGHANDLE hCurImg);
#endif // __PNG_H
