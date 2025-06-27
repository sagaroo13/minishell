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
	int i;
	int j;
	char *var;

	i = 0;
	if (!argv[1])
		return (0);
	var = argv[1];
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0
			&& envp[i][ft_strlen(var)] == '=')
		{
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			return (1);
		}
		i++;
	}
	printf("unset: variable %s not found\n", var);
	return (1);
}

int env_export(char **argv, char **envp)
{
	int i;
	char *new_var;
	
	i = 0;
	if (!argv[1] || !envp)	
		return (0);
	new_var = ft_strdup(argv[1]);
	if (!new_var)
		return (0);
	while (envp[i])
		i++;
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
