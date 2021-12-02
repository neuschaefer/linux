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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_edid.c,v $
 * $Revision: 
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "hw_vdoin.h"
#include "sv_const.h"
#include "hw_pdwnc.h"

extern CODE UINT8 VGA_EDID_TABLE[256];
extern UINT8 HDMIEDID_table[256]; 
extern UINT8 HDMICEC_SETTING[3];
extern void vDrvLoadHdmiEdid2Table(void);
extern void vDrvLoadVgaEdid2Table(void);

#define VGA_EDID_ADDR 0xa0
void vVGA_INTERNAL_EDID_Init(void)
{
	UINT8 i;

    // VGA DDC
    vIO32WriteFldAlign(PDWNC_VGA_DEV, 0, FLD_VGA_EN);
    vIO32WriteFldAlign(PDWNC_VGA_DEV, (VGA_EDID_ADDR >> 2), FLD_DEV_ADDR);	//bit [7:2] is valid
    //setup deglitch
    vIO32WriteFldAlign(PDWNC_VGA_DEV, 1, FLD_DEGLITCH_EN);
    vIO32WriteFldAlign(PDWNC_VGA_DEV, 3, FLD_DE_CNT); //should >=2

    vIO32WriteFldAlign(PDWNC_VGA_ADDR, 0, FLD_VGA_ADDR) ; //start from 0 and auto increase	

#ifndef CC_VGA_EDID_DISABLE 	
    vDrvLoadVgaEdid2Table();
    printf("\nLoad VGA internal EDID.................\n ");
    //VGA EDID dowload
    for(i=0; i<64; i++)
    {
        UINT32 u4Tmp ;
	u4Tmp = (VGA_EDID_TABLE[4*i+3] << 24) + (VGA_EDID_TABLE[4*i+2] << 16) + (VGA_EDID_TABLE[4*i+1] << 8) +(VGA_EDID_TABLE[4*i] ) ;
	vIO32WriteFldAlign(PDWNC_VGA_DATA, u4Tmp, FLD_VGA_DATA); 
    }
    vIO32WriteFldAlign(PDWNC_VGA_DEV, 1, FLD_VGA_EN);
#endif
#if 0
	//verify VGA EDID
	UINT8 tmp, value;
	UINT8 j, addr;
	UINT8 u1Temp[4] ;

	value = 0;
	vIO32WriteFldAlign(PDWNC_VGA_ADDR, 0, FLD_VGA_ADDR) ; //start from 0 and auto increase	
	for(i=0; i<64; i++)
	{
		UINT32 u4Temp ;	
		u4Temp = IO32ReadFldAlign(PDWNC_VGA_DATA, FLD_VGA_DATA);
		u1Temp[0]= (u4Temp)& 0xff;
		u1Temp[1]= (u4Temp>> 8) & 0xff;
		u1Temp[2]= (u4Temp >> 16) & 0xff;
		u1Temp[3]= (u4Temp >> 24) & 0xff;
		for(j=0; j<4; j++)
		{
			if( u1Temp[j] !=  (VGA_EDID_TABLE[4*i+j]) )
			{
				value = 1;	
				Printf("vga\n");
				tmp = VGA_EDID_TABLE[4*i+j];
				Printf("addr %d val: %x ,%x\n", 4*i+j, u1Temp[j],  tmp);
			}			
		}	
	}
	if(value == 1)
	{
		Printf("VGA_EDID_error\n");
	}
	else
	{
		Printf("VGA_EDID_pass\n");
	}

#endif


}

#define INTERNAL_EDID_DMSG
#define HDMI2_CEC_PHY_ADR (*(((UINT8 *)(&HDMICEC_SETTING[0]))+0))
#define HDMI3_CEC_PHY_ADR (*(((UINT8 *)(&HDMICEC_SETTING[0]))+1))
#define HDMI4_CEC_PHY_ADR (*(((UINT8 *)(&HDMICEC_SETTING[0]))+2))
void vHDMI_INTERNAL_EDID_Init(void)
{
#ifndef CC_HDMI_EDID_DISABLE // define this in xxxx.def
    UINT16 i;
    UINT8 u1PAOffset = 159;
    UINT8 u1Hdmi1PA = 0x10;
    UINT8 u1Checksum = 0;
    UINT8 u1Buf = 0;

	vDrvLoadHdmiEdid2Table();

	Printf("Load HDMI internal EDID.................\n ");

       for (i = 0 ; i <= 255 ; i++)
	{
		vIO32Write1B(PDWNC_EDID_ADDR,i);
		/*can't use vIO32Write1B to write data,because 1B will read first then write into,data address will increase 1 */
		vIO32Write4B(PDWNC_EDID_DATA,HDMIEDID_table[i]);
		//Printf("Addr=0x%x, Value=0x%x\n",i,HDMIEDID_CEC_192k_DeepColor12B_P1[i]);
	}

    for (i = 128 ; i < 253 ; i++)  // if 255, HDMIEDID may overflow
    {
        if ((HDMIEDID_table[i] == 0x3) && \
        	(HDMIEDID_table[i+1] == 0xc) && \
        	 (HDMIEDID_table[i+2] == 0x0))
        {
            u1PAOffset = i+3;
        }
    }

    if (u1PAOffset < 255)
    {
        u1Hdmi1PA = HDMIEDID_table[u1PAOffset];
#ifdef INTERNAL_EDID_DMSG        
        Printf("CEC Physical offset %d\n",u1PAOffset);
        Printf("HDMI1 Physical adr 0x%x\n",u1Hdmi1PA);
#endif        
    }
    else 
    {
        Printf("!!!Invalid CEC Physical address offset\n");        
        return;
    }
    
#ifdef INTERNAL_EDID_DMSG 
    Printf("HDMI1 PA 0x%x\n",u1Hdmi1PA);	
    Printf("HDMI1 checksum 0x%x\n",HDMIEDID_table[255]);
#endif    
    u1Checksum = (HDMIEDID_table[255] + u1Hdmi1PA)%256;
    /*
	0x0020,159,0x42,
	0x0030,159,0x32,
	0x0040,159,0x22
	*/
	if (u1Checksum >= HDMI2_CEC_PHY_ADR)
	{
	    u1Buf = u1Checksum - HDMI2_CEC_PHY_ADR;
	}
	else
	{
	    u1Buf = u1Checksum + (0x100 - HDMI2_CEC_PHY_ADR);
	}
#ifdef INTERNAL_EDID_DMSG 	
    Printf("HDMI2 PA 0x%x\n",HDMI2_CEC_PHY_ADR);	
    Printf("HDMI2 checksum 0x%x\n",u1Buf);
#endif    
	vIO32WriteFldAlign(PDWNC_EDID_CTL1, HDMI2_CEC_PHY_ADR, FLD_PHY_ADDR1);
	vIO32WriteFldAlign(PDWNC_EDID_CTL1, u1PAOffset, FLD_OFFSET1);
	vIO32WriteFldAlign(PDWNC_EDID_CTL1, u1Buf, FLD_CHKSUM1);

	if (u1Checksum >= HDMI3_CEC_PHY_ADR)
	{
	    u1Buf = u1Checksum - HDMI3_CEC_PHY_ADR;
	}
	else
	{
	    u1Buf = u1Checksum + (0x100 - HDMI3_CEC_PHY_ADR);
	}
#ifdef INTERNAL_EDID_DMSG 	
    Printf("HDMI3 PA 0x%x\n",HDMI3_CEC_PHY_ADR);
    Printf("HDMI3 checksum 0x%x\n",u1Buf);
#endif    
	vIO32WriteFldAlign(PDWNC_EDID_CTL2, HDMI3_CEC_PHY_ADR, FLD_PHY_ADDR2);
	vIO32WriteFldAlign(PDWNC_EDID_CTL2, u1PAOffset, FLD_OFFSET2);
	vIO32WriteFldAlign(PDWNC_EDID_CTL2, u1Buf, FLD_CHKSUM2);

	if (u1Checksum >= HDMI4_CEC_PHY_ADR)
	{
	    u1Buf = u1Checksum - HDMI4_CEC_PHY_ADR;
	}
	else
	{
	    u1Buf = u1Checksum + (0x100 - HDMI4_CEC_PHY_ADR);
	}
#ifdef INTERNAL_EDID_DMSG 	
    Printf("HDMI4 PA 0x%x\n",HDMI4_CEC_PHY_ADR);
    Printf("HDMI4 checksum 0x%x\n",u1Buf);
#endif    
	vIO32WriteFldAlign(PDWNC_EDID_CTL3, HDMI4_CEC_PHY_ADR, FLD_PHY_ADDR3);
	vIO32WriteFldAlign(PDWNC_EDID_CTL3, u1PAOffset, FLD_OFFSET3);
	vIO32WriteFldAlign(PDWNC_EDID_CTL3, u1Buf, FLD_CHKSUM3);
	
#else //use external edid, disable internal edid.
	printf("-------------use external edid, disable internal edid------------------\n");
	vIO32WriteFldAlign(PDWNC_EDID_DEV0, 1, FLD_EDID0_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV1, 1, FLD_EDID1_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV2, 1, FLD_EDID2_DIS);
	vIO32WriteFldAlign(PDWNC_EDID_DEV3, 1, FLD_EDID3_DIS);
#endif
}

#if defined(CC_MTK_LOADER)
void vInternalEDIDInit(void)
{
	vVGA_INTERNAL_EDID_Init();
	vHDMI_INTERNAL_EDID_Init();
}
#endif
