//
// Created by fedya on 05.02.17.
//

#ifndef MNE_JOPA_TYPES_WITH_CODE_H
#define MNE_JOPA_TYPES_WITH_CODE_H

#endif

DEF_TYPE(TYPE_IF, 1, {To_Asm_If(element); break;})
DEF_TYPE(TYPE_ELS, 2, {To_Asm_Els(element); break;})
DEF_TYPE(TYPE_IN, 3, {fprintf(prog, "\tGET_DEC 4, [rsp-8*%i]\n", element->right->value); break;})
DEF_TYPE(TYPE_OUT, 4, {To_Asm_Out(element); break;})
DEF_TYPE(TYPE_NEW, 5, {break;})
DEF_TYPE(TYPE_ASS, 6, {To_Intel_Asm(element->right); fprintf(prog, "\tpop "); To_Asm_Var(element); fprintf(prog, "\n"); break;})
DEF_TYPE(TYPE_RET, 7, {To_Asm_Ret(element); break;})
DEF_TYPE(TYPE_END, 8, {fprintf(prog, "\tmov rax, 60d\n\tsyscall\n"); break;})
DEF_TYPE(TYPE_OP, 13, {To_Asm_Op(element); break;})
DEF_TYPE(TYPE_NUMBER, 20, {To_Asm_Number(element); break;})
DEF_TYPE(TYPE_VAR, 21, {To_Asm_Var(element); break;})
DEF_TYPE(TYPE_FUNC, 22, {To_Asm_Func(element); break;})
DEF_TYPE(TYPE_REG, 23, {To_Asm_Reg(element); break;})
DEF_TYPE(TYPE_NEW_FUNC, 24, {cur_func = 1; fprintf(prog, "%s:\n\tpush rbp \n\tmov rbp, rsp\n\tsub rsp, 80\n\tmov [rsp-8*%i], rax\n", (char *) element->value, *(int*)(&(element->right->value))); break;})

