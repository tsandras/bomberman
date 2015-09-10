CC = gcc
NAME = bomberman

INC = -I inc/
CFLAGS = -W -Werror -Wall -g -ggdb -g3 $(INC)
OBJ = src/*.c lib/*.c
TMP = *~
RM  = rm -f
PWD = $(shell pwd)

all:
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

clean:
	$(RM) src/*.o lib/*.o

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TMP)
	$(RM) -r $(NAME).dSYM/

re:  fclean all fclean
