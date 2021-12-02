/*----------------------------------------------------------------------------
 * No Warranty :  Except  as  may  be  otherwise  agreed  to  in  writing, no *
 * warranties  of  any  kind,  whether  express or  implied, are given by MTK *
 * with  respect  to  any  Confidential  Information  or any use thereof, and *
 * the  Confidential  Information  is  provided  on  an  "AS IS"  basis.  MTK *
 * hereby  expressly  disclaims  all  such  warranties, including any implied *
 * warranties   of  merchantability ,  non-infringement  and  fitness  for  a *
 * particular purpose and any warranties arising out of course of performance *
 * course  of dealing or usage of trade.  Parties further acknowledge that in *
 * connection  with  the Purpose, Company may, either presently and/or in the *
 * future,   instruct   MTK   to   assist  it  in  the  development  and  the *
 * implementation,  in accordance with Company's designs, of certain software *
 * relating  to  Company's  product(s)  (the  "Services").   Except as may be *
 * otherwise agreed to in writing, no warranties of any kind, whether express *
 * or  implied,  are  given by MTK with respect to the Services provided, and *
 * the  Services  are  provided  on  an  "AS  IS"  basis.   Company   further *
 * acknowledges  that  the  Services  may  contain  errors,  which testing is *
 * important  and  it  is  solely  responsible for fully testing the Services *
 * and/or   derivatives   thereof   before  they  are  used,  sublicensed  or *
 * distributed.   Should  there  be any third party action brought again MTK, *
 * arising  out  of  or  relating  to  the  Services,  Company agree to fully *
 * indemnify and hold MTK harmless.                                           *
 *---------------------------------------------------------------------------*/
/*************************************************************************
 *  Copyright Statement:  
 *  -------------------- 
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the 
 *  information contained herein may not be used or disclosed except with 
 *  the written permission of MediaTek Inc. (C) 2003
 *
 *************************************************************************/
/**
 *   @file hw_reg.h
 
 *   Register Access macro & Field Access Macro expansion
 *
 *   @author Darren Tsao
 */
#ifndef _HW_REG_H_
#define _HW_REG_H_

// *********************************************************************
// HW register access macro
// *********************************************************************
#define PSEUDO_REG_ACCESS 0

/* field macros */
#define Fld(wid,shft,ac)	(((UINT32)wid<<16)|(shft<<8)|ac)
#define Fld_wid(fld)	(UINT8)((fld)>>16)
#define Fld_shft(fld)	(UINT8)((fld)>>8)
#define Fld_ac(fld)		(UINT8)(fld)

/* access method*/
#define	AC_FULLB0		1
#define	AC_FULLB1		2
#define	AC_FULLB2		3
#define	AC_FULLB3		4
#define	AC_FULLW10	5
#define	AC_FULLW21	6
#define	AC_FULLW32	7
#define	AC_FULLDW		8
#define	AC_MSKB0		11
#define	AC_MSKB1		12
#define	AC_MSKB2		13
#define	AC_MSKB3		14
#define	AC_MSKW10		15
#define	AC_MSKW21		16
#define	AC_MSKW32		17
#define	AC_MSKDW		18

/* --------FLD help macros, mask32 to mask8,mask16,maskalign ----------*/
/* mask32 -> mask8 */
	#define MSKB0(msk)	(UINT8)(msk)
	#define MSKB1(msk)	(UINT8)((msk)>>8)
	#define MSKB2(msk)	(UINT8)((msk)>>16)
	#define MSKB3(msk)	(UINT8)((msk)>>24)
/* mask32 -> mask16 */
	#define MSKW0(msk)	(UINT16)(msk)
	#define MSKW1(msk)	(UINT16)((msk)>>8)
	#define MSKW2(msk)	(UINT16)((msk)>>16)
/* mask32 -> maskalign */
#define MSKAlignB(msk)	(((msk)&0xff)?(msk):(\
			((msk)&0xff00)?((msk)>>8):(\
			((msk)&0xff0000)?((msk)>>16):((msk)>>24)\
		)\
	))
/* --------FLD help macros, mask32 to mask8,mask16,maskalign ----------*/
#define Fld2Msk32(fld)	((((UINT32)1<<Fld_wid(fld))-1)<<Fld_shft(fld))
#define Fld2MskB0(fld)	MSKB0(Fld2Msk32(fld))
#define Fld2MskB1(fld)	MSKB1(Fld2Msk32(fld))
#define Fld2MskB2(fld)	MSKB2(Fld2Msk32(fld))
#define Fld2MskB3(fld)	MSKB3(Fld2Msk32(fld))
#define Fld2MskBX(fld,byte)	((UINT8)(Fld2Msk32(fld)>>((byte&3)*8)))

#define Fld2MskW0(fld)	MSKW0(Fld2Msk32(fld))
#define Fld2MskW1(fld)	MSKW1(Fld2Msk32(fld))
#define Fld2MskW2(fld)	MSKW2(Fld2Msk32(fld))
#define Fld2MskWX(fld,byte)	((UINT16)(Fld2Msk32(fld)>>((byte&3)*8)))


#define Fld2MskAlignB(fld)  MSKAlignB(Fld2Msk32(fld))
#define FldshftAlign(fld)	((Fld_shft(fld)<8)?Fld_shft(fld):(\
			(Fld_shft(fld)<16)?(Fld_shft(fld)-8):(\
			(Fld_shft(fld)<24)?(Fld_shft(fld)-16):(Fld_shft(fld)-24)\
		)\
	))
#define ValAlign2Fld(val,fld)	((val)<<FldshftAlign(fld))


// Normal Access Macro
#if PSEUDO_REG_ACCESS
#define u1RegRead1B(reg16) (0)

#define u2RegRead2B(reg16) (0)
#define u4RegRead4B(reg16) (0)

#define vRegWrite1B(reg16,val8) 
#define vRegWrite1BMsk(reg16, val8, msk8)
#define vRegWrite1BOr(reg16, bOrVal) 
#define vRegWrite1BAnd(reg16, bAndVal)
#define vRegSetBit(reg16, bBit) 
#define vRegClrBit(reg16, bBit)

//#define vRegWrite2B(reg16, val16)
#define vRegWrite2BMsk(reg16,val16,msk16)
#define vRegWrite4B(reg16,val32)
#define vRegWrite4BMsk(reg16,val32,msk32)

// Field Access Macro
#define RegReadFld(reg16,fld) (0)
#define RegReadFldAlign(reg16,fld) (0)
#define vRegWriteFld(reg16,val,fld)
#define vRegWriteFldAlign(reg16,val,fld)
#define vRegWriteFldMulti(reg16,list)
#define vRegWriteFldD4val(reg16,dval32,list)

#else
#ifdef BRIDGE_RR_WA
UINT8 u1RegRead1B(UINT16 reg16) reentrant;
#else
#define u1RegRead1B(reg16) (XBYTE[(reg16)])
#endif

UINT16 u2RegRead2B(UINT16 reg16) reentrant;
UINT32 u4RegRead4B(UINT16 reg16) reentrant;

#define vRegWrite1B(reg16,val8) (XBYTE[(reg16)] = (val8))
#ifdef BRIDGE_RR_WA
void vRegWrite1BMsk(UINT16 reg16, UINT8 val8, UINT8 msk8) reentrant;
#else
#define vRegWrite1BMsk(reg16, val8, msk8) (XBYTE[(reg16)] = (XBYTE[(reg16)]  & (~(msk8))) | ((val8) & (msk8)))
#endif

#ifdef BRIDGE_RR_WA
void vRegWrite1BOr(UINT16 reg16, UINT8 bOrVal) reentrant;
void vRegWrite1BAnd(UINT16 reg16, UINT8 bAndVal) reentrant;
void vRegSetBit(UINT16 reg16, UINT8 bBit) reentrant;
void vRegClrBit(UINT16 reg16, UINT8 bBit) reentrant;
#else
#define vRegWrite1BOr(reg16, bOrVal) (XBYTE[(reg16)] |= (bOrVal))
#define vRegWrite1BAnd(reg16, bAndVal) (XBYTE[(reg16)] &= (bAndVal))
#define vRegSetBit(reg16, bBit)  (XBYTE[(reg16)] |= (bBit))
#define vRegClrBit(reg16, bBit) (XBYTE[(reg16)] &= (~(bBit)))
#endif

void vRegWrite2B(UINT16 reg16, UINT16 val16) reentrant;
void vRegWrite2BMsk(UINT16 reg16, UINT16 val16, UINT16 msk16) reentrant;
void vRegWrite4B(UINT16 reg16, UINT32 val32) reentrant;
void vRegWrite4B_S(UINT16 reg16, UINT8 bByte3, UINT8 bByte2, UINT8 bByte1, UINT8 bByte0) reentrant;
void vRegWrite4BMsk(UINT16 reg16, UINT32 val32, UINT32 msk32) reentrant;

// Field Access Macro

#define RegReadFld(reg16,fld) 	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?u1RegRead1B(reg16+(Fld_ac(fld)-AC_FULLB0)):( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?u2RegRead2B(reg16+(Fld_ac(fld)-AC_FULLW10)):( \
	(Fld_ac(fld)==AC_FULLDW)? u4RegRead4B(reg16):( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?(u1RegRead1B(reg16+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?(u2RegRead2B(reg16+(Fld_ac(fld)-AC_MSKW10))&Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))):( \
	(Fld_ac(fld)==AC_MSKDW)?(u4RegRead4B(reg16)&Fld2Msk32(fld)):0 \
  ))))))

#define RegReadFldAlign(reg16,fld) (((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?u1RegRead1B(reg16+(Fld_ac(fld)-AC_FULLB0)):( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?u2RegRead2B(reg16+(Fld_ac(fld)-AC_FULLW10)):( \
	(Fld_ac(fld)==AC_FULLDW)? u4RegRead4B(reg16):( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?((u1RegRead1B(reg16+(Fld_ac(fld)-AC_MSKB0))&Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0)))>>(Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKB0))):( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?((u2RegRead2B(reg16+(Fld_ac(fld)-AC_MSKW10))&Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10)))>>(Fld_shft(fld)-8*(Fld_ac(fld)-AC_MSKW10))):( \
	(Fld_ac(fld)==AC_MSKDW)?((u4RegRead4B(reg16)&Fld2Msk32(fld))>>Fld_shft(fld)):0 \
  ))))))

#define vRegWriteFld(reg16,val,fld) \
	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))? vRegWrite1B(reg16+(Fld_ac(fld)-AC_FULLB0),(UINT8)(val)):( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?vRegWrite2B(reg16+(Fld_ac(fld)-AC_FULLW10),(UINT16)(val)):( \
	(Fld_ac(fld)==AC_FULLDW)?vRegWrite4B(reg16,(val)):( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?vRegWrite1BMsk(reg16+(Fld_ac(fld)-AC_MSKB0),(UINT8)(val),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?vRegWrite2BMsk(reg16+(Fld_ac(fld)-AC_MSKW10),(UINT16)(val),Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))):( \
	(Fld_ac(fld)==AC_MSKDW)?vRegWrite4BMsk(reg16,(val),Fld2Msk32(fld)):0\
	))))))

#define vRegWriteFldAlign(reg16,val,fld) \
	(((Fld_ac(fld)>=AC_FULLB0)&&(Fld_ac(fld)<=AC_FULLB3))?vRegWrite1B(reg16+(Fld_ac(fld)-AC_FULLB0),(UINT8)(val)):( \
	((Fld_ac(fld)>=AC_FULLW10)&&(Fld_ac(fld)<=AC_FULLW32))?vRegWrite2B(reg16+(Fld_ac(fld)-AC_FULLW10),(UINT16)(val)):( \
	(Fld_ac(fld)==AC_FULLDW)?vRegWrite4B(reg16,(val)):( \
	((Fld_ac(fld)>=AC_MSKB0)&&(Fld_ac(fld)<=AC_MSKB3))?vRegWrite1BMsk(reg16+(Fld_ac(fld)-AC_MSKB0),(UINT8)ValAlign2Fld((val),fld),Fld2MskBX(fld,(Fld_ac(fld)-AC_MSKB0))):( \
	((Fld_ac(fld)>=AC_MSKW10)&&(Fld_ac(fld)<=AC_MSKW32))?vRegWrite2BMsk(reg16+(Fld_ac(fld)-AC_MSKW10),(UINT16)ValAlign2Fld((val),fld),Fld2MskWX(fld,(Fld_ac(fld)-AC_MSKW10))):( \
	(Fld_ac(fld)==AC_MSKDW)?vRegWrite4BMsk(reg16,((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld)):0\
	))))))


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

#define   vRegWriteFldMulti(reg16,list) \
{ \
	typedef UINT16 upk;\
	if ((list)==0xff){typedef UINT8 upk;vRegWrite1B(reg16,(UINT8)(list));}\
	else if((list)==0xff00){typedef UINT8 upk;vRegWrite1B(reg16+1,(UINT8)((list)>>8));}\
	else if((list)==0xff0000){typedef UINT8 upk;vRegWrite1B(reg16+2,(UINT8)((list)>>16));}\
	else if((list)==0xff000000){typedef UINT8 upk;vRegWrite1B(reg16+3,(UINT8)((list)>>24));}\
	else if((list)==0xffff){typedef UINT8 upk;vRegWrite2B(reg16,(UINT16)(list));}\
	else if((list)==0xffff00){typedef UINT8 upk;vRegWrite2B(reg16+1,(UINT16)((list)>>8));}\
	else if((list)==0xffff0000){typedef UINT8 upk;vRegWrite2B(reg16+2,(UINT16)((list)>>16));}\
	else if((list)==0xffffffff){typedef UINT8 upk;vRegWrite4B(reg16,(list));}\
	else if(((list)&0xff)&&(!((list)&0xffffff00))){enum {msk=(UINT8)(list)};{typedef UINT8 upk;vRegWrite1BMsk(reg16,(UINT8)(list),msk);}}\
	else if(((list)&0xff00)&&(!((list)&0xffff00ff))){enum {msk=(UINT8)((list)>>8)};{typedef UINT8 upk;vRegWrite1BMsk(reg16+1,(UINT8)((list)>>8),msk);}}\
	else if(((list)&0xff0000)&&(!((list)&0xff00ffff))){enum {msk=(UINT8)((list)>>16)};{typedef UINT8 upk;vRegWrite1BMsk(reg16+2,(UINT8)((list)>>16),msk);}}\
	else if(((list)&0xff000000)&&(!((list)&0x00ffffff))){enum {msk=(UINT8)((list)>>24)};{typedef UINT8 upk;vRegWrite1BMsk(reg16+3,(UINT8)((list)>>24),msk);}}\
	else if(((list)&0xffff)&&(!((list)&0xffff0000))){enum {msk=(UINT16)(list)};{typedef UINT8 upk;vRegWrite2BMsk(reg16,(UINT16)(list),msk);}}\
	else if(((list)&0xffff00)&&(!((list)&0xff0000ff))){enum {msk=(UINT16)((list)>>8)};{typedef UINT8 upk;vRegWrite2BMsk(reg16+1,(UINT16)((list)>>8),msk);}}\
	else if(((list)&0xffff0000)&&(!((list)&0x0000ffff))){enum {msk=(UINT16)((list)>>16)};{typedef UINT8 upk;vRegWrite2BMsk(reg16+2,(UINT16)((list)>>16),msk);}}\
	else if((list)){enum {msk=(UINT16)((list)&0xffff)};enum {msk2=(UINT16)((list)>>16)};{typedef UINT8 upk;vRegWrite4BMsk(reg16,(list),((UINT32)(msk&0xffff))+((UINT32)(msk2&0xffff)<<16));}}\
}

#define   vRegWriteFldD4val(reg16,dval32,list)  \
{ \
	typedef UINT16 upk;\
	{enum {msk=(UINT16)((list)&0xffff)};enum {msk2=(UINT16)((list)>>16)}; \
	{typedef UINT8 upk; vRegWrite4B(reg16,((dval32)&~(((UINT32)(msk&0xffff))+((UINT32)(msk2&0xffff)<<16)))|(list));}}\
}

#define u1IO32Read1B(reg16) 	u1RegRead1B(reg16) 
#define u4IO32Read4B(reg16) 	u4RegRead4B(reg16)

#define vIO32Write1B(reg16, val8) 	vRegWrite1B(reg16, val8) 
#define vIO32Write2B(reg16, val16) 	vRegWrite2B(reg16, val16)
#define vIO32Write4B(reg16,val32) 	vRegWrite4B(reg16,val32)

#define vIO32Write1BMsk(reg16, val8,msk8) 	vRegWrite1BMsk(reg16, val8,msk8) 
#define vIO32Write2BMsk(reg16, val16, msk16)	vRegWrite2BMsk(reg16, val16, msk16)
#define vIO32Write4BMsk(reg16, val32, msk32)	vRegWrite4BMsk(reg16, val32, msk32)

#define IO32ReadFld(reg16,fld)  			RegReadFld(reg16,fld)  
#define IO32ReadFldAlign(reg16,fld) 		RegReadFldAlign(reg16,fld)

#define vIO32WriteFld(reg16,val,fld) 		vRegWriteFld(reg16,val,fld) 
#define vIO32WriteFldAlign(reg16,val,fld)  	vRegWriteFldAlign(reg16,val,fld)
#define vIO32WriteFldMulti(reg16,list)  	vRegWriteFldMulti(reg16,list) 
#define vIO32WriteFldD4val(reg16,dval32,list)  vRegWriteFldD4val(reg16,dval32,list)
#endif

typedef struct 
{
	UINT16 wAddr;
	UINT8 dwVal;
} strucU8RegSetting;

typedef struct 
{
	UINT16 wAddr;
	UINT16 wVal;
} strucU16RegSetting;

typedef struct 
{
	UINT16 wAddr;
	UINT32 dwVal;
} strucU32RegSetting;


#endif
