/*
  BSD LICENSE 

  Copyright(c) 2011-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include <asm-generic/ioctls.h>
#ifndef _PUMA6_CRU_H
#define _PUMA6_CRU_H

/** \enum PAL_SYS_CRU_MODULE_T
    \brief  Enum for modules which can do Power an Reset
            management using CRU. Use these defines for all Pal
            APIs. CRU - Clock and Reset UNIT One per module (in
            some cases more than one) gate the clocks for power
            saving, and module reset controllability.
            The Output clocks from the pll's are connected to
            the CRU module which implemented clock gating and
            reset assertion for the IP modules.
*/
typedef enum PAL_SYS_CRU_MODULE_tag
{
 /* 0 */   CRU_NUM_ARM11,                 /* ATOM must enable this module's clocks to take ARM11 out of reset, after it uses DOCSIS_CNTL register to take Docsis IP out of reset */
 /* 1 */   CRU_NUM_C55,                 
 /* 2 */   CRU_NUM_I2C_DMA,             
 /* 3 */   CRU_NUM_DOCSIS_MAC0,         
 /* 4 */   CRU_NUM_DOCSIS_MAC1,         
 /* 5 */   CRU_NUM_DOCSIS_PHY0,         
 /* 6 */   CRU_NUM_DOCSIS_PHY1,         
 /* 7 */   CRU_NUM_DOCSIS_PHY2,         
 /* 8 */   CRU_NUM_PKT_PROCESSOR,       
 /* 9 */   CRU_NUM_DOCSIS_IP_INFRA,        /* This module's clocks are enabled by defualt */
 /* 10 */  CRU_NUM_BBU,                 
 /* 11 */  CRU_NUM_WDT,                 
 /* 12 */  CRU_NUM_RAM,                    /* This module's clocks are enabled by defualt */
 /* 13 */  CRU_NUM_TIMER0,              
 /* 14 */  CRU_NUM_TIMER1,              
 /* 15 */  CRU_NUM_TIMER2,              
 /* 16 */  CRU_NUM_UART0,               
 /* 17 */  CRU_NUM_UART1,               
 /* 18 */  CRU_NUM_UART2,               
 /* 19 */  CRU_NUM_CPSPDMA0,            
 /* 20 */  CRU_NUM_CPSPDMA1,            
 /* 21 */  CRU_NUM_BOOT_CFG,               /* This module's clocks are enabled by defualt */
 /* 22 */  CRU_NUM_TDM00,               
 /* 23 */  CRU_NUM_TDM01,               
 /* 24 */  CRU_NUM_TDM10,               
 /* 25 */  CRU_NUM_TDM11,               
 /* 26 */  CRU_NUM_DSP_PROXY,           
 /* 27 */  CRU_NUM_DSP_INC,             
 /* 28 */  CRU_NUM_I2C,                 
 /* 29 */  CRU_NUM_PREF_MON,            
 /* 30 */  CRU_NUM_C55_CLK,             
 /* 31 */  CRU_NUM_NBADC,               
 /* 32 */  CRU_NUM_DAC,                 
 /* 33 */  CRU_MAX,
}PAL_SYS_CRU_MODULE_T;

#endif /* _PUMA6_CRU_H */
