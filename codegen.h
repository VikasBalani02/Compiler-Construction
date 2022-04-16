#include "codegendef.h"

lexeme_decomp *get_lexdecomp_struct(char *lex);
lexeme_decomp *decompose_lexeme(char *lex);
lex_info* get_offset(lexeme_decomp *lex, struct symbolTable *local_table, struct symbolTable *global_table);
void get_offset_util(lexeme_decomp *lexList, SymbolTableRecord *sym_info, struct symbolTable *local_table, struct symbolTable *global_table);
void generate_code(tupleList* intermediateCode, symbolTable * global, FILE * output);
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
