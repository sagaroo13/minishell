/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_meta.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 18:37:07 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 20:48:31 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redir(t_lexer *handler, char **s, int len)
{
	int	i;

	i = -1;
	if (is_meta(&(*s)[len]) && ft_strncmp(&(*s)[len], "$", 1))
	{
		handler->cmd->cmd_line->err_msg = "error: unexpected"
			"redirection expression";
		handler->cmd->cmd_line->execute = false;
	}
	while (++i < len)
		handler->buffer[handler->buf_len++] = *(*s)++;
	push_buffer(handler, handler->quoted);
}

void	handle_status(t_lexer *handler, char **s, t_shell *shell)
{
	char	*status;
	char	*p;

	status = ft_itoa(shell->last_status.last_exit_code);
	p = status;
	if (status)
	{
		while (*status)
		{
			if (handler->buf_len < handler->buffer_size)
				handler->buffer[handler->buf_len++] = *status++;
		}
	}
	(*s) += 2;
	free(p);
}

void	tokenize_var(t_lexer *handler, char *val)
{
	bool	pushed;

	pushed = false;
	while (*val)
	{
		if (ft_isspace(*val))
		{
			if (!pushed)
			{
				push_buffer(handler, handler->quoted);
				pushed = true;
			}
		}
		else
		{
			if (handler->buf_len < handler->buffer_size)
				handler->buffer[handler->buf_len++] = *val;
			pushed = false;
		}
		val++;
	}
}

void	handle_var(t_lexer *handler, char **s, t_shell *shell)
{
	int		i;
	char	var[64];
	char	*val;

	i = 0;
	(*s)++;
	while (**s && (ft_isalnum(**s) || **s == '_') && i < handler->buffer_size)
		var[i++] = *(*s)++;
	var[i] = '\0';
	val = get_env(shell, var);
	if (val)
		tokenize_var(handler, val);
}

void	handle_meta(t_lexer *handler, char **s, t_shell *shell)
{
	if (**s == '$' && ft_isalpha((*s)[1]))
		handle_var(handler, s, shell);
	else if (**s == '$' && (*s)[1] == '?')
		handle_status(handler, s, shell);
	else if (**s == '<' && (*s)[1] == '<')
		handle_redir(handler, s, 2);
	else if (**s == '>' && (*s)[1] == '>')
		handle_redir(handler, s, 2);
	else if (**s == '2' && (*s)[1] == '>')
		handle_redir(handler, s, 2);
	else if (**s == '<')
		handle_redir(handler, s, 1);
	else if (**s == '>')
		handle_redir(handler, s, 1);
	else
		handler->buffer[handler->buf_len++] = *(*s)++;
}
