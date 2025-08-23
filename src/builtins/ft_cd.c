/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:50:58 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 10:32:28 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_oldpwd(t_shell *shell, char *old_dir)
{
	if (old_dir)
	{
		add_or_update_env("OLDPWD", old_dir, shell);
		free(old_dir);
	}
}

static char	*get_current_dir(void)
{
	char	buf[4096];

	if (getcwd(buf, sizeof(buf)) != NULL)
		return (ft_strdup(buf));
	return (NULL);
}

static int	cd_to_home(t_shell *shell)
{
	char	*home;
	char	*old_dir;

	old_dir = get_current_dir();
	home = get_env(shell, "HOME");
	if (!home || !*home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		free(old_dir);
		return (1);
	}
	if (chdir(home) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(home, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(old_dir);
		return (1);
	}
	update_oldpwd(shell, old_dir);
	return (0);
}

static int	cd_to_oldpwd(t_shell *shell)
{
	char	*oldpwd;
	char	*current_dir;

	current_dir = get_current_dir();
	oldpwd = get_env(shell, "OLDPWD");
	if (!oldpwd || !*oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		free(current_dir);
		return (1);
	}
	if (chdir(oldpwd) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(oldpwd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(current_dir);
		return (1);
	}
	ft_putstr_fd(oldpwd, 1);
	ft_putstr_fd("\n", 1);
	update_oldpwd(shell, current_dir);
	return (0);
}

int	exec_cd(char **args, t_shell *shell)
{
	char	*old_dir;
	int		result;

	if (!args)
		return (0);
	if (args[1] == NULL)
		return (cd_to_home(shell));
	if (args[1][0] == '-' && args[1][1] == '\0')
		return (cd_to_oldpwd(shell));
	if (args[1][0] == '~' && args[1][1] == '\0')
		return (cd_to_home(shell));
	old_dir = get_current_dir();
	if (chdir(args[1]) != 0)
	{
		result = print_builtin_error("cd: No such file or directory", args[1]);
		free(old_dir);
		return (result);
	}
	update_oldpwd(shell, old_dir);
	return (0);
}
