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

%type <dval> expression;
%type <dval> literal;
%type <dval> number;
%type <dval> datum;
%type <dval> procedure_call;
%type <sval> operator;
%type <dval> expressions;
%%

commands_or_defs:	commands_or_defs command_or_def
		|	command_or_def
		;

command_or_def:		define
	      |		command
	      ;

define:			'(' "define" VARIABLE expression ')'	{defineVar($3, $4); }
      ;

command:		expression	{ printDatum($1);}
       ;

expression:		VARIABLE		{$$ = lookupVar($1); }
	  |		literal			{$$ = $1; }
	  |		procedure_call		{$$ = $1; }
	  |		lambda_expression	{printf("expression "); }
	  ;

literal:		quotation	{printf("literal "); }
       |		number		{$$ = $1; }
       |		STRING		{$$ = malloc(sizeof(datum));
     					*$$ = (datum){D_STR, {.valStr=$1}}; }
       ;

number:			NUMBER		{$$ = malloc(sizeof(datum));
      					*$$ = (datum){D_INT, {.valInt=$1}}; }
      |			FNUMBER		{$$ = malloc(sizeof(datum));
      					*$$ = (datum){D_FLOAT, {.valFloat=$1}}; }
      ;

quotation:		QUOTE_MARK datum	{printf("quotation "); }
	 |		'(' "quote" datum ')'	{printf("quotation "); }

datum:			number		{printf("datum "); }
     |			STRING		{$$=malloc(sizeof(datum));
     					*$$=(datum){D_STR, {.valStr=$1}}; }
     |			VARIABLE	{printf("datum "); }
     |			list		{printf("datum "); }
     ;

list:			'(' datums ')'	{printf("list "); }
    ;

datums:			datums datum
      |
      ;

procedure_call:		'(' operator expressions ')'	{$$ = doProcedureCall($2, $3); }
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

lambda_expression:	'(' "lambda" formals expression ')'	{printf("lambda_expression "); }
		 ;

formals:		'(' variables ')'	{printf("formals "); }
       |		VARIABLE		{printf("formals "); }
       ;

variables:		variables VARIABLE
	 |		
	 ;
