GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

CC = cc
FLAGS = -Werror -Wall -Wextra -fsanitize=address -g
LDFLAGS = -lreadline -lhistory
NAME = minishell
RM = rm -rf
SRC_DIR = src/
BUILTINS_DIR = builtins/
PARSER_DIR = parser/
LEXER_DIR = lexer/
HEREDOC_DIR = heredoc/
PIPES_DIR = pipes/
REDIRS_DIR = redirs/
MAIN_DIRS = main/
SAFE_FUNCS_DIR = safe_funcs/
SIGNALS_DIR = signals/

SRC =	$(SRC_DIR)$(MAIN_DIRS)main.c				\
    	$(SRC_DIR)$(MAIN_DIRS)utils.c 				\
    	$(SRC_DIR)$(MAIN_DIRS)exec.c			\
		$(SRC_DIR)$(MAIN_DIRS)exec_utils.c			\
		$(SRC_DIR)$(MAIN_DIRS)expand_utils.c		\
		$(SRC_DIR)$(MAIN_DIRS)exec_pipeline.c		\
		$(SRC_DIR)$(MAIN_DIRS)shell_loop.c			\
		$(SRC_DIR)$(MAIN_DIRS)utils_2.c				\
		$(SRC_DIR)$(MAIN_DIRS)expand.c				\
		$(SRC_DIR)$(BUILTINS_DIR)exec_built_in.c		\
    	$(SRC_DIR)$(BUILTINS_DIR)ft_env.c			\
    	$(SRC_DIR)$(BUILTINS_DIR)ft_exit.c			\
		$(SRC_DIR)$(BUILTINS_DIR)ft_cd.c				\
		$(SRC_DIR)$(BUILTINS_DIR)ft_echo.c			\
		$(SRC_DIR)$(BUILTINS_DIR)ft_pwd.c			\
		$(SRC_DIR)$(BUILTINS_DIR)ft_export.c			\
		$(SRC_DIR)$(BUILTINS_DIR)ft_export_utils.c	\
		$(SRC_DIR)$(BUILTINS_DIR)ft_export_display.c	\
		$(SRC_DIR)$(BUILTINS_DIR)ft_unset.c			\
		$(SRC_DIR)$(HEREDOC_DIR)heredoc.c				\
		$(SRC_DIR)$(HEREDOC_DIR)heredoc_utils.c		\
		$(SRC_DIR)$(HEREDOC_DIR)heredoc_utils_2.c		\
		$(SRC_DIR)$(HEREDOC_DIR)heredoc_utils_3.c		\
		$(SRC_DIR)$(LEXER_DIR)lexer.c		\
		$(SRC_DIR)$(LEXER_DIR)lexer_utils.c		\
		$(SRC_DIR)$(LEXER_DIR)handle_meta.c		\
		$(SRC_DIR)$(PARSER_DIR)parser.c				\
		$(SRC_DIR)$(PARSER_DIR)parser_utils.c		\
		$(SRC_DIR)$(PARSER_DIR)parser_utils_2.c		\
		$(SRC_DIR)$(PARSER_DIR)parser_split.c		\
		$(SRC_DIR)$(PIPES_DIR)pipes.c				\
		$(SRC_DIR)$(PIPES_DIR)pipes_utils.c				\
		$(SRC_DIR)$(REDIRS_DIR)redirs.c				\
		$(SRC_DIR)$(REDIRS_DIR)redirs_utils.c				\
		$(SRC_DIR)$(SAFE_FUNCS_DIR)safe_funcs.c			\
		$(SRC_DIR)$(SAFE_FUNCS_DIR)safe_funcs2.c 		\
		$(SRC_DIR)$(SIGNALS_DIR)signal_handle.c		\
    	$(SRC_DIR)$(SIGNALS_DIR)termios_off.c			\
		$(SRC_DIR)$(SIGNALS_DIR)terminal_config.c		\

OBJ = $(SRC:.c=.o)

all: $(NAME)

libft/libft.a:
	@echo "$(GREEN)Compiling libft...$(RESET)"
	@$(MAKE) -C libft
	@echo "$(GREEN)Libft compiled successfully!$(RESET)"

$(NAME): $(OBJ) libft/libft.a
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@$(CC) $(FLAGS) $(OBJ) libft/libft.a -o $(NAME) $(LDFLAGS)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning objects...$(RESET)"
	@$(RM) $(OBJ)
	@$(MAKE) clean -C libft
	@echo "$(RED)Objects cleaned!$(RESET)"

fclean: clean
	@echo "$(RED)Cleaning executables...$(RESET)"
	@$(RM) $(OBJ) $(NAME)
	@$(MAKE) fclean -C libft
	@echo "$(RED)Executables cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
