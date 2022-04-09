#include "symbolTable.h"
typedef struct Error{
    char description[100];
    struct Error* next;
} Error;
#define DESCRIPTION_SIZE 100