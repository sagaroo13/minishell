/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 11:29:04 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 14:15:40 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_fragment_before_exit(char **result,
		char **tmp, char *pos)
{
	char	*fragment;
	char	*old_result;

	fragment = ft_substr(*tmp, 0, pos - *tmp);
	old_result = *result;
	*result = ft_strjoin(*result, fragment);
	free(old_result);
	free(fragment);
	*tmp = pos + 2;
}

static char	*append_exit_code_to_result(char **result, int exit_code)
{
	char	*exit_code_str;
	char	*old_result;

	exit_code_str = ft_itoa(exit_code);
	old_result = *result;
	*result = ft_strjoin(*result, exit_code_str);
	free(old_result);
	free(exit_code_str);
	return (*result);
}

char	*replace_exit_status(const char *str, int exit_code)
{
	char	*result;
	char	*tmp;
	char	*pos;
	char	*old_result;

	result = ft_strdup("");
	tmp = (char *)str;
	pos = ft_strstr(tmp, "$?");
	while (pos)
	{
		*pos = '\0';
		old_result = result;
		result = ft_strjoin(result, tmp);
		free(old_result);
		append_exit_code_to_result(&result, exit_code);
		tmp = pos + 2;
		pos = ft_strstr(tmp, "$?");
	}
	if (*tmp)
	{
		old_result = result;
		result = ft_strjoin(result, tmp);
		free(old_result);
	}
	return (result);
}
