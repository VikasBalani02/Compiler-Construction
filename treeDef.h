// Tushar Garg - 2019A7PS0104P
// Vikas Balani - 2019A7PS0054P
// Ruchir Jain - 2019A7PS0067P
// Usneek Singh - 2019A7PS0127P
// Abhijith S Raj - 2019A7PS0055P

#include "parserDef.h"
struct tNode
{
    Tokentype t;
    NON_TERMINAL nt;
    int is_terminal;
    int rule_no;
    struct tNode *next_sibling;
    struct tNode *child_first;
    int num_child;
    Token lex_token;
    struct tNode *parent; // for printing inparse tree
    struct ast_node *addr;
};
typedef struct tNode tNode;
struct tree
{
    tNode *root;
};
typedef struct tree Tree;