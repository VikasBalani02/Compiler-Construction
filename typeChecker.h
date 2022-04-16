#include "typeCheckerDef.h"
typeInfo *get_typeInfo(Type type, char *ruid);
void populate_Identifier_type(ast_node *id_node, Error *err_list, symbolTable* table,symbolTable* global);
SymbolTableRecord* get_function_entry(char *funid, Error *err_list, symbolTable* table,symbolTable* global);
void type_checker(ast_node* root,Error* err_list,symbolTable* table,symbolTable* global,int fun_num);
void check_type(ast_node *node, Error *err_list, symbolTable* table,symbolTable* global,int fun_num);
void check_assign_stmt(ast_node *node, Error *err_list);
void check_io_stmt(ast_node *node, Error *err_list);
int compare_param_list(struct func_struct* info, ast_node *node, typeInfo *formalParamList, Error *err_list);
void check_funCall_stmt(ast_node *node, Error *err_list, symbolTable* table,symbolTable* global,int fun_num);
void check_iterative_stmt(ast_node *node, Error *err_list);
void check_conditional_stmt(ast_node *node, Error *err_list);
void check_return_stmt(ast_node *node, Error *err_list);
void check_arithmeticExpression(ast_node *node, Error *err_list);
void check_term(ast_node *node, Error *err_list);
void check_factor(ast_node *node, Error *err_list);
void check_termPrime(ast_node *node, Error *err_list);
void check_expPrime(ast_node *node, Error *err_list);
void check_stmt_group(ast_node *node, Error *err_list);
void check_boolean_exp(ast_node *node, Error *err_list);
void check_id_list(ast_node *node, Error *err_list);
void check_num_type(ast_node *node, Error *err_list);

void check_highPrecedenceTerm(ast_node* node,Error* err_list);
void check_lowPrecedenceTerm(ast_node* node,Error* err_list);
int find_if_assigned(ast_node* stmt_list,char* lex);
void check_function_definition(ast_node *node, Error *err_list);