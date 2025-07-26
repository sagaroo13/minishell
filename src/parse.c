/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:23:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/07/08 20:51:59 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	search_file(char **files, char *str, int len)
{
	int	i;

	if (!files || !*files)
		return (false);
	i = -1;
	while (++i < len)
	{
		if (!ft_strcmp(files[i], str))
			return (true);
	}
	return (false);
}

bool	is_file(t_command *cmd, char *str)
{
	if (!str || !*str)
		return (false);
	if (search_file(cmd->stdin.redirs, str, cmd->stdin.n_redirs)
		|| search_file(cmd->stdout.redirs, str, cmd->stdout.n_redirs)
		|| search_file(cmd->stderr.redirs, str, cmd->stderr.n_redirs)
		|| search_file(cmd->append.redirs, str, cmd->append.n_redirs)
		|| search_file(cmd->heredoc.redirs, str, cmd->heredoc.n_redirs))
		return (true);
	return (false);
}

bool	is_meta(char *str)
{
	if (!str || !*str)
		return (true);
	if (ft_strstr(str, ">") || ft_strstr(str, "<") || ft_strstr(str, ">>")
		|| ft_strstr(str, "<<") || ft_strstr(str, "2>"))
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
		if ((ft_strchr_charset(handler.tokens[i].token_str, "<>")
				&& !handler.tokens[i].quoted) || is_file(cmd,
				handler.tokens[i].token_str))
			continue ;
		count++;
	}
	return (count);
}

void	get_arguments(t_command *cmd, t_lexer_handler handler)
{
	int	n_args;
	int	i;
	int	j;

	n_args = count_argv(cmd, handler);
	cmd->args = safe_malloc(sizeof(char *) * (n_args + 1), true);
	i = -1;
	j = -1;
	while (++i < handler.n_tokens - 1)
	{
		if ((ft_strchr_charset(handler.tokens[i].token_str, "<>")
				&& !handler.tokens[i].quoted) || is_file(cmd,
				handler.tokens[i].token_str))
			continue ;
		cmd->args[++j] = ft_strdup(handler.tokens[i].token_str);
	}
	cmd->args[++j] = NULL;
}

char	*find_redir(t_lexer_handler handler, int index, int n)
{
	char	*file;

	file = NULL;
	if ((int)ft_strlen(handler.tokens[index].token_str) > n)
		file = ft_strdup(handler.tokens[index].token_str + n);
	else if (handler.tokens[index + 1].quoted
		|| !is_meta(handler.tokens[index + 1].token_str))
		file = ft_strdup(handler.tokens[index + 1].token_str);
	else
	{
		handler.cmd->cmd_line->err_msg = "syntax error: redirection file not specified";
		handler.cmd->cmd_line->execute = false;
	}
	return (file);
}

char	**get_redirection(t_lexer_handler handler, char *redirection, int len, int n)
{
	char	**files;
	int		i;
	int		j;

	if (!handler.tokens || !len)
		return (NULL);
	files = safe_malloc(sizeof(char *) * (len + 1), true);
	i = 0;
	j = 0;
	while (i < handler.n_tokens - 1)
	{
		if (!ft_strncmp(handler.tokens[i].token_str, redirection, n)
			&& !handler.tokens[i].quoted
			&& !ft_strchr_charset((handler.tokens[i].token_str + n), "<>"))
			files[j++] = find_redir(handler, i, n);
		i++;
	}
	files[j] = NULL;
	return (files);
}

void	get_redirections(t_command *cmd, t_lexer_handler handler, char *cmd_str)
{
	cmd->stdin.n_redirs = ft_count_substr(cmd_str, "<");
	cmd->stdout.n_redirs = ft_count_substr(cmd_str, ">");
	cmd->stderr.n_redirs = ft_count_substr(cmd_str, "2>");
	cmd->append.n_redirs = ft_count_substr(cmd_str, ">>");
	cmd->heredoc.n_redirs = ft_count_substr(cmd_str, "<<");
	cmd->stdin.redirs = get_redirection(handler, "<", cmd->stdin.n_redirs, 1);
	cmd->stdout.redirs = get_redirection(handler, ">", cmd->stdout.n_redirs, 1);
	cmd->stderr.redirs = get_redirection(handler, "2>", cmd->stderr.n_redirs, 2);
	cmd->append.redirs = get_redirection(handler, ">>", cmd->append.n_redirs, 2);
	cmd->heredoc.redirs = get_redirection(handler, "<<", cmd->heredoc.n_redirs, 2);
}

void	free_handler(t_lexer_handler *handler)
{
	int	i;

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
	int		i;
	int		count;
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

void	init_handler(t_lexer_handler *handler, t_command *cmd, char *cmd_str)
{
	handler->buffer_size = BUFF_SIZE + 1;
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->cmd_str = cmd_str;
	handler->argc = 0;
	handler->buf_len = 0;
	handler->n_tokens = (count_tokens(cmd_str) + 1);
	handler->cmd = cmd;
	handler->tokens = safe_malloc(sizeof(t_token) * (handler->n_tokens), true);
}

static void	push_buffer(t_lexer_handler *handler, bool quoted)
{
	handler->cmd_str++;
	if (handler->buf_len == 0)
		return ;
	handler->tokens[handler->argc].quoted = false;
	if (quoted)
		handler->tokens[handler->argc].quoted = true;
	handler->buffer[handler->buf_len] = '\0';
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
	val = getenv(var);
	if (val)
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
			handler->buffer[handler->buf_len++] = *(*s)++;
	}
	if (**s != '\'')
	{
		handler->cmd->cmd_line->err_msg = "syntax error: missing closing single quote";
		handler->cmd->cmd_line->execute = false;
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
				handler->buffer[handler->buf_len++] = *(*s)++;
		}
	}
	if (**s != '\"')
	{
		handler->cmd->cmd_line->err_msg = "syntax error: missing closing double quote";
		handler->cmd->cmd_line->execute = false;
	}
	push_buffer(handler, true);
}

static void	handle_nq(t_lexer_handler *handler, char **s)
{
	if (**s == '$' && ft_isalpha((*s)[1]))
		handle_var(handler, s);
	else
		handler->buffer[handler->buf_len++] = *(*s)++;
}

void	lexer(t_lexer_handler *handler, t_command *cmd, char *cmd_str)
{
	init_handler(handler, cmd, cmd_str);
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

void	get_cmd_info(t_command_line *cmd_line, t_command *cmd, char *cmd_str)
{
	t_lexer_handler	handler;

	cmd->cmd_str = ft_strdup(cmd_str);
	cmd->cmd_line = cmd_line;
	lexer(&handler, cmd, cmd_str);
	get_redirections(cmd, handler, cmd_str);
	get_arguments(cmd, handler);
	if (is_builtin(cmd->args[0]))
		cmd->builtin = true;
	else
		cmd->builtin = false;
	free_handler(&handler);
}

int	count_cmds(char *line)
{
	bool	in_sq;
	bool	in_dq;
    int		i;
    int		count;
    
    in_sq = false;
	in_dq = false;
	i = -1;
	count = 1;
    while (line[++i])
    {
        if (line[i] == '\'' && !in_dq)
			in_sq = !in_sq;
        else if (line[i] == '\"' && !in_sq)
			in_dq = !in_dq;
        else if (line[i] == '|' && !in_sq && !in_dq)
            count++;
    }
	return (count);
}

void	update_mask(char c, int *f)
{
	if (c == '\'' && !(*f & 2))
		*f ^= 1;
	else if (c == '\"'&& !(*f & 1))
		*f ^= 2;
}

char	**split_pipes(char *line, int n_cmds)
{
	char	**line_parts;
	int		i;
	int		j;
	int		start;
	int		f;

	line_parts = safe_malloc(sizeof(char *) * (n_cmds + 1), true);
	i = -1;
	j = 0;
	start = 0;
	f = 0;
	while (line[++i])
    {
        if (line[i] == '\'' || line[i] == '\"')
			update_mask(line[i], &f);
        else if (line[i] == '|' && !f)
        {
            line_parts[j++] = ft_substr(line, start, i - start);
            start = i + 1;
        }
    }
	line_parts[j++] = ft_substr(line, start, i - start);
	line_parts[j] = NULL;
	return (line_parts);
}

void	get_cmds_info(t_command_line *cmd_line, char *line)
{
	t_command	*cmds;
	char	**line_parts;
	int		i;

	cmd_line->n_cmds = count_cmds(line);
	cmds = safe_malloc(sizeof(t_command) * (cmd_line->n_cmds), true);
	cmd_line->cmds = cmds;
	line_parts = split_pipes(line, cmd_line->n_cmds);
	i = -1;
	while (line_parts[++i])
		get_cmd_info(cmd_line, &cmd_line->cmds[i], line_parts[i]);
	if (ft_empty_str(line_parts[cmd_line->n_cmds - 1]))
	{
		cmd_line->err_msg = "syntax error: pipeline not closed";
		cmd_line->execute = false;
	}
	ft_free_matrix(line_parts);
}

void	print_info(t_command_line *cmd_line)
{
	int	i;

	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		printf(BLUE "Command %d: %s\n" RESET, i + 1, cmd_line->cmds[i].cmd_str);
		print_all(cmd_line->cmds[i].args);
		if (cmd_line->cmds[i].stdin.redirs)
		{
			printf(GREEN "Stdin: \n" RESET);
			print_all(cmd_line->cmds[i].stdin.redirs);
		}
		if (cmd_line->cmds[i].stdout.redirs)
		{
			printf(GREEN "Stdout:\n" RESET);
			print_all(cmd_line->cmds[i].stdout.redirs);
		}
		if (cmd_line->cmds[i].stderr.redirs)
		{
			printf(GREEN "Stderr:\n" RESET);
			print_all(cmd_line->cmds[i].stderr.redirs);
		}
		if (cmd_line->cmds[i].append.redirs)
		{
			printf(GREEN "Append:\n" RESET);
			print_all(cmd_line->cmds[i].append.redirs);
		}
		if (cmd_line->cmds[i].heredoc.redirs)
		{
			printf(GREEN "Heredoc:\n" RESET);
			print_all(cmd_line->cmds[i].heredoc.redirs);
		}
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
		ft_free_matrix(cmd_line->cmds[i].stdin.redirs);
		ft_free_matrix(cmd_line->cmds[i].stdout.redirs);
		ft_free_matrix(cmd_line->cmds[i].stderr.redirs);
		ft_free_matrix(cmd_line->cmds[i].append.redirs);
		ft_free_matrix(cmd_line->cmds[i].heredoc.redirs);
		free(cmd_line->cmds[i].cmd_str);
	}
	free(cmd_line->line);
	free(cmd_line->cmds);
	cmd_line->n_cmds = 0;
	cmd_line->line = NULL;
	cmd_line->cmds = NULL;
}

void	parse_line(t_command_line *cmd_line, char *line)
{
	cmd_line->line = ft_strdup(line);
	cmd_line->execute = true;
	get_cmds_info(cmd_line, line);
	// if (cmd_line->execute)
	// 	print_info(cmd_line);
	// free_cmd_line(cmd_line);
}
