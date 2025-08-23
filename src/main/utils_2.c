/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:09:35 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 11:29:30 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env(t_shell *shell, const char *name)
{
	int		i;
	char	*eq;

	i = 0;
	if (!shell || !name)
		return (NULL);
	while (shell->env && shell->env[i])
	{
		eq = ft_strchr(shell->env[i], '=');
		if (eq && ft_strncmp(shell->env[i], name, eq - shell->env[i]) == 0
			&& (size_t)(eq - shell->env[i]) == ft_strlen(name))
			return (eq + 1);
		i++;
	}
	return (NULL);
}

char	**copy_envp(char **envp)
{
	int		n;
	int		i;
	char	**new_env;

	n = 0;
	i = -1;
	while (envp && envp[n])
		n++;
	new_env = safe_malloc(sizeof(char *) * (n + 1), false);
	while (++i < n)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_free_matrix(new_env);
			return (NULL);
		}
	}
	new_env[n] = NULL;
	return (new_env);
}

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
