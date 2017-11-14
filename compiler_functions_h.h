#ifndef COMPILER_COMPILER_FUNCTIONS_H_H
#define COMPILER_COMPILER_FUNCTIONS_H_H

int _error = 0;

elem * GetN(elem * element);
elem * GetVar(elem * element);
elem * GetFunc(elem * element);
elem * GetE(elem * element);
elem * GetT(elem * element);
elem * GetP(elem * element);
elem * GetD(elem * element);
elem * GetEq(elem * element);


void Parse_Error();


int cur_label = 0;
variable * var_array[20];
int current_var = 1;
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

#endif //COMPILER_COMPILER_FUNCTIONS_H_H
