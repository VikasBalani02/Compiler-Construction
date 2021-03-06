/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include "typeChecker.h"
char *concat(char *str1, char *str2);
tuple *newTuple(OP op, char *arg1, char *arg2, char *arg3, tuple *next);
void addTupleEnd(tupleList *list, tuple *t);
void concatLists(tupleList **l1, tupleList *l2);
void addTupleFront(tupleList *list, tuple *t);
tupleList *newList();
char *newtemp(symbolTable *localTable, symbolTable *global, struct typeInfo *nodeType);
char *newlabel();
tupleList *get_intermediate_list(ast_node *root, symbolTable *global);
void createIR(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_for_astnode(ast_node *root, symbolTable *localTable, symbolTable *global);
insideRecord *getParamList(ast_node *params_anode, insideRecord *list_new, symbolTable *global);
void IR_funct_call(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_function(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_singleOrRecId(ast_node *root, symbolTable *localTable, symbolTable *global);
insideRecord *getInsideRecord(char *lexeme);
insideRecord *reverseList(insideRecord *head);
insideRecord *insertLexeme(insideRecord *head, char *lexeme);
insideRecord *getRecordDetails(char *lexeme, char *record_name, symbolTable *global);
insideRecord *getRecordDetails_util(char *lexeme, insideRecord *head, char *record_name, symbolTable *global);
void IR_intOrRealValue(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_assignmentStmt(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_lowPrecedenceTerm(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_highPrecedenceTerm(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_boolean_expression(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_conditional(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_iterative(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_stmts(ast_node *root, symbolTable *localTable, symbolTable *global);
void IR_iostmt(ast_node *root, symbolTable *localTable, symbolTable *global);
void print_tuple(tuple *t);
void print_tupleList(tupleList *tlist);