/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "tree.h"

struct stack_node
{
    tNode *t_node;
    struct stack_node *next;
};
typedef struct stack_node stack_node;
struct stack
{
    stack_node *head;
    int size_of_stack;
};
typedef struct stack stack;
