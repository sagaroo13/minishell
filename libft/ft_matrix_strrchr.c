/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_strrchr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:44:55 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/15 16:44:55 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_matrix_strrchr(char **matrix, const char *str, int n)
{
	int	i;
	int	last;

	if (!matrix || !str)
		return (-1);
	i = -1;
	last = -1;
	while (matrix[++i])
	{
		if (!ft_strncmp(matrix[i], str, n))
			last = i;
	}
	return (last);
}
