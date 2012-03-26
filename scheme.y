%{
	#include <stdio.h>
%}
%union {
	float fval;
	int ival;
	char *sval;
}
%token QUOTE
%token <sval> VARIABLE
%token <sval> KEYWORD
%token <sval> STRING
%token <ival> NUMBER
%token <fval> FNUMBER
%%

commands_or_defs:	command_or_def commands_or_defs
		|	command_or_def
		;

command_or_def:		define
	     ;

define:			'(' "define" VARIABLE expression ')'	{ printf("recognized definition"); }
      ;

expression:		VARIABLE
	  ;
