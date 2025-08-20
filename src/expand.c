/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:33:09 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/19 20:54:42 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_exit_status_direct(t_shell *shell, int code)
{
	shell->last_status.last_exit_code = code;
	shell->last_status.exit_called = true;
	printf("[DEBUG] set_exit_status_direct: code = %d\n", code);
}

void	update_last_exit_status(t_shell *shell, int new_status)
{
	if (WIFEXITED(new_status))
		shell->last_status.last_exit_code = WEXITSTATUS(new_status);
	else if (WIFSIGNALED(new_status))
		shell->last_status.last_exit_code = 128 + WTERMSIG(new_status);
	else if (new_status >= 0 && new_status <= 255)
		shell->last_status.last_exit_code = new_status;
	else
	{
		shell->last_status.last_exit_code = 1;
		printf("minishell: Unexpected status value: %d\n", new_status);
	}
	shell->last_status.exit_called = true;
}

void	expand_exit_status(char **args, t_shell *shell)
{
	printf("[DEBUG] expand_exit_status: starting expansion\n");
	int		i;
	char	*rest;
	char	*exit_code_str;
	char	*new_arg;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "$?", 2) == 0)
		{
			rest = args[i] + 2;
			exit_code_str = ft_itoa(shell->last_status.last_exit_code);
			new_arg = ft_strjoin(exit_code_str, rest);
			free(args[i]);
			free(exit_code_str);
			args[i] = new_arg;
			break ;
		}
		i++;
	}
}
// expand_env_vars.c
void expand_env_vars(char **args, t_shell *shell)
{
    int i = 1; // empezamos en 1 porque args[0] es "echo"
    char *val;

	printf("[DEBUG] expand_env_vars: starting expansion\n");
    while (args[i])
    {
        // Solo procesamos strings que empiezan por '$' y no sean solo '$'
        if (args[i][0] == '$' && args[i][1] != '\0' && args[i][1] != '?')
        {
            printf("[DEBUG] expand_env_vars: arg='%s'\n", args[i]);
            val = get_env_value(shell, args[i] + 1); // saltamos '$'
            free(args[i]);
            if (val)
            {
                args[i] = ft_strdup(val);
                printf("[DEBUG] expand_env_vars: found value='%s'\n", val);
            }
            else
            {
                args[i] = ft_strdup("");
                printf("[DEBUG] expand_env_vars: variable not defined\n");
            }
        }
        i++;
    }
}



