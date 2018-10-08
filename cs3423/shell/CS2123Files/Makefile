# Define the machine object files for your program
OBJECTS = p1abc123.o cs2123p1Driver.o
# Define your include file
INCLUDES = cs2123p1.h

# make for the executable
p1: ${OBJECTS}
	gcc -g -o p1 ${OBJECTS}

# Simple suffix rules for the .o
%.o: %.c ${INCLUDES}
	gcc -g -c $<

# Clean the .o files
clean:
	rm -f ${OBJECTS}