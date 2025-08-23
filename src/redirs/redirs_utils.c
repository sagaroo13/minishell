/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:15:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/08/23 17:58:27 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	open_all_files(t_redirections red, t_open_flags flags)
{
	int	i;
	int	fd;

	i = -1;
	while (++i < red.n_redirs)
	{
		fd = safe_open(red.redirs[i], flags);
		safe_close(fd);
	}
}
