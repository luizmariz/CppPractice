FLAGS = -g -Wall

default: shell

shell: main.c prompt.c utils.c jobs.c
	gcc $(FLAGS) -o shell main.c prompt.c utils.c jobs.c

clean:
	rm shell