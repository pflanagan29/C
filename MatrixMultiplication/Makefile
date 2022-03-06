# Makefile for lab5

# target all means all targets currently defined in this file
all: lab5 lab5.backup.txt

# this target creates the lab5 executable
lab5: lab5.c
	gcc -ansi -pedantic -g -lpthread -lrt -o lab5 lab5.c

# this target creates a backup lab5.c file
lab5.backup.txt: lab5.c
	cp lab5.c lab5.backup.txt

# this target deletes all files produced from the Makefile
# so that a completely new compile of all items is required
clean:
	rm *.txt lab5
