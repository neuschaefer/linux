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


#ifndef __PNG_COM_HDR_H
#define __PNG_COM_HDR_H

#include "x_typedef.h"

#define PNG_SIGNATURE1  0x89504E47
#define PNG_SIGNATURE2  0x0D0A1A0A

#define PNG_CHUNK_IHDR  0x49484452
#define PNG_CHUNK_IDAT  0x49444154
#define PNG_CHUNK_PLTE  0x504C5445
#define PNG_CHUNK_tRNS  0x74524E53
#define PNG_CHUNK_bKGD  0x624B4744
#define PNG_CHUNK_IEND  0x49454E44

#define PNG_HDR_IEND            2   //parsing finish
#define PNG_HDR_DONE            1   //parsing finish
#define PNG_HDR_OK              0   //function return ok because valid header
#define PNG_HDR_FAIL            -1  //function return fail because invalid header
#define PNG_HDR_NEED_MORE_DATA  -2  //need more data
#define PNG_HDR_SKIP_DATA       -3  //need jump some data

typedef struct {
    UINT32 u4Width;
    UINT32 u4Height;
    UINT8  u1BitDepth;
    UINT8  u1ColorType;
    BOOL   fgInterlace;
} PNG_IHDR;

typedef struct {
    UINT32 u4ChunkType;
    UINT32 u4ChunkSize;
    UINT32 u4PlteData;
} PNG_PLTE;

typedef struct {
    UINT32 u4ChunkType;
    UINT32 u4ChunkSize;
    UINT32 u4TrnsData;
} PNG_TRNS;

typedef struct {
    UINT16 u2R;
    UINT16 u2G;
    UINT16 u2B;
} PNG_BKGD;

typedef struct {
    UINT32 u4ChunkSize;
    UINT32 u4ChunkType;
    UINT32 u4ChunkCRC;
} PNG_CHUNK_INFO;

typedef struct {
    UINT32 u4FifoStart;     //[IN] need update by drv/verify code
    UINT32 u4FifoEnd;       //[IN] need update by drv/verify code
    UINT32 u4ReadPoint;     //[OUT/IN] need update by drv/verify/com code
    UINT32 u4WritePoint;    //[IN] need update by drv/verify code
    UINT32 u4JumpDataSz;    //[OUT] jump data from current read point, retunr SKIP, do skip, retunr MORE_DATA, check data available
    UINT32 u4ValidData;     //[IN] the valid data
} PNG_HDR_FIFO_INFO;

typedef struct _PNG_HDR_INFO_T
{
    PNG_HDR_FIFO_INFO   tPngHdrFifo;
    PNG_CHUNK_INFO      tPngChunkInfo;
    PNG_IHDR            tPngIhdr;
    PNG_PLTE            tPngPlte;
    PNG_TRNS            tPngTrns;
    PNG_BKGD            tPngBkgd;
    BOOL                fgPngPalette;
    BOOL                fgPngtRNS;
    BOOL                fgPngbKGD;
    UINT32              u41stIDATOfst;
} PNG_HDR_INFO_T;

INT32 i4PngHdrChkPngSign(PNG_HDR_INFO_T *ptPngHdr);
INT32 i4PngHdrPsr(PNG_HDR_INFO_T *ptPngHdr);
INT32 i4PngHdrIDAT(PNG_HDR_INFO_T *ptPngHdr);
void vPngHdrInit(PNG_HDR_INFO_T *ptPngHdr);

#endif // __PNG_COM_HDR_H
