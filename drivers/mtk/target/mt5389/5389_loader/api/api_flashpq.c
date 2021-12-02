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

 // === INCLUDE =============================================================================
#include "x_assert.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "pe_if.h"
#include "drv_ycproc.h"
#include "drv_pq_cust.h"	// TODO: Remove
#include "vdo_misc.h"
#include "drv_od.h"	
#include "drv_default.h"
#include "linux/file_ops.h"
 
#ifndef CC_COPLAT_MT82
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#endif
 
 // === DEFINE =============================================================================
#define ENABLE_FLASH_PQ_DEBUG       1  //  1
#define LOAD_FLASH_FROM_FILE        1
 
#define FLASH_PQ_LOG(x)    (IO32ReadFldAlign(FLASH_PQ_00, FALSH_PQ_LOG_CFG) & x)
 
#define FLASH_PQ_GAMMA_CHANNEL_NUM  3
#define FLASH_PQ_GAMMA_CHANNEL_SIZE_256  256
#define FLASH_PQ_GAMMA_CHANNEL_SIZE_257  257
 
#define QTY_SCRIPT_SIZE(x) ((aFlashPqFileHead[x].bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT) ? TIMING_SCRIPT_SIZE:0)\
                            +((aFlashPqFileHead[x].bRes & RES_SUPPORT_SMARTPIC_DESCRIPT)? SMARTPIC_SCRIPT_SIZE:0)

#define QTY_MAX_MIN_NUM(x) ((aFlashPqFileHead[x].bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0)
#define QTY_MIN_INDEX(x)   (aFlashPqFileHead[x].bNumOfSmartPic)       // The end of all smart pic + 1 is MIN table.
#define QTY_MAX_INDEX(x)   (aFlashPqFileHead[x].bNumOfSmartPic + 1)   // The end of all smart pic + 2 is MAX table.
 
#define IsSupportLongName(x)   (aFlashPqFileHead[x].bRes & RES_SUPPORT_LONG_PANEL_NAME)
#define PANEL_LONG_NAME_SIZE(x)  (IsSupportLongName(x) ? TBL_LONG_PANEL_NAME_SIZE : 0) 
 
 enum
 {
     E_FLASH_PQ_GAMMA_LOG        = 0x01,
     E_FLASH_PQ_SCE_LOG          = 0x02,
     E_FLASH_PQ_QTY_LOG          = 0x04, 
     E_FLASH_PQ_QTY_MIN_MAX_LOG  = 0x08,
     E_FLASH_PQ_OD_LOG           = 0x10,
     E_FLASH_PQ_PANEL_LOG        = 0x20,
     E_FLASH_PQ_ELSE_LOG         = 0x40,
 };
 
 enum
 {
     E_OD_SUPPORT_FB     = 0x01,
     E_OD_SUPPORT_PCID   = 0x02,
     E_OD_SUPPORT_ENABLE =0x04,
 };
 
#define FLASHPQ_INDEX_CHECK(x) \
     do{ ASSERT(x < FLASHPQ_NUM); \
         x = (x >= FLASHPQ_NUM) ? 0 : x; }while(0)
 
 // === FUNCTION PROTOTYPE =================================================================
 
 // === EXTERN FUNCTION ====================================================================
 
 // === GLOBAL VARIABLE ====================================================================
 
 // === EXTERN VARIABLE ====================================================================
 
 // === STATIC VARIABLE ====================================================================
         
 FLASH_PQ_FILE_HEAD aFlashPqFileHead[FLASHPQ_NUM];
 FLASH_PQ_TABLE_HEAD aFlashPqGammaHead, aFlashPqSceHead, aFlashPqOdHead, aFlashPqPanelHead;
 FLASH_PQ_TABLE_HEAD aFlashPqQtyHead[FLASHPQ_NUM];
 PANEL_LONG_NAME aPcfPqfPanelLongName[FLASHPQ_NUM];
 UINT64 u8PqBaseAddr[FLASHPQ_NUM];
 UINT8 bFileHeadSize[FLASHPQ_NUM];
 UINT8 bFlashPQInit[FLASHPQ_NUM];
 UINT8 bPqExtesion[FLASHPQ_NUM]; 
 UINT8 bQtyMin[FLASHPQ_NUM][CUSTOM_QTY_TBL_DIM];
 UINT8 bQtyMax[FLASHPQ_NUM][CUSTOM_QTY_TBL_DIM];
 
 PANEL_FLASH_DATA_ATTRIBUTE_T rFlashPanelTable;
 
 const FLASH_PQ_SW_INFO aFlashPqSWInfo[FLASHPQ_NUM] =
 {
     {   eFlagFlashPqEnableGamma,            eFlagFlashPqEnableSCE, 
         eFlagFlashPqEnableQty,              eFlagFlashPqEnableQtyMaxMin, 
         eFlagFlashPqEnableQtySmartPic,      eFlagFlashPqQtySmartPicNewMode},
#ifdef SUPPORT_2ND_FLASHPQ
     {   eFlagFlashPq2ndpartEnableGamma,         eFlagFlashPq2ndpartEnableSCE,   
         eFlagFlashPq2ndpartEnableQty,           eFlagFlashPq2ndpartEnableQtyMaxMin, 
         eFlagFlashPq2ndpartEnableQtySmartPic,   eFlagFlashPq2ndpartQtySmartPicNewMode},
#endif
 };
 
 const FLASH_PQ_FLASH_INFO aFlashPqFlashInfo[FLASHPQ_NUM] =
 {
     {eFlashPqBasePartition, eFlashPqBaseAddress, eFlashPqOffset,eFlashPqBlockSize},
#ifdef SUPPORT_2ND_FLASHPQ
     {eFlashPq2ndpartBasePartition, eFlashPq2ndpartBaseAddress, eFlashPq2ndpartOffset, eFlashPq2ndpartBlockSize},
#endif
 };
 
 
 // === BODY ===============================================================================
 static void vDrvGammaTopPt(UINT8 * pu1Table)
 {
     if(pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] >= pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -2])
     {
         pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256] =
             pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 - 1] +
             (pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] - pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 - 2]);
     }
     else
     {
         pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256] =
             pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] +
             (UINT8)((UINT16)(pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1]+0x100) - 
             pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -2]);
     }
 }
 
 static UINT64 u8CalcFlashPqOffset(UINT8 bIndex, UINT16 nType, UINT16 nModel, UINT16 nSmartPic, UINT16 nSource, UINT16 nIdx)
 {
     FLASHPQ_INDEX_CHECK(bIndex);
     
     switch (nType)
     {
         case TYPE_PQ_HEADER:
             return u8PqBaseAddr[bIndex];
         case TYPE_GAMMA_HEADER:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * nModel;
         case TYPE_SCE_HEADER:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * nModel;
         case TYPE_QTY_HEADER:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * aFlashPqFileHead[bIndex].wNumOfSCE +
                 (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) * 
                 (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * nModel;
         case TYPE_QTY_DESCRIPT:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] + 
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * aFlashPqFileHead[bIndex].wNumOfSCE +
                 (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                 (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * nModel + 
                 TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (nSmartPic * aFlashPqFileHead[bIndex].wSizeOfQtyTable *
                 aFlashPqFileHead[bIndex].bNumOfTiming) + nSource * aFlashPqFileHead[bIndex].wSizeOfQtyTable;
         case TYPE_QTY_CONTENT:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] + 
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * aFlashPqFileHead[bIndex].wNumOfSCE +
                 (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                 (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * nModel + 
                 TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (nSmartPic * aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) +
                 nSource * aFlashPqFileHead[bIndex].wSizeOfQtyTable + QTY_SCRIPT_SIZE(bIndex) + nIdx;        
         case TYPE_OD_HEADER:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * aFlashPqFileHead[bIndex].wNumOfSCE +
                 (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                 (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * nModel;
         case TYPE_PANEL_HEADER:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * aFlashPqFileHead[bIndex].wNumOfSCE +
                 (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                 (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * aFlashPqFileHead[bIndex].wNumOfOd +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfPanelTable) * nModel;
         default:
         case TYPE_FILE_END:
             return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfGammaTable) * aFlashPqFileHead[bIndex].wNumOfGamma +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfSCETable) * aFlashPqFileHead[bIndex].wNumOfSCE +
                 (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                 (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty + 
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * aFlashPqFileHead[bIndex].wNumOfOd +
                 (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfPanelTable) * aFlashPqFileHead[bIndex].wNumOfPanelTable;;
     }
 }
 
 static UINT32 dwDrvCalcFlashPqQtyNumber(UINT8 bIndex)
 {
     int i;
 
     for (i=0; i<CUSTOM_QTY_TBL_DIM; i++)
     {
         if (READ_CUST_QTY_ITEM(bIndex, i) == QUALITY_MAX)
         {
             return i;
         }
     }
 
     return 0;
 }
 
 
 static UINT16 wDrvGetODTableSize(UINT16 u2OdFormat)
 {
     //_ODParam.u1ODTblSizeMode = u2OdFormat;
     if (u2OdFormat == 0)
     {
         LOG(0, "FlashPQ Size for OD : 17x17 \n");
         return OD_TBL_S_SIZE;
     }
     else if (u2OdFormat == 1)
     {
         LOG(0, "FlashPQ Size for OD : 33x33 \n");
         return OD_TBL_M_SIZE;
     }
     else if (u2OdFormat == 2)
     {
         LOG(0, "FlashPQ Size for OD : 41x41 \n");
         return OD_TBL_L_SIZE;
     }
     else if (u2OdFormat == 3)
     {
         LOG(0, "FlashPQ Size for OD : 41x41 \n");
         return OD_TBL_L_SIZE;
     }
     else
     {
         LOG(0, "FlashPQ Size for OD : Error \n");
         return 0;
     }
 }
 
#if ENABLE_FLASH_PQ_DEBUG 
 static void vDrvDebugFileHeader(UINT8 bIndex)
 {
     FLASHPQ_INDEX_CHECK(bIndex);
 
 
     LOG(1, "=== FlashPQ<%d> File Header ===\n", bIndex);
     LOG(1, "PQ base address:%X %X\n", u8PqBaseAddr[bIndex]>>32, (UINT32)u8PqBaseAddr[bIndex]);
     LOG(1, "ID %s\n", aFlashPqFileHead[bIndex].bID);
     
     LOG(1, "Gamma num       %d\n", aFlashPqFileHead[bIndex].wNumOfGamma);
     LOG(1, "Gamma size      %d\n", aFlashPqFileHead[bIndex].wSizeOfGammaTable);
 
     LOG(1, "SCE num         %d\n", aFlashPqFileHead[bIndex].wNumOfSCE);
     LOG(1, "SCE size        %d\n", aFlashPqFileHead[bIndex].wSizeOfSCETable);
 
     LOG(1, "Qty num         %d\n", aFlashPqFileHead[bIndex].wNumOfQty);
     LOG(1, "Qty size        %d\n", aFlashPqFileHead[bIndex].wSizeOfQtyTable);
     LOG(1, "Timing Num      %d\n", aFlashPqFileHead[bIndex].bNumOfTiming);
     
     LOG(1, "Descript        %d\n", aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT ? 1 : 0);    
     LOG(1, "MinMax          %d\n", aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_MAX_MIN ? 1 : 0);    
     LOG(1, "Hash            %d\n", aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_HASH ? 1 : 0);
     
     LOG(1, "Xml ver         %s\n", aFlashPqFileHead[bIndex].bQtyXmlVer);
 
     if (x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID_EX) == 0)
     {
         LOG(1, "OD num          %d\n", aFlashPqFileHead[bIndex].wNumOfOd);
         LOG(1, "OD size         %d\n", aFlashPqFileHead[bIndex].wSizeOfOdTable);
         LOG(1, "OD Format       %d\n", aFlashPqFileHead[bIndex].wConfigOfOd >> 4);
         LOG(1, "Support FB      %d\n", aFlashPqFileHead[bIndex].wConfigOfOd & E_OD_SUPPORT_FB);
         LOG(1, "Support PCID    %d\n", aFlashPqFileHead[bIndex].wConfigOfOd & E_OD_SUPPORT_PCID);
     }    
 
     LOG(1, "Panel num       %d\n", aFlashPqFileHead[bIndex].wNumOfPanelTable);
     LOG(1, "Panel size      %d\n", aFlashPqFileHead[bIndex].wSizeOfPanelTable);
 }
 
 static void vDrvDebugSCE(UINT8 bIndex)
 {
     UINT32 i;
     
     Printf("\nBASE_SCE_TBL = \n");
     for (i = 0; i < aFlashPqFileHead[bIndex].wSizeOfSCETable; i ++)
     {
         Printf("%2x ", BASE_SCE_TBL[i]);
         if ((i+1) % 28 == 0)
         {
             Printf("\n");
         }
     }
     
     Printf("\nMIN_SCE_TBL = \n");
     for (i = 0; i < aFlashPqFileHead[bIndex].wSizeOfSCETable; i ++)
     {
         Printf("%2x ", MIN_SCE_TBL[i]);
         if ((i+1) % 28 == 0)
         {
             Printf("\n");
         }
     }
 }
 
 static void vDrvDebugOD(UINT16 u2OdTblSize)
 {    
     UINT32 i;
     
     Printf("\nOD_TBL = \n");
     for (i = 0; i < u2OdTblSize; i ++)
     {
         Printf("%2x ", OD_Table[i]);
         if ((i+1) % 16 == 0)
         {
             Printf("\n");
         }
     }
     Printf("\nFB_TBL = \n");
     for (i = 0; i < FB_TBL_SIZE; i ++)
     {
         Printf("%2x ", FB_Table[i]);
         if ((i+1) % FB_TBL_DIM == 0)
         {
             Printf("\n");
         }
     }
 
     Printf("\nPCID_TBL = \n");
     for (i = 0; i < PCID_TBL_SIZE; i ++)
     {
         Printf("%2x ", PCID_Table[i]);
         if ((i+1) % PCID_TBL_DIM == 0)
         {
             Printf("\n");
         }
     }
 }
 
 static void vDrvDebugPanel(UINT16 u2PanelTblSize)
 {
     UINT32 i;
     UINT8 * PanelTableAdr = (UINT8*)(&rFlashPanelTable);
 
     Printf("\nPanel Table = \n");
     for (i = 0; i < u2PanelTblSize; i ++)
     {        
         Printf("%2x ", *(PanelTableAdr+i));
         if ((i+1) % 16 == 0)
         {
             Printf("\n");
         }
     }
 }
 
 static void vDrvDebugGamma(void)
 {
     UINT32 i, j;    
     
     for (i = 0; i < FLASH_PQ_GAMMA_CHANNEL_NUM; i ++)
     {        
         Printf("\nGAMMA_256 [%d]= \n", i);
         for (j = 0; j < FLASH_PQ_GAMMA_CHANNEL_SIZE_257; j ++)
         {
             Printf("%2x ", GAMMA_256[i][j]);
             if ((j+1) % 16 == 0)
             {
                 Printf("\n");
             }
         }
     }
 }
 
 static void vDrvDebugQtyTable(UINT8 bIndex)
 {
     UINT32 i, j;
 
     UINT8 u1NumOfTiming = aFlashPqFileHead[bIndex].bNumOfTiming;
     UINT16 u2SizeOfQtyTable = aFlashPqFileHead[bIndex].wSizeOfQtyTable;
         
     for (j = 0; j < u1NumOfTiming; j++)
     {
         Printf("\nTiming %d\n", j);
         for (i = 0; i < u2SizeOfQtyTable; i++)
         {
             Printf("%02X ", (int)READ_CUSTOM_QTY_TBL(j, (i | (UINT16)bIndex<<QTY_TBL_REF_INDEX_SHIFT)));
             if (i % 16 == 15)
             {
                 Printf("\n");
             }
         }
     }
 }
 
 static void vDrvDebugQtyMinMax(UINT8 bIndex)
 {   
     int i;
     
     UINT16 u2SizeOfQtyTable = aFlashPqFileHead[bIndex].wSizeOfQtyTable;
     UINT8 bQty_script_size = QTY_SCRIPT_SIZE(bIndex);
 
     Printf("\nMin  Max\n");
     for (i = 0; i < u2SizeOfQtyTable - bQty_script_size; i++)
     {
         Printf("%02X  %02X\n", bQtyMin[i], bQtyMax[i]);
     }
 
     Printf("\nQty Min Max\n");
     for (i = 0; i < QUALITY_MAX; i++)
     {
         Printf("Dft[%d]  %02X  %02X\n", i, READ_DEFUALT_QTY_TBL_MIN(i), READ_DEFUALT_QTY_TBL_MAX(i));
     }
 }
#endif
 
 
 // Compare the hash between QtyItem.h and FlashPQ QtyHeader.
 static UINT8 bDrvCheckFlashPqQtyHash(UINT8 bIndex)
 {
     UINT32 u4QtyNumber = 0;
     UINT8 i;
 
     // Get the hash from QtyItem.h
     u4QtyNumber = dwDrvCalcFlashPqQtyNumber(bIndex);
     if (u4QtyNumber == 0)
     {
         LOG(0, "QTY item error!\n");
         return SV_FALSE;
     }
     u4QtyNumber ++; // Get the next position after the QUALITY_MAX entry.
 
     // Compare the hashs.
     for (i=0; i < TBL_HEAD_HASH; i++)
     {
         // Array index range check.
         if (u4QtyNumber+i < CUSTOM_QTY_TBL_DIM + TBL_HEAD_HASH)
         {
             if (READ_CUST_QTY_ITEM(bIndex, u4QtyNumber+i) != aFlashPqQtyHead[bIndex].bHash[i])
             {
                 LOG(0, "hash error at %d\n", i);
                 return SV_FALSE;
             }
         }
         else
         {
             LOG(0, "Qty index range error %d\n", u4QtyNumber+i);
             return SV_FALSE;
         }
     }
     // Compare finished.
     return SV_TRUE;
 }
 
 void vApiFlashPqReadFlash(UINT8 bIndex, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
 {
    struct file *file = file_open((bIndex==0)?("/ro/pcf.bin"):("/ro/pqf.bin"), O_RDONLY, 0);

    if (!file)
    {
        memset((void*)u4MemPtr, 0, u4MemLen);
        //return 0;
    }

    u4MemLen = file_read(file, (void*)u4MemPtr, u4MemLen, &u8Offset);

    file_close(file);
 }
 
 UINT8 * bApiFlashPqGetVersion(UINT8 bIndex, UINT8 bType)
 {
     switch (bType)
     {
         default:
         case FLASHPQ_XML_VER:
             return (UINT8 *)(aFlashPqFileHead[bIndex].bQtyXmlVer);
         case FLASHPQ_GAMMA_VER:
             return (UINT8 *)(aFlashPqGammaHead.bScript);
         case FLASHPQ_SCE_VER:
             return (UINT8 *)(aFlashPqSceHead.bScript);
         case FLASHPQ_QTY_VER:
             return (UINT8 *)(aFlashPqQtyHead[bIndex].bScript);
         case FLASHPQ_OD_VER:
             return (UINT8 *)(aFlashPqOdHead.bScript);            
     }
 }

 UINT8 bApiGetPcfPqfVersion(PCF_PQF_INFO *prPcfPqfInfo)
{
    if(prPcfPqfInfo == NULL)
    {
        LOG(0, "Pointer is Null, Get Pcf Pqf info fail !\n");
        return (SV_FALSE);
    }

    x_strcpy((CHAR *) (prPcfPqfInfo->bPcfVer), (CHAR *) (aFlashPqFileHead[0].bQtyXmlVer));
    x_strcpy((CHAR *) (prPcfPqfInfo->bPqfVer), (CHAR *) (aFlashPqFileHead[FLASHPQ_NUM-1].bQtyXmlVer));
    
    if(IsSupportLongName(0))
    {
        x_strcpy((CHAR *) (prPcfPqfInfo->bPcfPanelName), (CHAR *) (aPcfPqfPanelLongName[0].bScript));
    }
    else
    {
        x_strcpy((CHAR *) (prPcfPqfInfo->bPcfPanelName), (CHAR *) (aFlashPqQtyHead[0].bScript));
    }
    
    if((FLASHPQ_NUM == 2) && (IsSupportLongName(FLASHPQ_NUM-1)))
    {
        x_strcpy((CHAR *) (prPcfPqfInfo->bPqfPanelName), (CHAR *) (aPcfPqfPanelLongName[FLASHPQ_NUM-1].bScript));
    }
    else
    {
        x_strcpy((CHAR *) (prPcfPqfInfo->bPqfPanelName), (CHAR *) (aFlashPqQtyHead[FLASHPQ_NUM-1].bScript));
    }
    
    LOG(3, "bPcfVer = %s, bPcfPanelName = %s, bPqfVer = %s, bPqfPanelName = %s, \n",
        prPcfPqfInfo->bPcfVer, prPcfPqfInfo->bPcfPanelName, 
        prPcfPqfInfo->bPqfVer, prPcfPqfInfo->bPqfPanelName);
    
    return (SV_TRUE);
}
 
 UINT8 bApiFlashPqVerifyHeader(UINT8 bIndex)
 {
     FLASHPQ_INDEX_CHECK(bIndex);

     if (DRVCUST_OptGet(eFlagFlashPqEnable))
     {
         // Check if flash PQ address & block size definition.
        #ifndef LOAD_FLASH_FROM_FILE
        #ifndef CC_MTK_LOADER  
         if ((((UINT64)DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
             DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) == 0) ||
             (DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBlockSize) == 0))
        #else
         if ((((UINT64)DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
             DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) == 0) ||
             (DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBlockSize) == 0))
		#endif
         {
             LOG(0, "FlashPQ[%d] base address is zero !\n",bIndex);
             return (SV_FALSE);
         }
        #endif
         // Get PQ base address.
        #ifndef CC_MTK_LOADER 
         u8PqBaseAddr[bIndex] = 
            #ifndef LOAD_FLASH_FROM_FILE
            ((UINT64)DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            #endif
             DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) + DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashOffset);
		#else
         u8PqBaseAddr[bIndex] = 
            #ifndef LOAD_FLASH_FROM_FILE
            ((UINT64)DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            #endif
             DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) + DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashOffset);
		#endif
 
         // PQ base address must be 32-bit alignment.
         if ((u8PqBaseAddr[bIndex] % 4) != 0)
         {
             LOG(0, "FlashPQ[%d] base address is not 32-bit align !\n",bIndex);
             ASSERT(0);
         }
 
         vApiFlashPqReadFlash(bIndex, u8PqBaseAddr[bIndex], 
             (UINT32)&aFlashPqFileHead[bIndex], (UINT32)(sizeof(struct tagFileHead)));
 
         if ((x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID) == 0) ||
             (x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID_EX) == 0))
         {            
             if (x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID_EX) == 0)
             {
                 bFileHeadSize[bIndex] = sizeof(struct tagFileHead);
                 bPqExtesion[bIndex] = SV_TRUE;
             }
             else
             {  
                 bFileHeadSize[bIndex] = sizeof(struct tagFileHead) - 32;                   
                 bPqExtesion[bIndex] = SV_FALSE;
                 LOG(0, "Old version Flash PQ binary \n");
             }
             
             if ((aFlashPqFileHead[bIndex].wNumOfGamma>>8) == 48)
             {
                 aFlashPqFileHead[bIndex].wNumOfGamma = aFlashPqFileHead[bIndex].wNumOfGamma & 0xFF;
             }
             if ((aFlashPqFileHead[bIndex].wNumOfSCE>>8) == 48)
             {
                 aFlashPqFileHead[bIndex].wNumOfSCE= aFlashPqFileHead[bIndex].wNumOfSCE & 0xFF;
             }
             if ((aFlashPqFileHead[bIndex].wNumOfQty>>8) == 48)
             {
                 aFlashPqFileHead[bIndex].wNumOfQty = aFlashPqFileHead[bIndex].wNumOfQty & 0xFF;
             }
 
             LOG(0, "FlashPQ[%d] ID OK\n",bIndex);
             bFlashPQInit[bIndex] = SV_TRUE;
 
            
            #if ENABLE_FLASH_PQ_DEBUG
             vDrvDebugFileHeader(bIndex);
            #endif
 
             return (SV_TRUE);
         }
         else
         {
             bFlashPQInit[bIndex] = SV_FALSE;
             LOG(0, "FlashPQ[%d] ID NG\n",bIndex);
             return (SV_FALSE);
         }
     }
     else
     {
         LOG(0, "Not Support FlashPQ\n");
         return (SV_FALSE);
     }
 }
 
 UINT8 u1DrvCheckGammaFlashPQSel(UINT16 u2GammaIndex)
 {
     UINT8 i;
     
     for (i=0; i< FLASHPQ_NUM; i++)
     {
         if (bFlashPQInit[i] != SV_TRUE)
         {
             LOG(0, "Update Gamma Check init failed ----  %d\n", i);
         }
         else if (DRVCUST_OptGet(aFlashPqSWInfo[i].dwSwGammaEnable) == 0)
         {
             LOG(0, "FlashPQ[%d] not support Gamma ---- \n", i);
         }
         else if (u2GammaIndex >= aFlashPqFileHead[i].wNumOfGamma)
         {
             LOG(0, "FlashPQ[%d] Gamma Index Error! %d > %d\n", i, u2GammaIndex, aFlashPqFileHead[i].wNumOfGamma);
         }
         else
         {
             return i;
         }    
     }
 
     return 0xFF;
 }
 
 UINT8 bApiFlashPqUpdateGamma(void)
 {
     UINT8 bi;
     UINT8* u1GammaBufPtr;
     UINT64 u8GammaAddr;
     UINT16 u2GammaMdlSize;
     UINT16 u2GammaChlSize;
     UINT16 u2GammaIndex = vApiFlashPqGetGammaId();
     UINT8 u1Index = 0;
     
     if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
     {
         return (SV_FALSE);
     }
 
     u1Index = u1DrvCheckGammaFlashPQSel(u2GammaIndex);
     
     if (u1Index == 0xFF)
     {
         return (SV_FALSE);
     }
 
     // Calculate start address of Gamma table.
     u8GammaAddr = u8CalcFlashPqOffset(u1Index, TYPE_GAMMA_HEADER, u2GammaIndex, 0, 0, 0);
 
     // Gamma head base address must be 32-bit alignment.
     if ((u8GammaAddr % 4) != 0)
     {
         LOG(0, "FlashPQ gamma address error !(%d)\n", u8GammaAddr);
         return (SV_FALSE);
     }
 
     // Check Gamma table size in FlashPQ bin file: 256 or 257
     if (aFlashPqFileHead[u1Index].wSizeOfGammaTable == 
         FLASH_PQ_GAMMA_CHANNEL_SIZE_256 * FLASH_PQ_GAMMA_CHANNEL_NUM)
     {
         u2GammaChlSize = FLASH_PQ_GAMMA_CHANNEL_SIZE_256;
     }
     else if (aFlashPqFileHead[u1Index].wSizeOfGammaTable == 
         FLASH_PQ_GAMMA_CHANNEL_SIZE_256 * FLASH_PQ_GAMMA_CHANNEL_NUM + 16)
     {
         u2GammaChlSize = FLASH_PQ_GAMMA_CHANNEL_SIZE_257;
     }
     else
     {
         LOG(0, "FlashPQ Gamma size error !\n");
         return (SV_FALSE);
     }
     
     u2GammaMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead[u1Index].wSizeOfGammaTable;
     if ((u1GammaBufPtr = (UINT8*)x_mem_alloc(u2GammaMdlSize)) == NULL)
     {
         LOG(0, "FlashPQ Gamma memory request fail !\n");
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(u1Index, u8GammaAddr, (UINT32)u1GammaBufPtr, (UINT32)u2GammaMdlSize);
 
     x_memcpy(&aFlashPqGammaHead, u1GammaBufPtr, TBL_HEAD_SIZE);
 
     LOG(1, "\nGamma ID        %s\n", aFlashPqGammaHead.bID);
     LOG(1, "PANEL NO        %d\n", u2GammaIndex);
     LOG(1, "DATE            %s\n", aFlashPqGammaHead.bDate);
     LOG(1, "SCRIPT          %s\n", aFlashPqGammaHead.bScript);
 
     // Check Gamma ID
     if (x_strcmp((CHAR *) (aFlashPqGammaHead.bID), (CHAR *) GAMMA_ID) != 0)
     {
         LOG(0, "Gamma ID [%d]  Error  %s\n", u2GammaIndex, aFlashPqGammaHead.bID);
         x_mem_free(u1GammaBufPtr);
         return (SV_FALSE);
     }
     
     for (bi = 0; bi < FLASH_PQ_GAMMA_CHANNEL_NUM; bi ++)
     {
         x_memcpy(&GAMMA_256[bi], 
             u1GammaBufPtr + TBL_HEAD_SIZE + u2GammaChlSize * bi, 
             u2GammaChlSize);
 
         // Calculate point 257 if bin file only store 256 points
         if (u2GammaChlSize == FLASH_PQ_GAMMA_CHANNEL_SIZE_256)
         {
             vDrvGammaTopPt(GAMMA_256[bi]);
         }
     }
 
    #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_GAMMA_LOG))
     {
         vDrvDebugGamma();
     }
    #endif
     
     x_mem_free(u1GammaBufPtr);
     LOG(0, "FlashPQ Gamma OK\n");
     return (SV_TRUE);
 }
 
 UINT8 u1DrvCheckSCEFlashPQSel(UINT16 u2SCEIndex)
 {
     UINT8 i;
     
     for (i=0; i< FLASHPQ_NUM; i++)
     {
         if (bFlashPQInit[i] != SV_TRUE)
         {
             LOG(0, "Update SCE Check init failed  ----  %d\n", i);
         }    
         else if (DRVCUST_OptGet((aFlashPqSWInfo[i].dwSwSceEnable)) == 0)
         {
             LOG(0, "FlashPQ[%d] not support SCE\n", i);
         }
         else if (u2SCEIndex >= aFlashPqFileHead[i].wNumOfSCE)
         {
             LOG(0, "FlashPQ[%d] SCE Index Error! %d >= %d\n", i, u2SCEIndex, aFlashPqFileHead[i].wNumOfSCE);
         }
         else
         {
             return i;
         }
     }
 
     return 0xFF;    
 }
 
 UINT8 bApiFlashPqUpdateSCE(void)
 {
     UINT8* u1SceBufPtr;
     UINT64 u8SceAddr;
     UINT16 u2SceMdlSize;
     UINT16 u2SceIndex = vApiFlashPqGetSceId();
     UINT8 u1Index = 0;
     
     if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
     {
         return (SV_FALSE);
     }
 
     u1Index = u1DrvCheckSCEFlashPQSel(u2SceIndex);
 
     if (u1Index == 0xFF)
     {
         return (SV_FALSE);
     }    
 
     // Calcuate start address of SCE table.
     u8SceAddr = u8CalcFlashPqOffset(u1Index, TYPE_SCE_HEADER, u2SceIndex, 0, 0, 0);
 
     // SCE head base address must be 32-bit alignment.
     if ((u8SceAddr % 4) != 0)
     {
         LOG(0, "FlashPQ SCE address error!!!(%d)\n", u8SceAddr);
         return (SV_FALSE);
     }
 
     u2SceMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead[u1Index].wSizeOfSCETable;
 
     if ((u1SceBufPtr = (UINT8*)x_mem_alloc(u2SceMdlSize)) == NULL)
     {
         LOG(0, "FlashPQ SCE memory request fail\n");
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(u1Index, u8SceAddr, (UINT32)u1SceBufPtr, (UINT32)u2SceMdlSize);
 
     x_memcpy(&aFlashPqSceHead, u1SceBufPtr, TBL_HEAD_SIZE);
 
     LOG(1, "\nDef SCE ID      %s\n", aFlashPqSceHead.bID);
     LOG(1, "Def SCE NO      %d\n", u2SceIndex);
     LOG(1, "DATE            %s\n", aFlashPqSceHead.bDate);
     LOG(1, "SCRIPT          %s\n", aFlashPqSceHead.bScript);
 
     // Check SCE ID
     if (x_strcmp((CHAR *) (aFlashPqSceHead.bID), (CHAR *) SCE_ID) != 0)
     {
         LOG(0, "Def SCE ID [%d]  Error  %s\n", u2SceIndex, aFlashPqSceHead.bID);
         x_mem_free(u1SceBufPtr);
         return (SV_FALSE);
     }
 
     if (aFlashPqFileHead[u1Index].wSizeOfSCETable > 960)
     {
         LOG(0, "SCE table size %d is too big\n", aFlashPqFileHead[u1Index].wSizeOfSCETable);
         x_mem_free(u1SceBufPtr);
         return (SV_FALSE);
     }
     
     x_memcpy(&DEFAULT_SCE_TBL, (u1SceBufPtr + TBL_HEAD_SIZE), aFlashPqFileHead[u1Index].wSizeOfSCETable);
     x_memcpy(&BASE_SCE_TBL, &DEFAULT_SCE_TBL, aFlashPqFileHead[u1Index].wSizeOfSCETable);
 
     if (DRVCUST_OptGet(eFlagFlashPqSCEMin) == SV_TRUE) // Normanl Mode
     {        
         u2SceIndex = vApiFlashPqGetMinSceId();
         
         if (u2SceIndex >= aFlashPqFileHead[u1Index].wNumOfSCE)
         {
             LOG(0, "SCE Index Error! %d >= %d \n", u2SceIndex, aFlashPqFileHead[u1Index].wNumOfSCE);
             x_mem_free(u1SceBufPtr);
             return (SV_FALSE);
         }
         
         u8SceAddr = u8CalcFlashPqOffset(u1Index, TYPE_SCE_HEADER, u2SceIndex, 0, 0, 0);
 
         if ((u8SceAddr % 4) != 0)
         {
             LOG(0, "FlashPQ sce address error!!!(%d)\n", u8SceAddr);
             x_mem_free(u1SceBufPtr);
             return (SV_FALSE);
         }
         
         vApiFlashPqReadFlash(u1Index, u8SceAddr, (UINT32)u1SceBufPtr, (UINT32)u2SceMdlSize);
         
         x_memcpy(&aFlashPqSceHead, u1SceBufPtr, TBL_HEAD_SIZE);
         
         LOG(1, "\nMin SCE ID      %s\n", aFlashPqSceHead.bID);
         LOG(1, "Min SCE NO      %d\n", u2SceIndex);
         LOG(1, "DATE            %s\n", aFlashPqSceHead.bDate);
         LOG(1, "SCRIPT          %s\n", aFlashPqSceHead.bScript);
 
         if (x_strcmp((CHAR *) (aFlashPqSceHead.bID), (CHAR *) SCE_ID) != 0)
         {
             LOG(0, "Min SCE ID [%d]  Error  %s\n", u2SceIndex, aFlashPqSceHead.bID);
             x_mem_free(u1SceBufPtr);
             return (SV_FALSE);
         }
         
         x_memcpy(&MIN_SCE_TBL, (u1SceBufPtr + TBL_HEAD_SIZE), aFlashPqFileHead[u1Index].wSizeOfSCETable);
     }
     
    #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_SCE_LOG))
     {
         vDrvDebugSCE(u1Index);
     }
    #endif
     
     x_mem_free(u1SceBufPtr);    
     LOG(0, "FlashPQ SCE OK\n");
     return (SV_TRUE);
 }
 
 UINT8 bApiFlashPqSystemInfo(void)
 {
     // support flashpq info only when flashpq enabled
     if(DRVCUST_OptGet(eFlagFlashPqEnable))
     {
         LOG(0, "[FLASHPQ] SUPPORT_FLASH_PQ\n");            
         LOG(0, "[FLASHPQ] FLASH_PQ_BASE_PARTITION = %d\n", DRVCUST_OptGet(eFlashPqBasePartition));
        #ifdef SUPPORT_2ND_FLASHPQ
         LOG(0, "[FLASHPQ] FLASH_PQ_2ndpart_BASE_PARTITION = %d\n", DRVCUST_OptGet(eFlashPq2ndpartBasePartition));
        #endif
         
         if(DRVCUST_OptGet(eFlashPqUseNandFalsh))
         {
             LOG(0, "[FLASHPQ] FLASH_PQ_USE_NAND_FLASH\n");                        
             LOG(0, "[FLASHPQ] NAND_PART_SIZE = 0x%x\n", 
                 DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPqBasePartition))));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPqBaseAddress));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPqOffset));                                           
             
            #ifdef SUPPORT_2ND_FLASHPQ
             LOG(0, "\n[FLASHPQ] NAND_2nd_PART_SIZE = 0x%x\n", 
                 DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPq2ndpartBasePartition))));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartBaseAddress));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartOffset));                                           
            #endif
         }
         else
         {
             LOG(0, "[FLASHPQ] FLASH_PQ_USE_NOR_FLASH\n");                
             LOG(0, "[FLASHPQ] NOR_PART_SIZE = 0x%x\n", 
                 DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPqBasePartition))));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPqBaseAddress));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPqOffset));                                                       
 
            #ifdef SUPPORT_2ND_FLASHPQ
             LOG(0, "\n[FLASHPQ] NAND_2nd_PART_SIZE = 0x%x\n", 
                 DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPq2ndpartBasePartition))));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartBaseAddress));                    
             LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartOffset));                                           
            #endif
         }
     }
     else
     {
         LOG(0, "[FLASHPQ] NOT_SUPPORT_FLASH_PQ\n");            
     }
     return 0;
 }
 
 UINT8 bApiFlashPqUpdateQtyDft(UINT8 bIndex)
 {
     UINT8* u1QtyPtr;
     UINT8* u1QtyBufPtr;
     UINT8 bSmartPicIndex;
     UINT8 bQtyScriptSize = QTY_SCRIPT_SIZE(bIndex);
     UINT16 u2QtyMdlSize, i;
     UINT16 u2QtyIndex = vApiFlashPqGetQtyId(bIndex);
     UINT64 u8QtyAddr;
 
     if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
     {
         return (SV_FALSE);
     }
 
     if (bFlashPQInit[bIndex] != SV_TRUE)
     {
         LOG(0, "FlashPQ[%d] Update Qty Check init failed\n",bIndex);
         return (SV_FALSE);
     }
 
     if (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtyEnable) == 0)
     {
         LOG(0, "FlashPQ[%d] not support QtyTable\n",bIndex);
         return (SV_FALSE);
     }
  
     if (u2QtyIndex >= aFlashPqFileHead[bIndex].wNumOfQty)
     {
         LOG(0, "FlashPQ[%d] Qty Index Error %d >= %d!\n", bIndex, u2QtyIndex, aFlashPqFileHead[bIndex].wNumOfQty);
         return (SV_FALSE);
     }
 
     // Check SmartPic ID
     bSmartPicIndex = DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) ? vApiFlashPqGetSmartPicId() : 0;
     
     if (bSmartPicIndex >= aFlashPqFileHead[bIndex].bNumOfSmartPic)
     {
         LOG(0, "FlashPQ[%d] SmartPic Index Error!\n",bIndex);
         return (SV_FALSE);
     }
 
     // Read Qty header only
     // Calculate start address of Qty table header.
     u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_HEADER, u2QtyIndex, 0, 0, 0);
     
     // Qty head base address must be 32-bit alignment.
     if ((u8QtyAddr % 4) != 0)
     {
         LOG(0, "FlashPQ[%d] qty address error!!!(%d)\n", bIndex, u8QtyAddr);
         return SV_FALSE;
     }
     
     if ((u1QtyBufPtr = (UINT8*)x_mem_alloc(TBL_HEAD_SIZE)) == NULL)
     {
         LOG(0, "FlashPQ[%d] Qty memory request fail\n",bIndex);
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)u1QtyBufPtr, (UINT32)TBL_HEAD_SIZE);
 
     x_memcpy(&aFlashPqQtyHead[bIndex], u1QtyBufPtr, TBL_HEAD_SIZE);
     x_mem_free(u1QtyBufPtr);
 
     LOG(1, "\nQTY NO      %d\n", u2QtyIndex);
     LOG(1, "DATE        %s\n", aFlashPqQtyHead[bIndex].bDate);
     LOG(1, "PIC ID      %d\n", bSmartPicIndex);
     LOG(1, "SCRIPT      %s\n", aFlashPqQtyHead[bIndex].bScript);
     
     if(IsSupportLongName(bIndex))    // for vestel support longname
     {
         vApiFlashPqReadFlash(bIndex, (u8QtyAddr+TBL_HEAD_SIZE), (UINT32)&aPcfPqfPanelLongName[bIndex], (UINT32)TBL_LONG_PANEL_NAME_SIZE);
         LOG(1,"Panel Long Name SCRIPT      %s\n",aPcfPqfPanelLongName[bIndex].bScript);
     }
 
     // Read Qty table content
     u2QtyMdlSize = aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming;
 
     // Calculate start address of Qty table content.
     u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_DESCRIPT, u2QtyIndex, bSmartPicIndex, 0, 0);
     
     // Qty head base address must be 32-bit alignment.
     if ((u8QtyAddr % 4) != 0)
     {
         LOG(0, "FlashPQ[%d] qty address error!!!(%d)\n", bIndex, u8QtyAddr);
         return SV_FALSE;
     }
     
     if ((u1QtyBufPtr = (UINT8*)x_mem_alloc(u2QtyMdlSize)) == NULL)
     {
         LOG(0, "FlashPQ[%d] Qty memory request fail\n",bIndex);
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)u1QtyBufPtr, (UINT32)u2QtyMdlSize);
 
     // Check Qty ID
     if (x_strcmp((CHAR *) (aFlashPqQtyHead[bIndex].bID), (CHAR *) QTY_ID) != 0)
     {
         LOG(0, "FlashPQ[%d] QTY ID NG\n",bIndex);
         x_mem_free(u1QtyBufPtr);
         return (SV_FALSE);
     }
 
     // Check the hash info inside the Qty header.
     if (aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_HASH)
     {
         if (!bDrvCheckFlashPqQtyHash(bIndex))
         {
             LOG(0, "FlashPQ[%d] QTY hash error!\n",bIndex);
             x_mem_free(u1QtyBufPtr);
             return (SV_FALSE);
         }
     }
 
     // Check QTY table size.
     if (aFlashPqFileHead[bIndex].wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
     {
         LOG(0, "FlashPQ[%d] qty table size error!!!(%d!=%d)\n",
             bIndex,aFlashPqFileHead[bIndex].wSizeOfQtyTable, CUSTOM_QTY_TBL_DIM);
             x_mem_free(u1QtyBufPtr);
         return (SV_FALSE);
     }
 
     u1QtyPtr = u1QtyBufPtr;
 
     for (i = 0; i < aFlashPqFileHead[bIndex].bNumOfTiming; i ++)
     {           
         x_memcpy(&(bCustomQtyTbl[bIndex][i]), u1QtyPtr + bQtyScriptSize, 
                 aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);
 
         u1QtyPtr += aFlashPqFileHead[bIndex].wSizeOfQtyTable;
     }
 
    #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_QTY_LOG))
     {
         vDrvDebugQtyTable(bIndex);
     }
    #endif
     
     x_mem_free(u1QtyBufPtr);
     LOG(0, "FlashPQ[%d] Qty OK\n",bIndex);
     return (SV_TRUE);
 }
 
 UINT8 bApiFlashPqUpdateQtyMinMax(UINT8 u1VidPath, UINT8 bIndex)
 {
     UINT64 u8QtyAddr;
     UINT16 i;
     UINT8 u1SrcTypTmg;
     UINT8 bQtyScriptSize = QTY_SCRIPT_SIZE(bIndex);
     UINT16 u2QtyIndex = vApiFlashPqGetQtyId(bIndex);
 
     if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
     {
         return (SV_FALSE);
     }
 
     if (_fgAutoSearch == TRUE)
     {
         LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
         return SV_FALSE;
     }
 
     if (bFlashPQInit[bIndex] != SV_TRUE)
     {
         LOG(0, "FlashpQ[%d] Update MinMax Check init failed\n",bIndex);
         return (SV_FALSE);
     }
 
     // Check QtyTable Enable
     if (((aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_MAX_MIN) == 0) ||
         (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtyEnable) == 0) ||
         (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) == 0))
     {
         LOG(0, "FlashPQ[%d] not support Qty Min/Max\n",bIndex);
         return (SV_FALSE);
     }
 
     // Check QTY table size.
     if (aFlashPqFileHead[bIndex].wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
     {
         LOG(0, "FlashPQ[%d] qty table size error!!!(%d!=%d)\n",
             bIndex,aFlashPqFileHead[bIndex].wSizeOfQtyTable, CUSTOM_QTY_TBL_DIM);
         return (SV_FALSE);
     }
 
     for (i=0; i< CUSTOM_QTY_TBL_DIM; i++)
     {
         bQtyMin[bIndex][i] = 0;
         bQtyMax[bIndex][i] = 0;
     }
 
     u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);
 
     // Transfrom the QtyIndex.
     // Find a match SourceTypeTiming in CustomQtyIdx[].
     u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);
 
     if (u1SrcTypTmg == SOURCE_TYPE_TIMING_MAX)
     {
         return (SV_FALSE);
     }
 
     // Calculate start address of Qty table.
     u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_CONTENT, u2QtyIndex, aFlashPqFileHead[bIndex].bNumOfSmartPic, u1SrcTypTmg, 0);
 
     // Qty head base address must be 32-bit alignment.
     if ((u8QtyAddr % 4) != 0)
     {
         LOG(0, "FlashPQ[%d] qty address error!!!(%d)\n", bIndex,u8QtyAddr);
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)&bQtyMin[bIndex],
         (UINT32)aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);
     
     // Calculate start address of Qty table.
     u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_CONTENT, u2QtyIndex, aFlashPqFileHead[bIndex].bNumOfSmartPic+1, u1SrcTypTmg, 0);
 
     // Qty head base address must be 32-bit alignment.
     if ((u8QtyAddr % 4) != 0)
     {
         LOG(0, "FlashPQ[%d] qty table address error!!!(%d)\n",bIndex, u8QtyAddr);
         return (SV_FALSE);
     }
     
     vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)&bQtyMax[bIndex],
         (UINT32)aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);
 
     for (i=0; i < QUALITY_MAX; i++)
     {
         if ((READ_DEFUALT_QTY_TBL_MIN(i) == 0xFF) &&
             (READ_DEFUALT_QTY_TBL_MAX(i) == 0xFF) &&
             (READ_DEFUALT_QTY_TBL_DFT(i) == 0xFF))
         {
             break;
         }
 
         if (((READ_DEFUALT_QTY_TBL_REF(i) != FROM_DFT) &&
             ((READ_DEFUALT_QTY_TBL_REF(i) & (0x3FFF)) < CUSTOM_QTY_TBL_DIM)) &&
             ((UINT8)(READ_DEFUALT_QTY_TBL_REF(i)>>14) == bIndex))
         {
             WRITE_DEFUALT_QTY_TBL_MIN(i, bQtyMin[bIndex][(READ_DEFUALT_QTY_TBL_REF(i) & 0x3FFF)]);
             WRITE_DEFUALT_QTY_TBL_MAX(i, bQtyMax[bIndex][(READ_DEFUALT_QTY_TBL_REF(i) & 0x3FFF)]);
         }
     }
 
    #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_QTY_MIN_MAX_LOG))
     {
         vDrvDebugQtyMinMax(bIndex);
     }
    #endif
 
     LOG(0, "FlashPQ[%d] Qty Min/Max OK\n",bIndex);
     return (SV_TRUE);
 }

//Add for vestel --- pingan 0606
 UINT8 bApiFlashPqUpdateQtyMinMaxAllSrc(UINT8 u1VidPath, UINT8 bIndex, UINT8 u1SrcTypTmg)
 {
     UINT64 u8QtyAddr;
     UINT16 i;
     //UINT8 u1SrcTypTmg;
     UINT8 bQtyScriptSize = QTY_SCRIPT_SIZE(bIndex);
     UINT16 u2QtyIndex = vApiFlashPqGetQtyId(bIndex);
 
     if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
     {
         return (SV_FALSE);
     }
 
     if (_fgAutoSearch == TRUE)
     {
         LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
         return SV_FALSE;
     }
 
     if (bFlashPQInit[bIndex] != SV_TRUE)
     {
         LOG(0, "FlashPQ[%d] Update MinMax Check init failed\n",bIndex);
         return (SV_FALSE);
     }
 
     // Check QtyTable Enable
     if (((aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_MAX_MIN) == 0) ||
         (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtyEnable) == 0) ||
         (DRVCUST_OptGet(aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) == 0))
     {
         LOG(0, "FlashPQ[%d] not support Qty Min/Max\n",bIndex);
         return (SV_FALSE);
     }
 
     // Check QTY table size.
     if (aFlashPqFileHead[bIndex].wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
     {
         LOG(0, "FlashPQ[%d] qty table size error!!!(%d!=%d)\n",
             bIndex, aFlashPqFileHead[bIndex].wSizeOfQtyTable, CUSTOM_QTY_TBL_DIM);
         return (SV_FALSE);
     }
 
     for (i=0; i< CUSTOM_QTY_TBL_DIM; i++)
     {
         bQtyMin[bIndex][i] = 0;
         bQtyMax[bIndex][i] = 0;
     }
 
     //u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);
 
     // Transfrom the QtyIndex.
     // Find a match SourceTypeTiming in CustomQtyIdx[].
     //u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);
 
     if (u1SrcTypTmg == SOURCE_TYPE_TIMING_MAX)
     {
         return (SV_FALSE);
     }
 
     // Calculate start address of Qty table.
     u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_CONTENT, u2QtyIndex, aFlashPqFileHead[bIndex].bNumOfSmartPic, u1SrcTypTmg, 0);
 
     // Qty head base address must be 32-bit alignment.
     if ((u8QtyAddr % 4) != 0)
     {
         LOG(0, "FlashPQ[%d] qty address error!!!(%d)\n", bIndex,u8QtyAddr);
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)&bQtyMin[bIndex],
         (UINT32)aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);
     
     // Calculate start address of Qty table.
     u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_CONTENT, u2QtyIndex, aFlashPqFileHead[bIndex].bNumOfSmartPic+1, u1SrcTypTmg, 0);
 
     // Qty head base address must be 32-bit alignment.
     if ((u8QtyAddr % 4) != 0)
     {
         LOG(0, "FlashPQ[%d] qty table address error!!!(%d)\n", bIndex, u8QtyAddr);
         return (SV_FALSE);
     }
     
     vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)&bQtyMax[bIndex],
         (UINT32)aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);
 
     for (i=0; i < QUALITY_MAX; i++)
     {
         if ((READ_DEFUALT_QTY_TBL_MIN(i) == 0xFF) &&
             (READ_DEFUALT_QTY_TBL_MAX(i) == 0xFF) &&
             (READ_DEFUALT_QTY_TBL_DFT(i) == 0xFF))
         {
             break;
         }
 
         if (((READ_DEFUALT_QTY_TBL_REF(i) != FROM_DFT) &&
             ((READ_DEFUALT_QTY_TBL_REF(i) & (0x3FFF)) < CUSTOM_QTY_TBL_DIM)) &&
             ((UINT8)(READ_DEFUALT_QTY_TBL_REF(i)>>14) == bIndex))
         {
             WRITE_DEFUALT_QTY_TBL_MIN(i, bQtyMin[bIndex][(READ_DEFUALT_QTY_TBL_REF(i) & 0x3FFF)]);
             WRITE_DEFUALT_QTY_TBL_MAX(i, bQtyMax[bIndex][(READ_DEFUALT_QTY_TBL_REF(i) & 0x3FFF)]);
         }
     }
 
   #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_QTY_MIN_MAX_LOG))
     {
         vDrvDebugQtyMinMax(bIndex);
     }
   #endif
 
     LOG(0, "AllSrc FlashPQ[%d] Qty Min/Max OK\n",bIndex);
     return (SV_TRUE);
 }

 
 UINT8 u1DrvCheckODFlashPQSel(UINT16 u2ODIndex)
 {
     UINT8 i;
     
     for (i=0; i < FLASHPQ_NUM; i++)
     {
         if (bFlashPQInit[i] != SV_TRUE)
         {
             LOG(0, "Update OD Check init failed  ----  %d\n", i);
         }
         else if (bPqExtesion[i] == 0)
         {
             LOG(0, "FlashPQ[%d] not support OD\n", i);
         }
         else if (u2ODIndex >= aFlashPqFileHead[i].wNumOfOd)
         {
             LOG(0, "FlashPQ[%d] OD Index Error %d >= %d!\n", i, u2ODIndex, aFlashPqFileHead[i].wNumOfOd);
         }
         else
         {
             return i;
         }
     }
     
     return 0xFF;    
 }

 INT16 vApiFlashPqUpdateUiRange(UINT8 bQtyTiming, UINT8 bUiItem)
{
    INT16 u1DftRange;    
    
    switch(bUiItem)
    {
        case UI_SMART_PIC_CONTRAST:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 69); //Contrast Dft Range  in PCF
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_CONTRASTDFT));
            break;
        case UI_SMART_PIC_BRIGHTNESS:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 70);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_BRIGHTNESSDFT));
            break;
        case UI_SMART_PIC_SATURATION:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 71);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_SATURATIONDFT));
            break;
        case UI_SMART_PIC_HUE:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 72);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_HUEDFT));
            break;
        case  UI_SMART_PIC_SHAPRNESS:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 73);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_SHARPNESSDFT));
            break;
        case  UI_SMART_PIC_SKIN_TONE:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 74);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_SKINTONEDFT));
            break;
        case  UI_SMART_PIC_R_GAIN:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 75);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_REDGAINDFT));
            break;   
        case  UI_SMART_PIC_G_GAIN:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 76);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_GREENGAINDFT));
            break;    
        case  UI_SMART_PIC_B_GAIN:
            //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 77);
            u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_BLUEGAINDFT));
            break;      
        default:
            u1DftRange = -1;
            break;            
    }

    return u1DftRange;
}

//Begin add for Vestel ---pingan 0606
 INT16 vApiFlashPqUpdateUi3DRange(UINT8 bQtyTiming, UINT8 bUiItem)
 {
     INT16 u1DftRange;    
     
     switch(bUiItem)
     {
         case UI_SMART_PIC_CONTRAST:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 114);//69); //Contrast Dft Range  in PCF
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_CONTRASTDFT));
             break;
         case UI_SMART_PIC_BRIGHTNESS:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 115);//70);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_BRIGHTNESSDFT));
             break;
         case UI_SMART_PIC_SATURATION:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 116);//71);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_SATURATIONDFT));
             break;
         case UI_SMART_PIC_HUE:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 117);//72);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_HUEDFT));
             break;
         case  UI_SMART_PIC_SHAPRNESS:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 118);//73);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_SHARPNESSDFT));
             break;
         case  UI_SMART_PIC_SKIN_TONE:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 119);//74);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_SKINTONEDFT));
             break;
         case  UI_SMART_PIC_R_GAIN:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 120);//75);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_REDGAINDFT));
             break;   
         case  UI_SMART_PIC_G_GAIN:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 121);//76);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_GREENGAINDFT));
             break;    
         case  UI_SMART_PIC_B_GAIN:
             //u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming, 122);//77);
             u1DftRange = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_OSD_MAPPING_3D_BLUEGAINDFT));
             break;      
         default:
             u1DftRange = -1;
             break;            
     }
 
     return u1DftRange;
 }
 //End add for Vestel ---pingan 0606

 
INT16 vApiFlashPqUpdateBacklightDft(UINT8 bQtyTiming, UINT8 bUiItem)
{
    INT16 u1Dft;    
    
    switch(bUiItem)
    {
        case 0:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_VIDEO_PWM_MIN));
            break;
        case 1:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_VIDEO_PWM_MED));
            break;
        case 2:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_VIDEO_PWM_MAX));
            break;
        case 3:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_VIDEO_PWM_ECO));
            break;
        case 4:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_AUTO_ENABLE));
            break;
        case 5:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_AUTO_APL_PERCENT));
            break;
        case 6:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_AUTO_BLACK_PERCENT));
            break;   
        case 7:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_AUTO_PWM_MIN));
            break;    
        case 8:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_OSD_EVENT_PWM));
            break;  
        case 9:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_DECREASE_CNT));
            break; 
        case 10:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_INCREASE_CNT));
            break; 
        case 11:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_DBL_PANEL_MIN_PWM));
            break; 
        case 12:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_ECO_SWITCH_HI_LIMIT));
            break; 
        case 13:
            u1Dft = READ_CUSTOM_QTY_TBL(bQtyTiming,READ_DEFUALT_QTY_TBL_REF(QUALITY_BACKLIGHT_ECO_SWITCH_LO_LIMIT));
            break; 
        default:
            u1Dft = -1;
            break;            
    }

    return u1Dft;
}

 UINT8 bApiFlashPqUpdateOD(void)
 {
     UINT8* u1OdBufPtr;
     UINT64 u8OdAddr;
     UINT16 u2OdMdlSize;
     UINT16 u2OdFormat = aFlashPqOdHead.bConfig[0];
     UINT16 u2OdIndex = vApiFlashPqGetOdId();
     UINT16 u2OdTblSize = 0;
     UINT8 u1Index = 0;
     
     if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
     {
         return (SV_FALSE);
     }
 
     u1Index = u1DrvCheckODFlashPQSel(u2OdIndex);    
 
     if (u1Index == 0xFF)
     {
         return SV_FALSE;
     }
     
     u8OdAddr = u8CalcFlashPqOffset(u1Index, TYPE_OD_HEADER, u2OdIndex, 0, 0, 0);
 
     if ((u8OdAddr % 4) != 0)
     {
         LOG(0, "FlashPQ OD address error!!!(%d)\n", u8OdAddr);
         return (SV_FALSE);
     }
     
     u2OdMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead[u1Index].wSizeOfOdTable;
     if ((u1OdBufPtr = (UINT8*)x_mem_alloc(u2OdMdlSize)) == NULL)
     {
         LOG(0, "FlashPQ OD memory request fail\n");
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(u1Index, u8OdAddr, (UINT32)u1OdBufPtr, (UINT32)u2OdMdlSize);
 
     x_memcpy(&aFlashPqOdHead, u1OdBufPtr, TBL_HEAD_SIZE);
 
     LOG(1, "\nOD ID       %s\n", aFlashPqOdHead.bID);
     LOG(1, "OD NO           %d\n", u2OdIndex);
     LOG(1, "DATE            %s\n", aFlashPqOdHead.bDate);
     LOG(1, "SCRIPT          %s\n", aFlashPqOdHead.bScript);
     LOG(1, "Format          %d\n", aFlashPqOdHead.bConfig[0]>4);
     LOG(1, "Support OD Enable      %d\n", (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_ENABLE) ? 1 : 0);
     LOG(1, "Support FB      %d\n", (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_FB) ? 1 : 0);
     LOG(1, "Support PCID    %d\n", (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_PCID) ? 1 : 0);
     
     if (x_memcmp((void *) (aFlashPqOdHead.bID), (void *) OD_ID, TBL_HEAD_ID_SIZE) != 0)
     {
         LOG(0, "OD ID [%d] Error  %s\n", u2OdIndex, aFlashPqOdHead.bID);
         x_mem_free(u1OdBufPtr);
         return (SV_FALSE);
     }
 
     _ODParam.u1ODTblSizeMode = aFlashPqOdHead.bConfig[0] >>4;
     u2OdFormat = aFlashPqFileHead[u1Index].wConfigOfOd>>4;
     u2OdTblSize = wDrvGetODTableSize(u2OdFormat);
     
     x_memcpy(OD_Table, (u1OdBufPtr + TBL_HEAD_SIZE), u2OdTblSize);
 
     _ODParam.u1FB_ON =0;
     _ODParam.u1PCID_ON =0;
     _ODParam.u1OD_ON =0;
     
     if (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_FB)
     {
         _ODParam.u1FB_ON =1;
         x_memcpy(FB_Table, (u1OdBufPtr + TBL_HEAD_SIZE + u2OdTblSize + 15), FB_TBL_SIZE);
     }
     
     if (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_PCID)
     {
         _ODParam.u1PCID_ON =1;
         x_memcpy(PCID_Table, (u1OdBufPtr + TBL_HEAD_SIZE + u2OdTblSize + 15 + PCID_TBL_SIZE + 15), 
             PCID_TBL_SIZE);
     }
     
     if (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_ENABLE)
     {
         _ODParam.u1OD_ON =1;
     }
     
    #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_OD_LOG))
     {
         vDrvDebugOD(u2OdTblSize);
     }
    #endif
     
     x_mem_free(u1OdBufPtr);    
     LOG(0, "FlashPQ OD OK\n");
     return (SV_TRUE);
 }
 
 UINT8 u1DrvCheckPanelFlashPQSel(UINT16 u2PanleIndex)
 {
     UINT8 i;
     
     for (i=0; i < FLASHPQ_NUM; i++)
     {
         if (bFlashPQInit[i] != SV_TRUE)
         {
             LOG(0, "Update FlashPanel Check init failed  ----  %d\n", i);
         }
         else if (u2PanleIndex >= aFlashPqFileHead[i].wNumOfPanelTable)
         {
             LOG(0, "FlashPQ[%d] FlashPanel Index Error %d >= %d!\n", i, u2PanleIndex, aFlashPqFileHead[i].wNumOfPanelTable);
         }
         else
         {
             return i;
         }
     }
     
     return 0xFF;    
 }
 
 UINT8 bApiFlashPqUpdatePanelTable(void)
 {
     UINT8* u1PanelBufPtr;
     UINT64 u8PanelAddr;
     UINT16 u2PanelMdlSize;
     UINT16 u2PanelIndex = wApiFlashPqGetPanelId();
     UINT8 u1Index = 0;
     
#ifndef CC_MTK_LOADER 
         if (DRVCUST_OptGet(eFlagFlashPanelEnable) == 0)
#else
         if (DRVCUST_InitGet(eFlagFlashPanelEnable) == 0)
#endif        
         {
             LOG(0, "Not support FlashPanel\n");
             return (SV_FALSE);
         }
 
     if (u2PanelIndex == PANEL_TOTAL_NUMBER)
     {
         return SV_FALSE;
     }
 
     u1Index = u1DrvCheckPanelFlashPQSel(u2PanelIndex);    
 
     if (u1Index == 0xFF)
     {
         return SV_FALSE;
     }
     
     u8PanelAddr = u8CalcFlashPqOffset(u1Index, TYPE_PANEL_HEADER, u2PanelIndex, 0, 0, 0);
 
     if ((u8PanelAddr % 4) != 0)
     {
         LOG(0, "FlashPQ Panel address error!!!(%d)\n", u8PanelAddr);
         return (SV_FALSE);
     }
     
     u2PanelMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead[u1Index].wSizeOfPanelTable;
     if ((u1PanelBufPtr = (UINT8*)x_mem_alloc(u2PanelMdlSize)) == NULL)
     {
         LOG(0, "FlashPQ Panel memory request fail\n");
         return (SV_FALSE);
     }
 
     vApiFlashPqReadFlash(u1Index, u8PanelAddr, (UINT32)u1PanelBufPtr, (UINT32)u2PanelMdlSize);
 
     x_memcpy(&aFlashPqPanelHead, u1PanelBufPtr, TBL_HEAD_SIZE);
 
     LOG(1, "\nPanel ID       %s\n", aFlashPqPanelHead.bID);
     LOG(1, "Panel NO           %d\n", u2PanelIndex);
     LOG(1, "DATE            %s\n", aFlashPqPanelHead.bDate);
     LOG(1, "SCRIPT          %s\n", aFlashPqPanelHead.bScript);
 
     if (x_memcmp((void *) (aFlashPqPanelHead.bID), (void *) PANEL_ID, TBL_HEAD_ID_SIZE) != 0)
     {
         LOG(0, "Panel ID [%d] Error  %s\n", u2PanelIndex, aFlashPqPanelHead.bID);
         x_mem_free(u1PanelBufPtr);
         return (SV_FALSE);
     }
     
     if (*((UINT32 *)(aFlashPqPanelHead.bHash)) != FLASH_PANEL_HASH)
     {
         LOG(0, "Panel HASH = %x Error, should be %x\n", *((UINT32 *)(aFlashPqPanelHead.bHash)), FLASH_PANEL_HASH);
         return (SV_FALSE);
     }
     
     x_memcpy(&rFlashPanelTable, (u1PanelBufPtr + TBL_HEAD_SIZE), aFlashPqFileHead[u1Index].wSizeOfPanelTable);
 
    #if ENABLE_FLASH_PQ_DEBUG
     if (FLASH_PQ_LOG(E_FLASH_PQ_PANEL_LOG))
     {
         vDrvDebugPanel(aFlashPqFileHead[u1Index].wSizeOfPanelTable);
     }
    #endif
     
     x_mem_free(u1PanelBufPtr);    
     LOG(0, "FlashPQ Panel OK\n");
     return (SV_TRUE);
 }


