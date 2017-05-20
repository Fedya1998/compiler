DEFAULT REL
%include "printff.asm"
%include "in.asm"
section .text
	global _start
	
huec:
	push rax
	pop rax
	ret

_start:
	push rax
	jmp .aa
	pop rax
	.aa:
	cmp rax, rbx
	jne .aa
	.bb:
	call huec
	call eee_blet
	call huec
	call eee_blet
	call printff
	syscall
eee_blet:
	push rax
	ret


section .data
	hooi db '%s', 0
	pizda db '%d', 0
