# Define the machine object files for your program
OBJECTS = p5abc123.o cs1713p5Driver.o
# Define your include file
INCLUDES = cs1713p5.h

# make for the executable
p5: cs1713p5Driver.o p5srz888.o
	gcc -g -o p5 cs1713p5Driver.o p5srz888.o

# Simple suffix rules for the .o
p5Driver.o: cs1713p5Driver.c cs1713p5.h
	gcc -g -c cs1713p5Driver.c -o cs1713p5Driver.o 
p5srz888.o: p5srz888.c cs1713p5.h
	gcc -g -c p5srz888.c -o p5srz888.o

# Clean the .o files
clean:
	rm -f cs1713p5Driver.o p5srz888.o
