/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern NODE lookUpTable;

char *non_terminal_map[] = {
    "program",
    "otherFunctions",
    "mainFunction",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "primitiveDatatype",
    "constructedDatatype",
    "remaining_list",
    "stmts",
    "typeDefinitions",
    "typeDefinition",
    "actualOrRedefined",
    "fieldDefinitions",
    "fieldDefinition",
    "moreFields",
    "fieldType",
    "declarations",
    "declaration",
    "global_or_not",
    "otherStmts",
    "stmt",
    "assignmentStmt",
    "oneExpansion",
    "moreExpansions",
    "option_single_constructed",
    "singleOrRecId",
    "funCallStmt",
    "outputParameters",
    "inputParameters",
    "iterativeStmt",
    "conditionalStmt",
    "elsePart",
    "expPrime",
    "term",
    "termPrime",
    "factor",
    "highPrecedenceOperators",
    "lowPrecedenceOperators",
    "ioStmt",
    "arithmeticExpression",
    "booleanExpression",
    "var",
    "logicalOp",
    "relationalOp",
    "returnStmt",
    "optionalReturn",
    "idList",
    "more_ids",
    "definetypestmt",
    "A"};

char *terminal_map[] = {
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
    "DOLLAR",
    "ERROR",
    "eps"}; // DOLLAR corresponds to end of input and not the dollar sign

symbol_node *initialize_symbol_node(int value, int is_term)
{
    symbol_node *node = (symbol_node *)malloc(sizeof(symbol_node));
    if (is_term)
    {
        node->t = value;
    }
    else
    {
        node->nt = value;
    }
    node->is_terminal = is_term;
    node->next = NULL;
    return node;
}

symbol_LL *initialize_symbol_ll()
{
    symbol_LL *temp = (symbol_LL *)malloc(sizeof(symbol_LL));
    temp->rule_no = 0;
    temp->head = NULL;
    temp->tail = NULL;
    temp->next_list = NULL;
    temp->len = 0;
    return temp;
}

rule *initialize_rule(NON_TERMINAL nt)
{
    rule *temp = (rule *)malloc(sizeof(rule));
    temp->len = 0;
    temp->nt = nt;
    temp->head = NULL;
    temp->tail = NULL;
    return temp;
}

grammar *initialize_grammar()
{
    grammar *g = (grammar *)malloc(sizeof(grammar));
    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        g->rules[i] = initialize_rule(i);
    }
    g->total_number_of_rules = 0;
    return g;
}
void add_symbol_to_symbol_ll(symbol_LL *list, symbol_node *node)
{
    if (list->head == NULL)
    { // list is empty
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;       // add the symbol node to end of the list
        list->tail = list->tail->next; // update the tail pointer to point to the last node in the list
    }
    list->len += 1;
}

void add_symbol_ll_to_rule(grammar *g, symbol_LL *list, NON_TERMINAL nt)
{
    // associate the symbol list to production rules of non terminal
    rule *r = g->rules[nt];
    if (r->head == NULL)
    {
        // the non terminal didnt have any symbol list associated with it
        r->head = list;
        r->tail = list;
    }
    else
    {
        r->tail->next_list = list;
        r->tail = r->tail->next_list;
    }
    r->len += 1;
    g->total_number_of_rules += 1;
}

NON_TERMINAL map_string_to_nonterminal(char *str)
{
    NON_TERMINAL nt = -1;
    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        if (strcmp(str, non_terminal_map[i]) == 0)
        {
            nt = i;
            break;
        }
    }
    if (nt == -1)
        printf("\n%s", str);
    return nt; // returns -1 if string doesnt map to any non terminal
}
Tokentype map_string_to_terminal(char *str)
{
    Tokentype t = -1;
    for (int i = 0; i < TOTAL_NUMBER_OF_TERMINALS; i++)
    {
        if (strcmp(str, terminal_map[i]) == 0)
        {
            t = i;
            break;
        }
    }
    return t; // returns -1 if string doesnt map to any terminal
}

grammar *load_grammar(FILE *fptr)
{
    if (fptr == NULL)
    {
        printf("\nERROR: grammmar file not opened properly");
        exit(1);
    }
    char *non_terminal_buf = (char *)malloc(sizeof(char) * MAX_NONTERMINAL_LENGTH);
    char *terminal_buf = (char *)malloc(sizeof(char) * MAX_TERMINAL_LENGTH);
    char c;

    grammar *g = initialize_grammar();
    while (!feof(fptr))
    {
        memset(non_terminal_buf, '\0', sizeof(char) * MAX_NONTERMINAL_LENGTH);
        memset(terminal_buf, '\0', sizeof(char) * MAX_TERMINAL_LENGTH);
        while (c = fgetc(fptr))
        {
            if (c == EOF)
                return g;
            else if (c == '<')
            { //<A->bad|Abc
                // <B>-><A><d>abc
                ungetc(c, fptr);
                break;
            }
        }

        fscanf(fptr, "<%s>", non_terminal_buf);
        non_terminal_buf[strlen(non_terminal_buf) - 1] = '\0';
        NON_TERMINAL nt_for_rule = map_string_to_nonterminal(non_terminal_buf);
        if (nt_for_rule == -1)
        {
            printf("\nERROR: unrecognised non terminal in grammar");
            exit(1);
        }
        // read '==>' in the production rule
        c = fgetc(fptr);
        while (c == ' ' || c == '=' || c == '\t' || c == '>')
        {
            c = fgetc(fptr);
        }
        ungetc(c, fptr);

        symbol_LL *list = initialize_symbol_ll();
        while (1)
        {
            while ((c = fgetc(fptr)) == ' ')
                ;
            if (c == EOF || c == '\n' || c == '$')
                break;

            if (c == '<')
            { // start of a non terminal
                ungetc(c, fptr);
                memset(non_terminal_buf, '\0', sizeof(char) * MAX_NONTERMINAL_LENGTH);
                fscanf(fptr, "<%s>", non_terminal_buf);
                non_terminal_buf[strlen(non_terminal_buf) - 1] = '\0';
                NON_TERMINAL nt = map_string_to_nonterminal(non_terminal_buf);
                if (nt == -1)
                {
                    printf("\nERROR: unrecognised non terminal in grammar");
                    exit(1);
                }
                symbol_node *node = initialize_symbol_node(nt, 0);
                add_symbol_to_symbol_ll(list, node);
            }
            else if (c == '|')
            {
                add_symbol_ll_to_rule(g, list, nt_for_rule);
                list = initialize_symbol_ll();
            }
            else
            {
                memset(terminal_buf, '\0', sizeof(char) * MAX_TERMINAL_LENGTH);
                ungetc(c, fptr);
                fscanf(fptr, "%s ", terminal_buf); // end of a line should have ' ' character
                Tokentype t = map_string_to_terminal(terminal_buf);
                if (t == -1)
                {
                    printf("/nERROR: unrecognised terminal in grammar");
                    exit(1);
                }
                symbol_node *node = initialize_symbol_node(t, 1);
                add_symbol_to_symbol_ll(list, node);
            }
        }
        add_symbol_ll_to_rule(g, list, nt_for_rule);
    }
    return g;
}

void assign_rule_no(grammar *g)
{
    int rule_no=1;
    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        rule *r = g->rules[i];
        for (symbol_LL *ptr = r->head; ptr != NULL; ptr = ptr->next_list)
        {
            ptr->rule_no = rule_no++;
        }
    }
}

void print_grammar(grammar *g)
{
    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        rule *r = g->rules[i];
        printf("\n<%s> ===> ", non_terminal_map[r->nt]);
        for (symbol_LL *ptr = r->head; ptr != NULL; ptr = ptr->next_list)
        {
            printf("R:%d ", ptr->rule_no);
            symbol_node *temp = ptr->head;
            for (temp; temp != NULL; temp = temp->next)
            {
                if (temp->is_terminal)
                {
                    printf("%s ", terminal_map[temp->t]);
                }
                else
                {
                    printf("<%s> ", non_terminal_map[temp->nt]);
                }
            }
            if (ptr->next_list != NULL)
                printf("| ");
        }
        printf("\n");
    }
}

// FIRST AND FOLLOW
void printFirstOrFollowSets(char **sets)
{
    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        printf("\n%25s : ", non_terminal_map[i]);
        for (int j = 0; j < TOTAL_NUMBER_OF_TERMINALS; j++)
        {
            if (sets[i][j])
            {
                printf("%s, ", terminal_map[j]);
            }
        }
    }
}
FnF *ComputeFirstAndFollowSets(grammar *gr)
{

    // Initialize the first and follow sets in result object
    FnF *result = init_fnf();

    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        get_First(gr, (NON_TERMINAL)(i), result->firstSet);
    }
    // Get followSet for all non-terminals in given grammar
    // printFirstOrFollowSets(result->firstSet);
    get_Follow(gr, result->firstSet, result->followSet);
    // printf("\n\n\n\n\n\n----------FOLLOW SETS---------------");
    // printFirstOrFollowSets(result->followSet);
    return result;
}

void get_First(grammar *gr, NON_TERMINAL nt, char **firstSet)
{
    // production rule corresponding to the specific non-terminal
    rule *rl = gr->rules[nt];

    // first subrule
    symbol_LL *smb_ll = rl->head;

    // iterating through subrules in rule corresponding to non-terminal

    for (int i = 0; i < rl->len; i++)
    {
        int flag = 0;
        // current symbol pointer
        symbol_node *smb = smb_ll->head;

        // iterating through symbols in the symbol-linkedlist
        for (int j = 0; j < smb_ll->len; j++)
        {
            if (smb->is_terminal)
            {
                firstSet[nt][smb->t] = 1;
                flag = 1;
                break;
            }

            // get the firstSet of the non-terminal
            else
            {
                get_First(gr, smb->nt, firstSet);
                combine_sets(firstSet[nt], firstSet[smb->nt]);
                if (firstSet[smb->nt][EPSILON] == 0)
                {
                    flag = 1;
                    break;
                }
            }
            smb = smb->next;
        }
        if (!flag)
            firstSet[nt][EPSILON] = 1;
        smb_ll = smb_ll->next_list;
    }
}

FnF *init_fnf()
{
    FnF *fnf = (FnF *)malloc(sizeof(FnF));

    fnf->firstSet = (char **)malloc(TOTAL_NUMBER_OF_NONTERMINALS * sizeof(char *));
    fnf->followSet = (char **)malloc(TOTAL_NUMBER_OF_NONTERMINALS * sizeof(char *));

    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        fnf->firstSet[i] = (char *)malloc(sizeof(char) * TOTAL_NUMBER_OF_TERMINALS);
        fnf->followSet[i] = (char *)malloc(sizeof(char) * TOTAL_NUMBER_OF_TERMINALS);

        for (int j = 0; j < TOTAL_NUMBER_OF_TERMINALS; j++)
        {
            fnf->firstSet[i][j] = fnf->followSet[i][j] = 0;
        }
    }
    return fnf;
}

// takes union of two sets
void combine_sets(char *set1, char *set2)
{
    for (int i = 0; i < TOTAL_NUMBER_OF_TERMINALS; i++)
    {
        if ((Tokentype)i == EPSILON)
            continue;
        set1[i] = (set1[i] || set2[i]) ? 1 : 0;
    }
}

void get_Follow(grammar *gr, char **firstSet, char **followSet)
{
    followSet[program][DOLLAR] = 1;
    int counter = gr->total_number_of_rules;
    while (counter--)
    {
        for (int nt = 0; nt < TOTAL_NUMBER_OF_NONTERMINALS; nt++)
        {
            rule *rl = gr->rules[nt];
            // first subrule
            symbol_LL *smb_ll = rl->head;
            for (int j = 0; j < rl->len; j++)
            {
                symbol_node *smb = smb_ll->head;
                for (int k = 0; k < smb_ll->len; k++)
                {
                    // smb is not terminal
                    if (!smb->is_terminal)
                    {
                        symbol_node *smb_next = smb->next;
                        if (smb_next == NULL)
                        {
                            combine_sets(followSet[smb->nt], followSet[nt]);
                        
                        }
                        while (smb_next != NULL)
                        {
                            if (smb_next == NULL || (smb_next->next==NULL) && (smb_next->is_terminal==0)&&(firstSet[smb_next->nt][EPSILON]))
                            {
                                combine_sets(followSet[smb->nt], followSet[nt]);
                            }

                            // next symbol is terminal
                            if (smb_next->is_terminal)
                            {
                                if (followSet[smb->nt][smb_next->t] == 0)
                                {
                                    followSet[smb->nt][smb_next->t] = 1;
                                }
                                break;
                            }
                            // next symbol is non-terminal
                            else
                            {
                                combine_sets(followSet[smb->nt], firstSet[smb_next->nt]);
                                if (firstSet[smb_next->nt][EPSILON] == 0)
                                    break;
                            }
                            smb_next = smb_next->next;
                        }
                    }
                    smb = smb->next;
                }
                smb_ll = smb_ll->next_list;
            }
        }
    }
}

//////FIRST & FOLLOW SETS COMPUTATION NO MORE//////

// PARSE TABLE
void errorCheck(parseTable *pt, NON_TERMINAL i, Tokentype k, symbol_LL *list_to_insert)
{
    if (pt->pTable[i][k] != NULL && pt->pTable[i][k]->head != list_to_insert)
    {
        printf("\nMultiple entries in 1 block of the parse table for NT = %s and T = %s", non_terminal_map[i], terminal_map[k]);
        exit(-1);
    }
}
void print_list(symbol_LL *list)
{
    symbol_node *temp = list->head;
    while (temp != NULL)
    {
        if (temp->is_terminal)
        {
            printf("%s ", terminal_map[temp->t]);
        }
        else
            printf("<%s> ", non_terminal_map[temp->nt]);
        temp = temp->next;
    }
}
void printTable(parseTable *pt)
{
    for (int i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        for (int j = 0; j < TOTAL_NUMBER_OF_TERMINALS - 2; j++)
        {
            if (pt->pTable[i][j] != NULL)
            {
                printf("\n<%s> %s| <%s>-->", non_terminal_map[i], terminal_map[j], non_terminal_map[i]);
                print_list(pt->pTable[i][j]->head);
            }
            else
            {
                // printf("\nFor NT = %s and T= %s, rule doesn't exist\n", non_terminal_map[i], terminal_map[j]);
            }
        }
    }
}

parseTable *parseTableCreate(grammar *g, FnF *fnf)
{
    int i, j, k;
    parseTable *pt = (parseTable *)malloc(sizeof(parseTable));
    for (i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        for (j = 0; j < TOTAL_NUMBER_OF_TERMINALS - 2; j++)
        {
            pt->pTable[i][j] = NULL;
        }
    }

    for (i = 0; i < TOTAL_NUMBER_OF_NONTERMINALS; i++)
    {
        rule *rule_for_nt_i = g->rules[i];
        symbol_LL *curr_sym_list = rule_for_nt_i->head;
        while (curr_sym_list != NULL)
        {
            symbol_node *curr_sym_node = curr_sym_list->head;
            while (curr_sym_node != NULL)
            {
                if (!(curr_sym_node->is_terminal))
                {
                    for (k = 0; k < TOTAL_NUMBER_OF_TERMINALS - 2; k++)
                    {
                        if (fnf->firstSet[curr_sym_node->nt][k])
                        {
                            errorCheck(pt, i, k, curr_sym_list);
                            pt->pTable[i][k] = createBlock(curr_sym_list);
                        }
                    }
                    if (fnf->firstSet[curr_sym_node->nt][EPSILON])
                    { // A->Bc and first set of B has eps
                        curr_sym_node = curr_sym_node->next;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                { // it is a terminal
                    if (curr_sym_node->t != EPSILON)
                    {
                        k = curr_sym_node->t;
                        errorCheck(pt, i, k, curr_sym_list);
                        pt->pTable[i][k] = createBlock(curr_sym_list);
                    }
                    else
                    {
                        for (k = 0; k < TOTAL_NUMBER_OF_TERMINALS - 2; k++)
                        {
                            if (fnf->followSet[i][k]) // A->eps check follow set of A
                            {
                                errorCheck(pt, i, k, curr_sym_list);
                                pt->pTable[i][k] = createBlock(curr_sym_list);
                            }
                        }
                    }
                    break;
                }
            }
            if (curr_sym_node == NULL)
            {
                for (k = 0; k < TOTAL_NUMBER_OF_TERMINALS - 2; k++)
                {
                    if (fnf->followSet[i][k])
                    {
                        errorCheck(pt, i, k, curr_sym_list);
                        pt->pTable[i][k] = createBlock(curr_sym_list);
                    }
                }
            }
            curr_sym_list = curr_sym_list->next_list;
        }
    }
    return pt;
}
block *createBlock(symbol_LL *head)
{
    block *b = malloc(sizeof(block));
    b->head = head;
    return b;
}
Tree* parseInputSourceCode(char *testcaseFile, parseTable *T){
    FILE *lex = initialize_lexer(testcaseFile);
    printf("\n****************************Initialised the lexer*********************************\n");
    Tree *parseTree = initialiseParseTree();
    parseTree->root = get_tree_node(program,NULL,0);
    printf("\n****************************Initialised the parser*********************************\n");
    stack * Stack = getStack();
    tNode * dollar = get_tree_node(DOLLAR,NULL,1);
    push_stack_node(Stack,get_stack_node(dollar));
    push_stack_node(Stack,get_stack_node(parseTree->root));

    Token lookahead=getNextToken(&lex);
    while(lookahead!=NULL){
        if(lookahead->lineNo==28){
            int x=5;
            x=x+5;
        }
        if((lookahead->type) == ERROR){
            printf("Line No: %d, Lexical error, Wrong Lexeme: %s\n", lookahead->lineNo, lookahead->lexeme);
            lookahead = getNextToken(&lex);
            // if(lookahead==NULL) break;
            continue;
        }
        // if(lookahead==NULL) continue;
        if((top_tree_node(Stack)->is_terminal) == 1 && (top_tree_node(Stack)->t) == DOLLAR){
            printf("***********Syntax errors, dollar encountered**********\n");
            break;
        }
        tNode * top = pop_tree_node(Stack);
        tNode* top2 = top_tree_node(Stack); //used in recovery from errors
        block * rule;
        int found;
        switch(top->is_terminal){
            case 0:
                rule = T->pTable[top->nt][lookahead->type];
                found = rule==NULL ? 0:1;
                switch(found){
                    case 0:
                        printf("Line No: %d, Syntactical Error, Expected: %s, Got Token: %s\n", lookahead->lineNo,non_terminal_map[top->nt],terminal_map[lookahead->type]);
                        int flag = 0;
                        while(1){
                            //For Follow of Non terminal Check - Same as checking for first of the next token in the stack
                            switch (top2->is_terminal)
                            {
                                case 0:
                                    if(T->pTable[top2->nt][lookahead->type]!=NULL) flag =1;
                                    break;
                                case 1:
                                    if(lookahead->type == top2->t) flag =1; break;
                            }
                            if(flag ==1) break;
                            //if not matching with that as well, we look for more lookahead symbols
                            lookahead = getNextToken(&lex);
                            if(lookahead==NULL) break;
                            //for checking first of non-terminal, if found just push it so that we can get back the rule in the next iteration
                            if(T->pTable[top->nt][lookahead->type]!=NULL){
                                push_stack_node(Stack,get_stack_node(top));
                                break;
                            }
                        } continue; //go at top of while
                    case 1: 
                        bindChildren(top,rule->head);
                        stack * TempStack = getStack();
                        tNode *temp = top->child_first;
                        while(temp!=NULL)
                        {
                            push_stack_node(TempStack,get_stack_node(temp));
                            temp = temp->next_sibling;
                        }
                        while(top_tree_node(TempStack)!=NULL){
                            tNode* temp2 = pop_tree_node(TempStack);
                            push_stack_node(Stack,get_stack_node(temp2));
                        }
                        free(TempStack);
                }
                continue;
            case 1:
                    if(top->t==59) continue; //59 is for eps
                    if(lookahead->type == top->t){
                        top->lex_token = lookahead;
                        break;
                    }
                    else{
                        printf("Line No:%d, Syntactical Error, Expected token: %s, Got token:%s\n",lookahead->lineNo, terminal_map[top->t],lookahead->lexeme);
                        int flag = 0;
                            while(1){
                                //For Follow of Non terminal Check - Same as checking for first of the next token in the stack
                                switch (top2->is_terminal)
                                {
                                    case 0:
                                        if(T->pTable[top2->nt][lookahead->type]!=NULL) flag =1;
                                        break;
                                    case 1:
                                        if(lookahead->type == top2->t) flag =1; break;
                                }
                                if(flag ==1) break;
                                //if not matching with that as well, we look for more lookahead symbols
                                lookahead = getNextToken(&lex);
                                if(!lookahead) break;
                                //for checking first of non-terminal, if found just push it so that we can get back the rule in the next iteration
                                if(lookahead->type == top->t){
                                    push_stack_node(Stack,get_stack_node(top));
                                    break;
                                }
                            } continue;
                    }
            }
        lookahead=getNextToken(&lex);
    }
    if(lookahead==NULL && Stack->head->t_node->is_terminal&&Stack->head->t_node->t==DOLLAR ) printf("Parsing successfully ended\n");
    else printf("Stack was not empty but input was over");
    return parseTree;
}

void printParseTree(tNode* root , char* outfile)
{
    FILE* fptr = fopen(outfile, "a");
    if(fptr==NULL) return;
    if (root == NULL)
        return;
    tNode *child_ptr = root->child_first;
    if(child_ptr){
        printParseTree(root->child_first,outfile);
        child_ptr=child_ptr->next_sibling;
    }
    char dummy_string[]="----";
    char eps[]="eps";
    char* lexeme="\0";
    char* symbol=NULL;
    int lineNum = root->is_terminal && root->lex_token!=NULL?root->lex_token->lineNo:-1;
    char* LeafOrNot[]={"no","yes"};
    char*parent_symbol="\0";
    float value=0;
    if(root->parent){
        parent_symbol=non_terminal_map[root->parent->nt];
    }

    if (root->is_terminal) {
        if(root->lex_token)
        lexeme = root->lex_token->lexeme;
        else if(root->t==EPSILON)
        lexeme =dummy_string;
        symbol=terminal_map[root->t];
        if(root->lex_token && root->lex_token->tag!=-1){
        if(root->lex_token->tag==0)
            value=root->lex_token->val->ival;
        else
            value=root->lex_token->val->fval;   } 
    }
    else {
        lexeme = dummy_string;
        symbol=non_terminal_map[root->nt];
    }
    int rule_no = root->rule_no;
    // fprintf(fptr,"\n %20s %15s %4d %10f %15s %3s",lexeme,symbol,lineNum,value,parent_symbol,LeafOrNot[root->is_terminal]);
    if(root->is_terminal && root->lex_token && root->lex_token->tag==1) 
    {
        fprintf(fptr,"\n %20s %25s %7d %20f %25s %10s %10d",lexeme,symbol,lineNum,value,parent_symbol,LeafOrNot[root->is_terminal], rule_no);

    }  
    else {
        fprintf(fptr,"\n %20s %25s %7d %20d %25s %10s %10d",lexeme,symbol,lineNum,(int) value,parent_symbol,LeafOrNot[root->is_terminal], rule_no);

    }
    fclose(fptr);
    while (child_ptr)
    {
        printParseTree(child_ptr,outfile);
        child_ptr=child_ptr->next_sibling;
    }
}
// int main()
// {
//     FILE *fptr = fopen("grammar.txt", "r");
//     grammar *g = load_grammar(fptr);
//     // print_grammar(g);
//     FnF *fnf = ComputeFirstAndFollowSets(g);
//     // print_grammar(g);
//     parseTable *ptr = parseTableCreate(g, fnf);
//     // printf("\n\n\n");
//     // printTable(ptr);
//     Tree* parseTree = parseInputSourceCode("t2.txt",ptr);
//     // epsilon added to tokentype
// }