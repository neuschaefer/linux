.global secretkey_service_call
secretkey_service_call:
	push  {r0-r6}      /* Save registers */
	mov   r6, r1       /* Crypto mode */
	ldr   r2, [r6]
	ldr   r3, [r6, #4]
	ldr   r4, [r6, #8]
	ldr   r5, [r6, #12]
	mov   r1, r0       /* Crypto mode */
	mov   r0, #1       /* Set service request */
	.word 0xE1600070   /* SMC #0 (call monitor) */
	str   r2, [r6]
	str   r3, [r6, #4]
	str   r4, [r6, #8]
	str   r5, [r6, #12]
	pop   {r0-r6}      /* Restore registers */
	bx    r14

.global enable_l2_service_call
enable_l2_service_call:
	push  {r0-r1}
	mov   r0, #3
	.word 0xE1600070
	pop   {r0-r1}
	bx    r14
