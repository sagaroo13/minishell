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

void search_unmatched_quotes(t_command_line *cmd_line, char *str)
{
	int i;
	int single_quote;
	int double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			single_quote++;
		else if (str[i] == '\"')
			double_quote++;
		i++;
	}
	if (single_quote % 2 || double_quote % 2)
	{
		perror("Syntax error: Unmatched quotes");
		cmd_line->execute = false;
	}
}

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

int is_meta(char *str)
{
	if (!str || !*str)
		return (false);
	if (ft_strstr(str, ">") || ft_strstr(str, "<") ||
		ft_strstr(str, ">>") || ft_strstr(str, "<<") ||
		ft_strstr(str, "2>") || ft_strstr(str, "$")) 
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
	else if (!is_meta(cmd_parts[index + 1]))
		file_name = ft_strdup(cmd_parts[index + 1]);
	else
		perror("Syntax error: No se especificó un archivo para la redirección");
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

void	init_handler(t_handle_parsing *handler, char *cmd_str)
{
	handler->buffer = safe_malloc(sizeof(char) * (ft_strlen(cmd_str) + 1), false);
	handler->cmd_str = ft_strdup(cmd_str);
	handler->in_sq = false;
	handler->in_dq = false;
	handler->buf_len = 0;
	handler->toks.ptr = NULL;
	handler->toks.len = 0;
	handler->tokbuf.data = NULL;
	handler->tokbuf.len = 0;
}

char **handle_meta(char *cmd_str)
{
	t_handle_parsing handler;

	init_handler(&handler);

	buffer = safe_malloc(sizeof(char) * (ft_strlen(s) + 1), false);
	str_cpy = ft_strdup(cmd_str);
	in_sq = false;
	in_dq = false;
	buf_len = 0;
    while (*str_cpy)
	{
        if (!in_dq && !in_sq && *str_cpy == ' ')
		{
            if (buf_len > 0) {
                vec_push_ptr(&toks, strdup(tokbuf.data));
                vec_clear(&tokbuf);
            }
        }
        else if (!in_dq && *s == '\'') {
            in_sq = !in_sq;
        }
        else if (!in_sq && *s == '"' ) {
            in_dq = !in_dq;
        }
        else if (in_sq) {
            vec_push(&tokbuf, *s);
        }
        else if (in_dq && *s == '\\' && strchr("\"\\$", s[1])) {
            s++;
            vec_push(&tokbuf, *s);
        }
        else if (in_dq && *s == '$') {
            // extract var, lookup, append to tokbuf
        }
        else {
            vec_push(&tokbuf, *s);
        }
    }
    if (in_sq || in_dq) {
        fprintf(stderr, "Syntax error: unmatched quote\n");
        // cleanup
        return NULL;
    }
    if (tokbuf.len)
        vec_push_ptr(&toks, strdup(tokbuf.data));
    vec_push_ptr(&toks, NULL);
    return toks.ptr;
}


void	get_cmd_info(t_command *cmd, char *cmd_str)
{
	char **cmd_parts;

	cmd_parts = handle_meta(cmd_str);
	get_redirections(cmd, cmd_str);
	cmd->args = get_arguments(cmd, cmd_str);
	if (is_builtin(cmd->args[0]))
		cmd->builtin = true;
	else
		cmd->builtin = false;
}


void	get_cmds_info(t_command_line *cmd_line, char *line)
{
	char **line_parts;
	int i;

	search_unmatched_quotes(cmd_line, line);
	if (!cmd_line->execute)
		return ;
	line_parts = ft_split(line, '|');
	i = -1;
	while (line_parts[++i])
		get_cmd_info(&cmd_line->cmds[i], line_parts[i]);
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
    cmds = safe_malloc(sizeof(t_command) * (cmd_line->n_cmds), true);
	cmd_line->cmds = cmds;
	cmd_line->line = ft_strdup(line);
	cmd_line->execute = true;
	get_cmds_info(cmd_line, line);
	if (cmd_line->execute)
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