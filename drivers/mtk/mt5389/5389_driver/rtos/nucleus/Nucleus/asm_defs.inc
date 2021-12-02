;************************************************************************
;*
;*               Copyright Mentor Graphics Corporation 2002
;*                         All Rights Reserved.
;*
;* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
;* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
;* SUBJECT TO LICENSE TERMS.
;*
;************************************************************************

;************************************************************************
;*
;* FILE NAME                                  VERSION
;*
;*      asm_defs.inc                  Nucleus PLUS\Integrator\ADS 1.14.8
;*
;* COMPONENT
;*
;*      IN - Initialization
;*
;* DESCRIPTION
;*
;*      This file contains the target processor dependent initialization
;*      values used in int.s, tct.s, and tmt.s
;*
;* DATA STRUCTURES
;*
;*      NONE
;*
;* FUNCTIONS
;*
;*      NONE
;*
;* DEPENDENCIES
;*
;*      NONE
;*
;* HISTORY
;*
;*         NAME            DATE                    REMARKS
;*
;*      D. Driscoll     06-06-2001             Released version 1.13.2
;*                                             Fixed SPR 1070
;*      D. Driscoll     01-07-2002             Released version 1.13.3
;*      R. Baum         04-17-2002             Released version 1.13.5
;*                                             Added offset values for
;*                                             task and HISR control
;*                                             blocks.
;*      R. Baum         05-31-2002             Released version 1.13.6
;*      C. Sheppard     08-30-2002             Released version 1.14.7
;*      J. Pregeant     12-27-2002             Released version 1.14.8
;************************************************************************

;/* Define constants used in low-level initialization.  */
;************************************
;*  MODE BITS/MASKS DEFINES         *
;************************************

LOCKOUT         EQU     &C0                 ; Interrupt lockout value
LOCK_MSK        EQU     &C0                 ; Interrupt lockout mask value
MODE_MASK       EQU     &1F                 ; Processor Mode Mask
SUP_MODE        EQU     &13                 ; Supervisor Mode (SVC)
SYS_MODE        EQU     &1F                 ; System Mode (SYS)
IRQ_MODE        EQU     &12                 ; Interrupt Mode (IRQ)
FIQ_MODE        EQU     &11                 ; Fast Interrupt Mode (FIQ)
USR_MODE        EQU     &10                 ; User Mode (USR)
I_BIT           EQU     &80                 ; Interrupt bit of CPSR and SPSR
F_BIT           EQU     &40                 ; Interrupt bit of CPSR and SPSR


;************************************
;*  SYSTEM CLOCK                    *
;************************************

TIMER_INT_MODE      EQU     IRQ_MODE


;/* End of low-level initialization constants.  */

;*********************************************
;*  TC_TCB and TC_HCB STRUCT OFFSET DEFINES  *
;*********************************************

TC_CREATED              EQU     &0          ; Node for linking to created task list
TC_ID                   EQU     &C          ; Internal TCB ID
TC_NAME                 EQU     &10         ; Task name
TC_STATUS               EQU     &18         ; Task status
TC_DELAYED_SUSPEND      EQU     &19         ; Delayed task suspension
TC_PRIORITY             EQU     &1A         ; Task priority
TC_PREEMPTION           EQU     &1B         ; Task preemption enable
TC_SCHEDULED            EQU     &1C         ; Task scheduled count
TC_CUR_TIME_SLICE       EQU     &20         ; Current time slice
TC_STACK_START          EQU     &24         ; Stack starting address
TC_STACK_END            EQU     &28         ; Stack ending address
TC_STACK_POINTER        EQU     &2C         ; Task stack pointer
TC_STACK_SIZE           EQU     &30         ; Task stack's size
TC_STACK_MINIMUM        EQU     &34         ; Minimum stack size
TC_CURRENT_PROTECT      EQU     &38         ; Current protection
TC_SAVED_STACK_PTR      EQU     &3C         ; Previous stack pointer
TC_ACTIVE_NEXT          EQU     &3C         ; Next activated HISR
TC_TIME_SLICE           EQU     &40         ; Task time slice value
TC_ACTIVATION_COUNT     EQU     &40         ; Activation counter
TC_HISR_ENTRY           EQU     &44         ; HISR entry function
TC_HISR_SU_MODE         EQU     &58         ; Sup/User mode indicator for HISRs
TC_HISR_MODULE          EQU     &5C         ; Module identifier for HISR's
TC_SU_MODE              EQU     &A8         ; Sup/User mode indicator for Tasks
TC_MODULE               EQU     &AC         ; Module identifier for Tasks

;*********************************************
;*       TC_TCB and TC_HCB ID DEFINES        *
;*********************************************

TC_TASK_ID              EQU     &5441534b   ; Redefined in assembly to determine if
TC_HISR_ID              EQU     &48495352   ; current thread is a HISR or task.

;*********************************************
;*           Module Setup defines            *
;*********************************************
MS_TARGET               EQU     &24         ; module support offset for
                                            ; level 1 base

    END
