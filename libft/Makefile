# VARIABLES
NAME = libft.a
CC = gcc
CFLAGS = -Wall -Werror -Wextra
AR = ar rcs
RM = rm -rf

SRC = ft_isalpha.c	\
	ft_isdigit.c	\
	ft_isalnum.c	\
	ft_isascii.c	\
	ft_isprint.c	\
	ft_strlen.c		\
	ft_memset.c		\
	ft_memcpy.c		\
	ft_memmove.c	\
	ft_bzero.c		\
	ft_strlcpy.c	\
	ft_strlcat.c	\
	ft_toupper.c	\
	ft_tolower.c	\
	ft_strchr.c		\
	ft_strrchr.c	\
	ft_strncmp.c	\
	ft_memchr.c		\
	ft_memcmp.c		\
	ft_strnstr.c	\
	ft_atoi.c		\
	ft_calloc.c		\
	ft_strdup.c		\
	ft_substr.c		\
	ft_strjoin.c	\
	ft_strtrim.c	\
	ft_split.c		\
	ft_itoa.c		\
	ft_strmapi.c	\
	ft_striteri.c	\
	ft_putchar_fd.c	\
	ft_putnbr_fd.c	\
	ft_putstr_fd.c	\
	ft_putendl_fd.c

# REGLA PARA COMPILAR OBJETOS
OBJ = $(SRC:.c=.o)

all: $(NAME) # NOMBRE DE LA REGLA (SI NO HAY BUSCA COMO HACERLA)
# REGLA PARA COMPILAR BIBLIOTECA
$(NAME): $(OBJ)
	@$(AR) $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean: 
	@$(RM) $(OBJ)

fclean: 
	@$(RM) $(OBJ) $(NAME)

re: fclean all