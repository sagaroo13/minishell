#include "../include/minishell.h"

void process_redirs(char **args, char **redir)
{
    int i;

    i = -1;
    redir[0] = NULL;
    redir[1] = NULL;
    redir[2] = NULL;
    while (args[++i])
    {
        if (!strcmp(args[i], "<"))
        {
            redir[0] = args[i+1];
            args[i] = NULL;
            args[i + 1] = NULL;
            i++; // Añadiendo i++ en cada uno de los if's, conseguimos que se puedan detectar más de una redirecciones para el mismo comando
        }
        else if (!strcmp(args[i], ">"))
        {
            redir[1] = args[i+1];
            args[i] = NULL;
            args[i + 1] = NULL;
            i++;
        }
        else if (!strcmp(args[i], "2>"))
        {
            redir[2] = args[i+1];
            args[i] = NULL; 
            args[i + 1] = NULL;
            i++;
        }
    }
}

static void _tokenize(char **ptr, char *delim, char **tokens, int *i)
{
    char quote;

    if (**ptr == '\"' || **ptr == '\'')
    {
        quote = **ptr;
        (*ptr)++; // Saltamos la comilla
        tokens[(*i)++] = *ptr;  // El token comienza después de la comilla
        while (**ptr && **ptr != quote) // Buscamos la comilla de cierre correspondiente
            (*ptr)++;
        if (**ptr == quote)
        {
            **ptr = '\0';  // Finalizamos el token y avanzamos con el siguiente
            (*ptr)++;
        }
    }
    else
    {
        tokens[(*i)++] = *ptr; // Mismo proceso para argumentos sin comillas
        while (**ptr && !ft_strchr(delim, **ptr))
            (*ptr)++;
        if (**ptr)
        {
            **ptr = '\0';
            (*ptr)++;
        }
    }
}

int tokenize(char *linea, char *delim, char **tokens, int max_tokens)
{
    int i;
    char *ptr;

    i = 0;
    ptr = linea;
    while (*ptr && i < max_tokens - 1) // Recorremos la línea hasta que es distinto de \0 o hasta que hallamos obtenido (max_tokens) -1 strings
    {
        while (*ptr && ft_strchr(delim, *ptr)) // Saltamos delimitadores al inicio
            ptr++;
        if (!*ptr)
            break;
        _tokenize(&ptr, delim, tokens, &i);
    }
    tokens[i] = NULL;
    return (i);
}

void banner(void)
{
    printf(GREEN""BOLD
    "      ___                       ___                       ___           ___           ___           ___       ___\n"
    "     /\\__\\          ___        /\\__\\          ___        /\\  \\         /\\__\\         /\\  \\         /\\__\\     /\\__\\\n"
    "    /::|  |        /\\  \\      /::|  |        /\\  \\      /::\\  \\       /:/  /        /::\\  \\       /:/  /    /:/  /\n"
    "   /:|:|  |        \\:\\  \\    /:|:|  |        \\:\\  \\    /:/\\ \\  \\     /:/__/        /:/\\:\\  \\     /:/  /    /:/  / \n"
    "  /:/|:|__|__      /::\\__\\  /:/|:|  |__      /::\\__\\  _\\:\\~\\ \\  \\   /::\\  \\ ___   /::\\~\\:\\  \\   /:/  /    /:/  /  \n"
    " /:/ |::::\\__\\  __/:/\\/__/ /:/ |:| /\\__\\  __/:/\\/__/ /\\ \\:\\ \\ \\__\\ /:/\\:\\  /\\__\\ /:/\\:\\ \\:\\__\\ /:/__/    /:/__/   \n"
    " \\/__/~~/:/  / /\\/:/  /    \\/__|:|/:/  / /\\/:/  /    \\:\\ \\:\\ \\/__/ \\/__\\:\\/:/  / \\:\\~\\:\\ \\/__/ \\:\\  \\    \\:\\  \\   \n"
    "       /:/  /  \\::/__/         |:/:/  /  \\::/__/      \\:\\ \\:\\__\\        \\::/  /   \\:\\ \\:\\__\\    \\:\\  \\    \\:\\  \\  \n"
    "      /:/  /    \\:\\__\\         |::/  /    \\:\\__\\       \\:\\/:/  /        /:/  /     \\:\\ \\/__/     \\:\\  \\    \\:\\  \\ \n"
    "     /:/  /      \\/__/         /:/  /      \\/__/        \\::/  /        /:/  /       \\:\\__\\        \\:\\__\\    \\:\\__\\\n"
    "     \\/__/                     \\/__/                     \\/__/         \\/__/         \\/__/         \\/__/     \\/__/\n\n"
    RESET);
}

// int main(void)
// {
//     char line[] = "echo \"Hello, World!\" '42 Madrid' minishell";
//     char delim[] = " \t";
//     char *tokens[MAX_TOKENS]; // Array para almacenar los tokens
//     int num_tokens;
//     int i;

//     printf("Input line: %s\n", line);

//     // Llamamos a la función tokenize
//     num_tokens = tokenize(line, delim, tokens, MAX_TOKENS);

//     // Mostramos los tokens obtenidos
//     printf("Number of tokens: %d\n", num_tokens);
//     for (i = 0; i < num_tokens; i++)
//     {
//         printf("Token %d: %s\n", i + 1, tokens[i]);
//     }

//     return 0;
// }