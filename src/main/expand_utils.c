/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 11:29:04 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 12:38:39 by shirakim         ###   ########.fr       */
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

char	*replace_exit_status(const char *str, int exit_code)
{
	char	*result;
	char	*tmp;
	char	*pos;
	char	*exit_code_str;
	char	*old_result;

	result = ft_strdup("");
	tmp = (char *)str;
	while ((pos = ft_strstr(tmp, "$?")))
	{
		*pos = '\0';
		old_result = result;
		result = ft_strjoin(result, tmp);
		free(old_result);
		
		exit_code_str = ft_itoa(exit_code);
		old_result = result;
		result = ft_strjoin(result, exit_code_str);
		free(old_result);
		free(exit_code_str);
		
		tmp = pos + 2;  // Skip "$?"
	}
	if (*tmp)
	{
		old_result = result;
		result = ft_strjoin(result, tmp);
		free(old_result);
	}
	return (result);
}
