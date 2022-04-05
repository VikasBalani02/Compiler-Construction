/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "stack.h"
#include<stdlib.h>
stack_node *get_stack_node(tNode *node)
{ // utility function :never directly called
    stack_node *new_node = malloc(sizeof(stack_node));
    new_node->next = NULL;
    new_node->t_node = node;
    return new_node;
}
stack *getStack()
{
    stack *s = malloc(sizeof(stack));
    s->head = NULL;
    s->size_of_stack = 0;
    return s;
}
void push_stack_node(stack *s, stack_node *new_node)
{ // utility function :never directly called
    new_node->next = s->head;
    s->head = new_node;
    s->size_of_stack += 1;
}
void push_tree_node(stack *s, tNode *t)
{
    stack_node *new_node = get_stack_node(t);
    push_stack_node(s, new_node);
}
tNode *pop_tree_node(stack *s)
{
    if (s->head == NULL)
    {
        printf("\nERROR: stack is empty cannot pop");
        return NULL;
    }
    stack_node *temp = s->head;
    s->head = s->head->next;
    tNode *treenode = temp->t_node;
    s->size_of_stack -= 1;
    return treenode;
}
tNode *top_tree_node(stack *s)
{
    if (s->head)
    {
        return s->head->t_node;
    }
    // printf("\nERROR: stack is empty");
    return NULL;
}
void push_tree_node_list(stack *s, tNode *t)
{ // utility function :never directly called
    if (s == NULL || t == NULL)
        return;
    push_tree_node_list(s, t->next_sibling);
    push_tree_node(s, t);
}
void push_tnode_children(stack *s, tNode *t)
{
    if (s == NULL || t == NULL || t->child_first == NULL)
        return;
    tNode *children_list = t->child_first;

    if (children_list->is_terminal && children_list->t == EPSILON)
    {
        // Cannot push rules of type A->eps on to the stack
        return;
    }
    // push children on the stack
    push_tree_node_list(s, children_list);
}