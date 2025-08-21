/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:25:22 by dediaz-f          #+#    #+#             */
/*   Updated: 2025/08/21 12:51:22 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_cmds(char *line)
{
	bool	in_sq;
	bool	in_dq;
	int		i;
	int		count;

	in_sq = false;
	in_dq = false;
	i = -1;
	count = 1;
	while (line[++i])
	{
		if (line[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (line[i] == '\"' && !in_sq)
			in_dq = !in_dq;
		else if (line[i] == '|' && !in_sq && !in_dq)
			count++;
	}
	return (count);
}

static void	update_mask(char c, int *f)
{
	if (c == '\'' && !(*f & 2))
		*f ^= 1;
	else if (c == '\"' && !(*f & 1))
		*f ^= 2;
}

char	**split_pipes(char *line, int n_cmds)
{
	char	**line_parts;
	int		i;
	int		j;
	int		start;
	int		f;

	line_parts = safe_malloc(sizeof(char *) * (n_cmds + 1), true);
	i = -1;
	j = 0;
	start = 0;
	f = 0;
	while (line[++i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			update_mask(line[i], &f);
		else if (line[i] == '|' && !f)
		{
			line_parts[j++] = ft_substr(line, start, i - start);
			start = i + 1;
		}
	}
	line_parts[j++] = ft_substr(line, start, i - start);
	line_parts[j] = NULL;
	return (line_parts);
}
