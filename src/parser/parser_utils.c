/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:27:22 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 19:10:09 by jsagaro-         ###   ########.fr       */
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

char	*find_redir(t_lexer handler, t_token *aux)
{
	char	*file;

	file = NULL;
	if (aux->next)
		file = ft_strdup(aux->next->token_str);
	else
	{
		handler.cmd->cmd_line->err_msg = "minishell: error: Need a file";
		handler.cmd->cmd_line->execute = false;
	}
	return (file);
}

char	**get_redir(t_lexer handler, char *redir, int len)
{
	char	**files;
	int		i;
	t_token	*aux;

	if (!handler.token_head)
		return (NULL);
	files = safe_malloc(sizeof(char *) * (len + 1), true);
	i = 0;
	aux = handler.token_head;
	while (aux)
	{
		if (!ft_strcmp(aux->token_str, redir) && !aux->quoted)
			files[i++] = find_redir(handler, aux);
		aux = aux->next;
	}
	files[i] = NULL;
	return (files);
}
