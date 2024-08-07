CC		=	gcc
SRCS	=	tictactoe.c \
			src/helper.c \
			src/minimax_threaded.c

OBJS	= ${SRCS:.c=.o}

RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror -fsanitize=address -g
LDFLAGS	= -pthread

NAME	= tictactoe

.PHONY: all clean fclean re

all: ${NAME}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LDFLAGS} -o ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all