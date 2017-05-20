//
// Created by fedya on 18.02.17.
//


#ifndef COMPILER_TO_ASM_FUNCTIONS_H
#define COMPILER_TO_ASM_FUNCTIONS_H

#endif //COMPILER_TO_ASM_FUNCTIONS_H


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
/*
    if (element->left->data_type == TYPE_OP || element->left->data_type == TYPE_FUNC)
        To_Intel_Asm(element->left);
    if (element->right->data_type == TYPE_OP|| element->right->data_type == TYPE_FUNC)
        To_Intel_Asm(element->right);
*/
    /*if (* ((int *) &element->value) == CMD_POW) {
        if (element->left)
            To_Asm(element->left);
        if (element->right)
            To_Asm(element->right);
    }*/

    To_Intel_Asm(element->left);
    f_asm(to_f("\tpop rax\n");)
    f_bin(fprintf(prog, "%x", 0x58);)

    To_Intel_Asm(element->right);
    f_asm(to_f("\tpop rdx\n");)
    f_bin(fprintf(prog, "%x", 0x5a);)

    int value = *((int*)(&(element->value)));


    switch (value){
#define DEF_CMD(name, num)                       \
        case num: {                               \
            p(14, ("Eeee podoshlo %s\n", #name));  \
            if (num >= CMD_ja){                     \
                to_f("\tcmp ");                      \
            }                                         \
            else                                       \
                fprintf(prog, "\t%s ", #name);          \
            break;                                       \
        }
#include "supercmd.h"
        default: {
            p(10 ,("Undefined operation %i\n", *((int*)(&(element->value)))));
            exit(ERROR_UNDEFINED_OPERATION);
            break;
        }
    }
    if (value == CMD_jmp) {
        f_asm(fprintf(prog, "\n");)
        return 0;
    }

    if (value == CMD_mul || value == CMD_div) {
        f_asm(to_f("rdx\n\tpush rax\n");)
        f_bin()
    }
    else {
        f_asm(to_f("rax, rdx\n");)
        f_bin()
        if (value < CMD_ja) {
            f_asm(to_f("\tpush rax\n");)
            f_bin(fprintf(prog, "%x", 50);)
        }

    }


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
        f_asm(to_f("\tmov rdi, msgvardec\n");)
        f_bin()

        To_Asm_Var(element->right);

        f_asm(to_f("\tpop rsi\n");)
        f_bin()
        f_asm(to_f("\tcall printff\n");)
        f_bin(fprintf(prog, "%lx", 0xe83ffdffff);)
    }
    else {
        f_asm(fprintf(prog, "\tmov rdi, msgstring\n");)
        f_bin(fprintf(prog, "%lx%.10lx", 0x48bf690460, 0x0000000000);)

        f_asm(fprintf(prog, "\tmov rsi, %s\n", (char *) element->right->value);)
        f_bin(fprintf(prog, "%x%lx", 0x48be07 + *(((char *) element->right->value) + 3), 0x04600000000000);)
        f_asm(to_f("\tcall printff\n");)
        f_bin()
    }

}

int To_Asm_Var(elem * element){
    fprintf(prog, "\tmov r9, [rsp-8*%i]\n\tpush r9\n", *(int *)(&(element->value)));
    return 0;
}

int To_Asm_Number(elem * element){
    fprintf(prog, "\tmov r9, %lgd\n\tpush r9\n", *(double *) element->value);//
}

int To_Asm_Ret(elem * element){
    if (element->right->data_type == TYPE_NUMBER || element->right->data_type == TYPE_VAR) {
        To_Intel_Asm(element->right);
        to_f("\tpop rax\n");
    }
    else {
        To_Intel_Asm(element->right);
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
        if (element->left->left == NULL)
            to_f("\tret\n");

    }
    return 0;
}

int To_Asm_Els(elem * element){
    return 0;
}

int To_Asm_Ass(elem * element){
    To_Intel_Asm(element->right);
    fprintf(prog, "\tpop r9\n\tmov [rsp-8*%i], r9\n", element->value);
}

int To_Asm_In(elem * element){
    f_asm(
            fprintf(prog,
            "\txor rax, rax\n"
            "\tmov rsi, bbuf\n"
            "\txor rdi, rdi\n"
            "\tmov rdx, 10\n"
            "\tsyscall\n"
            "\tcall string_to_dec\n"
            "\tmov [rsp-8*%i], rax\n", element->right->value);
    )
    f_bin(
            fprintf(prog, "%x%lx%.10lx%x%x%x%x", 0x4831c0, 0x48beac0460, 0x0000000000, 0x4831ff, 0xba0a000000, 0x0f05, 0xe8a0ffffff );
    )


}

int To_Asm_Func(elem * element){
    //if (element->left->data_type == TYPE_OP)
    To_Intel_Asm(element->left);


    //To_Intel_Asm(element->left);
    to_f("\tpop rax\n");

    fprintf(prog, "\tcall %s\n", (char *) element->value);
    to_f("\tpush rax\n");
    element->data_type = TYPE_REG;
    element->value = 0;
    Mini_Delete(element->left);
    element->left = NULL;
    return 0;
}

int To_Asm_New_Func(elem * element){
    cur_func = 1; fprintf(prog, "%s:\n\tpush rbp \n"
            "\tmov rbp, rsp\n"
            "\tsub rsp, 80\n"
            "\tmov [rsp-8*%i], rax\n",
                          (char *) element->value,
                          *(int*)(&(element->right->value)));
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
