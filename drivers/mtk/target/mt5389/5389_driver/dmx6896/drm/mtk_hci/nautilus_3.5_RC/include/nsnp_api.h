/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2006,2007,2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

/** 
 * @addtogroup nsnp nsnp
 * @ingroup nsec
 * @{
 * @nsdepends
 * basic_types.h, npcommon_types.h and np_transportlayer.h
 *
 * @brief
 * The <i>nsnp</i> layer provides all the necessary functionality for integrating Marlin DRM into your application.
 *
 * @file
 * Basic Nautilus API layer for DRM, Transfer, Acquisition and registration
 *
 * @see API.reference.pdf

@note
While it is possible to perform Node, Link and license acquisition, as well as license
transfer, via the nsnp layer <b>it is not advised to do so.</b>. 
@note
The operations involved can be complicated. It is recommended to use the higher layer
@ref dorado and @ref pavo modules for these operations.
 */


#ifndef NSNP_API_H
#define NSNP_API_H

#include "nsnp_common.h"
#include "nsnp_bb.h"
#include "nsnp_drm.h"
#include "nsnp_datapath.h"
#include "nsnp_ltp.h"
#include "nsnp_omln.h"

#endif /* NSNP_API_H */
/** @} */
