/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pod_cis.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pod_cis.h
 *  This header file contains cis definitions.
 */

#ifndef PODCI_CIS_H
#define PODCI_CIS_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// CIS tuple codes
#define CISTPL_LINKTARGET       0x13
#define CISTPL_DEVICE_OA        0x1D
#define CISTPL_DEVICE_OC        0x1C
#define CISTPL_VERS_1           0x15
#define CISTPL_MANFID           0x20
#define CISTPL_CONFIG           0x1A
#define CISTPL_CFTABLE_ENTRY    0x1B
#define CISTPL_NO_LINK          0x14
#define CISTPL_END              0xFF

// Module type
#define MODULE_TYPE_NONE        0x00
#define MODULE_TYPE_PCCARD      0x01
#define MODULE_TYPE_POD         0x03
#define MODULE_TYPE_DVB         0x02

// Version values for CISTPL_VERS_1
#define CIS_TPLLV1_MAJOR        0x05
#define CIS_TPLLV1_MINOR        0x00

/* Custom interface string, max size should be based on "NRSS_CI_V1.00" (14)
   Point of Deployment (POD) Module Initialization (SCTE DVS/221) Chapter A.2.6
   Practically it may be anything ("OpenCable_POD_Vx.xx" for MOTOROLA POD module */
#define CIS_STRING_SIZE         40



typedef enum _CI_error {
    CI_ERROR_REG_OPEN= -999,
    CI_ERROR_REG_WRITE,
    CI_ERROR_REG_READ_ADDRESS,
    CI_ERROR_REG_READ_DATA,
    CI_ERROR_NULL_POINTER,
    CI_ERROR_GPIO_OPEN,
    CI_ERROR_GPIO_SET,
    CI_ERROR_GPIO_CLEAR,
    CI_ERROR_GPIO_CLOSE,
    CI_ERROR_UNEXPECTED_APDU_TAG,
    CI_ERROR_UNEXPECTED_APDU_LENGTH,
    CI_ERROR_INVALID_APDU,
    CI_ERROR_RESERVED_APPLICATION_TYPE,
    CI_ERROR_OUT_OF_MEMORY,
    CI_ERROR_TASK_CREATE,
    CI_ERROR_QUEUE_CREATE,
    /* pdriver errors */
    CI_ERROR_ILLEGAL_SLOT_ID,
    CI_ERROR_SLOT_NOT_OPEN,
    CI_ERROR_NO_FREE_SLOTS,
    CI_ERROR_INVALID_IO_REGISTER,
    CI_ERROR_ILLEGAL_SIGNAL,
    CI_ERROR_ILLEGAL_COMMAND,
    /* PHYSDRV.c */
    CI_ERROR_MODULE_BUSY,
    CI_ERROR_MODULE_HARDWARE_BUSY,
    CI_ERROR_INVALID_CIS,
    CI_ERROR_MODULE_NOT_PRESENT,
    CI_ERROR_RECEIVE_DATA_TIMEOUT,
    CI_ERROR_SEND_DATA_TIMEOUT,
    CI_ERROR_SLOT_IDLE,
    CI_ERROR_INVALID_WE_STATUS,
    CI_ERROR_MODULE_HAS_NO_DATA,
    CI_ERROR_ILLEGAL_DATA_SIZE,
    CI_ERROR_DRV_READ_FAILURE,
    /* Resource Layer */
    CI_ERROR_QUEUE_SEND,
    CI_ERROR_INVALID_RESOURCE_ID,
    CI_ERROR_INVALID_CAS_SERVICE,
    CI_ERROR_UNEXPECTED_CHOICE_NB,
    CI_ERROR_NO_APPLICATION,
    CI_ERROR_INVALID_SLOT,
    CI_ERROR_INVALID_SESSION_ID,
    CI_ERROR_INVALID_ANSWER,
    CI_ERROR_MMI_UNAVAILABLE,
    CI_ERROR_MAX_SERVICE_EXCEEDED,
    CI_FAILURE = 0,     /* This value if as per the ST stack for CI */
    CI_SUCCESS = 1      /* This value if as per the ST stack for CI */
} CI_error_t ;
typedef enum _cis_tulple {
	CISTPL_NULL             = 0x00,
	CISTPL_DEVICE           = 0x01,
	CISTPL_LONGLINK_CB      = 0x02,
	CISTPL_INDIRECT         = 0x03,
	CISTPL_CONFIG_CB        = 0x04,
	CISTPL_CFTABLE_ENTRY_CB = 0x05,
	CISTPL_LONGLINK_MFC     = 0x06,
	CISTPL_BAR              = 0x07,
	CISTPL_PWR_MGMNT        = 0x08,
	CISTPL_EXTDEVICE        = 0x09,
    CISTPL_CHECKSUM         = 0x10,
    CISTPL_LONGLINK_A       = 0x11,
    CISTPL_LONGLINK_C       = 0x12,
//    CISTPL_LINKTARGET       = 0x13,
//    CISTPL_NO_LINK          = 0x14,
//    CISTPL_VERS_1           = 0x15,
    CISTPL_ALTSTR           = 0x16,
    CISTPL_DEVICE_A         = 0x17,
    CISTPL_JEDEC_C          = 0x18,
    CISTPL_JEDEC_A          = 0x19,
//    CISTPL_CONFIG           = 0x1A,
//    CISTPL_CFTABLE_ENTRY    = 0x1B,
//    CISTPL_DEVICE_OC        = 0x1C,
//    CISTPL_DEVICE_OA        = 0x1D,
    CISTPL_DEVICEGEO        = 0x1E,
    CISTPL_DEVICEGEO_A      = 0x1F,
//    CISTPL_MANFID           = 0x20,
    CISTPL_FUNCID           = 0x21,
    CISTPL_FUNCE            = 0x22,
    CISTPL_SWIL             = 0x23,
} cis_tuple_t;
struct dvb_entry_bundle
{
    int             Is_TPCE_INDX_Ok;        /* Intface + Default bits in TPCE_INDX if first Entry */
    int             Is_TPCE_IF;             /* If TRUE, the field TPCE_IF is present */
    int             Is_TPCE_IF_Ok;          /* "Errata in EN 50221 from CENELEC..." TPCE_IF imposed. */
    int             IsMasked_TPCE_FS_Ok;    /* TPCE_FS (Features) must include Power + IO + IRQ */
    int             Is_TPCE_PD_Ok;          /* There is at least one Description structure for Vcc */
    int             Is_TPCE_IO_Ok;          /* "Errata in EN 50221 from CENELEC..." TPCE_IO imposed. */
    int             Is_TPCE_IRQ_Ok;         /* "Errata in EN 50221 from CENELEC..." TPCE_IR imposed. */
    int             Is_STCE_EV_Ok;          /* Subtuple of CISTPL_CFTABLE_ENTRY contains "DVB_HOST" */
    int             Is_STCE_PD_Ok;          /* Subtuple of CISTPL_CFTABLE_ENTRY contains "DVB_CI_MODULE" */
    unsigned char   EntryNumb;              /* Read value of Configuration Entry Number in TPCE_INDX */
    unsigned char   TPCE_IF_Value;          /* Read value of TPCE_IF field. */
};
/* physical state */
#define DRV_IDLE		1
#define DRV_CONNECT		2
#define	DRV_DISCONNECT	3
/* subtuples */
#define	CCST_CIF				0xc0
#define	STCE_EV					0xc0
#define	STCE_PD					0xc1
/* offset in tuple parameters (without the 2 first bytes tuple and link) */
/* CISTPL_VERS_1 */
#define TPLLV1_MAJOR		0
#define TPLLV1_MINOR		1
/* CISTPL_CONFIG */
#define TPCC_SZ				0
#define TPCC_LAST			1
#define TPCC_RADR			2
/* CISTPL_CFTABLE_ENTRY */
#define TPCE_INDX			0
#define TPCE_IF				1				/* If field present ! */
#define TPCE_FS				2				/* If TPCE_IF field present, else 1 */
#define TPCE_PD				3				/* If TPCE_IF field present, else 2 */
/* bit mask */
/* CISTPL_CONFIG */
#define TPCC_RASZ			0x03
#define TPCC_RMSZ			0x3c
#define TPCC_LST_MSK        0x3f
/* CISTPL_CFTABLE_ENTRY */
#define TPCE_INDX_MASK		0xc0
#define TPCE_INTFACE		0x80
#define TPCE_IF_DVB_VAL		0x04			/* Imposed in CENELEC Errata document. */
#define TPCE_FS_DVB_MASK	0x1b			/* Previously TPCE_FS_MASK set to 0x0b wrong */
#define TPCE_FS_IO_MASK		0x08
#define TPCE_FS_IO_IRQ_MASK	0x18
#define TPCE_FS_PD_MASK		0x03			/* Previously TPCE_PD_MASK set to 0x01 wrong */
#define TPCE_EXT			0x80
#define TPCE_TIMING			0x04
#define TPCE_TIMING_WAIT	0x03
#define TPCE_TIMING_READY	0x1c
#define TPCE_TIMING_RESERV	0xe0
#define TPCE_IO				0x08
#define TPCE_IO_DVB_VAL		0x22			/* Imposed in CENELEC Errata document. */
#define TPCE_IO_RANGE		0x80
#define TPCE_IO_SLN			0xc0
#define TPCE_IO_SLNR		0x06
#define TPCE_IO_SAN			0x30
#define TPCE_IO_SANR		0x04
#define TPCE_IRQ			0x10
#define TPCE_IRQ_MASK		0x10
#define TPCE_IR_DVB_VAL		0x20			/* Imposed in CENELEC Errata document. */
#define TPCE_MEM			0x60
#define TPCE_MEM_TWO_BYTES	0x20
#define TPCE_MEM_FOUR_BYTES	0x40
#define TPCE_MEM_DESCRIPTOR	0x60
#define TPCE_MEM_SL			0x18
#define TPCE_MEM_SLR		0x03
#define TPCE_MEM_SCA		0x60
#define TPCE_MEM_SCAR		0x05
#define TPCE_MEM_HPA		0x80
#define TPCE_MISC			0x80
/* value */
/* CISTPL_VERS_1 */
#define TPLLV1_MAJOR_VAL	0x05
#define TPLLV1_MINOR_VAL	0x00
/* CISTPL_CONFIG */
#define TPCC_RMSK			0x01
#define NBRBYTES			0x01
#define MAXADDR				0xffe
/* CISTPL_CFTABLE_ENTRY */
#define TPCE_IF_VAL			0x04
/* CISTPL_NOLINK */
#define	CISTPL_NOLINKLEN	0x00
/* CCST_CIF */
#define CCST_CIFLEN			0x0e
#define CCST_CIF1			0x41
#define CCST_CIF2			0x02
#define	STCI_STR			"DVB_CI_V"
#define	STCI_STRP			0x2e
/* STCE_EV */
#define STCE_EV_VAL			"DVB_HOST"
/* STCE_PD */
#define STCE_PD_VAL			"DVB_CI_MODULE"
#define isdigit         ISDIGIT
#define ISDIGIT(c)      ((c)>='0' && (c)<='9')


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// Module configuration structure
typedef struct
{
    UINT8     u1IntFace;
    UINT8     u1Io;
    UINT8     u1Power;
    UINT8     u1Irq;
    BOOL      fgValid;
}   MODULE_CONFIG_T;

/// Power description parameter
typedef struct
{
    UINT8 u1Mantissa;
    UINT8 u1Exponent;
}   CIS_POWER_PARAM_T;

/// Power description structure
typedef struct
{
    CIS_POWER_PARAM_T rNomV;
    CIS_POWER_PARAM_T rMinV;
    CIS_POWER_PARAM_T rMaxV;
    CIS_POWER_PARAM_T rStaticI;
    CIS_POWER_PARAM_T rAvgI;
    CIS_POWER_PARAM_T rPeakI;
    CIS_POWER_PARAM_T rPDwnI;
}   CIS_POWER_DESC_T;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN INT32 PODCI_GetCis(POD_IO_STATE_T eIoState, UINT8* pu1Cis, UINT32* pu4CisLen);
EXTERN INT32 PODCI_ParseCis(UINT8* pu1CisMap, CIS_INFO_T*  prInfo);
EXTERN INT32 PODCI_WriteCor(UINT16 u2CorAddress, UINT8 u1CorValue);
EXTERN INT32 PODCI_ReadCor(UINT16 u2CorAddress);
EXTERN INT32 PODCI_CheckCIS(UINT8* pu1Cis, UINT16 *addrcor, UINT8 *cor);

#endif	/// PODCI_CIS_H

