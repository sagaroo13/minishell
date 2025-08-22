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

// static void	get_redirecs(t_command *cmd, t_lexer handler, char *cmd_str)
// {
// 	cmd->stdin.n_redirs = ft_count_substr(cmd_str, "<");
// 	cmd->stdout.n_redirs = ft_count_substr(cmd_str, ">");
// 	cmd->stderr.n_redirs = ft_count_substr(cmd_str, "2>");
// 	cmd->append.n_redirs = ft_count_substr(cmd_str, ">>");
// 	cmd->heredoc.n_redirs = ft_count_substr(cmd_str, "<<");
// 	cmd->stdin.redirs = get_redirec(handler, "<", cmd->stdin.n_redirs, 1);
// 	cmd->stdout.redirs = get_redirec(handler, ">", cmd->stdout.n_redirs, 1);
// 	cmd->stderr.redirs = get_redirec(handler, "2>", cmd->stderr.n_redirs, 2);
// 	cmd->append.redirs = get_redirec(handler, ">>", cmd->append.n_redirs, 2);
// 	cmd->heredoc.redirs = get_redirec(handler, "<<", cmd->heredoc.n_redirs, 2);
// }

// static void	get_arguments(t_command *cmd, t_lexer handler)
// {
// 	int	n_args;
// 	int	i;
// 	int	j;

// 	n_args = count_argv(cmd, handler);
// 	cmd->args = safe_malloc(sizeof(char *) * (n_args + 1), true);
// 	i = -1;
// 	j = -1;
// 	while (++i < handler.n_tokens - 1)
// 	{
// 		if ((ft_strchr_charset(handler.tokens[i].token_str, "<>")
// 				&& !handler.tokens[i].quoted) || is_file(cmd,
// 				handler.tokens[i].token_str))
// 			continue ;
// 		cmd->args[++j] = ft_strdup(handler.tokens[i].token_str);
// 	}
// 	cmd->args[++j] = NULL;
// }

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
	// get_redirecs(cmd, handler, cmd_str);
	// get_arguments(cmd, handler);
	// if (is_builtin(cmd->args[0]))
	// 	cmd->builtin = true;
	// else
	// 	cmd->builtin = false;
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
	// if (cmd_line->execute)
	// 	print_info(cmd_line);
}
