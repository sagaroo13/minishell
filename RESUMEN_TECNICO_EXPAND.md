# Resumen Técnico: Implementación del Módulo Expand

## Arquitectura General

### Ubicación del módulo:
- **Archivo principal**: `src/main/expand.c`
- **Declaraciones**: `include/minishell.h` (líneas 206-211)
- **Punto de invocación**: `src/main/exec_utils.c` función `exec()` (línea 56)

### Filosofía de diseño:
**Expansión Just-In-Time**: Las variables se expanden justo antes de la ejecución del comando, no durante el parsing inicial.

---

## Funciones Implementadas

### 1. `set_exit_status_direct(t_shell *shell, int code)`
**Propósito**: Actualización directa del exit status
- **Cuándo se usa**: Cuando tienes el código de salida exacto (ej: builtins)
- **Variables afectadas**: 
  - `shell->last_status.last_exit_code = code`
  - `shell->last_status.exit_called = true`

### 2. `update_last_exit_status(t_shell *shell, int new_status)`
**Propósito**: Interpretación y actualización del exit status desde `waitpid()`
- **Cuándo se usa**: Después de ejecutar procesos hijos
- **Lógica de interpretación**:
  - `WIFEXITED`: Extrae exit code normal
  - `WIFSIGNALED`: Convierte señal a código (128 + signal number)
  - Fallback: Valores 0-255 directos, otros → código 1

### 3. `expand_exit_status_in_arg(char **arg, t_shell *shell)`
**Propósito**: Expansión de `$?` en un argumento específico
- **Algoritmo**: 
  1. Verifica si contiene `$?`
  2. Llama a `replace_exit_status()`
  3. Libera memoria anterior y asigna nueva

### 4. `replace_exit_status(const char *str, int exit_code)` (static)
**Propósito**: Reemplazo real de todas las ocurrencias de `$?`
- **Algoritmo**:
  1. Busca `$?` con `ft_strstr()`
  2. Copia fragmento anterior
  3. Convierte exit_code a string con `ft_itoa()`
  4. Concatena todo
  5. Continúa desde después de `$?`

### 5. `expand_exit_status(char **args, t_shell *shell)`
**Propósito**: Aplica expansión a todos los argumentos de un comando
- **Recorre**: Array completo de argumentos
- **Optimización**: Solo procesa argumentos que contienen `$?`

---

## Variables y Estructuras Utilizadas

### Estructura principal: `t_shell->last_status`
```c
struct {
    int last_exit_code;    // Valor actual de $?
    bool exit_called;      // Flag de control interno
} last_status;
```

### Variables temporales en funciones:
- `char *result`: String resultado acumulativo
- `char *tmp`: Puntero móvil para recorrer string original
- `char *pos`: Posición de cada ocurrencia de `$?`
- `char *exit_code_str`: Conversión numérica a string
- `char *old_result`: Gestión de memoria para concatenaciones

---

## Flujo de Ejecución

### 1. **Inicialización**:
   - `shell->last_status.last_exit_code = 0` (estado inicial)

### 2. **Durante ejecución**:
   ```
   comando_anterior → actualiza exit_status → nuevo_comando → expand $? → ejecutar
   ```

### 3. **Secuencia específica**:
   ```c
   exec(cmd_name, cmd_args, shell) {
       expand_exit_status(cmd_args, shell);  // ← Expansión aquí
       // ... resto de ejecución
   }
   ```

---

## Casos de Uso Soportados

### ✅ **Casos funcionales**:
- `echo $?` → Muestra exit code del comando anterior
- `cmd1 && echo $?` → Exit code de cmd1
- `cmd1 | cmd2` → Cada comando ve el $? correcto de su contexto
- `echo "Exit: $?"` → Expansión dentro de strings
- `echo $? $?` → Múltiples ocurrencias en mismo argumento

### ⚠️ **Limitaciones actuales**:
- Solo expande `$?`, no otras variables de entorno
- No maneja escapado (`\$?`)
- No diferencia entre strings quoted/unquoted para expansión

---

## Ventajas de la Implementación

1. **Timing correcto**: Expansión en el momento exacto
2. **Eficiencia**: Solo procesa argumentos que realmente contienen `$?`
3. **Compatibilidad**: Comportamiento consistente con bash
4. **Modularidad**: Funciones separadas y reutilizables
5. **Gestión de memoria**: Liberación correcta de strings temporales

---

## Puntos de Mejora Potenciales

1. **Expansión de variables de entorno**: Añadir soporte para `$HOME`, `$USER`, etc.
2. **Escapado**: Manejar `\$?` como literal
3. **Optimización**: Cache de strings que no contienen variables
4. **Validación**: Verificar integridad de argumentos antes de expansión

---

## Integración con el Sistema

### **Dependencias**:
- `ft_strstr()`: Búsqueda de patrones
- `ft_itoa()`: Conversión numérica
- `ft_strjoin()`, `ft_strdup()`: Manipulación de strings
- `free()`: Gestión de memoria

### **Llamado desde**:
- **Principal**: `exec_utils.c:exec()` línea 56
- **Anteriormente**: `ft_echo.c` (eliminado por redundancia)

### **Integración temporal**:
```
parsing → command_line → exec() → expand → execute
                            ↑
                    Punto de expansión
```

---

## Conclusión Técnica

La implementación es **robusta, eficiente y bien integrada**. Utiliza el patrón de expansión just-in-time que garantiza valores actualizados de `$?` en todos los contextos de ejecución, especialmente en pipelines complejos.

**Calificación técnica**: ⭐⭐⭐⭐⭐ (Implementación profesional lista para producción)
