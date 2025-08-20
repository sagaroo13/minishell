/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:46:54 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/19 08:44:26 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Elimina una variable del array env
// -------- remove_env_var.c --------
static int remove_env_var(const char *name, char **env)
{
    int i, j;
    char *eq;
    size_t key_len;

    if (!name || !env)
        return 1;
    i = 0;
    while (env[i])
    {
        eq = ft_strchr(env[i], '=');
        key_len = eq ? (size_t)(eq - env[i]) : ft_strlen(env[i]);
        if (ft_strlen(name) == key_len && ft_strncmp(env[i], name, key_len) == 0)
        {
            free(env[i]);
            j = i;
            while (env[j])
            {
                env[j] = env[j + 1];
                j++;
            }
            return 0;
        }
        i++;
    }
    return 1;
}

// -------- env_unset.c --------
// Auxiliar: procesa un argumento de unset
static int unset_one_var(char *arg, t_shell *shell)
{
    if (ft_strchr(arg, '='))
    {
        ft_putstr_fd("unset: invalid name: ", 2);
        ft_putendl_fd(arg, 2);
        return 1;
    }
    if (remove_env_var(arg, shell->env))
    {
        ft_putstr_fd("unset: variable not found: ", 2);
        ft_putendl_fd(arg, 2);
        return 1;
    }
    return 0;
}

// Builtin unset
int env_unset(char **argv, t_shell *shell)
{
    int i;
    int status;

    if (!argv || !argv[0] || !shell || !shell->env)
        return 1;
    if (!argv[1])
        return 0;
    i = 1;
    status = 0;
    while (argv[i])
    {
        if (unset_one_var(argv[i], shell))
            status = 1;
        i++;
    }
    shell->last_status.last_exit_code = status;
    shell->last_status.status = status;
    shell->last_status.exit_called = false;
    return status;
}

