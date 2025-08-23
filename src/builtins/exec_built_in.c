/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:30:27 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 19:35:25 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strncmp(command, "cd", 3) == 0
		|| ft_strncmp(command, "echo", 5) == 0
		|| ft_strncmp(command, "pwd", 4) == 0
		|| ft_strncmp(command, "exit", 5) == 0
		|| ft_strncmp(command, "env", 4) == 0
		|| ft_strncmp(command, "export", 7) == 0
		|| ft_strncmp(command, "unset", 6) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **args, t_shell *shell)
{
	if (!args || !args[0])
		return (0);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (exec_echo(args, shell));
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (exec_pwd());
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		return (exec_exit(args, shell));
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		return (exec_cd(args, shell));
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return (exec_env(shell));
	else if (ft_strncmp(args[0], "export", 6) == 0)
		return (env_export(args, shell));
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return (env_unset(args, shell));
	return (0);
}

int	print_builtin_error(const char *msg, const char *arg)
{
	write(2, "minishell: ", 11);
	if (msg)
		write(2, msg, ft_strlen(msg));
	if (arg)
	{
		write(2, ": ", 2);
		write(2, arg, ft_strlen(arg));
	}
	write(2, "\n", 1);
	return (1);
}
