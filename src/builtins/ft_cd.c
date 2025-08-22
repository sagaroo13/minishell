/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:50:58 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 19:53:55 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	print_cd_(const char *msg, const char *arg)
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

static int	cd_to_home(t_shell *shell)
{
	const char	*home;
	const char	*msg;

	(void)shell;
	home = get_env(shell, "HOME");
	if (!home || chdir(home) != 0)
	{
		msg = "minishell: cd: HOME not accessible\n";
		return (write(2, msg, ft_strlen(msg)), 1);
	}
	return (0);
}

static int	cd_to_oldpwd(t_shell *shell)
{
	const char	*oldpwd;
	const char	*msg;

	(void)shell;
	oldpwd = getenv("OLDPWD");
	if (!oldpwd)
	{
		msg = "minishell: OLDPWD not set\n";
		return (write(2, msg, ft_strlen(msg)), 1);
	}
	if (chdir(oldpwd) != 0)
	{
		msg = "minishell: cannot change directory\n";
		return (write(2, msg, ft_strlen(msg)), 1);
	}
	return (0);
}

int	exec_cd(char **args, t_shell *shell)
{
	if (!args)
		return (0);
	if (args[1] == NULL)
		return (cd_to_home(shell));
	if (args[1][0] == '-' && args[1][1] == '\0')
		return (cd_to_oldpwd(shell));
	if (args[1][0] == '~' && args[1][1] == '\0')
		return (cd_to_home(shell));
	if (chdir(args[1]) != 0)
		return (print_cd_("cd: No such file or directory", args[1]));
	return (0);
}
