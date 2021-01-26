.extern callConstructors
.extern KernelMain

.global loader

.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, (1 << 0) | (1 << 1)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

.section .multiboot
	.align 4
	.long MB_MAGIC
	.long MB_FLAGS
	.long MB_CHECKSUM

.section .bss
	.align 16
	stack_bottom:
		.skip 4096
	stack_top:

.section .text

	loader:
		mov $stack_top, %esp
		call callConstructors
		push %eax
		push %ebx

		call KernelMain
	hang:
		cli
		hlt
		jmp hang
