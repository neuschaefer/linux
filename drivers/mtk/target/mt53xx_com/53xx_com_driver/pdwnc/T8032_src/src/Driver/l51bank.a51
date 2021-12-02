;------------------------------------------------------------------------------
;  This file is part of the BL51 Banked Linker/Locater package
;  Copyright (c) 1995-1996 Keil Software, Inc.
;  Version 1.4b
;------------------------------------------------------------------------------
;************************ Configuration Section *******************************
?B_NBANKS       EQU     1       ; Define max. Number of Banks                *
;                                ; The max. value for ?B_BANKS is 32          *
;                                ; This number MUST be a power of 2.          *
;-----------------------------------------------------------------------------*
; if ?BANK?MODE is 0 define the following values                              *
; For Bank-Switching via 8051 Port define Port Address / Bits                 *
?B_PORT         EQU     P1       ; default is P1                              *
?B_FIRSTBIT     EQU     0        ; default is Bit 0                           *
;-----------------------------------------------------------------------------*
;                                                                             *
;******************************************************************************

                NAME    ?BANK?SWITCHING

                PUBLIC  ?B_NBANKS, ?B_CURRENTBANK, ?B_MASK
                PUBLIC  ?B_FACTOR
                PUBLIC  _SWITCHBANK

?B_FACTOR       EQU     1 SHL ?B_FIRSTBIT

?B_CURRENTBANK  EQU     ?B_PORT

; generate Mask and Bank Number Information
IF      ?B_NBANKS <= 2
  MASK          EQU     00000001B
ELSEIF  ?B_NBANKS <= 4
  MASK          EQU     00000011B
ELSEIF  ?B_NBANKS <= 8
  MASK          EQU     00000111B
ELSEIF  ?B_NBANKS <= 16
  MASK          EQU     00001111B
ELSE
  MASK          EQU     00011111B
ENDIF

?B_MASK         EQU     MASK SHL ?B_FIRSTBIT

BANKN           MACRO   N
BANK&N           EQU     N SHL ?B_FIRSTBIT
                ENDM

CNT             SET     0

                REPT    ?B_NBANKS
                BANKN   %CNT
CNT             SET     CNT+1
                ENDM

  ; Convert Bank No in Accu to Address * 8

  IF  ?B_FIRSTBIT = 0
  CONVBANKNO    MACRO
                SWAP    A
                RR      A
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 1
  CONVBANKNO    MACRO
                RL      A
                RL      A
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 2
  CONVBANKNO    MACRO
                RL      A
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 3
  CONVBANKNO    MACRO
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 4
  CONVBANKNO    MACRO
                RR      A
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 5
  CONVBANKNO    MACRO
                RR      A
                RR      A
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 6
  CONVBANKNO    MACRO
                SWAP    A
                RL      A
                ENDM
  ENDIF

  IF  ?B_FIRSTBIT = 7
  CONVBANKNO    MACRO
                SWAP    A
                ENDM
  ENDIF


SELECT          MACRO   N
LOCAL           XLABEL, YLABEL

                PUBLIC  ?B_BANK&N
?B_BANK&N:
                MOV     A,?B_CURRENTBANK
                ANL     A,#?B_MASK
                CJNE    A,#BANK&N,XLABEL

                CLR     A
                JMP     @A+DPTR

XLABEL:         CONVBANKNO         ; Convert Bank Number to Address
                PUSH    ACC
                MOV     A,#HIGH ?BANK?SWITCH
                PUSH    ACC
YLABEL:         PUSH    DPL
                PUSH    DPH
                LJMP    ?B_SWITCH&N

                ENDM


?BANK?SELECT    SEGMENT  CODE

                RSEG    ?BANK?SELECT
CNT             SET     0

                REPT    ?B_NBANKS
                SELECT  %CNT
CNT             SET     CNT+1

                ENDM


SWITCH          MACRO   N
                ORG     N * 8
                PUBLIC  ?B_SWITCH&N
    ?B_SWITCH&N:
       IF  N <> (?B_NBANKS-1)
                ANL     ?B_CURRENTBANK,#(NOT ?B_MASK)
       ENDIF
                ORL     ?B_CURRENTBANK,#BANK&N
                RET

                ENDM


?BANK?SWITCH    SEGMENT  CODE  PAGE

                RSEG    ?BANK?SWITCH
CNT             SET     0

                REPT    ?B_NBANKS
                SWITCH  %CNT

CNT             SET     CNT+1
                ENDM

    SWITCHBNK:  PUSH    IE
                CLR     EA
                ORL     ?B_CURRENTBANK, #?B_MASK
                ANL     ?B_CURRENTBANK, A
                POP     IE
                MOV     A, R0
                RET

RSEG            ?BANK?SELECT

;************************  SWITCHBANK FUNCTION  *******************************
;                                                                             *
; SWITCHBANK allows use of bank-switching for C programs                      *
;                                                                             *
; prototype:   extern switchbank (unsigned char bank_number);                 *
;                                                                             *
;******************************************************************************

_SWITCHBANK:    MOV     A,R7
                SWAP    A
                RR      A
                MOV     DPTR,#?BANK?SWITCH
                JMP     @A+DPTR


                END

