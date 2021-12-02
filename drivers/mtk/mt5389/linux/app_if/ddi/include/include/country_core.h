/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 2008 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file country_core.h
 *
 * This header file defines the macro/value definitions defining country/group.
 *
 *	@author 	David (DONG-IL) Lee
 *	@version	1.0
 *	@date		2008.05.12
 *	@note
 *	@see
 */
#ifndef	_COUNTRY_CORE_H_
#define	_COUNTRY_CORE_H_
#ifdef __cplusplus
extern "C" {
#endif

#define	RMM_REGION_OF(group, country)			( ((group & 0xff) << 24) | ((country & 0xffffff) << 0))
#define	RMM_GROUP_OF(region)					( (region & 0xff000000) >> 24 )
#define	RMM_COUNTRY_OF(region)					( (region & 0x00ffffff) >>  0 )

#define RMM_G0_OF(region)						((region & 0xff000000) >> 24)
#define RMM_C0_OF(region)						((region & 0x00ff0000) >> 16)
#define RMM_C1_OF(region)						((region & 0x0000ff00) >>  8)
#define RMM_C2_OF(region)						((region & 0x000000ff) >>  0)
#define RMM_CURR_COUNTRY_ORDER					((UINT8)((gpRmmCountry)?(gpRmmCountry->country_order):RMM_COUNTRY_ORDER_UNKNOWN))

#define	RMM_REGION_CODE							gnRmmRegion

#define	RMM_CODE_MASK							0xf
#define	RMM_CODE_VAL_GG(g, g0,g1)				(g)
#define	RMM_CODE_VAL_CC(c0,c1,c2)				((C_##c0 << 16) | (C_##c1 << 8) | (C_##c2 << 0))
#define	RMM_CODE_VAL_GG_CC(g0, g1,g2, c0,c1,c2)	(((RMM_CODE_VAL_GG(g0, g1,g2) & RMM_CODE_MASK) << 24) | RMM_CODE_VAL_CC(c0,c1,c2))

#define	str(s)		#s
#define	ASSIGN_COUNTRY_GROUP(g0, g1, g2, full_name)	\
			RMM_GROUP_ELMT_T Group_##g1##g2 = { RMM_CODE_VAL_GG(g0, g1,g2), str(g1 ## g2), full_name, aCountryList_##g1##g2}

#define	RMM_SET_REGION_CODE(code)	RMM_SetCode(RMM_CODE_TYPE_REGION,	(RMM_COMMON_CODE_T)code)

#ifdef __cplusplus
}
#endif
#endif	/* _COUNTRY_CORE_H_ */
