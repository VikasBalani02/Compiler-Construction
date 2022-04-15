#include "intermediate_code.h"

void generate_code(symbolTable * global, FILE * assembly_file_ptr);
char* op_map_[] = {
    "MUL",
    "DIV",
    "PLUS",
    "MINUS",
    "IF",
    "GOTO",
    "LT",
    "LE",
    "EQ",
    "NE",
    "GT",
    "GE",
    "NOT",
    "LABEL",
    "UNSET",
    "SET",
    "ASSIGN",
    "READ",
    "WRITE",
    "FUNCT",
    "ENDFUNCT"
};
