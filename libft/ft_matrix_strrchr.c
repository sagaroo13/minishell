#include "libft.h"

int	ft_matrix_strrchr(char **matrix, const char *str, int n)
{
	int i;
	int last;

	if (!matrix || !str)
		return (-1);
	i = -1;
	last = -1;
	while (matrix[++i])
	{
		if (!ft_strncmp(matrix[i], str, n))
			last = i;
	}
	return (last);
}
