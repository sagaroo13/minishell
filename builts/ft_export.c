/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:44:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/18 21:39:45 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


static int split_name_value(char *arg, char **name, char **value)
{
    char *equal_pos = ft_strchr(arg, '=');

    if (equal_pos)
    {
        *name = ft_substr(arg, 0, equal_pos - arg);
        if (!*name)
            return 1; // fallo memoria
        *value = equal_pos + 1;
    }
    else
    {
        *name = ft_strdup(arg);
        if (!*name)
            return 1;
        *value = NULL;
    }
    return 0;
}

static void add_or_update_env(char *name, char *value, char **env)
{
    int j = 0;

    while (env[j])
    {
        if (ft_strncmp(env[j], name, ft_strlen(name)) == 0 &&
            (env[j][ft_strlen(name)] == '=' || env[j][ft_strlen(name)] == '\0'))
        {
            free(env[j]);
            if (value)
                env[j] = ft_strjoin(name, "=");
            else
                env[j] = ft_strdup(name);
            return;
        }
        j++;
    }
    if (value)
        env[j] = ft_strjoin(name, "=");
    else
        env[j] = ft_strdup(name);
    env[j + 1] = NULL;
}

bool is_valid_identifier(const char *s)
{
    int i = 0;

    if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
        return false;
    while (s[i] && s[i] != '=')
    {
        if (!ft_isalnum(s[i]) && s[i] != '_')
            return false;
        i++;
    }
    return true;
}

static int process_export_arg(char *arg, char **env)
{
    char *name = NULL;
    char *value = NULL;

    if (!is_valid_identifier(arg))
    {
        ft_putstr_fd("bash: export: ", 2);
        ft_putstr_fd(arg, 2);
        ft_putstr_fd(": not a valid identifier\n", 2);
        return 1;
    }

    if (split_name_value(arg, &name, &value))
        return 1; // fallo memoria

    add_or_update_env(name, value, env);
    free(name);
    return 0;
}

int env_export(char **argv, t_shell *shell)
{
    int i = 1;
    int status = 0;

    if (!argv || !argv[0] || !shell || !shell->env)
        return 1;

    if (!argv[1])
        return exec_env(shell);

    while (argv[i])
    {
        if (process_export_arg(argv[i], shell->env))
            status = 1;
        i++;
    }
    // Actualizamos correctamente el struct t_last_status
    shell->last_status.status = status;
    shell->last_status.last_exit_code = status;
    shell->last_status.exit_called = false;
    return status;
}
