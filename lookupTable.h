/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include "lexer.h"
struct node{
    struct node * array[63]; // 0-25 = a-z, 26-51 = A-Z, 52-61 is for 0-9;
    int valid; //0 invalid and 1 valid;
    Tokentype type;
};

typedef struct node * NODE;

NODE newNode();
void addToken(NODE root, char* key, Tokentype type);
NODE lookup(NODE root, char* key);
NODE getLookUpTable();