/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:49:30 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/21 18:09:10 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_tokens(const char *s)
{
	t_token_state	st;

	st.i = -1;
	st.count = 0;
	st.in_sq = false;
	st.in_dq = false;
	st.in_token = false;
	while (s[++st.i])
	{
		if (s[st.i] == '\'' && !st.in_dq)
			st.in_sq = !st.in_sq;
		else if (s[st.i] == '\"' && !st.in_sq)
			st.in_dq = !st.in_dq;
		if (ft_isspace(s[st.i]) && !st.in_sq && !st.in_dq && st.in_token)
		{
			st.in_token = false;
			st.count++;
		}
		else if (!ft_isspace(s[st.i]) && !st.in_token)
			st.in_token = true;
	}
	if (st.in_token)
		st.count++;
	return (st.count);
}

void	handle_var(t_lexer *handler, char **s, t_shell *shell)
{
	int		i;
	char	var[64];
	char	*val;

	if (!handler || !s || !shell)
		return ;
	i = 0;
	(*s)++;
	while (**s && (ft_isalnum(**s) || **s == '_') && i < handler->buffer_size)
		var[i++] = *(*s)++;
	var[i] = '\0';
	val = get_env(shell, var);
	if (val)
	{
		while (*val)
		{
			if (handler->buf_len < handler->buffer_size)
				handler->buffer[handler->buf_len++] = *val++;
		}
	}
}

void	init_handler(t_lexer *handler, t_command *cmd, char *cmd_str)
{
	handler->buffer_size = BUFF_SIZE + 1;
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->cmd_str = cmd_str;
	handler->argc = 0;
	handler->buf_len = 0;
	handler->n_tokens = count_tokens(cmd_str) + 1;
	handler->cmd = cmd;
	handler->tokens = safe_malloc(sizeof(t_token) * (handler->n_tokens), true);
}

void	free_handler(t_lexer *handler)
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
