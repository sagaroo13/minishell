/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:47:36 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 13:10:44 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		write(1, shell->env[i], ft_strlen(shell->env[i]));
		write(1, "\n", 1);
		i++;
	}
	return (1);
}
