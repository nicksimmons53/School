# Define the machine object files for your program
OBJECTS = p5srz888.o p5Driver.o p5qzn876.o
# Define your include file
INCLUDES = cs2123p5.h

# make for the executable
p5: ${OBJECTS}
	gcc -g -o p5 ${OBJECTS}

# Simple suffix rules for the .o
%.o: %.c ${INCLUDES}
	gcc -g -c -std=c99 $<

# Clean the .o files
clean:
	rm -f ${OBJECTS}
