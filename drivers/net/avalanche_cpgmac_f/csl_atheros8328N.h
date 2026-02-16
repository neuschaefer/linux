/*
 *
 * csl_atheros8328N.h
 * Description:
 * atheros8328n driver csl layer
 *
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * Includes Intel Corporation's changes/modifications dated: May 2011.
 * Changed/modified portions - Copyright © 2011, Intel Corporation.   
 *
 */


#ifndef __SWITCH_CSL_ATHEROS_8328N_H__
#define __SWITCH_CSL_ATHEROS_8328N_H__


Int32 cslAtheros8328n_phy_get(Uint32 dev_id, Uint32 phy_addr, Uint32 reg, Uint16 * value);
Int32 cslAtheros8328n_phy_set(Uint32 dev_id, Uint32 phy_addr,Uint32 reg, Uint16 value);
Int32 cslAtheros8328n_reg_get(Uint32 dev_id, Uint32 reg_addr, void * value, Uint32 value_len);
Int32 cslAtheros8328n_reg_set(Uint32 dev_id, Uint32 reg_addr, void * value, Uint32 value_len);

Int32 cslAtheros8328n_reg_field_get(Uint32 dev_id,
                         Uint32 reg_addr,
                         Uint32 bit_offset,
                         Uint32 field_len,
                         Uint8  value[],
                         Uint32 value_len);

Int32 cslAtheros8328n_reg_field_set( Uint32 dev_id,
                          Uint32 reg_addr,
                          Uint32 bit_offset,
                          Uint32 field_len,
                          const Uint8 value[],
                          Uint32 value_len);

#endif /* __SWITCH_CSL_ATHEROS_8328N_H__*/


