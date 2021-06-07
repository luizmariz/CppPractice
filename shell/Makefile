FLAGS = -g -Wall

default: shell

shell: main.c prompt.c utils.c
	gcc $(FLAGS) -o shell main.c prompt.c utils.c

clean:
	rm shell *.o