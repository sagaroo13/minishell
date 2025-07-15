/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 11:38:01 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/15 18:03:17 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	echo_var(char **argv, char **envp)
{
	int i;
	int j;
 	char *value;

 	i = 0;
 	j = 0;
 	while (argv[i])
 	{
 		if (argv[i][0] == '$')
 		{
 			j = 0;
 			while (envp[j])
 			{
 				if (ft_strncmp(&argv[i][1], envp[j], ft_strlen(&argv[i][1])) == 0)
 				{
 					value = ft_strchr(envp[j], '=');
 					if (value)
 						write(1, value + 1, ft_strlen(value + 1));
 					return (1);
 					write(1, "\n", 1);
 				}
 				j++;
 			}
 			return (1);
 		}
 		i++;
 	}
 	return (0);
}

int	exec_pwd(void)
{
	char	cwd[1024];
	
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	return (1);
}

