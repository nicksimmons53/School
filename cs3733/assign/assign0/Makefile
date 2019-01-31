driver1Objects = myio.o driver1.o
driver2Objects = myio.o driver2.o
driver1.o: driver1.c
	gcc -g -c driver1.c -o driver1.o

driver2.o: driver2.c
	gcc -g -c driver2.c -o driver2.o

myio.o: myio.c
	gcc -g -c myio.c -o myio.o

driver1: ${driver1Objects}
	gcc -g -o driver1 ${driver1Objects}

driver2: ${driver2Objects}
	gcc -g -o driver2 ${driver2Objects}
