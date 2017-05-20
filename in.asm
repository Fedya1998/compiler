DEFAULT REL
section .bss
bbuf resb 64
section .text
	
string_to_dec:
	xor rax, rax
	xor r9, r9
	mov r10, 10d
	cmp byte [rsi+r8], 10d
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

section .data
msg db 'string %s', 0
