
/******************************************************************************
*[File]				pq_gamma_if.h
*[Version]			v0.1
*[Revision Date]	2009-07-01
*[Author]			cheah
*[Description]
*    Header file of Sony PQ Gamma
*[Copyright]
*    Copyright (C) 2009 Sony All Rights Reserved.
******************************************************************************/

#ifndef PQ_GAMMA_IF_H
#define PQ_GAMMA_IF_H


/*-----------------------------------------------------------------------------
	include files
-----------------------------------------------------------------------------*/
//#include "../../../../../d_inc/d_mtk_cust.h"


#ifdef CC_SONY_GAMMA
/*-----------------------------------------------------------------------------
	Defines
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Enum, typedef 
-----------------------------------------------------------------------------*/

enum{
	RGB_HIGHLIGHT,// 0
	RGB_CUTOFF    // 1
};

enum{
	RGB_COOL,   // 0
	RGB_NEUTRAL,// 1
	RGB_WARM1,  // 2
	RGB_WARM2,  // 3
	RGB_PHOTO   // 4
};

enum{
	RGB_RED,    // 0 
	RGB_GREEN,  // 1
	RGB_BLUE    // 2
};

/*-----------------------------------------------------------------------------
	Structures 
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Public functions (for Sony external src)
-----------------------------------------------------------------------------*/

/* Sony Gamma */
extern void DRVCUST_Gamma_Init(void);
extern void DRVCUST_Gamma_TblReg(UINT8 *ptr_b, UINT8 *ptr_m, UINT8 *ptr_d, UINT16 size);
extern void DRVCUST_Gamma_ISR(void);

/* Color temperature */
extern void DRVCUST_AutoWB_Init(void);
extern void DRVCUST_AutoWB_Update(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo);
extern void DRVCUST_AutoWB_Proc(void);
extern void DRVCUST_AutoWB_ISR(void);

/*>>> START Cheah: Sony GammaWB callback func 07Sep09 */
extern void Sony_GammaSetup (UINT16 write_data);
extern void Sony_GammaSave (UINT16 write_data);
extern void Sony_GammaQuit (UINT16 write_data);
extern void Sony_GammaExit (UINT16 write_data);
extern void Sony_GammaResetNVM (void);
extern void Sony_GammaSetSample (UINT16 write_data); /* for design evaluation only */
extern void Sony_WBSetup (UINT16 write_data);
extern void Sony_WBSave (UINT16 write_data);
extern void Sony_WBQuit (UINT16 write_data);
extern void Sony_WBExit (UINT16 write_data);
extern void Sony_WBResetNVM (void);
extern void Sony_WBSetLvl (UINT16 write_data);
extern void Sony_WBSetColor (UINT16 write_data);
/*<<< END Cheah: Sony GammaWB callback func 07Sep09 */
/*>>> Lim 170909: Sony test ptn callback func*/
extern void Sony_GammaSetPatgen (BOOL uGammaPatOnOff);
extern void Sony_PreSetup (BOOL bOnOff);
/*<<<Lim 170909: Sony test ptn callback func*/

/*>>> START cheah, fetch WB data from Sony (08-Sep-09) */
UINT16 DRVCUST_GetRGB(UINT8 bCTIndex, UINT8 bmode, UINT8 bRGB);
UINT16 DRVCUST_GetSonyRGB (UINT8 bCTIndex, UINT8 bmode, UINT8 bRGB);
/*<<< END cheah, fetch WB data from Sony (08-Sep-09) */


#endif /* CC_SONY_GAMMA */


#endif /* PQ_GAMMA_IF_H */

