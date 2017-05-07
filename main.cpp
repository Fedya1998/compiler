#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <ctype.h>
#include <my_functions.h>
#include <clocale>
#include <iostream>


#define NDEBUG
#define CANARY 228
#define VAR_IN_FUNCTION 10
#include <assert.h>
#include <my_debug.h>



//G0 ::=Code
//E ::= T {['+''-'] T}*
//T ::= D {['*''/'D}*
//D ::= P{^P}*
//P ::= '('E')'|St
//N ::= ['0' - '9']+
//X ::= 'x'

//Var::= [A-Z|a-z][A-Z|a-z|_]*
//Ass ::= Var '=' E
//New ::= double Var
//Cond ::= if (E) Code;
//Code ::= (Cond|Ass|New|in-out)*

FILE * super_tree_dump = fopen("dump.gv", "w");
FILE * super_list_dump = fopen("list_dump.gv", "w");

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
#define DEF_CMD(name, num) CMD_##name = num,

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


tree_header * Tree_Ctor(void);
void Tree_Dtor(tree_header * header);
bool Elem_And_Next_OK(const elem *element);
bool Tree_OK(const tree_header *header);
bool Elem_OK(const elem * element);
void Elem_Dump(const elem *element);
void Elem_And_Next_Dump(const elem * elm);
void Tree_Dump(const tree_header *header);
elem * Add_First(tree_header * header);
elem * Add_Left(elem * element);
elem * Add_Right(elem * element);
void Mini_Delete(elem * element);
void Mini_Delete(elem * element);
int Delete_Tree(tree_header * header);
void Mini_File_Dump(const elem * element);
int Optimize_First(elem * element);
int Tree_Optimize(elem * element);
bool Wow_Its_Null(elem * element);
elem * Pridvin_Poblizhe(elem * element, int otkooda);

void Delete_Useless_Parentheses(elem * element);

elem * Copy_Elem(elem * element);

int _error = 0;

elem * GetN(elem * element);
elem * GetVar(elem * element);
elem * GetFunc(elem * element);
elem * GetE(elem * element);
elem * GetT(elem * element);
elem * GetP(elem * element);
elem * GetD(elem * element);
elem * GetEq(elem * element);


void Mini_Elem_Dump(list_elem * element);
list_header *List_Ctor(int number);
void List_Dtor(list_header *header);
bool list_elem_OK(list_elem *element);
void list_elem_Dump(list_elem *element);
void Mini_Elem_Dump(list_elem * element);
bool List_OK(list_header *header);
void List_Dump(list_header *header);
void Add_To_The_Beginning(list_header *header);
void Add_To_The_End(list_header *header);
list_elem * Add_After(list_elem *element);
void Add_Before(list_elem *element);
void Delete_list_elem(list_elem *element);
list_elem *Get_Pointer(list_header *header, int n);
list_header * To_List(char * program);
list_elem * Code_To_List(list_elem * element);

void Parse_Error();


int cur_label = 0;
variable * var_array[20];
int current_var = 0;
tree_header * func_array[20];
int cur_func = 0;

elem * Get_Code(elem * element);
list_elem * Make_Functions(list_elem * element);

tree_header * To_Tree(const list_header * l_header);
elem * Che_Delat_Esli_if(elem * element);
elem * Che_Delat_Esli_else(elem * element);
elem * Che_Delat_Esli_in(elem * element);
elem * Che_Delat_Esli_out(elem * element);
elem * Che_Delat_Esli_double(elem * element);
elem * Che_Delat_Esli_ass(elem * element);
elem * Che_Delat_Esli_return(elem * element);
elem * Che_Delat_Esli_end(elem * element);
elem * Che_Delat_Esli_lbr(elem * element);
elem * Che_Delat_Esli_rbr(elem * element);
int Get_Var_Num();
void Super_Optimize(elem * element);
int To_Asm_Out(elem * element);
int To_Asm_Op(elem * element);
int To_Asm_Var(elem * element);
int To_Asm_Number(elem * element);
int To_Asm_Ret(elem * element);
int To_Asm_Func(elem * element);
int To_Asm_If(elem * element);
int To_Asm_Els(elem * element);
int To_Asm_Reg(elem * element);
void Super_Switch(elem * element);

int current_message = 0;

int To_Intel_Asm(elem * element);


int Destruction = 0;


FILE * prog = fopen("proga.asm", "w");



#define to_f(data) fprintf(prog, data)

List <char> super_list = List<char>();

int main(){

    char source_name[] = "noproc_factorial.txt";

    char * program = (char *)File_To_Buf_u(fopen(source_name, "r"));

    To_The_Only_One_Line(program);

    list_header * l_header = To_List(program);//List of tokens

    D_MNOGO_DEFIS;


    List_Dump(l_header);



    tree_header * header = To_Tree(l_header);

    Tree_Optimize(header->first);


    D_MNOGO_DEFIS;

    Tree_Dump(header);




    cur_func = 0;
    source_name[0] = '_';
    source_name[1] = '_';


    List <Label> label_list = List<Label>();
    label_list.Compare = Compare_Label;
    to_f("%%include \"io.inc\"\n%%include \"printff.asm\"\nsection .text\n\tglobal  _start\n_start:\n");
    To_Intel_Asm(header->first);
    to_f("section .data\n");
    super_list.dump();
    super_list.file_dump(prog);
    fputs("msgvardec db \"%d\", 0\n", prog);
    fputs("msgstring db \"%s\", 0\n", prog);

    fclose(prog);

    Delete_Tree(header);
    List_Dtor(l_header);
    system("make");



    return 0;
}

#include "Tree_Functions.h"
#include "gets.h"
#include "list_functions.h"
#include "to_new_asm_functions.h"


//#include "to_asm_functions.h"


