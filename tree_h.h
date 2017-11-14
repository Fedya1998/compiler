#ifndef COMPILER_TREE_H_H
#define COMPILER_TREE_H_H

tree_header * Tree_Ctor(void);
void Tree_Dtor(tree_header * header);
bool Elem_And_Next_OK(const elem *element);
bool Tree_OK(const tree_header *header);
bool Elem_OK(const elem * element);
void Elem_Dump(const elem *element);
void Elem_And_Next_Dump(const elem * elem);
void Tree_Dump(const tree_header *header);
elem * Add_First(tree_header * header);
elem * Add_Left(elem * elem);
elem * Add_Right(elem * elem);
void Mini_Delete(elem * element);
void Mini_Delete(elem * elem);
int Delete_Tree(tree_header * header);
void Mini_File_Dump(const elem * element);
int Optimize_First(elem * element);
int Tree_Optimize(elem * element);
bool Wow_Its_Null(elem * element);
elem * Pridvin_Poblizhe(elem * element, int otkooda);
void Delete_Useless_Parentheses(elem * element);
elem * Copy_Elem(elem * element);
void Super_Optimize(elem * element);


#endif //COMPILER_TREE_H_H
