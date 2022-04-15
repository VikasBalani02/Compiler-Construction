#include "codegen.h"
#include <string.h>
#define MAX_LEXEME_LEN 32
stack* st;
FILE * assemblyFile;


Type getNumType(char *str){
    int len = strlen(str);
    for(int i=0; i<len; i++){
        if(str[i] == '.')
            return REAL;
    }
    return INT;
}
void arithexpr_code_gen(tuple* tuple){
    /**
     * @brief Check the operator and generate code
     */
    
    /* 
        Integers are returned in rax or rdx:rax, and floating point values are returned in xmm0 or xmm1:xmm0
        Note: The floating point instructions have an sd suffix (i.e. convert single s to double precision d)
    */

    SymbolTableRecord * res = getSymbolInfo(tuple->arg1, (symbolTable *)st->head->t_node);
    SymbolTableRecord * operand1 = getSymbolInfo(tuple->arg2, (symbolTable *)st->head->t_node);
    SymbolTableRecord * operand2 = getSymbolInfo(tuple->arg3, (symbolTable *)st->head->t_node);
    char *arg1_str, *arg2_str, *res_str;
    
    arg1_str = (char*)malloc(sizeof(char) * MAX_LEXEME_LEN);
    arg2_str = (char*)malloc(sizeof(char) * MAX_LEXEME_LEN);
    
    
    
    // fprintf(assemblyFile, "\t\t\t\tpush_all\n");

    if(res->type == INT)
       
    {

         if(res){ //found in local table//
            snprintf(res_str, MAX_LEXEME_LEN, "word [EBP - %d]", res->offset);
        }
        else{ //found in global table//
            strncpy(res_str, res->lexeme, MAX_LEXEME_LEN);
        }
        
        if(operand1){ //found in local table//
            snprintf(arg1_str, MAX_LEXEME_LEN, "word [EBP - %d]", operand1->offset);
        }
        else{ //found in global table or immediate value//
            strncpy(arg1_str, operand1->lexeme, MAX_LEXEME_LEN);
        }
        if(operand2){ //found in local table//
            snprintf(arg2_str, MAX_LEXEME_LEN, "word [EBP - %d]", operand2->offset);
        }
        else{ //found in global table or immediate value//
            strncpy(arg2_str, operand2->lexeme, MAX_LEXEME_LEN);
        }

        switch(tuple->op)
        {
            case PLUS: 
            // fprintf(assemblyFile, "\t\t\t\t;Addition of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
                mov BX, %s\n\
                add AX, BX \n\
                mov %s, AX \n", arg1_str, arg2_str, res_str);
            break;

            case MINUS: 
            fprintf(assemblyFile, "\t\t\t\t;Subtraction of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
                mov BX, %s\n\
                sub AX, BX \n\
                mov %s, AX \n", arg1_str, arg2_str, res_str);
            break;
            
            case MUL: 
            fprintf(assemblyFile, "\t\t\t\t;Multiplication of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov AX, %s\n\
                mov BX, %s\n\
                mul BX \n\
                mov %s, AX \n", arg1_str, arg2_str, res_str);
            break;

            case DIV: 
            fprintf(assemblyFile, "\t\t\t\t;Division of integers\n");
            fprintf(assemblyFile, "\t\t\t\tmov DX, 0\n\
                mov AX, %s\n\
                mov BX, %s\n\
                div BX \n\
                mov %s, AX \n", arg1_str, arg2_str, res_str);
            break;
            
        }
    }
    else if(res->type == REAL)
    {
        if(!arg1_type_ptr)  // immediate value
        {
            char *real1_str = newlabel();
            fprintf(assemblyFile,"\t\t\t\tsection .data\n");
            fprintf(assemblyFile,"\t\t\t\t%s: dq %s\n", real1_str, arg1_str);
            fprintf(assemblyFile,"\t\t\t\tsection .text\n");
            fprintf(assemblyFile, "\t\t\t\tmovsd XMM0, [%s]\n", real1_str);            
        }
        else
            fprintf(assemblyFile, "\t\t\t\tmovsd XMM0, %s\n", arg1_str);

        if(!arg2_type_ptr)  // immediate value
        {
            char *real2_str = newlabel();
            fprintf(assemblyFile,"\t\t\t\tsection .data\n");
            fprintf(assemblyFile,"\t\t\t\t%s: dq %s\n", real2_str, arg2_str);
            fprintf(assemblyFile,"\t\t\t\tsection .text\n");
            fprintf(assemblyFile, "\t\t\t\tmovsd XMM2, [%s]\n", real2_str);
        }
        else
            fprintf(assemblyFile, "\t\t\t\tmovsd XMM2, %s\n", arg2_str);
        
        switch(quad.op)
        {
            case PLUS_OP: 
            fprintf(assemblyFile, "\t\t\t\t;Addition of reals\n");
            fprintf(assemblyFile,"\t\t\t\taddsd XMM0, XMM2 \n\
                movsd [EBP - %d], XMM0 \n",offset_result);
            break;

            case MINUS_OP: 
            fprintf(assemblyFile, "\t\t\t\t;Subtraction of reals\n");
            fprintf(assemblyFile,"\t\t\t\tsubsd XMM0, XMM2 \n\
                movsd [EBP - %d], XMM0 \n", offset_result);
            break;

            case MUL_OP: 
            fprintf(assemblyFile, "\t\t\t\t;Multiplication of reals\n");
            fprintf(assemblyFile,"\t\t\t\tmulsd XMM0, XMM2 \n\
                movsd [EBP - %d], XMM0 \n",offset_result);
            break;

            case DIV_OP: 
            fprintf(assemblyFile, "\t\t\t\t;Division of reals\n");
            fprintf(assemblyFile,"\t\t\t\tdivsd XMM0, XMM2 \n\
                movsd [EBP - %d], XMM0 \n", offset_result);
            break;
            
        }
    }

    fprintf(assemblyFile, "\t\t\t\tpop_all\n");
}



void one_var_output_code_gen(Type type, int offset){
    switch(type){
        case INT:
        {
            fprintf(assemblyFile, "\t\t\t\t\
                mov EDX, EBP\n\
                sub EDX, %d     ; make EDX to point at location of variable on the stack\n\
                push word [EDX]  ; for integer, value stored at offset should be passed to printf\n\
                push word [zero] ;\
                push dword int_fmt\
                call printf \n\
                ", offset);
        }
        break;
        case REAL:
        {
            fprintf(assemblyFile, "\t\t\t\t\
                mov EDX, EBP\n\
                sub EDX, %d     ; make EDX to point at location of variable on the stack\n\
                fld	dword [EDX]	            ; need to convert 32-bit to 64-bit \
	            fstp	qword [flttmp]      ; floating load makes 80-bit,\
	                                        ; store as 64-bit \
                push dword [flttmp+4]       \
                push dword [flttmp]         \
                push dword real_fmt \
                call printf \n\
                ", offset);
        }
        break;
    }
}

void output_code_gen(tuple* intermediateCode){
    SymbolTableRecord * entry = getSymbolInfo(intermediateCode->arg1, (symbolTable *)st->head->t_node);
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

void param_code_gen(tuple* intermediateCode){
    
    SymbolTableRecord * entry = getSymbolInfo(intermediateCode->arg1, (symbolTable *)st->head->t_node);
    Type type = entry->type;

    // int offset_arg1 = 0;
    int offset_arg1 = entry->offset;
    // fprintf(assemblyFile, "\t\t\t\t;pushing a parameter(%s) on stack\n", intermediateCode->arg1);
    if(type==INT){
        fprintf(assemblyFile, "\t\t\t\tmov EAX, EBP\n");
        fprintf(assemblyFile, "\t\t\t\tsub EAX, %d\n", offset_arg1);
        fprintf(assemblyFile, "\t\t\t\tpush word [EAX]\n");
    }
    else if (type == REAL){
        fprintf(assemblyFile, "\t\t\t\tmov EAX, EBP\n");
        fprintf(assemblyFile, "\t\t\t\tsub EAX, %d\n", offset_arg1);
        fprintf(assemblyFile, "\t\t\t\tpush dword [EAX]\n");
    }
    
    //do for record
}


void generate_code(tupleList* intermediateCode, symbolTable * global, FILE * output){
    assemblyFile = output;
    st = getStack();
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
            //add for record later
            entry = entry->next;

        }
     }
    // fprintf(assemblyFile, "\t\tffour_reg_fmt: db `EAX = %%ld, EBX = %%ld, ECX = %%ld, EDX = %%ld`, 10, 0\n");
    // fprintf(assemblyFile, "\t\tlfour_reg_fmt: db `ESP = %%ld, EBP = %%ld, ESI = %%ld, EDI = %%ld\\n`, 10, 0\n");
    fprintf(assemblyFile, "\t\tint_fmt: db \"%%d\", 0\n");
    fprintf(assemblyFile, "\t\treal_fmt: db \"%%lf\", 0\n");
    fprintf(assemblyFile, "\t\tzero: dw 0\n");
    fprintf(assemblyFile, "\t\tflt_temp: dq 0.0\n");
    // fprintf(assemblyFile, "\t\tbool_fmt: db \"%%d\", 0\n");
}