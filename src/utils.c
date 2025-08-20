/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 19:00:38 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 18:49:58 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_redirs(char **args, char **redir)
{
	int	i;
	int	type;

	i = -1;
	type = -1;
	redir[0] = NULL;
	redir[1] = NULL;
	redir[2] = NULL;
	while (args[++i])
	{
		if (!ft_strcmp(args[i], "<"))
			type = 0;
		else if (!ft_strcmp(args[i], ">"))
			type = 1;
		else if (!ft_strcmp(args[i], "2>"))
			type = 2;
		if (type != -1 && args[i + 1])
		{
			redir[type] = args[i + 1];
			args[i] = NULL;
			args[i + 1] = NULL;
			i++;
		}
	}
}

static void	_tokenize(char **ptr, char *delim, char **tokens, int *i)
{
	char	quote;

	i = 0;
	if (**ptr == '\'' || **ptr == '\"')
		quote = *(*ptr)++;
	tokens[(*i)++] = *ptr;
	while (**ptr)
	{
		if (quote)
		{
			if (**ptr == quote)
				break ;
		}
		else
		{
			if (ft_strchr(delim, **ptr))
				break ;
		}
		(*ptr)++;
	}
	if (**ptr)
		*(*ptr)++ = '\0';
}

int	tokenize(char *linea, char *delim, char **tokens, int max_tokens)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = linea;
	while (*ptr && i < max_tokens - 1)
	{
		while (*ptr && ft_strchr(delim, *ptr))
			ptr++;
		if (!*ptr)
			break ;
		_tokenize(&ptr, delim, tokens, &i);
	}
	tokens[i] = NULL;
	return (i);
}

void	print_all(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		printf("Arg %d: %s\n", i, args[i]);
		i++;
	}
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
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
