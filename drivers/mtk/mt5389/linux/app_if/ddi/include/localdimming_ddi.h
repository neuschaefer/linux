
/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @frc_ddi.h
 *
 * This source file defines the DDI functions related to FRC9449H Device Driver control
 *
 *  @author	Kim, Jeong sim(nanbean@lge.com)
 *  @author	Baek, Se hoon(baiksh5@lge.com) - modifier
 *  @version	0.1
 *  @date		2006.03.16
 *  @date		2006.08.07
 *  @note		Additional information.
 *  @see		frc_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_LOCALDIMMING_DDI_H_
#define	_LOCALDIMMING_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM // modified by dilly97 : fix include
#include "common.h"
#else
#include "common_win32.h"
#endif
#include "i2c_ddi.h"
#include "global_configurations.h"

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
   로컬 상수 정의 (Local Constant Definitions)
******************************************************************************/

/******************************************************************************
    로컬 형 정의 (Local Type Definitions)
******************************************************************************/

/******************************************************************************
    전역 형 정의 (Global Type Definitions)
******************************************************************************/

/******************************************************************************
    Extern 전역변수와 함수 prototype 선언
    (Extern Variables & Function Prototype Declarations)
******************************************************************************/

/******************************************************************************
    Static 변수와 함수 prototype 선언
    (Static Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    로컬 변수와 함수 prototype 선언
    (Local Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    전역 변수와 함수 prototype 선언
    (Global Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************

	함수 정의
	(Function Definitions)
******************************************************************************/
DTV_STATUS_T DDI_LOCALDIMMING_WriteI2C(UINT8  sub_addr, UINT16 num_of_data, UINT8  *txdata);
DTV_STATUS_T DDI_LOCALDIMMING_ReadI2C(UINT8  sub_addr, UINT16 num_of_data, UINT8  *rxdata);
DTV_STATUS_T DDI_LOCALDIMMING_Initialize(void);
DTV_STATUS_T DDI_LOCALDIMMING_UpdateFirmware(void);
DTV_STATUS_T DDI_LOCALDIMMING_CtrlDemo(BOOLEAN bCtrl);
DTV_STATUS_T DDI_LOCALDIMMING_CtrlLocalDimming(BOOLEAN bCtrl);
DTV_STATUS_T DDI_LOCALDIMMING_CtrlCompensation(BOOLEAN bCtrl);
void DDI_DEBUG_LOCALDIMMING(void);

#endif
