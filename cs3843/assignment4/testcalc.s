	.file	"testcalc.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"x=%d, y=%d, z=%d, result=%d\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$36, %esp
	movl	$11, 8(%esp)
	movl	$6, 4(%esp)
	movl	$2, (%esp)
	call	calc
	movl	%eax, 20(%esp)
	movl	$11, 16(%esp)
	movl	$6, 12(%esp)
	movl	$2, 8(%esp)
	movl	$.LC0, 4(%esp)
	movl	$1, (%esp)
	call	__printf_chk
	movl	$0, %eax
	addl	$36, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
