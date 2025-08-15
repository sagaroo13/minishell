/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:39:18 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/15 16:44:39 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len])
		len++;
	return (len);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if ((char)s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*new_str;
	size_t	lens1;
	size_t	lens2;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	lens1 = ft_strlen_gnl(s1);
	lens2 = ft_strlen_gnl(s2);
	new_str = malloc(sizeof(char) * (lens1 + lens2 + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < lens1)
	{
		new_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < lens2)
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}
