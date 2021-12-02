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
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef X_HAL_IO_H
#define X_HAL_IO_H

#include "x_typedef.h"

//===========================================================================

// Type of IO register, for recording the address of an IO register
typedef volatile UINT32             HAL_IO_REGISTER;

// Macros of register read
#define HAL_READ8(_reg_)            (*((volatile UINT8*)(_reg_)))
#define HAL_READ16(_reg_)           (*((volatile UINT16*)(_reg_)))
#define HAL_READ32(_reg_)           (*((volatile UINT32*)(_reg_)))

// Macros of register write
#define HAL_WRITE8(_reg_, _val_)    (*((volatile UINT8*)(_reg_)) = (_val_))
#define HAL_WRITE16(_reg_, _val_)   (*((volatile UINT16*)(_reg_)) = (_val_))
#define HAL_WRITE32(_reg_, _val_)   (*((volatile UINT32*)(_reg_)) = (_val_))

// Macros for read/write access
#define HAL_REG8(_reg_)         HAL_READ8((_reg_))
#define HAL_REG16(_reg_)        HAL_READ16((_reg_))
#define HAL_REG32(_reg_)        HAL_READ32((_reg_))

//===========================================================================

// Bit field definitions
#define BIT0                        0x00000001
#define BIT1                        0x00000002
#define BIT2                        0x00000004
#define BIT3                        0x00000008
#define BIT4                        0x00000010
#define BIT5                        0x00000020
#define BIT6                        0x00000040
#define BIT7                        0x00000080
#define BIT8                        0x00000100
#define BIT9                        0x00000200
#define BIT10                       0x00000400
#define BIT11                       0x00000800
#define BIT12                       0x00001000
#define BIT13                       0x00002000
#define BIT14                       0x00004000
#define BIT15                       0x00008000
#define BIT16                       0x00010000
#define BIT17                       0x00020000
#define BIT18                       0x00040000
#define BIT19                       0x00080000
#define BIT20                       0x00100000
#define BIT21                       0x00200000
#define BIT22                       0x00400000
#define BIT23                       0x00800000
#define BIT24                       0x01000000
#define BIT25                       0x02000000
#define BIT26                       0x04000000
#define BIT27                       0x08000000
#define BIT28                       0x10000000
#define BIT29                       0x20000000
#define BIT30                       0x40000000
#define BIT31                       0x80000000


// field access macro-----------------------------------------------------------

/* field macros */
#define Fld(wid,shft,ac)    (((UINT32)wid<<16)|(shft<<8)|ac)
#define Fld_wid(fld)    (UINT8)((fld)>>16)
#define Fld_shft(fld)   (UINT8)((fld)>>8)
#define Fld_ac(fld)     (UINT8)(fld)

/* access method*/
#define AC_FULLB0       1
#define AC_FULLB1       2
#define AC_FULLB2       3
#define AC_FULLB3       4
#define AC_FULLW10      5
#define AC_FULLW21      6
#define AC_FULLW32      7
#define AC_FULLDW       8
#define AC_MSKB0        11
#define AC_MSKB1        12
#define AC_MSKB2        13
#define AC_MSKB3        14
#define AC_MSKW10       15
#define AC_MSKW21       16
#define AC_MSKW32       17
#define AC_MSKDW        18

/* Reg32 to Reg8*/
#define REGB0(reg)  (reg)
#define REGB1(reg)  ((reg)+1)
#define REGB2(reg)  ((reg)+2)
#define REGB3(reg)  ((reg)+3)
/* Reg32 to Reg16*/
#define REGW0(reg)  (reg)
#define REGW1(reg)  ((reg)+1)
#define REGW2(reg)  ((reg)+2)

/* --------FLD help macros, mask32 to mask8,mask16,maskalign ----------*/
/* mask32 -> mask8 */
#define MSKB0(msk)  (UINT8)(msk)
#define MSKB1(msk)  (UINT8)((msk)>>8)
#define MSKB2(msk)  (UINT8)((msk)>>16)
#define MSKB3(msk)  (UINT8)((msk)>>24)
/* mask32 -> mask16 */
#define MSKW0(msk)  (UINT16)(msk)
#define MSKW1(msk)  (UINT16)((msk)>>8)
#define MSKW2(msk)  (UINT16)((msk)>>16)
                        /* mask32 -> maskalign */
#define MSKAlignB(msk)  (((msk)&0xff)?(msk):(\
            ((msk)&0xff00)?((msk)>>8):(\
            ((msk)&0xff0000)?((msk)>>16):((msk)>>24)\
        )\
    ))

/* --------FLD help macros, mask32 to mask8,mask16,maskalign ----------*/
#define Fld2Msk32(fld)  /*lint -save -e504 */ (((UINT32)0xffffffff>>(32-Fld_wid(fld)))<<Fld_shft(fld)) /*lint -restore */
#define Fld2MskB0(fld)  MSKB0(Fld2Msk32(fld))
#define Fld2MskB1(fld)  MSKB1(Fld2Msk32(fld))
#define Fld2MskB2(fld)  MSKB2(Fld2Msk32(fld))
#define Fld2MskB3(fld)  MSKB3(Fld2Msk32(fld))
#define Fld2MskBX(fld,byte) ((UINT8)(Fld2Msk32(fld)>>((byte&3)*8)))

#define Fld2MskW0(fld)  MSKW0(Fld2Msk32(fld))
#define Fld2MskW1(fld)  MSKW1(Fld2Msk32(fld))
#define Fld2MskW2(fld)  MSKW2(Fld2Msk32(fld))
#define Fld2MskWX(fld,byte) ((UINT16)(Fld2Msk32(fld)>>((byte&3)*8)))

#define Fld2MskAlignB(fld)  MSKAlignB(Fld2Msk32(fld))
#define FldshftAlign(fld)   ((Fld_shft(fld)<8)?Fld_shft(fld):(\
            (Fld_shft(fld)<16)?(Fld_shft(fld)-8):(\
            (Fld_shft(fld)<24)?(Fld_shft(fld)-16):(Fld_shft(fld)-24)\
        )\
    ))
#define ValAlign2Fld(val,fld)   ((val)<<FldshftAlign(fld))

#define u1IO32Read1B(reg32) (*(volatile UINT8 *)(reg32))
extern UINT16 u2IO32Read2B(UINT32 reg32);
#define u4IO32Read4B(reg32) (*(volatile UINT32 *)(reg32))

#define vIO32Write1B(reg32, val8) vIO32Write1BMsk(reg32,val8,0xff)
#define vIO32Write2B(reg32, val16) vIO32Write2BMsk(reg32,val16,0xffff)
#define vIO32Write4B(reg32,val32) (*(volatile UINT32 *)(reg32)=(val32))

extern void vIO32Write1BMsk(UINT32 reg32, UINT32 val8,UINT8 msk8);
extern void vIO32Write2BMsk(UINT32 reg32, UINT32 val16, UINT16 msk16);
extern void vIO32Write4BMsk(UINT32 reg32, UINT32 val32, UINT32 msk32);


#define IO32ReadFld(reg32,fld)  /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?u1IO32Read1B((reg32)+(Fld_ac(fld)-AC_FULLB0)):( \
    ((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?u2IO32Read2B((reg32)+(Fld_ac(fld)-AC_FULLW10)):( \
    (Fld_ac(fld)==AC_FULLDW)? u4IO32Read4B(reg32):( \
    ((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?(u1IO32Read1B((reg32)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):( \
    ((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?(u2IO32Read2B((reg32)+(Fld_ac(fld)-AC_MSKW10))&Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))):( \
    (Fld_ac(fld)==AC_MSKDW)?(u4IO32Read4B(reg32)&Fld2Msk32(fld)):0 \
  ))))))  /*lint -restore */


#define IO32ReadFldAlign(reg32,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?u1IO32Read1B((reg32)+(Fld_ac(fld)-AC_FULLB0)):( \
    ((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?u2IO32Read2B((reg32)+(Fld_ac(fld)-AC_FULLW10)):( \
    (Fld_ac(fld)==AC_FULLDW)? u4IO32Read4B(reg32):( \
    ((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?((u1IO32Read1B((reg32)+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0)))>>((Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKB0))&7)):( \
    ((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?((u2IO32Read2B((reg32)+(Fld_ac(fld)-AC_MSKW10))&Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10)))>>((Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKW10))&15)):( \
    (Fld_ac(fld)==AC_MSKDW)?((u4IO32Read4B(reg32)&Fld2Msk32(fld))>>Fld_shft(fld)):0 \
  ))))))  /*lint -restore */

#define vIO32WriteFld(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))? vIO32Write1B((reg32)+(Fld_ac(fld)-AC_FULLB0),(val)),0:( \
    ((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?vIO32Write2B((reg32)+(Fld_ac(fld)-AC_FULLW10),(val)),0:( \
    (Fld_ac(fld)==AC_FULLDW)?vIO32Write4B((reg32),(val)),0:( \
    ((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?vIO32Write1BMsk((reg32)+(Fld_ac(fld)-AC_MSKB0),(val),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:( \
    ((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?vIO32Write2BMsk((reg32)+(Fld_ac(fld)-AC_MSKW10),(val),Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))),0:( \
    (Fld_ac(fld)==AC_MSKDW)?vIO32Write4BMsk((reg32),(val),Fld2Msk32(fld)),0:0\
    ))))))  /*lint -restore */

#define vIO32WriteFldAlign(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?vIO32Write1B((reg32)+(Fld_ac(fld)-AC_FULLB0),(val)),0:( \
    ((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?vIO32Write2B((reg32)+(Fld_ac(fld)-AC_FULLW10),(val)),0:( \
    (Fld_ac(fld)==AC_FULLDW)?vIO32Write4B((reg32),(val)),0:( \
    ((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?vIO32Write1BMsk((reg32)+(Fld_ac(fld)-AC_MSKB0),ValAlign2Fld((val),fld),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))),0:( \
    ((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?vIO32Write2BMsk((reg32)+(Fld_ac(fld)-AC_MSKW10),ValAlign2Fld((val),fld),Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))),0:( \
    (Fld_ac(fld)==AC_MSKDW)?vIO32Write4BMsk((reg32),((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld)),0:0\
    )))))) /*lint -restore */

#define vIO32WriteFldMulti(reg32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(INT32)(list)}; \
	{UINT8 upk;\
	((UINT32)msk==0xff)?vIO32Write1B(reg32,(list)),0:(\
	((UINT32)msk==0xff00)?vIO32Write1B(reg32+1,(list)>>8),0:(\
	((UINT32)msk==0xff0000)?vIO32Write1B(reg32+2,(list)>>16),0:(\
	((UINT32)msk==0xff000000)?vIO32Write1B(reg32+3,(list)>>24),0:(\
	((UINT32)msk==0xffff)?vIO32Write2B(reg32,(list)),0:(\
	((UINT32)msk==0xffff00)?vIO32Write2B(reg32+1,(list)>>8),0:(\
	((UINT32)msk==0xffff0000)?vIO32Write2B(reg32+2,(list)>>16),0:(\
	((UINT32)msk==0xffffffff)?vIO32Write4B(reg32,(list)),0:(\
	(((UINT32)msk&0xff)&&(!((UINT32)msk&0xffffff00)))?vIO32Write1BMsk(reg32,(list),(UINT8)(UINT32)msk),0:(\
	(((UINT32)msk&0xff00)&&(!((UINT32)msk&0xffff00ff)))?vIO32Write1BMsk(reg32+1,(list)>>8,(UINT8)((UINT32)msk>>8)),0:(\
	(((UINT32)msk&0xff0000)&&(!((UINT32)msk&0xff00ffff)))?vIO32Write1BMsk(reg32+2,(list)>>16,(UINT8)((UINT32)msk>>16)),0:(\
	(((UINT32)msk&0xff000000)&&(!((UINT32)msk&0x00ffffff)))?vIO32Write1BMsk(reg32+3,(list)>>24,(UINT8)((UINT32)msk>>24)),0:(\
	(((UINT32)msk&0xffff)&&(!((UINT32)msk&0xffff0000)))?vIO32Write2BMsk(reg32,(list),(UINT16)(UINT32)msk),0:(\
	(((UINT32)msk&0xffff00)&&(!((UINT32)msk&0xff0000ff)))?vIO32Write2BMsk(reg32+1,(list)>>8,(UINT16)((UINT32)msk>>8)),0:(\
	(((UINT32)msk&0xffff0000)&&(!((UINT32)msk&0x0000ffff)))?vIO32Write2BMsk(reg32+2,(list)>>16,(UINT16)((UINT32)msk>>16)),0:(\
	((UINT32)msk)?vIO32Write4BMsk(reg32,(list),((UINT32)msk)),0:0\
	)))))))))))))));\
	}\
}/*lint -restore */

#define vIO32WriteFldD4val(reg32,dval32,list) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
{ \
	UINT16 upk;\
	enum {msk=(INT32)(list)};\
	{UINT8 upk;\
	vIO32Write4B(reg32,((dval32)&~(UINT32)msk)|(list));\
	}\
}/*lint -restore */

#endif  // X_HAL_IO_H
