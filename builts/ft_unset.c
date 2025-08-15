/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dediaz-f <dediaz-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:46:54 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/15 18:37:42 by dediaz-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_unset(char **argv, char **envp)
{
	char	*var;
	int		i;
	int		j;

	if (!argv[1])
		return (0);
	i = 0;
	while (envp[i])
	{
		var = ft_substr(envp[i], 0, (ft_strchr(envp[i], '=') - envp[i]));
		if (!ft_strcmp(var, argv[1]))
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
