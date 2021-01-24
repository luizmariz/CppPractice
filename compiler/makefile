all: out in.txt
	./out < in.txt

clean:
	rm out lex.yy.c

scan: scan.lex
	lex scan.lex

tradutor: tradutor.l
	flex tradutor.l

scan-out: lex.yy.c main.cc
	g++ -Wall -std=c++17 main.cc -o out -lfl

tradutor-out: lex.yy.c
	g++ -Wall -std=c++17 lex.yy.c -o out -ll

stack-out: stack.cc
	g++ -Wall -std=c++17 stack.cc -o stack