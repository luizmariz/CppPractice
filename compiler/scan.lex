WS  [ \t\n]
D [0-9]
L	[A-Za-z]

/* General */
ID  ({L}|\$|_)({L}|{D}|\$|_)*
INT (0|[1-9]{D}*)
FLOAT {D}*\.?{D}+([Ee][-+]?{D}+)?
FOR [Ff][Oo][Rr]
IF [Ii][Ff]

/* Comments */
C_SIMPLE \/\/[^\n]*
%x C_MULTI

STRING \"([^\\\"]|\\.|\"\")*\"

%%

{WS}	{}

{FOR} { return _FOR; }
{IF} { return _IF; }
">=" { return _MAIG; }
"<=" { return _MEIG; }
"==" { return _IG; }
"!=" { return _DIF; }
{ID}	{ return _ID; }
{INT}   { return _INT; }
{FLOAT} { return _FLOAT; }

{C_SIMPLE} { return _COMENTARIO; }

<INITIAL>"/*" { yymore(); BEGIN(C_MULTI); }
<C_MULTI>{
  [^*] { yymore(); }
  \*/[^/] { yymore(); }
  "*/" { BEGIN(INITIAL); return _COMENTARIO; }
}

{STRING} { return _STRING; }

. { return *yytext; }

%%