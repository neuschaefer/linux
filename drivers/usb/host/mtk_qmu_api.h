/*! @file Mtk_qmu_api.h
*
*  @par Modification Notice: 
*
*  This software is modified by MediaTek Inc. and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
* $Workfile: Mtk_qmu_api.h$
*
* @par Project: 
*    Mtk usb swip
*
* @par Description: 
*    Mtk DMAQ module interface header file
*    Define DMAQ Atomic Function
*
* @par Author_Name: 
*    tianhao.fei 
*
* @par Last_Changed: add comment for Doxygen
* $Author: dtvbm11 $
* $Modtime: 2010-5-7$
* $Revision: #1 $
*
*/

#ifndef MTK_QMU_API_H
#define MTK_QMU_API_H

#ifdef CONFIG_USB_QMU_SUPPORT
#include "mtk_hcd.h"

extern void QMU_select_dma_ch(MGC_LinuxCd *pThis, uint8_t channel, uint8_t burstmode);

extern void QMU_disable_q(MGC_LinuxCd *pThis, uint8_t bEnd, uint8_t isRx);

extern void QMU_disable_all_q(MGC_LinuxCd *pThis);

extern int QMU_is_enabled(MGC_LinuxCd *pThis, uint8_t EP_Num, uint8_t isRx);

extern int QMU_clean(MGC_LinuxCd *pThis);

extern int QMU_init(MGC_LinuxCd *pThis);

extern int QMU_enable(MGC_LinuxCd *pThis, uint8_t EP_Num, uint8_t isRx, uint8_t isZLP, uint8_t isCSCheck, uint8_t isEmptyCheck);

extern void QMU_disable(MGC_LinuxCd *pThis, uint8_t EP_Num, uint8_t isRx);

extern int QMU_insert_task(MGC_LinuxCd *pThis, uint8_t EP_Num, uint8_t isRx, uint8_t *buf, uint32_t length, uint8_t isIOC);

extern void QMU_stop(MGC_LinuxCd *pThis, uint8_t EP_Num, uint8_t isRx);

extern int QMU_irq(MGC_LinuxCd *pThis, uint32_t qisar);

#endif
#endif
