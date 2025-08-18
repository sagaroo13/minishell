/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:31:25 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/18 23:14:54 by shirakim         ###   ########.fr       */
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


char	*try_executable_path(char **paths, char *line)
{
	char	*path_part;
	char	*path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin("/", line);
		path = ft_strjoin(paths[i], path_part);
		free(path_part);
		if (!access(path, F_OK))
			return (path);
		free(path);
	}
	return (line);
}

char	*get_path(char *line)
{
	char	*env_path;
	char	**paths;
	char	*executable_path;

	env_path = getenv("PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	executable_path = try_executable_path(paths, line);
	ft_free_matrix(paths);
	return (executable_path);
}

void exec(char *cmd_name, char **cmd_args, t_shell *shell)
{
    int     builtin_status;
    char    *path;

    // expandir $? usando shell->last_status
    expand_exit_status(cmd_args, shell);

    if (is_builtin(cmd_name))
    {
        builtin_status = exec_builtin(cmd_args, shell);
        update_last_exit_status(shell, builtin_status); // usar shell en vez de global
        return;
    }

    path = get_path(cmd_name);
    if (!path)
    {
        perror("command not found");
        update_last_exit_status(shell, 127); // reflejar error en last_status
        return;
    }

    if (execve(path, cmd_args, shell->env) == -1)
    {
        perror("minishell");
        update_last_exit_status(shell, 127); // reflejar error en last_status
    }

    free(path);
}


void exec_line(char *line, t_shell*shell)
{
    t_command_line cmd_line;
    int i = 0;

    parse_line(&cmd_line, line);
    if (!cmd_line.execute)
    {
        perror(cmd_line.err_msg);
        free_cmd_line(&cmd_line);
        return;
    }
    while (i < cmd_line.n_cmds)
    {
       // printf("[DEBUG] Before command %d: last_exit_code = %d\n", i, shell->last_status.last_exit_code);

        if (cmd_line.cmds[i].heredoc.redirs)
        {
           // printf("[DEBUG] Before heredoc: last_exit_code = %d\n", shell->last_status.last_exit_code);
            heredoc(&cmd_line.cmds[i]);
            //printf("[DEBUG] After heredoc: last_exit_code = %d\n", shell->last_status.last_exit_code);
        }

        if (i != cmd_line.n_cmds - 1)
        {
            //printf("[DEBUG] Before exec_pipe: last_exit_code = %d\n", shell->last_status.last_exit_code);
            exec_pipe(&cmd_line.cmds[i], shell);
           // printf("[DEBUG] After exec_pipe: last_exit_code = %d\n", shell->last_status.last_exit_code);
        }
        else
        {
           // printf("[DEBUG] Before exec_last: last_exit_code = %d\n", shell->last_status.last_exit_code);
            exec_last(&cmd_line.cmds[i], shell);
           // printf("[DEBUG] After exec_last: last_exit_code = %d\n", shell->last_status.last_exit_code);
        }

        //printf("[DEBUG] After command %d: last_exit_code = %d\n", i, shell->last_status.last_exit_code);
        i++;
    }
    free_cmd_line(&cmd_line);
}


