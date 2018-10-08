CFLAGS = gcc -g
OBJ_CS = candyServer.o msleep.o errExit.o
OBJ_CC = candyClient.o msleep.o errExit.o

# Automatic substitution using suffix rules to
# make .o files from either .c or .cpp files
%.o: %.c
	${CFLAGS} -c $< 

%.o: %.cpp
	${CFLAGS} -c $<

# Build executable based on the required .o files
candyS: ${OBJ_CS}
	${CFLAGS} -o candyS ${OBJ_CS}

candyC: ${OBJ_CC}
	${CFLAGS} -o candyC ${OBJ_CC}
