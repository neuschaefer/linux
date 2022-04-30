/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

#ifndef SPEAR_JPEG_HAL_C
#define SPEAR_JPEG_HAL_C

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include "sprplus_jpeg_syn_hal.h"

/****************************************************************************/
/* 			 JPEG_StartDecoder()	    			*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Starts JPEG Decoding Process.                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                           */
/*      - NumOfLli: Number of LLI's that DMA will contain for transfer to   */
/*                  JPEG.                                                   */
/*      - HdrEnable: Enable or Disable Header.                              */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_JPEGError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_JPEGError JPEG_StartDecoder(uint NumOfLli, t_bool HdrEnable)
{
		/*Configure Ctrl register for number of LLIs.*/
		*JPEG_CTRL_STATUS = 0;
		*JPEG_CTRL_STATUS = (NumOfLli << SHIFT_NUM_LLI);

		/*Configure JPEG core register.*/
		*JPEG_REG1 = 0;
		if(HdrEnable)
				*JPEG_REG1	|= HDR_ENABLE;

		*JPEG_REG1 |= DECODER_ENABLE;

		*JPEG_REG0 = JPEG_ENABLE;
		return JPEG_OK;
}

/****************************************************************************/
/* 						 JPEG_IntClear()	                        	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Clears the JPEG Interrupt.                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                            	*/
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void JPEG_IntClear(void)
{
		/*Clear Interrupt.*/
		*JPEG_CTRL_STATUS &= INT_CLR;
}

/****************************************************************************/
/* 						 JPEG_Disable()	                        	    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Clears the JPEG Interrupt.                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                            	*/
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void JPEG_Disable(void)
{
		*JPEG_REG0 = JPEG_DISABLE;
}

/****************************************************************************/
/* 						 JPEG_GetHdrInfo()	                        	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Gives the Header info of the decoded image.            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                            	*/
/* OUT : 		                                                            */
/*       pHdrInfo: Poiter to the header info struct.                        */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void JPEG_GetHdrInfo(t_JPEGHdrInfo *pHdrInfo)
{
		pHdrInfo->NumOfColorCmp = (*JPEG_REG1 & MASK_NUM_COLOR_CMP) >> SHIFT_NUM_COLOR_CMP;
		pHdrInfo->ColSpcType = (*JPEG_REG1 & MASK_COL_SPC_TYPE) >> SHIFT_COL_SPC_TYPE;
		pHdrInfo->NumOfCmpForScanHdr = (*JPEG_REG1 & MASK_NUM_CMP_SCAN_HDR) >> SHIFT_NUM_CMP_SCAN_HDR;
		pHdrInfo->XSize = (*JPEG_REG3 & MASK_X_SIZE) >> SHIFT_X_SIZE;
		pHdrInfo->YSize = (*JPEG_REG1 & MASK_Y_SIZE) >> SHIFT_Y_SIZE;
		pHdrInfo->NumMcu = (*JPEG_REG2 & MASK_NUM_MCU) >> SHIFT_NUM_MCU;
		pHdrInfo->NumMcuInRST = (*JPEG_REG3 & MASK_RST_NUM_MCU) >> SHIFT_RST_NUM_MCU;
		pHdrInfo->H1 = (*JPEG_REG4 & MASK_HORZ_SAMP) >> SHIFT_HORZ_SAMP;
		pHdrInfo->V1 = (*JPEG_REG4 & MASK_VERT_SAMP) >> SHIFT_VERT_SAMP;
		pHdrInfo->H2 = (*JPEG_REG5 & MASK_HORZ_SAMP) >> SHIFT_HORZ_SAMP;
		pHdrInfo->V2 = (*JPEG_REG5 & MASK_VERT_SAMP) >> SHIFT_VERT_SAMP;
		pHdrInfo->H3 = (*JPEG_REG6 & MASK_HORZ_SAMP) >> SHIFT_HORZ_SAMP;
		pHdrInfo->V3 = (*JPEG_REG6 & MASK_VERT_SAMP) >> SHIFT_VERT_SAMP;
		pHdrInfo->H4 = (*JPEG_REG7 & MASK_HORZ_SAMP) >> SHIFT_HORZ_SAMP;
		pHdrInfo->V4 = (*JPEG_REG7 & MASK_VERT_SAMP) >> SHIFT_VERT_SAMP;
}
#endif /* SPEAR_JPEG_HAL_C*/
