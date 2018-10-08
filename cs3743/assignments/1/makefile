CFLAGS = gcc -g
OBJ = cs3743p1Driver.o cs3743p1.o 

# Automatic substitution using suffix rules to
# make .o files from either .c or .cpp files
%.o: %.c
	${CFLAGS} -c $< 

%.o: %.cpp
	${CFLAGS} -c $<

# Build p1 based on the required .o files
p1: ${OBJ}
	${CFLAGS} -o p1 ${OBJ}

clean:
	rm ${OBJ}
