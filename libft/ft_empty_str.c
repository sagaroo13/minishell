#include "libft.h"

int	ft_empty_str(const char *s)
{
    while (*s)
        if (!ft_isspace((unsigned char)*s++))
            return (0);
    return (1);
}
