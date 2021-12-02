/*******************************************************************************
 *
 * FILE NAME          : MxL601_OEM_Drv.h
 * 
 * AUTHOR             : Dong Liu 
 *
 * DATE CREATED       : 01/23/2011
 *
 * DESCRIPTION        : Header file for MxL601_OEM_Drv.c
 *
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MxL601_OEM_DRV_H__
#define __MxL601_OEM_DRV_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/

#include "MaxLinearDataTypes.h"
#include "MxL601_TunerApi.h"
#include "x_typedef.h"

/******************************************************************************
    Macros
******************************************************************************/
#define PAGE_CHANGE_REG                0x00 
#define IF_PATH_GAIN_REG               0x05 
#define DFE_DACIF_BYP_GAIN_REG         0x43 
#define IF_GAIN_SET_POINT1             10 
#define IF_GAIN_SET_POINT2             11 
#define IF_GAIN_SET_POINT3             12 

#define MXL601_MAX_REG_ADDRESS       410 

#define PIC_FREQ_DIFF_ANA_M_N        (1750)  //KHZ
#define PIC_FREQ_DIFF_ANA_B          (2250)
#define PIC_FREQ_DIFF_ANA_GH         (2750)
#define PIC_FREQ_DIFF_ANA_I          (2750)
#define PIC_FREQ_DIFF_ANA_D_K_L      (2750)
#define PIC_FREQ_DIFF_SECAM          (2750)
#define PIC_FREQ_DIFF_SECAM2         (2750)

/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
/* Define the parameter default setting (include XTAL, IF out, AGC and BW etc. for different work mode */
typedef struct
{
  UINT8 SignalMode;             // Tuner work mode, refers DEMOD_MODE_E and ANALOG_MODE_E
  MXL_BW_E BandWidth;           // signal band width 
  UINT8 IfPathSelection;        // define which IF path is selected 
  MXL_IF_FREQ_E IFOutFreq;      // band width of IF out signal 
  MXL_BOOL IFInversion;         // IF spectrum is inverted or not 
  UINT8 IfGainSelfAgc;          // IF out gain level for self AGC 
  UINT8 IfGainCloseAgc;         // IF out gain level for close loop AGC 
  MXL_AGC_ID_E AgcSel;          // AGC selection, AGC1 or AGC2 
  MXL_AGC_TYPE_E AgcType;       // AGC mode selection, self or closed loop 
  UINT8 AgcSetPoint;            // AGC attack point set value 
} MXL_IF_AGC_DEFAULT_SET_T, *PMXL_IF_AGC_DEFAULT_SET_T; 

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

/******************************************************************************
    Prototypes
******************************************************************************/
MXL_STATUS Ctrl_WriteRegister(UINT8 I2cSlaveAddr, UINT8 RegAddr, UINT8 RegData);
MXL_STATUS Ctrl_ReadRegister(UINT8 I2cSlaveAddr, UINT8 RegAddr, UINT8 *DataPtr);
void MxL_Sleep(UINT16 DelayTimeInMs);  


#endif /* __MxL601_OEM_DRV_H__*/




