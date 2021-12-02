/*
 * Copyright:
 * ----------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorised 
 * by a licensing agreement from ARM Limited.
 *      (C) COPYRIGHT 2008-2009 ARM Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised copies and 
 * copies may only be made to the extent permitted by a licensing agreement
 * from ARM Limited.
 * ----------------------------------------------------------------------------
 * $LastChangedRevision: 2839 $
 * ----------------------------------------------------------------------------
 */

#ifndef TZST_H
#define TZST_H

/* ============================================================================
    Includes
============================================================================ */
#include "x_typedef.h"
#include "x_printf.h"
#include "tz.h"


#ifdef NOLOG
#define DMSG(...)
#else
#define DMSG(...) dprintf("[%s] %s() line%d\n", __FILE__, __FUNCTION__, __LINE__); dprintf(__VA_ARGS__);
#endif


/* A structure that describes a single WSM reference; a buffer withing a
 * previously registered WSM region. */
typedef struct
{
    void * pBlock;
    UINT32 u4Length;
} TZ_WSM_T;

/* ============================================================================
    Function Structure Definition
============================================================================ */

/* This structure represents one Security Function. Each function is identified
 * by its ID, and this should be unique among all other function IDs. */
typedef struct
{
    /* The complete SMC ID which must be used in order to invoke this
     * function. This should be unique. */
    UINT32    u4SMCID;

    /* A pointer to the function itself. If WSM is not used or not available,
     * the psWSM argument will be set to NULL. 
     * Note that pksSMCArgs will never be NULL, and is guaranteed to point to
     * a valid structure. */
    UINT32 (*pFunction) (TZ_SMC_ARG_T const *prSMCArg, TZ_WSM_T const *prWSM);
    
} TZ_FUNCTION_T;

/* ============================================================================
    Service Structure Definition
============================================================================ */

/* This structure represents one service. Each service is identified by its
 * ID, and this should be unique among all other service IDs. This structure
 * also contains a pointer to the function table for this service. */
typedef struct
{
    /* The Service ID. */
    UINT32 u4ServiceID;

    /* If this is not NULL, the Secure World guarantees to call this function
     * before any other service function is called. The service can use this
     * function to put itself in a safe state before it receives requests from
     * the Normal World. */
    void (*pInitialize) (void);

    /* An array of Security Functions provided by this service. The length of
     * the array is given by the uiFunctionCount field. */
    TZ_FUNCTION_T const * prFunctions;

} TZ_SERVICE_T;


/* ============================================================================
    Function prototypes
============================================================================ */

extern void enableDebug(BOOL flag);
extern int dprintf(const char *format, ...);

/* ----------------------------------------------------------------------------
 * Description:
 *     This is the Secure World entry point when the Normal World invokes any
 *     SMC; the monitor returns directly to this function which must then 
 *     route the request accordingly.
 *
 * Arguments:
 *     prArgs - the contents of the registers passed in using the low-level
 *         connectionless protocol.
 *
 * Return:
 *     This function returns an error code from the system (if the function
 *     specified in the SMC_ID field of pksArgs cannot be found) or from the
 *     invoked function itself if it can.
 *
 *     TZAPI returns should be used to enable simpler client implementation.
 *     
 *     This return value is passed directly back to the Normal World by the 
 *     monitor.
 * ------------------------------------------------------------------------- */
UINT32 SvcMgr_Main(TZ_SMC_ARG_T const *prArgs);

/* ----------------------------------------------------------------------------
 * Description:
 *     This functions calls the initializer function of each service in the
 *     system, if they implement one.
 *
 * Arguments:
 *     None.
 *
 * Return:
 *     None.
 * ------------------------------------------------------------------------- */
void SvcMgr_InitServices(void);

#endif  /* TZST_H */
