/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include "codegendef.h"

lexeme_decomp *get_lexdecomp_struct(char *lex);
lexeme_decomp *decompose_lexeme(char *lex);
lex_info* get_offset(lexeme_decomp *lex, struct symbolTable *local_table, struct symbolTable *global_table);
void get_offset_util(lexeme_decomp *lexList, SymbolTableRecord *sym_info, struct symbolTable *local_table, struct symbolTable *global_table);
void generate_code(tupleList* intermediateCode, symbolTable * global, FILE * output);
Type getNumType(char *str);
void copyback_param_code_gen(char *lex, struct symbolTable *local);
// char* OP_MAP[] = {
//     "MUL",
//     "DIV",
//     "PLUS",
//     "MINUS",
//     "IF",
//     "GOTO",
//     "LT",
//     "LE",
//     "EQ",
//     "NE",
//     "GT",
//     "GE",
//     "NOT",
//     "LABEL",
//     "UNSET",
//     "SET",
//     "ASSIGN",
//     "READ",
//     "WRITE",
//     "FUNCT",
//     "ENDFUNCT"
// };
