/******************************************************************************

    LGE. DTV RESEARCH LABORATORY
    COPYRIGHT(c) LGE CO.,LTD. 1998-2003. SEOUL, KOREA.
    All rights are reserved.
    No part of this work covered by the copyright hereon may be
    reproduced, stored in a retrieval system, in any form or
    by any means, electronic, mechanical, photocopying, recording
    or otherwise, without the prior permission of LG Electronics.

    FILE NAME   : pc_key.h
    VERSION     : 1.0
    AUTHOR      : 이재길(jackee@lge.com)
    DATE        : 2003.01.27
    DESCRIPTION : PC KEY code definitions with LGTERM

*******************************************************************************/

#ifndef	_PC_KEY_H_
#define	_PC_KEY_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	제어 상수
	(Control Constants)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
    #include 파일들
    (File Inclusions)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	상수 정의
	(Constant Definitions)
------------------------------------------------------------------------------*/

/*	PC Keyboard Escape Sequence Map for LGTERM keymap LGDTV.CNF				  */
#define		PC_KEY_INS		0x005B317E		/*	0	<ESC>[1~	*/
#define		PC_KEY_DEL		0x005B347E		/*	.	<ESC>[4~	*/
#define		PC_KEY_HOME		0x005B327E		/*	7	<ESC>[2~	*/
#define		PC_KEY_END		0x005B357E		/*	1	<ESC>[5~	*/
#define		PC_KEY_PGUP		0x005B337E		/*	9	<ESC>[3~	*/
#define		PC_KEY_PGDN		0x005B367E		/*	3	<ESC>[6~	*/

#define		PC_KEY_UP		0x00005B41		/*	8	<ESC>[A~	*/
#define		PC_KEY_DOWN		0x00005B42		/*	2	<ESC>[B~	*/
#define		PC_KEY_RIGHT	0x00005B43		/*	6	<ESC>[C~	*/
#define		PC_KEY_LEFT		0x00005B44		/*	4	<ESC>[D~	*/

#define		PC_KEY_NOUSE	0x00000000		/*	5				*/

#define		PC_KEY_F01		0x5B31317E		/* F1	<ESC>[11~	*/
#define		PC_KEY_F02		0x5B31327E		/* F2	<ESC>[12~	*/
#define		PC_KEY_F03		0x5B31337E		/* F3	<ESC>[13~	*/
#define		PC_KEY_F04		0x5B31347E		/* F4	<ESC>[14~	*/
#define		PC_KEY_F05		0x5B31357E		/* F5	<ESC>[15~	*/
#define		PC_KEY_F06		0x5B31377E		/* F6	<ESC>[17~	*/
#define		PC_KEY_F07		0x5B31387E		/* F7	<ESC>[18~	*/
#define		PC_KEY_F08		0x5B31397E		/* F8	<ESC>[19~	*/
#define		PC_KEY_F09		0x5B32307E		/* F9	<ESC>[20~	*/
#define		PC_KEY_F10		0x5B32317E		/* F10	<ESC>[21~	*/
#define		PC_KEY_F11		0x5B32337E		/* F11	<ESC>[23~	*/
#define		PC_KEY_F12		0x5B32347E		/* F12	<ESC>[24~	*/
#define		PC_KEY_EXT		0x5B39397E		/* Extension 		*/

#define		PC_KEY_ESC		0x0000001B		/* ESC				*/
#define		PC_KEY_NAC		0xFFFFFFFF		/* Not A Char		*/

#define		PC_KEY_SF03		0x5B32357E		// <ESC>[25~
#define		PC_KEY_SF04		0x5B32367E		// <ESC>[26~
#define		PC_KEY_SF05		0x5B32387E		// <ESC>[28~
#define		PC_KEY_SF06		0x5B32397E		// <ESC>[29~
#define		PC_KEY_SF07		0x5B33317E		// <ESC>[31~
#define		PC_KEY_SF08		0x5B33327E		// <ESC>[32~
#define		PC_KEY_SF09		0x5B33337E		// <ESC>[33~
#define		PC_KEY_SF10		0x5B33347E		// <ESC>[34~

/*	PC Keyboard Escape Sequence Map for CRT with SCOANSI terminal emulatrion	  */
#define		PC_KEY_CRT_SANSI_F01  		0x00005B4D	// <ESC>[M"
#define		PC_KEY_CRT_SANSI_F02  		0x00005B4E	// <ESC>[N"
#define		PC_KEY_CRT_SANSI_F03  		0x00005B4F	// <ESC>[O"
#define		PC_KEY_CRT_SANSI_F04  		0x00005B50	// <ESC>[P"
#define		PC_KEY_CRT_SANSI_F05  		0x00005B51	// <ESC>[Q"
#define		PC_KEY_CRT_SANSI_F06  		0x00005B52	// <ESC>[R"
#define		PC_KEY_CRT_SANSI_F07  		0x00005B53	// <ESC>[S"
#define		PC_KEY_CRT_SANSI_F08  		0x00005B54	// <ESC>[T"
#define		PC_KEY_CRT_SANSI_F09  		0x00005B55	// <ESC>[U"
#define		PC_KEY_CRT_SANSI_F10  		0x00005B56	// <ESC>[V"
#define		PC_KEY_CRT_SANSI_F11  		0x00005B57	// <ESC>[V"
#define		PC_KEY_CRT_SANSI_F12  		0x00005B58	// <ESC>[X"

#define		PC_KEY_CRT_SANSI_SF01 		0x00005B59	// <ESC>[Y"
#define		PC_KEY_CRT_SANSI_SF02 		0x00005B5A	// <ESC>[Z"
#define		PC_KEY_CRT_SANSI_SF03 		0x00005B61	// <ESC>[a"
#define		PC_KEY_CRT_SANSI_SF04 		0x00005B62	// <ESC>[b"
#define		PC_KEY_CRT_SANSI_SF05 		0x00005B63	// <ESC>[c"
#define		PC_KEY_CRT_SANSI_SF06 		0x00005B64	// <ESC>[d"
#define		PC_KEY_CRT_SANSI_SF07 		0x00005B65	// <ESC>[e"
#define		PC_KEY_CRT_SANSI_SF08 		0x00005B66	// <ESC>[f"
#define		PC_KEY_CRT_SANSI_SF09 		0x00005B67	// <ESC>[g"
#define		PC_KEY_CRT_SANSI_SF10 		0x00005B68	// <ESC>[h"
#define		PC_KEY_CRT_SANSI_SF11 		0x00005B69	// <ESC>[i"
#define		PC_KEY_CRT_SANSI_SF12 		0x00005B6A	// <ESC>[j"

#define		PC_KEY_CRT_SANSI_INS  		0x00005B4C	// <ESC>[L"
#define		PC_KEY_CRT_SANSI_HOME 		0x00005B48	// <ESC>[H"
#define		PC_KEY_CRT_SANSI_END  		0x00005B46	// <ESC>[F"
#define		PC_KEY_CRT_SANSI_PGUP 		0x00005B49	// <ESC>[I"
#define		PC_KEY_CRT_SANSI_PGDN 		0x00005B47	// <ESC>[G"
#define		PC_KEY_CRT_SANSI_UP   		0x00005B41	// <ESC>[A"
#define		PC_KEY_CRT_SANSI_DOWN 		0x00005B42	// <ESC>[B"
#define		PC_KEY_CRT_SANSI_RIGHT		0x00005B43	// <ESC>[C"
#define		PC_KEY_CRT_SANSI_LEFT 		0x00005B44	// <ESC>[D"

/*	PC Keyboard Escape Sequence Map for CRT with linux terminal emulation	  	*/
#define		PC_KEY_CRT_LINUX_F01  		0x005B5B41	// <ESC>[[A"
#define		PC_KEY_CRT_LINUX_F02  		0x005B5B42	// <ESC>[[B"
#define		PC_KEY_CRT_LINUX_F03  		0x005B5B43	// <ESC>[[C"
#define		PC_KEY_CRT_LINUX_F04  		0x005B5B44	// <ESC>[[D"
#define		PC_KEY_CRT_LINUX_F05  		0x005B5B45	// <ESC>[[E"
#define		PC_KEY_CRT_LINUX_F06  		0x5B31377E	// <ESC>[17~"
#define		PC_KEY_CRT_LINUX_F07  		0x5B31387E	// <ESC>[18~"
#define		PC_KEY_CRT_LINUX_F08  		0x5B31397E	// <ESC>[19~"
#define		PC_KEY_CRT_LINUX_F09  		0x5B32307E	// <ESC>[20~"
#define		PC_KEY_CRT_LINUX_F10  		0x5B32317E	// <ESC>[21~"
#define		PC_KEY_CRT_LINUX_F11  		0x5B32337E	// <ESC>[23~"
#define		PC_KEY_CRT_LINUX_F12  		0x5B32347E	// <ESC>[24~"

#define		PC_KEY_CRT_LINUX_INS  		0x005B327E	// <ESC>[2~"
#define		PC_KEY_CRT_LINUX_DEL  		0x005B337E	// <ESC>[3~"
#define		PC_KEY_CRT_LINUX_HOME 		0x005B317E	// <ESC>[1~"
#define		PC_KEY_CRT_LINUX_END  		0x005B347E	// <ESC>[4~"
#define		PC_KEY_CRT_LINUX_PGUP 		0x005B357E	// <ESC>[5~"
#define		PC_KEY_CRT_LINUX_PGDN 		0x005B367E	// <ESC>[6~"
#define		PC_KEY_CRT_LINUX_UP   		0x00005B41	// <ESC>[A"
#define		PC_KEY_CRT_LINUX_DOWN 		0x00005B42	// <ESC>[B"
#define		PC_KEY_CRT_LINUX_RIGHT		0x00005B43	// <ESC>[C"
#define		PC_KEY_CRT_LINUX_LEFT 		0x00005B44	// <ESC>[D"

/*-----------------------------------------------------------------------------
	매크로 함수 정의
	(Macro Definitions)
------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
    형 정의
    (Type Definitions)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PC_KEY_H_ */
