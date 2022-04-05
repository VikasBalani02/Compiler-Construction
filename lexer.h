/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include<stdio.h>
#include"lexerDef.h"
void setToken(Token token, Tokentype type, char* lexeme, int tag, Number * value);
FILE* initialize_lexer(char *input_code_file);
Token getNextToken(FILE** fp);
void removeComments(char *testcaseFile);
