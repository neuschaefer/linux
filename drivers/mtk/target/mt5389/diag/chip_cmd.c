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
 *
 * Description:
 *      Memory test commands
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_bim.h"

//=====================================================================
// extern functions
//=====================================================================

typedef struct __ModulePower
{
    UINT32 u4RegOffset;
    UINT32 u4BitField;
    UINT32 u4BitFieldDisableSetting;
    UINT8 *pszName;
} MODULE_POWER_T;

MODULE_POWER_T  raModulePowerList[] = {
    { 0x00000000, 0x00000000, 0x00000000, "VGA ADC" },
    { 0x00000000, 0x00000000, 0x00000000, "ADCPLL" },
    { 0x00000000, 0x00000000, 0x00000000, "CVBS ADC" },
    { 0x00000000, 0x00000000, 0x00000000, "HDMI" },
    { 0x2000d218, 0x00000040, 0x00000040, "DEMOD ATD" },
    { 0x2000d21c, 0x00800000, 0x00800000, "DEMOD DVBT" },
    { 0x2000d278, 0x00000020, 0x00000000, "B2R Enable" },
    { 0x2000d27c, 0x00000020, 0x00000000, "B2R RST" },
    { 0x2000d278, 0x00000400, 0x00000000, "VDEC Enable" },
    { 0x2000d27c, 0x00000400, 0x00000000, "VDEC RST" },
    { 0x00000000, 0x00000000, 0x00000000, "PIP_SUB" },
    { 0x2000d278, 0x00004000, 0x00000000, "PSCAN_TOP Enable" },
    { 0x2000d27c, 0x00004000, 0x00000000, "PSCAN_TOP RST" },
    { 0x2000d278, 0x00008000, 0x00000000, "DDI Enable" },
    { 0x2000d27c, 0x00008000, 0x00000000, "DDI RST" },
    { 0x2000d278, 0x00010000, 0x00000000, "DEMUX Enable" },
    { 0x2000d27c, 0x00010000, 0x00000000, "DEMUX RST" },
    { 0x2000d29c, 0x00000002, 0x00000002, "TVD_54CK" },
    { 0x2000d29c, 0x00000004, 0x00000004, "HDTV_CLK" },
    { 0x2000d29c, 0x00000020, 0x00000020, "CCH_CLK" },
    { 0x2000d29c, 0x00000010, 0x00000010, "DVI_CLK" },
    { 0x2000d29c, 0x00000040, 0x00000040, "VBI_CLK" },
    { 0x2000d29c, 0x00000080, 0x00000080, "VBI2_CLK" },
    { 0x2000d29c, 0x00000200, 0x00000200, "PIP_CLK" }
};

static INT32 _CmdDynamicPowerStatus(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i;

    Printf("Dynamic Power Status\n");
    for (i=0; i<(sizeof(raModulePowerList)/sizeof(MODULE_POWER_T)); i++)
    {
        UINT32 u4Reg, u4Msk, u4Val;

        u4Reg = raModulePowerList[i].u4RegOffset;
        u4Msk = raModulePowerList[i].u4BitField;
        u4Val = raModulePowerList[i].u4BitFieldDisableSetting;
        if (u4Msk != 0)
        {
            Printf("\t%s - %s\n", raModulePowerList[i].pszName, ((HAL_READ32(u4Reg) & u4Msk) == u4Val) ? "OFF" : "ON");
        }
        else
        {
            Printf("\t%s - N/A\n", raModulePowerList[i].pszName);
        }
    }
    return 0;
}

//=====================================================================

static CLI_EXEC_T _arCHIPCmdTbl[] =
{
    { 
        "dps", 
        "dps",
        _CmdDynamicPowerStatus,
        NULL,
        "dynamic power status", 
        CLI_SUPERVISOR
    },    

 	// last cli command record, NULL
 	{
		NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
	}
};

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
CLI_MAIN_COMMAND_ITEM(CHIP)
{
	"chip",
	NULL,
	NULL,
	_arCHIPCmdTbl,
	"CHIP module test",
	CLI_SUPERVISOR
};

