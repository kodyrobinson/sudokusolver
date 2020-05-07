CC=gcc
CC_FLAGS=-Wall -Wextra -std=c11 -g
TARGET=solver
OBJ_FILES=eval.o solve.o

${TARGET}: ${OBJ_FILES}
	${CC} ${CC_FLAGS} ${OBJ_FILES} -o ${TARGET}

eval.o: src/eval.c
	${CC} ${CC_FLAGS} -I./include -c src/eval.c

solve.o: src/solve.c
	${CC} ${CC_FLAGS} -I./include -c src/solve.c
