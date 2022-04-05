/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "lookupTable.h"
// MACROS USED IN THE CODE
#define EPSILON 59
#define TOTAL_NUMBER_OF_NONTERMINALS 53
#define TOTAL_NUMBER_OF_TERMINALS 60
#define MAX_NONTERMINAL_LENGTH 20
#define MAX_TERMINAL_LENGTH 20
enum NON_TERMINAL
{
    program,
    otherFunctions,
    mainFunction,
    function,
    input_par,
    output_par,
    parameter_list,
    dataType,
    primitiveDatatype,
    constructedDatatype,
    remaining_list,
    stmts,
    typeDefinitions,
    typeDefinition,
    actualOrRedefined,
    fieldDefinitions,
    fieldDefinition,
    moreFields,
    fieldtype,
    declarations,
    declaration,
    global_or_not,
    otherStmts,
    stmt,
    assignmentStmt,
    oneExpansion,
    moreExpansions,
    option_single_constructed,
    singleOrRecId,
    funCallStmt,
    outputParameters,
    inputParameters,
    iterativeStmt,
    conditionalStmt,
    elsePart,
    expPrime,
    term,
    termPrime,
    factor,
    highPrecedenceOperators,
    lowPrecedenceOperators,
    ioStmt,
    arithmeticExpression,
    booleanExpression,
    var,
    logicalOp,
    relationalOp,
    returnStmt,
    optionalReturn,
    idList,
    more_ids
};
typedef enum NON_TERMINAL NON_TERMINAL;


// STRUCTURES FOR LOADING THE GRAMMAR
struct symbol_node
{
    Tokentype t;
    NON_TERMINAL nt;
    int is_terminal;
    struct symbol_node *next;
};

struct symbol_LL
{ // linked list of symbol_nodes
    int rule_no;
    struct symbol_node *head;
    struct symbol_node *tail;
    int len;
    struct symbol_LL *next_list;
};

struct rule
{                    // linked list of symbol lists, associating the symbol lists with a non terminal.
    NON_TERMINAL nt; // The LHS associated with the production rule.
    struct symbol_LL *head;
    struct symbol_LL *tail;
    int len;
};
struct grammar
{ // collection of all the production rules
    struct rule *rules[TOTAL_NUMBER_OF_NONTERMINALS];
    int total_number_of_rules;
};
typedef struct symbol_node symbol_node;
typedef struct symbol_LL symbol_LL;
typedef struct rule rule;
typedef struct grammar grammar;

typedef struct firstAndFollow
{
    char **firstSet;
    char **followSet;
} FnF;

// STRUCTURES FOR PARSE TABLE
typedef struct block
{
    symbol_LL *head;
} block;

typedef struct parseTable
{
    block *pTable[TOTAL_NUMBER_OF_NONTERMINALS][TOTAL_NUMBER_OF_TERMINALS - 2]; //-2 for removing EPSILON and
} parseTable;