/*
 * $RCSfile: aess_eventhandlerdrv.h,v $
 * $Revision: 1.2 $
 * $Date: 2008/12/17 11:19:20 $
 * $Author: solowu $
 *
 * Aspeed Kernel Event handler driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#ifndef AESSEVENTHANDLER_H
#define AESSEVENTHANDLER_H

#ifdef AESSEVENTHANDLER_C 
/* For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef int                 INT32;
/* init flag */
#define INIT_OK                 0x0
#define INIT_FAIL               0x1

#define MAX_EVENT_NUMBER        0x32

/******************************************************************************
*   STRUCT      :   sEventData
******************************************************************************/
/**
 *  @brief   Structure to event handler driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
	/* Driver ID */
	UINT16 u16DriverID;		
	
	/* Event command ID */
	UINT32 u32EventID;     
	
} sEventData;

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
int aess_generate_driver_event(
								  /* Driver ID, issue by the the event \n
									 handler API in user space */
								  UINT16 u16DriverID, 
								  
								  /* Event ID, defined by each event group */
								  UINT32 u32EventID
							  );

#else
extern int aess_generate_driver_event(
									  /* Driver ID, issue by the the event \n
										 handler API in user space */
									  UINT16 u16DriverID, 
								  
									  /* Event ID, defined by each  \n
										 event group */
									  UINT32 u32EventID
									 );


#endif   /* AESSEVENTHANDLER_C */

#endif   /* AESSEVENTHANDLER_H */
