/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   Permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 	sysres.h.h
 *
 *  Definitions for global configurations
 *  DESCRIPTION : In this file, all global configurations, enumerations, data
			   or structures should be shared between modules positioned in
			   different layers.
			   For example, enumeration for external video supported in the system,
			   should be referenced in User Interface module (APP layer) and,
			   at the same time, referenced Video module(Driver Layer).
			   So, these kind of definition or declaration should be stated
			   here.
 *  @author	Baekwon Choi (최배권, bk1472@lge.com)
 *  @version 	1.1
 *  @date	2008.07.28
 *  @see
 */

#ifndef _SYSRES_H_
#define _SYSRES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	매크로 함수  정의
	(Macros Definitions)
------------------------------------------------------------------------------*/
#ifdef _SYSRES_CONF_C_
#define SYSRS_VAR(vType, vName, vValue)		vType vName = (vType) vValue
#define SYSRS_EXT
#else
#define SYSRES_VAR(vType, vName, vValue)	extern vType vName
#define SYSRES_EXT                      	extern
#endif

#ifdef __cplusplus
}
#endif

#endif/*_SYSRES_H_*/
