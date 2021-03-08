	.file	"my_atoi.c"
	.text
.globl my_atoi
	.type	my_atoi, @function
my_atoi:
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rbx
#####################################################################
# DON'T EDIT ABOVE THIS LINE                                        #
#####################################################################
# atoi(char *buffer)                                                #
# parse a character string into an integer.                         #
# %rdi contains a pointer to the array of characters (string).      #
# Place your return value in %rax at the end.                       #
#                                                                   #
# Use registers %rax, %rcx, and %r8 - %r11.                         #
#####################################################################
# Write your code here...                                           #
#####################################################################	
               
movq $0, %rax
movq $0, %rcx
movq $47, %r8
movq $58, %r10
	
loop:

	movzbq (%rdi,%rcx), %r9
	inc  %rcx
	cmp %r8, %r9
	jle  endloop
	cmp %r10, %r9
	jge  endloop
	sub $48, %r9
	imul $10, %rax
	add  %r9, %rax
	jmp  loop
endloop:
	


	
##############################
# DON'T EDIT BELOW THIS LINE #
##############################
	popq	%rbx
	leave
	ret

	.size	my_atoi, .-my_atoi
	.ident	"GCC: (Debian 4.4.5-8) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
