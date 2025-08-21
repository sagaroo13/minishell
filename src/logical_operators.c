/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 09:00:00 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/21 10:03:39 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief Cuenta el número de comandos separados por pipes y operadores lógicos.
 * 
 * Los operadores lógicos (&&, ||) se cuentan como separadores adicionales.
 * Esta función es similar a count_cmds pero considera también &&, ||.
 * 
 * @param line Línea de comandos a analizar
 * @return int Número total de comandos individuales
 */
int	count_cmds_with_logical_ops(char *line)
{
	bool	in_sq;
	bool	in_dq;
	int		i;
	int		count;

	in_sq = false;
	in_dq = false;
	i = -1;
	count = 1;
	while (line[++i])
	{
		if (line[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (line[i] == '\"' && !in_sq)
			in_dq = !in_dq;
		else if (!in_sq && !in_dq)
		{
			if (line[i] == '|' && (i == 0 || line[i - 1] != '|'))
				count++;
			else if (line[i] == '&' && line[i + 1] == '&')
			{
				count++;
				i++;
			}
			else if (line[i] == '|' && line[i + 1] == '|')
			{
				count++;
				i++;
			}
		}
	}
	return (count);
}

/**
 * @brief Actualiza la máscara de comillas.
 * 
 * Función para saber si estamos dentro de comillas simples o dobles.
 * 
 * @param c Carácter actual
 * @param f Puntero al flag de estado de comillas
 */
static void	update_mask(char c, int *f)
{
	if (c == '\'' && !(*f & 2))
		*f ^= 1;
	else if (c == '\"' && !(*f & 1))
		*f ^= 2;
}

/**
 * @brief Identifica el tipo de operador en la posición actual.
 * 
 * @param line Línea de comandos
 * @param pos Posición actual
 * @param f Flag de estado de comillas
 * @return int 1: pipe, 2: &&, 3: ||, 0: ninguno
 */
static int	identify_operator(char *line, int pos, int f)
{
	if (f != 0)
		return (0);
	
	if (line[pos] == '|')
	{
		if (line[pos + 1] == '|')
			return (3);
		else
			return (1);
	}
	else if (line[pos] == '&' && line[pos + 1] == '&')
		return (2);
	
	return (0);
}

/**
 * @brief Divide la línea de comandos por pipes y operadores lógicos.
 * 
 * Almacena también el tipo de operador que sigue a cada parte.
 * 
 * @param line Línea de comandos
 * @param n_cmds Número de comandos
 * @return t_cmd_parts* Array de partes de comandos con sus operadores
 */
t_cmd_parts	*split_cmds_with_logical_ops(char *line, int n_cmds)
{
	t_cmd_parts	*cmd_parts;
	int		i;
	int		j;
	int		start;
	int		f;
	int		op_type;

	cmd_parts = safe_malloc(sizeof(t_cmd_parts) * n_cmds, true);
	i = -1;
	j = 0;
	start = 0;
	f = 0;
	while (line[++i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			update_mask(line[i], &f);
		else
		{
			op_type = identify_operator(line, i, f);
			if (op_type)
			{
				cmd_parts[j].cmd = ft_substr(line, start, i - start);
				cmd_parts[j].operator = op_type;
				j++;
				
				if (op_type == 2 || op_type == 3)
					i++;
				
				start = i + 1;
			}
		}
	}
	cmd_parts[j].cmd = ft_substr(line, start, i - start);
	cmd_parts[j].operator = 0;
	
	return (cmd_parts);
}

/**
 * @brief Procesa una línea de comandos con operadores lógicos.
 * 
 * Ejecuta los comandos secuencialmente, respetando la lógica de && y ||.
 * 
 * @param line Línea de comandos
 * @param shell Estructura shell
 */
void	process_logical_line(char *line, t_shell *shell)
{
	int			n_cmds;
	t_cmd_parts	*cmd_parts;
	int			i;
	int			prev_result;

	n_cmds = count_cmds_with_logical_ops(line);
	cmd_parts = split_cmds_with_logical_ops(line, n_cmds);
	
	i = 0;
	prev_result = 0;
	
	while (i < n_cmds)
	{
		if (i > 0)
		{
			if (cmd_parts[i-1].operator == 2 && prev_result != 0)
			{
				i++;
				continue;
			}
			else if (cmd_parts[i-1].operator == 3 && prev_result == 0)
			{
				i++;
				continue;
			}
		}
		
		exec_line(cmd_parts[i].cmd, shell);
		prev_result = shell->last_status.last_exit_code;
		
		i++;
	}
	
	i = 0;
	while (i < n_cmds)
		free(cmd_parts[i++].cmd);
	free(cmd_parts);
}

/**
 * @brief Detecta si una línea contiene operadores lógicos (&&, ||).
 * 
 * @param line Línea de comandos
 * @return true Si contiene operadores lógicos
 * @return false Si no contiene operadores lógicos
 */
bool	contains_logical_operators(char *line)
{
	bool	in_sq;
	bool	in_dq;
	int		i;

	in_sq = false;
	in_dq = false;
	i = -1;
	while (line[++i])
	{
		if (line[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (line[i] == '\"' && !in_sq)
			in_dq = !in_dq;
		else if (!in_sq && !in_dq)
		{
			if (line[i] == '&' && line[i + 1] == '&')
				return (true);
			else if (line[i] == '|' && line[i + 1] == '|')
				return (true);
		}
	}
	return (false);
}
