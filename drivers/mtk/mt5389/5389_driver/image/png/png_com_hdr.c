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

#include "x_assert.h"
#include "x_os.h"
#include "png_drv.h"
#include "png_debug.h"

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
UINT32 u4PngHdrGet4Bytes(PNG_HDR_FIFO_INFO *ptPngHdrFifo, UINT32 u4ByteShift)
{
    UINT32 u4ValidData1 = 0;
    UINT8 *pu1Src;
    INT32 i4 = 0;
    UINT32 u4RdPoint = ptPngHdrFifo->u4ReadPoint + u4ByteShift;
    UINT32 u4Ret;

    if (u4RdPoint >= ptPngHdrFifo->u4FifoEnd)
    {
        u4RdPoint = ptPngHdrFifo->u4FifoStart + (u4RdPoint - ptPngHdrFifo->u4FifoEnd);
    }
    pu1Src = (UINT8 *)(u4RdPoint);

    u4Ret = 0;
    if (ptPngHdrFifo->u4WritePoint >= u4RdPoint)
    {
        u4Ret += ((UINT32)(pu1Src[0])) << 24;
        u4Ret += ((UINT32)(pu1Src[1])) << 16;
        u4Ret += ((UINT32)(pu1Src[2])) << 8;
        u4Ret += ((UINT32)(pu1Src[3]));
    }
    else
    {
        u4ValidData1 = ptPngHdrFifo->u4FifoEnd - u4RdPoint;
        if (u4ValidData1 >= 4)
        {
            u4Ret += ((UINT32)(pu1Src[0])) << 24;
            u4Ret += ((UINT32)(pu1Src[1])) << 16;
            u4Ret += ((UINT32)(pu1Src[2])) << 8;
            u4Ret += ((UINT32)(pu1Src[3]));
        }
        else
        {
            for (i4=0; i4<u4ValidData1; i4++)
            {
                u4Ret += ((UINT32)(pu1Src[i4])) << ((3-i4)*8);
            }
            pu1Src = (UINT8 *)(ptPngHdrFifo->u4FifoStart);
            for (i4=u4ValidData1; i4<4; i4++)
            {
                u4Ret += ((UINT32)(pu1Src[i4-u4ValidData1])) << ((3-i4)*8);
            }
        }
    }
    return u4Ret;
}

UINT16 u2PngHdrGet2Bytes(PNG_HDR_FIFO_INFO *ptPngHdrFifo, UINT32 u4ByteShift)
{
    UINT32 u4ValidData1 = 0;
    UINT8 *pu1Src;
    UINT32 u4RdPoint = ptPngHdrFifo->u4ReadPoint + u4ByteShift;
    UINT16 u2Ret;

    if (u4RdPoint >= ptPngHdrFifo->u4FifoEnd)
    {
        u4RdPoint = ptPngHdrFifo->u4FifoStart + (u4RdPoint - ptPngHdrFifo->u4FifoEnd);
    }
    pu1Src = (UINT8 *)(u4RdPoint);

    u2Ret = 0;
    if (ptPngHdrFifo->u4WritePoint >= u4RdPoint)
    {
        u2Ret += ((UINT32)(pu1Src[0])) << 8;
        u2Ret += ((UINT32)(pu1Src[1]));
    }
    else
    {
        u4ValidData1 = ptPngHdrFifo->u4FifoEnd - u4RdPoint;
        if (u4ValidData1 >= 2)
        {
            u2Ret += ((UINT32)(pu1Src[0])) << 8;
            u2Ret += ((UINT32)(pu1Src[1]));
        }
        else
        {
            u2Ret += ((UINT32)(pu1Src[0])) << 8;
            pu1Src = (UINT8 *)(ptPngHdrFifo->u4FifoStart);
            u2Ret += ((UINT32)(pu1Src[0]));
        }
    }
    return u2Ret;
}

UINT8 u1PngHdrGet1Byte(PNG_HDR_FIFO_INFO *ptPngHdrFifo, UINT32 u4ByteShift)
{
    UINT8 *pu1Src;
    UINT32 u4RdPoint = ptPngHdrFifo->u4ReadPoint + u4ByteShift;

    if (u4RdPoint >= ptPngHdrFifo->u4FifoEnd)
    {
        u4RdPoint = ptPngHdrFifo->u4FifoStart + (u4RdPoint - ptPngHdrFifo->u4FifoEnd);
    }
    pu1Src = (UINT8 *)(u4RdPoint);

    return pu1Src[0];
}

void vPngHdrSkipBytes(PNG_HDR_FIFO_INFO *ptPngHdrFifo, UINT32 u4SkipData)
{
    UINT32 u4RdPoint = ptPngHdrFifo->u4ReadPoint + u4SkipData;

    if (ptPngHdrFifo->u4FifoEnd > u4RdPoint)
    {
        ptPngHdrFifo->u4ReadPoint = u4RdPoint;
    }
    else if (ptPngHdrFifo->u4FifoEnd < u4RdPoint)
    {
        ptPngHdrFifo->u4ReadPoint = ptPngHdrFifo->u4FifoStart + (u4RdPoint - ptPngHdrFifo->u4FifoEnd);
    }
    else
    {
        ptPngHdrFifo->u4ReadPoint = ptPngHdrFifo->u4FifoStart;
    }
    ptPngHdrFifo->u4ValidData -= u4SkipData;
}

void vPngHdrInit(PNG_HDR_INFO_T *ptPngHdr)
{
    ptPngHdr->tPngIhdr.fgInterlace = FALSE;
    ptPngHdr->fgPngPalette = FALSE;
    ptPngHdr->fgPngtRNS = FALSE;
    ptPngHdr->fgPngbKGD = FALSE;
    ptPngHdr->u41stIDATOfst = 0;
}

INT32 i4PngHdrChkPngSign(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    UINT32 u4Sign1 = u4PngHdrGet4Bytes(ptPngHdrFifo, 0);
    UINT32 u4Sign2 = u4PngHdrGet4Bytes(ptPngHdrFifo, 4);

    if ((u4Sign1 == PNG_SIGNATURE1) && (u4Sign2 == PNG_SIGNATURE2))
    {
        return PNG_HDR_OK;
    }
    else
    {
        return PNG_HDR_FAIL;
    }
}

INT32 i4PngHdrIHDR(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    UINT32 u4ChunkTotalSize = 8 + ptPngHdr->tPngChunkInfo.u4ChunkSize + 4; //size+type+size+crc
    UINT8 u1ValTmp;

    if (u4ChunkTotalSize > ptPngHdrFifo->u4ValidData)
    {
        ptPngHdrFifo->u4JumpDataSz = u4ChunkTotalSize;
        return PNG_HDR_NEED_MORE_DATA;
    }

    ptPngHdr->tPngIhdr.u4Width = u4PngHdrGet4Bytes(ptPngHdrFifo, 8);
    ptPngHdr->tPngIhdr.u4Height = u4PngHdrGet4Bytes(ptPngHdrFifo, 12); 
    ptPngHdr->tPngIhdr.u1BitDepth = u1PngHdrGet1Byte(ptPngHdrFifo, 16);
    ptPngHdr->tPngIhdr.u1ColorType = u1PngHdrGet1Byte(ptPngHdrFifo, 17);
    u1ValTmp = u1PngHdrGet1Byte(ptPngHdrFifo, 20);
    if (u1ValTmp == 1)
    {
        ptPngHdr->tPngIhdr.fgInterlace = TRUE;
    }
    else
    {
        ptPngHdr->tPngIhdr.fgInterlace = FALSE;
    }
    vPngHdrSkipBytes(ptPngHdrFifo, u4ChunkTotalSize);
    return PNG_HDR_OK;
}

INT32 i4PngHdrPLTE(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    UINT32 u4ChunkTotalSize = 8 + ptPngHdr->tPngChunkInfo.u4ChunkSize + 4; //size+type+size+crc

    if (u4ChunkTotalSize > ptPngHdrFifo->u4ValidData)
    {
        ptPngHdrFifo->u4JumpDataSz = u4ChunkTotalSize;
        return PNG_HDR_NEED_MORE_DATA;
    }

    ptPngHdr->fgPngPalette = TRUE;
    //Set PLTE
    ptPngHdr->tPngPlte.u4ChunkType = PNG_CHUNK_PLTE;
    ptPngHdr->tPngPlte.u4ChunkSize = ptPngHdr->tPngChunkInfo.u4ChunkSize;
    x_memcpy((void *)(ptPngHdr->tPngPlte.u4PlteData), (void *)(ptPngHdrFifo->u4ReadPoint + 8), ptPngHdr->tPngChunkInfo.u4ChunkSize);
    //skip chunk
    HalFlushInvalidateDCacheMultipleLine(ptPngHdr->tPngPlte.u4PlteData, ptPngHdr->tPngChunkInfo.u4ChunkSize);
    vPngHdrSkipBytes(ptPngHdrFifo, u4ChunkTotalSize);
    return PNG_HDR_OK;
}

INT32 i4PngHdrtRNS(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    UINT32 u4ChunkTotalSize = 8 + ptPngHdr->tPngChunkInfo.u4ChunkSize + 4; //size+type+size+crc

    if (u4ChunkTotalSize > ptPngHdrFifo->u4ValidData)
    {
        ptPngHdrFifo->u4JumpDataSz = u4ChunkTotalSize;
        return PNG_HDR_NEED_MORE_DATA;
    }
    
    ptPngHdr->fgPngtRNS = TRUE;
    //Set tRNS
    ptPngHdr->tPngTrns.u4ChunkType = PNG_CHUNK_tRNS;
    ptPngHdr->tPngTrns.u4ChunkSize= ptPngHdr->tPngChunkInfo.u4ChunkSize;
    x_memcpy((void *)(ptPngHdr->tPngTrns.u4TrnsData), (void *)(ptPngHdrFifo->u4ReadPoint + 8), ptPngHdr->tPngChunkInfo.u4ChunkSize);
    //skip chunk
    HalFlushInvalidateDCacheMultipleLine(ptPngHdr->tPngTrns.u4TrnsData, ptPngHdr->tPngChunkInfo.u4ChunkSize);
    vPngHdrSkipBytes(ptPngHdrFifo, u4ChunkTotalSize);
    return PNG_HDR_OK;
}

INT32 i4PngHdrbKGD(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    PNG_BKGD *ptPngBkgd = &(ptPngHdr->tPngBkgd);
    UINT32 u4ChunkTotalSize = 8 + ptPngHdr->tPngChunkInfo.u4ChunkSize + 4; //size+type+size+crc
    INT32 i4Ret = PNG_HDR_OK;

    if (u4ChunkTotalSize > ptPngHdrFifo->u4ValidData)
    {
        ptPngHdrFifo->u4JumpDataSz = u4ChunkTotalSize;
        return PNG_HDR_NEED_MORE_DATA;
    }

    if (ptPngHdr->fgPngPalette)
    {
        ptPngBkgd->u2R = u1PngHdrGet1Byte(ptPngHdrFifo, 8);
    }
    else
    {
        ptPngBkgd->u2R = u1PngHdrGet1Byte(ptPngHdrFifo, 8);
        ptPngBkgd->u2G = u1PngHdrGet1Byte(ptPngHdrFifo, 10);
        ptPngBkgd->u2B = u1PngHdrGet1Byte(ptPngHdrFifo, 12);
    }
    vPngHdrSkipBytes(ptPngHdrFifo, u4ChunkTotalSize);
    return i4Ret;
}

INT32 i4PngHdrSkipChunk(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    UINT32 u4ChunkTotalSize = 8 + ptPngHdr->tPngChunkInfo.u4ChunkSize + 4; //size+type+size+crc
    UINT8 u1ChunkType;
    INT32 i4 = 0;

    for (i4=0; i4<4; i4++)
    {
        u1ChunkType = (ptPngHdr->tPngChunkInfo.u4ChunkType >> (i4*8)) & 0xFF;
        if (((u1ChunkType > 0x7A) || (u1ChunkType < 0x61)) &&
            ((u1ChunkType > 0x5A) || (u1ChunkType < 0x41)))
        {
            return PNG_HDR_FAIL;
        }
    }

    if (u4ChunkTotalSize > ptPngHdrFifo->u4ValidData)
    {
        ptPngHdrFifo->u4JumpDataSz = u4ChunkTotalSize;
        if (u4ChunkTotalSize > ((ptPngHdrFifo->u4FifoEnd - ptPngHdrFifo->u4FifoStart)/2))
        {
            return PNG_HDR_SKIP_DATA;
        }
        else
        {
            return PNG_HDR_NEED_MORE_DATA;
        }
    }

    vPngHdrSkipBytes(ptPngHdrFifo, u4ChunkTotalSize);
    return PNG_HDR_OK;
}

INT32 i4PngHdrIDAT(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);
    INT32 i4Ret;

    while (ptPngHdrFifo->u4ValidData >= 8)
    {
        ptPngHdr->tPngChunkInfo.u4ChunkSize = u4PngHdrGet4Bytes(ptPngHdrFifo, 0);
        ptPngHdr->tPngChunkInfo.u4ChunkType = u4PngHdrGet4Bytes(ptPngHdrFifo, 4);
        switch(ptPngHdr->tPngChunkInfo.u4ChunkType)
        {
            case PNG_CHUNK_IDAT:
#if 0//(CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)
                vPngHdrSkipBytes(ptPngHdrFifo, 8); //chunk type + chunk size
#endif
                return PNG_HDR_DONE;
            case PNG_CHUNK_IEND:
                return PNG_HDR_IEND;
            default:
                if ( (i4Ret = i4PngHdrSkipChunk(ptPngHdr)) != PNG_HDR_OK)
                {
                    return i4Ret;
                }
        }
    }
    return PNG_HDR_NEED_MORE_DATA;
}

INT32 i4PngHdrProcChunk(PNG_HDR_INFO_T *ptPngHdr)
{
    PNG_HDR_FIFO_INFO *ptPngHdrFifo = &(ptPngHdr->tPngHdrFifo);

    if (ptPngHdrFifo->u4ValidData < 12) //size + type + crc
    {
        ptPngHdrFifo->u4JumpDataSz = 12;
        return PNG_HDR_NEED_MORE_DATA;
    }
    ptPngHdr->tPngChunkInfo.u4ChunkSize = u4PngHdrGet4Bytes(ptPngHdrFifo, 0);
    ptPngHdr->tPngChunkInfo.u4ChunkType = u4PngHdrGet4Bytes(ptPngHdrFifo, 4);
    switch(ptPngHdr->tPngChunkInfo.u4ChunkType)
    {
        case PNG_CHUNK_IHDR:
            vPngLog_L("PNG_CHUNK_IHDR\n");
            return i4PngHdrIHDR(ptPngHdr);
        case PNG_CHUNK_IDAT:
            vPngLog_L("PNG_CHUNK_IDAT\n");

            return PNG_HDR_DONE;
        case PNG_CHUNK_PLTE:
            vPngLog_L("PNG_CHUNK_PLTE\n");

            return i4PngHdrPLTE(ptPngHdr);
        case PNG_CHUNK_tRNS:
            vPngLog_L("PNG_CHUNK_tRNS\n");

            return i4PngHdrtRNS(ptPngHdr);
        case PNG_CHUNK_bKGD:
            vPngLog_L("PNG_CHUNK_tRNS\n");

            return i4PngHdrbKGD(ptPngHdr);
        case PNG_CHUNK_IEND:
            vPngLog_L("PNG_CHUNK_IEND\n");

            return PNG_HDR_IEND;
        default:
            return i4PngHdrSkipChunk(ptPngHdr);
    }
}

INT32 i4PngHdrPsr(PNG_HDR_INFO_T *ptPngHdr)
{
    return i4PngHdrProcChunk(ptPngHdr);
}

