
list_header * List_Ctor(int number){
    list_header * header1 = new list_header();
    if (number) header1->first = new list_elem();
    else return header1;
    list_elem * list_element = header1->first;
    list_element->header = header1;
    for (int i = 1; i < number - 1; i++){
        list_element->next = new list_elem();
        list_element->next->previous = list_element;
        list_element = list_element->next;
        list_element->header = header1;
    }
    list_element->next = new list_elem();
    list_element->next->previous = list_element;
    list_element->next->next = NULL;
    list_element->next->header = header1;

    header1->last = list_element->next;
    header1->count = number;
    return header1;
}

void List_Dtor(list_header * header1){
    list_elem * list_element = header1->first;
    while (1){
        if (!list_element) break;
        list_elem * list_element1 = list_element->next;
        /*if (list_element->value)
            free(list_element->value);*/
        delete list_element;
        list_element = list_element1;
    }
    delete header1;
}

bool list_elem_OK(list_elem * list_element){
    if (list_element->can1 != 228) return false;
    if (list_element->can2 != 228) return false;
    return list_element && list_element->header;
}

bool List_OK(list_header * header){
    if (header->count < 0) {Gde; return false;}//Bad count
    list_elem * list_element = header->first;
    if (list_element == NULL && header->last == NULL) {d(10, printf("Empty list\n")); return true;}//If our list is empty
    if (header->count == 1) return list_elem_OK(list_element);//If there is only one list_element
    list_element = list_element->next;//If there are more than one list_elements
    for (int i = 1; i < header->count - 1; i++){
        if (!list_elem_OK(list_element)) {Gde; return false;}
        if (list_element->next == NULL || list_element->previous == NULL) {Gde; printf("i %i\n", i); return false;}
        list_element = list_element->next;
    }
    if (!list_elem_OK(list_element)) {Gde; d(9, printf("list_element not ok. list_element %i\n", list_element)); return false;}
    if (list_element->next || list_element->previous == NULL) {Gde; return false;}
    return true;
}

void list_elem_Dump(list_elem * list_element){
    printf("value %s\n", list_element->value);
    printf("count %i\n", list_element->count);

    printf("pointer %i\n", list_element);
    printf("header %i\n", list_element->header);
    printf("next %i\n", list_element->next);
    printf("previous %i\n", list_element->previous);

}

void Header_Dump(list_header * header){
    printf("Header %i\n", header);
    if (header) {
        printf("First %i\n", header->first);
        printf("Last %i\n", header->last);
        printf("Count %i\n", header->count);
    }
}

void List_Dump(list_header * header){
    assert(header);
    super_list_dump = fopen("list_dump.gv", "w");
    fprintf(super_list_dump, "digraph G{\n");
    list_elem * element = header->first;
    while(1){
        if (!element)
            break;

        Mini_Elem_Dump(element);
        fprintf(super_list_dump, "->");
        Mini_Elem_Dump(element->next);
        element = element->next;
    }
    fprintf(super_list_dump, "}");
    fclose(super_list_dump);
    system("cd ~/code/ded/compiler");
    system("dot -Tpng list_dump.gv -o list_dump.png");
}

void Mini_Elem_Dump(list_elem * element){
    if (!element){
        p(10, ("Element %i", 0));
        fprintf(super_list_dump, "nul");
        return;
    }

    fprintf(super_list_dump, "%celement %p\ndata_type %i\nheader %p\nnext %p\n", 34,
            element, element->data_type,
            element->header, element->next);
    /*if (element->data_type == TYPE_TREE)
        Mini_File_Dump((elem *) element->value);

    if (element->value) {
        switch (element->data_type) {
            case TYPE_STRING: {
                fprintf(super_list_dump, "value %s", element->value);
                break;
            }
            case TYPE_NUMBER: {
                fprintf(super_list_dump, "value %lg", *(double *) element->value);
                break;
            }
            case TYPE_VAR: {
                fprintf(super_list_dump, "value %s", element->value);
                break;
            }
            case TYPE_FUNCTION: {
                fprintf(super_list_dump, "value %i", *(int *) element->value);
                break;
            }
            case TYPE_POINTER: {
                fprintf(super_list_dump, "value %i", *(int *) element->value);
                break;
            }
            case TYPE_OP: {
                fprintf(super_list_dump, "value %i", *(int *) element->value);
            }
        }
    } else
        fprintf(super_list_dump, "*null");
        */
    fprintf(super_list_dump, "value %.*s\n", 2, element->value);
    
    fprintf(super_list_dump, "%c", 34);
}

void Add_To_The_Beginning(list_header * This){
    SUPER_NOT_OK_DUMP(List);
    list_elem * lmn1 = This->first;
    This->first = new list_elem();
    This->first->next = lmn1;
    This->first->header = This;
    This->count++;
    SUPER_NOT_OK_DUMP(List);
}

void Add_To_The_End(list_header * This){

    SUPER_NOT_OK_DUMP(List);

    if (This->last) {
        This->last->next = new list_elem;
        This->last->next->previous = This->last;
        This->last = This->last->next;
        This->last->header = This;
    } else {
        d(10, printf("First list_element\n"));
        Err;
        This->first = new list_elem;
        Err;
        This->first->header = This;
        This->last = This->first;
    }
    This->count++;
    SUPER_NOT_OK_DUMP(List);

}

list_elem * Add_After(list_elem * list_element){
    assert(list_element);
    list_elem * lmn1 = list_element->next;
    list_element->next = new list_elem();
    list_element->next->header = list_element->header;
    if (lmn1)
        lmn1->previous = list_element->next;
    list_element->header->count++;
    return list_element->next;
}

void Add_Before(list_elem * list_element){
    list_header * This = list_element->header;
    SUPER_NOT_OK_DUMP(List);
    list_elem * lmn1 = list_element->previous;
    list_element->previous = new list_elem();
    list_element->previous->header = This;
    lmn1->next = list_element->next;
    This->count++;
    SUPER_NOT_OK_DUMP(List);
}

void Delete_list_elem(list_elem * list_element){
    list_header * This = list_element->header;
    SUPER_NOT_OK_DUMP(List);
    list_element->previous->next = list_element->next;
    list_element->next->previous = list_element->previous;
    delete(list_element);
    SUPER_NOT_OK_DUMP(List);
}

list_elem * Get_Pointer(list_header * This, int n){
    SUPER_NOT_OK_DUMP(List);
    list_elem * list_element = NULL;
    if (n > This->count) return list_element;
    if (This->count - n > n) goto first;
    else goto second;
    first:
    list_element = This->first;
    for (int i = 0; i < n; i++) {
        list_element = list_element->next;
    }
    SUPER_NOT_OK_DUMP(List);
    return list_element;

    second:
    list_element = This->last;
    try {
        for (int i = This->count - 1; i > n; i++) {
            list_element = list_element->previous;
            if (errno) throw errno;
        }
        throw errno;
    }
    catch (int i){
        if (i) printf("Bleen, slomalos';(((");
    }
    SUPER_NOT_OK_DUMP(List);
    return list_element;
}

list_header * To_List(char * program) {
    s = program;
    list_header *header = List_Ctor(1);
    list_elem *element = header->first;
    element = Code_To_List(header->first);

    Make_Functions(element);
    return header;
}
/*
list_elem * Get_Code(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    if (*s == '}') {
        element->data_type = TYPE_OP;
        int i = CMD_rfbr;
        element->value = &i;
        return element;
    }
    
    if (Get_Condition(element))
        return element;
    else if (Get_Other(element))
        return element;
    else if (Get_New(element))
        return element;
    else Get_Assignment(element);

    Add_After(element);
    element = element->next;
    Get_Code(element);
}

list_elem * Get_Condition(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    char str[5]="";//"if (" - 4 chars
    sscanf(s, "%4s", str);
    str[4] = '\0';
    if (strcmp("if (", str))
        return NULL;
    s += 3;
    int * i1 = new int;
    *i1 = CMD_if;
    element->value = i1;
    element->data_type = TYPE_OP;
    GetE((elem *) element->value);
    s++;//space
    if (*s != '{') {
        printf("Govno! It must be { here %s\n", s);
        return NULL;
    }
    Add_After(element);
    element = element->next;
    element->data_type = TYPE_OP;
    int * i2 = new int;
    *i2 = CMD_lfbr;
    element->value = i2;
    s++;
    Add_After(element);
    element = element->next;
    Get_Code(element);

}

list_elem * Get_Assignment(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    element->data_type = TYPE_VAR;
    element->value = GetVar((elem *)element->value);
    if (*s != '='){
        printf("Govno! '=' must be here %s", s);
    }
    s++;
    ((elem *) (element->value))->value = GetE((elem *) (((elem *)(element->value))->value));
    return element;
}

list_elem * Get_New(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    if (strcmp("int", s) == 0)
        Add_Int(element);
    else if (strcmp("double", s) == 0)
        Add_Double(element);
    else if (strcmp("string", s) == 0)
        Add_String(element);
    else return NULL;
}

variable * Add_Int(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    var_array[current_var]->type = type_int;
    char * pointer_space = strchr(s, ' ');
    char * id_end = strchr(s, ';');
    assert(pointer_space);
    assert(id_end);
    var_array[current_var]->id = (char *) calloc(id_end - pointer_space + 1, sizeof(char));
    *id_end = '\0';
    strcpy(var_array[current_var]->id, pointer_space + 1);
    *id_end = ';';
    s = id_end + 1;
    current_var++;
}

variable * Add_Double(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    var_array[current_var]->type = type_double;
    char * pointer_space = strchr(s, ' ');
    char * id_end = strchr(s, ';');
    assert(pointer_space);
    assert(id_end);
    var_array[current_var]->id = (char *) calloc(id_end - pointer_space + 1, sizeof(char));
    *id_end = '\0';
    strcpy(var_array[current_var]->id, pointer_space + 1);
    *id_end = ';';
    s = id_end + 1;
    current_var++;
}

variable * Add_String(list_elem * element){
    Gde;
    p(10, ("Prishlo %s\n", s));
    var_array[current_var]->type = type_string;
    char * pointer_space = strchr(s, ' ');
    char * id_end = strchr(s, '=');
    assert(pointer_space);
    assert(id_end);
    var_array[current_var]->id = (char *) calloc(id_end - pointer_space + 1, sizeof(char));
    *id_end = '\0';
    strcpy(var_array[current_var]->id, pointer_space + 1);
    *id_end = '=';
    s = id_end + 1;
    if (*s != 34){//34 = "
        printf("Govno! Kavichka must be here %s\n", s);
        return NULL;
    }
    s++;
    char * string_end = strchr(s + 1, 34);
    *string_end = '\0';
    if (string_end != (s + 1)) {
        assert(string_end);
        var_array[current_var]->value = (char *) calloc(string_end - s + 1, sizeof(char));
        *string_end = '\0';
        strcpy((char *) var_array[current_var]->value, s + 1);
    }
    *string_end = 34;
    s = string_end;
    current_var++;
    s++;
    if (*s != ';'){
        printf("Govno! ';' must be here %s", s);
        return NULL;
    }
    s++;
}

list_elem * Get_Other(list_elem * element){//out, in
    Gde;
    p(10, ("Prishlo %s\n", s));

    char * pointer_lbracket = strchr(s, '(');
    assert(pointer_lbracket);
    *pointer_lbracket = '\0';

    if (strcmp(s, "in")) {
        *pointer_lbracket = '(';
        s = pointer_lbracket;
        Do_In;
    }
    else if (strcmp(s, "out")) {
        *pointer_lbracket = '(';
        s = pointer_lbracket;
        Do_Out;
    }
    else {
        *pointer_lbracket = '(';
        s = pointer_lbracket;
        printf("Undefined command. Go fuck yourself or write something reasonable at %s\n", s);
    }
    return NULL;
}
*/

list_elem * Make_Functions(list_elem * element){
    assert(element);
    /*int level = 0;
    char * r_bracket = s;
    char * br_last = s;
    while (1){
        p(10, ("Prishlo to function%s\n", s));


        element->value = s;
        element->data_type = TYPE_FUNCTION;
        while(1) {
            printf("end %i of %i %i\n", strchr((char *) s, '\0'), s, *s);
            char *bracket = (char *) strchr((char *) s, '{');
            if (bracket) {
                level++;
                printf("level++\n");
            }

            if (r_bracket)
                br_last = r_bracket;

            if (r_bracket)
                r_bracket = (char *) strchr((char *) r_bracket + 1, '}');
            if (!r_bracket) {
                printf("breaking\n");
                s = br_last + 1;
                assert(s - 1);
                break;
            }
            level--;

            if (bracket)
                s = bracket + 1;
            else
                s = r_bracket + 1;

            printf("LVL %i\n\n", level);

        }

        s = Skip_Different_Shit(s);
        if (*s == '\0')
            break;
        element = Add_After(element);
    }*/
    while(1){
        p(10, ("Prishlo to function%s\n", s));
        s = Skip_Different_Shit(s);
        if (*s == '\0')
            break;

        element = Code_To_List(element);
    }
    return element;
}

list_elem * Code_To_List(list_elem * element){

    bool Break = false;
    while (1) {
        s = Skip_Different_Shit(s);

        p(10, ("Prishlo to code to list %s\n", s));
        if (*s == '\0')
            break;

        if (*s == '{'){
            element->data_type = TYPE_LBR;
            element->value = s;
            element = Add_After(element);
            s++;
            continue;
        }
        if (*s == '}'){
            element->data_type = TYPE_RBR;
            element->value = s;
            element = Add_After(element);
            s++;
            continue;
        }
        char * word_end = The_End_Of_The_Word(s);
        assert(word_end);
        unsigned char end_of_the_word = *word_end;
        *word_end = '\0';
        p(10, ("Prishlo %i %s\n", s, s));
#define DEF_CMD(name, type, num, code)\
        if (strcmp((char *)s, #name) == 0){\
            p(10, ("Eeee podoshlo %s\n", #name));\
            element->data_type = type;\
            element->value = s;\
            if (num == CMD_end)\
                Break = true;\
        } else

#include "commands.h"
        {
            *word_end = end_of_the_word;
            p(13, ("Pohodoo eto variablya ili function %s\n", s));
            char * pointer_ravno = strchr((char *) s, '=');
            char * pointer_lbracket = strchr((char *) s, '(');
            if (!pointer_lbracket){//Definitely assignment
                element->data_type = TYPE_ASS;
                element->value = s;
            } else if (!pointer_ravno){//Definitely function
                element->data_type = TYPE_FUNC;
                element->value = s;
                goto function;
            } else if (pointer_lbracket < pointer_ravno){//Function
                element->data_type = TYPE_FUNC;
                element->value = s;
                goto function;
            } else{
                element->data_type = TYPE_ASS;
                element->value = s;
            }

        }

        *word_end = end_of_the_word;

        if (element->data_type == TYPE_IF || element->data_type == TYPE_ELS){
            s = (char *) strchr((char *) s, '{');
        } else {
            s = (char *) strchr((char *) word_end, ';');
            assert(s);
            s++;
        }
        function:;
        if (element->data_type == TYPE_FUNC){
            s = (char *) strchr((char *) s, ')');
            assert(s);
            s++;
            p(13, ("Posle function %s\n", s));
        }

        s = Skip_Different_Shit(s);

        if (*s == '\0')
            break;
        element = Add_After(element);
        if (Break)
            break;
#undef DEF_CMD
    }
    return element;
}