#include "codegen.h"
#include <string.h>
#define MAX_LEXEME_LEN 32
stack* st;
FILE * assemblyFile;
#include <stdio.h>
#include<stdlib.h>
symbolTable * GLOBAL;
lexeme_decomp *get_lexdecomp_struct(char *lex)
{
    lexeme_decomp *new_temp = (lexeme_decomp *)malloc(sizeof(lexeme_decomp));
    new_temp->lex = lex;
    new_temp->info=(lex_info*)malloc(sizeof(lex_info));
    new_temp->info->offset=-1;
    new_temp->info->width=-1;
    new_temp->info->type=UNDEFINED;
    new_temp->info->type_ruid=NULL;
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
lex_info* get_lexinfo(char *lex, struct symbolTable *local_table, struct symbolTable *global_table)
{  
    //returns lexInfo with offset -1 if lexeme is not in local table
    // get a lexeme for a identitfier/temporary and find its offset.
    //lexeme can be of form t1.a.b.c where t1 is of type record

    lexeme_decomp *lexList = decompose_lexeme(lex);

    SymbolTableRecord *sym_info = getSymbolInfo(lexList->lex, local_table);
    if (sym_info == NULL)
    {
        // not a local variable
        //variable is a global variable if returned info->offfset==-1
        return lexList->info;
    }
    lexList->info->offset = sym_info->offset;
    lexList->info->type=sym_info->type;
    lexList->info->type_ruid=sym_info->type_ruid;
    lexList->info->width=sym_info->width;

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
        offset += ptr->info->offset;
        ptr = ptr->next;
    }
    //fetch the last lex_info 
    ptr=lexList;
    while(ptr->next){
        ptr=ptr->next;
    }
    lex_info* to_return=ptr->info;
    to_return->offset=offset;
    return to_return;
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
            lexList->info->offset = fields->offset;
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
    //sym_info now contains the symbol table record for the RECORD/UNION/VARIANT RECORD of the field whose offset we just set
    //extract useful info from this symbol table record now
    if (fields->type == RUID || fields->type == RECORD || fields->type == VARIANTRECORD || fields->type == UNION){
        lexList->info->type=sym_info->type;
        lexList->info->type_ruid=sym_info->type_ruid;
        lexList->info->width=sym_info->width;
    }
    else{
        if(fields->type==INT){
            lexList->info->type=INT;
            lexList->info->type_ruid=NULL;
            lexList->info->width=2;
        }
        else if(fields->type==REAL){
            lexList->info->type=REAL;
            lexList->info->type_ruid=NULL;
            lexList->info->width=4;
        }
    }    

    //get information about further expansions
    get_offset_util(lexList->next, sym_info, local_table, global_table);
}


Type getNumType(char *str){
    int len = strlen(str);
    for(int i=0; i<len; i++){
        if(str[i] == '.')
            return REAL;
    }
    return INT;
}
// void arithexpr_code_gen(tuple* tuple){
//     /**
//      * @brief Check the operator and generate code
//      */
    
//     /* 
//         Integers are returned in rax or rdx:rax, and floating point values are returned in xmm0 or xmm1:xmm0
//         Note: The floating point instructions have an sd suffix (i.e. convert single s to double precision d)
//     */

//     SymbolTableRecord * res = getSymbolInfo(tuple->arg1, (symbolTable *)st->head->t_node);
//     SymbolTableRecord * operand1 = getSymbolInfo(tuple->arg2, (symbolTable *)st->head->t_node);
//     SymbolTableRecord * operand2 = getSymbolInfo(tuple->arg3, (symbolTable *)st->head->t_node);
//     char *arg1_str, *arg2_str, *res_str;
    
//     arg1_str = (char*)malloc(sizeof(char) * MAX_LEXEME_LEN);
//     arg2_str = (char*)malloc(sizeof(char) * MAX_LEXEME_LEN);
    
    
    
//     // fprintf(assemblyFile, "\t\t\t\tpush_all\n");

//     if(res->type == INT)
       
//     {

//          if(res){ //found in local table//
//             snprintf(res_str, MAX_LEXEME_LEN, "word [EBP - %d]", res->offset);
//         }
//         else{ //found in global table//
//             strncpy(res_str, res->lexeme, MAX_LEXEME_LEN);
//         }
        
//         if(operand1){ //found in local table//
//             snprintf(arg1_str, MAX_LEXEME_LEN, "word [EBP - %d]", operand1->offset);
//         }
//         else{ //found in global table or immediate value//
//             strncpy(arg1_str, operand1->lexeme, MAX_LEXEME_LEN);
//         }
//         if(operand2){ //found in local table//
//             snprintf(arg2_str, MAX_LEXEME_LEN, "word [EBP - %d]", operand2->offset);
//         }
//         else{ //found in global table or immediate value//
//             strncpy(arg2_str, operand2->lexeme, MAX_LEXEME_LEN);
//         }

//         switch(tuple->op)
//         {
//             case PLUS: 
//             // fprintf(assemblyFile, "\t\t\t\t;Addition of integers\n");
//             fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
//                 mov BX, %s\n\
//                 add AX, BX \n\
//                 mov %s, AX \n", arg1_str, arg2_str, res_str);
//             break;

//             case MINUS: 
//             fprintf(assemblyFile, "\t\t\t\t;Subtraction of integers\n");
//             fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
//                 mov BX, %s\n\
//                 sub AX, BX \n\
//                 mov %s, AX \n", arg1_str, arg2_str, res_str);
//             break;
            
//             case MUL: 
//             fprintf(assemblyFile, "\t\t\t\t;Multiplication of integers\n");
//             fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
//                 mov BX, %s\n\
//                 mul BX \n\
//                 mov %s, AX \n", arg1_str, arg2_str, res_str);
//             break;

//             case DIV: 
//             fprintf(assemblyFile, "\t\t\t\t;Division of integers\n");
//             fprintf(assemblyFile, "\t\t\t\tmov DX, 0\n\
//                 mov AX, %s\n\
//                 mov BX, %s\n\
//                 div BX \n\
//                 mov %s, AX \n", arg1_str, arg2_str, res_str);
//             break;
            
//         }
//     }
//     // else if(res->type == REAL)
//     // {
//     //     if(!arg1_type_ptr)  // immediate value
//     //     {
//     //         char *real1_str = newlabel();
//     //         fprintf(assemblyFile,"\t\t\t\tsection .data\n");
//     //         fprintf(assemblyFile,"\t\t\t\t%s: dq %s\n", real1_str, arg1_str);
//     //         fprintf(assemblyFile,"\t\t\t\tsection .text\n");
//     //         fprintf(assemblyFile, "\t\t\t\tmovsd XMM0, [%s]\n", real1_str);            
//     //     }
//     //     else
//     //         fprintf(assemblyFile, "\t\t\t\tmovsd XMM0, %s\n", arg1_str);

//     //     if(!arg2_type_ptr)  // immediate value
//     //     {
//     //         char *real2_str = newlabel();
//     //         fprintf(assemblyFile,"\t\t\t\tsection .data\n");
//     //         fprintf(assemblyFile,"\t\t\t\t%s: dq %s\n", real2_str, arg2_str);
//     //         fprintf(assemblyFile,"\t\t\t\tsection .text\n");
//     //         fprintf(assemblyFile, "\t\t\t\tmovsd XMM2, [%s]\n", real2_str);
//     //     }
//     //     else
//     //         fprintf(assemblyFile, "\t\t\t\tmovsd XMM2, %s\n", arg2_str);
        
//     //     switch(quad.op)
//     //     {
//     //         case PLUS_OP: 
//     //         fprintf(assemblyFile, "\t\t\t\t;Addition of reals\n");
//     //         fprintf(assemblyFile,"\t\t\t\taddsd XMM0, XMM2 \n\
//     //             movsd [EBP - %d], XMM0 \n",offset_result);
//     //         break;

//     //         case MINUS_OP: 
//     //         fprintf(assemblyFile, "\t\t\t\t;Subtraction of reals\n");
//     //         fprintf(assemblyFile,"\t\t\t\tsubsd XMM0, XMM2 \n\
//     //             movsd [EBP - %d], XMM0 \n", offset_result);
//     //         break;

//     //         case MUL_OP: 
//     //         fprintf(assemblyFile, "\t\t\t\t;Multiplication of reals\n");
//     //         fprintf(assemblyFile,"\t\t\t\tmulsd XMM0, XMM2 \n\
//     //             movsd [EBP - %d], XMM0 \n",offset_result);
//     //         break;

//     //         case DIV_OP: 
//     //         fprintf(assemblyFile, "\t\t\t\t;Division of reals\n");
//     //         fprintf(assemblyFile,"\t\t\t\tdivsd XMM0, XMM2 \n\
//     //             movsd [EBP - %d], XMM0 \n", offset_result);
//     //         break;
            
//     //     }
//     // }

//     fprintf(assemblyFile, "\t\t\t\tpop_all\n");
// }



void one_var_output_code_gen(Type type, int offset){
    switch(type){
        case INT:
        {
            fprintf(assemblyFile, "\
                mov EDX, EBP\n\
                sub EDX, %d     ; make EDX to point at location of variable on the stack\n\
                push word [EDX]  ; for integer, value stored at offset should be passed to printf\n\
                push word [zero] ;\n\
                push dword int_fmt\n\
                call printf \n\
                add ESP, 8\
                ", offset);
        }
        break;
        case REAL:
        {
            fprintf(assemblyFile, "\
                mov EDX, EBP\n\
                sub EDX, %d     ; make EDX to point at location of variable on the stack\n\
                fld	dword [EDX]	            ; need to convert 32-bit to 64-bit \n\
	            fstp	qword [flttmp]      ; floating load makes 80-bit,\n\
	                                        ; store as 64-bit \n\
                push dword [flttmp+4]       \n\
                push dword [flttmp]         \n\
                push dword real_fmt \n\
                call printf \n\
                add ESP,12 \
                ", offset);
        }
        break;
    }
}

void one_var_output_code_gen_global(Type type, char * name){
    switch(type){
        case INT:
        {
            fprintf(assemblyFile, "\
                push word [%s]  ; for integer, value stored at offset should be passed to printf\n\
                push word [zero] ;\n\
                push dword int_fmt\n\
                call printf \n\
                add ESP, 8\
                ", name);
        }
        break;
        case REAL:
        {
            fprintf(assemblyFile, "\
                fld	dword [%s]	            ; need to convert 32-bit to 64-bit \n\
	            fstp	qword [flttmp]      ; floating load makes 80-bit,\n\
	                                        ; store as 64-bit \n\
                push dword [flttmp+4]       \n\
                push dword [flttmp]         \n\
                push dword real_fmt \n\
                call printf \n\
                add ESP,12 \
                ", name);
        }
        break;
    }
}

void output_code_gen(tuple* intermediateCode){
    SymbolTableRecord * entry = getSymbolInfo(intermediateCode->arg1, (symbolTable *)st->head->t_node);
    if(entry==NULL){
        entry = getSymbolInfo(intermediateCode->arg1, GLOBAL);
        one_var_output_code_gen_global(entry->type,entry->lexeme);
    }
    else {
            Type type = entry->type;
        // if(var_type_ptr == NULL){       // if a value has to be printed
        //     fprintf(assemblyFile, "\t\t\t\tprint_str \"%s\"\n", quad.arg1);
        //     return;
        // }
        // print_a_type(var_type_ptr);
        int offset = entry->offset;
        int width;
        // fprintf(assemblyFile, "\t\t\t\t; Code to output value(s) of %s\n", quad.arg1);
        // fprintf(assemblyFile, "\t\t\t\tprint_str \"\"\n");    
        // fprintf(assemblyFile, "\t\t\t\tprint_str_no_new_line \"Output: \"\n");    
        // fprintf(assemblyFile, "\t\t\t\tpush_all\n");    
        switch(type){
            case INT:
            {
                one_var_output_code_gen(INT, offset);
            }
            break;
            case REAL:
            {
                one_var_output_code_gen(REAL, offset);
            }
            break;
            default:
            {
                fprintf(assemblyFile, "\t\t\t\t		;type error, cannot take input \"%%d\" \n");
            }
            break;
        }    
        // fprintf(assemblyFile, "\t\t\t\tpop_all\n");
        // fprintf(assemblyFile, "\t\t\t\tprint_str \"\" ;print a newline after output\n");
        
        
        /** 
         * ToDo:
         * There are unmatching push and pops coming in the generated code
         */
    }
   
}

// void param_code_gen(tuple* intermediateCode){
    
//     SymbolTableRecord * entry = getSymbolInfo(intermediateCode->arg1, (symbolTable *)st->head->t_node);
//     Type type = entry->type;

//     // int offset_arg1 = 0;
//     int offset_arg1 = entry->offset;
//     // fprintf(assemblyFile, "\t\t\t\t;pushing a parameter(%s) on stack\n", intermediateCode->arg1);
//     if(type==INT){
//         fprintf(assemblyFile, "\t\t\t\tmov EAX, EBP\n");
//         fprintf(assemblyFile, "\t\t\t\tsub EAX, %d\n", offset_arg1);
//         fprintf(assemblyFile, "\t\t\t\tpush word [EAX]\n");
//     }
//     else if (type == REAL){
//         fprintf(assemblyFile, "\t\t\t\tmov EAX, EBP\n");
//         fprintf(assemblyFile, "\t\t\t\tsub EAX, %d\n", offset_arg1);
//         fprintf(assemblyFile, "\t\t\t\tpush dword [EAX]\n");
//     }
    
//     //do for record
// }

void relational_code_gen(tuple *intermediateCode, struct symbolTable *local, struct symbolTable *global)
{

    lex_info *arg1_info = get_lexinfo(intermediateCode->arg1, local, global);
    lex_info *arg2_info = get_lexinfo(intermediateCode->arg2, local, global);
    lex_info *arg3_info = get_lexinfo(intermediateCode->arg3, local, global);

    char *arg1_str = (char *)malloc(MAX_LEXEME_LEN);
    char *arg2_str = (char *)malloc(MAX_LEXEME_LEN);
    char *arg3_str = (char *)malloc(MAX_LEXEME_LEN);

    // initializing the locations from where the values are to be picked up from / be stored
    if (arg1_info->offset == -1)
    {
        // arg1 corresponds to a global variable
        snprintf(arg1_str, MAX_LEXEME_LEN, "word[%s]", intermediateCode->arg1); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg1_str, MAX_LEXEME_LEN, "word[EBP-%d]", arg1_info->offset); // access the variable value from offset
    }

    if (arg2_info->offset == -1)
    {
        // arg2 corresponds to a global variable
        snprintf(arg2_str, MAX_LEXEME_LEN, "word[%s]", intermediateCode->arg2); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg2_str, MAX_LEXEME_LEN, "word[EBP-%d]", arg2_info->offset); // access the variable value from offset
    }

    if (arg3_info->offset == -1)
    {
        // arg3 corresponds to a global variable
        snprintf(arg3_str, MAX_LEXEME_LEN, "%s", intermediateCode->arg3); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg3_str, MAX_LEXEME_LEN, "[EBP-%d]", arg3_info->offset); // access the variable value from offset
        // do not need keyword "word as we are moving to this destination"
    }

    // we need to store 0 in ECX initially and 1 in EDX so we can set ECX if condition holds
    fprintf(assemblyFile, "MOV ECX, 0\n\
        MOV EDX,1");

    if (arg1_info->type == INT)
    {
        // comparisons of 2 integers

        switch (intermediateCode->op)
        {

        case LT:
            fprintf(assemblyFile, "MOV EAX %s \n\
            MOV EBX, %s \n\
            CMP EAX, EBX\n\
            CMOVL ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case LE:
            fprintf(assemblyFile, "MOV EAX %s \n\
            MOV EBX, %s \n\
            CMP EAX, EBX\n\
            CMOVLE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case GT:
            fprintf(assemblyFile, "MOV EAX %s \n\
            MOV EBX, %s \n\
            CMP EAX, EBX\n\
            CMOVG ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case GE:
            fprintf(assemblyFile, "MOV EAX %s \n\
            MOV EBX, %s \n\
            CMP EAX, EBX\n\
            CMOVGE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case EQ:
            fprintf(assemblyFile, "MOV EAX %s \n\
            MOV EBX, %s \n\
            CMP EAX, EBX\n\
            CMOVE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case NE:
            fprintf(assemblyFile, "MOV EAX %s \n\
            MOV EBX, %s \n\
            CMP EAX, EBX\n\
            CMOVNE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        }
    }
    else if (arg1_info->type == REAL)
    {
        // comparisons of 2 REALS
         switch (intermediateCode->op)
        {

        case LT:
            fprintf(assemblyFile, "MOV XMM0 %s \n\
            MOV XMM1, %s \n\
            CMP XMM0, XMM1\n\
            CMOVL ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case LE:
            fprintf(assemblyFile, "MOV XMM0 %s \n\
            MOV XMM1, %s \n\
            CMP XMM0, XMM1\n\
            CMOVLE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case GT:
            fprintf(assemblyFile, "MOV XMM0 %s \n\
            MOV XMM1, %s \n\
            CMP XMM0, XMM1\n\
            CMOVG ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case GE:
            fprintf(assemblyFile, "MOV XMM0 %s \n\
            MOV XMM1, %s \n\
            CMP XMM0, XMM1\n\
            CMOVGE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case EQ:
            fprintf(assemblyFile, "MOV XMM0 %s \n\
            MOV XMM1, %s \n\
            CMP XMM0, XMM1\n\
            CMOVE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        case NE:
            fprintf(assemblyFile, "MOV XMM0 %s \n\
            MOV XMM1, %s \n\
            CMP XMM0, XMM1\n\
            CMOVNE ECX, EDX\n\
            MOV %s, ECX\n",
                    arg1_str, arg2_str, arg3_str);
            break;
        }
    }
}

//check argument numbers for next 3 functions 
void if_code_gen(tuple *intermediateCode, struct symbolTable *local, struct symbolTable *global){
    //jump if false
    lex_info *arg1_info = get_lexinfo(intermediateCode->arg1, local, global);
    //arg3 is a label

    char *arg1_str = (char *)malloc(MAX_LEXEME_LEN);
    char *label_str = (char *)malloc(MAX_LEXEME_LEN);
    strncpy(label_str,intermediateCode->arg3,MAX_LEXEME_LEN);
    // initializing the locations from where the values are to be picked up from / be stored
    if (arg1_info->offset == -1)
    {   
        //this branch will never be executed because temporaries are not stored in global table
        // arg1 corresponds to a global variable
        snprintf(arg1_str, MAX_LEXEME_LEN, "word[%s]", intermediateCode->arg1); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg1_str, MAX_LEXEME_LEN, "word[EBP-%d]", arg1_info->offset); // access the variable value from offset
    }
    //move value of boolean to register eax if value is 0 => bool expr was false so jump to label
    fprintf(assemblyFile,"MOV EAX, %s\n",arg1_str);
    fprintf(assemblyFile,"JZ %s\n",label_str);
}

void label_code_gen(tuple *intermediateCode, struct symbolTable *local, struct symbolTable *global){
    //place a label in the code
    fprintf(assemblyFile,"%s :\n",intermediateCode->arg1);
}

void goto_code_gen(tuple *intermediateCode, struct symbolTable *local, struct symbolTable *global){
    //jump to a label
    fprintf(assemblyFile,"GOTO %s\n",intermediateCode->arg1);
}

void arithexpr_code_gen(tuple* intermediateCode,struct symbolTable* local, struct symbolTable* global){

    /* 
        Integers are returned in rax or rdx:rax, and floating point values are returned in xmm0 or xmm1:xmm0
        Note: The floating point instructions have an sd suffix (i.e. convert single s to double precision d)
    */
    lex_info *arg1_info = get_lexinfo(intermediateCode->arg1, local, global);
    lex_info *arg2_info = get_lexinfo(intermediateCode->arg2, local, global);
    lex_info *arg3_info = get_lexinfo(intermediateCode->arg3, local, global);

    char *arg1_str = (char *)malloc(MAX_LEXEME_LEN);
    char *arg2_str = (char *)malloc(MAX_LEXEME_LEN);
    char *arg3_str = (char *)malloc(MAX_LEXEME_LEN);

    // initializing the locations from where the values are to be picked up from / be stored
    if (arg1_info->offset == -1)
    {
        // arg1 corresponds to a global variable
        snprintf(arg1_str, MAX_LEXEME_LEN, "word[%s]", intermediateCode->arg1); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg1_str, MAX_LEXEME_LEN, "word[EBP-%d]", arg1_info->offset); // access the variable value from offset
    }

    if (arg2_info->offset == -1)
    {
        // arg2 corresponds to a global variable
        snprintf(arg2_str, MAX_LEXEME_LEN, "word[%s]", intermediateCode->arg2); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg2_str, MAX_LEXEME_LEN, "word[EBP-%d]", arg2_info->offset); // access the variable value from offset
    }

    if (arg3_info->offset == -1)
    {
        // arg3 corresponds to a global variable
        snprintf(arg3_str, MAX_LEXEME_LEN, "%s", intermediateCode->arg3); // directly use the global variable lex as it will have a label associated with it
    }
    else
    {
        snprintf(arg3_str, MAX_LEXEME_LEN, "[EBP-%d]", arg3_info->offset); // access the variable value from offset
        // do not need keyword "word as we are moving to this destination"
    }
    // SymbolTableRecord * res = getSymbolInfo(tuple->arg1, (symbolTable *)st->head->t_node);
    // SymbolTableRecord * operand1 = getSymbolInfo(tuple->arg2, (symbolTable *)st->head->t_node);
    // SymbolTableRecord * operand2 = getSymbolInfo(tuple->arg3, (symbolTable *)st->head->t_node);   
    
    
    // fprintf(assemblyFile, "\t\t\t\tpush_all\n");
    if(intermediateCode->op==DIV){
        //code for handling division of two numbers
        // case DIV: 
        //     fprintf(assemblyFile, "\t\t\t\t;Division of integers\n");
        //     fprintf(assemblyFile, "\t\t\t\tmov DX, 0\n\
        //         mov AX, %s\n\
        //         mov BX, %s\n\
        //         div BX \n\
        //         mov %s, AX \n", arg1_str, arg2_str, res_str);
        //     break;

        // case DIV_OP: 
        //     fprintf(assemblyFile, "\t\t\t\t;Division of reals\n");
        //     fprintf(assemblyFile,"\t\t\t\tdivsd XMM0, XMM2 \n\
        //         movsd [EBP - %d], XMM0 \n", offset_result);
        //     break;
    }
    else if(arg1_info->type == INT)  
    {   //handle * - + on INT

        switch(intermediateCode->op)
        {
            case PLUS: 
            // fprintf(assemblyFile, "\t\t\t\t;Addition of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
                mov BX, %s\n\
                add AX, BX \n\
                mov %s, AX \n", arg1_str, arg2_str, arg3_str);
            break;

            case MINUS: 
            // fprintf(assemblyFile, "\t\t\t\t;Subtraction of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
                mov BX, %s\n\
                sub AX, BX \n\
                mov %s, AX \n", arg1_str, arg2_str, arg3_str);
            break;
            
            case MUL: 
            // fprintf(assemblyFile, "\t\t\t\t;Multiplication of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
                mov BX, %s\n\
                mul BX \n\
                mov %s, AX \n", arg1_str, arg2_str, arg3_str);
            break;
            
        }
    }
}

void assignment_stmt(tuple *intermediateCode){
    SymbolTableRecord * entry = getSymbolInfo(intermediateCode->arg3, (symbolTable *)st->head->t_node);
    if(entry==NULL){
        entry = getSymbolInfo(intermediateCode->arg1, GLOBAL);
        if(intermediateCode->arg1[0] > '0' && intermediateCode->arg1[0] < '9'){ //handle immediate value
            Type t = getNumType(intermediateCode->arg1);
            switch(t){
                case INT:
                    fprintf(assemblyFile, "\t\t\
                    mov word [%s], %s \n\
                    ", intermediateCode->arg3, intermediateCode->arg1);
                    break;
                case REAL:
                    fprintf(assemblyFile, "\
                    mov dword [%s], __?float32?__ (%s) \n\
                    ", intermediateCode->arg3, intermediateCode->arg1);
                    break;
            }
        }
    }
    else {//do nothing for now change later
          if(intermediateCode->arg1[0] > '0' && intermediateCode->arg1[0] < '9'){ //handle immediate value
            Type t = getNumType(intermediateCode->arg1);
            switch(t){
                case INT:
                    fprintf(assemblyFile, "\t\t\
                    mov EDX, EBP \n\
                    sub EDX, %d \n\
                    mov word [EDX], %s \n\
                    ", entry->offset , intermediateCode->arg1);
                    break;
                case REAL:
                    fprintf(assemblyFile, "\
                    mov EDX, EBP \n\
                    sub EDX, %d\n\
                    mov dword [EDX], __?float32?__ (%s) \n\
                    ", entry->offset, intermediateCode->arg1);
                    break;
            }
        }
    }
}
void generate_code(tupleList* intermediateCode, symbolTable * global, FILE * output){
    assemblyFile = output;
    GLOBAL = global;
    st = getStack();
    SymbolTableRecord* MAIN_ENTRY = getSymbolInfo("_main",global);
    push_stack_node(st,get_stack_node((tNode*)MAIN_ENTRY->functionTable));
    fprintf(assemblyFile, "extern printf, scanf, exit\n");
    fprintf(assemblyFile,"section .data\n");
     for(int i=0;i<global->no_slots;i++){
        SymbolTableRecord * entry = global->list[i]->head;
        while(entry!=NULL){
            if(entry->type == INT){
                fprintf(assemblyFile, "\t\t%s: dw 0\n",entry->lexeme);
            }
            else if (entry->type == REAL){
                fprintf(assemblyFile, "\t\t%s: dd 0.0\n", entry->lexeme);
            }
            //add for record later
            entry = entry->next;

        }
     }
    // fprintf(assemblyFile, "\t\tffour_reg_fmt: db `EAX = %%ld, EBX = %%ld, ECX = %%ld, EDX = %%ld`, 10, 0\n");
    // fprintf(assemblyFile, "\t\tlfour_reg_fmt: db `ESP = %%ld, EBP = %%ld, ESI = %%ld, EDI = %%ld\\n`, 10, 0\n");
    fprintf(assemblyFile, "\t\tint_fmt: db \"%%d\", 10, 0\n");
    fprintf(assemblyFile, "\t\treal_fmt: db \"%%lf\", 10, 0\n");
    fprintf(assemblyFile, "\t\tzero: dw 0\n");
    fprintf(assemblyFile, "\t\tflttmp: dq 0.0\n");
    fprintf(assemblyFile, "section .text\n");
    fprintf(assemblyFile, "global main\n");
    fprintf(assemblyFile, "main:\n");
    tuple * tup = intermediateCode->head;
    while(tup->next!=NULL){
        switch(tup->op){
            case WRITE:
                output_code_gen(tup);
            break;
        }
        tup = tup->next;
    }
    // fprintf(assemblyFile, "\t\tbool_fmt: db \"%%d\", 0\n");
}