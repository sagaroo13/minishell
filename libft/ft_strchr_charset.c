#include "libft.h"

char *ft_strchr_charset(const char *str, const char *charset)
{
    const char *s;
    const char *c;

	s = str;
    if (!str || !charset)
        return (NULL);
    while (*s)
    {
        c = charset;
        while (*c)
        {
            if (*s == *c)
                return (char *)s;
            c++;
        }
        s++;
    }
    return (NULL);
}