/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_charset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:49:06 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/15 16:49:06 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_charset(const char *str, const char *charset)
{
	const char	*s;
	const char	*c;

	s = str;
	if (!str || !charset)
		return (NULL);
	while (*s)
	{
		c = charset;
		while (*c)
		{
			if (*s == *c)
				return ((char *)s);
			c++;
		}
		s++;
	}
	return (NULL);
}
