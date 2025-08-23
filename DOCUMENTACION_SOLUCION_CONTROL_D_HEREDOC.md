# Documentación: Corrección del manejo de Control+D en Heredoc

## Problema inicial

Nuestro minishell presentaba un comportamiento incorrecto al manejar la secuencia Control+D (EOF) en los heredocs:

1. Si había texto escrito en la línea y se presionaba Control+D, el texto aparecía después del mensaje de advertencia.
2. Control+D no se ignoraba correctamente cuando había texto en la línea.
3. Aparecía un salto de línea adicional entre el mensaje de advertencia y el contenido anterior.

## Solución implementada

### 1. Uso de la función readline para manejo de EOF

La función `readline` incluida en la biblioteca estándar maneja naturalmente el Control+D de la siguiente manera:
- Si se presiona Control+D en una línea vacía, retorna `NULL` (EOF)
- Si se presiona Control+D en una línea con texto, lo ignora (no termina la entrada)

Implementamos esto en la función `read_from_stdin`:

```c
void read_from_stdin(int pipe_fd[2], char *delim)
{
    char *line;
    bool eof_reached = false;
    
    close(pipe_fd[0]);
    g_signal_received = 0;
    disable_echoctl(); // Desactivamos eco de control
    
    while (1)
    {
        // Usamos readline en lugar de get_next_line
        line = readline("heredoc> ");
        
        if (g_signal_received)
            break;
        
        // Si line es NULL, readline encontró EOF (Ctrl+D en línea vacía)
        if (!line)
        {
            eof_reached = true;
            break;
        }
        
        // Verificación del delimitador
        if (!ft_strcmp(delim, line))
        {
            free(line);
            break;
        }
        
        // Escribimos la línea y continuamos
        ft_putendl_fd(line, pipe_fd[1]);
        free(line);
    }
    
    restore_terminal();
    
    if (eof_reached)
        handle_eof_heredoc(delim);
    
    cleanup_and_exit(pipe_fd[1]);
}
```

### 2. Eliminación del salto de línea adicional en el mensaje de advertencia

Para evitar el salto de línea adicional entre el mensaje de advertencia y el contenido anterior, modificamos la función `handle_eof_heredoc`:

```c
void handle_eof_heredoc(char *delim)
{
    // Limpiar buffer para evitar caracteres residuales
    tcflush(STDIN_FILENO, TCIFLUSH);
    
    get_next_line(-1);
    
    // Eliminamos el '\n' inicial para que no haya salto de línea adicional
    ft_putstr_fd("minishell: warning: here-document "
        "delimited by end-of-file (wanted `", 2);
    ft_putstr_fd(delim, 2);
    ft_putstr_fd("')\n", 2);
    
    exit(1);
}
```

## Ventajas de la solución

1. **Comportamiento consistente con bash**: Ahora nuestro heredoc maneja Control+D exactamente como bash:
   - Ignora Control+D cuando hay texto en la línea
   - Solo considera EOF cuando se presiona Control+D en una línea vacía

2. **Uso de funciones estándar**: Utilizamos `readline` en lugar de crear una función personalizada, cumpliendo con las restricciones de la norma 42.

3. **Presentación limpia**: Eliminamos el salto de línea adicional, haciendo que el mensaje de advertencia aparezca correctamente después del prompt de heredoc.

4. **Prevención de fugas de memoria**: Nos aseguramos de liberar la memoria asignada para cada línea leída.

## Resultado final

El heredoc ahora se comporta correctamente con Control+D:

```
$ cat << EOF
heredoc> texto
heredoc> # Si presionas Ctrl+D aquí, no pasa nada
heredoc> 
heredoc> # Si presionas Ctrl+D en línea vacía:
minishell: warning: here-document delimited by end-of-file (wanted `EOF')
texto

$
```

Esta solución garantiza que nuestro minishell maneje los heredocs de manera consistente con bash, proporcionando una experiencia fluida al usuario.
