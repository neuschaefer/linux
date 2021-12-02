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
/********************************************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES 
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED 
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS 
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, 
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK 
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION 
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
* 
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH 
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, 
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE 
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 *     
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS 
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.  
 ************************************************************************************************/
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: ether_if.c $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *   Ethernet driver interface header file
 *
 * Author:
 * -------
 *   Hs Huang
 *
 * Last changed:
 * -------------
 * $Author:
 *
 * $Modtime:
 *
 * $Revision:
****************************************************************************/

#ifndef X_ETHER_IF_H
#define X_ETHER_IF_H

/****************************************************************************
** Constant definitions
****************************************************************************/
/* ethernet message queue priority, 0: highest, 255: lowest */
typedef enum 
{
    ETHER_MSG_PRIORITY_TIMER = 10,	/* periodical timer task */
    ETHER_MSG_PRIORITY_IRQ, 		/* interrupt procedure */
    ETHER_MSG_PRIORITY_IOCTL		/* ioctl operation */
} ETHER_MSG_PRIORITY_T;

/* ethernet message queue command */
typedef enum 
{
    ETHER_MSG_RECV_IRQ = 0,
    ETHER_MSG_WAKEUP_IRQ,
    ETHER_MSG_TIMER_SRV,
    ETHER_MSG_OID_SET,
    ETHER_MSG_OID_GET
} ETHER_MSG_T;


/* Event group flags */
typedef enum 
{
    /* wake up thread flags */
    ETHER_EG_TX_COMP_IRQ      = (EV_GRP_EVENT_T)(0x1 << 0),     /* tx completion interrupt */
    ETHER_EG_RX_IRQ           = (EV_GRP_EVENT_T)(0x1 << 1),     /* rx interrupt */
    ETHER_EG_RECV_PKT         = (EV_GRP_EVENT_T)(0x1 << 2),     /* continue to service received packet */
    ETHER_EG_WAKEUP_IRQ       = (EV_GRP_EVENT_T)(0x1 << 3),     /* PMT wakeup interrupt */
    ETHER_EG_TIMER_SRV        = (EV_GRP_EVENT_T)(0x1 << 4),     /* timer jobs service */

    ETHER_EG_WAKEUP_THREAD    = (EV_GRP_EVENT_T)(ETHER_EG_TX_COMP_IRQ | ETHER_EG_RX_IRQ 
                                                | ETHER_EG_WAKEUP_IRQ | ETHER_EG_TIMER_SRV
                                                | ETHER_EG_RECV_PKT),
}WLAN_EVENT_T;


#define ETHER_TX_TIMEOUT              200		/* ethernet tx timeout value(ms) */ 

/****************************************************************************
** Structure and Type definitions
****************************************************************************/
/* ethernet driver message parameter structure */
typedef struct _ETHER_MSG_PARM_T_
{
    UINT32			u4Cmd1;
    UINT32			u4Cmd2;
    void			*pvArg;
} ETHER_MSG_PARM_T;

/****************************************************************************
** Macros
****************************************************************************/
#if CONFIG_ETHER_USE_SAM_INSTEAD_EV
extern HANDLE_T _hEtherTaskSema;
#define ETHER_SET_EVENT(u8Event)\
    CRIT_STATE_T  t_state;\
    t_state = x_crit_start();\
    prMac->u8TaskEvt |= u8Event;\
    x_crit_end(t_state);

#else
extern HANDLE_T _hEtherEvent;

/* The macro set ethernet group event flag */
#define ETHER_SET_EVENT(u8Event)  \
        {\
            VERIFY(x_ev_group_set_event(_hEtherEvent, u8Event, X_EV_OP_OR) == OSR_OK); \
        }    

/* The macro clear ethernet group event flag */
#define ETHER_CLR_EVENT(u8Event)  \
        {\
            VERIFY(x_ev_group_set_event(_hEtherEvent, ~u8Event, X_EV_OP_AND) == OSR_OK); \
        }    
#endif
/****************************************************************************
** Function Declarations
****************************************************************************/
NET_DRV_IF_T* prEtherGetDev(void);
INT32 i4EtherSendMsgQ(const PETHER_ADAPTER_T prMac, 
                             ETHER_MSG_T eEtherMsg, 
                             ETHER_MSG_PRIORITY_T ePriority );


/****************************************************************************
** External ethernet API
****************************************************************************/

/*******************************************************************************
**  i4EtherDrv_Init
**
**  @brief  This routine is called to initialize ethernet driver and creat thread
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return NET_DRV_OK - driver init OK
**          NET_DRV_ERROR - driver init fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Init (NET_DRV_IF_T *prDev);                                        

/*******************************************************************************
**  i4EtherDrv_DeInit
**
**  @brief  This routine is called to de-initialize ethernet driver and thread
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_DeInit(NET_DRV_IF_T *prDev);

/*******************************************************************************
**  i4EtherDrv_Start
**
**  @brief  This routine is called to start ethernet driver processing
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Start(NET_DRV_IF_T *prDev);

/*******************************************************************************
**  i4EtherDrv_Stop
**
**  @brief  This routine is called to stop ethernet driver processing
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Stop(NET_DRV_IF_T *prDev);

/*******************************************************************************
**  i4EtherDrv_Txmit
**
**  @brief  This routine is called to transmit packet through ethernet driver 
**
**  @param  prDev - pointer to net device structure
**  @result
**  @return 0 if success
**          negative if fail
**  @note   ethernet driver will send the packet immediately and wait transmission 
**          completed sema, then rerun the fucntion call
*******************************************************************************/
INT32 i4EtherDrv_Txmit(const NET_DRV_IF_T *prDev, PKT_BUFF_T *prPkt);

/*******************************************************************************
**  i4EtherDrv_Set
**
**  @brief  This routine is called to set ethernet driver OIDs information
**
**  @param  prDev - pointer to net device structure
**          i4Cmd - OIDs command
**          pvParam - OIDs parameter
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Set(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam);

/*******************************************************************************
**  i4EtherDrv_Get
**
**  @brief  This routine is called to get ethernet driver OIDs information
**
**  @param  prDev - pointer to net device structure
**          i4Cmd - OIDs command
**          pvParam - OIDs parameter
**  @result
**  @return 0 if success
**          negative if fail
**  @note
*******************************************************************************/
INT32 i4EtherDrv_Get(NET_DRV_IF_T *prDev, INT32 i4Cmd, VOID *pvParam);


//for PHY setting CLI
void vSet100MAmp(UINT32 chipId, UINT32 val);
UINT32 u1Get100MAmp(UINT32 chipId);
void vSet10MAmp(UINT32 chipId, UINT32 val);
UINT32 u1Get10MAmp(UINT32 chipId);
void vSetSlewRate(UINT32 chipId, UINT32 val);
UINT32 u1GetSlewRate(UINT32 chipId);
void vSetInputBias(UINT32 chipId, UINT32 val);
UINT32 u1GetInputBias(UINT32 chipId);
void vSetOutputBias(UINT32 chipId, UINT32 val);
UINT32 u1GetOutputBias(UINT32 chipId);
void vSet50percentBW(UINT32 chipId, UINT32 val);
UINT32 u1Get50percentBW(UINT32 chipId);
void vSetFeedbackCap(UINT32 chipId, UINT32 val);
UINT32 u1GetFeedbackCap(UINT32 chipId);
void vSetEyeOpen(UINT32 chipId, UINT32 val);
UINT32 u1GetEyeOpen(UINT32 chipId);
void vSetPhyLoopOn(UINT32 chipId, UINT32 val);
void vSetForceMdiCrossover(UINT32 chipId, UINT32 val);
void vSetSpeed(UINT32 chipId, UINT32 val);
void vSetPhyEPG(UINT32 chipId, UINT32 val);

#endif /* X_ETHER_IF_H */
