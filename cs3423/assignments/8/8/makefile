a8OBJECTS = assign8.o cmdparse.o

assign8.o: assign8.c cmdparse.h
	gcc -g -c assign8.c -o assign8.o

cmdparse.o: cmdparse.c cmdparse.h
	gcc -g -c cmdparse.c -o cmdparse.o

assign8: ${a8OBJECTS}
	gcc -g -o myshell ${a8OBJECTS}

clean:
	rm -f ${a8Objects}
