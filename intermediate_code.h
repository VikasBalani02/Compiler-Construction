#include "ast.h"
void IR_boolean_expression(ast_node* root);
void IR_for_astnode(ast_node* root);
void generate(ast_node* root, char ** arr, int no_to_add);
void createIR(ast_node* root);
char* newlabel();
char* newtemp();
tuple* newTuple(OP op, char* arg1, char* arg2, char* arg3, tuple* next);
void addTuple(tupleList* list, tuple* t);
tupleList* newList();

// void update_code(ast_node* t, char* str);