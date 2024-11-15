CC=gcc
CFLAGS=-Wall -Wextra -Werror -I$(LIBFT_DIR)/include
LDFLAGS=-L$(LIBFT_DIR)
LIBS=-lft
SOURCES=src/main.c
OBJECTS=$(SOURCES:.c=.o)
NAME=pipex

LIBFT_DIR=lib/libft
LIBFT_LIB=$(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(LIBFT_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

clean:
	rm -f $(OBJECTS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all
