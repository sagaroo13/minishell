/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_substr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 11:28:39 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/10 11:28:53 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_len1(const char *haystack, const char *needle, size_t hlen)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (i < hlen)
	{
		if (haystack[i] == needle[0])
		{
			if (i + 1 < hlen && haystack[i + 1] == needle[0])
				i += 2;
			else if (i > 0 && !ft_isspace(haystack[i - 1]))
				i++;
			else
			{
				count++;
				i++;
			}
		}
		else
			i++;
	}
	return (count);
}

int	ft_count_substr(const char *haystack, const char *needle)
{
	int		count;
	size_t	i;
	size_t	hlen;
	size_t	nlen;

	count = 0;
	if (!haystack || !needle)
		return (0);
	nlen = ft_strlen(needle);
	hlen = ft_strlen(haystack);
	if (nlen == 1)
		return (ft_count_len1(haystack, needle, hlen));
	i = 0;
	while (i + nlen <= hlen)
	{
		if (!ft_strncmp(haystack + i, needle, nlen))
		{
			count++;
			i += nlen;
		}
		else
			i++;
	}
	return (count);
}
