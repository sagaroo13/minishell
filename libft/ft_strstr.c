#include "libft.h"

char	*ft_strstr(const char *big, const char *little)
{
	size_t	little_len;

	if (!*little)
		return ((char *)big);
	little_len = ft_strlen(little);
	while (*big)
	{
		if (*big == *little && !ft_strncmp(big, little, little_len) )
			return ((char *)big);
		big++;
	}
	return (NULL);
}