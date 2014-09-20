/********************************************************************************
 * Copyright (C) 2014 Daichi Teruya (@maruuusa83)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 *
 * This program is destributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *******************************************************************************/
%{
#include <stdio.h>
#include <stdlib.h>
#include "lambda_utils.h"
%}

%union {
    char *identifier;
    char *name;
    LambdaTerm *lambda_term;
    LambdaAbstraction *lambda_abs;
}

%token LAMBDA QUOTE
%token DEFUN LP RP PERIOD CR
%token <identifier> IDENTIFIER
%token <name> NAME

%type <lambda_term> lambda_term
%type <lambda_abs>  abstraction

%%

line_list
    : line
    | line_list line
    ;

line
    : lambda_term CR
    {
        LambdaTerm *result = NULL;

        result = lambda_eval($1);
        //result = $1;
        lambda_free_term($1);

        printf("\t-> ");
        lambda_print_term(result);
        printf("\n\n");
        
        lambda_free_term(result);
    }
    | NAME DEFUN lambda_term CR
    {
        LambdaTerm *result = NULL;

        result = lambda_eval($3);
        lambda_free_term($3);

        if (lambda_add_defined_term($1, result) == 0){
			printf("\tDEFINED %s -> ", $1);
			lambda_print_term(result);
			printf("\n\n");
        }
        else {
            printf("\t%s -> ERROR : already used identifier\n", $1);
        }

        lambda_free_term(result);
    }
    ;

lambda_term
    : IDENTIFIER
    {
        $$ = lambda_create_term_id($1);

        lambda_free_identifier($1);
    }
    | NAME
    {
        LambdaTerm *term = lambda_get_defined_term($1);

        if (term == NULL){
            yyerror("undefined identifier\n");
            return (1);
        }

        $$ = term;
    }
    | abstraction
    {
        $$ = lambda_create_term_abs($1);

        lambda_free_abstraction($1);
    }
    | LP lambda_term lambda_term RP
    {
        LambdaTerm *t1 = $2;
        LambdaTerm *t2 = $3;

        switch (t1->type){
          case ID:
          case APP:
          case ABS:
            $$ = lambda_create_term_app(t1, t2);
            break;

          //case ABS:
            //$$ = lambda_application(t1, ((t1->data).abs)->param, t2);
            //break;
        }

        lambda_free_term(t1);
        lambda_free_term(t2);
    }
    | LP QUOTE lambda_term RP
    {
        LambdaTerm *result = lambda_create_term_clone($3);
        result->quote_flag = 1;
        $$ = result;

        lambda_free_term($3);
    }
    ;

abstraction
    : LP LAMBDA IDENTIFIER PERIOD lambda_term RP
    {
        $$ = lambda_create_abstraction($3, $5);
        
        lambda_free_identifier($3);
        lambda_free_term($5);
    }
    ;

%%


int yyerror(char const *str)
{
    extern char *yytext;
    fprintf(stderr, str);
    fprintf(stderr, "\nperser error near %s\n", yytext);

    return (0);
}

int main(void)
{
    extern int yyparse(void);
    extern FILE *yyin;
    extern LambdaDefinedTerm defined_term_list;

    defined_term_list.next = NULL;

    printf("***********************************************************\n");
    printf(" Lambda Evaluator v1.2.2, Copyright (C) 2014 Daichi Teruya \n");
    printf("***********************************************************\n");
    printf("\n");
    printf("This program comes with ABSOLUTELY NO WARRANTY. This is free\n");
    printf("software, and you are welcome to redistribute it under certain\n");
    printf("conditions. See the GNU General Public License for more details.\n");
    printf("\n");

    yyin = stdin;
    if (yyparse()){
        fprintf(stderr, "Error! Error!\n");
        //exit(1);
    }

    return (0);
}

