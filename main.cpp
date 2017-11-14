#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <ctype.h>
#include <my_functions.h>
#include <clocale>
#include <iostream>
#include <printFe.h>


#define NDEBUG 1
#include <assert.h>
#include <my_debug.h>
#include <list.h>


//G0 ::=Code
//E ::= T {['+''-'] T}*
//T ::= D {['*''/'D}*
//D ::= P{^P}*
//P ::= '('E')'|St
//N ::= ['0' - '9']+
//X ::= 'x'

//Var ::= [A-Z|a-z][A-Z|a-z|_]*
//Ass ::= Var '=' E
//New ::= double Var
//Cond ::= if (E) Code;
//Code ::= (Cond|Ass|New|in-out)*

FILE * super_tree_dump = NULL;
FILE * super_list_dump = NULL;

typedef struct elem;
typedef struct tree_header;
typedef struct variable;
typedef struct super_struct;
typedef struct list_elem;

struct variable{
    char * id = NULL;
    elem * value = NULL;
};

struct super_struct{
    list_elem * list_element;
    elem * element;
};

struct elem{
    elem * left = NULL;
    elem * right = NULL;
    void * value = NULL;//It can be tree or string and so on
    int data_type = 0;// 1 - number, 2 - variable, 3 - operation, 4 - function, 5 - tree, 6 - string...
    elem * previous = NULL;
    tree_header * header = NULL;
};

enum operators{
#define DEF_CMD(name, num, code) CMD_##name = num,

#include "supercmd.h"
    _CMD_EMPTY
#undef DEF_CMD
};

#include "enum_types.h"

enum commands{
#define DEF_CMD(name, type, num, code)\
    CMD_##name = num,
#include "commands.h"
CMD_EMPTY
#undef DEF_CMD
};

const int CANARY = 228;


struct tree_header{
    elem * first = NULL;
};

struct list_elem{
    int can1 = CANARY;
    int data_type = 0;
    char * value = NULL;
    int count = 0;
    struct list_elem * next = NULL;
    struct list_elem * previous = NULL;
    struct list_header * header = NULL;
    int can2 = CANARY;
};



typedef struct list_header{
    list_elem * first = NULL;
    list_elem * last = NULL;
    int count = 0;
};


#include "tree_h.h"

#include "list_h.h"
#include "compiler_functions_h.h"

#include "to_asm_h.h"

int current_message = 0;

int To_Intel_Asm(elem * element);


int Destruction = 0;


FILE * prog = NULL;

enum translation_modes{
    ASM_MODE,
    BIN_MODE
};

const int SUPER_TRANSLATION_MODE = BIN_MODE;

#define f_asm(code) if (SUPER_TRANSLATION_MODE == ASM_MODE) {code}
#define f_bin(code) if (SUPER_TRANSLATION_MODE == BIN_MODE) {code}
#define f_ch(num) fprintf(prog, "%c", num)
#define to_f(data) fprintf(prog, data)

//  ╓---------------------------------------╖
//  |These must be taken from a.out.superlib|
        #define PRINTFF 0xf5
        #define IN_ASM 0xb0
        const short MSGSTRING = 2879;
        const short MSGDEC = 2876;
        short current_data = 2700;
        const int START_POINT = 900;
        const int END_POINT = 2850;
        const int END_LIB = 4700;
//  |                                       |
//  ╙---------------------------------------╜

List <Buf<char>> super_list;
List <Label> label_list;

using namespace std;

int main(){
    super_list.Compare = Compare_Char_Buf;
    label_list.Compare = Compare_Label;
    char * program = (char *) File_To_Buf_u(fopen("factorial.txt", "r"));

    To_The_Only_One_Line(program);

    list_header * l_header = To_List(program);          //List of tokens
    //List_Dump(l_header);
    tree_header * header = To_Tree(l_header);           //Program tree
    Tree_Optimize(header->first);                       //Ditch useless parentheses and other
    //Tree_Dump(header);

    cur_func = 0;
    FILE * lib = NULL;
    f_asm(prog = fopen("proga.asm", "w");)
    f_bin(
        lib = fopen("a.out.superlib", "r");
        prog = fopen("a.out", "w");
        assert(prog);
        char a[START_POINT] = {};
        fread(a, sizeof(char), START_POINT, lib);
        fwrite(a, sizeof(char), START_POINT, prog);
        fseek(lib, END_POINT, SEEK_SET);
    )

    f_asm(
        to_f("DEFAULT REL\n"
             "%%include \"printff.asm\"\n"
             "%%include \"in.asm\"\n"
             "section .text\n\t"
             "global _start\n"
             "_start:\n");
        To_Intel_Asm(header->first);
        to_f("section .data\n");
        super_list.dump();
        super_list.file_dump(prog);
        fputs("msgvardec db \'%d\', 0\n", prog);
        fputs("msgstring db \'%s\', 0\n", prog);
    )
    f_bin(
        elem * element = Copy_Elem(header->first);
        /*Includes and other things*/
        f_ch(0x55);
        f_ch(0x48);
        f_ch(0x89);
        f_ch(0xe5);

        f_ch(0x48);
        f_ch(0x83);
        f_ch(0xec);
        f_ch(0x50);
        /*Now the program starts*/
        To_Intel_Asm(header->first);
        f_ch(0x48);
        f_ch(0x89);
        f_ch(0xec);
        f_ch(0x5d);
        char a[END_LIB - END_POINT];
        fread(a, sizeof(char), END_LIB - END_POINT, lib);
        fseek(prog, END_POINT, SEEK_SET);
        fwrite(a, sizeof(char), END_LIB - END_POINT, prog);
        fseek(prog, START_POINT, SEEK_SET);
        label_list.label_dump(prog);
    )

    fclose(prog);

    Delete_Tree(header);
    List_Dtor(l_header);
    f_asm(
        system("make");
        system("./a.out");
    )

    Destruction = 1;

    return 0;
}

#include "Tree_Functions.h"
#include "gets.h"
#include "to_new_asm_functions.h"



#undef f_asm
#undef f_bin
#undef to_f
#undef PRINTFF