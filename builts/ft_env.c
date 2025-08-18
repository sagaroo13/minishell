/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:47:36 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/18 21:39:45 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exec_env(t_shell *shell)
{
    int i = 0;

    while (shell->env[i])
    {
        write(1, shell->env[i], ft_strlen(shell->env[i]));
        write(1, "\n", 1);
        i++;
    }
    return 1; // devolver 1 como status de éxito
}

