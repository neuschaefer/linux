	.align	4  
	.text
	.globl invalidate_dcache	        /*can not use push and pop, because inval will discard the data in the stack*/
invalidate_dcache:
	/* Corrupted registers: r0-r7, r9-r11 (r6 only in Thumb mode) */
	dmb					/* ensure ordering with previous memory accesses */
	MRC     p15, 1, r0, c0, c0, 1       /*read clidr                              */
	ANDS    r3, r0, #0x7000000          /*extract loc from clidr                  */
	MOV     r3, r3, lsr #23             /*left align loc bit field                */
	BEQ     inv_finished                    /*if loc is 0, then no need to clean      */
	mov     r10, #0                     /*start clean at cache level 0            */
inv_loop1:                                   
	ADD     r2, r10, r10, lsr #1        /*work out 3x current cache level         */
	MOV     r1, r0, lsr r2              /*extract cache type bits from clidr      */
	AND     r1, r1, #7                  /*mask of the bits for current cache only */
	CMP     r1, #2                      /*see what cache we have at this level    */
	BLT     inv_skip                        /*skip if no cache, or just i-cache       */
	MCR     p15, 2, r10, c0, c0, 0      /*select current cache level in cssr      */
	ISB                                 /*isb to sych the new cssr&csidr          */
	MRC     p15, 1, r1, c0, c0, 0       /*read the new csidr                      */
	AND     r2, r1, #7                  /*extract the length of the cache lines   */
	ADD     r2, r2, #4                  /*add 4 (line length offset)              */
	LDR     r4, =0x3ff                  
	ANDS    r4, r4, r1, lsr #3          /*find maximum number on the way size     */
	CLZ     r5, r4                      /*find bit position of way size increment */
	LDR     r7, =0x7fff               
	ANDS    r7, r7, r1, lsr #13         /*extract max number of the index size    */
inv_loop2:                                  
	MOV     r9, r4                      /*create working copy of max way size     */
inv_loop3:                                  
	ORR     r11, r10, r9, lsl r5        /*factor way and cache number into r11    */
	ORR     r11, r11, r7, lsl r2        /*factor index number into r11            */
	MCR     p15, 0, r11, c7, c6, 2	      /*invalidate by set/way                  */
	SUBS    r9, r9, #1                  /*decrement the way                       */
	BGE     inv_loop3                       /*                                        */
	SUBS    r7, r7, #1                  /*decrement the index                     */
	BGE     inv_loop2                       /*                                        */
inv_skip:                                   /*                                        */
	ADD     r10, r10, #2                /*increment cache number                  */
	CMP     r3, r10                     /*                                        */
	BGT     inv_loop1                       /*                                        */
inv_finished:                                /*                                        */
	MOV     r10, #0                     /*swith back to cache level 0             */
	
	MCR     p15, 2, r10, c0, c0, 0      /*select current cache level in cssr      */
	dsb
	ISB                                 /*                                        */
	
	MOV     pc, lr                      /*                                        */
    
	.align	4  
	.text
	.globl jump_to_resume
jump_to_resume:
	/*before jump to resume, invalidate the data*/                                                                          
	/* Set the return pointer */                                                 
	mov     r12, r0
	mov 	r8, r1
	bl	invalidate_dcache
	mov	r1, r8
	mov	lr, r12
	ldmia   r1, {r0 - r13}                                                   
        mov     pc, lr

	.align	4  

	.globl jump_to_resume0
jump_to_resume0: 
        /* Set the return pointer */                                                        
	mov     lr, r0
	mov     pc, lr

	.align	4  

	.globl jump_to_resume0_nommu
jump_to_resume0_nommu:  
	/*read cr*/
        MRC p15,0,r1,c1,c0,0
        BIC r1, #0x1000
        BIC r1, #0x0007
        /*write cr: disable cache and mmu*/
        MCR p15,0,r1,c1,c0,0
	/*read id reg*/
        mrc p15, 0, r3, c0, c0, 0 
        mov r3, r3
        mov r3, r3                                                  
	/* Set the return pointer */                                                        
	mov     lr, r0
	isb
	mov     pc, lr

	.align	4  
	.globl save_runtime_context
save_runtime_context:
	/*save r0-r13 register*/   
	stmia   r0, {r0 - r13}
	mov	pc, lr
                                                

	.align	4  
	.globl clear_reg_context
clear_reg_context:
	/*clear r0-r11 register*/   
	mov	r0, #0  
	mov	r1, #0 
	mov	r2, #0  
	mov	r3, #0
	mov	r4, #0  
	mov	r5, #0
	mov	r6, #0  
	mov	r7, #0
	mov	r8, #0  
	mov	r9, #0
	mov	r10,#0
	mov	r11,#0
	mov	r12,#0
	mov	pc, lr




