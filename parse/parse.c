/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:14:19 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 19:01:34 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	handle_sq(t_lexer *handler, char **s)
{
	(*s)++;
	while (**s && **s != '\'')
	{
		if (handler->buf_len < handler->buffer_size)
			handler->buffer[handler->buf_len++] = *(*s)++;
	}
	if (**s != '\'')
	{
		handler->cmd->cmd_line->err_msg = "error: missing closing single quote";
		handler->cmd->cmd_line->execute = false;
	}
	push_buffer(handler, true);
}

static void	handle_dq(t_lexer *handler, char **s, t_shell *shell)
{
	(*s)++;
	while (**s && **s != '\"')
	{
		if (**s == '$' && ft_isalpha((*s)[1]))
			handle_var(handler, s, shell);
		else
		{
			if (handler->buf_len < handler->buffer_size)
				handler->buffer[handler->buf_len++] = *(*s)++;
		}
	}
	if (**s != '\"')
	{
		handler->cmd->cmd_line->err_msg = "error: missing closing double quote";
		handler->cmd->cmd_line->execute = false;
	}
	push_buffer(handler, true);
}

static void	handle_nq(t_lexer *handler, char **s, t_shell *shell)
{
	if (**s == '$' && ft_isalpha((*s)[1]))
		handle_var(handler, s, shell);
	else
		handler->buffer[handler->buf_len++] = *(*s)++;
}

void	lexer(t_lexer *handler, t_command *cmd, char *cmd_str, t_shell *shell)
{
	init_handler(handler, cmd, cmd_str);
	while (*handler->cmd_str)
	{
		if (ft_isspace(*handler->cmd_str))
			push_buffer(handler, false);
		else if (*handler->cmd_str == '\'')
			handle_sq(handler, &handler->cmd_str);
		else if (*handler->cmd_str == '\"')
			handle_dq(handler, &handler->cmd_str, shell);
		else
			handle_nq(handler, &handler->cmd_str, shell);
	}
	push_buffer(handler, false);
	handler->tokens[handler->argc].token_str = NULL;
}

void	parse_line(t_command_line *cmd_line, t_shell *shell, char *line)
{
	(void)shell;
	cmd_line->line = ft_strdup(line);
	cmd_line->execute = true;
	get_cmds_info(cmd_line, shell, line);
}
