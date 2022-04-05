/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "treeDef.h"
tNode *get_tree_node(int enumerated, tNode *parent, int is_terminal);
void bindChildren(tNode *t, symbol_LL *rhs);
Tree* initialiseParseTree();