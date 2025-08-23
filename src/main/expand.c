/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:33:09 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 14:15:39 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_exit_status_direct(t_shell *shell, int code)
{
	shell->last_status.last_exit_code = code;
	shell->last_status.exit_called = true;
}

void	update_last_exit_status(t_shell *shell, int new_status)
{
	shell->last_status.last_exit_code = normalize_wait_status(new_status);
	shell->last_status.exit_called = true;
}

static char	*process_exit_replacement(char *tmp, char **result,
		t_shell *shell)
{
	char	*pos;
	char	*exit_code_str;
	char	*old_result;

	pos = ft_strstr(tmp, "$?");
	while (pos)
	{
		*pos = '\0';
		old_result = *result;
		*result = ft_strjoin(*result, tmp);
		free(old_result);
		exit_code_str = ft_itoa(shell->last_status.last_exit_code);
		old_result = *result;
		*result = ft_strjoin(*result, exit_code_str);
		free(old_result);
		free(exit_code_str);
		tmp = pos + 2;
		pos = ft_strstr(tmp, "$?");
	}
	return (tmp);
}

void	expand_exit_status_in_arg(char **arg, t_shell *shell)
{
	char	*result;
	char	*tmp;
	char	*old_result;

	if (!arg || !*arg)
		return ;
	result = ft_strdup("");
	tmp = *arg;
	tmp = process_exit_replacement(tmp, &result, shell);
	if (*tmp)
	{
		old_result = result;
		result = ft_strjoin(result, tmp);
		free(old_result);
	}
	free(*arg);
	*arg = result;
}

void	expand_exit_status(char **args, t_shell *shell)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strstr(args[i], "$?"))
			expand_exit_status_in_arg(&args[i], shell);
		i++;
	}
}
