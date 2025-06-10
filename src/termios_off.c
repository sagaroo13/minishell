#include <termios.h>
#include <unistd.h>
#include "../include/minishell.h"

#ifndef ECHOCTL
#define ECHOCTL 0001000
#endif

struct termios original_termios;

// Restaurar la configuración de la terminal al salir
void restore_terminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

// Desactivar impresión de `^C`
void disable_echoctl()
{
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &original_termios);  // Guardar configuración actual
    new_termios = original_termios;
    new_termios.c_lflag &= ~ECHOCTL;  // Desactivar impresión de caracteres de control
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}
