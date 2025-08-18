/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:05:57 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/18 22:48:42 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_redirecs(t_command *cmd, t_lexer_handler handler, char *cmd_str)
{
	cmd->stdin.n_redirs = ft_count_substr(cmd_str, "<");
	cmd->stdout.n_redirs = ft_count_substr(cmd_str, ">");
	cmd->stderr.n_redirs = ft_count_substr(cmd_str, "2>");
	cmd->append.n_redirs = ft_count_substr(cmd_str, ">>");
	cmd->heredoc.n_redirs = ft_count_substr(cmd_str, "<<");
	cmd->stdin.redirs = get_redirec(handler, "<", cmd->stdin.n_redirs, 1);
	cmd->stdout.redirs = get_redirec(handler, ">", cmd->stdout.n_redirs, 1);
	cmd->stderr.redirs = get_redirec(handler, "2>", cmd->stderr.n_redirs, 2);
	cmd->append.redirs = get_redirec(handler, ">>", cmd->append.n_redirs, 2);
	cmd->heredoc.redirs = get_redirec(handler, "<<", cmd->heredoc.n_redirs, 2);
}

void	init_handler(t_lexer_handler *handler, t_command *cmd, char *cmd_str)
{
	handler->buffer_size = BUFF_SIZE + 1;
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->cmd_str = cmd_str;
	handler->argc = 0;
	handler->buf_len = 0;
	handler->n_tokens = (count_tokens(cmd_str) + 1);
	handler->cmd = cmd;
	handler->tokens = safe_malloc(sizeof(t_token) * (handler->n_tokens), true);
}

void	get_cmds_info(t_command_line *cmd_line, char *line)
{
	t_command	*cmds;
	char		**line_parts;
	int			i;

	cmd_line->n_cmds = count_cmds(line);
	cmds = safe_malloc(sizeof(t_command) * (cmd_line->n_cmds), true);
	cmd_line->cmds = cmds;
	line_parts = split_pipes(line, cmd_line->n_cmds);
	i = -1;
	while (line_parts[++i])
		get_cmd_info(cmd_line, &cmd_line->cmds[i], line_parts[i]);
	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		if (ft_empty_str(line_parts[i]))
		{
			cmd_line->err_msg = "syntax error: pipeline not closed";
			cmd_line->execute = false;
		}
	}
	ft_free_matrix(line_parts);
}

void	push_buffer(t_lexer_handler *handler, bool quoted)
{
	handler->cmd_str++;
	if (handler->buf_len == 0)
		return ;
	handler->tokens[handler->argc].quoted = false;
	if (quoted)
		handler->tokens[handler->argc].quoted = true;
	handler->buffer[handler->buf_len] = '\0';
	handler->tokens[handler->argc++].token_str = ft_strdup(handler->buffer);
	free(handler->buffer);
	handler->buffer = safe_malloc(sizeof(char) * (handler->buffer_size), true);
	handler->buf_len = 0;
}
