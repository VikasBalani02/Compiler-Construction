#include "symbolTabledef.h"
typedef struct Error{
    char description[50];
} Error;

typeInfo *get_typeInfo(Type type, char *ruid);
void *populate_Identifier_type(ast_node *id_node, Error *err_list, symbolTable table);
struct symbolTableRecord *get_function_entry(ast_node *funid_node, Error *err_list, symbolTable table);
void check_stmt(ast_node *node, Error *err_list, symbolTable table);
void check_assign_stmt(ast_node *node, Error *err_list);
void check_io_stmt(ast_node *node, Error *err_list);
int compare_list(ast_node *node, typeInfo *formalParamList, Error *err_list);
void check_funCall_stmt(ast_node *node, Error *err_list, symbolTable table);
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