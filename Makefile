all:
	nasm -f elf64 proga.asm
	#nasm -f elf64 printff.asm
	#nasm -f elf64 in.asm
	ld -m elf_x86_64 proga.o -o a.out
	ld -m elf_x86_64 proga.o -o a2.out
	#nasm -f elf64 proga.asm
	#ld -m elf_x86_64 proga.o
