#include "codegen.h"

lexeme_decomp *get_lexdecomp_struct(char *lex)
{
    lexeme_decomp *new_temp = (lexeme_decomp *)malloc(sizeof(lexeme_decomp));
    new_temp->lex = lex;
    new_temp->offset = -1;
    new_temp->next = NULL;
    return new_temp;
}

lexeme_decomp *decompose_lexeme(char *lex)
{
    char *lex_copy = (char *)malloc(strlen(lex));
    lex_copy = strcpy(lex_copy, lex);
    lexeme_decomp *head = get_lexdecomp_struct(lex_copy);
    lexeme_decomp *temptr = head;

    char *lexptr = lex_copy;
    while (*lexptr != '\0')
    {
        if (*lexptr = '.')
        {
            *lexptr = '\0';
            temptr->next = get_lexdecomp_struct(lexptr + 1);
        }
        lexptr = lexptr + 1;
    }
    return head;
}
int get_offset(lexeme_decomp *lex, struct symbolTable *local_table, struct symbolTable *global_table)
{
    // get a lexeme for a identitfier/temporary and find its offset.
    lexeme_decomp *lexList = decompose_lexeme(lex);

    SymbolTableRecord *sym_info = getSymbolInfo(lexList->lex, local_table);
    if (sym_info == NULL)
    {
        // not a local variable
        // return;
    }
    lexList->offset = sym_info->offset;

    if (sym_info->type == RUID || sym_info->type == RECORD || sym_info->type == VARIANTRECORD || sym_info->type == UNION)
    {
        sym_info = getSymbolInfo(sym_info->type_ruid, global_table);
    }
    while ((sym_info->type == RUID))
    {
        sym_info = getSymbolInfo(sym_info->type_ruid, global_table);
    }

    get_offset_util(lexList->next, sym_info, local_table, global_table);

    int offset = 0;
    lexeme_decomp *ptr = lexList;
    while (ptr)
    {
        offset += ptr->offset;
        ptr = ptr->next;
    }
    return offset;
}

void get_offset_util(lexeme_decomp *lexList, SymbolTableRecord *sym_info, struct symbolTable *local_table, struct symbolTable *global_table)
{
    if (lexList == NULL)
        return;
    struct record_field *fields = sym_info->recordFields;
    while (fields)
    {
        if (strcmp(fields->lexeme, lexList->lex) == 0)
        {
            lexList->offset = fields->offset;
            break;
        }
    }
    if (fields == NULL)
    {
        // shouldnt reach here ever after type checking.
        printf("fields are null");
        return;
    }
    if (fields->type == RUID || fields->type == RECORD || fields->type == VARIANTRECORD || fields->type == UNION)
    {
        {
            sym_info = getSymbolInfo(fields->ruid, global_table);
            while ((sym_info->type == RUID))
            {
                sym_info = getSymbolInfo(sym_info->type_ruid, global_table);
            }
        }
    }
    get_offset_util(lexList->next, sym_info, local_table, global_table);
}

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