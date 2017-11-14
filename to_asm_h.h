
#ifndef COMPILER_TO_ASM_H_H
#define COMPILER_TO_ASM_H_H

/*How to translate different types of nodes*/
int To_Asm_Out(elem * element);
int To_Asm_Op(elem * element);
int To_Asm_Var(elem * element);
int To_Asm_Number(elem * element);
int To_Asm_Ret(elem * element);
int To_Asm_Func(elem * element);
int To_Asm_If(elem * element);
int To_Asm_In(elem * element);
int To_Asm_Els(elem * element);
int To_Asm_New_Func(elem * element);
int To_Asm_Ass(elem * element);
void Super_Switch(elem * element);//A function to change the condition in "if" to create proper jumps

#endif //COMPILER_TO_ASM_H_H
