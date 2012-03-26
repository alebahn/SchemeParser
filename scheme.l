%%
[\t ]+ {printf("skipping whitespace\n");}
[0-9]+ {printf("that's an int number\n");}
[0-9]+\.[0-9]+ {printf("that's a float number\n");}
lambda |
define |
car |
quote {printf("that's %s\n", yytext);}
\"[^\"]*\" {printf("that's a string\n");}
' {printf("that's a quote mark\n");}
[a-zA-Z][a-zA-Z0-9]* {printf("that's a variable");}
. {printf("that's annother character");}
%%
main()
{
	yylex();
}
