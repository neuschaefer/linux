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
 * $RCSfile: pod_cis.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pod_cis.c
 *  This file implements cis.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_926.h"
#include "x_pinmux.h"
#include "x_assert.h"
LINT_EXT_HEADER_END

#include "../pcmcia_if.h"
#include "../pcmcia_ctrl.h"
#include "../pcmcia_hw.h"

#include "podci_ctrl.h"
#include "podci_cis.h"

#ifdef PCMCIA_ENABLED

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Macro used for CIS get operation
#define CONTINUE_GET_CIS(offset, size)  ((offset < size) ? 1 : 0)


#ifndef CC_MTK_LOADER

static UINT8 ui1CiplusCompId = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _PODCI_PHYS_case_string (CHAR*  ps_str_io,
                                       UINT32       ui4_len,
                                       BOOL         b_to_lower);

static UINT8 _PODCI_GetCisTuple(UINT8*   pu1CurTuple,
                          UINT8**  ppu1TupleData,
                          UINT8*   pu1TupleSize)
{
    ASSERT((pu1CurTuple != NULL) && (ppu1TupleData != NULL) &&
           (pu1TupleSize != NULL));

    *pu1TupleSize = pu1CurTuple[1];
    *ppu1TupleData = &pu1CurTuple[2];
    return pu1CurTuple[0];
}

static BOOL _PODCI_CisTupleVersion(UINT8* pu1TupleData)
{
	UINT8*  pu1InfoString;
    INT32   i4Val;
    INT32   i4Set = 1; /* Default OOB gated clock control = rising-edge */
    INT32   i4Gpio;

    ASSERT(pu1TupleData != NULL);

	/* Check major and minor version number */
	if ((pu1TupleData[0] != CIS_TPLLV1_MAJOR) ||
	    (pu1TupleData[1] != CIS_TPLLV1_MINOR))
	{
		LOG(1, "_PODCI_CisTupleVersion(): a problem in major or minor verison checking\n");
		return FALSE;
	}
	else
	{
		/* Browse information strings */
		pu1InfoString = &pu1TupleData[2];
		while (*pu1InfoString != CISTPL_END)
		{
			while (*pu1InfoString != 0)
			{
			    pu1InfoString++;
		    }
			pu1InfoString++;
		}

        /* Resolve NDS falling-edge OOB gated clock issue */
        /* Temporary solution => should be removed after MT5112 ECO for this issue */
		if (x_strcmp((const CHAR *)(&pu1TupleData[2]), "SCM") == 0)
		{
		    i4Set = 0; /* OOB gated clock control = falling-edge */
		}

        /* Config GPIO 29 for OOB gated clock control (0 = falling-edge, 1 = rising-edge) */
        /* Change to input mode first */
        i4Gpio = 29;
//        UNUSED(BSP_PinGpioSel(i4Gpio));

        /* Set OOB gated clock control */
//        UNUSED(GPIO_Output(i4Gpio, &i4Set));

        /* Change to output mode */
        i4Val = 1;
//        UNUSED(GPIO_Enable(i4Gpio, &i4Val));

		return TRUE;
	}
}

static void _PODCI_GetPowerParam(UINT8**            ppu1TupleData,
                           CIS_POWER_PARAM_T* prPowerParam)
{
    ASSERT(ppu1TupleData != NULL);

    /* Get power definition mantissa and exponent */
	if (prPowerParam != NULL)
	{
		prPowerParam->u1Mantissa = (**ppu1TupleData & (BIT6 + BIT5 + BIT4 + BIT3)) >> 3;
		prPowerParam->u1Exponent = (**ppu1TupleData & (BIT2 + BIT1 + BIT0));
	}

	/* Skip power definition extension */
	do
	{
		(*ppu1TupleData)++;
	}
	while (**ppu1TupleData & BIT7); /* The final extension byte contains a 0 in the EXT field */
}

static void _PODCI_GerPowerDesc(UINT8**              ppu1TupleData,
                          CIS_POWER_DESC_T*    prPowerDesc)
{
	UINT8   u1ParamSelectionByte;

    ASSERT(ppu1TupleData != NULL);

	/* Init parameter description structure */
	if (prPowerDesc != NULL)
	{
		prPowerDesc->rNomV.u1Mantissa = 0x00;
		prPowerDesc->rNomV.u1Exponent = 0x00;
		prPowerDesc->rMinV.u1Mantissa = 0x00;
		prPowerDesc->rMinV.u1Exponent = 0x00;
		prPowerDesc->rMaxV.u1Mantissa = 0x00;
		prPowerDesc->rMaxV.u1Exponent = 0x00;
		prPowerDesc->rStaticI.u1Mantissa = 0x00;
		prPowerDesc->rStaticI.u1Exponent = 0x00;
		prPowerDesc->rAvgI.u1Mantissa = 0x00;
		prPowerDesc->rAvgI.u1Exponent = 0x00;
		prPowerDesc->rPeakI.u1Mantissa = 0x00;
		prPowerDesc->rPeakI.u1Exponent = 0x00;
		prPowerDesc->rPDwnI.u1Mantissa = 0x00;
		prPowerDesc->rPDwnI.u1Exponent = 0x00;
	}

	/* Parameter selection byte */
	/* Each bit in the parameter selection byte indicates whether or not the corresponding
	   parameter is described by parameter definitions which follow that byte */
	u1ParamSelectionByte = **ppu1TupleData;
	(*ppu1TupleData)++;

	/* Nominal operating supply voltage */
	/* In the absence of other information the nominal operating voltage has a tolerance of +5/-5% */
	if (u1ParamSelectionByte & BIT0)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rNomV));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* Minimum operating supply voltage */
	if (u1ParamSelectionByte & BIT1)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rMinV));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* Maximum operating supply voltage */
	if (u1ParamSelectionByte & BIT2)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rMaxV));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* Continuous supply current required */
	if (u1ParamSelectionByte & BIT3)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rStaticI));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* Maximum current required averaged over 1 second */
	if (u1ParamSelectionByte & BIT4)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rAvgI));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* Peak I maximum current required averaged over 10 milliseconds */
	if (u1ParamSelectionByte & BIT5)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rPeakI));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* PDwn I power-down supply current required */
	if (u1ParamSelectionByte & BIT6)
	{
		if (prPowerDesc != NULL)
		{
			_PODCI_GetPowerParam(ppu1TupleData, &(prPowerDesc->rPDwnI));
		}
		else
		{
			_PODCI_GetPowerParam(ppu1TupleData, NULL);
		}
	}

	/* RFU reserved for future standardization */
	if (u1ParamSelectionByte & BIT7)
	{
		_PODCI_GetPowerParam(ppu1TupleData, NULL);
	}
}

static BOOL _PODCI_CisTupleConfig(UINT8*     pu1TupleData,
                            UINT8      u1TupleSize,
                            UINT16*    pu2CorAddr,
                            BOOL*      pfgPodModuleType)
{
  	BOOL    fgReturn = TRUE;
	UINT8   u1TPCC_RMSize; /* Number of bytes (mninus 1) in the Configuration Register presence mask field */
	UINT8   u1TPCC_RASize; /* Number of bytes (minus 1) in the Configuration Registers Base Address
					          in Attribute Memory Space field.
					          The address should not be greater than 0xFFE, so bTPCC_RASize <= 2 */
    UINT8   u1TPCC_Last = 0; /* Index of the last entry in the Card Configuration Table */
	UINT8*  pu1SubTuple;

    UNUSED(u1TPCC_Last);

    ASSERT((pu1TupleData != NULL) && (pu2CorAddr != NULL) &&
           (pu2CorAddr != NULL) && (pfgPodModuleType != NULL));

	u1TPCC_RASize = (UINT8)((pu1TupleData[0] & (BIT1 | BIT0)) + 1);
	u1TPCC_RMSize = (UINT8)(((pu1TupleData[0] & (BIT5 | BIT4 | BIT3 | BIT2)) >> 2) + 1) ;
	u1TPCC_Last = (pu1TupleData[1] & (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0));

	/* Configuration Registers Base Address in attribute memory
	   1 to 4 bytes as indicated by the TPCC_RASZ field
	   POD module spec indicates that this address should no be greater	than 0x0FFE, u1I.e. 2 bytes max */
	*pu2CorAddr = (UINT16)pu1TupleData[2];
	if (u1TPCC_RASize > 1)
	{
		*pu2CorAddr += (((UINT16)pu1TupleData[3]) << 8);
    }

	/* Configuration Registers Present Mask. 1 to 16 bytes as indicated by the TPCC_RMSZ field */
	/* Ignored? */

	/* There should be only one subtuple: CISTPL_CIF */
	if ((1 + 1 + u1TPCC_RASize + u1TPCC_RMSize) >= u1TupleSize)
	{
		fgReturn = FALSE;
	}
	else
	{
		pu1SubTuple = &pu1TupleData[1 + 1 + u1TPCC_RASize + u1TPCC_RMSize];

		/* CISTPL_CIF: Custom interface subtuple */
		if (pu1SubTuple[0] == 0xC0)
		{
			/* Subtuple size should not extend beyond parent tuple */
			if ((pu1SubTuple[1] + 1 + 1 + u1TPCC_RASize + u1TPCC_RMSize) > u1TupleSize)
			{
				fgReturn = FALSE;
			}
			else
			{
				/* Interface id number should be either 0x0241 (DVB) or 0x0341 (POD) */
				if (pu1SubTuple[2] != 0x41)
				{
					fgReturn = FALSE;
				}
				else
				{
					switch (pu1SubTuple[3])
					{
    					case MODULE_TYPE_POD:  /* POD module id */
    						*pfgPodModuleType = TRUE;
    						break;

    					case MODULE_TYPE_DVB:  /* DVB module id */
    						*pfgPodModuleType = FALSE;
    						break;

    					default: /* Unknown module id */
    						fgReturn = FALSE;
					}
				}
			}
		}
	}
	return fgReturn;
}

static BOOL _PODCI_CisTupleConfigTableEntry(UINT8*             pu1TupleData,
                                      UINT8              pu1TupleSize,
                                      MODULE_CONFIG_T*   prConfig,
                                      UINT8*             pu1CorValue,
                                      BOOL*              pfgChangeVpp)

{
	UINT8               u1Features;
	UINT8               u1AddrRangeNum;
	UINT8               u1AddrSize;
	UINT8               u1LenSize;
	CIS_POWER_DESC_T    rPowerDesc;

    ASSERT((pu1TupleData != NULL) && (prConfig != NULL) &&
           (pu1CorValue != NULL) && (pfgChangeVpp != NULL));

    UNUSED(pu1TupleSize);

	/* This function will check if an entry matches DVB or POD configuration and
	   retrieve the	associated configuration entry number */

	/* A configuration entry can have a "default" flag which indicates that this
	   entry provides default values for succeeding entries until another entry
	   is encountered with its Default bit set.
	   So, every "default" entry conditions should be stored to be used for
	   following non-default entries */

	/* TPCE_INDX : configuration table index byte */
	/* TPCE_INDX: contains the value to be written to the COR (only bit 0 to 5 valid) */
	*pu1CorValue = (*pu1TupleData) & (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

	/* TPCE_INDX: reset previous config if the "default" config flag is set */
	if (*pu1TupleData & BIT7)
	{
		prConfig->u1IntFace = 0;
		prConfig->u1Io      = 0;
		prConfig->u1Power   = 0;
		prConfig->u1Irq     = 0;
	}

	pu1TupleData++;

	/* TPCE_IF: interface description field */
	if (*(pu1TupleData-1) & BIT6)
	{
		prConfig->u1IntFace = *pu1TupleData;
		pu1TupleData++;
	}

	/* TPCE_FS: feature selection byte */
	u1Features = *pu1TupleData;
	pu1TupleData++;

	/* TPCE_PD: power description structure */
	*pfgChangeVpp = FALSE; /* Assume VPP requested value 5V is not needed */
	if (u1Features & (BIT1 | BIT0))
	{
		/* Number of power descritpion fields:
		   0 -> No power-description structures, use the default
		   1 -> VCC power-description-structure only
		   2 -> VCC and VPP (VPP1 = VPP2) power-description-structures
		   3 -> VCC, VPP1 and VPP2 power-description-structures */
		prConfig->u1Power = u1Features & (BIT1 | BIT0);

		/* Power description structure parameters:
		   Each parameters (NomV, AvgI, etc.) contains a value (mantissa + exponent)
		   and optional extensions. Extensions are not taken in consideration,
		   value is coded according to PC card standard volume 4 metaformat specification
		   Chapter "3.3.2.4 TPCE_PD: Power Description Structure" */
		if (prConfig->u1Power > 0) /* Power-description structures exist */
		{
			/* Vcc power description */
			_PODCI_GerPowerDesc(&pu1TupleData, NULL);

			if (prConfig->u1Power > 1)
			{
				/* Get VPP1 power description */
				_PODCI_GerPowerDesc(&pu1TupleData, &rPowerDesc);

				/* Test if VPP required nominal voltage is 5V */
				if ((rPowerDesc.rNomV.u1Mantissa == 0x0A) &&
				    (rPowerDesc.rNomV.u1Exponent == 0x05))
                {
					*pfgChangeVpp = TRUE;
                }

				if (prConfig->u1Power > 2)
				{
					/* Get VPP2 power description */
					_PODCI_GerPowerDesc(&pu1TupleData, NULL);
				}
			}
		}
	}

	/* TPCE_TD: configuration tming information */
	if (u1Features & BIT2)
	{
		pu1TupleData++;
	}

	/* TPCE_IO: I/O space addresses required for this configuration */
	if (u1Features & BIT3)
	{
		/* I/O space description byte */
		prConfig->u1Io = *pu1TupleData;
		pu1TupleData++;

		/* Range flag: 0 -> no range, 1 -> presence of I/O addresses range */
		if (prConfig->u1Io & BIT7)
		{
			/* I/O Range Descriptor byte */

			/* Number of I/O address ranges :
			   This field specifies how many I/O Address Range Description
			   fields will follow the I/O Range Descriptor byte */
			u1AddrRangeNum = (UINT8)((*pu1TupleData & (BIT3 | BIT2 | BIT1 | BIT0)) + 1);

			/* Size of Address:
			   This field specifies how many bytes will be used to encode the
			   start of I/O Address block in each of the I/O range description
			   fields that follow
			   0 -> 0 byte
			   1 -> 1 byte
			   2 -> 2 bytes
			   3 -> 4 bytes */
			u1AddrSize = (UINT8)((*pu1TupleData & (BIT5 | BIT4)) >> 4);
			if (u1AddrSize == 0x03)
			{
				u1AddrSize = 0x04;
			}

			/* Size of length:
			   This field specifies how many bytes will be used to encode the
			   length of I/O address block in each of the I/O range	description
			   fields that follow
			   0 -> 0 byte
			   1 -> 1 byte
			   2 -> 2 bytes
			   3 -> 4 bytes */
			u1LenSize = (*pu1TupleData & (BIT7 | BIT6)) >> 6;
			if (u1LenSize == 0x03)
			{
				u1LenSize = 0x04;
			}

			/* Skipping I/O address ranges */
			pu1TupleData += (u1AddrRangeNum * (u1AddrSize + u1LenSize)) + 1;
		}
	}

	/* TPCE_IR: interrupt request description structure */
	if (u1Features & BIT4)
	{
		prConfig->u1Irq = *pu1TupleData;
		if (*pu1TupleData & BIT4)
		{
		    pu1TupleData += 2;
        }
		pu1TupleData++;
	}

	/* TPCE_MS: Memory Space Description Structure */

	/* Don't care */

	/* TPCE_MI: Miscellaneous Features Field */

	/* Don't care */

	/* The "ERRATA for EN50221" document (CIT057.doc) specifies that a valid DVB
	configuration has an IRQ descrition byte value equal to 0x20. This verification
	is not performed here since CA module cards tested do not specifies a valid IRQ.
	Other values are correctly tested */

	/* Check that values are correct */
	if ((prConfig->u1IntFace == 0x04) &&
		(prConfig->u1Io == 0x22) &&
		(prConfig->u1Power != 0x00))
	{
		LOG(1, "_PODCI_CisTupleConfigTableEntry(): POD-DVB compliant\n");
		prConfig->fgValid = TRUE;
	}
	else
	{
		LOG(1, "_PODCI_CisTupleConfigTableEntry(): not POD-DVB compliant\n");
		prConfig->fgValid = FALSE;
	}

	/* STCE_EV: environment descriptor subtuple */

    /* Don't care */

	/* STCE_PD: physical device name subtuple */

    /* Don't care */

	return TRUE;
}


/* ======================================================================== */
/* NAME : PHYS_Tuple_CISTPL_CFTABLE_ENTRY                                   */
/*                                                                          */
/* DESCRIPTION : Parses CISTPL_CFTABLE_ENTRY tuples data, from current CIS  */
/*                                                                          */
/* INPUT PARAMETERS :                                                       */
/*      pcis :  pointer to the tuple to check                               */
/*      first:  1 for the first CISTPL_CFTABLE_ENTRY tuple, else 0          */
/*      pentry: pointer on a private structure used to collect DVB info     */
/*                                                                          */
/* OUTPUT PARAMETERS :                                                      */
/*      first: 0 when an new entry is OK                                    */
/*      pentry: pointer on a private structure used to collect DVB info     */
/*                                                                          */
/* RETURN CODE                                                              */
/*         1 : OK this table entry is a DVB full compliant one.             */
/*         0 : accepted DVB table entry (but not compliant).                */
/*        -1 : A non-DVB table entry to skip (irrelevant for us).           */
/*                                                                          */
/* ======================================================================== */
int _PODCI_PHYS_Tuple_CISTPL_CFTABLE_ENTRY( unsigned char             * pcis,
                                     int                         IsFirst,
                                     struct   dvb_entry_bundle * pentry )
{
    unsigned char * pcheck;
    int             length = (int) *(pcis+1);
    unsigned char   Features, ParamSelect, SubTuple;
    int             Result, SubLen;
    pcis   += 2;
    /* Make pcheck point to the field TPCE_INDX, remaining length decrease of one byte. */
    pcheck  = pcis + TPCE_INDX;
    length -= 1;
    /* Intface and Default bits must be set in field TPCE_INDX of first table entry */
    if( IsFirst  &&  (*pcheck & TPCE_INDX_MASK) != TPCE_INDX_MASK )
    {
        /* Wrong checking on first CISTPL_CFTABLE_ENTRY tuple, but continue the scan, simply register the failure... */
        pentry->Is_TPCE_INDX_Ok = 0;
        LOG(7, "CFTABLE_ENTRY: TPCE_INDX=%x Intface+Default not compliant\n", *pcheck );
    }
    else                                            /* Even if not first ! */
    {
        pentry->Is_TPCE_INDX_Ok = 1;
        LOG(7, "CFTABLE_ENTRY: TPCE_INDX=%x Intface+Default OK\n", *pcheck );
    }
    /* Dealing with the field TPCE_INDX. */
    pentry->Is_TPCE_IF = (*pcheck & TPCE_INTFACE) ? 1:0;    /* If TRUE, the field TPCE_IF is present */
    pentry->EntryNumb  =  *pcheck & ~TPCE_INDX_MASK;        /* Configuration Entry Number to write into the COR */
    LOG(7, "CFTABLE_ENTRY: Entry Number = %d\n", pentry->EntryNumb );
    /* Dealing with the field TPCE_IF if any (New: its value is imposed) */
    pentry->Is_TPCE_IF_Ok = 0;                              /* Preset to FALSE */
    if( pentry->Is_TPCE_IF )
    {
        pcheck  = pcis + TPCE_IF;
        length -= 1;
        pentry->TPCE_IF_Value = *pcheck;                    /* Memorize IF field in case of DVB entry */
        /* According to "Errata in EN 50221 and the CENELEC ..." IF value is determined. */
        if( pentry->TPCE_IF_Value == TPCE_IF_DVB_VAL )
        {
            pentry->Is_TPCE_IF_Ok = 1;
            LOG(7, "CFTABLE_ENTRY: TPCE_IF field = %x  OK\n", TPCE_IF_DVB_VAL );
        }
        else
        {
            LOG(7, "CFTABLE_ENTRY: TPCE_IF field = %x  WRONG\n", pentry->TPCE_IF_Value);
        }
    }                                                       /* case (see below) it must be equal to TPCE_IF_VAL */
    /* Dealing with the field TPCE_FS which offset depends on the previous field TPCE_IF presence. */
    pcheck  = pcis + ((pentry->Is_TPCE_IF) ? TPCE_FS : (TPCE_FS - 1));
    length -= 1;
    /* Memorize the Feature Selection byte value. */
    Features = *pcheck;
    if( (Features & TPCE_FS_PD_MASK)     != 0  &&
        (Features & TPCE_FS_IO_MASK) == TPCE_FS_IO_MASK ) /* KATTA: Original Code (Features & TPCE_FS_IO_IRQ_MASK) == TPCE_FS_IO_IRQ_MASK ) */
    {
        pentry->IsMasked_TPCE_FS_Ok = 1;
        LOG(7,"CFTABLE_ENTRY: TPCE_FS = %x  OK\n", Features );
    }
    else
    {
        pentry->IsMasked_TPCE_FS_Ok = 0;
        LOG(7, "CFTABLE_ENTRY: TPCE_FS = %x  WRONG\n", Features );
    }
    /* Make pcheck point on the byte following TPCE_FS field (maybe 1st Power Description structures) */
    pcheck = pcis + ((pentry->Is_TPCE_IF) ? TPCE_PD : (TPCE_PD - 1));
    /* Two first LS bits to indicate following Power Description Structures. Compliant to DVB ? */
    pentry->Is_TPCE_PD_Ok = 0;                      /* Preset to 0. Set to 1 below if OK. */
    if( Features & TPCE_FS_PD_MASK )
    {
        int   PdStructNbBytes = 0;
        int   PdStructCount   = Features & TPCE_FS_PD_MASK;
        pentry->Is_TPCE_PD_Ok = 1;                  /* The client must check electrical values */
        while( PdStructCount-- )
        {
            unsigned char * pcheckpd;               /* To parse bytes inside each Power Description structure */
            int             BitPos;                 /* To parse bits inside each Param Selection Byte. */
            pcheckpd    = pcheck + PdStructNbBytes; /* Start pointing on Parameter Selection byte */
            ParamSelect = * pcheckpd;               /* Value of the Parameter Selection Byte. */
            ++PdStructNbBytes;                      /* For the Parameter Selection Byte itself. */
            for( BitPos = 0; BitPos < 8; BitPos++ )
            {
                /* No checking on stored electrical values now... The client is in charge of it. */
                if( ParamSelect & (1 << BitPos) )   /* Nom V, Min V, Max V, Static I, Avg I, Peak I, Pdwn I */
                {
                    do                              /* At least one byte for (Mantissa+Exponent) */
                    {
                        ++pcheckpd;
                        ++PdStructNbBytes;
                    } while( *pcheckpd & TPCE_EXT );
                }
            }
        }
        pcheck += PdStructNbBytes;                  /* PdStructNbBytes may be null here... */
        length -= PdStructNbBytes;
    }
    /* Third LS bit inside TPCE_FS field indicates Timing Information follows. */
    if( Features & TPCE_TIMING )
    {
        unsigned char * pcheckti       = pcheck;    /* Points on TPCE_TD field. */
        unsigned char   TiFieldValue   = * pcheckti;
        int             TiFieldNbBytes = 0;
        ++pcheckti;                                 /* Points now on WAIT, READY or RESERV value. */
        ++TiFieldNbBytes;                           /* For Timing Information field itself */
        if( (TiFieldValue & TPCE_TIMING_WAIT) != TPCE_TIMING_WAIT )/* WAIT value first */
        {
            while( *pcheckti & TPCE_EXT )           /* First bytes with TPCE_EXT bit set.  */
            {
                ++pcheckti;
                ++TiFieldNbBytes;
            }
            ++pcheckti;                             /* For last byte (maybe the first!)... */
            ++TiFieldNbBytes;                       /* ending with a TPCE_EXT bit zeroed.  */
        }
        if( (TiFieldValue & TPCE_TIMING_READY) != TPCE_TIMING_READY )       /* READY value */
        {
            while( *pcheckti & TPCE_EXT )           /* First bytes with TPCE_EXT bit set.  */
            {
                ++pcheckti;
                ++TiFieldNbBytes;
            }
            ++pcheckti;                             /* For last byte (maybe the first!)... */
            ++TiFieldNbBytes;                       /* ending with a TPCE_EXT bit zeroed.  */
        }
        if( (TiFieldValue & TPCE_TIMING_RESERV) != TPCE_TIMING_RESERV )     /* READY value */
        {
            while( *pcheckti & TPCE_EXT )           /* First bytes with TPCE_EXT bit set.  */
            {
                ++pcheckti;
                ++TiFieldNbBytes;
            }
            ++pcheckti;                             /* For last byte (maybe the first!)... */
            ++TiFieldNbBytes;                       /* ending with a TPCE_EXT bit zeroed.  */
        }
        pcheck += TiFieldNbBytes;                   /* Points now on next field first byte */
        length -= TiFieldNbBytes;
    }
    /* Fourth LS bit inside TPCE_FS field indicates IO Space Description follows. */
    pentry->Is_TPCE_IO_Ok = 0;                      /* Preset to FALSE */
    if( Features & TPCE_IO )
    {
        int   IOFieldNbBytes = 0;
        int   RangesCount, LengthSize, AddressSize;
        ++IOFieldNbBytes;                           /* For IO Space Descript field itself */
        /* Get ride of the DVB compliance indicator about IOs: "Errata in EN 50221..." imposes val */
        if( *pcheck == TPCE_IO_DVB_VAL )
        {
            pentry->Is_TPCE_IO_Ok = 1;
            LOG(7, "CFTABLE_ENTRY: TPCE_IO = %x ... OK\n", TPCE_IO_DVB_VAL );
        }
        else
        {
            LOG(7,"CFTABLE_ENTRY: TPCE_IO = %x ... WRONG\n", *pcheck );
        }
        if( *pcheck & TPCE_IO_RANGE )               /* Else only one byte for IOs. */
        {
            unsigned char IORange = * (pcheck + 1);
            ++IOFieldNbBytes;                       /* For IO Addr Range Desc byte itself */
            RangesCount = (IORange & 0x0f) + 1;     /* The standard specifies '- 1' encoding */
            LengthSize  = (IORange & TPCE_IO_SLN) >> TPCE_IO_SLNR;
            if( LengthSize == 3 )
                LengthSize = 4;
            AddressSize = (IORange & TPCE_IO_SAN) >> TPCE_IO_SANR;
            if( AddressSize == 3 )
                AddressSize = 4;
            IOFieldNbBytes += RangesCount * (LengthSize + AddressSize);
        }
        pcheck += IOFieldNbBytes;                   /* Points now on next field first byte */
        length -= IOFieldNbBytes;
    }
    /* Fifth LS bit inside TPCE_FS field indicates Interrupt Request Description follows. */
    pentry->Is_TPCE_IRQ_Ok = 0;                     /* Preset to FALSE */
    if( Features & TPCE_IRQ )
    {
        int   IRFieldNbBytes = 0;
        ++IRFieldNbBytes;                           /* For the first byte of the field (always) */
        if( *pcheck == TPCE_IR_DVB_VAL )
        {
            pentry->Is_TPCE_IRQ_Ok = 1;
        }
        if( *pcheck & TPCE_IRQ_MASK )               /* Else only one byte (this one) for IRQs. */
        {
            IRFieldNbBytes += 2;
        }
        pcheck += IRFieldNbBytes;
        length -= IRFieldNbBytes;
    }
    /* Sixth and seventh LS bits inside TPCE_FS field indicates Memory Space Description follows. */
    if( Features & TPCE_MEM )
    {
        int MEMFieldNbBytes;
        if( (Features & TPCE_MEM)  ==  TPCE_MEM_TWO_BYTES )
        {
            MEMFieldNbBytes = 2;
        }
        else if( (Features & TPCE_MEM)  ==  TPCE_MEM_FOUR_BYTES )
        {
            MEMFieldNbBytes = 4;
        }
        else                                        /* An array of window is present. */
        {
            unsigned char   MemSpaceDescr = *pcheck;
            int             WindowCount, LengthSize, AddressSize;
            WindowCount = (MemSpaceDescr & TPCE_MEM_SLR) + 1;
            LengthSize  = (MemSpaceDescr & TPCE_MEM_SL)  >> TPCE_MEM_SLR;
            AddressSize = (MemSpaceDescr & TPCE_MEM_SCA) >> TPCE_MEM_SCAR;
            /* Evaluate first the window array size in several steps. */
            MEMFieldNbBytes = LengthSize + AddressSize;
            if( MemSpaceDescr & TPCE_MEM_HPA )      /* A host Address field is present */
            {                                       /* of same size as Card Address.   */
                MEMFieldNbBytes += AddressSize;
            }
            MEMFieldNbBytes *= WindowCount;
            /* Here only there is a Memory Space Descriptor Byte (in first position). */
            ++MEMFieldNbBytes;
        }
        pcheck += MEMFieldNbBytes;
        length -= MEMFieldNbBytes;
    }
    /* Heighth LS bit inside TPCE_FS field indicates Miscellaneous Features Field follows. */
    if( Features & TPCE_MISC )
    {
        unsigned char * pcheckmi       = pcheck;/* Points on TPCE_MI field. */
        int             MiFieldNbBytes = 0;
        ++MiFieldNbBytes;                           /* At least one byte for the MISC field */
        while( *pcheckmi & TPCE_EXT )               /* Current byte with TPCE_EXT bit set ? */
        {
            ++pcheckmi;
            ++MiFieldNbBytes;                       /* Because next byte belongs to MISC field */
        }
        pcheck += MiFieldNbBytes;                   /* Points now on next field first byte */
        length -= MiFieldNbBytes;
    }
    pentry->Is_STCE_EV_Ok = 0;                      /* Preset last flags to 0 */
    pentry->Is_STCE_PD_Ok = 0;
    while( length > 0 )
    {
        SubTuple  = * pcheck;
        SubLen    = *(pcheck+1) + 2;
        if( SubTuple == STCE_EV  &&  !(x_strncmp( (char *)(pcheck+2), STCE_EV_VAL, x_strlen( STCE_EV_VAL ))))
        {
            pentry->Is_STCE_EV_Ok = 1;
        }
        else if( SubTuple == STCE_PD  &&  !(x_strncmp( (char *)(pcheck+2), STCE_PD_VAL, x_strlen( STCE_PD_VAL ))))
        {
            pentry->Is_STCE_PD_Ok = 1;
        }
        else
        {
            LOG(7, "CFTABLE_ENTRY: Invalid SubTuple\n" );
        }
        length   -= SubLen;
        pcheck   += SubLen;
    }
    if( pentry->Is_STCE_EV_Ok == 1  &&  pentry->Is_STCE_PD_Ok == 1 )
    {
        if( pentry->Is_TPCE_INDX_Ok     == 1  &&
            pentry->Is_TPCE_IF          == 1  &&  pentry->Is_TPCE_IF_Ok == 1  &&
            pentry->IsMasked_TPCE_FS_Ok == 1  &&
            pentry->Is_TPCE_PD_Ok       == 1  &&  pentry->Is_TPCE_IO_Ok == 1  &&  pentry->Is_TPCE_IRQ_Ok == 1 )
        {
            Result = 1;
            LOG(7,"CFTABLE_ENTRY: strings DVB_HOST and DVB_CI_MODULE found and ALL OK\n" );
        }
        else
        {
            Result = 0;
            LOG(7,"CFTABLE_ENTRY: strings DVB_HOST and DVB_CI_MODULE found but WRONG\n" );
        }
    }
    else
    {
        Result = -1;
    }
    return( Result );
}
/* ======================================================================== */
/* NAME : PHYS_Tuple_CISTPL_CONFIG                                          */
/*                                                                          */
/* DESCRIPTION : Parses CISTPL_CONFIG tuple data, from current CIS          */
/*                                                                          */
/* INPUT PARAMETERS :  pcis (unsigned char *) Pointer to the start of the   */
/*                     tuple of type CISTPL_CONFIG.                         */
/*                                                                          */
/* OUTPUT PARAMETERS : TpccRadr (unsigned char **) The base Address of the  */
/*                     Configuration Registers.                             */
/*                                                                          */
/* RETURN CODE :   The count of bytes read after the tuple_code plus the    */
/*                 tuple_link (tested by the caller equal to tuple_link)    */
/*                                                                          */
/* REMARK :             As defined in document "Errata in EN 50221",        */
/*                 - the "Configuration Register Base Address" shall not be */
/*                 greater than FFEh                                        */
/*                 - there shall be one TPCC_SBTPL field containing one sub */
/*                 tuple of type CCST_CIF starting with a 2 bytes value of  */
/*                 0x0241 (for the Interface ID Number) then a string that  */
/*                 matches the format DVB_CI_Vx.xx where x.xx preferably is */
/*                 1.00 The end of the sub-tuple may be 0xFF.               */
/*                                                                          */
/* ======================================================================== */
static int _PODCI_PHYS_Tuple_CISTPL_CONFIG( unsigned char  * pcis,
                                     unsigned char    ucLink,
                                     int            * piLastEntry,
                                     int            * TpccRadr )
{
    unsigned char ucSubLink;
    unsigned char ucRead, ucNbStr;
    unsigned char * ucCursor;
    unsigned char ucTpccSz, ucTpccRasz, ucTpccRmsz;
    int           iFound = 0;
    
    /* Get the global size field then the three components inside of it. */
    ucCursor   = pcis + TPCC_SZ;
    ucTpccSz   = * ucCursor;
    ucTpccRasz = ucTpccSz & TPCC_RASZ;
    ucTpccRmsz = ucTpccSz & TPCC_RMSZ;
    if( ucTpccRasz > NBRBYTES )         /* Max equal to 1 because 2 chars enough for Addr < FFEh */
    {
        LOG(7,"CISTPL_CONFIG: Error, TPCC_RASZ field greater than 1...\n" );
    }
    else
    {
        iFound = 1;
    }
    /* Get the Index of the last entry in the Card Configuration Table. */
    ucCursor      = pcis + TPCC_LAST;
    * piLastEntry = (int)((* ucCursor) & TPCC_LST_MSK);
    LOG(7,"CISTPL_CONFIG: Info, TPCC_LAST = %d\n", * piLastEntry );
    /* Get Base Address of the Configuration Registers in Attribute Memory Space. */
    ucCursor   = pcis + TPCC_RADR;
    * TpccRadr = * ucCursor;
    if( ucTpccRasz )                                    /* Don't care if > 1  !!! */
        * TpccRadr += ((int) * (ucCursor + 1)) << 8;
    LOG(7,"CISTPL_CONFIG: Info, TPCC_RADR = %lx\n", * TpccRadr );
    /* Is there a sub-tuple ? */
    ucRead = (unsigned char)(TPCC_RADR + (ucTpccRasz + 1) + (ucTpccRmsz + 1));
    if( ucLink > ucRead )
    {
        /* Make ucCursor point on sub-tuple beginning and analyse it. */
        ucCursor = pcis + ucRead;
        if( * ucCursor == CCST_CIF )
        {
            ucSubLink = * (ucCursor + 1);
            /* Make ucCursor point on the Interface Number ID and check it. */
            ucCursor += 2;
            if( * ucCursor != 0x41  ||  * (ucCursor + 1) != 0x02 )
            {
                LOG(5, "CCST_CIF: Error, Interf Numb ID not equal to 0x0241\n" );
            }
            /* Make ucCursor point on the string that must follows. Check this string. */
            ucCursor += 2;
            ucNbStr   = (unsigned char) x_strlen( STCI_STR );
            if( x_strncmp((char *)ucCursor, STCI_STR, ucNbStr )   ||
                ! isdigit( * (ucCursor + ucNbStr) )      ||
                * (ucCursor + ucNbStr + 1) != STCI_STRP  ||
                ! isdigit( * (ucCursor + ucNbStr + 2) )  ||
                ! isdigit( * (ucCursor + ucNbStr + 3) )     )
            {
                LOG(5, "CCST_CIF: Error, DVB_CI_Vx.xx not found...\n" );
            }
            else
            {
                int CharSum;
                LOG(7,"CCST_CIF: ok for string " );
                for( CharSum=ucNbStr+4; CharSum>0; CharSum--, ucCursor++ )
                    LOG(7, "%c", *ucCursor );
                LOG(7, "\n" );
            }
            ucRead += 2 + ucSubLink;
        }
    }
    if( ucRead != ucLink )
    {
        LOG(7,"CISTPL_CONFIG: Can't read count of bytes\n" );
    }
    return iFound;
}
/* ======================================================================== */
/* NAME : PHYS_Tuple                                                        */
/*                                                                          */
/* DESCRIPTION : control the current CIS                                    */
/*                                                                          */
/* INPUT PARAMETERS :                                                       */
/*      pcis : pointer to the tuple to control                              */
/*      len  : length of the remainding CIS with the current tuple          */
/*      first: 1 for the first CISTPL_CFTABLE_ENTRY tuple, else 0           */
/*      itpcc_radr : configuration registers base address                   */
/*                                                                          */
/* OUTPUT PARAMETERS :                                                      */
/*      pcis : pointer to the next tuple                                    */
/*      len : length of the remainding CIS without the current tuple        */
/*      first: 0 when an new entry is OK                                    */
/*      itcc_radr : configuration registers base address                    */
/*      dvb : index number of the dvb entry in case of a DVB entry.         */
/*                                                                          */
/* RETURN CODE                                                              */
/*         1 : OK   (maybe a full compliant DVB entry)                      */
/*         0 : a not fully compliant DVB entry (*dvb set to non NULL value) */
/*        -1 : error                                                        */
/*                                                                          */
/* ======================================================================== */
int _PODCI_PHYS_Tuple(unsigned char **pcis, int *len, int *first, int *itpcc_radr, int *dvb)
{
    int             ocis;               /* current offset in the CIS */
    unsigned char   link;               /* tuple link                */
    cis_tuple_t     tuple;              /* current tuple             */
    int             last;               /* index of the last entry.  */
    int             Result = -1;
    
    ocis  = 0;
    tuple = (cis_tuple_t)**pcis;

    if( tuple == CISTPL_END )
    {
        if( *len == 1 )
        {
            *len = 0;                                           /* no more tuples            */
            return 1;                                           /* good CISTPL_END tuple: OK */
        }
    }
    link   = *(*pcis+1);
    *pcis += 2;
    ocis  += 2;
    LOG(0, "PHYS_Tuple: Len passed = %d   Link = %d\n", *len, link );
    switch( tuple )
    {
        case (cis_tuple_t)CISTPL_VERS_1 :
        {
            unsigned char arr_ciplus_comp_id[MAX_CISLEN];
            unsigned char tuple_data[MAX_CISLEN];
            BOOL b_first_sign = FALSE;
            BOOL b_end_sign = FALSE;
            unsigned char dollar_sign_start_idx = 0;
            unsigned char *pt_dollar_sign_start = NULL;
            unsigned char dollar_sign_end_idx = 0;
            unsigned char ciplus_comp_len = 0;
            unsigned char i;
            
            if (*(*pcis + TPLLV1_MAJOR) != TPLLV1_MAJOR_VAL  || /* Control: wrong TPLLV1_MAJOR */
                *(*pcis + TPLLV1_MINOR) != TPLLV1_MINOR_VAL)    /* Control: wrong TPLLV1_MINOR */
            {
                Result = -1;
                LOG(7,"PHYS_Tuple: CISTPL_VERS_1 tuple INVALID\n" );
            }
            else
            {
                Result = 1;
            }
            
            /* check CI Plus Compatibility Identification, from CI+ spec, G.3 CI Plus Compatibility Identification*/
            ui1CiplusCompId = 0;
            x_memset(arr_ciplus_comp_id, 0x00, sizeof(unsigned char)*(MAX_CISLEN));
            b_first_sign = FALSE;
            b_end_sign = FALSE;
            x_memcpy(tuple_data, *pcis, link);

            for (i =0; i<link; i++)
            {
                /* get the first dollar sign and end of dollar sign. */
                if (tuple_data[i] == '$')
                {
                    if (b_first_sign == FALSE)
                    {
                        b_first_sign = TRUE;
                        dollar_sign_start_idx = i;
                        pt_dollar_sign_start = &tuple_data[i];
                    }                          
                    else
                    {
                        if(b_end_sign == FALSE)
                        {
                            b_end_sign = TRUE;
                            dollar_sign_end_idx= i;
                            break;
                        }
                    }
                }                      
            }
            if ((b_first_sign == TRUE) && (b_end_sign == TRUE))
            {   
                ciplus_comp_len = (dollar_sign_end_idx - dollar_sign_start_idx)+1;
            
                if (ciplus_comp_len < (link-2))
                {
                    x_memcpy(arr_ciplus_comp_id, pt_dollar_sign_start, sizeof(unsigned char)*ciplus_comp_len);   

                    LOG(0,"CI Plus Comp id =            %s\n", arr_ciplus_comp_id );
                    _PODCI_PHYS_case_string((CHAR*)arr_ciplus_comp_id, ciplus_comp_len, TRUE);
                    LOG(0,"CI Plus Comp id lower case = %s\n", arr_ciplus_comp_id );                   
            
                    /* check start and and of compatibility string and compatibility_sequence */
                    if ((x_memcmp(CIPLUS_COMP_S_STR, arr_ciplus_comp_id, sizeof(unsigned char)*CIPLUS_COMP_S_STR_LEN) == 0) &&
                       (x_memcmp(CIPLUS_COMP_E_STR, &arr_ciplus_comp_id[ciplus_comp_len-2], sizeof(unsigned char)*CIPLUS_COMP_E_STR_LEN) == 0) &&
                       (x_strstr((const char*)arr_ciplus_comp_id, CIPLUS_COMP_SEQ_STR) != NULL))
                    {
                        ui1CiplusCompId = 1;
                        LOG(0,"CI Plus Compatibility Identification\n");
                    }
                }
            }                  
            /* free all resource */
            x_memset(arr_ciplus_comp_id, 0x00, sizeof(unsigned char)*(MAX_CISLEN));
            b_first_sign = FALSE;
            b_end_sign = FALSE;
            }
            break;

        case (cis_tuple_t)CISTPL_CONFIG :
            if( ! _PODCI_PHYS_Tuple_CISTPL_CONFIG( * pcis, link, & last, itpcc_radr ) )
            {
                Result = -1;
            }
            else
            {
                Result = 1;
            }
            break;              /* =======                             */

        case (cis_tuple_t)CISTPL_CFTABLE_ENTRY :
            if( ! * dvb )
            {
                struct dvb_entry_bundle ThisEntry;
                Result = _PODCI_PHYS_Tuple_CISTPL_CFTABLE_ENTRY( *pcis - 2, * first, & ThisEntry );
                if( Result >= 0 )
                {
                    * dvb = ThisEntry.EntryNumb;
                    LOG(7,"PHYS_Tuple: DVB ENTRY ........No: %d\n", * dvb );
                }
                if( * first == 1  &&  ThisEntry.Is_TPCE_INDX_Ok != 1 )
                {
                    LOG(7, "PHYS_Tuple: Intface and Default bits not compliant in first entry\n" );
                }
                * first = 0;
            }
            break;
        case (cis_tuple_t)CISTPL_NO_LINK :
            if (link != CISTPL_NOLINKLEN)   /* Control */
            {
                Result = -1;                    /* ======= wrong CISTPL_NOLINK tuple */
            }
            else
            {
                Result = 1;
            }
            break;
        default :
            /* other tuples : no control */
            break;
    }
    /* skip the current tuple */
    ocis += link;
    if (ocis >= *len)   /* Control */
        Result = -1;    /* ======= tuple not found */
    *pcis = *pcis + link;
    *len -= ocis;
    return Result;
}
/* ======================================================================== */
/* NAME : PHYS_CheckCIS                                                     */
/*                                                                          */
/* DESCRIPTION : Check the CIS                                              */
/*      The following tuples must be found:                                 */
/*          - CISTPL_DEVICE                                                 */
/*          - CISTPL_DEVICE_A                                               */
/*          - CISTPL_DEVICE_0A                                              */
/*          - CISTPL_DEVICE_0C                                              */
/*          - CISTPL_VERS_1                                                 */
/*          - CISTPL_MANFID                                                 */
/*          - CISTPL_CONFIG                                                 */
/*          - CISTPL_CFTABLE_ENTRY                                          */
/*          - CISTPL_NOLINK                                                 */
/*          - CISTPL_END                                                    */
/*      Other tuples are ignored                                            */
/*                                                                          */
/* INPUT PARAMETERS :                                                       */
/*      cis : CIS buffer                                                    */
/*      len : length of the CIS buffer                                      */
/*                                                                          */
/* OUTPUT PARAMETERS :                                                      */
/*      addrcor : COR address                                               */
/*      cor     : COR value                                                 */
/*                                                                          */
/* RETURN CODE                                                              */
/*         0 : OK                                                           */
/*        -1 : error                                                        */
/*                                                                          */
/* ======================================================================== */
static CI_error_t _PODCI_PHYS_CheckCIS(UINT8 *cis, UINT16 len, UINT16 *addrcor, UINT8 *cor)
{
    unsigned char *pcis;    /* pointer to the remainding CIS    */
    cis_tuple_t   tuple;    /* current tuple type.              */
    unsigned char  offset;  /* link to next tuple.              */
    int lcis;               /* remainding length of the CIS     */
    int first;              /* 1 for the first configuration table entry, else 0 */
    int addr;               /* configuration registers base address */
    int dvb;                /* index number of the DVB entry    */
    int IsAddrCor = 0;      /* Cor Address is not found.        */
    int IsCor = 0;          /* Cor is not found.                */
    int Result;             /* Return value got from PHYS_Tuple */
    pcis  = cis;
    lcis  = (int)len;
    first = 1;
    dvb   = 0;

    while( * pcis != CISTPL_END )
    {
        tuple  = (cis_tuple_t)* pcis;
        offset = *(pcis+1);
        /* Check error case: Length of the TLV record is longer than the remaining length. */
        if( offset + 2 > lcis )
        {
            LOG(7, "PHYS_CheckCIS: Error in tuple length [%d!] CIS_length=%d offset=%d\n",
                        offset+2, len, (int) (pcis - cis) );
            break;
        }
        Result = _PODCI_PHYS_Tuple( & pcis, & lcis, & first, & addr, & dvb );
        if( Result != -1 )              /* Skip error cases */
        {
            switch( tuple )
            {
            case (cis_tuple_t)CISTPL_CONFIG:
                *addrcor = (UINT16)addr;
                IsAddrCor = 1;
                break;
            case (cis_tuple_t)CISTPL_CFTABLE_ENTRY:
                if( dvb != 0 )
                {
                    *cor = (UINT8)dvb;
                    IsCor = 1;
                }
                break;
            case (cis_tuple_t)CISTPL_DEVICE:
            case (cis_tuple_t)CISTPL_DEVICE_A:
            case (cis_tuple_t)CISTPL_DEVICE_OA:
            case (cis_tuple_t)CISTPL_DEVICE_OC:
            case (cis_tuple_t)CISTPL_VERS_1:
            case (cis_tuple_t)CISTPL_MANFID:
            case (cis_tuple_t)CISTPL_NO_LINK:
            //case (cis_tuple_t)CISTPL_END:
            default:
                break;
            } /* end of switch */
        } /* end of if */
    } /* end of while */
    if( IsAddrCor == 1  &&  IsCor == 1 ) {
        LOG(0, "PHYS_CheckCIS: ......CIS OK\n" );
        return CI_SUCCESS;
    } else {
        LOG(0, "PHYS_CheckCIS: ......CIS FAILURE  !!!!!\n" );
        return CI_ERROR_INVALID_CIS;
    }
}

/* ======================================================================== */
/* NAME : _PODCI_PHYS_case_string                                                  */
/*                                                                          */
/* DESCRIPTION : case string to lower case of upper case                    */
/*                                                                          */
/* INPUT PARAMETERS :                                                       */
/*    ps_str_io  : buffer of input string data                              */
/*    ui4_len : length of cis tuple data                                    */
/*    b_to_lower   :  TRUE - Transfer lower case.                           */
/*                    FALSE- Transfer upper case.                           */
/*                                                                          */
/* OUTPUT PARAMETERS :                                                      */
/*    ps_str_io  : buffer of input string data                              */
/*                                                                          */
/* RETURN CODE                                                              */
/*    > 0 : OK                                                              */
/*     -1 : error                                                           */
/*                                                                          */
/* ======================================================================== */
static INT32 _PODCI_PHYS_case_string (CHAR*  ps_str_io,
                                       UINT32       ui4_len,
                                       BOOL         b_to_lower)
{
    UINT32 ui4_i = 0;
    if ((ui4_len == 0) || (ps_str_io == NULL))
    {
        return(-1);
    }
    for (ui4_i =0; ui4_i<ui4_len; ui4_i++)
    {
        if (b_to_lower)
        {
            if((ps_str_io[ui4_i] >='A') && (ps_str_io[ui4_i] <='Z'))
            {
                ps_str_io[ui4_i] = ps_str_io[ui4_i]-'A' + 'a';
            }
        }
        else
        {
            if((ps_str_io[ui4_i] >='a') && (ps_str_io[ui4_i] <='z'))
            {
                ps_str_io[ui4_i] = ps_str_io[ui4_i]-'a' + 'A';
            }
        }        
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

INT32 PODCI_GetCis(POD_IO_STATE_T	eIoState,
				  UINT8*     	    pu1Cis,
                  UINT32*    	    pu4CisLen)
{
    UINT32      u4CisBufLen;
    UINT8       u1CurTupleCode;
    UINT8       u1CurTupleLen;
    UINT8       u1I;
    CIS_INFO_T  rCisInfo;
	rCisInfo.fgPodModuleType = 0;
    rCisInfo.fgVpp = 0;
    rCisInfo.fgIrq = 0;
    rCisInfo.u1ModuleInterfaceType = 0;
    rCisInfo.u1CorValue = 0;
    rCisInfo.u2CorAddr= 0;

    ASSERT((pu1Cis != NULL) && (pu4CisLen != NULL));

    switch (eIoState)
    {
    	case POD_SDA:
		    /* Read the first tuple code */
		    u4CisBufLen = *pu4CisLen;
		    *pu4CisLen = 0;
		    if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		    {
		        PCMCIAHW_WRITE32(REG_DEV_ADDR, *pu4CisLen);
		        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
		        (void)PCMCIA_GetCmdStatusDone();
		        pu1Cis[(*pu4CisLen)++] = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		        u1CurTupleCode = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		    }
		    else
		    {
		        return POD_OK;
		    }

		    while (u1CurTupleCode != CISTPL_END)
		    {
		        /* Read tuple length */
		        if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		        {
		            PCMCIAHW_WRITE32(REG_DEV_ADDR, (*pu4CisLen)*2); /* Only even byte is valid */
		            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
		            (void)PCMCIA_GetCmdStatusDone();
		            pu1Cis[(*pu4CisLen)++] = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		            u1CurTupleLen = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		        }
		        else
		        {
		            return POD_OK;
		        }

		        /* Read tuple data */
		        for (u1I = 0; u1I < u1CurTupleLen; u1I++)
		        {
		            if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		            {
		                PCMCIAHW_WRITE32(REG_DEV_ADDR, (*pu4CisLen)*2);
		                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
		                (void)PCMCIA_GetCmdStatusDone();
		                pu1Cis[(*pu4CisLen)++] = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		            }
		            else
		            {
		                return POD_OK;
		            }
		        }

		        /* Read tuple code */
		        if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		        {
		            PCMCIAHW_WRITE32(REG_DEV_ADDR, (*pu4CisLen)*2);
		            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
		            (void)PCMCIA_GetCmdStatusDone();
		            pu1Cis[(*pu4CisLen)++] = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		            u1CurTupleCode = PCMCIAHW_READ8(REG_SDA_DATA_READ);
		        }
		        else
		        {
		            return POD_OK;
		        }
		    }
		    break;

		case POD_DMA:
		    /* Read the first tuple code */
		    u4CisBufLen = *pu4CisLen;
		    *pu4CisLen = 0;
		    if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		    {
		        PCMCIAHW_WRITE32(REG_DEV_ADDR, *pu4CisLen);
		        PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)&pu1Cis[*pu4CisLen]);
		        PCMCIAHW_WRITE32(REG_BYTE_CNT, 1);
		        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE));
		        (void)PCMCIA_GetCmdStatusDone();
		        //HalFlushInvalidateDCache();
		        HalFlushInvalidateDCacheMultipleLine((UINT32)&pu1Cis[*pu4CisLen], 1);
		        u1CurTupleCode = pu1Cis[(*pu4CisLen)++];
		    }
		    else
		    {
		        return POD_OK;
		    }

		    while (u1CurTupleCode != CISTPL_END)
		    {
		        /* Read tuple length */
		        if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		        {
		            PCMCIAHW_WRITE32(REG_DEV_ADDR, (*pu4CisLen)*2); /* Only even byte is valid */
		            PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)&pu1Cis[*pu4CisLen]);
		            PCMCIAHW_WRITE32(REG_BYTE_CNT, 1);
		            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE));
		            (void)PCMCIA_GetCmdStatusDone();
		            //HalFlushInvalidateDCache();
		            HalFlushInvalidateDCacheMultipleLine((UINT32)&pu1Cis[*pu4CisLen], 1);
		            u1CurTupleLen = pu1Cis[(*pu4CisLen)++];
		        }
		        else
		        {
		            return POD_OK;
		        }

		        /* Read tuple data */
		        for (u1I = 0; u1I < u1CurTupleLen; u1I++)
		        {
		            if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		            {
		                PCMCIAHW_WRITE32(REG_DEV_ADDR, (*pu4CisLen)*2);
		                PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)&pu1Cis[(*pu4CisLen)++]);
		                PCMCIAHW_WRITE32(REG_BYTE_CNT, 1);
		                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE));
		                (void)PCMCIA_GetCmdStatusDone();
		                //HalFlushInvalidateDCache();
		                HalFlushInvalidateDCacheMultipleLine((UINT32)&pu1Cis[*pu4CisLen], 1);
		            }
		            else
		            {
		                return POD_OK;
		            }
		        }

		        /* Read tuple code */
		        if (CONTINUE_GET_CIS(*pu4CisLen, u4CisBufLen))
		        {
		            PCMCIAHW_WRITE32(REG_DEV_ADDR, (*pu4CisLen)*2);
		            PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)&pu1Cis[*pu4CisLen]);
		            PCMCIAHW_WRITE32(REG_BYTE_CNT, 1);
		            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE));
		            (void)PCMCIA_GetCmdStatusDone();
		            //HalFlushInvalidateDCache();
		            HalFlushInvalidateDCacheMultipleLine((UINT32)&pu1Cis[*pu4CisLen], 1);
		            u1CurTupleCode = pu1Cis[(*pu4CisLen)++];
		        }
		        else
		        {
		            return POD_OK;
		        }
		    }
			break;
	} /* switch (eIoState) */

    /* Resolve NDS falling-edge OOB gated clock issue */
    /* Temporary solution => should be removed after MT5112 ECO for this issue */
    VERIFY(PODCI_ParseCis(pu1Cis, &rCisInfo) == POD_OK);

    return POD_OK;
}

INT32 PODCI_ParseCis(UINT8*       pu1CisMap,
                   CIS_INFO_T*  prCisInfo)
{
    BOOL                fgCisError;
    UINT8               u1TupleCode;
    UINT8               u1TupleSize;
    UINT8*              pu1TupleData;
    UINT8*              pu1Cis;
    MODULE_CONFIG_T     rConfig;

    ASSERT((pu1CisMap != NULL) && (prCisInfo != NULL));

    rConfig.u1IntFace = 0;
    rConfig.u1Io = 0;
    rConfig.u1Power = 0;
    rConfig.u1Irq = 0;
    rConfig.fgValid = FALSE;

    pu1Cis = pu1CisMap;
    fgCisError = FALSE;

    u1TupleCode = _PODCI_GetCisTuple(pu1Cis, &pu1TupleData, &u1TupleSize);
    while ((u1TupleCode != CISTPL_END) &&
          (!fgCisError))
	{
		switch (u1TupleCode)
		{
			case CISTPL_LINKTARGET:
				break;

			case CISTPL_DEVICE_OA:
				break;

			case CISTPL_DEVICE_OC:
				break;

			case CISTPL_VERS_1:
				if (!_PODCI_CisTupleVersion(pu1TupleData))
				{
					fgCisError = TRUE;
				}
				break;

			case CISTPL_MANFID:
				break;

			case CISTPL_CONFIG:
			    if (!_PODCI_CisTupleConfig(pu1TupleData,
			                         u1TupleSize,
			                         &(prCisInfo->u2CorAddr),
								     &(prCisInfo->fgPodModuleType)))
			    {
			        fgCisError = TRUE;
			    }
				break;

			case CISTPL_CFTABLE_ENTRY:
				if (!rConfig.fgValid)
				{
					if (!_PODCI_CisTupleConfigTableEntry(pu1TupleData,
					                               u1TupleSize,
					                               &rConfig,
					                               &(prCisInfo->u1CorValue),
					                               &(prCisInfo->fgVpp)))
					{
					    fgCisError = TRUE;
					}
				}
				break;

			case CISTPL_NO_LINK:
				break;

		    default:
		        break;
		}

		if ((u1TupleCode == CISTPL_END) ||
		    fgCisError)
		{
			break;
		}

		pu1Cis += u1TupleSize + 2;
		u1TupleCode = _PODCI_GetCisTuple(pu1Cis, &pu1TupleData, &u1TupleSize);
	}

	if (fgCisError)
	{
	    return POD_CIS_ERROR;
	}
	else
	{
    	/* Modify COR value depending on IRQ mode :
    	   if module type is POD -then- BIT6 of COR value shall be set (indicating
    	   LEVEL MODE interrupts) (SCTE DVS/221 REV7)
    	   If a IRQ description field was found in a Config Table entry indicating
    	   IRQ LEVEL mode, BIT6 is also set (This does not comes from DVB or POD
    	   specification but is interpreted from PC card standards volume 2
    	   (chapter 4.15.1) */
    	if ((rConfig.u1Irq & BIT5) || prCisInfo->fgPodModuleType)
    	{
    		prCisInfo->u1CorValue |= BIT6;
    	}

	    return POD_OK;
	}
}

//-----------------------------------------------------------------------------
/** PODCI_WriteCor
 *  This API writes a value parsed from CIS into COR.
 */
//-----------------------------------------------------------------------------
INT32 PODCI_WriteCor(UINT16  u2CorAddress,
                   UINT8   u1CorValue)
{
    UINT16 u2CorAddr = u2CorAddress;
    if (u2CorAddr > 0xFFE)
    {
        u2CorAddr = 0xFFE;
    }
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u2CorAddr); /* COR address */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CorValue); /* COR value */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)((PC_SDA_WRITE & (UINT32)ATTRIBUTE_MEM_OP) | ICMD_IE)); /* COR is located in atribute memory */
    (void)PCMCIA_GetCmdStatusDone();
    x_thread_delay(1); /* At a minimum of 10 usec after the COR write signal,
                          the POD module shall switch to the OOB inerface signals
                          and the inband transport stream signals (SCTE 28 p41) */

    return POD_OK;
}

//-----------------------------------------------------------------------------
/** PODCI_ReadCor
 *  This API reads a value from COR.
 */
//-----------------------------------------------------------------------------
INT32 PODCI_ReadCor(UINT16 u2CorAddress)
{
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u2CorAddress); /* COR address */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)((PC_SDA_READ & (UINT32)ATTRIBUTE_MEM_OP) | ICMD_IE)); // COR is located in atribute memory
    (void)PCMCIA_GetCmdStatusDone();
    return (INT32)PCMCIAHW_READ8(REG_SDA_DATA_READ);
}


INT32 PODCI_CheckCIS(UINT8* pu1Cis, UINT16 *addrcor, UINT8 *cor)
{
    CI_error_t           error;
    error = _PODCI_PHYS_CheckCIS(pu1Cis, MAX_CISLEN ,addrcor, cor);
    if ( error != CI_SUCCESS ) 
    {
        LOG(1, "PODCI_CheckCIS failed with %d\n",  error);
        return error;       /* wrong CIS */
    }
    PODCI_SetCISInfo(*addrcor, *cor);
    LOG(0, "PODCI_CheckCIS OK\n");
    return error;
}

INT32 PODCI_CheckCIPLUSCapability(UINT8 *pui1CIPlusCap)
{   
    *pui1CIPlusCap = ui1CiplusCompId;
    LOG(0, "PODCI_CheckCIPLUSCapability = %d\n", *pui1CIPlusCap);
    return POD_OK;
}
#endif
#endif
