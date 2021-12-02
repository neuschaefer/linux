;***************           MTK CONFIDENTIAL            ****************
;***************                                       ****************
;***************  $Modtime:: 04/03/15 9:06p    $       ****************
;***************  $Revision: 1.1 $       ****************
;***************                                       ****************
;*************** Description : 8032 startup assembly   ****************
;***************               code                    ****************
;***************     Company : MediaTek Inc.           ****************
;***************  Programmer :                         ****************
;**********************************************************************
;------------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright KEIL ELEKTRONIK GmbH 1990
;------------------------------------------------------------------------------
;  STARTUP.A51:  This code is executed after processor reset.
;
;  To translate this file use A51 with the following invocation:
;
;     A51 STARTUP.A51
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  L51 invocation:
;
;     L51 <your object file list>, STARTUP.OBJ <controls>
;
;------------------------------------------------------------------------------
;
;  User-defined Power-On Initialization of Memory
;
;  With the following EQU statements the initialization of memory
;  at processor reset can be defined:
;
;   ; the absolute start-address of IDATA memory is always 0
IDATALEN    EQU 80H ; the length of IDATA memory in bytes.
;
XDATASTART  EQU 0D000H  ; the absolute start-address of XDATA memory
XDATALEN    EQU 0E00H  ; the length of XDATA memory in bytes.
;
XDATASTART2  EQU 0A800H  ; the absolute start-address of XDATA memory
XDATALEN2    EQU 0000H  ; the length of XDATA memory in bytes.
;
PDATASTART  EQU 0H  ; the absolute start-address of PDATA memory
PDATALEN    EQU 0H  ; the length of PDATA memory in bytes.
;
;  Notes:  The IDATA space overlaps physically the DATA and BIT areas of the
;          8051 CPU. At minimum the memory space occupied from the C51
;          run-time routines must be set to zero.
;------------------------------------------------------------------------------
  ;
;  Reentrant Stack Initilization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it:
;
;  Stack Space for reentrant functions in the SMALL model.
IBPSTACK    EQU 0 ; set to 1 if small reentrant is used.
IBPSTACKTOP EQU 0FFH+1  ; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the LARGE model.
XBPSTACK    EQU 1 ; set to 1 if large reentrant is used.
XBPSTACKTOP EQU 0D7FFH+1; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the COMPACT model.
PBPSTACK    EQU 0 ; set to 1 if compact reentrant is used.
PBPSTACKTOP EQU 0D7FFH+1; set top of stack to highest location+1.
;
;------------------------------------------------------------------------------
;
;  Page Definition for Using the Compact Model with 64 KByte xdata RAM
;
;  The following EQU statements define the xdata page used for pdata
;  variables. The EQU PPAGE must conform with the PPAGE control used
;  in the linker invocation.
;
PPAGEENABLE EQU     0     ; set to 1 if pdata object are used.
PPAGE       EQU   0F9H    ; define PPAGE number.
;
;------------------------------------------------------------------------------

    NAME  ?C_STARTUP


?C_C51STARTUP SEGMENT   CODE
?STACK    SEGMENT   IDATA

    RSEG  ?STACK
    DS  1

    EXTRN CODE (?C_START)
    PUBLIC  ?C_STARTUP

    CSEG  AT  0
?C_STARTUP: LJMP STARTUP1

    RSEG   ?C_C51STARTUP

STARTUP1:

;------------------------------------------------------------------------------
; Start of 8205 specfic settings
;------------------------------------------------------------------------------
    ; for MT8205 FLASH_SZ
    ; Set A16, A17, A18 low to switch to bank 0, [2:0]
    ; For debug test
    MOV   PSW, #0
    CLR   A
    MOV   IE,A
    MOV   IP,A
    ; End
    MOV   DPTR, #0000Ah
    MOVX  A, @DPTR
    ANL   A, #0F0h
    MOVX  @DPTR, A
    ; Unlock A16, A17, A18, A19

    ; for MT8205 BANK_NO
    CLR   P1.0
    CLR   P1.1
    CLR   P1.2
    CLR   P1.3
    ; Lock A16, A17, A18, A19 for 16 banks
    MOV   DPTR, #00005h
    MOVX  A, @DPTR
    ANL   A, #0F0h
    MOVX  @DPTR, A
		
	MOV DPTR,#00200h; rS232 DATA REGISTER
	MOVX A, @DPTR
	MOV A,#31h
	MOVX @DPTR,A
		
	MOV DPTR,#00200h; rS232 DATA REGISTER
	MOVX A, @DPTR
	MOV A,#32h
	MOVX @DPTR,A
		
	MOV DPTR,#00200h; rS232 DATA REGISTER
	MOVX A, @DPTR
	MOV A,#33h
	MOVX @DPTR,A
	
	MOV DPTR,#00200h; rS232 DATA REGISTER
	MOVX A, @DPTR
	MOV A,#34h
	MOVX @DPTR,A
		
;------------------------------------------------------------------------------
; End of 8205 specfic settings
;------------------------------------------------------------------------------

IF IDATALEN <> 0
    MOV   R0,#IDATALEN - 1
    CLR   A
IDATALOOP:
    MOV @R0,A
    DJNZ  R0,IDATALOOP
ENDIF

IF XDATALEN <> 0
    MOV   DPTR,#XDATASTART
    MOV   R7,#LOW (XDATALEN)
  IF (LOW (XDATALEN)) <> 0
    MOV   R6,#(HIGH XDATALEN) +1
  ELSE
    MOV   R6,#HIGH (XDATALEN)
  ENDIF
    CLR A
XDATALOOP:
    MOVX  @DPTR,A
    INC   DPTR
    DJNZ  R7,XDATALOOP
    DJNZ  R6,XDATALOOP
ENDIF

IF XDATALEN2 <> 0
    MOV   DPTR,#XDATASTART2
    MOV   R7,#LOW (XDATALEN2)
  IF (LOW (XDATALEN2)) <> 0
    MOV   R6,#(HIGH XDATALEN2) +1
  ELSE
    MOV   R6,#HIGH (XDATALEN2)
  ENDIF
    CLR A
XDATALOOP2:
    MOVX  @DPTR,A
    INC   DPTR
    DJNZ  R7,XDATALOOP2
    DJNZ  R6,XDATALOOP2
ENDIF

IF PPAGEENABLE <> 0
    MOV   P2,#PPAGE
ENDIF

IF PDATALEN <> 0
    MOV   R0,#PDATASTART
    MOV   R7,#LOW (PDATALEN)
    CLR   A
PDATALOOP:
    MOVX  @R0,A
    INC   R0
    DJNZ  R7,PDATALOOP
ENDIF

IF IBPSTACK <> 0
EXTRN DATA (?C_IBP)

    MOV   ?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0
EXTRN DATA (?C_XBP)

    MOV   ?C_XBP,#HIGH XBPSTACKTOP
    MOV   ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

IF PBPSTACK <> 0
EXTRN DATA (?C_PBP)
    MOV   ?C_PBP,#LOW PBPSTACKTOP
ENDIF

    MOV   SP,#?STACK-1
    LJMP  ?C_START

    END
