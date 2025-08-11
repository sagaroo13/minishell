/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:50:58 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/04 00:07:24 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_cd(char **args)
{
	if (args[1] == NULL)
	{
		if (chdir(getenv("HOME")) != 0)
			return (write(2, "cd: error\n", 10), 1);
		return (0);
	}
	if (args[1][0] == '-' || args[1][0] == '~' || args[1][1] == '\0')
	{
		chdir(getenv("OLDPWD"));
		return (1);
	}
	if (chdir(args[1]) != 0)
		return (write(2, "cd: No such file or directory\n", 30), 1);
	return (0);
}
