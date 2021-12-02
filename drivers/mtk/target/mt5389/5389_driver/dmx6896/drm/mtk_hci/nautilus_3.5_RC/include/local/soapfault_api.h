/** @addtogroup base base
 *  @ingroup cmn
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010 Sony Corporation, All Rights Reserved.
 *
 */
#ifndef SOAPFAULT_API_H
#define SOAPFAULTI_API_H

NP_BEGIN_EXTERN_C

typedef np_faultdetail_t faultdetail_t;
typedef np_faultinfo_t   faultinfo_t;

extern void
npi_soapfault_FreeDetail( npi_cmncontext_t in_context, faultdetail_t *io_detail );

extern void
npi_soapfault_FreeInfo( npi_cmncontext_t in_context, faultinfo_t *io_fault );

NP_END_EXTERN_C

#endif /* SOAPFAULT_API_H */
