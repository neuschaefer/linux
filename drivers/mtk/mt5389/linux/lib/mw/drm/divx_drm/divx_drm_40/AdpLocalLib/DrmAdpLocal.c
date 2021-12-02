/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpLocal.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KERNEL__

#include "DrmAdpLocal.h"

#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/memory.h"
#include "os/inc/x_os.h"    /* timer */
#else
#include <stdio.h>   /* io */
#include <string.h>  /* strcmp */
#include <time.h>    /* clock */
#endif

#include "divx_drm_glue.h"

#else

#include "DrmAdpLocal.h"

#include "../../divx_drm_glue.h"

#endif

#if 0
#define MODEL_ID_LOW_BYTE (0x31)		/* 0x31 is only simulation mode. */
#define MODEL_ID_TOP_BYTE (0x30 & 0xF0) /* Only half of the byte counts. 0x30 is only for simulation. */
#else
extern UINT8 gui1_divx_drm_model_id_low_byte;
extern UINT8 gui1_divx_drm_model_id_high_byte;
#endif

void localSetModelId(uint8_t *modelId)
{
	if (NULL != modelId)
	{
#if 0	
		modelId[0] = (uint8_t)MODEL_ID_LOW_BYTE;
		modelId[1] = (uint8_t)MODEL_ID_TOP_BYTE & 0xF0; /* Only half of the byte counts. */
#else
                modelId[0] = (uint8_t)gui1_divx_drm_model_id_low_byte;
                modelId[1] = (uint8_t)gui1_divx_drm_model_id_high_byte & 0xF0; /* Only half of the byte counts. */
#endif
	}
}

uint32_t localClock()
{
    return (uint32_t)x_os_get_sys_tick();
}

uint32_t localClockTicksPerSec()
{
    return (uint32_t)x_os_get_sys_tick_period();
}

uint8_t localLoadDrmMemory(uint8_t *memory)
{
    uint8_t returnCode = ADP_LOCAL_SUCCESS;

    if (!divx_drm_mem_read_glue((UINT8 *)memory))
    {
        returnCode = DRM_ERROR_READING_MEMORY;    
    }

    return returnCode;    
}

uint8_t localSaveDrmMemory(uint8_t *memory)
{  
    uint8_t returnCode = ADP_LOCAL_SUCCESS;

    if (!divx_drm_mem_write_glue((UINT8 *)memory))
    {
        returnCode = DRM_ERROR_READING_MEMORY;
    }

    return returnCode;    
}

