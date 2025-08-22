/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:46:11 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 02:49:48 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_cmd_line(t_command_line *cmd_line)
{
	int	i;

	if (!cmd_line || !cmd_line->cmds)
		return ;
	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		if (cmd_line->cmds[i].args)
			ft_free_matrix(cmd_line->cmds[i].args);
		if (cmd_line->cmds[i].stdin.redirs)
			ft_free_matrix(cmd_line->cmds[i].stdin.redirs);
		if (cmd_line->cmds[i].stdout.redirs)
			ft_free_matrix(cmd_line->cmds[i].stdout.redirs);
		if (cmd_line->cmds[i].stderr.redirs)
			ft_free_matrix(cmd_line->cmds[i].stderr.redirs);
		if (cmd_line->cmds[i].append.redirs)
			ft_free_matrix(cmd_line->cmds[i].append.redirs);
		if (cmd_line->cmds[i].heredoc.redirs)
			ft_free_matrix(cmd_line->cmds[i].heredoc.redirs);
		if (cmd_line->cmds[i].cmd_str)
			free(cmd_line->cmds[i].cmd_str);
	}
	if (cmd_line->line)
		free(cmd_line->line);
	if (cmd_line->err_msg)
		free(cmd_line->err_msg);
	if (cmd_line->cmds)
		free(cmd_line->cmds);
	cmd_line->n_cmds = 0;
	cmd_line->line = NULL;
	cmd_line->cmds = NULL;
	cmd_line->err_msg = NULL;
}

char	*find_redir(t_lexer handler, int index, int n)
{
	char	*file;

	file = NULL;
	if ((int)ft_strlen(handler.tokens[index].token_str) > n)
		file = ft_strdup(handler.tokens[index].token_str + n);
	else if (index + 1 < handler.n_tokens && (handler.tokens[index + 1].quoted
		|| !is_meta(handler.tokens[index + 1].token_str)))
		file = ft_strdup(handler.tokens[index + 1].token_str);
	else
	{
		handler.cmd->cmd_line->err_msg = ft_strdup("syntax error" 
				"near unexpected token `newline'");
		handler.cmd->cmd_line->execute = false;
	}
	return (file);
}

char	**get_redirec(t_lexer handler, char *redir, int len, int n)
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
		if (!ft_strncmp(handler.tokens[i].token_str, redir, n)
			&& !handler.tokens[i].quoted
			&& !ft_strchr_charset((handler.tokens[i].token_str + n), "<>"))
			files[j++] = find_redir(handler, i, n);
		i++;
	}
	files[j] = NULL;
	return (files);
}
