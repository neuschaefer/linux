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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_vdoin.h $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 *   VDOIN Module register definition and access function declaration
 *
 * Author:
 * -------
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/27 1:22p $
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_VDOIN_
#define _HW_VDOIN_

#ifndef CC_UP8032_ATV
#include "typedef.h"
#include "x_hal_5381.h"


#define VDOIN_BASE 	(IO_VIRT + 0x20000)
#define REG_ALL_FUNC 		0	/* 0 faster, but no reg func to set break ptr */
#define REG_2LAYER_IO32		1	/* 0 faster, but no reg func to set break ptr */


/* Reg READ/WRITE function, for 82xx & 53xx
 *    provide for VDOON(16bit,2002xxxx)register
 *    82xx only has 16 bit address space.
 */

#if REG_ALL_FUNC
UINT8 u1RegRead1B(UINT16 reg16);
UINT32 u4RegRead4B(UINT16 reg16);
void vRegWrite4B(UINT16 reg16, UINT32 val32);
void vRegWrite4B_S(UINT16 reg16, UINT8 bByte3, UINT8 bByte2, UINT8 bByte1, UINT8 bByte0);
#else
#define u1RegRead1B(reg16) 		(*(volatile UINT8 *)(VDOIN_BASE+(reg16)))
#define u4RegRead4B(reg16) 		(*(volatile UINT32 *)(VDOIN_BASE+(reg16)))
#define vRegWrite4B(reg16,val32)	(*(volatile UINT32 *)(VDOIN_BASE+(reg16))=(val32))
#define vRegWrite4B_S(reg16,bByte3,bByte2,bByte1,bByte0)	(*(volatile UINT32 *)(VDOIN_BASE+(reg16))=(((bByte3)<<24)|((bByte2)<<16)|((bByte1)<<8)|(bByte0)))
#endif

#if !REG_2LAYER_IO32
#define u2RegRead2B(reg16) u2IO32Read2B(VDOIN_BASE|(UINT16)(reg16))
#define vRegWrite1B(reg16,val8) vIO32Write1B(VDOIN_BASE|(UINT16)(reg16),(val8))
#define vRegWrite1BMsk(reg16,val8,msk8) vIO32Write1BMsk(VDOIN_BASE|(UINT16)(reg16),(val8),(msk8))
#define vRegWrite2B(reg16,val16) vIO32Write2B(VDOIN_BASE|(UINT16)(reg16),(val16))
#define vRegWrite2BMsk(reg16,val16,msk16) vIO32Write2BMsk(VDOIN_BASE|(UINT16)(reg16),(val16),(msk16))
#define vRegWrite4BMsk(reg16,val32,msk32) vIO32Write4BMsk(VDOIN_BASE|(UINT16)(reg16),(val32),(msk32))
#else
UINT16 u2RegRead2B(UINT16 reg16);
void vRegWrite1B(UINT16 reg16,/*UINT8*/UINT32 val8);
void vRegWrite1BMsk(UINT16 reg16,/*UINT8*/UINT32 val8,UINT8 msk8);
void vRegWrite2B(UINT16 reg16, /*UINT16*/UINT32 val16);
void vRegWrite2BMsk(UINT16 reg16, /*UINT16*/UINT32 val16, UINT16 msk16);
void vRegWrite4BMsk(UINT16 reg16, UINT32 val32, UINT32 msk32);
#endif

/* some issue lint -e666 need to add outside */
#define	RegReadFld(reg16,fld) 	/*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?u1RegRead1B((reg16)+(Fld_ac(fld)-AC_FULLB0)):( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?u2RegRead2B((reg16)+(Fld_ac(fld)-AC_FULLW10)):( \
	(Fld_ac(fld)==AC_FULLDW)? u4RegRead4B(reg16):( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?(u1RegRead1B((reg16)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?(u2RegRead2B((reg16)+(Fld_ac(fld)-AC_MSKW10))&Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))):( \
	(Fld_ac(fld)==AC_MSKDW)?(u4RegRead4B(reg16)&Fld2Msk32(fld)):0 \
  )))))) /*lint -restore */


#define	RegReadFldAlign(reg16,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?u1RegRead1B((reg16)+(Fld_ac(fld)-AC_FULLB0)):( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?u2RegRead2B((reg16)+(Fld_ac(fld)-AC_FULLW10)):( \
	(Fld_ac(fld)==AC_FULLDW)? u4RegRead4B(reg16):( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?((u1RegRead1B((reg16)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0)))>>((Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKB0))&7)):( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?((u2RegRead2B((reg16)+(Fld_ac(fld)-AC_MSKW10))&Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10)))>>((Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKW10))&15)):( \
	(Fld_ac(fld)==AC_MSKDW)?((u4RegRead4B(reg16)&Fld2Msk32(fld))>>Fld_shft(fld)):0 \
  ))))))  /*lint -restore */

#define	vRegWriteFld(reg16,val,fld)  /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))? vRegWrite1B((reg16)+(Fld_ac(fld)-AC_FULLB0),(val)),0:( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?vRegWrite2B((reg16)+(Fld_ac(fld)-AC_FULLW10),(val)),0:( \
	(Fld_ac(fld)==AC_FULLDW)?vRegWrite4B((reg16),(val)),0:( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?vRegWrite1BMsk((reg16)+(Fld_ac(fld)-AC_MSKB0),(val),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?vRegWrite2BMsk((reg16)+(Fld_ac(fld)-AC_MSKW10),(val),Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))),0:( \
	(Fld_ac(fld)==AC_MSKDW)?vRegWrite4BMsk((reg16),(val),Fld2Msk32(fld)),0:0\
	))))))  /*lint -restore */

#define	vRegWriteFldAlign(reg16,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?vRegWrite1B((reg16)+(Fld_ac(fld)-AC_FULLB0),(val)),0:( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?vRegWrite2B((reg16)+(Fld_ac(fld)-AC_FULLW10),(val)),0:( \
	(Fld_ac(fld)==AC_FULLDW)?vRegWrite4B((reg16),(val)),0:( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?vRegWrite1BMsk((reg16)+(Fld_ac(fld)-AC_MSKB0),ValAlign2Fld((val),fld),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?vRegWrite2BMsk((reg16)+(Fld_ac(fld)-AC_MSKW10),ValAlign2Fld((val),fld),Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))),0:( \
	(Fld_ac(fld)==AC_MSKDW)?vRegWrite4BMsk((reg16),((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld)),0:0\
	))))))  /*lint -restore */


/*
 * multiple field reg access, 2008 March
 * ex. vRegWriteFldMulti(CDET_0D,P_Fld(3,DET_V625_GAIN)|P_Fld(3,DET_V525_GAIN));
 * P_Fld(val,fld)  :  assign value and field definition into one packed macro
 * P_Pad1(wid,shft) :  for stuffing 1 to unused bits, make full byte access possible
 * P_Pad0(wid,shft) :  for stuffing 0 to unused bits, make full byte access possible
 * P_PadX(val,wid,shft) :  for stuffing dedicate value to unused bits, make full byte access possible
 */
#define P_Fld(val,fld) ((sizeof(upk)>1)?Fld2Msk32(fld):(((UINT32)(val)&((1<<Fld_wid(fld))-1))<<Fld_shft(fld)))

/* Use when input val range is already safe, reduce code size
  * P_Fnm(val,fld)  :  assign value and field definition into one packed macro,but doesn't mask val inside macro
  */
#define P_Fnm(val,fld) ((sizeof(upk)>1)?Fld2Msk32(fld):((UINT32)(val)<<Fld_shft(fld)))

#define P_Pad1(wid,shft) ((((UINT32)1<<(wid))-1)<<(shft))
#define P_Pad0(wid,shft) ((sizeof(upk)>1)?P_Pad1(wid,shft):0)
#define P_PadX(val,wid,shft) ((sizeof(upk)>1)?P_Pad1(wid,shft):(val)<<(shft))

#define   vRegWriteFldMulti(reg16,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(INT32)(list)};/* const UINT32 msk= (list);*/\
	{UINT8 upk;\
	((UINT32)msk==0xff)?vRegWrite1B(reg16,(list)),0:(\
	((UINT32)msk==0xff00)?vRegWrite1B(reg16+1,(list)>>8),0:(\
	((UINT32)msk==0xff0000)?vRegWrite1B(reg16+2,(list)>>16),0:(\
	((UINT32)msk==0xff000000)?vRegWrite1B(reg16+3,(list)>>24),0:(\
	((UINT32)msk==0xffff)?vRegWrite2B(reg16,(list)),0:(\
	((UINT32)msk==0xffff00)?vRegWrite2B(reg16+1,(list)>>8),0:(\
	((UINT32)msk==0xffff0000)?vRegWrite2B(reg16+2,(list)>>16),0:(\
	((UINT32)msk==0xffffffff)?vRegWrite4B(reg16,(list)),0:(\
	(((UINT32)msk&0xff)&&(!((UINT32)msk&0xffffff00)))?vRegWrite1BMsk(reg16,(list),(UINT8)(UINT32)msk),0:(\
	(((UINT32)msk&0xff00)&&(!((UINT32)msk&0xffff00ff)))?vRegWrite1BMsk(reg16+1,(list)>>8,(UINT8)((UINT32)msk>>8)),0:(\
	(((UINT32)msk&0xff0000)&&(!((UINT32)msk&0xff00ffff)))?vRegWrite1BMsk(reg16+2,(list)>>16,(UINT8)((UINT32)msk>>16)),0:(\
	(((UINT32)msk&0xff000000)&&(!((UINT32)msk&0x00ffffff)))?vRegWrite1BMsk(reg16+3,(list)>>24,(UINT8)((UINT32)msk>>24)),0:(\
	(((UINT32)msk&0xffff)&&(!((UINT32)msk&0xffff0000)))?vRegWrite2BMsk(reg16,(list),(UINT16)(UINT32)msk),0:(\
	(((UINT32)msk&0xffff00)&&(!((UINT32)msk&0xff0000ff)))?vRegWrite2BMsk(reg16+1,(list)>>8,(UINT16)((UINT32)msk>>8)),0:(\
	(((UINT32)msk&0xffff0000)&&(!((UINT32)msk&0x0000ffff)))?vRegWrite2BMsk(reg16+2,(list)>>16,(UINT16)((UINT32)msk>>16)),0:(\
	(UINT32)(msk)?vRegWrite4BMsk(reg16,(list),(UINT32)msk),0:0\
	)))))))))))))));\
	}\
}/*lint -restore */


#define   vRegWriteFldD4val(reg16,dval32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(INT32)(list)};\
	{UINT8 upk;\
	vRegWrite4B(reg16,((dval32)&~(UINT32)msk)|(list));\
	}\
}/*lint -restore */


// ======================================================================================================================================================
/*	 Reg READ/WRITE function By I2C , for 8297 & 539x later */

UINT8 u1MT8297DirRead1B(UINT8 reg8);
UINT8 u1MT8297InDRead1B(UINT8 reg8);
void vMT8297DirWrite1B(UINT8 reg8, UINT8 val8);
void vMT8297InDWrite1B(UINT8 reg8,UINT8 val8);
void vMT8297DirWrite1BMsk(UINT8 reg8,UINT8 val8,UINT8 msk8);
void vMT8297InDWrite1BMsk(UINT8 reg8,UINT8 val8,UINT8 msk8);
	
#define	MT8297DirReadFld(reg8,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
				((Fld_ac(fld) == AC_FULLB0)?u1MT8297DirRead1B((reg8)+(Fld_ac(fld)-AC_FULLB0)):( \
				(Fld_ac(fld) == AC_MSKB0)?(u1MT8297DirRead1B((reg8)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):0 \
			   ))/*lint -restore */
			
#define	MT8297DirReadFldAlign(reg8,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
				((Fld_ac(fld) == AC_FULLB0)?u1MT8297DirRead1B((reg8)+(Fld_ac(fld)-AC_FULLB0)):( \
				(Fld_ac(fld) == AC_MSKB0)?((u1MT8297DirRead1B((reg8)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0)))>>((Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKB0))&7)):0 \
			  ))/*lint -restore */
			
#define	vMT8297DirWriteFld(reg8,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */  \
				((Fld_ac(fld) == AC_FULLB0)? vMT8297DirWrite1B((reg8)+(Fld_ac(fld)-AC_FULLB0),(UINT8)(val)),0:( \
				(Fld_ac(fld) == AC_MSKB0)?vMT8297DirWrite1BMsk((reg8)+(Fld_ac(fld)-AC_MSKB0),(UINT8)(val),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:0 \
			  ))/*lint -restore */
			
#define	vMT8297DirWriteFldAlign(reg8,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
				((Fld_ac(fld) == AC_FULLB0)?vMT8297DirWrite1B((reg8)+(Fld_ac(fld)-AC_FULLB0),(UINT8)(val)),0:( \
				(Fld_ac(fld) == AC_MSKB0)?vMT8297DirWrite1BMsk((reg8)+(Fld_ac(fld)-AC_MSKB0),(UINT8)ValAlign2Fld((val),fld),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:0 \
			  ))/*lint -restore */

#define	MT8297InDReadFld(reg8,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
			((Fld_ac(fld) == AC_FULLB0)?u1MT8297InDRead1B((reg8)+(Fld_ac(fld)-AC_FULLB0)):( \
			(Fld_ac(fld) == AC_MSKB0)?(u1MT8297InDRead1B((reg8)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):0 \
		   ))/*lint -restore */
		
#define	MT8297InDReadFldAlign(reg8,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
			((Fld_ac(fld) == AC_FULLB0)?u1MT8297InDRead1B((reg8)+(Fld_ac(fld)-AC_FULLB0)):( \
			(Fld_ac(fld) == AC_MSKB0)?((u1MT8297InDRead1B((reg8)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0)))>>((Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKB0))&7)):0 \
		  ))/*lint -restore */
		
#define	vMT8297InDWriteFld(reg8,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
			((Fld_ac(fld) == AC_FULLB0)? vMT8297InDWrite1B((reg8)+(Fld_ac(fld)-AC_FULLB0),(UINT8)(val)),0:( \
			(Fld_ac(fld) == AC_MSKB0)?vMT8297InDWrite1BMsk((reg8)+(Fld_ac(fld)-AC_MSKB0),(UINT8)(val),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:0 \
		  ))/*lint -restore */
		
#define	vMT8297InDWriteFldAlign(reg8,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
			((Fld_ac(fld) == AC_FULLB0)?vMT8297InDWrite1B((reg8)+(Fld_ac(fld)-AC_FULLB0),(UINT8)(val)),0:( \
			(Fld_ac(fld) == AC_MSKB0)?vMT8297InDWrite1BMsk((reg8)+(Fld_ac(fld)-AC_MSKB0),(UINT8)ValAlign2Fld((val),fld),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:0 \
		  ))/*lint -restore */

#define   vMT8297DirWriteFldMulti(reg8,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(list)};/* const UINT32 msk= (list);*/\
	{UINT8 upk;\
	(msk==0xff)?vMT8297DirWrite1B(reg8,(UINT8)(list)),0:(\
	(msk)?vMT8297DirWrite1BMsk(reg8,(list),(msk)),0:0);\
	}\
}/*lint -restore */

#define   vMT8297InDWriteFldMulti(reg8,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(list)};/* const UINT32 msk= (list);*/\
	{UINT8 upk;\
	(msk==0xff)?vMT8297InDWrite1B(reg8,(UINT8)(list)),0:(\
	(msk)?vMT8297InDWrite1BMsk(reg8,(list),(msk)),0:0);\
	}\
}/*lint -restore */

#define   vMT8297DirWriteFldD4val(reg8,dval8,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(list)};\
	{UINT8 upk;\
	vMT8297DirWrite1B(reg8,((dval8)&~msk)|(list));\
	}\
}/*lint -restore */
#define   vMT8297InDWriteFldD4val(reg8,dval8,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(list)};\
	{UINT8 upk;\
	vMT8297InDWrite1B(reg8,((dval8)&~msk)|(list));\
	}\
}/*lint -restore */

#endif /* CC_UP8032_ATV */

// ======================================================================================================================================================
#ifdef CC_UP8032_ATV
#include "hw_reg.h" // FOR Replace THE Register ACCESS Declare before
#endif
BOOL fgIsVdoinInt(UINT32 u4mask);

// *********************************************************************
// HW register definitions
// *********************************************************************
#ifndef CC_UP8032_ATV
#ifndef IO_VDOIN_BASE
#define IO_VDOIN_BASE (0x2000)
#endif
#else
#define IO_VDOIN_BASE 0x4000
#endif

#define INT_COLLECT (IO_VDOIN_BASE+0)
 #define INT_COLLECT_0 (IO_VDOIN_BASE+0)
 #define INT_COLLECT_1 (IO_VDOIN_BASE+1)
 #define INT_COLLECT_2 (IO_VDOIN_BASE+2)
 #define INT_COLLECT_3 (IO_VDOIN_BASE+3)
	#define INTR_SW_TEST		Fld(1, 31, AC_MSKB3) //31
	#define INTR_FIFO_TDC		Fld(1, 30, AC_MSKB3) //30
	#define INTR_HDMI_SCDT		Fld(1, 29, AC_MSKB3) //29
	#define INTR_HDMI_MUTE		Fld(1, 28, AC_MSKB3) //28
	#define INTR_HDMI 		Fld(1, 27, AC_MSKB3) //27
	#define INTR_DVI_DE_ACT		Fld(1, 26, AC_MSKB3) //26
	#define INTR_DVI_DE_MODE	Fld(1, 25, AC_MSKB3) //25
	#define INTR_DVI_DE_STABLE	Fld(1, 24, AC_MSKB3) //24
	#define INTR_STA_CHG_TVD	Fld(1, 23, AC_MSKB2) //23
	#define INTR_SP1_V		Fld(1, 16, AC_MSKB2) //16
	#define INTR_VBI2		Fld(1, 15, AC_MSKB1) //15
	#define INTR_SP1_MODE		Fld(1, 14, AC_MSKB1) //14
	#define INTR_DDS_LOCK		Fld(1, 13, AC_MSKB1) //13
	#define INTR_SP0_V		Fld(1, 12, AC_MSKB1) //12
	#define INTR_SP0_MODE		Fld(1, 11, AC_MSKB1) //11
	#define INTR_PIP_V		Fld(1, 10, AC_MSKB1) //10
	#define INTR_MAIN_V		Fld(1, 9, AC_MSKB1) //9
	#define INTR_VBI1		Fld(1, 8, AC_MSKB1) //8
	#define INTR_TRICK_TVD		Fld(1, 6, AC_MSKB0) //6
	#define INTR_SVF_TVD		Fld(1, 4, AC_MSKB0) //4
	#define INTR_TIMERA_TVD		Fld(1, 3, AC_MSKB0) //3
	#define INTR_MV_TVD		Fld(1, 2, AC_MSKB0) //2
	#define INTR_MODE_TVD		Fld(1, 1, AC_MSKB0) //1
	#define INTR_VPRES_TVD		Fld(1, 0, AC_MSKB0) //0

	// Reset Control
#define SYS_00 (IO_VDOIN_BASE+0x400)
 #define SYS_00_0 (IO_VDOIN_BASE+0x400)
 #define SYS_00_1 (IO_VDOIN_BASE+0x401)
 #define SYS_00_2 (IO_VDOIN_BASE+0x402)
 #define SYS_00_3 (IO_VDOIN_BASE+0x403)
    #define DATA_SYNC_AUTO Fld(1, 31, AC_MSKB3) //31
    // #define MISC_CG_DISABLE Fld(1, 30, AC_MSKB3) //30
    #define DATA_SYNC_DUAL Fld(1, 27, AC_MSKB3) //27
    #define SUB_DATA_SYNC_INV Fld(1, 26, AC_MSKB3) //26
    #define DATA_SYNC_HIGH Fld(1, 23, AC_MSKB2) //23
    #define DATA_SYNC_INV Fld(1, 22, AC_MSKB2) //22
    #define PIC_RST Fld(1, 21, AC_MSKB2) //21
    #define MPEG2_RST Fld(1, 20, AC_MSKB2) //20
    #define MPEG1_RST Fld(1, 19, AC_MSKB2) //19
    #define TVEM_RST Fld(1, 18, AC_MSKB2) //18
    #define SP1_RST Fld(1, 17, AC_MSKB2) //17
    #define SP0_RST Fld(1, 16, AC_MSKB2) //16
    #define VBI2_RST Fld(1, 15, AC_MSKB1) //15
    #define SP2_RST Fld(1, 14, AC_MSKB1) //14
    #define PIP_RST Fld(1, 11, AC_MSKB1) //11
    #define MAIN_RST Fld(1, 10, AC_MSKB1) //10
    #define VBI_RST Fld(1, 9, AC_MSKB1) //9
    #define CCH_RST Fld(1, 8, AC_MSKB1) //8
    #define DVI_RST Fld(1, 6, AC_MSKB0) //6
    #define HDMI_RST Fld(1, 5, AC_MSKB0) //5
    #define HDTV_RST Fld(1, 4, AC_MSKB0) //4
    #define TVD3D_RST Fld(1, 2, AC_MSKB0) //2
    #define VSRC_RST Fld(1, 1, AC_MSKB0) //1
    #define FLD_REG_RST Fld(1, 0, AC_MSKB0) //0
  // LPF MAIN control  
 #define SYS_01 (IO_VDOIN_BASE+0x404)
	#define SYS_01_0 (IO_VDOIN_BASE+0x404)
	#define SYS_01_1 (IO_VDOIN_BASE+0x405)
	#define SYS_01_2 (IO_VDOIN_BASE+0x406)
	#define SYS_01_3 (IO_VDOIN_BASE+0x407)
		#define CCH_VSLEN_SEL Fld(1, 31, AC_MSKB3)//31
		#define DVI_VSLEN_SEL Fld(1, 30, AC_MSKB3)//30
		#define LPF_MAIN_COF_Y4 Fld(6, 24, AC_MSKB3)//29:24
		#define CCH_PROG_POL Fld(1, 23, AC_MSKB2)//23////am612
		#define DVI_HDELEN_SEL Fld(1, 22, AC_MSKB2)//22
		#define LPF_MAIN_COF_Y3 Fld(5, 16, AC_MSKB2)//20:16
		#define DVI_DE_VHS_END Fld(1, 15, AC_MSKB1)//15
		#define CCH_VHS_END Fld(1, 14, AC_MSKB1)//14
		#define LPF_MAIN_COF_Y2 Fld(5, 8, AC_MSKB1)//12:8
		#define LPF_MAIN_COF_Y1 Fld(5, 0, AC_MSKB0)//4:0
#define SYS_02 (IO_VDOIN_BASE+0x408)
	#define SYS_02_0 (IO_VDOIN_BASE+0x408)
	#define SYS_02_1 (IO_VDOIN_BASE+0x409)
	#define SYS_02_2 (IO_VDOIN_BASE+0x40A)
	#define SYS_02_3 (IO_VDOIN_BASE+0x40B)
		#define CCH_VCNT_OFFSET_F1 Fld(2, 30, AC_MSKB3)//31:30
		#define CCH_VCNT_OFFSET_F0 Fld(2, 28, AC_MSKB3)//29:28
		//#define SP1_ANAIN Fld(1, 27, AC_MSKB3)//27
		//#define CCH_FIX_VCNT Fld(1, 26, AC_MSKB3)//26
		#define PIP_LPF_C_EN Fld(1, 25, AC_MSKB3)//25
		#define PIP_LPF_Y_EN Fld(1, 24, AC_MSKB3)//24
		//#define LPF_MAIN_C_EN Fld(1, 23, AC_MSKB2)//23
		#define LPF_MAIN_COF_C5 Fld(7, 16, AC_MSKB2)//22:16
		#define LPF_MAIN_COES_C Fld(4, 12, AC_MSKB1)//15:12
		#define LPF_MAIN_COES_Y Fld(4, 8, AC_MSKB1)//11:8
		#define LPF_MAIN_Y_EN Fld(1, 7, AC_MSKB0)//7
		#define LPF_MAIN_COF_Y5 Fld(7, 0, AC_MSKB0)//6:0
	// Interrupt mask
#define SYS_03 (IO_VDOIN_BASE+0x40c)
 #define SYS_03_0 (IO_VDOIN_BASE+0x40c)
 #define SYS_03_1 (IO_VDOIN_BASE+0x40d)
 #define SYS_03_2 (IO_VDOIN_BASE+0x40e)
 #define SYS_03_3 (IO_VDOIN_BASE+0x40f)
		/* using  same filed define  as INT_COLLECT */
#define SYS_04 (IO_VDOIN_BASE+0x410)
 #define SYS_04_0 (IO_VDOIN_BASE+0x410)
 #define SYS_04_1 (IO_VDOIN_BASE+0x411)
 #define SYS_04_2 (IO_VDOIN_BASE+0x412)
 #define SYS_04_3 (IO_VDOIN_BASE+0x413)
    #define C_RAND_BIT_SEL Fld(8, 24, AC_FULLB3) //31:24
    #define TIMERA_TVD3D Fld(8, 16, AC_FULLB2) //23:16
    #define CRC_SRC_SEL Fld(1, 15, AC_MSKB1) //15
    #define WATER_YEN Fld(1, 14, AC_MSKB1) //14
    #define WATER_CBEN Fld(1, 13, AC_MSKB1) //13
    #define WATER_CREN Fld(1, 12, AC_MSKB1) //12
    #define DBG_MODE_EXT Fld(4, 8, AC_MSKB1) //11:8
    #define DBG_MODE Fld(8, 0, AC_FULLB0) //7:0
#define SYS_05 (IO_VDOIN_BASE+0x414)
 #define SYS_05_0 (IO_VDOIN_BASE+0x414)
 #define SYS_05_1 (IO_VDOIN_BASE+0x415)
 #define SYS_05_2 (IO_VDOIN_BASE+0x416)
 #define SYS_05_3 (IO_VDOIN_BASE+0x417)
    #define WATER_STEP Fld(8, 24, AC_FULLB3) //31:24
    #define WATER_BIT_NO Fld(2, 22, AC_MSKB2) //23:22
    #define SLOPE_PIP_EN Fld(1, 21, AC_MSKB2) //21
    #define MISC_CG_DISABLE Fld(1, 20, AC_MSKB2) //20
    #define FIRST_LINE_V_POL Fld(1, 19, AC_MSKB2) //19
    #define FIRST_LINE_H_POL Fld(1, 18, AC_MSKB2) //18
    #define FIRST_LINE_PIP_EN Fld(1, 17, AC_MSKB2) //17
    #define FIRST_LINE_MAIN_EN Fld(1, 16, AC_MSKB2) //16
    #define RETIME_SEL Fld(1, 15, AC_MSKB1) //15
    #define RETIME_AUTO Fld(1, 14, AC_MSKB1) //14
    #define RAND_CREN Fld(1, 13, AC_MSKB1) //13
    #define RAND_CBEN Fld(1, 12, AC_MSKB1) //12
    #define RAND_YEN Fld(1, 11, AC_MSKB1) //11
    #define RAND_EN Fld(1, 10, AC_MSKB1) //10
    #define RAND_CH_SEL Fld(1, 9, AC_MSKB1) //9
    #define DUMP_ADY Fld(1, 8, AC_MSKB1) //8
    #define FV_OUT_ALIGN Fld(1, 5, AC_MSKB0) //5
    #define TEST_IDX Fld(1, 4, AC_MSKB0) //4
    #define TEST_VGA_ADC Fld(2, 2, AC_MSKB0) //3:2
    #define TEST_ADC_SEL Fld(1, 1, AC_MSKB0) //1
    #define TVD3D_ADC_SEL Fld(1, 0, AC_MSKB0) //0

#define LPF_MAIN_00 (IO_VDOIN_BASE+0x420)
	#define LPF_MAIN_00_0 (IO_VDOIN_BASE+0x420)
	#define LPF_MAIN_00_1 (IO_VDOIN_BASE+0x421)
	#define LPF_MAIN_00_2 (IO_VDOIN_BASE+0x422)
	#define LPF_MAIN_00_3 (IO_VDOIN_BASE+0x423)
		#define DVI_VHS_END_MODE Fld(1, 31, AC_MSKB3)//31
		#define CCH_VHS_END_MODE Fld(1, 30, AC_MSKB3)//30
		#define LPF_MAIN_COF_C4 Fld(6, 24, AC_MSKB3)//29:24
		#define DVI_EN Fld(1, 23, AC_MSKB2)//23
		#define LPF_MAIN_COF_C3 Fld(5, 16, AC_MSKB2)//20:16
//		#define SP1_ANAIN Fld(1, 14, AC_MSKB1) //14
		#define CCH_FIX_VCNT Fld(1, 13, AC_MSKB1) //13
		#define LPF_MAIN_COF_C2 Fld(5, 8, AC_MSKB1)//12:8
//		#define LPF_MAIN_C_EN Fld(1, 5, AC_MSKB0) //5
		#define LPF_MAIN_COF_C1 Fld(5, 0, AC_MSKB0)//4:0

//#if CONFIG_MT8223        
#define PQCRC_00 (IO_VDOIN_BASE+0x7b0)
  #define PQCRC_00_0 (IO_VDOIN_BASE+0x7b0)
  #define PQCRC_00_1 (IO_VDOIN_BASE+0x7b1)
  #define PQCRC_00_2 (IO_VDOIN_BASE+0x7b2)
  #define PQCRC_00_3 (IO_VDOIN_BASE+0x7b3)
    #define PQCRC_TOP_EN Fld(1, 31, AC_MSKB3) //31
    #define PQCRC_BOT_EN Fld(1, 30, AC_MSKB3) //30
    #define PQCRC_Y_MSK Fld(10, 20, AC_MSKW32) //29:20
    #define PQCRC_C_MSK Fld(10, 10, AC_MSKW21) //19:10
    #define PQCRC_V_MSK Fld(10, 0, AC_MSKW10) //9:0
#define PQCRC_01 (IO_VDOIN_BASE+0x7b4)
  #define PQCRC_01_0 (IO_VDOIN_BASE+0x7b4)
  #define PQCRC_01_1 (IO_VDOIN_BASE+0x7b5)
  #define PQCRC_01_2 (IO_VDOIN_BASE+0x7b6)
  #define PQCRC_01_3 (IO_VDOIN_BASE+0x7b7)
    #define PQCRC_EVEN_LINE_EN Fld(1, 31, AC_MSKB3) //31
    #define PQCRC_ODD_LINE_EN Fld(1, 30, AC_MSKB3) //30
    #define PQCRC_CLR_RDY Fld(1, 29, AC_MSKB3) //29
    #define ATD_VMASK_END_I2C Fld(10, 12, AC_MSKW21) //21:12
    #define ATD_VMASK_MODE_I2C Fld(2, 10, AC_MSKB1) //11:10
    #define ATD_VMASK_START_I2C Fld(10, 0, AC_MSKW10) //9:0
//#endif

#define INTR_SW_TEST_MSK	Fld2Msk32(INTR_SW_TEST)
#define INTR_FIFO_TDC_MSK	Fld2Msk32(INTR_FIFO_TDC)
#define INTR_HDMI_SCDT_MSK	Fld2Msk32(INTR_HDMI_SCDT)
#define INTR_HDMI_MUTE_MSK	Fld2Msk32(INTR_HDMI_MUTE)
#define INTR_HDMI_MSK		Fld2Msk32(INTR_HDMI)
#define INTR_DVI_DE_ACT_MSK	Fld2Msk32(INTR_DVI_DE_ACT)
#define INTR_DVI_DE_MODE_MSK	Fld2Msk32(INTR_DVI_DE_MODE)
#define INTR_DVI_DE_STABLE_MSK	Fld2Msk32(INTR_DVI_DE_STABLE)
#define INTR_STA_CHG_TVD_MSK	Fld2Msk32(INTR_STA_CHG_TVD)
#define INTR_SCPOS_MSK		Fld2Msk32(INTR_SCPOS)
#define INTR_SP1_V_MSK		Fld2Msk32(INTR_SP1_V)
#define INTR_VBI2_MSK		Fld2Msk32(INTR_VBI2)
#define INTR_SP1_MODE_MSK	Fld2Msk32(INTR_SP1_MODE)
#define INTR_DDS_LOCK_MSK	Fld2Msk32(INTR_DDS_LOCK)
#define INTR_SP0_V_MSK		Fld2Msk32(INTR_SP0_V)
#define INTR_SP0_MODE_MSK	Fld2Msk32(INTR_SP0_MODE)
#define INTR_PIP_V_MSK		Fld2Msk32(INTR_PIP_V)
#define INTR_MAIN_V_MSK	Fld2Msk32(INTR_MAIN_V)
#define INTR_VBI1_MSK		Fld2Msk32(INTR_VBI1)
#define INTR_TRICK_TVD_MSK	Fld2Msk32(INTR_TRICK_TVD)
#define INTR_SVF_MSK		Fld2Msk32(INTR_SVF_TVD)
#define INTR_TIMERA_TVD_MSK	Fld2Msk32(INTR_TIMERA_TVD)
#define INTR_MV_TVD_MSK	Fld2Msk32(INTR_MV_TVD)
#define INTR_MODE_TVD_MSK	Fld2Msk32(INTR_MODE_TVD)
#define INTR_VPRES_TVD_MSK	Fld2Msk32(INTR_VPRES_TVD)

	/* reset control , sync invert*/
#define vTvd3dResetOn()		vRegWriteFldAlign(SYS_00,1,TVD3D_RST)
#define vTvd3dResetOff()		vRegWriteFldAlign(SYS_00,0,TVD3D_RST)

#define vHdtvResetOn()		vRegWriteFldAlign(SYS_00,1,HDTV_RST)
#define vHdtvResetOff()		vRegWriteFldAlign(SYS_00,0,HDTV_RST)

#define vDviResetOn()		vRegWriteFldAlign(SYS_00,1,DVI_RST)
#define vDviResetOff()		vRegWriteFldAlign(SYS_00,0,DVI_RST)

#define vMainSrcResetOn()	vRegWriteFldAlign(SYS_00,1,MAIN_RST)
#define vMainSrcResetOff()	vRegWriteFldAlign(SYS_00,0,MAIN_RST)

#define vMainFIFOResetOn()	vRegWriteFldAlign(SYS_00,1,DATA_SYNC_HIGH)
#define vMainFIFOResetOff()	vRegWriteFldAlign(SYS_00,0,DATA_SYNC_HIGH)

#define vPipSrcResetOn()	vRegWriteFldAlign(SYS_00,1,PIP_RST)
#define vPipSrcResetOff()	vRegWriteFldAlign(SYS_00,0,PIP_RST)

#define fgIsVdoIntTvd3dVdoSw() 	fgIsVdoinInt(INTR_VPRES_TVD_MSK)
#define fgIsVdoIntTvd3dModeSw() fgIsVdoinInt(INTR_MODE_TVD_MSK)
#define fgIsVdoIntTvd3dMvSw() 	fgIsVdoinInt(INTR_MV_TVD_MSK)
#define fgIsVdoIntTvd3dTrick() 	fgIsVdoinInt(INTR_TRICK_TVD_MSK)
#define fgIsVdoIntCC1() 	fgIsVdoinInt(INTR_VBI1_MSK)
#define fgIsVdoIntMainDet() 	fgIsVdoinInt(INTR_MAIN_V_MSK)
#define fgIsVdoIntPipDet() 	fgIsVdoinInt(INTR_PIP_V_MSK)
#define fgIsVdoIntSp0MdChg()	fgIsVdoinInt(INTR_SP0_MODE_MSK)
#define fgIsVdoIntSp0Vsyncout() fgIsVdoinInt(INTR_SP0_V_MSK)
#define fgIsVdoIntSp1MdChg()	fgIsVdoinInt(INTR_SP1_MODE_MSK)
#define fgIsVdoIntSp1Vsyncout() fgIsVdoinInt(INTR_SP1_V_MSK)
#define fgIsVdoIntCC2() 	fgIsVdoinInt(INTR_VBI2_MSK)
#define fgIsVdoIntMDSts() 	fgIsVdoinInt(INTR_STA_CHG_TVD_MSK)
#define fgIsVdoIntScpos() 	fgIsVdoinInt(INTR_SCPOS_MSK)



#define INTR_SW_TEST_MSK	Fld2Msk32(INTR_SW_TEST)
#define INTR_FIFO_TDC_MSK	Fld2Msk32(INTR_FIFO_TDC)
#define INTR_HDMI_SCDT_MSK	Fld2Msk32(INTR_HDMI_SCDT)
#define INTR_HDMI_MUTE_MSK	Fld2Msk32(INTR_HDMI_MUTE)
#define INTR_HDMI_MSK		Fld2Msk32(INTR_HDMI)
#define INTR_DVI_DE_ACT_MSK	Fld2Msk32(INTR_DVI_DE_ACT)
#define INTR_DVI_DE_MODE_MSK	Fld2Msk32(INTR_DVI_DE_MODE)
#define INTR_DVI_DE_STABLE_MSK	Fld2Msk32(INTR_DVI_DE_STABLE)
#define INTR_STA_CHG_TVD_MSK	Fld2Msk32(INTR_STA_CHG_TVD)
#define INTR_SCPOS_MSK		Fld2Msk32(INTR_SCPOS)
#define INTR_SP1_V_MSK		Fld2Msk32(INTR_SP1_V)
#define INTR_VBI2_MSK		Fld2Msk32(INTR_VBI2)
#define INTR_SP1_MODE_MSK	Fld2Msk32(INTR_SP1_MODE)
#define INTR_DDS_LOCK_MSK	Fld2Msk32(INTR_DDS_LOCK)
#define INTR_SP0_V_MSK		Fld2Msk32(INTR_SP0_V)
#define INTR_SP0_MODE_MSK	Fld2Msk32(INTR_SP0_MODE)
#define INTR_PIP_V_MSK		Fld2Msk32(INTR_PIP_V)
#define INTR_MAIN_V_MSK	Fld2Msk32(INTR_MAIN_V)
#define INTR_VBI1_MSK		Fld2Msk32(INTR_VBI1)
#define INTR_TRICK_TVD_MSK	Fld2Msk32(INTR_TRICK_TVD)
#define INTR_SVF_MSK		Fld2Msk32(INTR_SVF_TVD)
#define INTR_TIMERA_TVD_MSK	Fld2Msk32(INTR_TIMERA_TVD)
#define INTR_MV_TVD_MSK	Fld2Msk32(INTR_MV_TVD)
#define INTR_MODE_TVD_MSK	Fld2Msk32(INTR_MODE_TVD)
#define INTR_VPRES_TVD_MSK	Fld2Msk32(INTR_VPRES_TVD)

// VDOIN Irq On/Off, for 82xx Compatible
#define	MSK_TVD3D_VDO_SW	INTR_VPRES_TVD_MSK
#define	MSK_TVD3D_MODE_SW	INTR_MODE_TVD_MSK
#define	MSK_TVD3D_MV_SW	INTR_MV_TVD_MSK
#define	MSK_TVD3D_TIMER	INTR_TIMERA_TVD_MSK
#define	MSK_TVD3D_TRICK	INTR_TRICK_TVD_MSK
#define	MSK_CC1		INTR_VBI1_MSK
#define	MSK_MAIN_DET		INTR_MAIN_V_MSK
#define	MSK_PIP_DET		INTR_PIP_V_MSK
#define	MSK_SP0_MD_CHG	INTR_SP0_MODE_MSK
#define	MSK_SP0_VSYNCOUT	INTR_SP0_V_MSK
#define	MSK_DDS_LOCK		INTR_DDS_LOCK_MSK
#define	MSK_SP1_MD_CHG	INTR_SP1_MODE_MSK
#define	MSK_CC2		INTR_VBI2_MSK
#define	MSK_SP1_VSYNCOUT	INTR_SP1_V_MSK
#define	MSK_SCPOS		INTR_SCPOS_MSK
#define	MSK_TVD3D_MD_STS	INTR_STA_CHG_TVD_MSK
#define	MSK_IRQ_TEST		INTR_SW_TEST_MSK


#define	vVDOINIrqOn(arg)	vRegWrite4BMsk(SYS_03, 0, (arg))
#define	vVDOINIrqOff(arg)	vRegWrite4BMsk(SYS_03,(arg),(arg))

#if 0
#define OMUX_00 (IO_VDOIN_BASE+0x7fc)
 #define OMUX_00_0 (IO_VDOIN_BASE+0x7fc)
 #define OMUX_00_1 (IO_VDOIN_BASE+0x7fd)
 #define OMUX_00_2 (IO_VDOIN_BASE+0x7fe)
 #define OMUX_00_3 (IO_VDOIN_BASE+0x7ff)
    #define OMUX_MAIN_O_SEL Fld(1, 31, AC_MSKB3) //31
    #define OMUX_PIP_O_SEL Fld(1, 30, AC_MSKB3) //30
    #define VDOIN_MPCLK_SEL Fld(2, 28, AC_MSKB3) //29:28
    #define OMUX_TVEM_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define OMUX_PIP_RAMP_EN Fld(4, 20, AC_MSKB2) //23:20
    #define OMUX_PIP_RAMP_MSB Fld(1, 19, AC_MSKB2) //19
    #define OMUX_PIP_RAMP_FREQ Fld(3, 16, AC_MSKB2) //18:16
    #define OMUX_MAIN_RAMP_EN Fld(4, 12, AC_MSKB1) //15:12
    #define OMUX_MAIN_RAMP_MSB Fld(1, 11, AC_MSKB1) //11
    #define OMUX_MAIN_RAMP_FREQ Fld(3, 8, AC_MSKB1) //10:8
    #define OMUX_MAIN_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define OMUX_PIP_SEL Fld(4, 0, AC_MSKB0) //3:0
#else
#include "hw_dvi.h"
#endif

	typedef enum {
		MPCLK_162MHZ = 0x00,
		MPCLK_180MHZ = 0x01,
		MPCLK_PIPCLK = 0x02,
		MPCLK_MAINCLK = 0x03
	} MPCLK_e;
	
	#if 0
	#define SET_VDOIN_MPCLK(x) vRegWriteFldAlign(OMUX_00, (x), VDOIN_MPCLK_SEL)
	#endif
#endif //_HW_VDOIN_

