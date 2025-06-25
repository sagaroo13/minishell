CC = cc
FLAGS = -Werror -Wall -Wextra
LDFLAGS = -lreadline -lhistory
NAME = minishell
RM = rm -rf
SRC_DIR = src/

SRC = $(SRC_DIR)main.c				\
		$(SRC_DIR)utils.c 			\
		$(SRC_DIR)exec_command.c		\
		$(SRC_DIR)built_in.c		\
		$(SRC_DIR)built_in_env.c	\
		$(SRC_DIR)exec_built_in.c		\
		$(SRC_DIR)safe_funcs.c	 \
		$(SRC_DIR)signal_handle.c	\
		$(SRC_DIR)termios_off.c			\


OBJ = $(SRC:.c=.o)

all: $(NAME)

libft/libft.a:
	@echo "Compiling libft"
	@$(MAKE) -C libft
	@echo "Done"

$(NAME): $(OBJ) libft/libft.a
	@echo "Compiling $(NAME)"
	@$(CC) $(FLAGS) $(OBJ) libft/libft.a -o $(NAME) $(LDFLAGS)
	@echo "Done"

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo "Cleaning objects"
	@$(RM) $(OBJ)
	@$(MAKE) clean -C libft
	@echo "Done"

fclean:
	@echo "Cleaning executables"
	@$(RM) $(OBJ) $(NAME)
	@$(MAKE) fclean -C libft
	@echo "Done"

re: fclean all

.PHONY: all clean fclean re
