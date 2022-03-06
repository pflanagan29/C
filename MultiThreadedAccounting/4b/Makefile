# Makefile for lab4b

# target all means all targets currently defined in this file
all: lab4b lab4b.backup.txt

# this target creates the lab4a executable
lab4b: lab4b.c
	gcc -ansi -pedantic -g -lpthread -lrt -o lab4b lab4b.c

# this target creates a backup lab4a.c file
lab4b.backup.txt: lab4b.c
	cp lab4b.c lab4b.backup.txt

# this target deletes all files produced from the Makefile
# so that a completely new compile of all items is required
clean:
	rm *.txt lab4b
