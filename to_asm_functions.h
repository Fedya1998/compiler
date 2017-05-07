#ifndef MNE_JOPA_TO_ASM_FUNCTIONS_H
#define MNE_JOPA_TO_ASM_FUNCTIONS_H

#endif //MNE_JOPA_TO_ASM_FUNCTIONS_H


int To_Asm(elem * element){
    assert(element);
    switch (element->data_type){
#define DEF_TYPE(name, num, code)\
        case num: {p(11, ("Eeee podoshlo %s\n", #name)); code;}
#include "types_with_code.h"
#undef DEF_TYPE

        default: {
            printf ("Undefined type %i\n", element->data_type);
            _error = ERROR_UNDEFINED_TYPE;
            return _error;
        }

    }
    if (element->left && element->data_type != TYPE_OP && element->data_type != TYPE_FUNC)
        To_Asm(element->left);
    return 0;

}

int To_Asm_Op(elem * element){
    assert(element);

    if (* ((int *) &element->value) == CMD_POW) {
        if (element->left)
            To_Asm(element->left);
        if (element->right)
            To_Asm(element->right);
    } else {
        if (element->right)
            To_Asm(element->right);
        if (element->left)
            To_Asm(element->left);
    }


    switch (*((int*)(&(element->value)))){
#define DEF_CMD(name, num)\
        case num: {p(14, ("Eeee podoshlo %s\n", #name)); if (num <8) {fprintf(to_proc, "%s\n", #name);} else {fprintf(to_proc, "%s ", #name);}  break;}
#include "supercmd.h"
        default: {
            p(10 ,("Undefined operation %i\n", *((int*)(&(element->value)))));
            exit(ERROR_UNDEFINED_OPERATION);
            break;
        }
    }

    return 0;
}

int To_Asm_Var(elem * element){
    fprintf(to_proc, "POPR %i\n", *((int*)(&(element->value))) + VAR_IN_FUNCTION * cur_func);
    return 0;
}

void Super_Switch(elem * element){
    assert(element);
    switch (*((int*)(&(element->value)))){
        case CMD_JA: {
            element->value = (void *) CMD_JBE;
            break;
        }
        case CMD_JB: {
            element->value = (void *) CMD_JAE;
            break;
        }
        case CMD_JE: {
            element->value = (void *) CMD_JN;
            break;
        }
        case CMD_JN: {
            element->value = (void *) CMD_JE;
            break;
        }
        case CMD_JAE: {
            element->value = (void *) CMD_JB;
            break;
        }
        case CMD_JBE: {
            element->value = (void *) CMD_JA;
            break;
        }
        default: {
            p(5, ("You've zasunool this super function in the wrong place\n"));
            d(1, exit(1));
        }
    }
}
