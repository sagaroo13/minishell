/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 19:44:17 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/19 21:04:54 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *tmp = ft_strjoin(s1, s2);
    if (!tmp) return NULL;
    char *res = ft_strjoin(tmp, s3);
    free(tmp);
    return res;
}
static int split_name_value(char *arg, char **name, char **value)
{
    char *equal_pos = ft_strchr(arg, '=');

    if (equal_pos)
    {
        *name = ft_substr(arg, 0, equal_pos - arg);
        if (!*name)
            return 1; // fallo memoria
        *value = ft_strdup(equal_pos + 1); // 🔹 duplicamos el value para seguridad
    }
    else
    {
        *name = ft_strdup(arg);
        if (!*name)
            return 1;
        *value = NULL;
    }

    printf("[DEBUG] split_name_value: arg='%s', name='%s', value='%s'\n",
           arg, *name, *value ? *value : "NULL");

    return 0;
}

static void add_or_update_env(char *name, char *value, t_shell *shell)
{
    int j = 0;
    char *new_var = value ? ft_strjoin3(name, "=", value) : ft_strdup(name);

    // Buscar si ya existe
    while (shell->env[j])
    {
        if (ft_strncmp(shell->env[j], name, ft_strlen(name)) == 0 &&
            (shell->env[j][ft_strlen(name)] == '=' || shell->env[j][ft_strlen(name)] == '\0'))
        {
            free(shell->env[j]);
            shell->env[j] = new_var;
            printf("[DEBUG] Updated env[%d] = '%s'\n", j, shell->env[j]);
            return;
        }
        j++;
    }

    // Si no existe, añadir al final. Realloc si es necesario
    shell->env = realloc(shell->env, sizeof(char *) * (j + 2)); // +1 nuevo, +1 NULL
    shell->env[j] = new_var;
    shell->env[j + 1] = NULL;

    printf("[DEBUG] Added env[%d] = '%s'\n", j, shell->env[j]);
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
static int process_export_arg(char *arg, t_shell *shell)
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
        return 1;

    add_or_update_env(name, value, shell);
    free(name);
    free(value); // 🔹 liberar value también
    printf("[DEBUG] process_export_arg: finished processing '%s'\n", arg);
    return 0;
}
int env_export(char **argv, t_shell *shell)
{
    int i = 1;
    int status = 0;

    if (!argv || !argv[0] || !shell)
        return 1;

    if (!argv[1])
        return exec_env(shell);

    while (argv[i])
    {
        if (process_export_arg(argv[i], shell))
            status = 1;
        i++;
    }

    shell->last_status.status = status;
    shell->last_status.last_exit_code = status;
    shell->last_status.exit_called = false;

    return status;
}
