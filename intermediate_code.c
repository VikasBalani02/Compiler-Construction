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
        IR_factor(root, list);
    }
    else if (root->construct == termPrime_)
    {
        IR_termprime(root, list);
    }
    else if (root->construct == expPrime_)
    {
        IR_expPrime(root, list);
    }
    else if (root->construct == arithmeticExpression_)
    {
        IR_arithematic_statement(root, list);
    }
    else if (root->construct == ioStmt_)
    {
        IR_io_statement(root, list);
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
        if (((struct field_id_struct *)(temp->ninf))->lexID != NULL)
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
    // factor gives arithematic expression
    if (root->firstChild->construct == arithmeticExpression_)
    {
        root->place = root->firstChild->place;
        root->tuple = root->firstChild.tuple;
    }
    // factor gives var
    else
    {
        root->place = root->firstChild->place;
        root.tuple = root->firstChild->tuple;
    }
}

void IR_boolean_expression(ast_node *root)
{
    Tokentype op = ((struct operator_struct *)(root->ninf))->op;
    if (op == TK_AND)
    {
    }
    else if (op == TK_OR)
    {
    }
    else if(op == TK_NOT){
        char *arg1 = root->firstChild->place;
        char* res = newtemp();
        root->place = res;
        // ADD TO SYMBOL TABLE

        tupleList* t1 = root->firstChild->list;
        tuple* newT = newTuple(NOT, arg1, NULL, res, NULL);
        if(t1 == NULL){
            tupleList *newL = newList();
            addTupleEnd(newL, newT);
            root->list = newL;
        }
        else{
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
        else if(op == TK_LE)
            newT = newTuple(LE, arg1, arg2, res, NULL);
        else if(op == TK_GE)
            newT = newTuple(GE, arg1, arg2, res, NULL);
        else if(op == TK_GT)
            newT = newTuple(GT, arg1, arg2, res, NULL);
        else if(op == TK_EQ)
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
void IR_arithematic_statement(ast_node *root, TupleList *list)
{
    root->place = newtemp();
    Tuple *t = make_new_Tuple()
        add_to_list(t, list);
    root->tuple = t;
}
