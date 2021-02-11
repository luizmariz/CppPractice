%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

map<string, int> symbolTable = {};

struct Attr {
  vector<string> c;
  int l = 0;
};

#define YYSTYPE Attr

int yylex();
int yyparse();
void yyerror(const char *);

void registerSymbol(vector<string> v, int i);
void checkSymbol(vector<string> v);

string generate_label(string p);
vector<string> get_address( vector<string> input);

void print(vector<string> s);

vector<string> concat(string a, string b);
vector<string> operator+( vector<string> a, vector<string> b);
vector<string> operator+( vector<string> a, string b);

vector<string> blank;
%}

%token NUM ID LET STR NEG_NUM IF
%start S

%right '='
%nonassoc '[' '.'
%nonassoc '<' '>' _EQ
%left '+' '-'
%left '*' '/' '%'

%%

S: CMDs { print(get_address($1.c)); }
;

CMDs: CMD ';' CMDs { $$.c = $1.c + $3.c; }
| { $$.c = blank; }
;

CMD : A { $$.c = $1.c + "^"; }
| LET DECLVARS { $$ = $2; }
| IF '(' E ')' CMD { string endif = generate_label("end_if"); $$.c = $3.c + "!" + endif + "?" + $5.c + (":" + endif); }
;

DECLVARS: DECLVAR ',' DECLVARS { $$.c = $1.c + $3.c; }
| DECLVAR
;

DECLVAR: ID '=' E { registerSymbol($1.c, $1.l); $$.c = $1.c + "&" + $1.c  + $3.c + "=" + "^"; }
| ID { registerSymbol($1.c, $1.l); $$.c = $1.c + "&"; }
;

A: ID '=' A { checkSymbol($1.c); $$.c = $1.c + $3.c + "="; }
| LVALUEPROP '=' A { $$.c = $1.c + $3.c + "[=]"; }
| E
;

LVALUEPROP: E '[' E ']' { $$.c = $1.c  + $3.c; }
| E '.' ID { $$.c = $1.c + $3.c; }

E : E _EQ E
| E '<' E { $$.c = $1.c + $3.c + "<"; }
| E '*' E { $$.c = $1.c + $3.c + "*"; }
| E '+' E { $$.c = $1.c + $3.c + "+"; }
| E '-' E { $$.c = $1.c + $3.c + "-"; }
| E '/' E { $$.c = $1.c + $3.c + "/"; }
| E '>' E { $$.c = $1.c + $3.c + ">"; }
| E '%' E { $$.c = $1.c + $3.c + "%"; }
| F
;

F: ID { checkSymbol($1.c); $$.c = $1.c + "@"; }
| LVALUEPROP { $$.c = $1.c + "[@]"; }
| NUM { $$.c = $1.c; }
| NEG_NUM { $$.c = blank + "0" + $1.c[0].substr(1, $1.c[0].size() - 1) + "-"; }
| STR { $$.c = $1.c; }
| '(' E ')'   { $$.c = $2.c; }
| '{' '}' { $$.c = blank + "{}"; }
| '[' ']' { $$.c = blank + "[]"; }
;

%%

#include "lex.yy.c"

vector<string> concat(vector<string> a, vector<string> b) {
  a.insert( a.end(), b.begin(), b.end() );
  return a;
}

vector<string> operator+( vector<string> a, vector<string> b ) {
  return concat( a, b );
}

vector<string> operator+( vector<string> a, string b ) {
  a.push_back( b );
  return a;
}

string generate_label(string p) {
  static int n = 0;
  return p + "_" + to_string( ++n ) + ":";
}

vector<string> get_address(vector<string> input) {
  map<string,int> label;
  vector<string> saida;
  for( int i = 0; i < input.size(); i++ )
    if( input[i][0] == ':' )
        label[input[i].substr(1)] = saida.size();
    else
      saida.push_back( input[i] );

  for( int i = 0; i < saida.size(); i++ )
    if( label.count( saida[i] ) > 0 )
        saida[i] = to_string(label[saida[i]]);

  return saida;
}

void print(vector<string> s) {
  for (int i = 0; i < s.size(); i++)
    cout << s[i] << endl;
  cout << '.' << endl;
}

void registerSymbol(vector<string> v, int i) {
  string s = v[0];

  if (symbolTable[s] == 0) {
    symbolTable[s] = i+1;
  } else {
    cout << "Erro: a variável '" + s + "' já foi declarada na linha " + to_string(symbolTable[s]) + ".\n";
    exit( 1 );
  }
}

void checkSymbol(vector<string> v) {
  string s = v[0];

  if (symbolTable[s] == 0) {
    cout << "Erro: a variável '" + s + "' não foi declarada.\n";
    exit( 1 );
  }
}

void yyerror( const char* st ) {
  puts( st );
  printf( "Proximo a: %s\n", yytext );
  exit( 1 );
}

int main( int argc, char* argv[] ) {
  yyparse();
  return 0;
}