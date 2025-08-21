/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:09:35 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:33:24 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	print_all(char **args)
// {
// 	int	i;

// 	i = 0;
// 	while (args[i])
// 	{
// 		printf("Arg %d: %s\n", i, args[i]);
// 		i++;
// 	}
// }

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
