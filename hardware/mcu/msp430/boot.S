	.extern _etext
	.extern __data_start
	.extern _edata
	.extern __bss_start
	.extern __bss_end
	.extern __stack

	.section .init0, "ax", @progbits

	.global	__15_interrupt
.func __15_interrupt
__15_interrupt:
	mov	#23168, &288	

	.section .init2, "ax", @progbits
	.global _copy_data_init__
	.weak   _copy_data_init__

_copy_data_init__:
	mov     #_etext,	r15     ; load r15 with end of .text segment
	mov     #__data_start,  r14     ; load ram start
	mov     #_edata,	r13     ; end of data segment
	cmp     r14,	    r13
	jeq     .Lend_of_data_loop 
.Lcopy_data_loop:  
	/* copy data from @r15 to @r14 */
	mov.b   @r15+,  @r14	    ; move one byte
	inc     r14
	cmp     r13,    r14	     ; check if end of data reached
	jlo     .Lcopy_data_loop
.Lend_of_data_loop:

	.section .init4, "ax", @progbits
	.global _clear_bss_init__
	.weak   _clear_bss_init__
	
_clear_bss_init__:
	mov     #__bss_start,   r15
	mov     #__bss_end,     r13
	cmp     r15,	    r13
	jeq     .Lend_of_bss_loop  
.Lzero_bss_loop:
	clr.b   @r15
	inc     r15 
	cmp     r13,    r15     ; check if r15 < r13
	jlo     .Lzero_bss_loop
.Lend_of_bss_loop:

	.section .init6,"ax",@progbits
	.global __do_global_ctors
	
__do_global_ctors:
	mov	#__stack, r1	; load stack... might be dangerous!!! 
	mov	#__ctors_start,	r11
	mov	#__ctors_end, r10
	cmp	r10, r11
	jeq	.L__ctors_end
.L__ctors_loop:
	call	@r11	; call constructor
	incd	r11
	cmp	r10, r11
	jne	.L__ctors_loop
.L__ctors_end:

    .section .init8, "ax", @progbits
	.global _end_of_init0__
	.weak   _end_of_init0__
	
_end_of_init0__:
	br      #_main_	;	jump to _main_ procedure

	.section .init9, "ax", @progbits
	.global _end_of_init__
	.weak   _end_of_init__
	
_end_of_init__:
	br      #main	;	jump to main procedure
.endfunc


	.section .fini6,"ax",@progbits
	.global __do_global_dtors
__do_global_dtors:
	mov	#__dtors_start, r11
	mov	#__dtors_end, r10
	cmp	r10, r11
	jeq	.L__dtors_end
.L__dtors_loop:
	call	@r11
	incd	r11
	cmp	r10, r11
	jne	.L__dtors_loop
.L__dtors_end:

	.section .fini9, "ax", @progbits
	.global __stop_progExec__
	.func  __stop_progExec__
__stop_progExec__:
	br      #main	;	jump to main procedure
.endfunc


