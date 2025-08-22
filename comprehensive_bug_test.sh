#!/bin/bash

# Test exhaustivo para identificar bugs en minishell
# Compara comportamiento con bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Función para ejecutar test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_behavior="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "\n${YELLOW}TEST $TOTAL_TESTS: $test_name${NC}"
    echo "Command: $command"
    echo "Expected: $expected_behavior"
    
    # Ejecutar en bash para comparación
    echo "=== BASH OUTPUT ==="
    timeout 5 bash -c "$command" 2>&1
    local bash_exit=$?
    echo "Bash exit code: $bash_exit"
    
    # Ejecutar en minishell
    echo "=== MINISHELL OUTPUT ==="
    echo "$command" | timeout 5 ./minishell 2>&1
    local mini_exit=$?
    echo "Minishell exit code: $mini_exit"
    
    echo -e "${GREEN}MANUAL VERIFICATION NEEDED${NC}"
    echo "----------------------------------------"
}

# Función para test interactivo
interactive_test() {
    local test_name="$1"
    local description="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "\n${YELLOW}INTERACTIVE TEST $TOTAL_TESTS: $test_name${NC}"
    echo "Description: $description"
    echo "Please test manually and press enter to continue..."
    read -r
}

echo "==================================="
echo "COMPREHENSIVE MINISHELL BUG TESTS"
echo "==================================="

# Compilar minishell
echo "Compiling minishell..."
make re > /dev/null 2>&1

# 1. BUILTINS CON REDIRECCIONES
echo -e "\n${GREEN}=== BUILTINS CON REDIRECCIONES ===${NC}"

run_test "echo con redirección" \
    "echo hello > test_output.txt && cat test_output.txt && rm -f test_output.txt" \
    "Debe crear archivo con 'hello'"

run_test "pwd con redirección" \
    "pwd > test_pwd.txt && cat test_pwd.txt && rm -f test_pwd.txt" \
    "Debe mostrar directorio actual"

run_test "env con redirección" \
    "env > test_env.txt && wc -l test_env.txt && rm -f test_env.txt" \
    "Debe redirigir variables de entorno"

# 2. REDIRECCIONES SIN ESPACIOS
echo -e "\n${GREEN}=== REDIRECCIONES SIN ESPACIOS ===${NC}"

run_test "Redirección sin espacios >" \
    "echo hello>test_no_space.txt && cat test_no_space.txt && rm -f test_no_space.txt" \
    "Debe funcionar igual que con espacios"

run_test "Redirección sin espacios <" \
    "echo test > input.txt && cat<input.txt && rm -f input.txt" \
    "Debe leer del archivo"

run_test "Append sin espacios" \
    "echo line1>test_append.txt && echo line2>>test_append.txt && cat test_append.txt && rm -f test_append.txt" \
    "Debe tener dos líneas"

# 3. CÓDIGO DE ERROR DE ^C
echo -e "\n${GREEN}=== CÓDIGO DE ERROR ^C ===${NC}"

interactive_test "Ctrl+C en prompt vacío" \
    "Ejecuta minishell, presiona Ctrl+C en prompt vacío, luego 'echo \$?' - debe ser 130"

# 4. PIPELINE NOT CLOSED CON ENTER
echo -e "\n${GREEN}=== PIPELINE NOT CLOSED ===${NC}"

interactive_test "Enter en prompt vacío" \
    "Ejecuta minishell, presiona Enter varias veces - no debe mostrar error"

# 5. MÚLTIPLES ERRORES DE REDIRECCIÓN
echo -e "\n${GREEN}=== ERRORES DE REDIRECCIÓN MÚLTIPLES ===${NC}"

run_test "Redirección <>" \
    "cat <>" \
    "Debe mostrar un solo error de sintaxis"

run_test "Redirección ><" \
    "cat ><" \
    "Debe mostrar un solo error de sintaxis"

run_test "Múltiples <" \
    "cat <<<" \
    "Debe mostrar error de sintaxis"

run_test "Múltiples >" \
    "cat >>>" \
    "Debe mostrar error de sintaxis"

# 6. PIPELINE ESPECÍFICO
echo -e "\n${GREEN}=== PIPELINE ESPECÍFICO ===${NC}"

run_test "cat | cat | ls" \
    "cat | cat | ls" \
    "Debe ejecutar ls normalmente"

# 7. ECHO CON MÚLTIPLES GUIONES
echo -e "\n${GREEN}=== ECHO CON GUIONES ===${NC}"

run_test "Echo con múltiples guiones" \
    "echo - - - - - hola" \
    "Debe mostrar: - - - - - hola"

run_test "Echo con -n y guiones" \
    "echo -n - - - hola" \
    "Debe mostrar sin newline: - - - hola"

# 8. EXIT CON PROBLEMAS
echo -e "\n${GREEN}=== EXIT PROBLEMS ===${NC}"

run_test "Exit con demasiados argumentos" \
    "echo 'exit 1 2' | ./minishell; echo 'Exit code:' \$?" \
    "Debe mostrar error pero NO salir del shell"

run_test "Exit con letras" \
    "echo 'exit hello' | ./minishell; echo 'Exit code:' \$?" \
    "Debe salir con código 2"

run_test "Exit con número negativo" \
    "echo 'exit -1' | ./minishell; echo 'Exit code:' \$?" \
    "Debe salir con código 255"

run_test "Exit con número grande" \
    "echo 'exit 300' | ./minishell; echo 'Exit code:' \$?" \
    "Debe salir con código 44 (300 % 256)"

# 9. EXPANSIÓN CON COMILLAS
echo -e "\n${GREEN}=== EXPANSIÓN CON COMILLAS ===${NC}"

run_test "Comillas simples con contenido después" \
    "echo \"'\"hola\"'\"" \
    "Debe mostrar: 'hola'"

run_test "Expansión en comillas simples" \
    "echo 'echo \$USER'" \
    "Debe mostrar literalmente: echo \$USER"

# 10. CÓDIGOS DE ERROR
echo -e "\n${GREEN}=== CÓDIGOS DE ERROR ===${NC}"

run_test "Comando inexistente" \
    "ddfghjk; echo \$?" \
    "Debe mostrar código 127"

run_test "Archivo inexistente" \
    "cat archivo_que_no_existe; echo \$?" \
    "Debe mostrar código 1"

# 11. EXPORT Y ENV
echo -e "\n${GREEN}=== EXPORT Y ENV ===${NC}"

run_test "Export con redirección" \
    "export a=\"echo hola > a\" && echo \$a" \
    "Debe mostrar: echo hola > a"

run_test "Export con pipe" \
    "export a=\"ls | wc\" && echo \$a" \
    "Debe mostrar: ls | wc"

run_test "Export y verificación con env" \
    "export TEST_VAR=hello && env | grep TEST_VAR" \
    "Debe mostrar: TEST_VAR=hello"

# 12. CASOS EDGE DE SINTAXIS
echo -e "\n${GREEN}=== CASOS EDGE DE SINTAXIS ===${NC}"

run_test "Pipe al final" \
    "cat |" \
    "Debe mostrar error de pipeline not closed"

run_test "Múltiples pipes" \
    "cat | | ls" \
    "Debe mostrar error de sintaxis"

run_test "Redirección sin archivo" \
    "cat >" \
    "Debe mostrar error de sintaxis"

# 13. MEMORY LEAKS TEST
echo -e "\n${GREEN}=== MEMORY LEAKS TEST ===${NC}"

interactive_test "Verificar leaks con valgrind" \
    "Ejecuta: valgrind --leak-check=full ./minishell y prueba algunos comandos, luego exit"

# Resumen
echo -e "\n${GREEN}==================================="
echo "RESUMEN DE TESTS"
echo "===================================${NC}"
echo "Total tests ejecutados: $TOTAL_TESTS"
echo -e "${YELLOW}Nota: Todos los tests requieren verificación manual${NC}"
echo -e "${YELLOW}Compara la salida de bash vs minishell para cada caso${NC}"

# Cleanup
rm -f test_*.txt input.txt

echo -e "\n${GREEN}Test completado. Revisa cada resultado manualmente.${NC}"
