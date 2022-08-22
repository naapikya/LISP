NAME =  lisp.exe
CC = g++
RM = rm -rf
OBJS = ${SRCS:.cpp=.o}

all:  ${NAME}

${NAME}: ${OBJS}
	${CC}  *.cpp -o ${NAME}

clean: 
	${RM} *.o

fclean: clean
	${RM} *.exe 

re : fclean all

.PHONY: all clean fclean re
