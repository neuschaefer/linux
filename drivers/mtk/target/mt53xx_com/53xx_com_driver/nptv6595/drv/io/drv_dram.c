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
 * $Date  $
 * $RCSfile: drv_dram.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "x_dram.h"
#include "x_debug.h"
#include "drv_dram.h"
#include "x_timer.h"
UINT32 NR_DRAM_BASE;
UINT32 NR_DRAM_SIZE;
UINT32 TDC_DRAM_BASE;
UINT32 TDC_DRAM_SIZE;
UINT32 VBI_DRAM_BASE;
UINT32 VBI_DRAM_SIZE;
UINT32 SW_DRAM_BASE;
UINT32 SW_DRAM_SIZE;


/**
 * Read one UINT8 from Dram.
 *
 * @param dwAddr fetch address
 * @return the data
 */
UINT8 bReadDramB(UINT32 dwAddr)
{
    UINT8 u1Tmp;

    u1Tmp = (UINT8) ((*((UINT32 *) dwAddr)) >> 24);

    return (u1Tmp);
}

/**
 * Read one UINT16 from Dram.
 *
 * @param dwAddr fetch address
 * @return the data
 */
UINT16 wReadDramW(UINT32 dwAddr)
{
    UINT16 u2Tmp;

    u2Tmp = (UINT16) ((*((UINT32 *) dwAddr)) >> 16);

    return (u2Tmp);
}

/**
 * Read one double-UINT16 from Dram.
 *
 * @param fetch address
 * @return the data
 * @warning The dwAddr must be 4-UINT8 aligned, Or this function will apply 
 * UINT8 read method to fetch data.
 */
UINT32 dwReadDramD(UINT32 dwAddr)
{
    return (*((UINT32 *) dwAddr));
}

/* Wirte one UINT8 to Dram.
 *
 * @param dwAddr write address
 * @param bData write data
 */
void vWriteDramB(UINT32 u4Addr, UINT8 bData)
{
    UINT32 u4Tmp;
    UINT32 *ptr = (UINT32 *) u4Addr;

    u4Tmp = (dwReadDramD(u4Addr) & 0xffffff00) | bData;
    (*ptr) = u4Tmp;
}

/**
 * Write one UINT16 to Dram.
 *
 * @param dwAddr write address
 * @param wDmaLen write data
 */
void vWriteDramW(UINT32 u4Addr, UINT16 wData)
{
    UINT32 u4Tmp;
    UINT32 *ptr = (UINT32 *) u4Addr;

    u4Tmp = (dwReadDramD(u4Addr) & 0xffff0000) | wData;
    (*ptr) = u4Tmp;
}

/**
 * Write one double-UINT16 to Dram.
 *
 * @param dwAddr write address
 * @param dwData write data
 * @warning The dwAddr must be 4-UINT8 aligned or this function will apply 
 * UINT8 read method to fetch data.
 */
void vWriteDramD(UINT32 u4Addr, UINT32 u4Data)
{
    UINT32 *ptr = (UINT32 *) u4Addr;

    (*ptr) = u4Data;
}

/* for Big Endian Function */
/**
 * Read one UINT16 from Dram. (Big Endian)
 *
 * @param dwAddr fetch address
 * @return the data
 */
UINT16 wReadDramW_B(UINT32 dwAddr)
{
    return (wReadDramW(dwAddr));        // 5371
}

/**
 * Read one double-UINT16 from Dram. (Big Endian)
 *
 * @param fetch address
 * @return the data
 */
UINT32 dwReadDramD_B(UINT32 dwAddr)
{
    return (dwReadDramD(dwAddr));

}

/**
 * Write one UINT16 to Dram. (Big Endian)
 *
 * @param dwAddr write address
 * @param wDmaLen write data
 */
void vWriteDramW_B(UINT32 dwAddr, UINT16 wData)
{
    vWriteDramW(dwAddr, wData);
}

/**
 * Write one double-UINT16 to Dram. (Big Endian)
 *
 * @param dwAddr write address
 * @param dwData write data
 */
void vWriteDramD_B(UINT32 dwAddr, UINT32 dwData)
{
    vWriteDramD(dwAddr, dwData);
}




// OCD/ODT calibration

#define DRAM_ODTP		0x00070	//0x70c0[6:4]
#define DRAM_ODTN		0x00007	//0x70c0[2:0]
#define DRAM_DRVN		0x00f00	//0x70C0[11:8]
#define DRAM_DRVP		0x0f000	//0x70C0[15:12]
#define DRAM_ODTNE		0x00008	//0x70C0[3]
#define DRAM_ODTPE		0x00080	//0x70C0[7]
#define DRAM_CALP		0x40000	//0x70C0[18]
#define DRAM_DRVNE		0x20000	//0x70c0[17]
#define DRAM_DRVPE		0x10000	//0x70c0[16]

#define DRAM_REF_0D4	0x10000	//0x7100[16]
#define DRAM_REF_0D5	0x20000	//0x7100[17]
#define DRAM_DRVREF		0x01000000	//0x7100[24]

#define DETAILLOG			0

#if 0
INT32 u4GDDR3ODTP,u4GDDR3DRVP_tmp, u4GDDR3DRVN, u4GDDR3DRVP;

void vGDDR3Cal(void)
{
    UINT32 u4DLLSEL, u4OCDK, u4mask_dllsel, u4mask_ocdk;
    UINT32 u4CMPOT, u4CMPOT0;
	UINT32 i ,j;
	UINT8 bfound;


	u4DLLSEL = DRAM_READ32(0xc0);
	u4OCDK = DRAM_READ32(0x100);

	u4mask_ocdk = ~(DRAM_REF_0D4 | DRAM_REF_0D5 | DRAM_DRVREF);
	u4mask_dllsel = ~(DRAM_ODTP | DRAM_ODTN | DRAM_DRVN | DRAM_DRVP | DRAM_ODTNE | DRAM_ODTPE | DRAM_CALP | DRAM_DRVNE | DRAM_DRVPE);


	//cal ODTP 
	u4OCDK = (u4OCDK & u4mask_ocdk) | DRAM_DRVREF;	//DRAM_REF_0D4=0, DRAM_REF_0D5=0
	DRAM_WRITE32(0x100, u4OCDK);
#if DETAILLOG	
    LOG(3,"0x7100=0x%x\n", u4OCDK);
#endif

	bfound = 0;
	u4CMPOT0 = 0;
	for(j=8; (j<=12) && (bfound==0); j+=2)
	{
		for(i=0; (i<=7) && (bfound==0); i++)
		{
	//		u4DLLSEL = (u4DLLSEL & u4mask_dllsel) | (1 << 18) | (0x0a << 12) | (i << 4) | (1 << 7); 
			u4DLLSEL = (u4DLLSEL & u4mask_dllsel) | (1 << 18) | (j << 12) | (i << 4) | (1 << 7); 		
			DRAM_WRITE32(0xc0, u4DLLSEL);

			u4CMPOT = DRAM_READ32(0x3dc) & 0x80000000;		
			if(u4CMPOT0 != u4CMPOT)
			{
				u4CMPOT0 = u4CMPOT;
				bfound = 1;
				if(i!=0)
				{
					u4GDDR3DRVP_tmp = j;
					u4GDDR3ODTP = i-1;
				}
				else
				{
					u4GDDR3DRVP_tmp = j - 2;
					u4GDDR3ODTP = 7;
				}
			}
#if DETAILLOG	
		    LOG(3, "0x70c0=0x%x, 0x73dc=0x%x \n", u4DLLSEL, u4CMPOT);
#endif
		}
	}
//	LOG(3, "1.ODTP=0x%x DRVP=0x%x ", u4GDDR3ODTP, u4GDDR3DRVP);



	//cal DRVN
	u4OCDK = (u4OCDK & u4mask_ocdk) | DRAM_DRVREF | DRAM_REF_0D4;	//DRAM_REF_0D4=1, DRAM_REF_0D5=0
	DRAM_WRITE32(0x100, u4OCDK);
#if DETAILLOG	
    LOG(3,"0x7100=0x%x\n", u4OCDK);
#endif


	for(i=0; i<=15; i++)
	{
//		u4DLLSEL = (u4DLLSEL & u4mask_dllsel) | (i<<8) | (1<<17) | (0x0a<<12) | (7) | (1<<3) | (ODTP<<4) | (1<<7); 
		u4DLLSEL = (u4DLLSEL & u4mask_dllsel) | (i<<8) | (1<<17) | (u4GDDR3DRVP_tmp<<12) | (7) | (1<<3) | (u4GDDR3ODTP<<4) | (1<<7); 
		DRAM_WRITE32(0xc0, u4DLLSEL);

		u4CMPOT = DRAM_READ32(0x3dc) & 0x80000000;		
		if(u4CMPOT0 != u4CMPOT)
		{
			u4CMPOT0 = u4CMPOT;
			if(i!=0)
			{
				u4GDDR3DRVN = i-1;
			}
		}
#if DETAILLOG			
	    LOG(3, "0x70c0=0x%x, 0x73dc=0x%x \n", u4DLLSEL, u4CMPOT);
#endif	    
	}
//	LOG(3, "2.DRVN=0x%x ", u4GDDR3DRVN);



	//cal DRVP
	u4OCDK = (u4OCDK & u4mask_ocdk) | DRAM_DRVREF | DRAM_REF_0D4;	//DRAM_REF_0D4=0, DRAM_REF_0D5=1
	DRAM_WRITE32(0x100, u4OCDK);
#if DETAILLOG		
    LOG(3, "0x7100=0x%x\n", u4OCDK);
#endif


	for(i=0; i<=15; i++)
	{
		u4DLLSEL = (u4DLLSEL & u4mask_dllsel) | (u4GDDR3DRVN<<8) | (1<<17) | (i<<12) | (1<<16) | (7) | (1<<3) | (7<<4) | (1<<7); 
		DRAM_WRITE32(0xc0, u4DLLSEL);

		u4CMPOT = DRAM_READ32(0x3dc) & 0x80000000;		
		if(u4CMPOT0 != u4CMPOT)
		{
			u4CMPOT0 = u4CMPOT;
			if(i!=0)
			{
				u4GDDR3DRVP = i-1;
			}
		}
#if DETAILLOG	
	    LOG(3, "0x70c0=0x%x, 0x73dc=0x%x \n", u4DLLSEL, u4CMPOT);
#endif
	}
//	LOG(3, "3.DRVP=0x%x \n", u4GDDR3DRVP);



	//final setting
	u4DLLSEL = (u4DLLSEL & u4mask_dllsel) | (u4GDDR3DRVN<<8) | (1<<17) | (u4GDDR3DRVP<<12) | (1<<16) | (7) | (1<<3) | (u4GDDR3ODTP<<4) | (1<<7); 
	DRAM_WRITE32(0xc0, u4DLLSEL);
#if DETAILLOG	
    LOG(3, "final setting 0x70c0=0x%x\n", u4DLLSEL);
#endif

}



#define CALIBRATION_TOLERANCE	2
UINT32 u4GDDR3OnlineCal=0;	//0:disable 1:on-line cal wo log 2. on-line cal with log
UINT32 u4OnlineCalTime=60;	//cal per 1sec

void vGDDR3OnlineCal(void)
{
	static UINT32 cnt=0, u4time=0;
	static INT32 ODTP=0x00ff, DRVP, DRVN;
	UINT32 u4DRVCTRL;
	UINT8 u1RTT;
	if(u4GDDR3OnlineCal != 0)
	{
		if(cnt != 0)
		{
			cnt--;
		}
		else
		{
			u4time++;
			cnt = u4OnlineCalTime;
			vGDDR3Cal();
			
			if( (ODTP+CALIBRATION_TOLERANCE<u4GDDR3ODTP) || 
			    (ODTP-CALIBRATION_TOLERANCE>u4GDDR3ODTP) ||
			    (DRVN+CALIBRATION_TOLERANCE<u4GDDR3DRVN) ||
			    (DRVN-CALIBRATION_TOLERANCE>u4GDDR3DRVN) ||
				(DRVP+CALIBRATION_TOLERANCE<u4GDDR3DRVP) ||
			    (DRVP-CALIBRATION_TOLERANCE>u4GDDR3DRVP) )
			{
				ODTP = u4GDDR3ODTP & 0x07;
				DRVN = u4GDDR3DRVN & 0x0f;
				DRVP = u4GDDR3DRVP & 0x0f;

				switch(ODTP)
				{
					case 0:
						u1RTT = 0;
						break;
					case 1:
						u1RTT = 2;
						break;
					case 2:
						u1RTT = 1;
						break;
					case 3:
						u1RTT = 3;
						break;
					case 4:
						u1RTT = 4;
						break;
					case 5:
						u1RTT = 6;
						break;
					case 6:
						u1RTT = 5;
						break;
					case 7:
						u1RTT = 7;
						break;
					default:
					    u1RTT = 0;
					    break;
				}


				LOG(3,"Update %d: 1.ODTP=0x%x DRVP=0x%x 2.DRVN=0x%x 3.DRVP=0x%x\n",
					u4time, u4GDDR3ODTP, u4GDDR3DRVP_tmp, u4GDDR3DRVN,u4GDDR3DRVP);

				//DRVCTL0: DQS,DQ
				u4DRVCTRL = DRAM_READ32(0xb8);
				u4DRVCTRL = (u4DRVCTRL & 0x008F008F) | (DRVP<<28) | (DRVN<<24) | (u1RTT<<20) |
													   (DRVP<<12) | (DRVN<< 8) | (u1RTT<< 4);
				DRAM_WRITE32(0xb8, u4DRVCTRL);
				LOG(3,"0x70b8=%x ", u4DRVCTRL);

	
				//DRVCTL1: CMD
				u4DRVCTRL = DRAM_READ32(0xbc);
				u4DRVCTRL = (u4DRVCTRL & 0xFFFF008F) | (DRVP<<12) | (DRVN<< 8) | (u1RTT<< 4);
				DRAM_WRITE32(0xbc, u4DRVCTRL);
				LOG(3,"0x70bc=%x \n", u4DRVCTRL);
		
			}


			
			if(u4GDDR3OnlineCal == 2)
			{
				LOG(3, "%d: 1.ODTP=0x%x DRVP=0x%x 2.DRVN=0x%x 3.DRVP=0x%x\n",
					u4time, u4GDDR3ODTP, u4GDDR3DRVP_tmp, u4GDDR3DRVN,u4GDDR3DRVP);
			}			
		}
	}

}
#endif
