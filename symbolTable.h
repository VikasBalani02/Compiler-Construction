#include "symbolTabledef.h"
symbolTable* newSymbolTable(int no_slots);
void addSymbol(symbolTable* symbolTable, char* key, SymbolTableRecord* symbol);
SymbolTableRecord* getSymbolInfo(char* key, symbolTable* symbolTable);
symbolTable * populateSymbolTable(ast_node * root);
int traverseNodeFunction(ast_node * current, symbolTable* table, symbolTable* global, SymbolTableRecord * function);
void traverseNode(ast_node * current, symbolTable* global);