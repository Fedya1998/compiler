void Tree_To_File(tree_header * header){
    assert(header);
    FILE * akin = fopen("akinator.txt", "w");
    if (!header->first) return;
    Tree_To_File_Mini(header->first, akin);
}

void Tree_To_File_Mini(elem * element, FILE * akin){
    fprintf(akin, "(");
    fprintf(akin, "%s", element->value);
    fprintf(akin, " ");
    if (element->left) Tree_To_File_Mini(element->left, akin);
    else fprintf(akin, "null");
    fprintf(akin, " ");
    if (element->right) Tree_To_File_Mini(element->right, akin);
    else fprintf(akin, "null");
    fprintf(akin, ")");
}

tree_header * File_To_Tree(FILE * source){
    assert(source);
    char * buf = (char *) File_To_Buf(source);
    char * buf_to_tree = buf;
    tree_header * header = Tree_Ctor();
    d(10, printf("space_pointer %i &space_pointer%i\n", buf_to_tree, &buf_to_tree));
    header->first = File_To_Tree_Mini(&buf_to_tree, header);
    free(buf);
    return header;
}

elem * File_To_Tree_Mini(char ** buf, tree_header * header){
    p(10, ("source %i\n", buf));
    assert(buf);
    assert(header);
    p(9, ("Prishlo %s\n", *buf));
    elem * element = NULL;
    if (!(*buf)) {
        p(10, ("The end of the file, I'm returning null\n"));
        return element;
    }
    char * pointer_now = *buf + 1;
    d(10, printf("pointer_now %i\n", pointer_now));
    if (pointer_now[0] == '(') pointer_now++;
    Err;
    element = new elem();//Exactly here it breaks every time using big akinator file enough
    Err;

    element->header = header;
    char * space_pointer = strchr(pointer_now + 1, ' ');
    p(10, ("space_pointer %i\n", space_pointer));

    if (*(pointer_now + 4) == ')') space_pointer = pointer_now + 4;
    if (!space_pointer) return element;
    p(10, ("space_pointer %i\n", space_pointer));
    element->value = (char *)calloc(space_pointer - pointer_now + 1, sizeof(char));
    sscanf(pointer_now, "%s", element->value);
    element->value[space_pointer - pointer_now + 1] = '\0';
    p(9, ("value %s\n", element->value));

    if (strcmp(element->value, "null") == 0 || strcmp(element->value, "null)") == 0) {
        p(10, ("null was returned\n"));
        return NULL;
    }

    p(9, ("left "));
    element->left = File_To_Tree_Mini(&space_pointer, header);
    space_pointer = strchr(space_pointer + 1, ' ');
    p(9, ("right "));
    element->right = File_To_Tree_Mini(&space_pointer, header);
    *buf = space_pointer;
    return element;
}

elem * Define_Me(tree_header * header){
    assert(header);
    printf("Select a character to be guessed\n");
    elem * element = NULL;
    if (header->first->value) element = Mini_Define(header->first);
    else printf("Sorry, database tree is empty. Try another\n");

    return element;
}

elem * Mini_Define(elem * This){
    if (This == NULL) {
        printf("How can I define someone if you've just given me the NULL element?");
        return NULL;
    }
    SUPER_NOT_OK_DUMP(Elem);
    elem * element = NULL;
    char * answer = (char *) calloc(4, sizeof(char));
    char * question = strchr(This->value, '?');
    if (question) {
        printf("%s Type word exit if you don't wanna play with me anymore\n", This->value);
        typing1:;
        scanf("%s", answer);
        answer [3] = '\0';
        if (strcmp(answer, "yes") == 0) element = Mini_Define(This->left);
        else {
            if (strcmp(answer, "no")) {
                if (strcmp(answer, "exit") == 0) return NULL;
                printf("Wrong answer, try one more time\n");
                goto typing1;
            } else {
                element = Mini_Define(This->right);
            }
        }
    }
    else {
        printf("Your character is %s? Type word exit if you don't wanna play with me anymore\n", This->value);
        typing:;
        scanf("%s", answer);
        answer [3] = '\0';
        if (strcmp(answer, "yes") == 0) {
            printf("Eeeee PABEDA");
            return This;
        }
        else {
            if (strcmp(answer, "no")) {
                if (strcmp(answer, "exit") == 0) return NULL;
                printf("Wrong answer, try one more time\n");
                goto typing;
            } else {
                Add_Person(This);
            }
        }
    }
    free(answer);
    return element;
}



elem * Add_Person(elem * This){
    SUPER_NOT_OK_DUMP(Elem);
    printf("Who's your character?\n");
    char * new_person = (char *) calloc(20, sizeof(char));
    scanf("%s", new_person);
    char * new_person_lessmem = New_String_Lessmem(new_person);
    p(10, ("new_string_lessmem %s\n", new_person_lessmem));

    printf("What's the difference between your %s and %s?\n", new_person_lessmem, This->value);

    char * diff = (char *) calloc(30, sizeof(char));
    scanf("%s", diff);
    char * question_mark = strchr(diff, '\0');
    *question_mark = '?';
    *(question_mark + 1) = '\0';

    char * diff_new = New_String_Lessmem(diff);
    p(10, ("new_string_lessmem %s\n", new_person_lessmem));

    Gde;
    elem * right = Add_Right(This);
    if(!right){
        printf("Damn it! The right one is bad\n");
        return NULL;
    }
    right->value = This->value;
    elem * left = Add_Left(This);
    if(!right){
        printf("Damn it! The left one is bad\n");
        return NULL;
    }
    left->value = new_person_lessmem;
    This->value = diff_new;

    return This->left;
}