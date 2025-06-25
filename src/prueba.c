#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    // Test cases
    const char *tests[] = {
        "$HOME",
        "$PATH",
        "$UNDEFINED_VAR",
        "$?",
        "$",
        "normal_text",
        NULL
    };

    for (int i = 0; tests[i]; i++)
    {
        char *expanded = expand_var(tests[i]);
        printf("Token: %-16s -> Expanded: \"%s\"\n", tests[i], expanded);
        free(expanded);
    }

    return 0;
}
