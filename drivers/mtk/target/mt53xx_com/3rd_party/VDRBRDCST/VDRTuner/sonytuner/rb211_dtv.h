/*------------------------------------------------------------------------------

 Copyright 2010 Sony Corporation

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Sony Corporation.
 No part of this file may be copied, modified, sold, and distributed in any
 form or by any means without prior explicit permission in writing from
 Sony Corporation.

 Date: 2010/07/07
 Revision: 1.0.1.0

------------------------------------------------------------------------------*/

#ifndef RB211_DTV_H
#define RB211_DTV_H

/* Digital terrestrial System definitions */
typedef enum {
	RB211_DTV_SYSTEM_UNKNOWN,
	RB211_DTV_SYSTEM_QAM,      /* QAM */
	RB211_DTV_SYSTEM_ATSC,     /* ATSC(8VSB) */
	RB211_DTV_SYSTEM_ISDBT,    /* ISDB-T */
	RB211_DTV_SYSTEM_DVBT_6,   /* DVB-T 6MHzBW */
	RB211_DTV_SYSTEM_DVBT_7,   /* DVB-T 7MHzBW */
	RB211_DTV_SYSTEM_DVBT_8,   /* DVB-T 8MHzBW */
	RB211_DTV_SYSTEM_DVBT2_5,  /* DVB-T2 5MHzBW */
	RB211_DTV_SYSTEM_DVBT2_6,  /* DVB-T2 6MHzBW */
	RB211_DTV_SYSTEM_DVBT2_7,  /* DVB-T2 7MHzBW */
	RB211_DTV_SYSTEM_DVBT2_8,  /* DVB-T2 8MHzBW */
	RB211_DTV_SYSTEM_DVBC,     /* DVB-C */
	RB211_DTV_SYSTEM_DTMB      /* DTMB */
} rb211_dtv_system_t;

#endif /* RB211_DTV_H */
