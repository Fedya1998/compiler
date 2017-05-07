DEFAULT REL
section .bss
buf  resb    64      ; output buffer


section .text


%macro print_char 1      ; a macro with 1 par = str[i]
        mov     rax, 1   ; syscall  1 write
        mov     rdi, 1   ; fd = stdout(1)
        lea     rsi, %1  ; address = &str[i]
        mov     rdx, 1   ; length = 1
        syscall 
%endmacro


global printff:function

printff:

	mov r13, rdi
	pop     r11
        push    r9
        push    r8
        push    rcx     
        push    rdx    
        push    rsi
        push    rbp
        push    rbx
        push    r11
        mov     rbp, rsp

        mov     r8, 0
        mov     r9, 0
	mov	r12, 0
	mov 	r14, 0
	  

.che_tam_print:
	mov r9, 0
	cmp byte [r13+r8], r9b
	je .return
	mov r9, [prc]
	cmp byte [r13+r8], r9b
	je .percent
	mov r9, [r13+r8]
	mov [buf+r12], r9
	print_char [buf+r12]
	inc r12
	inc r8
	jmp .che_tam_print
	.return:
	pop r11
	pop rbx
	pop rbp
	pop rsi
	pop rdx
	pop rcx
	pop r8
	pop r9
	push r11
	;push r11
	ret


.percent:
	inc r14
	inc r8

	mov r9, prc
	cmp byte [r13+r8], r9b
	je .percent_percent
	
	mov r9, 'd'
	cmp byte [r13+r8], r9b
	je .percent_dec

	mov r9, 'o'
        cmp byte [r13+r8], r9b
        je .percent_oct

	mov r9, 'b'
        cmp byte [r13+r8], r9b
        je .percent_bin

	mov r9, 'x'
        cmp byte [r13+r8], r9b
        je .percent_hex

	mov r9, 's'
        cmp byte [r13+r8], r9b
        je .percent_string

	mov r9, 'c'
        cmp byte [r13+r8], r9b
        je .percent_char

.percent_percent:
	inc r8
	mov rax, [prc]
	mov [buf+r12], rax
	print_char [buf+r12]
	inc r12
	jmp .che_tam_print

.percent_string:
	inc r8
	mov r9, [rbp+16+8*r14]
	.print_string_begin:
	cmp byte [r9], 0
	je .print_end
	mov rax, [r9]
        mov [buf+r12], rax
        print_char [buf+r12]
        inc r12
	inc r9
	jmp .print_string_begin
	.print_end:
	jmp .che_tam_print




.percent_char:
	inc r8
	mov rax, [rbp+16+8*r14]
	mov [buf+r12], rax
	print_char [buf+r12]
	inc r12
	jmp .che_tam_print

.percent_dec:
	inc r8
	mov rax, [rbp+16+8*r14]
	mov r9, 0xa
	push 0

.print_start:
	xor rdx, rdx
	div r9
	cmp rdx, 10
	add rdx, '0'
	push rdx
	cmp rax, 0
	jne .print_start
	pop r9
.razverni_chislo:
	mov [buf+r12], r9
	print_char [buf+r12]
	inc r12
	pop r9
	cmp r9, 0
	jne .razverni_chislo
	jmp .che_tam_print
	


.percent_bin:
	inc r8
	push 0
        mov rax, [rbp+16+8*r14]
        mov r9, 2d
	jmp .print_start

.percent_hex:
	inc r8
	push 0
	mov rax, [rbp+16+8*r14]
	.start_hex:
	mov     rdx, rax
        and     rdx, 1111b
	cmp rdx, 0
	je .exact_print_hex
        shr     rax, 4
        cmp     rdx, 10
        jb      .A_to_F
        sub     rdx, 10       
        add     rdx, 'A'

	.print_hex:
	push rdx

	jmp .start_hex
.A_to_F:
        add     rdx, '0'  
	jmp .print_hex

.exact_print_hex:
	pop r9
	cmp r9, 0
	je .che_tam_print
	mov [buf+r12], r9
	print_char [buf+r12]
        inc r12
	jmp .exact_print_hex
	

.percent_oct:
	inc r8
	push 0
        mov rax, [rbp+16+8*r14]
        mov r9, 8d
	jmp .print_start






section .data


prc db '%'
