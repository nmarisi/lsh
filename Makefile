all: shell 

shell: shell.c parser.c parser.h
	clang parser.c shell.c -o shell

file:
	clang file.c -o file

clean:
	rm -f shell 
