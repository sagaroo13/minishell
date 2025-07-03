/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:23:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/07/01 14:54:32 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	search_unmatched_quotes(t_command_line *cmd_line, char *str)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			single_quote++;
		else if (str[i] == '\"')
			double_quote++;
		i++;
	}
	if (single_quote % 2 || double_quote % 2)
	{
		perror("Syntax error: Unmatched quotes");
		cmd_line->execute = false;
	}
}

int	is_file(t_command *cmd, char *str)
{
	if (!str || !*str)
		return (false);
	if (!ft_strcmp(str, cmd->stdin_file) || !ft_strcmp(str, cmd->stdout_file)
		|| !ft_strcmp(str, cmd->stderr_file) || !ft_strcmp(str,
			cmd->append_file) || !ft_strcmp(str, cmd->heredoc_delim))
		return (true);
	return (false);
}

int	is_meta(char *str)
{
	if (!str || !*str)
		return (true);
	if (ft_strstr(str, ">") || ft_strstr(str, "<") || ft_strstr(str, ">>")
		|| ft_strstr(str, "<<") || ft_strstr(str, "2>") || ft_strstr(str, "$"))
		return (true);
	return (false);
}

int	count_argv(t_command *cmd, t_lexer_handler handler)
{
	int	count;
	int	i;

	count = 0;
	i = -1;
	while (++i < handler.n_tokens - 1)
	{
		if (!handler.tokens[i].quoted && (ft_strchr_charset(handler.tokens[i].token_str, "<>") || is_file(cmd, handler.tokens[i].token_str)))
			continue ;
		count++;
	}
	return (count);
}

void	get_arguments(t_command *cmd, t_lexer_handler handler)
{
	int		n_args;
	int		i;
	int		j;

	n_args = count_argv(cmd, handler);
	cmd->args = safe_malloc(sizeof(char *) * (n_args + 1), true);
	i = -1;
	j = -1;
	while (++i < handler.n_tokens - 1)
	{
		if (!handler.tokens[i].quoted && (ft_strchr_charset(handler.tokens[i].token_str, "<>") || is_file(cmd, handler.tokens[i].token_str)))
			continue ;
		cmd->args[++j] = ft_strdup(handler.tokens[i].token_str);
	}
	cmd->args[++j] = NULL;
}

int	find_redir(t_lexer_handler handler, const char *str, int n)
{
	int i;
	int last;

	if (!handler.tokens || !str)
		return (-1);
	i = -1;
	last = -1;
	while (++i < handler.n_tokens - 1)
	{
		if (!ft_strncmp(handler.tokens[i].token_str, str, n) && !handler.tokens[i].quoted)
			last = i;
	}
	return (last);
}

char	*get_redirection(t_lexer_handler handler, char *redirection, int n)
{
	char	*file_name;
	int		index;

	file_name = NULL;
	index = find_redir(handler, redirection, n);
	if (index == -1 || ft_strchr_charset((handler.tokens[index].token_str + n), "<>"))
		return (NULL);
	if ((int)ft_strlen(handler.tokens[index].token_str) > n)
		file_name = ft_strdup(handler.tokens[index].token_str + n);
	else if (!is_meta(handler.tokens[index + 1].token_str))
		file_name = ft_strdup(handler.tokens[index + 1].token_str);
	else
		perror("Syntax error: No se especificó un archivo para la redirección");
	return (file_name);
}

void	get_redirections(t_command *cmd, t_lexer_handler handler)
{
	cmd->stdin_file = get_redirection(handler, "<", 1);
	cmd->stdout_file = get_redirection(handler, ">", 1);
	cmd->stderr_file = get_redirection(handler, "2>", 2);
	cmd->append_file = get_redirection(handler, ">>", 2);
	cmd->heredoc_delim = get_redirection(handler, "<<", 2);
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
	int i;

	handler->buffer_size = ft_strlen(cmd_str) + 1;
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->cmd_str = cmd_str;
	handler->in_sq = false;
	handler->in_dq = false;
	handler->argc = 0;
	handler->buf_len = 0;
	handler->n_tokens = (count_tokens(cmd_str) + 1);
	handler->tokens = safe_malloc(sizeof(t_token) * (handler->n_tokens), true);
	i = 0;
	while (i < handler->n_tokens)
		handler->tokens[i++].quoted = false;
}

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

static void	push_buffer(t_lexer_handler *handler)
{
	if (handler->buf_len == 0)
		return ;
	handler->buffer[handler->buf_len] = '\0';
	handler->tokens[handler->argc++].token_str = ft_strdup(handler->buffer);
	free(handler->buffer);
	handler->buffer = safe_malloc(sizeof(char) * (ft_strlen(handler->cmd_str) + 1), true);
	handler->buf_len = 0;
}

static void	handle_sq(t_lexer_handler *handler, char c)
{
	if (!handler->in_dq && c == '\'')
		handler->in_sq = !handler->in_sq;
	else
		handler->buffer[handler->buf_len++] = c;
}

void	handle_var(t_lexer_handler *handler, char **s)
{
	int		i;
	char	var[handler->buffer_size];
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
	if (!handler->in_sq && **s == '\"')
		handler->in_dq = !handler->in_dq;
}

static void	handle_dq(t_lexer_handler *handler, char **s)
{
	if (!handler->in_sq && **s == '\"')
		handler->in_dq = !handler->in_dq;
	else if (handler->in_dq && **s == '$' && ft_isalpha((*s)[1]))
		handle_var(handler, s);
	else
		handler->buffer[handler->buf_len++] = **s;
}

void	lexer(t_lexer_handler *handler, char *cmd_str)
{
	init_handler(handler, cmd_str);
	while (*handler->cmd_str)
	{
		if (handler->in_sq || handler->in_dq)
			handler->tokens[handler->argc].quoted = true;
		if (!handler->in_sq && !handler->in_dq && *handler->cmd_str == ' ')
			push_buffer(handler);
		else if (*handler->cmd_str == '\'' && handler->in_sq)
			handle_sq(handler, *handler->cmd_str);
		else
			handle_dq(handler, &handler->cmd_str);
		handler->cmd_str++;
	}
	push_buffer(handler);
	handler->tokens[handler->argc].token_str = NULL;
}

void	get_cmd_info(t_command *cmd, char *cmd_str)
{
	t_lexer_handler	handler;

	lexer(&handler, cmd_str);
	// print_all(handler);
	get_redirections(cmd, handler);
	get_arguments(cmd, handler);
	if (is_builtin(cmd->args[0]))
		cmd->builtin = true;
	else
		cmd->builtin = false;
	free_handler(&handler);
}

void	get_cmds_info(t_command_line *cmd_line, char *line)
{
	char	**line_parts;
	int		i;

	search_unmatched_quotes(cmd_line, line);
	if (!cmd_line->execute)
		return ;
	line_parts = ft_split(line, '|');
	i = -1;
	while (line_parts[++i])
		get_cmd_info(&cmd_line->cmds[i], line_parts[i]);
	// Set to false after parsing to avoid printing info
	ft_free_matrix(line_parts);
}

void	print_info(t_command_line *cmd_line)
{
	int	i;

	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		printf(BLUE "Command %d:\n" RESET, i + 1);
		print_all(cmd_line->cmds[i].args);
		if (cmd_line->cmds[i].stdin_file)
			printf("Stdin: %s\n", cmd_line->cmds[i].stdin_file);
		if (cmd_line->cmds[i].stdout_file)
			printf("Stdout: %s\n", cmd_line->cmds[i].stdout_file);
		if (cmd_line->cmds[i].stderr_file)
			printf("Stderr: %s\n", cmd_line->cmds[i].stderr_file);
		if (cmd_line->cmds[i].append_file)
			printf("Append: %s\n", cmd_line->cmds[i].append_file);
		if (cmd_line->cmds[i].heredoc_delim)
			printf("Heredoc: %s\n", cmd_line->cmds[i].heredoc_delim);
		if (cmd_line->cmds[i].builtin)
			printf("Builtin: %s\n", cmd_line->cmds[i].args[0]);
		else
			printf("Not a builtin command: %s\n", cmd_line->cmds[i].args[0]);
		printf("--------------------------------------------------\n");
	}
}

void	free_cmd_line(t_command_line *cmd_line)
{
	int	i;

	if (!cmd_line || !cmd_line->cmds)
		return ;
	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		ft_free_matrix(cmd_line->cmds[i].args);
		free(cmd_line->cmds[i].stdin_file);
		free(cmd_line->cmds[i].stdout_file);
		free(cmd_line->cmds[i].stderr_file);
		free(cmd_line->cmds[i].append_file);
		free(cmd_line->cmds[i].heredoc_delim);
	}
	free(cmd_line->line);
	free(cmd_line->cmds);
	cmd_line->n_cmds = 0;
	cmd_line->line = NULL;
	cmd_line->cmds = NULL;
}

void	parse_line(t_command_line *cmd_line, char *line)
{
	t_command *cmds;

	cmd_line->n_cmds = (ft_occurrence(line, '|') + 1);
	cmds = safe_malloc(sizeof(t_command) * (cmd_line->n_cmds), true);
	cmd_line->cmds = cmds;
	cmd_line->line = ft_strdup(line);
	cmd_line->execute = true;
	get_cmds_info(cmd_line, line);
	if (cmd_line->execute)
		print_info(cmd_line);
	free_cmd_line(cmd_line);
}