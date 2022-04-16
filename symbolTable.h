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