/*-----------------------------------------------------------------------------
// $Header: 
// (C) Copyright 2001 NXP Semiconductors, All rights reserved
//
// This source code and any compilation or derivative thereof is the sole
// property of NXP Corporation and is provided pursuant to a Software
// License Agreement.  This code is the proprietary information of NXP
// Corporation and is confidential in nature.  Its use and dissemination by
// any party other than NXP Corporation is strictly limited by the
// confidential information provisions of the Agreement referenced above.
//-----------------------------------------------------------------------------
// FILE NAME:    tmddTDA182I2Instance.h
//
// DESCRIPTION:  define the static Objects
//
// DOCUMENT REF: DVP Software Coding Guidelines v1.14
//               DVP Board Support Library Architecture Specification v0.5
//
// NOTES:        
//-----------------------------------------------------------------------------
*/
#ifndef _TMDDTDA182I2_INSTANCE_H //-----------------
#define _TMDDTDA182I2_INSTANCE_H

tmErrorCode_t ddTDA182I2AllocInstance (tmUnitSelect_t tUnit, pptmddTDA182I2Object_t ppDrvObject);
tmErrorCode_t ddTDA182I2DeAllocInstance (tmUnitSelect_t tUnit);
tmErrorCode_t ddTDA182I2GetInstance (tmUnitSelect_t tUnit, pptmddTDA182I2Object_t ppDrvObject);


#endif // _TMDDTDA182I2_INSTANCE_H //---------------
