all:
	nasm -f elf64 proga.asm
	ld -m elf_x86_64 proga.o -o a.out
	ld -m elf_x86_64 proga.o -o a2.out
