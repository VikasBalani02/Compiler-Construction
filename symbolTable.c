

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "symbolTable.h"
extern char* LabelMap[];
extern char* TerminalMap[];
extern char* NonTerminalMap[];
#define INTWIDTH 2
#define REALWIDTH 4
#define SLOTS 8

char * usageMAP[] = {"Local", "INPUTPAR", "OUTPUTPAR"};
symbolTable* newSymbolTable(int no_slots){
	
	symbolTable* table =  (symbolTable *)malloc(sizeof(symbolTable));
    
	table->no_slots = no_slots;
	table->currentOffset =0;
	table->list = (symbolTableSlot **)malloc(no_slots*sizeof(symbolTableSlot*));
	
	for(int i=0;i<no_slots;i++){
		table->list[i] = (symbolTableSlot*)malloc(sizeof(symbolTableSlot));
		table->list[i]->head = NULL;
	}

	return table;
}

int hashFunction(char* key, int no_of_slots){
	int sum = 0;
    int n=strlen(key);
	for(int i=0;i<n;i++){
        int temp=key[i]-'0';
		sum = sum *10 +temp;
		sum = sum%no_of_slots;
	}

	int hash = sum%no_of_slots;
	return hash;
}
int checkSymbol(symbolTable* symbolTable,char*key){
    int hash =  hashFunction(key,symbolTable->no_slots);
	SymbolTableRecord *temp = symbolTable->list[hash]->head;
    while(temp!=NULL){
        if(strcmp(temp->lexeme,key)==0){
             return 1;
        }
        temp=temp->next;       
    }
    return 0;
}

void addSymbol(symbolTable* symbolTable, char* key, SymbolTableRecord* symbol){
    //Keep the current offset stored in symbol table, assign that offset to any symbol added and update the offset
    if(symbol->width!=-1){
    int offset = symbolTable->currentOffset;
    symbol->offset = offset;
    symbolTable->currentOffset = symbolTable->currentOffset + symbol->width;
    }
    
	int hash =  hashFunction(key,symbolTable->no_slots);
	symbol->next = symbolTable->list[hash]->head;
	symbolTable->list[hash]->head = symbol;

	symbolTable->list[hash]->noOfRecords++;
}

SymbolTableRecord* getSymbolInfo(char* key, symbolTable* symbolTable){
	int hash = hashFunction(key, symbolTable->no_slots);

	SymbolTableRecord* temp = symbolTable->list[hash]->head;
	while(temp!=NULL){
		if(strcmp(temp->lexeme,key)==0)
			return temp;
		temp = temp->next;
		
	}
	return NULL;
}

int global_check(symbolTable *global, char *key){
    int hash=hashFunction(key,global->no_slots);
    SymbolTableRecord*temp= global->list[hash]->head;
    while(temp!=NULL){
		if(strcmp(temp->lexeme,key)==0)
			return 1;
		temp = temp->next;
		
	}
	return 0;


}

typeInfo* add_function_par(typeInfo * typeInfoList, Type type, char* lexeme, char *ruid, symbolTable* global){
    if(global_check(global,lexeme)){

        printf("Redeclaration of global variable %s",lexeme);
        return NULL;
    }
    else{
        typeInfo * typeInfoNode = (typeInfo *) malloc(sizeof(typeInfo));
        typeInfoNode->type = type;
        typeInfoNode->type_ruid = ruid;
        typeInfoNode->lexeme=lexeme;
        typeInfoNode->next = NULL;
        typeInfo * iterator = typeInfoList;
        if(iterator==NULL) {
            return typeInfoNode;
        } 
        while(iterator->next!=NULL){
            iterator=iterator->next;
        }
        iterator->next = typeInfoNode;
        return typeInfoList;

    }
    
}

void add_record_field (SymbolTableRecord * record, struct record_field * field){
    struct record_field * temp = record->recordFields;
    if(temp == NULL){
        record->recordFields = field;
    }
    else{
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = field;
    }
}
int traverseNode(ast_node * current, symbolTable* global){
    if(current->construct== typeDefinition_){
        struct constructed_type_struct * info = (struct constructed_type_struct *)current->ninf;
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        if(info->union_or_record == TK_RECORD){
            temp->type = RECORD;
            temp->lexeme = info->ruid;
            temp->line_no = info->lineNum;
            temp->recordFields = NULL;
            temp->width =-1;
            // int offset =0; //for calculting the offset of fields relative to the base of the record definition
            //typedefinitions -> fielddefiniitions->fielddefinition
            //child == **fieldDefinition**
            ast_node * child = current->firstChild->firstChild; 
            while(child != NULL){ 
                //for each field definition create a record field * and attach it to linked list
                struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                field->next = NULL;
                struct field_id_struct * info2 = (struct field_id_struct *)child->ninf;
                field->lexeme = info2->fieldid;
                field->line_no = info2->lineNum;

                if(child->firstChild->construct == primitiveDatatype_){
                    struct ast_node * primitive = child->firstChild; //primitive datatype node
                    struct primitive_type_struct * info3 = (struct primitive_type_struct *)primitive->ninf;
                    if(info3->int_or_real == TK_INT){
                        field->type = INT;
                        // field->width = 4 ;
                        // field->offset = offset;
                        // offset += field->width;
                        add_record_field(temp,field);
                    }
                    else {
                        field->type = REAL;
                        // field->width = 8 ;
                        // field->offset = offset;
                        // offset += field->width;
                        add_record_field(temp,field);
                    }   
                }
                else if (child->firstChild->construct == fieldtype_){
                    struct ast_node * constructed = child->firstChild;
                    struct constructed_type_struct * info2 = ( struct constructed_type_struct *)constructed->ninf;
                    field->ruid = info2->ruid;
                    field->type = RUID;
                //     SymbolTableRecord* record_ruid = getSymbolInfo(info2->ruid,global);
                //     if(record_ruid == NULL)
                //      if(record_ruid==NULL) {
                //         printf("Line NO: %d, Error: The ruid used in the field declaration has not been defined till now. %s",info2->lineNum,info2->ruid);
                //         exit(0);
                //     }
                //     SymbolTableRecord* record_original = getSymbolInfo(record_ruid->type_ruid, global);
                //    if(record_original==NULL){
                //        printf("LINENO: %d, Error: There is no record corresponding to the ruid %s",info2->lineNum,record_ruid->type_ruid);
                //    }
                //     if (record_original->type == RECORD){
                //         field->type = RECORD;
                //         field->width = record_original->width;
                //         field->offset = offset;
                //         offset += field->width;
                //     }
                //     else if(record_original->type == UNION){
                //         field->type = UNION;
                //         field->width = record_original->width;
                //         field->offset=offset;
                //         offset = field->width + offset;
                //     }
                    add_record_field(temp,field);
                }
                child = child->nextSib;
            }
            // temp->width = offset;
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme,temp);
        }
        else if (info->union_or_record = TK_UNION){
            temp->type = UNION;
            temp->lexeme = info->ruid;
            temp->line_no = info->lineNum;
            temp->recordFields = NULL;

            // int offset =0; //for calculting the offset of fields relative to the base of the record definition

            //child == **fieldDefinition**
            ast_node * child = current->firstChild->firstChild; 
            while(child != NULL){ 
                //for each field definition create a record field * and attach it to linked list
                struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                struct field_id_struct * info2 = (struct field_id_struct *)child->ninf;
                field->lexeme = info2->fieldid;
                field->line_no = info2->lineNum;
                field->next = NULL;
                if(child->firstChild->construct == primitiveDatatype_){
                    struct ast_node * primitive = child->firstChild; //primitive datatype node
                    struct primitive_type_struct * info3 = (struct primitive_type_struct *)primitive->ninf;
                    if(info3->int_or_real == TK_INT){
                        field->type = INT;
                        // field->width = 4 ;
                        // field->offset = 0;
                        // offset = (offset > field->width ? offset: field->width);
                        add_record_field(temp,field);
                    }
                    else {
                        field->type = REAL;
                        // field->width = 8 ;
                        // field->offset = 0;
                        // offset = (offset > field->width ? offset: field->width);
                        add_record_field(temp,field);
                    }   
                }
                else if (child->firstChild->construct == fieldtype_){
                    struct ast_node * constructed = child->firstChild;
                    struct constructed_type_struct * info2 = (struct constructed_type_struct *)constructed->ninf;
                    field->ruid = info2->ruid;
                    field->next = NULL;
                    field->type = RUID;
                //     SymbolTableRecord* record_ruid = getSymbolInfo(info2->ruid,global);
                //     if(record_ruid == NULL)
                //      if(record_ruid==NULL) {
                //         printf("Line NO: %d, Error: The ruid used in the field declaration has not been defined till now. %s",info2->lineNum,info2->ruid);
                //         exit(0);
                //     }
                //     SymbolTableRecord* record_original = getSymbolInfo(record_ruid->type_ruid, global);
                //    if(record_original==NULL){
                //        printf("LINENO: %d, Error: There is no record corresponding to the ruid %s",info2->lineNum,record_ruid->type_ruid);
                //    }
                //     if (record_original->type == RECORD){
                //         field->type = RECORD;
                //         field->width = record_original->width;
                //         field->offset = 0;
                //         offset = (offset > field->width ? offset: field->width);
                //     }
                //     else if(record_original->type == UNION){
                //         field->type = UNION;
                //         field->width = record_original->width;
                //         field->offset = 0;
                //         offset = (offset > field->width ? offset: field->width);
                //     }
                    add_record_field(temp,field);
                }
                child = child->nextSib;
            }
            temp->width = -1;
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme,temp);
        }
    }
    else if(current->construct == declaration_){
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        struct id_struct * info = (struct id_struct *)current->ninf;
        temp->lexeme = info->lexID;
        temp->line_no = info->lineNum;
        ast_node * dataType = current->firstChild;
        if(info->isGlobal){
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme, temp);
        }
    }

    else if (current->construct == definetypestmt_){
        struct defineType_struct *info = (struct defineType_struct *)current->ninf;
        struct constructed_type_struct *from = info->from;
        struct constructed_type_struct *to = info->to;
        SymbolTableRecord * entry = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        entry->lexeme = to->ruid;
        entry->type = RUID;
        entry->type_ruid = from->ruid;
        entry->line_no = to->lineNum;
        SymbolTableRecord * entry2 = getSymbolInfo(entry->lexeme,global);
            if(entry2 != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", entry->line_no, entry->lexeme, entry2->line_no);
                return 1;
            }
        addSymbol(global,entry->lexeme,entry);
    }
    ast_node * child = current->firstChild;
    while(child!=NULL){
        if(traverseNode(child,global)==1) return 1;
        child=child->nextSib;
    }
}
int checkTagValue(symbolTable * global, SymbolTableRecord * entry){
    struct record_field* field_ptr = entry->recordFields;
    int flag =0;
    while(field_ptr != NULL){
        if(strcmp(field_ptr->lexeme,"tagvalue")==0 && field_ptr->type == INT){
            flag = 1;
            break;
        }
        field_ptr = field_ptr->next;
    }
    if(flag ==1) return 0;
    printf("Line No: %d, ERROR: Variant Records must have a tagvalue field of type int",entry->line_no);
    return -1;
}
int calculateWidth(symbolTable * global, SymbolTableRecord * entry){
    if(entry->type == RECORD){
        struct record_field* field_ptr = entry->recordFields;
        int offset = 0;
        while(field_ptr != NULL){
            if(field_ptr->type == INT){
                field_ptr->width = INTWIDTH ;
                field_ptr->offset = offset;
                offset += field_ptr->width;
            }
            else if (field_ptr->type == REAL){
                field_ptr->width = REALWIDTH ;
                field_ptr->offset = offset;
                offset += field_ptr->width;
            }
            else if (field_ptr->type == RUID){
                SymbolTableRecord * recordEntryRUID = getSymbolInfo(field_ptr->ruid,global);
                if(recordEntryRUID==NULL){
                    printf("Line No:%d, ERROR : No definition found for the field type %s", field_ptr->line_no, field_ptr->ruid);
                    return -1;
                }
                SymbolTableRecord* ruidOriginal = getSymbolInfo(recordEntryRUID->type_ruid, global);
                if(ruidOriginal==NULL){
                    printf("Line No:%d, ERROR :Error: There is no record definition corresponding to the ruid %s", field_ptr->line_no, recordEntryRUID->type_ruid);
                    return -1;
                }

                int w;
                if(ruidOriginal->width == -1){
                    w = calculateWidth(global,ruidOriginal);
                    if(w==-1) return -1;
                }
                w = ruidOriginal->width;

                if(recordEntryRUID->width == -1){
                    recordEntryRUID->width = ruidOriginal->width;
                }

                if(ruidOriginal->type == RECORD){
                    field_ptr->type = RECORD;
                    field_ptr->width = w;
                    field_ptr->offset = offset;
                    offset += field_ptr->width;
                }
                else if(ruidOriginal->type == UNION){
                    entry->type = VARIANTRECORD;
                    field_ptr->type = UNION;
                    field_ptr->width = w;
                    field_ptr->offset = offset;
                    offset += field_ptr->width;
                    if(checkTagValue(global,entry)==-1)return -1;
                }                 
            }
            field_ptr = field_ptr->next;
        }
        entry->width = offset;
    }
    if(entry->type == UNION){
        struct record_field* field_ptr = entry->recordFields;
        int offset = 0;
        while(field_ptr != NULL){
            if(field_ptr->type == INT){
                field_ptr->width = INTWIDTH ;
                field_ptr->offset = offset;
                offset = (offset > field_ptr->width ? offset: field_ptr->width);
            }
            else if (field_ptr->type == REAL){
                field_ptr->width = REALWIDTH;
                field_ptr->offset = offset;
                offset = (offset > field_ptr->width ? offset: field_ptr->width);
            }
            else if (field_ptr->type == RUID){
                SymbolTableRecord * recordEntryRUID = getSymbolInfo(field_ptr->ruid,global);
                if(recordEntryRUID==NULL){
                    printf("Line No:%d, ERROR : No definition found for the field type %s", field_ptr->line_no, field_ptr->ruid);
                    return -1;
                }
                SymbolTableRecord* ruidOriginal = getSymbolInfo(recordEntryRUID->type_ruid, global);
                if(ruidOriginal==NULL){
                    printf("Line No:%d, ERROR :Error: There is no record definition corresponding to the ruid %s", field_ptr->line_no, recordEntryRUID->type_ruid);
                    return -1;
                }

                int w;
                if(ruidOriginal->width == -1){
                    w = calculateWidth(global,ruidOriginal);
                    if(w==-1) return -1;
                }
                w = ruidOriginal->width;

                if(recordEntryRUID->width == -1){
                    recordEntryRUID->width = ruidOriginal->width;
                }

                if(ruidOriginal->type == RECORD){
                    field_ptr->type = RECORD;
                    field_ptr->width = w;
                    field_ptr->offset = offset;
                    offset = (offset > field_ptr->width ? offset: field_ptr->width);
                }
                else if(ruidOriginal->type == UNION){
                    field_ptr->type = UNION;
                    field_ptr->width = w;
                    field_ptr->offset = offset;
                    offset = (offset > field_ptr->width ? offset: field_ptr->width);
                }                 
            }
            field_ptr = field_ptr->next;
        }
        entry->width = offset;
    }
    return 0;
}
int traverseSymbolTable(symbolTable * global){
    for(int i=0; i<global->no_slots ; i++){
        symbolTableSlot * slot = global->list[i];
        SymbolTableRecord * entry = slot->head;
        while(entry != NULL){
            if(entry->type == RECORD || entry->type == UNION){
                if(entry->width == -1){
                    if(calculateWidth(global, entry)==-1) return -1;
                }
            }
            entry = entry->next;
        }
    }
    return 0;
}

int traverseNodeFunction(ast_node * current, symbolTable* table, symbolTable* global, SymbolTableRecord * function){ //returns 0 if no error
    if(current->construct == input_par_ || current->construct == output_par_){
        ast_node * child = current->firstChild;
        while(child!=NULL){
            SymbolTableRecord * record = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
            Type type;
            int width;
            char * ruid = NULL;
            int lineNo;
            if(child->construct==primitiveDatatype_){
                struct primitive_type_struct * info = (struct primitive_type_struct *)child->ninf;
                if(info->int_or_real==TK_INT){
                    type = INT;
                    width = INTWIDTH;
                }
                else {type = REAL; width = REALWIDTH;}

                lineNo = info->lineNum;
            }
            else if (child->construct==constructedDatatype_){
                struct constructed_type_struct * info = (struct  constructed_type_struct*)child->ninf;
                ruid = info->ruid;
                if(info->union_or_record==TK_RECORD){
                    type = RECORD;
                    lineNo = info->lineNum;
                    SymbolTableRecord * entry = getSymbolInfo(ruid,global);
                    if(entry==NULL){
                        printf("Line Num: %d, Error: Couldn't find definition for this constructed datatype.",lineNo);
                        return 1;
                    }
                    width = entry->width;
                }
                else if(info->union_or_record == TK_UNION) 
                {
                    printf("Line No: %d, Union %s cannot be declared in this manner %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                else if (info->union_or_record == -1){
                    lineNo = info->lineNum;
                    SymbolTableRecord * entry = getSymbolInfo(ruid,global);
                    if(entry==NULL){
                        printf("Line Num: %d, Error: Couldn't find definition for this constructed datatype.",lineNo);
                        return 1;
                    }
                    type = entry->type;
                    ruid = entry->type_ruid;
                    SymbolTableRecord * entry2 = getSymbolInfo(entry->type_ruid,global);
                    if(entry2->type==4){
                       printf("Line No: %d, Union %s cannot be declared in this manner \n",info->lineNum,info->ruid);
                       return 1;
                    }
                    width = entry2->width;
                }
            }
            child = child->nextSib;
            struct id_struct * info = (struct  id_struct*)child->ninf;
            char * lexeme = info->lexID;
            record->type = type;
            record->lexeme = lexeme;
            record->line_no = lineNo;
            record->type_ruid = ruid;
            record->width = width;
            if(current->construct==input_par_){
                function->function_field->InputHead = add_function_par(function->function_field->InputHead,record->type,record->lexeme,record->type_ruid,global);
                record->usage = INPUTPAR;
            }else{
              function->function_field->OutputHead = add_function_par(function->function_field->OutputHead,record->type,record->lexeme,record->type_ruid,global);   
                record->usage = OUTPUTPAR;
            }
            SymbolTableRecord * entry = getSymbolInfo(record->lexeme,table);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", record->line_no, record->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(table,record->lexeme,record);
            child = child->nextSib;
        }
    }
    if(current->construct == declaration_){
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        struct id_struct * info = (struct id_struct *)current->ninf;
        temp->lexeme = info->lexID;
        temp->line_no = info->lineNum;
        ast_node * dataType = current->firstChild;
        if(dataType->construct == primitiveDatatype_){
            struct primitive_type_struct *info =(struct primitive_type_struct *) dataType->ninf;
            temp->line_no = info->lineNum;
            temp->usage = LOCAL;
            if(info->int_or_real==TK_INT){
                temp->type = INT;
                temp->type_ruid = NULL;
                temp->width = INTWIDTH;
            }
            else if(info->int_or_real==TK_REAL){
                temp->type=REAL;
                temp->type_ruid = NULL;
                temp->width = REALWIDTH;
            }
        }
        else if (dataType->construct==constructedDatatype_){
            struct constructed_type_struct *info = (struct constructed_type_struct *)dataType->ninf;
            if(info->union_or_record==TK_UNION){
                
                /*SymbolTableRecord * entry = getSymbolInfo(info->ruid,global);
                if(entry == NULL){
                    printf("Line No: %d, No type definition found corresponding to this type %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->type = UNION;
                temp->width = entry->width;
                temp->type_ruid = info->ruid;
                temp->line_no = info->lineNum;*/
                printf("Line No: %d, Union %s cannot be declared in this manner %s\n",info->lineNum,info->ruid);
                return 1;

            }
            else if (info->union_or_record==TK_RECORD){
                SymbolTableRecord * entry = getSymbolInfo(info->ruid,global);
                if(entry == NULL){
                    printf("Line No: %d, No type definition found corresponding to this type %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->type = entry->type;
                temp->type_ruid = info->ruid;
                temp->line_no = info->lineNum;
                temp->width = entry->width;
            }
            else if (info->union_or_record == -1){
                SymbolTableRecord * entry = getSymbolInfo(info->ruid,global);
                if(entry==NULL || entry->type !=RUID){
                    printf("Line No: %d, No type definition found corresponding to this type %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->type = entry->type;
                temp->type_ruid = entry->type_ruid;
                SymbolTableRecord * entry2 = getSymbolInfo(entry->type_ruid,global);
                if(entry2->type==4){
                    printf("Line No: %d, Union %s cannot be declared in this manner %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->line_no = info->lineNum;
                temp->width = entry2->width;
            }
        }
        if(info->isGlobal){
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme, temp);
        }
        else {
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,table);
            SymbolTableRecord * entry_2 = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            if(entry_2 != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry_2->line_no);
                return 1;
            }
            addSymbol(table,temp->lexeme, temp);
            }
    }
    ast_node * child = current->firstChild;
    while(child!=NULL){
        if(traverseNodeFunction(child,table,global, function)==1) return 1;
        child=child->nextSib;
    }
    return 0;
}

symbolTable * populateSymbolTable(ast_node * root){
    symbolTable * globalTable = newSymbolTable(SLOTS);
    traverseNode(root,globalTable);
    if(traverseSymbolTable(globalTable)==-1) return NULL;
    ast_node * function = root->firstChild;
    int i=0;
    while(function!=NULL){
        SymbolTableRecord * FunEntry = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        struct func_struct *info = (struct func_struct *)function->ninf;
        FunEntry->lexeme = info->funID;
        FunEntry->line_no = info->lineNum;
        FunEntry->type = FUNCTION;
        symbolTable * funTable = newSymbolTable(SLOTS);
        FunEntry->functionTable = funTable;
        if(checkSymbol(globalTable,info->funID)){
            printf("Function already exists");
            return NULL;
        }
        addSymbol(globalTable,FunEntry->lexeme,FunEntry);
        FunEntry->function_field = (struct function_field *)malloc(sizeof(struct function_field));
        FunEntry->function_field->InputHead = NULL;
        FunEntry->function_field->OutputHead=NULL;
        FunEntry->function_field->num=i;
        i++;
        FunEntry->width =0;
        if(traverseNodeFunction(function,funTable,globalTable,FunEntry)==1) exit(0);
        function = function->nextSib;
    }
    return globalTable;
}
void printTypeExpression(SymbolTableRecord * entry, symbolTable * global){
    if(entry->type == RUID|| entry->type_ruid !=NULL){
        SymbolTableRecord * original = getSymbolInfo(entry->type_ruid, global);
        printTypeExpression(original,global);
    }
    else if(entry->type == RECORD || entry->type == VARIANTRECORD){
        printf("<");
        struct record_field * field = entry->recordFields;
        while(field!=NULL){
            if(field->type == RECORD || field->type == UNION){
                SymbolTableRecord * record = getSymbolInfo(field->ruid,global);
                printTypeExpression(record,global);
            }
            else if (field->type == INT){
                printf(" int ");
            }
            else if (field->type == REAL){
                printf(" real ");
            }
            field = field->next;
            if(field!=NULL) printf(",");
        }
        printf(">");
    }
    else if(entry->type == INT){
        printf(" int ");
    }
    else if(entry->type == REAL){
        printf(" real ");
    }
    else if(entry->type == FUNCTION){
        printf(" function ");
    }
    else if(entry->type == UNION){
        printf("<");
        struct record_field * field = entry->recordFields;
        while(field!=NULL){
            if(field->type == RECORD || field->type == UNION){
                SymbolTableRecord * record = getSymbolInfo(field->ruid,global);
                printTypeExpression(record,global);
            }
            else if (field->type == INT){
                printf(" int ");
            }
            else if (field->type == REAL){
                printf(" real ");
            }
            field = field->next;
            if(field!=NULL) printf("|");
        }
        printf(">");
    }
}

void printSymbolTableFunction(symbolTable * table, char * name, symbolTable * global){
printf("**************%s Symbol Table*****************\n",name);
for(int i=0;i<table->no_slots;i++){
    SymbolTableRecord * entry = table->list[i]->head;
    while(entry!=NULL){
            printf("Name: %s\n",entry->lexeme);

            printf("Scope: %s\n", name);
            if(entry->type == RECORD || entry->type == VARIANTRECORD || entry->type == RUID){
                if(entry->type_ruid !=NULL)
                printf("TypeName : %s\n", entry->type_ruid);
            }
            printf("Type Expression: ");
            printTypeExpression(entry, global);
            printf("\n");

            if(entry->type == INT || entry->type == REAL || entry->type == RECORD||entry->type == VARIANTRECORD || entry->type == UNION || entry->type == RUID){
                printf("Width: ");
                printf("%d\n",entry->width);
            }

            printf("isGlobal : ---\n");

            if(entry->type == INT || entry->type == REAL || entry->type == RECORD || entry->type == UNION || entry->type == VARIANTRECORD || entry->type == RUID)
            printf("Offset: %d\n", entry->offset);

            printf("Variable Usage : %s\n", usageMAP[entry->usage]);
            printf("******************************************************************\n");
            entry = entry->next;
        }
}
printf("\n\n");
}
void printSymbolTable(symbolTable * global){
    for(int i=0;i<global->no_slots;i++){
        SymbolTableRecord * entry = global->list[i]->head;
        while(entry!=NULL){
            printf("Name: %s\n",entry->lexeme);

            printf("Scope: GLOBAL\n");

            if(entry->type == RECORD || entry->type == VARIANTRECORD || entry->type == RUID){
                if(entry->type_ruid !=NULL)
                printf("TypeName : %s\n", entry->type_ruid);
            }

            printf("Type Expression: ");
            printTypeExpression(entry, global);
            printf("\n");

            if(entry->type == INT || entry->type == REAL || entry->type == RECORD||entry->type == VARIANTRECORD || entry->type == UNION || entry->type == RUID){
                printf("Width: ");
                printf("%d\n",entry->width);
            }

            printf("isGlobal : global\n");

            if(entry->type == INT || entry->type == REAL || entry->type == RECORD || entry->type == UNION)
            printf("Offset: %d\n", entry->offset);

            printf("Variable Usage : ---\n");
            printf("******************************************************************\n");
            entry = entry->next;
        }
    }

    for(int i=0;i<global->no_slots;i++){
        SymbolTableRecord *  entry = global->list[i]->head;
        while(entry!=NULL){
            if(entry->type == FUNCTION){
                printSymbolTableFunction(entry->functionTable, entry->lexeme, global);
            }
            entry = entry->next;
        }
    }

}