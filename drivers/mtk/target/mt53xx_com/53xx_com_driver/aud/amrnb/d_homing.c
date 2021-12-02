/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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
 *   d_homing.c
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR codec implementation
 *
 * Author:
 * -------
 *   Stan Huang
 *
 *------------------------------------------------------------------------------
 * $Revision: #1 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

/*
*****************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
*****************************************************************************
*
*      File             : d_homing.c
*
*****************************************************************************
*/

/*
*****************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
*****************************************************************************
*/

#include "d_homing.h"


/*
*****************************************************************************
*                         INCLUDE FILES
*****************************************************************************
*/


#include "typedef.h"
#include "bits2prm.h"


#include "tab_bitno.h"
static const Word16 dhf_MR475[PRMNO_MR475] =
{
    0x00F8,
    0x009D,
    0x001C,
    0x0066,
    0x0000,
    0x0003,
    0x0028,
    0x000F,
    0x0038,
    0x0001,
    0x000F,
    0x0031,
    0x0002,
    0x0008,
    0x000F,
    0x0026,
    0x0003
};
static const Word16 dhf_MR515[PRMNO_MR515] =
{
    0x00F8,
    0x009D,
    0x001C,
    0x0066,
    0x0000,
    0x0003,
    0x0037,
    0x000F,
    0x0000,
    0x0003,
    0x0005,
    0x000F,
    0x0037,
    0x0003,
    0x0037,
    0x000F,
    0x0023,
    0x0003,
    0x001F
};
static const Word16 dhf_MR59[PRMNO_MR59] =
{
    0x00F8,
    0x00E3,
    0x002F,
    0x00BD,
    0x0000,
    0x0003,
    0x0037,
    0x000F,
    0x0001,
    0x0003,
    0x000F,
    0x0060,
    0x00F9,
    0x0003,
    0x0037,
    0x000F,
    0x0000,
    0x0003,
    0x0037
};
static const Word16 dhf_MR67[PRMNO_MR67] =
{
    0x00F8,
    0x00E3,
    0x002F,
    0x00BD,
    0x0002,
    0x0007,
    0x0000,
    0x000F,
    0x0098,
    0x0007,
    0x0061,
    0x0060,
    0x05C5,
    0x0007,
    0x0000,
    0x000F,
    0x0318,
    0x0007,
    0x0000
};
static const Word16 dhf_MR74[PRMNO_MR74] =
{
    0x00F8,
    0x00E3,
    0x002F,
    0x00BD,
    0x0006,
    0x000F,
    0x0000,
    0x001B,
    0x0208,
    0x000F,
    0x0062,
    0x0060,
    0x1BA6,
    0x000F,
    0x0000,
    0x001B,
    0x0006,
    0x000F,
    0x0000
};
static const Word16 dhf_MR795[PRMNO_MR795] =
{
    0x00C2,
    0x00E3,
    0x002F,
    0x00BD,
    0x0006,
    0x000F,
    0x000A,
    0x0000,
    0x0039,
    0x1C08,
    0x0007,
    0x000A,
    0x000B,
    0x0063,
    0x11A6,
    0x000F,
    0x0001,
    0x0000,
    0x0039,
    0x09A0,
    0x000F,
    0x0002,
    0x0001
};
static const Word16 dhf_MR102[PRMNO_MR102] =
{
    0x00F8,
    0x00E3,
    0x002F,
    0x0045,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x001B,
    0x0000,
    0x0001,
    0x0000,
    0x0001,
    0x0326,
    0x00CE,
    0x007E,
    0x0051,
    0x0062,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x015A,
    0x0359,
    0x0076,
    0x0000,
    0x001B,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x017C,
    0x0215,
    0x0038,
    0x0030
};
static const Word16 dhf_MR122[PRMNO_MR122] =
{
    0x0004,
    0x002A,
    0x00DB,
    0x0096,
    0x002A,
    0x0156,
    0x000B,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0036,
    0x000B,
    0x0000,
    0x000F,
    0x000E,
    0x000C,
    0x000D,
    0x0000,
    0x0001,
    0x0005,
    0x0007,
    0x0001,
    0x0008,
    0x0024,
    0x0000,
    0x0001,
    0x0000,
    0x0005,
    0x0006,
    0x0001,
    0x0002,
    0x0004,
    0x0007,
    0x0004,
    0x0002,
    0x0003,
    0x0036,
    0x000B,
    0x0000,
    0x0002,
    0x0004,
    0x0000,
    0x0003,
    0x0006,
    0x0001,
    0x0007,
    0x0006,
    0x0005,
    0x0000
};
/* overall table with the parameters of the
   decoder homing frames for all modes */
static const Word16 * const dhf[] =
{
   dhf_MR475,
   dhf_MR515,
   dhf_MR59,
   dhf_MR67,
   dhf_MR74,
   dhf_MR795,
   dhf_MR102,
   dhf_MR122
};

/* get rid of compiler warning "`bitno' defined but never used" */
static void* dummy[] = { (void *) bitno, (void *) dummy };


/*
*****************************************************************************
*                         PRIVATE PROGRAM CODE
*****************************************************************************
*/

/*
********************************************************************************
*
*     Function        : dhf_test
*     In              : input_frame[]  one frame of encoded serial bits
*                       mode           mode type
*                       nparms         number of parameters to check
*     Out             : none
*     Calls           : Bits2prm
*     Tables          : d_homing.tab
*     Compile Defines : none
*     Return          : 0  input frame does not match the decoder homing
*                          frame pattern (up to nparms)
*                       1  input frame matches the decoder homing frame pattern
*                          (for the first nparms parameters)
*     Information     : The encoded serial bits are converted to all parameters
*                       of the corresponding mode. These parameters are compared
*                       with all parameters of the corresponding decoder homing frame.
*
********************************************************************************
*/

static Word16 dhf_test (Word16 input_frame[], enum Mode mode, Word16 nparms)
{
    Word32 i, j;
    Word16 param[MAX_PRM_SIZE];


    /* retrieve the encoded parameters from the received serial bits */
    Bits2prm(mode, input_frame, param);

    if (mode > MR122)
        return (Word16)0;

    j = 0;

    /* check if the encoded parameters matches the parameters
       of the corresponding decoder homing frame */
    for (i = 0; i < nparms; i++)
    {
        j = param[i] ^ dhf[mode][i];

        if (j)
			break;
    }

    return !j;
}

/*
*****************************************************************************
*                         PUBLIC PROGRAM CODE
*****************************************************************************
*/


/*
********************************************************************************
*
*     Function        : decoder_homing_frame_test
*     In              : input_frame[]  one frame of encoded serial bits
*                       mode           mode type
*     Out             : none
*     Calls           : dhf_test
*     Tables          : d_homing.tab
*     Compile Defines : none
*     Return          : 0  input frame does not match the decoder homing frame
*                          pattern
*                       1  input frame matches the decoder homing frame pattern
*     Information     : The encoded serial bits are converted to all parameters
*                       of the corresponding mode. These parameters are compared
*                       with all parameters of the corresponding decoder homing frame.
*
********************************************************************************
*/

Word16 decoder_homing_frame_test (Word16 input_frame[], enum Mode mode)
{
    if (mode > MRDTX)
        return (Word16)0;

    /* perform test for COMPLETE parameter frame */
    return dhf_test(input_frame, mode, prmno[mode]);
}


/*
********************************************************************************
*
*     Function        : decoder_homing_frame_test_first
*     In              : input_frame[]  one frame of encoded serial bits
*                       mode           mode type
*     Out             : none
*     Calls           : Bits2prm
*     Tables          : d_homing.tab
*     Compile Defines : none
*     Return          : 0  input frame does not match the decoder homing frame
*                          pattern (up to and including the first subframe)
*                       1  input frame matches the decoder homing frame pattern
*                          (up to and including the first subframe)
*     Information     : The encoded serial bits are converted to all parameters
*                       of the corresponding mode. These parameters are
*                       compared with the parameters for LPC and first subframe
*                       of the decoder homing frame.
*
********************************************************************************
*/

Word16 decoder_homing_frame_test_first (Word16 input_frame[], enum Mode mode)
{
    if (mode > MR122)
        return (Word16)0;
    /* perform test for FIRST SUBFRAME of parameter frame ONLY */
    return dhf_test(input_frame, mode, prmnofsf[mode]);
}

#endif

