WS      [ \t]
D       [0-9]
L	      [A-Za-z]
ID       ({L}|\$|_)({L}|{D}|\$|_)*
NUM     {D}*\.?{D}+([Ee][-+]?{D}+)?
NEG_NUM (-){D}*\.?{D}+([Ee][-+]?{D}+)?
STRING  (\"([^\\\"]|\\.|\"\")*\")|('([^\\\']|\\.|\'\')*')


%%

{WS}      { }

"\n"      { yylval.l++; }

{NEG_NUM} { yylval.c = blank + yytext; return NEG_NUM; }

{NUM}     { yylval.c = blank + yytext; return NUM; }

"let"	    { yylval.c = blank + "error"; return LET; }

"if"	    { yylval.c = blank + "error"; return IF; }

{STRING}  { yylval.c = blank + yytext; return STR; }

{ID}      { yylval.c = blank + yytext; return ID; }

.         { yylval.c = blank + yytext; return yytext[0]; }

%%