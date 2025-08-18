/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:46:54 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/18 21:39:45 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


static int remove_env_var(char *name, char **env)
{
    int i = 0, j;

    while (env[i])
    {
        char *eq_pos = ft_strchr(env[i], '=');
        char *var_name;

        if (eq_pos)
            var_name = ft_substr(env[i], 0, eq_pos - env[i]);
        else
            var_name = ft_strdup(env[i]); // si no hay '=', tomamos toda la cadena

        if (!var_name)
            return 1; // fallo memoria

        if (!ft_strcmp(var_name, name))
        {
            free(var_name);
            // desplazamos todas las siguientes para sobreescribir
            j = i;
            while (env[j])
            {
                env[j] = env[j + 1];
                j++;
            }
            return 0; // éxito
        }

        free(var_name);
        i++;
    }

    // No se encontró la variable
    return 1;
}

int env_unset(char **argv, t_shell *shell)
{
    int i = 1;
    int status = 0;

    if (!argv || !argv[0] || !shell || !shell->env)
        return 1;

    if (!argv[1])
        return 0; // nada que hacer

    while (argv[i])
    {
        if (remove_env_var(argv[i], shell->env))
        {
            ft_putstr_fd("unset: variable not found: ", 2);
            ft_putendl_fd(argv[i], 2);
            status = 1; // error si alguna variable no existe
        }
        i++;
    }

    // Actualizamos correctamente el struct t_last_status
    shell->last_status.status = status;
    shell->last_status.last_exit_code = status;
    shell->last_status.exit_called = false;

    return status;
}
