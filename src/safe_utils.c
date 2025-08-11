/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:17:16 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/08 15:17:54 by shirakim         ###   ########.fr       */
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

void	*safe_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
		return (NULL);
	if (old_size == new_size)
		return (ptr);
	new_ptr = realloc(ptr, new_size);
	if (!new_ptr)
		perror(RED"[ERROR]"RESET" Memory reallocation failed.");
	return (new_ptr);
}

void	safe_getcwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror(RED"[ERROR]"RESET" getcwd failed.");
}
