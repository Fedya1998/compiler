char * s;
list_elem * list_elem_now = NULL;
tree_header * Tree_Ctor(void) {
    tree_header *header1 = new tree_header();
    header1->first = NULL;
    return header1;
}

elem * Add_First(tree_header * header){
    assert(header);
    header->first = new elem();
    header->first->header = header;
    return header->first;
}

void Tree_Dtor(tree_header *header) {
    Delete_Tree(header);
    delete header;
}

bool Elem_OK(const elem * element){
    if (!element) return false;
    /*if (element->data_type == TYPE_OP && (!element->left || !element->right))
        return false;*/
    return (bool) element->header;
}

bool Elem_And_Next_OK(const elem *element) {
    bool OK = Elem_OK(element);
    if (element->right) OK = OK && Elem_And_Next_OK(element->right);
    if (element->left) OK = OK && Elem_And_Next_OK(element->left);
    return OK;
}

bool Tree_OK(const tree_header *header) {
    const elem *element = header->first;
    if (element == NULL) {
        p(12, ("Empty list\n"));
        return true;
    }//If our list is empty
    return Elem_And_Next_OK(element);
}



void Elem_Dump(const elem *element) {
    printf("\nelement %i\n", element);
    if (!element)
        return;
    printf("type_value %i\nvalue %lg\n", element->data_type, element->value);
    d(10,{
        printf("pointer %i\n", element);
        printf("header %i\n", element->header);
        printf("right %i\n", element->right);
        printf("left %i\n", element->left);
        if (element->data_type == TYPE_NEW_FUNC)
            printf("value %s\n", (char *)element->value);
    });
}

void Mini_File_Dump(const elem * element){
    if (!element){
        fprintf(super_tree_dump, "%celement %p%c", 34, element, 34);
        return;
    }
    fprintf(super_tree_dump, "%celement %p\nvalue %p\nleft %p\nright %p", 34,
            element, element->value,
            element->left, element->right);
    if (element->data_type == TYPE_NUMBER)
        fprintf(super_tree_dump, "\nmy double value %lg", *(double *) element->value);
    switch (element->data_type){
#define DEF_TYPE(name, num)\
        case num: {fprintf(super_tree_dump, "\n%s", #name); break;}

#include "types.h"

        default: fprintf(super_tree_dump, "\nundefined type");
#undef DEF_TYPE
    }
    /*switch (element->data_type){
        case TYPE_STRING: {
            fprintf(super_tree_dump, "value %s", element->value);
            break;
        }
        case TYPE_NUMBER: {
            fprintf(super_tree_dump, "value %lg", element->value);
            break;
        }
        case TYPE_VAR: {
            fprintf(super_tree_dump, "value %s", element->value);
            break;
        }
        case TYPE_FUNCTION: {
            fprintf(super_tree_dump, "value %i", element->value);
            break;
        }
        case TYPE_POINTER: {
            fprintf(super_tree_dump, "value %i", element->value);
            break;
        }
        case TYPE_OP: {
            fprintf(super_tree_dump, "value %i", element->value);
        }
    }*/
    if (element->data_type == TYPE_STRING || element->data_type == TYPE_NEW_FUNC || element->data_type == TYPE_FUNC)
        fprintf(super_tree_dump, "\nvalue string %s\n", (char *) element->value);

    fprintf(super_tree_dump, "%c", 34);
}

void Elem_And_Next_Dump(const elem * element){
    if (!element) return;

    if (element->left) {
        Mini_File_Dump(element);
        fprintf(super_tree_dump, "->");
        Mini_File_Dump(element->left);
    }
    if (element->left) Elem_And_Next_Dump(element->left);

    if (element->data_type == TYPE_IF || element->data_type == TYPE_OP){
        if (element->value > (void *) 25) {
            Mini_File_Dump(element);
            fprintf(super_tree_dump, "->");
            Mini_File_Dump((elem *) element->value);
            Elem_And_Next_Dump((elem *) (element->value));
        }
    }

    if(element->right) {
        Mini_File_Dump(element);
        fprintf(super_tree_dump, "->");
        Mini_File_Dump(element->right);
    }
    if (element->right) Elem_And_Next_Dump(element->right);
}

void Tree_Dump(const tree_header *header) {
    super_tree_dump = fopen("tree_dump.gv", "w");
    fprintf(super_tree_dump, "digraph G{\n");
    if (!header) return;
    if (!header->first) return;
    Elem_And_Next_Dump(header->first);
    fprintf(super_tree_dump, "}");
    fclose(super_tree_dump);
    system("cd ~/code/ded/compiler");
    system("dot -Tpng tree_dump.gv -o dump.png");
}





elem * Add_Left(elem * This) {
    SUPER_NOT_OK_DUMP(Elem);
    //Gde;
    This->left = new elem();
    //Gde;
    This->left->header = This->header;
    This->left->previous = This;
    SUPER_NOT_OK_DUMP(Elem);
    return This->left;
}

elem * Add_Right(elem * This) {
    SUPER_NOT_OK_DUMP(Elem);
    //Gde;
    This->right = new elem();
    //Gde;
    This->right->header = This->header;
    This->right->previous = This;
    SUPER_NOT_OK_DUMP(Elem);
    return This->right;
}

elem * Add_Value(elem * This){
    SUPER_NOT_OK_DUMP(Elem);
    //Gde;
    This->value = new elem();
    //Gde;
    ((elem *) (This->value))->header = This->header;
    ((elem *) (This->value))->previous = This;
    SUPER_NOT_OK_DUMP(Elem);
    return (elem *) This->value;
}

int Delete_Tree(tree_header * header){
    Destruction = 1;
    Mini_Delete(header->first);
    delete header;
    return errno;
}

void Super_Mini_Delete(elem * element){
    if (element->data_type == TYPE_NUMBER) {
        delete element->value;
    }
    if (Destruction) {
        if (element->data_type == TYPE_FUNC)
            free(element->value);
        if (element->data_type == TYPE_STRING)
            free(element->value);
    }
    if (element->data_type == TYPE_IF)
        Mini_Delete((elem *) element->value);

    delete element;
    //printf("deleting\n");
}

void Mini_Delete(elem * element){
    if (element->right) Mini_Delete(element->right);
    if (element->left) Mini_Delete(element->left);
    Super_Mini_Delete(element);
}




void Super_Optimize(elem * element){
    if (!element)
        return;

    if (element->right && element->right->data_type == 0 && element->right->right == 0){
        //elem * to_the_left = element->right->left;
        elem * to_the_right = element->right->left;
        Super_Mini_Delete(element->right);
        element->right = to_the_right;
        //element->left = to_the_left;
    }
    if (element->left && element->left->data_type == 0 && element->left->right == 0){
        elem * to_the_left = element->left->left;
        //elem * to_the_right = element->left->right;
        Super_Mini_Delete(element->left);
        //element->right = to_the_right;
        element->left = to_the_left;
    }
    if (element->right)
        Super_Optimize(element->right);
    if (element->left)
        Super_Optimize(element->left);
    if (element->value && element->data_type == TYPE_IF)
        Super_Optimize((elem *) element->value);
}

int Optimize_First(elem * element){
    if (!element) return 0;
    if (!element->value && !element->data_type && element == element->header->first && element->left) {
        elem * elm = element->left;
        element->header->first = elm;
        Super_Mini_Delete(element);
        element = elm->header->first;
        element->previous = NULL;
    }
    if (!element->value && !element->data_type && element == element->header->first) Optimize_First(element);

    return errno;
}

int Tree_Optimize(elem * element){
    Super_Optimize(element);
    Super_Optimize(element);
    Super_Optimize(element);
    Delete_Useless_Parentheses(element);

    /*
    if (!element) return ERROR_BAD_TREE;
    if (element->left  && !element->left->data_type && element->left->left) {
        elem * elm = element->left->left;
        Super_Mini_Delete(element->left);
        element->left = elm;
        element->left->previous = element;
    }

    if (element->right  && !element->right->data_type && element->right->left) {
        elem * elm = element->right->left;
        Super_Mini_Delete(element->right);
        element->right = elm;
        element->right->previous = element;
    }

    if (element->left) Tree_Optimize(element->left);
    if (element->right) Tree_Optimize(element->right);
     */
    return 0;

}
/*
int Optimize_Before_Diff(elem * element){
    tree_header * header = element->header;
    Optimize_First(header->first);
    Tree_Optimize(header->first);
    Tree_Optimize(header->first);
    Delete_Useless_Parentheses(header->first);
    return _error;
}
*/
elem * Copy_Elem(elem * element){
    if (!element) return NULL;
    elem * new_elem = new elem();
    new_elem->value = element->value;
    new_elem->data_type = element->data_type;
    new_elem->header = element->header;
    new_elem->left = Copy_Elem(element->left);
    if (new_elem->left) new_elem->left->previous = new_elem;
    new_elem->right = Copy_Elem(element->right);
    if (new_elem->right) new_elem->right->previous = new_elem;
    return new_elem;
}
/*
elem * Optimize_Mul(elem * element){
    if (!Elem_OK) return NULL;
    if (element->data_type == TYPE_OP && element->value == CMD_MUL){

        if (element->right && Wow_Its_Null(element->right)) {
            Mini_Delete(element->left);
            element->left = NULL;
            element->right = NULL;
            element->data_type = TYPE_NUMBER;
            element->value = 0;
        } else if (element->left && Wow_Its_Null(element->left)) {
            Mini_Delete(element->right);
            element->right = NULL;
            element->left = NULL;
            element->data_type = TYPE_NUMBER;
            element->value = 0;
        }

        if (element->left) {
            if (element->left->data_type == TYPE_NUMBER && element->left->value == 1)
                Pridvin_Poblizhe(element->left, 1);
        }

        if (element->right) {
            if (element->right->data_type == TYPE_NUMBER && element->right->value == 1)
                Pridvin_Poblizhe(element->right, 2);
        }


    }


    if (element->left) Optimize_Mul(element->left);
    if (element->right) Optimize_Mul(element->right);
    return element;
}

elem * Optimize_Add(elem * element){
    if (!Elem_OK(element)) {
        return NULL;
    }
    if (element->data_type == TYPE_OP && (element->value == CMD_SUB || element->value == CMD_ADD)){
        if (element->left->data_type == TYPE_NUMBER && element->left->value == 0){
            Pridvin_Poblizhe(element->left, 1);
        }
        if (element->right->data_type == TYPE_NUMBER && element->right->value == 0)
            Pridvin_Poblizhe(element->right, 2);
    }
    if (element->left) Optimize_Add(element->left);
    if (element->right) Optimize_Add(element->right);
    return element;
}
*/
bool Wow_Its_Null(elem * element){
    if (element->data_type == TYPE_NUMBER && element->value == 0){
        Mini_Delete(element);
        return true;
    }
    return false;
}

elem * Pridvin_Poblizhe(elem * element, int otkooda){//1 - left, 2 - right
    if (!Elem_OK(element)) return NULL;
    if (otkooda == 1) {//In the left branch there is 1
        elem * to_the_right = element->previous->right->right;
        elem * to_the_left = element->previous->right->left;
        element->previous->value = element->previous->right->value;
        element->previous->data_type = element->previous->right->data_type;

        if (to_the_right) to_the_right->previous = element->previous;
        if (to_the_left) to_the_left->previous = element->previous;
        if (element->previous->right)
            Super_Mini_Delete(element->previous->right);
        
        element->previous->right = to_the_right;
        element->previous->left = to_the_left;
        
        Super_Mini_Delete(element);
    }

    if (otkooda == 2) {//Now we won't give a damn about right elem, we'll just take left one higher
        elem * to_the_left = element->previous->left->left;
        elem * to_the_right = element->previous->left->right;
        element->previous->value = element->previous->left->value;
        element->previous->data_type = element->previous->left->data_type;

        if (to_the_right) to_the_right->previous = element->previous;
        if (to_the_left) to_the_left->previous = element->previous;
        if (element->previous->left)
            Super_Mini_Delete(element->previous->left);


        element->previous->left = to_the_left;
        element->previous->right = to_the_right;

        Super_Mini_Delete(element);
    }


    return element;
}

/*
int Pokazhee_Pliz(elem * element){
    if (!Elem_OK(element)) return 1;
    if (element->data_type == TYPE_OP && element->value != CMD_LBR && element->value != CMD_RBR) printf("(");

    if (element->left && element->data_type != TYPE_FUNCTION) {
        Pokazhee_Pliz(element->left);
    }

    switch (element->data_type){
        case TYPE_NUMBER: {
            printf("%lg", element->value);
            break;
        }
        case TYPE_X: {
            printf("x");
            break;
        }
        case TYPE_OP:{
            Che_Tam_Op(element);
            break;
        }
        case TYPE_FUNCTION:{
            switch ((int) element->value) {
                #define FUNC(name, num)\
                case num: {\
                printf(#name);\
                break;\
                }

                #include "super_functions.h"
                default: {
                    printf("_Undefined function_");
                    _error = ERROR_UNDEFINED_FUNCTION;
                }
                #undef FUNC
            }
            break;
        }
        default:{
            printf("Undefined type");
            _error = ERROR_UNDEFINED_TYPE;
        }
    }

    if (element->left && element->data_type == TYPE_FUNCTION) {
        printf("(");
        Pokazhee_Pliz(element->left);
    }
    if (element->right)Pokazhee_Pliz(element->right);
    if (element->data_type == TYPE_FUNCTION)
        printf(")");
    if (element->data_type == TYPE_OP && element->value != CMD_LBR && element->value != CMD_RBR) printf(")");

    return 0;
}

int Che_Tam_Op(elem * element){
    if (!element)
        Elem_Dump(element);

    switch ((int) element->value) {
        case CMD_ADD: {
            printf(" + ");
            break;
        }
        case CMD_SUB: {
            printf(" - ");
            break;
        }
        case CMD_MUL: {
            printf(" * ");
            break;
        }
        case CMD_DIV: {
            printf(" / ");
            break;
        }
        case CMD_LBR: {
            if (element->left->left == NULL) break;
            printf("(");
            break;
        }
        case CMD_RBR:{
            if (element->previous->left->left == NULL) break;
            printf(")");
            break;
        }
        case CMD_POW: {
            printf("^");
            break;
        }
        default: {
            printf("Undefined value\n");
            Elem_Dump(element);
            _error = ERROR_UNDEFINED_VALUE;
            return 1;
        }
    }
    return 0;
}
*/
void Delete_Useless_Parentheses(elem * element){
    elem * This = element;
    SUPER_NOT_OK_DUMP(Elem);
    if (element->data_type == TYPE_OP && element->value == (void *)CMD_LBR){
        //printf("deleting\n");
        Pridvin_Poblizhe(element->right, 2);

    }
    if (element->data_type == TYPE_IF)
        Delete_Useless_Parentheses((elem *) element->value);
    if (element->left)
        Delete_Useless_Parentheses(element->left);
    if (element->right)
        Delete_Useless_Parentheses(element->right);
}


tree_header * To_Tree(const list_header * l_header){
    assert(l_header);
    s = l_header->first->value;
    tree_header * header = Tree_Ctor();
    elem * element = Add_First(header);
    list_elem * list_element = l_header->first;
    /*super_struct super_struct1;
    super_struct1.list_element = list_element;
    super_struct1.element = element;*/
    list_elem_now = list_element;
    Get_Code(element);
    return header;

}

elem * Get_Code(elem * element) {
    if (!list_elem_now->value)
        return element;
    p(9, ("Prishlo to get code %i %s\n", list_elem_now->value, list_elem_now->value));
    s = list_elem_now->value;
    Skip_Different_Shit(s);

    if (!list_elem_now->next) {//This is the end
        element = Che_Delat_Esli_end(element);
        return element;
    }
    if (list_elem_now->next->value == NULL)
        return NULL;

    element->data_type = list_elem_now->data_type;
    char *word_end = The_End_Of_The_Word(list_elem_now->value);
    unsigned char the_word_end = *word_end;
    s = word_end + 1;

    *word_end = '\0';
    p(12, ("My command %s\n", list_elem_now->value));
#define DEF_CMD(name, type, num, code)\
                if (strcmp(#name, (char*) list_elem_now->value) == 0){\
                    *word_end = the_word_end;\
                    Che_Delat_Esli_##name(element);\
                } else

#include "commands.h"
#undef DEF_CMD
    {
        *word_end = the_word_end;
        s = list_elem_now->value;
        p(15, ("Undefined command or assignment\n"));
        char *pointer_ravno = strchr((char *) s, '=');
        char *pointer_comm = strchr((char *) s, ';');
        if (pointer_ravno && pointer_ravno < pointer_comm) {//Assignment
            elem *element2 = Che_Delat_Esli_ass(element);
            if (!element2)
                return NULL;
        } else {
#include "Function_in_get_code.h"
        }
    }

    if (!list_elem_now)
        return element;

    /*if (*s == '}')
        return element;*/

    if (list_elem_now->data_type == TYPE_RBR) {
        //list_elem_now = list_elem_now->next;
        p(11, ("Return from get code %s\n", list_elem_now->value));
        list_elem_now = list_elem_now->next;
        return NULL;
    }

    if (*s == '\0') {//The end
        return NULL;
    }

    element = Add_Left(element);
    Gde;
    element = Get_Code(element);


    return element;

}

elem * Che_Delat_Esli_if(elem * element){//Засунуть всё говно в right, условие в value. Парсить value до (, говно от { до }
    p(10, ("Prishlo v che delat if %s", s));
    s = (char *) strchr((char *) s, '(');
    Add_Value(element);
    ((elem *) (element->value))->data_type = TYPE_OP;
    Add_Left((elem *) element->value);
    GetEq((elem *) element->value);
    p(9, ("After GetEq in if %s\n", s));
    s++;
    if (*s != ' ') {
        //printf("\n DAMN IT!!11 A SPACE MUST BE HERE, I'LL TRY S++ %s\n", s);
        Gde;
        getchar();
        s++;
    }
    Gde;
    s = Skip_Different_Shit(s);
    if (*s != '{') {
        //printf("\n DAMN IT!!11 A { MUST BE HERE %s\n", s);
        Gde;
        getchar();
    }
    Gde;
    Add_Right(element);

    list_elem_now = list_elem_now->next;
    if (list_elem_now->data_type == TYPE_LBR)
        list_elem_now = list_elem_now->next;

    Get_Code(element->right);


    if (list_elem_now)
        p(12, ("I'm in if, after get code %s", list_elem_now->value));
    return element;
}

elem *Che_Delat_Esli_else(elem * element){
    p(10, ("Prishlo in else %s\n", list_elem_now->value));
    s = Skip_Different_Shit(s);
    list_elem_now = list_elem_now->next;

    Add_Right(element);
    list_elem_now = list_elem_now->next;

    Get_Code(element->right);
    return element;
}

elem *Che_Delat_Esli_in(elem * element){ ;
    p(10, ("Prishlo v che delat in %s\n", s));
    char * word_end = (char *) strchr((char *) s, ')');
    char the_word_end = *word_end;
    *word_end = '\0';

    int my_var = Get_Var_Num();
    *word_end = the_word_end;
    s = word_end + 1;
    element->data_type = TYPE_IN;
    Add_Right(element);
    element->right->data_type = TYPE_VAR;
    element->right->value = (elem **) my_var;
    s = (char *) strchr((char *) s, ';');
    list_elem_now = list_elem_now->next;
    return element;
}


elem *Che_Delat_Esli_out(elem * element){
    s = list_elem_now->value;


    s = strchr(s, '(');
    assert(s);
    char * word_end = strchr(s, ')');
    char * kavich = strchr(s, 34);
    *word_end = '\0';

    if (kavich && kavich < word_end){
        //printf("Out string\n");
        word_end = (char *)strchr((kavich + 1), 34);
        *word_end = '\0';
        assert(word_end);
        //printf("string %s\n", kavich);
        Buf <char> * str = new Buf <char> (word_end - kavich);
        str->m_data = (char *) calloc(sizeof(char), word_end - kavich);
        strcpy(str->m_data, kavich + 1);
        //printf("My string %s\n", str);
        s = word_end + 2;
        Add_Right(element);
        element->right->data_type = TYPE_STRING;
        char * string = (char *) calloc(sizeof(*string), 5);
        strcpy(string, "msg");
        sprintf(string+3, "%i", current_message++);
        string[4] = '\0';

        super_list.append(str);

        element->right->value = string;
    } else {
        //printf("Out var\n");

        s = list_elem_now->value;
        s = strchr(s, '(') + 1;
        int my_var = Get_Var_Num();
        element->data_type = TYPE_OUT;
        Add_Right(element);
        element->right->data_type = TYPE_VAR;
        element->right->value = (void *) my_var;
    }
    list_elem_now = list_elem_now->next;
    return element;
}

elem *Che_Delat_Esli_double(elem * element){
    s = list_elem_now->value;
    p(10, ("Prishlo in che delat double %s\n", s));
    s = (char *) strchr((char *) s, ' ');
    s++;
    //printf("My new var %s\n", s);


    char * word_end = (char *) strchr((char *) s, ';');
    *word_end = '\0';
    char * str = (char *) calloc(word_end - s + 1, sizeof(char));
    strcpy(str, (char *) s);
    str[word_end - s] = '\0';
    var_array[current_var] = new variable();
    var_array[current_var]->id = str;
    current_var++;
    list_elem_now = list_elem_now->next;
    return element;
}

elem *Che_Delat_Esli_ass(elem * element){
    s = list_elem_now->value;
    p(10, ("Prishlo in ass %s\n", s));
    element->data_type = TYPE_ASS;
    char * word_end = (char *) strchr((char *) s, '=');
    *word_end = '\0';


    int my_var_num = Get_Var_Num();

    element->value = (void *) my_var_num;
    Add_Right(element);

    s = word_end + 1;
    GetEq(element->right);
    list_elem_now = list_elem_now->next;
    return element;
}

elem *Che_Delat_Esli_return(elem * element){
    s = list_elem_now->value;
    p(10, ("Prishlo in esli return %s\n", s));
    s = (char *) strchr((char *) s, ' ');
    assert(s);
    s++;

    element->data_type = TYPE_RET;
    Add_Right(element);
    GetEq(element->right);
    if (*s != ';') {
        printf("\n DAMN IT!!11 A ; MUST BE HERE %s\n", s);
        getchar();
    }
    list_elem_now = list_elem_now->next;
    p(13, ("s after return %s\n", s));

    return element;

}

elem *Che_Delat_Esli_end(elem * element){
    assert(element);
    p(11, ("In end: %s", list_elem_now->value));
    element->data_type = TYPE_END;
    element->value = NULL;
    list_elem_now = list_elem_now->next;
    return NULL;
}

elem *Che_Delat_Esli_lbr(elem * element){
    list_elem_now = list_elem_now->next;
}

elem *Che_Delat_Esli_rbr(elem * element){
    list_elem_now = list_elem_now->next;
}

int Get_Var_Num(){
    for (int i = 1; i < current_var; i++){
        if (strcmp((char *) s, var_array[i]->id) == 0)
            return i;
    }
    return -1;
}