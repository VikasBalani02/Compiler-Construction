#include "symbolTable.h"
#include "symbolTabledef.h"
typedef struct Error{
    char description[50];
} Error;

typedef struct type_struct{
    Type type;
    char* ruid;
} type_struct;