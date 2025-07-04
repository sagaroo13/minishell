#include "../include/minishell.h"

void    free_handler(t_lexer_handler *handler)
{
	int i;

	i = -1;
    if (handler->tokens)
	{
		while (++i < handler->n_tokens)
		{
			if (handler->tokens[i].token_str)
				free(handler->tokens[i].token_str);
		}
		free(handler->tokens);
	}
    if (handler->buffer)
        free(handler->buffer);
}

int	count_tokens(const char *s)
{
	int	i;
	int	count;
	bool	in_sq;
	bool	in_dq;
	bool	in_token;

	i = -1;
	count = 0;
	in_sq = false;
	in_dq = false;
	in_token = false;
	while (s[++i])
	{
		if (s[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (s[i] == '\"' && !in_sq)
			in_dq = !in_dq;
		if (ft_isspace(s[i]) && !in_sq && !in_dq && in_token)
			in_token = false, count++;
		else if (!ft_isspace(s[i]) && !in_token)
			in_token = true;
	}
	if (in_token)
		count++;
	return (count);
}

void	init_handler(t_lexer_handler *handler, char *cmd_str)
{
	handler->buffer_size = BUFFER_SIZE + 1;
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->cmd_str = cmd_str;
	handler->in_sq = false;
	handler->in_dq = false;
	handler->argc = 0;
	handler->buf_len = 0;
	handler->n_tokens = (count_tokens(cmd_str) + 1);
	handler->tokens = safe_malloc(sizeof(t_token) * (handler->n_tokens), true);
}

static void	push_buffer(t_lexer_handler *handler, bool quoted)
{
    handler->cmd_str++;
	if (handler->buf_len == 0)
		return ;
	handler->buffer[handler->buf_len] = '\0';
    if (quoted)
        handler->tokens[handler->argc].quoted = true;
    else
        handler->tokens[handler->argc].quoted = false;
	handler->tokens[handler->argc++].token_str = ft_strdup(handler->buffer);
	free(handler->buffer);
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->buf_len = 0;
}

void	handle_var(t_lexer_handler *handler, char **s)
{
	int		i;
	char	var[64];
	char	*val;

	i = 0;
	(*s)++;
	while (**s && (ft_isalnum(**s) || **s == '_') && i < handler->buffer_size)
		var[i++] = *(*s)++;
	var[i] = '\0';
	if ((val = getenv(var)))
	{
		while (*val)
		{
			if (handler->buf_len < handler->buffer_size)
				handler->buffer[handler->buf_len++] = *val++;
		}
	}
}

static void	handle_sq(t_lexer_handler *handler, char **s)
{
    (*s)++;
    while (**s && **s != '\'')
    {
        if (handler->buf_len < handler->buffer_size)
            handler->buffer[handler->buf_len++] = **s;
        (*s)++;
    }
    if (**s != '\'')
    {
        perror("Syntax error: missing closing single quote");
        exit(EXIT_FAILURE);
    }
    push_buffer(handler, true);
}

static void	handle_dq(t_lexer_handler *handler, char **s)
{
	(*s)++;
    while (**s && **s != '\"')
    {
        if (**s == '$' && ft_isalpha((*s)[1]))
            handle_var(handler, s);
        else
        {
            if (handler->buf_len < handler->buffer_size)
                handler->buffer[handler->buf_len++] = **s;
            (*s)++;
        }
    }
    if (**s != '\"')
    {
        perror("Syntax error: missing closing double quote");
        exit(EXIT_FAILURE);
    }
    push_buffer(handler, true);
}

static void    handle_nq(t_lexer_handler *handler, char **s)
{
    if (**s == '$' && ft_isalpha((*s)[1]))
        handle_var(handler, s);
    else
        handler->buffer[handler->buf_len++] = *(*s)++;
}

void	lexer(t_lexer_handler *handler, char *cmd_str)
{
	init_handler(handler, cmd_str);
	while (*handler->cmd_str)
	{
		if (ft_isspace(*handler->cmd_str))
			push_buffer(handler, false);
		else if (*handler->cmd_str == '\'')
			handle_sq(handler, &handler->cmd_str);
		else if (*handler->cmd_str == '\"')
			handle_dq(handler, &handler->cmd_str);
        else
            handle_nq(handler, &handler->cmd_str);
	}
	push_buffer(handler, false);
	handler->tokens[handler->argc].token_str = NULL;
}

void print_tokens(t_lexer_handler *handler)
{
	for (int i = 0; i < handler->argc; i++)
	{
		printf("Token %d: \"%s\" (quoted: %s)\n", i,
			handler->tokens[i].token_str,
			handler->tokens[i].quoted ? "yes" : "no");
	}
}

int main(void)
{
	t_lexer_handler handler;
	char *line = "echo 'Hello World' \"$ Path: '$HOME'\" $";

	printf("Input line: %s\n\n", line);
	lexer(&handler, line);
	print_tokens(&handler);
	free_handler(&handler);
	return 0;
}
