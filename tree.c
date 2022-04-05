/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "tree.h"
#include<stdlib.h>

tNode *get_tree_node(int enumerated, tNode *parent, int is_terminal)
{
    tNode *t = (tNode *)malloc(sizeof(tNode));
    t->is_terminal = is_terminal;
    t->num_child = 0;
    t->rule_no = 0;
    t->child_first = NULL;
    t->lex_token = NULL;
    t->next_sibling = NULL;
    t->parent = parent;
    if (is_terminal)
        t->t = enumerated;
    else
        t->nt = enumerated;
    return t;
}
void bindChildren(tNode *t, symbol_LL *rhs)
{
    if (rhs == NULL || t == NULL)
        return;
    t->num_child = rhs->len;
    int enumerated;
    t->rule_no = rhs->rule_no;
    tNode *curr_child_node;
    tNode *prev_child_node = NULL;
    symbol_node *curr_sym_node = rhs->head;
    while (curr_sym_node != NULL)
    {
        if (curr_sym_node->is_terminal)
            enumerated = curr_sym_node->t;
        else
            enumerated = curr_sym_node->nt;
        curr_child_node = get_tree_node(enumerated, t,curr_sym_node->is_terminal);
        if (prev_child_node == NULL)
        {
            prev_child_node = curr_child_node;
            t->child_first = curr_child_node;
        }
        else
        {
            prev_child_node->next_sibling = curr_child_node;
            prev_child_node = curr_child_node;
        }
        curr_sym_node = curr_sym_node->next;
    }
}
Tree * initialiseParseTree(){
    Tree* tree = (Tree *)malloc(sizeof(Tree));
	tree->root = NULL;
    return tree;
}
