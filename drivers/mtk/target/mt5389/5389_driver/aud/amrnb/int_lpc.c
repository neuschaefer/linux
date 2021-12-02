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
 *   int_lpc.c
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
*      File             : int_lpc.c
*
********************************************************************************
*/
/*
********************************************************************************
*                         MODULE INCLUDE FILE AND VERSION ID
********************************************************************************
*/
#include "int_lpc.h"
 
/*
********************************************************************************
*                         INCLUDE FILES
********************************************************************************
*/
#include "typedef.h"
#include "basic_op.h"
#include "lsp_az.h" 

/*
********************************************************************************
*                         LOCAL VARIABLES AND TABLES
********************************************************************************
*/
/*
*--------------------------------------*
* Constants (defined in cnst.h)        *
*--------------------------------------*
*  M         : LPC order               *
*  MP1       : LPC order + 1           *
*--------------------------------------*
*/
 
/*
********************************************************************************
*                         PUBLIC PROGRAM CODE
********************************************************************************
*/
/*
**************************************************************************
*
*  Function    : Int_lpc_1and3
*  Purpose     : Interpolates the LSPs and converts to LPC parameters 
*                to get a different LP filter in each subframe.
*  Description : The 20 ms speech frame is divided into 4 subframes.
*                The LSPs are quantized and transmitted at the 2nd and 
*                4th subframes (twice per frame) and interpolated at the
*                1st and 3rd subframe.
*
*                      |------|------|------|------|
*                         sf1    sf2    sf3    sf4
*                   F0            Fm            F1
*      
*                 sf1:   1/2 Fm + 1/2 F0         sf3:   1/2 F1 + 1/2 Fm
*                 sf2:       Fm                  sf4:       F1
*  Returns     : void
*
**************************************************************************
*/
void Int_lpc_1and3 (
	Word32 *f1,
	Word32 *f2,	
	Word16 *lspt,
    Word16 lsp_old[],  /* i : LSP vector at the 4th subfr. of past frame (M) */
    Word16 lsp_mid[],  /* i : LSP vector at the 2nd subfr. of
                              present frame (M)                              */
    Word16 lsp_new[],  /* i : LSP vector at the 4th subfr. of
                              present frame (M)                              */
    Word16 Az[]        /* o : interpolated LP parameters in all subfr.
                              (AZ_SIZE)                                      */
)
{
   Word32 i,s,t;
	Word32 *ptr32m ,*ptr32o;
   Word16 *lsp = lspt;
	Word16 *ptr16 ;
	
	/*  lsp[i] = lsp_mid[i] * 0.5 + lsp_old[i] * 0.5 */
	ptr16 = lsp;
	ptr32m = (Word32 *)lsp_mid;
	ptr32o = (Word32 *)lsp_old;	
	i = M/2;	
	do	
   {
   	s = *ptr32m++;  t = *ptr32o++; 
   	*ptr16++ = ((Word16)s >> 1) + ((Word16)t >> 1); 
   	*ptr16++ = (Word16)((s >> 17) + (t >> 17));                                		
   }while(--i != 0);
	
   Lsp_Az (f1,f2,lsp, Az);           /* Subframe 1 */	 
   Az += (MP1+1);                  

   Lsp_Az (f1,f2,lsp_mid, Az);       /* Subframe 2 */	 
   Az += (MP1+1);                  

	ptr16 = lsp;
	ptr32m = (Word32 *)lsp_mid;
	ptr32o = (Word32 *)lsp_new;	
	i = M/2;	
	do	
   {
   	s = *ptr32m++;  t = *ptr32o++; 
   	*ptr16++ = ((Word16)s >> 1) + ((Word16)t >> 1); 
   	*ptr16++ = (Word16)(s >> 17) + (t >> 17);                                		
   }while(--i != 0);

   Lsp_Az (f1,f2,lsp, Az);           /* Subframe 3 */
   Az += (MP1+1);                  
   Lsp_Az (f1,f2,lsp_new, Az);       /* Subframe 4 */

   return;
}

/*
**************************************************************************
*
*  Function    : Int_lpc_1and3_2
*  Purpose     : Interpolation of the LPC parameters. Same as the Int_lpc
*                function but we do not recompute Az() for subframe 2 and
*                4 because it is already available.
*  Returns     : void
*
**************************************************************************
*/
void Int_lpc_1and3_2 (
    AMR_Encode_FrameState *Enc, 
    Word16 lsp_old[],  /* i : LSP vector at the 4th subfr. of past frame (M) */
    Word16 lsp_mid[],  /* i : LSP vector at the 2nd subframe of
                             present frame (M)                                  */
    Word16 lsp_new[],  /* i : LSP vector at the 4th subframe of
                             present frame (M)                                  */
    Word16 Az[]        /* o :interpolated LP parameters
                             in subframes 1 and 3 (AZ_SIZE)                     */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;
   Word32 i,s,t;
	Word32 *ptr32m ,*ptr32o;	
	Word32 *f1 = Enc_share->shabuf.stE.u2E.s2E.f11;
	Word32 *f2 = Enc_share->shabuf.stE.u2E.s2E.f22;	
   Word16 *ptr16 ;	
	Word16 *lsp = Enc_share->shabuf.stE.u2E.s2E.lsp;

   /*  lsp[i] = lsp_mid[i] * 0.5 + lsp_old[i] * 0.5 */
	ptr16 = lsp;
	ptr32m = (Word32 *)lsp_mid;
	ptr32o = (Word32 *)lsp_old;	
	i = M/2;	
	do	
   {
   	s = *ptr32m++;  t = *ptr32o++; 
   	*ptr16++ = ((Word16)s >> 1) + ((Word16)t >> 1); 
   	*ptr16++ = (Word16)((s >> 17) + (t >> 17));                                		
   }while(--i != 0);

   Lsp_Az (f1,f2,lsp, Az);           /* Subframe 1 */	
   Az += (MP1+1) * 2;             

	ptr16 = lsp;
	ptr32m = (Word32 *)lsp_mid;
	ptr32o = (Word32 *)lsp_new;	
	i = M/2;	
	do	
   {
   	s = *ptr32m++;  t = *ptr32o++; 
   	*ptr16++ = ((Word16)s >> 1) + ((Word16)t >> 1); 
   	*ptr16++ = (Word16)(s >> 17) + (t >> 17);                                		
   }while(--i != 0);

   Lsp_Az (f1,f2,lsp, Az);           /* Subframe 3 */

   return;
}
/*************************************************************************
 *
 *  FUNCTION:  Int_lpc_1to3()
 *
 *  PURPOSE:  Interpolates the LSPs and convert to LP parameters to get
 *            a different LP filter in each subframe.
 *
 *  DESCRIPTION:
 *     The 20 ms speech frame is divided into 4 subframes.
 *     The LSPs are quantized and transmitted at the 4th subframe
 *     (once per frame) and interpolated at the 1st, 2nd and 3rd subframe.
 *
 *          |------|------|------|------|
 *             sf1    sf2    sf3    sf4
 *       F0                          F1
 *
 *     sf1:   3/4 F0 + 1/4 F1         sf3:   1/4 F0 + 3/4 F1
 *     sf2:   1/2 F0 + 1/2 F1         sf4:       F1
 *
 *************************************************************************/
void Int_lpc_1to3(
	Word32 *f1,
	Word32 *f2,	
	Word16 *lspt,
    Word16 lsp_old[], /* input : LSP vector at the 4th SF of past frame    */
    Word16 lsp_new[], /* input : LSP vector at the 4th SF of present frame */
    Word16 Az[]       /* output: interpolated LP parameters in all SFs     */
)
{
   Word32 i;
   Word16 *lsp = lspt;
	Word16 *ptr16  ,*ptr16o , *ptr16n;

	ptr16  = lsp;
	ptr16n = lsp_new;
	ptr16o = lsp_old;	
	i = M/2;	
	do	
   {
        *ptr16++ = (*ptr16n >> 2) + (*ptr16o - (*ptr16o >> 2)); ptr16n++; ptr16o++;
        *ptr16++ = (*ptr16n >> 2) + (*ptr16o - (*ptr16o >> 2)); ptr16n++; ptr16o++;
   }while(--i != 0);

   Lsp_Az(f1,f2,lsp, Az);        /* Subframe 1 */
   Az += MP1+1;              

	ptr16 = lsp;
	ptr16n = lsp_new;
	ptr16o = lsp_old;	
	i = M/2;	
	do	
   {
   	*ptr16++ = (*ptr16o >> 1) + (*ptr16n >> 1); ptr16n++; ptr16o++;
   	*ptr16++ = (*ptr16o >> 1) + (*ptr16n >> 1); ptr16n++; ptr16o++;                               		
   }while(--i != 0);

   Lsp_Az(f1,f2,lsp, Az);        /* Subframe 2 */
   Az += MP1+1;              

	ptr16  = lsp;
	ptr16n = lsp_new;
	ptr16o = lsp_old;	
	i = M/2;	
	do	
   {
        *ptr16++ = (*ptr16o >> 2) + (*ptr16n - (*ptr16n >> 2)); ptr16n++; ptr16o++;
        *ptr16++ = (*ptr16o >> 2) + (*ptr16n - (*ptr16n >> 2)); ptr16n++; ptr16o++;
   }while(--i != 0);

   Lsp_Az(f1,f2,lsp, Az);       /* Subframe 3 */
   Az += MP1+1;              

   Lsp_Az(f1,f2,lsp_new, Az);        /* Subframe 4 */

   return;
}

/*************************************************************************
 * Function Int_lpc_1to3_2()
 * Interpolation of the LPC parameters.
 * Same as the previous function but we do not recompute Az() for
 * subframe 4 because it is already available.
 *************************************************************************/

void Int_lpc_1to3_2(
     AMR_Encode_FrameState *Enc, 
     Word16 lsp_old[],  /* input : LSP vector at the 4th SF of past frame    */
     Word16 lsp_new[],  /* input : LSP vector at the 4th SF of present frame */
     Word16 Az[]        /* output: interpolated LP parameters in SFs 1,2,3   */
)
{
  	AMR_Shared_Buf *Enc_share = Enc->share;	
   Word32 i;	 
	Word32 *f1 = Enc_share->shabuf.stE.u2E.s2E.f11;
	Word32 *f2 = Enc_share->shabuf.stE.u2E.s2E.f22;

	
   Word16 *ptr16  ,*ptr16o , *ptr16n;
	Word16 *lsp = Enc_share->shabuf.stE.u2E.s2E.lsp;

	ptr16  = lsp;
	ptr16n = lsp_new;
	ptr16o = lsp_old;	
	i = M/2;	
	do	
   {
        *ptr16++ = (*ptr16n >> 2) + (*ptr16o - (*ptr16o >> 2)); ptr16n++; ptr16o++;
        *ptr16++ = (*ptr16n >> 2) + (*ptr16o - (*ptr16o >> 2)); ptr16n++; ptr16o++;
   }while(--i != 0);


   Lsp_Az(f1,f2,lsp, Az);        /* Subframe 1 */
   Az += MP1+1;              
	
	ptr16 = lsp;
	ptr16n = lsp_new;
	ptr16o = lsp_old;	
	i = M/2;	
	do	
   {
   	*ptr16++ = (*ptr16o >> 1) + (*ptr16n >> 1); ptr16n++; ptr16o++;
   	*ptr16++ = (*ptr16o >> 1) + (*ptr16n >> 1); ptr16n++; ptr16o++;                               		
   }while(--i != 0);

   Lsp_Az(f1,f2,lsp, Az);        /* Subframe 2 */
   Az += MP1+1;              

	ptr16  = lsp;
	ptr16n = lsp_new;
	ptr16o = lsp_old;	
	i = M/2;	
	do	
   {
        *ptr16++ = (*ptr16o >> 2) + (*ptr16n - (*ptr16n >> 2)); ptr16n++; ptr16o++;
        *ptr16++ = (*ptr16o >> 2) + (*ptr16n - (*ptr16n >> 2)); ptr16n++; ptr16o++;
   }while(--i != 0);

   Lsp_Az(f1,f2,lsp, Az);        /* Subframe 3 */

    return;
}

#endif

