CPP = gcc
FLAGS = -Wall -Werror
EXEC = simulate

default:
	gcc -o simulate PageReplace.c -Wall -Werror

clean:
	rm -f ${EXEC}

run: ${EXEC}
	./${EXEC}

${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS}