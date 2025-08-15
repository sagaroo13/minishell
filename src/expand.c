/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:33:09 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/15 16:53:06 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_exit_status_direct(int code)
{
	g_last_exit_status.last_exit_code = code;
	g_last_exit_status.exit_called = true;
}

void	update_last_exit_status(t_last_status *status_struct, int new_status)
{
	if (WIFEXITED(new_status))
		status_struct->last_exit_code = WEXITSTATUS(new_status);
	else if (WIFSIGNALED(new_status))
		status_struct->last_exit_code = 128 + WTERMSIG(new_status);
	else if (new_status >= 0 && new_status <= 255)
		status_struct->last_exit_code = new_status;
	else
	{
		status_struct->last_exit_code = 1;
		printf("minishell: Unexpected status value: %d\n", new_status);
	}
	status_struct->exit_called = true;
}

void	expand_exit_status(char **args, t_last_status *status)
{
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
			exit_code_str = ft_itoa(status->last_exit_code);
			new_arg = ft_strjoin(exit_code_str, rest);
			free(args[i]);
			free(exit_code_str);
			args[i] = new_arg;
			break ;
		}
		i++;
	}
}
