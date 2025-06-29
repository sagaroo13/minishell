#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/minishell.h"

// Simulate the last command exit status
int g_last_status = 42;

/**
 * Expand a single token beginning with '$':
 * - If token == "$?", returns the string form of g_last_status
 * - Else if token == "$VAR", returns getenv("VAR") or "" if not set
 * - Otherwise, returns a strdup of the original token
 */
char *expand_var(const char *token)
{
    char buf[32];

    if (!token || token[0] != '$')
        return strdup(token);

    if (strcmp(token, "$?") == 0)
    {
        snprintf(buf, sizeof(buf), "%d", g_last_status);
        return strdup(buf);
    }

    const char *name = token + 1;  // skip '$'
    if (*name == '\0')
        return strdup(token);

    char *val = getenv(name);
    return strdup(val ? val : "");
}

int main(void)
{
   char *line = "echo 'Hello, World!' '$? 42 Madrid' 'minishell' e";
   char **matrix = ft_split(line, '\'');

   for (int i = 0; matrix[i]; i++)
   {
       printf("Expanded token: %s\n", matrix[i]);
   }
   ft_free_matrix(matrix);
   return 0;
}
