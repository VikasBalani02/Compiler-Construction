/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include <stdio.h>
#include "lookupTable.h"
#include<stdlib.h>
int totalKeyword = 28;
Tokentype keywordEnum[] = {
		TK_WITH,
		TK_PARAMETERS,
		TK_END,
		TK_WHILE,
        TK_UNION,
        TK_ENDUNION,
        TK_DEFINETYPE,
        TK_AS,
		TK_TYPE,
		TK_MAIN,
		TK_GLOBAL,
		TK_PARAMETER,
		TK_LIST,
        TK_INPUT,
		TK_OUTPUT,
		TK_INT,
		TK_REAL,
		TK_ENDWHILE,
		TK_IF,
		TK_ENDIF,
		TK_THEN,
		TK_READ,
		TK_WRITE,
		TK_RETURN,
		TK_CALL,
		TK_RECORD,
		TK_ENDRECORD,
		TK_ELSE
	};
char* keywordList[] = {
		"with",
		"parameters",
		"end",
		"while",
        "union",
        "endunion",
        "definetype",
        "as",
		"type",
		"_main",
		"global",
		"parameter",
		"list",
		"input",
        "output",
		"int",
		"real",
		"endwhile",
		"if",
		"endif",
		"then",
		"read",
		"write",
		"return",
		"call",
		"record",
		"endrecord",
		"else"
	};

NODE newNode(){
	NODE initial = (NODE)(malloc(sizeof(struct node)));
	for(int i=0;i<62;i++){
		initial->array[i] = NULL;
	}
	initial->valid =0;
}
int getIndex(char c){
	if(c=='_')return 62;
	if(c>='a' && c<='z'){
		return c-'a';
	}
	else if(c>='A' && c<='Z'){
		return (c-'A'+26);
	}
	else return (c-'0'+52);
}
void addToken(NODE root, char* key, Tokentype type)
{
    NODE temp = root;
 
    for (int i = 0; key[i]!='\0'; i++)
    {
        int index = getIndex(key[i]);
        if (!temp->array[index])
            temp->array[index] = newNode();
 
        temp = temp->array[index];
    }
 
    // mark last node as leaf
    temp->valid = 1;
	temp->type = type;
}
NODE lookup(NODE root, char* key)
{
    NODE temp = root;
 
    for (int i = 0; key[i]!='\0'; i++)
    {
        int index = getIndex(key[i]);
        if (!temp->array[index])
            return NULL;
 
        temp = temp->array[index];
    }
 
    if(temp->valid) return temp;
	else return NULL;
}
NODE getLookUpTable(){
    NODE root = newNode();
    for(int i=0;i<totalKeyword; i++){
        addToken(root,keywordList[i],keywordEnum[i]);
    }
    return root;
}