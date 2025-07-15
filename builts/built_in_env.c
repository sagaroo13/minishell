/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 11:39:21 by shirakim          #+#    #+#             */
/*   Updated: 2025/07/15 18:03:12 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char *get_env_value(const char *key, char **envp)
{
	int i = 0;
	size_t key_len = strlen(key);

	while (envp[i])
	{
		// Verifica coincidencia exacta con nombre + '='
		if (strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] + key_len + 1); // devuelve el valor (después del '=')
		i++;
	}
	return (NULL);
}
