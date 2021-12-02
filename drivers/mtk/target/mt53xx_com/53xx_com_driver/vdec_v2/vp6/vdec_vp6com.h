/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6com.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

#ifndef VDEC_VP6_COM_H
#define VDEC_VP6_COM_H

#define VP6_DEBUG
#ifdef CC_VP6SW_ENABLE
#define VP6_USE_SW_DECODER
#endif

#ifdef CC_VP6HW_ENABLE
#define VP6_USE_HW_DECODER
#endif

#ifdef VP6_USE_SW_DECODER

//#define VP6_COMBUV_IN_DECODING
#ifndef VP6_COMBUV_IN_DECODING
//#define VP6_USE_IMGRZ
//#define VP6_ONLYY_TO_FB
//#define VP6_COMB_FUNCTION_VERIFY
#endif
#endif

#ifdef VP6_USE_SW_DECODER
typedef enum
{
   VP6_INSTANCE_CB_HEADER_INFO,
   VP6_INSTANCE_CB_RPT,
   VP6_INSTANCE_CB_FRAME_TYPE,
   VP6_INSTANCE_CB_RESET_FRAME,
   VP6_INSTANCE_CB_MALLOC,
   VP6_INSTANCE_CB_FREE,
#ifdef VP6_DEBUG
   VP6_INSTANCE_CB_DBGPARAM,
#endif
   VP6_INSTANCE_CB_RESIZE,
   VP6_INSTANCE_CB_MAX
}VP6_INSTANCE_CB_TYPE;

typedef enum
{  
   VP6_UPDATE_SET_INIT,
   VP6_UPDATE_FRAME_CUR,
   VP6_UPDATE_FRAME_LAST,
   VP6_UPDATE_FRAME_GOLDEN,
   VP6_UPDATE_SET_BUFFER,
   VP6_UPDATE_FRAME_NULL,
}VP6_UPDATE_FRAME_TYPE;

typedef struct
{
  unsigned long u4InY;
  unsigned long u4InCb;
  unsigned long u4InCr;
  unsigned long u4OutY;
  unsigned long u4OutC;
  unsigned long u4RZBuf;
}VP6_IMGRZ_PARAM;

typedef unsigned char (*VP6_INSTANCE_CB)(
    long i4DecId,
    unsigned long u4Type, unsigned long u4Param1, unsigned long u4Param2, unsigned long u4Param3
);

#define FlashMemSet x_memset
#define FlashMemCpy x_memcpy
extern void *x_memcpy(void *pv_to, const void *pv_from, unsigned int z_l);
extern void *x_memset(void *pv_to, unsigned char ui1_c, unsigned int z_l);
//extern void HalFlushInvalidateDCache(void);
extern long _VDEC_Vp6DecDecodeFrame(unsigned long u4Instance,unsigned char *puFrameData,unsigned long u4DataLen);
extern unsigned char _VDEC_Vp6DecUpdateFrameBuf(unsigned long  u4Instance,unsigned long  u4FrameType,unsigned long  u4YAddr,unsigned long  u4CAddr);
extern unsigned char _VDEC_Vp6DecUpdateWorkingBuffer(unsigned long u4Instance,unsigned char *puWorkBuf);
extern unsigned char _VDEC_Vp6DecSetCb(unsigned long u4Instance,unsigned long u4CbPtr);
extern unsigned char _VDEC_Vp6SwSetReturnParam(unsigned long u4Instance,void *Param1Addr,void *Param2Addr,void *Param3Addr,void *Param4Addr);
extern unsigned char _VDEC_VP6SwCreateInstance(unsigned long *u4Instance,long i4DecId,VP6_INSTANCE_CB pfCallBack);
extern unsigned char _VDEC_Vp6DecReleaseInstance(unsigned long u4Instance);
extern unsigned char _Vdec_VP6SwGetHeaderInfo( unsigned long u4Instance,unsigned char *pBitstream,unsigned long u4Len);
extern unsigned char _VDEC_Vp6DecGetPicInfo(unsigned long u4Instance,unsigned long *pu4Yaddr,unsigned long *puCaddr,unsigned long *puPitch,unsigned long *puYDataOffset,unsigned long *puCDataOffset);
#endif
#endif
