#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

void update_code(ast_node* t, char* str){
    int l1= 0;
    if(t->code == NULL){
        l1=0;
        t->code = str;
        // free(str);
        return;
    }
    else
        l1=strlen(t->code);
    int l2 =0;
    if(str == NULL){
        return;
    }
    else
        l2= strlen(str);
    l1+=(l2+10);
    char* temp;
    temp = (char*) malloc(l1 * sizeof(char));
    memset(temp,'\0',sizeof(temp));
    temp = strcat(temp,t->code);
    temp = strcat(temp,str);
    free(t->code);
    free(str);
    t->code = temp;
    return;
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
void createIR(ast_node* root){
	if(root==NULL)
		return;
	ast_node* temp = root->firstChild;
    while(temp!=NULL){
        createIR(temp);
        temp = temp->nextSib;
    }
	IR_for_astnode(root);
}
void generate(ast_node* root, char ** arr, int no_to_add){
    for(int i=0;i<no_to_add;i++){
        update_code(root->code, arr[i]);
    }
    return;
}
void IR_for_astnode(ast_node* root){
	if(root->construct==booleanExpression_){
		IR_boolean_expression(root);
	}
}

IR_boolean_expression(ast_node* root){
    ast_node *bool_exp1 = root->firstChild;
    if (bool_exp1->nextSib == NULL)
    {
        // <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1> TK_CL
        // booleanExpression.addr = makeNode(booleanExpression, struct{TK_NOT}, booleanExpression1.addr)
        root->place = newtemp();
        char** arr= (char**) malloc(5*sizeof(char*));
        arr[0] = bool_exp1->code;
        arr[1] = root->place;
        arr[2] = " := NOT ";
        arr[3] = bool_exp1->place;
        arr[4] = "\n";
        generate(root, arr, 5);
        return;
    }
    ast_node *bool_exp2 = bool_exp1->nextSib->nextSib;
}
// void main(){
//     node* t = (node*) malloc(sizeof(node));
//     t->place = newtemp();
//     update_code(t,t->place);
//     printf("%s\n", t->code);
//     update_code(t," ");
//     update_code(t,newtemp());
//     update_code(t," hello from the other side\nAtleast i can say that i tried\n");
//     update_code(t,newlabel());
//     update_code(t,":\nThis is new world!!\n");
//     printf("%s\n", t->code);
// }