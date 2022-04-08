#include "typeChecker.h"
#include "symbolTabledef.h"
#include "ast.h"
#include "astDef.h"

typeInfo *get_typeInfo(Type type, char *ruid)
{
    typeInfo *temp = (typeInfo *)malloc(sizeof(typeInfo));
    temp->type_ruid = ruid;
    temp->type = type;
    temp->next = NULL;
    return temp;
}
void *populate_Identifier_type(ast_node *id_node, Error *err_list, symbolTable table)
{
    // TO-DO
    // called on Identifiers_ , singleOrRecId_
    // Functionalities: Given a singleorRecId node fetch the type for the variable from the symbol table
    // important: if type is ruid: need to resolve it and return the corresponding TK_RECORD + declaration RUID
    // if there is no corresponding entry in the symbol table corresponding to the occurence of the name or if there is an attempt to use oneExpansion on a real or int then return type as ERROR_
    // struct id_struct* id_info=(struct id_struct*)id_node->ninf;
}

struct symbolTableRecord *get_function_entry(ast_node *funid_node, Error *err_list, symbolTable table)
{
    // return typeInfo corresponding to FUNID
}
void check_stmt(ast_node *node, Error *err_list, symbolTable table)
{
    switch (node->construct)
    {
    case assignmentStmt_:
        check_assign_stmt(node, err_list);
        break;
    case iterativeStmt_:
        check_iterative_stmt(node, err_list);
        break;
    case funCallStmt_:
        check_funCall_stmt(node, err_list, table);
        break;
    case ioStmt_:
        check_io_stmt(node, err_list);
        break;
    case conditionalStmt_:
        check_conditional_stmt(node, err_list);
        break;
    case returnStmt_:
        check_return_stmt(node, err_list);
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

    typeInfo *lhs_type = lhs->node_type;
    typeInfo *rhs_type = rhs->node_type;

    if (lhs_type->type == ERROR_ || rhs_type->type == ERROR_ || lhs_type->type != rhs_type->type)
    {
        node->node_type = get_typeInfo(ERROR_, NULL);
    }
    else
    {
        if (lhs_type->type == RECORD || lhs_type->type == UNION)
        {
            if (strcmp(lhs_type->type_ruid, rhs_type->type_ruid) != 0)
            {
                node->node_type = get_typeInfo(ERROR_, NULL);
                // add error in the list for type mismatch
            }
        }
        else
        {
            node->node_type = get_typeInfo(VOID, NULL);
        }
    }
}

void check_io_stmt(ast_node *node, Error *err_list)
{
    // regardless of node being read/write, we need to check if var is of correct type
    ast_node *var_node = node->firstChild;
    if (var_node->construct == INTNUM_ || var_node->construct == REALNUM_)
    {
        node->node_type = get_typeInfo(VOID, NULL);
    }
    else if (var_node->construct == singleOrRecId_)
    {
        if (var_node->node_type->type == ERROR_)
        {
            node->node_type = get_typeInfo(ERROR_, NULL);
        }
        else
        {
            node->node_type = get_typeInfo(VOID, NULL);
        }
    }
    else
    {
        // This will never be executed after syntax check
        // error about variable not declared handled in get_id_type
        node->node_type = get_typeInfo(ERROR_, NULL);
    }
}
int compare_list(ast_node *node, typeInfo *formalParamList, Error *err_list)
{
    // return 1 on no errors
    if (node->node_type->type == ERROR_)
    {
        return 0;
    }
    if (node->node_type->type == EMPTY && formalParamList == NULL)
    {
        // equal number of params (0)
        return 1;
    }
    typeInfo *list1 = node->node_type;
    typeInfo *list2 = formalParamList;
    while (list1 && list2)
    {
        if (list1->type != list2->type)
        {
            return 0;
        }
        list1 = list1->next;
        list2 = list2->next;
    }
    if ((list1 == NULL) ^ (list2 == NULL))
    {
        return 0;
    }
    return 1;
}
void check_funCall_stmt(ast_node *node, Error *err_list, symbolTable table)
{
    struct func_struct *info = (struct func_struct *)node->ninf;
    struct symbolTableRecord *func_record = get_function_type(info->funID, err_list, table);
    ast_node *outputParameters_anode = node->firstChild;
    ast_node *inputParameters_anode = outputParameters_anode->nextSib;
    int error_free_out = compare_param_list(outputParameters_anode, func_record->function_field->OutputHead, err_list);
    int error_free_in = compare_param_list(inputParameters_anode, func_record->function_field->InputHead, err_list);
    if (error_free_in + error_free_out != 2)
    {
        node->node_type = get_typeInfo(ERROR_, NULL);
    }
    else
    {
        node->node_type = get_typeInfo(VOID, NULL);
    }
}

void check_iterative_stmt(ast_node *node, Error *err_list)
{
    ast_node *bool_condition = node->firstChild;
    ast_node *stmts_to_repeat = node->firstChild->nextSib;

    // Check if the boolean expression has no errors
    if (bool_condition->node_type->type == ERROR_)
    {
        node->node_type = get_typeInfo(ERROR_, NULL);
    }
    else
    {
        // The rule was <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
        // stmts to repeat node is an otherstmts node
        if (stmts_to_repeat->node_type->type == ERROR_)
        {
            node->node_type = get_typeInfo(ERROR_, NULL);
        }
        else
        {
            node->node_type = get_typeInfo(VOID, NULL);
        }
    }
}

void check_conditional_stmt(ast_node *node, Error *err_list)
{
    ast_node *booleanExpression_node = node->firstChild;
    ast_node *thenStmts_node = booleanExpression_node->nextSib;
    ast_node *elsePart_node = thenStmts_node->nextSib;

    node->node_type = get_typeInfo(VOID, NULL); // if there is any error we will assign error to stmts
    if (booleanExpression_node->node_type->type == ERROR_ || thenStmts_node->node_type->type == ERROR_ || elsePart_node->node_type->type == ERROR_)
    {
        node->node_type = get_typeInfo(ERROR_, NULL);
    }
}
void check_return_stmt(ast_node *node, Error *err_list)
{
    ast_node *optionalReturn_anode = node->firstChild;
    if (optionalReturn_anode->node_type->type == EMPTY || optionalReturn_anode->node_type->type != ERROR_)
    {
        // optional return derived eps or there are no errors in output params
        node->node_type = get_typeInfo(VOID, NULL);
    }
    else
    {
        // error in optionalReturn
        node->node_type = get_typeInfo(ERROR_, NULL);
    }
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
        if (expPrime_node->node_type->type == ERROR_ || term_node->node_type->type == ERROR_ || term_node->node_type->type != expPrime_node->node_type->type)
        {
            // add type mismatch error
            node->node_type = get_typeInfo(ERROR_, NULL);
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
        if (termPrime_node->node_type->type == ERROR_ || factor_node->node_type->type == ERROR_ || termPrime_node->node_type->type != factor_node->node_type->type)
        {
            // add type mismatch error
            node->node_type = get_typeInfo(ERROR_, NULL);
        }
        else
        {
            if (factor_node->node_type->type == RECORD || factor_node->node_type->type == UNION)
            {
                // error: multiplication and division not supported on constructed data types
                node->node_type = get_typeInfo(ERROR_, NULL);
            }
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
        node->node_type = get_typeInfo(EMPTY, NULL);
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
            if (termPrime_node->node_type->type == ERROR_ || factor_node->node_type->type == ERROR_ || termPrime_node->node_type->type != factor_node->node_type->type)
            {
                // add type mismatch error
                node->node_type = get_typeInfo(ERROR_, NULL);
            }
            else
            {
                if (factor_node->node_type->type == RECORD || factor_node->node_type->type == UNION)
                {
                    // error: multiplication and division not supported on constructed data types
                    node->node_type = get_typeInfo(ERROR_, NULL);
                }
                node->node_type = factor_node->node_type;
            }
        }
    }
}

void check_expPrime(ast_node *node, Error *err_list)
{
    if (node->firstChild == NULL)
    {
        node->node_type = get_typeInfo(EMPTY, NULL);
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
            if (expPrime_node->node_type->type == ERROR_ || term_node->node_type->type == ERROR_ || term_node->node_type->type != expPrime_node->node_type->type)
            {
                // add type mismatch error
                node->node_type = get_typeInfo(ERROR_, NULL);
            }
            else
            {
                node->node_type = term_node->node_type;
            }
        }
    }
}
void check_stmt_group(ast_node *node, Error *err_list)
{
    // to be used for thenStmt_ and elsePart_ ,otherStmts_ and stmts_
    // loop through all the children statements and check if any is error
    node->node_type = get_typeInfo(VOID, NULL);
    ast_node *temp = node->firstChild;
    while (temp)
    {
        if (temp->node_type->type == ERROR_)
        {
            node->node_type = get_typeInfo(ERROR_, NULL);
        }
        temp = temp->nextSib;
    }
}

void check_boolean_exp(ast_node *node, Error *err_list)
{
    ast_node *bool_exp1 = node->firstChild;
    if (bool_exp1->nextSib == NULL)
    {
        // Negation boolean expression
        node->node_type = bool_exp1->node_type;
        return;
    }
    ast_node *bool_exp2 = bool_exp1->nextSib->nextSib;

    if (bool_exp1->node_type->type == ERROR_ || bool_exp1->node_type->type == ERROR_)
    {
        // 1. boolexp==>boolexp relop boolexp 2. boolexp==>var relop var
        node->node_type = get_typeInfo(ERROR_, NULL);
        return;
    }
    if (bool_exp1->construct != booleanExpression_)
    {
        // rule of form boolexp==>var relop var
        if ((bool_exp1->construct == singleOrRecId_ && bool_exp1->node_type->type == RECORD) || (bool_exp1->construct == singleOrRecId_ && bool_exp1->node_type->type == UNION) || (bool_exp2->construct == singleOrRecId_ && bool_exp2->node_type->type == RECORD) || (bool_exp2->construct == singleOrRecId_ && bool_exp2->node_type->type == UNION))
        {
            // ERROR_: records and unions cannot be used in relational operators
            node->node_type = get_typeInfo(ERROR_, NULL);
            return;
        }
    }
    node->node_type = get_typeInfo(BOOL, NULL);
}
void check_id_list(ast_node *node, Error *err_list)
{
    // called for outputParameters_ inputParameters_ optionalReturn_
    ast_node *temp = node->firstChild;
    if (temp == NULL)
    {
        temp->node_type = get_typeInfo(EMPTY, NULL); // optionalReturn_ and outputParameters_ can derive eps
        return;
    }
    while (temp)
    {
        if (temp->node_type->type == ERROR_ || temp->node_type->type == RECORD || temp->node_type->type == UNION)
        {
            node->node_type = get_typeInfo(ERROR_, NULL);
            if (temp->node_type->type == UNION || temp->node_type->type == RECORD)
            {
                // Error: idList cannot have identifier of a record or a union
            }
            return;
        }
        temp = temp->nextSib;
    }
    node->node_type = node->firstChild->node_type;
    temp = node->firstChild;
    while (temp->nextSib)
    {
        temp->node_type->next = temp->nextSib->node_type;
        temp = temp->nextSib;
    }
}
