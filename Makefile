all:
	nasm -f elf64 proga.asm
	ld -m elf_x86_64 prog.o
