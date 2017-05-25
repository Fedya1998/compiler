DEFAULT REL
%include "printff.asm"
%include "in.asm"
section .text
	global _start
_start:
	mov rdi, msgstring
	mov rsi, msg0
	call printff
	call in
	mov [rbp-8], rax
	mov r9, [rbp-8]
	push r9
	pop rax
	call Factorial
	push rax
	pop r9
	mov [rbp-8], r9
	mov rdi, msgstring
	mov rsi, msg1
	call printff
	mov rdi, msgvardec
	mov r9, [rbp-8]
	push r9
	pop rsi
	call printff
	mov rax, 60d
	syscall
Factorial:
	push rbp 
	mov rbp, rsp
	sub rsp, 80
	mov [rbp-8*1], rax
	mov r9, [rbp-8*1]
	push r9
	pop rax
	mov r9, 1d
	push r9
	pop rdx
	cmp rax, rdx
	jae .lbl0
	mov r9, 1d
	push r9
	pop rax

	mov rsp, rbp
	pop rbp
	ret
	jmp .lble0
.lbl0:
	mov r9, [rbp-8*1]
	push r9
	pop rax
	mov r9, 1d
	push r9
	pop rdx
	sub rax, rdx
	push rax
	pop rax
	call Factorial
	push rax
	pop rax
	mov r9, [rbp-8*1]
	push r9
	pop rdx
	mul rdx
	push rax

	mov rsp, rbp
	pop rbp
	ret
.lble0:
	ret
section .data
msg0 db 'Enter_number_to_fucktorial', 0
msg1 db 'Result_', 0
msgvardec db '%d', 0
msgstring db '%s', 0
