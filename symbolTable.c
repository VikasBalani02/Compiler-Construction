

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "symbolTabledef.h"
#include "astDef.h"
extern char* LabelMap[];
extern char* TerminalMap[];
extern char* NonTerminalMap[];



// Complete make Symbol 
// currentScope of funid


/**** Functions to initialize symbol table and add entries to it ******/

symbolTable* newSymbolTable(int no_slots){
	
	symbolTable* table =  (symbolTable *)malloc(sizeof(symbolTable));
    
	table->no_slots = no_slots;
	table->currentOffset =0;
	table->list = (symbolTableSlot **)malloc(no_slots*sizeof(symbolTableSlot*));
	
	for(int i=0;i<no_slots;i++){
		table->list[i] = (symbolTableSlot*)malloc(sizeof(symbolTableSlot));
		table->list[i]->head = NULL;
		table->no_slots = 0;
	}

	return table;
}

void addSymbol(symbolTable* symbolTable, char* key, SymbolTableRecord* symbol){
    //Keep the current offset stored in symbol table, assign that offset to any symbol added and update the offset
    int offset = symbolTable->currentOffset;
    symbol->offset = offset;
    symbolTable->currentOffset = symbolTable->currentOffset + symbol->width;

	int hash =  hashFunction(key,symbolTable->no_slots);
	symbol->next = symbolTable->list[hash]->head;
	symbolTable->list[hash]->head = symbol;

	symbolTable->list[hash]->noOfRecords++;
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


// /*************************************************************************************************/



// SymTableTreeNode* make_symTableTreeNode(SymTableTreeNode* parent, char* scope){

// 	SymTableTreeNode* node = (SymTableTreeNode*)malloc(sizeof(SymTableTreeNode));
// 	node->table = newSymbolTable(SYMBOL_TABLE_SLOTS);
// 	node->next = NULL;
// 	node->children = initialize_nested_scopes();

// 	node->parent = parent;

// 	node->scope = scope;

// 	if(parent!=NULL)
// 		add_to_nested_scopes(parent,node);

// 	return node;
// }

// void add_to_nested_scopes(SymTableTreeNode* parent, SymTableTreeNode* node){
	
// 	node->next = parent->children->head;
// 	parent->children->head = node;

// 	parent->children->no_siblings++;
// }

// nestedScopes* initialize_nested_scopes(){
// 	nestedScopes* children = (nestedScopes*)malloc(sizeof(nestedScopes));
// 	children->head = NULL;
// 	children->no_siblings = 0;
	
// 	return children;
// }



// /************************************************************************************************/

// SymTableTree* buildAllSymbolTables(AST* ast, ErrorList* errors){

// 	SymTableTree* tree = initialize_SymbolTableTree();

// 	//Add root to the tree
// 	char* _main = (char*)malloc(sizeof(char)*10);
// 	strcpy(_main,"_main");
// 	tree->root = make_symTableTreeNode(NULL, _main);

// 	int number = 0;
// 	traverseAST(ast->root, tree->root, errors, &number);

// 	return tree;

// }


// void traverseAST(ASTNode* node, SymTableTreeNode* current_scope, ErrorList* errors, int* number){

// 	if(node==NULL)
// 		return;

// 	node->current_scope = current_scope;

// 	SymTableTreeNode* new_scope = current_scope;
	

// 	/*** if Node is FUNCTIONDEF_NODE ***/
// 	if(node->leaf== 0 && node->label == FUNCTIONDEF_NODE){
// 		new_scope = make_symTableTreeNode(current_scope, node->children->head->next->lexicalTOKEN->lexeme);
// 	}



// 	/******** Now which entries have to be added to symbol Table ******/

// 	//Function Definition Encountered
// 	if(node->leaf == 1 && node->lexicalTOKEN->type==FUNID && node->parent->label == FUNCTIONDEF_NODE){

// 		//Look into current scope for same definition
// 		SymbolNode* info = getSymbolInfo(node->lexicalTOKEN->lexeme, node->parent->current_scope->table);

// 		//If  function overloading
// 		if(info!=NULL){
// 			char* str = (char*)malloc(sizeof(str)*ERROR_STRING_SIZE);
// 			sprintf(str,"Error: Function overloading for function '%s()'",node->lexicalTOKEN->lexeme);
// 			add_error(errors,str,node->lexicalTOKEN->line_no);
// 			//REPORT ERROR

// 		}
// 		else{
// 			*number = *number+1;
// 			Symbol* symbol = makeSymbol(node, *number);
// 			addSymbol(node->parent->current_scope->table,node->lexicalTOKEN->lexeme, symbol);
// 		}

// 		set_parent_function(node);
// 	}

// 	//Function Call Encountered
// 	else if(node->leaf == 1 && node->lexicalTOKEN->type==FUNID){

// 		//First set the static parent
// 		set_parent_function(node);

		
// 		SymbolNode* info = getSymbolInfo(node->lexicalTOKEN->lexeme, current_scope->table);

// 		int recursion = check_recursion(node);

// 		if(recursion==1){

// 			node->nodeType = ERROR;
			
// 			char* str = (char*)malloc(sizeof(str)*ERROR_STRING_SIZE);
// 			sprintf(str,"Error: Recursive Function Call '%s()' encountered",node->lexicalTOKEN->lexeme);
// 			add_error(errors,str,node->lexicalTOKEN->line_no);
// 		}

// 		//If the function definition is not there in the current scope
// 		else if(info==NULL){

// 			node->nodeType = ERROR;
			
// 			char* str = (char*)malloc(sizeof(str)*ERROR_STRING_SIZE);
// 			sprintf(str,"Error: Undefined Function '%s()' Called",node->lexicalTOKEN->lexeme);
// 			add_error(errors,str,node->lexicalTOKEN->line_no);			
// 			//REPORT ERROR

// 		}

// 	}

// 	//Variable declaration Encountered
// 	else if(node->leaf == 1 && (node->parent->label == DECLARATIONSTMT_NODE || node->parent->label == PARAMETER_NODE) && (node->lexicalTOKEN->type < 8 || node->lexicalTOKEN->type > 11)){
		
// 		//Look into current scope for same declaration
// 		SymbolNode* info = getSymbolInfo(node->lexicalTOKEN->lexeme, current_scope->table);		

// 		//If multiple declaration case
// 		if(info!=NULL){

// 			char* str = (char*)malloc(sizeof(str)*ERROR_STRING_SIZE);
// 			sprintf(str,"Error: Multiple Declaration for Variable '%s'",node->lexicalTOKEN->lexeme);
// 			add_error(errors,str,node->lexicalTOKEN->line_no);
// 			//REPORT Variable Declaration with a same name already done
// 		}

// 		else{
// 			*number = *number+1;
// 			Symbol* symbol = makeSymbol(node, *number);
// 			addSymbol(node->current_scope->table,node->lexicalTOKEN->lexeme, symbol);
// 		}
		
// 	}


// 	//Variable use Encountered
// 	else if(node->leaf == 1 && node->lexicalTOKEN->type==ID){

// 		//Look into the entire scope hierarchy for this variable declaration
// 		SymbolNode* info = findInfo(node);

// 		 //If the variable is declared earlier
// 		if(info!=NULL){
// 			node->nodeType = info->symbol->datatype;
// 		}
// 		else{
// 			node->nodeType = ERROR;

// 			char* str = (char*)malloc(sizeof(str)*ERROR_STRING_SIZE);
// 			sprintf(str,"Error: Undeclared Variable '%s' Used",node->lexicalTOKEN->lexeme);
// 			add_error(errors,str,node->lexicalTOKEN->line_no);
// 			//REPORT ERROR

// 		}
// 	}



// 	/*** Now go to the children ***/

// 	astChildren* children = node->children;
// 	ASTNode* temp;

// 	/*** Children in the new scope ***/
// 	if(children!=NULL){

// 		temp = children->head;

// 		while(temp!=NULL){
// 			traverseAST(temp,new_scope,errors,number);
// 			temp = temp->next;
// 		}
// 	}


	
// 	//If its a compresses concatenated node
// 	temp = node->concat;

// 	//Concatenated entities in the same scope
// 	traverseAST(temp,current_scope,errors,number);	

// }

// Symbol* makeSymbol(ASTNode* node, int number){

// 	Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
// 	symbol->lexicalTOKEN = node->lexicalTOKEN;
// 	symbol->width = 0;
// 	symbol->offset = 0;
// 	symbol->number = number;

// 	/**Pointer to corresponding AST code**/
// 	symbol->astNode = node;

// 	//For parameters and declarations
// 	if(symbol->lexicalTOKEN->type!=FUNID){
// 		symbol->datatype = node->parent->children->head->lexicalTOKEN->type;
// 	}
// 	else{
// 		symbol->datatype = FUNID;		
// 	}

// 	/**** Deciding Width if Symbol ****/

// 	if(symbol->datatype==INT){
// 		symbol->width = 2;
// 	}
	
// 	else if(symbol->datatype==STRING){
// 		symbol->width = 0;
// 	}
	
// 	else if(symbol->datatype==REAL){
// 		symbol->width = 4;
// 	}
// 	else if(symbol->datatype==MATRIX){
// 		symbol->width = 0;
// 	}

// 	symbol->assigned = 0;
// 	symbol->dimension1 = 0;
// 	symbol->dimension2 = 0;

// 	symbol->val = NULL;
// 	symbol->str = NULL;

// 	return symbol;

// }

// /**********************************************************************************************/

// void printSymbolTableTree(SymTableTree* tree){
// 	printf("\n\n-------------------------------------Printing Symbol Table Tree--------------------------------------\n\n");

// 	printf("%-20s %-20s %-15s %-20s %-10s %-5s %s\n\n", "Identifier name","Scope","Nesting level","Static parent","Type","Width","Offset");

// 	printSymbolTableTree_util(tree->root, 1);

// 	printf("\n\n-----------------------------------------------------------------------------------------------------\n\n");
// }

// void printSymbolTableTree_util(SymTableTreeNode* node, int nested){

// 	/****** Printing the current table ******/

// 	printSymbolTable(node, nested);

// 	/****** Now go to the children ********/

// 	nestedScopes* children = node->children;

// 	SymTableTreeNode* temp;

// 	if(children!=NULL){
// 		temp = children->head;

// 		while(temp!=NULL){
// 			printSymbolTableTree_util(temp, nested+1);
// 			temp = temp->next;			
// 		}
		
// 	}
// }

// void printSymbol(Symbol* symbol, int nested, char* scope, char* static_parent){

// 	if(symbol->lexicalTOKEN->type==FUNID)
// 		return;

// 	char* null = "NULL";
// 	if(static_parent!=NULL){
// 		printf("%-20s %-20s %-15d %-20s %-10s %-5d %d\n", symbol->lexicalTOKEN->lexeme,scope,nested,static_parent,TerminalMap[symbol->datatype],symbol->width,symbol->offset);
// 	}
// 	else{
// 		printf("%-20s %-20s %-15d %-20s %-10s %-5d %d\n", symbol->lexicalTOKEN->lexeme,scope,nested,null,TerminalMap[symbol->datatype],symbol->width,symbol->offset);
// 	}	

// }


// void printSymbolTable(SymTableTreeNode* node, int nested){
// 	char* scope = node->scope;
// 	char* static_parent;

// 	if(node->parent!=NULL){
// 		static_parent = node->parent->scope;
// 	}
// 	else{
// 		static_parent=NULL;
// 	}

// 	symbolTable* table = node->table;

// 	slotsList* symbolList = (slotsList*)malloc(sizeof(slotsList));
// 	symbolList->head = NULL;
// 	SymbolNode* temp;

// 	//Collecting symbols
// 	for(int i=0;i<SYMBOL_TABLE_SLOTS;i++){
// 		SymbolNode* n = table->slots[i]->head;

// 		while(n!=NULL){
// 			temp = (SymbolNode*)malloc(sizeof(SymbolNode));
// 			temp->symbol = n->symbol;

// 			temp->next = symbolList->head;
// 			symbolList->head = temp;

// 			n = n->next;
// 		}
// 	}

// 	sort_symbols(symbolList);

// 	temp = symbolList->head;

// 	while(temp!=NULL){
// 		printSymbol(temp->symbol, nested, scope, static_parent);
// 		temp = temp->next;
// 	}

// 	printf("\n\n");
// }


// void sort_symbols(slotsList* list){
// 	if(list->head==NULL){
// 		return;
// 	}
// 	sort_symbols_util(list->head,list->total_symbols);
// }


// void sort_symbols_util(SymbolNode* temp, int count){
// 	if(temp->next==NULL){
// 		return;
// 	}	
// 	sort_symbols_util(temp->next,count--);	
	
// 	// insert_in_sorted_order
// 	Symbol* symbol = temp->symbol;
// 	int number = temp->symbol->number;
	
// 	SymbolNode* pretemp = temp;
// 	temp = temp->next;
// 	while(temp!=NULL){

// 		if(temp->symbol->number > number){
// 			break;
// 		}		

// 		pretemp->symbol = temp->symbol;
				
// 		temp = temp->next;
// 		pretemp = pretemp->next;	
// 	}

// 	pretemp->symbol = symbol;
	
// }


// SymbolNode* findInfo(ASTNode* node){	
// 	SymbolNode* info = NULL;

// 	SymTableTreeNode* temp_scope = node->current_scope;

// 	while(temp_scope!=NULL){

// 		info = getSymbolInfo(node->lexicalTOKEN->lexeme, temp_scope->table);

// 		if(info!=NULL){
// 			break;
// 		}

// 		temp_scope = temp_scope->parent;
// 	}

// 	return info;
// }

// void set_parent_function(ASTNode* node){

// 	//Skip 1 parent
// 	ASTNode* temp = node->parent->parent;
// 	if(temp->label==FUNCTIONDEF_NODE){
// 		node->parent_function = temp->children->head->next;
// 	}

// 	//If the function call is inside the main function
// 	if(temp->label==MAINFUNCTION_NODE){
// 		node->parent_function = NULL;		
// 	}
// }

// int check_recursion(ASTNode* node){
		
// 	if(node->parent_function==NULL){
// 		return 0;
// 	}

// 	 if(strcmp(node->lexicalTOKEN->lexeme,node->parent_function->lexicalTOKEN->lexeme)==0){
// 	 	return 1;
// 	 }

// 	 return 0;	
// }

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
void traverseNode(ast_node * current, symbolTable* table, symbolTable* global){
    if(current->construct== typeDefinition){
        struct constructed_type_struct * info = (struct constructed_type_struct *)current->ninf;
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        if(info->union_or_record = TK_RECORD){
            temp->type = RECORD;
            temp->lexeme = info->ruid;
            temp->line_no = info->lineNum;
            temp->recordFields = NULL;

            int offset =0; //for calculting the offset of fields relative to the base of the record definition

            //child == **fieldDefinition**
            ast_node * child = current->firstChild; 
            while(child != NULL){ 
                //for each field definition create a record field * and attach it to linked list
                struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                struct field_id_struct * info2 = child->ninf;
                field->lexeme = info2->fieldid;
                field->line_no = info2->lineNum;

                if(child->firstChild->construct == primitiveDatatype){
                    struct ast_node * primitive = child->firstChild; //primitive datatype node
                    struct primitive_type_struct * info3 = primitive->ninf;
                    if(info3->int_or_real == TK_INT){
                        field->type = INT;
                        field->width = 4 ;
                        field->offset = offset;
                        offset += field->width;
                        add_record_field(temp,field);
                    }
                    else {
                        field->type = REAL;
                        field->width = 8 ;
                        field->offset = offset;
                        offset += field->width;
                        add_record_field(temp,field);
                    }   
                }
                else if (child->firstChild->construct == fieldtype){
                    struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                    struct ast_node * constructed = child->firstChild;
                    struct constructed_type_struct * info2 = constructed->ninf;
                    field->lexeme = info2->ruid;
                    field->line_no = info->lineNum;
                    SymbolTableRecord* record_ruid = getSymbolInfo(info2->ruid,global);
                    if(record_ruid == NULL)
                     if(record_ruid==NULL) {
                        printf("Line NO: %d, Error: The ruid used in the field declaration has not been defined till now.",info2->ruid);
                        exit(0);
                    }
                    SymbolTableRecord* record_original = getSymbolInfo(record_ruid->type_ruid, global);
                   if(record_original==NULL){
                       printf("LINENO: %d, Error: There is no record corresponding to the ruid %s",info2->lineNum,record_ruid->type_ruid);
                   }
                    if (record_original->type == RECORD){
                        field->type = RECORD;
                        field->width = record_original->width;
                        field->offset = offset;
                        offset += field->width;
                    }
                    else if(record_original->type == UNION){
                        field->type = UNION;
                        field->width = record_original->width;
                        field->offset=offset;
                        offset = field->width + offset;
                    }
                    add_record_field(temp,field);
                }
                child = child->nextSib;
            }
            temp->width = offset;
            addSymbol(temp,temp->lexeme,global);
        }
        else if (info->union_or_record = TK_UNION){
            temp->type = UNION;
            temp->lexeme = info->ruid;
            temp->line_no = info->lineNum;
            temp->recordFields = NULL;

            int offset =0; //for calculting the offset of fields relative to the base of the record definition

            //child == **fieldDefinition**
            ast_node * child = current->firstChild; 
            while(child != NULL){ 
                //for each field definition create a record field * and attach it to linked list
                struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                struct field_id_struct * info2 = child->ninf;
                field->lexeme = info2->fieldid;
                field->line_no = info2->lineNum;

                if(child->firstChild->construct == primitiveDatatype){
                    struct ast_node * primitive = child->firstChild; //primitive datatype node
                    struct primitive_type_struct * info3 = primitive->ninf;
                    if(info3->int_or_real == TK_INT){
                        field->type = INT;
                        field->width = 4 ;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                        add_record_field(temp,field);
                    }
                    else {
                        field->type = REAL;
                        field->width = 8 ;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                        add_record_field(temp,field);
                    }   
                }
                else if (child->firstChild->construct == fieldtype){
                    struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                    struct ast_node * constructed = child->firstChild;
                    struct constructed_type_struct * info2 = constructed->ninf;
                    field->lexeme = info2->ruid;
                    field->line_no = info->lineNum;
                    SymbolTableRecord* record_ruid = getSymbolInfo(info2->ruid,global);
                    if(record_ruid == NULL)
                     if(record_ruid==NULL) {
                        printf("Line NO: %d, Error: The ruid used in the field declaration has not been defined till now.",info2->ruid);
                        exit(0);
                    }
                    SymbolTableRecord* record_original = getSymbolInfo(record_ruid->type_ruid, global);
                   if(record_original==NULL){
                       printf("LINENO: %d, Error: There is no record corresponding to the ruid %s",info2->lineNum,record_ruid->type_ruid);
                   }
                    if (record_original->type == RECORD){
                        field->type = RECORD;
                        field->width = record_original->width;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                    }
                    else if(record_original->type == UNION){
                        field->type = UNION;
                        field->width = record_original->width;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                    }
                    add_record_field(temp,field);
                }
                child = child->nextSib;
            }
            temp->width = offset;
            addSymbol(temp,temp->lexeme,global);
        }
    }

    else if (current->construct == definetypestmt_){
        struct defineType_struct *info = current->ninf;
        struct constructed_type_struct *from = info->from;
        struct constructed_type_struct *to = info->to;
        SymbolTableRecord * entry = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        entry->lexeme = to->ruid;
        entry->type = RUID;
        entry->type_ruid = from->ruid;
        entry->line_no = to->lineNum;
        addSymbol(entry,entry->lexeme,global);
    }
}
void traverseNode2(ast_node * current, symbolTable* table, symbolTable* global, int offset){
    if(current->construct == input_par){
        ast_node * child = current->firstChild;
        while(child!=NULL){
            SymbolTableRecord * record = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
            Type type;
            int width;
            char * ruid = NULL;
            int lineNo;
            if(child->construct==primitiveDatatype){
                struct primitive_type_struct * info = (struct primitive_type_struct *)current->ninf;
                if(info->int_or_real==TK_INT){
                    type = INT;
                    width = 4;
                }
                else {type = REAL; width = 8;}

                lineNo = info->lineNum;
            }
            else if (child->construct==constructedDatatype){
                struct constructed_type_struct * info = (struct  constructed_type_struct*)current->ninf;
                ruid = info->ruid;
                if(info->union_or_record==TK_RECORD){
                    type = RECORD;
                }
                else type = UNION;

                lineNo = info->lineNum;
            }
            child = child->nextSib;
            struct id_struct * info = (struct  id_struct*)current->ninf;
            char * lexeme = info->lexID;
            record->type = type;
            record->lexeme = lexeme;
            record->line_no = lineNo;
            record->type_ruid = ruid;
            addSymbol(global,record->lexeme,record);

            
        }
    }
    if(current->construct == declaration){
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        struct id_struct * info = (struct id_struct *)current->ninf;
        temp->lexeme = info->lexID;
        temp->line_no = info->lineNum;
        if(info->isGlobal){
            addSymbol(global,temp->lexeme, temp);
        }
        else addSymbol(table,temp->lexeme, temp);
    }
}
symbolTable * getSymbolTableFunction(ast_node * node, symbolTable *global, char* funid){
    symbolTable * table = (symbolTable *) malloc(sizeof(symbolTable));
    traverseNode(node, table, global);
}