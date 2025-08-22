/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shirakim <shirakim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:50:58 by shirakim          #+#    #+#             */
/*   Updated: 2025/08/22 02:38:57 by shirakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Actualiza la variable OLDPWD en el entorno
 * 
 * @param shell Estructura del shell
 * @param old_dir Directorio anterior
 */
static void	update_oldpwd(t_shell *shell, char *old_dir)
{
	if (old_dir)
	{
		// Añadir o actualizar la variable OLDPWD
		add_or_update_env("OLDPWD", old_dir, shell);
		free(old_dir);
	}
}

/**
 * @brief Obtiene el directorio actual
 * 
 * @return char* Cadena con el directorio actual o NULL si hay error
 */
static char	*get_current_dir(void)
{
	char	buf[4096];

	if (getcwd(buf, sizeof(buf)) != NULL)
		return (ft_strdup(buf));
	return (NULL);
}
/**
 * @brief Imprime un mensaje de error para el comando cd
 * 
 * @param msg Mensaje de error
 * @param arg Argumento que causó el error
 * @return int Siempre devuelve 1 (código de error)
 */
static int	print_cd_(const char *msg, const char *arg)
{
	write(2, "minishell: ", 11);
	if (msg)
		write(2, msg, ft_strlen(msg));
	if (arg)
	{
		write(2, ": ", 2);
		write(2, arg, ft_strlen(arg));
	}
	write(2, "\n", 1);
	return (1);
}

/**
 * @brief Cambia al directorio home
 * 
 * @param shell Estructura del shell
 * @return int 0 si tiene éxito, 1 si hay error
 */
static int	cd_to_home(t_shell *shell)
{
	char	*home;
	char	*old_dir;

	old_dir = get_current_dir();
	home = get_env(shell, "HOME");
	if (!home || !*home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		free(old_dir);
		return (1);
	}
	if (chdir(home) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(home, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(old_dir);
		return (1);
	}
	update_oldpwd(shell, old_dir);
	return (0);
}

/**
 * Cambia al directorio anterior (OLDPWD) e imprime la ruta
 */
static int	cd_to_oldpwd(t_shell *shell)
{
	char	*oldpwd;
	char	*current_dir;

	current_dir = get_current_dir();
	oldpwd = get_env(shell, "OLDPWD");
	if (!oldpwd || !*oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		free(current_dir);
		return (1);
	}
		
	if (chdir(oldpwd) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(oldpwd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(current_dir);
		return (1);
	}
	
	// Imprimir el directorio al que se ha cambiado
	ft_putstr_fd(oldpwd, 1);
	ft_putstr_fd("\n", 1);
	
	// Actualizar OLDPWD con el directorio anterior
	update_oldpwd(shell, current_dir);
		
	return (0);
}

int	exec_cd(char **args, t_shell *shell)
{
	char	*old_dir;
	int		result;

	if (!args)
		return (0);
	if (args[1] == NULL)
		return (cd_to_home(shell));
	if (args[1][0] == '-' && args[1][1] == '\0')
		return (cd_to_oldpwd(shell));
	if (args[1][0] == '~' && args[1][1] == '\0')
		return (cd_to_home(shell));
	
	// Guardar el directorio actual antes de cambiarlo
	old_dir = get_current_dir();
	if (chdir(args[1]) != 0)
	{
		result = print_cd_("cd: No such file or directory", args[1]);
		free(old_dir);
		return (result);
	}
	
	// Actualizar OLDPWD
	update_oldpwd(shell, old_dir);
	
	return (0);
}
