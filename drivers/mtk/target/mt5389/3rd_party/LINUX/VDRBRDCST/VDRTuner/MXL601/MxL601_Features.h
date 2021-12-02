/*******************************************************************************
 *
 * FILE NAME          : MxL601_Features.h
 * 
 * AUTHOR             : Dong Liu
 *  
 * DATE CREATED       : 08/17/2012
 *
 * DESCRIPTION        : This file contains MxL601 device feature definition. 
 *                      User can enable or disable certain macro-definition to 
 *                      add or delete corresponding feature (funtion) 
 *
 *******************************************************************************
 *                Copyright (c) 2012, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL601_TUNER_FEATURES_H__
#define __MXL601_TUNER_FEATURES_H__

/******************************************************************************
   Features List 
******************************************************************************/
// ENABLE_MANUAL_AFC is used to protect Tuner AFC program bit while do  
//   application mode over write default. 
// Enable this macro-definition means Tuner AFC feature will be turned on   
//   or turned off by calling MXL_TUNER_AFC_CFG API.
// Disable this macro-definition means in analog TV mode, tuner AFC function 
//   is always turned on. 
#define ENABLE_MANUAL_AFC  	            1    

// ENABLE_TELETEXT_SPUR is used enable or disable the feature of "reduce spurs 
//   that caused by teletext broadcast"
// For the application that does not has teletext broadcast, this feature can 
//   be disabled. 
//#define ENABLE_TELETEXT_SPUR_FEATURE    1 

// CUSTOMER_SPECIFIC_SETTING_1 is used for one customer special setting while 
//   implement application mode setting and channel tune 
//#define CUSTOMER_SPECIFIC_SETTING_1   1

// CUSTOMER_SPECIFIC_SETTING_2 is used for one customer special setting while 
//   implement channel tune operation
//#define CUSTOMER_SPECIFIC_SETTING_2   1

// CUSTOMER_SPECIFIC_SETTING_3 is used on one customer platform, it is used to   
//   enable or disable NTSC N+1 block performance improvement.  
//#define CUSTOMER_SPECIFIC_SETTING_3    1 

#endif /* __MXL601_TUNER_FEATURES_H__ */




