.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptHandler15HandleInterruptEhj 

.macro HandleException num
.global _ZN16InterruptHandler26HandleInterruptRequest\num\()Ev
_ZN16InterruptHandler26HandleInterruptRequest\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptHandler26HandleInterruptRequest\num\()Ev
	_ZN16InterruptHandler26HandleInterruptRequest\num\()Ev:
		movb $\num + IRQ_BASE, (interruptnumber)
		jmp int_bottom
.endm

.global _ZN16InterruptHandler22IgnoreInterruptRequestEv

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:

	pusha
    	pushl %ds
   	pushl %es
    	pushl %fs
    	pushl %gs

	pushl %esp
	push (interruptnumber)
	call _ZN16InterruptHandler15HandleInterruptEhj
	mov %eax, %esp
	
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	
	iret

_ZN16InterruptHandler22IgnoreInterruptRequestEv:
	iret

.data
	interruptnumber: .byte 0
