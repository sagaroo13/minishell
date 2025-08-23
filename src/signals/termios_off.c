/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_off.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:44:41 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/23 12:58:29 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	disable_echoctl(void)
{
	disable_ctrl_chars();
}

void	restore_terminal(void)
{
	restore_ctrl_chars();
}
