# Documentación Completa: Builtins, Heredoc, Signals y Cat con Pipes

## 1. BUILTINS - Comandos Internos del Shell

### 1.1 ft_echo.c
**Sintaxis:** `echo [-n] [string ...]`

**Lógica:**
```c
// Función principal que maneja el comando echo
int exec_echo(char **args, t_shell *shell)
{
    // 1. Detecta si hay flag -n (sin salto de línea final)
    // 2. Procesa cada argumento y lo imprime
    // 3. Añade espacios entre argumentos
    // 4. Añade salto de línea final (si no hay -n)
}

// Función que verifica si un argumento es el flag -n
static bool is_n_flag(char *arg)
{
    // Verifica que sea exactamente "-n"
}

// Función que imprime los argumentos
static void print_echo_args(char **args, int i, int newline)
{
    // Itera por todos los argumentos desde el índice i
    // Imprime cada uno con espacios entre ellos
}
```

**Características:**
- Soporte completo para el flag `-n`
- Manejo correcto de múltiples argumentos
- Espaciado adecuado entre palabras

### 1.2 ft_pwd.c
**Sintaxis:** `pwd`

**Lógica:**
```c
int exec_pwd(void)
{
    char cwd[1024];
    
    // Obtiene el directorio actual usando getcwd()
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        write(1, cwd, ft_strlen(cwd));
    write(1, "\n", 1);
    return (0);
}
```

**Características:**
- Simple y directo
- Usa getcwd() para obtener el directorio actual
- Manejo de errores incorporado

### 1.3 ft_cd.c
**Sintaxis:** `cd [directory]` | `cd -` | `cd ~`

**Lógica:**
```c
// Función principal
int exec_cd(char **args, t_shell *shell)
{
    // Sin argumentos -> ir a HOME
    if (!args[1])
        return (cd_to_home(shell));
    
    // Con "-" -> ir a OLDPWD
    if (args[1][0] == '-' && args[1][1] == '\0')
        return (cd_to_oldpwd(shell));
    
    // Con "~" -> ir a HOME
    if (args[1][0] == '~' && args[1][1] == '\0')
        return (cd_to_home(shell));
    
    // Directorio específico
    return (change_to_directory(args[1], shell));
}

// Funciones auxiliares
static int cd_to_home(t_shell *shell)     // Navega a HOME
static int cd_to_oldpwd(t_shell *shell)   // Navega a directorio anterior
static void update_oldpwd(t_shell *shell, char *old_dir) // Actualiza OLDPWD
```

**Características:**
- Soporte para `cd`, `cd -`, `cd ~`
- Actualización automática de PWD y OLDPWD
- Manejo de errores (directorio no existe)

### 1.4 ft_env.c
**Sintaxis:** `env`

**Lógica:**
```c
int exec_env(t_shell *shell)
{
    int i = 0;
    
    // Itera por todas las variables de entorno
    while (shell->env[i])
    {
        // Imprime cada variable en formato NAME=value
        write(1, shell->env[i], ft_strlen(shell->env[i]));
        write(1, "\n", 1);
        i++;
    }
    return (1);
}
```

**Características:**
- Muestra todas las variables de entorno
- Formato idéntico a bash
- Acceso directo al array de entorno interno

### 1.5 ft_export.c y ft_export_utils.c
**Sintaxis:** `export [NAME[=value] ...]`

**Lógica:**
```c
// Función principal
int env_export(char **argv, t_shell *shell)
{
    // Sin argumentos -> mostrar todas las variables exportadas
    if (!argv[1])
        return (exec_env(shell));
    
    // Con argumentos -> procesar cada uno
    while (argv[i])
    {
        if (process_export_arg(argv[i], shell))
            status = 1;
        i++;
    }
}

// Procesa un argumento individual
static int process_export_arg(char *arg, t_shell *shell)
{
    // 1. Divide NAME=value
    // 2. Valida el identificador
    // 3. Añade o actualiza la variable
}

// Funciones de soporte
void add_or_update_env(char *name, char *value, t_shell *shell)
void add_env_var(char *new_var, t_shell *shell)
int update_env_var(char *name, char *new_var, t_shell *shell)
```

**Características:**
- Validación de nombres de variables (solo letras, números, _)
- Soporte para `export VAR` y `export VAR=value`
- Gestión dinámica del array de entorno

### 1.6 ft_unset.c
**Sintaxis:** `unset NAME [NAME ...]`

**Lógica:**
```c
// Función principal
int env_unset(char **argv, t_shell *shell)
{
    // Procesa cada variable a eliminar
    while (argv[i])
    {
        if (unset_one_var(argv[i], shell))
            status = 1;
        i++;
    }
}

// Elimina una variable específica
static int unset_one_var(char *arg, t_shell *shell)
{
    // 1. Valida que no contenga '='
    // 2. Busca y elimina la variable
}

// Funciones auxiliares
static int find_env_index(const char *name, char **env)  // Busca índice
static int remove_env_var(const char *name, char **env)  // Elimina variable
```

**Características:**
- Eliminación segura de variables
- Recompactación automática del array
- Validación de argumentos

### 1.7 ft_exit.c
**Sintaxis:** `exit [code]`

**Lógica:**
```c
// Función principal
int exec_exit(char **args, t_shell *shell)
{
    write(1, "exit\n", 5);
    return (process_exit_args(args, shell));
}

// Procesa argumentos de exit
static int process_exit_args(char **args, t_shell *shell)
{
    // Sin argumentos -> exit con código actual
    if (!args[1])
        exit(shell->last_status.last_exit_code);
    
    // Con argumento -> validar y convertir
    if (!is_valid_number(args[1]))
        return (handle_invalid_exit_arg(args[1], shell));
    
    // Demasiados argumentos
    if (args[2])
        return (handle_too_many_args(shell));
}

// Conversión segura a número
static long safe_atol(char *str, int *overflow)
{
    // Convierte string a long con detección de overflow
}
```

**Características:**
- Validación estricta de argumentos numéricos
- Manejo de overflow
- Códigos de error apropiados

## 2. HEREDOC - Documentos Integrados

### 2.1 heredoc.c
**Sintaxis:** `command << DELIMITER`

**Lógica:**
```c
// Función principal que gestiona todos los heredocs de un comando
void heredoc(t_command *cmd)
{
    // Para cada heredoc en el comando
    for (int i = 0; i < cmd->heredoc.n_redirs; i++)
    {
        // 1. Crea un pipe para comunicación
        // 2. Fork para crear proceso hijo
        // 3. Hijo lee entrada hasta delimiter
        // 4. Padre espera y maneja señales
    }
}
```

### 2.2 heredoc_utils.c
**Funciones auxiliares para heredoc:**

```c
// Handler de señal específico para heredoc
static void _sigint_heredoc(int sig)
{
    // Maneja Ctrl+C durante heredoc
    // Cierra stdin y termina el proceso hijo
}

// Inicia proceso hijo para heredoc
pid_t start_heredoc_child(int pipe_fd[2], const char *delim)
{
    // 1. Fork
    // 2. En hijo: lee desde stdin hasta delimiter
    // 3. Escribe al pipe
    // 4. Sale cuando encuentra delimiter
}

// Espera al proceso hijo
static int wait_for_child(pid_t pid)
{
    // Espera terminación del hijo
    // Maneja códigos de salida
}
```

### 2.3 heredoc_utils_2.c
**Funciones de soporte adicionales:**

```c
// Maneja el estado del proceso hijo
static bool handle_child_status(t_command *cmd, int status, t_heredoc_ctx *ctx)
{
    // Analiza el código de salida del hijo
    // Determina si continuar o abortar
}

// Conecta el último heredoc a stdin
void attach_last_heredoc_to_stdin(int last_fd)
{
    // Redirige el descriptor del último heredoc a stdin
    // Para que el comando lo use como entrada
}
```

**Características del Heredoc:**
- **Aislamiento de señales:** Cada heredoc corre en proceso hijo
- **Múltiples heredocs:** Soporte para varios `<<` en un comando
- **Manejo de Ctrl+C:** Solo interrumpe el heredoc, no el shell
- **Delimitadores flexibles:** Cualquier string como delimitador

## 3. SIGNALS - Gestión de Señales

### 3.1 signal_handle.c
**Gestión completa de señales:**

```c
// Variable global (única permitida)
int g_signal_received;

// Handler para SIGINT en modo shell
void sigint_handler(int sig)
{
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();           // Notifica nueva línea a readline
    rl_replace_line("", 0);     // Limpia la línea actual
    rl_redisplay();             // Redibuja el prompt
    g_signal_received = 130;    // Código de salida por SIGINT
}

// Handler para procesos hijo
void sigint_handler_child(int sig)
{
    write(STDOUT_FILENO, "\n", 1);
    exit(130);  // Termina el proceso hijo
}

// Handler específico para heredoc
void sigint_handler_heredoc(int sig)
{
    write(STDOUT_FILENO, "\n", 1);
    g_signal_received = 130;
    close(STDIN_FILENO);  // Cierra stdin para terminar lectura
    exit(130);
}

// Configuración de señales según contexto
void set_signals(int mode)
{
    switch (mode)
    {
        case MODE_CHILD:
            signal(SIGINT, sigint_handler_child);
            signal(SIGQUIT, SIG_DFL);
            break;
        case MODE_SHELL:
            signal(SIGINT, sigint_handler);
            signal(SIGQUIT, SIG_IGN);
            break;
        case MODE_PIPE:
            signal(SIGINT, SIG_IGN);
            signal(SIGQUIT, SIG_IGN);
            break;
        case MODE_HEREDOC:
            signal(SIGINT, sigint_handler_heredoc);
            signal(SIGQUIT, SIG_IGN);
            break;
    }
}
```

**Características de Signals:**
- **Una sola variable global:** `g_signal_received`
- **Handlers específicos:** Diferente comportamiento según contexto
- **No acceso a estructuras:** Los handlers no tocan datos del shell
- **Manejo de readline:** Integración correcta con la librería readline

## 4. CAT CON PIPES - Implementación Especial

### 4.1 Detección de Patrón Cat
```c
// En exec.c
static int is_cat_pipeline_pattern(t_command_line *cmd_line)
{
    // Verifica si tenemos patrón: cat | cat | ... | comando
    // Todos los comandos excepto el último deben ser 'cat'
    // El último comando debe ser diferente de 'cat'
}
```

### 4.2 Ejecución Especial para Cat
```c
// En pipes.c
static bool is_interactive_command(const char *cmd_name)
{
    // Identifica comandos interactivos como 'cat'
    return (ft_strcmp((char*)cmd_name, "cat") == 0 || 
            ft_strcmp((char*)cmd_name, "/bin/cat") == 0);
}

// Configuración especial para cat en pipe
static int setup_interactive_pipe(t_command *cmd, t_shell *shell, int pipe_fd[2])
{
    // 1. Configura descriptores de archivo
    // 2. Maneja redirecciones si existen
    // 3. Prepara para entrada interactiva
}

// Procesamiento de entrada interactiva
static void process_interactive_input(int needed_newlines)
{
    char buffer[1024];
    ssize_t bytes_read;
    int consecutive_newlines = 0;
    
    while (1)
    {
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
        
        if (bytes_read == 1 && buffer[0] == '\n')
        {
            consecutive_newlines++;
            write(STDOUT_FILENO, buffer, bytes_read);
            
            // Termina cuando se alcanzan suficientes newlines consecutivos
            if (consecutive_newlines >= needed_newlines) 
                exit(0);
        } 
        else 
        {
            consecutive_newlines = 0;
            write(STDOUT_FILENO, buffer, bytes_read);
        }
    }
}
```

### 4.3 Contador de Cat
```c
// En exec.c
static void count_cat_commands(t_command_line *cmd_line, t_shell *shell)
{
    int cat_count = 0;
    
    // Cuenta cuántos comandos 'cat' hay en la tubería
    for (int i = 0; i < cmd_line->n_cmds; i++)
    {
        if (es_comando_cat(cmd_line->cmds[i]))
            cat_count++;
    }
    
    // Guarda el contador para usar en pipes
    shell->cat_count = cat_count;
}
```

### 4.4 Ejecución de Pipeline Cat
```c
// En exec.c
static void execute_cat_pipeline(t_command_line *cmd_line, t_shell *shell)
{
    // 1. Ejecuta el último comando primero (ej: ls, pwd)
    exec_last(&cmd_line->cmds[cmd_line->n_cmds - 1], shell);
    
    // 2. Luego procesa los comandos cat en secuencia
    for (int i = 0; i < cmd_line->n_cmds - 1; i++)
    {
        // Cada cat necesita el número correcto de enters para terminar
        if (i != cmd_line->n_cmds - 2)
            exec_pipe(&cmd_line->cmds[i], shell);
        else
            exec_last(&cmd_line->cmds[i], shell);
    }
}
```

**Características del Cat con Pipes:**
- **Detección automática:** Reconoce patrones `cat | cat | ... | comando`
- **Ejecución inversa:** Primero ejecuta el comando final, luego los cats
- **Enters proporcionales:** Número de enters = número de comandos cat
- **Comportamiento interactivo:** Cada cat procesa entrada hasta conseguir suficientes enters consecutivos

## 5. FLUJO GENERAL DEL PROYECTO

1. **Parseo:** Se analiza la línea de comandos
2. **Detección:** Se identifica si hay patrones especiales (cat pipes, heredoc)
3. **Configuración de señales:** Se establece el modo apropiado
4. **Ejecución:** Se ejecutan comandos según su tipo (builtin/externo)
5. **Limpieza:** Se restauran descriptores y se liberan recursos

Este diseño garantiza un comportamiento robusto y compatible con bash en todos los aspectos clave del proyecto.
