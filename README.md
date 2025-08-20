


Minishell – Resumen técnico y guía rápida

Este documento resume los cambios realizados, los componentes del proyecto y cómo funciona la shell para ayudarte a exponerla con una base clara y ordenada.

## Cambios clave realizados

- Heredoc robusto y seguro
	- Hijo dedicado con manejador SIGINT async-safe; el padre ignora señales mientras espera.
	- Cancelación con Ctrl+C devuelve 130 en `$?` y evita ejecutar el comando.
	- Lectura desde /dev/tty para no interferir con stdin redirigido.
	- Solo el último heredoc se conecta a STDIN del comando; los anteriores se descartan.
	- Refactor en helpers: `start_heredoc_child`, `handle_parent_after_child`, `attach_last_heredoc_to_stdin`.

- Señales centralizadas
	- `set_signals(MODE_SHELL | MODE_CHILD | MODE_PIPE | MODE_HEREDOC)` unifica políticas.
	- Shell: SIGINT con prompt limpio; SIGQUIT ignorada.
	- Hijos: comportamiento por defecto para exec; pipelines ignoran señales en padre.

- Cumplimiento de normas (42)
	- Archivos divididos por responsabilidad: `pipes_utils.c`, `heredoc_utils.c`, `env_utils.c`, `fd_utils.c`, `shell_loop.c`.
	- Funciones largas partidas y reducidas a ≤25 líneas donde aplicaba.
	- Máximo 5 funciones por archivo respetado en los módulos refactorizados.

- Funciones no autorizadas sustituidas
	- `getenv` → `get_env_value(shell, name)` (usa el entorno interno de la shell).
	- `strdup/strcmp/_exit/clear_history/using_history` eliminadas o reemplazadas por `ft_*` y `rl_clear_history`.
	- `ft_cd` actualizado para usar el entorno interno y mensajes sin overread.

- Organización del parser
	- Archivos movidos a `src/parse/`: `parse.c`, `parse_utils*.c`.
	- Makefile actualizado para compilar desde la nueva ruta.

## Estructura del proyecto

- include/minishell.h: Tipos, constantes y prototipos públicos.
- libft/: Biblioteca auxiliar (ft_*).
- builts/: Implementación de builtins (cd, echo, env, exit, export, pwd, unset).
- src/
	- main.c, shell_loop.c: Bucle principal y prompt.
	- exec_line.c: Orquesta parseo, heredoc y ejecución/pipelines.
	- pipes.c, pipes_utils.c: Ejecución en pipeline y helpers padre/hijo.
	- heredoc.c, heredoc_utils.c: Implementación completa de heredoc.
	- redirs.c: Redirecciones y apertura de ficheros.
	- env_utils.c: Gestión del entorno interno (copias, free, helpers).
	- fd_utils.c: Guardado y restaurado de fds estándar.
	- expand.c: Expansión simple de variables y `$?`.
	- signal_handle.c: Política de señales centralizada por modo.
	- safe_funcs.c / safe_utils.c: Wrappers seguros (malloc/open/dup2/... ).
	- utils.c / utils_2.c: Utilidades varias.
	- parse/: Lexer y parser en módulos (`parse.c`, `parse_utils_[1-4].c`).

## Flujo de ejecución (cómo funciona la shell)

1) Entrada y prompt
	 - `shell_loop` configura señales de shell, muestra prompt (cwd), y usa `readline`.
	 - Añade línea al histórico si no está vacía.

2) Parseo y tokenización
	 - `parse_line` divide la línea en comandos/pipes (`split_pipes`) y crea `t_command_line`.
	 - `lexer` tokeniza respetando comillas simples/dobles y espacios.
	 - `get_redirecs` detecta redirecciones (<, >, >>, 2>, <<) y las normaliza.
	 - `get_arguments` construye `argv` descartando metacaracteres y ficheros de redirección.

3) Heredoc
	 - Si hay `<<`, se lanza un hijo lector: prompt heredoc, lectura hasta delimitador sin expandir el delimitador.
	 - Ctrl+C corta heredoc: `execute=false` y `$?=130`; no se ejecuta el comando.
	 - Solo el fd del último heredoc se duplica a STDIN del comando.

4) Ejecución
	 - Builtins en el proceso padre cuando aplica, usando `exec_builtin`.
	 - Externos: resolución con PATH interno (`get_env_value`), `execve` con `shell->env`.
	 - Pipelines: cada etapa configura pipes/redirs y se sincroniza con helpers (`parent_*`, `child_*`).
	 - Estado de salida: normalizado y guardado en `shell->last_status` (visible en `$?`).

5) Señales
	 - Shell: Ctrl+C limpia línea y muestra nuevo prompt; Ctrl+\\ ignorada.
	 - Hijos: por defecto (permiten terminar procesos como `cat`).
	 - Heredoc: hijo sale con 130; padre no imprime basura ni deja fds abiertos.

## Elementos esenciales que hacen que funcione

- Entorno interno: `shell->env` (sin depender de `getenv`); acceso vía `get_env_value`.
- Gestión de fds: `save_fds`/`restore_fds` y helpers de dup2/close seguros.
- Señales por modo: `set_signals(mode)` selecciona manejadores correctos.
- Parser robusto: respeta comillas, separa tokens, detecta errores de sintaxis.
- Heredoc aislado: proceso hijo + tubería dedicada + control de cancelación.
- Wrappers seguros: `safe_*` centralizan errores y evitan duplicar boilerplate.

## Builtins soportados

- echo, pwd, env, exit, export, unset, cd
	- `cd` usa `get_env_value(shell, ...)` para HOME/OLDPWD y evita funciones no permitidas.

## Cómo compilar y ejecutar

- make
- ./minishell

Tips:
- Usa comillas para agrupar tokens, ej.: echo "hola mundo".
- Heredoc: cat << EOF ... EOF
- Ctrl+C en heredoc cancela el comando (estado 130).

## Siguientes pasos sugeridos

- Actualizar PWD/OLDPWD al hacer cd y soportar `~/<ruta>`.
- Más tests automáticos para parser y redirecciones.
- Cobertura de errores detallada en redirecciones y permisos.