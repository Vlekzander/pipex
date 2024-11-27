CC=gcc
CFLAGS=-Wall -Wextra -Werror -Iinclude -I$(LIBFT_DIR)/include
LDFLAGS=-L$(LIBFT_DIR)
LIBS=-lft
SOURCES=src/main.c src/pipex.c src/pipex_error.c src/pipex_files.c src/pipex_memory.c src/pipex_fds.c
SOURCES_BONUS=src/main_bonus.c src/pipex.c src/pipex_error.c src/pipex_files.c src/pipex_memory.c src/pipex_fds.c
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_BONUS=$(SOURCES_BONUS:.c=.o)
NAME=pipex

LIBFT_DIR=lib/libft
LIBFT_LIB=$(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJECTS)
	@make -C $(LIBFT_DIR) --no-print-directory
	@$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(NAME)
	@echo "\033[2K\r$(NAME) compilation complete."

bonus: $(OBJECTS_BONUS)
	@make -C $(LIBFT_DIR) --no-print-directory
	@$(CC) $(OBJECTS_BONUS) $(LDFLAGS) $(LIBS) -o $(NAME)
	@echo "\033[2K\r$(NAME) compilation complete."

.c.o:
	@printf "\033[2K\rCompiling %s..." $<
	@$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

clean:
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@rm -f $(OBJECTS)
	@rm -f $(OBJECTS_BONUS)
	@echo "Object files removed."

fclean: clean
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@rm -f $(NAME)
	@echo "$(NAME) removed."

re: fclean all
