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
 *   g_adapt.c
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
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0                
*                                REL-4 Version 4.1.0                
*
********************************************************************************
*
*      File             : g_adapt.c
*      Purpose          : gain adaptation for MR795 gain quantization
*
********************************************************************************
*/

/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "g_adapt.h"


/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/

#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "gmed_n.h"

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
#define LTP_GAIN_THR1 2721 /* 2721 Q13 = 0.3322 ~= 1.0 / (10*log10(2)) */
#define LTP_GAIN_THR2 5443 /* 5443 Q13 = 0.6644 ~= 2.0 / (10*log10(2)) */

/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/


/*************************************************************************
*
*  Function:   gain_adapt_reset
*  Purpose:    Initializes state memory to zero
*
**************************************************************************
*/
int gain_adapt_reset (GainAdaptState *st)
{
   st->onset = 0;
   st->prev_alpha = 0;
   st->prev_gc = 0;


	st->ltpg_mem[0] = 0;
   st->ltpg_mem[1] = 0;
	st->ltpg_mem[2] = 0;
   st->ltpg_mem[3] = 0;
	st->ltpg_mem[4] = 0;

    return 0;
}


/*************************************************************************
 *
 *  Function:   gain_adapt()
 *  Purpose:    calculate pitch/codebook gain adaptation factor alpha
 *              (and update the adaptor state)
 *
 **************************************************************************
 */
void gain_adapt(
    GainAdaptState *st,  /* i  : state struct                  */
    Word16 ltpg,         /* i  : ltp coding gain (log2()), Q13 */
    Word16 gain_cod,     /* i  : code gain,                Q1  */
    Word16 *alpha        /* o  : gain adaptation factor,   Q15 */
)
{
    Word16 adapt;      /* adaptdation status; 0, 1, or 2       */
    Word16 result;     /* alpha factor, Q13                    */
    Word16 filt;       /* median-filtered LTP coding gain, Q13 */
    Word16 tmp;
    
    /* basic adaptation */

    if(ltpg <= LTP_GAIN_THR1)
    {
        adapt = 0;                           
    }
    else
    {
        if(ltpg <= LTP_GAIN_THR2)
        {
            adapt = 1;                        
        }
        else
        {
            adapt = 2;                        
        }
    }

    /*
     * // onset indicator
     * if ((cbGain > onFact * cbGainMem[0]) && (cbGain > 100.0))
     *     onset = 8;
     * else
     *     if (onset)
     *         onset--;
     */
    /* tmp = cbGain / onFact; onFact = 2.0; 200 Q1 = 100.0 */
    tmp = shr_r(gain_cod, 1);
	 
    if (( (tmp > st->prev_gc) ) && (gain_cod > 200) )
    {
        st->onset = 8;                            
    }
    else
    {

        if (st->onset != 0)
        {
            st->onset = (st->onset - 1);       
        }
    }

    /*
     *  // if onset, increase adaptor state
     *  if (onset && (gainAdapt < 2)) gainAdapt++;
     */

    if ((st->onset != 0) && ( (adapt < 2) ))
    {
        adapt = (adapt + 1);
    }

    st->ltpg_mem[0] = ltpg;                       
    filt = gmed_n (st->ltpg_mem, 5);    /* function result */


    if (adapt == 0)
    {
        if ( (filt > 5443) ) /* 5443 Q13 = 0.66443... */
        {
            result = 0; 
        }
        else
        {

            if (filt < 0)
            {
                result = 16384;   /* 16384 Q15 = 0.5 */
            }
            else
            {   /* result       =   0.5 - 0.75257499*filt     */
                /* result (Q15) = 16384 - 24660 * (filt << 2) */
                filt = (filt << 2); /* Q15 */
                result =  (16384 - (smulbb(24660, filt)>>15) );
            }
        }
    }
    else
    {
        result = 0; 
    }
    /*
     *  if (prevAlpha == 0.0) result = 0.5 * (result + prevAlpha);
     */

    if (st->prev_alpha == 0)
    {
        result = (result >> 1);
    }

    /* store the result */
    *alpha = result;                           
    
    /* update adapter state memory */
    st->prev_alpha = result;                   
    st->prev_gc = gain_cod;                    
 
    st->ltpg_mem[4] = st->ltpg_mem[3];   
    st->ltpg_mem[3] = st->ltpg_mem[2];   
    st->ltpg_mem[2] = st->ltpg_mem[1];   
    st->ltpg_mem[1] = st->ltpg_mem[0];   


	 
    /* mem[0] is just present for convenience in calling the gmed_n[5]
     * function above. The memory depth is really LTPG_MEM_SIZE-1.
     */
}

#endif

