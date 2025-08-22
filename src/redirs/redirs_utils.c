/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:15:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/22 16:54:38 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	search_last_redir(t_redirections red, char *cmd_str, int *iter)
{
	int		i;
	char	*p;

	i = -1;
	*iter = 0;
	if (!cmd_str || !red.redirs)
		return;
	while (++i < red.n_redirs)
	{
		if (!red.redirs[i])
			continue;
		p = ft_strstr(cmd_str, red.redirs[i]);
		if (p)
			*iter = p - cmd_str;
	}
}

void	open_all_files(t_redirections red, t_open_flags flags)
{
	int	i;
	int	fd;

	i = -1;
	if (!red.redirs)
		return;
	while (++i < red.n_redirs)
	{
		if (!red.redirs[i])
			continue;
		fd = safe_open(red.redirs[i], flags);
		safe_close(fd);
	}
}
