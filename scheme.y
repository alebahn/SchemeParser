%{
	#include <stdio.h>
%}
%union {
	float fval;
	int ival;
	char *sval;
}
%token QUOTE_MARK
%token <sval> STRING
%token <ival> NUMBER
%token <fval> FNUMBER
%token <sval> VARIABLE
%token QUOTE "quote"
%token DEFINE "define"
%token LAMBDA "lambda"
%%

commands_or_defs:	commands_or_defs command_or_def
		|	command_or_def
		;

command_or_def:		define
	      |		command
	      ;

define:			'(' "define" VARIABLE expression ')'	{ printf("definition"); }
      ;

command:		expression	{printf("command"); }
       ;

expression:		VARIABLE
	  |		literal
	  |		procedure_call
	  |		lambda_expression
	  ;

literal:		quotation
       |		number
       |		STRING
       ;

number:			NUMBER
      |			FNUMBER
      ;

quotation:		QUOTE_MARK datum
	 |		'(' "quote" datum ')'

datum:			number
     |			STRING
     |			VARIABLE
     |			list
     ;

list:			'(' datums ')'
    ;

datums:			datums datum
      |
      ;

procedure_call:		'(' operator expressions ')'
	      ;

operator:		VARIABLE
	|		'+'
	|		'-'
	|		'*'
	|		'/'
	;

expressions:		expressions expression
	   |
	   ;

lambda_expression:	'(' "lambda" formals expression ')'
		 ;

formals:		'(' variables ')'
       |		VARIABLE
       ;

variables:		variables VARIABLE
	 |		
	 ;
