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
/*****************************************************************************
*
* Filename:
* ---------
*   $Workfile:$
*
* Project:
* --------
* MT8530
*
* Description:
* ------------
* The prototypes of API for ipod USB Client driver
*
* Author:
* -------
* Hui Zhang
*
* Last changed:
* -------------
* $Author: kai.wang $
*
* $Modtime: $
*
* $Revision: #2 $
****************************************************************************/

#ifndef _PCM_OSS_H_
#define _PCM_OSS_H_

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER

#include <linux/soundcard.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#include "x_common.h"
#include "x_rm.h"
//#include "x_drv_cb.h"     //FIX_BUILD_ERROR
//#include "x_memtype.h"    //FIX_BUILD_ERROR

typedef struct file OSS_FILE;

typedef enum
{
    READ_ONLY,
    WRITE_ONLY,
    DUPLEX
}E_OP_MODE;

typedef enum
{
    DESTROIED,
    CREATED
}E_INST_STATE;

typedef struct
{
    OSS_FILE *ossfile;
    mm_segment_t old_fs;    
    E_INST_STATE eInstStat;
}OSS_INSTANCE_T;

#define MAX_OSS_INST 2

INT32 i4OssCreateInstance(UINT16 u2CardNum, E_OP_MODE eMode);
INT32 i4OssDestroyInstance(VOID);
INT32 i4OssGetOptimumFragmentSize(UINT32 *pu4size);
INT32 i4OssSetChannel(UINT32 u4ChannelNO);
INT32 i4OssSetFormat(UINT32 u4Format);
INT32 i4OssSetFragmentSize(UINT32 u4size);
INT32 i4OssSetSpeed(UINT32 u4SampleRate);
INT32 i4OssWrite(UCHAR *pcBuffer, UINT32 u4size);
INT32 i4OssQueryStatus(VOID);
INT32 i4OssGetOutputDelay(VOID);

#endif

#endif //_PCM_OSS_H_
