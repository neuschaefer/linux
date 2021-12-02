/*******************************************************************************
 *
 * FILE NAME          : MxL601_TunerSpurTable.h
 * 
 * AUTHOR             : Dong Liu
 *  
 * DATE CREATED       : 11/16/2011
 *
 * DESCRIPTION        : This file contains spur table definition.
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL601_TUNER_SPUR_TABLE_H__
#define __MXL601_TUNER_SPUR_TABLE_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/
#include "MaxLinearDataTypes.h"
#include "x_typedef.h"

/******************************************************************************
    Macros
******************************************************************************/
#define   MAX_SPUR_REG_NUM    4 
/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
typedef struct
{
  UINT8 SpurRegNum;  
  UINT8 SpurRegAddr[MAX_SPUR_REG_NUM];
} CHAN_DEPENDENT_SPUR_REGISTER_T, *PCHAN_DEPENDENT_SPUR_REGISTER_T;

typedef struct
{
  UINT32 centerFreqHz;
  UINT8  rfLutSwp1Reg;
  UINT8  rfLutDivInBypReg;
  UINT8  refLutBypReg;
  UINT8  refIntModReg;
} CHAN_DEPENDENT_FREQ_TABLE_T, *PCHAN_DEPENDENT_FREQ_TABLE_T;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/
extern CHAN_DEPENDENT_SPUR_REGISTER_T MxL601_SPUR_REGISTER;

extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_XTAL_16MHZ_LIF[];
extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_XTAL_24MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_HRC_16MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_HRC_24MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_IRC_16MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_IRC_24MHZ[];

extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_BG_7MHZ_LUT_XTAL_16MHZ_LIF[];
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_BG_7MHZ_LUT_XTAL_24MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_BG_8MHZ_LUT_XTAL_16MHZ_LIF[];
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_BG_8MHZ_LUT_XTAL_24MHZ[];

extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_D_LUT_XTAL_16MHZ_LIF[]; 
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_D_LUT_XTAL_24MHZ[]; 

extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_I_LUT_XTAL_16MHZ_LIF[]; 
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_I_LUT_XTAL_24MHZ[]; 

extern CHAN_DEPENDENT_FREQ_TABLE_T SECAM_L_LUT_XTAL_16MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T SECAM_L_LUT_XTAL_24MHZ[];

extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_D_LUT_XTAL_16MHZ_HIF[];
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_I_LUT_XTAL_16MHZ_HIF[]; 
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_BG_7MHZ_LUT_XTAL_16MHZ_HIF[];
extern CHAN_DEPENDENT_FREQ_TABLE_T PAL_BG_8MHZ_LUT_XTAL_16MHZ_HIF[];
extern CHAN_DEPENDENT_FREQ_TABLE_T NTSC_FREQ_LUT_XTAL_16MHZ_HIF[];

extern CHAN_DEPENDENT_FREQ_TABLE_T DIG_CABLE_FREQ_LUT_BW_6MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T DIG_CABLE_FREQ_LUT_BW_8MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T DIG_TERR_FREQ_LUT_BW_6MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T DIG_TERR_FREQ_LUT_BW_7MHZ[];
extern CHAN_DEPENDENT_FREQ_TABLE_T DIG_TERR_FREQ_LUT_BW_8MHZ[];

/******************************************************************************
    Prototypes
******************************************************************************/

#endif /* __MXL601_TUNER_SPUR_TABLE_H__*/




