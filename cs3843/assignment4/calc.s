	.file	"calc.c"
	.text
.globl calc
	.type	calc, @function
calc:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	16(%ebp), %ecx
	leal	(%edx,%edx,2), %edx
	movl	12(%ebp), %eax
	leal	(%edx,%eax,2), %eax
	movl	%ecx, %edx
	sall	$4, %edx
	subl	%ecx, %edx
	addl	%edx, %eax
	popl	%ebp
	ret
	.size	calc, .-calc
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
