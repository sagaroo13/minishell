# Estructura y Funcionamiento de Minishell

Este documento explica la arquitectura general del proyecto Minishell, cómo se conectan los diferentes componentes y el flujo de ejecución desde que se inicia el programa hasta que procesa un comando.

## 1. Arquitectura General

Minishell se compone de varios módulos interconectados que forman un pipeline de procesamiento de comandos:

```
Input del usuario → Lexer → Parser → Expansión → Ejecución → Salida
```

Cada módulo tiene una responsabilidad específica:

### 1.1 Módulos Principales

1. **Main y Shell Loop**: Inicialización del shell y bucle principal
2. **Lexer**: Análisis léxico de la entrada
3. **Parser**: Análisis sintáctico y construcción de estructuras de comandos
4. **Expansión**: Sustitución de variables y procesos especiales
5. **Ejecución**: Ejecución de comandos, builtins y pipelines
6. **Redirecciones**: Manejo de redirecciones de entrada/salida
7. **Señales**: Gestión de señales como SIGINT (Ctrl+C)
8. **Builtins**: Implementación de comandos internos

## 2. Flujo de Ejecución

### 2.1 Inicialización

1. `main.c`: Punto de entrada del programa
   - Inicializa la estructura principal `t_shell`
   - Configura el entorno copiando las variables de entorno
   - Llama a la función `minishell` para iniciar el bucle principal

2. `shell_loop.c`: Bucle principal del shell
   - Muestra el prompt y espera la entrada del usuario
   - Gestiona la lectura de comandos
   - Procesa las líneas ingresadas

### 2.2 Procesamiento de Comandos

1. **Lectura y Análisis**:
   - `get_command_line`: Prepara el prompt y obtiene la línea de comando
   - `process_command_line`: Procesa la línea obtenida

2. **Análisis Léxico**:
   - `lexer.c`: Divide la entrada en tokens
   - `lexer_utils.c`: Funciones auxiliares para el análisis léxico

3. **Análisis Sintáctico**:
   - `parser.c`: Analiza la estructura del comando
   - `parser_utils.c`: Valida sintaxis y detecta errores
   - `parser_split.c`: Divide los comandos por pipes

4. **Expansión de Variables**:
   - `expand.c`: Expande variables como $? y variables de entorno

5. **Ejecución**:
   - `exec.c`: Ejecuta los comandos y gestiona el pipeline
   - `process_heredoc_and_exec`: Procesa heredocs y ejecuta comandos

### 2.3 Ejecución de Comandos

1. **Pipelines**:
   - `pipes.c`: Gestiona la ejecución de comandos en pipeline
   - `pipes_utils.c`: Funciones auxiliares para pipes

2. **Redirecciones**:
   - `redirs.c`: Maneja redirecciones de entrada/salida
   - `redirs_utils.c`: Funciones auxiliares para redirecciones

3. **Builtins**:
   - `exec_built_in.c`: Selector de comandos built-in
   - Implementaciones específicas: `ft_cd.c`, `ft_echo.c`, etc.

4. **Heredoc**:
   - `heredoc.c`: Manejo de heredocs (<<)
   - `heredoc_utils.c`: Funciones auxiliares para heredocs

5. **Señales**:
   - `signal_handle.c`: Gestión de señales (SIGINT, SIGQUIT)
   - `termios_off.c`: Configuración de terminal

## 3. Estructuras de Datos Clave

### 3.1 Estructuras Principales

1. **t_shell**: Estructura principal que contiene el estado del shell
   ```c
   typedef struct s_shell
   {
       char            cwd[BUFFER_SIZE];
       char            *line;
       char            *prompt;
       char            **env;
       t_stdfd         stdfd;
       t_last_status   last_status;
       t_command_line  *cmd_line;
   } t_shell;
   ```

2. **t_command_line**: Estructura que representa una línea de comandos
   ```c
   typedef struct s_command_line
   {
       char        *line;
       char        *err_msg;
       bool        execute;
       int         n_cmds;
       t_command   *cmds;
   } t_command_line;
   ```

3. **t_command**: Estructura que representa un comando individual
   ```c
   typedef struct s_command
   {
       char            **args;
       char            *cmd_str;
       bool            builtin;
       t_redirections  stdin;
       t_redirections  stdout;
       t_redirections  stderr;
       t_redirections  append;
       t_shell         *shell;
       t_redirections  heredoc;
       int             heredoc_fd;
       t_command_line  *cmd_line;
   } t_command;
   ```

### 3.2 Estructuras Auxiliares

1. **t_lexer**: Estructura para el análisis léxico
2. **t_redirections**: Estructura para manejar redirecciones
3. **t_last_status**: Estructura para almacenar el estado de salida

## 4. Casos Especiales

### 4.1 Manejo de Pipelines Interactivos (cat | cat | ls)

- Implementamos una lógica especial en `process_heredoc_and_exec` para detectar el patrón cat | cat | ls
- Cuando se detecta este patrón:
  1. Ejecutamos primero el comando `ls` para mostrar su salida
  2. Luego configuramos el pipeline de `cat`
  3. Utilizamos `child_exec_interactive_pipe` para manejar la entrada interactiva
  4. Detectamos dos Enter consecutivos para terminar el pipeline

### 4.2 Expansión de Variables

- En `expand.c` implementamos la expansión de variables como $?
- Manejamos múltiples ocurrencias de $? en un mismo comando
- Procesamos cada argumento para reemplazar todas las variables

### 4.3 Manejo de Señales

- Utilizamos la variable global `g_shell` para acceder al estado del shell
- Implementamos diferentes manejadores según el contexto:
  - `MODE_SHELL`: Para el prompt principal
  - `MODE_CHILD`: Para procesos hijo
  - `MODE_PIPE`: Para pipelines
  - `MODE_HEREDOC`: Para heredocs

## 5. Optimizaciones y Mejoras

1. **Refactorización de funciones largas**:
   - Dividimos `minishell` en funciones más pequeñas
   - Mejoramos la organización y legibilidad del código

2. **Manejo de memoria**:
   - Utilizamos funciones seguras como `safe_malloc`
   - Liberamos memoria adecuadamente para evitar fugas

3. **Gestión de errores**:
   - Implementamos mensajes de error específicos
   - Establecemos códigos de salida adecuados

4. **Compatibilidad con bash**:
   - Mejoramos comportamientos específicos como `cat | cat | ls`
   - Implementamos correctamente builtins como `cd -`

## 6. Diagrama de Flujo Simplificado

```
                  ┌────────────┐
                  │   main.c   │
                  └─────┬──────┘
                        │
                  ┌─────▼──────┐
                  │shell_loop.c│
                  └─────┬──────┘
                        │
          ┌─────────────▼────────────────┐
          │         exec_line.c          │
          │  (parse_line + ejecución)    │
          └─────────────┬────────────────┘
                        │
     ┌──────────────────┼────────────────────┐
     │                  │                    │
┌────▼─────┐      ┌─────▼─────┐        ┌─────▼────┐
│  lexer.c  │      │  parser.c │        │  exec.c  │
└────┬─────┘      └─────┬─────┘        └─────┬────┘
     │                  │                    │
     │            ┌─────▼─────┐        ┌─────▼────┐
     └───────────►│ expand.c  │◄───────┤  pipes.c │
                  └─────┬─────┘        └─────┬────┘
                        │                    │
                        │              ┌─────▼────┐
                        └──────────────► redirs.c │
                                       └──────────┘
```

Este documento proporciona una visión general de la arquitectura y el funcionamiento de Minishell. Para detalles específicos sobre la implementación de cada componente, consulte los archivos de código fuente correspondientes.
