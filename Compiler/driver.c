/*
Author: Abel Joise
*/

#include "interface.h"
#include "keyword_table.h"
#include "lexer.h"
#include "parser.h"
#include "nary_tree.h"
#include "ast.h"
#include "printer.h"
#include "error_handler.h"
#include "symbol_table.h"
#include "semantic_analyzer.h"
#include "type_checker.h"
#include "code_gen.h"
#include <time.h>

int main(int argc, char* argv[]) {
    int userOption;

    if(argc != 3) {
        printf("No of arguments not sufficient \n");
        return 0;
    }

    printf("\n\n");
    printf("-------- Compiler --------\n");
    printf("Lexing, parsing, AST construction, semantic analysis, and NASM code generation\n");
    printf("Build and run generated assembly with:\n");
 	printf("nasm -felf64 code.asm && gcc code.o && ./a.out\n");
    printf("--------------------------\n\n");


    printf("Enter 0 to exit\n");
    printf("Enter 1 to print the list of tokens created by the lexer\n");
    printf("Enter 2 to print the parse tree\n");
    printf("Enter 3 to print the AST, (Please ensure that the input creates a valid AST)\n");
    printf("Enter 4 to calculate the compression ratio\n");
    printf("Enter 5 to print the symbol table\n");
    printf("Enter 6 to print information about the global variables\n");
    printf("Enter 7 to print information about the functions\n");
    printf("Enter 8 to print all the records\n");
    printf("Enter 9 to verify correctness of the program\n");
    printf("Enter 10 to generate NASM code\n");

    while(1) {

        scanf("%d", &userOption);

        switch(userOption) {
                case 0: {
                    return 0;
                    break;
                }
                case 1: {
                    printf("Commencing printing of token list \n");
                    printf("--------\n");
                    int f = open(argv[1],O_RDONLY);
                    if(f < 0) {
                        printf("Error: unable to open input source file %s\n", argv[1]);
                        break;
                    }
                    initializeLexer(f);
                    if(isLexerInitialized() == 0) {
                        close(f);
                        break;
                    }
                    Token* t;
                    while((t = getToken()) != NULL) {
                        printf("%s        %s        %d \n", getTerminal(t->TOKEN_NAME),t->LEXEME,t->LINE_NO);
                    }
                    close(f);

                    printf("\nFinished printing of token list \n");
                    break;
                }
                case 2: {
                    printf("Commencing parsing of input source code \n");
                    printf("--------\n");

                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    // Lexer and parser are both invoked inside parseInputSourceCode
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }
                    printParseTree(pt,NULL);

                    printf("\nFinished parsing of input source code \n");
                    break;
                }
                case 3: {

                    printf("Printing AST in a level format (Each level being printed)\n");
                    printf("Each node will have it's parent being printed");
                    // Adding thhe initialisation of structures also in time calculation
                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }

                    AST* ast = constructAST(pt);

                    levelPrint(ast->root);

                    printf("Completed printing AST\n");
                    break;
                }
                case 4: {

                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }

                    AST* ast = constructAST(pt);

                    printf("Parse tree Number of nodes = %d Allocated Memory = %d Bytes\n", getParseTreeNodeCount(), getParseTreeMemory());

                    printf("AST Number of nodes = %d Allocated Memory = %d Bytes\n", getASTNodeCount(), getASTMemory());

                    float compression = (100*((float)getParseTreeMemory()-(float)getASTMemory()))/((float) getParseTreeMemory());
                    printf("Compression is %f\n" ,compression);
                    break;
                }
                case 5: {
                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }

                    AST* ast = constructAST(pt);

                    ErrorList* els = initializeErrorList();

                    SymbolTable* st = constructSymbolTable(ast,els);

                    captureErrors(ast,els);

                    printSymbolTable(st,1);
                    break;
                }
                case 6: {


                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }

                    AST* ast = constructAST(pt);

                    ErrorList* els = initializeErrorList();

                    SymbolTable* st = constructSymbolTable(ast,els);

                    captureErrors(ast,els);

                    printGlobals(st);
                    break;
                }
                case 7: {

                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }

                    AST* ast = constructAST(pt);

                    ErrorList* els = initializeErrorList();

                    SymbolTable* st = constructSymbolTable(ast,els);

                    captureErrors(ast,els);

                    printFunctions(st);
                    break;
                }
                case 8: {
                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }


                    AST* ast = constructAST(pt);

                    ErrorList* els = initializeErrorList();

                    SymbolTable* st = constructSymbolTable(ast,els);

                    captureErrors(ast,els);

                    printRecords(st);
                    break;
                }
                case 9: {


                    clock_t    start_time, end_time;
                    double total_CPU_time, total_CPU_time_in_seconds;
                    start_time = clock();

                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }
                    else {
                        /**First Pass**/
                        AST* ast = constructAST(pt);

                        ErrorList* els = initializeErrorList();

                        /**Second Pass**/
                        SymbolTable* st = constructSymbolTable(ast,els);

                        /**Third Pass**/
                        captureErrors(ast,els);


                        printf("----PRINTING ERRORS-----\n");
                        printErrors(els);
                    }

                    end_time = clock();
                    total_CPU_time  =  (double) (end_time - start_time);
                    total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

                    printf("Calculated time is %f \n",total_CPU_time);
                    printf("Calculated time in seconds is %f \n", total_CPU_time_in_seconds);

                    break;
                }
                case 10: {

                    Grammar* g = extractGrammar();
                    if(g == NULL) {
                        break;
                    }
                    FirstAndFollow* fafl = computeFirstAndFollowSets(g);
                    ParsingTable* pTable = initialiseParsingTable();
                    createParseTable(fafl,pTable);
                    ParseTree* pt = parseInputSourceCode(argv[1],pTable,fafl);
                    if(pt == NULL) {
                        break;
                    }

                    if(getErrorStatus() == 1) {
                        printf("PARSING OR LEXICAL ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO AST CONSTRUCTION\n");
                        break;
                    }
                    else {
                        /**First Pass**/
                        AST* ast = constructAST(pt);

                        ErrorList* els = initializeErrorList();

                        /**Second Pass**/
                        SymbolTable* st = constructSymbolTable(ast,els);

                        /**Third Pass**/
                        captureErrors(ast,els);


                        printf("----PRINTING ERRORS-----\n");
                        printErrors(els);

                        if(els->head != NULL) {
                            printf("SEMANTIC ANALYSIS YIELDED ERRORS, NOT PROCEEDING TO CODE GENERATION\n");
                            break;
                        }

                        FILE* f = fopen(argv[2],"w");
                        if(f == NULL) {
                            printf("Error: unable to open output file %s\n", argv[2]);
                            break;
                        }
                        codeGeneration(ast,st,f);
                        fclose(f);
                        printf("Completed code generation\n");
                    }
                    break;
                }
                default: {
                    continue;
                }

        }

    }

}
