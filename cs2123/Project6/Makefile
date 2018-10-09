# Define the machine object files for your program
OBJECTS = p6srz888.o p5Driver.o p5qzn876.o p5tae015.o 
# Define your include file
INCLUDES = cs2123p6.h 

# make for the executable
p6: ${OBJECTS}
	gcc -g -o p6 ${OBJECTS}

# Simple suffix rules for the .o
%.o: %.c ${INCLUDES}
	gcc -g -c -std=c99 $<

# Clean the .o files
clean:
	rm -f ${OBJECTS}
