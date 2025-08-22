# DOCUMENTACIÓN COMPLETA DE BUILT-INS - MINISHELL

## 📋 IMPORTANCIA DE LOS BUILT-INS

Los built-ins son comandos **internos del shell** que se ejecutan directamente en el proceso del shell sin crear un proceso hijo. Esto es fundamental porque:

1. **Eficiencia**: No requieren `fork()` + `execve()`
2. **Modificación del entorno**: Pueden cambiar variables del shell padre
3. **Control de flujo**: Pueden afectar el estado interno del shell
4. **Compatibilidad**: Bash requiere que ciertos comandos sean built-ins

---

## 🔧 BUILT-INS IMPLEMENTADOS

### 1. **ECHO** 
```bash
Sintaxis: echo [-n] [string ...]
```

**Función**: Imprime argumentos separados por espacios, seguido de newline.

**Lógica implementada**:
- Sin argumentos: imprime solo newline
- Con `-n`: suprime el newline final  
- Múltiples `-n` consecutivos se interpretan como uno solo
- `--` termina el procesamiento de opciones
- Exit code: Siempre 0 (éxito)

**Casos especiales**:
```bash
echo hello world          # "hello world\n"
echo -n hello             # "hello" (sin newline)
echo -n -n -n hello       # "hello" (múltiples -n = uno)
echo -- -n hello          # "-- -n hello\n" (literal)
```

---

### 2. **PWD**
```bash
Sintaxis: pwd
```

**Función**: Imprime el directorio de trabajo actual.

**Lógica implementada**:
- Usa `getcwd()` para obtener ruta absoluta
- No acepta argumentos ni opciones
- Funciona con redirecciones
- Exit code: 0 (éxito) o 1 (error de sistema)

**Casos especiales**:
```bash
pwd                        # /current/path
pwd > file.txt            # guarda path en archivo
```

---

### 3. **CD**
```bash
Sintaxis: cd [directory]
```

**Función**: Cambia el directorio de trabajo.

**Lógica implementada**:
- Sin argumentos: va a `$HOME`
- `cd -`: va a `$OLDPWD` y muestra nueva ubicación
- `cd ~`: va a `$HOME`
- Actualiza variables `PWD` y `OLDPWD`
- Exit code: 0 (éxito) o 1 (error - directorio no existe)

**Casos especiales**:
```bash
cd                         # va a HOME
cd -                       # va a directorio anterior
cd ~                       # va a HOME
cd /nonexistent           # error: exit code 1
```

---

### 4. **ENV**
```bash
Sintaxis: env
```

**Función**: Muestra todas las variables de entorno exportadas.

**Lógica implementada**:
- Imprime todas las variables en formato `NAME=value`
- Una variable por línea
- Solo variables exportadas (no locales)
- Exit code: Siempre 0

**Casos especiales**:
```bash
env                        # lista todas las variables
env > env.txt             # guarda en archivo
```

---

### 5. **EXPORT**
```bash
Sintaxis: export [name[=value] ...]
```

**Función**: Exporta variables al entorno o las muestra.

**Lógica implementada**:
- Sin argumentos: muestra variables exportadas (formato `declare -x`)
- Con argumentos: exporta variables
- Validación de identificadores: debe empezar con letra/_, seguido de letras/números/_
- Con `=`: asigna valor; sin `=`: solo marca como exportada
- Exit code: 0 (éxito) o 1 (identificador inválido)

**Casos especiales**:
```bash
export                     # muestra exportadas
export VAR=value          # exporta con valor
export VAR                # marca existente como exportada
export 123BAD=value       # error: identificador inválido
```

---

### 6. **UNSET**
```bash
Sintaxis: unset name ...
```

**Función**: Elimina variables del entorno.

**Lógica implementada**:
- Elimina completamente la variable del entorno
- Acepta múltiples argumentos
- Variables inexistentes no generan error
- Exit code: Siempre 0

**Casos especiales**:
```bash
unset VAR                  # elimina VAR
unset VAR1 VAR2           # elimina múltiples
unset NONEXISTENT         # no genera error
```

---

### 7. **EXIT**
```bash
Sintaxis: exit [n]
```

**Función**: Termina el shell con código de salida.

**Lógica implementada**:
- Sin argumentos: usa último exit code
- Con número: usa ese código (módulo 256)
- Números negativos: se convierten (ej: -5 → 251)
- Argumentos no numéricos: error, sale con código 2
- Demasiados argumentos: muestra error, NO sale, exit code 1
- Overflow: trata como no numérico

**Casos especiales**:
```bash
exit                       # sale con último código
exit 42                    # sale con código 42
exit -5                    # sale con código 251
exit hello                 # error: exit code 2
exit 1 2                   # error: no sale, continúa shell
exit 256                   # sale con código 0 (256%256)
```

---

## 🎯 CÓDIGOS DE SALIDA (EXIT CODES)

### Built-ins:
- **0**: Éxito
- **1**: Error general (cd falla, export inválido, etc.)
- **2**: Error de sintaxis o argumento inválido (solo exit)

### Comandos externos:
- **0**: Éxito
- **1-125**: Error específico del comando
- **126**: Comando no ejecutable
- **127**: Comando no encontrado
- **128+n**: Terminado por señal n

---

## 🔄 FLUJO DE EJECUCIÓN

1. **Detección**: `is_builtin()` verifica si es comando interno
2. **Ejecución**: `exec_builtin()` llama función específica
3. **Exit code**: Se establece con `set_exit_status_direct()`
4. **Propagación**: Exit code se mantiene hasta el final del shell
5. **Finalización**: `main()` retorna el último exit code

---

## 🐛 LIMITACIONES CONOCIDAS

### Operadores lógicos NO soportados:
- `&&` (AND lógico)
- `||` (OR lógico)  
- `;` (separador de comandos)

**Razón**: Responsabilidad del parser/lexer, no de built-ins.

### Redirecciones:
- ✅ Soportadas: `>`, `>>`, `<`, `2>`
- ❌ No soportadas: `&>`, `|&`, etc.

---

## 📊 COMPATIBILIDAD CON BASH

### ✅ Funcionalidad completa:
- Echo con `-n`
- Pwd básico  
- Export/unset completos
- Exit con aritmética módulo 256
- Exit codes correctos

### 🟡 Funcionalidad parcial:
- CD (falta `cd ~user`)
- Heredocs (implementados pero con bugs)

### ❌ No implementado:
- Operadores lógicos
- Expansión completa de variables
- Alias y funciones

---

## 🔧 DEBUGGING

Para depurar exit codes:
```bash
echo $?                    # muestra último exit code
echo 'comando' | ./minishell; echo $?
```

Para depurar variables:
```bash
export VAR=test
echo $VAR                  # verifica valor
env | grep VAR            # verifica si está exportada
```

---

**Conclusión**: Los built-ins están implementados con alta fidelidad a bash en cuanto a funcionalidad básica y manejo de exit codes. Las limitaciones principales están en el parser, no en la lógica de built-ins.
