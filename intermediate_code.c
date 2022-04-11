#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
tuple* newTuple(OP op, char* arg1, char* arg2, char* arg3, tuple* next){
	tuple* t = (tuple*) malloc(sizeof(tuple));
	t->op = op;
	t->arg1 = arg1;
	t->arg2 = arg2;
	t->arg3 = arg3;
    t->next = next;
	return t;
}
void addTuple(tupleList* list, tuple* t){
	if(list->head==NULL){
		list->head = t;
		list->tail = t;
	}
	else{
		list->tail->next = t;
		list->tail = t;
	}
	list->no_tuples++;
}
tupleList* newList(){
	tupleList* list = (tupleList*) malloc(sizeof(tupleList));
    list->no_tuples = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}
char* newtemp(){
    static temp_no = 0;
	char* str = (char*)malloc(20 * sizeof(char));
	sprintf(str, "t%d",temp_no);
	temp_no = temp_no + 1;
	return str;
}
char* newlabel(){
    static label_no = 0;
	char* str = (char*)malloc(20 * sizeof(char));
	sprintf(str, "L%d",label_no);
	label_no = label_no + 1;
	return str;
}

// postorder traversal
tupleList* get_intermediate_list(ast_node* root){
    tupleList* list = newList();
    createIR(root, list);
    return list;
}
void createIR(ast_node* root, tupleList* list){
	if(root==NULL)
		return;
	ast_node* temp = root->firstChild;
    while(temp!=NULL){
        createIR(temp, list);
        temp = temp->nextSib;
    }
	IR_for_astnode(root, list);
}
// this is for a specific node
void IR_for_astnode(ast_node* root, tupleList* list){
	if(root->construct==booleanExpression_){
		IR_boolean_expression(root, list);
	}
    else if(root->construct == factor_){
        IR_factor(root, list);
    }
    else if(root->construct == termPrime_){
        IR_termprime(root, list);
    }
    else if(root->construct == expPrime_){
        IR_expPrime(root, list);
    }
    else if(root->construct==arithmeticExpression_){
        IR_arithematic_statement(root, list);
    }
    else if(root->construct == ioStmt_){
        IR_io_statement(root, list);
    }
    else{
        
    }

}
void IR_termprime(ast_node* root, TupleList* list){
    // termPrime gives an epsilon
    if(root->firstChild == NULL){
        root->place = NULL;
        root->tuple = NULL;
    }
    // termPrime gives HPO factor termPrime
    else{
        root->place = newtemp();
        Tuple* temp = root->firstChild->nextSib->nextSib->tuple;
        Tokentype op = ((struct operator_struct *)(root->firstChild->ninf))->op;
        Tuple* t;
        if(op == TK_MUL)
            t = makeTuple(MUL, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
        else
            t = makeTuple(DIV, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
        if(temp)
            temp->next = t;
        add_to_list(t, list);
        root->tuple = t;
    }
}

void IR_expPrime(ast_node* root, TupleList* list){
    // expPrime gives an epsilon
    if(root->firstChild == NULL){
        root->place = NULL;
        root->tuple = NULL;
    }
    // expPrime gives LPO factor termPrime
    else{
        root->place = newtemp();
        Tuple* temp = root->firstChild->nextSib->nextSib->tuple;
        Tokentype op = ((struct operator_struct *)(root->firstChild->ninf))->op;
        Tuple* t;
        if(op == TK_PLUS)
            t = makeTuple(PLUS, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
        else
            t = makeTuple(MINUS, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
        if(temp)
            temp->next = t;
        add_to_list(t, list);
        root->tuple = t;
    }
}
void IR_factor(ast_node* root){
    // factor gives arithematic expression
    if(root->firstChild->construct == arithmeticExpression_){
        root->place = root->firstChild->place;
        root->tuple = root->firstChild.tuple;
    }
    // factor gives var
    else{
        root->place = root->firstChild->place;
        root.tuple = root->firstChild->tuple;
    }
}

void IR_boolean_expression(ast_node* root){
}
void IR_arithematic_statement(ast_node* root, TupleList* list){
    root->place = newtemp();
    Tuple* t = make_new_Tuple()
    add_to_list(t, list);
    root->tuple = t;
}
