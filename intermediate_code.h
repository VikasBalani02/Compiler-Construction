#include "typeChecker.h"
void IR_boolean_expression(ast_node* root, symbolTable* localTable, symbolTable* global);
void IR_singleOrRecId(ast_node* root, symbolTable* localTable, symbolTable* global);
void IR_lowPrecedenceTerm(ast_node* root, symbolTable* localTable, symbolTable* global);
void IR_highPrecedenceTerm(ast_node* root, symbolTable* localTable, symbolTable* global);
void IR_assignmentStmt(ast_node* root, symbolTable* localTable, symbolTable* global);
insideRecord* getRecordDetails(char* lexeme, char* record_name, symbolTable* global);
insideRecord* getRecordDetails_util(char* lexeme, insideRecord* head, char* record_name, symbolTable* global);
void IR_otherStmts(ast_node* root, symbolTable* localTable, symbolTable* global);
void IR_for_astnode(ast_node* root, symbolTable* localTable, symbolTable* global);
void IR_funct_call(ast_node* root, symbolTable* localTable, symbolTable* global);
void generate(ast_node* root, char ** arr, int no_to_add);
void createIR(ast_node* root, symbolTable* localTable, symbolTable* global);
char* newlabel();
char* newtemp();
tuple* newTuple(OP op, char* arg1, char* arg2, char* arg3, tuple* next);
void addTuple(tupleList* list, tuple* t);
tupleList* newList();
void IR_iostmt(ast_node *root, symbolTable* localTable, symbolTable* global);

// void update_code(ast_node* t, char* str);