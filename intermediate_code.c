#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
char *concat(char *str1, char *str2)
{
    int l1 = 0;
    l1 = strlen(str1);
    int l2 = 0;
    l2 = strlen(str2);
    l1 += (l2 + 5);
    char *temp;
    temp = (char *)malloc(l1 * sizeof(char));
    memset(temp, '\0', sizeof(temp));
    temp = strcat(temp, str1);
    temp = strcat(temp, ".");
    temp = strcat(temp, str2);
    // free karna ho to yaha par kar dena
    return temp;
}
tuple *newTuple(OP op, char *arg1, char *arg2, char *arg3, tuple *next)
{
    tuple *t = (tuple *)malloc(sizeof(tuple));
    t->op = op;
    t->arg1 = arg1;
    t->arg2 = arg2;
    t->arg3 = arg3;
    t->next = next;
    return t;
}
void addTupleEnd(tupleList *list, tuple *t)
{
    if (list->head == NULL)
    {
        list->head = t;
        list->tail = t;
    }
    else
    {
        list->tail->next = t;
        list->tail = t;
    }
}
void addTupleFront(tupleList *list, tuple *t)
{
    if (list->head == NULL)
    {
        list->head = t;
        list->tail = t;
    }
    else
    {
        t->next = list->head;
        list->head = t;
    }
}
tupleList *newList()
{
    tupleList *list = (tupleList *)malloc(sizeof(tupleList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}
char *newtemp()
{
    static temp_no = 0;
    char *str = (char *)malloc(20 * sizeof(char));
    sprintf(str, "t%d", temp_no);
    temp_no = temp_no + 1;
    return str;
}
char *newlabel()
{
    static label_no = 0;
    char *str = (char *)malloc(20 * sizeof(char));
    sprintf(str, "L%d", label_no);
    label_no = label_no + 1;
    return str;
}

// postorder traversal
tupleList *get_intermediate_list(ast_node *root)
{
    tupleList *list = newList();
    createIR(root, list);
    return list;
}
void createIR(ast_node *root, tupleList *list)
{
    if (root == NULL)
        return;
    ast_node *temp = root->firstChild;
    while (temp != NULL)
    {
        createIR(temp, list);
        temp = temp->nextSib;
    }
    IR_for_astnode(root, list);
}
// this is for a specific node
void IR_for_astnode(ast_node *root, tupleList *list)
{
    if (root->construct == booleanExpression_)
    {
        IR_boolean_expression(root);
    }
    else if (root->construct == singleOrRecId_)
    {
        IR_singleOrRecId(root);
    }
    else if (root->construct == lowPrecedenceTerm_)
    {
        IR_lowPrecedenceTerm(root);
    }
    else if (root->construct == highPrecedenceTerm_)
    {
        IR_highPrecedenceTerm(root);
    }

    else if (root->construct == factor_)
    {
        IR_factor(root);
    }
    // else if (root->construct == termPrime_)
    // {
    //     IR_termprime(root, list);
    // }
    // else if (root->construct == expPrime_)
    // {
    //     IR_expPrime(root, list);
    // }
    else if (root->construct == arithmeticExpression_)
    {
        IR_arithematic_statement(root);
    }
    else if (root->construct == ioStmt_)
    {
        IR_io_statement(root);
    }
    else
    {
    }
}
void IR_singleOrRecId(ast_node *root)
{
    char *lexeme = ((struct id_struct *)(root->ninf))->lexID;
    ast_node *temp = root->firstChild;
    while (temp != NULL)
    {
        if (((struct field_id_struct *)(temp->ninf))->fieldid != NULL)
        {
            lexeme = concat(lexeme, ((struct field_id_struct *)(temp->ninf))->fieldid);
        }
        temp = temp->nextSib;
    }
    root->place = lexeme;
    root->list = NULL;
}

void IR_lowPrecedenceTerm(ast_node *root)
{
    char *arg1 = root->firstChild->place;
    char *arg2 = root->firstChild->nextSib->place;
    char *res = newtemp();
    root->place = res;
    // ADD TO SYMBOL TABLE

    tupleList *t1 = root->firstChild->list;
    tupleList *t2 = root->firstChild->nextSib->list;
    tuple *newT;
    if (((struct operator_struct *)(root->ninf))->op == TK_PLUS)
    {
        newT = newTuple(PLUS, arg1, arg2, res, NULL);
    }
    else
    {
        newT = newTuple(MINUS, arg1, arg2, res, NULL);
    }
    if (t1 == NULL)
    {
        if (t2 == NULL)
        {
            tupleList *newL = newList();
            addTupleEnd(newL, newT);
            root->list = newL;
        }
        else
        {
            addTupleEnd(t2, newT);
            root->list = t2;
        }
    }
    else
    {
        if (t2 == NULL)
        {
            addTupleEnd(t1, newT);
            root->list = t1;
        }
        else
        {
            t1->tail->next = t2->head;
            t2->tail->next = newT;
            t1->tail = newT;
            root->list = t1;
        }
    }
}
void IR_highPrecedenceTerm(ast_node *root)
{
    char *arg1 = root->firstChild->place;
    char *arg2 = root->firstChild->nextSib->place;
    char *res = newtemp();
    root->place = res;
    // ADD TO SYMBOL TABLE

    tupleList *t1 = root->firstChild->list;
    tupleList *t2 = root->firstChild->nextSib->list;
    tuple *newT;
    if (((struct operator_struct *)(root->ninf))->op == TK_MUL)
    {
        newT = newTuple(MUL, arg1, arg2, res, NULL);
    }
    else
    {
        newT = newTuple(DIV, arg1, arg2, res, NULL);
    }
    if (t1 == NULL)
    {
        if (t2 == NULL)
        {
            tupleList *newL = newList();
            addTupleEnd(newL, newT);
            root->list = newL;
        }
        else
        {
            addTupleEnd(t2, newT);
            root->list = t2;
        }
    }
    else
    {
        if (t2 == NULL)
        {
            addTupleEnd(t1, newT);
            root->list = t1;
        }
        else
        {
            t1->tail->next = t2->head;
            t2->tail->next = newT;
            t1->tail = newT;
            root->list = t1;
        }
    }
}
// void IR_termprime(ast_node* root, TupleList* list){
//     // termPrime gives an epsilon
//     if(root->firstChild == NULL){
//         root->place = NULL;
//         root->tuple = NULL;
//     }
//     // termPrime gives HPO factor termPrime
//     else{
//         root->place = newtemp();
//         Tuple* temp = root->firstChild->nextSib->nextSib->tuple;
//         Tokentype op = ((struct operator_struct *)(root->firstChild->ninf))->op;
//         Tuple* t;
//         if(op == TK_MUL)
//             t = makeTuple(MUL, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
//         else
//             t = makeTuple(DIV, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
//         if(temp)
//             temp->next = t;
//         add_to_list(t, list);
//         root->tuple = t;
//     }
// }

// void IR_expPrime(ast_node* root, TupleList* list){
//     // expPrime gives an epsilon
//     if(root->firstChild == NULL){
//         root->place = NULL;
//         root->tuple = NULL;
//     }
//     // expPrime gives LPO factor termPrime
//     else{
//         root->place = newtemp();
//         Tuple* temp = root->firstChild->nextSib->nextSib->tuple;
//         Tokentype op = ((struct operator_struct *)(root->firstChild->ninf))->op;
//         Tuple* t;
//         if(op == TK_PLUS)
//             t = makeTuple(PLUS, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
//         else
//             t = makeTuple(MINUS, root->firstChild->nextSib->place, NULL, root->place, root->firstChild->nextSib, NULL, node, 0);
//         if(temp)
//             temp->next = t;
//         add_to_list(t, list);
//         root->tuple = t;
//     }
// }
void IR_factor(ast_node *root)
{
    // // factor gives arithematic expression
    // if (root->firstChild->construct == arithmeticExpression_)
    // {
    //     root->place = root->firstChild->place;
    //     root->tuple = root->firstChild.tuple;
    // }
    // // factor gives var
    // else
    // {
    //     root->place = root->firstChild->place;
    //     root.tuple = root->firstChild->tuple;
    // }
}

void IR_boolean_expression(ast_node *root)
{
    Tokentype op = ((struct operator_struct *)(root->ninf))->op;
    if (op == TK_AND)
    {
        char *arg1 = root->firstChild->place;
        char *arg2 = root->firstChild->nextSib->place;
        char *res = newtemp();
        root->place = res;
        // ADD TO SYMBOL TABLE

        tupleList *t1 = root->firstChild->list;
        tupleList *t2 = root->firstChild->nextSib->list;
        tuple *newT;

        tuple* newT1 = newTuple(IF, arg1, NULL, NULL, NULL);
        char* falseset = newlabel();
        tuple* newT2 = newTuple(GOTO, falseset, NULL, NULL, NULL);
        char* after = newlabel();
        tuple* newT3 = newTuple(GOTO, after, NULL, NULL, NULL);
        tuple* newT4 = newTuple(LABEL, falseset, NULL, NULL, NULL);
        tuple* newT5 = newTuple(LABEL, after, NULL, NULL, NULL);
        tuple* newT6 = newTuple(UNSET, res, NULL, NULL, NULL);
        tuple* newT7 = newTuple(IF, arg2, NULL, NULL, NULL);
        tuple* newT8 = newTuple(GOTO, falseset, NULL, NULL, NULL);
        tuple* newT9 = newTuple(SET, res, NULL, NULL, NULL);

        tupleList* newL = newList();
        // t1 followed by IF t1.place
        if(t1 != NULL){
            newL->head = t1->head;
            newL->tail = t1->tail;
            newL->tail->next = newT1;
            newL->tail = newT1;
        }
        else{
            addTupleEnd(newL, newT1);
        }
        // goto falseset
        addTupleEnd(newL, newT2);
        
        // add t2's code
        if(t2 != NULL){
            newL->tail->next = t2->head;
            newL->tail = t2->tail;
        }
        // IF t2.place
        addTupleEnd(newL, newT7);
        // goto falseset
        addTupleEnd(newL, newT8);
        // res = true
        addTupleEnd(newL, newT9);
        // goto after
        addTupleEnd(newL, newT3);
        // falseset: 
        addTupleEnd(newL, newT4);
        // res = false
        addTupleEnd(newL, newT6);
        // after: 
        addTupleEnd(newL, newT5);

        root->list = newL;
    }
    else if (op == TK_OR)
    {
    }
    else if (op == TK_NOT)
    {
        char *arg1 = root->firstChild->place;
        char *res = newtemp();
        root->place = res;
        // ADD TO SYMBOL TABLE

        tupleList *t1 = root->firstChild->list;
        tuple *newT = newTuple(NOT, arg1, NULL, res, NULL);
        if (t1 == NULL)
        {
            tupleList *newL = newList();
            addTupleEnd(newL, newT);
            root->list = newL;
        }
        else
        {
            t1->tail->next = newT;
            t1->tail = newT;
            root->list = t1;
        }
    }
    else
    {
        char *arg1 = root->firstChild->place;
        char *arg2 = root->firstChild->nextSib->place;
        char *res = newtemp();
        root->place = res;
        // ADD TO SYMBOL TABLE

        tupleList *t1 = root->firstChild->list;
        tupleList *t2 = root->firstChild->nextSib->list;
        tuple *newT;
        if (op == TK_LT)
            newT = newTuple(LT, arg1, arg2, res, NULL);
        else if (op == TK_LE)
            newT = newTuple(LE, arg1, arg2, res, NULL);
        else if (op == TK_GE)
            newT = newTuple(GE, arg1, arg2, res, NULL);
        else if (op == TK_GT)
            newT = newTuple(GT, arg1, arg2, res, NULL);
        else if (op == TK_EQ)
            newT = newTuple(EQ, arg1, arg2, res, NULL);
        else
            newT = newTuple(NE, arg1, arg2, res, NULL);
        if (t1 == NULL)
        {
            if (t2 == NULL)
            {
                tupleList *newL = newList();
                addTupleEnd(newL, newT);
                root->list = newL;
            }
            else
            {
                addTupleEnd(t2, newT);
                root->list = t2;
            }
        }
        else
        {
            if (t2 == NULL)
            {
                addTupleEnd(t1, newT);
                root->list = t1;
            }
            else
            {
                t1->tail->next = t2->head;
                t2->tail->next = newT;
                t1->tail = newT;
                root->list = t1;
            }
        }
    }
}
void IR_arithematic_statement(ast_node *root)
{
}

void IR_conditional(ast_node *root)
{

    if (root->firstChild == NULL)
    {
        root->place = NULL;
        root->list = NULL;
    }

    ast_node *boolExp = root->firstChild;
    tupleList *boollist = boolExp->list;
    char *elselabel = newlabel();
    char *afterlabel = newlabel();

    tuple *t1 = newTuple(IF, boolExp->place, NULL, NULL, NULL);
    tuple *t2 = newTuple(GOTO, elselabel, NULL, NULL, NULL);
    tuple *t3 = newTuple(GOTO, afterlabel, NULL, NULL, NULL);
    tuple *t4 = newTuple(LABEL, elselabel, NULL, NULL, NULL);
    tuple *t5 = newTuple(LABEL, afterlabel, NULL, NULL, NULL);

    root->list->head = boollist->head;
    // tuple* temp = boollist->tail->next;
    boollist->tail->next = t1;
    t1->next = t2;
    // t2->next = temp;

    ast_node *then = boolExp->nextSib;
    tupleList *thenlist = then->list;

    t2->next = thenlist->head;
    thenlist->tail->next = t3;
    t3->next = t4;

    ast_node *elseExp = then->nextSib;
    tupleList *elselist = elseExp->list;

    t4->next = elselist->head;
    elselist->tail->next = t5;

    t5->next = NULL;

    root->list->head = boollist->head;
    root->list->tail = t5;
    // root->list->no_tuples = boollist->no_tuples + thenlist->no_tuples + elselist->no_tuples + 5;
}

void IR_iterative(ast_node *root)
{
    if (root->firstChild == NULL)
    {
        root->place = NULL;
        root->list = NULL;
    }

    ast_node *boolExp = root->firstChild;
    tupleList *boollist = boolExp->list;
    char *beginlabel = newlabel();
    char *afterlabel = newlabel();

    tuple *t1 = newTuple(LABEL, beginlabel, NULL, NULL, NULL);
    tuple *t2 = newTuple(IF, boolExp->place, NULL, NULL, NULL);
    tuple *t3 = newTuple(GOTO, afterlabel, NULL, NULL, NULL);
    tuple *t4 = newTuple(GOTO, beginlabel, NULL, NULL, NULL);
    tuple *t5 = newTuple(LABEL, afterlabel, NULL, NULL, NULL);

    t1->next = boollist->head;
    boollist->tail = t2;
    t2->next = t3;

    ast_node *otherExp = boolExp->nextSib;
    tupleList *otherlist = otherExp->list;

    t3->next = otherlist->head;
    otherlist->tail = t4;

    t4->next = t5;
    t5->next = NULL;

    root->list->head = t1;
    root->list->tail = t5;
}