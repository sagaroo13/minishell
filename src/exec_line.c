/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:31:25 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/20 14:32:43 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char *find_path(char **envp)
// {
// 	char *env_path;
// 	int i;

// 	env_path = NULL;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
// 		{
// 			env_path = envp[i] + 5;
// 			break ;
// 		}
// 		i++;
// 	}
// 	return (env_path);
// }

char *get_env_value(t_shell *shell, const char *name)
{
    int i;
    char *eq;

    if (!shell || !name)
        return NULL;

    // Buscar en el entorno interno
    i = 0;
    while (shell->env[i])
    {
        eq = ft_strchr(shell->env[i], '=');
        if (eq && ft_strncmp(shell->env[i], name, eq - shell->env[i]) == 0
            && (size_t)(eq - shell->env[i]) == ft_strlen(name))
            return eq + 1;
        i++;
    }

    printf("[DEBUG] get_env_value: variable '%s' not found in shell env\n", name);
    // Fallback a env del sistema
    return getenv(name);
}

char *try_executable_path(char **paths, char *line)
{
    char *path_part;
    char *path;
    int i;

    i = -1;
    while (paths[++i])
    {
        path_part = ft_strjoin("/", line);
        path = ft_strjoin(paths[i], path_part);
        free(path_part);
        if (!access(path, F_OK))
            return path;
        free(path);
    }
    return NULL; // no devolver line
}

char *get_path(char *line, t_shell *shell)
{
    char *env_path;
    char **paths;
    char *executable_path;

    env_path = get_env_value(shell, "PATH");
    if (!env_path || !*env_path)
        return NULL;
    paths = ft_split(env_path, ':');
    if (!paths)
        return NULL;
    executable_path = try_executable_path(paths, line);
    ft_free_matrix(paths);
    return executable_path;
}
void exec(char *cmd_name, char **cmd_args, t_shell *shell)
{
    int builtin_status;
    char *path;

    printf("[DEBUG] exec: cmd_name='%s'\n", cmd_name);
    expand_exit_status(cmd_args, shell);

    if (is_builtin(cmd_name))
    {
        printf("[DEBUG] exec: '%s' es builtin\n", cmd_name);
        builtin_status = exec_builtin(cmd_args, shell);
        update_last_exit_status(shell, builtin_status);
        return;
    }

    path = get_path(cmd_name, shell);
    if (!path)
    {
        if (!ft_strchr(cmd_name, '/'))
            ft_putstr_fd("minishell: command not found: ", 2);
        ft_putendl_fd(cmd_name, 2);
        update_last_exit_status(shell, 127);
        printf("[DEBUG] exec: '%s' no encontrado en PATH\n", cmd_name);
        return;
    }

    printf("[DEBUG] exec: ejecutando '%s'\n", path);
    if (execve(path, cmd_args, shell->env) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        perror(cmd_name);
        update_last_exit_status(shell, 127);
    }
    if (path != cmd_name)
        free(path);
}

void exec_line(char *line, t_shell *shell)
{
    int i;
    t_command_line cmd_line;

    printf("[DEBUG] exec_line: linea='%s'\n", line);
    parse_line(&cmd_line, shell, line);

    printf("[DEBUG] exec_line: execute=%d, n_cmds=%d\n",
           cmd_line.execute, cmd_line.n_cmds);

    if (!cmd_line.execute)
    {
        if (cmd_line.err_msg)
            perror(cmd_line.err_msg);
        free_cmd_line(&cmd_line);
        return;
    }

    for (i = 0; i < cmd_line.n_cmds; i++)
    {
        printf("[DEBUG] exec_line: cmd[%d].args[0]='%s'\n",
               i, cmd_line.cmds[i].args ? cmd_line.cmds[i].args[0] : NULL);

        // Ejecutar heredoc si hay
        if (cmd_line.cmds[i].heredoc.redirs)
        {
            set_signals(MODE_HEREDOC);
            printf("[DEBUG] exec_line: cmd[%d] tiene heredoc\n", i);
            heredoc(&cmd_line.cmds[i]);
            // Si heredoc fue cancelado, abortar ejecución del resto
            if (!cmd_line.execute)
            {
                free_cmd_line(&cmd_line);
                return;
            }
        }

        if (i != cmd_line.n_cmds - 1)
        {
            printf("[DEBUG] exec_line: cmd[%d] pipe intermedio\n", i);
            exec_pipe(&cmd_line.cmds[i], shell);
        }
        else
        {
            printf("[DEBUG] exec_line: cmd[%d] ultimo comando\n", i);
            exec_last(&cmd_line.cmds[i], shell);
        }
    }

    printf("[DEBUG] exec_line: fin de linea\n");
    free_cmd_line(&cmd_line);
}
