assign7OBJECTS = assign7.o

assign7.o: assign7.c assign7.h
	gcc -g -c assign7.c -o assign7.o

assign7: ${assign7OBJECTS}
	gcc -g -o assign7 ${assign7OBJECTS}

clean:
	rm -f ${assign7OBJECTS}
