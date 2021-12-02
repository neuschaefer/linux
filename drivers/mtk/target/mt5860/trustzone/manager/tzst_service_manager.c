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
 * $LastChangedRevision: 2875 $
 * ----------------------------------------------------------------------------
*/

/* ============================================================================
    Includes
============================================================================ */
#include <string.h>

#include "svc_registry.h"

/* ============================================================================
    Private Functions
============================================================================ */

/* ----------------------------------------------------------------------------
 * Description:
 *     This function searches the registry for a service with ID = uiSvcID.
 *
 * Arguments:
 *     uiSvcID - The Service ID to match.
 *
 * Return:
 *     sService_t const * - A pointer to the service structure, or NULL if not
 *         found.
 * ------------------------------------------------------------------------- */
static TZ_SERVICE_T const * SvcMgr_FindService(UINT32 uiSvcID)
{
    /* The service currently being examined. */
    TZ_SERVICE_T const * prService = _garService;

    /* Loop through registry until exhausted or a matching service is found. */
    while(prService->prFunctions != NULL)
    {
        /* If the Service ID matches the current service, return pointer. */
        if (prService->u4ServiceID == uiSvcID)
        {
            return prService;
        }

        /* If service does not match, increment. */
        prService++;
    }
    
    /* If we reach here, no service matched. */
    return NULL;
}

/* ----------------------------------------------------------------------------
 * Description:
 *     This function searches a service for a function with SMCID = u4SMCID.
 *
 * Arguments:
 *     pksService - The Service to search.
 *     uiSvcID - The SMCID (including Service ID, Function ID and SMC Type) to 
 *         match.
 *
 * Return:
 *     TZ_FUNCTION_T const * - A pointer to the function descriptor, or 
 *         NULL if not found.
 * ------------------------------------------------------------------------- */
static TZ_FUNCTION_T const * SvcMgr_FindFunction(
    TZ_SERVICE_T const * prService, UINT32 u4SMCID)
{
    /* The function currently being examined. */
    TZ_FUNCTION_T const * prFunction = prService->prFunctions;

    /* Loop through function list until exhausted or match found. */
    while(prFunction->pFunction != NULL)
    {
        /* If the SMC ID matches the current function, return pointer. */
        if (prFunction->u4SMCID == u4SMCID)
        {
            return prFunction;
        }

        /* If function does not match, increment. */
        prFunction++;
    }

    /* If we reach here, no function matched. */
    return NULL;
}

/* ============================================================================
    Public Functions
============================================================================ */

/* ============================================================================
   See header for more details.
============================================================================ */

/* ============================================================================
   See header for more details.
============================================================================ */
UINT32 SvcMgr_Main(TZ_SMC_ARG_T const * pksArgs)
{
    /* Extract the SMC ID into a local variable for convenient use. */
    UINT32        u4SMCID = pksArgs->u4SMCID;

    /* Extract fields from the SMC ID. */
    UINT32        uiSMCType = (u4SMCID & SMC_TP_MASK) >> SMC_TP_SHIFT;
    UINT32        uiSvcID =   (u4SMCID & SVC_ID_MASK) >> SVC_ID_SHIFT;

    /* Convenience pointers to the service and function structures. */
    TZ_SERVICE_T const * pksService;
    TZ_FUNCTION_T const * pksFunction;

    //Printf("[Trustzone] SMC ID: 0x%x\n", u4SMCID);

    /* Find the service in question, based on the Service ID (uiSvcID). */
    pksService = SvcMgr_FindService(uiSvcID);
    if (pksService == NULL)
    {
        return TZ_ERROR_ACCESS_DENIED;
    }

    /* Find the function in question, based on the whole SMC ID. */
    pksFunction = SvcMgr_FindFunction(pksService, u4SMCID);
    if (pksFunction == NULL)
    {
        return TZ_ERROR_ACCESS_DENIED;
    }

    /* ------------------------------------------------------------------------
     * Functions of type SMC_TP_SYSTEM do not use the SMC protocol, and must 
     * implement a local WSM check. Functions of type SMC_TP_TZAPI mandate WSM, 
     * and these are checked automatically by the Service Manager before 
     * invoking the service function.
     * --------------------------------------------------------------------- */

    switch (uiSMCType)
    {         
        /* System functions do not receive WSM details. */
        case SMC_TP_SYSTEM:
        {
            return pksFunction->pFunction(pksArgs, NULL);
        }

        /* If SMC type does not match, then error. */
        default:
        {
            return TZ_ERROR_ACCESS_DENIED;
        }
    }
}

/* ============================================================================
   See header for more details.
============================================================================ */
void SvcMgr_InitServices(void)
{
    TZ_SERVICE_T const * pksService;

    /* Run the initialization function for each service, if available. */
    pksService = _garService;

    while(pksService->prFunctions != NULL)
    {
        if (pksService->pInitialize != NULL)
        {
            pksService->pInitialize();
        }

        pksService++;
    }
}

