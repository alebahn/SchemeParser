%{
	#include <stdio.h>
%}
%union {
	float fval;
	int ival;
	char *sval;
}
%token <sval> NAME
%token <ival> NUMBER
%token <fval> FNUMBER
%type <fval> expression
%type <fval> term
%type <fval> factor
%%
statement:	NAME '=' expression	{ printf("%s = %f\n", $1, $3);}
	|	expression		{ printf("= %f\n", $1); }
	;

expression:	expression '+' term	{ $$ = $1 + $3; }
	|	expression '-' term	{ $$ = $1 - $3; }
	|	term			{ $$ = $1; }
	;

term:		term '*' factor		{ $$ = $1 * $3; }
    	|	term '/' factor		{ $$ = $1 / $3; }
	|	factor			{ $$ = $1; }
	;

factor:		'(' expression ')'	{ $$ = $2; }
      	|	NUMBER			{ $$ = $1; }
	|	FNUMBER			{ $$ = $1; }

