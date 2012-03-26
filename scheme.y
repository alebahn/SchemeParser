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

expression:		VARIABLE		{printf("expression "); }
	  |		literal			{printf("expression "); }
	  |		procedure_call		{printf("expression "); }
	  |		lambda_expression	{printf("expression "); }
	  ;

literal:		quotation	{printf("literal "); }
       |		number		{printf("literal "); }
       |		STRING		{printf("literal "); }
       ;

number:			NUMBER		{printf("number "); }
      |			FNUMBER		{printf("number "); }
      ;

quotation:		QUOTE_MARK datum	{printf("quotation "); }
	 |		'(' "quote" datum ')'	{printf("quotation "); }

datum:			number		{printf("datum "); }
     |			STRING		{printf("datum "); }
     |			VARIABLE	{printf("datum "); }
     |			list		{printf("datum "); }
     ;

list:			'(' datums ')'	{printf("list "); }
    ;

datums:			datums datum
      |
      ;

procedure_call:		'(' operator expressions ')'	{printf("procedure_call "); }
	      ;

operator:		VARIABLE	{printf("operator "); }
	|		'+'		{printf("operator "); }
	|		'-'		{printf("operator "); }
	|		'*'		{printf("operator "); }
	|		'/'		{printf("operator "); }
	;

expressions:		expressions expression
	   |
	   ;

lambda_expression:	'(' "lambda" formals expression ')'	{printf("lambda_expression "); }
		 ;

formals:		'(' variables ')'	{printf("formals "); }
       |		VARIABLE		{printf("formals "); }
       ;

variables:		variables VARIABLE
	 |		
	 ;
