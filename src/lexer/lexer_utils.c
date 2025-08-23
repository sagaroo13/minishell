/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:49:30 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 20:50:44 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_token(t_lexer *handler, const char *str, bool quoted)
{
	t_token	*new;

	new = safe_malloc(sizeof(t_token), true);
	new->token_str = ft_strdup(str);
	new->quoted = quoted;
	new->next = NULL;
	if (!handler->token_head)
	{
		new->index = 0;
		handler->token_head = new;
		handler->token_tail = new;
	}
	else
	{
		new->index = handler->token_tail->index + 1;
		handler->token_tail->next = new;
		handler->token_tail = new;
	}
}

int	size_token_lst(const t_lexer *handler)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = handler->token_head;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

bool	is_meta(char *str)
{
	if (!str || !*str)
		return (false);
	if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1)
		|| !ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2)
		|| !ft_strncmp(str, "2>", 2) || !ft_strncmp(str, "$", 1))
		return (true);
	return (false);
}

void	init_handler(t_lexer *handler, t_command *cmd, char *cmd_str)
{
	handler->buffer_size = BUFF_SIZE + 1;
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->cmd_str = cmd_str;
	handler->cmd = cmd;
	handler->n_tokens = 0;
	handler->buf_len = 0;
	handler->quoted = false;
	handler->token_head = NULL;
	handler->token_tail = NULL;
}

void	free_handler(t_lexer *handler)
{
	t_token	*curr;
	t_token	*next;

	curr = handler->token_head;
	while (curr)
	{
		next = curr->next;
		if (curr->token_str)
			free(curr->token_str);
		free(curr);
		curr = next;
	}
	if (handler->buffer)
		free(handler->buffer);
}
