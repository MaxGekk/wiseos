    .section .vectors, "a", @progbits
    .word	.Lint_0		//0xffe0
    .word	.Lint_1		//0xffe2	
    .word	.Lint_2		//0xffe4
    .word	.Lint_3		//0xffe6
    .word	.Lint_4		//0xffe8
    .word	.Lint_5		//0xffeA Timer A CC1-2,TA
    .word	.Lint_6		//0xffeC Timer A CC0
    .word	.Lint_7		//0xffeE
    .word	.Lint_8		//0xfff0
    .word	.Lint_9		//0xfff2
    .word	.Lint_10	//0xfff4
    .word	.Lint_11	//0xfff6
    .word	.Lint_12	//0xfff8 Timer B CC1..,TB
    .word	.Lint_13	//0xfffA Timer B CC0
    .word	.Lint_14	//0xfffC
    .word	__15_interrupt
    .section .text
    .global __irq_trap
.func __irq_trap
__irq_trap:
.Lint_0:
    push  r15 //First register for function parameters
    mov   #0,r15
    jmp   .Lmsp_int
.Lint_1:
    push  r15 
    mov   #1,r15
    jmp   .Lmsp_int
.Lint_2:
    push  r15 
    mov   #2,r15
    jmp   .Lmsp_int
.Lint_3:
    push  r15 
    mov   #3,r15
    jmp   .Lmsp_int
.Lint_4:
    push  r15 
    mov   #4,r15
    jmp   .Lmsp_int
.Lint_5:
    push  r15 
    mov   #5,r15
    jmp   .Lmsp_int
.Lint_6:
    push  r15 
    mov   #6,r15
    jmp   .Lmsp_int
.Lint_7:
    push  r15 
    mov   #7,r15
    jmp   .Lmsp_int
.Lint_8:
    push  r15 
    mov   #8,r15
    jmp   .Lmsp_int
.Lint_9:
    push  r15 
    mov   #9,r15
    jmp   .Lmsp_int
.Lint_10:
    push  r15 
    mov   #10,r15
    jmp   .Lmsp_int
.Lint_11:
    push  r15 
    mov   #11,r15
    jmp   .Lmsp_int
.Lint_12:
    push  r15 
    mov   #12,r15
    jmp   .Lmsp_int
.Lint_13:
    push  r15 
    mov   #13,r15
    jmp   .Lmsp_int
.Lint_14:
    push  r15 
    mov   #14,r15
.Lmsp_int:

//  Не сохраняем регистры, потому что используемые
//  в обработчике прерывания регистры будут сохранены в эпилоге    
//    push  r14
//    push  r13
//    push  r12
//    push  r11
//    push  r10
//    push  r9
//    push  r8
//    push  r7
//    push  r6
//    push  r5
//    push  r4

    call   #process_irq

//    pop   r4
//    pop   r5
//    pop   r6
//    pop   r7
//    pop   r8
//    pop   r9
//    pop   r10
//    pop   r11
//    pop   r12
//    pop   r13
//    pop   r14

    pop   r15
    reti
.endfunc

