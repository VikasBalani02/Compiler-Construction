/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include <stdlib.h>
#include "parser.h"
enum CONSTRUCT
{
    program_,
    otherFunctions_,
    mainFunction_,
    function_,
    input_par_,
    output_par_,
    parameter_list_,
    Identifier_,
    dataType_,
    primitiveDatatype_,
    constructedDatatype_,
    remaining_list_,
    stmts_,
    typeDefinitions_,
    typeDefinitionStmts_,
    typeDefinition_,
    actualOrRedefined_,
    fieldDefinitions_,
    fieldDefinition_,
    moreFields_,
    fieldtype_,
    declarations_,
    declaration_,
    global_or_not_,
    otherStmts_,
    stmt_,
    assignmentStmt_,
    oneExpansion_,
    moreExpansions_,
    option_single_constructed_,
    singleOrRecId_,
    funCallStmt_,
    outputParameters_,
    inputParameters_,
    iterativeStmt_,
    conditionalStmt_,
    elsePart_,
    expPrime_,
    term_,
    termPrime_,
    factor_,
    highPrecedenceOperators_,
    lowPrecedenceOperators_,
    ioStmt_,
    arithmeticExpression_,
    booleanExpression_,
    var_,
    logicalOp_,
    relationalOp_,
    returnStmt_,
    optionalReturn_,
    idList_,
    definetypestmt_,
    defineMapping_,
    A_,
    more_ids_,
    thenStmts_,
    INTNUM_,
    REALNUM_,
    highPrecedenceTerm_,
    lowPrecedenceTerm_
};
typedef enum CONSTRUCT CONSTRUCT;

typedef enum TYPE{
    PARAMO,
    PARAMI,
    CALL,
	INT,
	REAL,
	RECORD,
    VARIANTRECORD,
	UNION,
	RUID,
    FUNCTION,
    BOOL,
    VOID,
    ERROR_,
    EMPTY, //to be used in case A->eps
    UNDEFINED //if there is no type associated with an ast node.
}Type;

typedef enum OP{
    MUL,
    DIV,
    PLUS,
    MINUS,
    IF,
    GOTO,
    LT,
    LE,
    EQ,
    NE,
    GT,
    GE,
    NOT,
    LABEL,
    UNSET,
    SET,
    ASSIGN,
    READ,
    WRITE,
    FUNCT,
    ENDFUNCT
}OP;
struct tuple
{
    OP op;
    char* arg1; //first operand
    char* arg2; //second operand
    char* arg3; //result
    struct tuple* next;
};
typedef struct tuple tuple;
struct tupleList{
    tuple* head;
    tuple* tail;
};
typedef struct tupleList tupleList;


typedef struct typeInfo{
	Type type;
	char * type_ruid;
    char *lexeme;
	struct typeInfo * next;
}typeInfo;

struct insideRecord{
    struct insideRecord* next;
    char* lex;
}; 
typedef struct insideRecord insideRecord;

struct ast_node {
    struct ast_node *firstChild;
    struct ast_node *nextSib;
    struct node_info *ninf;
    typeInfo* node_type; //to be populated during type checking 
    //next field is populated for outputParameters_ inputParameters_ optionalReturn_
    CONSTRUCT construct;
    char* place;
    tupleList* list;
};
typedef struct ast_node ast_node;

struct node_info {
    char arr[200];
};
struct func_struct {
    int lineNum;
    char* funID;
};
struct id_struct {
    int lineNum;
    char* lexID;
    int isGlobal;
    ast_node* dataType;
};
struct global_struct{
    int isGlobal;
};
struct primitive_type_struct {
    int lineNum;
    Tokentype int_or_real;
};
struct constructed_type_struct {
    int lineNum;
    Tokentype union_or_record;
    char* ruid; 
};

struct field_id_struct {
    int lineNum;
    char* fieldid;
};

struct io_struct {
    int lineNum;
    Tokentype read_or_write;
};

struct operator_struct{
    int lineno;
    Tokentype op;
};

struct num_struct{
    int lineno;
    Tokentype type;
    union number * value;
    int tag;
};
struct defineType_struct{
    struct constructed_type_struct * from;
    struct constructed_type_struct * to;
};
struct A{
    Tokentype UnionOrRecord;
};


// ast_node* makeNode(CONSTRUCT c, ast_node** a, int no_children, struct node_info* n_info){
//     struct ast_node* new_node= (ast_node*)malloc(sizeof(ast_node));
//     new_node->construct=c;
//     new_node->firstChild= (a==NULL)?NULL:*a;
//     new_node->nextSib=NULL;
//     new_node->ninf=n_info;
//     for(int i=0;i<no_children-1;i++){
//         a[i]->nextSib=a[i+1];
//     }
//     return new_node;
// }





