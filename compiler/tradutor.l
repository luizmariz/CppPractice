%{
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include <string>
#include <map>
#include <vector>

using namespace std;

int token;

void AS();
void A();
void E();
void E_AUX();
void T();
void T_AUX();
void F();
void P();

void print(string);
int nextToken();
string getTokenName(int);
void check(int);

enum TOKEN { _ID = 256, _NUM, _STRING };

map<int,string> tokenByName = {
  { _ID, "identificador" },
  { _NUM, "número" },
  { _STRING, "cadeia de caracteres" }
};

%}

WS  [ \t\n]
D [0-9]
L	[A-Za-z]
ID  ({L}|\$|_)({L}|{D}|\$|_)*
NUM {D}*\.?{D}+([Ee][-+]?{D}+)?
STRING \"([^\\\"]|\\.|\"\")*\"

%%

{WS}	{}

{ID}	{ return _ID; }
{NUM} { return _NUM; }
{STRING} { return _STRING; }

. { return yytext[0]; }

%%

void AS() {
  A();

  switch (token) {
    case _NUM: break;
    case _STRING: break;
    case _ID: cout << "\n"; AS(); break;
  }
}

void A() {
  string temp = yytext;
  check(_ID);

  if (temp == "print") {
    E();
    print("print #");
    check(';');
  } else {
    print(temp);
    check('=');
    E();
    check(';');
    print("=");
  }
}

void E() {
  T();
  E_AUX();
}

void E_AUX() {
  switch (token) {
    case '+': check('+'); T(); print("+"); E_AUX(); break;
    case '-': check('-'); T(); print("-"); E_AUX(); break;
  }
}

void T() {
  F();
  T_AUX();
}

void T_AUX() {
  switch (token) {
    case '*': check('*'); F(); print("*"); T_AUX(); break;
    case '/': check('/'); F(); print("/"); T_AUX(); break;
  }
}

void F() {
  string temp;

  switch (token) {
    case _ID:
      temp = yytext;
      check(_ID);
      if (token == '(') {
        check('(');
        P();
        check(')');
        print(temp + " #");
      } else { print(temp + " @"); }
      break;
    case _NUM:
      temp = yytext;
      check(_NUM);
      print(temp);
      break;
    case _STRING:
      temp = yytext;
      check(_STRING);
      print(temp);
      break;
    case '(':
      check( '(' );
      E();
      check(')');
      break;
    default:
      break;
  }
}

void P() {
  E();
  if (token == ',') {
    check(',');
    E();
  }
}

void print(string s) {
  cout << s + ' ';
}

int nextToken() {
  return yylex();
}

string getTokenName(int token) {
  if (tokenByName.find(token) != tokenByName.end())
    return tokenByName[token];
  else {
    string r;
    r = token;
    return r;
  }
}

void check(int expectedToken) {
  if (token == expectedToken)
    token = nextToken();
  else {
    cout << "\nExpected: " << getTokenName(expectedToken) << " Found: " << getTokenName(token) << endl;
    exit(1);
  }
}

auto p = &yyunput;

int main() {
  token = nextToken();
  AS();
  if ( token != 0 ) return 1;
  return 0;
}