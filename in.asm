DEFAULT REL
section .bss
bbuf resb 64
section .text
	

in:
	xor rax, rax
	mov rsi, bbuf
	xor rdi, rdi
	mov rdx, 10
	syscall
	call string_to_dec
string_to_dec:
	xor rax, rax
	xor r9, r9
	mov r10, 10d
	cmp byte [rsi], 10d
	je .end
.cmp:
	add al, [rsi+r9]
	sub rax, '0'
	inc r9
	cmp byte [rsi+r9], 10d
	je .end
	mul r10
	jmp .cmp
.end:
	;div r10
	ret

