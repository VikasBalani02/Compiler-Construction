/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/
#include "lookupTable.h"
#include<ctype.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
char* tokenMap[] = {
	"TK_ASSIGNOP",
	"TK_COMMENT",
	"TK_FIELDID",
	"TK_ID",
	"TK_NUM",
	"TK_RNUM",
	"TK_FUNID",
	"TK_RUID",
	"TK_WITH",
	"TK_PARAMETERS",
	"TK_END",
	"TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
	"TK_TYPE",
	"TK_MAIN",
	"TK_GLOBAL",
	"TK_PARAMETER",
	"TK_LIST",
	"TK_SQL",
	"TK_SQR",
	"TK_INPUT",
	"TK_OUTPUT",
	"TK_INT",
	"TK_REAL",
	"TK_COMMA",
	"TK_SEM",
	"TK_COLON",
	"TK_DOT",
	"TK_ENDWHILE",
	"TK_OP",
	"TK_CL",
	"TK_IF",
	"TK_THEN",
	"TK_ENDIF",
	"TK_READ",
	"TK_WRITE",
	"TK_RETURN",
	"TK_PLUS",
	"TK_MINUS",
	"TK_MUL",
	"TK_DIV",
	"TK_CALL",
	"TK_RECORD",
	"TK_ENDRECORD",
	"TK_ELSE",
	"TK_AND",
	"TK_OR",
	"TK_NOT",
	"TK_LT",
	"TK_LE",
	"TK_EQ",
	"TK_GT",
	"TK_GE",
	"TK_NE",
    "ERROR"  //Token for error output
};

#define SIZE 2048 
char * lexeme;
int lexeme_ptr;
char* current_buffer;
char* previous_buffer;
int lineNo = 1;
int main_ptr = 0;
NODE lookUpTable;
int file_over = 0; 
int boundary_cond = 0;

void swapBuffers(char **current_buffer, char** previous_buffer){
	char * temp = *current_buffer;
	*current_buffer = *previous_buffer;
	*previous_buffer= temp;
}

FILE* getStream(FILE* fp){
	if(boundary_cond){
		swapBuffers(&current_buffer, &previous_buffer);
		boundary_cond = 0;
		return fp; //previous buffer already has current data so do nothing
	}
	if(feof(fp)){
		if(fp!=NULL)
		close(fileno(fp));
 		return NULL;
 	}
	char* temp = previous_buffer;
	previous_buffer = current_buffer;
	current_buffer = temp;
	int i;
	int count = fread(current_buffer, sizeof(char), SIZE, fp);
	if(count>0) {
		current_buffer[count]='\0';
		return fp;
	}
	else{
		fclose(fp);
		return NULL;
	}
}
void retract(int num){
	if(main_ptr>=num) main_ptr-=num;
	else {
		boundary_cond = 1;
		int len = strlen(previous_buffer);
		main_ptr = len - (num-main_ptr);
		swapBuffers(&current_buffer,&previous_buffer);
	}
}
int checkCurrentPointer(char c){
	return current_buffer[main_ptr]==c;
}
int checkBufferEnd(FILE **fp){
	if(checkCurrentPointer('\0')){
		main_ptr = 0;
		*fp = getStream(*fp);
		if(*fp==NULL){
			file_over = 1;
			return 1;
		} 
	}
	return 0;
}
void setLexeme(){
	lexeme[lexeme_ptr] = current_buffer[main_ptr];
	lexeme_ptr++;
}
Token getNextToken(FILE** fp){
	if(file_over){
		if(*fp!=NULL) //if file not closed
			fclose(*fp);
		return NULL;
	}

	Token token = (Token)malloc(sizeof(struct token));

	if(checkBufferEnd(fp)) return NULL;

	int state = 0;
	int final_state_reached = 0;
	lexeme = (char*)malloc(100*sizeof(char));
	lexeme_ptr = 0;
    while(1){
		switch(state){

			case 0:
			if(checkBufferEnd(fp)) return NULL;
			else if(checkCurrentPointer(' ')|| checkCurrentPointer('\t') ){
					state = 58;
					main_ptr++;
				}	
            else if(checkCurrentPointer('\n')){
                state=64;
                main_ptr++;

            }	
			else if(checkCurrentPointer('@')){
                setLexeme();
                state = 1;
                main_ptr++;
				}

            else if(checkCurrentPointer('&')){
                setLexeme();
                state = 4;
                main_ptr++;
            }
            else if(checkCurrentPointer('+')){
                state = 7;
                main_ptr++;
            }

            else if(checkCurrentPointer('-')){
                state = 8;
                main_ptr++;
            }

            else if(checkCurrentPointer('*')){
                state = 9;
                main_ptr++;
            }

            else if(checkCurrentPointer('/')){
                state = 10;
                main_ptr++;
            }
            else if(checkCurrentPointer('[')){
                state = 11;
                main_ptr++;
            }
            else if(checkCurrentPointer(']')){
                state = 12;
                main_ptr++;
            }
            else if(checkCurrentPointer('(')){
                state = 13;
                main_ptr++;
            }

            else if(checkCurrentPointer(')')){
                state = 14;
                main_ptr++;
            }
            else if(checkCurrentPointer('~')){
                state = 15;
                main_ptr++;
            }
            else if(checkCurrentPointer(',')){
                state = 16;
                main_ptr++;
            }

            else if(checkCurrentPointer(';')){
                state = 17;
                main_ptr++;
            }

            else if(checkCurrentPointer(':')){
                state = 18;
                main_ptr++;
            }

            else if(checkCurrentPointer('.')){
                state = 19;
                main_ptr++;
            }
            else if(checkCurrentPointer('!')){
                setLexeme();
                state = 21;
                main_ptr++;
            }
            else if(checkCurrentPointer('=')){
                setLexeme();
                state = 23;
                main_ptr++;
            }
            else if(checkCurrentPointer('<')){
                setLexeme();
                state = 25;
                main_ptr++;
            } 
            else if(checkCurrentPointer('>')){
                setLexeme();
                state = 31;
                main_ptr++;
            }
            else if((checkCurrentPointer('a'))||((current_buffer[main_ptr]>='e')&&(current_buffer[main_ptr]<='z'))){
                setLexeme();
                state = 34;
                main_ptr++;
            }
            else if((checkCurrentPointer('b'))||(checkCurrentPointer('c'))||(checkCurrentPointer('d'))){
                setLexeme();
                state = 36;
                main_ptr++;
            }
            else if(checkCurrentPointer('_')){
                setLexeme();
                state = 41;
                main_ptr++;
            }
            else if(checkCurrentPointer('#')){
                setLexeme();
                state = 45;
                main_ptr++;
            }
            else if(isdigit(current_buffer[main_ptr])){
                setLexeme();
                state = 48;
                main_ptr++;
            }
            else if(checkCurrentPointer('%')){
                state = 62;
                main_ptr++;
            }
            else{
                lexeme[lexeme_ptr] = current_buffer[main_ptr];
                main_ptr++;
				lexeme_ptr++;
                state = 65;
            }
            break;
            case 1:
                if(checkCurrentPointer('@')){
                    setLexeme();
                    state = 2;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){ 
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    }    
                }
                else{
                    lexeme[lexeme_ptr]='\0';
                    state = 65;
                }
                break;
            case 2:
                if(checkCurrentPointer('@')){
                    setLexeme();
                    state = 3;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    }
                }
                else
                {
                    lexeme[lexeme_ptr]='\0';
                    state = 65;
                break;
            case 3:
                lexeme[lexeme_ptr]='\0';
                setToken(token, TK_OR, lexeme,-1, NULL);
                final_state_reached = 1;
                break;
            case 4:
                if(checkCurrentPointer('&')){
                    setLexeme();
                    state = 5;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    }
                }
                else{
                    lexeme[lexeme_ptr]='\0';
                    state = 65;
                }
                break;
            case 5:
                if(checkCurrentPointer('&')){
                    setLexeme();
                    state = 6;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    } 
                }
                else
                {
                    lexeme[lexeme_ptr]='\0';
                    state = 65;
                }
                break;
            case 6:
                lexeme[lexeme_ptr]='\0';
                setToken(token, TK_AND, lexeme, -1, NULL);
                final_state_reached = 1;
                break;
            case 7:{		
				strcpy(lexeme,"+");
				setToken(token, TK_PLUS, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 8:{		
				strcpy(lexeme,"-");
				setToken(token, TK_MINUS, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 9:{		
				strcpy(lexeme,"*");
				setToken(token, TK_MUL, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 10:{		
				strcpy(lexeme,"/");
				setToken(token, TK_DIV, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 11:{		
				strcpy(lexeme,"[");
				setToken(token, TK_SQL, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;

			case 12:{
				strcpy(lexeme,"]");
				setToken(token, TK_SQR, lexeme, -1, NULL);	
				final_state_reached = 1;
            }break;
            case 13:{		
				strcpy(lexeme,"(");
				setToken(token, TK_OP, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 14:{		
				strcpy(lexeme,")");
				setToken(token, TK_CL, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
            case 15:{		
				strcpy(lexeme,"~");
				setToken(token, TK_NOT, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
            case 16:{		
				strcpy(lexeme,",");
				setToken(token, TK_COMMA, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;	
			case 17:{		
				strcpy(lexeme,";");
				setToken(token, TK_SEM, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 18:{		
				strcpy(lexeme,":");
				setToken(token, TK_COLON, lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
			case 19:{		
				strcpy(lexeme,".");
				setToken(token, TK_DOT,lexeme, -1, NULL);	
				final_state_reached = 1;
			}break;
            case 21:
				if(checkCurrentPointer('=')){
					setLexeme();
					state = 22;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 65;					
					}
				}
				else
				{
					lexeme[lexeme_ptr]='\0';
					state = 65;
				}
				break;
			case 22:
                lexeme[lexeme_ptr]='\0';
                setToken(token, TK_NE, lexeme,-1, NULL);
                final_state_reached = 1;
                break;
            case 23:
                if(checkCurrentPointer('=')){
                    setLexeme();
                    state = 24;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    }
                }
                else
                {
                    lexeme[lexeme_ptr]='\0';
                    state = 65;
                }
                break;
            case 24:
                lexeme[lexeme_ptr]='\0';
                setToken(token, TK_EQ, lexeme, -1, NULL);
                final_state_reached = 1;
                break;
            case 25:
				if(checkCurrentPointer('=')){
					setLexeme();
					state = 26;
					main_ptr++;
				} 
				else if(checkCurrentPointer('-')){
					setLexeme();
					state = 27;
					main_ptr++;
				}
                else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    }
                }
				else{
					state = 30;
					main_ptr++;
				}
				break;
			case 26:
				lexeme[lexeme_ptr]='\0';
				setToken(token, TK_LE, lexeme, -1, NULL);	
				final_state_reached = 1;
				break;
			case 27:
				if(checkCurrentPointer('-')){
					setLexeme();
					state = 28;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 65;					
					}
				}
				else{
					state=60;
				}

				break;
            case 28:
				if(checkCurrentPointer('-')){
					setLexeme();
					state = 29;
					main_ptr++;
				}
				else
				{
					state = 61;
				}
				break;
            case 29:
				lexeme[lexeme_ptr]='\0';
				setToken(token, TK_ASSIGNOP, lexeme, -1, NULL);
				final_state_reached = 1;
				break;
            case 30:
				strcpy(lexeme,"<");
				setToken(token, TK_LT, lexeme, -1, NULL);	
				retract(1);
				final_state_reached = 1;
				break;
            case 31:
                if(checkCurrentPointer('=')){
                    setLexeme();
                    state = 32;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        lexeme[lexeme_ptr]='\0';
                        file_over = 1;
                        state = 65;
                    }
                }
                else{
                    state = 33;
                    main_ptr++;
                }
                break;
            case 32:
                lexeme[lexeme_ptr]='\0';
                setToken(token, TK_GE, lexeme, -1, NULL);
                final_state_reached = 1;
                break;
            case 33:
                strcpy(lexeme,">");
                setToken(token, TK_GT, lexeme, -1, NULL);
                retract(1);
                final_state_reached = 1;
                break;
            case 34:
				if((current_buffer[main_ptr]>='a') && (current_buffer[main_ptr]<='z')){
					setLexeme();
					state = 34;
					main_ptr++;
				}

				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 35;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';	
					main_ptr++;					
					state = 35;
				}
				break;
			case 35:
				lexeme[lexeme_ptr] = '\0';
                retract(1);

				NODE search = lookup(lookUpTable,lexeme); 

				if(search==NULL){
					token->type = TK_FIELDID;
					addToken(lookUpTable,lexeme,TK_FIELDID);
					setToken(token,TK_FIELDID,lexeme,-1,NULL);
				}
					
				else
					token->type = search->type;

				setToken(token, token->type, lexeme, -1, NULL);	
				final_state_reached = 1;
                break;
			case 36:
				if((current_buffer[main_ptr]>='a') && (current_buffer[main_ptr]<='z')){
					setLexeme();
					state = 34;
					main_ptr++;
				}
				else if((current_buffer[main_ptr]>='2') && (current_buffer[main_ptr]<='7')){
					setLexeme();
					state = 37;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 65;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';						
					state = 35;
				}
				break;
			case 37:
				if((current_buffer[main_ptr]>='b') && (current_buffer[main_ptr]<='d')){
					setLexeme();
					state = 38;
					main_ptr++;
				}
				else if((current_buffer[main_ptr]>='2') && (current_buffer[main_ptr]<='7')){
					setLexeme();
					state = 40;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 39;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';	
					main_ptr++;					
					state = 39;
				}
				break;
			
            case 38:
                if((current_buffer[main_ptr]>='b') && (current_buffer[main_ptr]<='d')){
					setLexeme();
					state = 38;
					main_ptr++;
				}
				else if((current_buffer[main_ptr]>='2') && (current_buffer[main_ptr]<='7')){
					setLexeme();
					state=40;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 39;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';	
					main_ptr++;					
					state = 39;
				}
				break;
			case 39:
				lexeme[lexeme_ptr] = '\0';
                retract(1);
				if(strlen(lexeme)>20){
					fprintf(stderr, "Line No: %d, Lexical Error, Identifier is longer than the prescribed length of 20 characters\n",lineNo);
					state = 65;
				}
				else{
                	setToken(token, TK_ID, lexeme, -1, NULL);
                	final_state_reached = 1;
				}
                break;
            case 40:
				if((current_buffer[main_ptr]>='2') && (current_buffer[main_ptr]<='7')){
					setLexeme();
					state = 40;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 39;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';	
					main_ptr++;					
					state = 39;
				}
				break;
            case 41:
				if(((current_buffer[main_ptr]>='a') && (current_buffer[main_ptr]<='z'))||((current_buffer[main_ptr]>='A') && (current_buffer[main_ptr]<='Z'))){
					setLexeme();
					state = 42;
					main_ptr++;
				}

				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 65;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';						
					state = 65;
				}
				break;
			case 42:
				if(((current_buffer[main_ptr]>='a') && (current_buffer[main_ptr]<='z'))||((current_buffer[main_ptr]>='A') && (current_buffer[main_ptr]<='Z'))){
					setLexeme();
					state = 42;
					main_ptr++;
				}
				else if(isdigit(current_buffer[main_ptr])){
					setLexeme();
					state = 43;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 65;					
					}
				}
				else{						
					state = 44; 
					main_ptr++;
				}
				break;
			case 43:
				if(isdigit(current_buffer[main_ptr])){
					setLexeme();
					state = 43;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 44;					
					}
				}
				else{
					state = 44;
					main_ptr++;
				}
				break;
			case 44:
				lexeme[lexeme_ptr] = '\0';
                retract(1);
                if(strlen(lexeme)>30){
					fprintf(stderr, "Line No: %d, Lexical Error, Identifier is longer than the prescribed length of 30 characters\n",lineNo);
					state = 65;
				}
				else{
					NODE search = lookup(lookUpTable, lexeme);

					if(search==NULL){
						token->type = TK_FUNID;
						addToken(lookUpTable,lexeme,TK_FUNID);
					}
					else
						token->type = search->type;

					setToken(token, token->type, lexeme, -1, NULL);	
					final_state_reached = 1;
                    }
                break;
            case 45:
				if((current_buffer[main_ptr]>='a') && (current_buffer[main_ptr]<='z')){
					setLexeme();
					state = 46;
					main_ptr++;
				}

				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 65;					
					}
				}
				else{
					lexeme[lexeme_ptr]='\0';						
					state = 65;
				}
				break;
			case 46:
				if((current_buffer[main_ptr]>='a') && (current_buffer[main_ptr]<='z')){
					setLexeme();
					state = 46;
					main_ptr++;
				}

				else if(checkCurrentPointer('\0')){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){
						lexeme[lexeme_ptr]='\0';
						file_over = 1;
						state = 47;					
					}
				}
				else{						
					state = 47;
					main_ptr++;
				}
				break;
			case 47:
				lexeme[lexeme_ptr] = '\0';
                setToken(token, TK_RUID, lexeme, -1, NULL);
                retract(1);
                final_state_reached = 1;
                break;
            case 48:
                if(isdigit(current_buffer[main_ptr])){
                    setLexeme();
                    state = 48;
                    main_ptr++;
                }
                
                else if(checkCurrentPointer('.')){
                    setLexeme();
                    state = 50;
                    main_ptr++;
                }
                else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
                        state = 49;
                        main_ptr++;
                        file_over = 1;
                    }
                }
                else{
                    state = 49;
                    main_ptr++;
                }
                
                break;
            case 49:
                lexeme[lexeme_ptr] = '\0';
                int value = atoi(lexeme);
                token->val = (Number *)malloc(sizeof(Number));
                token->val->ival = value;
                setToken(token, TK_NUM, lexeme, 0, token->val);
                retract(1);
                final_state_reached = 1;
                break;
			case 50:
				if(isdigit(current_buffer[main_ptr])){
                    setLexeme();
					state = 51;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 65;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 65;
				}
				break;
			case 51:
				if(isdigit(current_buffer[main_ptr])){
                    setLexeme();
					state = 52;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 65;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 65;
					main_ptr++;
				}
				break;
            case 52:
                if(checkCurrentPointer('E')){
                    setLexeme();
					state = 54;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 53;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 53;
					main_ptr++;
				}
				break;
                

			case 53:
				lexeme[lexeme_ptr] = '\0';
                float fval2 = (float)atof(lexeme);
                token->val = (Number *)malloc(sizeof(Number));
                token->val->fval = fval2;
                retract(1);
                setToken(token, TK_RNUM, lexeme, 1, token->val);
                final_state_reached = 1;
                break;
            case 54:
              if((checkCurrentPointer('+')) || (checkCurrentPointer('-')) ){
                    setLexeme();
					state = 55;
					main_ptr++;
				}
               else if(isdigit(current_buffer[main_ptr])){
                   setLexeme();
					state = 56;
					main_ptr++;

               }
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 65;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 65;
				}
				break;

            case 55:
            if(isdigit(current_buffer[main_ptr])){
                    setLexeme();
					state = 56;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 65;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 65;
				}
				break;
            case 56:
              if(isdigit(current_buffer[main_ptr])){
                    setLexeme();
					state = 57;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 65;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 65;
				}
				break;
            case 57:
                lexeme[lexeme_ptr] = '\0';
                float fval = (float)atof(lexeme);
                token->val = (Number *)malloc(sizeof(Number));
                token->val->fval = fval;
                setToken(token, TK_RNUM, lexeme, 1, token->val);
                final_state_reached = 1;
                break;
            case 58:
                if(checkCurrentPointer(' ')|| checkCurrentPointer('\t')){
					state = 58;
					main_ptr++;
				}
				else if(checkCurrentPointer('\0')){
                    main_ptr = 0;
                    *fp = getStream(*fp);
                    if(*fp==NULL){
						lexeme[lexeme_ptr] = '\0';						
                        state = 59;
                        file_over = 1;
                    }
                }
				else{
					lexeme[lexeme_ptr] = '\0';
					state = 59;
					main_ptr++;
				}
				break;
            case 59:
                retract(1);
				state = 0;
                break;
            case 60:
               strcpy(lexeme,"<-");
			   setToken(token, ERROR, lexeme, -1, NULL);	
                final_state_reached = 1;
                break;
            case 61:
                strcpy(lexeme,"<--");
			    setToken(token, ERROR, lexeme, -1, NULL);	
                final_state_reached = 1;
                break;
            case 62:
				if(checkCurrentPointer('\n')){
					lineNo++;
					state = 0;
					main_ptr++;
                }
				else if(checkCurrentPointer('\0')|| checkCurrentPointer(-1)){
					main_ptr = 0;
					*fp = getStream(*fp);
					if(*fp==NULL){					
						return NULL;					
					}
				}
				else{
					state = 62;
					main_ptr++;
				}
				break;
            case 64:
				lineNo++;
				state = 0; break;
            case 65:
				lexeme[lexeme_ptr] = '\0';
				setToken(token, ERROR, lexeme, -2, NULL);
				final_state_reached = 1;
				state=0;
				break;
            }
            if(final_state_reached==1)
			break;
	}
	if(final_state_reached==1) return token;
	else continue;
}
}
FILE* initialize_lexer(char *input_code_file){
	current_buffer = (char*)malloc(sizeof(char)*(SIZE+1));
	previous_buffer = (char*)malloc(sizeof(char)*(SIZE+1));
	int i;
	for(i=0;i<SIZE;i++)
		current_buffer[i] = '\0';
	for(i=0;i<SIZE;i++)
		previous_buffer[i] = '\0';
    lineNo=1; 
	file_over = 0;  
	main_ptr = 0;  
	lookUpTable = getLookUpTable();

	FILE *fp = fopen(input_code_file, "r");
	
	if(fp == NULL){
		fprintf(stderr,"Error Opening File\n");
		return NULL;
	}

	else{
		printf("\n\n-File Opened Successfully For Parsing-\n\n");
	}
	
    return fp;

}

void printToken(Token token){
	if(token->tag==0)
	printf("\n Token : %s\t LineNo:%d\t Lexeme:%s\t Tag:%d, Val:%d",tokenMap[token->type], token->lineNo, token->lexeme, token->tag, token->val->ival);
	else if(token->tag==1)
	printf("\n Token : %s\t LineNo:%d\t Lexeme:%s\t Tag:%d Val:%f",tokenMap[token->type], token->lineNo, token->lexeme, token->tag, token->val->fval);
	else
	printf("\n Token : %s\t LineNo:%d\t Lexeme:%s\t Tag:%d",tokenMap[token->type], token->lineNo, token->lexeme, token->tag);
}
void setToken(Token token, Tokentype type, char* lexeme, int tag, Number * value){
	token->lineNo = lineNo;
	token->type = type;
	token-> lexeme = lexeme;
	token-> tag = tag;
	token->val = value;
}


void removeComments(char *testcaseFile){
	printf("We have removed all newlines and tabs as a part of the preprocessing step.");
	current_buffer = (char*)malloc(sizeof(char)*(SIZE+1));
	previous_buffer = (char*)malloc(sizeof(char)*(SIZE+1));
	
	memset(current_buffer, 0, sizeof(current_buffer));
	memset(previous_buffer, 0, sizeof(previous_buffer));

	FILE* input = fopen(testcaseFile, "r");
	if(input==NULL){
		fprintf(stderr,"File cannot be opened\n");
		return;
	}
	else printf("\nFile opened successfully\n\n");
	char ch;
    lineNo=1; file_over = 0;  main_ptr = 0; 
	input = getStream(input);
	int end_of_file = 0;
	int flag = 1;
	for(int i=0;i<INT_MAX;i++){

		if(current_buffer[i]=='\0'|| checkCurrentPointer(-1)){
			input = getStream(input);
			if(input==NULL){
				break;
			}
			i=0;
		}		

		if(current_buffer[i]=='%'){
			i++;
			while(current_buffer[i]!='\n'){

				if(current_buffer[i]=='\0'|| checkCurrentPointer(-1)){
					input = getStream(input);
					if(input==NULL){
						end_of_file = 1;
						break;
					}
					i= -1;
				}
				i++;
			}
		}

		if(current_buffer[i]!='\n')
			flag = 0;
        if(current_buffer[i]=='\n' || current_buffer[i] == '\t')
			flag = 1;
		if(current_buffer[i]==-1)
			break;
		if(!(flag==1 && (current_buffer[i]=='\n' || current_buffer[i] == '\t'))){
			printf("%c",current_buffer[i]);			
		}
        if(end_of_file==1)
			break;
	}
	if(input!=NULL)
		fclose(input);

	printf("\n\nFile Over\n");
	
}

// int main(int argc, char*argv[]){
// 	FILE * n = initialize_lexer("test.txt");
// 	while(1){
// 		Token temp = getNextToken(&n);
// 		printToken(temp);
// 		getchar();
// 	}
// }
            
            