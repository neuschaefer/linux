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
 * $RCSfile: extmjc_8280_upgrade.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifdef DRV_SUPPORT_EXTMJC

#define FULL_ERASE_ENABLE 0

#include "extmjc_debug.h"
//#include "u_drv_cust.h"
#include "extmjc_if.h"
#include "extmjc_msg_hdlr.h"
#include "x_os.h"
#include "x_timer.h"
#include "drvcust_if.h"
#include "sif_if.h"

#define BYTE3(arg)        (*((UINT8 *)&(arg) + 3))
#define BYTE2(arg)        (*((UINT8 *)&(arg) + 2))
#define BYTE1(arg)        (*((UINT8 *)&(arg) + 1))
#define BYTE0(arg)        (* (UINT8 *)&(arg))


#define SFLASH_TIMEOUT          500000
#define SFLASH_REG_BASE         ((UINT32)0x8700)

#define SFLASH_CMD_REG          ((UINT32)0x00)
#define SFLASH_CNT_REG          ((UINT32)0x04)
#define SFLASH_RDSR_REG         ((UINT32)0x08)
#define SFLASH_RDATA_REG        ((UINT32)0x0C)
#define SFLASH_RADR0_REG        ((UINT32)0x20)
#define SFLASH_RADR1_REG        ((UINT32)0x24)
#define SFLASH_RADR2_REG        ((UINT32)0x28)
#define SFLASH_WDATA_REG        ((UINT32)0x2C)
#define SFLASH_PRGDATA0_REG     ((UINT32)0x30)
#define SFLASH_PRGDATA1_REG     ((UINT32)0x34)
#define SFLASH_PRGDATA2_REG     ((UINT32)0x38)
#define SFLASH_PRGDATA3_REG     ((UINT32)0x3C)
#define SFLASH_PRGDATA4_REG     ((UINT32)0x40)
#define SFLASH_PRGDATA5_REG     ((UINT32)0x44)
#define SFLASH_SHREG0_REG       ((UINT32)0x48)
#define SFLASH_SHREG1_REG       ((UINT32)0x4C)
#define SFLASH_SHREG2_REG       ((UINT32)0x50)
#define SFLASH_SHREG3_REG       ((UINT32)0x54)
#define SFLASH_SHREG4_REG       ((UINT32)0x58)
#define SFLASH_SHREG5_REG       ((UINT32)0x5C)
#define SFLASH_CFG1_REG         ((UINT32)0x60)
#define SFLASH_CFG2_REG         ((UINT32)0x64)
#define SFLASH_CFG3_REG         ((UINT32)0x68)
#define SFLASH_STATUS0_REG      ((UINT32)0x70)
#define SFLASH_STATUS1_REG      ((UINT32)0x74)
#define SFLASH_STATUS2_REG      ((UINT32)0x78)
#define SFLASH_STATUS3_REG      ((UINT32)0x7C)
#define SFLASH_PP_DATA_REG      ((UINT32)0x80)
#define SFLASH_PREBUF_STUS_REG  ((UINT32)0x84)
#define SFLASH_SF_INTRSTUS_REG  ((UINT32)0x88)
#define SFLASH_SF_INTREN_REG    ((UINT32)0x8C)
#define SFLASH_SF_TIME_REG      ((UINT32)0xA4)
#define SFLASH_CHKSUM_CTL_REG   ((UINT32)0xA8)
#define SFLASH_CHECKSUM_REG     ((UINT32)0xAC)
#define SFLASH_CKGEN_REG        ((UINT32)0xB0)
#define SFLASH_SAMPLE_EDGE_REG  ((UINT32)0xB4)
#define SFLASH_DUAL_REG         ((UINT32)0xC0)

#define SFLASH_WRBUF_SIZE   128
#define SFLASH_SECTOR_SIZE  64*1024

#define NORRead32(u4addr,u4data)           u1ExtmjcReadRegByI2C(SFLASH_REG_BASE+(u4addr),(UINT8*)(&u4data))
#define NORWrite32(u4addr, u4data)  u1ExtmjcWriteRegByI2C(SFLASH_REG_BASE+(u4addr), u4data)

UINT32 __nor_send_cmd(UINT8 u1cmd)
{
    UINT32 timeout = SFLASH_TIMEOUT,u4Tmp;
    NORWrite32(SFLASH_CMD_REG, u1cmd);

    while (--timeout != 0)
    {
        NORRead32(SFLASH_CMD_REG,u4Tmp);
        if ((u4Tmp & u1cmd) == 0)
            break;
    }
    if (timeout==0)
    {
        Printf("[NOR] CMD[%02X] timeout!", u1cmd);
    }
    return timeout;
}

void __nor_write_en(void)
{
    NORWrite32(SFLASH_PRGDATA5_REG, 0x06);
    NORWrite32(SFLASH_CNT_REG, 8);
    
    __nor_send_cmd(0x04);
}

UINT32 __nor_wait_busy()
{
    UINT32 timeout = SFLASH_TIMEOUT, u4Tmp;

    while (--timeout != 0)
    {
    	 __nor_send_cmd(0x02);
    	 NORRead32(SFLASH_RDSR_REG, u4Tmp);
        if ((u4Tmp & 0x01) == 0)
            break;
    }

    return timeout;    	
}

/*---------------------------------------------------------------------------
 * Name
 *
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *---------------------------------------------------------------------------*/
 
static BOOL b_sf_upg_enable = FALSE;

INT32 d_custom_extmjc_upgrade_open(VOID)
{
   // UINT8 data;
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    //drv_extmjc_cpu_stop(&data);
    b_sf_upg_enable = TRUE;
    Printf("8283 upgrade--Open!\n");
	 
    NORWrite32(SFLASH_CFG1_REG, 0x20);      // enable Write
	   
    __nor_write_en();

    // disable write protect    	
    NORWrite32(SFLASH_PRGDATA5_REG, 0x0);
    NORWrite32(SFLASH_CNT_REG, 8);
    __nor_send_cmd(0x20);
    	
    NORWrite32(SFLASH_CFG2_REG, 0x0d);

    return (DRV_CUSTOM_EXTMJCR_OK);
}

#if 1
INT32 d_custom_extmjc_register_test(VOID)
{
    UINT32 u4Tmp = 0x00;
    UINT32 u1MenuID, u1DevID1, u1DevID2; 
	  if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    if (b_sf_upg_enable == FALSE)
        return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
        	
    NORWrite32(SFLASH_WDATA_REG, 0x87);
    NORRead32(SFLASH_WDATA_REG, u4Tmp);
    Printf("SFLASH_WDATA_REG = 0x%08X\n", u4Tmp);
    
    NORWrite32(SFLASH_PRGDATA5_REG, 0x9F);    // Write
    NORWrite32(SFLASH_PRGDATA4_REG, 0x00);    // Write
    NORWrite32(SFLASH_PRGDATA3_REG, 0x00);    // Write
    NORWrite32(SFLASH_PRGDATA2_REG, 0x00);    // Write
    NORWrite32(SFLASH_CNT_REG, 32);           // Write SF Bit Count
    
    __nor_send_cmd(0x4);
    
    NORRead32(SFLASH_SHREG0_REG, u1DevID2);
    NORRead32(SFLASH_SHREG1_REG, u1DevID1);
    NORRead32(SFLASH_SHREG2_REG, u1MenuID);
    
    Printf("MenuID: 0x%08X, DevID1: 0x%08X, DevID2: 0x%08X\n", u1MenuID, u1DevID1, u1DevID2);
       
    return (DRV_CUSTOM_EXTMJCR_OK);
}
#endif

INT32 d_custom_extmjc_upgrade_erase(
    UINT32                                  ui4_offset,
    UINT32                                  ui4_data_size)
{
    UINT32 u4Addr;
	
    u4Addr = ui4_offset;
    
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    if (b_sf_upg_enable == FALSE)
        return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
#if 0 // whole chip
    Printf("8283 upgrade--Erase!\n");
    __nor_write_en();
    NORWrite32(SFLASH_PRGDATA5_REG, 0xc7); // erase while chip command
    NORWrite32(SFLASH_CNT_REG, 8);
    __nor_send_cmd(0x04);
#else
    Printf("8283 upgrade--Erase!\n");
	    
    u4Addr = ui4_offset & ~(SFLASH_SECTOR_SIZE - 1);
    while (u4Addr < ui4_offset + ui4_data_size)
    {
        if(!__nor_wait_busy())
        {
            Printf("Wait Busy failed when erasing Nor Flash!\n");
            return (DRV_CUSTOM_EXTMJCR_REG_FAIL);
    		}
    		
        __nor_write_en();
        
        NORWrite32(SFLASH_PRGDATA5_REG, 0xd8); // erase sector command
        NORWrite32(SFLASH_PRGDATA4_REG, BYTE2(u4Addr));
        NORWrite32(SFLASH_PRGDATA3_REG, BYTE1(u4Addr));
        NORWrite32(SFLASH_PRGDATA2_REG, BYTE0(u4Addr));
        NORWrite32(SFLASH_CNT_REG, 32);       // Write SF Bit Count
        __nor_send_cmd(0x04);
        
        NORWrite32(SFLASH_CMD_REG, 0x00);
        
        u4Addr += SFLASH_SECTOR_SIZE;
        
    	}
	#endif

    return (DRV_CUSTOM_EXTMJCR_OK);
}

INT32 d_custom_extmjc_upgrade_write(
    UINT32                                  ui4_offset,
    UINT8*                                  pui1_data,
    UINT32                                  ui4_data_size)
{
    UINT32 i4Len;
    UINT32 u4Count, u4Addr,i,j, u4Tmp, u4LenTmp;
  
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    if (b_sf_upg_enable == FALSE)
        return (DRV_CUSTOM_EXTMJCR_REG_FAIL);

    Printf("\n8283 upgrade--Write!\n");

    // program nor flash
    //if(i4Len % 0x10000 != 0)
    if(ui4_data_size % 0x10000 != 0)
    {
        u4LenTmp = ui4_data_size & (~0x00000003);
        i4Len = u4LenTmp + 0x04;
    }
    else
    {
        u4LenTmp = ui4_data_size;
        i4Len = (INT32)u4LenTmp;    	
    }
    u4Addr = ui4_offset;
    u4Count = SFLASH_WRBUF_SIZE;
    
    Printf("len = %08X\n", i4Len);

    if(ui4_offset==0)
    {
        Printf("--Up Start\n");
        j=0;
        for(i=0;i<10;i++)
        {
            Printf("DWORD(%d): 0x%08x\n",i,*(UINT32*)(pui1_data+j));
    		    j+=4;
    	  }
    }
    else if(i4Len!=0x10000)
    {
        Printf("--Up Last\n");
        j=40;
        for(i=0;i<10;i++)
        {
            Printf("DWORD(%d): 0x%08x\n",i,*(UINT32*)(pui1_data+i4Len -j));
            j-=4;
        }
    }

    __nor_write_en();
    NORWrite32(SFLASH_CFG2_REG, 0x0d);      // enable BUFWREN
    NORWrite32(SFLASH_CKGEN_REG, 0x01);
    
	  u1ExtmjcReadRegByI2C(0x8260, (UINT8*)(&u4Tmp));
	  u1ExtmjcWriteRegByI2C(0x8260, u4Tmp&(~0x02));            // set 0x8260[1] = 0, non-incremental register write

    while (i4Len > 0)
    {
        	
        if (i4Len < SFLASH_WRBUF_SIZE)
            u4Count = i4Len;
        
        NORWrite32(SFLASH_RADR2_REG, BYTE2(u4Addr)); // Write
        NORWrite32(SFLASH_RADR1_REG, BYTE1(u4Addr)); // Write
        NORWrite32(SFLASH_RADR0_REG, BYTE0(u4Addr)); // Write        

#if 1
		    if(SIF_X_Write(EXTMJC_I2C_BUS, 0x40, 0x0c, 0x02, (SFLASH_REG_BASE+SFLASH_PP_DATA_REG), pui1_data, u4Count) != u4Count)    // Burst Mode
		    {
			      Printf("Write Nor Flash By I2C Burst Mode Failed!\n");
			      return (DRV_CUSTOM_EXTMJCR_REG_FAIL); 
		    }  

#else      
        for (i=0; i<u4Count; i+=4)
        {
            NORWrite32(SFLASH_PP_DATA_REG, *(UINT32*)(pui1_data+i));
        }
#endif
        __nor_send_cmd(0x10);

        i4Len -= u4Count;
        u4Addr += u4Count;
        pui1_data += u4Count;      
    }
    
	  u1ExtmjcWriteRegByI2C(0x8260, u4Tmp);       //restore 8260 
	  
	  return (DRV_CUSTOM_EXTMJCR_OK);

}

INT32 d_custom_extmjc_upgrade_close(VOID)
{
    if (FALSE == drv_extmjc_is_enable())
    {
        return (DRV_CUSTOM_EXTMJCR_OK);
    }
    Printf("8283 upgrade--Close!");
    b_sf_upg_enable = FALSE;
    __nor_write_en();
    NORWrite32(SFLASH_PRGDATA5_REG, 0x3C);
    NORWrite32(SFLASH_CNT_REG, 8);
    __nor_send_cmd(0x20);
    HAL_Delay_us(100*1000); // wait 100 ms
    
    NORWrite32(SFLASH_CFG2_REG, 0x0c);
    return (DRV_CUSTOM_EXTMJCR_OK);
}


#endif /* DRV_SUPPORT_EXTMJC */

