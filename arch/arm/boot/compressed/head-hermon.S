#include <asm/mach-types.h>

		.section	".start", "ax"
		mov     r7,     #(MACH_TYPE_HERMON & 0xff)
	        orr     r7, r7, #(MACH_TYPE_HERMON & 0xff00)
