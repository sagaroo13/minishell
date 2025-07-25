#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>



int main(void) {
    printf("Hola desde minishell\n");
    char *args[] = {"ls", "a", NULL};
    execve("/bin/ls", args, NULL);
    printf("Esto nunca se imprimirá si execve funciona\n");
    return 0;
}
