simObject = sim.o

sim.o: sim.c
	gcc -g -c sim.c -o sim.o

sim: ${simObject}
	gcc -g -o sim ${simObject} -lm

clean:
	rm -f ${simObject}
