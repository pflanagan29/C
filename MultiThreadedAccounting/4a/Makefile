# Makefile for lab4a

# target all means all targets currently defined in this file
all: lab4a lab4a.backup.txt

# this target creates the lab4a executable
lab4a: lab4a.c
	gcc -ansi -pedantic -g -lpthread -lrt -o lab4a lab4a.c

# this target creates a backup lab4a.c file
lab4a.backup.txt: lab4a.c
	cp lab4a.c lab4a.backup.txt

# this target deletes all files produced from the Makefile
# so that a completely new compile of all items is required
clean:
	rm *.txt lab4a
