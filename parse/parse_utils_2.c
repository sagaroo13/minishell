/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:04:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 20:49:09 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_meta(char *str)
{
	if (!str || !*str)
		return (true);
	if (ft_strstr(str, ">") || ft_strstr(str, "<") || ft_strstr(str, ">>")
		|| ft_strstr(str, "<<") || ft_strstr(str, "2>"))
		return (true);
	return (false);
}

void	get_arguments(t_command *cmd, t_lexer handler)
{
	int	n_args;
	int	i;
	int	j;

	n_args = count_argv(cmd, handler);
	cmd->args = safe_malloc(sizeof(char *) * (n_args + 1), true);
	i = -1;
	j = -1;
	while (++i < handler.n_tokens - 1)
	{
		if ((ft_strchr_charset(handler.tokens[i].token_str, "<>")
				&& !handler.tokens[i].quoted) || is_file(cmd,
				handler.tokens[i].token_str))
			continue ;
		cmd->args[++j] = ft_strdup(handler.tokens[i].token_str);
	}
	cmd->args[++j] = NULL;
}

char	*find_redir(t_lexer handler, int index, int n)
{
	char	*file;

	file = NULL;
	if ((int)ft_strlen(handler.tokens[index].token_str) > n)
		file = ft_strdup(handler.tokens[index].token_str + n);
	else if (handler.tokens[index + 1].quoted
		|| !is_meta(handler.tokens[index + 1].token_str))
		file = ft_strdup(handler.tokens[index + 1].token_str);
	else
	{
		handler.cmd->cmd_line->err_msg = "minishell: error: Need a file";
		handler.cmd->cmd_line->execute = false;
	}
	return (file);
}

void	get_cmd_info(t_command_line *cmd_line, t_command *cmd,
		char *cmd_str, t_shell *shell)
{
	t_lexer	handler;

	cmd->cmd_str = ft_strdup(cmd_str);
	cmd->cmd_line = cmd_line;
	cmd->shell = shell;
	printf("[DEBUG] get_cmd_info: cmd_str='%s'\n", cmd_str);
	lexer(&handler, cmd, cmd_str, shell);
	get_redirecs(cmd, handler, cmd_str);
	get_arguments(cmd, handler);
	if (is_builtin(cmd->args[0]))
		cmd->builtin = true;
	else
		cmd->builtin = false;
	free_handler(&handler);
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
