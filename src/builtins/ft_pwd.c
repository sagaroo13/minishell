/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:38:08 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:11:02 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write (STDOUT_FILENO, cwd, ft_strlen(cwd));
		write (STDOUT_FILENO, "\n", 1);
		return (1);
	}
	else
	{
		perror ("getcwd() error");
		return (0);
	}
}
