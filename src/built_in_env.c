#include "../include/minishell.h"

int	exec_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		write(1, envp[i], ft_strlen(envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (1);
}

int	env_unset(char **argv, char **envp)
{
	int		i, j;
	char	*var;
	int		found = 0;

	if (!argv[1])
		return (0); // No se pasó ninguna variable
	var = argv[1];
	i = 0;
	while (envp[i])
	{
		if (ft_strcmp(envp[i], var) == 0)
		{
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			found = 1;
			continue; // No incrementar i, porque ahora envp[i] es la siguiente
		}
		i++;
	}
	if (!found)
		printf("unset: variable %s not found\n", var);
	return (1);
}




int env_export(char **argv, char **envp)
{
	int i = 0;
	char *new_var;

	if (!argv || !argv[0] || !envp)
		return (0);
	// Caso: solo "export" sin argumentos
	/* if (!argv[1])
	{
		exec_env(envp);
		return (1);
	} */
	// Caso: hay argumento como "MYVAR=123"
	new_var = ft_strdup(argv[1]);
	if (!new_var)
		return (0);
	// Mostrar para depuración
	// Buscar el final del entorno
	while (envp[i] != NULL)
		i++;
	//  Agregar la nueva variable
	envp[i] = new_var;
	envp[i + 1] = NULL;
	return (1);
}


char *get_env_value(const char *key, char **envp)
{
	int i = 0;
	size_t key_len = strlen(key);

	while (envp[i])
	{
		// Verifica coincidencia exacta con nombre + '='
		if (strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] + key_len + 1); // devuelve el valor (después del '=')
		i++;
	}
	return (NULL);
}
