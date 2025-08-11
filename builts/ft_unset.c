/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:46:54 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/12 00:48:53 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_unset(char **argv, char **envp)
{
	int		i;
	int		j;

	if (!argv[1])
		return (0);
	i = 0;
	while (envp[i])
	{
		if (ft_strcmp(envp[i], argv[1]) == 0)
		{
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			return (1);
		}
		i++;
	}
	printf ("unset: variable %s not found\n", argv[1]);
	return (1);
}
