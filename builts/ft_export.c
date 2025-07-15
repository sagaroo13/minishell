/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:44:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/15 18:02:59 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int env_export(char **argv, char **envp)
{
	int i = 0;
	char *new_var;

	if (!argv || !argv[0] || !envp)
		return (0);
	// Caso: solo "export" sin argumentos
	 if (!argv[1])
	{
		exec_env(envp);
		return (1);
	} 
	// Caso: hay argumento como "MYVAR=123"
	new_var = ft_strdup(argv[1]);
	if (!new_var)
		return (0);
	// Mostrar para depuración
	// Buscar el final del entorno
	while (envp[i] != NULL)
		i++;
	//  Agregar la nueva variable
	envp[i] = new_var;
	envp[i + 1] = NULL;
	return (1);
}


