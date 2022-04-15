#include "intermediate_code.h"
#include <stdlib.h>
struct lexeme_decomp{
    char* lex;
    int offset;
    struct lexeme_decomp* next;
};
typedef struct lexeme_decomp lexeme_decomp;