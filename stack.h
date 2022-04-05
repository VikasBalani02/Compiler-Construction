/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include"stackDef.h"
stack_node *get_stack_node(tNode *node);
stack *getStack();
void push_stack_node(stack *s, stack_node *new_node);
void push_tree_node(stack *s, tNode *t);
tNode *pop_tree_node(stack *s);
tNode *top_tree_node(stack *s);
void push_tree_node_list(stack *s, tNode *t);
void push_tnode_children(stack *s, tNode *t);