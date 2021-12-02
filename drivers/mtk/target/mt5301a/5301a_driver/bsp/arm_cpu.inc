;/*----------------------------------------------------------------------------
; * Copyright (c) 2004, MediaTek, Inc.
; * All rights reserved.
; *
; * Unauthorized use, practice, perform, copy, distribution, reproduction,
; * or disclosure of this information in whole or in part is prohibited.
; *----------------------------------------------------------------------------
; *
; * Description:
; *
; *--------------------------------------------------------------------------*/

;*************************************************************************
;*  CPU related stuff
;*************************************************************************
; CPU mode definitions
LOCKOUT                 EQU     0xC0            ; Interrupt lockout value
LOCK_MSK                EQU     0xC0            ; Interrupt lockout mask value

; CPU mode
MODE_USR                EQU     0x10
MODE_FIQ                EQU     0x11
MODE_IRQ                EQU     0x12
MODE_SVC                EQU     0x13
MODE_ABT                EQU     0x17
MODE_UDF                EQU     0x1B
MODE_MASK               EQU     0x1F

BIT_I                   EQU     0x80
BIT_F                   EQU     0x40
IF_MASK                 EQU     0xc0

; CP15 control register (r1) definitions
CR_MMU                  EQU     (1 << 0)        ; MMU enable
CR_ICACHE               EQU     (1 << 12)       ; I-cache enable
CR_BPRDCT               EQU     (1 << 11)       ; branch prediction enable
CR_DCACHE               EQU     (1 << 2)        ; D-cache enable
CR_ALIGNMENT            EQU     (1 << 1)        ; Alignment fault checking

; CP15 control register (r9) definitions
CR_TCM_ENABLE           EQU     (1 << 0)        ; TCM enable
CR_TCM_4KB              EQU     (3 << 2)        ; TCM 4KB setup.
CR_TCM_8KB              EQU     (4 << 2)        ; TCM 8KB setup.
CR_TCM_MASK             EQU     (0xf)           ; TCM flag mask

    END
