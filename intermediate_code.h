#include "typeChecker.h"
void IR_boolean_expression(ast_node* root);
void IR_singleOrRecId(ast_node* root);
void IR_lowPrecedenceTerm(ast_node* root);
void IR_highPrecedenceTerm(ast_node* root);
void IR_assignmentStmt(ast_node* root);
insideRecord* getDetails(char* lexeme, insideRecord* head , char* record_name, symbolTable* global);
void IR_otherStmts(ast_node* root);
void IR_for_astnode(ast_node* root);
void generate(ast_node* root, char ** arr, int no_to_add);
void createIR(ast_node* root);
char* newlabel();
char* newtemp();
tuple* newTuple(OP op, char* arg1, char* arg2, char* arg3, tuple* next);
void addTuple(tupleList* list, tuple* t);
tupleList* newList();

// void update_code(ast_node* t, char* str);