/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:35:53 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/19 20:54:26 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_n_flag(char *s)
{
	int	i;

	i = 1;
	if (!s || s[0] != '-')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
int exec_echo(char **args, t_shell *shell)
{
    int i = 1;
    int newline = 1;

    expand_env_vars(args, shell);
    expand_exit_status(args, shell);
    
    while (args[i] && is_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }
    if (args[i])
    {
        write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
        i++;
        while (args[i])
        {
            write(STDOUT_FILENO, " ", 1);
            write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
            i++;
        }
    }
    if (newline)
        write(STDOUT_FILENO, "\n", 1);
    return 0;
}




