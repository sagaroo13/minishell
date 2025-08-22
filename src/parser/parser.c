/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:14:19 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 20:36:32 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	get_redirs(t_command *cmd, t_lexer handler)
{
	cmd->stdin.n_redirs = count_redirs(handler, "<");
	cmd->stdout.n_redirs = count_redirs(handler, ">");
	cmd->stderr.n_redirs = count_redirs(handler, "2>");
	cmd->append.n_redirs = count_redirs(handler, ">>");
	cmd->heredoc.n_redirs = count_redirs(handler, "<<");
	cmd->stdin.redirs = get_redir(handler, "<", cmd->stdin.n_redirs);
	cmd->stdout.redirs = get_redir(handler, ">", cmd->stdout.n_redirs);
	cmd->stderr.redirs = get_redir(handler, "2>", cmd->stderr.n_redirs);
	cmd->append.redirs = get_redir(handler, ">>", cmd->append.n_redirs);
	cmd->heredoc.redirs = get_redir(handler, "<<", cmd->heredoc.n_redirs);
}

static void	get_arguments(t_command *cmd, t_lexer handler)
{
	int	n_args;
	int	i;
	t_token	*aux;

	n_args = count_args(cmd, handler);
	printf("[DEBUG] get_arguments: n_args = %d\n", n_args);
	cmd->args = safe_malloc(sizeof(char *) * (n_args + 1), true);
	i = 0;
	aux = handler.token_head;
	while (aux)
	{
		if (!(ft_strchr_charset(aux->token_str, "<>") && !aux->quoted)
			&& !is_file(cmd, aux->token_str))
			cmd->args[i++] = ft_strdup(aux->token_str);
		aux = aux->next;
	}
	cmd->args[i] = NULL;
}

static void	get_cmd_info(t_command_line *cmd_line, t_command *cmd,
		char *cmd_str, t_shell *shell)
{
	t_lexer	handler;

	cmd->cmd_str = ft_strdup(cmd_str);
	cmd->cmd_line = cmd_line;
	cmd->shell = shell;
	lexer(&handler, cmd, cmd_str, shell);
	if (cmd_line->execute)
		print_lexer(&handler);
	get_redirs(cmd, handler);
	get_arguments(cmd, handler);
	if (is_builtin(cmd->args[0]))
		cmd->builtin = true;
	else
		cmd->builtin = false;
	free_handler(&handler);
}

static void	get_cmds_info(t_command_line *cmd_line, t_shell *shell, char *line)
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
		get_cmd_info(cmd_line, &cmd_line->cmds[i], line_parts[i], shell);
	check_pipe_closed(cmd_line);
	ft_free_matrix(line_parts);
}

void	parse_line(t_command_line *cmd_line, t_shell *shell, char *line)
{
	(void)shell;
	cmd_line->line = ft_strdup(line);
	cmd_line->execute = true;
	get_cmds_info(cmd_line, shell, line);
	if (cmd_line->execute)
		print_info(cmd_line);
}
