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

// int	count_argv(t_command *cmd, t_lexer handler)
// {
// 	int	count;
// 	int	i;

// 	count = 0;
// 	i = -1;
// 	while (++i < handler.n_tokens - 1)
// 	{
// 		if ((ft_strchr_charset(handler.tokens[i].token_str, "<>")
// 				&& !handler.tokens[i].quoted) || is_file(cmd,
// 				handler.tokens[i].token_str))
// 			continue ;
// 		count++;
// 	}
// 	return (count);
// }

// bool	iss_meta(char *str)
// {
// 	if (!str || !*str)
// 		return (true);
// 	if (ft_strstr(str, ">") || ft_strstr(str, "<") || ft_strstr(str, ">>")
// 		|| ft_strstr(str, "<<") || ft_strstr(str, "2>"))
// 		return (true);
// 	return (false);
// }

// bool	search_file(char **files, char *str, int len)
// {
// 	int	i;

// 	if (!files || !*files)
// 		return (false);
// 	i = -1;
// 	while (++i < len)
// 	{
// 		if (!ft_strcmp(files[i], str))
// 			return (true);
// 	}
// 	return (false);
// }

// bool	is_file(t_command *cmd, char *str)
// {
// 	if (!str || !*str)
// 		return (false);
// 	if (search_file(cmd->stdin.redirs, str, cmd->stdin.n_redirs)
// 		|| search_file(cmd->stdout.redirs, str, cmd->stdout.n_redirs)
// 		|| search_file(cmd->stderr.redirs, str, cmd->stderr.n_redirs)
// 		|| search_file(cmd->append.redirs, str, cmd->append.n_redirs)
// 		|| search_file(cmd->heredoc.redirs, str, cmd->heredoc.n_redirs))
// 		return (true);
// 	return (false);
// }

void 	check_pipe_closed(t_commad_line *cmd_line)
{
	int	i;

	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		if (ft_empty_str(cmd_line->cmds[i].cmd_str) && i != 0)
		{
			cmd_line->err_msg = "syntax error: pipeline not closed";
			cmd_line->execute = false;
		}
	}
}
