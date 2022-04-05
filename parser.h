/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include <stdio.h>
#include "stack.h"


// FUNCTION PROTOTYPES
symbol_node *initialize_symbol_node(int value, int is_term);
symbol_LL *initialize_symbol_ll();
rule *initialize_rule(NON_TERMINAL nt);
grammar *initialize_grammar();
void add_symbol_to_symbol_ll(symbol_LL *list, symbol_node *node);
void add_symbol_ll_to_rule(grammar *g, symbol_LL *list, NON_TERMINAL nt);
NON_TERMINAL map_string_to_nonterminal(char *str);
Tokentype map_string_to_terminal(char *str);
grammar *load_grammar(FILE *fptr);
void print_grammar(grammar *g);
void printFirstOrFollowSets(char **sets);
FnF *ComputeFirstAndFollowSets(grammar *gr);
void get_First(grammar *gr, NON_TERMINAL nt, char **firstSet);
FnF *init_fnf();
void combine_sets(char *set1, char *set2);
void get_Follow(grammar *gr, char **firstSet, char **followSet);
void errorCheck(parseTable *pt, NON_TERMINAL i, Tokentype k, symbol_LL *list_to_insert);
void print_list(symbol_LL *list);
void printTable(parseTable *pt);
parseTable *parseTableCreate(grammar *g, FnF *fnf);
block *createBlock(symbol_LL *head);
Tree* parseInputSourceCode(char *testcaseFile, parseTable *T);
void printParseTree(tNode* root , char* outfile);
void assign_rule_no(grammar *g);
char *non_terminal_map[53];