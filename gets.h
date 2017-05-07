void Parse_Error(){
    printf("Error %i on %s\n", _error, s);
}

tree_header * GetG0(char * str){
    s = str;
    tree_header * header = Tree_Ctor();
    Add_First(header);
    GetE(header->first);
    if (*s != '\0' || _error) Parse_Error();
    return header;
}

elem * GetSt(elem * element){//Try to ger number, var or function

    //Gde;
    //p(11, ("Got %s\n", s));
    if (!GetN(element)) GetFunc(element);
    return element;
}

elem * GetN(elem * element){
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));
    double * res = new double;
    *res = 0;
    int minus = 0;
    _error = ERROR_EMPTY_LINE;
    if (*s == '-') {
        minus = 1;
        s++;
    }

    while('0'<= *s && *s <= '9'){
        _error = 0;
        *res = *res * 10 + *s - '0';
        s++;
    }
    if (*s == '.'){
        s++;
        while('0'<= *s && *s <= '9') {
            *res += (double) (*s - '0') / 10;
            s++;
        }
    }
    if (minus)
        *res *= -1;
    if (!_error){
        element->value = res;
        element->data_type = TYPE_NUMBER;
        p(10, ("I'm returning *res %lg element %i\n", *res, element));
        return element;
    } else {
        delete res;
        return NULL;
    }

}

elem * GetFunc(elem * element) {
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));
    if (GetVar(element)) return element;
    char *func_end = (char *) strchr((char *) s, '(');
    *func_end = '\0';
    assert(func_end - s + 1 > 0);
    char *super_func = (char *) calloc(func_end - s + 1, sizeof(char));
    sscanf((char *) s, "%s", super_func);
    super_func[func_end - s] = '\0';
    *func_end = '(';
    //d(12, printf("my function %s\n", super_func));

    element->value = super_func;
    element->data_type = TYPE_FUNC;
    s = func_end;
    Add_Left(element);
    //Gde;
    GetP(element->left);

    //printf("I'm returning element %i", element);
    return element;
}

elem * GetVar(elem * element){
    assert(element);
    Gde;
    p(11, ("Got %s\n", s));

    char * cur = s;
    char * end = The_End_Of_The_Word(cur);
    assert(end);
    char wut_end = *end;
    *end = '\0';

    for (int i = 0; i < current_var; i++) {
        if (strcmp((char *) s, (char *) var_array[i]->id) == 0) {
            //p(10, ("Nashel var from array\n"));
            *end = wut_end;
            s = end;
            element->value = (void *) i;
            element->data_type = TYPE_VAR;
            return element;
        }
    }

    *end = wut_end;

    p(10, ("Not var\n"));
    return NULL;
}

elem * GetEq(elem * element){
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));
    Add_Left(element);
    GetE(element->left);
    //p(12, ("After GetE %s", s));
    while (*s == '>' || *s == '<' || *s == '='){
        s++;
        char operation = s[-1];
        element->data_type = TYPE_OP;
        if (operation == '>' && *s == '=') {
            element->value = (void *) CMD_jae;
            s++;
        }
        else if (operation == '<' && *s == '=') {
            element->value = (void *) CMD_jbe;
            s++;
        }
        else if (operation == '>') {
            element->value = (void *) CMD_ja;
        }
        else if (operation == '<') {
            element->value = (void *) CMD_jb;
        }
        else if (operation == '=' && *s == '=') {
            s++;
            element->value = (void *) CMD_je;
        }
        else if (operation == '!' && *s == '=') {
            s++;
            element->value = (void *) CMD_jne;
        }


        Add_Right(element);
        element = element->right;
        GetE(element);
    }
    //p(10, ("I'm returning element %p\n", element));
    return element;
}

elem * GetE(elem * element){
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));
    Add_Left(element);
    GetT(element->left);
    elem * to_right = element;
    while (*s == '+' || *s == '-'){
        s++;
        char operation = s[-1];

        to_right->data_type = TYPE_OP;
        if (operation == '+') {
            //*value = CMD_ADD;
            to_right->value = (void *) CMD_add;
        }
        if (operation == '-') {
            //*value = CMD_SUB;
            to_right->value = (void *) CMD_sub;
        }

        to_right = Add_Right(to_right);
        GetT(to_right);
    }
    //p(10, ("Get E I'm returning element %p %s\n", element, s));
    return element;
}

elem * GetT(elem * element){
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));
    Add_Left(element);
    GetD(element->left);
    elem * to_right = element;
    //Gde;
    /*int * value = new int;
    *value = 0;*/
    while (*s == '*' || *s == '/'){
        //p(10, ("Eeee eto for GetT\n"));
        s++;
        char operation = s[-1];
        to_right->data_type = TYPE_OP;
        if (operation == '*') {
            //*value = CMD_MUL;
            to_right->value = (void *) CMD_mul;
        }
        if (operation == '/') {
            //*value = CMD_DIV;
            to_right->value = (void *) CMD_div;
        }
        to_right = Add_Right(to_right);
        GetD(to_right);
    }
    //p(10, ("Get T I'm returning element %p %s\n", element, s));
    return element;
}

elem * GetD(elem * element){
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));
    Add_Left(element);
    GetP(element->left);
    elem * to_right = element;

    while (*s == '^'){
        s++;
        to_right->data_type = TYPE_OP;
        to_right->value = (void *) CMD_pow;

        to_right = Add_Right(to_right);
        GetP(to_right);
    }
    //p(10, ("Get D I'm returning element %p %s\n", element, s));
    return element;
}

elem * GetP(elem * element){
    assert(element);
    //Gde;
    //p(11, ("Got %s\n", s));

    if (*s == '('){
        Add_Left(element);
        s++;
        //*value = CMD_LBR;
        element->value = (void *) CMD_LBR;
        element->data_type = TYPE_OP;
        GetE(element->left);
        if (*s != ')') _error = ERROR_WRONG_PARENTHESES;

        Add_Right(element);

        element->right->value = (void *) CMD_RBR;
        element->right->data_type = TYPE_OP;

        if (*s == ')')
            s++;
    }
    else return GetSt(element);

    //p(10, ("Get P I'm returning element %p %s\n", element, s));
    return element;
}

/*
elem * GetCode(elem * element){
    char * pointer_space = strchr(s, ' ');
    if (pointer_space) *pointer_space = '\0';

    if (strcmp("if", s) == 0)
        Che_Delat_Esli_If(element);

    else if GetVar(element);

    Just_Do(element);
















    Get
    char * pointer_space = strchr(s, ' ');
    if (pointer_space) *pointer_space = '\0';
    #define DEF_CMD(name, type, num, code) if (strcmp(#name, s) == 0) {\
        element->data_type = type;\
        element->value = num;\
    }

#include "commands.h"
#undef DEF_CMD
    if (pointer_space) *pointer_space = ' ';
    Add_Left(Element->)

}*/