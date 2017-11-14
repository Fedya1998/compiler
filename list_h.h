#ifndef COMPILER_LIST_H_H
#define COMPILER_LIST_H_H

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


#endif //COMPILER_LIST_H_H
