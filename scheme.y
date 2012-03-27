%{
	#include <stdio.h>
%}
%code requires {
	#include "semantics.h"
}
%union {
	float fval;
	int ival;
	char *sval;
	datum *dval;
}
%token QUOTE_MARK
%token <sval> STRING
%token <ival> NUMBER
%token <fval> FNUMBER
%token <sval> VARIABLE
%token QUOTE "quote"
%token DEFINE "define"
%token LAMBDA "lambda"

%type <dval> command;
%type <dval> expression;
%type <dval> literal;
%type <dval> number;
%type <dval> quotation;
%type <dval> datum;
%type <dval> list;
%type <dval> datums;
%type <dval> procedure_call;
%type <sval> operator;
%type <dval> expressions;
%type <dval> lambda_expression;
%type <dval> formals;
%type <dval> variables;
%%

commands_or_defs:	commands_or_defs command_or_def
		|	command_or_def
		;

command_or_def:		define
	      |		command	{ printDatum($1); }
	      ;

define:			'(' "define" VARIABLE expression ')'	{defineVar($3, $4); }
      ;

command:		expression	{ $$ = executeCommand($1); }
       ;

expression:		VARIABLE		{$$ = malloc(sizeof(datum));
	  					*$$ = (datum){D_STR, {.valStr=$1}}; }
	  |		literal			{$$ = $1; }
	  |		procedure_call		{$$ = $1; }
	  |		lambda_expression	{$$ = $1; }
	  ;

literal:		quotation	{$$ = $1; }
       |		number		{$$ = $1; }
       |		STRING		{$$ = malloc(sizeof(datum));
     					*$$ = (datum){D_STR, {.valStr=$1}}; }
       ;

number:			NUMBER		{$$ = malloc(sizeof(datum));
      					*$$ = (datum){D_INT, {.valInt=$1}}; }
      |			FNUMBER		{$$ = malloc(sizeof(datum));
      					*$$ = (datum){D_FLOAT, {.valFloat=$1}}; }
      ;

quotation:		QUOTE_MARK datum	{$$ = $2; }
	 |		'(' "quote" datum ')'	{$$ = $3; }

datum:			number		{$$ = $1; }
     |			STRING		{$$ = malloc(sizeof(datum));
     					*$$ = (datum){D_STR, {.valStr=$1}}; }
     |			VARIABLE	{$$ = malloc(sizeof(datum));
     					*$$ = (datum){D_STR, {.valStr=$1}}; }
     |			list		{$$ = $1; }
     ;

list:			'(' datums ')'	{$$ = $2; }
    ;

datums:			datum datums	{$$ = malloc(sizeof(datum));
      					*$$ = (datum){D_CONS, {.valCons=(cons){$1, $2}}}; }
      |			/*empty*/	{$$ = malloc(sizeof(datum));
      					*$$ = (datum){D_NULL}; }
      ;

procedure_call:		'(' operator expressions ')'	{$$ = malloc(sizeof(datum));
	      						*$$ = (datum){D_CONS, {.valCons=(cons){lookupVar($2), $3}}}; }
	      ;

operator:		VARIABLE	{$$ = $1; }
	|		'+'		{$$ = "+"; }
	|		'-'		{$$ = "-"; }
	|		'*'		{$$ = "*"; }
	|		'/'		{$$ = "/"; }
	;

expressions:		expression expressions	{$$ = malloc(sizeof(datum));
	   					*$$ = (datum){D_CONS, {.valCons=(cons){$1, $2}}}; }
	   |		/*empty*/		{$$ = malloc(sizeof(datum));
	   					*$$ = (datum){D_NULL}; }
	   ;

lambda_expression:	'(' "lambda" formals expression ')'	{$$ = createLambda($3, $4); }
		 ;

formals:		'(' variables ')'	{$$ = $2; }
       |		VARIABLE		{datum* dNull = malloc(sizeof(datum));
       						datum* dVar = malloc(sizeof(datum));
						$$ = malloc(sizeof(datum));
						*dNull = (datum){D_NULL};
						*dVar = (datum){D_STR, {.valStr=$1}};
						*$$ = (datum){D_CONS, {.valCons=(cons){dVar, dNull}}}; }
       ;

variables:		VARIABLE variables	{datum* dVar = malloc(sizeof(datum));
	 					$$ = malloc(sizeof(datum));
						*dVar = (datum){D_STR, {.valStr=$1}};
						*$$ = (datum){D_CONS, {.valCons=(cons){dVar, $2}}}; }
	 |		/*empty*/		{$$ = malloc(sizeof(datum));
	 					*$$ = (datum){D_NULL}; }
	 ;
