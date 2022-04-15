#include "codegen.h"

void generate_code(symbolTable * global, FILE * assemblyFile){
    fprintf(assemblyFile, "extern printf, scanf, exit\n");
    fprintf(assemblyFile,"section .data\n");
     for(int i=0;i<global->no_slots;i++){
        SymbolTableRecord * entry = global->list[i]->head;
        while(entry!=NULL){
            if(entry->type == INT){
                fprintf(assemblyFile, "%s: dw 0\n",entry->lexeme);
            }
            else if (entry->type == REAL){
                fprintf(assemblyFile, "%s: dd 0.0", entry->lexeme);
            }
            entry = entry->next;

        }
    // fprintf(assemblyFile, "\t\tffour_reg_fmt: db `RAX = %%ld, RBX = %%ld, RCX = %%ld, RDX = %%ld`, 10, 0\n");
    // fprintf(assemblyFile, "\t\tlfour_reg_fmt: db `RSP = %%ld, RBP = %%ld, RSI = %%ld, RDI = %%ld\\n`, 10, 0\n");
    fprintf(assemblyFile, "\t\tint_fmt: db \"%d\", 0\n");
    fprintf(assemblyFile, "\t\treal_fmt: db \"%lf\", 0\n");
    // fprintf(assemblyFile, "\t\tbool_fmt: db \"%%d\", 0\n");
}