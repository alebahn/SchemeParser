%{
#include "scheme.tab.h"
%}
%%
[\t ]+
[0-9]+ {yylval.ival=atoi(yytext); return NUMBER;}
[0-9]+\.[0-9]+ {yylval.fval=atof(yytext); return FNUMBER;}
\"[^\"]*\" {yylval.sval=yytext; return STRING;}
lambda {return LAMBDA;}
define {return DEFINE;}
quote {return QUOTE;}
' {return QUOTE_MARK;}
	[a-zA-Z][a-zA-Z0-9]* {yylval.sval=malloc(strlen(yytext)+1); strcpy(yylval.sval,yytext); return VARIABLE;}
. {return yytext[0];}
%%
int main(int argc, char** argv)
{
	initializeTable();
	FILE *file;
	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (!file) {
			fprintf(stderr, "Error: couldn't open %s\n", argv[1]);
			exit(1);
		}
		yyin = file;
	}
	yyparse();
}
