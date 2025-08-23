/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_funcs2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:17:16 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 20:58:01 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		return ;
}
