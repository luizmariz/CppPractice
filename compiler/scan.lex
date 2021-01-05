WS	[ \t\n]
DIGITO	[0-9]
LETRA	[A-Za-z_]

ID	{LETRA}({LETRA}|{DIGITO})*

%%

{ID}	{ return _ID; }
{WS}	{ /* ignora espaços, tabs e '\n' */ }
.			{ return *yytext; }

%%