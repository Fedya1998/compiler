//
// Created by fedya on 18.02.17.
//


#ifndef MNE_JOPA_TO_ASM_FUNCTIONS_H
#define MNE_JOPA_TO_ASM_FUNCTIONS_H

#endif //MNE_JOPA_TO_ASM_FUNCTIONS_H


int To_Intel_Asm(elem * element){
    assert(element);
    switch (element->data_type){
#define DEF_TYPE(name, num, code)                               \
        case num: {                                              \
            p(11, ("Eeee podoshlo %s\n", #name));                 \
            code;                                                  \
        }
#include "types_with_code.h"
#undef DEF_TYPE

        default: {
            printf ("Undefined type %i\n", element->data_type);
            _error = ERROR_UNDEFINED_TYPE;
            return _error;
        }

    }
    if (element->left && element->data_type != TYPE_OP && element->data_type != TYPE_FUNC)
        To_Intel_Asm(element->left);
    return 0;

}

int To_Asm_Op(elem * element){
    assert(element);

    if (element->left->data_type == TYPE_OP || element->left->data_type == TYPE_FUNC)
        To_Intel_Asm(element->left);
    if (element->right->data_type == TYPE_OP|| element->right->data_type == TYPE_FUNC)
        To_Intel_Asm(element->right);

    /*if (* ((int *) &element->value) == CMD_POW) {
        if (element->left)
            To_Asm(element->left);
        if (element->right)
            To_Asm(element->right);
    }*/



    switch (*((int*)(&(element->value)))){
#define DEF_CMD(name, num)                       \
        case num: {                               \
            p(14, ("Eeee podoshlo %s\n", #name));  \
            if (num >= CMD_ja){                     \
                to_f("\tcmp ");                        \
            }                                         \
            else                                       \
                fprintf(prog, "\t%s ", #name);            \
            break;                                       \
        }
#include "supercmd.h"
        default: {
            p(10 ,("Undefined operation %i\n", *((int*)(&(element->value)))));
            exit(ERROR_UNDEFINED_OPERATION);
            break;
        }
    }
    if (*((int*)(&(element->value))) == CMD_jmp) {
        fprintf(prog, "\n");
        return 0;
    }

    To_Intel_Asm(element->left);
    to_f(", ");
    To_Intel_Asm(element->right);
    to_f("\n");

    element->value = element->left->value;
    element->data_type = element->left->data_type;
    Mini_Delete(element->left);
    Mini_Delete(element->right);
    element->left = NULL;
    element->right = NULL;

    return 0;
}

int To_Asm_Out(elem * element){

    if (element->right->data_type == TYPE_VAR) {
        to_f("\tmov rdi, msgvardec\n");
        to_f("\tmov rsi, ");
        To_Asm_Var(element->right);
        to_f("\n\tcall printff\n");
    }
    else {
        fprintf(prog, "\tmov rdi, msgstring\n");
        fprintf(prog, "\tmov rsi, %s\n", (char *) element->right->value);
        to_f("\tcall printff\n");
    }

}

int To_Asm_Var(elem * element){
    fprintf(prog, "[rsp-8*%i]", *(int *)(&(element->value)));
    return 0;
}

int To_Asm_Number(elem * element){
    fprintf(prog, "%lgd", *(double *) element->value);
}

int To_Asm_Ret(elem * element){
    if (element->right->data_type == TYPE_NUMBER || element->right->data_type == TYPE_VAR) {
        to_f("\tmov rax, ");
        To_Intel_Asm(element->right);
    }
    else {
        To_Intel_Asm(element->right);
        to_f("\tmov rax, ");//This can be removed
        To_Intel_Asm(element->right);//be removed
    }

    to_f("\n");
    fprintf(prog, "\tmov rsp, rbp\n\tpop rbp\n\tret\n");
}

int To_Asm_If(elem * element) {
    int label = cur_label++;
    Super_Switch((elem *) element->value);
    int super_value = *(int *) (&(((elem *)element->value)->value));
    To_Asm_Op((elem *) element->value);//condition
    switch (super_value) {
#define DEF_CMD(name, num)                                \
        case num:{                                         \
            fprintf(prog, "\t%s .lbl%i\n", #name, label);   \
            break;                                           \
        }
        default:{
            printf("Da takogo ne mozhet bit\n");
            break;
        }

#include "supercmd.h"
#undef DEF_CMD
    }
    To_Intel_Asm(element->right);//code in if
    if (element->left->data_type == TYPE_ELS)
        fprintf(prog, "\tjmp .lble%i\n", label);
    fprintf(prog, ".lbl%i:\n", label);
    if (element->left->data_type == TYPE_ELS){
        To_Intel_Asm(element->left->right);
        fprintf(prog, ".lble%i:\n", label);
    }
    return 0;
}

int To_Asm_Els(elem * element){
    return 0;
}

int To_Asm_Func(elem * element){
    if (element->left->data_type == TYPE_OP)
        To_Asm_Op(element->left);

    to_f("\n\tmov rax, ");
    To_Intel_Asm(element->left);
    to_f("\n");

    fprintf(prog, "\n\tcall %s\n\tmov ", (char *) element->value);
    To_Intel_Asm(element->left);
    to_f(", rax\n");
    element->data_type = TYPE_REG;
    element->value = 0;
    Mini_Delete(element->left);
    element->left = NULL;
    return 0;
}

int To_Asm_Reg(elem * element){
    to_f("rax");
}

void Super_Switch(elem * element){
    assert(element);
    switch (*((int*)(&(element->value)))){
        case CMD_ja: {
            element->value = (void *) CMD_jbe;
            break;
        }
        case CMD_jb: {
            element->value = (void *) CMD_jae;
            break;
        }
        case CMD_je: {
            element->value = (void *) CMD_jne;
            break;
        }
        case CMD_jne: {
            element->value = (void *) CMD_je;
            break;
        }
        case CMD_jae: {
            element->value = (void *) CMD_jb;
            break;
        }
        case CMD_jbe: {
            element->value = (void *) CMD_ja;
            break;
        }
        default: {
            p(5, ("You've zasunool this super function in the wrong place\n"));
            d(1, exit(1));
        }
    }
}
