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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_edid.c,v $
 * $Revision: 
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "hw_vdoin.h"
#include "sv_const.h"
#include "hw_pdwnc.h"
#include "nptv_debug.h"
#include "x_pdwnc.h"

#define DFT_DDCCI_ADDR (0x6e)


#define WDONE_STS    (1<<0)
#define W8_STS       (1<<1)
#define W16_STS      (1<<2)
#define WMSK_STS     (WDONE_STS|W8_STS|W16_STS)
#define RDONE_STS    (1<<3)
#define R8_STS       (1<<4)
#define R16_STS      (1<<5)
#define RMSK_STS     (RDONE_STS|R8_STS|R16_STS)

#define MAX_COMMAND_DATA_LENGTH (48) // but according to spec, the maximum is 32 bytes

static UINT32 au4CmdRecvBuf[MAX_COMMAND_DATA_LENGTH/sizeof(UINT32)];
static UINT8 u1CmdRecvBufIdx = 0;
static UINT8 u1LastCmdLength = 0;

static UINT8 au1CmdReplyBuf[MAX_COMMAND_DATA_LENGTH];
static UINT8 u1CmdReplyBufIdx = 0;
static UINT8 u1CmdReplyLength = 0;

UINT8 u1DrvDDCCI_SetReplyMsg(UINT8 *buf, UINT32 buflen)
{
    UINT8 ret = 0;

    if(buf == NULL || buflen == 0)
    {
        return 1;
    }

    if(buflen > MAX_COMMAND_DATA_LENGTH) 
    {
        buflen = MAX_COMMAND_DATA_LENGTH;
        ret = 2;
    }

    u1CmdReplyLength = buflen;
    x_memcpy(au1CmdReplyBuf, buf, buflen);

    /* fill with 0 padding. */
    while(buflen < 16)
    {
        au1CmdReplyBuf[buflen] = 0;
        buflen++;
    }
    x_memcpy((UINT8*)PDWNC_DDCCI_RDATA0, au1CmdReplyBuf, 16);

    return ret;
}

UINT8 u1DrvDDCCI_GetRequestCmd(UINT8 *buf, UINT32 *cmdlen)
{
    if(buf == NULL || cmdlen == NULL)
    {
        return 1;
    }

    if(*cmdlen < u1LastCmdLength || *cmdlen > MAX_COMMAND_DATA_LENGTH)
    {
        /* buf space is not enough */
        *cmdlen = 0;
        return 2;
    }

    *cmdlen = u1LastCmdLength;
    if(*cmdlen > MAX_COMMAND_DATA_LENGTH)
    {
        /* should never happen */
        return 3;
    }
    x_memcpy(buf, au4CmdRecvBuf, *cmdlen);

    return 0;
}

#if 0 //test function
static void vDrvDDCCI_SetReplyNULLMsg()
{ 
    const UINT8 au1NullMessage[] = {0x6e, 0x80, 0xbe};
    
    u1DrvDDCCI_SetReplyMsg((UINT8*)au1NullMessage, sizeof(au1NullMessage));
    return;
}

static void vDrvDDCCI_SetReplyCapcMsg(UINT8 vcp)
{
    UINT8 u1TmpCnt;
    UINT8 au1ReplyCapcMsg[] = {0x6e, 0x88, 0x2, 0x0, 0x0, 0x0, 100, 0, 0, 50, 0x50};

    au1ReplyCapcMsg[4] = vcp;

    for(u1TmpCnt=0;u1TmpCnt<(sizeof(au1ReplyCapcMsg)-1);u1TmpCnt++)
    {
       au1ReplyCapcMsg[sizeof(au1ReplyCapcMsg)-1]^=au1ReplyCapcMsg[u1TmpCnt]; 
    }
    
    u1DrvDDCCI_SetReplyMsg(au1ReplyCapcMsg, sizeof(au1ReplyCapcMsg));
    return;
}
#endif

static void vDrvDDCCI_IntHandler(UINT16 u2Vector)
{
    UINT32 u4IntStatus;

    LOG(5, "DDCCI_IntHandler\n");
 
    u4IntStatus = IO32ReadFldAlign(PDWNC_DDCCI_INTST, FLD_INT_ALL_ST);

    if(u4IntStatus & W8_STS)
    {
        LOG(5, "W8!\n");
        if(u1CmdRecvBufIdx + 8 > MAX_COMMAND_DATA_LENGTH)
        {
            LOG(1, "[ddcci]err, cmdlength invalid\n");
        }
        else
        {
            au4CmdRecvBuf[u1CmdRecvBufIdx/sizeof(UINT32)] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA0, FLD_WDATA0);
            au4CmdRecvBuf[(u1CmdRecvBufIdx/sizeof(UINT32))+1] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA1, FLD_WDATA1);
            u1CmdRecvBufIdx+=8;
        }
    }

    if(u4IntStatus & W16_STS)
    {
        LOG(5, "W16!\n");
        if(u1CmdRecvBufIdx + 8 > MAX_COMMAND_DATA_LENGTH)
        {
            LOG(1, "[ddcci]err, cmdlength invalid\n");
        }
        else
        {
            au4CmdRecvBuf[u1CmdRecvBufIdx/sizeof(UINT32)] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA2, FLD_WDATA2);
            au4CmdRecvBuf[(u1CmdRecvBufIdx/sizeof(UINT32))+1] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA3, FLD_WDATA3);
            u1CmdRecvBufIdx+=8;
        }
    }

    if(u4IntStatus & WDONE_STS)
    {
        LOG(5, "WDONE!\n");
        if(u1CmdRecvBufIdx + 8 > MAX_COMMAND_DATA_LENGTH)
        {
            LOG(1, "[ddcci]err, cmdlength invalid\n");
        }
        else
        {
            if(((u1CmdRecvBufIdx/8)%2) == 0)
            {
                au4CmdRecvBuf[u1CmdRecvBufIdx/sizeof(UINT32)] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA0, FLD_WDATA0);
                au4CmdRecvBuf[(u1CmdRecvBufIdx/sizeof(UINT32))+1] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA1, FLD_WDATA1);
            }
            else
            {
                au4CmdRecvBuf[u1CmdRecvBufIdx/sizeof(UINT32)] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA2, FLD_WDATA2);
                au4CmdRecvBuf[(u1CmdRecvBufIdx/sizeof(UINT32))+1] = IO32ReadFldAlign(PDWNC_DDCCI_WDATA3, FLD_WDATA3);
            }
        }
      
        u1LastCmdLength = u1CmdRecvBufIdx;
        u1CmdRecvBufIdx = 0;

#if 0 /* just for test */
        vDrvDDCCI_SetReplyNULLMsg();
        if((au4CmdRecvBuf[0] & 0x00ff0000 )== 0x00010000) // get vcp feature
        {
            vDrvDDCCI_SetReplyCapcMsg(au4CmdRecvBuf[0] >> 24);
        }
#endif
    }

    if(u4IntStatus & R8_STS)
    {
        LOG(5, "R8!\n");
        if(u1CmdReplyBufIdx+16+8 > MAX_COMMAND_DATA_LENGTH)
        {
            LOG(1, "[ddcci]err, rply cmdlen invalid\n");
        }
        else if( u1CmdReplyLength - u1CmdReplyBufIdx > 16)
        {
            x_memcpy((UINT8*)PDWNC_DDCCI_RDATA0, &au1CmdReplyBuf[u1CmdReplyBufIdx+16], 8);
        }
        u1CmdReplyBufIdx += 8;
    }

    if(u4IntStatus & R16_STS)
    {
        LOG(5, "R16!\n");
        if(u1CmdReplyBufIdx+16+8 > MAX_COMMAND_DATA_LENGTH)
        {
            LOG(1, "[ddcci]err, rply cmdlen invalid\n");
        }
        else if( u1CmdReplyLength - u1CmdReplyBufIdx > 16)
        {
            x_memcpy((UINT8*)PDWNC_DDCCI_RDATA2, &au1CmdReplyBuf[u1CmdReplyBufIdx+16], 8);
        }
        u1CmdReplyBufIdx += 8;
    }

    if(u4IntStatus & RDONE_STS)
    {
        LOG(5, "RDONE! %d read\n", u1CmdReplyBufIdx);
        u1CmdReplyBufIdx = 0;
#if 0
        vDrvDDCCI_SetReplyNULLMsg();
#endif
    }


    vIO32WriteFldAlign(PDWNC_DDCCI_INTCLR, u4IntStatus, FLD_INT_ALL_CLR);
    return;
}

void vDrvDDCCI_Init(void)
{
    vIO32WriteFldAlign(PDWNC_DDCCI_CFG, DFT_DDCCI_ADDR>>1, FLD_DDCCI_DEV_ADDR);
    vIO32WriteFldAlign(PDWNC_DDCCI_CFG, 3, FLD_DDCCI_DE_CNT);
    vIO32WriteFldAlign(PDWNC_DDCCI_CFG, 1, FLD_DDCCI_DEG_EN);
    vIO32WriteFldAlign(PDWNC_DDCCI_CFG, 0, FLD_DDCCI_OFF);

    PDWNC_RegIsr(PINTNO_DDCCI, vDrvDDCCI_IntHandler, NULL);

    //enable all interrupt
    vIO32WriteFldAlign(PDWNC_DDCCI_INTEN, 0x3f, FLD_INT_ALL_EN);
    return;
}

void vDrvDDCCI_Disable(void)
{
    vIO32WriteFldAlign(PDWNC_DDCCI_CFG, 1, FLD_DDCCI_OFF);

    //disable all interrupt
    vIO32WriteFldAlign(PDWNC_DDCCI_INTEN, 0, FLD_INT_ALL_EN);

    PDWNC_RegIsr(PINTNO_DDCCI, NULL, NULL);
    return;

}
