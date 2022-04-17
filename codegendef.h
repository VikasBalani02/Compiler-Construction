/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include "intermediate_code.h"
#include <stdlib.h>
struct lex_info{
    int offset;
    int width;
    Type type;
    char* type_ruid;
};
typedef struct lex_info lex_info ;

struct lexeme_decomp{
    char* lex;
    lex_info* info;
    struct lexeme_decomp* next;
};
typedef struct lexeme_decomp lexeme_decomp;