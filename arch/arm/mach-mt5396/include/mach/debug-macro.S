/* linux/include/asm-arm/arch-mt5391/debug-macro.S
 *
 * Debugging macro include header
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author:  $
 * $Date:  $
 * $RCSfile:  $
 * $Revision:  $
 *
 */

#include <mach/hardware.h>

        .macro  addruart,rx
        mrc p15, 0, \rx, c1, c0
        tst \rx, #1                 @ MMU enabled?
        moveq   \rx, #IO_PHYS       @ physical base address
        movne   \rx, #IO_BASE       @ virtual base
        add     \rx, \rx, #0xc0 << 8
        .endm

        .macro  senduart,rd,rx
        strb    \rd, [\rx, #0x0]
        .endm

        .macro  waituart,rd,rx
1001:   ldr \rd, [\rx, #0x4]    @ UART0 status
        tst \rd, 0x2            @ write allow
        bne 1001b
        .endm

        .macro  busyuart,rd,rx
1002:   ldr \rd, [\rx, #0xcc]   @ UART0 status
        tst \rd, 0x2            @ uart full .eq. busy
        beq 1002b
        .endm

