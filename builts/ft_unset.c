/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:46:54 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/14 19:46:59 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_unset(char **argv, char **envp)
{
	int		i, j;
	char	*var;
	int		found = 0;

	if (!argv[1])
		return (0); // No se pasó ninguna variable
	var = argv[1];
	i = 0;
	while (envp[i])
	{
		if (ft_strcmp(envp[i], var) == 0)
		{
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			found = 1;
			continue; // No incrementar i, porque ahora envp[i] es la siguiente
		}
		i++;
	}
	if (!found)
		printf("unset: variable %s not found\n", var);
	return (1);
}
