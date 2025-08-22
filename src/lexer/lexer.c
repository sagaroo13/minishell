/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:44:18 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/22 20:24:45 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	push_buffer(t_lexer *handler, bool quoted)
{
	if (handler->buf_len == 0)
		return ;
	handler->buffer[handler->buf_len] = '\0';
	add_token(handler, handler->buffer, quoted);
	ft_memset(handler->buffer, 0, handler->buffer_size);
	handler->buf_len = 0;
}

static void	handle_sq(t_lexer *handler, char **s)
{
	handler->quoted = true;
	(*s)++;
	while (**s && **s != '\'')
		handler->buffer[handler->buf_len++] = *(*s)++;
	if (**s != '\'')
	{
		handler->cmd->cmd_line->err_msg = "error: missing closing single quote";
		handler->cmd->cmd_line->execute = false;
	}
	else
		(*s)++;
}

static void	handle_dq(t_lexer *handler, char **s, t_shell *shell)
{
	handler->quoted = true;
	(*s)++;
	while (**s && **s != '\"')
	{
		if (**s == '$' && ft_isalpha((*s)[1]))
			handle_var(handler, s, shell);
		else if (**s == '$' && (*s)[1] == '?')
			handle_status(handler, s, shell);
		else
			handler->buffer[handler->buf_len++] = *(*s)++;
	}
	if (**s != '\"')
	{
		handler->cmd->cmd_line->err_msg = "error: missing closing double quote";
		handler->cmd->cmd_line->execute = false;
	}
	else
		(*s)++;
}

static void	handle_nq(t_lexer *handler, char **s, t_shell *shell)
{
	handler->quoted = false;
	while (**s && !ft_isspace(**s) && **s != '\'' && **s != '\"')
	{
		if (is_meta(*s))
		{
			if (ft_strncmp(*s, "$", 1))
				push_buffer(handler, handler->quoted);
			handle_meta(handler, s, shell);
		}
		else
			handler->buffer[handler->buf_len++] = *(*s)++;
	}
}

void	lexer(t_lexer *handler, t_command *cmd, char *cmd_str, t_shell *shell)
{
	init_handler(handler, cmd, cmd_str);
	while (*handler->cmd_str)
	{
		if (ft_isspace(*handler->cmd_str))
		{
			push_buffer(handler, handler->quoted);
			handler->cmd_str++;
		}
		else if (*handler->cmd_str == '\'')
			handle_sq(handler, &handler->cmd_str);
		else if (*handler->cmd_str == '\"')
			handle_dq(handler, &handler->cmd_str, shell);
		else
			handle_nq(handler, &handler->cmd_str, shell);
	}
	push_buffer(handler, handler->quoted);
	handler->n_tokens = size_token_lst(handler);
}
