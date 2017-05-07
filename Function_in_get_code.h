printf("CUR FUNC ++\n\n\n\n");//It's a new function
func_array[cur_func] = Tree_Ctor();
Add_First(func_array[cur_func]);
func_array[cur_func++]->first = element;
element->data_type = TYPE_NEW_FUNC;

char *word_end = The_End_Of_The_Word(list_elem_now->value);
char the_word_end = *word_end;
*word_end = '\0';
char *value = (char *) calloc(word_end - list_elem_now->value + 1, sizeof(char));
strcpy(value, (char *) list_elem_now->value);
value[word_end - list_elem_now->value] = '\0';
*word_end = the_word_end;

Add_Right(element);
element->right->data_type = TYPE_VAR;
s = word_end + 1;
char *right_bracket = strchr((char *) s, ')');
assert(right_bracket);
*right_bracket = '\0';
element->right->value = (void *) Get_Var_Num();
*right_bracket = ')';

if (value)
element->value = value;
Add_Left(element);
list_elem_now = list_elem_now->next;
element = Get_Code(element->left);