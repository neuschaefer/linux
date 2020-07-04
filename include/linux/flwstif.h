#ifndef __FLWSTIF_H_INCLUDED__
#define __FLWSTIF_H_INCLUDED__

                /*--------------------------------------*/
                /* flwstif.h and flwstif.c for Linux OS */
                /*--------------------------------------*/

/* 
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom Corporation
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
:>
*/

#if defined(__KERNEL__)                 /* Kernel space compilation         */
#include <linux/types.h>                /* LINUX ISO C99 7.18 Integer types */
#else                                   /* User space compilation           */
#include <stdint.h>                     /* C-Lib ISO C99 7.18 Integer types */
#endif

typedef struct {
    unsigned long rx_packets;
    unsigned long rx_bytes;
    unsigned long pollTS_ms; // Poll timestamp in ms
}FlwStIf_t;

typedef enum {
    FLWSTIF_REQ_GET,
    FLWSTIF_REQ_PUSH,
    FLWSTIF_REQ_MAX
}FlwStIfReq_t;

extern uint32_t flwStIf_request( FlwStIfReq_t req, void *ptr, uint32_t param1,
                                 uint32_t param2, uint32_t param3 );

typedef int (* flwStIfGetHook_t)( uint32_t flwIdx, FlwStIf_t *flwSt_p );

typedef int (* flwStIfPushHook_t)( void *ctk1, void *ctk2, uint32_t direction,
                                   FlwStIf_t *flwSt_p );

extern void flwStIf_bind(flwStIfGetHook_t flwStIfGetHook, flwStIfPushHook_t flwStIfPushHook);

#endif /* __FLWSTIF_H_INCLUDED__ */
