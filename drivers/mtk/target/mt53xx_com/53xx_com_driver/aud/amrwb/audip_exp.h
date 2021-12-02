/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
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
*****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 *   audip_exp.h
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   The SWIP Common Functions
 *
 * Author:
 * -------
 *   Doug Wang
 *
 *==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/
#ifndef _AUDIP_EXP_H
#define _AUDIP_EXP_H

#define AUDIP_VALUE0        0xAABB
#define AUDIP_VALUE1        0x7788
#define AUDIP_VALUE2        0x5566
#define AUDIP_VALUE3        0x8787
#define AUDIP_VALUE4        0x3462
#define AUDIP_VALUE5        0x4342
#define AUDIP_VALUE6        0x1A3B
#define AUDIP_VALUE7        0xCABD
#define AUDIOKEKY           0x5353

#define IPCOMMON_PLUS

extern void DRVTST( int *param1, int *param2, int *param3 );
extern void DRVSET( int *param1, int *param2, int *param3 );
extern void AUDIP_ENTRY( int *param1, int *param2, int *param3 );
extern void CODEC_ENTRY( int *param1, int *param2, int *param3 );
extern void STREAM_ENTRY( int *param1, int *param2, int *param3 );
extern void SYSTEM_SPECIFIC( int *param1, int *param2, int *param3 );
extern void SYSTEM_EXCLUSIVE( int *param1, int *param2, int *param3 );
extern void SYSTEM_SETUP( int *param1, int *param2, int *param3 );
extern void SYSTEM_COMMON( int *param1, int *param2, int *param3 );
extern void SEQSTRM( int *param1, int *param2, int *param3 );
extern void SYNSTRM( int *param1, int *param2, int *param3 );
extern int AUDIP_REFERENCE( int *param1, int *param2, int *param3 );
extern void AUDIP_DRVSET(void);
extern void AUDIP_COMMON(void);

#endif
