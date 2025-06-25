/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsagaro- <jsagaro-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:23:53 by jsagaro-          #+#    #+#             */
/*   Updated: 2025/06/18 13:29:22 by jsagaro-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_file(t_command *cmd, char *str)
{
	if (!str || !*str)
		return (false);
	if (!ft_strcmp(str, cmd->stdin_file) || !ft_strcmp(str, cmd->stdout_file) ||
		!ft_strcmp(str, cmd->stderr_file) || !ft_strcmp(str, cmd->append_file) ||
		!ft_strcmp(str, cmd->heredoc_delim))
		return (true);
	return (false);
}

int count_argv(t_command *cmd, char **args)
{
	int count;
	int i;

	count = 0;
	i = -1;
	while (args[++i])
	{
		if (ft_strchr_charset(args[i], "<>") || is_file(cmd, args[i]))
			continue;
		count++;
	}
	return (count);
}

char	**get_arguments(t_command *cmd, char *cmd_str)
{
	char **args;
	char **argv;
	int argc;
	int i;
	int j;

	args = ft_split(cmd_str, ' ');
	argc = count_argv(cmd, args);
	argv = safe_malloc(sizeof(char *) * (argc + 1), false);
	i = -1;
	j = -1;
	while (args[++i])
	{
		if (ft_strchr_charset(args[i], "<>") || is_file(cmd, args[i]))
			continue;
		argv[++j] = ft_strdup(args[i]);
	}
	argv[++j] = NULL;
	ft_free_matrix(args);
	return (argv);
}

char	*get_redirection(char **cmd_parts, char *redirection, int n)
{
	char	*file_name;
	int		index;

	file_name = NULL;
	index = ft_matrix_strrchr(cmd_parts, redirection, n);
	if (index == -1 || ft_strchr_charset((cmd_parts[index] + n), "<>"))
		return (NULL);
	if ((int)ft_strlen(cmd_parts[index]) > n)
		file_name = ft_strdup(cmd_parts[index] + n);
	else if (cmd_parts[index + 1])
		file_name = ft_strdup(cmd_parts[index + 1]);
	else
		perror("Error: No se especificó un archivo para la redirección");
	return (file_name);
}

void	get_redirections(t_command *cmd, char *cmd_str)
{
	char	**cmd_parts;

	cmd_parts = ft_split(cmd_str, ' ');
	cmd->stdin_file = get_redirection(cmd_parts, "<", 1);
	cmd->stdout_file = get_redirection(cmd_parts, ">", 1);
	cmd->stderr_file = get_redirection(cmd_parts, "2>", 2);
	cmd->append_file = get_redirection(cmd_parts, ">>", 2);
	cmd->heredoc_delim = get_redirection(cmd_parts, "<<", 2);
	ft_free_matrix(cmd_parts);
}

void	get_cmd_info(t_command *cmd, char *cmd_str)
{
	get_redirections(cmd, cmd_str);
	cmd->args = get_arguments(cmd, cmd_str);
	if (is_builtin(cmd->args[0]))
		cmd->builtin = true;
	else
		cmd->builtin = false;
}


void	get_cmds_info(t_command **cmds, char *line)
{
	char **line_parts;
	int i;

	line_parts = ft_split(line, '|');
	i = -1;
	while (line_parts[++i])
		get_cmd_info(&(*cmds)[i], line_parts[i]);
	ft_free_matrix(line_parts);
}

void print_info(t_command_line *cmd_line)
{
	int i;

	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		printf(BLUE"Command %d:\n"RESET, i + 1);
		print_all(cmd_line->cmds[i].args);
		if (cmd_line->cmds[i].stdin_file)
			printf("Stdin: %s\n", cmd_line->cmds[i].stdin_file);
		if (cmd_line->cmds[i].stdout_file)
			printf("Stdout: %s\n", cmd_line->cmds[i].stdout_file);
		if (cmd_line->cmds[i].stderr_file)
			printf("Stderr: %s\n", cmd_line->cmds[i].stderr_file);
		if (cmd_line->cmds[i].append_file)
			printf("Append: %s\n", cmd_line->cmds[i].append_file);
		if (cmd_line->cmds[i].heredoc_delim)
			printf("Heredoc: %s\n", cmd_line->cmds[i].heredoc_delim);
		if (cmd_line->cmds[i].builtin)
			printf("Builtin: %s\n", cmd_line->cmds[i].args[0]);
		else
			printf("Not a builtin command: %s\n", cmd_line->cmds[i].args[0]);
		printf("--------------------------------------------------\n");
	}

}

void	free_cmd_line(t_command_line *cmd_line)
{
	int i;

	if (!cmd_line || !cmd_line->cmds)
		return ;
	i = -1;
	while (++i < cmd_line->n_cmds)
	{
		ft_free_matrix(cmd_line->cmds[i].args);
		free(cmd_line->cmds[i].stdin_file);
		free(cmd_line->cmds[i].stdout_file);
		free(cmd_line->cmds[i].stderr_file);
		free(cmd_line->cmds[i].append_file);
		free(cmd_line->cmds[i].heredoc_delim);
	}
	free(cmd_line->line);
	free(cmd_line->cmds);
	cmd_line->n_cmds = 0;
	cmd_line->line = NULL;
	cmd_line->cmds = NULL;
}

void	parse_line(t_command_line *cmd_line, char *line)
{
    t_command *cmds;

	cmd_line->n_cmds = (ft_occurrence(line, '|') + 1);
    cmds = safe_malloc(sizeof(t_command) * (cmd_line->n_cmds), false);
	cmd_line->cmds = cmds;
	cmd_line->line = ft_strdup(line);
	get_cmds_info(&cmds, line);
	print_info(cmd_line);
	free_cmd_line(cmd_line);
}

// // Main de prueba
// int main(void)
// {
//     t_command_line cmd_line;
//     char *line = "cat <input.txt | grep foo >>output.log 2>error.log";

//     parse_line(&cmd_line, line);
//     return (0);
// }