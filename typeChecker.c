#include "typeChecker.h"
#include "symbolTabledef.h"
#include "ast.h"
#include "astDef.h"

type_struct *get_type_struct(Type type, char *ruid)
{
    type_struct *temp = (type_struct *)malloc(sizeof(type_struct));
    temp->ruid = ruid;
    temp->type = type;
    return temp;
}
void *get_singleOrRecId_type(ast_node *id_node, Error *err_list)
{
    // TO-DO
    // Functionalities: Given a singleorRecId node fetch the type for the variable from the symbol table
    // important: if type is ruid: need to resolve it and return the corresponding TK_RECORD + declaration RUID
    // if there is no corresponding entry in the symbol table corresponding to the occurence of the name or if there is an attempt to use oneExpansion on a real or int then return type as ERROR
    // struct id_struct* id_info=(struct id_struct*)id_node->ninf;
}
void check_stmt(ast_node *node, Error *err_list)
{
    // assignment stmt
    switch (node->construct)
    {
    case assignmentStmt_:
        check_assign_stmt(node, err_list);
        break;
    case iterativeStmt_:
        check_iterative_stmt(node, err_list);
        break;
    case funCallStmt_:
        check_funCall_stmt(node, err_list);
        break;
    case ioStmt_:
        check_io_stmt(node, err_list);
        break;
    case conditionalStmt_:
        check_conditional_stmt(node, err_list);
        break;

    default:
        add_error(err_list, "Unidentified type of statement");
    }
}

void check_assign_stmt(ast_node *node, Error *err_list)
{
    // <assignmentStmt> ===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    ast_node *lhs = node->firstChild;
    ast_node *rhs = node->firstChild->nextSib; // arithmetic expression node

    type_struct *lhs_type = lhs->node_type;
    type_struct *rhs_type = rhs->node_type;

    if (lhs_type->type == ERROR || rhs_type->type == ERROR || lhs_type->type != rhs_type->type)
    {
        node->node_type = get_type_struct(ERROR, NULL);
    }
    else
    {
        if (lhs_type->type == RECORD || lhs_type->type == UNION)
        {
            if (strcmp(lhs_type->ruid, rhs_type->ruid) != 0)
            {
                node->node_type = get_type_struct(ERROR, NULL);
                // add error in the list for type mismatch
            }
        }
        else
        {
            node->node_type = get_type_struct(VOID, NULL);
        }
    }
}

void check_io_stmt(ast_node *node, Error *err_list)
{
    // regardless of node being read/write, we need to check if var is of correct type
    ast_node *var_node = node->firstChild;
    if (var_node->construct == INTNUM_ || var_node->construct == REALNUM_)
    {
        node->node_type = get_type_struct(VOID, NULL);
    }
    else if (var_node->construct == singleOrRecId_)
    {
        type_struct *var_type = var_node->node_type;
        if (var_type == ERROR)
        {
            node->node_type = get_type_struct(ERROR, NULL);
        }
        else
        {
            node->node_type = get_type_struct(VOID, NULL);
        }
    }
    else
    {
        // This will never be executed after syntax check
        // error about variable not declared handled in get_id_type
        node->node_type = get_type_struct(ERROR, NULL);
    }
}

void check_funCall_stmt(ast_node *node, Error *err_list)
{
    // TO-DO
}

check_iterative_stmt(ast_node *node, Error *err_list)
{
    ast_node *bool_condition = node->firstChild;
    ast_node *stmts_to_repeat = node->firstChild->nextSib;

    // Check if the boolean expression has no errors
    if (bool_condition->node_type->type == ERROR)
    {
        node->node_type = get_type_struct(ERROR, NULL);
    }
    else
    {
        // The rule was <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
        // stmts to repeat node is an otherstmts node
        if (stmts_to_repeat->node_type->type == ERROR)
        {
            node->node_type = get_type_struct(ERROR, NULL);
        }
        else
        {
            node->node_type = get_type_struct(VOID, NULL);
        }
    }
}

void check_conditional_stmt(ast_node *node, Error *err_list)
{
    ast_node* booleanExpression_node=node->firstChild;
    ast_node* thenStmts_node=booleanExpression_node->nextSib;
    ast_node* elsePart_node=thenStmts_node->nextSib;

    node->node_type=get_type_struct(VOID,NULL);// if there is any error we will assign error to stmts
    if(booleanExpression_node->node_type->type==ERROR || thenStmts_node->node_type->type==ERROR ||elsePart_node->node_type->type==ERROR){
        node->node_type=get_type_struct(ERROR,NULL);
    }       
    
}
void check_return_stmt(ast_node *node, Error *err_list)
{
}
void check_arithmeticExpression(ast_node *node, Error *err_list)
{
    ast_node *term_node = node->firstChild;
    ast_node *expPrime_node = term_node->nextSib;
    if (expPrime_node->node_type->type == EMPTY)
    {
        node->node_type = term_node->node_type;
    }
    else
    {
        if (expPrime_node->node_type->type == ERROR || term_node->node_type->type == ERROR || term_node->node_type->type != expPrime_node->node_type->type)
        {
            // add type mismatch error
            node->node_type = get_type_struct(ERROR, NULL);
        }
        else
        {
            node->node_type = term_node->node_type;
        }
    }
}

void check_term(ast_node *node, Error *err_list)
{
    ast_node *factor_node = node->firstChild;
    ast_node *termPrime_node = factor_node->nextSib;

    if (termPrime_node->node_type->type == EMPTY)
    {
        node->node_type = factor_node->node_type;
    }
    else
    {
        if (termPrime_node->node_type->type == ERROR || factor_node->node_type->type == ERROR || termPrime_node->node_type->type != factor_node->node_type->type)
        {
            // add type mismatch error
            node->node_type = get_type_struct(ERROR, NULL);
        }
        else
        {
            node->node_type = factor_node->node_type;
        }
    }
}

void check_factor(ast_node *node, Error *err_list)
{
    node->node_type = node->firstChild->node_type;
}

void check_termPrime(ast_node *node, Error *err_list)
{
    if (node->firstChild == NULL)
    {
        node->node_type = get_type_struct(EMPTY, NULL);
    }
    else
    {
        ast_node *factor_node = node->firstChild->nextSib;
        ast_node *termPrime_node = factor_node->nextSib;

        if (termPrime_node->node_type->type == EMPTY)
        {
            node->node_type = factor_node->node_type;
        }
        else
        {
            if (termPrime_node->node_type->type == ERROR || factor_node->node_type->type == ERROR || termPrime_node->node_type->type != factor_node->node_type->type)
            {
                // add type mismatch error
                node->node_type = get_type_struct(ERROR, NULL);
            }
            else
            {
                node->node_type = factor_node->node_type;
            }
        }
    }
}

void check_expPrime(ast_node *node, Error *err_list)
{
    if (node->firstChild == NULL)
    {
        node->node_type = get_type_struct(EMPTY, NULL);
    }
    else
    {
        ast_node *term_node = node->firstChild->nextSib;
        ast_node *expPrime_node = term_node->nextSib;
        if (expPrime_node->node_type->type == EMPTY)
        {
            node->node_type = term_node->node_type;
        }
        else
        {
            if (expPrime_node->node_type->type == ERROR || term_node->node_type->type == ERROR || term_node->node_type->type != expPrime_node->node_type->type)
            {
                // add type mismatch error
                node->node_type = get_type_struct(ERROR, NULL);
            }
            else
            {
                node->node_type = term_node->node_type;
            }
        }
    }
}
check_stmt_group(ast_node* node,Error* err_list){
    //to be used for thenStmt_ and elsePart_ ,otherStmts_ and stmts_
    //loop through all the children statements and check if any is error
    node->node_type=get_type_struct(VOID,NULL);
    ast_node* temp=node->firstChild;
    while(temp){
        if(temp->node_type->type==ERROR){
            node->node_type=get_type_struct(ERROR,NULL);
        }
        temp=temp->nextSib;
    }
}

void check_boolean_exp(ast_node* node, Error* err_list){
    
}
// void check_var(ast_node* node,Error* err_list){
//     node->node_type=
// }
