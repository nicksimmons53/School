	.file	"calc.c"
	.text
.globl calcnew
	.type	calcnew, @function
calcnew:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	16(%ebp), %ecx
	imull   $3, %edx
	movl	12(%ebp), %eax
        imull   $7, %eax
        addl    %edx, %eax
	movl	%ecx, %edx
        imull   $14, %edx
	addl	%edx, %eax
	popl	%ebp
	ret
	.size	calcnew, .-calcnew
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
