	.file	"calc.c"
	.text
.globl calcnew
	.type	calcnew, @function
calcnew:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx   
	movl	16(%ebp), %ecx  
        movl    %edx, %eax      
	sall    $2, %edx        
        subl    %eax, %edx  
        movl    %ecx, %eax  
        sall    $4, %ecx        
        subl    %eax, %ecx      
        subl    %eax, %ecx      
        addl    %edx, %ecx  
	movl	12(%ebp), %eax  
        movl    %eax, %edx  
        sall    $3, %eax        
        subl    %edx, %eax      
	addl	%ecx, %eax  
	popl	%ebp
	ret
	.size	calcnew, .-calcnew
	.ident	"GCC: (Ubuntu 4.3.3-5ubuntu4) 4.3.3"
	.section	.note.GNU-stack,"",@progbits
