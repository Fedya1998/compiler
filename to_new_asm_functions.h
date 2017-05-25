//
// Created by fedya on 18.02.17.
//


#ifndef COMPILER_TO_ASM_FUNCTIONS_H
#define COMPILER_TO_ASM_FUNCTIONS_H

#endif //COMPILER_TO_ASM_FUNCTIONS_H

int write_int(FILE * file, int a){
    fprintf(file, "%c%c%c%c", *(char *)&a, *((char *)&a + 1), *((char *)&a + 2), *((char *)&a + 3));
}



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
    To_Intel_Asm(element->left);
    To_Intel_Asm(element->right);

    f_asm(to_f("\tpop rdx\n");)
    f_bin(fprintf(prog, "%c", 0x5a);)

    f_asm(to_f("\tpop rax\n");)
    f_bin(fprintf(prog, "%c", 0x58);)

    int value = *((int*)(&(element->value)));

    switch (value){
#define DEF_CMD(name, num, code)                                    \
        case num: {                                                  \
            p(14, ("Eeee podoshlo %s\n", #name));                     \
            if (num >= CMD_ja){                                        \
                f_asm(to_f("\tcmp ");)                                  \
                f_bin(fprintf(prog, "%c%c%c", 0x48, 0x39, 0xd0);)        \
            }                                                             \
            else {                                                         \
                f_asm(fprintf(prog, "\t%s ", #name);)                       \
                f_bin(code;)                                                 \
            }                                                                 \
            break;                                                             \
        }
#include "supercmd.h"
        default: {
            p(10 ,("Undefined operation %i\n", *((int*)(&(element->value)))));
            exit(ERROR_UNDEFINED_OPERATION);
        }
    }
#undef DEF_CMD
    if (value == CMD_jmp) {
        f_asm(fprintf(prog, "\n");)
        return 0;
    }

    if (value == CMD_mul || value == CMD_div) {
        f_asm(to_f("rdx\n\tpush rax\n");)
        f_bin(f_ch(0x50);)
    } else {
        f_asm(to_f("rax, rdx\n");)
        f_bin()
        if (value < CMD_ja) {
            f_asm(to_f("\tpush rax\n");)
            f_bin(f_ch(0x50);)
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
        f_bin(f_ch(0x48); f_ch(0xbf); fprintf(prog, "%c%c%c", *(char *) &MSGDEC, * ((char *)&MSGDEC + 1), 0x60);)
        f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x00);
        To_Asm_Var(element->right);

        f_asm(to_f("\tpop rsi\n");)
        f_bin(f_ch(0x5e);)
        f_asm(to_f("\tcall printff\n");)
        f_bin(
            f_ch(0xe8);
            write_int(prog, (int)(PRINTFF - ftell(prog) - 4));
        )

    }
    else {
        f_asm(fprintf(prog, "\tmov rdi, msgstring\n");)
        f_bin(
                f_bin(f_ch(0x48); f_ch(0xbf); fprintf(prog, "%c%c%c", *(char *) &MSGSTRING, *((char *)&MSGSTRING + 1), 0x60); f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x00);)
        )
        f_asm(fprintf(prog, "\tmov rsi, %s\n", (char *) element->right->value);)
        f_bin(

            long ptr_now = ftell(prog);
            fseek(prog, current_data, SEEK_SET);
            unsigned index = (unsigned int)(((char *) element->right->value)[3] - '0');
            fprintf(prog, "%s%c", super_list.search_by_index(index)->m_data->m_data, 0x00);
            short data_now = current_data;
            current_data = (short) ftell(prog);
            fseek(prog, ptr_now, SEEK_SET);
            f_ch(0x48);
            f_ch(0xbe);
            fprintf(prog, "%c%c%c", *((char *)&data_now), *((char *)&data_now + 1), 0x60);
            f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x00); f_ch(0x00);

        )
        f_asm(to_f("\tcall printff\n");)
        f_bin(
            f_ch(0xe8);
            int delta_printf = (int) (PRINTFF - ftell(prog) - 4);//Точно збс
            fprintf(prog, "%c%c%c%c", *(char *)&delta_printf, *((char *)&delta_printf + 1), *((char *)&delta_printf + 2), *((char *)&delta_printf + 3));
        )
    }
    return 0;

}

int To_Asm_Var(elem * element){
    f_asm(
        fprintf(prog, "\tmov r9, [rbp-8*%i]\n"
                      "\tpush r9\n", *(int *)(&(element->value)));
    )
    f_bin(
        f_ch(0x4c);
        f_ch(0x8b);
        f_ch(0x4d);
        //f_ch(0x24);
        fprintf(prog, "%c", (char)(-8*(*(int *)(&(element->value)))));

        f_ch(0x41);
        f_ch(0x51);
    )


    return 0;
}

int To_Asm_Number(elem * element){
    f_asm(fprintf(prog, "\tmov r9, %lgd\n\tpush r9\n", *(double *) element->value);)
    f_bin(
        f_ch(0x41);
        f_ch(0xb9);
        int value = (int) *(double *) element->value;
        write_int(prog, value);
        f_ch(0x41);
        f_ch(0x51);
    )

}

int To_Asm_Ret(elem * element){
    if (element->right->data_type == TYPE_NUMBER || element->right->data_type == TYPE_VAR) {
        To_Intel_Asm(element->right);
        f_asm(to_f("\tpop rax\n");)
        f_bin(f_ch(0x58);)
    }
    else {
        To_Intel_Asm(element->right);
    }

    f_asm(
        to_f("\n");
        fprintf(prog, "\tmov rsp, rbp\n\tpop rbp\n\tret\n");
    )
    f_bin(
        f_ch(0x48);
        f_ch(0x89);
        f_ch(0xec);
        f_ch(0x5d);
        f_ch(0xc3);
    )
}

int To_Asm_If(elem * element) {
    int label = cur_label++;
    Super_Switch((elem *) element->value);
    int super_value = *(int *) (&(((elem *)element->value)->value));
    To_Asm_Op((elem *) element->value);//condition

    switch (super_value) {
#define DEF_CMD(name1, num, code)                                                      \
        case num:{                                                                      \
            f_asm(fprintf(prog, "\t%s .lbl%i\n", #name1, label);)                        \
            f_bin(code;)                                                                  \
            f_bin(                                                                         \
                Label * super_lbl = new Label;                                              \
                char * label_name = (char *) calloc(sizeof(*label_name), 5);                 \
                sprintf(label_name, "%s%i", "lbl", label);                                    \
                label_name[4] = '\0';                                                          \
                                                                                                \
                super_lbl->name = label_name;                                                    \
                super_lbl->source = ftell(prog);                                                  \
                label_list.append(super_lbl);                                                      \
                f_ch(0x00);                                                                         \
                /*if (label_list.find(&super_lbl))*/                                                 \
                    /*f_ch((char) label_list.find(&super_lbl)->m_data->number + 1 - ftell(prog));*/   \
                                                                                                       \
            )                                                                                           \
            break;                                                                                       \
        }
        default:{
            printf("Da takogo ne mozhet bit\n");
            break;
        }

#include "supercmd.h"
#undef DEF_CMD
    }
    To_Intel_Asm(element->right);//code in if

    if (element->left->data_type == TYPE_ELS) {
        f_asm(fprintf(prog, "\tjmp .lble%i\n", label);)
        f_bin(
            char * label_name_else2 = (char *) calloc(sizeof(label_name_else2), 6);
            sprintf(label_name_else2, "%s", "lble");
            sprintf(label_name_else2 + 4, "%i", label);
            label_name_else2[5] = '\0';
            f_ch(0xeb);
            Label * super_lbl2 = new Label;
            super_lbl2->name = label_name_else2;
            super_lbl2->source = (int) ftell(prog);
            label_list.append(super_lbl2);
            f_ch(0x00);
        )
    }
    f_asm(fprintf(prog, ".lbl%i:\n", label);)
    f_bin(
        Label lbl;
        char * label_name2 = (char *) calloc(sizeof(label_name2), 5);
        sprintf(label_name2, "%s%i", "lbl", label);
        label_name2[4] = '\0';
        lbl.name = label_name2;
        if (!label_list.find(&lbl)) {
            printf("WTF??? Cannot find a proper label %s\n", label_name2);
            getchar();
            exit(ERROR_LOST_LABEL);
        }
        label_list.find(&lbl)->m_data->number = (size_t) ftell(prog);
    )
    if (element->left->data_type == TYPE_ELS){
        To_Intel_Asm(element->left->right);
        f_asm(fprintf(prog, ".lble%i:\n", label);)
        f_bin(
            Label lbl;
            char * label_name_else = (char *) calloc(sizeof(label_name_else), 6);
            sprintf(label_name_else, "%s", "lble");
            sprintf(label_name_else + 4, "%i", label);
            label_name_else[5] = '\0';
            lbl.name = label_name_else;
            if (!label_list.find(&lbl)) {
                printf("WTF??? Cannot find a proper label %s\n", label_name_else);
                getchar();
                exit(ERROR_LOST_LABEL);
            }
            label_list.find(&lbl)->m_data->number = (size_t) ftell(prog);
        )
        if (element->left->left == NULL) {
            f_asm(to_f("\tret\n");)
            f_bin(f_ch(0xc3);)
        }

    }
    return 0;
}

int To_Asm_Els(elem * element){
    return 0;
}

int To_Asm_Ass(elem * element){
    To_Intel_Asm(element->right);
    f_asm(fprintf(prog, "\tpop r9\n\tmov [rbp-8*%i], r9\n", element->value);)
    f_bin(
        f_ch(0x41);
        f_ch(0x59);

        f_ch(0x4c);
        f_ch(0x89);
        f_ch(0x4d);
        //f_ch(0x24);
        fprintf(prog, "%c", (char)(-8*(*(int*)(&(element->value)))));
    )
}

int To_Asm_In(elem * element){
    f_asm(
        fprintf(prog,
            "\tcall in\n"
            "\tmov [rbp-8*%i], rax\n", element->right->value);
    )
    f_bin(
        f_ch(0xe8);
        write_int(prog, (int)(IN_ASM - ftell(prog) - 4));
        f_ch(0x48);
        f_ch(0x89);
        f_ch(0x45);
        //f_ch(0x24);
        fprintf(prog, "%c", (char)(-8 * (*(int *)(&element->right->value))));
    )


}

int To_Asm_Func(elem * element){
    To_Intel_Asm(element->left);
    f_asm(to_f("\tpop rax\n");)
    f_bin(f_ch(0x58);)

    f_asm(
        fprintf(prog, "\tcall %s\n", (char *) element->value);
        to_f("\tpush rax\n");
    )
    f_bin(
        f_ch(0xe8);
        Label super_lbl;
        char * super_name = (char *) calloc((sizeof(char)), strlen((char *) element->value));
        strcpy(super_name, (char *) element->value);
        super_lbl.name = super_name;
        if (label_list.find(&super_lbl)) {
            int addr = (int) (label_list.find(&super_lbl)->m_data->number - ftell(prog) - 4);
            fprintf(prog, "%c%c%c%c", *(char *)&addr, *((char *)&addr + 1), *((char *)&addr + 2), *((char *)&addr + 3));
        }
        else {
            Label * super_lbl2 = new Label;
            char * super_name = (char *) calloc((sizeof(char)), strlen((char *) element->value) + 1);
            strcpy(super_name, (char *) element->value);
            super_lbl2->name = super_name;
            super_lbl2->source = -(int)ftell(prog);
            label_list.append(super_lbl2);
            f_ch(0xaa);
            f_ch(0xaa);
            f_ch(0xaa);
            f_ch(0xaa);

        }
        f_ch(0x50);
    )
    element->data_type = TYPE_REG;
    element->value = 0;
    Mini_Delete(element->left);
    element->left = NULL;
    return 0;
}

int To_Asm_New_Func(elem * element){
    cur_func = 1;
    f_asm(
            fprintf(prog, "%s:\n\tpush rbp \n"
            "\tmov rbp, rsp\n"
            "\tsub rsp, 80\n"
            "\tmov [rbp-8*%i], rax\n",
                          (char *) element->value,
                          *(int*)(&(element->right->value)));
    )
    f_bin(

        Label lbl;
        char * super_name = (char *) calloc((sizeof(char)), strlen((char *) element->value));
        strcpy(super_name, (char *) element->value);
        lbl.name = super_name;
        if (label_list.find(&lbl))
            label_list.find(&lbl)->m_data->number = ftell(prog);
        f_ch(0x55);
        f_ch(0x48);
        f_ch(0x89);
        f_ch(0xe5);

        f_ch(0x48);
        f_ch(0x83);
        f_ch(0xec);
        f_ch(0x50);

        f_ch(0x48);
        f_ch(0x89);
        f_ch(0x45);
        //f_ch(0x24);
        fprintf(prog, "%c", (char)(-8*(*(int*)(&(element->right->value)))));
    )
    return 0;

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
/*
int Razverni_Hex(int num){
    int * number = new int(num);
    if (*number < 0)
        *number = 1<<32 - *number;
    printf("num %i %i %i %i\n", (unsigned char) *number, (unsigned char) *(number + 1), (unsigned char) *(number + 2), (unsigned char) *(number + 3));
    char now = *(char *) number;
    printf("now %i\n", now);
    *(char *) number = *(char *) (number + 3);
    *(char *) (number + 3) = now;
    now = *(char *) (number + 1);
    printf("now %i\n", now);
    *(char *) number = *(char *) (number + 2);
    *(char *) (number + 2) = now;
    int res = *number;
    delete number;
    return res;
}*/