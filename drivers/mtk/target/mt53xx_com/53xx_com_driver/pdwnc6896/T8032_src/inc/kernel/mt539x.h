/*----------------------------------------------------------------------------
 * No Warranty :  Except  as  may  be  otherwise  agreed  to  in  writing, no *
 * warranties  of  any  kind,  whether  express or  implied, are given by MTK *
 * with  respect  to  any  Confidential  Information  or any use thereof, and *
 * the  Confidential  Information  is  provided  on  an  "AS IS"  basis.  MTK *
 * hereby  expressly  disclaims  all  such  warranties, including any implied *
 * warranties   of  merchantability ,  non-infringement  and  fitness  for  a *
 * particular purpose and any warranties arising out of course of performance *
 * course  of dealing or usage of trade.  Parties further acknowledge that in *
 * connection  with  the Purpose, Company may, either presently and/or in the *
 * future,   instruct   MTK   to   assist  it  in  the  development  and  the *
 * implementation,  in accordance with Company's designs, of certain software *
 * relating  to  Company's  product(s)  (the  "Services").   Except as may be *
 * otherwise agreed to in writing, no warranties of any kind, whether express *
 * or  implied,  are  given by MTK with respect to the Services provided, and *
 * the  Services  are  provided  on  an  "AS  IS"  basis.   Company   further *
 * acknowledges  that  the  Services  may  contain  errors,  which testing is *
 * important  and  it  is  solely  responsible for fully testing the Services *
 * and/or   derivatives   thereof   before  they  are  used,  sublicensed  or *
 * distributed.   Should  there  be any third party action brought again MTK, *
 * arising  out  of  or  relating  to  the  Services,  Company agree to fully *
 * indemnify and hold MTK harmless.                                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2003
 *
 *****************************************************************************/
/**
 * @file mt8202.h
 *
 * This file contains mt8202 specific
 *
 * @author Mtk00469
 */

#ifndef _MT5391_H_
#define _MT5391_H_

#define BIM_REG_START			0x0000
#define RS232_REG_START		0x0700
#define CKGEN_REG_START		0x0800
#define RAMBUF_REG_START		0x1000
#define SCPOS_REG_START		0x2000
#define VDOIN_REG_START		0x3000
#define AUDIO_REG_START		0x4000
#define PSCAN0_REG_START		0x5000
#define PSCAN1_REG_START		0x5000
#define OSD_REG_START			0x7000
#define GRAPH_REG_START		0x9000

#define VIR_REG_START			0xC800 // ~ 0xCFFF
#define GP_REG_START			0xD000
#define VIR_REG_START2			0xE000

#endif
