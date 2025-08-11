/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:09:35 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 15:12:51 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_var(t_lexer_handler *handler, char **s)
{
	int		i;
	char	var[64];
	char	*val;

	i = 0;
	(*s)++;
	while (**s && (ft_isalnum(**s) || **s == '_') && i < handler->buffer_size)
		var[i++] = *(*s)++;
	var[i] = '\0';
	val = getenv(var);
	if (val)
	{
		while (*val)
		{
			if (handler->buf_len < handler->buffer_size)
				handler->buffer[handler->buf_len++] = *val++;
		}
	}
}

/* void	print_info(t_command_line *cmd_line)
{
	int	i;

	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		printf(BLUE "Command %d: %s\n" RESET, i + 1, cmd_line->cmds[i].cmd_str);
		print_all(cmd_line->cmds[i].args);
		if (cmd_line->cmds[i].stdin.redirs)
		{
			printf(GREEN "Stdin: \n" RESET);
			print_all(cmd_line->cmds[i].stdin.redirs);
		}
		if (cmd_line->cmds[i].stdout.redirs)
		{
			printf(GREEN "Stdout:\n" RESET);
			print_all(cmd_line->cmds[i].stdout.redirs);
		}
		if (cmd_line->cmds[i].stderr.redirs)
		{
			printf(GREEN "Stderr:\n" RESET);
			print_all(cmd_line->cmds[i].stderr.redirs);
		}
		if (cmd_line->cmds[i].append.redirs)
		{
			printf(GREEN "Append:\n" RESET);
			print_all(cmd_line->cmds[i].append.redirs);
		}
		if (cmd_line->cmds[i].heredoc.redirs)
		{
			printf(GREEN "Heredoc:\n" RESET);
			print_all(cmd_line->cmds[i].heredoc.redirs);
		}
		if (cmd_line->cmds[i].builtin)
			printf("Builtin: %s\n", cmd_line->cmds[i].args[0]);
		else
			printf("Not a builtin command: %s\n", cmd_line->cmds[i].args[0]);
		printf("--------------------------------------------------\n");
	}
} */

int	count_argv(t_command *cmd, t_lexer_handler handler)
{
	int	count;
	int	i;

	count = 0;
	i = -1;
	while (++i < handler.n_tokens - 1)
	{
		if ((ft_strchr_charset(handler.tokens[i].token_str, "<>")
				&& !handler.tokens[i].quoted) || is_file(cmd,
				handler.tokens[i].token_str))
			continue ;
		count++;
	}
	return (count);
}

void	search_last_redir(t_redirections red, char *cmd_str, int *iter)
{
	int		i;
	char	*p;

	i = -1;
	*iter = 0;
	while (++i < red.n_redirs)
	{
		p = ft_strstr(cmd_str, red.redirs[i]);
		if (p)
			*iter = p - cmd_str;
	}
}
