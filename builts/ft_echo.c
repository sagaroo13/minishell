/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:35:53 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/25 15:19:19 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int is_n_flag(char *s)
{
    if (!s || s[0] != '-')
        return 0;
    for (int i = 1; s[i]; i++)
    {
        if (s[i] != 'n')
            return 0;
    }
    return 1;
}

int exec_echo(char **args)
{
    int i = 1;
    int newline = 1;

    while (args[i] && is_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    expand_exit_status(args, &g_last_exit_status);
    while (args[i])
    {
        if (args[i][0] == '$')
        {
            char *env_val = getenv(args[i] + 1); // Salta el $
            if (env_val)
                write(STDOUT_FILENO, env_val, ft_strlen(env_val));
        }
        else
            write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
    if (args[i + 1])
        write(STDOUT_FILENO, " ", 1);
    i++;
    }
    if (newline)
        return (write(STDOUT_FILENO, "\n", 1), 0);
    return 0;
}


