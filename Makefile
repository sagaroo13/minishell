# Colores
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

CC = cc
FLAGS = -Werror -Wall -Wextra
LDFLAGS = -lreadline -lhistory
NAME = minishell
RM = rm -rf
SRC_DIR = src/

SRC = $(SRC_DIR)main.c				\
		$(SRC_DIR)utils.c 			\
		$(SRC_DIR)error.c			\
		$(SRC_DIR)exec_line.c		\
		$(SRC_DIR)pipes_and_redirs.c\
		$(SRC_DIR)built_in.c		\
		$(SRC_DIR)built_in_env.c	\
		$(SRC_DIR)exec_built_in.c	\
		$(SRC_DIR)safe_funcs.c		\
		$(SRC_DIR)signal_handle.c	\
		$(SRC_DIR)termios_off.c		\

OBJ = $(SRC:.c=.o)

all: $(NAME)

libft/libft.a:
	@echo -e "$(GREEN)Compiling libft...$(RESET)"
	@$(MAKE) -C libft > /dev/null
	@echo -e "$(GREEN)Libft compiled successfully!$(RESET)"

$(NAME): $(OBJ) libft/libft.a
	@echo -e "$(GREEN)Compiling $(NAME)...$(RESET)"
	@$(CC) $(FLAGS) $(OBJ) libft/libft.a -o $(NAME) $(LDFLAGS)
	@echo -e "$(GREEN)$(NAME) compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@ > /dev/null

clean:
	@echo -e "$(RED)Cleaning objects...$(RESET)"
	@$(RM) $(OBJ)
	@$(MAKE) clean -C libft > /dev/null
	@echo -e "$(RED)Objects cleaned!$(RESET)"

fclean:
	@echo -e "$(RED)Cleaning executables and objects...$(RESET)"
	@$(RM) $(OBJ) $(NAME)
	@$(MAKE) fclean -C libft > /dev/null
	@echo -e "$(RED)Executables and objects cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
