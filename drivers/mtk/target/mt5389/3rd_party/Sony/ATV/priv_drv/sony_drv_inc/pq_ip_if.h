
/******************************************************************************
*[File]				pq_ip_if.h
*[Version]			v0.1
*[Revision Date]	2009-07-15
*[Author]			
*[Description]
*    Header file of Sony Intelligent-Picture
*[Copyright]
*    Copyright (C) 2009 Sony All Rights Reserved.
******************************************************************************/

#ifndef PQ_IP_IF_H
#define PQ_IP_IF_H


/*-----------------------------------------------------------------------------
	include files
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Defines
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Enum, typedef 
-----------------------------------------------------------------------------*/
typedef enum
{
	IP_LVL1 = 3, // good signal
	IP_LVL2,
	IP_LVL3,
	IP_LVL4,
	IP_LVL5,
	IP_LVL6,
	IP_LVL7,
	IP_LVL8, // weak signal
	IP_RESET,
	NUM_OF_IP_LVL
}IP_LVL_T;


/*-----------------------------------------------------------------------------
	Structures 
-----------------------------------------------------------------------------*/
typedef struct _CUST_IP_NOISE_TH_INFO_T
{
	UINT8 lvl;
	UINT8 data;
}CUST_IP_NOISE_TH_INFO_T;
/*-----------------------------------------------------------------------------
	Public functions (for Sony external src)
-----------------------------------------------------------------------------*/
extern void DRVCUST_IP_Init(void);
extern IP_LVL_T DRVCUST_IP_Get8Level(void);
extern BOOL DRVCUST_IsSourceNoisy(void);
extern UINT8 sony_IPGetIndicatorNoiseLevel(void);
extern void sony_IpSetTh (CUST_IP_NOISE_TH_INFO_T* p_param);
extern void sony_IpGetTh (CUST_IP_NOISE_TH_INFO_T* p_param);

#endif /* PQ_IP_IF_H */

