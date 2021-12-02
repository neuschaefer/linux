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
#ifndef DDR_QMU_H
#define DDR_QMU_H
//#include "base/cerr.h"
//#include "base/ddriver.h"
//#include "base/llist.h"
//#include <linux/list.h>
//#include "ddr_usbhost.h"

#include "x_typedef.h"


BYTE PDU_calcCksum(BYTE *data, int len);
BYTE HDR_calcCksum(BYTE *data);

#define TGPD_FLAGS_HWO              0x01
#define TGPD_IS_FLAGS_HWO(_pd)      (((TGPD *)_pd)->flag & TGPD_FLAGS_HWO)
#define TGPD_SET_FLAGS_HWO(_pd)     (((TGPD *)_pd)->flag |= TGPD_FLAGS_HWO)
#define TGPD_CLR_FLAGS_HWO(_pd)     (((TGPD *)_pd)->flag &= (~TGPD_FLAGS_HWO))
#define TGPD_FORMAT_BDP             0x02
#define TGPD_IS_FORMAT_BDP(_pd)     (((TGPD *)_pd)->flag & TGPD_FORMAT_BDP)
#define TGPD_SET_FORMAT_BDP(_pd)    (((TGPD *)_pd)->flag |= TGPD_FORMAT_BDP)
#define TGPD_CLR_FORMAT_BDP(_pd)    (((TGPD *)_pd)->flag &= (~TGPD_FORMAT_BDP))

#define TGPD_SET_FLAG(_pd, _flag)   ((TGPD *)_pd)->flag = (((TGPD *)_pd)->flag&(~TGPD_FLAGS_HWO))|(_flag)
#define TGPD_GET_FLAG(_pd)             (((TGPD *)_pd)->flag & TGPD_FLAGS_HWO)
#define TGPD_SET_CHKSUM(_pd, _n)    ((TGPD *)_pd)->chksum = PDU_calcCksum((BYTE *)_pd, _n)
#define TGPD_SET_CHKSUM_HWO(_pd, _n)    ((TGPD *)_pd)->chksum = PDU_calcCksum((BYTE *)_pd, _n)-1
#define TGPD_GET_CHKSUM(_pd)        ((TGPD *)_pd)->chksum
#define TGPD_SET_FORMAT(_pd, _fmt)  ((TGPD *)_pd)->flag = (((TGPD *)_pd)->flag&(~TGPD_FORMAT_BDP))|(_fmt)
#define TGPD_GET_FORMAT(_pd)        ((((TGPD *)_pd)->flag & TGPD_FORMAT_BDP)>>1)
#define TGPD_SET_DataBUF_LEN(_pd, _len) ((TGPD *)_pd)->DataBufferLen = _len
#define TGPD_ADD_DataBUF_LEN(_pd, _len) ((TGPD *)_pd)->DataBufferLen += _len
#define TGPD_GET_DataBUF_LEN(_pd)       ((TGPD *)_pd)->DataBufferLen
//#define TGPD_SET_EXT_LEN(_pd, _len) ((TGPD *)_pd)->DataBufferLen = (((TGPD *)_pd)->DataBufferLen&0x00FF)|((_len)<<8)
//#define TGPD_ADD_EXT_LEN(_pd, _len) ((TGPD *)_pd)->DataBufferLen += (_len)<<8
//#define TGPD_GET_EXT_LEN(_pd)       ((TGPD *)_pd)->DataBufferLen>>8
#define TGPD_SET_NEXT(_pd, _next)   ((TGPD *)_pd)->pNext = (TGPD *)_next;
#define TGPD_GET_NEXT(_pd)          (TGPD *)((TGPD *)_pd)->pNext
#define TGPD_SET_TBD(_pd, _tbd)     ((TGPD *)_pd)->pBuf = (BYTE *)_tbd;\
                                    TGPD_SET_FORMAT_BDP(_pd)
#define TGPD_GET_TBD(_pd)           (TBD *)((TGPD *)_pd)->pBuf
#define TGPD_SET_DATA(_pd, _data)   ((TGPD *)_pd)->pBuf = (BYTE *)_data
#define TGPD_GET_DATA(_pd)          (BYTE*)((TGPD *)_pd)->pBuf
#define TGPD_SET_BUF_LEN(_pd, _len) ((TGPD *)_pd)->bufLen = _len
#define TGPD_ADD_BUF_LEN(_pd, _len) ((TGPD *)_pd)->bufLen += _len
#define TGPD_GET_BUF_LEN(_pd)       ((TGPD *)_pd)->bufLen
//#define TGPD_SET_SeqNO(_pd, _Seq)  ((TGPD *)_pd)->SeqNO = _Seq
//#define TGPD_GET_SeqNO(_pd)        ((TGPD *)_pd)->SeqNO
#define TGPD_SET_EXT_LEN(_pd, _len) ((TGPD *)_pd)->ExtLength = _len
#define TGPD_GET_EXT_LEN(_pd)        ((TGPD *)_pd)->ExtLength
#define TGPD_SET_EPaddr(_pd, _EP)  ((TGPD *)_pd)->ZTepFlag =(((TGPD *)_pd)->ZTepFlag&0xF0)|(_EP)
#define TGPD_GET_EPaddr(_pd)        ((TGPD *)_pd)->ZTepFlag & 0x0F 

#define TGPD_FORMAT_TGL             0x10
#define TGPD_IS_FORMAT_TGL(_pd)     (((TGPD *)_pd)->ZTepFlag & TGPD_FORMAT_TGL)
#define TGPD_SET_FORMAT_TGL(_pd)    (((TGPD *)_pd)->ZTepFlag |=TGPD_FORMAT_TGL)
#define TGPD_CLR_FORMAT_TGL(_pd)    (((TGPD *)_pd)->ZTepFlag &= (~TGPD_FORMAT_TGL))
#define TGPD_FORMAT_ZLP             0x20
#define TGPD_IS_FORMAT_ZLP(_pd)     (((TGPD *)_pd)->ZTepFlag & TGPD_FORMAT_ZLP)
#define TGPD_SET_FORMAT_ZLP(_pd)    (((TGPD *)_pd)->ZTepFlag |=TGPD_FORMAT_ZLP)
#define TGPD_CLR_FORMAT_ZLP(_pd)    (((TGPD *)_pd)->ZTepFlag &= (~TGPD_FORMAT_ZLP))

#ifdef CC_MT5368
#define TGPD_FORMAT_IOC             0x80
#define TGPD_IS_FORMAT_IOC(_pd)     (((TGPD *)_pd)->flag & TGPD_FORMAT_IOC)
#define TGPD_SET_FORMAT_IOC(_pd)    (((TGPD *)_pd)->flag |=TGPD_FORMAT_IOC) 
//#define TGPD_SET_FORMAT_IOC(_pd)    (((TGPD *)_pd)->ZTepFlag &= (~TGPD_FORMAT_IOC)) //2010.06.22 for test.
#define TGPD_CLR_FORMAT_IOC(_pd)    (((TGPD *)_pd)->flag &= (~TGPD_FORMAT_IOC))
#endif

#define TGPD_SET_TGL(_pd, _TGL)  ((TGPD *)_pd)->ZTepFlag |=(( _TGL) ? 0x10: 0x00)
#define TGPD_GET_TGL(_pd)        ((TGPD *)_pd)->ZTepFlag & 0x10 ? 1:0
#define TGPD_SET_ZLP(_pd, _ZLP)  ((TGPD *)_pd)->ZTepFlag |= ((_ZLP) ? 0x20: 0x00)
#define TGPD_GET_ZLP(_pd)        ((TGPD *)_pd)->ZTepFlag & 0x20 ? 1:0

#define TGPD_GET_EXT(_pd)           ((BYTE *)_pd + sizeof(TGPD))
#define TGPD_SET_KEY_IDX(_pd, _data) \
{\
	BYTE *_tmp = ((BYTE *)_pd + sizeof(TGPD));\
	*_tmp = (unsigned char)_data;\
}
#define TGPD_GET_KEY_IDX(_pd)        *((BYTE *)_pd + sizeof(TGPD))

typedef struct _TGPD
{
    unsigned char	flag;
    unsigned char	chksum;
    unsigned short	DataBufferLen; /*Rx Allow Length*/
    struct _TGPD*	pNext;
    BYTE*			pBuf;
    unsigned short	bufLen;
    unsigned char	ExtLength;
    unsigned char	ZTepFlag;
}TGPD, *PGPD;



#define TBD_FLAGS_EOL               0x01
#define TBD_IS_FLAGS_EOL(_bd)       (((TBD *)_bd)->flag & TBD_FLAGS_EOL)
#define TBD_SET_FLAGS_EOL(_bd)      (((TBD *)_bd)->flag |= TBD_FLAGS_EOL)
#define TBD_CLR_FLAGS_EOL(_bd)      (((TBD *)_bd)->flag &= (~TBD_FLAGS_EOL))

#define TBD_SET_FLAG(_bd, _flag)    ((TBD *)_bd)->flag = (unsigned char)_flag
#define TBD_GET_FLAG(_bd)           ((TBD *)_bd)->flag
#define TBD_SET_CHKSUM(_pd, _n)     ((TBD *)_pd)->chksum = PDU_calcCksum((BYTE *)_pd, _n)
#define TBD_GET_CHKSUM(_pd)         ((TBD *)_pd)->chksum
#define TBD_SET_NEXT(_bd, _next)    ((TBD *)_bd)->pNext = (TBD *)_next
#define TBD_GET_NEXT(_bd)           (TBD *)((TBD *)_bd)->pNext
#define TBD_SET_DATA(_bd, _data)    ((TBD *)_bd)->pBuf = (BYTE *)_data
#define TBD_GET_DATA(_bd)           (BYTE*)((TBD *)_bd)->pBuf
#define TBD_SET_BUF_LEN(_bd, _len)  ((TBD *)_bd)->bufLen = _len
#define TBD_ADD_BUF_LEN(_bd, _len)  ((TBD *)_bd)->bufLen += _len
#define TBD_GET_BUF_LEN(_bd)        ((TBD *)_bd)->bufLen
#define TBD_SET_EXT_LEN(_bd, _len)  ((TBD *)_bd)->extLen = _len
#define TBD_ADD_EXT_LEN(_bd, _len)  ((TBD *)_bd)->extLen += _len
#define TBD_GET_EXT_LEN(_bd)        ((TBD *)_bd)->extLen
#define TBD_GET_EXT(_bd)            ((BYTE *)_bd + sizeof(TBD))

/**
 * TBD
 */
typedef struct _TBD
{
    unsigned char  flag;
    unsigned char  chksum;
    unsigned short  reserved1;    
    struct _TBD *pNext;
    BYTE *pBuf;
    unsigned short bufLen;    
    unsigned char  extLen;
    unsigned char  reserved2;
}TBD, *PBD;


#define AT_GPD_EXT_LEN	0
#define AT_BD_EXT_LEN 4
#endif

