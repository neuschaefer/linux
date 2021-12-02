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
 *   WBAMR_BASICOP2.C
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$  
 * $Log:$
 *
 *
 *******************************************************************************/

#ifdef LINUX_TURNKEY_SOLUTION

#include "wbamr_typedef.h"
#include "wbamr_basic_op.h"
#include "wbamr_sections.h"


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_Shr_r                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as Shr_fr1x16(var1,var2) but with rounding. Saturate the result    |
 |   in case of                                                              |
 |   underflows or overflows :                                               |
 |    - If var2 is greater than zero :                                       |
 |          if (Sub_fr1x16(Shl_fr1x16(Shr_fr1x16(var1,var2),1),              | 
 |              Shr_fr1x16(var1,Sub_fr1x16(var2,1))))                        |
 |          is equal to zero                                                 |
 |                     then                                                  |
 |                     WBAMR_Shr_r(var1,var2) = Shr_fr1x16(var1,var2)        |
 |                     else                                                  |
 |                     WBAMR_Shr_r(var1,var2) = Add_fr1x16(                  |
 |                                                 Shr_fr1x16(var1,var2),1)  |
 |    - If var2 is less than or equal to zero :                              |
 |                     WBAMR_Shr_r(var1,var2) = Shr_fr1x16(var1,var2).       |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
WBAMR_CODE_SECTION fract16 WBAMR_Shr_r ( 
  fract16 var1, 
  fract16 var2
)
{
  return AWB_shr_r((fract32)var1, (fract32)var2);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_L_deposit_h                                       |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 MS bits of the 32 bit output. The   |
 |   16 LS bits of the output are zeroed.                                    |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (fract32) whose value falls in the |
 |             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                |
 |___________________________________________________________________________|
*/
WBAMR_CODE_SECTION fract32 WBAMR_L_deposit_h (
  fract16 var1
)
{
  return AWB_L_deposit_h((fract32)var1);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_L_deposit_l                                       |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 LS bits of the 32 bit output. The   |
 |   16 MS bits of the output are sign extended.                             |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (fract32) whose value falls in the |
 |             range : 0xFFFF 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
WBAMR_CODE_SECTION fract32 WBAMR_L_deposit_l (
  fract16 var1
)
{
  return AWB_L_deposit_l((fract32)var1);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_L_shr_r                                           |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as Shr_fr1x32(L_var1,var2) but with rounding. Saturate the result in
 |   case of underflows or overflows :                                       |
 |    - If var2 is greater than zero :                                       |
 |          if (Sub_fr1x32(Shl_fr1x32(Shr_fr1x32(L_var1,var2),1),            |
 |                        Shr_fr1x32(L_var1,Sub_fr1x16(var2,1))))            |
 |          is equal to zero                                                 |
 |                     then                                                  |
 |                     WBAMR_L_shr_r(L_var1,var2) = Shr_fr1x32(L_var1,var2)  |
 |                     else                                                  |
 |                     WBAMR_L_shr_r(L_var1,var2) = Add_fr1x32(Shr_fr1x32    |
 |                                                       (L_var1,var2),1)    |
 |    - If var2 is less than or equal to zero :                              |
 |                     WBAMR_L_shr_r(L_var1,var2) = Shr_fr1x32(L_var1,var2). |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (fract32) whose value falls in the |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (fract32) whose value falls in the |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/


WBAMR_CODE_SECTION fract32 WBAMR_L_shr_r ( 
  fract32 L_var1, 
  fract16 var2
)
{
  return AWB_L_shr_r(L_var1, (fract32)var2);
}


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : WBAMR_Norm_s                                            |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shift needed to normalize the 16 bit varia- |
 |   ble var1 for positive values on the interval with minimum of 16384 and  |
 |   maximum of 32767, and for negative values on the interval with minimum  |
 |   of -32768 and maximum of -16384; in order to normalize the result, the  |
 |   following operation must be done :                                      |
 |                    norm_var1 = Shl_fr1x16(var1,WBAMR_Norm_s(var1)).       |
 |                                                                           |
 |   Complexity weight : 15                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (fract16) whose value falls in the|
 |             range : 0x0000 0000 <= var_out <= 0x0000 000f.                |
 |___________________________________________________________________________|
*/
WBAMR_CODE_SECTION fract16 WBAMR_Norm_s (
  fract16 var1
)
{
  return (fract16)AWB_norm_s((fract32)var1);
}

#endif

