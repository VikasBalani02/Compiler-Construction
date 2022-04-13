#include "symbolTable.h"
#define DESCRIPTION_SIZE 200
typedef struct Error{
    char description[DESCRIPTION_SIZE];
    struct Error* next;
} Error;
