#include <stdio.h>
#include "ast.h"

ast_node* makeNode(CONSTRUCT c, ast_node** a, int no_children, struct node_info* n_info){
    struct ast_node* new_node= (ast_node*)malloc(sizeof(ast_node));

    new_node->construct=c;
    new_node->nextSib = NULL;
    new_node->ninf=n_info;

    int i= 0;
    if(a==NULL) {
        new_node->firstChild=NULL;
        return new_node;
    }
    while(a[i] ==NULL && i<no_children)
        i++;
    if(i == no_children)
        new_node->firstChild=NULL;
    else
        new_node->firstChild= (a==NULL) ? NULL:a[i];
    for(;i<no_children-1;i++){
        struct ast_node* temp= a[i];
        if(temp==NULL)
            continue; 
        //this is when we have to add an entire list of children to the ast children
        while(temp->nextSib!=NULL){
            temp=temp->nextSib;
        }
        ast_node* t= a[i+1];
        while(t == NULL && i< no_children-1){
            i++;
            if((i+1) == no_children){
                t = NULL;
                break;
            }
            t = a[i+1];
        }
        temp->nextSib=t;
    }
    return new_node;
}

struct ast_node *return_child(tNode *node, int n)
{
    tNode *child_ptr = node->child_first;
    for (int i = 1; i < n; i++)
    {
        child_ptr = child_ptr->next_sibling;
    }
    return child_ptr->addr;
}

// struct ast_node* makeNode(CONSTRUCT c, ast_node** a, int no_children, struct node_info* n_info){
void ast_r1(tNode *prog)
{
    ast_node *otherf = return_child(prog, 1);
    ast_node *mainf = return_child(prog, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(ast_node *));
    arr[0] = (otherf->firstChild);
    arr[1] = mainf;
    prog->addr = makeNode(program_, arr, 2, NULL);
    free(otherf);
}
//<otherFunctions> ===> <function> <otherFunctions1>
void ast_r2(tNode *othFunc)
{
    ast_node *func = return_child(othFunc, 1);
    ast_node *othfunc1 = return_child(othFunc, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = func;
    arr[1] = othfunc1->firstChild;
    othFunc->addr = makeNode(otherFunctions_, arr, 2, NULL);
    free(othfunc1);
}
// <otherFunctions> ===> eps
// otherFunctions.addr =makeNode(otherFunctions,NULL)
// // link to otherFunction’s children is NULL, this is done to bring coherency in freeing otherFunctions1 in rule 2.
// // otherFunctions.children = NULL is implicit from above

void ast_r3(tNode *othFunc)
{
    othFunc->addr = makeNode(otherFunctions_, NULL, 0, NULL);
}
// <mainFunction> ===> TK_MAIN <stmts> TK_END
// mainFunction.addr = makeNode(mainFunction,struct{TK_MAIN, _main, lineno },NULL)
// mainFunction.children = stmts.addr
// free(TK_MAIN)
// free(TK_END)

void ast_r4(tNode *mainF)
{
    ast_node *stmt_anode = return_child(mainF, 2);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = stmt_anode;
    struct func_struct *info = (struct func_struct *)malloc(sizeof(struct func_struct));
    info->funID = "_main"; // check if this works
    info->lineNum = mainF->child_first->lex_token->lineNo;
    mainF->addr = makeNode(mainFunction_, arr, 1, (struct node_info *)info);
}
// <function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
// function.addr = makeNode(function,struct{TK_FUNID, TK_FUNID.entry, lineno}, NULL)
// function.children = list(input_par.addr,output_par.addr,stmts.addr)
// free(TK_FUNID)
// free(TK_SEM)
// free(TK_END)

void ast_r5(tNode *function_tnode)
{
    Token FUNID = function_tnode->child_first->lex_token;
    ast_node *input_par_anode = return_child(function_tnode, 2);
    ast_node *output_par_anode = return_child(function_tnode, 3);
    ast_node *stmts_anode = return_child(function_tnode, 5);
    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = input_par_anode;
    arr[1] = output_par_anode;
    arr[2] = stmts_anode;

    struct func_struct *info = (struct func_struct *)malloc(sizeof(struct func_struct));
    info->funID = (FUNID->lexeme);
    info->lineNum = FUNID->lineNo;
    function_tnode->addr = makeNode(function_, arr, 3, (struct node_info *)info);
}
// <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
// input_par.addr = makeNode(input_par,NULL)
// input_par.children = parameter_list.children
// free(parameter_list.addr)
// free(TK_INPUT)
// free(TK_PARAMETER)
// free(TK_LIST)
// free(TK_SQL)
// free(TK_SQR)
void ast_r6(tNode *input_par_tnode)
{
    ast_node *parameter_list_anode = return_child(input_par_tnode, 5);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = parameter_list_anode->firstChild;
    input_par_tnode->addr = makeNode(input_par_, arr, 1, NULL);
    free(parameter_list_anode);
}
// <output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
// output_par.addr = makeNode(output_par,NULL)
// output_par.children = parameter_list.children
// free(parameter_list.addr)
// free(TK_OUTPUT)
// free(TK_PARAMETER)
// free(TK_LIST)
// free(TK_SQL)
// free(TK_SQR)
void ast_r7(tNode *output_par_tnode)
{
    ast_node *parameter_list_anode = return_child(output_par_tnode, 5);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = (parameter_list_anode->firstChild);
    output_par_tnode->addr = makeNode(output_par_, arr, 1, NULL);
    free(parameter_list_anode);
}
// <output_par> ===> eps
// output_par.addr = makeNode(output_par,NULL)
void ast_r8(tNode *output_par_tnode)
{
    output_par_tnode->addr = makeNode(output_par_, NULL, 0, NULL);
}

// <parameter_list> ===> <dataType> TK_ID <remaining_list>
// parameter_list.addr=makeNode(parameter_list,NULL)
// Identifier.addr = makeNode(Identifier,  struct{TK_ID, TK_ID.entry, lineno}, NULL)
// parameter_list.children=list(dataType.addr,Identifier.addr, remaining_list.children)
// free(remaining_list.addr)
void ast_r9(tNode *parameter_list_tnode)
{
    ast_node *dataType_anode = return_child(parameter_list_tnode, 1);
    ast_node *remaining_list_anode = return_child(parameter_list_tnode, 3);
    ast_node **arr;
    struct id_struct *info = malloc(sizeof(struct id_struct));
    info->isGlobal = 0;
    info->lexID = (parameter_list_tnode->child_first->next_sibling->lex_token->lexeme);
    info->lineNum = parameter_list_tnode->child_first->next_sibling->lex_token->lineNo;
    info->dataType = dataType_anode;
    ast_node *Identifier_node = makeNode(Identifier_, NULL, 0, (struct node_info *)info);
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = dataType_anode;
    arr[1] = Identifier_node;
    arr[2] = (remaining_list_anode->firstChild);
    parameter_list_tnode->addr = makeNode(parameter_list_, arr, 3, NULL);
    free(remaining_list_anode);
}

// <dataType> ===> <primitiveDatatype>
// dataType.addr=primitiveDatatype.addr
void ast_r10(tNode *dataType_tnode)
{
    ast_node *primitiveDatatype_anode = return_child(dataType_tnode, 1);
    dataType_tnode->addr = primitiveDatatype_anode;
}

// <dataType> ===> <constructedDatatype>
// dataType.addr = constructedDatatype.addr
void ast_r11(tNode *dataType_tnode)
{
    ast_node *constructedDatatype_anode = return_child(dataType_tnode, 1);
    dataType_tnode->addr = constructedDatatype_anode;
}

// <primitiveDatatype> ===> TK_INT
// primitiveDatatype.addr = makeNode(primitiveDatatype, struct{TK_INT,lineno}, NULL)
// free(TK_INT)
void ast_r12(tNode *primitiveDatatype_tnode)
{
    struct primitive_type_struct *info = (struct primitive_type_struct *)malloc(sizeof(struct primitive_type_struct));
    info->lineNum = primitiveDatatype_tnode->child_first->lex_token->lineNo;
    info->int_or_real = TK_INT;
    primitiveDatatype_tnode->addr = makeNode(primitiveDatatype_, NULL, 0, (struct node_info *)info);
}
// <primitiveDatatype> ===> TK_REAL
// primitiveDatatype.addr = makeNode(primitiveDatatype, struct{TK_REAL,lineno}, NULL)
// free(TK_REAL)
void ast_r13(tNode *primitiveDatatype_tnode)
{
    struct primitive_type_struct *info = (struct primitive_type_struct *)malloc(sizeof(struct primitive_type_struct));
    info->lineNum = primitiveDatatype_tnode->lex_token->lineNo;
    info->int_or_real = TK_REAL;
    primitiveDatatype_tnode->addr = makeNode(primitiveDatatype_, NULL, 0, (struct node_info *)info);
}

// <constructedDatatype> ===> TK_RECORD TK_RUID
// constructedDatatype.addr=makeNode(constructedDatatype,struct{TK_RECORD,TK_RUID,TK_RUID.entry},NULL)
// free(TK_RECORD)
// free(TK_RUID)
void ast_r14(tNode *constructedDatatype_tnode)
{
    struct constructed_type_struct *info = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID = constructedDatatype_tnode->child_first->next_sibling->lex_token;
    info->lineNum = RUID->lineNo;
    info->ruid = (RUID->lexeme);
    info->union_or_record = TK_RECORD;
    constructedDatatype_tnode->addr = makeNode(constructedDatatype_, NULL, 0, (struct node_info *)info);
}

// <constructedDatatype> ===> TK_UNION TK_RUID
// constructedDatatype.addr=makeNode(constructedDatatype,struct{TK_UNION,TK_RUID,TK_RUID.entry},NULL)
// free(TK_UNION)
// free(TK_RUID)
void ast_r15(tNode *constructedDatatype_tnode)
{
    struct constructed_type_struct *info = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID = constructedDatatype_tnode->child_first->next_sibling->lex_token;
    info->lineNum = RUID->lineNo;
    info->ruid = (RUID->lexeme);
    info->union_or_record = TK_UNION;
    constructedDatatype_tnode->addr = makeNode(constructedDatatype_, NULL, 0, (struct node_info *)info);
}

// <constructedDatatype> ===> TK_RUID
// constructedDatatype.ptr=makeNode(constructedDatatype,struct{TK_RUID,TK_RUID.entry})
// free(TK_RUID)
void ast_r16(tNode *constructedDatatype_tnode)
{
    struct constructed_type_struct *info = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID = constructedDatatype_tnode->child_first->lex_token;
    info->lineNum = RUID->lineNo;
    info->ruid = (RUID->lexeme);
    info->union_or_record = -1; // typedef'd so we dont know
    constructedDatatype_tnode->addr = makeNode(constructedDatatype_, NULL, 0, (struct node_info *)info);
}

// <remaining_list> ===> TK_COMMA <parameter_list>
// remaining_list.addr= makeNode(remaining_list,NULL)
// remaining_list.children=parameter_list.children
// free(TK_COMMA)
void ast_r17(tNode *remaining_list_tnode)
{
    ast_node *parameter_list_anode = return_child(remaining_list_tnode, 2);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = (parameter_list_anode->firstChild);
    remaining_list_tnode->addr = makeNode(remaining_list_, arr, 1, NULL);
}

// <remaining_list> ===> eps
// remaining_list.addr= makeNode(remaining_list,NULL)
void ast_r18(tNode *remaining_list_tnode)
{
    remaining_list_tnode->addr = makeNode(remaining_list_, NULL, 0, NULL);
}

// <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
// stmts=makeNode(stmts,NULL)
// typeDefinitionStmts=makeNode(typeDefinitionsStmts,typeDefinitions.children)
// stmts.children=list(typeDefinitionStmts.addr,declarations.addr,otherStmts.children,returnStmt.addr)
// free(otherStmts.addr)
// free(typeDefinitions.addr)
void ast_r19(tNode *stmts_tnode)
{ // recheck this rule
    ast_node *typeDefinition_anode = return_child(stmts_tnode, 1);
    ast_node *declarations_anode = return_child(stmts_tnode, 2);
    ast_node *otherStmts_anode = return_child(stmts_tnode, 3);
    ast_node *returnStmt_anode = return_child(stmts_tnode, 4);

    ast_node **arr;
    arr = (ast_node **)malloc(4 * sizeof(struct ast_node *));
    arr[0] = typeDefinition_anode;
    arr[1] = declarations_anode;
    arr[2] = (otherStmts_anode);
    arr[3] = returnStmt_anode;

    stmts_tnode->addr = makeNode(stmts_, arr, 4, NULL);
}

void ast_r20(tNode *typeDefinitions_node)
{
    ast_node *actualOrRedefined_node = return_child(typeDefinitions_node, 1);
    ast_node *typeDefinition_node = return_child(typeDefinitions_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(ast_node *));
    arr[0] = actualOrRedefined_node;
    arr[1] = typeDefinition_node->firstChild;
    typeDefinitions_node->addr = makeNode(typeDefinitions_, arr, 2, NULL);
    // free(typeDefinition);
}

void ast_r21(tNode *typedefn)
{
    typedefn->addr = makeNode(typeDefinitions_, NULL, 0, NULL);
}

void ast_r22(tNode *typeDefinition_node)
{
    ast_node *fieldDefinitions_node = return_child(typeDefinition_node, 3);
    struct constructed_type_struct *info = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID = typeDefinition_node->child_first->next_sibling->lex_token;
    info->lineNum = RUID->lineNo;
    info->ruid = (RUID->lexeme);
    info->union_or_record = TK_RECORD;
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = fieldDefinitions_node;
    typeDefinition_node->addr = makeNode(typeDefinition_, arr, 1, (struct node_info *)info);
    // free(fieldDefinitions);
}

void ast_r23(tNode *typeDefinition_node)
{
    ast_node *fieldDefinitions_node = return_child(typeDefinition_node, 3);
    struct constructed_type_struct *info = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID = typeDefinition_node->child_first->next_sibling->lex_token;
    info->lineNum = RUID->lineNo;
    info->ruid = (RUID->lexeme);
    info->union_or_record = TK_UNION;
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = fieldDefinitions_node;
    typeDefinition_node->addr = makeNode(typeDefinition_, arr, 1, (struct node_info *)info);
    // free(fieldDefinitions);
}

void ast_r24(tNode *actualOrRedefined_node)
{
    ast_node *typeDefinition_node = return_child(actualOrRedefined_node, 1);
    actualOrRedefined_node->addr = typeDefinition_node;
}

void ast_r25(tNode *actualOrRedefined_node)
{
    ast_node *definetypestmt_node = return_child(actualOrRedefined_node, 1);
    actualOrRedefined_node->addr = definetypestmt_node;
}

void ast_r26(tNode *fieldDefinitions_node)
{
    ast_node *fieldDefinition_1_node = return_child(fieldDefinitions_node, 1);
    ast_node *fieldDefinition_2_node = return_child(fieldDefinitions_node, 2);
    ast_node *moreFields_node = return_child(fieldDefinitions_node, 3);

    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = fieldDefinition_1_node;
    arr[1] = fieldDefinition_2_node;
    arr[2] = (moreFields_node->firstChild);

    fieldDefinitions_node->addr = makeNode(fieldDefinitions_, arr, 3, NULL);
}

void ast_r27(tNode *fieldDefinition_node)
{
    ast_node *fieldtype_node = return_child(fieldDefinition_node, 2);
    struct field_id_struct *info = (struct field_id_struct *)malloc(sizeof(struct field_id_struct));
    Token FIELDID = fieldDefinition_node->child_first->next_sibling->next_sibling->next_sibling->lex_token;
    info->fieldid = FIELDID->lexeme;
    info->lineNum = FIELDID->lineNo;
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = fieldtype_node;
    fieldDefinition_node->addr = makeNode(fieldDefinition_, arr, 1, (struct node_info *)info);
    // free(fieldtype);
}

void ast_r28(tNode *moreFields_node)
{
    ast_node *fieldDefinition_node = return_child(moreFields_node, 1);
    ast_node *moreFields_1_node = return_child(moreFields_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(ast_node *));
    arr[0] = fieldDefinition_node;
    arr[1] = moreFields_1_node->firstChild;
    moreFields_node->addr = makeNode(moreFields_, arr, 2, NULL);
    // free(moreFields_1);
}

void ast_r29(tNode *moreFields_node)
{
    moreFields_node->addr = makeNode(moreFields_, NULL, 0, NULL);
}

void ast_r30(tNode *fieldType_node)
{
    ast_node *primitiveDatatype_node = return_child(fieldType_node, 1);
    fieldType_node->addr = primitiveDatatype_node;
}

void ast_r31(tNode *fieldType_node)
{
    struct constructed_type_struct *info = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID = fieldType_node->child_first->lex_token;
    info->lineNum = RUID->lineNo;
    info->ruid = (RUID->lexeme);
    info->union_or_record = -1; // typedef'd so we dont know
    fieldType_node->addr = makeNode(fieldtype_, NULL, 0, (struct node_info *)info);
}
void ast_r32(tNode *declarations_node)
{
    ast_node *declaration_node = return_child(declarations_node, 1);
    ast_node *declarations_1_node = return_child(declarations_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(ast_node *));
    arr[0] = declaration_node;
    arr[1] = declarations_1_node->firstChild;
    declarations_node->addr = makeNode(declarations_, arr, 2, NULL);
    // free(declarations_1);
}

void ast_r33(tNode *declarations_node)
{
    declarations_node->addr = makeNode(declarations_, NULL, 0, NULL);
}

void ast_r34(tNode *declaration_node)
{
    ast_node *dataType = return_child(declaration_node, 2);
    ast_node *global_or_not = return_child(declaration_node, 5);
    struct id_struct *info = (struct id_struct *)malloc(sizeof(struct id_struct));
    Token ID = declaration_node->child_first->next_sibling->next_sibling->next_sibling->lex_token;
    info->lexID = ID->lexeme;
    info->lineNum = ID->lineNo;
    struct global_struct *globalinfo = (struct global_struct *)global_or_not->ninf;
    info->isGlobal = globalinfo->isGlobal;

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = dataType;

    declaration_node->addr = makeNode(declaration_, arr, 1, (struct node_info *)info);
}

void ast_r35(tNode *global_or_not_node)
{
    struct global_struct *info = (struct global_struct *)malloc(sizeof(struct global_struct));
    info->isGlobal = 1;
    global_or_not_node->addr = makeNode(global_or_not_, NULL, 0, (struct node_info *)info);
}

void ast_r36(tNode *global_or_not_node)
{
    struct global_struct *info = (struct global_struct *)malloc(sizeof(struct global_struct));
    info->isGlobal = 0;
    global_or_not_node->addr = makeNode(global_or_not_, NULL, 0, (struct node_info *)info);
}

void ast_r37(tNode *otherStmts_node)
{
    ast_node *stmt_node = return_child(otherStmts_node, 1);
    ast_node *otherStmts_1_node = return_child(otherStmts_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(ast_node *));
    arr[0] = stmt_node;
    arr[1] = otherStmts_1_node->firstChild;
    otherStmts_node->addr = makeNode(otherStmts_, arr, 2, NULL);
    // free(otherStmts_1);
}

void ast_r38(tNode *otherStmts_node)
{
    otherStmts_node->addr = makeNode(otherStmts_, NULL, 0, NULL);
}

// <stmt> ===> <assignmentStmt>
// stmt.addr = assignmentStmt.addr
void ast_r39(tNode *stmt_tNode)
{
    ast_node *assignmentStmt_anode = return_child(stmt_tNode, 1);
    stmt_tNode->addr = assignmentStmt_anode;
}

// <stmt> ===> <iterativeStmt>
//  stmt.addr = iterativeStmt.addr
void ast_r40(tNode *stmt_tNode)
{
    ast_node *iterativeStmt_anode = return_child(stmt_tNode, 1);
    stmt_tNode->addr = iterativeStmt_anode;
}

// <stmt> ===> <conditionalStmt>
// stmt.addr = conditionalStmt.addr
void ast_r41(tNode *stmt_tNode)
{
    ast_node *conditionalStmt_anode = return_child(stmt_tNode, 1);
    stmt_tNode->addr = conditionalStmt_anode;
}

// <stmt> ===> <ioStmt>
// stmt.addr = ioStmt.addr
void ast_r42(tNode *stmt_tNode)
{
    ast_node *ioStmt_anode = return_child(stmt_tNode, 1);
    stmt_tNode->addr = ioStmt_anode;
}

// <stmt> ===> <funCallStmt>
// stmt.addr=funCallStmt.addr
void ast_r43(tNode *stmt_tNode)
{
    ast_node *funCallStmt_anode = return_child(stmt_tNode, 1);
    stmt_tNode->addr = funCallStmt_anode;
}

// <assignmentStmt> ===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
// assignmentStmt.addr = makeNode(assignmentStmt,NULL)
// assignmentStmt.children = list(singleOrRecId.addr, arithmeticExpression.addr)
// free(TK_ASSIGNOP)
// free(TK_SEM)
void ast_r44(tNode *assignmentStmt_tNode)
{
    ast_node *singleOrRecId_anode = return_child(assignmentStmt_tNode, 1);
    ast_node *arithmeticExpression_anode = return_child(assignmentStmt_tNode, 3);
    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = singleOrRecId_anode;
    arr[1] = arithmeticExpression_anode;

    assignmentStmt_tNode->addr = makeNode(function_, arr, 2, NULL);
}

// <oneExpansion> ===> TK_DOT TK_FIELDID
// oneExpansion.addr = makeNode(oneExpansion,struct{TK_FIELDID,TK_FIELDID.entry, lineno},NULL)
// free(TK_DOT)
// free(TK_FIELDID)
void ast_r45(tNode *oneExpansion_tNode)
{
    Token FIELDID = oneExpansion_tNode->child_first->next_sibling->lex_token;

    struct field_id_struct *info = (struct field_id_struct *)malloc(sizeof(struct field_id_struct));
    info->fieldid = FIELDID->lexeme;
    info->lineNum = FIELDID->lineNo;
    oneExpansion_tNode->addr = makeNode(oneExpansion_, NULL, 0, (struct node_info *)info);
}

// <moreExpansions> ===> <oneExpansion> <moreExpansions1>
// moreExpansions.addr = makeNode(moreExpansions,NULL)
// moreExpansions.children = list(oneExpansion.addr,moreExpansions1.children)
// free(moreExpansions1.addr)
void ast_r46(tNode *moreExpansions_tNode)
{
    ast_node *oneExpansion_anode = return_child(moreExpansions_tNode, 1);
    ast_node *moreExpansions1_anode = return_child(moreExpansions_tNode, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = oneExpansion_anode;
    arr[1] = moreExpansions1_anode->firstChild;
    moreExpansions_tNode->addr = makeNode(moreExpansions_, arr, 2, NULL);
    free(moreExpansions1_anode);
}

// <moreExpansions> ===> eps
// moreExpansions.addr=makeNode(moreExpansions,NULL)
void ast_r47(tNode *moreExpansions_tNode)
{
    moreExpansions_tNode->addr = makeNode(moreExpansions_, NULL, 0, NULL);
}

// <option_single_constructed> ===> eps’
// option_single_constructed.addr =makeNode(option_single_constructed,NULL)
void ast_r48(tNode *option_single_constructed_tNode)
{
    option_single_constructed_tNode->addr = makeNode(option_single_constructed_, NULL, 0, NULL);
}

// <option_single_constructed> ===> <oneExpansion> <moreExpansions>
// option_single_constructed.addr = makeNode(option_single_constructed,NULL)
// option_single_constructed.children=list(oneExpansion.addr,moreExpansions.children)
// free(moreExpansions.addr)
void ast_r49(tNode *option_single_constructed_tNode)
{
    ast_node *oneExpansion_anode = return_child(option_single_constructed_tNode, 1);
    ast_node *moreExpansions_anode = return_child(option_single_constructed_tNode, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = oneExpansion_anode;
    arr[1] = moreExpansions_anode->firstChild;
    option_single_constructed_tNode->addr = makeNode(moreExpansions_, arr, 2, NULL);
    free(moreExpansions_anode);
}

// <singleOrRecId> ===> TK_ID <option_single_constructed>
// singleOrRecId.addr = makeNode(singleOrRecId, struct{TK_ID, TK_ID.ptr, lineno},NULL)
// singleOrRecId.children = option_single_constructed.addr
// free(TK_ID)
void ast_r50(tNode *singleOrRecId_tNode)
{
    ast_node *option_single_constructed_anode = return_child(singleOrRecId_tNode, 2);
    Token TKID = singleOrRecId_tNode->child_first->lex_token;
    struct id_struct *info = (struct id_struct *)malloc(sizeof(struct id_struct));
    info->isGlobal = 0;
    info->lexID = TKID->lexeme;
    info->lineNum = TKID->lineNo;
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = option_single_constructed_anode;
    singleOrRecId_tNode->addr = makeNode(singleOrRecId_, arr, 1, (struct node_info *)info);
}

// <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
// funCallStmt.ptr =makeNode(funCallStmt,struct{TK_FUNID, TK_FUNID.ptr, lineno}, NULL)
// funCallStmt.children = list(outputParameters.addr, inputParameters.addr)
// free(TK_CALL)
// free(TK_FUNID)
// free(TK_WITH)
// free(TK_PARAMETERS)
// free(TK_SEM)
void ast_r51(tNode *funCallStmt_tNode)
{
    Token FUNID = funCallStmt_tNode->child_first->next_sibling->next_sibling->lex_token;
    ast_node *outputParameters_anode = return_child(funCallStmt_tNode, 1);
    ast_node *inputParameters_anode = return_child(funCallStmt_tNode, 6);
    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = outputParameters_anode;
    arr[1] = inputParameters_anode;

    struct func_struct *info = (struct func_struct *)malloc(sizeof(struct func_struct));
    info->funID = (FUNID->lexeme);
    info->lineNum = FUNID->lineNo;
    funCallStmt_tNode->addr = makeNode(funCallStmt_, arr, 2, (struct node_info *)info);
}

// <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
// outputParameters.addr=makeNode(outputParameters,NULL)
// outputParameters.children = idList.children
// free(TK_SQL)
// free(idList.addr)
// free(TK_SQR)
// free(TK_ASSIGNOP)
void ast_r52(tNode *outputParameters_tNode)
{
    ast_node *idList_anode = return_child(outputParameters_tNode, 2);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = (idList_anode->firstChild);
    outputParameters_tNode->addr = makeNode(outputParameters_, arr, 1, NULL);
    free(idList_anode);
}

// <outputParameters> ===> eps’
// outputParameters.addr = makeNode(outputParameters, NULL)
void ast_r53(tNode *outputParameters_tNode)
{
    outputParameters_tNode->addr = makeNode(outputParameters_, NULL, 0, NULL);
}

// <inputParameters> ===> TK_SQL <idList> TK_SQR
// inputParameters.addr=makeNode(inputParameters,NULL)
// inputParameters.children = idList.children
// free(TK_SQL)
// free(idList.addr)
// free(TK_SQR)
void ast_r54(tNode *inputParameters_tNode)
{
    ast_node *idList_anode = return_child(inputParameters_tNode, 2);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = (idList_anode->firstChild);
    inputParameters_tNode->addr = makeNode(inputParameters_, arr, 1, NULL);
    free(idList_anode);
}

// <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
// iterativeStmt.addr =makeNode(iterativeStmt, NULL)
// iterativeStmt.children = list(booleanExpression.addr, stmt.addr, otherStmts.children)
// free(TK_WHILE)
// free(TK_OP)
// free(TK_CL)
// free(otherstmts.addr)
// free(TK_ENDWHILE)
void ast_r55(tNode *iterativeStmt_tNode)
{
    ast_node *booleanExpression_anode = return_child(iterativeStmt_tNode, 3);
    ast_node *stmt_anode = return_child(iterativeStmt_tNode, 5);
    ast_node *otherStmts_anode = return_child(iterativeStmt_tNode, 6);
    ast_node **arr;
    stmt_anode->nextSib=otherStmts_anode->firstChild;
    otherStmts_anode->firstChild=stmt_anode;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = booleanExpression_anode;
    arr[1] = (otherStmts_anode);
    iterativeStmt_tNode->addr = makeNode(iterativeStmt_, arr, 2, NULL);
}

// <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
// conditionalStmt.addr =makeNode(conditionalStmt,NULL)
// conditionalStmt.children=list(booleanExpression.addr, stmt.addr, otherStmts.children, elsePart.addr)
// free(TK_IF)
// free(TK_OP)
// free(TK_CL)
// free(TK_THEN)
// free(otherStmts.addr)
void ast_r56(tNode *conditionalStmt_tNode)
{
    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    ast_node *booleanExpression_anode = return_child(conditionalStmt_tNode, 3);
    ast_node *stmt_anode = return_child(conditionalStmt_tNode, 6);
    ast_node *otherStmts_anode = return_child(conditionalStmt_tNode, 7);
    ast_node *elsePart_anode = return_child(conditionalStmt_tNode, 8);

    stmt_anode->nextSib=otherStmts_anode->firstChild;
    otherStmts_anode->firstChild=stmt_anode;
    arr[0] = otherStmts_anode;
    arr[1] = NULL;
    arr[2] = NULL;
    ast_node *thenPart_anode = makeNode(thenStmts_, arr, 1, NULL);
    arr[0] = booleanExpression_anode;
    arr[1] = thenPart_anode;
    arr[2] = elsePart_anode;
    conditionalStmt_tNode->addr = makeNode(iterativeStmt_, arr, 3, NULL);
}

// <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
// elsePart.addr = makeNode(elsePart,NULL)
// elsePart.children=list(stmt.addr,otherStmts.children)
// free(TK_ELSE)
// free(TK_ENDIF)
// free(otherStmts.addr)
void ast_r57(tNode *elsePart_tNode)
{
    ast_node *stmt_anode = return_child(elsePart_tNode, 2);
    ast_node *otherStmts_anode = return_child(elsePart_tNode, 3);
    ast_node **arr;
    stmt_anode->nextSib=otherStmts_anode->firstChild;
    otherStmts_anode->firstChild=stmt_anode;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = otherStmts_anode;
    elsePart_tNode->addr = makeNode(elsePart_, arr, 1, NULL);
}

// <elsePart> ===> TK_ENDIF
// elsePart.addr = makeNode(elsePart,NULL)
// free(ENDIF)
void ast_r58(tNode *elsePart_tNode)
{
    elsePart_tNode->addr = makeNode(elsePart_, NULL, 0, NULL);
}

// <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime1>
// expPrime.ptr = makeNode(expPrime,NULL)
// expPrime.children=list(lowPrecedenceOperators.addr,term.addr,expPrime1.addr)
void ast_r59(tNode *expPrime_tNode)
{
    ast_node *lowPrecedenceOperators_anode = return_child(expPrime_tNode, 1);
    ast_node *term_anode = return_child(expPrime_tNode, 2);
    ast_node *expPrime1_anode = return_child(expPrime_tNode, 3);
    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = lowPrecedenceOperators_anode;
    arr[1] = term_anode;
    arr[2] = expPrime1_anode;
    expPrime_tNode->addr = makeNode(expPrime_, arr, 3, NULL);
}
// <expPrime> ===> eps
// expPrime.addr = makeNode(expPrime,NULL)

void ast_r60(tNode *expPrime_tnode)
{
    expPrime_tnode->addr = makeNode(expPrime_, NULL, 0, NULL);
}

// <term> ===> <factor> <termPrime>
// temp.addr = makeNode(temp, NULL)
// temp.children = list(factor.addr, termPrime.addr)

void ast_r61(tNode *term_tnode)
{
    ast_node *fct = return_child(term_tnode, 1);
    ast_node *trmp = return_child(term_tnode, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = fct;
    arr[1] = trmp;
    term_tnode->addr = makeNode(term_, arr, 2, NULL);
}

// <termPrime> ===> <highPrecedenceOperators> <factor> <termPrime>
// termPrime.addr = makeNode(termPrime, NULL)
// termPrime.children = list(highPrecedenceOperators.addr,factor.addr,termPrime.addr)

void ast_r62(tNode *termPrime_tnode)
{
    ast_node *highP = return_child(termPrime_tnode, 1);
    ast_node *fact = return_child(termPrime_tnode, 2);
    ast_node *termP = return_child(termPrime_tnode, 3);

    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = highP;
    arr[1] = fact;
    arr[2] = termP->firstChild;
    termPrime_tnode->addr = makeNode(termPrime_, arr, 3, NULL);
    free(termP);
}

// <termPrime> ===> eps
// termPrime.addr = makeNode(termPrime,NULL)

void ast_r63(tNode *termPrime_tnode)
{
    termPrime_tnode->addr = makeNode(termPrime_, NULL, 0, NULL);
}

// <factor> ===> TK_OP <arithmeticExpression> TK_CL
// factor.addr=arithmeticExpression.addr

void ast_r64(tNode *factor_tnode)
{
    ast_node *arithmeticExpression_anode = return_child(factor_tnode, 2);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = arithmeticExpression_anode;
    factor_tnode->addr = makeNode(factor_, arr, 1, NULL);
}

// <factor> ===> <var>
// factor.addr = var.addr

void ast_r65(tNode *factor_tnode)
{
    ast_node *var_anode = return_child(factor_tnode, 1);
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = var_anode;
    factor_tnode->addr = makeNode(factor_, arr, 1, NULL);
}

// <highPrecedenceOperators> ===> TK_MUL
// highPrecedenceOperators.addr = makeNode(highPrecedenceOperators, struct{TK_MUL}, NULL)
void ast_r66(tNode *highPrecedenceOperators_tnode)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token mult = highPrecedenceOperators_tnode->child_first->lex_token;
    Tokentype op_ = highPrecedenceOperators_tnode->child_first->t;
    info->lineno = mult->lineNo;
    info->op = TK_MUL;
    highPrecedenceOperators_tnode->addr = makeNode(highPrecedenceOperators_, NULL, 0, (struct node_info *)info);
}

// <highPrecedenceOperators> ===> TK_DIV
// highPrecedenceOperators.addr = makeNode(highPrecedenceOperators, struct{TK_DIV}, NULL)

void ast_r67(tNode *highPrecedenceOperators_tnode)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token div = highPrecedenceOperators_tnode->child_first->lex_token;
    Tokentype op_ = highPrecedenceOperators_tnode->child_first->t;
    info->lineno = div->lineNo;
    info->op = TK_DIV;
    highPrecedenceOperators_tnode->addr = makeNode(highPrecedenceOperators_, NULL, 0, (struct node_info *)info);
}

// <lowPrecedenceOperators> ===> TK_PLUS
// lowPrecedenceOperators.addr = makeNode(lowPrecedenceOperators, struct{TK_PLUS}, NULL)

void ast_r68(tNode *lowPrecedenceOperators_tnode)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token plus = lowPrecedenceOperators_tnode->child_first->lex_token;
    Tokentype op_ = lowPrecedenceOperators_tnode->child_first->t;
    info->lineno = plus->lineNo;
    info->op = TK_PLUS;
    lowPrecedenceOperators_tnode->addr = makeNode(lowPrecedenceOperators_, NULL, 0, (struct node_info *)info);
}

// <lowPrecedenceOperators> ===> TK_MINUS
// lowPrecedenceOperators.addr = makeNode(lowPrecedenceOperators, struct{TK_MINUS}, NULL)

void ast_r69(tNode *lowPrecedenceOperators_tnode)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token minus = lowPrecedenceOperators_tnode->child_first->lex_token;
    Tokentype op_ = lowPrecedenceOperators_tnode->child_first->t;
    info->lineno = minus->lineNo;
    info->op = TK_MINUS;
    lowPrecedenceOperators_tnode->addr = makeNode(lowPrecedenceOperators_, NULL, 0, (struct node_info *)info);
}

// <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
// ioStmt.addr= makeNode(ioStmt,struct{TK_READ },var.addr)
// free(TK_OP)
// free(TK_CL)
// free(TK_SEM)

void ast_r70(tNode *ioStmt_tnode)
{
    ast_node *var_anode = return_child(ioStmt_tnode, 3);
    struct io_struct *info = (struct io_struct *)malloc(sizeof(struct io_struct));
    Token ioread = ioStmt_tnode->child_first->lex_token;
    info->lineNum = ioread->lineNo;
    info->read_or_write = TK_READ;

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = var_anode;

    ioStmt_tnode->addr = makeNode(ioStmt_, arr, 1, (struct node_info *)info);
}

// <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
// ioStmt.addr= makeNode(ioStmt,struct{TK_WRITE},var.addr)
// free(TK_OP)
// free(TK_CL)
// free(TK_SEM)

void ast_r71(tNode *ioStmt_tnode)
{
    ast_node *var_anode = return_child(ioStmt_tnode, 3);
    struct io_struct *info = (struct io_struct *)malloc(sizeof(struct io_struct));
    Token iowrite = ioStmt_tnode->child_first->lex_token;
    info->lineNum = iowrite->lineNo;
    info->read_or_write = TK_WRITE;

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = var_anode;

    ioStmt_tnode->addr = makeNode(ioStmt_, arr, 1, (struct node_info *)info);
}

// <arithmeticExpression> ===> <term> <expPrime>
// arithmeticExpression.addr = makeNode(arithmeticExpression,NULL)
// arithmeticExpression.children = list(term.addr,expPrime.addr)

void ast_r72(tNode *arithmeticExpression_tnode)
{
    ast_node *term_anode = return_child(arithmeticExpression_tnode, 1);
    ast_node *expPrime_anode = return_child(arithmeticExpression_tnode, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(2 * sizeof(struct ast_node *));
    arr[0] = term_anode;
    arr[1] = expPrime_anode;
    arithmeticExpression_tnode->addr = makeNode(arithmeticExpression_, arr, 2, NULL);
}

// <booleanExpression> ===> TK_OP <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL
// booleanExpression.ptr = makeNode(booleanExpression,NULL)
// booleanExpression.children = list(booleanExpression1.addr, logicalOp.addr, booleanExpression2.addr)
// free(TK_OP)
// free(TK_CL)
// free(TK_OP)
// free(TK_CL)

void ast_r73(tNode *booleanExpression_tnode)
{
    ast_node *booleanExpression_anode1 = return_child(booleanExpression_tnode, 1);
    ast_node *logicalOp_anode = return_child(booleanExpression_tnode, 2);
    ast_node *booleanExpression_anode2 = return_child(booleanExpression_tnode, 3);

    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = booleanExpression_anode1;
    arr[1] = logicalOp_anode;
    arr[2] = booleanExpression_anode2;
    booleanExpression_tnode->addr = makeNode(booleanExpression_, arr, 3, NULL);
}

// <booleanExpression> ===> <var1> <relationalOp> <var2>
// booleanExpression.addr = makeNode(booleanExpression, NULL)
// booleanExpression.children = list(var1.addr, relationalOp.addr, var2.addr)

void ast_r74(tNode *booleanExpression_tnode)
{
    ast_node *var_anode1 = return_child(booleanExpression_tnode, 1);
    ast_node *relationalOp_anode = return_child(booleanExpression_tnode, 2);
    ast_node *var_anode2 = return_child(booleanExpression_tnode, 3);

    ast_node **arr;
    arr = (ast_node **)malloc(3 * sizeof(struct ast_node *));
    arr[0] = var_anode1;
    arr[1] = relationalOp_anode;
    arr[2] = var_anode2;
    booleanExpression_tnode->addr = makeNode(booleanExpression_, arr, 3, NULL);
}

// <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1> TK_CL
// booleanExpression.addr = makeNode(booleanExpression, struct{TK_NOT}, booleanExpression1.addr)
// free(TK_NOT)
// free(TK_OP)
// free(TK_CL)

void ast_r75(tNode *booleanExpression_tnode)
{

}


// <var> ===> <singleOrRecId>
// var.addr=singleOrRecId.addr

void ast_r76(tNode *var_tnode)
{
    ast_node *singleOrRecId_anode = return_child(var_tnode, 1);

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(struct ast_node *));
    arr[0] = singleOrRecId_anode;
    var_tnode->addr = makeNode(var_, arr, 1, NULL);
}

void ast_r77(tNode *var_node)
{
    struct num_struct *info = (struct num_struct *)malloc(sizeof(struct num_struct));
    Token NUM = var_node->child_first->lex_token;
    info->lineno = NUM->lineNo;
    info->type = TK_NUM;
    info->value = NUM->val;
    var_node->addr = makeNode(var_, NULL, 0, (struct node_info *)info);
}
void ast_r78(tNode *var_node)
{
    struct num_struct *info = (struct num_struct *)malloc(sizeof(struct num_struct));
    Token NUM = var_node->child_first->lex_token;
    info->lineno = NUM->lineNo;
    info->type = TK_RNUM;
    info->value = NUM->val;
    var_node->addr = makeNode(var_, NULL, 0, (struct node_info *)info);
}
void ast_r79(tNode *logicalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = logicalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    logicalOp_node->addr = makeNode(logicalOp_, NULL, 0, (struct node_info *)info);
}
void ast_r80(tNode *logicalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = logicalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    logicalOp_node->addr = makeNode(logicalOp_, NULL, 0, (struct node_info *)info);
}
void ast_r81(tNode *relationalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = relationalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    relationalOp_node->addr = makeNode(relationalOp_, NULL, 0, (struct node_info *)info);
}
void ast_r82(tNode *relationalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = relationalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    relationalOp_node->addr = makeNode(relationalOp_, NULL, 0, (struct node_info *)info);
}
void ast_r83(tNode *relationalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = relationalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    relationalOp_node->addr = makeNode(relationalOp_, NULL, 0, (struct node_info *)info);
}
void ast_r84(tNode *relationalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = relationalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    relationalOp_node->addr = makeNode(relationalOp_, NULL, 0, (struct node_info *)info);
}
void ast_r85(tNode *relationalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = relationalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    relationalOp_node->addr = makeNode(relationalOp_, NULL, 0, (struct node_info *)info);
}

void ast_r86(tNode *relationalOp_node)
{
    struct operator_struct *info = (struct operator_struct *)malloc(sizeof(struct operator_struct));
    Token Operator = relationalOp_node->child_first->lex_token;
    info->lineno = Operator->lineNo;
    info->op = Operator->type;
    relationalOp_node->addr = makeNode(relationalOp_, NULL, 0, (struct node_info *)info);
}

void ast_r87(tNode *returnStmt_node)
{
    ast_node *optionalReturn = return_child(returnStmt_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = optionalReturn->firstChild;
    returnStmt_node->addr = makeNode(returnStmt_, arr, 1, NULL);
    // free(optionalReturn);
}

void ast_r88(tNode *optionalReturn_node)
{
    ast_node *idList = return_child(optionalReturn_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = idList->firstChild;
    optionalReturn_node->addr = makeNode(optionalReturn_, arr, 1, NULL);
    // free(idList);
}

void ast_r89(tNode *optionalReturn_node)
{
    optionalReturn_node->addr = makeNode(optionalReturn_, NULL, 0, NULL);
}

void ast_r90(tNode *idList_node)
{
    ast_node *more_ids = return_child(idList_node, 2);
    struct id_struct *info = (struct id_struct *)malloc(sizeof(struct id_struct));
    Token ID = idList_node->child_first->lex_token;
    info->lexID = ID->lexeme;
    info->lineNum = ID->lineNo;
    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = more_ids->firstChild;
    idList_node->addr = makeNode(idList_, arr, 1, (struct node_info *)info);
    // free(more_ids);
}
void ast_r91(tNode *more_ids_node)
{
    ast_node *idList = return_child(more_ids_node, 2);

    ast_node **arr;
    arr = (ast_node **)malloc(1 * sizeof(ast_node *));
    arr[0] = idList->firstChild;
    more_ids_node->addr = makeNode(more_ids_, arr, 1, NULL);
    // free(idList);
}

void ast_r92(tNode *more_ids_node)
{
    more_ids_node->addr = makeNode(more_ids_, NULL, 0, NULL);
}

// ***************************************
// DOUBTFUL THST THIS WOULD WORK AT ALL

void ast_r93(tNode *definetypestmt_node)
{
    ast_node *A = return_child(definetypestmt_node, 2);
    struct defineType_struct *info = (struct defineType_struct *)malloc(sizeof(struct defineType_struct));
    struct constructed_type_struct *from = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    struct constructed_type_struct *to = (struct constructed_type_struct *)malloc(sizeof(struct constructed_type_struct));
    Token RUID1 = definetypestmt_node->child_first->next_sibling->next_sibling->lex_token;
    from->lineNum = RUID1->lineNo;
    from->ruid = (RUID1->lexeme);
    from->union_or_record = ((struct A *)A->ninf)->UnionOrRecord;
    Token RUID2 = definetypestmt_node->child_first->next_sibling->next_sibling->next_sibling->next_sibling->lex_token;
    to->lineNum = RUID2->lineNo;
    to->ruid = (RUID2->lexeme);
    to->union_or_record = -1;

    // ast_node** arr;
    // arr = (ast_node **) malloc (1*sizeof(ast_node*));
    // arr[0] =A;
    definetypestmt_node->addr = makeNode(definetypestmt_, NULL, 0, NULL);
    // free(A);
}

void ast_r94(tNode *A_node)
{
    struct A *info = (struct A *)malloc(sizeof(struct A));
    info->UnionOrRecord = TK_RECORD;
    A_node->addr = makeNode(A_, NULL, 0, (struct node_info *)info);
}

void ast_r95(tNode *A_node)
{
    struct A *info = (struct A *)malloc(sizeof(struct A));
    info->UnionOrRecord = TK_UNION;
    A_node->addr = makeNode(A_, NULL, 0, (struct node_info *)info);
}

void (*func_ptr[95])(tNode *) = {
    ast_r1,
    ast_r2,
    ast_r3,
    ast_r4,
    ast_r5,
    ast_r6,
    ast_r7,
    ast_r8,
    ast_r9,
    ast_r10,
    ast_r11,
    ast_r12,
    ast_r13,
    ast_r14,
    ast_r15,
    ast_r16,
    ast_r17,
    ast_r18,
    ast_r19,
    ast_r20,
    ast_r21,
    ast_r22,
    ast_r23,
    ast_r24,
    ast_r25,
    ast_r26,
    ast_r27,
    ast_r28,
    ast_r29,
    ast_r30,
    ast_r31,
    ast_r32,
    ast_r33,
    ast_r34,
    ast_r35,
    ast_r36,
    ast_r37,
    ast_r38,
    ast_r39,
    ast_r40,
    ast_r41,
    ast_r42,
    ast_r43,
    ast_r44,
    ast_r45,
    ast_r46,
    ast_r47,
    ast_r48,
    ast_r49,
    ast_r50,
    ast_r51,
    ast_r52,
    ast_r53,
    ast_r54,
    ast_r55,
    ast_r56,
    ast_r57,
    ast_r58,
    ast_r59,
    ast_r60,
    ast_r61,
    ast_r62,
    ast_r63,
    ast_r64,
    ast_r65,
    ast_r66,
    ast_r67,
    ast_r68,
    ast_r69,
    ast_r70,
    ast_r71,
    ast_r72,
    ast_r73,
    ast_r74,
    ast_r75,
    ast_r76,
    ast_r77,
    ast_r78,
    ast_r79,
    ast_r80,
    ast_r81,
    ast_r82,
    ast_r83,
    ast_r84,
    ast_r85,
    ast_r86,
    ast_r87,
    ast_r88,
    ast_r89,
    ast_r90,
    ast_r91,
    ast_r92,
    ast_r93,
    ast_r94,
    ast_r95
};

void ast_create(tNode *root)
{
    if (root->is_terminal)
        return;
    tNode *child_ptr = root->child_first;
    while (child_ptr)
    {
        ast_create(child_ptr);
        child_ptr = child_ptr->next_sibling;
    }
    printf("%s\n", non_terminal_map[root->nt]);
    func_ptr[root->rule_no - 1](root);
}