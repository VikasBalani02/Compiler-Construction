// #include"lexer.h"
// #include "lookupTable.h"

/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include"codegen.h"
#include <stdio.h>
#include <time.h>
#include<unistd.h>
extern NODE lookUpTable;
extern char* terminal_map[]; 
// char *terminal__map[] = {
//     "TK_ASSIGNOP",
//     "TK_COMMENT",
//     "TK_FIELDID",
//     "TK_ID",
//     "TK_NUM",
//     "TK_RNUM",
//     "TK_FUNID",
//     "TK_RUID",
//     "TK_WITH",
//     "TK_PARAMETERS",
//     "TK_END",
//     "TK_WHILE",
//     "TK_UNION",
//     "TK_ENDUNION",
//     "TK_DEFINETYPE",
//     "TK_AS",
//     "TK_TYPE",
//     "TK_MAIN",
//     "TK_GLOBAL",
//     "TK_PARAMETER",
//     "TK_LIST",
//     "TK_SQL",
//     "TK_SQR",
//     "TK_INPUT",
//     "TK_OUTPUT",
//     "TK_INT",
//     "TK_REAL",
//     "TK_COMMA",
//     "TK_SEM",
//     "TK_COLON",
//     "TK_DOT",
//     "TK_ENDWHILE",
//     "TK_OP",
//     "TK_CL",
//     "TK_IF",
//     "TK_THEN",
//     "TK_ENDIF",
//     "TK_READ",
//     "TK_WRITE",
//     "TK_RETURN",
//     "TK_PLUS",
//     "TK_MINUS",
//     "TK_MUL",
//     "TK_DIV",
//     "TK_CALL",
//     "TK_RECORD",
//     "TK_ENDRECORD",
//     "TK_ELSE",
//     "TK_AND",
//     "TK_OR",
//     "TK_NOT",
//     "TK_LT",
//     "TK_LE",
//     "TK_EQ",
//     "TK_GT",
//     "TK_GE",
//     "TK_NE",
//     "DOLLAR",
//     "ERROR",
//     "eps"}; // DOLLAR corresponds to end of input and not the dollar sign

int main(int argc, char **argv)
{
    setbuf(stdout,NULL);
    if(argc != 3) {
        printf("Too many/few arguments provided.");
        return -1;
    }
    printf("\n\033[0;35m****IMPLEMENTATION DETAILS****\n1. FIRST and FOLLOW set automated\n2. Both lexical and syntax analysis modules implemented\n3. In-order traversal of parse tree can be printed in the provided file.\n4. The option 2 cannot be run more than once per execution of the driver.\n5. For error recovery, we are checking the lookahead in the first sets of the non-terminal on the stack and the next nonterminal on the stack and if it doesn't match with either of them then we skip the lookahead and keep searching until we find a match.\nIn case the lookahead is in the first set of the next non-terminal on the stack after the top we simply pop the top element of the stack and continue parsing\033[0m");
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    // start_time = clock();

    FILE *fp;
    fp = fopen("./grammar.txt", "r");

    grammar *gr = load_grammar(fp);
    assign_rule_no(gr);
    FnF *fnf = ComputeFirstAndFollowSets(gr);

    parseTable *pt = parseTableCreate(gr, fnf);

    // the lexer is initialized inside parseInputSourceCode in parser.c
    Tree *tr = parseInputSourceCode(argv[1], pt);
    ast_create(tr->root);
    symbolTable * global;
    // end_time = clock();
    // total_CPU_time = (double)(end_time - start_time);
    // total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    // printf("total_CPU_time: %f" , total_CPU_time);
    // printf("\ntotal_CPU_time_in_seconds: %f" , total_CPU_time_in_seconds);

    FILE *fp1 = fopen(argv[1], "r");
    FILE *fpt = fopen(argv[2], "w");
    // removeComments(argv[1]);
    char menu[] = "\n\033[0;32mOptions Available\n0. Exit\n1. Print token list generated by lexer\n2. Verify syntactic correctness and print parse tree\n3. Print abstract syntax tree\n4. Print memory allocation for syntax tree and ast and compression percentage\n5. Print symbol table\n6. Print all global variables\n7. Print total memory requirement for each function\n8. Print type expressions and width of globally visible record definitions\n9. Type checking and semantic analysis\n10. Code generation and dynamic type checking\n11. Print intermediate code\033[0m";
    // printf("%s\n", menu);
    struct Error* err=(Error*)malloc(sizeof(Error));
    err->next=NULL;
    global = populateSymbolTable(tr->root->addr,err);
    tupleList* complist;
    FILE * f;
    int ast_product, pt_product;
    while (1)
    {
        int option;
        printf("%s\n", menu);
        printf("\nEnter option: ");
        scanf("%d", &option);
        if (option == 0)
        {   
            printf("\n*END OF PROGRAM*");
            // printf("\n OR SO I THINK");
            break;
        }
        switch (option)
        {
        case 1:
            fp1 = initialize_lexer(argv[1]);
            printf("\n%4s | %15s | %22s", "lNum", "tokenType", "lexeme");
            while (1)
            {
                Token tk = getNextToken(&(fp1));
                if (tk == NULL){
                    break;
                }
                    
                printf("\n%4d | %15s | %22s", tk->lineNo, terminal_map[tk->type], tk->lexeme);
            }
            break;
        case 2:
            fprintf(fpt, "\n %20s %25s %7s %20s %25s %10s %10s\n", "lexeme", "CurrentNodeSymbol", "lineNum", "value (0 if not num)", "parentNodeSymbol", "isLeafNode", "Rule No");
            fflush(fpt);
            printParseTree(tr->root, argv[2]);
            break;
        case 3:
            printf("Printing AST Tree in Preorder traversal\n");
            print_ast(tr->root->addr, 0);
            break;
        case 4:
            pt_product = get_pt_product(tr->root);
            ast_product = get_ast_product(tr->root->addr);
            printf("\n");
            printf("Compression percentage: %d\n", ((pt_product-ast_product)*100/pt_product));
            break;
        case 5:
            if(global == NULL) exit(0);
            printSymbolTable(global);
            break;
        case 6:
            if(global == NULL) exit(0);
            printglobalvariables(global);
            break;
        case 7:
            if(global == NULL) exit(0);
            printactivationrecords(global);
            break;
        case 8: 
            if(global == NULL) exit(0);
            printglobalrecords(global);
            break;
        case 9: 
            start_time = clock();
            type_checker(tr->root->addr,err,global,global,-1);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            print_error_list(err);
            printf("\ntotal_cpu_time: %f",total_CPU_time);
            printf("\ntotal_cpu_time_in_seconds: %f", total_CPU_time_in_seconds);
            break;
        case 10:
            complist = get_intermediate_list(tr->root->addr, global);
            
            f = fopen("assembly.asm", "w");
            start_time = clock();
            generate_code(complist, global,f);
            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            printf("\ntotal_cpu_time: %f",total_CPU_time);
            printf("\ntotal_cpu_time_in_seconds: %f", total_CPU_time_in_seconds);
            fclose(f); break;
        case 11:
            printf("******************Intermediate Code******************\n");
            complist = get_intermediate_list(tr->root->addr, global);
            print_tupleList(complist);
        break;
        default:
            printf("\nIncorrect option.");
            break;
        }
    }
}