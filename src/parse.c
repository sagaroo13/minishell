/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dediaz-f <dediaz-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:14:19 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/15 17:21:31 by dediaz-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		handler->cmd->cmd_line->err_msg = "error: missing closing single quote";
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
		handler->cmd->cmd_line->err_msg = "error: missing closing double quote";
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

void	parse_line(t_command_line *cmd_line, char *line)
{
	cmd_line->line = ft_strdup(line);
	cmd_line->execute = true;
	get_cmds_info(cmd_line, line);
	// if (cmd_line->execute)
	// 	print_info(cmd_line);
	// free_cmd_line(cmd_line);
}
