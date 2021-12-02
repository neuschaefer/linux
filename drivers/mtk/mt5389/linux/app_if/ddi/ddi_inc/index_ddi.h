/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2006 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file index_ddi.h
 *
 *  INDEX Control 에 사용되는 DDI 함수들을 위한 Header 파일.
 *
 *  @author	siskjy
 *  @version	1.0
 *  @date		2006.9.12
 */

#ifndef _INDEX_DDI_H_
#define _INDEX_DDI_H_

/**
 *	Include Files
 */


/**
 *	Constant Definitions
 */
#define		CP_SLAVRX_VFD_STRING_DISPLAY		0x70
#define		CP_SLAVRX_VFD_TIME_DISPLAY			0x71
#define		CP_SLAVRX_VFD_ONOFF					0x72
#define		CP_SLAVRX_VFD_ICON_ONOFF			0x73
#define		CP_SLAVRX_VFD_DIMMING				0x74


#if 0
/* CHAR define */
#define		_BLK_		0x20
#define		_PLUS_		0x2B
#define		_MINUS_		0x2D

#define		_0_		0x30
#define		_1_		0x31
#define		_2_		0x32
#define		_3_		0x33
#define		_4_		0x34
#define		_5_		0x35
#define		_6_		0x36
#define		_7_		0x37
#define		_8_		0x38
#define		_9_		0x39
#define		_COLON_			0x3A
#define		_SEMICOLON_		0x3B

#define		_A_		0x41
#define		_B_		0x42
#define		_C_		0x43
#define		_D_		0x44
#define		_E_		0x45
#define		_F_		0x46
#define		_G_		0x47
#define		_H_		0x48
#define		_I_		0x49
#define		_J_		0x4A
#define		_K_		0x4B
#define		_L_		0x4C
#define		_M_		0x4D
#define		_N_		0x4E
#define		_O_		0x4F
#define		_P_		0x50
#define		_Q_		0x51
#define		_R_		0x52
#define		_S_		0x53
#define		_T_		0x54
#define		_U_		0x55
#define		_V_		0x56
#define		_W_		0x57
#define		_X_		0x58
#define		_Y_		0x59
#define		_Z_		0x5A

#define		_a_		0x61
#define		_b_		0x62
#define		_c_		0x63
#define		_d_		0x64
#define		_e_		0x65
#define		_f_		0x66
#define		_g_		0x67
#define		_h_		0x68
#define		_i_		0x69
#define		_j_		0x6A
#define		_k_		0x6B
#define		_l_		0x6C
#define		_m_		0x6D
#define		_n_		0x6E
#define		_o_		0x6F
#define		_p_		0x70
#define		_q_		0x71
#define		_r_		0x72
#define		_s_		0x73
#define		_t_		0x74
#define		_u_		0x75
#define		_v_		0x76
#define		_w_		0x77
#define		_x_		0x78
#define		_y_		0x79
#define		_z_		0x7A
#endif	// #if 0

/**
 *	Function Declaration
 */
DTV_STATUS_T	DDI_INDEX_VFDOnOff (UINT8 vfdOnOff);
DTV_STATUS_T	DDI_INDEX_VFDDimming (UINT8 vfdDimming);
DTV_STATUS_T	DDI_INDEX_VFDIconOnOff (UINT8 iconOnOff);
DTV_STATUS_T	DDI_INDEX_VFDStringDisplay (UINT8 *stringData);
DTV_STATUS_T	DDI_INDEX_VFDTimeDisplay (UINT8 *timeData);

#endif	/* end of _INDEX_DDI_H_ */

