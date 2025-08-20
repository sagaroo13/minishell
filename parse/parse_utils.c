/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:27:22 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 19:01:29 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	count_tokens(const char *s)
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
