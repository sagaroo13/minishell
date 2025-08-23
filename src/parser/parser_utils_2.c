/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:04:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 20:14:24 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_args(t_command *cmd, t_lexer handler)
{
	int	count;
	t_token	*aux;

	count = 0;
	aux = handler.token_head;
	while (aux)
	{
		if ((!is_meta_redir(aux->token_str) || aux->quoted)
			&& !is_file(cmd, aux->token_str))
			count++;
		aux = aux->next;
	}
	return (count);
}

int	count_redirs(t_lexer handler, char *redir)
{
	int count;
	t_token *aux;

	aux = handler.token_head;
	count = 0;
	while (aux)
	{
		if (!ft_strcmp(aux->token_str, redir) && !aux->quoted)
			count++;
		aux = aux->next;
	}
	return (count);			
}

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

bool is_meta_redir(char *s)
{
	return (!ft_strcmp(s, "<") || !ft_strcmp(s, ">")
		|| !ft_strcmp(s, "2>") || !ft_strcmp(s, ">>")
		|| !ft_strcmp(s, "<<"));
}
