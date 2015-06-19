CC = gcc
NAME = bomberman
SRC  = main.c

CFLAGS = -W -Wall -Werror -fPIC -g3 -Wextra
OBJ = $(SRC:%.c=%.o)
TMP = *~
RM  = rm -f
PWD = $(shell pwd)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -g3 -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(TMP)

re:  fclean all fclean
