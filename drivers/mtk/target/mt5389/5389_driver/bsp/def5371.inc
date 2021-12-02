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
;*  System configurations
;*************************************************************************

;ADDR_TIME_ARRAY         EQU     0x60002000

; PBI_B and DRAM_B base addresses and sizes

INIT_DRAM_B_BASE        EQU     0x80000000          ; DRAM B initial base
INIT_PBI_B_BASE         EQU     0xF8000000          ; PBI B initial base

PBI_B_BASE              EQU     0xF8000000          ; PBI B base after remapping
DRAM_B_BASE             EQU     0x80000000          ; DRAM B base after remapping
DRAM_B_SIZE             EQU     0x40000000          ; 1024 MB
DRAM_B_OFFSET           EQU     0x0                 ; DRAM B offset
SRAM_ITCM_BASE          EQU     SRAM_LZHS_BASE
SRAM_OSD_BASE           EQU     0xfb000000 ;osd=16k
SRAM_LZHS_BASE          EQU     SRAM_OSD_BASE + 0x4000 ;lzhs=4k
SRAM_DMX_BASE           EQU     SRAM_LZHS_BASE + 0x1000 ;dmx=28k
SRAM_DMX_END            EQU     SRAM_DMX_BASE + 0x7000
SRAM_START              EQU     SRAM_OSD_BASE
SRAM_END                EQU     SRAM_DMX_END
SRAM_CORE1_BASE         EQU     SRAM_END - 0x100 ;start address for core1

; IO base address

IO_VIRT                 EQU     0xf0000000          ; IO base address

; Stack configuration

IRQ_STACK_SIZE          EQU     4096
FIQ_STACK_SIZE          EQU     2048
ABT_STACK_SIZE          EQU     2048
UDF_STACK_SIZE          EQU     2048
SVC_STACK_SIZE          EQU     8192

TOTAL_STACK_SIZE        EQU     (IRQ_STACK_SIZE + FIQ_STACK_SIZE + SVC_STACK_SIZE)

TIMER_SIZE              EQU     4096                ; Define timer HISR stack size
TIMER_PRIORITY          EQU     2                   ; Timer HISR priority (values from
                                                    ; 0 to 2, where 0 is highest)

; Heap size for ARM library (malloc and new), should be adjusted appropriately

ARM_LIB_HEAP_SIZE       EQU     0x10000             ; 64K bytes

;*************************************************************************
;*  Register definitions
;*************************************************************************

; Base addresses of peripherals

DRAM_BASE               EQU     IO_VIRT  + 0x7000
BIM_BASE                EQU     IO_VIRT  + 0x8000
CTL_BASE                EQU     IO_VIRT  + 0xD100
PDWNC_BASE              EQU     IO_VIRT  + 0x28000

; BIM registers
REG_RO_ICEMODE          EQU     BIM_BASE + 0x0      ; ICE Control register
BIT_RISCICE         EQU     1
REG_RW_REMAP            EQU     BIM_BASE + 0x1C     ; Remap register
BIT_REMAP           EQU     1
REG_RW_IRQEN            EQU     BIM_BASE + 0x34     ; IRQ Enable register
REG_WO_IRQCL            EQU     BIM_BASE + 0x38     ; IRQ Clear register
REG_RW_FIQEN            EQU     BIM_BASE + 0x40     ; FIQ Enable register
REG_WO_FIQCL            EQU     BIM_BASE + 0x44     ; FIQ Clear register
REG_RW_MISC             EQU     BIM_BASE + 0x98     ; Misc register
BIT_INT_ST          EQU     (1 << 5)

REG_RW_TIMER_CFG        EQU     BIM_BASE + 0x78     ; timer control
REG_RW_TIMER2_LMT_LOW   EQU     BIM_BASE + 0x70
REG_RW_TIMER2_LMT_HIGH  EQU     BIM_BASE + 0x190
REG_RW_TIMER2_VAL_LOW   EQU     BIM_BASE + 0x74
REG_RW_TIMER2_VAL_HIGH  EQU     BIM_BASE + 0x194

;************************************
;*  INTERRUPT DEFINES               *
;************************************

IRQ_BASE                EQU     BIM_BASE + 0x30
FIQ_BASE                EQU     BIM_BASE + 0x3c

INT_IRQ_STATUS_OFFSET       EQU     0
INT_IRQ_ENABLE_OFFSET       EQU     4
INT_IRQ_CLEAR_OFFSET        EQU     8
INT_FIQ_STATUS_OFFSET       EQU     0
INT_FIQ_ENABLE_OFFSET       EQU     4
INT_FIQ_CLEAR_OFFSET        EQU     8

INT_CLEAR_VALUE         EQU     0xFFFFFFFF

;************************************
;*  INTERRUPT VECTOR DEFINES        *
;************************************
; Defines for the vector numbers associated with each interrupt.  The vector
; is determined by the bit position of the interrupt in the IRQ pending
; register.
;
IRQ_MISCI               EQU     31      ; MISC
IRQ_MISC2I              EQU     30      ; MISC2
IRQ_PODI                EQU     29      ; POD
IRQ_FCII                EQU     28      ; FCI
IRQ_DEMODI              EQU     27      ; Demod
IRQ_DMXRI               EQU     26      ; Transpot demuxer
IRQ_GRAI                EQU     25      ; Graphics
IRQ_VDECI               EQU     24      ; VLD/Video Decode
IRQ_PPI                 EQU     23      ; Post Processing
IRQ_SNORI               EQU     22      ; Serial Flash
IRQ_B2RI                EQU     21      ; B2R
IRQ_VDOIN               EQU     20      ; VDOIN
IRQ_OSDI                EQU     19      ; OSD
IRQ_LEDI                EQU     18      ; LED
IRQ_RS232I              EQU     17      ; RS232
IRQ_DSPI                EQU     16      ; Audio DSP
IRQ_USB0I               EQU     15      ; USB0
IRQ_AUDIOI              EQU     14      ; Audio: SPDIF/PSR
IRQ_GDMAI               EQU     13      ; GDMA
IRQ_IMGRZI              EQU     12      ; IMGRZ
IRQ_USB1I               EQU     11      ; USB1
IRQ_PSCANI              EQU     10      ; PScan
IRQ_DTCPI               EQU     9       ; Ethernet DTCP
IRQ_EMACI               EQU     8       ; Internal Ethernet MAC
IRQ_WDTI               EQU     7       ; WDT
IRQ_SMCARDI             EQU     6       ; Smart Card
IRQ_T2I                 EQU     5       ; Timer 2
IRQ_T1I                 EQU     4       ; Timer 1
IRQ_T0I                 EQU     3       ; Timer 0
IRQ_NFII                EQU     2       ; NAND Flash interface
IRQ_SIFI                EQU     1       ; Serial interface
IRQ_PDWNCI              EQU     0       ; PDWNC

;*************************************************************************
;*  CKGEN Register definitions
;*************************************************************************

REG_RW_CLKSEL0      EQU     CTL_BASE + 0x38
REG_RW_CLKSEL1      EQU     CTL_BASE + 0x3c
REG_RW_CLKSEL2      EQU     CTL_BASE + 0x40
REG_RW_CLKSEL3      EQU     CTL_BASE + 0x44
REG_RW_CLKSEL4      EQU     CTL_BASE + 0x48
REG_RW_CLKSEL5      EQU     CTL_BASE + 0x4c
REG_RW_CLKSEL6      EQU     CTL_BASE + 0x50
REG_RW_CLKSEL7      EQU     CTL_BASE + 0x54

;*************************************************************************
;*  DRAM clock
;*************************************************************************

;DMPLL_CLOCK         EQU     202500000
;DMPLL_CLOCK         EQU     189000000
;DMPLL_CLOCK         EQU     175500000
;DMPLL_CLOCK         EQU     162000000
;DMPLL_CLOCK         EQU     148500000
;DMPLL_CLOCK         EQU     135000000

;*************************************************************************
REG_RW_IOPAD        EQU     PDWNC_BASE + 0x80


;*************************************************************************
;*  Column address bit number
;*************************************************************************

    [   :LNOT::DEF:FORCE_COL_ADDR_BIT_NUM_9

COL_ADDR_BIT_NUM    EQU     10      ; 32M * 16

    |

COL_ADDR_BIT_NUM    EQU     9       ; 16M * 16

    ]

;*************************************************************************

    END
