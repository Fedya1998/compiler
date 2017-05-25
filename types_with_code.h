//
// Created by fedya on 05.02.17.
//

#ifndef COMPILER_TYPES_WITH_CODE_H
#define COMPILER_TYPES_WITH_CODE_H

#endif

DEF_TYPE(TYPE_IF, 1, {To_Asm_If(element); break;})
DEF_TYPE(TYPE_ELS, 2, {To_Asm_Els(element); break;})
DEF_TYPE(TYPE_IN, 3, {To_Asm_In(element); break;})
DEF_TYPE(TYPE_OUT, 4, {To_Asm_Out(element); break;})
DEF_TYPE(TYPE_NEW, 5, {break;})
DEF_TYPE(TYPE_ASS, 6, {To_Asm_Ass(element); break;})
DEF_TYPE(TYPE_RET, 7, {To_Asm_Ret(element); break;})
DEF_TYPE(TYPE_END, 8, {f_asm(fprintf(prog, "\tmov rax, 60d\n\tsyscall\n");) f_bin(f_ch(0xb8); f_ch(0x3c); f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x0f); f_ch(0x05);) break;})
DEF_TYPE(TYPE_OP, 13, {To_Asm_Op(element); break;})
DEF_TYPE(TYPE_NUMBER, 20, {To_Asm_Number(element); break;})
DEF_TYPE(TYPE_VAR, 21, {To_Asm_Var(element); break;})
DEF_TYPE(TYPE_FUNC, 22, {To_Asm_Func(element); break;})
DEF_TYPE(TYPE_NEW_FUNC, 24, {To_Asm_New_Func(element); break;})

