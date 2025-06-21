/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:23:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/06/18 13:29:22 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	parse_line(t_command_line *cmd_line, char *line)
{
    t_command *cmds;

    cmds = safe_malloc(sizeof(t_command), false);
	cmd_line->line = ft_strdup(line);
	cmd_line->cmds = ft_split(line, '|');
}