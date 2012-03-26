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
quote |
' {return QUOTE;}
[a-zA-Z][a-zA-Z0-9]* {yylval.sval=yytext; return VARIABLE;}
. {return yytext[0];}
%%
main()
{
	yyparse();
}
