
/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include "symbolTabledef.h"
symbolTable* newSymbolTable(int no_slots);
void addSymbol(symbolTable* symbolTable, char* key, SymbolTableRecord* symbol);
SymbolTableRecord* getSymbolInfo(char* key, symbolTable* symbolTable);
symbolTable * populateSymbolTable(ast_node * root,Error* err_list);
int traverseNodeFunction(ast_node * current, symbolTable* table, symbolTable* global, SymbolTableRecord * function);
int traverseNode(ast_node * current, symbolTable* global);
void printSymbolTable(symbolTable * global);
void printglobalvariables(symbolTable * global);
void printactivationrecords(symbolTable * global);
void printglobalrecords(symbolTable * global);