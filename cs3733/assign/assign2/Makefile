a2Objects = prog.o

prog.o: prog.c
	gcc -g -c prog.c -o prog.o
	
prog: ${a2Objects}
	gcc -g -o prog ${a2Objects}

clean:
	rm -f ${a2Objects}
