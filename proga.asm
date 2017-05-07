%include "io.inc"
%include "printff.asm"
section .text
	global  _start
_start:
	mov rdi, msgstring
	mov rsi, msg0
	call printff
	GET_DEC 4, [rsp-8*0]

	mov rax, [rsp-8*0]

	call Factorial
	mov [rsp-8*0], rax
	pop [rsp-8*0]
	mov rdi, msgstring
	mov rsi, msg1
	call printff
	mov rdi, msgvardec
	mov rsi, [rsp-8*0]
	call printff
	mov rax, 60d
	syscall
Factorial:
	push rbp 
	mov rbp, rsp
	sub rsp, 80
	mov [rsp-8*0], rax
	cmp [rsp-8*0], 1d
	jae .lbl0
	mov rax, 1d
	mov rsp, rbp
	pop rbp
	ret
	jmp .lble0
.lbl0:
	sub [rsp-8*0], 1d

	mov rax, [rsp-8*0]

	call Factorial
	mov [rsp-8*0], rax
	mul rax, [rsp-8*0]
	mov rax, rax
	mov rsp, rbp
	pop rbp
	ret
.lble0:
section .data
msg0 db Enter_number_to_fucktorial, 0
msg1 db Result_, 0
msgvardec db "%d", 0
msgstring db "%s", 0
