/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:17:16 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 01:38:37 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*safe_malloc(size_t size, bool calloc_flag)
{
	void	*ptr;

	if (size == 0)
		perror(RED"[ERROR]"RESET" Memory allocation failed.");
	if (calloc_flag)
		ptr = ft_calloc(1, size);
	else
		ptr = malloc(size);
	if (!ptr)
		perror(RED"[ERROR]"RESET" Memory allocation failed.");
	return (ptr);
}

void	safe_getcwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror(RED"[ERROR]"RESET" getcwd failed.");
}
void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}