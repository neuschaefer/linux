/** @addtogroup npbb npbb
 *  @ingroup api
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2008,2009,2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef NPBB_TYPES_H
#define NPBB_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/
/** @file
 * @brief Nautilus Library Types (BB)
 */
/*===========================================================================*/

/*---------------------------------------------------------------------------*
 * constant definition
 *---------------------------------------------------------------------------*/

/** BB protocol types */
enum {
    NPBB_PROTOCOLTYPE_MINVALUE = 2,
    NPBB_PROTOCOLTYPE_REGISTRATIONNODE = 2, /** Node registration protocol    */
    NPBB_PROTOCOLTYPE_REGISTRATIONLINK,     /** License registration protocol */
    NPBB_PROTOCOLTYPE_LICENSEACQUISITION,   /** License acquisition protocol  */
    NPBB_PROTOCOLTYPE_DEREGISTRATION,       /** Deregistration protocol       */
    NPBB_PROTOCOLTYPE_DATACERTIFICATION,    /** Data Certification protocol   */
    NPBB_PROTOCOLTYPE_DATAUPDATE,           /** Data Update protocol          */
    NPBB_PROTOCOLTYPE_LTP,
    NPBB_PROTOCOLTYPE_MAXVALUE
};

#define NPBB_DUS_UPDATE_TT        0x01
#define NPBB_DUS_UPDATE_LSL       0x02

/* Same values as NEMO_PROTOCOL_STEPTYPE_... in nemo_api.h */
#define NPBB_PROTOCOL_STEPTYPE_REQUEST        (0x0001)
#define NPBB_PROTOCOL_STEPTYPE_RESPONSE       (0x0002)

#ifdef __cplusplus
}
#endif

#endif /* NPBB_TYPES_H */
/** @} */
