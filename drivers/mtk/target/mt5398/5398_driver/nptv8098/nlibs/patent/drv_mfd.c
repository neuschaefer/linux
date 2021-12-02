/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mfd.c $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "general.h"
#include "hw_mjcfd.h"
#include "hw_mfd.h"
#include "drv_mfd.h"
#include "drv_mjc.h"
#include "vdo_misc.h"
#include "x_os.h"
#include "x_assert.h"
#include "hw_vdoin.h"


/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
MFD_REGTBL_T CODE MJCFD_PARAM[6][12] =
{
    // 480p
    {
        {MJCFD_00, 0x88430000, 0xFFFFFFFB},
        {MJCFD_01, 0x40201008, 0xFFFFFFFF},
        {MJCFD_02, 0x01000080, 0xFFFF00FF},
        {MJCFD_03, 0x40100402, 0xFFFFFFFF},
        {MJCFD_04, 0x0A40590A, 0xFFFFFFFF},
        {MJCFD_05, 0x02100C16, 0xFFFFFFFF},
        {MJCFD_06, 0x4BAD3039, 0xFFFFFFFF},
        {MJCFD_07, 0x02201000, 0xFFFFFFFF},
        {MJCFD_0D, 0x00000018, 0x00FFFFFF},
        {MJCFD_BD_0, 0x00100010, 0x00FFFFFF},
        {MJCFD_BD_1, 0x00100010, 0x00FFFFFF},
        {MFD_REGTBL_END, 0x00000000, 0x00000000}
    },
    // 576p
    {
        {MJCFD_00, 0x88430000, 0xFFFFFFFB},
        {MJCFD_01, 0x40201008, 0xFFFFFFFF},
        {MJCFD_02, 0x01000080, 0xFFFF00FF},
        {MJCFD_03, 0x40100402, 0xFFFFFFFF},
        {MJCFD_04, 0x0A40590A, 0xFFFFFFFF},
        {MJCFD_05, 0x02100C16, 0xFFFFFFFF},
        {MJCFD_06, 0x4BAD3039, 0xFFFFFFFF},
        {MJCFD_07, 0x02201000, 0xFFFFFFFF},
        {MJCFD_0D, 0x00000018, 0x00FFFFFF},
        {MJCFD_BD_0, 0x00100010, 0x00FFFFFF},
        {MJCFD_BD_1, 0x00100010, 0x00FFFFFF},
        {MFD_REGTBL_END, 0x00000000, 0x00000000}
    },
    // 480p_oversample
    {
        {MJCFD_00, 0x88430000, 0xFFFFFFFB},
        {MJCFD_01, 0x40201008, 0xFFFFFFFF},
        {MJCFD_02, 0x01000080, 0xFFFF00FF},
        {MJCFD_03, 0x40100402, 0xFFFFFFFF},
        {MJCFD_04, 0x0A40590A, 0xFFFFFFFF},
        {MJCFD_05, 0x02280C16, 0xFFFFFFFF},
        {MJCFD_06, 0x4BAD6439, 0xFFFFFFFF},
        {MJCFD_07, 0x02201000, 0xFFFFFFFF},
        {MJCFD_0D, 0x00000018, 0x00FFFFFF},
        {MJCFD_BD_0, 0x00100010, 0x00FFFFFF},
        {MJCFD_BD_1, 0x00100010, 0x00FFFFFF},
        {MFD_REGTBL_END, 0x00000000, 0x00000000}
    },
    // 576p_oversample
    {
        {MJCFD_00, 0x88430000, 0xFFFFFFFB},
        {MJCFD_01, 0x40201008, 0xFFFFFFFF},
        {MJCFD_02, 0x01000080, 0xFFFF00FF},
        {MJCFD_03, 0x40100402, 0xFFFFFFFF},
        {MJCFD_04, 0x0A40590A, 0xFFFFFFFF},
        {MJCFD_05, 0x02280C16, 0xFFFFFFFF},
        {MJCFD_06, 0x4BAD6439, 0xFFFFFFFF},
        {MJCFD_07, 0x02201000, 0xFFFFFFFF},
        {MJCFD_0D, 0x00000018, 0x00FFFFFF},
        {MJCFD_BD_0, 0x00100010, 0x00FFFFFF},
        {MJCFD_BD_1, 0x00100010, 0x00FFFFFF},
        {MFD_REGTBL_END, 0x00000000, 0x00000000}
    },
    // 720p60, 720p50
    {
        {MJCFD_00, 0x88430000, 0xFFFFFFFB},
        {MJCFD_01, 0x40201008, 0xFFFFFFFF},
        {MJCFD_02, 0x01000080, 0xFFFF00FF},
        {MJCFD_03, 0x40100402, 0xFFFFFFFF},
        {MJCFD_04, 0x0A40590A, 0xFFFFFFFF},
        {MJCFD_05, 0x03280C16, 0xFFFFFFFF},
        {MJCFD_06, 0x4BAD6439, 0xFFFFFFFF},
        {MJCFD_07, 0x02201000, 0xFFFFFFFF},
        {MJCFD_0D, 0x00000018, 0x00FFFFFF},
        {MJCFD_BD_0, 0x00100010, 0x00FFFFFF},
        {MJCFD_BD_1, 0x00100010, 0x00FFFFFF},
        {MFD_REGTBL_END, 0x00000000, 0x00000000}
    },
    // 1080p60, 1080p50
    {
        {MJCFD_00, 0x88430000, 0xFFFFFFFB},
        {MJCFD_01, 0x40201008, 0xFFFFFFFF},
        {MJCFD_02, 0x01000080, 0xFFFF00FF},
        {MJCFD_03, 0x40100402, 0xFFFFFFFF},
        {MJCFD_04, 0x0A40590A, 0xFFFFFFFF},
        {MJCFD_05, 0x043C0C16, 0xFFFFFFFF},
        {MJCFD_06, 0x4BADC039, 0xFFFFFFFF},
        {MJCFD_07, 0x02201000, 0xFFFFFFFF},
        {MJCFD_0D, 0x00000018, 0x00FFFFFF},
        {MJCFD_BD_0, 0x00100010, 0x00FFFFFF},
        {MJCFD_BD_1, 0x00100010, 0x00FFFFFF},
        {MFD_REGTBL_END, 0x00000000, 0x00000000}
    }
};


/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void vDrvMFDInit(void)
{
    vDrvMFDSetQuality(MODE_720p_60);
	vRegWriteFldAlign(MFD_00, SV_OFF , MFD_FILM_SEL_FOR_MJC);
}

void vDrvMFDModeChgDone(UINT8 u1VdpId)
{  
    UINT8 u1Timing;
    u1Timing = bDrvVideoGetTiming(u1VdpId);
    vDrvMFDSetQuality(u1Timing);
}

void vDrvMFDSetQuality(UINT8 u1Timing)
{
    vRegWriteFldAlign(MJCFD_00, SV_OFF, MJCFD_C_FD_3DFS_MODE); // default is for 2D and 3D SBS/TAB
    switch (u1Timing)
    {
        case MODE_3D_480p_60_FP:
            vRegWriteFldAlign(MJCFD_00, SV_ON, MJCFD_C_FD_3DFS_MODE); // enable for 3D FS
        case MODE_480P:
            vDrvMJCFDLoadRegTbl(MJCFD_PARAM[0]); 
            break;

        case MODE_3D_576p_50_FP:
            vRegWriteFldAlign(MJCFD_00, SV_ON, MJCFD_C_FD_3DFS_MODE); // enable for 3D FS
        case MODE_576P:
            vDrvMJCFDLoadRegTbl(MJCFD_PARAM[1]);
            break;

        case MODE_480P_OVERSAMPLE:
            vDrvMJCFDLoadRegTbl(MJCFD_PARAM[2]); 
            break;
        case MODE_576P_OVERSAMPLE:
            vDrvMJCFDLoadRegTbl(MJCFD_PARAM[3]);
            break;

        case MODE_3D_720p_50_FP:
        case MODE_3D_720p_60_FP:
            vRegWriteFldAlign(MJCFD_00, SV_ON, MJCFD_C_FD_3DFS_MODE); // enable for 3D FS
        case MODE_720p_50:
        case MODE_720p_60:
            vDrvMJCFDLoadRegTbl(MJCFD_PARAM[4]);
            break;      

        case MODE_1080p_50:
        case MODE_1080p_60:
            vDrvMJCFDLoadRegTbl(MJCFD_PARAM[5]);
            break;

        default: // interlace timing
            //vRegWriteFldAlign(MJCFD_00, SV_ON , MJCFD_C_FD_INFO_SEL);
			//vRegWriteFldAlign(MJCFD_00, SV_OFF , MJCFD_C_FD_SRC_SEL);
            break;
    }
}

void vDrvMJCFDLoadRegTbl(MFD_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != MFD_REGTBL_END)
    {
        #ifdef CC_FPGA
        vIO32Write4BMsk(prRegTbl->u2Addr + 0x20000000, prRegTbl->u4Value, prRegTbl->u4Mask);
        #else
        vRegWrite4BMsk(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        #endif
        prRegTbl++;
    }
}

void vDrvMJCFDStatusPolling(void *pvArgv)
{
    UINT32 u4HSubSeq, u4VSubSeq, u4MovFrmSeq, u4MovPxlSeq;
    UINT32 u4FrmInfo, u4BlkInfo;
    UINT32 u4Count = 10;
    static BOOL u1First = 1;
    
    if (u1First)
    {
        u1First = 0;
        Printf("H_SUB V_SUB M_PXL M_FRM HS_FRM HS_BLK VS_FRM VS_BLK MP_FRM MP_BLK MOVING_FRM CB 3D\n");
        Printf("===== ===== ===== ===== ====== ====== ====== ====== ====== ====== ========== == ==\n");
    }
        
    u4HSubSeq = RegReadFldAlign(MJCFD_09, MJCFD_FD_SAW_SEQ);
    u4VSubSeq = RegReadFldAlign(MJCFD_09, MJCFD_FD_VST_SEQ);
    u4MovFrmSeq = RegReadFldAlign(MJCFD_0A, MJCFD_FD_MOV_SEQ);
    u4MovPxlSeq = RegReadFldAlign(MJCFD_0A, MJCFD_FD_DIF_SEQ);

    for (u4Count = 5; u4Count > 0; u4Count--)
    {
        Printf("%d", (u4HSubSeq >> (u4Count - 1)) & 0x1);
    }
    Printf(" ");

    for (u4Count = 5; u4Count > 0; u4Count--)
    {
        Printf("%d", (u4VSubSeq >> (u4Count - 1)) & 0x1);
    }
    Printf(" ");

    for (u4Count = 5; u4Count > 0; u4Count--)
    {
        Printf("%d", (u4MovPxlSeq >> (u4Count - 1)) & 0x1);
    }
    Printf(" ");

    for (u4Count = 5; u4Count > 0; u4Count--)
    {
        Printf("%d", (u4MovFrmSeq >> (u4Count - 1)) & 0x1);
    }
    Printf(" ");

    vRegWriteFldAlign(MJCFD_06, 0x1, MJCFD_C_FD_DBG_SEL);
    u4FrmInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_FRM_INFO);
    u4BlkInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_BLK_INFO);
    Printf(" %04X   %04X  ", u4FrmInfo, u4BlkInfo);
    
    vRegWriteFldAlign(MJCFD_06, 0x2, MJCFD_C_FD_DBG_SEL);
    u4FrmInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_FRM_INFO);
    u4BlkInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_BLK_INFO);
    Printf(" %04X   %04X  ", u4FrmInfo, u4BlkInfo);

    vRegWriteFldAlign(MJCFD_06, 0x3, MJCFD_C_FD_DBG_SEL);
    u4FrmInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_FRM_INFO);
    u4BlkInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_BLK_INFO);
    Printf(" %04X   %04X  ", u4FrmInfo, u4BlkInfo);

    vRegWriteFldAlign(MJCFD_06, 0x4, MJCFD_C_FD_DBG_SEL);
    u4FrmInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_FRM_INFO);
    u4BlkInfo = RegReadFldAlign(MJCFD_0B, MJCFD_FD_DET_BLK_INFO);
    Printf(" %04X%04X ", u4FrmInfo, u4BlkInfo);
    vRegWriteFldAlign(MJCFD_06, 0x0, MJCFD_C_FD_DBG_SEL);
    
    Printf(" %02d ", MJC_READ_FLD(MJCFD_08, MJCFD_FD_INFO));
    Printf(" %s", MJC_READ_FLD(MJC_STA_FM_03, MJC_STA_LR3D_I) ? "R" : "L");
    
    if (u4MJCStatusPollingCount == 1)
        u1First = 1;
}

